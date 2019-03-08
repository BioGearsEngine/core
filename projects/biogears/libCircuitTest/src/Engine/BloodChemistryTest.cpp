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
  std::string stabilizeFile2 = rptDirectory + "/Stabilization2.csv";
  std::string cvFile = rptDirectory + "/CVCircuit.csv";
  BioGears bg(m_Logger, std::string{"./"});
  SECircuitManager circuits(m_Logger);
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  DataTrack circuitTrk;
  DataTrack circuitTrk2;
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

  //Albumin
  SESubstance& Albumin = bg.GetSubstances().GetAlbumin();
  bg.GetSubstances().AddActiveSubstance(Albumin);

  //Tissue parameters
  double skinTissueMass_kg = 0.15;
  double skinTissueVolume_L = 1.092; //Levitt2003Pharmacokinetics
  double organTissueVolume_L = 10.0;
  double skinEWFraction = 0.382;
  double Alb_Vasc_g_Per_dL = 4.5;
  double Alb_Extra_g_Per_dL = 2.0;
  double totalProteinVascular_g_Per_dL = Alb_Vasc_g_Per_dL * 1.6;
  double totalProteinInterstitium_g_Per_dL = Alb_Extra_g_Per_dL * 1.6;
  double vascularCOP_mmHg = 2.1 * totalProteinVascular_g_Per_dL + 0.16 * std::pow(totalProteinVascular_g_Per_dL, 2) + 0.009 * std::pow(totalProteinVascular_g_Per_dL, 3); //Mazzoni1988Dynamic
  double interstitialCOP_mmHg = 2.1 * totalProteinInterstitium_g_Per_dL + 0.16 * std::pow(totalProteinInterstitium_g_Per_dL, 2) + 0.009 * std::pow(totalProteinInterstitium_g_Per_dL, 3);
  double targetNetGradient_mmHg = 5.0;
  double targetHydrostaticGradient_mmHg = targetNetGradient_mmHg + (vascularCOP_mmHg - interstitialCOP_mmHg); //COP gradient opposes flow into capillaries.  Thus, to get to target total gradient we need a hydrostatic gradient to oppose it

  //Values for determining osmotic pressure from tissue integrity
  double averageProtein_g_Per_dL = (totalProteinVascular_g_Per_dL + totalProteinInterstitium_g_Per_dL) / 2.0; //Used to adjust osmotic pressure later
  double effectiveProteinVascular_g_Per_dL = totalProteinVascular_g_Per_dL;
  double effectiveProteinInterstitium_g_Per_dL = totalProteinInterstitium_g_Per_dL;
  double reflectionCoefficient = 0.95;

  //Pressures
  double aorta1Pressure_mmHg = 90.0;
  double aorta2Pressure_mmHg = 0.98 * aorta1Pressure_mmHg;
  double organBedPressure_mmHg = 0.33 * aorta1Pressure_mmHg;
  double skinVascularPressure_mmHg = 9.462; //From BioGears::SetUpCardiovascular
  double venaCavaPressure_mmHg = 3.5;
  double skinLumenPressure_mmHg = skinVascularPressure_mmHg - vascularCOP_mmHg;
  double skinInterstitialPressure_mmHg = skinVascularPressure_mmHg - targetHydrostaticGradient_mmHg;
  double skinGlycocalyxPressure_mmHg = skinInterstitialPressure_mmHg - interstitialCOP_mmHg;
  ;
  double organLumenPressure_mmHg = skinVascularPressure_mmHg - vascularCOP_mmHg;
  double organInterstitialPressure_mmHg = organBedPressure_mmHg - targetHydrostaticGradient_mmHg;
  double organGlycocalyxPressure_mmHg = organInterstitialPressure_mmHg - interstitialCOP_mmHg;
  double lymphPressure_mmHg = venaCavaPressure_mmHg + 4.5; //Based on values from LymphLinearCircuit doc
  double skinToLymphPump_mmHg = lymphPressure_mmHg - skinInterstitialPressure_mmHg;
  double organToLymphPump_mmHg = lymphPressure_mmHg - organInterstitialPressure_mmHg;

  //Target flow
  double cardiacOutput_mL_Per_min = totalVolume_mL;
  double skinTargetFlow_mL_Per_min = 0.067 * cardiacOutput_mL_Per_min;
  double organTargetFlow_mL_Per_min = (1.0 - 0.067) * cardiacOutput_mL_Per_min;
  double filteredFlow_mL_Per_min = 3.0;
  double skinFilteredFlow_mL_Per_min = 0.067 * filteredFlow_mL_Per_min;
  double organFilteredFlow_mL_Per_min = (1.0 - 0.067) * filteredFlow_mL_Per_min;

  //Estimated albumin filtration (needed for lymph volume calculation)

  //Resistances
  double aortaResistance_mmHg_min_Per_mL = (aorta1Pressure_mmHg - aorta2Pressure_mmHg) / cardiacOutput_mL_Per_min;
  double organSupplyResistance_mmHg_min_Per_mL = (aorta2Pressure_mmHg - organBedPressure_mmHg) / organTargetFlow_mL_Per_min;
  double skinSupplyResistance_mmHg_min_Per_mL = (aorta2Pressure_mmHg - skinVascularPressure_mmHg) / skinTargetFlow_mL_Per_min;
  double organReturnResistance_mmHg_min_Per_mL = (organBedPressure_mmHg - venaCavaPressure_mmHg) / organTargetFlow_mL_Per_min;
  double skinReturnResistance_mmHg_min_Per_mL = (skinVascularPressure_mmHg - venaCavaPressure_mmHg) / skinTargetFlow_mL_Per_min;
  double veinousReturn_mmHg_min_Per_mL = venaCavaPressure_mmHg / cardiacOutput_mL_Per_min; //Need this to return to ground pressure upon entering heart
  double skinEndothelialResistance_mmHg_min_Per_mL = targetNetGradient_mmHg / skinFilteredFlow_mL_Per_min;
  double organEndotheialResistance_mmHg_min_Per_mL = targetNetGradient_mmHg / organFilteredFlow_mL_Per_min;
  double lymphReturnResistance_mmHg_min_Per_mL = (lymphPressure_mmHg - venaCavaPressure_mmHg) / filteredFlow_mL_Per_min;

  //Compliances
  double aortaCompliance_mL_Per_mmHg = aortaTotalVolume_mL / aorta2Pressure_mmHg;
  double organBedCompliance_mL_Per_mmHg = organBedVascularVolume_mL / organBedPressure_mmHg;
  double skinCompliance_mL_Per_mmHg = skinVascularVolume_mL / skinVascularPressure_mmHg;
  double venaCavaCompliance_mL_Per_mmHg = venaCavaTotalVolume_mL / venaCavaPressure_mmHg;
  double skinInterstitialCompliance_mL_Per_mmHg = 90.0;
  double organInterstitialCompliance_mL_Per_mmHg = organTissueVolume_L * 1000.0 / organInterstitialPressure_mmHg;
  double lymphCompliance_mL_Per_mmHg = 1000.0 / lymphPressure_mmHg; //Lymph volume = 1.0 L      //1.0 / 175.0;

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

  //Added for albumin test
  SEFluidCircuitNode& nOrganLumen = bgCircuit->CreateNode("nOrganLumen");
  nOrganLumen.GetPressure().SetValue(organLumenPressure_mmHg, PressureUnit::mmHg);
  nOrganLumen.GetNextPressure().SetValue(organLumenPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nOrganGlycocalyx = bgCircuit->CreateNode("nOrganGlycocalyx");
  nOrganGlycocalyx.GetPressure().SetValue(organGlycocalyxPressure_mmHg, PressureUnit::mmHg);
  nOrganGlycocalyx.GetNextPressure().SetValue(organGlycocalyxPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitNode& nOrganInterstitial = bgCircuit->CreateNode("nOrganInterstitial");
  nOrganInterstitial.GetPressure().SetValue(organInterstitialPressure_mmHg, PressureUnit::mmHg);
  nOrganInterstitial.GetNextPressure().SetValue(organInterstitialPressure_mmHg, PressureUnit::mmHg);
  nOrganInterstitial.GetVolumeBaseline().SetValue(organTissueVolume_L, VolumeUnit::L);
  SEFluidCircuitNode& nLymphCollect = bgCircuit->CreateNode("nLymphCollect");
  nLymphCollect.GetPressure().SetValue(lymphPressure_mmHg, PressureUnit::mmHg);
  nLymphCollect.GetNextPressure().SetValue(lymphPressure_mmHg, PressureUnit::mmHg);
  nLymphCollect.GetVolumeBaseline().SetValue(1.0, VolumeUnit::L);

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
  pSkinEndothelium.GetResistanceBaseline().SetValue(skinEndothelialResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pSkinInterstitialCOP = bgCircuit->CreatePath(nSkinGlycocalyx, nSkinInterstitium, "pSkinInterstitialCOP");
  pSkinInterstitialCOP.GetPressureSourceBaseline().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pSkinTissueCompliance = bgCircuit->CreatePath(nSkinInterstitium, nGround, "pSkinTissueCompliance");
  pSkinTissueCompliance.GetComplianceBaseline().SetValue(skinInterstitialCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pHeartToGround = bgCircuit->CreatePath(nGround, nHeart, "pHeartToGround");

  //Paths added for Albumin Test
  SEFluidCircuitPath& pOrganVascularCOP = bgCircuit->CreatePath(nOrganBed, nOrganLumen, "pOrganVascularCOP");
  pOrganVascularCOP.GetPressureSourceBaseline().SetValue(-vascularCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pOrganEndothelium = bgCircuit->CreatePath(nOrganLumen, nOrganGlycocalyx, "pOrganEndothelium");
  pOrganEndothelium.GetResistanceBaseline().SetValue(organEndotheialResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pOrganInterstitialCOP = bgCircuit->CreatePath(nOrganGlycocalyx, nOrganInterstitial, "pOrganInterstitialCOP");
  pOrganInterstitialCOP.GetPressureSourceBaseline().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pOrganCompliance = bgCircuit->CreatePath(nOrganInterstitial, nGround, "pOrganTissueCompliance");
  pOrganCompliance.GetComplianceBaseline().SetValue(organInterstitialCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pSkinToLymph = bgCircuit->CreatePath(nSkinInterstitium, nLymphCollect, "pSkinToLymph");
  pSkinToLymph.GetPressureSourceBaseline().SetValue(skinToLymphPump_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pOrganToLymph = bgCircuit->CreatePath(nOrganInterstitial, nLymphCollect, "pOrganToLymph");
  pOrganToLymph.GetPressureSourceBaseline().SetValue(organToLymphPump_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pLymphReturn = bgCircuit->CreatePath(nLymphCollect, nVein, "pLymphReturn");
  pLymphReturn.GetResistanceBaseline().SetValue(lymphReturnResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pLymphCompliance = bgCircuit->CreatePath(nLymphCollect, nGround, "pLymphCompliance");
  pLymphCompliance.GetComplianceBaseline().SetValue(lymphCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  bgCircuit->SetNextAndCurrentFromBaselines();
  bgCircuit->StateChange();

  //Compartments
  SELiquidCompartment& cArtery = bg.GetCompartments().CreateLiquidCompartment("cArtery");
  cArtery.MapNode(nHeart);
  cArtery.MapNode(nAorta1);
  cArtery.MapNode(nAorta2);
  SELiquidCompartment& cVein = bg.GetCompartments().CreateLiquidCompartment("cVein");
  cVein.MapNode(nVein);
  SELiquidCompartment& cSkin = bg.GetCompartments().CreateLiquidCompartment("cSkin");
  cSkin.MapNode(nSkin);
  cSkin.MapNode(nSkinLumen);
  SELiquidCompartment& cOrganBed = bg.GetCompartments().CreateLiquidCompartment("cOrgan");
  cOrganBed.MapNode(nOrganBed);
  //SELiquidCompartment& cHeart = bg.GetCompartments().CreateLiquidCompartment("cHeart");
  //cHeart.MapNode(nHeart);
  SELiquidCompartment& cSkinInterstitium = bg.GetCompartments().CreateLiquidCompartment("cSkinInterstitium");
  cSkinInterstitium.MapNode(nSkinGlycocalyx);
  cSkinInterstitium.MapNode(nSkinInterstitium);
  SELiquidCompartment& cOrganInterstitium = bg.GetCompartments().CreateLiquidCompartment("cOrganInterstitium");
  cOrganInterstitium.MapNode(nOrganGlycocalyx);
  cOrganInterstitium.MapNode(nOrganInterstitial);
  SELiquidCompartment& cLymph = bg.GetCompartments().CreateLiquidCompartment("cLymph");
  cLymph.MapNode(nLymphCollect);

  SETissueCompartment& SkinTissue = bg.GetCompartments().CreateTissueCompartment("cSkinTissue");
  SkinTissue.GetReflectionCoefficient().SetValue(reflectionCoefficient);
  SETissueCompartment& OrganTissue = bg.GetCompartments().CreateTissueCompartment("cOrganTissue");
  OrganTissue.GetReflectionCoefficient().SetValue(reflectionCoefficient);

  //Links
  //SELiquidCompartmentLink& lHeartToArtery = bg.GetCompartments().CreateLiquidLink(cHeart, cArtery, "HeartToArteryLink");
  //lHeartToArtery.MapPath(pDrivePressure);
  SELiquidCompartmentLink& lArteryToOrgan = bg.GetCompartments().CreateLiquidLink(cArtery, cOrganBed, "ArteryToOrgansLink");
  lArteryToOrgan.MapPath(pOrganBedInflow);
  SELiquidCompartmentLink& lArteryToSkin = bg.GetCompartments().CreateLiquidLink(cArtery, cSkin, "ArteryToSkinLink");
  lArteryToSkin.MapPath(pSkinInflow);
  SELiquidCompartmentLink& lSkinToVein = bg.GetCompartments().CreateLiquidLink(cSkin, cVein, "SkinToVeinLink");
  lSkinToVein.MapPath(pSkinOutflow);
  SELiquidCompartmentLink& lOrganToVein = bg.GetCompartments().CreateLiquidLink(cOrganBed, cVein, "OrganToVeinLink");
  lOrganToVein.MapPath(pOrganBedOutflow);
  SELiquidCompartmentLink& lVeinToHeart = bg.GetCompartments().CreateLiquidLink(cVein, cArtery, "VeinToHeartLink");
  lVeinToHeart.MapPath(pReturn);

  bgGraph->AddCompartment(cArtery);
  bgGraph->AddCompartment(cOrganBed);
  bgGraph->AddCompartment(cVein);
  //bgGraph->AddCompartment(cHeart);
  bgGraph->AddCompartment(cSkin);
  bgGraph->AddCompartment(cSkinInterstitium);
  bgGraph->AddCompartment(cOrganInterstitium);
  bgGraph->AddCompartment(cLymph);
  //bgGraph->AddLink(lHeartToArtery);
  bgGraph->AddLink(lArteryToOrgan);
  bgGraph->AddLink(lOrganToVein);
  bgGraph->AddLink(lVeinToHeart);
  bgGraph->AddLink(lArteryToSkin);
  bgGraph->AddLink(lSkinToVein);
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
  double pathogen = 12.0;
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
  double simTime_min = 420.0;
  double stabilizationTime_min = 15.0;
  double secondaryStabilizationTime_min = 45.0;
  double dt_s = 0.25;
  double stateTime_s = 0.0;
  double circuitTime_s = 0.0;
  double secondaryTime_s = 0.0;
  double dt_h = dt_s / 3600.0;
  double stateTime_h = 0.0;
  double scale = 1.0;

  //Physiological Effects
  // double respirationRate = 12.0 * (1.0 + 0.65 * (1.0 - pain)); //As in Respiratory::ProcessDriverActions
  double map_mmHg = nAorta1.GetPressure(PressureUnit::mmHg);
  double totalVascularVolume_mL;
  double totalInterstitialVolume_mL;
  double systemicResistanceBase_mmHg_min_Per_mL = map_mmHg / cardiacOutput_mL_Per_min;
  double systemicResistance_mmHg_min_Per_mL = systemicResistanceBase_mmHg_min_Per_mL;
  double strokeVolume_mL = cardiacOutput_mL_Per_min / heartRate;

  double albuminMoved_ug = 0.0; //debugging
  double totalAlbumin_mg = 0.0;
  double dt_min = dt_s / 60.0;
  double netAlbuminDiffusion_ug = 0.0;
  double interstitialVolume_mL = 0.0;

  std::vector<SELiquidCompartment*> vasc = { &cArtery, &cSkin, &cOrganBed, &cVein };
  std::vector<SELiquidCompartment*> extra = { &cOrganInterstitium, &cSkinInterstitium, &cLymph };

  //Initial stabilization (no albumin)
  for (int i = 0; i < stabilizationTime_min * 60.0 / dt_s; i++) {
    calc.Process(*bgCircuit, dt_s);
    calc.PostProcess(*bgCircuit);

    totalVascularVolume_mL = cArtery.GetVolume(VolumeUnit::mL) + cVein.GetVolume(VolumeUnit::mL) + cSkin.GetVolume(VolumeUnit::mL) + cOrganBed.GetVolume(VolumeUnit::mL);
    circuitTrk.Track(circuitTime_s, *bgCircuit);
    circuitTrk.Track("TotalVascularVolume", circuitTime_s, totalVascularVolume_mL);
    circuitTime_s += dt_s;
  }
  bgCircuit->RemovePath("pHeartToGround");
  bgCircuit->StateChange();
  circuitTrk.WriteTrackToFile(stabilizeFile.c_str());

  cvTrk.WriteTrackToFile(cvFile.c_str());
  std::cout << "Initial Stabilization" << std::endl;

  //Initialize albumin concentrations and balance
  cArtery.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cVein.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cOrganBed.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cSkin.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Vasc_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cSkinInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cOrganInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  cLymph.GetSubstanceQuantity(Albumin)->GetConcentration().SetValue(Alb_Extra_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  for (auto comp : bgGraph->GetCompartments()) {
    comp->GetSubstanceQuantity(Albumin)->Balance(BalanceLiquidBy::Concentration);
    totalAlbumin_mg += comp->GetSubstanceQuantity(Albumin)->GetMass(MassUnit::mg);
  }

  //Secondary stabilization
  for (int i = 0; i <= secondaryStabilizationTime_min * 60 / dt_s; i++) {

    UpdateOncoticPressure(cSkin.GetSubstanceQuantity(Albumin)->GetConcentration(), cSkinInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration(), pSkinVascularCOP, pSkinInterstitialCOP);
    UpdateOncoticPressure(cOrganBed.GetSubstanceQuantity(Albumin)->GetConcentration(), cOrganInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration(), pOrganVascularCOP, pOrganInterstitialCOP);
    calc.Process(*bgCircuit, dt_s);
    txpt.Transport(*bgGraph, dt_s);
    totalAlbumin_mg = 0.0;
    netAlbuminDiffusion_ug = 0.0;
    albuminMoved_ug = AlbuminTransport(cSkin, cSkinInterstitium, pSkinEndothelium, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug += albuminMoved_ug;
    circuitTrk2.Track("Alb_Skin_VtoE", secondaryTime_s, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cSkinInterstitium, cLymph, pSkinToLymph, Albumin, dt_min, tissueIntegrity);
    circuitTrk2.Track("Alb_Skin_EtoL", secondaryTime_s, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cOrganBed, cOrganInterstitium, pOrganEndothelium, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug += albuminMoved_ug;
    circuitTrk2.Track("Alb_Organ_VtoE", secondaryTime_s, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cOrganInterstitium, cLymph, pOrganToLymph, Albumin, dt_min, tissueIntegrity);
    circuitTrk2.Track("Alb_Organ_EtoL", secondaryTime_s, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cLymph, cVein, pLymphReturn, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug -= albuminMoved_ug;
    circuitTrk2.Track("Alb_Lymph_Return", secondaryTime_s, albuminMoved_ug);
    circuitTrk2.Track("Alb_NetDiffusion", secondaryTime_s, netAlbuminDiffusion_ug);

    for (auto comp : bgGraph->GetCompartments()) {
      comp->GetSubstanceQuantity(Albumin)->Balance(BalanceLiquidBy::Mass);
    }
    for (auto comp : bgGraph->GetCompartments()) {
      totalAlbumin_mg += comp->GetSubstanceQuantity(Albumin)->GetMass(MassUnit::mg);
    }

    calc.PostProcess(*bgCircuit);

    totalVascularVolume_mL = cArtery.GetVolume(VolumeUnit::mL) + cVein.GetVolume(VolumeUnit::mL) + cSkin.GetVolume(VolumeUnit::mL) + cOrganBed.GetVolume(VolumeUnit::mL);
    interstitialVolume_mL = cSkinInterstitium.GetVolume(VolumeUnit::mL) + cOrganInterstitium.GetVolume(VolumeUnit::mL);
    circuitTrk2.Track(secondaryTime_s, *bgCircuit);
    circuitTrk2.Track("InterstitialVolume", secondaryTime_s, interstitialVolume_mL);
    circuitTrk2.Track("TotalVascularVolume", secondaryTime_s, totalVascularVolume_mL);
    circuitTrk2.Track("TotalAlbumin_mg", secondaryTime_s, totalAlbumin_mg);
    circuitTrk2.Track(secondaryTime_s, *bgGraph);
    secondaryTime_s += dt_s;
  }
  std::cout << "Secondary stabilization" << std::endl;
  circuitTrk2.WriteTrackToFile(stabilizeFile2.c_str());

  for (int i = 0; i < simTime_min * 60.0 / dt_s; i++) {
    totalVascularVolume_mL = cArtery.GetVolume(VolumeUnit::mL) + cVein.GetVolume(VolumeUnit::mL) + cSkin.GetVolume(VolumeUnit::mL) + cOrganBed.GetVolume(VolumeUnit::mL);
    totalInterstitialVolume_mL = cSkinInterstitium.GetVolume(VolumeUnit::mL) + cOrganInterstitium.GetVolume(VolumeUnit::mL) + cLymph.GetVolume(VolumeUnit::mL);

    totalVolume_mL = totalVascularVolume_mL + totalInterstitialVolume_mL;
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
    stateTrk.Track("TotalInterstitialVolume_mL", stateTime_h, totalInterstitialVolume_mL);

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
    dTissueIntegrity = kD * (1.0 - tissueIntegrity) * tissueIntegrity - tissueIntegrity * (kDB * fB + kD6 * Up2(IL6, xD6, 2.0) + kDTR * trauma) * (1.0 / (std::pow(xDNO, 2.0) + std::pow(NO, 2.0)));
    dTemp = -temp + kT * (TMax - TMin) * (kTTnf * Up2(TNF, nTTnf, hTTnf) + kT6 * Up2(IL6, nT6, hT6) - kT10 * (1 - Down(IL10, nT10, hT10))) + TMin;
    dHeartRate = (-heartRate + kH * (HMax - HBase) * Up2(temp - TMin, nHT, hHT) + HBase) / tau2;
    dPain = -kPTP * pathogen * pain + kPT * (PTM - pain);

    pathogen += (dPathogen * dt_h)*scale;
    macrophageResting += (dMacrophageResting * dt_h)*scale;
    macrophageActive += (dMacrophageActive * dt_h)*scale;
    neutrophilResting += (dNeutrophilResting * dt_h)*scale;
    neutrophilActive += (dNeutrophilActive * dt_h)*scale;
    iNOS += (dINOS * dt_h)*scale;
    iNOSd += (dINOSD * dt_h)*scale;
    eNOS += (dENOS * dt_h)*scale;
    NO3 += (dNO3 * dt_h)*scale;
    TNF += (dTNF * dt_h)*scale;
    IL6 += (dIL6 * dt_h)*scale;
    IL10 += (dIL10 * dt_h)*scale;
    IL12 += (dIL12 * dt_h)*scale;
    Ca += (dCa * dt_h)*scale;
    bloodPressure += (dBloodPressure * dt_h) *scale;
    tissueIntegrity += (dTissueIntegrity * dt_h)*scale;
    temp += (dTemp * dt_h)*scale;
    heartRate += (dHeartRate * dt_h) * 0 * scale;   //Doesn't really play nice with the albumin transport (vein volume goes negative), so not including this right now
    pain += (dPain * dt_h)*scale;

    NO = iNOS * (1.0 + kNOMA * (macrophageActive + neutrophilActive)) + eNOS;

    // //Physiological effects
    // //Tissue
    reflectionCoefficient = GeneralMath::LinearInterpolator(1.0, 0, 1.0, 0, tissueIntegrity);
    SkinTissue.GetReflectionCoefficient().SetValue(reflectionCoefficient);
    BLIM(reflectionCoefficient, 0.0, 1.0);
    double endothelialResistanceScale = GeneralMath::ResistanceFunction(10.0, 1.0, 0.1, tissueIntegrity);
    endothelialResistanceScale = GeneralMath::LinearInterpolator(1.0, 0.0, 1.0, 0.1, tissueIntegrity);
    pSkinEndothelium.GetNextResistance().SetValue(pSkinEndothelium.GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * endothelialResistanceScale, FlowResistanceUnit::mmHg_min_Per_mL);
    pOrganEndothelium.GetNextResistance().SetValue(pOrganEndothelium.GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * endothelialResistanceScale, FlowResistanceUnit::mmHg_min_Per_mL);
   
    UpdateOncoticPressure(cSkin.GetSubstanceQuantity(Albumin)->GetConcentration(), cSkinInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration(), pSkinVascularCOP, pSkinInterstitialCOP);
    UpdateOncoticPressure(cOrganBed.GetSubstanceQuantity(Albumin)->GetConcentration(), cOrganInterstitium.GetSubstanceQuantity(Albumin)->GetConcentration(), pOrganVascularCOP, pOrganInterstitialCOP);
    
    //Cardiovascular
    for (auto p : bgCircuit->GetPaths()) {
      if (p->HasResistanceBaseline() && (p->GetName() != "pSkinEndothelium" && p->GetName() != "pOrganEndothelium")) {
        p->GetNextResistance().SetValue(p->GetResistanceBaseline(FlowResistanceUnit::mmHg_min_Per_mL) * bloodPressure, FlowResistanceUnit::mmHg_min_Per_mL);
      }
    }

    systemicResistance_mmHg_min_Per_mL = systemicResistanceBase_mmHg_min_Per_mL * bloodPressure;
    cardiacOutput_mL_Per_min = strokeVolume_mL * heartRate;
    map_mmHg = systemicResistance_mmHg_min_Per_mL * cardiacOutput_mL_Per_min;
    pDrivePressure.GetNextPressureSource().SetValue(map_mmHg, PressureUnit::mmHg);

    //Process Circuit
    calc.Process(*bgCircuit, dt_s);
    txpt.Transport(*bgGraph, dt_s);
    totalAlbumin_mg = 0.0;
    netAlbuminDiffusion_ug = 0.0;
    albuminMoved_ug = AlbuminTransport(cSkin, cSkinInterstitium, pSkinEndothelium, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug += albuminMoved_ug;
    stateTrk.Track("Alb_Skin_VtoE", stateTime_h, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cSkinInterstitium, cLymph, pSkinToLymph, Albumin, dt_min, tissueIntegrity);
    stateTrk.Track("Alb_Skin_EtoL", stateTime_h, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cOrganBed, cOrganInterstitium, pOrganEndothelium, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug += albuminMoved_ug;
    stateTrk.Track("Alb_Organ_VtoE", stateTime_h, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cOrganInterstitium, cLymph, pOrganToLymph, Albumin, dt_min, tissueIntegrity);
    stateTrk.Track("Alb_Organ_EtoL", stateTime_h, albuminMoved_ug);
    albuminMoved_ug = AlbuminTransport(cLymph, cVein, pLymphReturn, Albumin, dt_min, tissueIntegrity);
    netAlbuminDiffusion_ug -= albuminMoved_ug;
    //Post-process circuit
    calc.PostProcess(*bgCircuit);
    cvTrk.Track(stateTime_h, *bgCircuit);
    cvTrk.Track(stateTime_h, *bgGraph);
    stateTime_s += dt_s;
    stateTime_h += dt_h;

    if (i % (3000 * 30) == 0) {
      std::cout << stateTime_s / 60 << " min" << std::endl;
    }
  }
  std::cout << "Writing state" << std::endl;
  stateTrk.WriteTrackToFile(stateFile.c_str());
  cvTrk.WriteTrackToFile(cvFile.c_str());
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

double BioGearsEngineTest::AlbuminTransport(SELiquidCompartment& from, SELiquidCompartment& to, SEFluidCircuitPath& flowPath, SESubstance& alb, double dt_min, double tissueIntegrity)
{
  double alphaSmall = 0.84; //0.6;
  double alphaLarge = 1.0 - alphaSmall; //0.175;
  double diffusivityCoefficientSmallBase_mL_Per_min = 0.09;
  double diffusivityCoefficientLarge_mL_Per_min = 0.80;
  double reflectionCoefficientSmallBase = 0.975;
  double reflectionCoefficientLargeBase = 0.09;
  double fluidFlux_mL_Per_min = flowPath.GetFlow(VolumePerTimeUnit::mL_Per_min);

  //As tissue integrity declines, decrease reflection coefficient of small pores and increase their permeability (basically transition them to large pores)
  double reflectionCoefficientSmall = GeneralMath::LinearInterpolator(1.0, 0.0, reflectionCoefficientSmallBase, 0.1, tissueIntegrity);
  double reflectionCoefficientLarge = GeneralMath::LinearInterpolator(1.0, 0.0, reflectionCoefficientLargeBase, 0.1, tissueIntegrity);
  double diffusivityCoefficientSmall_mL_Per_min = GeneralMath::LinearInterpolator(1.0, 0.0, diffusivityCoefficientSmallBase_mL_Per_min, diffusivityCoefficientLarge_mL_Per_min, tissueIntegrity);

  double totalMassMoved_ug = 0.0;
  if (flowPath.GetName() == "pSkinEndothelium" || flowPath.GetName() == "pOrganEndothelium") {
    double albuminVascular_ug_Per_mL = from.GetSubstanceQuantity(alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    double albuminExtracellular_ug_Per_mL = to.GetSubstanceQuantity(alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
    double averageConcentration_ug_Per_mL = (albuminVascular_ug_Per_mL + albuminExtracellular_ug_Per_mL) / 2.0;

    double diffusiveSmall_ug_per_min = diffusivityCoefficientSmall_mL_Per_min * (albuminVascular_ug_Per_mL - albuminExtracellular_ug_Per_mL);
    double diffusiveLarge_ug_Per_min = diffusivityCoefficientLarge_mL_Per_min * (albuminVascular_ug_Per_mL - albuminExtracellular_ug_Per_mL);
    double convectiveSmall_ug_Per_min = (1.0 - reflectionCoefficientSmall) * alphaSmall * fluidFlux_mL_Per_min * averageConcentration_ug_Per_mL;
    double convectiveLarge_ug_Per_min = (1.0 - reflectionCoefficientLarge) * alphaLarge * fluidFlux_mL_Per_min * averageConcentration_ug_Per_mL;

    totalMassMoved_ug = (diffusiveSmall_ug_per_min + diffusiveLarge_ug_Per_min + convectiveSmall_ug_Per_min + convectiveLarge_ug_Per_min) * dt_min;
  } else {

    totalMassMoved_ug = from.GetSubstanceQuantity(alb)->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * fluidFlux_mL_Per_min * dt_min;
  }

  from.GetSubstanceQuantity(alb)->GetMass().IncrementValue(-totalMassMoved_ug, MassUnit::ug);
  to.GetSubstanceQuantity(alb)->GetMass().IncrementValue(totalMassMoved_ug, MassUnit::ug);
  return totalMassMoved_ug;
}

void BioGearsEngineTest::UpdateOncoticPressure(SEScalarMassPerVolume& albVas, SEScalarMassPerVolume& albExtra, SEFluidCircuitPath& plasma, SEFluidCircuitPath& inter)
{
  double albuminVacular_g_Per_dL = albVas.GetValue(MassPerVolumeUnit::g_Per_dL);
  double albuminInterstitial_g_Per_dL = albExtra.GetValue(MassPerVolumeUnit::g_Per_dL);
  double totalProteinVascular_g_Per_dL = 1.6 * albuminVacular_g_Per_dL;
  double totalProteinInterstitium_g_Per_dL = 1.6 * albuminInterstitial_g_Per_dL;

  double vascularCOP_mmHg = 2.1 * totalProteinVascular_g_Per_dL + 0.16 * std::pow(totalProteinVascular_g_Per_dL, 2) + 0.009 * std::pow(totalProteinVascular_g_Per_dL, 3); //Mazzoni1988Dynamic
  double interstitialCOP_mmHg = 2.1 * totalProteinInterstitium_g_Per_dL + 0.16 * std::pow(totalProteinInterstitium_g_Per_dL, 2) + 0.009 * std::pow(totalProteinInterstitium_g_Per_dL, 3);

  plasma.GetNextPressureSource().SetValue(-vascularCOP_mmHg, PressureUnit::mmHg);
  inter.GetNextPressureSource().SetValue(interstitialCOP_mmHg, PressureUnit::mmHg);
}

}