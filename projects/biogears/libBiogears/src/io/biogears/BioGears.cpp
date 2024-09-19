#include "BioGears.h"

#include <vector>
#include <memory>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/version.h>

#include "io/biogears/BioGears.h"
#include "io/biogears/BioGearsConfiguration.h"
#include "io/cdm/Actions.h"
#include "io/cdm/Circuit.h"
#include "io/cdm/Compartment.h"
#include "io/cdm/Conditions.h"
#include "io/cdm/DataRequests.h"
#include "io/cdm/Patient.h"
#include "io/cdm/Property.h"
#include "io/cdm/Substance.h"

namespace biogears {
namespace io {

  // BioGears
  void BioGears::UnMarshall(const CDM::BioGearsStateData& in, BioGearsEngine& out, const SEScalarTime* simTime)
  {
    auto requests = out.GetEngineTrack()->GetDataRequestManager().GetDataRequests();
    auto resultsFile = out.GetEngineTrack()->GetDataRequestManager().GetResultsFilename();
    std::vector<std::unique_ptr<CDM::DataRequestData>> dataVector;
    for (auto& dr : requests) {
      auto drData = std::make_unique<CDM::DataRequestData>();
      io::DataRequests::Marshall(*dr, *drData);
      dataVector.emplace_back(std::move(drData));
    }
    out.BioGears::SetUp();
    out.m_EngineTrack = PhysiologyEngineTrack(out);
    out.m_DataTrack = &out.m_EngineTrack.GetDataTrack();
    for (auto& data : dataVector) {
      out.m_EngineTrack.GetDataRequestManager().CreateDataRequest(io::DataRequests::factory(data.get(), *out.m_Substances));
    }
    out.m_EngineTrack.GetDataRequestManager().SetResultsFilename(resultsFile);
    out.m_ss.str("");
    out.m_ss.clear();

    out.m_State = EngineState::NotReady;

    // if (in.DataRequests).present()) {
    //   out.m_EngineTrack.GetDataRequestManager().Clear();
    //   out.m_EngineTrack.GetDataRequestManager().Load(in.DataRequests().get(), *out.m_Substances);
    //   out.m_EngineTrack.ForceConnection(); // I don't want to rest the file because I would loose all my data
    // }

    if (simTime != nullptr) {
      out.m_CurrentTime->Set(*simTime);
      out.m_SimulationTime->Set(*simTime);
    } else {
      if (in.SimulationTime().present()) {
        io::Property::UnMarshall(in.SimulationTime(), *(out.m_CurrentTime));
        io::Property::UnMarshall(in.SimulationTime(), *(out.m_SimulationTime));
      } else {
        out.m_CurrentTime->SetValue(0, TimeUnit::s);
        out.m_SimulationTime->SetValue(0, TimeUnit::s);
      }
    }

    io::BioGears::UnMarshall(in.AirwayMode(), out.m_AirwayMode);
    io::Property::UnMarshall(in.Intubation(), out.m_Intubation);

    /// Patient //
    if (!in.Patient().present()) {
      throw biogears::CommonDataModelException("BioGearsState must have a patient");
    } else {
      try {
        io::Patient::UnMarshall(in.Patient().get(), *out.m_Patient);
      } catch (CommonDataModelException ex) {
        throw biogears::CommonDataModelException(std::string("Error loading patient data: ") + ex.what());
      }
    }
    // Conditions //
    out.m_Conditions->Clear();
    for (const CDM::ConditionData& cData : in.Condition()) {
      if (!out.m_Conditions->ProcessCondition(cData, out)) {
        throw biogears::CommonDataModelException("Unable to load condition");
      }
    }
    // Actions //
    out.m_Actions->Clear();
    for (const CDM::ActionData& cData : in.ActiveAction()) {
      if (!out.m_Actions->ProcessAction(cData, out)) {
        throw biogears::CommonDataModelException("Unable to load action");
      }
    }
    // Substances //
    for (const CDM::SubstanceData& subData : in.ActiveSubstance()) {
      SESubstance* sub = out.m_Substances->GetSubstance(subData.Name());
      if (sub == nullptr) {
        sub = new SESubstance(out.GetLogger());
        out.m_Substances->AddSubstance(*sub);
      }
      try {
        io::Substance::UnMarshall(subData, *sub);
      } catch (CommonDataModelException ex) {
        throw biogears::CommonDataModelException("Unable to load substance " + subData.Name() + "\n\tReason: " + ex.what());
      }
      out.m_Substances->AddActiveSubstance(*sub);
    }
    // Compounds //
    for (const CDM::SubstanceCompoundData& cmpdData : in.ActiveSubstanceCompound()) {
      SESubstanceCompound* cmpd = out.m_Substances->GetCompound(cmpdData.Name());
      if (cmpd == nullptr) {
        cmpd = new SESubstanceCompound(out.GetLogger());
        out.m_Substances->AddCompound(*cmpd);
      }
      try {
        io::Substance::UnMarshall(cmpdData, *out.m_Substances, *cmpd);
      } catch (CommonDataModelException ex) {
        throw biogears::CommonDataModelException("Unable to load compound " + cmpdData.Name() + "\n\tReason: " + ex.what());
      }
    }

    // Circuit Manager //
    if (!in.CircuitManager().present()) {
      throw biogears::CommonDataModelException("BioGearsState must have a circuit manager");
    } else {
      const CDM::CircuitManagerData* cmptMgrData = dynamic_cast<const CDM::CircuitManagerData*>(&in.CircuitManager().get());
      if (cmptMgrData == nullptr) {
        throw biogears::CommonDataModelException("BioGearsState must have a BioGears circuit manager");
      } else {
        try {
          UnMarshall(*cmptMgrData, *out.m_Circuits);
        } catch (biogears::CommonDataModelException ex) {
          throw biogears::CommonDataModelException("BiogearsState Error loading circuit manager data");
        }
      }
    }
    // Compartment Manager //
    if (!in.CompartmentManager().present()) {
      throw biogears::CommonDataModelException("BioGearsState must have a compartment manager");
    } else {
      const CDM::CompartmentManagerData* cmptMgrData = dynamic_cast<const CDM::CompartmentManagerData*>(&in.CompartmentManager().get());
      if (cmptMgrData == nullptr) {
        throw biogears::CommonDataModelException("BioGearsState must have a BioGears compartment manager");
      } else {
        try {
          UnMarshall(*cmptMgrData, *out.m_Compartments, out.m_Circuits.get());
        } catch (biogears::CommonDataModelException ex) {
          throw biogears::CommonDataModelException("BiogearsState Error loading compartment manager data");
        }
      }
    }
    // Configuration //
    if (!in.Configuration().present()) {
      throw biogears::CommonDataModelException("BioGearsState must have a configuration");
    } else {
      const CDM::BioGearsConfigurationData* confData = dynamic_cast<const CDM::BioGearsConfigurationData*>(&in.Configuration().get());
      if (confData == nullptr) {
        throw biogears::CommonDataModelException("BioGearsState must have a BioGears configuration");
      } else {
        try {
          BiogearsEngineConfiguration::UnMarshall(*confData, *out.m_Configuration);
        } catch (biogears::CommonDataModelException ex) {
          throw biogears::CommonDataModelException("BiogearsState Error loading configuration data");
        }
      }
    }

    // Now, Let's see if there is anything to merge into our base configuration
    // At this point I don't think we should be doing this... but maybe you want to...
    // m_Logger->Info("Merging OnDisk Configuration");
    // BioGearsConfiguration cFile(*m_Substances);
    // cFile.Load("BioGearsConfiguration.xml");
    // m_Config->Merge(cFile);

    /////////////
    // Systems //
    /////////////
    // Physiology

    size_t PresentSystems = 0x0;
    const CDM::BioGearsInhalerData* nhlData = nullptr;
    for (const CDM::SystemData& sysData : in.System()) {
      if (auto bcData = dynamic_cast<const CDM::BioGearsBloodChemistrySystemData*>(&sysData)) {
        if (out.m_BloodChemistrySystem->Load(*bcData)) {
          PresentSystems |= BioGearsSystem::BLOOD_CHEMISTRY_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Blood Chemistry data" << std::endl;
      }
      if (auto cvData = dynamic_cast<const CDM::BioGearsCardiovascularSystemData*>(&sysData)) {
        if (out.m_CardiovascularSystem->Load(*cvData)) {
          PresentSystems |= BioGearsSystem::CARDIOVASCULAR_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Cardiovascular data" << std::endl;
      }
      if (auto dData = dynamic_cast<const CDM::BioGearsDrugSystemData*>(&sysData)) {

        if (out.m_DrugSystem->Load(*dData)) {
          PresentSystems |= BioGearsSystem::DRUG_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Drug data" << std::endl;
      }
      if (auto ndoData = dynamic_cast<const CDM::BioGearsEndocrineSystemData*>(&sysData)) {

        if (out.m_EndocrineSystem->Load(*ndoData)) {
          PresentSystems |= BioGearsSystem::ENDOCRINE_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Endocrine data" << std::endl;
      }
      if (auto nrgData = dynamic_cast<const CDM::BioGearsEnergySystemData*>(&sysData)) {

        if (out.m_EnergySystem->Load(*nrgData)) {
          PresentSystems |= BioGearsSystem::ENERGY_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Energy data" << std::endl;
      }
      if (auto gasData = dynamic_cast<const CDM::BioGearsGastrointestinalSystemData*>(&sysData)) {

        if (out.m_GastrointestinalSystem->Load(*gasData)) {
          PresentSystems |= BioGearsSystem::GASTROINTESTINAL_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Gastrointestinal data" << std::endl;
      }
      if (auto hepData = dynamic_cast<const CDM::BioGearsHepaticSystemData*>(&sysData)) {

        if (out.m_HepaticSystem->Load(*hepData)) {
          PresentSystems |= BioGearsSystem::HEPTIC_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Hepatic data" << std::endl;
      }
      if (auto nrvData = dynamic_cast<const CDM::BioGearsNervousSystemData*>(&sysData)) {

        if (out.m_NervousSystem->Load(*nrvData)) {
          PresentSystems |= BioGearsSystem::NERVOUS_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Nervous data" << std::endl;
      }
      if (auto rnlData = dynamic_cast<const CDM::BioGearsRenalSystemData*>(&sysData)) {
        if (out.m_RenalSystem->Load(*rnlData)) {
          PresentSystems |= BioGearsSystem::RENAL_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Renal data" << std::endl;
      }
      if (auto rspData = dynamic_cast<const CDM::BioGearsRespiratorySystemData*>(&sysData)) {

        if (out.m_RespiratorySystem->Load(*rspData)) {
          PresentSystems |= BioGearsSystem::RESPIRATORY_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Respiratory data" << std::endl;
      }
      if (auto tsuData = dynamic_cast<const CDM::BioGearsTissueSystemData*>(&sysData)) {

        if (out.m_TissueSystem->Load(*tsuData)) {
          PresentSystems |= BioGearsSystem::TISSUE_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Tissue data" << std::endl;
      }
      if (auto envData = dynamic_cast<const CDM::BioGearsEnvironmentData*>(&sysData)) {

        if (out.m_Environment->Load(*envData)) {
          PresentSystems |= BioGearsSystem::ENVIRONMENT_SYSTEM;
          continue;
        }
        out.m_ss << "Error loading Environment data" << std::endl;
      }
      if (auto amData = dynamic_cast<const CDM::BioGearsAnesthesiaMachineData*>(&sysData)) {

        if (out.m_AnesthesiaMachine->Load(*amData)) {
          PresentSystems |= BioGearsSystem::ANESTHESIA_NACHINE_EQUIPMENT;
          continue;
        }
        out.m_ss << "Error loading Anesthesia Machine data" << std::endl;
      }
      if (auto ecgData = dynamic_cast<const CDM::BioGearsElectroCardioGramData*>(&sysData)) {

        if (out.m_ECG->Load(*ecgData)) {
          PresentSystems |= BioGearsSystem::ELECTRO_CARDIO_GRAM_EQUIPMENT;
          continue;
        }
        out.m_ss << "Error loading ECG data" << std::endl;
      }
      if (auto nhlData = dynamic_cast<const CDM::BioGearsInhalerData*>(&sysData)) {

        if (out.m_Inhaler->Load(*nhlData)) {
          PresentSystems |= BioGearsSystem::INHALER_EQUIPMENT;
          continue;
        }
        out.m_ss << "Error loading Inhaler data" << std::endl;
      }
    }
    // Make sure we had all systems in the state
    if (PresentSystems != BioGearsSystem::ALL_SYSTEMS_AND_EQUIPMENT_PRESENT) {
      out.m_ss << "StateData is missing SystemData for ";

      if (0 == (BioGearsSystem::BLOOD_CHEMISTRY_SYSTEM & PresentSystems)) {
        out.m_ss << "Blood Chemistry, " << std::endl;
      }
      if (0 == (BioGearsSystem::CARDIOVASCULAR_SYSTEM & PresentSystems)) {
        out.m_ss << "Cardiovascula," << std::endl;
      }
      if (0 == (BioGearsSystem::DRUG_SYSTEM & PresentSystems)) {
        out.m_ss << "Drug, " << std::endl;
      }
      if (0 == (BioGearsSystem::ENDOCRINE_SYSTEM & PresentSystems)) {
        out.m_ss << "Endocrine, " << std::endl;
      }
      if (0 == (BioGearsSystem::ENERGY_SYSTEM & PresentSystems)) {
        out.m_ss << "Energy, " << std::endl;
      }
      if (0 == (BioGearsSystem::GASTROINTESTINAL_SYSTEM & PresentSystems)) {
        out.m_ss << "Gastrointestinal, " << std::endl;
      }
      if (0 == (BioGearsSystem::HEPTIC_SYSTEM & PresentSystems)) {
        out.m_ss << "Hepatic, " << std::endl;
      }
      if (0 == (BioGearsSystem::NERVOUS_SYSTEM & PresentSystems)) {
        out.m_ss << "Nervous, " << std::endl;
      }
      if (0 == (BioGearsSystem::RENAL_SYSTEM & PresentSystems)) {
        out.m_ss << "Renal, " << std::endl;
      }
      if (0 == (BioGearsSystem::RESPIRATORY_SYSTEM & PresentSystems)) {
        out.m_ss << "Respiratory, " << std::endl;
      }
      if (0 == (BioGearsSystem::TISSUE_SYSTEM & PresentSystems)) {
        out.m_ss << "Tissue, " << std::endl;
      }
      if (0 == (BioGearsSystem::ENVIRONMENT_SYSTEM & PresentSystems)) {
        out.m_ss << "Environment, " << std::endl;
      }
      if (0 == (BioGearsSystem::ANESTHESIA_NACHINE_EQUIPMENT & PresentSystems)) {
        out.m_ss << "Anesthesia Machine, " << std::endl;
      }
      if (0 == (BioGearsSystem::ELECTRO_CARDIO_GRAM_EQUIPMENT & PresentSystems)) {
        out.m_ss << "ECG, " << std::endl;
      }
      if (0 == (BioGearsSystem::INHALER_EQUIPMENT & PresentSystems)) {
        out.m_ss << "Inhaler, " << std::endl;
      }
      out.m_ss << "}.";

      throw CommonDataModelException(out.m_ss.str());
    }
    // Make sure links are correct with our modes
    out.m_Compartments->UpdateAirwayGraph();
    out.m_Compartments->GetActiveRespiratoryGraph();
    out.m_Compartments->GetActiveAerosolGraph();

    // It helps to unload what you just loaded and to a compare if you have issues
    // SaveStateToFile("WhatIJustLoaded.xml");

    // Good to go, save it off and carry on!
    out.m_State = EngineState::Active;
    if (in.Patient().present()) {
      out.m_Logger->Info("Biogears has sucesfully loaded " + in.Patient()->Name() + ".");
    } else {
      out.m_Logger->Info("Biogears has sucesfully loaded the given statefile");
    }
  }
  void BioGears::Marshall(const BioGearsEngine& in, CDM::BioGearsStateData& out)
  {

    out.contentVersion(biogears::branded_version_string());

    io::Property::Marshall(*in.m_SimulationTime, out.SimulationTime());

    if (in.m_EngineTrack.GetDataRequestManager().HasDataRequests()) {
      auto dataRequestsData = std::make_unique<CDM::DataRequestManagerData>();
      io::DataRequests::Marshall( in.m_EngineTrack.GetDataRequestManager(), *dataRequestsData );
      out.DataRequests(std::move(dataRequestsData));
    }

    io::BioGears::Marshall(in.m_AirwayMode, out.AirwayMode());
    io::Property::Marshall(in.m_Intubation, out.Intubation());
    // Patient
    CDM_OPTIONAL_PATIENT_MARSHALL_HELPER(in, out, Patient);
    // Conditions
    auto conditions = std::vector<std::unique_ptr<CDM::ConditionData>>();
    Conditions::Marshall(*in.m_Conditions, conditions);
    for (auto& cData : conditions)
    {
      out.Condition().push_back(std::move(cData));
    }
    // Actions
    std::vector<std::unique_ptr<CDM::ActionData>> activeActions;
    io::Actions::Marshall(*in.m_Actions, activeActions);
    for (auto& aData : activeActions) {
      out.ActiveAction().push_back(std::move(aData));
    }
    //
    // Active Substances/Compounds
    for (SESubstance* s : in.m_Substances->GetActiveSubstances()) {
      auto subData = std::make_unique<CDM::SubstanceData>();
      io::Substance::Marshall(*s, *subData);
      out.ActiveSubstance().push_back(std::move(subData));
    }

    for (SESubstanceCompound* c : in.m_Substances->GetActiveCompounds()) {
      auto subData = std::make_unique<CDM::SubstanceCompoundData>();
      io::Substance::Marshall(*c, *subData);
      out.ActiveSubstanceCompound().push_back(std::move(subData));
    }

    // Systems
    out.System().push_back(std::unique_ptr<CDM::BioGearsBloodChemistrySystemData>(in.m_BloodChemistrySystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsCardiovascularSystemData>(in.m_CardiovascularSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsDrugSystemData>(in.m_DrugSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsEndocrineSystemData>(in.m_EndocrineSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsEnergySystemData>(in.m_EnergySystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsGastrointestinalSystemData>(in.m_GastrointestinalSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsHepaticSystemData>(in.m_HepaticSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsNervousSystemData>(in.m_NervousSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsRenalSystemData>(in.m_RenalSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsRespiratorySystemData>(in.m_RespiratorySystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsTissueSystemData>(in.m_TissueSystem->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsEnvironmentData>(in.m_Environment->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsAnesthesiaMachineData>(in.m_AnesthesiaMachine->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsElectroCardioGramData>(in.m_ECG->Unload()));
    out.System().push_back(std::unique_ptr<CDM::BioGearsInhalerData>(in.m_Inhaler->Unload()));
    // Compartments
    // out.CompartmentManager(std::unique_ptr<CDM::CompartmentManagerData>(in.m_Compartments->Unload()));
    auto compartments = std::make_unique<CDM::CompartmentManagerData>();
    io::Compartment::Marshall(*static_cast<SECompartmentManager*>(in.m_Compartments.get()), *compartments);
    out.CompartmentManager(std::move(compartments));
    // Configuration
    if (in.m_Configuration) {
      auto biogearsConfigurationData = std::make_unique<CDM::BioGearsConfigurationData>();
      io::BiogearsEngineConfiguration::Marshall(*in.m_Configuration, *biogearsConfigurationData);
      out.Configuration(std::move(biogearsConfigurationData));
    }

    // Circuits
    auto circuits = std::make_unique<CDM::CircuitManagerData>();
    io::Circuit::Marshall(*static_cast<SECircuitManager*>(in.m_Circuits.get()), *circuits);
    out.CircuitManager(std::move(circuits));
  }
  //----------------------------------------------------------------------------------------
  // BioGearsCompartments
  void BioGears::UnMarshall(const CDM::CompartmentManagerData& in, BioGearsCompartments& out, SECircuitManager* circuits)
  {
    namespace BGE = biogears::physiology;

    io::Compartment::UnMarshall(in, out, circuits);

    out.m_CombinedCardiovascularGraph = out.GetLiquidGraph(BGE::Graph::ActiveCardiovascular);
    if (out.m_CombinedCardiovascularGraph == nullptr) {

      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::ActiveCardiovascular));
    }
    out.m_CardiovascularGraph = out.GetLiquidGraph(BGE::Graph::Cardiovascular);
    if (out.m_CardiovascularGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::Cardiovascular));
    }
    out.m_CerebralGraph = out.GetLiquidGraph(BGE::Graph::Cerebral);
    if (out.m_CerebralGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::Cerebral));
    }
    out.m_RenalGraph = out.GetLiquidGraph(BGE::Graph::Renal);
    if (out.m_RenalGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::Renal));
    }
    out.m_RespiratoryGraph = out.GetGasGraph(BGE::Graph::Respiratory);
    if (out.m_RespiratoryGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::Respiratory));
    }
    out.m_AnesthesiaMachineGraph = out.GetGasGraph(BGE::Graph::AnesthesiaMachine);
    if (out.m_AnesthesiaMachineGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::AnesthesiaMachine));
    }
    out.m_CombinedRespiratoryAnesthesiaGraph = out.GetGasGraph(BGE::Graph::RespiratoryAndAnesthesiaMachine);
    if (out.m_CombinedRespiratoryAnesthesiaGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndAnesthesiaMachine));
    }
    out.m_CombinedRespiratoryInhalerGraph = out.GetGasGraph(BGE::Graph::RespiratoryAndInhaler);
    if (out.m_CombinedRespiratoryInhalerGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndInhaler));
    }
    out.m_CombinedRespiratoryNasalCannulaGraph = out.GetGasGraph(BGE::Graph::RespiratoryAndNasalCannula);
    if (out.m_CombinedRespiratoryNasalCannulaGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndNasalCannula));
    }
    out.m_AerosolGraph = out.GetLiquidGraph(BGE::Graph::Aerosol);
    if (out.m_AerosolGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::Aerosol));
    }
    out.m_CombinedAerosolInhalerGraph = out.GetLiquidGraph(BGE::Graph::AerosolAndInhaler);
    if (out.m_CombinedAerosolInhalerGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::AerosolAndInhaler));
    }
    out.m_CombinedRespiratoryMechanicalVentilatorGraph = out.GetGasGraph(BGE::Graph::RespiratoryAndMechanicalVentilator);
    if (out.m_CombinedRespiratoryMechanicalVentilatorGraph == nullptr) {
      throw CommonDataModelException("BioGearsCompartments: Could not find required Graph " + std::string(BGE::Graph::RespiratoryAndMechanicalVentilator));
    }
  }
  void BioGears::Marshall(const BioGearsCompartments& in, CDM::CompartmentManagerData& out)
  {
    io::Compartment::Marshall(in, out);
  }
  //----------------------------------------------------------------------------------------
  //  SEBioGearsCircuits
  void BioGears::UnMarshall(const CDM::CircuitManagerData& in, BioGearsCircuits& out)
  {

    io::Circuit::UnMarshall(in, out);

    out.m_CombinedCardiovascularCircuit = out.GetFluidCircuit(BGE::Circuits::FullCardiovascular);
    if (out.m_CombinedCardiovascularCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::FullCardiovascular));
    }
    out.m_CardiovascularCircuit = out.GetFluidCircuit(BGE::Circuits::Cardiovascular);
    if (out.m_CardiovascularCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Cardiovascular));
    }
    out.m_CerebralCircuit = out.GetFluidCircuit(BGE::Circuits::Cerebral);
    if (out.m_CardiovascularCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Cerebral));
    }
    out.m_RenalCircuit = out.GetFluidCircuit(BGE::Circuits::Renal);
    if (out.m_RenalCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Renal));
    }
    out.m_RespiratoryCircuit = out.GetFluidCircuit(BGE::Circuits::Respiratory);
    if (out.m_RespiratoryCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Respiratory));
    }
    out.m_AnesthesiaMachineCircuit = out.GetFluidCircuit(BGE::Circuits::AnesthesiaMachine);
    if (out.m_AnesthesiaMachineCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::AnesthesiaMachine));
    }
    out.m_CombinedRespiratoryAnesthesiaCircuit = out.GetFluidCircuit(BGE::Circuits::RespiratoryAnesthesia);
    if (out.m_CombinedRespiratoryAnesthesiaCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryAnesthesia));
    }
    out.m_CombinedRespiratoryInhalerCircuit = out.GetFluidCircuit(BGE::Circuits::RespiratoryInhaler);
    if (out.m_CombinedRespiratoryInhalerCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryInhaler));
    }
    out.m_CombinedRespiratoryNasalCannulaCircuit = out.GetFluidCircuit(BGE::Circuits::RespiratoryNasalCannula);
    if (out.m_CombinedRespiratoryNasalCannulaCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryNasalCannula));
    }
    out.m_CombinedRespiratoryMechanicalVentilatorCircuit = out.GetFluidCircuit(BGE::Circuits::RespiratoryMechanicalVentilator);
    if (out.m_CombinedRespiratoryMechanicalVentilatorCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::RespiratoryMechanicalVentilator));
    }
    out.m_TemperatureCircuit = out.GetThermalCircuit(BGE::Circuits::Temperature);
    if (out.m_TemperatureCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::Temperature));
    }
    out.m_InternalTemperatureCircuit = out.GetThermalCircuit(BGE::Circuits::InternalTemperature);
    if (out.m_InternalTemperatureCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::InternalTemperature));
    }
    out.m_ExternalTemperatureCircuit = out.GetThermalCircuit(BGE::Circuits::ExternalTemperature);
    if (out.m_ExternalTemperatureCircuit == nullptr) {
      throw CommonDataModelException("BioGearsCircuits::Load could not find circuit : " + std::string(BGE::Circuits::ExternalTemperature));
    }
  }
  void BioGears::Marshall(BioGearsCircuits const& in, CDM::CircuitManagerData& out)
  {
    io::Circuit::Marshall(in, out);
  }
  //----------------------------------------------------------------------------------------
  //  SEErrorType
  void BioGears::UnMarshall(const CDM::enumBioGearsAirwayMode& in, SEBioGearsAirwayMode& out)
  {
    try {
      switch (in) {
      case CDM::enumBioGearsAirwayMode::Free:
        out = SEBioGearsAirwayMode::Free;
        break;
      case CDM::enumBioGearsAirwayMode::AnesthesiaMachine:
        out = SEBioGearsAirwayMode::AnesthesiaMachine;
        break;
      case CDM::enumBioGearsAirwayMode::Inhaler:
        out = SEBioGearsAirwayMode::Inhaler;
        break;
      case CDM::enumBioGearsAirwayMode::NasalCannula:
        out = SEBioGearsAirwayMode::NasalCannula;
        break;
      case CDM::enumBioGearsAirwayMode::MechanicalVentilator:
        out = SEBioGearsAirwayMode::MechanicalVentilator;
        break;
      default:
        out = SEBioGearsAirwayMode::Invalid;
        break;
      }
    } catch (xsd::cxx::tree::unexpected_enumerator<char>) {
      out = SEBioGearsAirwayMode::Invalid;
    }
  }
  void BioGears::Marshall(const SEBioGearsAirwayMode& in, CDM::enumBioGearsAirwayMode& out)
  {
    switch (in) {
    case SEBioGearsAirwayMode::Free:
      out = CDM::enumBioGearsAirwayMode::Free;
      break;
    case SEBioGearsAirwayMode::AnesthesiaMachine:
      out = CDM::enumBioGearsAirwayMode::AnesthesiaMachine;
      break;
    case SEBioGearsAirwayMode::Inhaler:
      out = CDM::enumBioGearsAirwayMode::Inhaler;
      break;
    case SEBioGearsAirwayMode::NasalCannula:
      out = CDM::enumBioGearsAirwayMode::NasalCannula;
      break;
    case SEBioGearsAirwayMode::MechanicalVentilator:
      out = CDM::enumBioGearsAirwayMode::MechanicalVentilator;
      break;
    default:
      out = "";
      break;
    }
  }
}

bool operator==(CDM::enumBioGearsAirwayMode const& lhs, SEBioGearsAirwayMode const& rhs)
{

  switch (rhs) {
  case SEBioGearsAirwayMode::Free:
    return (CDM::enumBioGearsAirwayMode::Free == lhs);
  case SEBioGearsAirwayMode::AnesthesiaMachine:
    return (CDM::enumBioGearsAirwayMode::AnesthesiaMachine == lhs);
  case SEBioGearsAirwayMode::Inhaler:
    return (CDM::enumBioGearsAirwayMode::Inhaler == lhs);
  case SEBioGearsAirwayMode::NasalCannula:
    return (CDM::enumBioGearsAirwayMode::NasalCannula == lhs);
  case SEBioGearsAirwayMode::MechanicalVentilator:
    return (CDM::enumBioGearsAirwayMode::MechanicalVentilator == lhs);
  case SEBioGearsAirwayMode::Invalid:
    return ((CDM::enumBioGearsAirwayMode::value)-1 == lhs);
  default:
    return false;
  }
}
}
