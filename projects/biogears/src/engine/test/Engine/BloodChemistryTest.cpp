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

//Property includes
#include <biogears/cdm/properties/SEScalarArea.h>

namespace biogears {

void BioGearsEngineTest::AcuteInflammationTest(const std::string& rptDirectory)
{
  //Items for scenario definition
  m_Logger->ResetLogFile(rptDirectory + "/AcuteInflammation.log");
  std::string stateFile = rptDirectory + "/ShockData.csv";
  std::string circuitFile = rptDirectory + "/AIR_Circuit.csv";
  std::string stabilizeFile = rptDirectory + "/Stabilization.csv";
  std::string cvFile = rptDirectory + "/CVCircuit.csv";
  BioGears bg(m_Logger);
  SECircuitManager circuits(m_Logger);
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  DataTrack circuitTrk;
  DataTrack stateTrk;
  DataTrack cvTrk;
  Info("Setting Up");

  //Set up patient
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();

  //Volume and pressure parameters for 4-element CV circuit (Brady2017Mathematical)
  //Volumes
  double totalVolume_mL = (3.229 * bg.GetPatient().GetSkinSurfaceArea(AreaUnit::m2) - 1.229) * 1000.0;
  double aortaTotalVolume_mL = 0.05 * totalVolume_mL;
  double venaCavaTotalVolume_mL = 0.247 * totalVolume_mL;
  double organBedVascularVolume_mL = 0.614 * totalVolume_mL;
  double skinVascularVolume_mL = 0.037 * totalVolume_mL;

  //double totalECF_mL = 17.

  //Tissue parameters
  double skinTissueMass_kg = 0.15;
  double skinTissueVolume_L = 1.092;   //Levitt2003Pharmacokinetics
  double skinEWFraction = 0.382;
  double Alb_Vasc_g_Per_dL = 4.5;
  double Alb_Extra_g_Per_dL = 2.0;
  double totalProteinVascular_g_Per_dL = Alb_Vasc_g_Per_dL * 1.6;
  double totalProteinInterstitium_g_Per_dL = Alb_Extra_g_Per_dL * 1.6;
  double vascularCOP_mmHg = 2.1 * totalProteinVascular_g_Per_dL + 0.16 * std::pow(totalProteinVascular_g_Per_dL, 2) + 0.009 * std::pow(totalProteinVascular_g_Per_dL, 3); //Mazzoni1988Dynamic
  double interstitialCOP_mmHg = 2.1 * totalProteinInterstitium_g_Per_dL + 0.16 * std::pow(totalProteinInterstitium_g_Per_dL, 2) + 0.009 * std::pow(totalProteinInterstitium_g_Per_dL, 3);
  double targetHydrostaticPressure_mmHg = vascularCOP_mmHg - interstitialCOP_mmHg;
  double endothelialResistance_mmHg_min_Per_mL = 1.0 / (1000 * skinTissueVolume_L);
  //Values for determining osmotic pressure from tissue integrity
  double averageProtein_g_Per_dL = (totalProteinVascular_g_Per_dL + totalProteinInterstitium_g_Per_dL) / 2.0; //Used to adjust osmotic pressure later
  double effectiveProteinVascular_g_Per_dL = totalProteinVascular_g_Per_dL;
  double effectiveProteinInterstitium_g_Per_dL = totalProteinInterstitium_g_Per_dL;
  double reflectionCoefficient = 1.0;

  //Pressures
  double aorta1Pressure_mmHg = 90.0;
  double aorta2Pressure_mmHg = 0.98 * aorta1Pressure_mmHg;
  double organBedPressure_mmHg = 0.33 * aorta1Pressure_mmHg;
  double skinVascularPressure_mmHg = 9.462; //From BioGears::SetUpCardiovascular
  double venaCavaPressure_mmHg = 3.5;
  double skinLumenPressure_mmHg = skinVascularPressure_mmHg - vascularCOP_mmHg;
  double skinInterstitialPressure_mmHg = skinLumenPressure_mmHg - targetHydrostaticPressure_mmHg;
  double skinGlycocalyxPressure_mmHg = skinInterstitialPressure_mmHg - interstitialCOP_mmHg;

  //Target flow
  double cardiacOutput_mL_Per_min = totalVolume_mL;
  double skinTargetFlow_mL_Per_min = 0.067 * cardiacOutput_mL_Per_min;
  double organTargetFlow_mL_Per_min = (1.0 - 0.067) * cardiacOutput_mL_Per_min;

  //Resistances
  double aortaResistance_mmHg_min_Per_mL = (aorta1Pressure_mmHg - aorta2Pressure_mmHg) / cardiacOutput_mL_Per_min;
  double organSupplyResistance_mmHg_min_Per_mL = (aorta2Pressure_mmHg - organBedPressure_mmHg) / organTargetFlow_mL_Per_min;
  double skinSupplyResistance_mmHg_min_Per_mL = (aorta2Pressure_mmHg - skinVascularPressure_mmHg) / skinTargetFlow_mL_Per_min;
  double organReturnResistance_mmHg_min_Per_mL = (organBedPressure_mmHg - venaCavaPressure_mmHg) / organTargetFlow_mL_Per_min;
  double skinReturnResistance_mmHg_min_Per_mL = (skinVascularPressure_mmHg - venaCavaPressure_mmHg) / skinTargetFlow_mL_Per_min;
  double veinousReturn_mmHg_min_Per_mL = venaCavaPressure_mmHg / cardiacOutput_mL_Per_min; //Need this to return to ground pressure upon entering heart

  //Compliances
  double aortaCompliance_mL_Per_mmHg = aortaTotalVolume_mL / aorta2Pressure_mmHg;
  double organBedCompliance_mL_Per_mmHg = organBedVascularVolume_mL / organBedPressure_mmHg;
  double skinCompliance_mL_Per_mmHg = skinVascularVolume_mL / skinVascularPressure_mmHg;
  double venaCavaCompliance_mL_Per_mmHg = venaCavaTotalVolume_mL / venaCavaPressure_mmHg;

  //Circuit Construction
  SEFluidCircuit* bgCircuit = &circuits.CreateFluidCircuit("BgCircuit");
  SELiquidCompartmentGraph* bgGraph = &bg.GetCompartments().CreateLiquidGraph("BgGraph");
  bgGraph->Clear();
  bgGraph->StateChange();
  //Nodes
  SEFluidCircuitNode& nAorta1 = bgCircuit->CreateNode("nAorta1");
  nAorta1.GetPressure().SetValue(aorta1Pressure_mmHg, PressureUnit::mmHg);
  nAorta1.GetNextPressure().SetValue(aorta1Pressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nAorta2 = bgCircuit->CreateNode("nAorta2");
  nAorta2.GetPressure().SetValue(aorta2Pressure_mmHg, PressureUnit::mmHg);
  nAorta2.GetNextPressure().SetValue(aorta2Pressure_mmHg, PressureUnit::mmHg);
  nAorta2.GetVolumeBaseline().SetValue(aortaTotalVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nOrganBed = bgCircuit->CreateNode("nOrganArt");
  nOrganBed.GetPressure().SetValue(organBedPressure_mmHg, PressureUnit::mmHg);
  nOrganBed.GetPressure().SetValue(organBedPressure_mmHg, PressureUnit::mmHg);
  nOrganBed.GetVolumeBaseline().SetValue(organBedVascularVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nSkin = bgCircuit->CreateNode("nSkin");
  nSkin.GetPressure().SetValue(skinVascularPressure_mmHg, PressureUnit::mmHg);
  nSkin.GetNextPressure().SetValue(skinVascularPressure_mmHg, PressureUnit::mmHg);
  nSkin.GetVolumeBaseline().SetValue(skinVascularVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nVein = bgCircuit->CreateNode("nVein");
  nVein.GetPressure().SetValue(venaCavaPressure_mmHg, PressureUnit::mmHg);
  nVein.GetNextPressure().SetValue(venaCavaPressure_mmHg, PressureUnit::mmHg);
  nVein.GetVolumeBaseline().SetValue(venaCavaTotalVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nHeart = bgCircuit->CreateNode("nHeart");
  nHeart.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  nHeart.GetNextPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& nGround = bgCircuit->CreateNode("nGround");
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  bgCircuit->AddReferenceNode(nGround);

  SEFluidCircuitNode& nSkinLumen = bgCircuit->CreateNode("nSkinLumen");
  nSkinLumen.GetPressure().SetValue(skinLumenPressure_mmHg, PressureUnit::mmHg);
  nSkinLumen.GetNextPressure().SetValue(skinLumenPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nSkinGlycocalyx = bgCircuit->CreateNode("nSkinGlycocalyx");
  nSkinGlycocalyx.GetPressure().SetValue(skinGlycocalyxPressure_mmHg, PressureUnit::mmHg);
  nSkinGlycocalyx.GetNextPressure().SetValue(skinGlycocalyxPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nSkinInterstitium = bgCircuit->CreateNode("nSkinInterstitial");
  nSkinInterstitium.GetPressure().SetValue(skinInterstitialPressure_mmHg, PressureUnit::mmHg);
  nSkinInterstitium.GetNextPressure().SetValue(skinInterstitialPressure_mmHg, PressureUnit::mmHg);
  nSkinInterstitium.GetVolumeBaseline().SetValue(skinTissueVolume_L * 1000.0, VolumeUnit::mL);

  //Paths
  SEFluidCircuitPath& pDrivePressure = bgCircuit->CreatePath(nHeart, nAorta1, "pPressureSource");
  pDrivePressure.GetPressureSourceBaseline().SetValue(aorta1Pressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pAorta1ToAorta2 = bgCircuit->CreatePath(nAorta1, nAorta2, "pAorta1ToAorta2");
  pAorta1ToAorta2.GetResistanceBaseline().SetValue(aortaResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pOrganBedInflow = bgCircuit->CreatePath(nAorta2, nOrganBed, "pOrganBedInflow");
  pOrganBedInflow.GetResistanceBaseline().SetValue(organSupplyResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pOrganBedOutflow = bgCircuit->CreatePath(nOrganBed, nVein, "pOrganBedOutflow");
  pOrganBedOutflow.GetResistanceBaseline().SetValue(organReturnResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pSkinInflow = bgCircuit->CreatePath(nAorta2, nSkin, "pSkinInflow");
  pSkinInflow.GetResistanceBaseline().SetValue(skinSupplyResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pSkinOutflow = bgCircuit->CreatePath(nSkin, nVein, "pSkinOutflow");
  pSkinOutflow.GetResistanceBaseline().SetValue(skinReturnResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pReturn = bgCircuit->CreatePath(nVein, nHeart, "pReturn");
  pReturn.GetResistanceBaseline().SetValue(veinousReturn_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pAortaToGround = bgCircuit->CreatePath(nAorta2, nGround, "pAortaCompliance");
  pAortaToGround.GetComplianceBaseline().SetValue(aortaCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pOrganBedToGround = bgCircuit->CreatePath(nOrganBed, nGround, "pOrganArterialCompliance");
  pOrganBedToGround.GetComplianceBaseline().SetValue(organBedCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pSkinToGround = bgCircuit->CreatePath(nSkin, nGround, "pSkinCompliance");
  pSkinToGround.GetComplianceBaseline().SetValue(skinCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pVeinToGround = bgCircuit->CreatePath(nVein, nGround, "pVeinCompliance");
  pVeinToGround.GetComplianceBaseline().SetValue(venaCavaCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pSkinVascularCOP = bgCircuit->CreatePath(nSkin, nSkinLumen, "pSkinVascularCOP");
  pSkinVascularCOP.GetPressureSourceBaseline().SetValue(-vascularCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pSkinEndothelium = bgCircuit->CreatePath(nSkinLumen, nSkinGlycocalyx, "pSkinEndothelium");
  pSkinEndothelium.GetResistanceBaseline().SetValue(endothelialResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pSkinInterstitialCOP = bgCircuit->CreatePath(nSkinGlycocalyx, nSkinInterstitium, "pSkinInterstitialCOP");
  pSkinInterstitialCOP.GetPressureSourceBaseline().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pSkinTissueCompliance = bgCircuit->CreatePath(nSkinInterstitium, nGround, "pSkinTissueCompliance");
  pSkinTissueCompliance.GetComplianceBaseline().SetValue(90.0, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pHeartToGround = bgCircuit->CreatePath(nGround, nHeart, "pHeartToGround");
 

  bgCircuit->SetNextAndCurrentFromBaselines();
  bgCircuit->StateChange();

  //Compartments
  SELiquidCompartment& cArtery = bg.GetCompartments().CreateLiquidCompartment("cArtery");
  cArtery.MapNode(nAorta1);
  cArtery.MapNode(nAorta2);
  SELiquidCompartment& cVein = bg.GetCompartments().CreateLiquidCompartment("cVein");
  cVein.MapNode(nVein);
  SELiquidCompartment& cSkin = bg.GetCompartments().CreateLiquidCompartment("cSkin");
  cSkin.MapNode(nSkin);
  cSkin.MapNode(nSkinLumen);
  SELiquidCompartment& cOrganBed = bg.GetCompartments().CreateLiquidCompartment("cOrgan");
  cOrganBed.MapNode(nOrganBed);
  SELiquidCompartment& cHeart = bg.GetCompartments().CreateLiquidCompartment("cHeart");
  cHeart.MapNode(nHeart);
  SELiquidCompartment& cSkinInterstitium = bg.GetCompartments().CreateLiquidCompartment("cSkinInterstitium");
  cSkinInterstitium.MapNode(nSkinGlycocalyx);
  cSkinInterstitium.MapNode(nSkinInterstitium);

  SETissueCompartment& SkinTissue = bg.GetCompartments().CreateTissueCompartment("cSkinTissue");
  SkinTissue.GetReflectionCoefficient().SetValue(reflectionCoefficient);

  //Links
  SELiquidCompartmentLink& lHeartToArtery = bg.GetCompartments().CreateLiquidLink(cHeart, cArtery, "HeartToArteryLink");
  lHeartToArtery.MapPath(pDrivePressure);
  SELiquidCompartmentLink& lArteryToOrgan = bg.GetCompartments().CreateLiquidLink(cArtery, cOrganBed, "ArteryToOrgansLink");
  lArteryToOrgan.MapPath(pOrganBedInflow);
  SELiquidCompartmentLink& lArteryToSkin = bg.GetCompartments().CreateLiquidLink(cArtery, cSkin, "ArteryToSkinLink");
  lArteryToSkin.MapPath(pSkinInflow);
  SELiquidCompartmentLink& lSkinToVein = bg.GetCompartments().CreateLiquidLink(cSkin, cVein, "SkinToVeinLink");
  lSkinToVein.MapPath(pSkinOutflow);
  SELiquidCompartmentLink& lOrganToVein = bg.GetCompartments().CreateLiquidLink(cOrganBed, cVein, "OrganToVeinLink");
  lOrganToVein.MapPath(pOrganBedOutflow);
  SELiquidCompartmentLink& lVeinToHeart = bg.GetCompartments().CreateLiquidLink(cVein, cHeart, "VeinToHeartLink");
  lVeinToHeart.MapPath(pReturn);
  SELiquidCompartmentLink& lSkinTissue = bg.GetCompartments().CreateLiquidLink(cSkin, cSkinInterstitium, "SkinTissueLink");
  lSkinTissue.MapPath(pSkinEndothelium);

  bgGraph->AddCompartment(cArtery);
  bgGraph->AddCompartment(cOrganBed);
  bgGraph->AddCompartment(cVein);
  bgGraph->AddCompartment(cHeart);
  bgGraph->AddCompartment(cSkin);
  bgGraph->AddCompartment(cSkinInterstitium);
  bgGraph->AddLink(lHeartToArtery);
  bgGraph->AddLink(lArteryToOrgan);
  bgGraph->AddLink(lOrganToVein);
  bgGraph->AddLink(lVeinToHeart);
  bgGraph->AddLink(lArteryToSkin);
  bgGraph->AddLink(lSkinToVein);
  bgGraph->AddLink(lSkinTissue);
  bgGraph->StateChange();


  //Model Parameters
  //Source terms
  double sM = 1.0, sN = 1.0, s6 = 0.001, s10 = 0.01;
  //Endotoxin decay
  double kL = 1.0;
  //Macrophage interaction
  double kML = 1.01, kMTR = 0.04, kM6 = 0.1, kMB = 0.0495, kMR = 0.05, kMD = 0.05, xML = 10.0, xMD = 1.0, xMTNF = 0.4, xM6 = 1.0, xM10 = 0.297, xMCA = 0.9;
  //Activate macrophage interactions
  double kMANO = 0.2, kMA = 0.2;
  //Neutrophil interactions
  double kNL = 0.15, kNTNF = 0.2, kN6 = 0.557, kNB = 0.1, kND = 0.05, kNTR = 0.02, kNTGF = 0.1, kNR = 0.05, kNNO = 0.4, kNA = 0.5, xNL = 15.0, xNTNF = 2.0, xN6 = 1.0, xND = 0.4, xN10 = 0.2, xNNO = 0.5;
  //Inducible NOS
  double kINOSN = 1.5, kINOSM = 0.1, kINOSEC = 0.1, kINOS6 = 2.0, kINOSd = 0.05, kINOS = 0.101, xINOS10 = 0.1, xINOSTNF = 0.05, xINOS6 = 0.1, xINOSNO = 0.3;
  //E NOS
  double kENOS = 4.0, kENOSEC = 0.05, xENOSTNF = 0.4, xENOSL = 1.015, xENOSTR = 0.1;
  //Nitric oxide
  double kN = 0.5, kNO3 = 0.46, kNOMA = 2.0;
  //TNF
  double kTNFN = 2.97, kTNFM = 0.1, kTNF = 1.4, xTNF6 = 0.059, xTNF10 = 0.079;
  //IL6
  double k6M = 3.03, k6TNF = 1.0, k62 = 3.4, k6NO = 2.97, k6 = 0.7, k6N = 0.2, x610 = 0.1782, x6TNF = 0.1, x66 = 0.2277, x6NO = 0.4;
  //IL10
  double k10MA = 0.1, k10N = 0.1, k10A = 62.87, k10TNF = 1.485, k106 = 0.051, k10 = 0.35, k10R = 0.1, x10TNF = 0.05, x1012 = 0.049, x106 = 0.08;
  //CA
  double kCA = 0.1, kCATR = 0.16;
  //IL12
  double k12M = 0.303, k12 = 0.05, x12TNF = 0.2, x126 = 0.2, x1210 = 0.2525;
  //Blood pressure
  double kB = 4.0, kBNO = 0.2, xBNO = 0.05;
  //Damage --- changed kDB from 0.02, changed xD6 from 0.25
  double kDB = 0.005, kD6 = 0.3, kD = 0.05, kDTR = 0.05, xD6 = 1.2, xDNO = 0.4;
  //Temperature parameters
  double kT = 1.0, kTTnf = 1.5, nTTnf = 0.2, hTTnf = 0.75, TMax = 39.5, TMin = 37.0, kT6 = 1.5, nT6 = 0.5, hT6 = 0.75, kT10 = 0.0625, nT10 = 0.2, hT10 = 1.0;
  //Heart rate parameters
  double kH = 0.2, nHT = 1.0, hHT = 2.0, HMax = 192.0, HBase = 72.0, tau2 = 0.003;
  //Pain threshold parameters
  double kPTP = 0.025, kPT = 0.011, PTM = 1.0;

  //Initial state
  double pathogen = 6.0;
  double macrophageResting = 1.0;
  double macrophageActive = 0.0;
  double neutrophilResting = 1.0;
  double neutrophilActive = 0.0;
  double iNOS = 0.0;
  double iNOSd = 0.0;
  double eNOS = 0.05;
  double NO3 = 0.0;
  double TNF = 0.0;
  double IL6 = 0.001;
  double IL10 = 0.01;
  double IL12 = 0.0;
  double Ca = 0.0;
  double bloodPressure = 1.0;
  double tissueIntegrity = 1.0;
  double temp = TMin;
  double heartRate = HBase;
  double pain = 1.0;

  double trauma = 0.0;
  double autonomic = 0.0;
  double NO = 0.0;
  double fB = 0.0;

  //Initialize differential terms
  double dPathogen;
  double dMacrophageResting;
  double dMacrophageActive;
  double dNeutrophilResting;
  double dNeutrophilActive;
  double dINOS;
  double dINOSD;
  double dENOS;
  double dNO3;
  double dTNF;
  double dIL6;
  double dIL10;
  double dIL12;
  double dCa;
  double dBloodPressure;
  double dTissueIntegrity;
  double dTemp;
  double dHeartRate;
  double dPain;

  //Simulation Parameters
  double simTime_min =24*60.0;
  double stabilizationTime_min = 15;
  double dt_s = 0.25;
  double stateTime_s = 0.0;
  double circuitTime_s = 0.0;
  double dt_h = dt_s / 3600.0;
  double stateTime_h = 0.0;

  //Physiological Effects
 // double respirationRate = 12.0 * (1.0 + 0.65 * (1.0 - pain)); //As in Respiratory::ProcessDriverActions
  double map_mmHg = nAorta1.GetPressure(PressureUnit::mmHg);
  double totalVascularVolume_mL;
  double systemicResistanceBase_mmHg_min_Per_mL = map_mmHg / cardiacOutput_mL_Per_min;
  double systemicResistance_mmHg_min_Per_mL = systemicResistanceBase_mmHg_min_Per_mL;
  double strokeVolume_mL = cardiacOutput_mL_Per_min / heartRate;


  //Stabilize circuit
  for (int i = 0; i < stabilizationTime_min * 60.0 / dt_s; i++) {
    calc.Process(*bgCircuit, dt_s);
    calc.PostProcess(*bgCircuit);
    circuitTrk.Track(circuitTime_s, *bgCircuit);
    circuitTime_s += dt_s;
  }
  bgCircuit->RemovePath("pHeartToGround");
  bgCircuit->StateChange();
  circuitTrk.WriteTrackToFile(stabilizeFile.c_str());
  totalVascularVolume_mL = cArtery.GetVolume(VolumeUnit::mL) + cVein.GetVolume(VolumeUnit::mL) + cSkin.GetVolume(VolumeUnit::mL) + cOrganBed.GetVolume(VolumeUnit::mL);
  cvTrk.WriteTrackToFile(cvFile.c_str());
  std::cout << "Stabilized circuit" << std::endl;
  //Advance model
  for (int i = 0; i <= simTime_min * 60.0 / dt_s; i++) {

    //Record state
    stateTrk.Track("Pathogen", stateTime_h, pathogen);
    stateTrk.Track("Macrophage_Resting", stateTime_h, macrophageResting);
    stateTrk.Track("Macrophage_Active", stateTime_h, macrophageActive);
    stateTrk.Track("Neutrophil_Resting", stateTime_h, neutrophilResting);
    stateTrk.Track("Neutrophil_Active", stateTime_h, neutrophilActive);
    stateTrk.Track("Nitric_INOS", stateTime_h, iNOS);
    stateTrk.Track("Nitric_INOSD", stateTime_h, iNOSd);
    stateTrk.Track("Nitric_ENOS", stateTime_h, eNOS);
    stateTrk.Track("Nitric_NO3", stateTime_h, NO3 * 1000.0);
    stateTrk.Track("Nitric_NO", stateTime_h, NO);
    stateTrk.Track("TNF", stateTime_h, TNF * 35000.0);
    stateTrk.Track("IL-6", stateTime_h, IL6 * 17000.0);
    stateTrk.Track("IL-10", stateTime_h, IL10 * 8000.0);
    stateTrk.Track("IL-12", stateTime_h, IL12);
    stateTrk.Track("Catecholmines", stateTime_h, Ca);
    stateTrk.Track("PressureModifier", stateTime_h, bloodPressure);
    stateTrk.Track("TissueIntegrity", stateTime_h, tissueIntegrity);
    stateTrk.Track("BloodPressure_Scale", stateTime_h, bloodPressure);
    stateTrk.Track("Systemic_Resistance", stateTime_h, systemicResistanceBase_mmHg_min_Per_mL);
    stateTrk.Track("CoreTemp", stateTime_h, temp);
    stateTrk.Track("HeartRate", stateTime_h, heartRate);
    stateTrk.Track("PainThreshold", stateTime_h, pain);

    stateTrk.Track("AortaVolume_L", stateTime_h, cArtery.GetVolume(VolumeUnit::L));
    stateTrk.Track("OrganBedVolume_L", stateTime_h, cOrganBed.GetVolume(VolumeUnit::L));
    stateTrk.Track("SkinVascularVolume_L", stateTime_h, cSkin.GetVolume(VolumeUnit::L));
    stateTrk.Track("VeinousVolume_L", stateTime_h, cVein.GetVolume(VolumeUnit::L));
    stateTrk.Track("SkinInterstitialVolume_L", stateTime_h, cSkinInterstitium.GetVolume(VolumeUnit::L));
    stateTrk.Track("VascularVolume_L", stateTime_h, totalVascularVolume_mL / 1000.0);
    stateTrk.Track("ProteinVascular", stateTime_h, effectiveProteinVascular_g_Per_dL);
    stateTrk.Track("ProteinInterstitial", stateTime_h, effectiveProteinInterstitium_g_Per_dL);
  
    stateTrk.Track("SystemicResistance", stateTime_h, systemicResistance_mmHg_min_Per_mL);
    stateTrk.Track("EndothelialResistance", stateTime_h, pSkinEndothelium.GetResistance(FlowResistanceUnit::mmHg_min_Per_mL));
    stateTrk.Track("TotalVolume_L", stateTime_h, totalVolume_mL / 1000.0);


    //Intermediate
    if (1.0 - bloodPressure > 0.0) {
      fB = std::pow(1.0 - bloodPressure, 4.0);
    } else {
      fB = 0.0;
    }

    dPathogen = -kL * pathogen;
    dMacrophageResting = -((kML * Up1(pathogen, xML, 2.0) + kMD * Up2(1.0 - tissueIntegrity, xMD, 4.0)) * (Up2(TNF, xMTNF, 2.0) + kM6 * Up2(IL6, xM6, 2.0)) + kMTR * trauma + kMB * fB) * macrophageResting * Down(IL10 + Ca, xM10, 2.0) - kMR * (macrophageResting - sM); 
    dMacrophageActive = ((kML * Up1(pathogen, xML, 2.0) + kMD * Up2(1.0 - tissueIntegrity, xMD, 4.0)) * (Up2(TNF, xMTNF, 2.0) + kM6 * Up2(IL6, xM6, 2.0)) + kMTR * trauma + kMB * fB) * macrophageResting * Down(IL10 + Ca, xM10, 2) - kMA * macrophageActive;
    dNeutrophilResting = -(kNL * Up1(pathogen, xNL, 1.0) + kNTNF * Up1(TNF, xNTNF, 1.0) + kN6 * Up1(IL6, xN6, 2.0) + kND * Up1(1.0 - tissueIntegrity, xND, 2.0) + kNB * fB * kNTR * trauma) * Down(IL10 + Ca, xN10, 2.0) * neutrophilResting - kNR * (neutrophilResting - sN);
    dNeutrophilActive = (kNL * Up1(pathogen, xNL, 1.0) + kNTNF * Up1(TNF, xNTNF, 1.0) + kN6 * Up1(IL6, xN6, 2.0) + kND * Up1(1.0 - tissueIntegrity, xND, 2.0) + kNB * fB + kNTR * trauma) * Down(IL10 + Ca, xN10, 2.0) * neutrophilResting - kNA * neutrophilActive;
    dINOS = kINOS * (iNOSd - iNOS);
    dINOSD = (kINOSN * neutrophilActive + kINOSM * macrophageActive + kINOSEC * (Up1(TNF, xINOSTNF, 2.0) + kINOS6 * Up1(IL6, xINOS6, 2.0))) * Down(IL10, xINOS10, 2.0) * Down(NO, xINOSNO, 2.0) - kINOSd * iNOSd;
    dENOS = kENOSEC * Down(TNF, xENOSTNF, 1.0) * Down(pathogen, xENOSL, 1.0) * Down(trauma, xENOSTR, 4.0) - kENOS * eNOS;
    dNO3 = kNO3 * (NO - NO3);
    dTNF = (kTNFN * neutrophilActive + kTNFM * macrophageActive) * Down(IL10 + Ca, xTNF10, 2.0) * Down(IL6, xTNF6, 3.0) - kTNF * TNF;
    dIL6 = (k6N * neutrophilActive + macrophageActive) * (k6M + k6TNF * Up2(TNF, x6TNF, 2.0) + k6NO * Up2(NO, x6NO, 2.0)) * Down(IL10 + Ca, x610, 2.0) + k6 * (s6 - IL6);
    dIL10 = (k10N * neutrophilActive + macrophageActive * (1 + k10A * autonomic)) * (k10MA + k10TNF * Up2(TNF, x10TNF, 4.0) + k106 * Up2(IL6, x106, 4.0)) * ((1 - k10R) * Down(IL12, x1012, 4.0) + k10R) - k10 * (IL10 - s10);
    dIL12 = k12M * macrophageActive * Down(IL10, x1210, 2.0) - k12 * IL12;
    dCa = kCATR * autonomic - kCA * Ca;
    dBloodPressure = kB * (1.0 / (1.0 + kBNO * (NO - xBNO)) - bloodPressure);
    dTissueIntegrity = kD * (1.0 - tissueIntegrity)*tissueIntegrity - tissueIntegrity * (kDB * fB + kD6 * Up2(IL6, xD6, 2.0) + kDTR * trauma) * (1.0 / (std::pow(xDNO, 2.0) + std::pow(NO, 2.0)));
    dTemp = -temp + kT * (TMax - TMin) * (kTTnf * Up2(TNF, nTTnf, hTTnf) + kT6 * Up2(IL6, nT6, hT6) - kT10 * (1 - Down(IL10, nT10, hT10))) + TMin;
    dHeartRate = (-heartRate + kH * (HMax - HBase) * Up2(temp-TMin,nHT,hHT)+HBase) / tau2;
    dPain = -kPTP * pathogen * pain + kPT * (PTM - pain);


    pathogen += (dPathogen * dt_h);
    macrophageResting += (dMacrophageResting * dt_h);
    macrophageActive += (dMacrophageActive * dt_h);
    neutrophilResting += (dNeutrophilResting * dt_h);
    neutrophilActive += (dNeutrophilActive * dt_h);
    iNOS += (dINOS * dt_h);
    iNOSd += (dINOSD * dt_h);
    eNOS += (dENOS * dt_h);
    NO3 += (dNO3 * dt_h);
    TNF += (dTNF * dt_h);
    IL6 += (dIL6 * dt_h);
    IL10 += (dIL10 * dt_h);
    IL12 += (dIL12 * dt_h);
    Ca += (dCa * dt_h);
    bloodPressure += (dBloodPressure * dt_h);
    tissueIntegrity += (dTissueIntegrity * dt_h);
    temp += (dTemp * dt_h);
    heartRate += (dHeartRate * dt_h);
    pain += (dPain * dt_h);

    NO = iNOS * (1.0 + kNOMA * (macrophageActive + neutrophilActive)) + eNOS;


    // //Physiological effects
    // //Tissue
    reflectionCoefficient = GeneralMath::LinearInterpolator(1.0, 0, 1.0, 0, tissueIntegrity);
    SkinTissue.GetReflectionCoefficient().SetValue(reflectionCoefficient);
    BLIM(reflectionCoefficient, 0.0, 1.0);
    effectiveProteinVascular_g_Per_dL = GeneralMath::LinearInterpolator(0.0, 1.0, averageProtein_g_Per_dL, totalProteinVascular_g_Per_dL, reflectionCoefficient);
    effectiveProteinInterstitium_g_Per_dL = GeneralMath::LinearInterpolator(0.0, 1.0, averageProtein_g_Per_dL, totalProteinInterstitium_g_Per_dL, reflectionCoefficient);
    vascularCOP_mmHg = 2.1 * effectiveProteinVascular_g_Per_dL + 0.16 * std::pow(effectiveProteinVascular_g_Per_dL, 2) + 0.009 * std::pow(effectiveProteinVascular_g_Per_dL, 3); //Mazzoni1988Dynamic
    interstitialCOP_mmHg = 2.1 * effectiveProteinInterstitium_g_Per_dL + 0.16 * std::pow(effectiveProteinInterstitium_g_Per_dL, 2) + 0.009 * std::pow(effectiveProteinInterstitium_g_Per_dL, 3);
    pSkinVascularCOP.GetNextPressureSource().SetValue(-vascularCOP_mmHg, PressureUnit::mmHg);
    pSkinInterstitialCOP.GetNextPressureSource().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);
    pSkinEndothelium.GetNextResistance().SetValue(pSkinEndothelium.GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * std::pow(10.0, tissueIntegrity - 1.0), FlowResistanceUnit::mmHg_min_Per_mL);

    //Cardiovascular
    for (auto p : bgCircuit->GetPaths()) {
      if (p->HasResistanceBaseline() && p->GetName() != "pSkinEndothelium") {
        p->GetNextResistance().SetValue(p->GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * bloodPressure, FlowResistanceUnit::mmHg_min_Per_mL);
      }
    }

    systemicResistance_mmHg_min_Per_mL = systemicResistanceBase_mmHg_min_Per_mL * bloodPressure;
    cardiacOutput_mL_Per_min = strokeVolume_mL * heartRate;
    map_mmHg = systemicResistance_mmHg_min_Per_mL * cardiacOutput_mL_Per_min;
    pDrivePressure.GetNextPressureSource().SetValue(map_mmHg, PressureUnit::mmHg);

    totalVascularVolume_mL = cArtery.GetVolume(VolumeUnit::mL) + cVein.GetVolume(VolumeUnit::mL) + cSkin.GetVolume(VolumeUnit::mL) + cOrganBed.GetVolume(VolumeUnit::mL);
    totalVolume_mL = totalVascularVolume_mL + cSkinInterstitium.GetVolume(VolumeUnit::mL);

    //Process Circuit
    calc.Process(*bgCircuit, dt_s);

    //Post-process circuit
    calc.PostProcess(*bgCircuit);



    stateTime_s += dt_s;
    stateTime_h += dt_h;

    if (i % (3000 * 30) == 0) {
      std::cout << stateTime_s / 60 << " min" << std::endl;
    }
  }
  std::cout << "Writing state" << std::endl;
  stateTrk.WriteTrackToFile(stateFile.c_str());
 /* std::cout << "Writing circuit" << std::endl;
  circuitTrk.WriteTrackToFile(circuitFile.c_str());*/
}

double BioGearsEngineTest::Up1(double y, double x, double n)
{
  return std::pow(y, n) / (1.0 + std::pow(y / x, n));
}
double BioGearsEngineTest::Up2(double y, double x, double n)
{
  return std::pow(y, n) / (std::pow(x, n) + std::pow(y, n));
}
double BioGearsEngineTest::Down(double y, double x, double n)
{
  return 1.0 / (1.0 + std::pow(y / x, n));
}



double BioGearsEngineTest::Hill(double x, double& n, double& h)
{
  return (std::pow(x, h)) / (std::pow(x, h) + std::pow(n, h));
}

double BioGearsEngineTest::DownReg(double x, double& cin, double& hin, double& ilbar, double& il)
{
  return x * ((1.0 - cin) / (1.0 + std::pow(il / ilbar, hin)) + cin);
}





/*    
 *      //Model Parameters--From Reynolds2008Mathematical (substance interactions) and Brady2017Mathematical (temp, HR, pain modifiers)
  //Notes:
  //First Letter: s = source, u = decay, k = interaction
  //States:  m = macrophage, nr = resting neutrophil, na = active neutophil, TNF = tumor necrosis factor, IL = interleukin-10
  // NO = nitric oxide, TI = tissue integrity, Z = inflammation level, Fe = temperature (fever), HR = heart rate, Pa = pain threshold
  //b suffix denotes substance in blood
  //All rates are /hr
  //Volumes (change later based on compartments)
  double volumeBlood = totalVolume_mL / 1000.0, volumeTis =cSkinInterstitium.GetVolume(VolumeUnit::L);
  //Source and decay terms
  double sm = 10.0, snb = sm, sb = 0.0075, sbb = sb, um = 0.12, uma = 0.05, ut = 1.8, utb = ut, unb = um, unba = 0.05, una = unba;
  double uiMax = 0.34, uiMin = 0.005, uibMax = uiMax, uibMin = uiMin, ur = 4, urb = ur;
  //Pathogen population parameters
  double kpg = 0.6, kpbg = kpg, pInf = 20000.0, pbInf = 20.0;
  //Tissue integrity and repair
  double ktg = 2.0, TInf = 1.0;
  //Proportion of macrophages and neutrophils consumed during phagocytosis
  double r = 0.98, rb = r;
  //Background immune system (in this case, first b = background, second b = blood)
  double kpb = 0.461, kpbb = kpb, ub = 0.0023, ubb = ub, kbp = 0.0001, kbbp = 0.02;
  //Activation of macrophages and neutrophils
  double kmp = 5.0, kmtcell = 20.0, kmtmol = 5.0, kmrcell = 0.01, knpbcell = 5.0, kntbcell = 2.0, kntbmol = 0.8, knrbcell = 0.1;
  //Cytokine production
  double kmai = 1000.0, kmat = 3000.0;
  //Depletion of pathogen
  double kmappath = 2.8, knappath = 5.8, knapbpath = 0.04, knapcell = 5.0, knapbcell = knapcell;
  //Tissue depletion from radicals
  double krtt = 0.01;
  //Radical production
  double krtr = 0.1, knar = 0.01, knarb = knar, knatr = knar, krntp = 0.2, krtmp = 0.00001;
  //Inflammation hill coefficients
  double ktz = 0.5, uz = 0.01, xtz = 20.0, ktzi = 0.1, htz = 2.0;
  //Tissue integrity threshold
  double a = 0.1;
  //TNF activation hill parameters
  double xt = 20.0, xtb = 2.0, ht = 2.0, htb = ht;
  //Neutrophil self-regulation
  double ksnb = 3.0, xsnb = 200.0, hsnb = 2.0;
  //Pathogen interaction hill terms
  double xpma = 200.0, xpna = 1500.0, hpma = 3.0, hpna = hpma, xpb = 0.1, hpb = 1.0;
  //Hill parameters for TNF production
  double xma = 80.0, hma = 2.0;
  //Hill parameters for IL10
  double vi = 8.0, hi = 2.0, xmi = 140, hmi = 2.0, vib = vi, hib = hi;
  //IL10 down-regulation parameters
  double dci = 0.05, dhi = 3.0, dilbar = 200.0;
  double dcin = 0.15, dhin = 1.0, dilnbar = 80.0;
  double dcit = 1e-6, dhit = 5.0, diltbar = 60.0;
  double dcib = 0.05, dhib = 3.0, dilbbar = 200.0;
  double dcinb = 0.15, dhinb = 1.0, dilnbbar = 80.0;
  //Diffusion parameters
  double difbn = 0.005; 
  double diffn = 50;
  double difbmol = 10.0; 
  double diffmol = 1.0;
  double difpb = 1.0, diffp = 1.0;
  double difat = 50.0; 
  double difbt = 0.8;
  double difab = 2000.0;
  //Temperature parameters
  double kT = 1.0, kTTnf = 1.5, nTTnf = 30.0, hTTnf = 0.75, TMax = 39.5, TMin = 37.0;
  //Heart rate parameters
  double kH = 0.353, nHT = 1.0, hHT = 2.0, HMax = 192.0, HBase = 72.0, tau2 = 0.003;
  //Pain threshold parameters
  double kPTP = 0.00025, kPT = 0.011, PTM = 1.0;
  //Resistance parameters
  double kB = 0.35;
  double kBNO = 0.05;
  double minRScale = 0.5;

  //State variables
  double pathogenTis = 100.0;
  double pathogenBlood = 0.0;
  double macrophageRestingTis = sm / um;
  double macrophageActiveTis = 0.0;
  double tnfTis = 0.0;
  double tnfBlood = 0.0;
  double neutrophilRestingBlood = snb / unb;
  double neutrophilActiveTis = 0.0;
  double neutrophilActiveBlood = 0.0;
  double ilTis = 0.0;
  double ilBlood = 0.0;
  double nitricTis = 0.0;
  double nitricBlood = 0.0;
  double tissueIntegrity = 1.0;
  double inflammation = 0.0;
  double temp = TMin;
  double heartRate = HBase;
  double pain = PTM;
  double resistanceScale = 1.0;

  //Declare diffusion and differential terms outside of loop
  //Diffusion Terms
  double difZ;
  double difPTis;
  double difPBlood;
  double difMol;
  double difN;
  double dPathogenTis;
  double dPathogenBlood;
  double dMacrophageRestingTis;
  double dMacrophageActiveTis;
  double dTnfTis;
  double dTnfBlood;
  double dNeutrophilRestingBlood;
  double dNeutrophilActiveTis;
  double dNeutrophilActiveBlood;
  double dIlTis;
  double dIlBlood;
  double dNitricTis;
  double dNitricBlood;
  double dTissueIntegrity;
  double dInflammation;
  double dTemp;
  double dHeartRate;
  double dPain;
  double dResistanceScale;
  double tisVolumeScale = skinTissueVolume_L / 0.5;
  double bloodVolumeScale = skinVascularVolume_mL / 1000.0 / 50.0;

  
    //Diffusion Terms
    volumeBlood = skinVascularVolume_mL / 1000.0;
    volumeTis = cSkinInterstitium.GetVolume(VolumeUnit::L);
    bloodVolumeScale = volumeBlood / 50.0;
    volumeTis = volumeTis / 0.5;
    difZ = difpb * (1.0 + diffp * inflammation);
    if (pathogenTis > 0.0) {
      difPTis = (difat * std::pow(pathogenTis, 2.0 / 3.0)) / (1.0 + difbt * std::pow(pathogenTis, 1.0 / 3.0));
    } else {
      difPTis = 0.0;
    }
    if (pathogenBlood > 0.0) {
      difPBlood = difab * pathogenBlood;
    } else {
      difPBlood = 0;
    }

    difMol = difbmol * (1.0 + diffmol * inflammation);
    difN = difbn * (1.0 + diffn * inflammation);

    //Differential terms
    dPathogenTis = kpg * pathogenTis * (1.0 - pathogenTis / pInf) - kmappath * DownReg(macrophageActiveTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) - knappath * DownReg(neutrophilActiveTis * Hill(pathogenTis, xpna, hpna), dcin, dhin, dilnbar, ilTis) - sb * kpb * pathogenTis / (ub + kbp * pathogenTis) + difZ * (difPBlood - difPTis) / volumeTis * tisVolumeScale;
    dPathogenBlood = kpbg * pathogenBlood * (1.0 - pathogenBlood / pbInf) - knapbpath * DownReg(neutrophilActiveBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood) + difZ * (difPTis - difPBlood) / volumeBlood * bloodVolumeScale - sbb * kpbb * pathogenBlood / (ubb + kbbp * pathogenBlood);
    dMacrophageRestingTis = sm - um * macrophageRestingTis - (kmp * DownReg(macrophageRestingTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) + kmtcell * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmrcell * DownReg(macrophageRestingTis, dci, dhi, dilbar, ilTis) * nitricTis);
    dMacrophageActiveTis = -uma * macrophageActiveTis + (kmp * DownReg(macrophageRestingTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) + kmtcell * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmrcell * DownReg(macrophageRestingTis, dci, dhi, dilbar, ilTis) * nitricTis);
    dTnfTis = -ut * tnfTis + difMol * (tnfBlood - tnfTis) / volumeTis * tisVolumeScale - kmtmol * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmat * DownReg(Hill(macrophageActiveTis, xma, hma), dcit, dhit, diltbar, ilTis);
    dTnfBlood = -utb * tnfBlood + difMol * (tnfTis - tnfBlood) / volumeBlood * bloodVolumeScale - kntbmol * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood);
    dNeutrophilRestingBlood = (snb + ksnb * Hill(neutrophilActiveBlood + neutrophilActiveTis, xsnb, hsnb)) - unb * neutrophilRestingBlood - (kntbcell * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood) + knrbcell * DownReg(neutrophilRestingBlood * nitricBlood, dcinb, dhinb, dilnbbar, ilBlood) + knpbcell * DownReg(neutrophilRestingBlood * Hill(pathogenBlood, xpb, hpb), dcib, dhib, dilbbar, ilBlood));
    dNeutrophilActiveTis = -una * neutrophilActiveTis + difN * neutrophilActiveBlood / volumeTis * tisVolumeScale - (1 - r) * knapcell * DownReg(neutrophilActiveTis * Hill(pathogenTis, xpna, hpna), dcin, dhin, dilnbar, ilTis);
    dNeutrophilActiveBlood = -unba * neutrophilActiveBlood - difN * neutrophilActiveBlood / volumeBlood * bloodVolumeScale + (knpbcell * DownReg(neutrophilRestingBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood) + kntbcell * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood) + knrbcell * DownReg(neutrophilRestingBlood * nitricBlood, dcinb, dhinb, dilnbbar, ilBlood)) - (1 - rb) * knapbcell * DownReg(neutrophilActiveBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood);
    dIlTis = -(uiMax - (uiMax - uiMin) * Hill(ilTis, vi, hi)) * ilTis + difMol * (ilBlood - ilTis) / volumeTis * tisVolumeScale + kmai * DownReg(Hill(macrophageActiveTis, xmi, hmi), dci, dhi, dilbar, ilTis);
    dIlBlood = -(uibMax - (uibMax - uibMin) * Hill(ilBlood, vib, hib)) * ilBlood + difMol * (ilTis - ilBlood) / volumeBlood * bloodVolumeScale;
    dNitricTis = -ur * nitricTis + difMol * (nitricBlood - nitricTis) / volumeTis * tisVolumeScale + krtr * nitricBlood * tissueIntegrity + knar * DownReg(neutrophilActiveTis, dcin, dhin, dilnbar, ilTis) + knatr * neutrophilActiveTis * tissueIntegrity + krntp * neutrophilActiveTis * pathogenTis * tissueIntegrity + krtmp * macrophageActiveTis * pathogenTis * tissueIntegrity;
    dNitricBlood = -urb * nitricBlood + difMol * (nitricTis - nitricBlood) / volumeBlood * bloodVolumeScale + knarb * DownReg(neutrophilActiveBlood, dcinb, dhinb, dilnbbar, ilBlood);
    dTissueIntegrity = ktg * tissueIntegrity * (1.0 - tissueIntegrity / TInf) * (tissueIntegrity - a) - krtt * nitricTis * tissueIntegrity;
    dInflammation = ktz * (Hill(tnfTis, xtz, htz) + ktzi * (TInf - tissueIntegrity)) * (1 - inflammation) - uz * inflammation;
    dTemp = (TMax - temp) * (Hill(tnfTis, nTTnf, hTTnf) + ktzi * (TInf - tissueIntegrity)) - uz * (temp - TMin);
    dHeartRate = (-heartRate + kH * (HMax - HBase) * Hill(temp - TMin, nHT, hHT) + HBase) / tau2;
    dPain = -kPTP * pathogenTis * pain + kPT * (PTM - pain);
    dResistanceScale = kB * (resistanceScale - minRScale) * (1.0 / (1.0 + kBNO * nitricTis) - resistanceScale);
   

    //Increment state
    pathogenTis += (dPathogenTis * dt_h);
    pathogenBlood += (dPathogenBlood * dt_h);
    macrophageRestingTis += (dMacrophageRestingTis * dt_h);
    macrophageActiveTis += (dMacrophageActiveTis * dt_h);
    tnfTis += (dTnfTis * dt_h);
    tnfBlood += (dTnfBlood * dt_h);
    neutrophilRestingBlood += (dNeutrophilRestingBlood * dt_h);
    neutrophilActiveTis += (dNeutrophilActiveTis * dt_h);
    neutrophilActiveBlood += (dNeutrophilActiveBlood * dt_h);
    ilTis += (dIlTis * dt_h);
    ilBlood += (dIlBlood * dt_h);
    nitricTis += (dNitricTis * dt_h);
    nitricBlood += (dNitricBlood * dt_h);
    tissueIntegrity += (dTissueIntegrity * dt_h);
    inflammation += (dInflammation * dt_h);
    temp += (dTemp * dt_h);
    heartRate += (dHeartRate * dt_h);
    pain += (dPain * dt_h);
    resistanceScale += (dResistanceScale * dt_h);







    //Track State
    stateTrk.Track("Pathogen_Tissue", stateTime_h, pathogenTis);
    stateTrk.Track("Pathogen_Blood", stateTime_h, pathogenBlood);
    stateTrk.Track("MacrophageTissue_Resting", stateTime_h, macrophageRestingTis);
    stateTrk.Track("MacrophageTissue_Active", stateTime_h, macrophageActiveTis);
    stateTrk.Track("TNF_Tissue", stateTime_h, tnfTis);
    stateTrk.Track("TNF_Blood", stateTime_h, tnfBlood);
    stateTrk.Track("NeutrophilTissue_Active", stateTime_h, neutrophilActiveTis);
    stateTrk.Track("NeutrophilBlood_Resting", stateTime_h, neutrophilRestingBlood);
    stateTrk.Track("NeutrophilBlood_Active", stateTime_h, neutrophilActiveBlood);
    stateTrk.Track("IL10_Tissue", stateTime_h, ilTis);
    stateTrk.Track("IL10_Blood", stateTime_h, ilBlood);
    stateTrk.Track("NO_Tissue", stateTime_h, nitricTis);
    stateTrk.Track("NO_Blood", stateTime_h, nitricBlood);
    stateTrk.Track("TissueIntegrity", stateTime_h, tissueIntegrity);
    stateTrk.Track("Inflammation", stateTime_h, inflammation);
    stateTrk.Track("CoreTemp", stateTime_h, temp);
    stateTrk.Track("HeartRate", stateTime_h, heartRate);
    stateTrk.Track("PainThreshold", stateTime_h, pain);
    stateTrk.Track("RespirationRate", stateTime_h, respirationRate);
    stateTrk.Track("MAP-Set", stateTime_h, map_mmHg);
    stateTrk.Track("AortaVolume_L", stateTime_h, cArtery.GetVolume(VolumeUnit::L));
    stateTrk.Track("OrganBedVolume_L", stateTime_h, cOrganBed.GetVolume(VolumeUnit::L));
    stateTrk.Track("SkinVascularVolume_L", stateTime_h, cSkin.GetVolume(VolumeUnit::L));
    stateTrk.Track("VeinousVolume_L", stateTime_h, cVein.GetVolume(VolumeUnit::L));
    stateTrk.Track("SkinInterstitialVolume_L", stateTime_h, cSkinInterstitium.GetVolume(VolumeUnit::L));
    stateTrk.Track("VascularVolume_L", stateTime_h, totalVascularVolume_mL / 1000.0);
    stateTrk.Track("ProteinVascular", stateTime_h, effectiveProteinVascular_g_Per_dL);
    stateTrk.Track("ProteinInterstitial", stateTime_h, effectiveProteinInterstitium_g_Per_dL);
    stateTrk.Track("ResistanceScale", stateTime_h, resistanceScale);
    stateTrk.Track("SystemicResistance", stateTime_h, systemicResistance_mmHg_min_Per_mL);
    stateTrk.Track("EndothelialResistance", stateTime_h, pSkinEndothelium.GetResistance(FlowResistanceUnit::mmHg_min_Per_mL));
    stateTrk.Track("TotalVolume_L", stateTime_h, totalVolume_mL / 1000.0);*/


}