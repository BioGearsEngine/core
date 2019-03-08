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
#include <biogears/engine/test/BioGearsEngineTest.h>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/GeneralMath.h>

namespace biogears {
void BioGearsEngineTest::DigestionTest(const std::string& rptDirectory, MealType mealType)
{
  DataTrack trk;
  std::string outputName;

  if (mealType == CarbsOnly)
    outputName = "/DigestionTestCarbs";
  if (mealType == ProteinOnly)
    outputName = "/DigestionTestProtein";
  if (mealType == FatOnly)
    outputName = "/DigestionTestFat";
  if (mealType == Balanced)
    outputName = "/DigestionTestABalanced";

  m_Logger->ResetLogFile(rptDirectory + outputName + ".log");
  std::ofstream file;
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  BioGears bg(m_Logger);
  Gastrointestinal& gi = (Gastrointestinal&)bg.GetGastrointestinal();

  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  SEPatient* patient = (SEPatient*)&bg.GetPatient();

  //parameters:
  double smallIntestineVolume_mL = 219.8;
  double stomachVolume_mL = 1000.0; //average resting volume, can change between 20mL-4L so large range here
  double systolicPressureTarget_mmHg = patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  double VascularPressureTargetSmallIntestine = 0.33 * systolicPressureTarget_mmHg;
  double bloodVolume_mL = patient->GetBloodVolumeBaseline(VolumeUnit::mL);
  double aortaVolume_mL = 0.05 * bloodVolume_mL; // in-engine value for aorta volume
  double AmbientPresure_cmH20 = 1033.23; // = 1 atm
  double aortaSodium_g = 0.86; //to get around 3.2 mg/L blood concentration
  //meal parameters:
  double proteinMass_g = 0.0;
  double carbsMass_g = 0.0;
  double fatMass_g = 0.0;

  //tracking absorption
  double glucoseAbsorbed_mg = 0.0;
  double stomachGlucose_mg = 0.0;

  //for absorption
  double sodiumSecretion_mg_Per_s = 0.29; //about 25 g/day secreted into small intestinal bile
  double sodiumAbsorption_mg_Per_s = 0.35; // about 30 g/day absorbed
  double sodiumBileMass_mg = 7.0;

  //enzyme parameters for each nutrient:
  /// \cite gangadharan2009biochemical
  //https://www.wolframalpha.com/input/?i=plot+p%3D4.11*x%2F(3.076+%2B+x)+for+x%3D7,35
  double vmaxCarbohydrate_mg_Per_min = 4.11;
  double kmCarbohydrate_mg = 3.076;

  /// \cite ohta1986purification
  //https://www.wolframalpha.com/input/?i=plot+p%3D7.1*x%2F(0.71+%2B+x)+for+x%3D0,15
  double vmaxProtein_mg_Per_min = 7.1;
  double kmProtein_mg = 0.71;

  /// \cite botham1997lipolysis
  //https://www.wolframalpha.com/input/?i=plot+p%3D3.40*x%2F(0.34+%2B+x)+for+x%3D0,15
  double vmaxFat_mg_Per_min = 3.40;
  double kmFat_mg = 0.34;

  //set up three compartments: Aorta, stomach and small intestine:
  SELiquidCompartment& cAortaVascular = bg.GetCompartments().CreateLiquidCompartment("cAortaVascular");
  cAortaVascular.GetVolume().SetValue(aortaVolume_mL, VolumeUnit::mL);
  SELiquidCompartment& cSmallIntestineChyme = bg.GetCompartments().CreateLiquidCompartment("cSmallIntestineChyme");
  cSmallIntestineChyme.GetVolume().SetValue(smallIntestineVolume_mL, VolumeUnit::mL);
  SELiquidCompartment& cStomach = bg.GetCompartments().CreateLiquidCompartment("cStomach");
  cStomach.GetVolume().SetValue(stomachVolume_mL, VolumeUnit::mL);

  //Create the graph
  SELiquidCompartmentGraph& Graph = bg.GetCompartments().CreateLiquidGraph("Graph");
  Graph.AddCompartment(cAortaVascular);
  Graph.AddCompartment(cSmallIntestineChyme);

  SELiquidSubstanceQuantity* SmallIntestineChymeAminoAcids;
  SELiquidSubstanceQuantity* SmallIntestineChymeGlucose;
  SELiquidSubstanceQuantity* SmallIntestineChymeTriacylglycerol;
  SELiquidSubstanceQuantity* SmallIntestineChymeSodium;
  SELiquidSubstanceQuantity* AortaSodium;
  SELiquidSubstanceQuantity* AortaAminoAcids;
  SELiquidSubstanceQuantity* AortaGlucose;
  SELiquidSubstanceQuantity* AortaTriacylglycerol;

  //substances
  SESubstance& aminoAcids = bg.GetSubstances().GetAminoAcids();
  SESubstance& glucose = bg.GetSubstances().GetGlucose();
  SESubstance& triacylglycerol = bg.GetSubstances().GetTriacylglycerol();
  SESubstance& sodium = bg.GetSubstances().GetSodium();

  bg.GetSubstances().AddActiveSubstance(aminoAcids);
  bg.GetSubstances().AddActiveSubstance(glucose);
  bg.GetSubstances().AddActiveSubstance(triacylglycerol);
  bg.GetSubstances().AddActiveSubstance(sodium);

  //set substance on intestine and aorta (initialize to 0 mass)
  cSmallIntestineChyme.GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(sodium)->GetMass().SetValue(sodiumBileMass_mg, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(sodium)->GetMass().SetValue(aortaSodium_g, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.Balance(BalanceLiquidBy::Mass);
  cAortaVascular.Balance(BalanceLiquidBy::Mass);

  //small intestine nutrients/substances
  SmallIntestineChymeAminoAcids = cSmallIntestineChyme.GetSubstanceQuantity(aminoAcids);
  SmallIntestineChymeGlucose = cSmallIntestineChyme.GetSubstanceQuantity(glucose);
  SmallIntestineChymeTriacylglycerol = cSmallIntestineChyme.GetSubstanceQuantity(triacylglycerol);
  SmallIntestineChymeSodium = cSmallIntestineChyme.GetSubstanceQuantity(sodium);

  //aorta nutrients/substances
  AortaSodium = cAortaVascular.GetSubstanceQuantity(sodium);
  AortaTriacylglycerol = cAortaVascular.GetSubstanceQuantity(triacylglycerol);
  AortaAminoAcids = cAortaVascular.GetSubstanceQuantity(aminoAcids);
  AortaGlucose = cAortaVascular.GetSubstanceQuantity(glucose);

  //Nutrition
  SENutrition nutrition(m_Logger);

  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 50.0;
  double runTime_hr = 1;
  bool fed = false;
  std::stringstream ss;

  //given the mealtype we will test digestion of each nutrient (this is base concentration, will increment value):
  switch (mealType) {
  case MealType::ProteinOnly:
    proteinMass_g = 3;
    break;
  case MealType::CarbsOnly:
    carbsMass_g = 3;
    break;
  case MealType::FatOnly:
    fatMass_g = 3;
    break;
  case MealType::Balanced: //https://www.bodybuilding.com/fun/ask-the-muscle-prof-what-are-the-ideal-macros-for-my-breakfast.html
    proteinMass_g = 20;
    carbsMass_g = 15;
    fatMass_g = 15;
    break;
  default:
    break;
  }

  int i = 0;
  double increment_g = 3.0;
  int totalIncrements = 10;

  for (int j = 0; j < totalIncrements; j++) {
    //update food increment for each j
    switch (mealType) {
    case MealType::ProteinOnly:
      proteinMass_g += increment_g;
      break;
    case MealType::CarbsOnly:
      carbsMass_g += increment_g;
      break;
    case MealType::FatOnly:
      fatMass_g += increment_g;
      break;
    case MealType::Balanced: //https://www.bodybuilding.com/fun/ask-the-muscle-prof-what-are-the-ideal-macros-for-my-breakfast.html
      proteinMass_g += increment_g;
      carbsMass_g += increment_g;
      fatMass_g += increment_g;
      break;
    default:
      break;
    }

    //nutrition mass (this is how its done in-engine)
    nutrition.GetCarbohydrate().SetValue(carbsMass_g, MassUnit::g);
    nutrition.GetProtein().SetValue(proteinMass_g, MassUnit::g);
    nutrition.GetFat().SetValue(fatMass_g, MassUnit::g);

    //reset intestine masses:
    SmallIntestineChymeGlucose->GetMass().SetValue(0, MassUnit::mg);
    SmallIntestineChymeTriacylglycerol->GetMass().SetValue(0, MassUnit::mg);
    SmallIntestineChymeAminoAcids->GetMass().SetValue(0, MassUnit::mg);

    //compute concentrations in the stomach:
    double carbConcentration_mg_Per_ml = nutrition.GetCarbohydrate().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
    double carbConcentrationTemp = carbConcentration_mg_Per_ml;

    double proteinConcentration_mg_Per_ml = nutrition.GetProtein().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
    double proteinConcentrationTemp = proteinConcentration_mg_Per_ml;

    double fatConcentration_mg_Per_ml = nutrition.GetFat().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
    double fatConcentrationTemp = fatConcentration_mg_Per_ml;

    //for output compute the product due to enzyme kinetics and store (don't update):
    double glucoseProductTemp_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
    double aminoAcidsProductTemp_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
    double triacylglycerolProductTemp_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);

    double glucoseProduct_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
    double aminoAcidsProduct_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
    double triacylglycerolProduct_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);

    double carbMassDepleted_mg = glucoseProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);
    double proteinMassDepleted_mg = aminoAcidsProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);
    double fatMassDepleted_mg = triacylglycerolProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

    double sodiumSecreted_mg = sodiumSecretion_mg_Per_s * deltaT_s;
    double sodiumAbsorbed_mg = sodiumAbsorption_mg_Per_s * deltaT_s;

    // 1 sodium = 2 glucose or 1 amino acid
    double glucoseAbsorbed_mg = 2.0 * sodiumAbsorbed_mg;
    double aminoAcidAbsorbed_mg = sodiumAbsorbed_mg;

    //quick check for concentration:
    //std::cout << "carbohydrate concentration: " << carbConcentration_mg_Per_ml << std::endl;
    //std::cout << "protein concentration: " << proteinConcentration_mg_Per_ml << std::endl;
    //std::cout << "fat concentration: " << fatConcentration_mg_Per_ml << std::endl;

    //check for mass on small intestine:
    std::cout << "glucose mass" << SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg) << std::endl;

    //reset timer:
    double timer_s = 0.0;

    //put the mass into the small intestine chyme compartment from digestion:
    while ((carbConcentration_mg_Per_ml > 0.1 || proteinConcentration_mg_Per_ml > 0.1 || fatConcentration_mg_Per_ml > 0.1)) {
      //for a given carbohydrate substrate concentration compute the product conversion time:
      //https://www.wolframalpha.com/input/?i=plot+p%3D4.11*x%2F(3.076+%2B+x)+for+x%3D10,70
      //Update each interaction:
      glucoseProduct_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
      carbMassDepleted_mg = glucoseProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

      aminoAcidsProduct_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
      proteinMassDepleted_mg = aminoAcidsProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

      triacylglycerolProduct_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);
      fatMassDepleted_mg = triacylglycerolProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

      //add digested nutrient mass into chyme
      SmallIntestineChymeGlucose->GetMass().IncrementValue(carbMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeGlucose->Balance(BalanceLiquidBy::Mass);
      SmallIntestineChymeAminoAcids->GetMass().IncrementValue(proteinMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeAminoAcids->Balance(BalanceLiquidBy::Mass);
      SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(fatMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);

      //decrement from nutrition
      nutrition.GetCarbohydrate().IncrementValue(-carbMassDepleted_mg, MassUnit::mg);
      nutrition.GetProtein().IncrementValue(-proteinMassDepleted_mg, MassUnit::mg);
      nutrition.GetFat().IncrementValue(-fatMassDepleted_mg, MassUnit::mg);

      //update concentration on the stomach
      carbConcentration_mg_Per_ml = nutrition.GetCarbohydrate().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
      proteinConcentration_mg_Per_ml = nutrition.GetProtein().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
      fatConcentration_mg_Per_ml = nutrition.GetFat().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);

      //update timer s
      timer_s += deltaT_s;
      time += deltaT_s;
      i += 1;
    }

    if (mealType == CarbsOnly) {
      std::cout << "glucose" << carbConcentrationTemp << std::endl;
      trk.Track("carbDigestion_s", timer_s, carbConcentrationTemp);
      trk.Track("Glucose_mg_Per_min", timer_s, glucoseProductTemp_mg_Per_min);
      trk.Track("GlucoseIntestineMass_mg", timer_s, SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg));
      trk.Track("glucoseConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeGlucose->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
    }

    if (mealType == ProteinOnly) {
      trk.Track("proteinDigestion_s", timer_s, proteinConcentrationTemp);
      trk.Track("aminoAcid_mg_Per_min", timer_s, aminoAcidsProductTemp_mg_Per_min);
      trk.Track("AminoAcidIntestineMass_mg", timer_s, SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::mg));
    }

    if (mealType == FatOnly) {
      trk.Track("fatDigestion_s", timer_s, fatConcentrationTemp);
      trk.Track("Triacyglycerol_mg_Per_min", timer_s, triacylglycerolProductTemp_mg_Per_min);
      trk.Track("TriacylglycerolIntestineMass_mg", timer_s, SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::mg));
    }

    if (mealType == Balanced) {
      trk.Track("totalDigestion_s", timer_s, fatConcentrationTemp);
      trk.Track("proteinDigestion_s", timer_s, proteinConcentrationTemp);
      trk.Track("carbDigestion_s", timer_s, carbConcentrationTemp);
      trk.Track("Triacyglycerol_mg_Per_min", timer_s, triacylglycerolProductTemp_mg_Per_min);
      trk.Track("aminoAcid_mg_Per_min", timer_s, aminoAcidsProductTemp_mg_Per_min);
      trk.Track("Glucose_mg_Per_min", timer_s, glucoseProductTemp_mg_Per_min);
    }

    if (j == 0)
      trk.CreateFile(std::string(rptDirectory + outputName + ".csv").c_str(), file);
    trk.StreamTrackToFile(file);
  }
  file.close();
}

void BioGearsEngineTest::AbsorptionTest(const std::string& rptDirectory, MealType mealType)
{
  DataTrack trk;
  std::string outputName;

  if (mealType == CarbsOnly)
    outputName = "/AbsoptionCarbs";
  if (mealType == ProteinOnly)
    outputName = "/AbsoptionProtein";
  if (mealType == FatOnly)
    outputName = "/AbsoptionFat";
  if (mealType == Balanced)
    outputName = "/AbsoptionBalanced";

  m_Logger->ResetLogFile(rptDirectory + outputName + ".log");
  std::ofstream file;
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  BioGears bg(m_Logger);
  Gastrointestinal& gi = (Gastrointestinal&)bg.GetGastrointestinal();

  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  SEPatient* patient = (SEPatient*)&bg.GetPatient();

  //parameters:
  double smallIntestineVolume_mL = 219.8;
  double stomachVolume_mL = 1000.0; //average resting volume, can change between 20mL-4L so large range here
  double lymphVolume_mL = 2000.0; //around 2L lymph capacity, in BioGears currently this is set to zero?
  double systolicPressureTarget_mmHg = patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  double VascularPressureTargetSmallIntestine = 0.33 * systolicPressureTarget_mmHg;
  double bloodVolume_mL = patient->GetBloodVolumeBaseline(VolumeUnit::mL);
  double aortaVolume_mL = 0.05 * bloodVolume_mL; // in-engine value for aorta volume
  double AmbientPresure_cmH20 = 1033.23; // = 1 atm
  double aortaSodium_g = 1000.86; //just putting  a pool of sodium here for now, need to fix this on implementation
    //meal parameters:
  double proteinMass_g = 0.0;
  double carbsMass_g = 0.0;
  double fatMass_g = 0.0;

  //for absorption (calculate water flow rate from this)
  double sodiumSecretionVmax_g_Per_h = 5.0;
  double sodiumSecretionKm_g = 0.2; //0.51;   //https://www.wolframalpha.com/input/?i=plot+p%3D8.0*x%2F(0.51+%2B+x)+for+x%3D0,15
  double sodiumAbsorptionVmax_g_Per_h = 9.0;
  double sodiumAbsorptionKm_g = 2.0; //https://www.wolframalpha.com/input/?i=plot+p%3D10.0*x%2F(2.0+%2B+x)+for+x%3D0,15
  //double sodiumSecretion_mg_Per_s = 0.29;  //about 25 g/day secreted into small intestinal bile Guyton pg 794
  //double sodiumAbsorption_mg_Per_s = 0.35;    // about 30 g/day absorbed
  double sodiumSecretion_g_Per_h = 0.0;
  double sodiumAbsorption_g_Per_h = 0.0;
  double sodiumBileMass_g = 7.0;
  double stomachGlucose_mg = 0.0;
  double intestineFatAbsorption_mg_Per_s = (0.019) * 100.0; // This is scaled from rat data, 1 g avg rat meal to 30g average human meal in fat. /// \cite aberdeen1960concurrent

  //fractional absorption of nutrients
  double fatFraction = 0.97; //Guyton, most fat is absorbed

  //digestion proteins:
  double trypsinConcentration_mg_Per_ml = 7.0;
  double lipaseConcentration_mg_Per_ml = 7.0;
  double amalayzeConcentration_mg_Per_ml = 7.0;

  //enzyme parameters for each nutrient:
  /// \cite gangadharan2009biochemical
  //https://www.wolframalpha.com/input/?i=plot+p%3D4.11*x%2F(3.076+%2B+x)+for+x%3D7,35
  double vmaxCarbohydrate_mg_Per_min = 4.11;
  double kmCarbohydrate_mg = 3.076;

  /// \cite ohta1986purification
  //https://www.wolframalpha.com/input/?i=plot+p%3D7.1*x%2F(0.71+%2B+x)+for+x%3D0,15
  double vmaxProtein_mg_Per_min = 7.1;
  double kmProtein_mg = 20.71;

  /// \cite botham1997lipolysis
  //https://www.wolframalpha.com/input/?i=plot+p%3D3.40*x%2F(0.34+%2B+x)+for+x%3D0,15
  double vmaxFat_mg_Per_min = 3.40;
  double kmFat_mg = 0.34;

  //set up three compartments: Aorta, stomach and small intestine:
  SELiquidCompartment& cAortaVascular = bg.GetCompartments().CreateLiquidCompartment("cAortaVascular");
  cAortaVascular.GetVolume().SetValue(aortaVolume_mL, VolumeUnit::mL);
  SELiquidCompartment& cSmallIntestineChyme = bg.GetCompartments().CreateLiquidCompartment("cSmallIntestineChyme");
  cSmallIntestineChyme.GetVolume().SetValue(smallIntestineVolume_mL, VolumeUnit::mL);
  SELiquidCompartment& cStomach = bg.GetCompartments().CreateLiquidCompartment("cStomach");
  cStomach.GetVolume().SetValue(stomachVolume_mL, VolumeUnit::mL);
  SELiquidCompartment& cLymph = bg.GetCompartments().CreateLiquidCompartment("cLymph");
  cLymph.GetVolume().SetValue(lymphVolume_mL, VolumeUnit::mL);

  //Create the graph
  SELiquidCompartmentGraph& Graph = bg.GetCompartments().CreateLiquidGraph("Graph");
  Graph.AddCompartment(cAortaVascular);
  Graph.AddCompartment(cSmallIntestineChyme);
  Graph.AddCompartment(cLymph);

  SELiquidSubstanceQuantity* SmallIntestineChymeAminoAcids;
  SELiquidSubstanceQuantity* SmallIntestineChymeGlucose;
  SELiquidSubstanceQuantity* SmallIntestineChymeTriacylglycerol;
  SELiquidSubstanceQuantity* SmallIntestineChymeSodium;
  SELiquidSubstanceQuantity* AortaSodium;
  SELiquidSubstanceQuantity* AortaAminoAcids;
  SELiquidSubstanceQuantity* AortaGlucose;
  SELiquidSubstanceQuantity* AortaTriacylglycerol;
  SELiquidSubstanceQuantity* LymphTriacylglycerol;

  //substances
  SESubstance& aminoAcids = bg.GetSubstances().GetAminoAcids();
  SESubstance& glucose = bg.GetSubstances().GetGlucose();
  SESubstance& triacylglycerol = bg.GetSubstances().GetTriacylglycerol();
  SESubstance& sodium = bg.GetSubstances().GetSodium();

  bg.GetSubstances().AddActiveSubstance(aminoAcids);
  bg.GetSubstances().AddActiveSubstance(glucose);
  bg.GetSubstances().AddActiveSubstance(triacylglycerol);
  bg.GetSubstances().AddActiveSubstance(sodium);

  //set substance on intestine and aorta (initialize to 0 mass)
  cSmallIntestineChyme.GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.GetSubstanceQuantity(sodium)->GetMass().SetValue(sodiumBileMass_g, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(sodium)->GetMass().SetValue(aortaSodium_g, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
  cAortaVascular.GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
  cLymph.GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
  cSmallIntestineChyme.Balance(BalanceLiquidBy::Mass);
  cAortaVascular.Balance(BalanceLiquidBy::Mass);

  //small intestine nutrients/substances
  SmallIntestineChymeAminoAcids = cSmallIntestineChyme.GetSubstanceQuantity(aminoAcids);
  SmallIntestineChymeGlucose = cSmallIntestineChyme.GetSubstanceQuantity(glucose);
  SmallIntestineChymeTriacylglycerol = cSmallIntestineChyme.GetSubstanceQuantity(triacylglycerol);
  SmallIntestineChymeSodium = cSmallIntestineChyme.GetSubstanceQuantity(sodium);

  //aorta nutrients/substances
  AortaSodium = cAortaVascular.GetSubstanceQuantity(sodium);
  AortaTriacylglycerol = cAortaVascular.GetSubstanceQuantity(triacylglycerol);
  AortaAminoAcids = cAortaVascular.GetSubstanceQuantity(aminoAcids);
  AortaGlucose = cAortaVascular.GetSubstanceQuantity(glucose);

  //Lymph:
  LymphTriacylglycerol = cLymph.GetSubstanceQuantity(triacylglycerol);

  //Nutrition
  SENutrition nutrition(m_Logger);

  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 50.0;
  double runTime_hr = 5.0;
  double timeSteps = (runTime_hr * 3600.0) / deltaT_s;
  int j = 0;

  std::stringstream ss;

  //given the mealtype we will test digestion of each nutrient (this is base concentration, will increment value):
  switch (mealType) {
  case MealType::ProteinOnly:
    proteinMass_g = 30;
    break;
  case MealType::CarbsOnly:
    carbsMass_g = 30;
    break;
  case MealType::FatOnly:
    fatMass_g = 30;
    break;
  case MealType::Balanced: //https://www.bodybuilding.com/fun/ask-the-muscle-prof-what-are-the-ideal-macros-for-my-breakfast.html
    proteinMass_g = 40;
    carbsMass_g = 30;
    fatMass_g = 30;
    break;
  default:
    break;
  }

  //nutrition mass (this is how its done in-engine)
  nutrition.GetCarbohydrate().SetValue(carbsMass_g, MassUnit::g);
  nutrition.GetProtein().SetValue(proteinMass_g, MassUnit::g);
  nutrition.GetFat().SetValue(fatMass_g, MassUnit::g);

  //reset intestine masses:
  SmallIntestineChymeGlucose->GetMass().SetValue(0, MassUnit::mg);
  SmallIntestineChymeTriacylglycerol->GetMass().SetValue(0, MassUnit::mg);
  SmallIntestineChymeAminoAcids->GetMass().SetValue(0, MassUnit::mg);

  //compute concentrations in the stomach:
  double carbConcentration_mg_Per_ml = nutrition.GetCarbohydrate().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
  double carbConcentrationTemp = carbConcentration_mg_Per_ml;

  double proteinConcentration_mg_Per_ml = nutrition.GetProtein().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
  double proteinConcentrationTemp = proteinConcentration_mg_Per_ml;

  double fatConcentration_mg_Per_ml = nutrition.GetFat().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
  double fatConcentrationTemp = fatConcentration_mg_Per_ml;

  //for output compute the product due to enzyme kinetics and store (don't update):
  double glucoseProductTemp_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
  double aminoAcidsProductTemp_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
  double triacylglycerolProductTemp_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);

  double glucoseProduct_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
  double aminoAcidsProduct_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
  double triacylglycerolProduct_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);

  double carbMassDepleted_mg = glucoseProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);
  double proteinMassDepleted_mg = aminoAcidsProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);
  double fatMassDepleted_mg = triacylglycerolProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

  //calculate sodium secretion and absorption:
  double sodiumSecreted_g = sodiumSecretion_g_Per_h * deltaT_s;
  double sodiumAbsorbed_g = sodiumAbsorption_g_Per_h * deltaT_s;
  double triacylglycerolAbsorbed_mg = intestineFatAbsorption_mg_Per_s * deltaT_s;

  // 1 sodium = 2 glucose or 1 amino acid
  double glucoseAbsorbed_g = 2.0 * sodiumAbsorbed_g;
  double aminoAcidAbsorbed_g = sodiumAbsorbed_g;

  //quick check for concentration:
  //std::cout << "carbohydrate concentration: " << carbConcentration_mg_Per_ml << std::endl;
  //std::cout << "protein concentration: " << proteinConcentration_mg_Per_ml << std::endl;
  //std::cout << "fat concentration: " << fatConcentration_mg_Per_ml << std::endl;

  //check for mass on small intestine:
  std::cout << "glucose mass" << SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg) << std::endl;

  //reset timer:
  double timer_s = 0.0;

  //put the mass into the small intestine chyme compartment from digestion:
  while (j < timeSteps) {
    //for a given carbohydrate substrate concentration compute the product conversion time:
    //https://www.wolframalpha.com/input/?i=plot+p%3D4.11*x%2F(3.076+%2B+x)+for+x%3D10,70
    //Update each interaction:
    glucoseProduct_mg_Per_min = vmaxCarbohydrate_mg_Per_min * (carbConcentration_mg_Per_ml) / (kmCarbohydrate_mg + carbConcentration_mg_Per_ml);
    carbMassDepleted_mg = glucoseProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

    aminoAcidsProduct_mg_Per_min = vmaxProtein_mg_Per_min * (proteinConcentration_mg_Per_ml) / (kmProtein_mg + proteinConcentration_mg_Per_ml);
    proteinMassDepleted_mg = aminoAcidsProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

    triacylglycerolProduct_mg_Per_min = vmaxFat_mg_Per_min * (fatConcentration_mg_Per_ml) / (kmFat_mg + fatConcentration_mg_Per_ml);
    fatMassDepleted_mg = triacylglycerolProduct_mg_Per_min * deltaT_s; //*(1 / 60.0);

    //Add nutrients to chyme and decrement from nutrition
    if (nutrition.GetCarbohydrate().GetValue(MassUnit::mg) > 0) {
      SmallIntestineChymeGlucose->GetMass().IncrementValue(carbMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeGlucose->Balance(BalanceLiquidBy::Mass);
      nutrition.GetCarbohydrate().IncrementValue(-carbMassDepleted_mg, MassUnit::mg);
    } else {
      //std::cout << "no more carbs" << std::endl;
      nutrition.GetCarbohydrate().SetValue(0, MassUnit::mg);
    }

    if (nutrition.GetProtein().GetValue(MassUnit::mg) > 0) {
      SmallIntestineChymeAminoAcids->GetMass().IncrementValue(proteinMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeAminoAcids->Balance(BalanceLiquidBy::Mass);
      nutrition.GetProtein().IncrementValue(-proteinMassDepleted_mg, MassUnit::mg);
    } else {
      //std::cout << "no more protein" << std::endl;
      nutrition.GetProtein().SetValue(0, MassUnit::mg);
    }

    if (nutrition.GetFat().GetValue(MassUnit::mg) > 0) {
      SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(fatMassDepleted_mg, MassUnit::mg);
      SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);
      nutrition.GetFat().IncrementValue(-fatMassDepleted_mg, MassUnit::mg);
    } else {
      //std::cout << "no more fat" << std::endl;
      nutrition.GetFat().SetValue(0, MassUnit::mg);
    }

    //update concentration on the stomach
    carbConcentration_mg_Per_ml = nutrition.GetCarbohydrate().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
    proteinConcentration_mg_Per_ml = nutrition.GetProtein().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);
    fatConcentration_mg_Per_ml = nutrition.GetFat().GetValue(MassUnit::mg) / cStomach.GetVolume().GetValue(VolumeUnit::mL);

    //compute secretion and absorption as a function of glucose mass:
    if (mealType == CarbsOnly) {
      sodiumSecretion_g_Per_h = sodiumSecretionVmax_g_Per_h * (SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g) / (sodiumSecretionKm_g + SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g)));
      sodiumAbsorption_g_Per_h = sodiumAbsorptionVmax_g_Per_h * (SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g) / (sodiumAbsorptionKm_g + SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::g)));
    }

    if (mealType == ProteinOnly) {
      sodiumSecretion_g_Per_h = sodiumSecretionVmax_g_Per_h * (SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g) / (sodiumSecretionKm_g + SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g)));
      sodiumAbsorption_g_Per_h = sodiumAbsorptionVmax_g_Per_h * (SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g) / (sodiumAbsorptionKm_g + SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::g)));
    }

    //compute mass moved:
    sodiumSecreted_g = sodiumSecretion_g_Per_h * (1.0 / 3600.0) * deltaT_s;
    sodiumAbsorbed_g = sodiumAbsorption_g_Per_h * (1.0 / 3600.0) * deltaT_s;

    //compute nutrient absorption from new absorped values (same co-transporter assumption here):
    glucoseAbsorbed_g = 2.0 * sodiumAbsorbed_g;
    aminoAcidAbsorbed_g = sodiumAbsorbed_g;

    //absorption into the vasculature:
    //Secrected salt into the small intestine, simulate "bile" (20-30g/day secreted into small intestine, 5-8 g/day ingested)
    //Absorption of sodium: around 25-35 g/day, Guyton
    //secrete salt, check for aorta sodium first:
    if (AortaSodium->GetMass().GetValue(MassUnit::mg) > 0) {
      AortaSodium->GetMass().IncrementValue(-sodiumSecreted_g, MassUnit::g);
      SmallIntestineChymeSodium->GetMass().IncrementValue(sodiumSecreted_g, MassUnit::g);
      AortaSodium->Balance(BalanceLiquidBy::Mass);
      SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
    }
    //absorb and take glucose with you 1 mg sodium = 2mg glucose, 1mg sodium = 1mg protein
    if (mealType == CarbsOnly) {
      if (SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg) > 0 && SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::g) > sodiumAbsorbed_g) {
        SmallIntestineChymeSodium->GetMass().IncrementValue(-sodiumAbsorbed_g, MassUnit::g);
        AortaSodium->GetMass().IncrementValue(sodiumAbsorbed_g, MassUnit::g);
        SmallIntestineChymeGlucose->GetMass().IncrementValue(-glucoseAbsorbed_g, MassUnit::g);
        AortaGlucose->GetMass().IncrementValue(glucoseAbsorbed_g, MassUnit::g);

        AortaSodium->Balance(BalanceLiquidBy::Mass);
        AortaGlucose->Balance(BalanceLiquidBy::Mass);
        SmallIntestineChymeGlucose->Balance(BalanceLiquidBy::Mass);
        SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
      }
    }
    if (mealType == ProteinOnly) {
      if (SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::mg) > 0 && SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::g) > sodiumAbsorbed_g) {
        //std::cout << "difference between amount in chyme and amount absorped" << SmallIntestineChymeSodium->GetMass().GetValue(MassUnit::mg) - sodiumAbsorbed_g << std::endl;
        SmallIntestineChymeSodium->GetMass().IncrementValue(-sodiumAbsorbed_g, MassUnit::g);
        AortaSodium->GetMass().IncrementValue(sodiumAbsorbed_g, MassUnit::g);
        SmallIntestineChymeAminoAcids->GetMass().IncrementValue(-aminoAcidAbsorbed_g, MassUnit::g);
        AortaAminoAcids->GetMass().IncrementValue(aminoAcidAbsorbed_g, MassUnit::g);

        AortaSodium->Balance(BalanceLiquidBy::Mass);
        AortaAminoAcids->Balance(BalanceLiquidBy::Mass);
        SmallIntestineChymeAminoAcids->Balance(BalanceLiquidBy::Mass);
        SmallIntestineChymeSodium->Balance(BalanceLiquidBy::Mass);
      }
    }
    if (mealType == FatOnly) {
      //dont check sodium here (transport not dependant upon it)
      if (SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::mg) > triacylglycerolAbsorbed_mg) {
        SmallIntestineChymeTriacylglycerol->GetMass().IncrementValue(-triacylglycerolAbsorbed_mg, MassUnit::mg);
        LymphTriacylglycerol->GetMass().IncrementValue(triacylglycerolAbsorbed_mg, MassUnit::mg);

        LymphTriacylglycerol->Balance(BalanceLiquidBy::Mass);
        SmallIntestineChymeTriacylglycerol->Balance(BalanceLiquidBy::Mass);
        //std::cout << "lymph triacylglycerol mg: " << LymphTriacylglycerol->GetMass().GetValue(MassUnit::mg);
      }
    }

    if (mealType == CarbsOnly) {
      trk.Track("sodiumConcentrationAorta_mg_Per_L", timer_s, AortaSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("sodiumConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("glucoseConcentrationAorta_mg_Per_L", timer_s, AortaGlucose->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("glucoseConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeGlucose->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("glucoseMassAbsorbedAorta_mg", timer_s, AortaGlucose->GetMass().GetValue(MassUnit::mg));
      trk.Track("glucsoseMassSmallIntestine_mg", timer_s, SmallIntestineChymeGlucose->GetMass().GetValue(MassUnit::mg));
    }

    if (mealType == ProteinOnly) {
      trk.Track("sodiumConcentrationAorta_mg_Per_L", timer_s, AortaSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("sodiumConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("AminoAcidConcentrationAorta_mg_Per_L", timer_s, AortaAminoAcids->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("AminoAcidConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeGlucose->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("AminoAcidMassAbsorbedAorta_mg", timer_s, AortaAminoAcids->GetMass().GetValue(MassUnit::mg));
      trk.Track("AminoAcideMassSmallIntestine_mg", timer_s, SmallIntestineChymeAminoAcids->GetMass().GetValue(MassUnit::mg));
    }

    if (mealType == FatOnly) {
      trk.Track("TriacylglycerolConcentrationSmallIntestine_mg_Per_L", timer_s, SmallIntestineChymeTriacylglycerol->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("TriacylglycerolConcentrationSmallIntestine_mg_Per_L", timer_s, LymphTriacylglycerol->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L));
      trk.Track("TriacylglycerolMassAbsorbedLymph_mg", timer_s, LymphTriacylglycerol->GetMass().GetValue(MassUnit::mg));
      trk.Track("TriacylglyceroleMassSmallIntestine_mg", timer_s, SmallIntestineChymeTriacylglycerol->GetMass().GetValue(MassUnit::mg));
    }
    if (j == 0)
      trk.CreateFile(std::string(rptDirectory + outputName + ".csv").c_str(), file);
    trk.StreamTrackToFile(file);

    //update timer s
    timer_s += deltaT_s;
    time += deltaT_s;
    j += 1;
  }

  file.close();
}

void BioGearsEngineTest::DigestionCarbs(const std::string& sOutputDirectory)
{
  DigestionTest(sOutputDirectory, MealType::CarbsOnly);
}

void BioGearsEngineTest::DigestionProtein(const std::string& sOutputDirectory)
{
  DigestionTest(sOutputDirectory, MealType::ProteinOnly);
}

void BioGearsEngineTest::DigestionFat(const std::string& sOutputDirectory)
{
  DigestionTest(sOutputDirectory, MealType::FatOnly);
}

void BioGearsEngineTest::DigestionMixed(const std::string& sOutputDirectory)
{
  DigestionTest(sOutputDirectory, MealType::Balanced);
}

void BioGearsEngineTest::AbsorptionCarbs(const std::string& sOutputDirectory)
{
  AbsorptionTest(sOutputDirectory, MealType::CarbsOnly);
}

void BioGearsEngineTest::AbsorptionProtein(const std::string& sOutputDirectory)
{
  AbsorptionTest(sOutputDirectory, MealType::ProteinOnly);
}

void BioGearsEngineTest::AbsorptionFat(const std::string& sOutputDirectory)
{
  AbsorptionTest(sOutputDirectory, MealType::FatOnly);
}
}