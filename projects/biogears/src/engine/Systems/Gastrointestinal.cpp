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
#include <biogears/engine/Systems/Gastrointestinal.h>

#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/patient/SENutrition.h>
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
namespace BGE = mil::tatrc::physiology::biogears;

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)

//#define logMeal

Gastrointestinal::Gastrointestinal(BioGears& bg)
  : SEGastrointestinalSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
  /* Move to a unit test
  SENutrition one(m_Logger);
  SENutrition two(m_Logger);
  one.GetCarbohydrate()->SetValue(1, MassUnit::g);
  one.GetCarbohydrateDigestionRate()->SetValue(1, MassPerTimeUnit::g_Per_s);
  one.GetFat()->SetValue(2, MassUnit::g);
  one.GetFatDigestionRate()->SetValue(2, MassPerTimeUnit::g_Per_s);
  one.GetProtein()->SetValue(3, MassUnit::g);
  one.GetProteinDigestionRate()->SetValue(3, MassPerTimeUnit::g_Per_s);

  two.GetCarbohydrate()->SetValue(1, MassUnit::g);
  two.GetCarbohydrateDigestionRate()->SetValue(2, MassPerTimeUnit::g_Per_s);
  two.GetFat()->SetValue(2, MassUnit::g);
  two.GetFatDigestionRate()->SetValue(4, MassPerTimeUnit::g_Per_s);
  two.GetProtein()->SetValue(3, MassUnit::g);
  two.GetProteinDigestionRate()->SetValue(6, MassPerTimeUnit::g_Per_s);
  one.Increment(two);
  std::cout << "Carbo Rate: " << one.GetCarbohydrateDigestionRate() << std::endl;
  std::cout << "Fat Rate: " << one.GetFatDigestionRate() << std::endl;
  std::cout << "Protein Rate: " << one.GetProteinDigestionRate() << std::endl;
  */
}

Gastrointestinal::~Gastrointestinal()
{
  Clear();
}

void Gastrointestinal::Clear()
{
  SEGastrointestinalSystem::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::Initialize()
{
  BioGearsSystem::Initialize();

  if (m_data.GetConfiguration().HasDefaultStomachContents()) {
    // We are going to initialize the body with 2 meals so we process the default meal twice
    // 1 meal about 5hrs ago, and one meal at the start of the scenario
    CDM_COPY((m_data.GetConfiguration().GetDefaultStomachContents()), (&GetStomachContents()));
    m_data.GetPatient().GetWeight().IncrementValue(m_StomachContents->GetWeight(MassUnit::g), MassUnit::g);
  }
  // Cache off the initial Gut masses so we can reset back to them after stabilization
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeAminoAcids] = m_SmallIntestineChymeAminoAcids->GetMass(MassUnit::ug);
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeGlucose] = m_SmallIntestineChymeGlucose->GetMass(MassUnit::ug);
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeTriacylglycerol] = m_SmallIntestineChymeTriacylglycerol->GetMass(MassUnit::ug);
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeCalcium] = m_SmallIntestineChymeCalcium->GetMass(MassUnit::ug);
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeSodium] = m_SmallIntestineChymeSodium->GetMass(MassUnit::ug);
  m_InitialSubstanceMasses_ug[m_SmallIntestineChymeUrea] = m_SmallIntestineChymeUrea->GetMass(MassUnit::ug);
}

bool Gastrointestinal::Load(const CDM::BioGearsGastrointestinalSystemData& in)
{
  if (!SEGastrointestinalSystem::Load(in))
    return false;
  BioGearsSystem::LoadState();
  m_DecrementNutrients = true;
  return true;
}
CDM::BioGearsGastrointestinalSystemData* Gastrointestinal::Unload() const
{
  CDM::BioGearsGastrointestinalSystemData* data = new CDM::BioGearsGastrointestinalSystemData();
  Unload(*data);
  return data;
}
void Gastrointestinal::Unload(CDM::BioGearsGastrointestinalSystemData& data) const
{
  SEGastrointestinalSystem::Unload(data);
}

void Gastrointestinal::SetUp()
{
  m_ConsumeRate = false;
  m_DecrementNutrients = false;

  m_WaterDigestionRate.SetValue(m_data.GetConfiguration().GetWaterDigestionRate(VolumePerTimeUnit::mL_Per_s), VolumePerTimeUnit::mL_Per_s);
  m_CalciumDigestionRate.SetValue(m_data.GetConfiguration().GetCalciumDigestionRate(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);

  m_GItoCVPath = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::ChymePath::SmallIntestineC1ToSmallIntestine1);
  m_GutE3ToGroundPath = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::ChymePath::GutE3ToGroundGI);

  m_secretionRate_mL_Per_s = 0.041; // Average from daily secretion rates : Gastric (1500mL) ,Small Intestine(1800mL), Large Intestine (200mL) form Guyton p775
  m_dT_s = m_data.GetTimeStep().GetValue(TimeUnit::s);

  m_vSmallIntestine = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine);
  m_SmallIntestineChyme = m_data.GetCompartments().GetLiquidCompartment(BGE::ChymeCompartment::SmallIntestine);
  m_Lymph = m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph);

  m_SmallIntestineChymeAminoAcids = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetAminoAcids());
  m_SmallIntestineChymeGlucose = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetGlucose());
  m_SmallIntestineChymeTriacylglycerol = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetTriacylglycerol());
  m_SmallIntestineChymeCalcium = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetCalcium());
  m_SmallIntestineChymeSodium = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetSodium());
  m_SmallIntestineChymeUrea = m_SmallIntestineChyme->GetSubstanceQuantity(m_data.GetSubstances().GetUrea());
  m_SmallIntestineVascularSodium = m_vSmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetSodium());
  m_smallIntestineVascularGlucose = m_vSmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetGlucose());
  m_smallIntestineVascularAminoAcids = m_vSmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetAminoAcids());
  m_SmallIntestineVascularCalcium = m_vSmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetCalcium());
  m_LymphTriacylglycerol = m_Lymph->GetSubstanceQuantity(m_data.GetSubstances().GetTriacylglycerol());
  m_smallintestineVAscularTriacylglycerol = m_vSmallIntestine->GetSubstanceQuantity(m_data.GetSubstances().GetTriacylglycerol());
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Do anything needed between stabilization
///
/// \details
/// At the end of resting stabilization we apply a ConsumeMeal condition to modify the body state
/// if no condition is present we reset the body state substance levels to their original level
//--------------------------------------------------------------------------------------------------

void Gastrointestinal::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    /*
    // Apply our conditions   
    if (m_data.GetConditions().HasConsumeMeal())
    {
      SEMeal& meal = m_data.GetConditions().GetConsumeMeal()->GetMeal();
      DefaultNutritionRates(meal);// If no rates are provided, use the defaults
#ifdef logMeal
      m_ss << "Original Patient weight : " << m_data.GetPatient()->GetWeight();
      Info(m_ss);
      m_ss << "Original Stomach weight : " << m_StomachContents->GetWeight(MassUnit::g) << "(g)";
      Info(m_ss);
      m_ss << m_NutrientsInStomach;
      Info(m_ss);
#endif
      // Remove the default meal weight from the patient
      m_data.GetPatient().GetWeight().IncrementValue(-m_StomachContents->GetWeight(MassUnit::g), MassUnit::g);
      // Overwrite meal contents into our stomach
      CDM_COPY((&meal), (m_StomachContents));
      if (!m_StomachContents->HasWater() || m_StomachContents->GetWater().IsZero())
        m_StomachContents->GetWater().SetValue(m_secretionRate_mL_Per_s*m_dT_s, VolumeUnit::mL);//Add a time steps' worth of water if empty
      // Increase our weight by the meal
      m_data.GetPatient().GetWeight().IncrementValue(m_StomachContents->GetWeight(MassUnit::g), MassUnit::g);
      // Empty out the gut
      m_SmallIntestineChyme->GetVolume().SetValue(0, VolumeUnit::mL);
      m_SmallIntestineChymeAminoAcids->GetMass().SetValue(0, MassUnit::ug);
      m_SmallIntestineChymeAminoAcids->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeGlucose->GetMass().SetValue(0,MassUnit::ug);
      m_SmallIntestineChymeGlucose->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeTriacylglycerol->GetMass().SetValue(0, MassUnit::ug);
      m_SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeCalcium->GetMass().SetValue(0, MassUnit::ug);
      m_SmallIntestineChymeCalcium->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeSodium->GetMass().SetValue(0, MassUnit::ug);
      m_SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeUrea->GetMass().SetValue(0, MassUnit::ug);
      m_SmallIntestineChymeUrea->Balance(BalanceLiquidBy::Mass);
#ifdef logMeal
      m_ss << "Stomach weight with meal : " << m_StomachContents->GetWeight(MassUnit::g) << "(g)";
      Info(m_ss);
      m_ss << m_NutrientsInStomach;
      Info(m_ss);
      m_ss << "New Patient weight : " << m_data.GetPatient()->GetWeight();
      Info(m_ss);
#endif
      // Flip flag in order for digestion to actually remove the proper amount from the stomach contents
      m_DecrementNutrients = true;
      // Digest for the elapsed time
#ifdef logMeal
      for (auto isub : m_InitialGutMasses_ug)
      {
        m_ss << "Predigestion Gut mass for " << isub.first->GetSubstance().GetName() << " : " << isub.first->GetMass();
        Info(m_ss);
      }
      m_ss << "Meal Elapsed time " << meal->GetElapsedTime();
      Info(m_ss);
#endif
      DigestStomachNutrients(meal.GetElapsedTime(TimeUnit::s));
#ifdef logMeal
      m_ss << "Stomach weight with digested meal : " << m_StomachContents->GetWeight(MassUnit::g) << "(g)";
      Info(m_ss);
      m_ss << m_NutrientsInStomach;
      Info(m_ss);
      for (auto isub : m_InitialGutMasses_ug)
      {
        m_ss << "Postdigestion Gut mass for " << isub.first->GetSubstance().GetName() << " : " << isub.first->GetMass();
        Info(m_ss);
      }
#endif
      // Flip it back to not decrement since we are still in stabilization 
      m_DecrementNutrients = false;
      // Based on flow, take off mass from GutChyme and put that mass throughout the body (CV/Tissues)
      AbsorbMeal(meal.GetElapsedTime(TimeUnit::min));
#ifdef logMeal
      for (auto isub : m_InitialGutMasses_ug)
      {
        m_ss << "Post-absorbed Gut mass for " << isub.first->GetSubstance().GetName() << " : " << isub.first->GetMass();
        Info(m_ss);
      }
#endif
      // Update our initial masses
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeAminoAcids] = m_SmallIntestineChymeAminoAcids->GetMass(MassUnit::ug);
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeGlucose]    = m_SmallIntestineChymeGlucose->GetMass(MassUnit::ug);
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeTriacylglycerol] = m_SmallIntestineChymeTriacylglycerol->GetMass(MassUnit::ug);
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeCalcium]    = m_SmallIntestineChymeCalcium->GetMass(MassUnit::ug);
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeSodium]     = m_SmallIntestineChymeSodium->GetMass(MassUnit::ug);
      m_InitialSubstanceMasses_ug[m_SmallIntestineChymeUrea]       = m_SmallIntestineChymeUrea->GetMass(MassUnit::ug);
    }
    */
  }
  if (m_data.GetState() == EngineState::AtSecondaryStableState) {
    m_DecrementNutrients = true;
    // Reset the Gut Chyme substance to their original values
    for (auto i : m_InitialSubstanceMasses_ug) {
      i.first->GetMass().SetValue(i.second, MassUnit::ug);
      i.first->Balance(BalanceLiquidBy::Mass);
    }
    // We never decremented the volume so we are good there
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal Preprocess function
///
/// \details
/// Apply any meal consumption actions
/// Absorb fluids from the tissues back into the stomach so it always has fluid in it
/// Digest substances based on our time step and transfer them to the Gut
/// Absorb substances from the gut into the vascular system via the small intestine
//--------------------------------------------------------------------------------------------------

void Gastrointestinal::PreProcess()
{
  if (m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().HasConsumeNutrients()) {
      // Use Default Rates if none provided
      SEConsumeNutrients* c = m_data.GetActions().GetPatientActions().GetConsumeNutrients();
      if (c->HasNutritionFile()) { // Grab file, then load it (note GetNutrition will remove the file name, so get it first)
        std::string file = c->GetNutritionFile();
        if (!c->GetNutrition().Load(file)) {
          /// \error Unable to read consume meal action file
          Error("Could not read provided nutrition file", "Gastrointestinal::PreProcess");
        }
      }
      DefaultNutritionRates(c->GetNutrition());
      m_StomachContents->Increment(c->GetNutrition());
      m_data.GetPatient().GetWeight().IncrementValue(c->GetNutrition().GetWeight(MassUnit::kg), MassUnit::kg);
      m_data.GetActions().GetPatientActions().RemoveConsumeNutrients();
    }
    GastricSecretion(m_dT_s); // Move some water from the Gut EV fluids to the Stomach
    DigestNutrient();
  } else {
    // Reset the Gut Chyme substance to their original values
    for (auto i : m_InitialSubstanceMasses_ug) {
      i.first->GetMass().SetValue(i.second, MassUnit::ug);
      i.first->Balance(BalanceLiquidBy::Mass);
    }
  }
  ChymeSecretion(); // Secrete sodium first
  AbsorbNutrients(); // Absorb nutrients into the Blood from the small intestine chyme
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Absorb fluids from the tissues back into the stomach so it always has fluid in it
///
/// \details
/// Fluid absorption is based on time step at a constant rate
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::GastricSecretion(double duration_s)
{
  // Increase the water in the stomach by the flow rate
  // Take it out of the tissue at the same rate it's being added to the stomach
  //There is a compliance, so the volume will be modified accordingly
  m_GutE3ToGroundPath->GetNextFlowSource().SetValue(m_secretionRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  m_StomachContents->GetWater().IncrementValue(m_secretionRate_mL_Per_s * duration_s, VolumeUnit::mL);
}

// --------------------------------------------------------------------------------------------------
/// \brief
/// Digestion code to convert nutrients into appropriate product due to enzyme interaction
///
/// \details
/// Simulates digestion in the stomach between the three major nutrients and their corresponding enzymes
/// Computes the rate of conversion and then increments that amount per time step into the chyme
/// Handles all movement into the small intestine: ions and nutrients
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::DigestNutrient()
{
  double temp_mg;
  //enzyme parameters for each nutrient:
  /// \cite gangadharan2009biochemical
  //https://www.wolframalpha.com/input/?i=plot+p%3D4.11*x%2F(3.076+%2B+x)+for+x%3D7,35
  double vmaxCarbohydrate_mg_Per_min = 7.11;
  double kmCarbohydrate_mg = 3.076;

  /// \cite ohta1986purification
  //https://www.wolframalpha.com/input/?i=plot+p%3D7.1*x%2F(0.71+%2B+x)+for+x%3D0,15
  double vmaxProtein_mg_Per_min = 7.1;
  double kmProtein_mg = 0.71;

  /// \cite botham1997lipolysis
  //https://www.wolframalpha.com/input/?i=plot+p%3D3.40*x%2F(0.34+%2B+x)+for+x%3D0,15
  double vmaxFat_mg_Per_min = 3.40;
  double kmFat_mg = 0.34;

  //because the stomach node is not a liquid node we compute a "concentration" from an assumed resting volume
  double stomachVolume_mL = 1000.0; //average resting volume, can change between 20mL-4L so large range here
  double digestedAmount = 0.0; //a temp file to compute digested amount of calcium, sodium, other ions or
  double totalCheck = 0.0; //a check to make sure we have enough "stuff" to move

  double proteinConcentration_mg_Per_mL = m_StomachContents->GetProtein(MassUnit::mg) / stomachVolume_mL;
  double fatConcentration_mg_Per_mL = m_StomachContents->GetFat(MassUnit::mg) / stomachVolume_mL;
  double carbConcentration_mg_Per_mL = m_StomachContents->GetCarbohydrate(MassUnit::mg) / stomachVolume_mL;

  //compute conversion rate (enzyme kinetics based on total meal in stomach):
  double glucoseProduct_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_mL) / (kmCarbohydrate_mg + carbConcentration_mg_Per_mL);
  double aminoAcidsProduct_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_mL) / (kmProtein_mg + proteinConcentration_mg_Per_mL);
  double triacylglycerolProduct_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_mL) / (kmFat_mg + fatConcentration_mg_Per_mL);

  //compute mass depleted in stomach
  double carbMassDepleted_mg = glucoseProduct_mg_Per_min * m_dT_s; //*(1 / 60.0);
  double proteinMassDepleted_mg = aminoAcidsProduct_mg_Per_min * m_dT_s; //*(1 / 60.0);
  double fatMassDepleted_mg = triacylglycerolProduct_mg_Per_min * m_dT_s; //*(1 / 60.0);

  //decrement stomach contents and place into small intestine:
  //carbs
  if (m_StomachContents->GetCarbohydrate().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_StomachContents->GetCarbohydrate(MassUnit::mg) > carbMassDepleted_mg) {
      m_StomachContents->GetCarbohydrate().IncrementValue(-carbMassDepleted_mg, MassUnit::mg);
      m_SmallIntestineChymeGlucose->GetMass().IncrementValue(carbMassDepleted_mg, MassUnit::mg);
    } else {
      //otherwise just empty the remaining contents
      temp_mg = std::max(m_StomachContents->GetCarbohydrate(MassUnit::mg), 0.0);
      m_StomachContents->GetCarbohydrate().IncrementValue(-temp_mg, MassUnit::mg);
      m_SmallIntestineChymeGlucose->GetMass().IncrementValue(temp_mg, MassUnit::mg);
    }
    m_SmallIntestineChymeGlucose->Balance(BalanceLiquidBy::Mass);
  }

  //protein
  if (m_StomachContents->GetProtein().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_StomachContents->GetProtein(MassUnit::mg) > proteinMassDepleted_mg) {
      m_StomachContents->GetProtein().IncrementValue(-proteinMassDepleted_mg, MassUnit::mg);
      m_SmallIntestineChymeAminoAcids->GetMass().IncrementValue(proteinMassDepleted_mg, MassUnit::mg);
    } else {
      temp_mg = std::max(m_StomachContents->GetProtein(MassUnit::mg), 0.0);
      m_StomachContents->GetProtein().IncrementValue(-temp_mg, MassUnit::mg);
      m_SmallIntestineChymeAminoAcids->GetMass().IncrementValue(temp_mg, MassUnit::mg);
    }
    m_SmallIntestineChymeAminoAcids->Balance(BalanceLiquidBy::Mass);
  }

  //fat
  if (m_StomachContents->GetFat().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_StomachContents->GetFat(MassUnit::mg) > fatMassDepleted_mg) {
      m_StomachContents->GetFat().IncrementValue(-fatMassDepleted_mg, MassUnit::mg);
      m_SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(fatMassDepleted_mg, MassUnit::mg);
    } else {
      temp_mg = std::max(m_StomachContents->GetFat(MassUnit::mg), 0.0);
      m_StomachContents->GetFat().IncrementValue(-temp_mg, MassUnit::mg);
      m_SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(temp_mg, MassUnit::mg);
    }
    m_SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);
  }

  //handle transfer of remaining substances:
  //calcium
  if (m_StomachContents->HasCalcium()) {
    totalCheck = m_StomachContents->GetCalcium(MassUnit::mg);
    digestedAmount = m_CalciumDigestionRate.GetValue(MassPerTimeUnit::mg_Per_s) * m_dT_s;
    digestedAmount *= m_data.GetConfiguration().GetCalciumAbsorptionFraction(); // Take off percent that usually passes through the body
    if (digestedAmount < totalCheck) {
#ifdef logDigest
      m_ss << "Digested " << digestedAmount << "(g) of Calcium";
      Info(m_ss);
#endif
      m_StomachContents->GetCalcium().IncrementValue(-digestedAmount, (const MassUnit&)MassUnit::g); //TODO: DEBUG THIS
      m_SmallIntestineChymeCalcium->GetMass().IncrementValue(digestedAmount, MassUnit::mg);
      m_SmallIntestineChymeCalcium->Balance(BalanceLiquidBy::Mass);
    }
  }

  //sodium
  if (m_StomachContents->HasSodium()) { // Sodium rate is a function of the concentration of sodium in the stomach, so do this before we do water
    totalCheck = m_StomachContents->GetSodium(MassUnit::g);
    double digestedNa_g = (totalCheck / m_StomachContents->GetWater(VolumeUnit::mL))
      * m_WaterDigestionRate.GetValue(VolumePerTimeUnit::mL_Per_s) * m_dT_s;
    if (totalCheck <= digestedNa_g) {
      digestedNa_g = totalCheck;
      if (m_DecrementNutrients) {
        m_StomachContents->GetSodium().Invalidate();
        Info("Stomach is out of Sodium");
      }
    } else {
      if (m_DecrementNutrients)
        m_StomachContents->GetSodium().IncrementValue(-digestedNa_g, MassUnit::g);
    }
#ifdef logDigest
    m_ss << "Digested " << digestedNa_g << "(g) of Sodium";
    Info(m_ss);
#endif
    m_SmallIntestineChymeSodium->GetMass().IncrementValue(digestedNa_g, MassUnit::g);
    // Wait till the water volume is correct on the chyme before we balance
  }

  digestedAmount = DigestNutrient(m_StomachContents->GetWater(), m_WaterDigestionRate, false, m_dT_s);
  if (digestedAmount > 0) {
#ifdef logDigest
    m_ss << "Digested " << digestedAmount << "(mL) of Water";
    Info(m_ss);
#endif
    m_SmallIntestineChyme->GetVolume().IncrementValue(digestedAmount, VolumeUnit::mL);
  }
  // Balance Sodium, now that we have proper volume on the gut
  m_SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
}

// --------------------------------------------------------------------------------------------------
/// \brief
/// Generic code for removal of an amount of a substance from the stomach based on rate and duration
///
/// \details
/// This function is deprecated as of 6.2 release, it is kept to include nutritional conditions
//--------------------------------------------------------------------------------------------------
double Gastrointestinal::DigestNutrient(SEUnitScalar& totalAmt, SEUnitScalar& rate, bool mass, double duration_s)
{
  double digestedAmt = 0;
  if (totalAmt.IsValid()) {
    double t = totalAmt.GetValue(mass ? (const CCompoundUnit&)MassUnit::g : (const CCompoundUnit&)VolumeUnit::mL);
    digestedAmt = rate.GetValue(mass ? (const CCompoundUnit&)MassPerTimeUnit::g_Per_s : (const CCompoundUnit&)VolumePerTimeUnit::mL_Per_s) * duration_s;
    if (t <= digestedAmt) {
      digestedAmt = t;
      if (m_DecrementNutrients) { // Decrement stomach contents only if we are running (not stabilizing)
        totalAmt.Invalidate();
        if (m_ConsumeRate) // We keep this rate, it's a system parameter not a per nutrition rate as the masses are
          rate.Invalidate();
      }
    } else {
      if (m_DecrementNutrients) // Decrement stomach content only if we are running (not stabilizing)
        totalAmt.IncrementValue(-digestedAmt, mass ? (const CCompoundUnit&)MassUnit::g : (const CCompoundUnit&)VolumeUnit::mL);
    }
  }
  return digestedAmt;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// If no substance rates are provided on meals (Action or Condition) use a default from configuration
///
/// \details
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::DefaultNutritionRates(SENutrition& n)
{
  if (n.HasCarbohydrate() && !n.HasCarbohydrateDigestionRate())
    n.GetCarbohydrateDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultCarbohydrateDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
  if (n.HasFat() && !n.HasFatDigestionRate())
    n.GetFatDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultFatDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
  if (n.HasProtein() && !n.HasProteinDigestionRate())
    n.GetProteinDigestionRate().SetValue(m_data.GetConfiguration().GetDefaultProteinDigestionRate(MassPerTimeUnit::g_Per_min), MassPerTimeUnit::g_Per_min);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Absorb substances from the gut into the vascular system via the small intestine
///
/// \details
/// Move substances from the chyme system into the vascular system based on a rate that is a
/// function of the sodium mass in the chyme. co-transporters are used to carry glucose and amino acids
/// Sodium absorption rate is determined by a hill-type function that increases during
/// heavy nutrient loads. Water moves at a constant rate and doesn't transport substances with it
/// because there isn't a compartment link connecting the chyme and vasculature.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::AbsorbNutrients()
{

  //parameters for sodium absorption
  double sodiumAbsorptionVmax_g_Per_h = 9.0;
  double sodiumAbsorptionKm_g = 2.0; //https://www.wolframalpha.com/input/?i=plot+p%3D10.0*x%2F(2.0+%2B+x)+for+x%3D0,15
  double sodiumAbsorption_g_Per_h = 0.0;
  double sodiumAbsorbed_g = 0.0;

  double smallIntestineVolume_mL = m_SmallIntestineChyme->GetVolume(VolumeUnit::mL);

  //parameters for nutrient absorption:
  double glucoseAbsorbed_g = 0.0;
  double aminoAcidAbsorbed_g = 0.0;
  double tempAbsorbed_g = 0.0;
  double intestineFatAbsorption_mg_Per_s = (0.019) * 100.0; // This is scaled from rat data, 1 g avg rat meal to 30g average human meal in fat. /// \cite aberdeen1960concurrent
  double triacylglycerolAbsorbed_mg = intestineFatAbsorption_mg_Per_s * m_dT_s;

  //access the chyme mass of our nutrients:
  double massNutrients_g = std::max(std::max(m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g), m_SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g)),
    std::max(m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g), m_SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::g)));

  //compute the hill function sodium absorption
  sodiumAbsorption_g_Per_h = sodiumAbsorptionVmax_g_Per_h * (massNutrients_g / (sodiumAbsorptionKm_g + massNutrients_g));

  //compute mass moved:
  sodiumAbsorbed_g = sodiumAbsorption_g_Per_h * (1.0 / 3600.0) * m_dT_s;

  //compute nutrient absorption from new absorbed values (same co-transporter assumption here):
  glucoseAbsorbed_g = 2.0 * sodiumAbsorbed_g;
  aminoAcidAbsorbed_g = sodiumAbsorbed_g;

  //move nutrients, glucose
  if (m_SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g) > glucoseAbsorbed_g && m_SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::g) > sodiumAbsorbed_g) {
      m_SmallIntestineChymeSodium->GetMass().IncrementValue(-sodiumAbsorbed_g, MassUnit::g);
      m_SmallIntestineChymeGlucose->GetMass().IncrementValue(-glucoseAbsorbed_g, MassUnit::g);

      m_SmallIntestineVascularSodium->GetMass().IncrementValue(sodiumAbsorbed_g, MassUnit::g);
      m_smallIntestineVascularGlucose->GetMass().IncrementValue(glucoseAbsorbed_g, MassUnit::g);
    }
  }

  //protein
  if (m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g) > aminoAcidAbsorbed_g && m_SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::g) > sodiumAbsorbed_g) {
      m_SmallIntestineChymeSodium->GetMass().IncrementValue(-sodiumAbsorbed_g, MassUnit::g);
      m_SmallIntestineChymeAminoAcids->GetMass().IncrementValue(-aminoAcidAbsorbed_g, MassUnit::g);

      m_SmallIntestineVascularSodium->GetMass().IncrementValue(sodiumAbsorbed_g, MassUnit::g);
      m_smallIntestineVascularAminoAcids->GetMass().IncrementValue(aminoAcidAbsorbed_g, MassUnit::g);
    }
  }

  //fat
  if (m_SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::mg) > ZERO_APPROX) {
    if (m_SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::mg) > triacylglycerolAbsorbed_mg) {
      m_SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(-triacylglycerolAbsorbed_mg, MassUnit::mg);
      m_smallintestineVAscularTriacylglycerol->GetMass().IncrementValue(triacylglycerolAbsorbed_mg, MassUnit::mg);

      m_smallintestineVAscularTriacylglycerol->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);
    }
  }

  //compute absorption rate as a function of volume in intestine
  double absorptionRate_mL_Per_min = 0.0; //3.3; // Average water absorption rate Peronnet
  absorptionRate_mL_Per_min = GeneralMath::LogisticFunction(13, 500, 0.007, smallIntestineVolume_mL);
  double absorbedVolume_mL = absorptionRate_mL_Per_min / 60.0 * m_dT_s;

  //move fluid
  if (m_data.GetState() == EngineState::Active) { // Don't Remove volume while stabilizing
    if (smallIntestineVolume_mL > absorbedVolume_mL) {
      m_GItoCVPath->GetNextFlowSource().SetValue(absorptionRate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
      m_GItoCVPath->GetSourceNode().GetNextVolume().IncrementValue(-absorbedVolume_mL, VolumeUnit::mL);
    }
  }

  //only move sodium independently if it wasn't moved through the co-transporter
  if (sodiumAbsorbed_g < ZERO_APPROX) {
    double ionMassMoved_mg = m_SmallIntestineChymeSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL) * absorbedVolume_mL;

    //substance connect ions to flow rate when nutrients aren't present, needs to have volume present
    if (m_SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::mg) > ionMassMoved_mg && m_SmallIntestineChyme->GetVolume(VolumeUnit::mL) > absorbedVolume_mL) {
      m_SmallIntestineChymeSodium->GetMass().IncrementValue(-ionMassMoved_mg, MassUnit::mg);
      m_SmallIntestineVascularSodium->GetMass().IncrementValue(ionMassMoved_mg, MassUnit::mg);
      m_SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
      m_SmallIntestineVascularSodium->Balance(BalanceLiquidBy::Mass);
    }
  }

  double ionCalciumMassMoved_mg = m_SmallIntestineChymeCalcium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL) * absorbedVolume_mL;
  //Move calcium regardless
  if (m_SmallIntestineChymeCalcium->GetMass().GetValue(MassUnit::mg) > ionCalciumMassMoved_mg) {
    m_SmallIntestineChymeCalcium->GetMass().IncrementValue(-ionCalciumMassMoved_mg, MassUnit::mg);
    m_SmallIntestineVascularCalcium->GetMass().IncrementValue(ionCalciumMassMoved_mg, MassUnit::mg);
    m_SmallIntestineChymeCalcium->Balance(BalanceLiquidBy::Mass);
    m_SmallIntestineVascularCalcium->Balance(BalanceLiquidBy::Mass);
  }

  // Calculate new concentrations for everything based on new volume
  for (SELiquidSubstanceQuantity* subQ : m_SmallIntestineChyme->GetSubstanceQuantities()) {
    if (subQ->HasMass())
      subQ->Balance(BalanceLiquidBy::Mass);
  }
  for (SELiquidSubstanceQuantity* subQ : m_vSmallIntestine->GetSubstanceQuantities()) {
    if (subQ->HasMass())
      subQ->Balance(BalanceLiquidBy::Mass);
  }

  GetChymeAbsorptionRate().SetValue(absorbedVolume_mL / m_dT_s, VolumePerTimeUnit::mL_Per_s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Absorb substances from the gut into the vascular system via the small intestine
///
/// \details
/// Move substances from the chyme system into the vascular system based on a rate that is a
/// function of the sodium concentration in the body. This function is deprecated as of 6.2
/// but is included to support meal conditions that will be updated as of 6.3
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::AbsorbNutrients(double duration_s)
{
  double absorptionRate_mL_Per_min = 3.3; // Average water absorption rate Peronnet

  double absorbedVolume_mL = absorptionRate_mL_Per_min * duration_s / 60;
  double siVolume_mL = m_SmallIntestineChyme->GetVolume().GetValue(VolumeUnit::mL);
  if (siVolume_mL <= 0) {
    absorptionRate_mL_Per_min = 0; // No volume, no flow
  } else if (absorbedVolume_mL > siVolume_mL) { //Only take what we have
    absorbedVolume_mL = siVolume_mL;
    absorptionRate_mL_Per_min = 60 * (absorbedVolume_mL / duration_s);
  }

  if (m_data.GetState() == EngineState::AtSecondaryStableState) { // Don't Remove volume while stabilizing
    m_SmallIntestineChyme->GetVolume().IncrementValue(-absorbedVolume_mL, VolumeUnit::mL);
    // Calculate new concentrations for everything based on new volume
    for (SELiquidSubstanceQuantity* subQ : m_SmallIntestineChyme->GetSubstanceQuantities()) {
      if (subQ->HasMass())
        subQ->Balance(BalanceLiquidBy::Mass);
    }
  }

  GetChymeAbsorptionRate().SetValue(absorptionRate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  //This will move substances
  m_GItoCVPath->GetNextFlowSource().SetValue(absorptionRate_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  //Need to manually remove volume, since there isn't a compliance
  m_GItoCVPath->GetSourceNode().GetNextVolume().IncrementValue(-absorptionRate_mL_Per_min / 60.0 * m_dT_s, VolumeUnit::mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Secrete sodium into the intestinal chyme
///
/// \details
/// Secretion of sodium into the small intestine chyme is computed as a function of digested amount
/// a hill function determines rates of sodium transport. Sodium is then used as a co-transporter to facilitate
/// movement into the blood. Could be expanded to support other ions and substances to get correct pH balance
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::ChymeSecretion()
{
  //sigmoidal secretion as a function of nutrient content in the chyme
  double sodiumSecretionVmax_g_Per_h = 5.0;
  double sodiumSecretionKm_g = 0.2;
  double sodiumSecretion_g_Per_h = 0.0;
  double sodiumSecreted_g = 0.0;
  double temp_g = 0.0;

  //access the chyme mass of our nutrients:
  double massNutrients_g = std::max(std::max(m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g), m_SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g)),
    m_SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g));

  //compute the secretion of salt into the intestine:
  sodiumSecretion_g_Per_h = sodiumSecretionVmax_g_Per_h * (massNutrients_g / (sodiumSecretionKm_g + massNutrients_g));

  //compute mass moved:
  sodiumSecreted_g = sodiumSecretion_g_Per_h * (1.0 / 3600.0) * m_dT_s;

  //move sodium from vascular into the chyme compartment
  if (sodiumSecreted_g > 0.0) {
    if (m_SmallIntestineVascularSodium->GetMass().GetValue(MassUnit::g) > sodiumSecreted_g) {
      m_SmallIntestineVascularSodium->GetMass().IncrementValue(-sodiumSecreted_g, MassUnit::g);
      m_SmallIntestineChymeSodium->GetMass().IncrementValue(sodiumSecreted_g, MassUnit::g);
    }
    m_SmallIntestineVascularSodium->Balance(BalanceLiquidBy::Mass);
    m_SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Absorb a meal over a specified length of time
///
/// \details
/// This method supports the ConsumeMeal Condition, it will move the provided meal into the stomach
/// Digest food for the specified length of time and move it to the Gut, then calculate the amount
/// of substance that would then have been absorbed from the Gut and distributes that mass throughout
/// the body tissues
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::AbsorbMeal(double duration_min)
{
  if (duration_min <= 0)
    return;

  // We use the default healthy flow rate throughout this absorption
  // We could step wise this and see if we can figure out how pH is changing
  // in order to change the flow rate, but that is a bit much for this model implementation
  double flowRate_mL_Per_min = 3.3;

  // Calculate the amount of fluid flow based on the elapsed time
  double absorbedVolume_mL = flowRate_mL_Per_min * duration_min;
  double siVolume_mL = m_SmallIntestineChyme->GetVolume().GetValue(VolumeUnit::mL);
  if (absorbedVolume_mL > siVolume_mL)
    absorbedVolume_mL = siVolume_mL; // Don't take off what we don't have
#ifdef logMeal
  double totVol_mL = m_data.GetBloodChemistry()->GetExtravascularFluidVolume()->GetValue(VolumeUnit::mL);
  totVol_mL += m_data.GetCardiovascular()->GetBloodVolume()->GetValue(VolumeUnit::mL);
  m_ss << "Blood Volume before absorption " << totVol_mL << "(mL)";
  Info(m_ss);
  m_ss << "Gut Volume before absorption: " << m_GutChyme->GetVolume();
  Info(m_ss);
  m_ss << "Absorbed Volume : " << absorbedVolume_mL << "(mL)";
  Info(m_ss);
#endif
  m_SmallIntestineChyme->GetVolume().IncrementValue(-absorbedVolume_mL, VolumeUnit::mL);

  SEScalarVolume absorbedVolume;
  absorbedVolume.SetValue(absorbedVolume_mL, VolumeUnit::mL);
//m_data.GetCircuits().DistributeVolume(absorbedVolume);
#ifdef logMeal
  double newtotVol_mL = m_data.GetBloodChemistry()->GetExtravascularFluidVolume()->GetValue(VolumeUnit::mL);
  newtotVol_mL += m_data.GetCardiovascular()->GetBloodVolume()->GetValue(VolumeUnit::mL);
  m_ss << "Blood Volume after absorption " << newtotVol_mL << "(mL), body absorbed : " << newtotVol_mL - totVol_mL;
  Info(m_ss);
  m_ss << "Gut Volume after absorption: " << m_GutChyme->GetVolume();
  Info(m_ss);
  if (newtotVol_mL - totVol_mL != absorbedVolume_mL)
    Error("Water volume is NOT conserved");
  else
    Info("Water volume is conserved");
#endif

  // Doing mass then volume, I don't think it matters which order this happens in since
  // the amount of mass that goes on a compartment is the ratio of it volume over the total
  // and incrementing total volume via compartment should keep that same ratio

  double bloodVol_mL = m_data.GetCardiovascular().GetBloodVolume(VolumeUnit::mL);
  double tissueVol_mL = m_data.GetTissue().GetExtravascularFluidVolume(VolumeUnit::mL);

  SEScalarMass mass;
  double gutMass_g;
  double absorbedMass_g = 0;
  for (auto i : m_InitialSubstanceMasses_ug) {
    gutMass_g = i.first->GetMass().GetValue(MassUnit::g);
    if (gutMass_g == 0)
      continue; // Nothing to move!
    // Calculate the amount that absorbed over the elapsed duration time
    absorbedMass_g = flowRate_mL_Per_min * duration_min * i.first->GetConcentration(MassPerVolumeUnit::g_Per_mL);

    // Make sure we do not pull out more than we have in the Gut Chyme
    if (gutMass_g < absorbedMass_g)
      absorbedMass_g = gutMass_g;
    if (absorbedMass_g <= 0)
      continue;
      // Pull mass off the chyme
#ifdef logMeal
    m_ss << "Preabsorption Gut mass for " << i.first->GetSubstance().GetName() << " " << i.first->GetMass();
    Info(m_ss);
    m_ss << "Absorbing " << absorbedMass_g << " (g)";
    Info(m_ss);
#endif
    i.first->GetMass().IncrementValue(-absorbedMass_g, MassUnit::g);
    i.first->Balance(BalanceLiquidBy::Mass);
    m_InitialSubstanceMasses_ug[i.first] = i.first->GetMass(MassUnit::ug);
#ifdef logMeal
    m_ss << "Postabsorption Gut mass for " << i.first->GetSubstance().GetName() << " " << i.first->GetMass();
    Info(m_ss);

    // Add mass to the tissue
    double bPreAbsorbed_g = m_data.GetCircuits()->TotalSubstanceInBlood(i.first->GetSubstance(), MassUnit::g);
    double tPreAbsorbed_g = m_data.GetCircuits()->TotalSubstanceInTissue(i.first->GetSubstance(), MassUnit::g);
    m_ss << "Preabsorbed Total " << i.first->GetSubstance().GetName() << " in body : " << bPreAbsorbed_g + tPreAbsorbed_g << "(g) b(" << bPreAbsorbed_g << ") t(" << tPreAbsorbed_g << ")";
    Info(m_ss);
#endif
    // Distribute the mass over the body
    mass.SetValue(absorbedMass_g, MassUnit::g);
//  m_data.GetCircuits().DistributeBloodAndTissueMass(i.first->GetSubstance(), mass);
#ifdef logMeal
    double bPostAbsorbed_g = m_data.GetCircuits()->TotalSubstanceInBlood(i.first->GetSubstance(), MassUnit::g);
    double tPostAbsorbed_g = m_data.GetCircuits()->TotalSubstanceInTissue(i.first->GetSubstance(), MassUnit::g);
    m_ss << "PostAbsorbed Total " << i.first->GetSubstance().GetName() << " in body : " << bPostAbsorbed_g + tPostAbsorbed_g << "(g), body absorbed : " << (bPostAbsorbed_g + tPostAbsorbed_g) - (bPreAbsorbed_g + tPreAbsorbed_g) << "(g) b(" << bPostAbsorbed_g << ") t(" << tPostAbsorbed_g << ")";
    Info(m_ss);
    double massDifference_g = (bPostAbsorbed_g + tPostAbsorbed_g) - (bPreAbsorbed_g + tPreAbsorbed_g);
    if (!SEScalar::IsZero(massDifference_g - absorbedMass_g))
      Error("Mass is NOT Conserved");
    else
      Info("Mass is conserved");
#endif
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal Preprocess function
///
/// \details
/// The current BioGears implementation has no functionality in the process function for Gastrointestinal.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::Process()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Gastrointestinal postprocess function
///
/// \details
/// The current BioGears implementation has no specific postprocess functionality.
//--------------------------------------------------------------------------------------------------
void Gastrointestinal::PostProcess()
{
}
