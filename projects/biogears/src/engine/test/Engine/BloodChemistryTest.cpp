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
  std::string resultsFile = rptDirectory + "/AcuteInflammation.csv";
  BioGears bg(m_Logger);
  SECircuitManager circuits(m_Logger);
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  DataTrack circuitTrk;
  DataTrack testTrk;
  DataTrack trk;

  //Set up patient
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();

  //Volume and pressure parameters for 4-element CV circuit (Brady2017Mathematical)
  //Volumes
  double totalVolume_mL = (3.229 * bg.GetPatient().GetSkinSurfaceArea(AreaUnit::m2) - 1.229) * 1000.0;
  double systemicVolume_mL = 0.85 * totalVolume_mL;
  double arterialTotalVolume_mL = 0.2 * systemicVolume_mL;
  double veinousTotalVolume_mL = 0.8 * systemicVolume_mL;
  double arteryVolume_mL = 0.85 * arterialTotalVolume_mL;
  double arteryStressedVolume_mL = 0.18 * arteryVolume_mL;
  double arteryOrganVolume_mL = 0.15 * arterialTotalVolume_mL;
  double arteryOrganStressedVolume_mL = 0.18 * arteryOrganVolume_mL;
  double veinVolume_mL = 0.85 * veinousTotalVolume_mL;
  double veinStressedVolume_mL = 0.05 * veinVolume_mL;
  double veinOrganVolume_mL = 0.15 * veinousTotalVolume_mL;
  double veinOrganStressedVolume_mL = 0.05 * veinOrganVolume_mL;
  //Pressures
  double arterialPressure_mmHg = 90.0;
  double arterialOrganPressure_mmHg = 0.98 * arterialPressure_mmHg;
  double veinousPressure_mmHg = 3.5;
  double veinousOrganPressure_mmHg = 3.75;

  //Target flow
  double cardiacOutput_mL_Per_min = totalVolume_mL;

  //Resistances
  double arterialResistance_mmHg_min_Per_mL = (arterialPressure_mmHg - arterialOrganPressure_mmHg) / cardiacOutput_mL_Per_min;
  double organResistance_mmHg_min_Per_mL = (arterialOrganPressure_mmHg - veinousOrganPressure_mmHg) / cardiacOutput_mL_Per_min;
  double veinousResistance_mmHg_min_Per_mL = (veinousOrganPressure_mmHg - veinousPressure_mmHg) / cardiacOutput_mL_Per_min;
  double veinousReturn_mmHg_min_Per_mL = veinousPressure_mmHg / cardiacOutput_mL_Per_min; //Need this to return to ground pressure upon entering heart

  //Compliances
  double arterialCompliance_mL_Per_mmHg = arteryStressedVolume_mL / arterialPressure_mmHg;
  double arterialOrganCompliance_mL_Per_mmHg = arteryOrganStressedVolume_mL / arterialOrganPressure_mmHg;
  double veinousOrganCompliance_mL_Per_mmHg = veinOrganStressedVolume_mL / veinousOrganPressure_mmHg;
  double veinousCompliance_mL_Per_mmHg = veinStressedVolume_mL / veinousPressure_mmHg;

  //Circuit Construction
  SEFluidCircuit* bgCircuit = &circuits.CreateFluidCircuit("BgCircuit");
  SELiquidCompartmentGraph* bgGraph = &bg.GetCompartments().CreateLiquidGraph("BgGraph");
  bgGraph->Clear();
  bgGraph->StateChange();
  //Nodes
  SEFluidCircuitNode& nArt = bgCircuit->CreateNode("nArt");
  nArt.GetPressure().SetValue(arterialPressure_mmHg, PressureUnit::mmHg);
  nArt.GetNextPressure().SetValue(arterialPressure_mmHg, PressureUnit::mmHg);
  nArt.GetVolumeBaseline().SetValue(arteryVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nOrganArt = bgCircuit->CreateNode("nOrganArt");
  nOrganArt.GetPressure().SetValue(arterialOrganPressure_mmHg, PressureUnit::mmHg);
  nOrganArt.GetPressure().SetValue(arterialOrganPressure_mmHg, PressureUnit::mmHg);
  nOrganArt.GetVolumeBaseline().SetValue(arteryOrganVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nOrganVein = bgCircuit->CreateNode("nOrganVein");
  nOrganVein.GetPressure().SetValue(veinousOrganPressure_mmHg, PressureUnit::mmHg);
  nOrganVein.GetNextPressure().SetValue(veinousOrganPressure_mmHg, PressureUnit::mmHg);
  nOrganVein.GetVolumeBaseline().SetValue(veinOrganVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nVein = bgCircuit->CreateNode("nVein");
  nVein.GetPressure().SetValue(veinousPressure_mmHg, PressureUnit::mmHg);
  nVein.GetNextPressure().SetValue(veinousPressure_mmHg, PressureUnit::mmHg);
  nVein.GetVolumeBaseline().SetValue(veinVolume_mL, VolumeUnit::mL);
  SEFluidCircuitNode& nHeart = bgCircuit->CreateNode("nHeart");
  nHeart.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  nHeart.GetNextPressure().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitNode& nGround = bgCircuit->CreateNode("nGround");
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  bgCircuit->AddReferenceNode(nGround);
  //Paths
  SEFluidCircuitPath& pDrivePressure = bgCircuit->CreatePath(nHeart, nArt, "pPressureSource");
  pDrivePressure.GetPressureSourceBaseline().SetValue(arterialPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pArterialTree = bgCircuit->CreatePath(nArt, nOrganArt, "pArterialTree");
  pArterialTree.GetResistanceBaseline().SetValue(arterialResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pOrganFlow = bgCircuit->CreatePath(nOrganArt, nOrganVein, "pOrganFlow");
  pOrganFlow.GetResistanceBaseline().SetValue(organResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pVeinousTree = bgCircuit->CreatePath(nOrganVein, nVein, "pVeinousTree");
  pVeinousTree.GetResistanceBaseline().SetValue(veinousResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pReturn = bgCircuit->CreatePath(nVein, nHeart, "pReturn");
  pReturn.GetResistanceBaseline().SetValue(veinousReturn_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& pArtToGround = bgCircuit->CreatePath(nArt, nGround, "pArterialCompliance");
  pArtToGround.GetComplianceBaseline().SetValue(arterialCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pOrganArtToGround = bgCircuit->CreatePath(nOrganArt, nGround, "pOrganArterialCompliance");
  pOrganArtToGround.GetComplianceBaseline().SetValue(arterialOrganCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pOrganVeinToGround = bgCircuit->CreatePath(nOrganVein, nGround, "pOrganVeinCompliance");
  pOrganVeinToGround.GetComplianceBaseline().SetValue(veinousCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pVeinToGround = bgCircuit->CreatePath(nVein, nGround, "pVeinCompliance");
  pVeinToGround.GetComplianceBaseline().SetValue(veinousCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& pHeartToGround = bgCircuit->CreatePath(nHeart, nGround, "pHeartToGround");

  bgCircuit->SetNextAndCurrentFromBaselines();
  bgCircuit->StateChange();

  //Compartments
  SELiquidCompartment& cArtery = bg.GetCompartments().CreateLiquidCompartment("cArtery");
  cArtery.MapNode(nArt);
  SELiquidCompartment& cVein = bg.GetCompartments().CreateLiquidCompartment("cVein");
  cVein.MapNode(nVein);
  SELiquidCompartment& cOrgans = bg.GetCompartments().CreateLiquidCompartment("cOrgan");
  cOrgans.MapNode(nOrganArt);
  cOrgans.MapNode(nOrganVein);
  SELiquidCompartment& cHeart = bg.GetCompartments().CreateLiquidCompartment("cHeart");
  cHeart.MapNode(nHeart);

  //Links
  SELiquidCompartmentLink& lHeartToArtery = bg.GetCompartments().CreateLiquidLink(cHeart, cArtery, "HeartToArteryLink");
  lHeartToArtery.MapPath(pDrivePressure);
  SELiquidCompartmentLink& lArteryToOrgan = bg.GetCompartments().CreateLiquidLink(cArtery, cOrgans, "ArteryToOrgansLink");
  lArteryToOrgan.MapPath(pArterialTree);
  SELiquidCompartmentLink& lOrganToVein = bg.GetCompartments().CreateLiquidLink(cOrgans, cVein, "OrganToVeinLink");
  lOrganToVein.MapPath(pVeinousTree);
  SELiquidCompartmentLink& lVeinToHeart = bg.GetCompartments().CreateLiquidLink(cVein, cHeart, "VeinToHeartLink");

  bgGraph->AddCompartment(cArtery);
  bgGraph->AddCompartment(cOrgans);
  bgGraph->AddCompartment(cVein);
  bgGraph->AddCompartment(cHeart);
  bgGraph->AddLink(lHeartToArtery);
  bgGraph->AddLink(lArteryToOrgan);
  bgGraph->AddLink(lOrganToVein);
  bgGraph->AddLink(lVeinToHeart);
  bgGraph->StateChange();

  //Simulation Parameters
  double simTime_min = 10.0;
  double dt_s = 0.02;
  double t_s = 0.0;

  for (int i = 0; i < simTime_min * 60.0 / dt_s; i++)
  {
    pDrivePressure.GetNextPressureSource().SetValue(90.0 + 20*sin(t_s / 10.0), PressureUnit::mmHg);
    calc.Process(*bgCircuit,dt_s);
    calc.PostProcess(*bgCircuit);
    circuitTrk.Track(t_s, *bgCircuit);
    t_s += dt_s;
  }
  circuitTrk.WriteTrackToFile(resultsFile.c_str());


}

}