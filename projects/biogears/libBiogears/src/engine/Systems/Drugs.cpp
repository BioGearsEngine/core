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
#include <biogears/engine/Systems/Drugs.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
Drugs::Drugs(BioGears& bg)
  : SEDrugSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
  //Initialize debug variables for OTFC
  mSolidMouth = 0.0;
  cSaliva = 0.0;
  mSolid2 = 0.0;
  cSaliva2 = 0.0;
  cEpi1_u = 0.0;
  cEpi1 = 0.0;
  cEpi2 = 0.0;
  cEpi3 = 0.0;
  cEpi4 = 0.0;
  cEpi5 = 0.0;
  cEpi6 = 0.0;
  cLamE = 0.0;

  cEpithelium = 0.0;
  cLamina = 0.0;
  mStomach = 0.0;
  cLumen = 0.0;
  cEnterocyte = 0.0;
  cVilli = 0.0;
  totalSwallowed = 0.0;
  totalMetabolized = 0.0;
  totalExcreted = 0.0;
  totalAbsorbed = 0.0;
  massConservation = 0.0;

  mDuodenum = 0.0;
  mJejunum1 = 0.0;
  mJejunum2 = 0.0;
  mIleum1 = 0.0;
  mIleum2 = 0.0;
  mIleum3 = 0.0;
  mCecum = 0.0;
  mColon = 0.0;
  mEnterocyte = 0.0;
  mVilli = 0.0;
}

Drugs::~Drugs()
{
  Clear();
}

void Drugs::Clear()
{
  SEDrugSystem::Clear();
  m_aortaVascular = nullptr;
  m_venaCavaVascular = nullptr;
  m_muscleIntracellular = nullptr;
  m_liverVascular = nullptr;
  m_liverTissue = nullptr;
  m_IVToVenaCava = nullptr;
  m_Sarin = nullptr;
  m_Pralidoxime = nullptr;
  DELETE_MAP_SECOND(m_BolusAdministrations);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Drugs::Initialize()
{
  BioGearsSystem::Initialize();
  GetBronchodilationLevel().SetValue(0.0);
  GetHeartRateChange().SetValue(0.0, FrequencyUnit::Per_min);
  GetMeanBloodPressureChange().SetValue(0.0, PressureUnit::mmHg);
  GetNeuromuscularBlockLevel().SetValue(0.0);
  GetPulsePressureChange().SetValue(0.0, PressureUnit::mmHg);
  GetPupillaryResponse().GetSizeModifier().SetValue(0);
  GetPupillaryResponse().GetReactivityModifier().SetValue(0);
  GetRespirationRateChange().SetValue(0.0, FrequencyUnit::Per_min);
  GetSedationLevel().SetValue(0.0);
  GetTidalVolumeChange().SetValue(0.0, VolumeUnit::mL);
  GetTubularPermeabilityChange().SetValue(0);
  GetCentralNervousResponse().SetValue(0.0);
  m_data.GetBloodChemistry().GetRedBloodCellAcetylcholinesterase().SetValue(8.0 * 1e-9, AmountPerVolumeUnit::mol_Per_L); //Need to initialize here since Drugs processed before BloodChemistry

  m_SarinRbcAcetylcholinesteraseComplex_nM = 0.0;
  m_AgedRbcAcetylcholinesterase_nM = 0.0;
}

bool Drugs::Load(const CDM::BioGearsDrugSystemData& in)
{
  if (!SEDrugSystem::Load(in))
    return false;

  m_SarinRbcAcetylcholinesteraseComplex_nM = in.SarinRbcAcetylcholinesteraseComplex_nM();
  m_AgedRbcAcetylcholinesterase_nM = in.AgedRbcAcetylcholinesterase_nM();

  BioGearsSystem::LoadState();

  for (const CDM::SubstanceBolusStateData& bData : in.BolusAdministration()) {
    SESubstance* sub = m_data.GetSubstances().GetSubstance(bData.Substance());
    if (sub == nullptr) {
      /// \error Error: Unable to find substance for IV bolus administration
      m_ss << "Unable to find substance " << bData.Substance();
      Error(m_ss, "Drugs::Load::BolusAdministration");
      return false;
    }
    SESubstanceBolusState* bolusState = new SESubstanceBolusState(*sub);
    m_BolusAdministrations[sub] = bolusState;
    bolusState->Load(bData);
  }

  for (const CDM::SubstanceOralStateData& oData : in.OralAdministration()) {
    SESubstance* sub = m_data.GetSubstances().GetSubstance(oData.Substance());
    if (sub == nullptr) {
      m_ss << "Unable to find subtance " << oData.Substance();
      Error(m_ss, "Drugs::Load::OralAdministration");
      return false;
    }
    SESubstanceOralState* oralState = new SESubstanceOralState(*sub);
    m_OralAdministrations[sub] = oralState;
    oralState->Load(oData);
  }

  return true;
}
CDM::BioGearsDrugSystemData* Drugs::Unload() const
{
  CDM::BioGearsDrugSystemData* data = new CDM::BioGearsDrugSystemData();
  Unload(*data);
  return data;
}
void Drugs::Unload(CDM::BioGearsDrugSystemData& data) const
{
  SEDrugSystem::Unload(data);
  data.SarinRbcAcetylcholinesteraseComplex_nM(m_SarinRbcAcetylcholinesteraseComplex_nM);
  data.AgedRbcAcetylcholinesterase_nM(m_AgedRbcAcetylcholinesterase_nM);

  for (auto itr : m_BolusAdministrations) {
    if (itr.second != nullptr)
      data.BolusAdministration().push_back(std::unique_ptr<CDM::SubstanceBolusStateData>(itr.second->Unload()));
  }

  for (auto itr : m_OralAdministrations) {
    if (itr.second != nullptr)
      data.OralAdministration().push_back(std::unique_ptr<CDM::SubstanceOralStateData>(itr.second->Unload()));
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes parameters for the Drugs Class
///
///  \details
///  Initializes member variables and system level values on the common data model.
//--------------------------------------------------------------------------------------------------
void Drugs::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_RbcAcetylcholinesteraseFractionInhibited = 0.0;
  m_muscleIntracellular = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::MuscleIntracellular);
  m_aortaVascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  m_venaCavaVascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  m_fatTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Fat);
  m_liverVascular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Liver);
  m_liverTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Liver);
  m_IVToVenaCava = m_data.GetCircuits().GetCardiovascularCircuit().GetPath(BGE::CardiovascularPath::IVToVenaCava);
  //Need to set up pointers for Sarin and Pralidoxime to handle nerve agent events since they use a different method to calculate effects
  m_Sarin = m_data.GetSubstances().GetSubstance("Sarin");
  m_Pralidoxime = m_data.GetSubstances().GetSubstance("Pralidoxime");
  DELETE_MAP_SECOND(m_BolusAdministrations);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines if the engine is stable.
///
/// \details
/// When the engine is stable, the CDM makes this call to update the member variable.
//--------------------------------------------------------------------------------------------------
void Drugs::AtSteadyState()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Substance Preprocess function
///
/// \details
/// The AdminsterSubstances, AdministerIVSubstances, and Hemorrhage actions are processed in this step. The masses
/// of the substances associated with these actions are updated as the actions are processed.
//--------------------------------------------------------------------------------------------------
void Drugs::PreProcess()
{
  AdministerSubstanceBolus();
  AdministerSubstanceOral();
  //**Write debug info to data track here because it is guaranteed to execute
  m_data.GetDataTrack().Probe("MouthDrugMass_ug", mSolidMouth);
  m_data.GetDataTrack().Probe("cSaliva_ug_Per_mL", cSaliva);
  m_data.GetDataTrack().Probe("cEpi1_Unbound_ug_Per_mL", cEpi1_u);
  m_data.GetDataTrack().Probe("cEpi1_ug_Per_mL", cEpi1);
  m_data.GetDataTrack().Probe("cEpi2_ug_Per_mL", cEpi2);
  m_data.GetDataTrack().Probe("cEpi3_ug_Per_mL", cEpi3);
  m_data.GetDataTrack().Probe("cEpi4_ug_Per_mL", cEpi4);
  m_data.GetDataTrack().Probe("cEpi5_ug_Per_mL", cEpi5);
  m_data.GetDataTrack().Probe("cEpi6_ug_Per_mL", cEpi6);
  m_data.GetDataTrack().Probe("cLamE_ug_Per_mL", cLamE);
  m_data.GetDataTrack().Probe("StomachDrugMass_ug", mStomach);
  m_data.GetDataTrack().Probe("MassAbsorbed_ug", totalAbsorbed);

  m_data.GetDataTrack().Probe("MassConservation_ug", massConservation);

  m_data.GetDataTrack().Probe("cTon1_Unbound_ug_Per_mL", cTon1_u);
  m_data.GetDataTrack().Probe("cTon1_ug_Per_mL", cTon1);
  m_data.GetDataTrack().Probe("cTon2_ug_Per_mL", cTon2);
  m_data.GetDataTrack().Probe("cTon3_ug_Per_mL", cTon3);
  m_data.GetDataTrack().Probe("cTon4_ug_Per_mL", cTon4);
  m_data.GetDataTrack().Probe("cTon5_ug_Per_mL", cTon5);
  m_data.GetDataTrack().Probe("cTon6_ug_Per_mL", cTon6);
  m_data.GetDataTrack().Probe("cLamTongue_ug_Per_mL", cLamT);

  m_data.GetDataTrack().Probe("StomachDrugMass_ug", mStomach);
  //m_data.GetDataTrack().Probe("LumenConcentration_ug_Per_mL", cLumen);
  //m_data.GetDataTrack().Probe("EnterocyteConcentration_ug_Per_mL", cEnterocyte);
  //m_data.GetDataTrack().Probe("VilliConcentration_ug_Per_mL", cVilli);
  m_data.GetDataTrack().Probe("MassSwallowed_ug", totalSwallowed);
  m_data.GetDataTrack().Probe("MassExcreted_ug", totalExcreted);
  m_data.GetDataTrack().Probe("MassMetabolized_ug", totalMetabolized);
  m_data.GetDataTrack().Probe("MassAbsorbed_ug", totalAbsorbed);

  m_data.GetDataTrack().Probe("mDuodenum_ug", mDuodenum);
  m_data.GetDataTrack().Probe("mJejunum1_ug", mJejunum1);
  m_data.GetDataTrack().Probe("mJejunum2_ug", mJejunum2);
  m_data.GetDataTrack().Probe("mIleum1_ug", mIleum1);
  m_data.GetDataTrack().Probe("mIleum2_ug", mIleum2);
  m_data.GetDataTrack().Probe("mIleum3_ug", mIleum3);
  m_data.GetDataTrack().Probe("mCecum_ug", mCecum);
  m_data.GetDataTrack().Probe("mColon_ug", mColon);
  m_data.GetDataTrack().Probe("mEnterocyte_ug", mEnterocyte);
  // m_data.GetDataTrack().Probe("VilliConcentration_ug_Per_mL", mVilli);

  m_data.GetDataTrack().Probe("Test_SolidMass_ug", mSolid2);
  m_data.GetDataTrack().Probe("Test_Saliva_ug_Per_mL", cSaliva2);

  AdministerSubstanceInfusion();
  AdministerSubstanceCompoundInfusion();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Substances process function
///
/// \details
/// For any drugs circulating in the system the renal and hepatic clearance factors and metabolic factors
/// are assigned to the appropriate compartments.
/// The drug effects categories and system level drug effects are calculated and the values are set on the common data
/// model for use in other systems. Drug effects are only calculated if the PD is enabled.
//--------------------------------------------------------------------------------------------------
void Drugs::Process()
{
  CalculatePartitionCoefficients();

  CalculateSubstanceClearance();

  CalculatePlasmaSubstanceConcentration();

  if (m_data.GetConfiguration().IsPDEnabled())
    CalculateDrugEffects();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Increments the mass of a substance to represent drug injection
///
/// \details
/// The mass of a substance is increased based on the administration of a substance. For a bolus administration,
/// the user specifies a dose (amount), a concentration, and a substance. The calculated mass is then incrementally
/// added to the vena cava over two seconds.
//--------------------------------------------------------------------------------------------------
void Drugs::AdministerSubstanceBolus()
{
  //Need to loop over Bolus Dose Objects
  const std::map<const SESubstance*, SESubstanceBolus*>& boluses = m_data.GetActions().GetPatientActions().GetSubstanceBoluses();
  if (boluses.empty())
    return;

  std::vector<const SESubstance*> completedBolus;
  SESubstanceBolus* bolus;
  const SESubstance* sub;
  SELiquidSubstanceQuantity* subQ;
  SESubstanceBolusState* bolusState;
  double dose_mL;
  double concentration_ugPermL;
  double massIncrement_ug = 0;
  double administrationTime_s = 2; //administer dose over 2 seconds for a bolus dose

  for (auto b : boluses) {
    sub = b.first;
    bolus = b.second;
    bolusState = m_BolusAdministrations[b.first];
    if (bolusState == nullptr) {
      bolusState = new SESubstanceBolusState(*b.first);
      m_BolusAdministrations[b.first] = bolusState;
    }
    dose_mL = bolus->GetDose().GetValue(VolumeUnit::mL);
    if (bolusState->GetAdministeredDose().GetValue(VolumeUnit::mL) >= dose_mL) {
      // Finished, remove it
      completedBolus.push_back(&bolus->GetSubstance());
      continue;
    }

    switch (bolus->GetAdminRoute()) {
    case CDM::enumBolusAdministration::Intraarterial:
      subQ = m_aortaVascular->GetSubstanceQuantity(*sub);
      break;
    case CDM::enumBolusAdministration::Intravenous:
      subQ = m_venaCavaVascular->GetSubstanceQuantity(*sub);
      break;
    case CDM::enumBolusAdministration::Intramuscular:
      subQ = m_muscleIntracellular->GetSubstanceQuantity(*sub);
      break;
    default:
      /// \error Error: Unavailable Administration Route
      Error(std::string{ "Unavailable Bolus Administration Route for substance " } + b.first->GetName(), "Drugs::AdministerSubstanceBolus");
      completedBolus.push_back(b.first); // Remove it
      continue;
    }

    concentration_ugPermL = bolus->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
    massIncrement_ug = dose_mL * concentration_ugPermL * m_dt_s / administrationTime_s;
    bolusState->GetElapsedTime().IncrementValue(m_dt_s, TimeUnit::s);
    bolusState->GetAdministeredDose().IncrementValue(massIncrement_ug / concentration_ugPermL, VolumeUnit::mL);

    subQ->GetMass().IncrementValue(massIncrement_ug, MassUnit::ug);
    subQ->Balance(BalanceLiquidBy::Mass);
    /// \todo Add fluid amount to fluid system
  }
  // Remove any bolus that are complete
  for (const SESubstance* s : completedBolus) {
    m_data.GetActions().GetPatientActions().RemoveSubstanceBolus(*s);
    delete m_BolusAdministrations[s];
    m_BolusAdministrations[s] = nullptr;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Increments the mass of a substance to represent drug infusion
///
/// \details
/// The mass of a substance is increased based on the administration of a substance.
/// For an infusion, the user specifies a concentration and a rate. The calculated mass
/// is then added to the vena cava every time step until the infusion is stopped.
//--------------------------------------------------------------------------------------------------
void Drugs::AdministerSubstanceInfusion()
{
  //Note:  Currently, user removes state by setting the infusion rate of the drug in question to 0.0
  const std::map<const SESubstance*, SESubstanceInfusion*>& infusions = m_data.GetActions().GetPatientActions().GetSubstanceInfusions();
  if (infusions.empty())
    return;

  SESubstanceInfusion* infusion;
  SESubstance* sub;
  SELiquidSubstanceQuantity* subQ;
  double patientMass_kg = m_data.GetPatient().GetWeight(MassUnit::kg);
  SEScalarTemperature& ambientTemp = m_data.GetEnvironment().GetConditions().GetAmbientTemperature();
  SEScalarMassPerVolume densityFluid;
  GeneralMath::CalculateWaterDensity(ambientTemp, densityFluid);

  double concentration_ug_Per_mL = 0.0;
  double rate_mL_Per_s = 0.0;
  double totalRate_mL_Per_s = 0.0;
  double massIncrement_ug = 0.0;

  for (auto i : infusions) {
    sub = (SESubstance*)i.first; /// \todo sub needs to be const
    infusion = i.second;
    concentration_ug_Per_mL = infusion->GetConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
    rate_mL_Per_s = infusion->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s);
    if (rate_mL_Per_s < 0) {
      std::stringstream InfusionRateError;
      InfusionRateError << "Cannot specify a rate less than 0, setting to a default of 1 mL/min";
      Info(InfusionRateError);
      rate_mL_Per_s = 1.0 / 60.0;
      infusion->GetRate().SetValue(rate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
    }

    if (concentration_ug_Per_mL < 0) {
      std::stringstream InfusionConcentrationError;
      InfusionConcentrationError << "Cannot specify a concentration less than 0, setting to a default of 100 ug/mL";
      Info(InfusionConcentrationError);
      concentration_ug_Per_mL = 100.0;
      infusion->GetConcentration().SetValue(concentration_ug_Per_mL, MassPerVolumeUnit::ug_Per_mL);
    }

    massIncrement_ug = rate_mL_Per_s * concentration_ug_Per_mL * m_dt_s;
    subQ = m_venaCavaVascular->GetSubstanceQuantity(*sub);
    subQ->GetMass().IncrementValue(massIncrement_ug, MassUnit::ug);
    totalRate_mL_Per_s += rate_mL_Per_s;
    patientMass_kg += rate_mL_Per_s * m_dt_s * densityFluid.GetValue(MassPerVolumeUnit::kg_Per_mL);
  }
  m_data.GetPatient().GetWeight().SetValue(patientMass_kg, MassUnit::kg);
  m_IVToVenaCava->GetNextFlowSource().SetValue(totalRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
}

//-------------------------------------------------------------------------------------------------
/// \brief
/// Administer drugs via transmucosal and gastrointestinal routes
///
/// \details
/// This function adminsters drug as lozenges which dissolve in the mouth (transmucosal) or as pills that
/// are swallowed and dissolve in the stomarch (gastrointestinal).  The transmucosal route takes into account
/// dissolved drug that is swallowed and enters circulation through the GI.

void Drugs::AdministerSubstanceOral()
{
  //Need to loop over Bolus Dose Objects
  const std::map<const SESubstance*, SESubstanceOralDose*>& oralDoses = m_data.GetActions().GetPatientActions().GetSubstanceOralDoses();
  if (oralDoses.empty())
    return;

  SESubstanceOralDose* oDose;
  const SESubstance* sub;
  SESubstanceOralState* oState;
  double timeStep_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
  for (auto od : oralDoses) {
    sub = od.first;
    oDose = od.second;
    oState = m_OralAdministrations[sub];
    if (oState == nullptr) {
      oState = new SESubstanceOralState(*sub);
      if (!oState->Initialize(oDose->GetDose(), oDose->GetAdminRoute())) {
        Error("SEOralTransmucosalState::Probable vector length mismatch");
      }
      m_OralAdministrations[sub] = oState;
      //mSolid2 = oDose->GetDose().GetValue(MassUnit::ug);
      mSolidMouth = oDose->GetDose().GetValue(MassUnit::ug);
      mSolid2 = 1.767;
    }

    //A fraction of all orally administered drugs reaches a dissolved state in the stomach.  However, there are two ways that we can get there.
    //1) We can give a lozenge that is absorbed transmucosally--some drug will traverse oral mucosa and enter circulation, but some will be
    //swallowed and enter the stomach in a dissolved state. 2) We can give a pill that enters the stomach as a solid and dissolves there.
    //We therefore have two route-specific paths (Transmucosal and Gastrointestinal) that converge at the dissolved state in the stomach.

    //Concentrations from last time step--set to 0 and then check if they exist because on the first time step after admin so we don't pull a null pointer
    double plasmaConcentration_ug_Per_mL = 0.0;
    double gutVascularConcentration_ug_Per_mL = 0.0;
    if (sub->HasPlasmaConcentration()) {
      plasmaConcentration_ug_Per_mL = sub->GetPlasmaConcentration(MassPerVolumeUnit::ug_Per_mL);
    }
    if (m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine)->HasSubstanceQuantity(*sub)) {
      gutVascularConcentration_ug_Per_mL = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Gut)->GetSubstanceQuantity(*sub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    }

    //Physiochemical constants
    double subLogP = sub->GetPK()->GetPhysicochemicals()->GetLogP();
    double fracUnbound_plasma = sub->GetPK()->GetPhysicochemicals()->GetFractionUnboundInPlasma();
    double subPka = sub->GetPK()->GetPhysicochemicals()->GetAcidDissociationConstant();
    double subBloodPlasmaRatio = sub->GetPK()->GetPhysicochemicals()->GetBloodPlasmaRatio();
    double molarMass_g_Per_mol = sub->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    double subPolarSurfaceArea_A = 23.6; //Add this to sub CDM later if we do more GI subs
    double subHydrogenBondCount = 2.0; //Add this to sub CDM later if we do more GI subs
    double subSolubility_ug_Per_mL = 200.0; //Add this to sub CDM later if we do more tranmucosal subs
    //Defining mass transfer rates to stomach here so that we can accout for both routes if need be
    double rateSwallowedDrugToStomach_ug_Per_s = 0.0; //Transmucosal route
    double rateDrugDissolutionInStomach_ug_Per_s = 0.0; //Gastrointestinal route
    //Reset mass conservation count
    massConservation = 0.0;

    if (oState->IsTransmucosalRoute()) {
      SEOralTransmucosalState& otData = oState->GetTransmucosalSpecificData();
      double solidMassInMouth_ug = otData.GetMouthSolidMass().GetValue(MassUnit::ug);
      if (solidMassInMouth_ug < ZERO_APPROX)
        solidMassInMouth_ug = 0.0;

      double initialMass_ug = od.second->GetDose().GetValue(MassUnit::ug);
      //Characteristic mouth parameters
      double salivaThickness_cm = 0.0085;
      salivaThickness_cm = 30.0e-4;
      double buccalSA_cm2 = 50.0;  //Was 50
      double buccalThickness_cm = 400.0e-4; //um to cm //was 450
      double buccalH_cm = buccalThickness_cm / 6.0;
      double buccalLaminaThickness_cm = buccalH_cm; //This ensures that our mesh points are equally spaced
      double volumeSaliva_mL = 1.0; //Round 1-2 Test = 0.32
      double volumeBuccalSlice_mL = buccalH_cm * buccalSA_cm2; //volume of the Nth epithelial layer
      double volumeBuccalLamina_mL = buccalLaminaThickness_cm * buccalSA_cm2;
      double bloodSupplyBuccal_mL_Per_s = 2.4 / 60.0 * buccalSA_cm2; //From Sattar 2014, buccal blood supply = 2.4 mL/min/cm2
      double tongueSA_cm2 = 15.0;
      double tongueThickness_cm = 125.0e-4; //Was 150e-4
      double tongueH_cm = tongueThickness_cm / 6.0;
      double tongueLaminaThickness_cm = tongueH_cm;
      double volumeTongueSlice_mL = tongueH_cm * tongueSA_cm2;
      double volumeTongueLamina_mL = tongueLaminaThickness_cm * tongueSA_cm2;
      double bloodSupplyTongue_mL_Per_s = 1.0 / 60.0 * tongueSA_cm2;

      //Derived physiochemical data
      double Kp_SalivaToEpithelium = 2.12 * std::exp(0.523 * subLogP);
      Kp_SalivaToEpithelium = 25.0;
      double fracUnbound_tis = 1.0 / Kp_SalivaToEpithelium;
      double Diff_Mucosa_cm2_Per_s;
      if (subLogP < 3.0) {
        double exponent = -0.0803 * (subLogP * subLogP) + 0.5005 * subLogP - 6.7316;
        Diff_Mucosa_cm2_Per_s = std::pow(10.0, exponent);
      } else {
        Diff_Mucosa_cm2_Per_s = std::pow(10.0, -5.9514);
      }
      double Diff_Saliva_cm2_Per_s = 8.0e-6;
      
      //Diff_Mucosa_cm2_Per_s = 3.625e-6;
	  //For test
      double rho_g_Per_mL = 1.2;
      double sol_ug_Per_mL = 200.0;
      double radius_cm = 5.0e-4;

      //Rate constants
      double kSwallow_mL_Per_s = 0.5 / 60; //Was 0.45, Xia 0.36 mL/min saliva production, Sattar has 0.5 mL/min
      double kDis_mL_Per_s_g = 3.0 * Diff_Saliva_cm2_Per_s / (rho_g_Per_mL * radius_cm * salivaThickness_cm);     

      //Values from last time step
      double mouthMass_ug = otData.GetMouthSolidMass().GetValue(MassUnit::ug);
      double salivaConcentration_ug_Per_mL = otData.GetSalivaConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
      std::vector<double> buccalCon = otData.GetBuccalConcentrations(MassPerVolumeUnit::ug_Per_mL);
      std::vector<double> sublingualCon = otData.GetSublingualConcentrations(MassPerVolumeUnit::ug_Per_mL);
      //Differential containers--could update this to Eigen implementation
      std::vector<double> dBuccalMass(buccalCon.size());
      std::vector<double> dSublingualMass(sublingualCon.size());
      //Intermediate expressions -- makes typing out equations easier
      double rateMassDissolutionInMouth_ug_Per_s = (kDis_mL_Per_s_g / 1.0e6) * mouthMass_ug * (sol_ug_Per_mL - salivaConcentration_ug_Per_mL);
      rateSwallowedDrugToStomach_ug_Per_s = kSwallow_mL_Per_s * salivaConcentration_ug_Per_mL;
      double scaleBuccal_Per_s = Diff_Mucosa_cm2_Per_s / (buccalH_cm * buccalH_cm);
      double scaleTongue_Per_s = Diff_Mucosa_cm2_Per_s / (tongueH_cm * tongueH_cm);
      //Figure out how to distribute mass in saliva and unbound in upper layers in buccal and sublingual
      double totalMassSalivaInterface = cSaliva * volumeSaliva_mL + cEpi1_u * volumeBuccalSlice_mL + cTon1_u * volumeTongueSlice_mL;
      //double totalMassSalivaInterface = salivaConcentration_ug_Per_mL * volumeSaliva_mL + fracUnbound_tis * buccalCon[0] * volumeBuccalSlice_mL; // + fracUnbound_tis * sublingualCon[0] * volumeTongueSlice_mL;
      double volumeSum_mL = volumeSaliva_mL + volumeBuccalSlice_mL + volumeTongueSlice_mL;
      double salivaMassNext = totalMassSalivaInterface * (volumeSaliva_mL / volumeSum_mL);
      double buccalMassNext = totalMassSalivaInterface * (volumeBuccalSlice_mL / volumeSum_mL);
      double tongueMassNext = totalMassSalivaInterface * (volumeTongueSlice_mL / volumeSum_mL);
      double deltaMassSaliva = salivaConcentration_ug_Per_mL * volumeSaliva_mL - salivaMassNext;
      double deltaMassBuccal = fracUnbound_tis * buccalCon[0] * volumeBuccalSlice_mL - buccalMassNext;
      double deltaMassTongue = fracUnbound_tis * sublingualCon[0] * volumeTongueSlice_mL - tongueMassNext;

      //Differential Expressions
      double dm_MassMouth = - rateMassDissolutionInMouth_ug_Per_s;
      double dmSaliva = rateMassDissolutionInMouth_ug_Per_s - rateSwallowedDrugToStomach_ug_Per_s;

      //Buccal epithelial layer
      dBuccalMass[0] = -scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[0] - buccalCon[1]) * volumeBuccalSlice_mL;
      dBuccalMass[1] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[0] - 2 * buccalCon[1] + buccalCon[2]) * volumeBuccalSlice_mL;
      dBuccalMass[2] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[1] - 2 * buccalCon[2] + buccalCon[3]) * volumeBuccalSlice_mL;
      dBuccalMass[3] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[2] - 2 * buccalCon[3] + buccalCon[4]) * volumeBuccalSlice_mL;
      dBuccalMass[4] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[3] - 2 * buccalCon[4] + buccalCon[5]) * volumeBuccalSlice_mL;
      dBuccalMass[5] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[4] - 2 * buccalCon[5] + buccalCon[6]) * volumeBuccalSlice_mL;
	  //Buccal Lamina propria layer
      dBuccalMass[6] = scaleBuccal_Per_s * fracUnbound_tis * (buccalCon[5] - buccalCon[6]) * volumeBuccalLamina_mL - bloodSupplyBuccal_mL_Per_s * subBloodPlasmaRatio * (fracUnbound_tis * buccalCon[6] / fracUnbound_plasma - plasmaConcentration_ug_Per_mL);
	  ////Sublingual epithelial layer
      dSublingualMass[0] = -scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[0] - sublingualCon[1]) * volumeTongueSlice_mL;
      dSublingualMass[1] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[0] - 2 * sublingualCon[1] + sublingualCon[2]) * volumeTongueSlice_mL;
      dSublingualMass[2] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[1] - 2 * sublingualCon[2] + sublingualCon[3]) * volumeTongueSlice_mL;
      dSublingualMass[3] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[2] - 2 * sublingualCon[3] + sublingualCon[4]) * volumeTongueSlice_mL;
      dSublingualMass[4] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[3] - 2 * sublingualCon[4] + sublingualCon[5]) * volumeTongueSlice_mL;
      dSublingualMass[5] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[4] - 2 * sublingualCon[5] + sublingualCon[6]) * volumeTongueSlice_mL;
      //Sublingual Lamina propria layer
      dSublingualMass[6] = scaleTongue_Per_s * fracUnbound_tis * (sublingualCon[5] - sublingualCon[6]) * volumeTongueLamina_mL - bloodSupplyTongue_mL_Per_s * subBloodPlasmaRatio * (fracUnbound_tis * sublingualCon[6] / fracUnbound_plasma - plasmaConcentration_ug_Per_mL);
      
	  
	  double massIntoVasculature = bloodSupplyBuccal_mL_Per_s * subBloodPlasmaRatio * (fracUnbound_tis * buccalCon[6] / fracUnbound_plasma - plasmaConcentration_ug_Per_mL) + bloodSupplyTongue_mL_Per_s * subBloodPlasmaRatio * (fracUnbound_tis * sublingualCon[6] / fracUnbound_plasma - plasmaConcentration_ug_Per_mL);
      m_venaCavaVascular->GetSubstanceQuantity(*sub)->GetMass().IncrementValue(massIntoVasculature * timeStep_s, MassUnit::ug);
      totalAbsorbed += massIntoVasculature * timeStep_s;
      //Update states
      otData.GetMouthSolidMass().IncrementValue(dm_MassMouth * timeStep_s, MassUnit::ug);
      otData.GetSalivaConcentration().IncrementValue((1.0 / volumeSaliva_mL) * (dmSaliva * timeStep_s - deltaMassSaliva), MassPerVolumeUnit::ug_Per_mL);
	  buccalCon[0] += (1.0 / volumeBuccalSlice_mL) * (dBuccalMass[0] * timeStep_s - deltaMassBuccal);
      for (size_t pos = 1; pos < dBuccalMass.size(); pos++) {
        buccalCon[pos] += (1.0 / volumeBuccalSlice_mL) * (dBuccalMass[pos] * timeStep_s);
      }
      sublingualCon[0] += (1.0 / volumeTongueSlice_mL) * (dSublingualMass[0] * timeStep_s - deltaMassTongue);
      for (size_t pos = 1; pos < dSublingualMass.size(); pos++) {
        sublingualCon[pos] += (1.0 / volumeTongueSlice_mL) * (dSublingualMass[pos] * timeStep_s);
      }
      otData.SetBuccalConcentrations(buccalCon, MassPerVolumeUnit::ug_Per_mL);
      otData.SetSublingualConcentrations(sublingualCon, MassPerVolumeUnit::ug_Per_mL);

      totalSwallowed += rateSwallowedDrugToStomach_ug_Per_s * timeStep_s;
      mSolidMouth = solidMassInMouth_ug; //Just for tracking
      cSaliva = otData.GetSalivaConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
      //Tracking
      cEpi1 = buccalCon[0], cEpi2 = buccalCon[1], cEpi3 = buccalCon[2], cEpi4 = buccalCon[3], cEpi5 = buccalCon[4], cEpi6 = buccalCon[5], cLamE = buccalCon[6];
      cTon1 = sublingualCon[0], cTon2 = sublingualCon[1], cTon3 = sublingualCon[2], cTon4 = sublingualCon[3], cTon5 = sublingualCon[4], cTon6 = sublingualCon[5], cLamT = sublingualCon[6];
      cEpi1_u = fracUnbound_tis * cEpi1;
      cTon1_u = fracUnbound_tis * cTon1;
      massConservation += mSolidMouth;
      massConservation += cSaliva * volumeSaliva_mL;
      massConservation += volumeBuccalSlice_mL * (cEpi1 + cEpi2 + cEpi3 + cEpi4 + cEpi5 + cEpi6 + cLamE);
      massConservation += volumeTongueSlice_mL * (cTon1 + cTon2 + cTon3 + cTon4 + cTon5 + cTon6 + cLamT);
    }

    if (oState->IsGastrointestinalRoute()) {
      //Nothing here for now
    }
    //Following calculations account for transport from dissolved state in stomach to circulation

    //-----------Good model, testing an improved version.  Preferring to leave this one as-is commented out and copy/paste elements
    //-----------to new one so that if we need to revert it won't be that hard--------------------------------------------------------

    //Previous state variables
    /* double dissolvedMassInStomach_ug = oState->GetStomachDissolvedMass().GetValue(MassUnit::ug);
    std::vector<double> transitMasses = oState->GetGastrointestinalTransitMasses(MassUnit::ug);
    double enterocyteMass_ug = oState->GetEnterocyteMass().GetValue(MassUnit::ug);
    double villiConcentration_ug_Per_mL = oState->GetVilliCapillaryConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL);
    //Differential container
    std::vector<double> dTransitMass(transitMasses.size());
    //Characeteristic stomach/small intestine parameters
    double volumeVilli_mL = 380; //Ando
    double villiBloodFlow_mL_Per_s = 5.0; //Ando
    double radiusLumen_cm = 1.80; //Moxon
    //Permeability constants
    double A = 3.67e-5, B = 3.45e-5, C = -1.04e-7, D = -5.48e-6, E = -2.3e-8, F = 1.46e-4;
    double upperPH = 6.5;
    double middlePH = 7.0;
    double lowerPH = 7.5;
    double upperFi = 1.0 / (1.0 + std::pow(10.0, subPka - upperPH));
    double middleFi = 1.0 / (1.0 + std::pow(10.0, subPka - middlePH));
    double lowerFi = 1.0 / (1.0 + std::pow(10.0, subPka - lowerPH));
    double upperPeff = A * subLogP + B * std::log10(upperFi) + C * molarMass_g_Per_mol + D * subHydrogenBondCount + E * subPolarSurfaceArea_A + F;
    double middlePeff = A * subLogP + B * std::log10(middleFi) + C * molarMass_g_Per_mol + D * subHydrogenBondCount + E * subPolarSurfaceArea_A + F;
    double lowerPeff = A * subLogP + B * std::log10(lowerFi) + C * molarMass_g_Per_mol + D * subHydrogenBondCount + E * subPolarSurfaceArea_A + F;

    //Rate constants
    double kA_upper_Per_s = 2.0 * upperPeff / radiusLumen_cm;
    double kA_middle_Per_s = 2.0 * middlePeff / radiusLumen_cm;
    double kA_lower_Per_s = 2.0 * lowerPeff / radiusLumen_cm;
    double kA_ent_Per_s = kA_lower_Per_s;
    double kStomach_Per_s = 2.03 / 3600.0;
    double kDuodenum_Per_s = 3.85 / 3600.0; //28.75 / 3600.0;
    double kJejunum1_Per_s = 1.05 / 3600.0; //18.07 / 3600.0;
    double kJejunum2_Per_s = 1.31 / 3600.0; //4.21 / 3600.0;
    double kIleum1_Per_s = 1.24 / 3600.0; //1.16 / 3600.0;
    double kIleum2_Per_s = 2.35 / 3600.0; //0.46 / 3600.0;
    double kColon_Per_s = 0.055 / 3600.0; //0.025 * kA_lower_Per_s;
    double kMetabolic_Per_s = 2.0 * kA_ent_Per_s;

	double enterocyteKP = 30.0;  //Approx Fentanyl Gut partition coefficient
    //Intermediate expressions
    double rateMassToSmallIntestine_ug_Per_s = villiBloodFlow_mL_Per_s * (villiConcentration_ug_Per_mL / enterocyteKP);
    LLIM(rateMassToSmallIntestine_ug_Per_s, 0.0);
    //Differential equations
    double dDissolvedMassInStomach_ug_Per_s = rateSwallowedDrugToStomach_ug_Per_s - kStomach_Per_s * dissolvedMassInStomach_ug;
    dTransitMass[0] = kStomach_Per_s * dissolvedMassInStomach_ug - (kDuodenum_Per_s + kA_upper_Per_s) * transitMasses[0];
    dTransitMass[1] = kDuodenum_Per_s * transitMasses[0] - (kJejunum1_Per_s + kA_upper_Per_s) * transitMasses[1];
    dTransitMass[2] = kJejunum1_Per_s * transitMasses[1] - (kJejunum2_Per_s + kA_middle_Per_s) * transitMasses[2];
    dTransitMass[3] = kJejunum2_Per_s * transitMasses[2] - (kIleum1_Per_s + kA_middle_Per_s) * transitMasses[3];
    dTransitMass[4] = kIleum1_Per_s * transitMasses[3] - (kIleum2_Per_s + kA_lower_Per_s) * transitMasses[4];
    dTransitMass[5] = kIleum2_Per_s * transitMasses[4] - (kColon_Per_s + kA_lower_Per_s) * transitMasses[5];
    double dEnteroycyte_ug_Per_s = kA_upper_Per_s * (transitMasses[0] + transitMasses[1]) + kA_middle_Per_s * (transitMasses[2] + transitMasses[3]) + kA_lower_Per_s * (transitMasses[4] + transitMasses[5]) - (kA_ent_Per_s + kMetabolic_Per_s) * enterocyteMass_ug;
    double dVilliMass_ug_Per_s = kA_ent_Per_s * enterocyteMass_ug - rateMassToSmallIntestine_ug_Per_s;

    //Update states
    m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine)->GetSubstanceQuantity(*sub)->GetMass().IncrementValue(rateMassToSmallIntestine_ug_Per_s * timeStep_s, MassUnit::ug);
    oState->GetStomachDissolvedMass().IncrementValue(dDissolvedMassInStomach_ug_Per_s * timeStep_s, MassUnit::ug);
    for (size_t pos = 0; pos < transitMasses.size(); pos++) {
      transitMasses[pos] += timeStep_s * dTransitMass[pos];
    }
    oState->SetGastrointestinalTransitMasses(transitMasses, MassUnit::ug);
    oState->GetEnterocyteMass().IncrementValue(dEnteroycyte_ug_Per_s * timeStep_s, MassUnit::ug);
    oState->GetVilliCapillaryConcentration().IncrementValue((1.0 / volumeVilli_mL) * dVilliMass_ug_Per_s * timeStep_s, MassPerVolumeUnit::ug_Per_mL);

    //Tracking
    mStomach = dissolvedMassInStomach_ug;
    mDuodenum = transitMasses[0], mJejunum1 = transitMasses[1], mJejunum2 = transitMasses[2], mIleum1 = transitMasses[3], mIleum2 = transitMasses[4], mColon = transitMasses[5];
    mVilli = oState->GetVilliCapillaryConcentration().GetValue(MassPerVolumeUnit::ug_Per_mL) * volumeVilli_mL;
    mEnterocyte = oState->GetEnterocyteMass().GetValue(MassUnit::ug);
    totalMetabolized += (mEnterocyte * kMetabolic_Per_s * timeStep_s);
    totalExcreted += (kColon_Per_s * mColon * timeStep_s);
    totalAbsorbed += (rateMassToSmallIntestine_ug_Per_s * timeStep_s);*/

    //------------------------------New--------------------------------------------------
    //Common parameters
    //double cardiacOutput_mL_Per_s = m_data.GetCardiovascular().GetCardiacOutput(VolumePerTimeUnit::mL_Per_s);
    //double fCO = 0.024;
	double villousBloodFlow_mL_Per_s = 5.0;
    double bodyMass_g = m_data.GetPatient().GetWeight(MassUnit::g);
    double fBlood = 1.0 / 8.0;	//Assume blood flow evenly distributed between regions	
    double A = 3.67e-5, B = 3.45e-5, C = -1.04e-7, D = -5.48e-6, E = -2.3e-8, F = 1.46e-4; //Permeability constants
    double constTerms = A * subLogP + C * molarMass_g_Per_mol + D * subHydrogenBondCount + E * subPolarSurfaceArea_A + F; //Only thing that varies is f_unionized
    double fracMetabolized = 0.67;
    double gutKp = 30.0;
    double HrToS = 3600.0;
    double kTransitStomach_Per_s = 1.0 / HrToS;  //2.03 / HrToS;
    // Compartment specific parameters:  Parameters are organized into vectors to reduce space and hopefully avoid redundant code.
    // Each vector entry represents a compartment in the GI transit model, according to the following convention:
    // [0] = Duodenum, [1] = Jejunum1, [2] = Jejunum2, [3] = Ileum1, [4] = Ileum2, [5] = Ileum3, [6] = Cecum, [7] = Colon
    std::vector<double> pHValues{ 6.0, 6.2, 6.4, 6.6, 6.9, 7.4, 6.4, 6.8 }; //pH of each segment
    std::vector<double> siRadii_cm{ 1.6, 1.5, 1.34, 1.18, 1.01, 0.85, 3.5, 2.5 }; //radius of each segment
    std::vector<double> siLength_cm{ 15.0, 62.0, 62.0, 62.0, 62.0, 62.0, 13.75, 57. };
    std::vector<double> siSA_cm2(siRadii_cm.size()); //{ 19995., 77482., 69217., 60952., 52171., 43906., 1964., 2961. }; //surface area of each segment, accounting for villi
    std::vector<double> siVol_mL{ 48., 175., 140., 109., 79., 56., 53., 57. }; //volume of each segment
    std::vector<double> entVolFraction{ 7.7e-4, 1.9e-3, 1.9e-3, 1.4e-3, 1.4e-3, 1.4e-3, 4.0e-4, 8.5e-4 }; //volume of enterocyte as fraction of total body weight (assuming density = 1 g /mL)
    std::vector<double> kTransit_Per_s{ 3.846 / HrToS, 1.053 / HrToS, 1.316 / HrToS, 1.695 / HrToS, 2.326 / HrToS, 3.226 / HrToS, 0.222 / HrToS, 0.074 / HrToS }; //transit constant--size is 1 larger so that we can put this in loop
    std::vector<double> fUnionized; //Fraction unionized in each compartment, depends on pH
    std::vector<double> effP_cm_Per_s; //Effective permeabiliy in each compartment--depends on fUnionized
    for (auto pH : pHValues) {
      double fUn = 1.0 / (1.0 + std::pow(10.0, subPka - pH));
      double kA = constTerms + B * std::log10(fUn);
      fUnionized.push_back(fUn);
      effP_cm_Per_s.push_back(kA);
    }
    for (size_t pos = 0; pos < siRadii_cm.size(); pos++) {
      siSA_cm2[pos] = 2.0 * 3.14159 * siRadii_cm[pos] * siLength_cm[pos];
    }

    //Previous state
    double dissolvedMassInStomach_ug = oState->GetStomachDissolvedMass().GetValue(MassUnit::ug);
    std::vector<double> transitDisMasses_ug = oState->GetTransitDissolvedMasses(MassUnit::ug);
    std::vector<double> entDisMasses_ug = oState->GetEnterocyteDissolvedMasses(MassUnit::ug);
    //Differential containers
    std::vector<double> dTransitDisMass(transitDisMasses_ug.size());
    std::vector<double> dEntDisMass(entDisMasses_ug.size());
    //Double intermediate values
    double entEffluxToPortal_ug_Per_s = 0.0;
    double entInfluxFromPortal_ug_Per_s = 0.0;
    double totalEffluxToPortal_ug_Per_s = 0.0;
    //Derivatives
    double dStomachDis = rateSwallowedDrugToStomach_ug_Per_s - kTransitStomach_Per_s * dissolvedMassInStomach_ug;
    dTransitDisMass[0] = kTransitStomach_Per_s * dissolvedMassInStomach_ug - kTransit_Per_s[0] * transitDisMasses_ug[0] - effP_cm_Per_s[0] * siSA_cm2[0]  * (transitDisMasses_ug[0] / siVol_mL[0] - entDisMasses_ug[0] / (entVolFraction[0] * bodyMass_g));
    for (size_t pos = 1; pos < transitDisMasses_ug.size(); pos++) {
      dTransitDisMass[pos] = kTransit_Per_s[pos - 1] * transitDisMasses_ug[pos - 1] - kTransit_Per_s[pos] * transitDisMasses_ug[pos]
        - effP_cm_Per_s[pos] * siSA_cm2[pos] * (transitDisMasses_ug[pos] / siVol_mL[pos] - entDisMasses_ug[pos] / (entVolFraction[pos] * bodyMass_g));
    }

    for (size_t pos = 0; pos < entDisMasses_ug.size(); pos++) {
      entEffluxToPortal_ug_Per_s = fBlood * villousBloodFlow_mL_Per_s * (entDisMasses_ug[pos] / (entVolFraction[pos] * bodyMass_g) * (1. / gutKp));
      entInfluxFromPortal_ug_Per_s = fBlood * villousBloodFlow_mL_Per_s * gutVascularConcentration_ug_Per_mL;
	  totalEffluxToPortal_ug_Per_s += (entEffluxToPortal_ug_Per_s-entInfluxFromPortal_ug_Per_s);

      dEntDisMass[pos] = effP_cm_Per_s[pos] * siSA_cm2[pos]  * (transitDisMasses_ug[pos] / siVol_mL[pos] - entDisMasses_ug[pos] / (entVolFraction[pos] * bodyMass_g))
        +entInfluxFromPortal_ug_Per_s - entEffluxToPortal_ug_Per_s - fracMetabolized / (1.0 - fracMetabolized) * entEffluxToPortal_ug_Per_s;

      totalMetabolized += fracMetabolized / (1.0 - fracMetabolized) * entEffluxToPortal_ug_Per_s * timeStep_s;
    }
    //Update states
    m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine)->GetSubstanceQuantity(*sub)->GetMass().IncrementValue(totalEffluxToPortal_ug_Per_s * timeStep_s, MassUnit::ug);
    oState->GetStomachDissolvedMass().IncrementValue(dStomachDis * timeStep_s, MassUnit::ug);
    for (size_t pos = 0; pos < transitDisMasses_ug.size(); pos++) {
      transitDisMasses_ug[pos] += timeStep_s * dTransitDisMass[pos];
      entDisMasses_ug[pos] += timeStep_s * dEntDisMass[pos];
    }
    oState->SetTransitDissolvedMasses(transitDisMasses_ug, MassUnit::ug);
    oState->SetEnterocyteDissolvedMasses(entDisMasses_ug, MassUnit::ug);

    //Tracking
    totalAbsorbed += totalEffluxToPortal_ug_Per_s * timeStep_s;
    totalExcreted += transitDisMasses_ug[7] * kTransit_Per_s[7] * timeStep_s; //What's leaving colon
    mStomach = dissolvedMassInStomach_ug;
    mDuodenum = transitDisMasses_ug[0], mJejunum1 = transitDisMasses_ug[1], mJejunum2 = transitDisMasses_ug[2], mIleum1 = transitDisMasses_ug[3], mIleum2 = transitDisMasses_ug[4], mIleum3 = transitDisMasses_ug[5], mCecum = transitDisMasses_ug[6], mColon = transitDisMasses_ug[7];
    mEnterocyte = entDisMasses_ug[0] + entDisMasses_ug[1] + entDisMasses_ug[2] + entDisMasses_ug[3] + entDisMasses_ug[4] + entDisMasses_ug[5] + entDisMasses_ug[6] + entDisMasses_ug[7];
    massConservation += mStomach + mDuodenum + mJejunum1 + mJejunum2 + mIleum1 + mIleum2 + mIleum3 + mCecum + mColon + mEnterocyte;
    massConservation += totalMetabolized;
    massConservation += totalExcreted;
    massConservation += totalAbsorbed;
  }
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Increases the substance masses for compounds
///
/// \details
/// The masses of all substances associated with the compound substance are increased.
/// The mass of each substance is incremented in the vena cava based on the rate specified by the user
/// and a concentration specified in the compound substance file.
//--------------------------------------------------------------------------------------------------
void Drugs::AdministerSubstanceCompoundInfusion()
{
  const std::map<const SESubstanceCompound*, SESubstanceCompoundInfusion*>& infusions = m_data.GetActions().GetPatientActions().GetSubstanceCompoundInfusions();
  if (infusions.empty())
    return;

  SESubstanceCompoundInfusion* infusion;
  const SESubstanceCompound* compound;
  SELiquidSubstanceQuantity* subQ;
  double concentration_ug_Per_mL = 0;
  double rate_mL_Per_s = 0;
  double totalRate_mL_Per_s = 0;
  double massIncrement_ug = 0;
  double volumeRemaining_mL = 0.0;
  double volumeToAdminister_mL = 0.0;
  double patientMass_kg = m_data.GetPatient().GetWeight(MassUnit::kg);
  double densityFluid_kg_Per_mL = 0.0;

  ///\ToDo: Re. Sepsis - How does rate compare to that cited in literature?   ///\@cite malbrain2014fluid, @cite wise2017strategies

  std::vector<const SESubstanceCompound*> emptyBags;

  for (auto i : infusions) {
    compound = i.first;
    infusion = i.second;

    rate_mL_Per_s = infusion->GetRate().GetValue(VolumePerTimeUnit::mL_Per_s);
    totalRate_mL_Per_s += rate_mL_Per_s;
    volumeRemaining_mL = infusion->GetBagVolume().GetValue(VolumeUnit::mL);
    volumeToAdminister_mL = rate_mL_Per_s * m_dt_s;
    if (volumeRemaining_mL < volumeToAdminister_mL) {
      volumeToAdminister_mL = volumeRemaining_mL;
      emptyBags.push_back(compound);
    }
    infusion->GetBagVolume().IncrementValue(-volumeToAdminister_mL, VolumeUnit::mL);

    for (const SESubstanceConcentration* component : compound->GetComponents()) {
      subQ = m_venaCavaVascular->GetSubstanceQuantity(component->GetSubstance());
      double massIncrement_ug = volumeToAdminister_mL * component->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
      subQ->GetMass().IncrementValue(massIncrement_ug, MassUnit::ug);
      subQ->Balance(BalanceLiquidBy::Mass);
    }

    if (compound->GetName() == "Saline" || compound->GetName() == "RingersLactate" || compound->GetName() == "Antibiotic") //Note: Saline and ringers lactate have different densities than pure water
    {
      SEScalarTemperature& ambientTemp = m_data.GetEnvironment().GetConditions().GetAmbientTemperature();
      SEScalarMassPerVolume densityFluid;
      GeneralMath::CalculateWaterDensity(ambientTemp, densityFluid);
      densityFluid_kg_Per_mL = densityFluid.GetValue(MassPerVolumeUnit::kg_Per_mL);
    } else if (compound->GetName() == "Blood")
      densityFluid_kg_Per_mL = m_data.GetBloodChemistry().GetBloodDensity(MassPerVolumeUnit::kg_Per_mL);
    patientMass_kg += volumeToAdminister_mL * densityFluid_kg_Per_mL;
  }

  for (const SESubstanceCompound* c : emptyBags)
    m_data.GetActions().GetPatientActions().RemoveSubstanceCompoundInfusion(*c);

  m_data.GetPatient().GetWeight().SetValue(patientMass_kg, MassUnit::kg);
  m_IVToVenaCava->GetNextFlowSource().SetValue(totalRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate partition coefficients for the PK Model
///
/// \param  vSubstances
/// Coefficients are calculated for the substances in the vector with IonicState.
///
/// \details
/// The partition coefficients are calculated for each substance in the input vector (active substances).
/// To ensure the diffusion is modeled with PK for the substance, a check is performed for Ionic State. If
/// the property is present the coefficients are calculated. They are calculated for all tissue compartments.
/// coefficients are set on the substance compartment effects for use during the diffusion calculation.
//--------------------------------------------------------------------------------------------------
void Drugs::CalculatePartitionCoefficients()
{
  SEBloodChemistrySystem& BloodChemistry = m_data.GetBloodChemistry();
  double IntracellularPH = m_data.GetTissue().GetIntracellularFluidPH().GetValue();
  double PlasmaPH = BloodChemistry.GetArterialBloodPH().GetValue();
  double NeutralLipidInPlasmaVolumeFraction = BloodChemistry.GetVolumeFractionNeutralLipidInPlasma().GetValue();
  double NeutralPhosphoLipidInPlasmaVolumeFraction = BloodChemistry.GetVolumeFractionNeutralPhospholipidInPlasma().GetValue();
  double IntracellularPHEffects = 0;
  double PlasmaPHEffects = 0;
  double PHEffectPower = 0;
  double PHEffects = 0;
  double EquationPartA = 0;
  double EquationPartB = 0;
  double EquationPartC = 0;
  double PartitionCoefficient = 0;
  double TissueToPlasmaProteinRatio = 0;

  //Loop over tissue nodes
  for (SETissueCompartment* tissue : m_data.GetCompartments().GetTissueLeafCompartments()) {
    if (!tissue->HasMatrixVolume()) {
      continue;
    }
    SELiquidCompartment& ExtracellularFluid = m_data.GetCompartments().GetExtracellularFluid(*tissue);
    SELiquidCompartment& IntracellularFluid = m_data.GetCompartments().GetIntracellularFluid(*tissue);

    //Loop over substances
    for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
      if (!sub->HasPK())
        continue;
      if (!sub->GetPK().HasPhysicochemicals())
        continue;

      SESubstancePhysicochemicals& pk = sub->GetPK().GetPhysicochemicals();
      CDM::enumSubstanceIonicState::value IonicState = pk.GetIonicState();
      double AcidDissociationConstant = pk.GetAcidDissociationConstant().GetValue();
      double P = exp(log(10) * pk.GetLogP().GetValue()); //Getting P from logP value
      if (tissue == m_fatTissue) {
        P = 1.115 * pk.GetLogP().GetValue() - 1.35;
        P = exp(log(10) * P);
      }
      if (pk.GetBindingProtein() == CDM::enumSubstanceBindingProtein::AAG) {
        TissueToPlasmaProteinRatio = tissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio().GetValue();
      } else if (pk.GetBindingProtein() == CDM::enumSubstanceBindingProtein::Albumin) {
        TissueToPlasmaProteinRatio = tissue->GetTissueToPlasmaAlbuminRatio().GetValue();
      } else if (pk.GetBindingProtein() == CDM::enumSubstanceBindingProtein::Lipoprotein) {
        TissueToPlasmaProteinRatio = tissue->GetTissueToPlasmaLipoproteinRatio().GetValue();
      } else {
        /// \error Fatal: Binding Protein not supported
        std::stringstream ss;
        ss << "The binding protein is not supported. Unable to compute partition coefficient for: ";
        ss << sub->GetName();
        ss << " in compartment: ";
        ss << tissue->GetName();
        Fatal(ss);
      }
      //Based on the ionic state, the partition coefficient equation and/or pH effect equations are varied.
      if (IonicState == CDM::enumSubstanceIonicState::Base) {
        IntracellularPHEffects = std::pow(10.0, (AcidDissociationConstant - IntracellularPH));
        PHEffectPower = PlasmaPH - AcidDissociationConstant;
        PlasmaPHEffects = 1.0 + std::pow(10.0, PHEffectPower);
        EquationPartA = 1.0 + IntracellularPHEffects * IntracellularFluid.GetWaterVolumeFraction().GetValue() / PlasmaPHEffects;
        /// \todo How to support oral absorption - should I check if oral administration then use Oral absorption rate otherwise assume 1?
        EquationPartB = tissue->GetAcidicPhospohlipidConcentration().GetValue(MassPerMassUnit::mg_Per_g) * IntracellularPHEffects / PlasmaPHEffects;
        EquationPartC = P * tissue->GetNeutralLipidsVolumeFraction().GetValue() + (0.3 * P + 0.7) * tissue->GetNeutralPhospholipidsVolumeFraction().GetValue() / PlasmaPHEffects;
      } else {
        if (IonicState == CDM::enumSubstanceIonicState::Acid) {
          PHEffectPower = IntracellularPH - AcidDissociationConstant;
          IntracellularPHEffects = 1.0 + std::pow(10.0, PHEffectPower);
          PHEffectPower = PlasmaPH - AcidDissociationConstant;
          PlasmaPHEffects = 1.0 + std::pow(10.0, PHEffectPower);
        } else if (IonicState == CDM::enumSubstanceIonicState::WeakBase) {
          PHEffectPower = AcidDissociationConstant - IntracellularPH;
          IntracellularPHEffects = 1.0 + std::pow(10.0, PHEffectPower);
          PHEffectPower = AcidDissociationConstant - PlasmaPH;
          PlasmaPHEffects = 1.0 + std::pow(10.0, PHEffectPower);
        } else {
          IntracellularPHEffects = 1.0;
          PlasmaPHEffects = 1.0;
        }
        EquationPartA = IntracellularPHEffects * IntracellularFluid.GetWaterVolumeFraction().GetValue() / PlasmaPHEffects;
        EquationPartB = (P * tissue->GetNeutralLipidsVolumeFraction().GetValue() + (0.3 * P + 0.7) * tissue->GetNeutralPhospholipidsVolumeFraction().GetValue())
          / PlasmaPHEffects;
        EquationPartC = ((1 / pk.GetFractionUnboundInPlasma().GetValue()) - 1.0 - ((P * NeutralLipidInPlasmaVolumeFraction + (0.3 * P + 0.7) * NeutralPhosphoLipidInPlasmaVolumeFraction) / PlasmaPHEffects)) * TissueToPlasmaProteinRatio;
      }
      //Calculate the partition coefficient and set it on the substance compartment effects
      PartitionCoefficient = EquationPartA + ExtracellularFluid.GetWaterVolumeFraction().GetValue() + EquationPartB + EquationPartC;
      PartitionCoefficient = PartitionCoefficient * pk.GetFractionUnboundInPlasma().GetValue() / pk.GetBloodPlasmaRatio().GetValue();
      sub->GetPK().GetTissueKinetics(tissue->GetName()).GetPartitionCoefficient().SetValue(PartitionCoefficient);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the drug effects on other system parameters
///
/// \details
/// If the substance is a drug with an EC50 value, the effects on heart rate, blood pressure, respiration rate,
/// tidal volume, neuromuscular block level, sedation level, bronchodilation level, and pupillary state are
/// calculated using the current plasma concentration, the EC50, and the maximum drug response.
//--------------------------------------------------------------------------------------------------
void Drugs::CalculateDrugEffects()
{

  double deltaHeartRate_Per_min = 0;
  double deltaDiastolicBP_mmHg = 0;
  double deltaSystolicBP_mmHg = 0;
  double deltaRespirationRate_Per_min = 0;
  double deltaTidalVolume_mL = 0;
  double neuromuscularBlockLevel = 0;
  double sedationLevel = 0;
  double bronchodilationLevel = 0;
  double concentrationEffects_unitless = 0;
  double deltaTubularPermeability = 0.0;
  double pupilSizeResponseLevel = 0;
  double pupilReactivityResponseLevel = 0;
  double shapeParameter = 1.;
  double ec50_ug_Per_mL = 0.0;
  SEPatient& patient = m_data.GetPatient();
  double HRBaseline_per_min = patient.GetHeartRateBaseline(FrequencyUnit::Per_min);
  double effectSiteConcentration_ug_Per_mL = 0.0;
  double centralNervousResponseLevel = 0.0;

  //Naloxone reversal
  SESubstance* m_Naloxone = m_data.GetSubstances().GetSubstance("Naloxone");
  double inhibitorConcentration_ug_Per_mL = 0.0;
  double inhibitorConstant_ug_Per_mL = 1.0; //Can't initialize to 0 lest we divide by 0.  Won't matter what it is when there is no inhibitor because this will get mulitplied by 0 anyway

  //Loop over substances
  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
    if (!sub->HasPD())
      continue;

    SESubstancePharmacodynamics& pd = sub->GetPD();
    shapeParameter = pd.GetEMaxShapeParameter().GetValue();
    ec50_ug_Per_mL = pd.GetEC50().GetValue(MassPerVolumeUnit::ug_Per_mL);

    //Get effect site concentration and use it to calculate unitless drug effects.
    //Currently, effect site concentration is same as plasma concentration for all drugs except morphine and sarin
    effectSiteConcentration_ug_Per_mL = sub->GetEffectSiteConcentration(MassPerVolumeUnit::ug_Per_mL);

    if (sub->GetClassification() == CDM::enumSubstanceClass::Opioid) {
      if (m_data.GetSubstances().IsActive(*m_Naloxone)) {
        inhibitorConstant_ug_Per_mL = m_Naloxone->GetPD().GetEC50().GetValue(MassPerVolumeUnit::ug_Per_mL);
        inhibitorConcentration_ug_Per_mL = m_Naloxone->GetEffectSiteConcentration(MassPerVolumeUnit::ug_Per_mL);
      }
      concentrationEffects_unitless = std::pow(effectSiteConcentration_ug_Per_mL, shapeParameter) / (std::pow(ec50_ug_Per_mL, shapeParameter) * std::pow(1 + inhibitorConcentration_ug_Per_mL / inhibitorConstant_ug_Per_mL, shapeParameter) + std::pow(effectSiteConcentration_ug_Per_mL, shapeParameter));
    } else if (sub->GetName() == "Sarin") {
      concentrationEffects_unitless = m_RbcAcetylcholinesteraseFractionInhibited;
    } else {
      if (shapeParameter == 1) // Avoiding using pow if we don't have to. I don't know if this is good practice or not, but seems legit.
      {
        concentrationEffects_unitless = effectSiteConcentration_ug_Per_mL / (ec50_ug_Per_mL + effectSiteConcentration_ug_Per_mL);

      } else {
        concentrationEffects_unitless = std::pow(effectSiteConcentration_ug_Per_mL, shapeParameter) / (std::pow(ec50_ug_Per_mL, shapeParameter) + std::pow(effectSiteConcentration_ug_Per_mL, shapeParameter));
      }
    }

    if (m_data.GetActions().GetPatientActions().HasOverride()
        && m_data.GetActions().GetPatientActions().GetOverride()->GetOverrideConformance() == CDM::enumOnOff::Off) {
      if (m_data.GetActions().GetPatientActions().GetOverride()->HasMAPOverride()) {
        pd.GetDiastolicPressureModifier().SetValue(0.0);
        pd.GetSystolicPressureModifier().SetValue(0.0);
      }
      if (m_data.GetActions().GetPatientActions().GetOverride()->HasHeartRateOverride()) {
        pd.GetHeartRateModifier().SetValue(0.0);
      }
    }

    /// \todo The drug effect is being applied to the baseline, so if the baseline changes the delta heart rate changes.
    // This would be a problem for something like a continuous infusion of a drug or an environmental drug
    // where we need to establish a new homeostatic point. Once the patient stabilizes with the drug effect included, a new baseline is
    // set, and suddenly the drug effect is being computed using the new baseline. We may need to add another layer of
    // stabilization and restrict drugs to post-feedback stabilization. Alternatively, we could base the drug effect on a baseline
    // concentration which is normally zero but which gets set to a new baseline concentration at the end of feedback (see chemoreceptor
    // and the blood gas setpoint reset for example).
    deltaHeartRate_Per_min += HRBaseline_per_min * pd.GetHeartRateModifier().GetValue() * concentrationEffects_unitless;

    deltaDiastolicBP_mmHg += patient.GetDiastolicArterialPressureBaseline(PressureUnit::mmHg) * pd.GetDiastolicPressureModifier().GetValue() * concentrationEffects_unitless;

    deltaSystolicBP_mmHg += patient.GetSystolicArterialPressureBaseline(PressureUnit::mmHg) * pd.GetSystolicPressureModifier().GetValue() * concentrationEffects_unitless;

    sedationLevel += pd.GetSedation().GetValue() * concentrationEffects_unitless;
    centralNervousResponseLevel += pd.GetCentralNervousModifier().GetValue() * concentrationEffects_unitless;

    deltaTubularPermeability += (pd.GetTubularPermeabilityModifier().GetValue()) * concentrationEffects_unitless;

    if (sedationLevel > 0.15) {
      deltaRespirationRate_Per_min += patient.GetRespirationRateBaseline(FrequencyUnit::Per_min) * pd.GetRespirationRateModifier().GetValue();
      deltaTidalVolume_mL += patient.GetTidalVolumeBaseline(VolumeUnit::mL) * pd.GetTidalVolumeModifier().GetValue();
    } else {
      deltaRespirationRate_Per_min += patient.GetRespirationRateBaseline(FrequencyUnit::Per_min) * pd.GetRespirationRateModifier().GetValue() * concentrationEffects_unitless;
      deltaTidalVolume_mL += patient.GetTidalVolumeBaseline(VolumeUnit::mL) * pd.GetTidalVolumeModifier().GetValue() * concentrationEffects_unitless;
    }

    neuromuscularBlockLevel += pd.GetNeuromuscularBlock().GetValue() * concentrationEffects_unitless;

    bronchodilationLevel += pd.GetBronchodilation().GetValue() * concentrationEffects_unitless;

    const SEPupillaryResponse& pupillaryResponse = pd.GetPupillaryResponse();
    pupilSizeResponseLevel += pupillaryResponse.GetSizeModifier() * concentrationEffects_unitless;
    pupilReactivityResponseLevel += pupillaryResponse.GetReactivityModifier() * concentrationEffects_unitless;
  }

  //Sepsis Effects
  if (m_data.GetActions().GetPatientActions().HasSepsis()) {
    double nitricOxideBaseline = 0.05;
    double nitricOxide = m_data.GetBloodChemistry().GetAcuteInflammatoryResponse().GetNitricOxide().GetValue() - nitricOxideBaseline;
    LLIM(nitricOxide, 0.0);
    double nitricOxideEC50 = 0.4;
    double nitricOxideBPMod = -0.3;
    double nitricOxideHRMod = 0.3;
    double nitricOxideBPChange = nitricOxideBPMod * std::pow(nitricOxide, 2.0) / (std::pow(nitricOxide, 2.0) + std::pow(nitricOxideEC50, 2.0));
    double nitricOxideHRChange = nitricOxideHRMod * std::pow(nitricOxide, 2.0) / (std::pow(nitricOxide, 2.0) + std::pow(nitricOxideEC50, 2.0));
    deltaHeartRate_Per_min += nitricOxideHRChange * m_data.GetPatient().GetHeartRateBaseline(FrequencyUnit::Per_min);
    deltaSystolicBP_mmHg += nitricOxideBPChange * m_data.GetPatient().GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
    deltaDiastolicBP_mmHg += nitricOxideBPChange * m_data.GetPatient().GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
  }

  //Translate Diastolic and Systolic Pressure to pulse pressure and mean pressure
  double deltaMeanPressure_mmHg = (2 * deltaDiastolicBP_mmHg + deltaSystolicBP_mmHg) / 3;
  double deltaPulsePressure_mmHg = (deltaSystolicBP_mmHg - deltaDiastolicBP_mmHg);

  //Set values on the CDM System Values
  GetHeartRateChange().SetValue(deltaHeartRate_Per_min, FrequencyUnit::Per_min);
  GetMeanBloodPressureChange().SetValue(deltaMeanPressure_mmHg, PressureUnit::mmHg);
  GetPulsePressureChange().SetValue(deltaPulsePressure_mmHg, PressureUnit::mmHg);
  GetRespirationRateChange().SetValue(deltaRespirationRate_Per_min, FrequencyUnit::Per_min);
  GetTidalVolumeChange().SetValue(deltaTidalVolume_mL, VolumeUnit::mL);
  GetNeuromuscularBlockLevel().SetValue(neuromuscularBlockLevel);
  GetSedationLevel().SetValue(sedationLevel);
  GetBronchodilationLevel().SetValue(bronchodilationLevel);
  GetTubularPermeabilityChange().SetValue(deltaTubularPermeability);
  GetCentralNervousResponse().SetValue(centralNervousResponseLevel);

  //Pupil effects

  //We need to handle Sarin pupil effects (if Sarin is active) separately because technically they stem from contact and not systemic levels, meaning that they
  //do not depend on the Sarin plasma concentration in the same way as other PD effects.  We still perform the calculation here because
  //we cannot "contact" the eye, but scale them differently.  Sarin pupil effects are large and fast, so it's reasonable to
  //overwrite other drug pupil effects (and we probably aren't modeling opioid addicts inhaling Sarin)
  if (m_data.GetSubstances().IsActive(*m_data.GetSubstances().GetSubstance("Sarin"))) {
    pupilSizeResponseLevel = GeneralMath::LogisticFunction(-1, 0.0475, 250, m_data.GetSubstances().GetSubstance("Sarin")->GetPlasmaConcentration(MassPerVolumeUnit::ug_Per_L));
    pupilReactivityResponseLevel = pupilSizeResponseLevel;
  }

  //Bound pupil modifiers
  BLIM(pupilSizeResponseLevel, -1, 1);
  BLIM(pupilReactivityResponseLevel, -1, 1);
  GetPupillaryResponse().GetSizeModifier().SetValue(pupilSizeResponseLevel);
  GetPupillaryResponse().GetReactivityModifier().SetValue(pupilReactivityResponseLevel);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the concentration of a substance in the plasma
///
/// \details
/// If the substance has PK properties, the concentration of the substance in the plasma is calculated.
/// PlasmaConcentration = TotalMassInTheBlood / PlasmaVolume.
/// The plasma concentration is then set on the substance.
/// The concentration computation is obviously erroneous. This is a known issue. See @ref pharmacokinetics
//--------------------------------------------------------------------------------------------------
void Drugs::CalculatePlasmaSubstanceConcentration()
{
  double PlasmaMass_ug = 0;
  double effectConcentration;
  double PlasmaVolume_mL = m_data.GetBloodChemistry().GetPlasmaVolume(VolumeUnit::mL);
  double rate_Per_s = 0.0;

  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {

    PlasmaMass_ug = m_data.GetSubstances().GetSubstanceMass(*sub, m_data.GetCompartments().GetVascularLeafCompartments(), MassUnit::ug);
    sub->GetPlasmaConcentration().SetValue(PlasmaMass_ug / PlasmaVolume_mL, MassPerVolumeUnit::ug_Per_mL);

    //Get substance PD Data if it exists, including rate constant describing transfer to effect compartment and previous effect site concentration
    if (sub->HasPD()) {
      SESubstancePharmacodynamics& pd = sub->GetPD();
      rate_Per_s = pd.GetEffectSiteRateConstant(FrequencyUnit::Per_s);
      if (!sub->HasEffectSiteConcentration()) {
        effectConcentration = 0.0;
        sub->GetEffectSiteConcentration().SetValue(effectConcentration, MassPerVolumeUnit::ug_Per_mL);
      }

      effectConcentration = sub->GetEffectSiteConcentration(MassPerVolumeUnit::ug_Per_mL);

      //If a substance has rate constant set to 0, no effect concentration is needed.  Just use plasma concentration as before
      if (rate_Per_s == 0) {
        effectConcentration = sub->GetPlasmaConcentration(MassPerVolumeUnit::ug_Per_mL);
      } else {
        //For drugs with effect site rate constant, use first order difference equation to calculate transfer at next time step
        effectConcentration += (pd.GetEffectSiteRateConstant(FrequencyUnit::Per_s)) * m_dt_s * (sub->GetPlasmaConcentration(MassPerVolumeUnit::ug_Per_mL) - effectConcentration);
      }
      //Store effect site concentration for use in CalculateDrugEffects function
      sub->GetEffectSiteConcentration().SetValue(effectConcentration, MassPerVolumeUnit::ug_Per_mL);
    }

    if (sub->GetName() == "Sarin" && (m_data.GetSubstances().IsActive(*m_Sarin)))
      SarinKinetics();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the substance mass cleared
///
/// \details
/// If the substance has PK properties, the substance clearance is calculated. The renal, liver, and systemic
/// clearance terms are applied to each anatomy compartment per substance. The amount of mass is then removed from the
/// tissue node each time step. The tissue node mass and concentration are then updated. Sets the amount of substance
/// cleared as a compartment effect for each substance.
//--------------------------------------------------------------------------------------------------
void Drugs::CalculateSubstanceClearance()
{
  double PatientWeight_kg = m_data.GetPatient().GetWeight(MassUnit::kg);
  double HepaticClearance_mLPers = 0;
  double FractionUnboundInPlasma = 0;
  double IntrinsicClearance_mLPersPerkg = 0;
  double TotalVolumeCleared_mL = 0;
  double RenalVolumeCleared_mL = 0;
  double HepaticVolumeCleared_mL = 0;
  double OtherSystemicVolumeCleared_mL = 0;

  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
    if (!sub->HasClearance() || !sub->GetClearance().HasSystemic())
      continue;
    SESubstanceClearance& clearance = sub->GetClearance();

    //Renal Volume Cleared - Clearance happens through the renal system
    RenalVolumeCleared_mL = (clearance.GetRenalClearance().GetValue(VolumePerTimeMassUnit::mL_Per_s_kg) * PatientWeight_kg * m_dt_s);

    //Intrinsic Clearance
    IntrinsicClearance_mLPersPerkg = clearance.GetIntrinsicClearance().GetValue(VolumePerTimeMassUnit::mL_Per_s_kg);
    FractionUnboundInPlasma = clearance.GetFractionUnboundInPlasma().GetValue();
    //Hepatic Clearance
    double LiverVascularFlow_mL_Per_s = m_liverVascular->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
    HepaticClearance_mLPers = (LiverVascularFlow_mL_Per_s * FractionUnboundInPlasma * IntrinsicClearance_mLPersPerkg * PatientWeight_kg) / (LiverVascularFlow_mL_Per_s + (FractionUnboundInPlasma * IntrinsicClearance_mLPersPerkg * PatientWeight_kg));
    HepaticVolumeCleared_mL = HepaticClearance_mLPers * m_dt_s;

    //Systemic Clearance
    TotalVolumeCleared_mL = clearance.GetSystemicClearance().GetValue(VolumePerTimeMassUnit::mL_Per_s_kg) * PatientWeight_kg * m_dt_s;
    OtherSystemicVolumeCleared_mL = TotalVolumeCleared_mL - RenalVolumeCleared_mL - HepaticVolumeCleared_mL;
    // The following makes it so that you cannot have more than the total systemic clearance for the substance.
    LLIM(OtherSystemicVolumeCleared_mL, 0.);

    //Hepatic Clearance
    m_data.GetSubstances().CalculateGenericClearance(HepaticVolumeCleared_mL, *m_liverTissue, *sub);

    //Systemic Clearance
    m_data.GetSubstances().CalculateGenericClearance(OtherSystemicVolumeCleared_mL, *m_venaCavaVascular, *sub);

    //Hepatic Excretion
    m_data.GetSubstances().CalculateGenericExcretion(LiverVascularFlow_mL_Per_s, *m_liverTissue, *sub, clearance.GetFractionExcretedInFeces().GetValue(), m_dt_s);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the inhibition of erythrocyte bound acetylcholinesterase by the nerve agent Sarin using reaction kinetics
///
/// \details
/// Unlike other drugs in BioGears, the PD effects of Sarin are not based directly on agent plasma concentration.  Instead, we model
/// Sarin binding to red blood cell acetylcholinesterase (Rbc-Ache) and link the inhibition of this enzyme to Sarin pharmacodynamics.
/// This calculation utilizies a kinetic model that incorporates irreversible aging of Rbc-Ache and the salvaging effect of Pralidoxime.
/// As such, this strategy represents a more mechanistic, receptor-target approach to pharmacodynamics
//--------------------------------------------------------------------------------------------------
void Drugs::SarinKinetics()
{
  //Get Sarin and Pralidoxime (if active) concentrations and convert units to nM (units that all the rate equations are in)
  double SarinConcentration_g_Per_L = m_Sarin->GetPlasmaConcentration(MassPerVolumeUnit::g_Per_L);
  double SarinMolarMass_g_Per_umol = m_Sarin->GetMolarMass(MassPerAmountUnit::g_Per_umol);
  double SarinConcentration_nM = SarinConcentration_g_Per_L / SarinMolarMass_g_Per_umol * 1000;
  double PralidoximeConcentration_nM = 0.0;
  double PralidoximeMolarMass_g_Per_umol = m_Pralidoxime->GetMolarMass(MassPerAmountUnit::g_Per_umol);
  double PralidoximeConcentration_g_Per_L = 0.0;
  if ((m_data.GetSubstances().IsActive(*m_Pralidoxime)) && (m_Pralidoxime->HasPlasmaConcentration())) //Substance can get flagged active w/ null plasma concentration so need to check both for stabilization step
  {
    PralidoximeConcentration_g_Per_L = m_Pralidoxime->GetPlasmaConcentration(MassPerVolumeUnit::g_Per_L);
    PralidoximeConcentration_nM = PralidoximeConcentration_g_Per_L / PralidoximeMolarMass_g_Per_umol * 1000;
  }

  //Get RBC-AChE concentration and create copies of all concentrations so that they are not overwritten in the rate equations
  double RbcAcetylcholinesterase_nM = m_data.GetBloodChemistry().GetRedBloodCellAcetylcholinesterase(AmountPerVolumeUnit::mol_Per_L) * 1e9;
  double RbcAche_nM = RbcAcetylcholinesterase_nM;
  double SarinRbcAche_nM = m_SarinRbcAcetylcholinesteraseComplex_nM;
  double AgedSarin_nM = m_AgedRbcAcetylcholinesterase_nM;
  double BaselineRbcAcetylcholinesterase_nM = 8.0;

  //Rate constants
  double RateRbcAcheInhibition_per_nM_s = 4.50e-4; /// \cite gupta2009handbook \cite rodriguez2015model
  double RateRbcAcheAging_per_s = 4.83e-5; /// \cite gupta2009handbook \cite rodriguez2015model
  double RateRbcAcheSynthesis_nM_per_s = 9.33e-7; /// \cite grob1958effects
  double RateRbcAcheDegredation_per_s = 1.17e-7; /// \cite rodriguez2015model
  double RatePralidoximeReversal_per_s = 4.22e-3; /// \cite rodriguez2015model
  double PralidoximeDissociationConstant_nM = 27630.0; /// \cite rodriguez2015model

  //Michaelis-Menten expression for rate of pralidoxime interaction with bound sarin/rbc-ache complex
  double PralidoximeReversal = RatePralidoximeReversal_per_s * m_SarinRbcAcetylcholinesteraseComplex_nM * PralidoximeConcentration_nM / (PralidoximeDissociationConstant_nM + PralidoximeConcentration_nM); ///\cite rodriguez2015model

  //Kinetic model equations adapted from
  ///\cite rodriguez2015model and \cite gupta2009handbook
  RbcAche_nM += m_dt_s * (-RateRbcAcheInhibition_per_nM_s * SarinConcentration_nM * RbcAcetylcholinesterase_nM - RateRbcAcheDegredation_per_s * RbcAcetylcholinesterase_nM + RateRbcAcheSynthesis_nM_per_s + PralidoximeReversal);
  SarinRbcAche_nM += m_dt_s * (RateRbcAcheInhibition_per_nM_s * SarinConcentration_nM * RbcAcetylcholinesterase_nM - RateRbcAcheAging_per_s * m_SarinRbcAcetylcholinesteraseComplex_nM - PralidoximeReversal);
  AgedSarin_nM += m_dt_s * (RateRbcAcheAging_per_s * m_SarinRbcAcetylcholinesteraseComplex_nM);
  SarinConcentration_nM += m_dt_s * (-RateRbcAcheInhibition_per_nM_s * SarinConcentration_nM * RbcAcetylcholinesterase_nM);
  PralidoximeConcentration_nM = -PralidoximeReversal;
  m_RbcAcetylcholinesteraseFractionInhibited = 1 - RbcAche_nM / BaselineRbcAcetylcholinesterase_nM;

  //Update values for next time step
  m_Sarin->GetPlasmaConcentration().SetValue(SarinConcentration_nM / 1000.0 * SarinMolarMass_g_Per_umol, MassPerVolumeUnit::g_Per_L);
  m_data.GetBloodChemistry().GetRedBloodCellAcetylcholinesterase().SetValue(RbcAche_nM * 1e-9, AmountPerVolumeUnit::mol_Per_L);
  m_SarinRbcAcetylcholinesteraseComplex_nM = SarinRbcAche_nM;
  m_AgedRbcAcetylcholinesterase_nM = AgedSarin_nM;

  if (m_data.GetSubstances().IsActive(*m_Pralidoxime))
    m_Pralidoxime->GetPlasmaConcentration().SetValue(PralidoximeConcentration_nM / 1000 * PralidoximeMolarMass_g_Per_umol, MassPerVolumeUnit::g_Per_L);
}
}