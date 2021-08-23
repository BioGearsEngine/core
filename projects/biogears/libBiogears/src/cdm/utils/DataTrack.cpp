/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/utils/DataTrack.h>

//Standard Includes
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
//Project Includes
#include <biogears/cdm/circuit/electrical/SEElectricalCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/properties/SEDecimalFormat.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/io/io-manager.h>

namespace biogears {

DataTrack::DataTrack(Logger* logger)
  : Loggable(logger)
{
  m_Delimiter = ',';
}

DataTrack::DataTrack(DataTrack&& obj)
  : m_Track(std::move(obj.m_Track))
  , m_Probe(std::move(obj.m_Probe))
  , m_Formatting(std::move(obj.m_Formatting))
  , m_Delimiter(std::move(obj.m_Delimiter))
  , m_LastTime(std::move(obj.m_LastTime))
  , m_Time(std::move(obj.m_Time))
  , m_HeadingOrder(std::move(obj.m_HeadingOrder))
  , m_DefaultPrecision(std::move(obj.m_DefaultPrecision))
#ifndef ANDROID
  , m_FileStream(std::move(obj.m_FileStream))
#endif
{
}

DataTrack& DataTrack::operator=(DataTrack&& rhs)
{
  m_Track = std::move(rhs.m_Track);
  m_Probe = std::move(rhs.m_Probe);
  m_Formatting = std::move(rhs.m_Formatting);

  m_Delimiter = std::move(rhs.m_Delimiter);
  m_LastTime = std::move(rhs.m_LastTime);
  m_Time = std::move(rhs.m_Time);
  m_HeadingOrder = std::move(rhs.m_HeadingOrder);
  m_DefaultPrecision = std::move(rhs.m_DefaultPrecision);
#ifndef ANDROID
  m_FileStream = std::move(rhs.m_FileStream);
#endif

  return *this;
}

DataTrack::~DataTrack()
{
  Clear();
}

void DataTrack::Clear()
{
  Reset();
  if (m_FileStream.is_open())
    m_FileStream.close();
  m_HeadingOrder.clear();
}

void DataTrack::Reset()
{
  // Clear out our data
  m_Time.clear();
  for (TrackMapItr itr = m_Track.begin(); itr != m_Track.end(); ++itr) {
    delete itr->second;
  }
  m_Track.clear();
  m_Probe.clear();
  m_Formatting.clear();
}

std::vector<std::string>& DataTrack::GetHeadings()
{
  return m_HeadingOrder;
}

void DataTrack::SetFormatting(const std::string& name, const SEDecimalFormat& f)
{
  if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), name) == m_HeadingOrder.end())
    m_HeadingOrder.push_back(name);
  m_Formatting[name].Set(f);
}
void DataTrack::SetFormatting(const std::string& name, std::streamsize precision)
{
  if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), name) == m_HeadingOrder.end())
    m_HeadingOrder.push_back(name);
  m_Formatting[name].SetPrecision(precision);
}
void DataTrack::SetDefaultFormatting(std::streamsize precision)
{
  m_DefaultPrecision = precision;
}

void DataTrack::Probe(const std::string& name, double value, int i)
{
  std::stringstream ss;
  ss << name << i;
  std::string count = ss.str();
  if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), count) == m_HeadingOrder.end()) {
    m_HeadingOrder.push_back(count);
  }
  m_Probe[count] = value;
}

void DataTrack::Probe(const std::string& name, double value)
{
  if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), name) == m_HeadingOrder.end()) {
    m_HeadingOrder.push_back(name);
  }
  m_Probe[name] = value;
}

void DataTrack::Probe(const ::std::string& name, std::vector<double>& values)
{
  std::stringstream ss;
  std::string header;
  int valCount = 1;
  for (auto val : values) {
    ss.str("");
    ss << name << " - "  << valCount;
    header = ss.str();
    if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), header) == m_HeadingOrder.end()) {
      m_HeadingOrder.push_back(header);
    }
    m_Probe[header] = val;
    header.clear();
    ++valCount;
  }
}

void DataTrack::Probe(const SEFluidCircuit& c)
{
  for (SEFluidCircuitNode* n : c.GetNodes()) {
    if (n->HasPressure()) {
      auto unit = n->GetPressure().GetUnit();
      Probe(std::string { n->GetName() } + "_Pressure_" + unit->GetString(), n->GetPressure().GetValue(*unit));
    }
    if (n->HasVolume()) {
      auto unit = n->GetVolume().GetUnit();
      Probe(std::string { n->GetName() } + "_Volume_" + unit->GetString(), n->GetVolume().GetValue(*unit));
    }
  }
  for (SEFluidCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Probe(std::string { p->GetName() } + "_Switch", p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Probe(std::string { p->GetName() } + "_Valve", p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Probe(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), p->GetResistance().GetValue(*unit));
    }
    if (p->HasCompliance()) {
      auto unit = p->GetCompliance().GetUnit();
      Probe(std::string { p->GetName() } + "_Compliance_" + unit->GetString(), p->GetCompliance().GetValue(*unit));
    }
    if (p->HasInertance()) {
      auto unit = p->GetInertance().GetUnit();
      Probe(std::string { p->GetName() } + "_Inertance_" + unit->GetString(), p->GetInertance().GetValue(*unit));
    }
    if (p->HasFlow()) {
      auto unit = p->GetFlow().GetUnit();
      Probe(std::string { p->GetName() } + "_Flow_" + unit->GetString(), p->GetFlow().GetValue(*unit));
    }
    if (p->HasPressureSource()) {
      auto unit = p->GetPressureSource().GetUnit();
      Probe(std::string { p->GetName() } + "_PressureSource_" + unit->GetString(), p->GetPressureSource().GetValue(*unit));
    }
    if (p->HasFlowSource()) {
      auto unit = p->GetFlowSource().GetUnit();
      Probe(std::string { p->GetName() } + "_FlowSource_" + unit->GetString(), p->GetFlowSource().GetValue(*unit));
    }
  }
}

void DataTrack::Probe(const SEThermalCircuit& c)
{
  for (SEThermalCircuitNode* n : c.GetNodes()) {
    if (n->HasTemperature()) {
      auto unit = n->GetTemperature().GetUnit();
      Probe(std::string { n->GetName() } + "_Temperature_" + unit->GetString(), n->GetTemperature().GetValue(*unit));
    }
    if (n->HasHeat()) {
      auto unit = n->GetHeat().GetUnit();
      Probe(std::string { n->GetName() } + "_Heat_" + unit->GetString(), n->GetHeat().GetValue(*unit));
    }
  }
  for (SEThermalCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Probe(std::string { p->GetName() } + "_Switch", p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Probe(std::string { p->GetName() } + "_Valve", p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Probe(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), p->GetResistance().GetValue(*unit));
    }
    if (p->HasCapacitance()) {
      auto unit = p->GetCapacitance().GetUnit();
      Probe(std::string { p->GetName() } + "_Capacitance_" + unit->GetString(), p->GetCapacitance().GetValue(*unit));
    }
    if (p->HasInductance()) {
      auto unit = p->GetInductance().GetUnit();
      Probe(std::string { p->GetName() } + "_Inductance_" + unit->GetString(), p->GetInductance().GetValue(*unit));
    }
    if (p->HasHeatTransferRate()) {
      auto unit = p->GetHeatTransferRate().GetUnit();
      Probe(std::string { p->GetName() } + "_HeatTransferRate_" + unit->GetString(), p->GetHeatTransferRate().GetValue(*unit));
    }
    if (p->HasTemperatureSource()) {
      auto unit = p->GetTemperatureSource().GetUnit();
      Probe(std::string { p->GetName() } + "_TemperatureSource_" + unit->GetString(), p->GetTemperatureSource().GetValue(*unit));
    }
    if (p->HasHeatSource()) {
      auto unit = p->GetHeatSource().GetUnit();
      Probe(std::string { p->GetName() } + "_HeatSource_" + unit->GetString(), p->GetHeatSource().GetValue(*unit));
    }
  }
}

void DataTrack::Probe(const SEElectricalCircuit& c)
{
  for (SEElectricalCircuitNode* n : c.GetNodes()) {
    if (n->HasVoltage()) {
      auto unit = n->GetVoltage().GetUnit();
      Probe(std::string { n->GetName() } + "_Voltage_" + unit->GetString(), n->GetVoltage().GetValue(*unit));
    }
    if (n->HasCharge()) {
      auto unit = n->GetCharge().GetUnit();
      Probe(std::string { n->GetName() } + "_Charge_" + unit->GetString(), n->GetCharge().GetValue(*unit));
    }
  }
  for (SEElectricalCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Probe(std::string { p->GetName() } + "_Switch", p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Probe(std::string { p->GetName() } + "_Valve", p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Probe(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), p->GetResistance().GetValue(*unit));
    }
    if (p->HasCapacitance()) {
      auto unit = p->GetCapacitance().GetUnit();
      Probe(std::string { p->GetName() } + "_Capacitance_" + unit->GetString(), p->GetCapacitance().GetValue(*unit));
    }
    if (p->HasInductance()) {
      auto unit = p->GetInductance().GetUnit();
      Probe(std::string { p->GetName() } + "_Inductance_" + unit->GetString(), p->GetInductance().GetValue(*unit));
    }
    if (p->HasCurrent()) {
      auto unit = p->GetCurrent().GetUnit();
      Probe(std::string { p->GetName() } + "_Current_" + unit->GetString(), p->GetCurrent().GetValue(*unit));
    }
    if (p->HasVoltageSource()) {
      auto unit = p->GetVoltageSource().GetUnit();
      Probe(std::string { p->GetName() } + "_VoltageSource_" + unit->GetString(), p->GetVoltageSource().GetValue(*unit));
    }
    if (p->HasCurrentSource()) {
      auto unit = p->GetCurrentSource().GetUnit();
      Probe(std::string { p->GetName() } + "_CurrentSource_" + unit->GetString(), p->GetCurrentSource().GetValue(*unit));
    }
  }
}

void DataTrack::Probe(const SELiquidCompartmentGraph& graph)
{
  for (SELiquidCompartment* cmpt : graph.GetCompartments()) {
    if (cmpt->HasPressure()) {
      auto unit = cmpt->GetPressure().GetUnit();
      Probe(std::string { cmpt->GetName() } + "_Pressure_" + unit->GetString(), cmpt->GetPressure().GetValue(*unit));
    }
    if (cmpt->HasVolume()) {
      auto unit = cmpt->GetVolume().GetUnit();
      Probe(std::string { cmpt->GetName() } + "_Volume_" + unit->GetString(), cmpt->GetVolume().GetValue(*unit));
    }
    if (cmpt->HasPH()) {
      Probe(std::string { cmpt->GetName() } + "_pH_", cmpt->GetPH().GetValue());
    }

    for (SELiquidSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
      if (subQ->HasSaturation()) {
        Probe(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Saturation", subQ->GetSaturation().GetValue());
      }
      if (subQ->HasPartialPressure()) {
        auto unit = subQ->GetPartialPressure().GetUnit();
        Probe(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_PartialPressure_" + unit->GetString(), subQ->GetPartialPressure().GetValue(*unit));
      }
      if (subQ->HasMass()) {
        auto unit = subQ->GetMass().GetUnit();
        Probe(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Mass_" + unit->GetString(), subQ->GetMass().GetValue(*unit));
      }
      if (subQ->HasConcentration()) {
        auto unit = subQ->GetConcentration().GetUnit();
        Probe(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Concentration_" + unit->GetString(), subQ->GetConcentration().GetValue(*unit));
      }
      if (subQ->HasMolarity()) {
        auto unit = subQ->GetMolarity().GetUnit();
        Probe(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Molarity_" + unit->GetString(), subQ->GetMolarity().GetValue(*unit));
      }
    }
  }
  for (SELiquidCompartmentLink* link : graph.GetLinks()) {
    if (link->HasFlow()) {
      auto unit = link->GetFlow().GetUnit();
      Probe(std::string { link->GetName() } + "_Flow_" + unit->GetString(), link->GetFlow(*unit));
    }
  }
}

double DataTrack::GetProbe(const std::string& name)
{
  if (m_Probe.find(name) == m_Probe.end())
    return std::numeric_limits<double>::quiet_NaN();
  return m_Probe[name];
}

ProbeMap* DataTrack::GetProbes()
{
  return &m_Probe;
}

void DataTrack::Track(const std::string& name, double time, double value)
{
  if (std::find(m_HeadingOrder.begin(), m_HeadingOrder.end(), name) == m_HeadingOrder.end()) {
    m_HeadingOrder.push_back(name);
  }
  std::vector<double>* v;
  if (m_LastTime != time) {
    m_LastTime = time;
    m_Time.push_back(time);
    // Will also push NaN to all other vectors
    for (TrackMapItr itr = m_Track.begin(); itr != m_Track.end(); ++itr) {
      itr->second->push_back(std::numeric_limits<double>::quiet_NaN());
    }
  }
  // Check to see if we have a vector for provided name
  v = m_Track[name];
  if (v == nullptr) {
    v = new std::vector<double>();
    m_Track[name] = v;
    // Initialize any empty slots to NaN
    for (unsigned int i = 0; i < m_Time.size(); i++)
      v->push_back(std::numeric_limits<double>::quiet_NaN());
  }

  if (v->size() == 0) {
    if (m_Logger != nullptr) {
      m_Logger->Error("DataTrack array size error");
    } else {
      std::cout << "DataTrack array size error";
    }
  } else {
    v->at(v->size() - 1) = value;
  }
}

void DataTrack::Track(double time_s, const SEElectricalCircuit& c)
{
  for (SEElectricalCircuitNode* n : c.GetNodes()) {
    if (n->HasVoltage()) {
      auto unit = n->GetVoltage().GetUnit();
      Track(std::string { n->GetName() } + "_Voltage_" + unit->GetString(), time_s, n->GetVoltage().GetValue(*unit));
    }
    if (n->HasCharge()) {
      auto unit = n->GetCharge().GetUnit();
      Track(std::string { n->GetName() } + "_Charge_" + unit->GetString(), time_s, n->GetCharge().GetValue(*unit));
    }
  }
  for (SEElectricalCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Track(std::string { p->GetName() } + "_Switch", time_s, p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Track(std::string { p->GetName() } + "_Valve", time_s, p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Track(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), time_s, p->GetResistance().GetValue(*unit));
    }
    if (p->HasCapacitance()) {
      auto unit = p->GetCapacitance().GetUnit();
      Track(std::string { p->GetName() } + "_Capacitance_" + unit->GetString(), time_s, p->GetCapacitance().GetValue(*unit));
    }
    if (p->HasInductance()) {
      auto unit = p->GetInductance().GetUnit();
      Track(std::string { p->GetName() } + "_Inductance_" + unit->GetString(), time_s, p->GetInductance().GetValue(*unit));
    }
    if (p->HasCurrent()) {
      auto unit = p->GetCurrent().GetUnit();
      Track(std::string { p->GetName() } + "_Current_" + unit->GetString(), time_s, p->GetCurrent().GetValue(*unit));
    }
    if (p->HasVoltageSource()) {
      auto unit = p->GetVoltageSource().GetUnit();
      Track(std::string { p->GetName() } + "_VoltageSource_" + unit->GetString(), time_s, p->GetVoltageSource().GetValue(*unit));
    }
    if (p->HasCurrentSource()) {
      auto unit = p->GetCurrentSource().GetUnit();
      Track(std::string { p->GetName() } + "_CurrentSource_" + unit->GetString(), time_s, p->GetCurrentSource().GetValue(*unit));
    }
  }
}
void DataTrack::Track(double time_s, const SEFluidCircuit& c)
{
  for (SEFluidCircuitNode* n : c.GetNodes()) {
    if (n->HasPressure()) {
      auto unit = n->GetPressure().GetUnit();
      Track(std::string { n->GetName() } + "_Pressure_" + unit->GetString(), time_s, n->GetPressure().GetValue(*unit));
    }
    if (n->HasVolume()) {
      auto unit = n->GetVolume().GetUnit();
      Track(std::string { n->GetName() } + "_Volume_" + unit->GetString(), time_s, n->GetVolume().GetValue(*unit));
    }
  }
  for (SEFluidCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Track(std::string { p->GetName() } + "_Switch", time_s, p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Track(std::string { p->GetName() } + "_Valve", time_s, p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Track(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), time_s, p->GetResistance().GetValue(*unit));
    }
    if (p->HasCompliance()) {
      auto unit = p->GetCompliance().GetUnit();
      Track(std::string { p->GetName() } + "_Compliance_" + unit->GetString(), time_s, p->GetCompliance().GetValue(*unit));
    }
    if (p->HasInertance()) {
      auto unit = p->GetInertance().GetUnit();
      Track(std::string { p->GetName() } + "_Inertance_" + unit->GetString(), time_s, p->GetInertance().GetValue(*unit));
    }
    if (p->HasFlow()) {
      auto unit = p->GetFlow().GetUnit();
      Track(std::string { p->GetName() } + "_Flow_" + unit->GetString(), time_s, p->GetFlow().GetValue(*unit));
    }
    if (p->HasPressureSource()) {
      auto unit = p->GetPressureSource().GetUnit();
      Track(std::string { p->GetName() } + "_PressureSource_" + unit->GetString(), time_s, p->GetPressureSource().GetValue(*unit));
    }
    if (p->HasFlowSource()) {
      auto unit = p->GetFlowSource().GetUnit();
      Track(std::string { p->GetName() } + "_FlowSource_" + unit->GetString(), time_s, p->GetFlowSource().GetValue(*unit));
    }
  }
}
void DataTrack::Track(double time_s, const SEThermalCircuit& c)
{
  for (SEThermalCircuitNode* n : c.GetNodes()) {
    if (n->HasTemperature()) {
      auto unit = n->GetTemperature().GetUnit();
      Track(std::string { n->GetName() } + "_Temperature_" + unit->GetString(), time_s, n->GetTemperature().GetValue(*unit));
    }
    if (n->HasHeat()) {
      auto unit = n->GetHeat().GetUnit();
      Track(std::string { n->GetName() } + "_Heat_" + unit->GetString(), time_s, n->GetHeat().GetValue(*unit));
    }
  }
  for (SEThermalCircuitPath* p : c.GetPaths()) {
    if (p->HasSwitch())
      Track(std::string { p->GetName() } + "_Switch", time_s, p->GetSwitch() == CDM::enumOpenClosed::Open ? 1 : 0);
    if (p->HasValve())
      Track(std::string { p->GetName() } + "_Valve", time_s, p->GetValve() == CDM::enumOpenClosed::Closed ? 1 : 0);

    if (p->HasResistance()) {
      auto unit = p->GetResistance().GetUnit();
      Track(std::string { p->GetName() } + "_Resistance_" + unit->GetString(), time_s, p->GetResistance().GetValue(*unit));
    }
    if (p->HasCapacitance()) {
      auto unit = p->GetCapacitance().GetUnit();
      Track(std::string { p->GetName() } + "_Capacitance_" + unit->GetString(), time_s, p->GetCapacitance().GetValue(*unit));
    }
    if (p->HasInductance()) {
      auto unit = p->GetInductance().GetUnit();
      Track(std::string { p->GetName() } + "_Inductance_" + unit->GetString(), time_s, p->GetInductance().GetValue(*unit));
    }
    if (p->HasHeatTransferRate()) {
      auto unit = p->GetHeatTransferRate().GetUnit();
      Track(std::string { p->GetName() } + "_HeatTransferRate_" + unit->GetString(), time_s, p->GetHeatTransferRate().GetValue(*unit));
    }
    if (p->HasTemperatureSource()) {
      auto unit = p->GetTemperatureSource().GetUnit();
      Track(std::string { p->GetName() } + "_TemperatureSource_" + unit->GetString(), time_s, p->GetTemperatureSource().GetValue(*unit));
    }
    if (p->HasHeatSource()) {
      auto unit = p->GetHeatSource().GetUnit();
      Track(std::string { p->GetName() } + "_HeatSource_" + unit->GetString(), time_s, p->GetHeatSource().GetValue(*unit));
    }
  }
}
void DataTrack::Track(double time_s, const SEGasCompartmentGraph& graph, std::vector<SESubstance*>* substances)
{
  for (SEGasCompartment* cmpt : graph.GetCompartments()) {
    if (cmpt->HasPressure()) {
      auto unit = cmpt->GetPressure().GetUnit();
      Track(std::string { cmpt->GetName() } + "_Pressure_" + unit->GetString(), time_s, cmpt->GetPressure().GetValue(*unit));
    }
    if (cmpt->HasVolume()) {
      auto unit = cmpt->GetVolume().GetUnit();
      Track(std::string { cmpt->GetName() } + "_Volume_" + unit->GetString(), time_s, cmpt->GetVolume().GetValue(*unit));
    }

    for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
      if (substances != nullptr && !substances->empty() && !Contains((*substances), subQ->GetSubstance()))
        continue;

      if (subQ->HasPartialPressure()) {
        auto unit = subQ->GetPartialPressure().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_PartialPressure_" + unit->GetString(), time_s, subQ->GetPartialPressure().GetValue(*unit));
      }
      if (subQ->HasVolume()) {
        auto unit = subQ->GetVolume().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Volume_" + unit->GetString(), time_s, subQ->GetVolume().GetValue(*unit));
      }
      if (subQ->HasVolumeFraction()) {
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_VolumeFraction", time_s, subQ->GetVolumeFraction().GetValue());
      }
    }
  }
  for (SEGasCompartmentLink* link : graph.GetLinks()) {
    if (link->HasFlow()) {
      auto unit = link->GetFlow().GetUnit();
      Track(std::string { link->GetName() } + "_Flow_" + unit->GetString(), time_s, link->GetFlow(*unit));
    }
  }
}
void DataTrack::Track(double time_s, const SELiquidCompartmentGraph& graph, std::vector<SESubstance*>* substances)
{
  for (SELiquidCompartment* cmpt : graph.GetCompartments()) {
    if (cmpt->HasPressure()) {
      auto unit = cmpt->GetPressure().GetUnit();
      Track(std::string { cmpt->GetName() } + "_Pressure_" + unit->GetString(), time_s, cmpt->GetPressure().GetValue(*unit));
    }
    if (cmpt->HasVolume()) {
      auto unit = cmpt->GetVolume().GetUnit();
      Track(std::string { cmpt->GetName() } + "_Volume_" + unit->GetString(), time_s, cmpt->GetVolume().GetValue(*unit));
    }
    if (cmpt->HasPH()) {
      Track(std::string { cmpt->GetName() } + "_pH_", time_s, cmpt->GetPH().GetValue());
    }

    for (SELiquidSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities()) {
      if (substances != nullptr && !substances->empty() && !Contains((*substances), subQ->GetSubstance()))
        continue;

      if (subQ->HasSaturation()) {
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Saturation", time_s, subQ->GetSaturation().GetValue());
      }
      if (subQ->HasPartialPressure()) {
        auto unit = subQ->GetPartialPressure().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_PartialPressure_" + unit->GetString(), time_s, subQ->GetPartialPressure().GetValue(*unit));
      }
      if (subQ->HasMass()) {
        auto unit = subQ->GetMass().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Mass_" + unit->GetString(), time_s, subQ->GetMass().GetValue(*unit));
      }
      if (subQ->HasConcentration()) {
        auto unit = subQ->GetConcentration().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Concentration_" + unit->GetString(), time_s, subQ->GetConcentration().GetValue(*unit));
      }
      if (subQ->HasMolarity()) {
        auto unit = subQ->GetMolarity().GetUnit();
        Track(std::string { cmpt->GetName() } + "_" + subQ->GetSubstance().GetName() + "_Molarity_" + unit->GetString(), time_s, subQ->GetMolarity().GetValue(*unit));
      }
    }
  }
  for (SELiquidCompartmentLink* link : graph.GetLinks()) {
    if (link->HasFlow()) {
      auto unit = link->GetFlow().GetUnit();
      Track(std::string { link->GetName() } + "_Flow_" + unit->GetString(), time_s, link->GetFlow(*unit));
    }
  }
}

double DataTrack::GetTrack(const std::string& name, double time)
{
  std::vector<double>* v = m_Track[name];
  if (v == nullptr)
    return std::numeric_limits<double>::quiet_NaN();
  for (unsigned int i = 0; i < m_Time.size(); i++) {
    if (m_Time[i] == time)
      return v->at(i); // All lengths should be the same
  }
  return std::numeric_limits<double>::quiet_NaN();
}

std::vector<double>* DataTrack::GetTrack(const std::string& name)
{
  return m_Track[name];
}

std::vector<double>& DataTrack::GetTimes()
{
  return m_Time;
}

std::vector<std::string> DataTrack::ReadTrackFromFile(const char* fileName)
{
  m_Track.clear();
  std::string line;
  std::ifstream infile(fileName);
  // Grab the headings from the first line
  std::getline(infile, line);
  std::istringstream iss(line);
  std::vector<std::string> headings;
  std::size_t pos = 0;
  while (pos < line.size())
    if ((pos = line.find_first_of(',', pos)) != std::string::npos)
      line[pos] = ' ';
  copy(std::istream_iterator<std::string>(iss),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string>>(headings));
  // Pull out each value
  std::vector<double> values;
  while (std::getline(infile, line)) {
    pos = 0;
    while (pos < line.size())
      if ((pos = line.find_first_of(',', pos)) != std::string::npos)
        line[pos] = ' ';
    std::istringstream iss(line);
    copy(std::istream_iterator<double>(iss),
         std::istream_iterator<double>(),
         std::back_inserter<std::vector<double>>(values));
    double time = values[0];
    for (unsigned int i = 1; i < values.size(); i++) {
      Track(headings[i], time, values[i]);
    }
    values.clear();
  }
  // Get each value for a time
  infile.close();
  return headings;
}

std::vector<std::string> DataTrack::StreamDataFromFile(const char* fileName)
{
  Reset();
  std::string line;
  auto io = m_Logger->GetIoManager().lock();
  m_FileStream.open(io->ResolveLogFileLocation(fileName));
  // Grab the headings from the first line
  std::getline(m_FileStream, line);
  std::size_t pos = 0;
  while (pos < line.size()) {
    if ((pos = line.find_first_of(',', pos)) != std::string::npos) {
      line[pos] = ' ';
    }
  }
  std::istringstream iss(line);
  std::vector<std::string> headings;
  copy(std::istream_iterator<std::string>(iss),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string>>(headings));

  return headings;
}

double DataTrack::StreamDataFromFile(std::vector<std::string>* headings)
{
  double time = std::numeric_limits<double>::quiet_NaN();
  std::string line;
  // Pull out each value
  std::vector<double> values;
  if (std::getline(m_FileStream, line)) {
    std::size_t pos = 0;
    while (pos < line.size())
      if ((pos = line.find_first_of(',', pos)) != std::string::npos)
        line[pos] = ' ';
    std::istringstream iss(line);
    copy(std::istream_iterator<double>(iss),
         std::istream_iterator<double>(),
         std::back_inserter<std::vector<double>>(values));
    time = values[0];
    for (unsigned int i = 1; i < values.size(); i++)
      Probe(headings->at(i), values[i]);
    values.clear();
  }
  return time;
}

void DataTrack::CreateFile(const std::string& fileName, std::ofstream& newFile, std::ios_base::openmode mode)
{

  auto io = m_Logger->GetIoManager().lock();
  newFile.open(io->ResolveLogFileLocation(fileName), std::ofstream::out | mode);
  // Write our headers
  newFile << ((mode == std::ios_base::trunc) ? "Time(s)" : "#Time(s)") << m_Delimiter;
  for (unsigned int i = 0; i < m_HeadingOrder.size(); i++) {
    newFile << m_HeadingOrder.at(i);
    if (i < (m_HeadingOrder.size() - 1))
      newFile << m_Delimiter;
  }
  newFile << std::endl;
  newFile.flush();
}
void DataTrack::CreateFile(const char* fileName, std::ofstream& newFile, std::ios_base::openmode mode)
{
  CreateFile(std::string { fileName }, newFile, mode);
}

void DataTrack::WriteTrackToFile(const char* fileName, std::ios_base::openmode mode)
{
  std::ofstream file;

  auto io = m_Logger->GetIoManager().lock();
  std::string filepath = io->ResolveLogFileLocation(fileName);
  file.open(fileName, std::ofstream::out | mode);
  // Write our headers
  file << "Time(s)" << m_Delimiter;
  for (unsigned int i = 0; i < m_HeadingOrder.size(); i++) {
    file << m_HeadingOrder.at(i);
    if (i < (m_HeadingOrder.size() - 1))
      file << m_Delimiter;
  }
  file << std::endl;
  file.flush();
  StreamTrackToFile(file);
}

void DataTrack::StreamTrackToFile(std::ofstream& file)
{
  double d;
  // Write out map values
  for (unsigned int i = 0; i < m_Time.size(); i++) {
    file << std::fixed << std::setprecision(m_DefaultPrecision) << m_Time.at(i) << m_Delimiter;
    for (unsigned int h = 0; h < m_HeadingOrder.size(); h++) {
      const std::string& s = m_HeadingOrder[h];

      d = m_Track[s]->at(i);
      if (d == 0) {
        file << std::fixed << std::setprecision(0);
        file << 0;
      } else if (d - ((int)d) == 0) {
        file << std::fixed << std::setprecision(0);
        file << d;
      } else if (std::isnan(d)) {
        file << "-1.$";
      } else {
        m_Formatting[s].SetStream(file);
        file << d;
      }
      if (h < (m_HeadingOrder.size() - 1))
        file << m_Delimiter;
    }
    file << std::endl;
  }
  file.flush();

  // Clear out the data we had now that it is written
  m_Time.clear();
  for (TrackMapItr itr = m_Track.begin(); itr != m_Track.end(); ++itr) {
    itr->second->clear();
  }
}

void DataTrack::StreamProbesToFile(double time, std::ofstream& file)
{
  double d;
  // Write out probe values in heading order
  file << std::fixed << std::setprecision(2) << time << m_Delimiter;
  for (unsigned int h = 0; h < m_HeadingOrder.size(); h++) {
    const std::string& s = m_HeadingOrder[h];
    d = GetProbe(s);
    if (d == 0) {
      file << std::fixed << std::setprecision(0);
      file << 0;
    } else if (d - ((int)d) == 0) {
      file << std::fixed << std::setprecision(0);
      file << d;
    } else if (std::isnan(d)) {
      file << "-1.$";
    } else {
      m_Formatting[s].SetStream(file);
      file << d;
    }
    if (h < (m_HeadingOrder.size() - 1))
      file << m_Delimiter;
  }
  file << std::endl;
  file.flush();
}
}
