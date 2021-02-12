#include "ElectroCardioGram.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>

namespace biogears {
namespace io {
  //class SEElectroCardioGram
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
  //----------------------------------------------------------------------------------
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGram& in, CDM::ElectroCardioGramData& out)
  {
    if (in.m_Lead1ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead1ElectricPotential, out.Lead1ElectricPotential());
    if (in.m_Lead2ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead2ElectricPotential, out.Lead2ElectricPotential());
    if (in.m_Lead3ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead3ElectricPotential, out.Lead3ElectricPotential());
    if (in.m_Lead4ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead4ElectricPotential, out.Lead4ElectricPotential());
    if (in.m_Lead5ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead5ElectricPotential, out.Lead5ElectricPotential());
    if (in.m_Lead6ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead6ElectricPotential, out.Lead6ElectricPotential());
    if (in.m_Lead7ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead7ElectricPotential, out.Lead7ElectricPotential());
    if (in.m_Lead8ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead8ElectricPotential, out.Lead8ElectricPotential());
    if (in.m_Lead9ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead9ElectricPotential, out.Lead9ElectricPotential());
    if (in.m_Lead10ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead10ElectricPotential, out.Lead10ElectricPotential());
    if (in.m_Lead11ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead11ElectricPotential, out.Lead11ElectricPotential());
    if (in.m_Lead12ElectricPotential != nullptr)
      io::Property::UnMarshall(*in.m_Lead12ElectricPotential, out.Lead12ElectricPotential());
  }
  //----------------------------------------------------------------------------------
  //class SEElectroCardioGramInterpolationWaveform
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramInterpolationWaveformData& in, SEElectroCardioGramInterpolatorWaveform& out)
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
  //----------------------------------------------------------------------------------
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGramInterpolatorWaveform& in, CDM::ElectroCardioGramInterpolationWaveformData& out)
  {
    if (in.HasRhythm())
      out.Rhythm(in.m_Rhythm);
    if (in.HasLeadNumber())
      out.Lead(in.m_LeadNumber);
    if (in.HasData()) {
      io::Property::UnMarshall(*in.m_Data, out.Data());
      out.ActiveIndicies(std::unique_ptr<CDM::IntegerArray>(new CDM::IntegerArray()));
      out.ActiveIndicies().get().IntegerList(std::unique_ptr<CDM::IntegerList>(new CDM::IntegerList()));
      for (int i : in.m_ActiveIndicies)
        out.ActiveIndicies().get().IntegerList().push_back(i);
    }
    if (in.HasTimeStep())
      io::Property::UnMarshall(*in.m_TimeStep, out.TimeStep());
  }
  //----------------------------------------------------------------------------------
  //class SEElectroCardioGramWaveformInterpolator
  void ElectroCardioGram::Marshall(const CDM::ElectroCardioGramWaveformInterpolatorData& in, SEElectroCardioGramInterpolator& out)
  {
    out.Clear();
    for (auto w : in.Waveform()) {
      SEElectroCardioGramInterpolatorWaveform* waveform = new SEElectroCardioGramInterpolatorWaveform(out.GetLogger());
      Marshall(w, *waveform);
      out.m_Waveforms[waveform->GetLeadNumber()][waveform->GetRhythm()] = waveform;
    }
  }
  //----------------------------------------------------------------------------------
  void ElectroCardioGram::UnMarshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramWaveformInterpolatorData& out)
  {
    for (auto i : in.m_Waveforms) {
      for (auto j : i.second) {
        auto ecgiwfData = CDM::ElectroCardioGramInterpolationWaveformData();
        UnMarshall(*j.second, ecgiwfData);
        out.Waveform().push_back(ecgiwfData);
      }
    }
  }
  //----------------------------------------------------------------------------------
}
}