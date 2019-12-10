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
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/GeneralMath.h>

namespace biogears {
void BioGearsEngineTest::NutrientKineticsTest(bool usingAbsorption, bool usingDynamicHormones, bool usingGlycogen, bool usingProteinStorage, bool usingFatStorage, bool fullStores, bool useDiffusion, bool useConsumption, bool usingLipogenesis, bool usingGluconeogenesis, bool isAnaerobic, double exerciseWork_W, const std::string& rptDirectory, double testDuration_hr, MealType mealType, std::string testName)
{
  DataTrack trk;
  std::string outputName;
  if (testName != "Custom") {
    outputName = "/" + testName;
  } else {
    outputName = "/CustomNutrientKineticsTest";
  }

  m_Logger->ResetLogFile(rptDirectory + outputName + ".log");
  std::ofstream file;
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();
  Hepatic& hptc = (Hepatic&)bg.GetHepatic();
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();

  //Create a circuit
  double total_flow_mL_Per_min = 5600.0;

  //Assign volumes (compartment volumes are vascular values we see in engine, except aorta and vena cava, which just lump all other volume together)
  double totalVolume_mL = 5600;
  double smallIntestineVolume_mL = 219.8;
  double liverVolume_mL = 587;
  double kidneysVolume_mL = 79;
  double muscleVolume_mL = 782.6;
  double fatVolume_mL = 280;
  double brainVolume_mL = 66;
  double aortaVolume_mL = (totalVolume_mL - smallIntestineVolume_mL - liverVolume_mL - kidneysVolume_mL - muscleVolume_mL - fatVolume_mL - brainVolume_mL) / 2;
  double venaCavaVolume_mL = aortaVolume_mL;

  //Flows are relative flows based on engine inflows
  double smallIntestineFlow_mL_per_min = (9 / 87.3) * total_flow_mL_Per_min;
  double liverFlow_mL_per_min = (24.2 / 87.3) * total_flow_mL_Per_min;
  double kidneysFlow_mL_per_min = (22.9 / 87.3) * total_flow_mL_Per_min;
  double muscleFlow_mL_per_min = (15.6 / 87.3) * total_flow_mL_Per_min;
  double fatFlow_mL_per_min = (4.7 / 87.3) * total_flow_mL_Per_min;
  double brainFlow_mL_per_min = (10.9 / 87.3) * total_flow_mL_Per_min;

  //double smallIntestineFlow_mL_per_min = 9*60;
  //double liverFlow_mL_per_min = 24.2*60;
  //double kidneysFlow_mL_per_min = 22.9*60;
  //double muscleFlow_mL_per_min = 15.6*60;
  //double fatFlow_mL_per_min = 4.7*60;
  //double brainFlow_mL_per_min = 10.9*60;

  //Targets and initialization
  double patientWeight_kg = 77.1107;
  double insulinBaseline_pmol_Per_L = 118.1;
  double glucagonBaseline_pg_Per_mL = 70;
  double aminoAcidsBaseline_mg_Per_dL = 50;
  double glucoseBaseline_mg_Per_dL = 90;
  double ketonesBaseline_mg_Per_dL = 2.04;
  double triacylglycerolBaseline_mg_Per_dL = 75; //Guyton says 15 mg/dL for FFA bound to albumin, but Leeuwen says <150 mg/dL for normal blood triglycerides
  double maxLiverGlycogen_g = 97.5; //Glycogen can make up 5-8% of liver's weight, and average liver is 1.5 kg, so max glycogen we'll store is 97.5 g (guyton)
  double maxMuscleGlycogen_g = .35 * patientWeight_kg * 1000 * .02; //Glycogen in muscles can make up 1-3% of their weight, but this glycogen can't diffuse out of the muscle (guyton); Average male has ~35% muscle mass, giving ~539g glycogen
  //Some excess amino acids diffuse into tissues and combine into protein chains and can be broken down if needed, much like glycogen for glucose
  //Sources say this occurs in kidneys, liver, and/or visceral tissue; we'll put it in muscle since it has a large extracellular volume and so it doesn't
  //interfere with liver blood concentrations
  //"Reusable" protein stores are usually about 1% of total body protein, ~110 g (https://www.nap.edu/read/10490/chapter/12#595)
  double maxStoredProtein_g = 110;
  double initialStoredFat_g = .21 * patientWeight_kg * 1000; //Standard male has 21% body fat
  double BMR_kcal_day = 88.632 + 13.397 * patientWeight_kg + 4.799 * 177 - 5.677 * 44; //Harris Benedict formula w/ 177 cm, 44 yr old male

  //Some cumulative values for tracking
  double totalTAGFormedByLipogenesis_g = 0;
  double totalGlucoseFromLactate_g = 0;
  double totalGlucoseFromAA_g = 0;
  double totalGlucoseFromTAG_g = 0;
  double totalKetonesFromTAG_g = 0;
  double totalLactateFromGlucose = 0;

  //Total Masses (g) for permeability calculation (using typical male); from BioGears::SetupTissue()
  std::vector<double> tissueTotalMasses;
  tissueTotalMasses.push_back(1800); //liver
  tissueTotalMasses.push_back(1405); //brain
  tissueTotalMasses.push_back(155 * 2); //kidneys
  tissueTotalMasses.push_back(29000); //muscle
  tissueTotalMasses.push_back(initialStoredFat_g); //fat
  //tissueTotalMasses.push_back(10500 + 1020 + 250*2 + 330 + 3300 + 150);  //other

  SECircuitManager circuits(m_Logger);
  SEFluidCircuit* Circuit = &circuits.CreateFluidCircuit("Circuit");

  SEFluidCircuitNode& nAorta = Circuit->CreateNode("Aorta");
  nAorta.GetVolumeBaseline().SetValue(aortaVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nSmallIntestine = Circuit->CreateNode("SmallIntestine");
  nSmallIntestine.GetVolumeBaseline().SetValue(smallIntestineVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nLiver = Circuit->CreateNode("Liver");
  nLiver.GetVolumeBaseline().SetValue(liverVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nKidneys = Circuit->CreateNode("Kidneys");
  nKidneys.GetVolumeBaseline().SetValue(kidneysVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nMuscle = Circuit->CreateNode("Muscle");
  nMuscle.GetVolumeBaseline().SetValue(muscleVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nFat = Circuit->CreateNode("Fat");
  nFat.GetVolumeBaseline().SetValue(fatVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nBrain = Circuit->CreateNode("Brain");
  nBrain.GetVolumeBaseline().SetValue(brainVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nVenaCava = Circuit->CreateNode("VenaCava");
  nVenaCava.GetVolumeBaseline().SetValue(venaCavaVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nGround = Circuit->CreateNode("Ground");
  Circuit->AddReferenceNode(nGround);
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& pAortaToSmallIntestine = Circuit->CreatePath(nAorta, nSmallIntestine, "AortaToSmallIntestine");
  pAortaToSmallIntestine.GetFlowSourceBaseline().SetValue(smallIntestineFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pAortaToLiver = Circuit->CreatePath(nAorta, nLiver, "AortaToLiver");
  pAortaToLiver.GetFlowSourceBaseline().SetValue(liverFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pAortaToKidneys = Circuit->CreatePath(nAorta, nKidneys, "AortaToKidneys");
  pAortaToKidneys.GetFlowSourceBaseline().SetValue(kidneysFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pAortaToMuscle = Circuit->CreatePath(nAorta, nMuscle, "AortaToMuscle");
  pAortaToMuscle.GetFlowSourceBaseline().SetValue(muscleFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pAortaToFat = Circuit->CreatePath(nAorta, nFat, "AortaToFat");
  pAortaToFat.GetFlowSourceBaseline().SetValue(fatFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pAortaToBrain = Circuit->CreatePath(nAorta, nBrain, "AortaToBrain");
  pAortaToBrain.GetFlowSourceBaseline().SetValue(brainFlow_mL_per_min, VolumePerTimeUnit::mL_Per_min);

  SEFluidCircuitPath& pSmallIntestineToVenaCava = Circuit->CreatePath(nSmallIntestine, nVenaCava, "SmallIntestineToVenaCava");
  SEFluidCircuitPath& pLiverToVenaCava = Circuit->CreatePath(nLiver, nVenaCava, "LiverToVenaCava");
  SEFluidCircuitPath& pKidneysToVenaCava = Circuit->CreatePath(nKidneys, nVenaCava, "KidneysToVenaCava");
  SEFluidCircuitPath& pMuscleToVenaCava = Circuit->CreatePath(nMuscle, nVenaCava, "MuscleToVenaCava");
  SEFluidCircuitPath& pFatToVenaCava = Circuit->CreatePath(nFat, nVenaCava, "FatToVenaCava");
  SEFluidCircuitPath& pBrainToVenaCava = Circuit->CreatePath(nBrain, nVenaCava, "BrainToVenaCava");
  SEFluidCircuitPath& pVenaCavaToAorta = Circuit->CreatePath(nVenaCava, nAorta, "VenaCavaToAorta");
  ;

  SEFluidCircuitPath& pGroundToVenaCava = Circuit->CreatePath(nGround, nVenaCava, "GroundToVenaCava");

  Circuit->SetNextAndCurrentFromBaselines();
  Circuit->StateChange();

  //Vascular compartments
  SELiquidCompartment* cAortaVascular = &bg.GetCompartments().CreateLiquidCompartment("cAortaVascular");
  cAortaVascular->MapNode(nAorta);
  SELiquidCompartment* cSmallIntestineVascular = &bg.GetCompartments().CreateLiquidCompartment("cSmallIntestineVascular");
  cSmallIntestineVascular->MapNode(nSmallIntestine);
  SELiquidCompartment* cLiverVascular = &bg.GetCompartments().CreateLiquidCompartment("cLiverVascular");
  cLiverVascular->MapNode(nLiver);
  SELiquidCompartment* cKidneysVascular = &bg.GetCompartments().CreateLiquidCompartment("cKidneysVascular");
  cKidneysVascular->MapNode(nKidneys);
  SELiquidCompartment* cMuscleVascular = &bg.GetCompartments().CreateLiquidCompartment("cMuscleVascular");
  cMuscleVascular->MapNode(nMuscle);
  SELiquidCompartment* cFatVascular = &bg.GetCompartments().CreateLiquidCompartment("cFatVascular");
  cFatVascular->MapNode(nFat);
  SELiquidCompartment* cBrainVascular = &bg.GetCompartments().CreateLiquidCompartment("cBrainVascular");
  cBrainVascular->MapNode(nBrain);
  SELiquidCompartment* cVenaCavaVascular = &bg.GetCompartments().CreateLiquidCompartment("cVenaCavaVascular");
  cVenaCavaVascular->MapNode(nVenaCava);

  //Non-vascular compartments
  //Only brain and muscle will have intracellular for consumption
  SELiquidCompartment* cSmallIntestineChyme = &bg.GetCompartments().CreateLiquidCompartment("cSmallIntestineChyme");
  double chymeVolume_mL = 287.6; //gut extracellular expected volume (all gut, not just small intestine, but it shouldn't matter since we're using active diffusion)
  cSmallIntestineChyme->GetVolume().SetValue(chymeVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cLiverExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cLiverExtracellular");
  double liverExtracellularVolume_mL = 289.8; //liver extracellular expected volume
  cLiverExtracellular->GetVolume().SetValue(liverExtracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cBrainExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cBrainExtracellular");
  double brainExtracellularVolume_mL = 234.9; //brain extracellular expected volume
  cBrainExtracellular->GetVolume().SetValue(brainExtracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cBrainIntracellular = &bg.GetCompartments().CreateLiquidCompartment("cBrainIntracellular");
  double brainIntracellularVolume_mL = 899; //brain intracellular expected volume
  cBrainIntracellular->GetVolume().SetValue(brainIntracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cKidneysExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cKidneysExtracellular");
  double kidneysExtracellularVolume_mL = 42.3 * 2; //kidneys extracellular expected volume
  cKidneysExtracellular->GetVolume().SetValue(kidneysExtracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cMuscleExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cMuscleExtracellular");
  double muscleExtracellularVolume_mL = 3422; //muscle extracellular expected volume
  cMuscleExtracellular->GetVolume().SetValue(muscleExtracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cMuscleIntracellular = &bg.GetCompartments().CreateLiquidCompartment("cMuscleIntracellular");
  double muscleIntracellularVolume_mL = 18270; //muscle intracellular expected volume
  cMuscleIntracellular->GetVolume().SetValue(muscleIntracellularVolume_mL, VolumeUnit::mL);
  SELiquidCompartment* cFatExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cFatExtracellular");
  double fatExtracellularVolume_mL = 2127.6; //fat extracellular expected volume
  cFatExtracellular->GetVolume().SetValue(fatExtracellularVolume_mL, VolumeUnit::mL);

  //SELiquidCompartment* cOtherIntracellular = &bg.GetCompartments().CreateLiquidCompartment("cOtherIntracellular");
  //double otherIntracellularVolume_mL = 0;  //intracellular expected volume for all other tissues not specifically modeled in the unit test
  //cOtherIntracellular->GetVolume().SetValue(muscleIntracellularVolume_mL, VolumeUnit::mL);
  //SELiquidCompartment* cOtherExtracellular = &bg.GetCompartments().CreateLiquidCompartment("cOtherExtracellular");
  //double otherExtracellularVolume_mL = 0;  //extracellular expected volume for all other tissues not specifically modeled in the unit test
  //cOtherExtracellular->GetVolume().SetValue(otherExtracellularVolume_mL, VolumeUnit::mL);

  //Create the links
  SELiquidCompartmentLink& lAortaToSmallIntestine = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cSmallIntestineVascular, "lAortaToSmallIntestine");
  lAortaToSmallIntestine.MapPath(pAortaToSmallIntestine);
  SELiquidCompartmentLink& lAortaToLiver = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cLiverVascular, "lAortaToLiver");
  lAortaToLiver.MapPath(pAortaToLiver);
  SELiquidCompartmentLink& lAortaToKidneys = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cKidneysVascular, "lAortaToKidneys");
  lAortaToKidneys.MapPath(pAortaToKidneys);
  SELiquidCompartmentLink& lAortaToMuscle = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cMuscleVascular, "lAortaToMuscle");
  lAortaToMuscle.MapPath(pAortaToMuscle);
  SELiquidCompartmentLink& lAortaToFat = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cFatVascular, "lAortaToFat");
  lAortaToFat.MapPath(pAortaToFat);
  SELiquidCompartmentLink& lAortaToBrain = bg.GetCompartments().CreateLiquidLink(*cAortaVascular, *cBrainVascular, "lAortaToBrain");
  lAortaToBrain.MapPath(pAortaToBrain);

  SELiquidCompartmentLink& lSmallIntestineToVenaCava = bg.GetCompartments().CreateLiquidLink(*cSmallIntestineVascular, *cVenaCavaVascular, "lSmallIntestineToVenaCava");
  lSmallIntestineToVenaCava.MapPath(pSmallIntestineToVenaCava);
  SELiquidCompartmentLink& lLiverToVenaCava = bg.GetCompartments().CreateLiquidLink(*cLiverVascular, *cVenaCavaVascular, "lLiverToVenaCava");
  lLiverToVenaCava.MapPath(pLiverToVenaCava);
  SELiquidCompartmentLink& lKidneysToVenaCava = bg.GetCompartments().CreateLiquidLink(*cKidneysVascular, *cVenaCavaVascular, "lKidneysToVenaCava");
  lKidneysToVenaCava.MapPath(pKidneysToVenaCava);
  SELiquidCompartmentLink& lMuscleToVenaCava = bg.GetCompartments().CreateLiquidLink(*cMuscleVascular, *cVenaCavaVascular, "lMuscleToVenaCava");
  lMuscleToVenaCava.MapPath(pMuscleToVenaCava);
  SELiquidCompartmentLink& lFatToVenaCava = bg.GetCompartments().CreateLiquidLink(*cFatVascular, *cVenaCavaVascular, "lFatToVenaCava");
  lFatToVenaCava.MapPath(pFatToVenaCava);
  SELiquidCompartmentLink& lBrainToVenaCava = bg.GetCompartments().CreateLiquidLink(*cBrainVascular, *cVenaCavaVascular, "lBrainToVenaCava");
  lBrainToVenaCava.MapPath(pBrainToVenaCava);

  SELiquidCompartmentLink& lVenaCavaToAorta = bg.GetCompartments().CreateLiquidLink(*cVenaCavaVascular, *cAortaVascular, "lVenaCavaToAorta");
  lVenaCavaToAorta.MapPath(pVenaCavaToAorta);

  //Create the graph
  SELiquidCompartmentGraph& Graph = bg.GetCompartments().CreateLiquidGraph("Graph");
  Graph.AddCompartment(*cAortaVascular);
  Graph.AddCompartment(*cSmallIntestineVascular);
  Graph.AddCompartment(*cLiverVascular);
  Graph.AddCompartment(*cKidneysVascular);
  Graph.AddCompartment(*cMuscleVascular);
  Graph.AddCompartment(*cFatVascular);
  Graph.AddCompartment(*cBrainVascular);
  Graph.AddCompartment(*cVenaCavaVascular);
  Graph.AddLink(lAortaToSmallIntestine);
  Graph.AddLink(lAortaToLiver);
  Graph.AddLink(lAortaToKidneys);
  Graph.AddLink(lAortaToMuscle);
  Graph.AddLink(lAortaToFat);
  Graph.AddLink(lAortaToBrain);
  Graph.AddLink(lSmallIntestineToVenaCava);
  Graph.AddLink(lLiverToVenaCava);
  Graph.AddLink(lKidneysToVenaCava);
  Graph.AddLink(lMuscleToVenaCava);
  Graph.AddLink(lFatToVenaCava);
  Graph.AddLink(lBrainToVenaCava);
  Graph.AddLink(lVenaCavaToAorta);
  Graph.StateChange();

  //Use some vectors to store refs to intracellular, extracellular, and vascular compartments, kind of similar to the map in Tissue::SetUp()
  //This will let us loop through tissues for diffusion
  std::vector<SELiquidCompartment*> vascularCompartments;
  vascularCompartments.push_back(cLiverVascular);
  vascularCompartments.push_back(cBrainVascular);
  vascularCompartments.push_back(cKidneysVascular);
  vascularCompartments.push_back(cMuscleVascular);
  vascularCompartments.push_back(cFatVascular);
  //vascularCompartments.push_back(cOtherVascular);
  std::vector<SELiquidCompartment*> extracellularCompartments;
  extracellularCompartments.push_back(cLiverExtracellular);
  extracellularCompartments.push_back(cBrainExtracellular);
  extracellularCompartments.push_back(cKidneysExtracellular);
  extracellularCompartments.push_back(cMuscleExtracellular);
  extracellularCompartments.push_back(cFatExtracellular);
  //extracellularCompartments.push_back(cOtherExtracellular);
  std::vector<SELiquidCompartment*> intracellularCompartments;
  //intracellularCompartments.push_back(cLiverIntracellular);
  intracellularCompartments.push_back(cBrainIntracellular);
  //intracellularCompartments.push_back(cKidneysIntracellular);
  intracellularCompartments.push_back(cMuscleIntracellular);
  //intracellularCompartments.push_back(cFatIntracellular);
  //vascularCompartments.push_back(cOtherVascular);

  //Initialize substances
  SESubstance& aminoAcids = bg.GetSubstances().GetAminoAcids();
  SESubstance& glucose = bg.GetSubstances().GetGlucose();
  SESubstance& triacylglycerol = bg.GetSubstances().GetTriacylglycerol();
  SESubstance& insulin = bg.GetSubstances().GetInsulin();
  SESubstance& glucagon = bg.GetSubstances().GetGlucagon();
  SESubstance& lactate = bg.GetSubstances().GetLactate();
  SESubstance& ketones = bg.GetSubstances().GetKetones();
  SESubstance& creatinine = bg.GetSubstances().GetCreatinine();
  SESubstance& urea = bg.GetSubstances().GetUrea();

  bg.GetSubstances().AddActiveSubstance(aminoAcids);
  bg.GetSubstances().AddActiveSubstance(glucose);
  bg.GetSubstances().AddActiveSubstance(triacylglycerol);
  bg.GetSubstances().AddActiveSubstance(insulin);
  bg.GetSubstances().AddActiveSubstance(glucagon);
  bg.GetSubstances().AddActiveSubstance(lactate);
  bg.GetSubstances().AddActiveSubstance(ketones);
  bg.GetSubstances().AddActiveSubstance(creatinine);
  bg.GetSubstances().AddActiveSubstance(urea);

  //Initialize vascular compartments
  for (SELiquidCompartment* c : Graph.GetCompartments()) {
    c->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * aminoAcidsBaseline_mg_Per_dL, MassUnit::mg); //guyton
    c->GetSubstanceQuantity(glucose)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * glucoseBaseline_mg_Per_dL, MassUnit::mg); //guyton
    c->GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * triacylglycerolBaseline_mg_Per_dL, MassUnit::mg); //leeuwen
    c->GetSubstanceQuantity(glucagon)->GetMass().SetValue(c->GetVolume(VolumeUnit::mL) * glucagonBaseline_pg_Per_mL * 1e-9, MassUnit::mg); //leeuwen, claessens
    c->GetSubstanceQuantity(lactate)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 13, MassUnit::mg); //leeuwen
    c->GetSubstanceQuantity(ketones)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * ketonesBaseline_mg_Per_dL, MassUnit::mg); //.2 mmol
    c->GetSubstanceQuantity(creatinine)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * .91, MassUnit::mg); //leeuwen
    c->GetSubstanceQuantity(urea)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 18.5, MassUnit::mg); //leeuwen (BUN)
    c->Balance(BalanceLiquidBy::Mass);
    c->GetSubstanceQuantity(insulin)->GetMolarity().SetValue(insulinBaseline_pmol_Per_L * 1e-9, AmountPerVolumeUnit::mmol_Per_L); //leeuwen
    c->Balance(BalanceLiquidBy::Molarity);
  }

  //Initialize tissue compartments
  //No insulin or glucagon should be in tissues
  for (SELiquidCompartment* c : extracellularCompartments) {
    c->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * aminoAcidsBaseline_mg_Per_dL, MassUnit::mg);
    c->GetSubstanceQuantity(glucose)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * glucoseBaseline_mg_Per_dL, MassUnit::mg);
    if (c != cBrainExtracellular) //TAG can't cross blood-brain barrier
      c->GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * triacylglycerolBaseline_mg_Per_dL, MassUnit::mg);
    c->GetSubstanceQuantity(glucagon)->GetMass().SetValue(0, MassUnit::mg);
    c->GetSubstanceQuantity(lactate)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 13, MassUnit::mg);
    c->GetSubstanceQuantity(ketones)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * ketonesBaseline_mg_Per_dL, MassUnit::mg); //.2 mmol
    c->GetSubstanceQuantity(creatinine)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * .91, MassUnit::mg);
    c->GetSubstanceQuantity(urea)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 18.5, MassUnit::mg);
    c->Balance(BalanceLiquidBy::Mass);
    c->GetSubstanceQuantity(insulin)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mmol_Per_L);
    c->Balance(BalanceLiquidBy::Molarity);
  }
  for (SELiquidCompartment* c : intracellularCompartments) {
    c->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * aminoAcidsBaseline_mg_Per_dL, MassUnit::mg);
    c->GetSubstanceQuantity(glucose)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * glucoseBaseline_mg_Per_dL, MassUnit::mg);
    if (c != cBrainIntracellular) //TAG can't cross blood-brain barrier
      c->GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * triacylglycerolBaseline_mg_Per_dL, MassUnit::mg);
    c->GetSubstanceQuantity(glucagon)->GetMass().SetValue(0, MassUnit::mg);
    c->GetSubstanceQuantity(lactate)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 13, MassUnit::mg);
    c->GetSubstanceQuantity(ketones)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * ketonesBaseline_mg_Per_dL, MassUnit::mg); //.2 mmol
    c->GetSubstanceQuantity(creatinine)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * .91, MassUnit::mg);
    c->GetSubstanceQuantity(urea)->GetMass().SetValue(c->GetVolume(VolumeUnit::dL) * 18.5, MassUnit::mg);
    c->Balance(BalanceLiquidBy::Mass);
    c->GetSubstanceQuantity(insulin)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mmol_Per_L);
    c->Balance(BalanceLiquidBy::Molarity);
  }

  //Initialize nutrient stores to full, if desired
  double liverGlycogen_g = 0;
  double muscleGlycogen_g = 0;
  double storedProtein_g = 0;
  double storedFat_g = initialStoredFat_g; //since fat doesn't have a max, we can keep it accurate at all times

  if (fullStores) {
    liverGlycogen_g = maxLiverGlycogen_g;
    muscleGlycogen_g = maxMuscleGlycogen_g;
    storedProtein_g = maxStoredProtein_g;
  }

  //Execution parameters
  int trackSkipper = 10; //only track every nth data point to cut down on file size
  double time = 0;
  double deltaT_s = 1.0 / 50.0;
  bool fed = false;
  std::stringstream ss;

  for (unsigned int i = 0; i < testDuration_hr * 3600.0 / deltaT_s; i++) {
    //PreProcess - Absorption, Storage/Breakdown, Diffusion, Energy Consumption

    //Meal is eaten after 30 seconds; put nutrients in chyme
    if (usingAbsorption && time > 30 && !fed) {
      double proteinMass_g = 0;
      double carbsMass_g = 0;
      double fatMass_g = 0;

      switch (mealType) {
      case MealType::ProteinOnly:
        proteinMass_g = 75;
        break;
      case MealType::CarbsOnly:
        carbsMass_g = 75; //to match with Leeuwen (75); 637 will fill all glycogen from empty
        break;
      case MealType::FatOnly:
        fatMass_g = 75;
        break;
      case MealType::Balanced: //https://www.bodybuilding.com/fun/ask-the-muscle-prof-what-are-the-ideal-macros-for-my-breakfast.html
        proteinMass_g = 40;
        carbsMass_g = 30;
        fatMass_g = 30;
        break;
      default:
        break;
      }

      cSmallIntestineChyme->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(proteinMass_g, MassUnit::g);
      cSmallIntestineChyme->GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(fatMass_g, MassUnit::g);
      cSmallIntestineChyme->GetSubstanceQuantity(glucose)->GetMass().SetValue(carbsMass_g, MassUnit::g);
      cSmallIntestineChyme->Balance(BalanceLiquidBy::Mass);

      fed = true;
    }

    //Insulin has a validated production rate and a BS clearance rate to get it stable (Note: Guyton says insulin production at 90 mg/dL glucose concentration should be
    // 25 ng/min/kg, which is about 300 pmol/min, double what we have using this curve from Tolic; also, we won't capture insulin behavior for very high glucose concentrations, see Guyton p 991)
    //Glucagon has a validated clearance rate and a BS production rate to get it stable
    //When testing response to meals, if we want faster response, increase production rates and clearance, maintaining stability
    //If response is too fast, slow them (this unit test would be good for that)
    if (usingDynamicHormones) {
      //Synthesize insulin and glucagon and release in vena cava (mimics Endocrine::SynthesizeInsulin() and Endocrine::SynthesizeGlucagon())
      double bloodGlucoseConcentration_g_Per_L = cAortaVascular->GetSubstanceQuantity(glucose)->GetConcentration(MassPerVolumeUnit::g_Per_L);
      // 2.0 = glucose upperConcentration_g_Per_L
      // 0.3 = glucose lowerConcentration_g_Per_l
      // 65.421 = insulin production amplitudeRate_mU_Per_min
      // 6.67 = insulinConversionToAmount_pmol_Per_mU

      //https://www.wolframalpha.com/input/?i=y%3D(6.67+*+65.421)+%2F+(1.0+%2B+exp((2.0+-+2.0*x)+%2F+0.3))+from+0%3Cx%3C1.5
      double insulinSynthesisRate_pmol_Per_min = 6.67 * 65.421 / (1.0 + exp((2.0 - 2.0 * bloodGlucoseConcentration_g_Per_L) / 0.3));
      double insulinMassDelta_g = Convert(insulinSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min, AmountPerTimeUnit::mol_Per_s);
      insulinMassDelta_g *= insulin.GetMolarMass(MassPerAmountUnit::g_Per_mol) * deltaT_s;

      cVenaCavaVascular->GetSubstanceQuantity(insulin)->GetMass().IncrementValue(insulinMassDelta_g, MassUnit::g);

      //Used insulin as a guideline for determining glucagon synthesis; want to be stable at 9 ml/min kg clearance, which is from
      //https://www.ncbi.nlm.nih.gov/pubmed/773949
      double glucagonSynthesisRate_pmol_Per_min = 21 - (21 / (1.0 + exp((2 - 2 * bloodGlucoseConcentration_g_Per_L) / .3)));
      double glucagonMassDelta_g = Convert(glucagonSynthesisRate_pmol_Per_min, AmountPerTimeUnit::pmol_Per_min, AmountPerTimeUnit::mol_Per_s);
      glucagonMassDelta_g *= glucagon.GetMolarMass(MassPerAmountUnit::g_Per_mol) * deltaT_s;

      cVenaCavaVascular->GetSubstanceQuantity(glucagon)->GetMass().IncrementValue(glucagonMassDelta_g, MassUnit::g);

      cVenaCavaVascular->Balance(BalanceLiquidBy::Mass);

      //Clear Insulin (current stable clearance value from BioGears.xlsx is 5 mL/min kg, but it's not stable here, so up it)
      double insulinVolumeCleared_mL = (16.5 / 60) * patientWeight_kg * deltaT_s;
      bg.GetSubstances().CalculateGenericClearance(insulinVolumeCleared_mL, *cVenaCavaVascular, insulin);

      //Clear Glucagon
      double glucagonVolumeCleared_mL = (9.0 / 60) * patientWeight_kg * deltaT_s;
      bg.GetSubstances().CalculateGenericClearance(glucagonVolumeCleared_mL, *cVenaCavaVascular, glucagon);
    }

    //Get hormone factor for use in other Preprocess steps
    double hormoneFactor = 0;

    //Nutrient storage handling
    if (usingGlycogen) {
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cLiverVascular->GetSubstanceQuantity(insulin), cLiverVascular->GetSubstanceQuantity(glucagon), bg);
      if (i % trackSkipper == 0)
        trk.Track("LiverHormoneFactor", time, hormoneFactor);
      //double insulinDeviation = (cLiverVascular.GetSubstanceQuantity(insulin)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L)*1e9 - insulinBaseline_pmol_Per_L) / insulinBaseline_pmol_Per_L;
      //double glucagonDeviation = (cLiverVascular.GetSubstanceQuantity(glucagon)->GetConcentration(MassPerVolumeUnit::mg_Per_mL)*1e9 - glucagonBaseline_pg_Per_mL) / glucagonBaseline_pg_Per_mL;
      //trk.Track("InsulinDeviation", time, insulinDeviation);
      //trk.Track("GlucagonDeviation", time, glucagonDeviation);

      //Normally, 2% of glycogen is regenerated per hour after exercise, but can reach 5%
      //This is for muscle and liver glycogen, but we'll assume they are created at the same rates
      //http://thesportjournal.org/article/glycogen-replenishment-after-exhaustive-exercise/
      double glycogenesisLowerRate_g_Per_s = .02 * (maxLiverGlycogen_g + maxMuscleGlycogen_g) / 3600;
      double glycogenesisUpperRate_g_Per_s = .05 * (maxLiverGlycogen_g + maxMuscleGlycogen_g) / 3600;

      //https://www.wolframalpha.com/input/?i=y%3D.0005417%2B.0008125%2F(1%2Be%5E(-6(x-.5)))+from+0%3Cy%3C.0015+and+0%3Cx%3C2
      //Can tweak sigmoid midpoint based on ratio values observed; shape parameter should be tweaked to give appropriate refill rates for glycogen (>20 hours)
      double glycogenesisRate_g_Per_s = glycogenesisLowerRate_g_Per_s + GeneralMath::LogisticFunction(glycogenesisUpperRate_g_Per_s - glycogenesisLowerRate_g_Per_s, .5, 6, hormoneFactor);

      //Glycogenolysis rate - fully depleted in 12 hours of fasting; we'll say with max exercise that it depletes in 30 min
      //This is only for liver glycogen
      double glycogenolysisLowerRate_g_Per_s = maxLiverGlycogen_g / 12 / 3600;
      double glycogenolysisUpperRate_g_Per_s = maxLiverGlycogen_g / .5 / 3600;

      //https://www.wolframalpha.com/input/?i=y%3D.002257%2B.05417%2F(1%2Be%5E(-22(x-.25)))+from+0%3Cy%3C.06+and+0%3Cx%3C2
      //Need to tweak sigmoid midpoint and shape parameter to get fasting depletion in ~12 hours and exercise depletion in ~30 min
      double glycogenolysisRate_g_Per_s = glycogenolysisLowerRate_g_Per_s + GeneralMath::LogisticFunction(glycogenolysisUpperRate_g_Per_s - glycogenolysisLowerRate_g_Per_s, .25, 22, -hormoneFactor);

      //remove excess glucose from blood and store in glycogen while there's room
      if ((hormoneFactor > 0) && (liverGlycogen_g < maxLiverGlycogen_g)) {
        if (cLiverVascular->GetSubstanceQuantity(glucose)->GetMass(MassUnit::g) < glycogenesisRate_g_Per_s * deltaT_s) {
          //ss << "Not enough glucose in blood to store as liver glycogen!";
          //Info(ss);
        } else {
          cLiverVascular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(-glycogenesisRate_g_Per_s * deltaT_s, MassUnit::g);
          liverGlycogen_g += (glycogenesisRate_g_Per_s * deltaT_s);
          cLiverVascular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Mass);
        }
      }

      //remove glucose from glycogen stores and dump into blood
      else if (hormoneFactor < 0) {
        if (liverGlycogen_g < glycogenolysisRate_g_Per_s * deltaT_s) {
          //ss << "Not enough glycogen remaining! Glycogen: "<<liverGlycogen_g<<" Decrement: "<< glycogenolysisRate_g_Per_s * deltaT_s;
          //Info(ss);
        } else {
          cLiverVascular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(glycogenolysisRate_g_Per_s * deltaT_s, MassUnit::g);
          liverGlycogen_g -= (glycogenolysisRate_g_Per_s * deltaT_s);
          cLiverVascular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Mass);
        }
      }

      //Now check the hormone factor in the muscle for storage of muscle glycogen
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cMuscleVascular->GetSubstanceQuantity(insulin), cMuscleVascular->GetSubstanceQuantity(glucagon), bg);

      //remove excess glucose from blood and store in muscle glycogen while there's room
      if ((hormoneFactor > 0) && (muscleGlycogen_g < maxMuscleGlycogen_g)) {
        if (cMuscleVascular->GetSubstanceQuantity(glucose)->GetMass(MassUnit::g) < glycogenesisRate_g_Per_s * deltaT_s) {
          //ss << "Not enough glucose in blood to store as muscle glycogen!";
          //Info(ss);
        } else {
          cMuscleVascular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(-glycogenesisRate_g_Per_s * deltaT_s, MassUnit::g);
          muscleGlycogen_g += (glycogenesisRate_g_Per_s * deltaT_s);
          cMuscleVascular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Mass);
        }
      }
    }

    if (usingProteinStorage) {
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cMuscleVascular->GetSubstanceQuantity(insulin), cMuscleVascular->GetSubstanceQuantity(glucagon), bg);
      if (i % trackSkipper == 0)
        trk.Track("MuscleHormoneFactor", time, hormoneFactor);

      //Guyton says protein blood concentrations should only rise 2-3 mg/dL even after eating because of absorption into tissues
      //These values allow a 75g protein meal to only raise amino acid blood concentration ~7 mg/dL
      //These might need to change if rate at which protein is absorbed into blood changes
      double proteinStorageLowerRate_g_Per_s = .3;
      double proteinStorageUpperRate_g_Per_s = .6;

      //https://www.wolframalpha.com/input/?i=y%3D.3%2B.3%2F(1%2Be%5E(-6(x-.5)))+from+0%3Cy%3C.6+and+0%3Cx%3C2
      double proteinStorageRate_g_Per_s = proteinStorageLowerRate_g_Per_s + GeneralMath::LogisticFunction(proteinStorageUpperRate_g_Per_s - proteinStorageLowerRate_g_Per_s, .5, 6, hormoneFactor);

      //Assume protein breakdown rates are similar to storage rates, maybe a bit slower; could be adjusted
      double proteinBreakdownLowerRate_g_Per_s = .1;
      double proteinBreakdownUpperRate_g_Per_s = .2;

      //https://www.wolframalpha.com/input/?i=y%3D.1%2B.1%2F(1%2Be%5E(-22(x-.25)))+from+0%3Cy%3C.2+and+0%3Cx%3C2
      double proteinBreakdownRate_g_Per_s = proteinBreakdownLowerRate_g_Per_s + GeneralMath::LogisticFunction(proteinBreakdownUpperRate_g_Per_s - proteinBreakdownLowerRate_g_Per_s, .25, 22, -hormoneFactor);

      //remove excess amino acids from blood and store in muscle while there's room
      //Body mobilizes protein when glucagon dominates, but we'll have protein shift toward storage unless insulin drops significantly
      if (hormoneFactor >= -.2 && storedProtein_g < maxStoredProtein_g && cMuscleVascular->GetSubstanceQuantity(aminoAcids)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) > aminoAcidsBaseline_mg_Per_dL) {
        if (cMuscleVascular->GetSubstanceQuantity(aminoAcids)->GetMass(MassUnit::g) < proteinStorageRate_g_Per_s * deltaT_s) {
          ss << "Not enough amino acids in blood to store!";
          Info(ss);
        } else {
          cMuscleVascular->GetSubstanceQuantity(aminoAcids)->GetMass().IncrementValue(-proteinStorageRate_g_Per_s * deltaT_s, MassUnit::g);
          storedProtein_g += proteinStorageRate_g_Per_s * deltaT_s;
          cMuscleVascular->GetSubstanceQuantity(aminoAcids)->Balance(BalanceLiquidBy::Mass);
        }
      }

      //remove amino acids from protein stores and dump into blood
      //This is not the same as muscle degradation during starvation
      //We don't dump AA into blood if blood concentration is already at basal level
      //TODO Ideally, gluconeogenesis would increase the hormone factor to regulate this, but it's not done yet
      else if (hormoneFactor < 0 && cMuscleVascular->GetSubstanceQuantity(aminoAcids)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) < aminoAcidsBaseline_mg_Per_dL) {
        if (storedProtein_g < proteinBreakdownRate_g_Per_s * deltaT_s) {
          //ss << "Not enough stored amino acids remaining!";
          //Info(ss);
        } else {
          cMuscleVascular->GetSubstanceQuantity(aminoAcids)->GetMass().IncrementValue(proteinBreakdownRate_g_Per_s * deltaT_s, MassUnit::g);
          storedProtein_g -= proteinBreakdownRate_g_Per_s * deltaT_s;
          cMuscleVascular->GetSubstanceQuantity(aminoAcids)->Balance(BalanceLiquidBy::Mass);
        }
      }
    }

    if (usingFatStorage) {
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cFatVascular->GetSubstanceQuantity(insulin), cFatVascular->GetSubstanceQuantity(glucagon), bg);
      if (i % trackSkipper == 0)
        trk.Track("FatHormoneFactor", time, hormoneFactor);

      //Guyton says triglycerides in chylomicrons are clear from blood in "a few hours" with chylomicrons having half-life <1 hour
      //Other sources show triglycerides returning to normal in ~4 hours after peak
      //These values currently allow 75g of fat to be absorbed into bloodstream in ~4 hours, with all being stored in ~6 hours
      //Peak triglyceride levels are about 5x normal, maybe too high
      double fatStorageLowerRate_g_Per_s = .004;
      double fatStorageUpperRate_g_Per_s = .012;

      //https://www.wolframalpha.com/input/?i=y%3D.4%2B.8%2F(1%2Be%5E(-6(x-.5)))+from+0%3Cy%3C1.2+and+0%3Cx%3C2
      double fatStorageRate_g_Per_s = fatStorageLowerRate_g_Per_s + GeneralMath::LogisticFunction(fatStorageUpperRate_g_Per_s - fatStorageLowerRate_g_Per_s, .5, 6, hormoneFactor);

      //Assume fat release rates are similar to glycogen for now... but this could be revisited
      double fatReleaseLowerRate_g_Per_s = .002546; //maxStoredProtein_g/ 12 * 3600
      double fatReleaseUpperRate_g_Per_s = .061111; //maxStoredProtein_g/ .5 * 3600

      //https://www.wolframalpha.com/input/?i=y%3D.002546%2B.058565%2F(1%2Be%5E(-22(x-.25)))+from+0%3Cy%3C.08+and+0%3Cx%3C2
      double fatReleaseRate_g_Per_s = fatReleaseLowerRate_g_Per_s + GeneralMath::LogisticFunction(fatReleaseUpperRate_g_Per_s - fatReleaseLowerRate_g_Per_s, .25, 22, -hormoneFactor);

      //remove excess triacylglycerol from blood and store in fat tissue
      if (hormoneFactor >= -.2 && cFatVascular->GetSubstanceQuantity(triacylglycerol)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) > triacylglycerolBaseline_mg_Per_dL) {
        if (cFatVascular->GetSubstanceQuantity(triacylglycerol)->GetMass(MassUnit::g) < fatStorageRate_g_Per_s * deltaT_s) {
          ss << "Not enough triacylglycerol in blood to store!";
          Info(ss);
        } else {
          cFatVascular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue(-fatStorageRate_g_Per_s * deltaT_s, MassUnit::g);
          storedFat_g += fatStorageRate_g_Per_s * deltaT_s;
          cFatVascular->GetSubstanceQuantity(triacylglycerol)->Balance(BalanceLiquidBy::Mass);
        }
      }

      //remove triacylglycerol from fat stores and dump into blood
      //Guyton says during starvation (i.e. glucagon dominant times) triacylglycerol blood concentration can be 5-8x normal
      //We won't continue to dump TAG into blood if blood concentration is already high
      else if (hormoneFactor < 0 && cFatVascular->GetSubstanceQuantity(triacylglycerol)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL) < triacylglycerolBaseline_mg_Per_dL * 6.5) {
        if (storedFat_g < fatReleaseRate_g_Per_s * deltaT_s) {
          //ss << "Not enough stored fat remaining!";
          //Info(ss);
        } else {
          cFatVascular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue(fatReleaseRate_g_Per_s * deltaT_s, MassUnit::g);
          storedFat_g -= fatReleaseRate_g_Per_s * deltaT_s;
          cFatVascular->GetSubstanceQuantity(triacylglycerol)->Balance(BalanceLiquidBy::Mass);
        }
      }
    }

    double CO2Produced_mol = 0;
    double O2Consumed_mol = 0;

    //Lipogenesis (liver making new TAG from AA and glucose in times of excess)
    if (usingLipogenesis) {
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cLiverVascular->GetSubstanceQuantity(insulin), cLiverVascular->GetSubstanceQuantity(glucagon), bg);

      //Get ratio of glucose to AA in liver to determine proportions that get converted to fat (will be a percentage of nutrients as glucose)
      //Really, what gets broken down to fat depends on concentrations of the facilitating enzymes
      //But it seems reasonable that you'll convert more of the one that's in excess
      double liverGlucoseToAARatio = cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L) / (cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L) + cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_L));

      //We'll assume our lipogenesis rate is comparable at max to the glycogenesis rate for now
      //But for negative hormone factors, we shouldn't make fat at all
      //TODO a pure protein meal could conceivably not raise the hormone factor, and so AA wouldn't get converted
      //https://www.wolframalpha.com/input/?i=y%3D.006%2F(1%2Be%5E(-15(x-.35)))+from+-.002%3Cy%3C.008+and+-.5%3Cx%3C2
      double lipogenesisLowerRate_g_Per_s = 0;
      double lipogenesisUpperRate_g_Per_s = .006;

      double lipogenesisRate_g_Per_s = lipogenesisLowerRate_g_Per_s + GeneralMath::LogisticFunction(lipogenesisUpperRate_g_Per_s - lipogenesisLowerRate_g_Per_s, .35, 15, hormoneFactor);

      //https://www.diapedia.org/metabolism-insulin-and-other-hormones/5105592814/fatty-acid-oxidation-and-synthesis
      //Lipogenesis of one palmitic acid requires 8 acetyl-COAs (also 7 ATPs, but we won't consider that for now)
      //Breakdown of AA (we assume alanine) is same as in protein usage for energy, gives NH4+ and pyruvate
      //From pyruvate we get one acetyl-COA
      //Then 2 NH4+ to urea
      //Glucose breaks down to 2 pyruvates by glycolysis, which then turn to acetyl-COA (4 glucose = 8 acetyl-COA); then you need one more for the glycerol backbone
      //Then you need 3 palmitic acids per triacylglycerol
      double totalTriacylglycerolToBeFormed_mol = lipogenesisRate_g_Per_s * deltaT_s / triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      double glucoseToBeConverted_mol = liverGlucoseToAARatio * totalTriacylglycerolToBeFormed_mol * 13;
      double AAToBeConverted_mol = (1 - liverGlucoseToAARatio) * totalTriacylglycerolToBeFormed_mol * 24;
      double UreaFormed_mol = AAToBeConverted_mol * .5;
      double leftoverTAGDeficit_mol = 0;

      //If there's enough AA, convert it to triacylglycerol
      if (cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().GetValue(MassUnit::g) > AAToBeConverted_mol * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().IncrementValue(-AAToBeConverted_mol * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(UreaFormed_mol * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue((AAToBeConverted_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += (AAToBeConverted_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }
      //If there's not enough to be converted, convert what's there, shift the deficit to glucose conversion
      else {
        double AAActuallyConsumed_mol = cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().GetValue(MassUnit::g) / aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        leftoverTAGDeficit_mol = (AAToBeConverted_mol - AAActuallyConsumed_mol) / 24;
        cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(AAActuallyConsumed_mol * .5 * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += (AAActuallyConsumed_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }

      glucoseToBeConverted_mol += leftoverTAGDeficit_mol * 13;

      //If there's enough glucose, convert it to triacylglycerol
      if (cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().GetValue(MassUnit::g) > glucoseToBeConverted_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(-glucoseToBeConverted_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue((glucoseToBeConverted_mol / 13) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += (glucoseToBeConverted_mol / 13) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }
      //If there's not enough to be converted, convert what's there
      else {
        double glucoseActuallyConsumed_mol = cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().GetValue(MassUnit::g) / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        leftoverTAGDeficit_mol = (glucoseToBeConverted_mol - glucoseActuallyConsumed_mol) / 13;
        cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue((glucoseActuallyConsumed_mol / 13) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += (glucoseActuallyConsumed_mol / 13) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }

      //Try one last time to convert from AA, in case glucose had a deficit
      if (cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().GetValue(MassUnit::g) > leftoverTAGDeficit_mol * 24 * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol)) {
        cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().IncrementValue(-leftoverTAGDeficit_mol * 24 * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(leftoverTAGDeficit_mol * 24 * .5 * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue(leftoverTAGDeficit_mol * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += leftoverTAGDeficit_mol * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }
      //If there's not enough to be converted, convert what's there, remaining deficit just means lipogenesis is limited by lack of nutrients in liver
      else {
        double AAActuallyConsumed_mol = cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().GetValue(MassUnit::g) / aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(AAActuallyConsumed_mol * .5 * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue((AAActuallyConsumed_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
        totalTAGFormedByLipogenesis_g += (AAActuallyConsumed_mol / 24) * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
      }
    }

    //Gluconeogenesis (the liver making new glucose from lactate, glycerol backbones of TAG, and AA)
    //Current Renal::CalculateGluconeogenesis only converts lactate to glucose and excretes the rest (and the ratio seems to be wrong)
    //Note: Gluconeogenesis costs ATP, but I guess that is rolled up in the BMR we calculate
    //Another note: These processes consume O2, so we will skip them under anaerobic conditions
    if (usingGluconeogenesis) {
      hormoneFactor = hptc.CalculateRelativeHormoneChange(insulinBaseline_pmol_Per_L, glucagonBaseline_pg_Per_mL, cLiverVascular->GetSubstanceQuantity(insulin), cLiverVascular->GetSubstanceQuantity(glucagon), bg);

      //Handle lactate conversion
      //TODO should this only be done when hormone factor is negative? This would allow for some lactate excretion and maybe make it so that the blood maintains the expected 2-23 mg/dL
      //Guyton says up to 75% of lactate is converted back to glucose in the liver after exercise (the rest is converted to pyruvate for use in citric acid cycle)
      //Some other tissues can also do this, to a lesser extent, but we only do it in liver
      //Since lactate->pyruvate is one reaction facilitated by one enzyme, and 2 pyruvate->glucose is glycolysis in reverse,
      //we will assume this conversion is limited only by the amount of lactate in the liver and presence of O2
      //TODO Can't find how much O2 this costs, if any; may need to make it still work under anaerobic conditions
      double rateLimitingTuningFactor = 1;
      double liverLactate_mol = cLiverExtracellular->GetSubstanceQuantity(lactate)->GetMass().GetValue(MassUnit::g) / lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      double reconvertedGlucose_mol = liverLactate_mol * .5;
      if (rateLimitingTuningFactor == 1)
        cLiverExtracellular->GetSubstanceQuantity(lactate)->GetMass().SetValue(0, MassUnit::g); //using IncrementValue to remove ALL lactate can result in numerical error negative masses
      else
        cLiverExtracellular->GetSubstanceQuantity(lactate)->GetMass().IncrementValue(-rateLimitingTuningFactor * liverLactate_mol * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(rateLimitingTuningFactor * reconvertedGlucose_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);

      totalGlucoseFromLactate_g += rateLimitingTuningFactor * reconvertedGlucose_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);

      //Handle fat conversion (glycerol to glucose and ketogenesis)
      //One TAG breaks down to one glycerol (which converts to glucose) and 3 palmitic acids
      //One palmitic acid breaks down to 8 acetyl-COAs, which all convert to ketones (2 acetyl-COAs combine to make acetoacetate, our "ketones")
      //This rate is really governed by the presence of oxaloacetate, but since we're not modeling that,
      //we have to empirically determine rates
      //A fasting person will have low levels of oxaloacetate, meaning it can't aid in converting acetyl-COA for Citric Acid Cycle
      //Thus, Acetyl-COA converts to ketones instead
      //TODO consider having TAG convert to glucose for mildly negative hormone factors and ketones for more negative ones, since ketones only generate after all oxaloactate is gone, which is presumably after a while
      if (hormoneFactor < -.25 && !isAnaerobic) {
        double TAGBreakdownRate_g_Per_s = .0015; //This is a complete guess for now; this should be tuned such that there is no brain energy deficit on a long-term no-carb diet and fat loss isn't ridiculous
        double glucosePerTAG_mol = 1; //one glycerol backbone
        double ketonesPerTAG_mol = 12; //8*3 acetyl-COAs -> 12 acetoacetate (in reality, there's also acetone and B-hydroxybutyrate)
        double O2ConsumedToMakeKetones = 5;
        //double O2ConsumedToMakeGlucose = ? ; find this if you implement glucose regeneration

        double TAGBrokenDown_mol = TAGBreakdownRate_g_Per_s * deltaT_s / triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol);

        //If we have enough TAG in the liver
        if (cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * liverExtracellularVolume_mL > TAGBrokenDown_mol) {
          cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().IncrementValue(-TAGBrokenDown_mol * triacylglycerol.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(TAGBrokenDown_mol * glucosePerTAG_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(ketones)->GetMass().IncrementValue(TAGBrokenDown_mol * ketonesPerTAG_mol * ketones.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          O2Consumed_mol += TAGBrokenDown_mol * O2ConsumedToMakeKetones;
          totalGlucoseFromTAG_g += TAGBrokenDown_mol * glucosePerTAG_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
          totalKetonesFromTAG_g += TAGBrokenDown_mol * ketonesPerTAG_mol * ketones.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        }
        //If we don't have enough TAG in the liver, break down what's there
        else {
          double TAGActuallyBrokenDown_mol = cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * liverExtracellularVolume_mL;
          cLiverExtracellular->GetSubstanceQuantity(triacylglycerol)->GetMass().SetValue(0, MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * glucosePerTAG_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(ketones)->GetMass().IncrementValue(TAGActuallyBrokenDown_mol * ketonesPerTAG_mol * ketones.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          O2Consumed_mol += TAGActuallyBrokenDown_mol * O2ConsumedToMakeKetones;
          totalGlucoseFromTAG_g += TAGActuallyBrokenDown_mol * glucosePerTAG_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
          totalKetonesFromTAG_g += TAGActuallyBrokenDown_mol * ketonesPerTAG_mol * ketones.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        }
      }

      //Handle AA conversion
      //We assume all AA is alanine, which will undergo the same conversion to pyruvate as in consumption, releasing urea as a byproduct
      //1 alanine -> 1 pyruvate + .5 urea
      //2 pyruvate -> 1 glucose
      //TODO figure out O2 consumption, if any
      if (hormoneFactor < 0 && !isAnaerobic) {
        double AAConversionRate_g_Per_s = .001; //May need to be tuned to maintain brain glucose levels
        double glucosePerAA_mol = .5;
        double ureaPerAA_mol = .5;

        double AAConverted_mol = AAConversionRate_g_Per_s * deltaT_s / aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol);

        //If we have enough AA in the liver
        if (cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * liverExtracellularVolume_mL > AAConverted_mol) {
          cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().IncrementValue(-AAConverted_mol * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(AAConverted_mol * glucosePerAA_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(AAConverted_mol * ureaPerAA_mol * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          totalGlucoseFromAA_g += AAConverted_mol * glucosePerAA_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        }
        //If we don't have enough AA in the liver, break down what's there
        else {
          double AAActuallyConverted_mol = cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity().GetValue(AmountPerVolumeUnit::mol_Per_mL) * liverExtracellularVolume_mL;
          cLiverExtracellular->GetSubstanceQuantity(aminoAcids)->GetMass().SetValue(0, MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(AAActuallyConverted_mol * glucosePerAA_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          cLiverExtracellular->GetSubstanceQuantity(urea)->GetMass().IncrementValue(AAActuallyConverted_mol * ureaPerAA_mol * urea.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
          totalGlucoseFromAA_g += AAActuallyConverted_mol * glucosePerAA_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
        }
      }
      cLiverExtracellular->Balance(BalanceLiquidBy::Mass);
    }

    //Determine energy request and translate to glucose required (don't consider protein as of now)
    //Consume as much glucose as you can from intracellular (assume oxygen is present for now)
    //Produce CO2 and consume O2
    double baseEnergyRequested_kcal = (BMR_kcal_day / 86400) * deltaT_s;
    double exerciseEnergyRequested_kcal = (exerciseWork_W * 20.64 / 86400) * deltaT_s;
    double brainEnergyDeficit_kcal = 0;
    double muscleEnergyDeficit_kcal = 0;
    //double brainFlowFraction = brainFlow_mL_per_min / (brainFlow_mL_per_min + muscleFlow_mL_per_min); //Try this in engine to see if we can have consumption based on flow
    double brainFlowFraction = .2; //Brain uses about 20% of body's energy https://www.scientificamerican.com/article/thinking-hard-calories/
    if (useConsumption)
      ProduceAndConsume(baseEnergyRequested_kcal, exerciseEnergyRequested_kcal, isAnaerobic, bg, deltaT_s, brainFlowFraction, muscleGlycogen_g, CO2Produced_mol, O2Consumed_mol, brainEnergyDeficit_kcal, muscleEnergyDeficit_kcal, totalLactateFromGlucose, trk);

    //Process - Execute the circuit
    calc.Process(*Circuit, deltaT_s);
    //Execute the substance transport function
    txpt.Transport(Graph, deltaT_s);

    //convert 'Next' values to current
    calc.PostProcess(*Circuit);

    //Track everything
    if (i % trackSkipper == 0) {
      for (SELiquidCompartment* c : Graph.GetCompartments()) {
        trk.Track(std::string{c->GetName()} + "_AminoAcidConcentration_mg_per_dL", time, c->GetSubstanceQuantity(aminoAcids)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_GlucoseConcentration_mg_per_dL", time, c->GetSubstanceQuantity(glucose)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_TriacyglycerolConcentration_mg_per_dL", time, c->GetSubstanceQuantity(triacylglycerol)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_InsulinMolarity_pmol_per_L", time, c->GetSubstanceQuantity(insulin)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) * 1e9);
        trk.Track(std::string{c->GetName()} + "_GlucagonConcentration_pg_per_mL", time, c->GetSubstanceQuantity(glucagon)->GetConcentration(MassPerVolumeUnit::mg_Per_mL) * 1e9);
        trk.Track(std::string{c->GetName()} + "_LactateConcentration_mg_per_dL", time, c->GetSubstanceQuantity(lactate)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_KetonesConcentration_mg_per_dL", time, c->GetSubstanceQuantity(ketones)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_CreatinineConcentration_mg_per_dL", time, c->GetSubstanceQuantity(creatinine)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_UreaConcentration_mg_per_dL", time, c->GetSubstanceQuantity(urea)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));

        //trk.Track(std::string{c->GetName()} + "_Volume_mL", time, c->GetVolume(VolumeUnit::mL));
        //trk.Track(std::string{c->GetName()} + "_CompartmentInFlow_mL_per_min", time, c->GetInFlow(VolumePerTimeUnit::mL_Per_min));
        //trk.Track(std::string{c->GetName()} + "_CompartmentOutFlow_mL_per_min", time, c->GetOutFlow(VolumePerTimeUnit::mL_Per_min));
      }
      for (SELiquidCompartment* c : extracellularCompartments) {
        trk.Track(std::string{c->GetName()} + "_AminoAcidConcentration_mg_per_dL", time, c->GetSubstanceQuantity(aminoAcids)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_GlucoseConcentration_mg_per_dL", time, c->GetSubstanceQuantity(glucose)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_TriacyglycerolConcentration_mg_per_dL", time, c->GetSubstanceQuantity(triacylglycerol)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_InsulinMolarity_pmol_per_L", time, c->GetSubstanceQuantity(insulin)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) * 1e9);
        trk.Track(std::string{c->GetName()} + "_GlucagonConcentration_pg_per_mL", time, c->GetSubstanceQuantity(glucagon)->GetConcentration(MassPerVolumeUnit::mg_Per_mL) * 1e9);
        trk.Track(std::string{c->GetName()} + "_LactateConcentration_mg_per_dL", time, c->GetSubstanceQuantity(lactate)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_KetonesConcentration_mg_per_dL", time, c->GetSubstanceQuantity(ketones)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_CreatinineConcentration_mg_per_dL", time, c->GetSubstanceQuantity(creatinine)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_UreaConcentration_mg_per_dL", time, c->GetSubstanceQuantity(urea)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
      }
      for (SELiquidCompartment* c : intracellularCompartments) {
        trk.Track(std::string{c->GetName()} + "_AminoAcidConcentration_mg_per_dL", time, c->GetSubstanceQuantity(aminoAcids)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_GlucoseConcentration_mg_per_dL", time, c->GetSubstanceQuantity(glucose)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_TriacyglycerolConcentration_mg_per_dL", time, c->GetSubstanceQuantity(triacylglycerol)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_InsulinMolarity_pmol_per_L", time, c->GetSubstanceQuantity(insulin)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) * 1e9);
        trk.Track(std::string{c->GetName()} + "_GlucagonConcentration_pg_per_mL", time, c->GetSubstanceQuantity(glucagon)->GetConcentration(MassPerVolumeUnit::mg_Per_mL) * 1e9);
        trk.Track(std::string{c->GetName()} + "_LactateConcentration_mg_per_dL", time, c->GetSubstanceQuantity(lactate)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_KetonesConcentration_mg_per_dL", time, c->GetSubstanceQuantity(ketones)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_CreatinineConcentration_mg_per_dL", time, c->GetSubstanceQuantity(creatinine)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
        trk.Track(std::string{c->GetName()} + "_UreaConcentration_mg_per_dL", time, c->GetSubstanceQuantity(urea)->GetConcentration(MassPerVolumeUnit::mg_Per_dL));
      }

      trk.Track(std::string{cSmallIntestineChyme->GetName()} + "_ProteinMass_g", time, cSmallIntestineChyme->GetSubstanceQuantity(aminoAcids)->GetMass(MassUnit::g));
      trk.Track(std::string{cSmallIntestineChyme->GetName()} + "_CarbsMass_g", time, cSmallIntestineChyme->GetSubstanceQuantity(glucose)->GetMass(MassUnit::g));
      trk.Track(std::string{cSmallIntestineChyme->GetName()} + "_FatMass_g", time, cSmallIntestineChyme->GetSubstanceQuantity(triacylglycerol)->GetMass(MassUnit::g));

      trk.Track("LiverGlycogenMass_g", time, liverGlycogen_g);
      trk.Track("MuscleGlycogenMass_g", time, muscleGlycogen_g);
      trk.Track("StoredProteinMass_g", time, storedProtein_g);
      trk.Track("StoredFatDeltaMass_g", time, storedFat_g - initialStoredFat_g);

      trk.Track("BrainEnergyDeficit_kcal", time, brainEnergyDeficit_kcal);
      trk.Track("MuscleEnergyDeficit_kcal", time, muscleEnergyDeficit_kcal);
      trk.Track("CO2Produced_ug", time, CO2Produced_mol * 44.01 * 1000000);
      trk.Track("O2Consumed_ug", time, O2Consumed_mol * 32 * 1000000);

      trk.Track("totalGlucoseFromAA_g", time, totalGlucoseFromAA_g);
      trk.Track("totalGlucoseFromLactate_g", time, totalGlucoseFromLactate_g);
      trk.Track("totalGlucoseFromTAG_g", time, totalGlucoseFromTAG_g);
      trk.Track("totalKetonesFromTAG_g", time, totalKetonesFromTAG_g);
      trk.Track("totalGlucoseFromAA_g", time, totalGlucoseFromAA_g);
      trk.Track("totalTAGFormedByLipogenesis_g", time, totalTAGFormedByLipogenesis_g);

      trk.StreamTrackToFile(file);
    }

    if (i == 0) {
      trk.CreateFile(std::string(rptDirectory + outputName + ".csv").c_str(), file);
    }

    time += deltaT_s;
  }
  file.close();
}


//Manage intracellular substance masses based on energy requested
void BioGearsEngineTest::ProduceAndConsume(double baseEnergyRequested_kcal, double exerciseEnergyRequested_kcal, bool isAnaerobic, BioGears& bg, double deltaT_s, double brainFlowFraction, double& muscleGlycogen_g, double& CO2Produced_mol, double& O2Consumed_mol, double& brainEnergyDeficit_kcal, double& muscleEnergyDeficit_kcal, double& lactateFromGlucose_g, DataTrack& trk)
{
  //Cache some useful stuff
  SESubstance& aminoAcids = bg.GetSubstances().GetAminoAcids();
  SESubstance& glucose = bg.GetSubstances().GetGlucose();
  SESubstance& urea = bg.GetSubstances().GetUrea();
  SESubstance& triacylglycerol = bg.GetSubstances().GetTriacylglycerol();
  SESubstance& ketones = bg.GetSubstances().GetKetones();
  SESubstance& lactate = bg.GetSubstances().GetLactate();
  SELiquidCompartment* brainIntracellular = bg.GetCompartments().GetLiquidCompartment("cBrainIntracellular");
  SELiquidCompartment* muscleIntracellular = bg.GetCompartments().GetLiquidCompartment("cMuscleIntracellular");
  SELiquidCompartment* liverExtracellular = bg.GetCompartments().GetLiquidCompartment("cLiverExtracellular");
  double brainVolume_L = brainIntracellular->GetVolume(VolumeUnit::L);
  double muscleVolume_L = muscleIntracellular->GetVolume(VolumeUnit::L);
  double liverVolume_L = liverExtracellular->GetVolume(VolumeUnit::L);
  double energyPerMolATP_kcal = bg.GetConfiguration().GetEnergyPerATP(EnergyPerAmountUnit::kcal_Per_mol);

  //Split energy needs between brain and muscle
  //Brain's burden doesn't increase under exercise, only muscle
  double muscleFlowFraction = 1 - brainFlowFraction;
  double brainNeededEnergy_kcal = brainFlowFraction * baseEnergyRequested_kcal;
  double muscleNeededEnergy_kcal = (muscleFlowFraction * baseEnergyRequested_kcal) + exerciseEnergyRequested_kcal;

  //First, we'll consume brain glucose, since the brain always needs energy
  //The full aerobic glucose breakdown gives ~29.85 ATP, including inefficiencies \cite rich2003molecular
  //For this unit test, we assume the brain always has oxygen available, even during exercise
  double ATP_Per_Glucose = 29.85;
  double CO2_Per_Glucose = 6;
  double O2_Per_Glucose = 6;

  double energyAsIntracellularBrainGlucose_kcal = (brainIntracellular->GetSubstanceQuantity(glucose)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * brainVolume_L) * ATP_Per_Glucose * energyPerMolATP_kcal;

  //If we have enough energy as glucose, consume it
  if (energyAsIntracellularBrainGlucose_kcal >= brainNeededEnergy_kcal) {
    double glucoseToConsume_mol = brainNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;
    brainIntracellular->GetSubstanceQuantity(glucose)->GetMolarity().IncrementValue(-glucoseToConsume_mol / brainVolume_L, AmountPerVolumeUnit::mol_Per_L);
    CO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
    O2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
    brainIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Molarity);
    brainNeededEnergy_kcal = 0;
  }
  //Otherwise, consume what intracellular glucose is there and track the deficit
  else {
    double glucoseToConsume_mol = energyAsIntracellularBrainGlucose_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;
    brainIntracellular->GetSubstanceQuantity(glucose)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mol_Per_L);
    CO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
    O2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
    brainIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Molarity);
    brainNeededEnergy_kcal -= energyAsIntracellularBrainGlucose_kcal;
  }

  //Next, consume ketones in brain, since brain can't consume AA or TAG
  //This is heavily used in starvation
  double ATP_Per_Ketone = 24;
  double CO2_Per_Ketone = 6; //double check this, ketones consumed via citric acid cycle, so should be the same as glucose
  double O2_Per_Ketone = 6; //Not sure about this either

  double energyAsIntracellularBrainKetones_kcal = (brainIntracellular->GetSubstanceQuantity(ketones)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * brainVolume_L) * ATP_Per_Ketone * energyPerMolATP_kcal;

  //If we still need brain energy and we have enough ketones to cover it
  if (brainNeededEnergy_kcal > 0 && energyAsIntracellularBrainKetones_kcal >= brainNeededEnergy_kcal) {
    double ketonesToConsume_mol = brainNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Ketone;
    brainIntracellular->GetSubstanceQuantity(ketones)->GetMolarity().IncrementValue(-ketonesToConsume_mol / brainVolume_L, AmountPerVolumeUnit::mol_Per_L);
    CO2Produced_mol += ketonesToConsume_mol * CO2_Per_Ketone;
    O2Consumed_mol += ketonesToConsume_mol * O2_Per_Ketone;
    brainIntracellular->GetSubstanceQuantity(ketones)->Balance(BalanceLiquidBy::Molarity);
    brainNeededEnergy_kcal = 0;
  }
  //Otherwise, consume the ketones in the brain and track the deficit
  else if (brainNeededEnergy_kcal > 0) {
    double ketonesToConsume_mol = energyAsIntracellularBrainKetones_kcal / energyPerMolATP_kcal / ATP_Per_Ketone;
    brainIntracellular->GetSubstanceQuantity(ketones)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mol_Per_L);
    CO2Produced_mol += ketonesToConsume_mol * CO2_Per_Ketone;
    O2Consumed_mol += ketonesToConsume_mol * O2_Per_Ketone;
    brainIntracellular->GetSubstanceQuantity(ketones)->Balance(BalanceLiquidBy::Molarity);
    brainNeededEnergy_kcal -= energyAsIntracellularBrainGlucose_kcal;
  }
  brainEnergyDeficit_kcal = brainNeededEnergy_kcal > 0 ? brainNeededEnergy_kcal : 0; //Any needed energy we have left is a deficit

  //Next, consume Amino Acids, since humans always metabolize some protein
  //We'll assume all amino acids are alanine, though in reality different amino acids decompose to different compounds (usually parts of the Citric Acid Cycle)
  //that have different energy production and reactants.
  //From 1 mol alanine and .5 mol CO2, we produce  NADH (=2.5 ATP) + NH4+ and pyruvate
  //From pyruvate via Citric Acid Cycle, we get 12.5 ATP, 2CO2, and water
  //Then NH4+ to urea costs 2 ATP
  //Net +13 ATP, 1.5 CO2, .5 urea, and some water
  //The muscle tissue can break down AA to oxidizable keto acid and ammonia, but can't form urea
  //In reality the additional amino groups get stuck on glutamine or alanine and transported to the liver for urea generation
  //We won't model this mechanism, but will just consume AA in muscle and increment urea in the liver

  //We consume ~30g of protein a day (guyton), so make sure we consume it as long as we can
  double AAToConsume_mol = (30 * deltaT_s) / (24 * 3600 * aminoAcids.GetMolarMass(MassPerAmountUnit::g_Per_mol));
  double ATP_Per_AA = 13;
  double CO2_Per_AA = 1.5;
  double Urea_Per_AA = .5;
  double O2_Per_AA = 0; //TODO figure out how much O2 is consumed metabolizing AA

  //See if we actually have enough muscle AA to meet the request and carry it out
  //Note: Brain can't consume AA
  double intracellularAA_mol = muscleIntracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * muscleVolume_L;
  double AADeficit_mol = intracellularAA_mol - AAToConsume_mol;

  //There wasn't enough; consume all intracellular AA and track deficit if desired
  if (!isAnaerobic) {
    if (AADeficit_mol < 0) {
      // -AADeficit_mol //Track this to see if there's a problem with AA deficiency; this shouldn't be an energy deficit since muscles can still get energy from glycogen and glucose
      muscleIntracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mol_Per_L);
      liverExtracellular->GetSubstanceQuantity(urea)->GetMolarity().IncrementValue(intracellularAA_mol * Urea_Per_AA / muscleVolume_L, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += intracellularAA_mol * CO2_Per_AA;
      O2Consumed_mol += intracellularAA_mol * O2_Per_AA;
      muscleNeededEnergy_kcal -= intracellularAA_mol * 13 * energyPerMolATP_kcal; //We'll still need to consume glycogen and glucose using this if possible
    }
    //There was enough; consume the required amount
    else {
      muscleIntracellular->GetSubstanceQuantity(aminoAcids)->GetMolarity().IncrementValue(-AAToConsume_mol / muscleVolume_L, AmountPerVolumeUnit::mol_Per_L);
      liverExtracellular->GetSubstanceQuantity(urea)->GetMolarity().IncrementValue(AAToConsume_mol * Urea_Per_AA / muscleVolume_L, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += AAToConsume_mol * CO2_Per_AA;
      O2Consumed_mol += AAToConsume_mol * O2_Per_AA;
      muscleNeededEnergy_kcal -= AAToConsume_mol * 13 * energyPerMolATP_kcal;
    }
    muscleIntracellular->GetSubstanceQuantity(aminoAcids)->Balance(BalanceLiquidBy::Molarity);
    liverExtracellular->GetSubstanceQuantity(urea)->Balance(BalanceLiquidBy::Molarity);
  }

  //If our obligatory protein consumption met our energy request (unlikely), no need to consume glucose
  if (muscleNeededEnergy_kcal <= 0)
    return;

  //Under aerobic conditions, muscles and other tissues burn intracellular FFA by B-oxidation
  //Fat-burning is highest at low-to-moderate exertion levels, and drops back down at high levels
  //https://www.unm.edu/~lkravitz/Article%20folder/physiologgfatloss.html
  //Assuming all fatty chains are palmitate, not consumed at theoretical yield (so 106 ATP each), and 12 ATP from glycerol backbone to pyruvate to Citric Acid Cycle
  //See https://en.wikipedia.org/wiki/Beta_oxidation and try to find a better source
  double ATP_Per_TAG = 330;
  double CO2_Per_TAG = 55; //meerman2014when
  double O2_Per_TAG = 78;
  double rateLimitingTuningFactor = 1;

  double energyAsMuscleTAG_kcal = rateLimitingTuningFactor * (muscleIntracellular->GetSubstanceQuantity(triacylglycerol)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * muscleVolume_L) * ATP_Per_TAG * energyPerMolATP_kcal;

  //If we have enough usable intracellular TAG to meet the request
  if (!isAnaerobic) {
    if (energyAsMuscleTAG_kcal >= muscleNeededEnergy_kcal) {
      double TAGToConsume_mol = muscleNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_TAG;
      muscleIntracellular->GetSubstanceQuantity(triacylglycerol)->GetMolarity().IncrementValue(-TAGToConsume_mol / muscleVolume_L, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += TAGToConsume_mol * CO2_Per_TAG;
      O2Consumed_mol += TAGToConsume_mol * O2_Per_TAG;
      muscleNeededEnergy_kcal = 0;
      muscleIntracellular->GetSubstanceQuantity(triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
    }
    //If there's not enough, consume what we can and move on to glycogen and glucose
    else {
      double TAGToConsume_mol = energyAsMuscleTAG_kcal / energyPerMolATP_kcal / ATP_Per_TAG;
      muscleIntracellular->GetSubstanceQuantity(triacylglycerol)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += TAGToConsume_mol * CO2_Per_TAG;
      O2Consumed_mol += TAGToConsume_mol * O2_Per_TAG;
      muscleNeededEnergy_kcal -= energyAsMuscleTAG_kcal;
      muscleIntracellular->GetSubstanceQuantity(triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
    }
  }

  //Consume intracellular glucose (aerobic)
  if (!isAnaerobic) {
    double energyAsMuscleIntracellularGlucose_kcal = (muscleIntracellular->GetSubstanceQuantity(glucose)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * muscleVolume_L) * ATP_Per_Glucose * energyPerMolATP_kcal;

    //If we have enough intracellular glucose to meet the request
    if (energyAsMuscleIntracellularGlucose_kcal >= muscleNeededEnergy_kcal) {
      double glucoseToConsume_mol = muscleNeededEnergy_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;
      muscleIntracellular->GetSubstanceQuantity(glucose)->GetMolarity().IncrementValue(-glucoseToConsume_mol / muscleVolume_L, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
      O2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
      muscleNeededEnergy_kcal = 0;
      muscleIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Molarity);
    }
    //If we don't have enough
    else {
      double glucoseToConsume_mol = energyAsMuscleIntracellularGlucose_kcal / energyPerMolATP_kcal / ATP_Per_Glucose;
      muscleIntracellular->GetSubstanceQuantity(glucose)->GetMolarity().SetValue(0, AmountPerVolumeUnit::mol_Per_L);
      CO2Produced_mol += glucoseToConsume_mol * CO2_Per_Glucose;
      O2Consumed_mol += glucoseToConsume_mol * O2_Per_Glucose;
      muscleNeededEnergy_kcal -= energyAsMuscleIntracellularGlucose_kcal;
      muscleIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Molarity);
    }
  }
  //Consume intracellular glucose (anaerobic)
  else {
    double anaerobic_ATP_Per_Glucose = 2;
    double energyAsMuscleIntracellularGlucose_kcal = (muscleIntracellular->GetSubstanceQuantity(glucose)->GetMolarity(AmountPerVolumeUnit::mol_Per_L) * muscleVolume_L) * anaerobic_ATP_Per_Glucose * energyPerMolATP_kcal;
    double lactate_Per_Glucose = 2;

    //If we have enough intracellular glucose to meet the request
    if (energyAsMuscleIntracellularGlucose_kcal >= muscleNeededEnergy_kcal) {
      double glucoseToConsume_mol = muscleNeededEnergy_kcal / energyPerMolATP_kcal / anaerobic_ATP_Per_Glucose;
      muscleIntracellular->GetSubstanceQuantity(glucose)->GetMass().IncrementValue(-glucoseToConsume_mol * glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      muscleIntracellular->GetSubstanceQuantity(lactate)->GetMass().IncrementValue(glucoseToConsume_mol * lactate_Per_Glucose * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      lactateFromGlucose_g += glucoseToConsume_mol * lactate_Per_Glucose * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleNeededEnergy_kcal = 0;
      muscleIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Mass);
      muscleIntracellular->GetSubstanceQuantity(lactate)->Balance(BalanceLiquidBy::Mass);
    }
    //If we'll end up with a deficit
    else {
      double glucoseToConsume_mol = energyAsMuscleIntracellularGlucose_kcal / energyPerMolATP_kcal / anaerobic_ATP_Per_Glucose;
      muscleIntracellular->GetSubstanceQuantity(glucose)->GetMass().SetValue(0, MassUnit::g);
      muscleIntracellular->GetSubstanceQuantity(lactate)->GetMass().IncrementValue(glucoseToConsume_mol * lactate_Per_Glucose * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      lactateFromGlucose_g += glucoseToConsume_mol * lactate_Per_Glucose * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleNeededEnergy_kcal -= energyAsMuscleIntracellularGlucose_kcal;
      muscleIntracellular->GetSubstanceQuantity(glucose)->Balance(BalanceLiquidBy::Mass);
      muscleIntracellular->GetSubstanceQuantity(lactate)->Balance(BalanceLiquidBy::Mass);
    }
  }

  double aerobic_ATP_Per_Glycogen = ATP_Per_Glucose + 1; //Since muscle glycogen is already phosphorylated, we get more ATP from it than when we split glucose by glycolysis (Guyton p 904)
  double anaerobic_ATP_Per_Glycogen = 3;
  double lactate_Per_Glycogen = 2;

  //Muscle glycogen consumption (aerobic)
  if (!isAnaerobic) {
    double energyAsMuscleGlycogen_kcal = (muscleGlycogen_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol)) * aerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;

    //If we have enough energy as glycogen, consume it
    //TODO Glycogen should be consumed at a particular rate, like AA
    if (energyAsMuscleGlycogen_kcal >= muscleNeededEnergy_kcal) {
      double glycogenConsumed_g = (muscleNeededEnergy_kcal / energyAsMuscleGlycogen_kcal) * muscleGlycogen_g;
      double glycogenConsumed_mol = glycogenConsumed_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleGlycogen_g -= glycogenConsumed_g; //we passed this by reference, so it will be updated for subsequent timesteps
      CO2Produced_mol += glycogenConsumed_mol * CO2_Per_Glucose;
      O2Consumed_mol += glycogenConsumed_mol * O2_Per_Glucose;
      muscleNeededEnergy_kcal = 0;
    }
    //Otherwise, consume what glycogen we can
    else {
      double glycogenConsumed_mol = muscleGlycogen_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleGlycogen_g = 0;
      CO2Produced_mol += glycogenConsumed_mol * CO2_Per_Glucose;
      O2Consumed_mol += glycogenConsumed_mol * O2_Per_Glucose;
      muscleNeededEnergy_kcal -= energyAsMuscleGlycogen_kcal;
    }
  }
  //Muscle glycogen consumption (anaerobic)
  //Glycogen->Glucose->2Pyruvate->2Lactate
  else {
    double energyAsMuscleGlycogen_kcal = (muscleGlycogen_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol)) * anaerobic_ATP_Per_Glycogen * energyPerMolATP_kcal;

    //If we have enough
    if (energyAsMuscleGlycogen_kcal >= muscleNeededEnergy_kcal) {
      double glycogenConsumed_g = (muscleNeededEnergy_kcal / energyAsMuscleGlycogen_kcal) * muscleGlycogen_g;
      double glycogenConsumed_mol = glycogenConsumed_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleGlycogen_g -= glycogenConsumed_g; //we passed this by reference, so it will be updated for subsequent timesteps
      muscleIntracellular->GetSubstanceQuantity(lactate)->GetMass().IncrementValue(glycogenConsumed_mol * lactate_Per_Glycogen * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      muscleIntracellular->GetSubstanceQuantity(lactate)->Balance(BalanceLiquidBy::Mass);
      lactateFromGlucose_g += glycogenConsumed_mol * lactate_Per_Glycogen * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleNeededEnergy_kcal = 0;
    }
    //If we still can't meet the energy request, we have an energy deficit
    else {
      double glycogenConsumed_mol = muscleGlycogen_g / glucose.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleGlycogen_g = 0;
      muscleIntracellular->GetSubstanceQuantity(lactate)->GetMass().IncrementValue(glycogenConsumed_mol * lactate_Per_Glycogen * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol), MassUnit::g);
      muscleIntracellular->GetSubstanceQuantity(lactate)->Balance(BalanceLiquidBy::Mass);
      lactateFromGlucose_g += glycogenConsumed_mol * lactate_Per_Glycogen * lactate.GetMolarMass(MassPerAmountUnit::g_Per_mol);
      muscleNeededEnergy_kcal -= energyAsMuscleGlycogen_kcal;
    }
  }
  muscleEnergyDeficit_kcal = muscleNeededEnergy_kcal > 0 ? muscleNeededEnergy_kcal : 0; //Any needed energy we have left is a deficit
}

void BioGearsEngineTest::CustomNutrientKineticsTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    true, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    true, //initialize stores to full
    true, //diffusion
    false, //consumption
    true, //lipogenesis
    false, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    12, //simulation duration
    MealType::Balanced);
}

void BioGearsEngineTest::ProteinGluconeogenesisTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    true, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    false, //initialize stores to full
    true, //diffusion
    true, //consumption
    true, //lipogenesis
    true, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    24, //simulation duration
    MealType::ProteinOnly, //meal type
    "ProteinGluconeogenesisTest" //test name
  );
}

void BioGearsEngineTest::StarvingKetogenesisTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    true, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    false, //initialize stores to full
    true, //diffusion
    true, //consumption
    true, //lipogenesis
    true, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    24, //simulation duration
    MealType::None, //meal type
    "StarvingKetogenesisTest" //test name
  );
}

void BioGearsEngineTest::AnaerobicExerciseTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    false, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    true, //initialize stores to full
    true, //diffusion
    true, //consumption
    true, //lipogenesis
    true, //gluconeogenesis
    true, //anaerobic exercise
    50, //Watts of additional work done anaerobically
    sOutputDirectory,
    1, //simulation duration
    MealType::None, //meal type
    "AnaerobicExerciseTest" //test name
  );
}

void BioGearsEngineTest::HormoneStabilityTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    false, //absorption of a meal
    true, //dynamic hormones
    false, //glycogen
    false, //protein storage
    false, //fat storage
    false, //initialize stores to full
    false, //diffusion
    false, //consumption
    false, //lipogenesis
    false, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    24, //simulation duration
    MealType::None, //meal type
    "HormoneStabilityTest" //test name
  );
}

void BioGearsEngineTest::StorageTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    true, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    false, //initialize stores to full
    true, //diffusion
    true, //consumption
    false, //lipogenesis
    false, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    12, //simulation duration
    MealType::Balanced, //meal type
    "StorageTest" //test name
  );
}

void BioGearsEngineTest::FullStoresFastingTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    false, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    true, //initialize stores to full
    true, //diffusion
    true, //consumption
    true, //lipogenesis
    true, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    12, //simulation duration
    MealType::None, //meal type
    "FullStoresFastingTest" //test name
  );
}

void BioGearsEngineTest::LipogenesisTest(const std::string& sOutputDirectory)
{
  NutrientKineticsTest(
    true, //absorption of a meal
    true, //dynamic hormones
    true, //glycogen
    true, //protein storage
    true, //fat storage
    true, //initialize stores to full
    true, //diffusion
    false, //consumption
    true, //lipogenesis
    false, //gluconeogenesis
    false, //anaerobic exercise
    0, //Watts of additional work done anaerobically
    sOutputDirectory,
    12, //simulation duration
    MealType::Balanced, //meal type
    "LipogenesisTest" //test name
  );
}
}