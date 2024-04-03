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
#include <biogears/io/io-manager.h>
#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/config.h>
#endif
namespace biogears {
namespace io {
  // class PhysiologyEngineConfiguration
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineConfigurationData& in, PhysiologyEngineConfiguration& out)
  {
    if (!out.m_Merge)
      out.Clear(); // Reset only if we are not merging

    if (in.TimeStep().present())

      io::Property::UnMarshall(in.TimeStep(), out.GetTimeStep());
    if (in.WritePatientBaselineFile().present()) {
      io::Property::UnMarshall(in.WritePatientBaselineFile(), out.m_WritePatientBaselineFile);
    }
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

      auto io = out.GetLogger()->GetIoManager().lock();
      auto possible_path = io->FindConfigFile(in.StabilizationCriteriaFile().get().c_str());
      if (possible_path.empty()) {
        size_t content_size;
#ifdef BIOGEARS_IO_PRESENT
        auto content = io::get_embedded_config_file(in.StabilizationCriteriaFile().get().c_str(), content_size);
        sData = Serializer::ReadBuffer((XMLByte*)content, content_size, out.GetLogger());
#endif
      } else {
        sData = Serializer::ReadFile(possible_path, out.GetLogger());
      }

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
  void EngineConfiguration::Marshall(const PhysiologyEngineConfiguration& in, CDM::PhysiologyEngineConfigurationData& out)
  {
    if (in.HasECGInterpolator())
      out.ElectroCardioGramInterpolator(std::unique_ptr<CDM::ElectroCardioGramInterpolatorData>(in.m_ECGInterpolator->Unload()));
    if (in.HasStabilizationCriteria())
      out.StabilizationCriteria(std::unique_ptr<CDM::PhysiologyEngineStabilizationData>(in.m_StabilizationCriteria->Unload()));
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TimeStep)
    io::Property::Marshall(in.m_WritePatientBaselineFile, out.WritePatientBaselineFile());
  }
  //----------------------------------------------------------------------------------
  // class PhysiologyEngineStabilization
#pragma optmize("", off)
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineStabilizationData& in, PhysiologyEngineStabilization& out)
  {
    out.Clear();

    if (in.Canceled().present())
      out.m_Canceled = in.Canceled().get();
    if (in.CurrentTime().present())
      io::Property::UnMarshall(in.CurrentTime(), out.GetCurrentTime());
    if (in.StabilizationDuration().present())
      io::Property::UnMarshall(in.StabilizationDuration(), out.GetStabilizationDuration());
    if (in.LogProgress().present())
      out.m_LogProgress = in.LogProgress().get();
  }
#pragma optmize("", on)
  //----------------------------------------------------------------------------------
  void EngineConfiguration::Marshall(const PhysiologyEngineStabilization& in, CDM::PhysiologyEngineStabilizationData& out)
  {
    out.Canceled(in.m_Canceled);
    out.LogProgress(in.m_LogProgress);

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, CurrentTime)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, StabilizationDuration)
  }
  //----------------------------------------------------------------------------------
  // class PhysiologyEngineTimedStabilization
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineTimedStabilizationData& in, PhysiologyEngineTimedStabilization& out)
  {
    UnMarshall(static_cast<const CDM::PhysiologyEngineStabilizationData&>(in), static_cast<PhysiologyEngineStabilization&>(out));
    io::Property::UnMarshall(in.RestingStabilizationTime(), out.GetRestingStabilizationTime());
    if (in.FeedbackStabilizationTime().present()) {
      io::Property::UnMarshall(in.FeedbackStabilizationTime(), out.GetFeedbackStabilizationTime());
    }
    for (auto cc : in.ConditionStabilization()) {
      auto* sc = new PhysiologyEngineTimedStabilizationCriteria(nullptr);
      UnMarshall(cc, *sc);
      out.m_ConditionCriteria.push_back(sc);
    }
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::Marshall(const PhysiologyEngineTimedStabilization& in, CDM::PhysiologyEngineTimedStabilizationData& out)
  {
    Marshall(static_cast<const PhysiologyEngineStabilization&>(in), static_cast<CDM::PhysiologyEngineStabilizationData&>(out));
    if (in.m_RestingStabilizationTime.IsValid()) {
      out.RestingStabilizationTime(std::make_unique<std::remove_reference<decltype(out.RestingStabilizationTime())>::type>());
      io::Property::Marshall(in.m_RestingStabilizationTime, out.RestingStabilizationTime());
    }
    if (in.HasFeedbackStabilizationTime())
      io::Property::Marshall(*in.m_FeedbackStabilizationTime, out.FeedbackStabilizationTime());
    for (auto& cc : in.m_ConditionCriteria) {
      auto petcsData = CDM::PhysiologyEngineTimedConditionStabilizationData();
      Marshall(*cc, petcsData);
      out.ConditionStabilization().push_back(petcsData);
    }
  }
  //----------------------------------------------------------------------------------
  // class PhysiologyEngineTimedConditionStabilization
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineTimedConditionStabilizationData& in, PhysiologyEngineTimedStabilizationCriteria& out)
  {
    out.Clear();
    out.SetName(in.Name());
    io::Property::UnMarshall(in.Time(), out.GetTime());
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::Marshall(const PhysiologyEngineTimedStabilizationCriteria& in, CDM::PhysiologyEngineTimedConditionStabilizationData& out)
  {
    out.Name(in.m_Name);
    io::Property::Marshall(in.GetTime(), out.Time());
  }
  //----------------------------------------------------------------------------------
  // class PhysiologyEngineDynamicStabilization
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineDynamicStabilizationData& in, PhysiologyEngineDynamicStabilization& out)
  {
    UnMarshall(static_cast<const CDM::PhysiologyEngineStabilizationData&>(in), static_cast<PhysiologyEngineStabilization&>(out));
    UnMarshall(in.RestingStabilizationCriteria(), out.GetRestingCriteria());

    if (in.FeedbackStabilizationCriteria().present())
      UnMarshall(in.FeedbackStabilizationCriteria(), out.GetFeedbackCriteria());
    for (auto cData : in.ConditionStabilization()) {
      PhysiologyEngineDynamicStabilizationCriteria* c = new PhysiologyEngineDynamicStabilizationCriteria(out.GetLogger());
      UnMarshall(cData.Criteria(), *c);
      c->SetName(cData.Name());
      out.AddConditionCriteria(*c);
    }
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::Marshall(const PhysiologyEngineDynamicStabilization& in, CDM::PhysiologyEngineDynamicStabilizationData& out)
  {
    Marshall(static_cast<const PhysiologyEngineStabilization&>(in), static_cast<CDM::PhysiologyEngineStabilizationData&>(out));
    out.RestingStabilizationCriteria(std::make_unique<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>());
    Marshall(in.GetRestingCriteria(), out.RestingStabilizationCriteria());

    if (in.HasFeedbackCriteria()) {
      out.FeedbackStabilizationCriteria(std::make_unique<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>());
      Marshall(*in.m_FeedbackCriteria, out.FeedbackStabilizationCriteria());
    }
    for (auto& c : in.m_ConditionCriteria) {
      std::unique_ptr<CDM::PhysiologyEngineDynamicConditionStabilizationData> csData(new CDM::PhysiologyEngineDynamicConditionStabilizationData());
      csData->Criteria(std::make_unique<CDM::PhysiologyEngineDynamicStabilizationCriteriaData>());
      Marshall(*c, csData->Criteria());
      csData->Name(c->GetName());
      out.ConditionStabilization().push_back(*csData);
    }
  }
  //----------------------------------------------------------------------------------
  // class PhysiologyEngineDynamicStabilizationCriteria
  void EngineConfiguration::UnMarshall(const CDM::PhysiologyEngineDynamicStabilizationCriteriaData& in, PhysiologyEngineDynamicStabilizationCriteria& out)
  {
    out.Clear();
    io::Property::UnMarshall(in.ConvergenceTime(), out.GetConvergenceTime());
    io::Property::UnMarshall(in.MinimumReactionTime(), out.GetMinimumReactionTime());
    io::Property::UnMarshall(in.MaximumAllowedStabilizationTime(), out.GetMaximumAllowedStabilizationTime());
    for (auto pcData : in.PropertyConvergence())
      out.CreateSystemPropertyConvergence(pcData.PercentDifference(), pcData.Name());
  }
  //----------------------------------------------------------------------------------
  void EngineConfiguration::Marshall(const PhysiologyEngineDynamicStabilizationCriteria& in, CDM::PhysiologyEngineDynamicStabilizationCriteriaData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ConvergenceTime)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MinimumReactionTime)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, MaximumAllowedStabilizationTime)
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
