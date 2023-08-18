#include "Scenario.h"

#include "EngineConfiguration.h"
#include "Patient.h"
#include "PatientConditions.h"
#include "PatientActions.h"
#include "InhalerActions.h"
#include "AnesthesiaActions.h"
#include "EnvironmentActions.h"
#include "PatientConditions.h"
#include "Property.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>
#include <biogears/cdm/scenario/SECondition.h>

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
#include <biogears/cdm/scenario/requests/SEDataRequestManager.h>
#include <biogears/cdm/scenario/requests/SEEnvironmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEEquipmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEGasCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SELiquidCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPatientDataRequest.h>
#include <biogears/cdm/scenario/requests/SEPhysiologyDataRequest.h>
#include <biogears/cdm/scenario/requests/SESubstanceDataRequest.h>
#include <biogears/cdm/scenario/requests/SEThermalCompartmentDataRequest.h>
#include <biogears/cdm/scenario/requests/SETissueCompartmentDataRequest.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
namespace io {
  void Scenario::Marshall(const CDM::ConditionData& in, SECondition& out)
  {
    out.Clear();
    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  void Scenario::UnMarshall(const SECondition& in, CDM::ConditionData& out)
  {
    if (in.HasComment()) {
      out.Comment(in.m_Comment);
    }
  }
  //-----------------------------------------------------------------------------
  // class SEAction
  void Scenario::Marshall(const CDM::ActionData& in, SEAction& out)
  {
    out.Clear();
    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  void Scenario::UnMarshall(const SEAction& in, CDM::ActionData& out)
  {
    if (in.HasComment()) {
      out.Comment(in.m_Comment);
    }
  }
  //-----------------------------------------------------------------------------
  // class SEDataRequestManager
  void Scenario::Marshall(const CDM::DataRequestManagerData& in, const SESubstanceManager& subMgr, SEDataRequestManager& out)
  {
    out.Clear();
    if (in.Filename().present())
      out.m_ResultsFile = in.Filename().get();
    if (in.WorkingDir().present())
      out.m_WorkingDir = in.WorkingDir().get();
    if (in.SamplesPerSecond().present())
      out.m_SamplesPerSecond = in.SamplesPerSecond().get();
    if (in.DefaultDecimalFormatting().present())
      io::Property::Marshall(in.DefaultDecimalFormatting(), out.GetDefaultDecimalFormatting());
    if (in.OverrideDecimalFormatting().present())
      io::Property::Marshall(in.OverrideDecimalFormatting(), out.GetOverrideDecimalFormatting());

    for (auto& request : in.DataRequest()) {
      auto dr = factory(request, subMgr, out.m_DefaultDecimalFormatting);
      if (dr != nullptr) {
        if (out.HasOverrideDecimalFormatting())
          static_cast<SEDecimalFormat*>(dr.get())->Set(*out.m_OverrideDecimalFormatting);
        out.m_Requests.push_back(dr.release());
      }
    }
  }

  void Scenario::UnMarshall(const SEDataRequestManager& in, CDM::DataRequestManagerData& out)
  {
    out.SamplesPerSecond(in.m_SamplesPerSecond);
    if (in.HasResultsFilename())
      out.Filename(in.m_ResultsFile);
    if (in.HasWorkingDir())
      out.WorkingDir(in.m_WorkingDir);
    if (in.HasDefaultDecimalFormatting())
      io::Property::UnMarshall(*in.m_DefaultDecimalFormatting, out.DefaultDecimalFormatting());
    if (in.HasOverrideDecimalFormatting())
      io::Property::UnMarshall(*in.m_OverrideDecimalFormatting, out.OverrideDecimalFormatting());
    for (auto* dr : in.m_Requests) {
      CDM::DataRequestData data;
      out.DataRequest().push_back(io::Scenario::factory(dr));
    }
  }
  //-----------------------------------------------------------------------------
  // class SEDataRequest
  void Scenario::Marshall(const CDM::DataRequestData& in, SEDataRequest& out)
  {
    io::Property::Marshall(static_cast<CDM::DecimalFormatData const&>(in), static_cast<SEDecimalFormat&>(out));
    out.m_Name = in.Name();
    if (in.Unit().present()) {
      out.m_RequestedUnit = in.Unit().get();
    }
  }
  void Scenario::UnMarshall(const SEDataRequest& in, CDM::DataRequestData& out)
  {
    io::Property::UnMarshall(static_cast<SEDecimalFormat const&>(in), static_cast<CDM::DecimalFormatData&>(out));
    out.Name(in.m_Name);
    if (in.HasUnit()) {
      out.Unit(in.m_Unit->GetString());
    } else if (in.HasRequestedUnit()) {

      out.Unit(in.m_RequestedUnit);
    }
  }
  //-----------------------------------------------------------------------------
  // class SEPatientDataRequest
  void Scenario::Marshall(const CDM::PatientDataRequestData& in, SEPatientDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SEPatientDataRequest& in, CDM::PatientDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEPhysiologyDataRequest
  void Scenario::Marshall(const CDM::PhysiologyDataRequestData& in, SEPhysiologyDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SEPhysiologyDataRequest& in, CDM::PhysiologyDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEEnvironmentDataRequest
  void Scenario::Marshall(const CDM::EnvironmentDataRequestData& in, SEEnvironmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SEEnvironmentDataRequest& in, CDM::EnvironmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEEquipmentDataRequest
  void Scenario::Marshall(const CDM::EquipmentDataRequestData& in, SEEquipmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    out.SetType(in.Type());
  }
  void Scenario::UnMarshall(const SEEquipmentDataRequest& in, CDM::EquipmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //-----------------------------------------------------------------------------
  // class SECompartmentDataRequest
  void Scenario::Marshall(const CDM::CompartmentDataRequestData& in, SECompartmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    out.SetCompartment(in.Compartment());
  }
  void Scenario::UnMarshall(const SECompartmentDataRequest& in, CDM::CompartmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    out.Compartment(in.m_Compartment);
  }
  //-----------------------------------------------------------------------------
  // class SECompartmentSubstanceDataRequest
  void Scenario::Marshall(const CDM::CompartmentSubstanceDataRequestData& in, SESubstanceManager const& substances, SECompartmentSubstanceDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
    if (in.Substance().present()) {
      out.SetSubstance(substances.GetSubstance(in.Substance().get()));
    }
  }

  void Scenario::UnMarshall(const SECompartmentSubstanceDataRequest& in, CDM::CompartmentSubstanceDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
    if (in.HasSubstance()) {
      out.Substance(in.m_Substance->GetName());
    }
  }
  //-----------------------------------------------------------------------------
  // class SEGasCompartmentDataRequest
  void Scenario::Marshall(const CDM::GasCompartmentDataRequestData& in, SESubstanceManager const& substances, SEGasCompartmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::CompartmentSubstanceDataRequestData const&>(in), substances, static_cast<SECompartmentSubstanceDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SEGasCompartmentDataRequest& in, CDM::GasCompartmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SECompartmentSubstanceDataRequest const&>(in), static_cast<CDM::CompartmentSubstanceDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SELiquidCompartmentDataRequest0
  void Scenario::Marshall(const CDM::LiquidCompartmentDataRequestData& in, SESubstanceManager const& substances, SELiquidCompartmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::CompartmentSubstanceDataRequestData const&>(in), substances, static_cast<SECompartmentSubstanceDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SELiquidCompartmentDataRequest& in, CDM::LiquidCompartmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SECompartmentSubstanceDataRequest const&>(in), static_cast<CDM::CompartmentSubstanceDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SEThermalCompartmentDataRequest
  void Scenario::Marshall(const CDM::ThermalCompartmentDataRequestData& in, SEThermalCompartmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SEThermalCompartmentDataRequest& in, CDM::ThermalCompartmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
  }
  //-----------------------------------------------------------------------------
  // class SETissueCompartmentDataRequest
  void Scenario::Marshall(const CDM::TissueCompartmentDataRequestData& in, SETissueCompartmentDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::CompartmentDataRequestData const&>(in), static_cast<SECompartmentDataRequest&>(out));
  }
  void Scenario::UnMarshall(const SETissueCompartmentDataRequest& in, CDM::TissueCompartmentDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SECompartmentDataRequest const&>(in), static_cast<CDM::CompartmentDataRequestData&>(out));
  }
  // class SESubstanceDataRequest
  void Scenario::Marshall(const CDM::SubstanceDataRequestData& in, SESubstanceManager const& substances, SESubstanceDataRequest& out)
  {
    io::Scenario::Marshall(static_cast<CDM::DataRequestData const&>(in), static_cast<SEDataRequest&>(out));
    if (in.Compartment().present()) {
      out.SetCompartment(in.Compartment().get());
    }
    out.SetSubstance(substances.GetSubstance(in.Substance()));
  }
  void Scenario::UnMarshall(const SESubstanceDataRequest& in, CDM::SubstanceDataRequestData& out)
  {
    io::Scenario::UnMarshall(static_cast<SEDataRequest const&>(in), static_cast<CDM::DataRequestData&>(out));
    if (in.HasCompartment()) {
      out.Compartment(in.m_Compartment);
    }
    if (in.HasSubstance()) {
      out.Substance(in.m_Substance->GetName());
    }
  }
  //-----------------------------------------------------------------------------
  // class SEScenario
  void Scenario::Marshall(const CDM::ScenarioData& in, SEScenario& out)
  {
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
      Marshall(in.InitialParameters(), out.GetInitialParameters());
    } else {
      throw CommonDataModelException("No State or Initial Parameters provided");
    }
    if (in.AutoSerialization().present()) {
      Marshall(in.AutoSerialization(), out.GetAutoSerialization());
    }
    if (in.DataRequests().present()) {
      Scenario::Marshall(in.DataRequests().get(), out.m_SubMgr, out.m_DataRequestMgr);
    }
    for (auto& actionData : in.Action()) {
      if (auto new_action = PatientActions::factory(&actionData, out.m_SubMgr); new_action ) {
        out.m_Actions.push_back(new_action.release());
      }
    }
    if (!out.IsValid()) {
      throw CommonDataModelException("Unable Marshall SEScenario from ScenarioData");
    }
  }
  void Scenario::UnMarshall(const SEScenario& in, CDM::ScenarioData& out)
  {
    out.Name(in.m_Name);
    out.Description(in.m_Description);
    if (in.HasEngineStateFile()) {
      out.EngineStateFile(in.m_EngineStateFile);
    } else if (in.HasInitialParameters()) {
      out.InitialParameters(std::make_unique<CDM::ScenarioInitialParametersData>());
      UnMarshall(*in.m_InitialParameters, out.InitialParameters());
    }
    if (in.HasAutoSerialization()) {
      out.AutoSerialization(std::make_unique<CDM::ScenarioAutoSerializationData>());
      UnMarshall(*in.m_AutoSerialization, out.AutoSerialization());
    }
    out.DataRequests(std::unique_ptr<CDM::DataRequestManagerData>(in.m_DataRequestMgr.Unload()));
    for (auto* action : in.m_Actions) {     
      out.Action().push_back(Scenario::factory(action));
    }
  }
  //-----------------------------------------------------------------------------
  // class SEScenarioInitialParameters
  void Scenario::Marshall(const CDM::ScenarioInitialParametersData& in, SEScenarioInitialParameters& out)
  {
    out.Clear();

    if (in.Configuration().present()) {
      out.GetConfiguration().Load(in.Configuration().get());
    }
    if (in.PatientFile().present()) {
      out.m_PatientFile = in.PatientFile().get();
    } else if (in.Patient().present()) {
      out.GetPatient().Load(in.Patient().get());
    } else {
      throw CommonDataModelException("No patient provided");
    }

    for (auto const& conditionData : in.Condition()) {
      out.m_Conditions.push_back(PatientConditions::factory(&conditionData, out.m_SubMgr).release());
    }

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable Marshall SEScenarioInitialParamaters from ScenarioInitialParamatersData");
    }
  }
  void Scenario::UnMarshall(const SEScenarioInitialParameters& in, CDM::ScenarioInitialParametersData& out)
  {
    if (in.HasPatientFile()) {
      out.PatientFile(in.m_PatientFile);
    } else if (in.HasPatient()) {
      Patient::UnMarshall(*in.m_Patient, out.Patient());
    }
    for (SECondition* condition : in.m_Conditions) {
      auto conditionData = PatientConditions::factory(condition);
      out.Condition().push_back(std::move(conditionData));
    }
    if (in.HasConfiguration()) {
      EngineConfiguration::UnMarshall(*in.m_Configuration, out.Configuration());
    }
  }
  //-----------------------------------------------------------------------------
  // class SEScenarioAutoSerialization
  void Scenario::Marshall(const CDM::ScenarioAutoSerializationData& in, SEScenarioAutoSerialization& out)
  {
    out.Clear();
    io::Property::Marshall(in.Period(), out.GetPeriod());
    out.SetPeriodTimeStamps(in.PeriodTimeStamps());
    out.SetAfterActions(in.AfterActions());
    out.SetReloadState(in.ReloadState());
    out.SetDirectory(in.Directory());
    out.SetFileName(in.FileName());
  }
  void Scenario::UnMarshall(const SEScenarioAutoSerialization& in, CDM::ScenarioAutoSerializationData& out)
  {
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Period)

    if (in.HasPeriodTimeStamps()) {
      out.PeriodTimeStamps(in.m_PeriodTimeStamps);
    }
    if (in.HasAfterActions()) {
      out.AfterActions(in.m_AfterActions);
    }
    if (in.HasReloadState()) {
      out.ReloadState(in.m_ReloadState);
    }
    if (in.HasDirectory()) {
      out.Directory(in.m_Directory);
    }
    if (in.HasFileName()) {
      out.FileName(in.m_FileName);
    }
  }
  //-----------------------------------------------------------------------------
  // class SEAdvanceTime
  void Scenario::Marshall(const CDM::AdvanceTimeData& in, SEAdvanceTime& out)
  {
    Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
    io::Property::Marshall(in.Time(), out.GetTime());
  }
  void Scenario::UnMarshall(const SEAdvanceTime& in, CDM::AdvanceTimeData& out)
  {
    UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Time)
  }
  //-----------------------------------------------------------------------------
  // class SESerializeState
  void Scenario::Marshall(const CDM::SerializeStateData& in, SESerializeState& out)
  {
    Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
    out.SetType(in.Type());
    out.SetFilename(in.Filename());
  }
  void Scenario::UnMarshall(const SESerializeState& in, CDM::SerializeStateData& out)
  {
    UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
    if (in.HasFilename()) {
      out.Filename(in.m_Filename);
    }
    if (in.HasType()) {
      out.Type(in.m_Type);
    }
  }
  //-----------------------------------------------------------------------------
  std::unique_ptr<SEDataRequest> Scenario::factory(const CDM::DataRequestData& in, SESubstanceManager const& substances, const SEDecimalFormat* df)
  {
    const CDM::DataRequestData* drData = &in;
    const CDM::PhysiologyDataRequestData* physSysData = dynamic_cast<const CDM::PhysiologyDataRequestData*>(drData);
    if (physSysData != nullptr) {
      auto sys = std::make_unique<SEPhysiologyDataRequest>(df);
      Scenario::Marshall(*physSysData, *sys);
      return sys;
    }
    const CDM::GasCompartmentDataRequestData* gasData = dynamic_cast<const CDM::GasCompartmentDataRequestData*>(drData);
    if (gasData != nullptr) {
      auto Comp = std::make_unique<SEGasCompartmentDataRequest>(df);
      Scenario::Marshall(*gasData, substances, *Comp);
      return Comp;
    }
    const CDM::LiquidCompartmentDataRequestData* liquidData = dynamic_cast<const CDM::LiquidCompartmentDataRequestData*>(drData);
    if (liquidData != nullptr) {
      auto Comp = std::make_unique<SELiquidCompartmentDataRequest>(df);
      Scenario::Marshall(*liquidData, substances, *Comp);
      return Comp;
    }
    const CDM::ThermalCompartmentDataRequestData* thermData = dynamic_cast<const CDM::ThermalCompartmentDataRequestData*>(drData);
    if (thermData != nullptr) {
      auto Comp = std::make_unique<SEThermalCompartmentDataRequest>(df);
      Scenario::Marshall(*thermData, *Comp);
      return Comp;
    }
    const CDM::TissueCompartmentDataRequestData* tissueData = dynamic_cast<const CDM::TissueCompartmentDataRequestData*>(drData);
    if (tissueData != nullptr) {
      auto Comp = std::make_unique<SETissueCompartmentDataRequest>(df);
      Scenario::Marshall(*tissueData, *Comp);
      return Comp;
    }
    const CDM::PatientDataRequestData* patData = dynamic_cast<const CDM::PatientDataRequestData*>(drData);
    if (patData != nullptr) {
      auto sys = std::make_unique<SEPatientDataRequest>(df);
      Scenario::Marshall(*patData, *sys);
      return sys;
    }
    const CDM::SubstanceDataRequestData* subData = dynamic_cast<const CDM::SubstanceDataRequestData*>(drData);
    if (subData != nullptr) {
      auto sub = std::make_unique<SESubstanceDataRequest>(df);
      Scenario::Marshall(*subData, substances, *sub);
      return sub;
    }
    const CDM::EnvironmentDataRequestData* envData = dynamic_cast<const CDM::EnvironmentDataRequestData*>(drData);
    if (envData != nullptr) {
      auto env = std::make_unique<SEEnvironmentDataRequest>(df);
      Scenario::Marshall(*envData, *env);
      return env;
    }
    const CDM::EquipmentDataRequestData* equipSysData = dynamic_cast<const CDM::EquipmentDataRequestData*>(drData);
    if (equipSysData != nullptr) {
      auto sys = std::make_unique<SEEquipmentDataRequest>(df);
      Scenario::Marshall(*equipSysData, *sys);
      return sys;
    }

    if (substances.GetLogger() != nullptr)
      substances.GetLogger()->Error("Unsupported DataRequest Received", "SEDataRequest::newFromBind");
    return nullptr;
  }
  //-----------------------------------------------------------------------------
  std::unique_ptr<CDM::DataRequestData> Scenario::factory(const SEDataRequest* in)
  {

    auto physSysData = dynamic_cast<const SEPhysiologyDataRequest*>(in);
    if (physSysData != nullptr) {
      auto sys = std::make_unique<CDM::PhysiologyDataRequestData>();
      Scenario::UnMarshall(*physSysData, *sys);
      return sys;
    }
    auto gasData = dynamic_cast<const SEGasCompartmentDataRequest*>(in);
    if (gasData != nullptr) {
      auto Comp = std::make_unique<CDM::GasCompartmentDataRequestData>();
      Scenario::UnMarshall(*gasData, *Comp);
      return Comp;
    }
    auto liquidData = dynamic_cast<const SELiquidCompartmentDataRequest*>(in);
    if (liquidData != nullptr) {
      auto Comp = std::make_unique<CDM::LiquidCompartmentDataRequestData>();
      Scenario::UnMarshall(*liquidData, *Comp);
      return Comp;
    }
    auto thermData = dynamic_cast<const SEThermalCompartmentDataRequest*>(in);
    if (thermData != nullptr) {
      auto Comp = std::make_unique<CDM::ThermalCompartmentDataRequestData>();
      Scenario::UnMarshall(*thermData, *Comp);
      return Comp;
    }
    auto tissueData = dynamic_cast<const SETissueCompartmentDataRequest*>(in);
    if (tissueData != nullptr) {
      auto Comp = std::make_unique<CDM::TissueCompartmentDataRequestData>();
      Scenario::UnMarshall(*tissueData, *Comp);
      return Comp;
    }
    auto patData = dynamic_cast<const SEPatientDataRequest*>(in);
    if (patData != nullptr) {
      auto sys = std::make_unique<CDM::PatientDataRequestData>();
      Scenario::UnMarshall(*patData, *sys);
      return sys;
    }
    auto subData = dynamic_cast<const SESubstanceDataRequest*>(in);
    if (subData != nullptr) {
      auto sub = std::make_unique<CDM::SubstanceDataRequestData>();
      Scenario::UnMarshall(*subData, *sub);
      return sub;
    }
    auto envData = dynamic_cast<const SEEnvironmentDataRequest*>(in);
    if (envData != nullptr) {
      auto env = std::make_unique<CDM::EnvironmentDataRequestData>();
      Scenario::UnMarshall(*envData, *env);
      return env;
    }
    auto equipSysData = dynamic_cast<const SEEquipmentDataRequest*>(in);
    if (equipSysData != nullptr) {
      auto sys = std::make_unique<CDM::EquipmentDataRequestData>();
      Scenario::UnMarshall(*equipSysData, *sys);
      return sys;
    }

    throw CommonDataModelException("No Acceptable DataRequest Conversion Found");
  }
  //-----------------------------------------------------------------------------
  std::unique_ptr<CDM::ActionData> Scenario::factory(biogears::SEAction const* action)
  {
    
    if (auto patientAction = dynamic_cast<biogears::SEPatientAction const*>(action);patientAction) {
      return PatientActions::factory(patientAction);
    }
    
    if (auto advanceTimeAction = dynamic_cast<biogears::SEAdvanceTime const*>(action);advanceTimeAction) {
      auto advanceTimeData = std::make_unique<CDM::AdvanceTimeData>();
      Scenario::UnMarshall(*advanceTimeAction, *advanceTimeData);
      return std::move(advanceTimeData);
    }
    
    if (auto serilizeAction = dynamic_cast<biogears::SESerializeState const*>(action);serilizeAction) {
      auto serilizeActionData = std::make_unique<CDM::AdvanceTimeData>();
      Scenario::UnMarshall(*serilizeAction, *serilizeActionData);
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
