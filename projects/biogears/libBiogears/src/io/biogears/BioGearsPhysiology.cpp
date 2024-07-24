#include "BioGearsPhysiology.h"

#include <sstream>

#include "../cdm/PatientActions.h"
#include "../cdm/Property.h"
#include "../cdm/System.h"

#include <biogears/schema/biogears/BioGearsPhysiology.hxx>
#include <biogears/schema/cdm/Substance.hxx>

#include <biogears/cdm/properties/SEScalarPressure.h>

#include <biogears/engine/Systems/BloodChemistry.h>
#include <biogears/engine/Systems/Cardiovascular.h>
#include <biogears/engine/Systems/Diffusion.h>
#include <biogears/engine/Systems/Drugs.h>
#include <biogears/engine/Systems/Endocrine.h>
#include <biogears/engine/Systems/Energy.h>
#include <biogears/engine/Systems/Environment.h>
#include <biogears/engine/Systems/Gastrointestinal.h>
#include <biogears/engine/Systems/Hepatic.h>
#include <biogears/engine/Systems/Nervous.h>
#include <biogears/engine/Systems/Renal.h>
#include <biogears/engine/Systems/Respiratory.h>
#include <biogears/engine/Systems/Saturation.h>
#include <biogears/engine/Systems/Tissue.h>

namespace biogears {
namespace io {
  // class BloodChemistry
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsBloodChemistrySystemData& in, const SESubstanceManager& substances, BloodChemistry& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    io::Property::UnMarshall(in.ArterialOxygenPressure(), out.GetArterialOxygenPressure());
    io::Property::UnMarshall(in.ArterialCarbonDioxidePressure(), out.GetArterialCarbonDioxidePressure());

    out.m_RhFactorMismatch_ct = in.RhFactorMismatch_ct();
    out.m_RhTransfusionReactionVolume_mL = in.RhTransfusionReactionVolume_mL();
    out.m_donorRBC_ct = in.DonorRBC_ct();
    out.m_patientRBC_ct = in.PatientRBC_ct();
    out.m_2Agglutinate_ct = in.TwoCellAgglutinates_ct();
    out.m_p3Agglutinate_ct = in.ThreeCellPatAgglutinates_ct();
    out.m_d3Agglutinate_ct = in.ThreeCellDonAgglutinates_ct();
    out.m_4Agglutinate_ct = in.FourCellAgglutinates_ct();
    out.m_RemovedRBC_ct = in.RemovedRBC_ct();

    // radiation model values
    out.m_progenitorLymphocytes_ct = in.progenitorLymphocytes();
    out.m_progenitorLymphocytes_wd_ct = in.progenitorLymphocytes_wd();
    out.m_progenitorLymphocytes_d_ct = in.progenitorLymphocytes_d();
    out.m_progenitorLymphocytes_hd_ct = in.progenitorLymphocytes_hd();
    out.m_maturingLymphocytes_ct = in.maturingLymphocytes();
    out.m_maturingLymphocytes_d_ct = in.maturingLymphocytes_d();
    out.m_maturingLymphocytes_hd_ct = in.maturingLymphocytes_hd();
    out.m_Lymphocytes_ct = in.Lymphocytes();
    out.m_Lymphocytes_d_ct = in.Lymphocytes_d();
    out.m_Lymphocytes_hd_ct = in.Lymphocytes_hd();
    out.m_radAbsorbed_Gy = in.RadiationAbsorbed();

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const BloodChemistry& in, CDM::BioGearsBloodChemistrySystemData& out)
  {
    io::System::Marshall(in, out);

    if (in.m_ArterialOxygenPressure != nullptr) {
      io::Property::Marshall(*in.m_ArterialOxygenPressure, out.ArterialOxygenPressure());
    }
    if (in.m_ArterialCarbonDioxidePressure != nullptr) {
      io::Property::Marshall(*in.m_ArterialCarbonDioxidePressure, out.ArterialCarbonDioxidePressure());
    }

    out.RhFactorMismatch_ct(in.m_RhFactorMismatch_ct);
    out.RhTransfusionReactionVolume_mL(in.m_RhTransfusionReactionVolume_mL);
    out.DonorRBC_ct(in.m_donorRBC_ct);
    out.PatientRBC_ct(in.m_patientRBC_ct);
    out.TwoCellAgglutinates_ct(in.m_2Agglutinate_ct);
    out.ThreeCellPatAgglutinates_ct(in.m_p3Agglutinate_ct);
    out.ThreeCellDonAgglutinates_ct(in.m_d3Agglutinate_ct);
    out.FourCellAgglutinates_ct(in.m_4Agglutinate_ct);
    out.RemovedRBC_ct(in.m_RemovedRBC_ct);

    // radiation model values
    out.progenitorLymphocytes(in.m_progenitorLymphocytes_ct);
    out.progenitorLymphocytes_wd(in.m_progenitorLymphocytes_wd_ct);
    out.progenitorLymphocytes_d(in.m_progenitorLymphocytes_d_ct);
    out.progenitorLymphocytes_hd(in.m_progenitorLymphocytes_hd_ct);
    out.maturingLymphocytes(in.m_maturingLymphocytes_ct);
    out.maturingLymphocytes_d(in.m_maturingLymphocytes_d_ct);
    out.maturingLymphocytes_hd(in.m_maturingLymphocytes_hd_ct);
    out.Lymphocytes(in.m_Lymphocytes_ct);
    out.Lymphocytes_d(in.m_Lymphocytes_d_ct);
    out.Lymphocytes_hd(in.m_Lymphocytes_hd_ct);
    out.RadiationAbsorbed(in.m_radAbsorbed_Gy);
  }
  // class Cardiovascular
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsCardiovascularSystemData& in, const SESubstanceManager& substances, Cardiovascular& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    out.m_StartSystole = in.StartSystole();
    out.m_HeartFlowDetected = in.HeartFlowDetected();
    out.m_EnterCardiacArrest = in.EnterCardiacArrest();
    out.m_CardiacCyclePeriod_s = in.CardiacCyclePeriod_s();
    out.m_CurrentCardiacCycleDuration_s = in.CurrentCardiacCycleDuration_s();
    out.m_LeftHeartElastanceModifier = in.LeftHeartElastanceModifier();
    out.m_LeftHeartElastance_mmHg_Per_mL = in.LeftHeartElastance_mmHg_Per_mL();
    out.m_LeftHeartElastanceMax_mmHg_Per_mL = in.LeftHeartElastanceMax_mmHg_Per_mL();
    out.m_LeftHeartElastanceMin_mmHg_Per_mL = in.LeftHeartElastanceMin_mmHg_Per_mL();
    out.m_RightHeartElastance_mmHg_Per_mL = in.RightHeartElastance_mmHg_Per_mL();
    out.m_RightHeartElastanceMax_mmHg_Per_mL = in.RightHeartElastanceMax_mmHg_Per_mL();
    out.m_RightHeartElastanceMin_mmHg_Per_mL = in.RightHeartElastanceMin_mmHg_Per_mL();

    // need to initialize these on a state load or there is an nan error that pops up,
    /// todo: might want to revisit the override functionality to properly do this
    out.m_OverrideLHEMax_Conformant_mmHg = out.m_LeftHeartElastanceMax_mmHg_Per_mL;
    out.m_OverrideRHEMax_Conformant_mmHg = out.m_RightHeartElastanceMax_mmHg_Per_mL;
    out.m_OverrideLHEMin_Conformant_mmHg = out.m_LeftHeartElastanceMin_mmHg_Per_mL;
    out.m_OverrideRHEMin_Conformant_mmHg = out.m_RightHeartElastanceMin_mmHg_Per_mL;

    out.m_CompressionTime_s = in.CompressionTime_s();
    out.m_CompressionRatio = in.CompressionRatio();
    out.m_CompressionPeriod_s = in.CompressionPeriod_s();

    out.m_CurrentCardiacCycleTime_s = in.CurrentCardiacCycleTime_s();
    out.m_CardiacCycleDiastolicVolume_mL = in.CardiacCycleDiastolicVolume_mL();
    out.m_CardiacCycleAortaPressureLow_mmHg = in.CardiacCycleAortaPressureLow_mmHg();
    out.m_CardiacCycleAortaPressureHigh_mmHg = in.CardiacCycleAortaPressureHigh_mmHg();
    out.m_CardiacCyclePulmonaryArteryPressureLow_mmHg = in.CardiacCyclePulmonaryArteryPressureLow_mmHg();
    out.m_CardiacCyclePulmonaryArteryPressureHigh_mmHg = in.CardiacCyclePulmonaryArteryPressureHigh_mmHg();
    out.m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg = in.LastCardiacCycleMeanArterialCO2PartialPressure_mmHg();
    out.m_CardiacCycleStrokeVolume_mL = in.CardiacCycleStrokeVolume_mL();

    io::Property::UnMarshall(in.CardiacCycleArterialPressure_mmHg(), out.m_CardiacCycleArterialPressure_mmHg);
    io::Property::UnMarshall(in.CardiacCycleArterialCO2PartialPressure_mmHg(), out.m_CardiacCycleArterialCO2PartialPressure_mmHg);
    io::Property::UnMarshall(in.CardiacCyclePulmonaryCapillariesWedgePressure_mmHg(), out.m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg);
    io::Property::UnMarshall(in.CardiacCyclePulmonaryCapillariesFlow_mL_Per_s(), out.m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s);
    io::Property::UnMarshall(in.CardiacCyclePulmonaryShuntFlow_mL_Per_s(), out.m_CardiacCyclePulmonaryShuntFlow_mL_Per_s);
    io::Property::UnMarshall(in.CardiacCyclePulmonaryArteryPressure_mmHg(), out.m_CardiacCyclePulmonaryArteryPressure_mmHg);
    io::Property::UnMarshall(in.CardiacCycleCentralVenousPressure_mmHg(), out.m_CardiacCycleCentralVenousPressure_mmHg);
    io::Property::UnMarshall(in.CardiacCycleSkinFlow_mL_Per_s(), out.m_CardiacCycleSkinFlow_mL_Per_s);

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Cardiovascular& in, CDM::BioGearsCardiovascularSystemData& out)
  {
    io::System::Marshall(in, out);

    out.StartSystole(in.m_StartSystole);
    out.HeartFlowDetected(in.m_HeartFlowDetected);
    out.EnterCardiacArrest(in.m_EnterCardiacArrest);
    out.CardiacCyclePeriod_s(in.m_CardiacCyclePeriod_s);
    out.CurrentCardiacCycleDuration_s(in.m_CurrentCardiacCycleDuration_s);
    out.LeftHeartElastance_mmHg_Per_mL(in.m_LeftHeartElastance_mmHg_Per_mL);
    out.LeftHeartElastanceModifier(in.m_LeftHeartElastanceModifier);
    out.LeftHeartElastanceMax_mmHg_Per_mL(in.m_LeftHeartElastanceMax_mmHg_Per_mL);
    out.LeftHeartElastanceMin_mmHg_Per_mL(in.m_LeftHeartElastanceMin_mmHg_Per_mL);
    out.RightHeartElastance_mmHg_Per_mL(in.m_RightHeartElastance_mmHg_Per_mL);
    out.RightHeartElastanceMax_mmHg_Per_mL(in.m_RightHeartElastanceMax_mmHg_Per_mL);
    out.RightHeartElastanceMin_mmHg_Per_mL(in.m_RightHeartElastanceMin_mmHg_Per_mL);

    out.CompressionTime_s(in.m_CompressionTime_s);
    out.CompressionRatio(in.m_CompressionRatio);
    out.CompressionPeriod_s(in.m_CompressionPeriod_s);

    out.CurrentCardiacCycleTime_s(in.m_CurrentCardiacCycleTime_s);
    out.CardiacCycleDiastolicVolume_mL(in.m_CardiacCycleDiastolicVolume_mL);
    out.CardiacCycleAortaPressureLow_mmHg(in.m_CardiacCycleAortaPressureLow_mmHg);
    out.CardiacCycleAortaPressureHigh_mmHg(in.m_CardiacCycleAortaPressureHigh_mmHg);
    out.CardiacCyclePulmonaryArteryPressureLow_mmHg(in.m_CardiacCyclePulmonaryArteryPressureLow_mmHg);
    out.CardiacCyclePulmonaryArteryPressureHigh_mmHg(in.m_CardiacCyclePulmonaryArteryPressureHigh_mmHg);
    out.LastCardiacCycleMeanArterialCO2PartialPressure_mmHg(in.m_LastCardiacCycleMeanArterialCO2PartialPressure_mmHg);
    out.CardiacCycleStrokeVolume_mL(in.m_CardiacCycleStrokeVolume_mL);

    io::Property::Marshall(in.m_CardiacCycleArterialPressure_mmHg, out.CardiacCycleArterialPressure_mmHg());
    io::Property::Marshall(in.m_CardiacCycleArterialCO2PartialPressure_mmHg, out.CardiacCycleArterialCO2PartialPressure_mmHg());
    io::Property::Marshall(in.m_CardiacCyclePulmonaryCapillariesWedgePressure_mmHg, out.CardiacCyclePulmonaryCapillariesWedgePressure_mmHg());
    io::Property::Marshall(in.m_CardiacCyclePulmonaryCapillariesFlow_mL_Per_s, out.CardiacCyclePulmonaryCapillariesFlow_mL_Per_s());
    io::Property::Marshall(in.m_CardiacCyclePulmonaryShuntFlow_mL_Per_s, out.CardiacCyclePulmonaryShuntFlow_mL_Per_s());
    io::Property::Marshall(in.m_CardiacCyclePulmonaryArteryPressure_mmHg, out.CardiacCyclePulmonaryArteryPressure_mmHg());
    io::Property::Marshall(in.m_CardiacCycleCentralVenousPressure_mmHg, out.CardiacCycleCentralVenousPressure_mmHg());
    io::Property::Marshall(in.m_CardiacCycleSkinFlow_mL_Per_s, out.CardiacCycleSkinFlow_mL_Per_s());
  }

  // class DrugSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsDrugSystemData& in, const SESubstanceManager& substances, Drugs& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    out.m_SarinRbcAcetylcholinesteraseComplex_nM = in.SarinRbcAcetylcholinesteraseComplex_nM();
    out.m_AgedRbcAcetylcholinesterase_nM = in.AgedRbcAcetylcholinesterase_nM();

    out.BioGearsSystem::LoadState();

    std::stringstream ss;
    for (const CDM::SubstanceBolusStateData& bData : in.BolusAdministration()) {
      SESubstance* sub = substances.GetSubstance(bData.Substance());
      if (sub == nullptr) {
        ss.str("");
        ss << "Drugs::Load::BolusAdministration Substance not found : " + bData.Substance();
        throw CommonDataModelException(ss.str());
      }
      SESubstanceBolusState* bolusState = new SESubstanceBolusState(*sub);
      out.m_BolusAdministrations[sub] = bolusState;
      bolusState->Load(bData);
    }

    for (const CDM::TransmucosalStateData& otData : in.TransmucosalStates()) {
      SESubstance* sub = substances.GetSubstance(otData.Substance());
      if (sub == nullptr) {
        ss.str("");
        ss << "Drugs::Load::OralAdministration Substance not found : " + otData.Substance();
        throw CommonDataModelException(ss.str());
      }
      SETransmucosalState* otState = new SETransmucosalState(*sub);
      out.m_TransmucosalStates[sub] = otState;
      otState->Load(otData);
    }

    for (const CDM::NasalStateData& nData : in.NasalStates()) {
      SESubstance* sub = substances.GetSubstance(nData.Substance());
      if (sub == nullptr) {
        ss.str("");
        ss << "Drugs::Load::NasalAdministration Substance not found : " + nData.Substance();
        throw CommonDataModelException(ss.str());
      }
      SENasalState* nState = new SENasalState(*sub);
      out.m_NasalStates[sub] = nState;
      nState->Load(nData);
    }

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Drugs& in, CDM::BioGearsDrugSystemData& out)
  {
    io::System::Marshall(in, out);

    out.SarinRbcAcetylcholinesteraseComplex_nM(in.m_SarinRbcAcetylcholinesteraseComplex_nM);
    out.AgedRbcAcetylcholinesterase_nM(in.m_AgedRbcAcetylcholinesterase_nM);

    CDM::SubstanceBolusStateData bolus_state;
    for (auto itr : in.m_BolusAdministrations) {
      if (itr.second != nullptr) {
        io::PatientActions::Marshall(*itr.second, bolus_state);
        out.BolusAdministration().push_back(bolus_state);
      }
    }

    CDM::TransmucosalStateData transmucosal_state;
    for (auto itr : in.m_TransmucosalStates) {
      if (itr.second != nullptr) {
        io::PatientActions::Marshall(*itr.second, transmucosal_state);
        out.TransmucosalStates().push_back(transmucosal_state);
      }
    }

    CDM::NasalStateData nasal_state;
    for (auto itr : in.m_NasalStates) {
      if (itr.second != nullptr) {
        io::PatientActions::Marshall(*itr.second, nasal_state);
        out.NasalStates().push_back(nasal_state);
      }
    }
  }
  // class EndocrineSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsEndocrineSystemData& in, const SESubstanceManager& substances, Endocrine& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);
    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Endocrine& in, CDM::BioGearsEndocrineSystemData& out)
  {
    io::System::Marshall(in, out);
  }
  // class EnergySystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsEnergySystemData& in, const SESubstanceManager& substances, Energy& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);
    io::Property::UnMarshall(in.BloodpH(), out.m_BloodpH);
    io::Property::UnMarshall(in.BicarbonateMolarity_mmol_Per_L(), out.m_BicarbonateMolarity_mmol_Per_L);
    out.m_packOn = in.PackOn();
    out.m_previousWeightPack_kg = in.PreviousWeightPack_kg();
    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Energy& in, CDM::BioGearsEnergySystemData& out)
  {
    io::System::Marshall(in, out);
    io::Property::Marshall(in.m_BloodpH, out.BloodpH());
    io::Property::Marshall(in.m_BicarbonateMolarity_mmol_Per_L, out.BicarbonateMolarity_mmol_Per_L());
    out.PackOn(in.m_packOn);
    out.PreviousWeightPack_kg(in.m_previousWeightPack_kg);
  }
  // class GastrointestinalSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsGastrointestinalSystemData& in, const SESubstanceManager& substances, Gastrointestinal& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    // Loading drug absorption/transit model states here (rather than SE side) because we want to map them to an SESubstance already defined in Sub Manager
    std::stringstream ss;
    for (auto transitData : in.DrugTransitStates()) {
      SESubstance* sub = substances.GetSubstance(transitData.Substance());
      if (sub == nullptr) {
        ss.str("");
        ss << "Gastrointestinal: Load Transit Model Substance not found : " + transitData.Substance();
        throw CommonDataModelException(ss.str());
      }
      out.NewDrugTransitState(sub);
      out.GetDrugTransitState(sub)->Load(transitData);
    }

    out.m_DecrementNutrients = true;
  }
  void BiogearsPhysiology::Marshall(const Gastrointestinal& in, CDM::BioGearsGastrointestinalSystemData& out)
  {
    io::System::Marshall(in, out);
  }
  // class HepaticSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsHepaticSystemData& in, const SESubstanceManager& substances, Hepatic& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);
    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Hepatic& in, CDM::BioGearsHepaticSystemData& out)
  {
    io::System::Marshall(in, out);
  }
  // class Cardiovascular
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsNervousSystemData& in, const SESubstanceManager& substances, Nervous& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    // We assume state have to be after all stabilization
    out.m_FeedbackActive = true;

    out.m_AfferentChemoreceptor_Hz = in.AfferentChemoreceptor_Hz();
    out.m_AfferentPulmonaryStretchReceptor_Hz = in.AfferentPulmonaryStrechReceptor_Hz();
    out.m_AorticBaroreceptorStrain = in.AorticBaroreceptorStrain();
    out.m_ArterialCarbonDioxideBaseline_mmHg = in.ArterialCarbonDioxideBaseline_mmHg();
    out.m_ArterialOxygenBaseline_mmHg = in.ArterialOxygenBaseline_mmHg();
    out.m_BaroreceptorOperatingPoint_mmHg = in.BaroreceptorOperatingPoint_mmHg();
    out.m_CardiopulmonaryInputBaseline_mmHg = in.CardiopulmonaryInputBaseline_mmHg();
    out.m_CardiopulmonaryInput_mmHg = in.CardiopulmonaryInput_mmHg();
    out.m_CarotidBaroreceptorStrain = in.CarotidBaroreceptorStrain();
    out.m_CentralFrequencyDelta_Per_min = in.CentralFrequencyDelta_Per_min();
    out.m_CentralPressureDelta_cmH2O = in.CentralPressureDelta_cmH2O();
    out.m_CerebralArteriesEffectors_Large.clear();
    for (auto effectorLarge : in.CerebralArteriesEffectors_Large()) {
      out.m_CerebralArteriesEffectors_Large.push_back(effectorLarge);
    }
    out.m_CerebralArteriesEffectors_Small.clear();
    for (auto effectorSmall : in.CerebralArteriesEffectors_Small()) {
      out.m_CerebralArteriesEffectors_Small.push_back(effectorSmall);
    }
    out.m_CerebralBloodFlowBaseline_mL_Per_s = in.CerebralBloodFlowBaseline_mL_Per_s();
    out.m_CerebralBloodFlowInput_mL_Per_s = in.CerebralBloodFlowInput_mL_Per_s();
    out.m_CerebralOxygenSaturationBaseline = in.CerebralOxygenSaturationBaseline();
    out.m_CerebralPerfusionPressureBaseline_mmHg = in.CerebralPerfusionPressureBaseline_mmHg();
    out.m_ChemoreceptorFiringRateSetPoint_Hz = in.ChemoreceptorFiringRateSetPoint_Hz();
    out.m_ComplianceModifier = in.ComplianceModifier();
    out.m_HeartElastanceModifier = in.HeartElastanceModifier();
    out.m_HeartOxygenBaseline = in.HeartOxygenBaseline();
    out.m_HeartRateModifierSympathetic = in.HeartRateModifierSympathetic();
    out.m_HeartRateModifierVagal = in.HeartRateModifierVagal();
    out.m_HypercapniaThresholdHeart = in.HypercapniaThresholdHeart();
    out.m_HypercapniaThresholdPeripheral = in.HypercapniaThresholdPeripheral();
    out.m_HypoxiaThresholdHeart = in.HypoxiaThresholdHeart();
    out.m_HypoxiaThresholdPeripheral = in.HypoxiaThresholdPeripheral();
    out.m_MeanLungVolume_L = in.MeanLungVolume_L();
    out.m_MuscleOxygenBaseline = in.MuscleOxygenBaseline();
    out.m_OxygenAutoregulatorHeart = in.OxygenAutoregulatorHeart();
    out.m_OxygenAutoregulatorMuscle = in.OxygenAutoregulatorMuscle();
    out.m_PeripheralBloodGasInteractionBaseline_Hz = in.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz();
    out.m_PeripheralFrequencyDelta_Per_min = in.PeripheralFrequencyDelta_Per_min();
    out.m_PeripheralPressureDelta_cmH2O = in.PeripheralPressureDelta_cmH2O();
    out.m_ResistanceModifierExtrasplanchnic = in.ResistanceModifierExtrasplanchnic();
    out.m_ResistanceModifierMuscle = in.ResistanceModifierMuscle();
    out.m_ResistanceModifierSplanchnic = in.ResistanceModifierSplanchnic();
    out.m_SympatheticPeripheralSignalBaseline_Hz = in.SympatheticPeripheralSignalBaseline();
    out.m_SympatheticSinoatrialSignalBaseline_Hz = in.SympatheticSinoatrialSignalBaseline();
    out.m_SympatheticPeripheralSignalFatigue = in.SympatheticPeripheralSignalFatigue();
    out.m_VagalSignalBaseline_Hz = in.VagalSignalBaseline();

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Nervous& in, CDM::BioGearsNervousSystemData& out)
  {
    io::System::Marshall(in, out);
    out.AfferentChemoreceptor_Hz(in.m_AfferentChemoreceptor_Hz);
    out.AfferentPulmonaryStrechReceptor_Hz(in.m_AfferentPulmonaryStretchReceptor_Hz);
    out.AorticBaroreceptorStrain(in.m_AorticBaroreceptorStrain);
    out.AttentionLapses(in.m_AttentionLapses);
    out.ArterialCarbonDioxideBaseline_mmHg(in.m_ArterialCarbonDioxideBaseline_mmHg);
    out.ArterialOxygenBaseline_mmHg(in.m_ArterialOxygenBaseline_mmHg);
    out.BaroreceptorOperatingPoint_mmHg(in.m_BaroreceptorOperatingPoint_mmHg);
    out.BiologicalDebt(in.m_BiologicalDebt);
    out.CardiopulmonaryInputBaseline_mmHg(in.m_CardiopulmonaryInputBaseline_mmHg);
    out.CardiopulmonaryInput_mmHg(in.m_CardiopulmonaryInput_mmHg);
    out.CarotidBaroreceptorStrain(in.m_CarotidBaroreceptorStrain);
    out.CentralFrequencyDelta_Per_min(in.m_CentralFrequencyDelta_Per_min);
    out.CentralPressureDelta_cmH2O(in.m_CentralPressureDelta_cmH2O);

    for (auto eLarge : in.m_CerebralArteriesEffectors_Large) {
      out.CerebralArteriesEffectors_Large().push_back(eLarge);
    }
    for (auto eSmall : in.m_CerebralArteriesEffectors_Small) {
      out.CerebralArteriesEffectors_Small().push_back(eSmall);
    }

    out.CerebralBloodFlowBaseline_mL_Per_s(in.m_CerebralBloodFlowBaseline_mL_Per_s);
    out.CerebralBloodFlowInput_mL_Per_s(in.m_CerebralBloodFlowInput_mL_Per_s);
    out.CerebralOxygenSaturationBaseline(in.m_CerebralOxygenSaturationBaseline);
    out.CerebralPerfusionPressureBaseline_mmHg(in.m_CerebralPerfusionPressureBaseline_mmHg);
    out.ChemoreceptorFiringRateSetPoint_Hz(in.m_ChemoreceptorFiringRateSetPoint_Hz);
    out.ChemoreceptorPeripheralBloodGasInteractionBaseline_Hz(in.m_PeripheralBloodGasInteractionBaseline_Hz);
    out.ComplianceModifier(in.m_ComplianceModifier);
    out.HeartElastanceModifier(in.m_HeartElastanceModifier);
    out.HeartOxygenBaseline(in.m_HeartOxygenBaseline);
    out.HeartRateModifierSympathetic(in.m_HeartRateModifierSympathetic);
    out.HeartRateModifierVagal(in.m_HeartRateModifierVagal);
    out.HypercapniaThresholdHeart(in.m_HypercapniaThresholdHeart);
    out.HypercapniaThresholdPeripheral(in.m_HypercapniaThresholdPeripheral);
    out.HypoxiaThresholdHeart(in.m_HypoxiaThresholdHeart);
    out.HypoxiaThresholdPeripheral(in.m_HypoxiaThresholdPeripheral);
    out.MeanLungVolume_L(in.m_MeanLungVolume_L);
    out.MuscleOxygenBaseline(in.m_MuscleOxygenBaseline);
    out.OxygenAutoregulatorHeart(in.m_OxygenAutoregulatorHeart);
    out.OxygenAutoregulatorMuscle(in.m_OxygenAutoregulatorMuscle);
    out.PeripheralFrequencyDelta_Per_min(in.m_PeripheralFrequencyDelta_Per_min);
    out.PeripheralPressureDelta_cmH2O(in.m_PeripheralPressureDelta_cmH2O);
    out.ResistanceModifierExtrasplanchnic(in.m_ResistanceModifierExtrasplanchnic);
    out.ResistanceModifierMuscle(in.m_ResistanceModifierMuscle);
    out.ResistanceModifierSplanchnic(in.m_ResistanceModifierSplanchnic);
    out.SympatheticPeripheralSignalBaseline(in.m_SympatheticPeripheralSignalBaseline_Hz);
    out.SympatheticSinoatrialSignalBaseline(in.m_SympatheticSinoatrialSignalBaseline_Hz);
    out.SympatheticPeripheralSignalFatigue(in.m_SympatheticPeripheralSignalFatigue);
    out.VagalSignalBaseline(in.m_VagalSignalBaseline_Hz);
  }
  // class RenalSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsRenalSystemData& in, const SESubstanceManager& substances, Renal& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    out.m_Urinating = in.Urinating();
    out.m_leftAfferentResistance_mmHg_s_Per_mL = in.LeftAfferentResistance_mmHg_s_Per_mL();
    out.m_rightAfferentResistance_mmHg_s_Per_mL = in.RightAfferentResistance_mmHg_s_Per_mL();
    out.m_leftSodiumFlowSetPoint_mg_Per_s = in.LeftSodiumFlowSetPoint_mg_Per_s();
    out.m_rightSodiumFlowSetPoint_mg_Per_s = in.RightSodiumFlowSetPoint_mg_Per_s();

    io::Property::UnMarshall(in.UrineProductionRate_mL_Per_min(), out.m_urineProductionRate_mL_Per_min_runningAvg);
    io::Property::UnMarshall(in.UrineOsmolarity_mOsm_Per_L(), out.m_urineOsmolarity_mOsm_Per_L_runningAvg);
    io::Property::UnMarshall(in.SodiumConcentration_mg_Per_mL(), out.m_sodiumConcentration_mg_Per_mL_runningAvg);
    io::Property::UnMarshall(in.SodiumExcretionRate_mg_Per_min(), out.m_sodiumExcretionRate_mg_Per_min_runningAvg);
    io::Property::UnMarshall(in.LeftSodiumFlow_mg_Per_s(), out.m_leftSodiumFlow_mg_Per_s_runningAvg);
    io::Property::UnMarshall(in.RightSodiumFlow_mg_Per_s(), out.m_rightSodiumFlow_mg_Per_s_runningAvg);
    io::Property::UnMarshall(in.LeftRenalArterialPressure_mmHg(), out.m_leftRenalArterialPressure_mmHg_runningAvg);
    io::Property::UnMarshall(in.RightRenalArterialPressure_mmHg(), out.m_rightRenalArterialPressure_mmHg_runningAvg);

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Renal& in, CDM::BioGearsRenalSystemData& out)
  {
    io::System::Marshall(in, out);

    out.Urinating(in.m_Urinating);
    out.LeftAfferentResistance_mmHg_s_Per_mL(in.m_leftAfferentResistance_mmHg_s_Per_mL);
    out.RightAfferentResistance_mmHg_s_Per_mL(in.m_rightAfferentResistance_mmHg_s_Per_mL);
    out.LeftSodiumFlowSetPoint_mg_Per_s(in.m_leftSodiumFlowSetPoint_mg_Per_s);
    out.RightSodiumFlowSetPoint_mg_Per_s(in.m_rightSodiumFlowSetPoint_mg_Per_s);

    io::Property::Marshall(in.m_urineProductionRate_mL_Per_min_runningAvg, out.UrineProductionRate_mL_Per_min());
    io::Property::Marshall(in.m_urineOsmolarity_mOsm_Per_L_runningAvg, out.UrineOsmolarity_mOsm_Per_L());
    io::Property::Marshall(in.m_sodiumConcentration_mg_Per_mL_runningAvg, out.SodiumConcentration_mg_Per_mL());
    io::Property::Marshall(in.m_sodiumExcretionRate_mg_Per_min_runningAvg, out.SodiumExcretionRate_mg_Per_min());
    io::Property::Marshall(in.m_leftSodiumFlow_mg_Per_s_runningAvg, out.LeftSodiumFlow_mg_Per_s());
    io::Property::Marshall(in.m_rightSodiumFlow_mg_Per_s_runningAvg, out.RightSodiumFlow_mg_Per_s());
    io::Property::Marshall(in.m_leftRenalArterialPressure_mmHg_runningAvg, out.LeftRenalArterialPressure_mmHg());
    io::Property::Marshall(in.m_rightRenalArterialPressure_mmHg_runningAvg, out.RightRenalArterialPressure_mmHg());
  }
  // class RespiratorySystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsRespiratorySystemData& in, const SESubstanceManager& substances, Respiratory& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    out.m_InitialExpiratoryReserveVolume_L = in.InitialExpiratoryReserveVolume_L();
    out.m_InitialFunctionalResidualCapacity_L = in.InitialFunctionalResidualCapacity_L();
    out.m_InitialInspiratoryCapacity_L = in.InitialInspiratoryCapacity_L();
    out.m_InitialResidualVolume_L = in.InitialResidualVolume_L();

    out.m_bNotBreathing = in.NotBreathing();
    out.m_TopBreathTotalVolume_L = in.TopBreathTotalVolume_L();
    out.m_TopBreathAlveoliVolume_L = in.TopBreathAlveoliVolume_L();
    out.m_TopBreathDeadSpaceVolume_L = in.TopBreathDeadSpaceVolume_L();
    out.m_TopBreathPleuralPressure_cmH2O = in.TopBreathPleuralPressure_cmH2O();
    out.m_LastCardiacCycleBloodPH = in.LastCardiacCycleBloodPH();
    out.m_PreviousTotalLungVolume_L = in.PreviousTotalLungVolume_L();

    io::Property::UnMarshall(in.BloodPHRunningAverage(), out.m_BloodPHRunningAverage);

    out.m_BreathingCycle = in.BreathingCycle();
    out.m_ArterialO2PartialPressure_mmHg = in.ArterialOxygenPressure_mmHg();
    out.m_ArterialCO2PartialPressure_mmHg = in.ArterialCarbonDioxidePressure_mmHg();
    out.m_BreathingCycleTime_s = in.BreathingCycleTime_s();
    out.m_BreathTimeExhale_min = in.BreathTimeExhale_min();
    out.m_DefaultDrivePressure_cmH2O = in.DefaultDrivePressure_cmH2O();
    out.m_DriverPressure_cmH2O = in.DriverPressure_cmH2O();
    out.m_DriverPressureMin_cmH2O = in.DriverPressureMin_cmH2O();
    out.m_ElapsedBreathingCycleTime_min = in.ElapsedBreathingCycleTime_min();
    out.m_IEscaleFactor = in.IEscaleFactor();
    out.m_InstantaneousFunctionalResidualCapacity_L = in.InstantaneousFunctionalResidualCapacity_L();
    out.m_MaxDriverPressure_cmH2O = in.MaxDriverPressure_cmH2O();
    out.m_PeakRespiratoryDrivePressure_cmH2O = in.PeakRespiratoryDrivePressure_cmH2O();

    out.m_VentilationFrequency_Per_min = in.VentilationFrequency_Per_min();

    io::Property::UnMarshall(in.ArterialOxygenAverage_mmHg(), out.m_ArterialO2Average_mmHg);
    io::Property::UnMarshall(in.ArterialCarbonDioxideAverage_mmHg(), out.m_ArterialCO2Average_mmHg);

    out.m_ConsciousBreathing = in.ConsciousBreathing();
    out.m_ConsciousRespirationPeriod_s = in.ConsciousRespirationPeriod_s();
    out.m_ConsciousRespirationRemainingPeriod_s = in.ConsciousRespirationRemainingPeriod_s();
    out.m_ExpiratoryReserveVolumeFraction = in.ExpiratoryReserveVolumeFraction();
    out.m_InspiratoryCapacityFraction = in.InspiratoryCapacityFraction();
    out.m_ConsciousStartPressure_cmH2O = in.ConsciousStartPressure_cmH2O();
    out.m_ConsciousEndPressure_cmH2O = in.ConsciousEndPressure_cmH2O();

    out.m_HadAirwayObstruction = in.HadAirwayObstruction();
    out.m_HadBronchoconstriction = in.HadBronchoconstriction();

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Respiratory& in, CDM::BioGearsRespiratorySystemData& out)
  {
    io::System::Marshall(in, out);

    out.InitialExpiratoryReserveVolume_L(in.m_InitialExpiratoryReserveVolume_L);
    out.InitialFunctionalResidualCapacity_L(in.m_InitialFunctionalResidualCapacity_L);
    out.InitialInspiratoryCapacity_L(in.m_InitialInspiratoryCapacity_L);
    out.InitialResidualVolume_L(in.m_InitialResidualVolume_L);

    out.NotBreathing(in.m_bNotBreathing);
    out.TopBreathTotalVolume_L(in.m_TopBreathTotalVolume_L);
    out.TopBreathAlveoliVolume_L(in.m_TopBreathAlveoliVolume_L);
    out.TopBreathDeadSpaceVolume_L(in.m_TopBreathDeadSpaceVolume_L);
    out.TopBreathPleuralPressure_cmH2O(in.m_TopBreathPleuralPressure_cmH2O);
    out.LastCardiacCycleBloodPH(in.m_LastCardiacCycleBloodPH);
    out.PreviousTotalLungVolume_L(in.m_PreviousTotalLungVolume_L);

    io::Property::Marshall(in.m_BloodPHRunningAverage, out.BloodPHRunningAverage());

    out.BreathingCycle(in.m_BreathingCycle);
    out.ArterialOxygenPressure_mmHg(in.m_ArterialO2PartialPressure_mmHg);
    out.ArterialCarbonDioxidePressure_mmHg(in.m_ArterialCO2PartialPressure_mmHg);
    out.BreathingCycleTime_s(in.m_BreathingCycleTime_s);
    out.BreathTimeExhale_min(in.m_BreathTimeExhale_min);
    out.DefaultDrivePressure_cmH2O(in.m_DefaultDrivePressure_cmH2O);
    out.DriverPressure_cmH2O(in.m_DriverPressure_cmH2O);
    out.DriverPressureMin_cmH2O(in.m_DriverPressureMin_cmH2O);
    out.ElapsedBreathingCycleTime_min(in.m_ElapsedBreathingCycleTime_min);
    out.IEscaleFactor(in.m_IEscaleFactor);
    out.InstantaneousFunctionalResidualCapacity_L(in.m_InstantaneousFunctionalResidualCapacity_L);
    out.MaxDriverPressure_cmH2O(in.m_MaxDriverPressure_cmH2O);
    out.PeakRespiratoryDrivePressure_cmH2O(in.m_PeakRespiratoryDrivePressure_cmH2O);

    out.VentilationFrequency_Per_min(in.m_VentilationFrequency_Per_min);

    io::Property::Marshall(in.m_ArterialO2Average_mmHg, out.ArterialOxygenAverage_mmHg());
    io::Property::Marshall(in.m_ArterialCO2Average_mmHg, out.ArterialCarbonDioxideAverage_mmHg());

    out.ConsciousBreathing(in.m_ConsciousBreathing);
    out.ConsciousRespirationPeriod_s(in.m_ConsciousRespirationPeriod_s);
    out.ConsciousRespirationRemainingPeriod_s(in.m_ConsciousRespirationRemainingPeriod_s);
    out.ExpiratoryReserveVolumeFraction(in.m_ExpiratoryReserveVolumeFraction);
    out.InspiratoryCapacityFraction(in.m_InspiratoryCapacityFraction);
    out.ConsciousStartPressure_cmH2O(in.m_ConsciousStartPressure_cmH2O);
    out.ConsciousEndPressure_cmH2O(in.m_ConsciousEndPressure_cmH2O);

    out.HadAirwayObstruction(in.m_HadAirwayObstruction);
    out.HadBronchoconstriction(in.m_HadBronchoconstriction);
  }
  // class TissueSystem
  void BiogearsPhysiology::UnMarshall(const CDM::BioGearsTissueSystemData& in, const SESubstanceManager& substances, Tissue& out)
  {
    out.Clear();
    io::System::UnMarshall(in, out);

    io::Property::UnMarshall(in.O2ConsumedRunningAverage_mL_Per_s(), out.m_O2ConsumedRunningAverage_mL_Per_s);
    io::Property::UnMarshall(in.CO2ProducedRunningAverage_mL_Per_s(), out.m_CO2ProducedRunningAverage_mL_Per_s);
    io::Property::UnMarshall(in.RespiratoryQuotientRunningAverage(), out.m_RespiratoryQuotientRunningAverage);
    out.m_RestingPatientMass_kg = in.RestingPatientMass_kg();
    out.m_RestingFluidMass_kg = in.RestingFluidMass_kg();
    io::Property::UnMarshall(in.FatigueRunningAverage(), out.m_FatigueRunningAverage);

    out.BioGearsSystem::LoadState();
  }
  void BiogearsPhysiology::Marshall(const Tissue& in, CDM::BioGearsTissueSystemData& out)
  {
    io::System::Marshall(in, out);

    io::Property::Marshall(in.m_O2ConsumedRunningAverage_mL_Per_s,out.O2ConsumedRunningAverage_mL_Per_s());
    io::Property::Marshall(in.m_CO2ProducedRunningAverage_mL_Per_s,out.CO2ProducedRunningAverage_mL_Per_s());
    io::Property::Marshall(in.m_RespiratoryQuotientRunningAverage,out.RespiratoryQuotientRunningAverage());
    out.RestingPatientMass_kg(in.m_RestingPatientMass_kg);
    out.RestingFluidMass_kg(in.m_RestingFluidMass_kg);
    io::Property::Marshall(in.m_FatigueRunningAverage,out.FatigueRunningAverage());
  }
}
}