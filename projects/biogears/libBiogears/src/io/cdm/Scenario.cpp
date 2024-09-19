#include "Scenario.h"

#include "Actions.h"
#include "AnesthesiaActions.h"
#include "DataRequests.h"
#include "EngineConfiguration.h"
#include "EnvironmentActions.h"
#include "InhalerActions.h"
#include "Patient.h"
#include "PatientActions.h"
#include "PatientConditions.h"
#include "Property.h"
#include "DataRequests.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SECondition.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/patient/SEPatient.h>

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>

#include <biogears/cdm/scenario/SEAnesthesiaMachineActionCollection.h>
#include <biogears/cdm/scenario/SEEnvironmentActionCollection.h>
#include <biogears/cdm/scenario/SEInhalerActionCollection.h>
#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioAutoSerialization.h>
#include <biogears/cdm/scenario/SEScenarioInitialParameters.h>
#include <biogears/cdm/scenario/SESerializeState.h>
#include <biogears/cdm/scenario/requests/SECompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SECompartmentSubstanceDataRequest.h>
#include <biogears/cdm/scenario/requests/SEDataRequest.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/filesystem/path.h>
#include <biogears/io/io-manager.h>

namespace biogears {
namespace io {

  //-----------------------------------------------------------------------------
  // class SEScenario
#pragma optimize("", off)
  void Scenario::UnMarshall(const CDM::ScenarioData& in, SEScenario& out)
  {

    auto loadActions = [](SEScenario& scenario, CDM::ActionListData const* actionList) {
      std::unique_ptr<std::seed_seq> seed;
      std::random_device random_device;
      std::unique_ptr<std::default_random_engine> default_random_engine;

      if (actionList->RandomSeed().present() && actionList->RandomSeed().get().seed().size() != 0) {
        auto seeds = actionList->RandomSeed().get().seed();
        seed = std::make_unique<std::seed_seq>(seeds.begin(), seeds.end());
        default_random_engine = std::make_unique<std::default_random_engine>(*seed);

        std::stringstream ss;
        ss << "Using seed={";
        for (auto& seed : actionList->RandomSeed().get().seed()) {
          ss << seed << ", ";
        }
        ss.seekp(-2, ss.cur);
        ss << "}" << std::endl;
        scenario.m_Logger->Warning(ss.str());
      } else {
        seed.reset(new std::seed_seq { random_device(), random_device(), random_device(), random_device(), random_device() });
        default_random_engine = std::make_unique<std::default_random_engine>(*seed);
      }

      for (auto& action : actionList->Action()) {
        auto new_action = Actions::factory(&action, scenario.m_SubMgr, default_random_engine.get());
        if (new_action != nullptr) {
          scenario.m_Actions.push_back(new_action.release());
        }
      }
    };

    out.Clear();
    if (in.Name().present()) {
      out.m_Name = in.Name().get();
    }
    if (in.Description().present()) {
      out.m_Description = in.Description().get();
    }
    if (in.EngineStateFile().present()) {
      out.SetEngineStateFile(in.EngineStateFile().get());
    } else if (in.InitialParameters().present()) {
      UnMarshall(in.InitialParameters(), out.GetInitialParameters());
    } else {
      throw CommonDataModelException("No State or Initial Parameters provided");
    }

    if (in.AutoSerialization().present()) {
      UnMarshall(in.AutoSerialization(), out.GetAutoSerialization());
    }

    if (in.DataRequests().present()) {
      auto dataRequests = in.DataRequests().get();
      if (dataRequests.DataRequestFile().present()) {
        biogears::filesystem::path requestFile = in.DataRequests()->DataRequestFile().get();
        auto weak_io = out.GetLogger()->GetIoManager();
        auto iom = weak_io.lock();

        if (requestFile.exists()) {
          auto sData = Serializer::ReadFile(requestFile.ToString(), out.GetLogger());
          if (auto requestManagerData = dynamic_cast<CDM::DataRequestManagerData*>(sData.get())) {
            // We are ignoring recursive DataRequestManagerData where an DataRequestManagerData has an DataRequestFile reference
            DataRequests::UnMarshall(*requestManagerData, out.m_SubMgr, out.m_DataRequestMgr);
          }
        } else {
          throw CommonDataModelException("Can not find " + requestFile.ToString());
        }
      } else {
        DataRequests::UnMarshall(dataRequests, out.m_SubMgr, out.m_DataRequestMgr);
      }
    }

    if (in.Actions().ActionFile().present()) {
      biogears::filesystem::path actionFile = in.Actions().ActionFile().get();
      auto weak_io = out.GetLogger()->GetIoManager();
      auto iom = weak_io.lock();

      if (actionFile.exists()) {
        auto sData = Serializer::ReadFile(actionFile.ToString(), out.GetLogger());
        if (auto actionList = dynamic_cast<CDM::ActionListData*>(sData.get())) {
          // We are ignoring recursive ActionListData where an ActionListData has an ActionFile reference
          loadActions(out, actionList);
        } else {
          throw CommonDataModelException("Unable to load " + actionFile.ToString() + "File is not ofrmated properly.");
        }
      } else {
        throw CommonDataModelException("Can not find " + actionFile.ToString());
      }
    } else {
      loadActions(out, &in.Actions());
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable UnMarshall SEScenario from ScenarioData");
    }
  }

  void Scenario::Marshall(const SEScenario& in, CDM::ScenarioData& out)
  {

    out.Name(in.m_Name);
    out.Description(in.m_Description);
    if (in.HasEngineStateFile()) {
      out.EngineStateFile(in.m_EngineStateFile);
    } else if (in.HasInitialParameters()) {
      Marshall(*in.m_InitialParameters, out.InitialParameters());
    }

    if (in.HasAutoSerialization()) {
      out.AutoSerialization(std::make_unique<CDM::ScenarioAutoSerializationData>());
      Marshall(*in.m_AutoSerialization, out.AutoSerialization());
    }

    io::DataRequests::Marshall(in.m_DataRequestMgr, out.DataRequests());

    out.Actions(std::make_unique<CDM::ActionListData>());
    if (in.HasActionFile()) {
      out.Actions().ActionFile().set(in.m_ActionFile);
      in.m_Logger->Warning("The ActionFile property has a value so no Actions will be unloaded.");
      in.m_Logger->Debug("Use UnLoad (CDM::ActionFileListData) to populate an ActionFile Object.");
    } else {
      for (auto* action : in.m_Actions) {
        out.Actions().Action().push_back(Scenario::factory(action));
      }
    }
  }

  void Scenario::Marshall(const SEScenario& in, CDM::ActionListData& out)
  {
    for (auto& action : in.GetActions()) {
      auto actionData = std::make_unique<CDM::ActionData>();
      Actions::Marshall(*action, *actionData);
      out.Action().push_back(std::move(actionData));
    }
  }
#pragma optimize("", on)
  //-----------------------------------------------------------------------------
  // class SEScenarioInitialParameters
  void Scenario::UnMarshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out)
  {
    out.Clear();

    if (in.Configuration().present()) {
      io::EngineConfiguration::UnMarshall(in.Configuration(), out.GetConfiguration());
    }
    if (in.PatientFile().present()) {
      out.m_PatientFile = in.PatientFile().get();
    } else if (in.Patient().present()) {      
      io::Patient::UnMarshall(in.Patient().get(), out.GetPatient());
    } else {
      throw CommonDataModelException("No patient provided");
    }

    for (auto const& conditionData : in.Condition()) {
      out.m_Conditions.push_back(Conditions::factory(&conditionData, out.m_SubMgr).release());
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable UnMarshall SEScenarioInitialParamaters from ScenarioInitialParamatersData");
    }
  }
  void Scenario::Marshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out)
  {
    if (in.HasPatientFile()) {
      out.PatientFile(in.m_PatientFile);
    } else if (in.HasPatient()) {
      Patient::Marshall(*in.m_Patient, out.Patient());
    }
    for (SECondition* condition : in.m_Conditions) {
      auto conditionData = Conditions::factory(condition);
      out.Condition().push_back(std::move(conditionData));
    }
    if (in.HasConfiguration()) {
      EngineConfiguration::Marshall(*in.m_Configuration, out.Configuration());
    }
  }
  //-----------------------------------------------------------------------------
  // class SEScenarioAutoSerialization
  void Scenario::UnMarshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out)
  {
    out.Clear();
    io::Property::UnMarshall(in.Period(), out.GetPeriod());

    auto periodTimeStamp = out.GetPeriodTimeStamps();
    io::Property::UnMarshall(in.PeriodTimeStamps(), periodTimeStamp);
    out.SetPeriodTimeStamps(periodTimeStamp);

    auto afterActions = out.GetAfterActions();
    io::Property::UnMarshall(in.AfterActions(), afterActions);
    out.SetAfterActions(afterActions);

    auto reloadState = out.GetReloadState();
    io::Property::UnMarshall(in.ReloadState(), reloadState);
    out.SetReloadState(reloadState);

    out.SetDirectory(in.Directory());
    out.SetFileName(in.FileName());
  }
  void Scenario::Marshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Period)


    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, PeriodTimeStamps)
    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, AfterActions)
    SE_PROPERTY_ENUM_MARSHALL_HELPER(in, out, ReloadState)

    if (in.HasDirectory()) {
      out.Directory(in.m_Directory);
    }
    if (in.HasFileName()) {
      out.FileName(in.m_FileName);
    }
  }

  //-----------------------------------------------------------------------------
  std::unique_ptr<CDM::ActionData> Scenario::factory(biogears::SEAction const* action)
  {

    if (auto patientAction = dynamic_cast<biogears::SEPatientAction const*>(action); patientAction) {
      return PatientActions::factory(patientAction);
    }

    if (auto advanceTimeAction = dynamic_cast<biogears::SEAdvanceTime const*>(action); advanceTimeAction) {
      auto advanceTimeData = std::make_unique<CDM::AdvanceTimeData>();
      Actions::Marshall(*advanceTimeAction, *advanceTimeData);
      return std::move(advanceTimeData);
    }

    if (auto serilizeAction = dynamic_cast<biogears::SESerializeState const*>(action); serilizeAction) {
      auto serilizeActionData = std::make_unique<CDM::SerializeStateData>();
      Actions::Marshall(*serilizeAction, *serilizeActionData);
      return std::move(serilizeActionData);
    }

    if (auto environmentAction = dynamic_cast<biogears::SEEnvironmentAction const*>(action); environmentAction) {
      return EnvironmentActions::factory(environmentAction);
    }

    if (auto anesthesiaMachineAction = dynamic_cast<biogears::SEAnesthesiaMachineAction const*>(action); anesthesiaMachineAction) {
      return AnesthesiaActions::factory(anesthesiaMachineAction);
    }

    if (auto inhalerAction = dynamic_cast<biogears::SEInhalerAction const*>(action); inhalerAction) {
      return InhalerActions::factory(inhalerAction);
    }

    throw biogears::CommonDataModelException("Scenario::factory does not support the derived SEAction. If you are not a developer contact upstream for support.");
  }

}
}
