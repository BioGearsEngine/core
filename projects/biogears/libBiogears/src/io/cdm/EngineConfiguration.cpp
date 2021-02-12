#include "EngineConfiguration.h"

#include "Property.h"

#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>

#include "biogears/cdm/Serializer.h"
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/engine/PhysiologyEngineDynamicStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineTimedStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>

namespace biogears {
namespace io {
  //class PhysiologyEngineConfiguration
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineConfigurationData& in, PhysiologyEngineConfiguration& out)
  {
    if (!out.m_Merge)
      out.Clear(); // Reset only if we are not merging

    if (in.TimeStep().present())

      io::Property::Marshall(in.TimeStep(), out.GetTimeStep());
    if (in.WritePatientBaselineFile().present())
      out.SetWritePatientBaselineFile(in.WritePatientBaselineFile().get());

    if (in.ElectroCardioGramInterpolatorFile().present()) {
      if (!out.GetECGInterpolator().LoadWaveforms(in.ElectroCardioGramInterpolatorFile().get())) {
        throw CommonDataModelException("Unable to load ElectroCardioGram Waveforms file");
      }
    } else if (in.ElectroCardioGramInterpolator().present()) {
      if (!out.GetECGInterpolator().Load(in.ElectroCardioGramInterpolator().get())) {
        throw CommonDataModelException("Unable to load ElectroCardioGram Waveforms");
      }
    }

    std::unique_ptr<CDM::ObjectData> sData;
    const CDM::PhysiologyEngineTimedStabilizationData* tData = nullptr;
    const CDM::PhysiologyEngineDynamicStabilizationData* dData = nullptr;
    if (in.StabilizationCriteriaFile().present()) {
      sData = Serializer::ReadFile(in.StabilizationCriteriaFile().get(), out.GetLogger());
      if (sData == nullptr) {
        throw CommonDataModelException("Unable to load Stabilization Criteria file");
      }
      tData = dynamic_cast<const CDM::PhysiologyEngineTimedStabilizationData*>(sData.get());
      dData = dynamic_cast<const CDM::PhysiologyEngineDynamicStabilizationData*>(sData.get());
    } else if (in.StabilizationCriteria().present()) {
      tData = dynamic_cast<const CDM::PhysiologyEngineTimedStabilizationData*>(&in.StabilizationCriteria().get());
      dData = dynamic_cast<const CDM::PhysiologyEngineDynamicStabilizationData*>(&in.StabilizationCriteria().get());
    }
    if (tData != nullptr) {
      if (!out.GetTimedStabilizationCriteria().Load(*tData)) {
        throw CommonDataModelException("Unable to load Stabilization Criteria");
      }
    } else if (dData != nullptr) {
      if (!out.GetDynamicStabilizationCriteria().Load(*dData)) {
        throw CommonDataModelException("Unable to load Stabilization Criteria");
      }
    }
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineConfiguration& in, CDM::PhysiologyEngineConfigurationData& out)
  {
    if (in.HasECGInterpolator())
      out.ElectroCardioGramInterpolator(std::unique_ptr<CDM::ElectroCardioGramWaveformInterpolatorData>(in.m_ECGInterpolator->Unload()));
    if (in.HasStabilizationCriteria())
      out.StabilizationCriteria(std::unique_ptr<CDM::PhysiologyEngineStabilizationData>(in.m_StabilizationCriteria->Unload()));
    if (in.HasTimeStep())
      io::Property::UnMarshall(*in.m_TimeStep, out.TimeStep());
    if (in.HasWritePatientBaselineFile())
      out.WritePatientBaselineFile(in.m_WritePatientBaselineFile);
  }
  //----------------------------------------------------------------------------------
  //class PhysiologyEngineStabilization
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineStabilizationData& in, PhysiologyEngineStabilization& out)
  {
    out.Clear();

  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineStabilization& in, CDM::PhysiologyEngineStabilizationData& out)
  {
  }
  //----------------------------------------------------------------------------------
  //class PhysiologyEngineTimedStabilization
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineTimedStabilizationData& in, PhysiologyEngineTimedStabilization& out)
  {
    Marshall(static_cast<const CDM::PhysiologyEngineStabilizationData&>(in), static_cast<PhysiologyEngineStabilization&>(out));
    io::Property::Marshall(in.RestingStabilizationTime(), out.GetRestingStabilizationTime());
    if (in.FeedbackStabilizationTime().present()) {
      io::Property::Marshall(in.FeedbackStabilizationTime(), out.GetFeedbackStabilizationTime());
    }
    for (auto cc : in.ConditionStabilization()) {
      auto* sc = new PhysiologyEngineTimedStabilizationCriteria(nullptr);
      Marshall(cc, *sc);
      out.m_ConditionCriteria.push_back(sc);
    }
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineTimedStabilization& in, CDM::PhysiologyEngineTimedStabilizationData& out)
  {
    UnMarshall(static_cast<const PhysiologyEngineStabilization&>(in), static_cast<CDM::PhysiologyEngineStabilizationData&>(out));
    io::Property::UnMarshall(in.m_RestingStabilizationTime, out.RestingStabilizationTime());
    if (in.HasFeedbackStabilizationTime())
      io::Property::UnMarshall(*in.m_FeedbackStabilizationTime, out.FeedbackStabilizationTime());
    for (auto& cc : in.m_ConditionCriteria) {
      auto petcsData = CDM::PhysiologyEngineTimedConditionStabilizationData();
      UnMarshall(*cc, petcsData);
      out.ConditionStabilization().push_back(petcsData);
    }
  }
  //----------------------------------------------------------------------------------
  //class PhysiologyEngineTimedConditionStabilization
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineTimedConditionStabilizationData& in, PhysiologyEngineTimedStabilizationCriteria& out)
  {
    out.Clear();
    out.SetName(in.Name());
    io::Property::Marshall(in.Time(), out.GetTime());
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineTimedStabilizationCriteria& in, CDM::PhysiologyEngineTimedConditionStabilizationData& out)
  {
    out.Name(in.m_Name);
    io::Property::UnMarshall(in.GetTime(), out.Time());
  }
  //----------------------------------------------------------------------------------
  //class PhysiologyEngineDynamicStabilization
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineDynamicStabilizationData& in, PhysiologyEngineDynamicStabilization& out)
  {
    Marshall(static_cast<const CDM::PhysiologyEngineStabilizationData&>(in), static_cast<PhysiologyEngineStabilization&>(out));
    Marshall(in.RestingStabilizationCriteria(), out.GetRestingCriteria());

    if (in.FeedbackStabilizationCriteria().present())
      Marshall(in.FeedbackStabilizationCriteria(), out.GetFeedbackCriteria());
    for (auto cData : in.ConditionStabilization()) {
      PhysiologyEngineDynamicStabilizationCriteria* c = new PhysiologyEngineDynamicStabilizationCriteria(out.GetLogger());
      Marshall(cData.Criteria(), *c);
      c->SetName(cData.Name());
      out.AddConditionCriteria(*c);
    }
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineDynamicStabilization& in, CDM::PhysiologyEngineDynamicStabilizationData& out)
  {
    UnMarshall(static_cast<const PhysiologyEngineStabilization&>(in), static_cast<CDM::PhysiologyEngineStabilizationData&>(out));
    UnMarshall(in.GetRestingCriteria(), out.RestingStabilizationCriteria());
    
    if (in.HasFeedbackCriteria())
      UnMarshall(*in.m_FeedbackCriteria, out.FeedbackStabilizationCriteria());
    for (auto& c : in.m_ConditionCriteria) {
      std::unique_ptr<CDM::PhysiologyEngineDynamicConditionStabilizationData> csData(new CDM::PhysiologyEngineDynamicConditionStabilizationData());
      csData->Criteria(std::make_unique<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>());
      UnMarshall(*c, csData->Criteria());
      csData->Name(c->GetName());
      out.ConditionStabilization().push_back(*csData);
    }
  }
  //----------------------------------------------------------------------------------
  //class PhysiologyEngineDynamicStabilizationCriteria
  void EngineConfiguration::Marshall(const CDM::PhysiologyEngineDynamicStabilizationCriteriaData& in, PhysiologyEngineDynamicStabilizationCriteria& out)
  {
    out.Clear();
    io::Property::Marshall(in.ConvergenceTime(), out.GetConvergenceTime());
    io::Property::Marshall(in.MinimumReactionTime(), out.GetMinimumReactionTime());
    io::Property::Marshall(in.MaximumAllowedStabilizationTime(), out.GetMaximumAllowedStabilizationTime());
    for (auto pcData : in.PropertyConvergence())
      out.CreateSystemPropertyConvergence(pcData.PercentDifference(), pcData.Name());
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::UnMarshall(const PhysiologyEngineDynamicStabilizationCriteria& in, CDM::PhysiologyEngineDynamicStabilizationCriteriaData& out)
  {
    io::Property::UnMarshall(*in.m_ConvergenceTime, out.ConvergenceTime());
    io::Property::UnMarshall(*in.m_MinimumReactionTime, out.MinimumReactionTime());
    io::Property::UnMarshall(*in.m_MaximumAllowedStabilizationTime, out.MaximumAllowedStabilizationTime());
    for (auto pc : in.m_PropertyConvergence) {
      std::unique_ptr<CDM::PhysiologyEngineDynamicStabilizationCriteriaPropertyData> pcData(new CDM::PhysiologyEngineDynamicStabilizationCriteriaPropertyData());
      pcData->Name(pc->GetDataRequest().GetName());
      pcData->PercentDifference(pc->m_Target);
      out.PropertyConvergence().push_back(*pcData.get());
    }
  }
  //----------------------------------------------------------------------------------
}
}
