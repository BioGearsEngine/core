#include "ElectroCardioGram.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolationWaveform.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramWaveformLeadNumber.h>

namespace biogears {
namespace io {
  // class SEElectroCardioGram
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramData& in, SEElectroCardioGram& out)
  {
    io::Property::Marshall(in.Lead1ElectricPotential(), out.GetLead1ElectricPotential());
    io::Property::Marshall(in.Lead2ElectricPotential(), out.GetLead2ElectricPotential());
    io::Property::Marshall(in.Lead3ElectricPotential(), out.GetLead3ElectricPotential());
    io::Property::Marshall(in.Lead4ElectricPotential(), out.GetLead4ElectricPotential());
    io::Property::Marshall(in.Lead5ElectricPotential(), out.GetLead5ElectricPotential());
    io::Property::Marshall(in.Lead6ElectricPotential(), out.GetLead6ElectricPotential());
    io::Property::Marshall(in.Lead7ElectricPotential(), out.GetLead7ElectricPotential());
    io::Property::Marshall(in.Lead8ElectricPotential(), out.GetLead8ElectricPotential());
    io::Property::Marshall(in.Lead9ElectricPotential(), out.GetLead9ElectricPotential());
    io::Property::Marshall(in.Lead10ElectricPotential(), out.GetLead10ElectricPotential());
    io::Property::Marshall(in.Lead11ElectricPotential(), out.GetLead11ElectricPotential());
    io::Property::Marshall(in.Lead12ElectricPotential(), out.GetLead12ElectricPotential());
  }
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGram& in, CDM::ElectroCardioGramData& out)
  {
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead1ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead2ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead3ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead4ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead5ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead6ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead7ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead8ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead9ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead10ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead11ElectricPotential)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Lead12ElectricPotential)
  }
  // class SEElectroCardioGram
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramWaveformLeadNumberData& in, SEElectroCardioGramWaveformLeadNumber& out)
  {
    out.m_value = static_cast<int>(in);
  }
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGramWaveformLeadNumber& in, CDM::ElectroCardioGramWaveformLeadNumberData& out)
  {
    out = in.m_value;
  }
  //----------------------------------------------------------------------------------
  // class SEElectroCardioGramInterpolationWaveform
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramInterpolationWaveformData& in, SEElectroCardioGramInterpolationWaveform& out)
  {
    out.Clear();
    out.m_Rhythm = in.Rhythm();
    out.m_LeadNumber = in.Lead();
    io::Property::Marshall(in.Data(), out.GetData());
    if (in.TimeStep().present())
      io::Property::Marshall(in.TimeStep(), out.GetTimeStep());
    if (in.ActiveIndicies().present()) {
      for (size_t i = 0; i < in.ActiveIndicies().get().IntegerList().size(); i++)
        out.m_ActiveIndicies.push_back(in.ActiveIndicies().get().IntegerList()[i]);
    }
  }
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGramInterpolationWaveform& in, CDM::ElectroCardioGramInterpolationWaveformData& out)
  {
    out.Rhythm("");
    CDM_ENUM_UNMARSHAL_HELPER(in, out, Rhythm);

    out.Lead(std::make_unique<CDM::ElectroCardioGramWaveformLeadNumberData>());
    if (in.HasLeadNumber())
      out.Lead(in.m_LeadNumber);

    out.Data(std::make_unique<CDM::FunctionElectricPotentialVsTimeData>());
    if (in.HasData()) {
      CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Data)
      out.ActiveIndicies(std::unique_ptr<CDM::IntegerArray>(new CDM::IntegerArray()));
      out.ActiveIndicies().get().IntegerList(std::unique_ptr<CDM::IntegerList>(new CDM::IntegerList()));
      for (int i : in.m_ActiveIndicies)
        out.ActiveIndicies().get().IntegerList().push_back(i);
    }
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TimeStep)
  }
  //----------------------------------------------------------------------------------
  // class SEElectroCardioGramInterpolator
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramInterpolatorData& in, SEElectroCardioGramInterpolator& out)
  {
    out.Clear();
    for (auto w : in.Waveform()) {
      SEElectroCardioGramInterpolationWaveform* waveform = new SEElectroCardioGramInterpolationWaveform(out.GetLogger());
      Marshall(w, *waveform);
      out.m_Waveforms[waveform->GetLeadNumber()][waveform->GetRhythm()] = waveform;
    }

    for (auto lead : in.Leads()) {
      auto potential = std::make_unique<SEScalarElectricPotential>();
      io::Property::Marshall(lead.ExlectricalPotential(), *potential);
      out.m_Leads[lead.Lead()] = potential.release();
    }
  }
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramInterpolatorData& out)
  {
    for (auto i : in.m_Waveforms) {
      for (auto j : i.second) {
        auto ecgiwfData = CDM::ElectroCardioGramInterpolationWaveformData();
        UnMarshall(*j.second, ecgiwfData);
        out.Waveform().push_back(ecgiwfData);
      }
    }

    for (auto lead : in.m_Leads) {
      auto potential = CDM::ElectroCardioGramLeadPotentialData();
      potential.Lead(lead.first);
      auto ecp = CDM::ScalarElectricPotentialData();
      io::Property::UnMarshall(*lead.second, ecp);
      potential.ExlectricalPotential(ecp);
      out.Leads().push_back(potential);
    }
  }
  //----------------------------------------------------------------------------------
}
}