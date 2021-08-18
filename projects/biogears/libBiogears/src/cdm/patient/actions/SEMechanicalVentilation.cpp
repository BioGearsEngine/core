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
#include <biogears/cdm/patient/actions/SEMechanicalVentilation.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/schema/cdm/Properties.hxx>


namespace biogears {
SEMechanicalVentilation::SEMechanicalVentilation()
  : SEPatientAction()
{
  m_State = CDM::enumOnOff::value(-1);
  m_Flow = nullptr;
  m_Pressure = nullptr;
}
//-------------------------------------------------------------------------------
SEMechanicalVentilation::~SEMechanicalVentilation()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEMechanicalVentilation::Clear()
{
  SEPatientAction::Clear();

  m_State = CDM::enumOnOff::value(-1);
  SAFE_DELETE(m_Flow);
  SAFE_DELETE(m_Pressure);

  DELETE_VECTOR(m_GasFractions);
  m_cGasFractions.clear();
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::IsValid() const
{
  if (!HasState()) {
    Error("Mechanical Ventilation must have state.");
    return false;
  }
  if (GetState() == CDM::enumOnOff::Off)
    return true;
  else {
    if (HasGasFraction()) {
      double total = 0;
      for (const SESubstanceFraction* sf : m_cGasFractions) {
        total += sf->GetFractionAmount();
      }
      if (!SEScalar::IsValue(1, total)) {
        Error("Mechanical Ventilation Gas fractions do not sum to 1");
        return false;
      }
    }
  }
  if (!HasPressure() && !HasFlow()) {
    Error("Mechanical Ventilation must have pressure and/or flow");
    return false;
  }
  return true;
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::IsActive() const
{
  if (!HasState())
    return false;
  return GetState() == CDM::enumOnOff::On;
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::Load(const CDM::MechanicalVentilationData& in, const SESubstanceManager& subMgr)
{
  SEPatientAction::Clear();
  SetState(in.State());
  if (in.Flow().present())
    GetFlow().Load(in.Flow().get());
  else
    GetFlow().Invalidate();
  if (in.Pressure().present())
    GetPressure().Load(in.Pressure().get());
  else
    GetPressure().Invalidate();

  m_GasFractions.clear();
  m_cGasFractions.clear();
  SESubstance* sub;
  for (const CDM::SubstanceFractionData& sfData : in.GasFraction()) {
    sub = subMgr.GetSubstance(sfData.Name());
    if (sub == nullptr) {
      Error("Substance not found : " + sfData.Name());
      return false;
    }
    if (sub->GetState() != CDM::enumSubstanceState::Gas) {
      Error("Substance not gas : " + sfData.Name());
      return false;
    }
    SESubstanceFraction* sf = new SESubstanceFraction(*sub);
    sf->Load(sfData);
    m_GasFractions.push_back(sf);
    m_cGasFractions.push_back(sf);
  }

  return IsValid();
}
//-------------------------------------------------------------------------------
CDM::MechanicalVentilationData* SEMechanicalVentilation::Unload() const
{
  CDM::MechanicalVentilationData* data = new CDM::MechanicalVentilationData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEMechanicalVentilation::Unload(CDM::MechanicalVentilationData& data) const
{
  SEPatientAction::Unload(data);
  if (HasState())
    data.State(m_State);
  if (HasFlow())
    data.Flow(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_Flow->Unload()));
  if (HasPressure())
    data.Pressure(std::unique_ptr<CDM::ScalarPressureData>(m_Pressure->Unload()));

  for (SESubstanceFraction* sf : m_GasFractions)
    data.GasFraction().push_back(std::unique_ptr<CDM::SubstanceFractionData>(sf->Unload()));
}
//-------------------------------------------------------------------------------
CDM::enumOnOff::value SEMechanicalVentilation::GetState() const
{
  return m_State;
}
void SEMechanicalVentilation::SetState(CDM::enumOnOff::value state)
{
  m_State = state;
}
bool SEMechanicalVentilation::HasState() const
{
  return m_State == ((CDM::enumOnOff::value)-1) ? false : true;
}
void SEMechanicalVentilation::InvalidateState()
{
  m_State = (CDM::enumOnOff::value)-1;
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::HasFlow() const
{
  return m_Flow == nullptr ? false : m_Flow->IsValid();
}
SEScalarVolumePerTime& SEMechanicalVentilation::GetFlow()
{
  if (m_Flow == nullptr)
    m_Flow = new SEScalarVolumePerTime();
  return *m_Flow;
}
double SEMechanicalVentilation::GetFlow(const VolumePerTimeUnit& unit) const
{
  if (m_Flow == nullptr)
    return SEScalar::dNaN();
  return m_Flow->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::HasPressure() const
{
  return m_Pressure == nullptr ? false : m_Pressure->IsValid();
}
SEScalarPressure& SEMechanicalVentilation::GetPressure()
{
  if (m_Pressure == nullptr)
    m_Pressure = new SEScalarPressure();
  return *m_Pressure;
}
double SEMechanicalVentilation::GetPressure(const PressureUnit& unit) const
{
  if (m_Pressure == nullptr)
    return SEScalar::dNaN();
  return m_Pressure->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::HasGasFraction() const
{
  return m_GasFractions.size() == 0 ? false : true;
}
bool SEMechanicalVentilation::HasGasFraction(const SESubstance& s) const
{
  for (const SESubstanceFraction* sf : m_GasFractions) {
    if (&s == &sf->GetSubstance())
      return true;
  }
  return false;
}
const std::vector<SESubstanceFraction*>& SEMechanicalVentilation::GetGasFractions()
{
  return m_GasFractions;
}
const std::vector<const SESubstanceFraction*>& SEMechanicalVentilation::GetGasFractions() const
{
  return m_cGasFractions;
}
SESubstanceFraction& SEMechanicalVentilation::GetGasFraction(SESubstance& s)
{
  for (SESubstanceFraction* sf : m_GasFractions) {
    if (&s == &sf->GetSubstance())
      return *sf;
  }
  SESubstanceFraction* sf = new SESubstanceFraction(s);
  sf->GetFractionAmount().SetValue(0);
  m_GasFractions.push_back(sf);
  m_cGasFractions.push_back(sf);
  return *sf;
}
const SESubstanceFraction* SEMechanicalVentilation::GetGasFraction(const SESubstance& s) const
{
  const SESubstanceFraction* sf = nullptr;
  for (unsigned int i = 0; i < m_GasFractions.size(); i++) {
    sf = m_GasFractions[i];
    if (&s == &sf->GetSubstance())
      return sf;
  }
  return sf;
}
void SEMechanicalVentilation::RemoveGasFraction(const SESubstance& s)
{
  const SESubstanceFraction* sf;
  for (unsigned int i = 0; i < m_GasFractions.size(); i++) {
    sf = m_GasFractions[i];
    if (&s == &sf->GetSubstance()) {
      m_GasFractions.erase(m_GasFractions.begin() + i);
      m_cGasFractions.erase(m_cGasFractions.begin() + i);
      delete sf;
    }
  }
}
void SEMechanicalVentilation::RemoveGasFractions()
{
  DELETE_VECTOR(m_GasFractions);
  m_cGasFractions.clear();
}
//-------------------------------------------------------------------------------
void SEMechanicalVentilation::ToString(std::ostream& str) const
{
  str << "Patient Action : Mechanical Ventilation";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;

  str << "\n\tState: ";
  HasState() ? str << GetState() : str << "Not Set";
  str << "\n\tFlow: ";
  HasFlow() ? str << *m_Flow : str << "Not Set";
  str << "\n\tPressure: ";
  HasPressure() ? str << *m_Pressure : str << "Not Set";
  if (HasGasFraction()) {
    for (const SESubstanceFraction* sf : GetGasFractions()) {
      str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
    }
  }
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::operator==(const SEMechanicalVentilation& rhs) const
{
  /*
  
  CDM::enumOnOff::value m_State;
  SEScalarVolumePerTime* m_Flow;
  SEScalarPressure* m_Pressure;

  std::vector<SESubstanceFraction*> m_GasFractions;
  std::vector<const SESubstanceFraction*> m_cGasFractions;
  */

  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_State == rhs.m_State;
  equivilant &= (m_Flow && rhs.m_Flow) ? m_Flow->operator==(*rhs.m_Flow) : m_Flow == rhs.m_Flow;
  equivilant &= (m_Pressure && rhs.m_Pressure) ? m_Pressure->operator==(*rhs.m_Pressure) : m_Pressure == rhs.m_Pressure;
  equivilant &= m_GasFractions.size() == rhs.m_GasFractions.size();
  if (equivilant) {
    for (auto i = 0; i < m_GasFractions.size(); ++i) {
      equivilant &= (m_GasFractions[i] && rhs.m_GasFractions[i]) ? m_GasFractions[i]->operator==(*rhs.m_GasFractions[i]) : m_GasFractions[i] == rhs.m_GasFractions[i];
    }
  }
  equivilant &= m_GasFractions.size() == rhs.m_GasFractions.size();

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEMechanicalVentilation::operator!=(const SEMechanicalVentilation& rhs) const
{
  return !(*this == rhs);
}
}