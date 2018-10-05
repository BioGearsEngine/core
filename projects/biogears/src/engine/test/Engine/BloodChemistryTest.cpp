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
  std::string stateFile = rptDirectory + "/AcuteInflammation.csv";
  std::string circuitFile = rptDirectory + "/AIR_Circuit.csv";
  BioGears bg(m_Logger);
  SECircuitManager circuits(m_Logger);
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  DataTrack circuitTrk;
  DataTrack stateTrk;

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

  //Model Parameters--From Reynolds2008Mathematical (substance interactions) and Brady2017Mathematical (temp, HR, pain modifiers)
  //Notes:
  //First Letter: s = source, u = decay, k = interaction
  //States:  m = macrophage, nr = resting neutrophil, na = active neutophil, TNF = tumor necrosis factor, IL = interleukin-10
  // NO = nitric oxide, TI = tissue integrity, Z = inflammation level, Fe = temperature (fever), HR = heart rate, Pa = pain threshold
  //b suffix denotes substance in blood
  //All rates are /hr
  //Volumes (change later based on compartments)
  double volumeBlood = 50.0, volumeTis = 0.5;
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
  double kmp = 40.0, kmtcell = 20.0, kmtmol = 5.0, kmrcell = 0.01, knpbcell = 5.0, kntbcell = 2.0, kntbmol = 0.8, knrbcell = 0.1;
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
  double difbn = 0.005, diffn = 50.0;
  double difbmol = 10.0, diffmol = 1.0;
  double difpb = 1.0, diffp = 1.0;
  double difat = 50.0, difbt = 0.8, difab = 2000.0;
  //Temperature parameters
  double kT = 1.0, kTTnf = 1.5, nTTnf = 30.0, hTTnf = 0.75, TMax = 39.5, TMin = 37.0;
  //Heart rate parameters
  double kH = 0.353, nHT = 1.0, hHT = 2.0, HMax = 192.0, HBase = 68.0, tau2 = 0.003;
  //Pain threshold parameters
  double kPTP = 0.0005, kPT = 0.011, PTM = 1.0;

  //State variables
  double pathogenTis = 500.0;
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
  //Simulation Parameters
  double simTime_min = 480.0;
  double stabilizationTime_min = 2.0;
  double dt_s = 0.02;
  double t_s = 0.0;
  double t_min = 0.0;
  double dt_h = dt_s / 3600.0;

  //Stabilize circuit
  for (int i = 0; i < stabilizationTime_min * 60.0 / dt_s; i++)
  {
    pDrivePressure.GetNextPressureSource().SetValue(90.0 + 20 * sin(t_s / 10.0), PressureUnit::mmHg);
    calc.Process(*bgCircuit, dt_s);
    calc.PostProcess(*bgCircuit);
    circuitTrk.Track(t_s, *bgCircuit);
    t_s += dt_s;
  }
    std::cout << "Stabilized circuit, writing to file" << std::endl;
    circuitTrk.WriteTrackToFile(circuitFile.c_str());
  //Advance model
  t_s = 0.0;
  t_min = 0.0;
  for (int i = 0; i < simTime_min * 60.0 / dt_s; i++) {
    //Diffusion Terms
    difZ = difpb * (1.0 + diffp * inflammation);
    if (pathogenTis > 0.0) {
      difPTis = (difat * std::pow(pathogenTis, 2.0 / 3.0)) / (1.0 + difbt * std::pow(pathogenTis, 1.0 / 3.0));
    } else {
      difPTis = 0.0;
    }
    difPBlood = difab * pathogenBlood;
    difMol = difbmol * (1.0 + diffmol * inflammation);
    difN = difbn * (1.0 + diffn * inflammation);

    //Differential terms
    dPathogenTis = kpg * pathogenTis * (1.0 - pathogenTis / pInf) - kmappath * DownReg(macrophageActiveTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) - knappath * DownReg(neutrophilActiveTis * Hill(pathogenTis, xpna, hpna), dcin, dhin, dilnbar, ilTis) - sb * kpb * pathogenTis / (ub + kbp * pathogenTis) + difZ * (difPBlood - difPTis) / volumeTis;
    dPathogenBlood = kpbg * pathogenBlood * (1.0 - pathogenBlood / pbInf) - knapbpath * DownReg(neutrophilActiveBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood) + difZ * (difPTis - difPBlood) / volumeBlood - sbb * kpbb * pathogenBlood / (ubb + kbbp * pathogenBlood);
    dMacrophageRestingTis = sm - um * macrophageRestingTis - (kmp * DownReg(macrophageRestingTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) + kmtcell * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmrcell * DownReg(macrophageRestingTis, dci, dhi, dilbar, ilTis) * nitricTis);
    dMacrophageActiveTis = -uma * macrophageActiveTis + (kmp * DownReg(macrophageRestingTis * Hill(pathogenTis, xpma, hpma), dci, dhi, dilbar, ilTis) + kmtcell * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmrcell * DownReg(macrophageRestingTis, dci, dhi, dilbar, ilTis) * nitricTis);
    dTnfTis = -ut * tnfTis + difMol * (tnfBlood - tnfTis) / volumeTis - kmtmol * DownReg(macrophageRestingTis * Hill(tnfTis, xt, ht), dci, dhi, dilbar, ilTis) + kmat * DownReg(Hill(macrophageActiveTis, xma, hma), dcit, dhit, diltbar, ilTis);
    dTnfBlood = -utb * tnfBlood + difMol * (tnfTis - tnfBlood) / volumeBlood - kntbmol * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood);
    dNeutrophilRestingBlood = (snb + ksnb * Hill(neutrophilActiveBlood + neutrophilActiveTis, xsnb, hsnb)) - unb * neutrophilRestingBlood - (kntbcell * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood) + knrbcell * DownReg(neutrophilRestingBlood * nitricBlood, dcinb, dhinb, dilnbbar, ilBlood) + knpbcell * DownReg(neutrophilRestingBlood * Hill(pathogenBlood, xpb, hpb), dcib, dhib, dilbbar, ilBlood));
    dNeutrophilActiveTis = -una * neutrophilActiveTis + difN * neutrophilActiveBlood / volumeTis - (1 - r) * knapcell * DownReg(neutrophilActiveTis * Hill(pathogenTis, xpna, hpna), dcin, dhin, dilnbar, ilTis);
    dNeutrophilActiveBlood = -unba * neutrophilActiveBlood - difN * neutrophilActiveBlood / volumeBlood + (knpbcell * DownReg(neutrophilRestingBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood) + kntbcell * DownReg(neutrophilRestingBlood * Hill(tnfBlood, xtb, htb), dcinb, dhinb, dilnbbar, ilBlood) + knrbcell * DownReg(neutrophilRestingBlood * nitricBlood, dcinb, dhinb, dilnbbar, ilBlood)) - (1 - rb) * knapbcell * DownReg(neutrophilActiveBlood * Hill(pathogenBlood, xpb, hpb), dcinb, dhinb, dilnbbar, ilBlood);
    dIlTis = -(uiMax - (uiMax - uiMin) * Hill(ilTis, vi, hi)) * ilTis + difMol * (ilBlood - ilTis) / volumeTis + kmai * DownReg(Hill(macrophageActiveTis, xmi, hmi), dci, dhi, dilbar, ilTis);
    dIlBlood = -(uibMax - (uibMax - uibMin) * Hill(ilBlood, vib, hib)) * ilBlood + difMol * (ilTis - ilBlood) / volumeBlood;
    dNitricTis = -ur * nitricTis + difMol * (nitricBlood - nitricTis) / volumeTis + krtr * nitricBlood * tissueIntegrity + knar * DownReg(neutrophilActiveTis, dcin, dhin, dilnbar, ilTis) + knatr * neutrophilActiveTis * tissueIntegrity + krntp * neutrophilActiveTis * pathogenTis * tissueIntegrity + krtmp * macrophageActiveTis * pathogenTis * tissueIntegrity;
    dNitricBlood = -urb * nitricBlood + difMol * (nitricTis - nitricBlood) / volumeBlood + knarb * DownReg(neutrophilActiveBlood, dcinb, dhinb, dilnbbar, ilBlood);
    dTissueIntegrity = ktg * tissueIntegrity * (1.0 - tissueIntegrity / TInf) * (tissueIntegrity - a) - krtt * nitricTis * tissueIntegrity;
    dInflammation = ktz * (Hill(tnfTis, xtz, htz) + ktzi * (TInf - tissueIntegrity)) * (1 - inflammation) - uz * inflammation;
    dTemp = (TMax - temp) * (Hill(tnfTis, nTTnf, hTTnf) + ktzi * (TInf - tissueIntegrity)) - uz * (temp - TMin);
    dHeartRate = (-heartRate + kH * (HMax - HBase) * Hill(temp - TMin, nHT, hHT) + HBase) / tau2;
    dPain = -kPTP * pathogenTis * pain + kPT * (PTM - pain);

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

    //Track State
    stateTrk.Track("Pathogen_Tissue", t_s, pathogenTis);
    stateTrk.Track("Pathogen_Blood", t_s, pathogenBlood);
    stateTrk.Track("MacrophageTissue_Resting", t_s, macrophageRestingTis);
    stateTrk.Track("MacrophageTissue_Active", t_s, macrophageActiveTis);
    stateTrk.Track("TNF_Tissue", t_s, tnfTis);
    stateTrk.Track("TNF_Blood", t_s, tnfBlood);
    stateTrk.Track("NeutrophilTissue_Active", t_s, neutrophilActiveTis);
    stateTrk.Track("NeutrophilBlood_Resting", t_s, neutrophilRestingBlood);
    stateTrk.Track("NeutrophilBlood_Active", t_s, neutrophilActiveBlood);
    stateTrk.Track("IL10_Tissue", t_s, ilTis);
    stateTrk.Track("IL10_Blood", t_s, ilBlood);
    stateTrk.Track("NO_Tissue", t_s, nitricTis);
    stateTrk.Track("NO_Blood", t_s, nitricBlood);
    stateTrk.Track("TissueIntegrity", t_s, tissueIntegrity);
    stateTrk.Track("Inflammation", t_s, inflammation);
    stateTrk.Track("CoreTemp", t_s, temp);
    stateTrk.Track("HeartRate", t_s, heartRate);
    stateTrk.Track("PainThreshold", t_s, pain);

    if (fmod(t_min, 10.0) <ZERO_APPROX) {
      std::cout << "Sim Time = " << t_min << std::endl;
      }

    t_s += dt_s;
    t_min = t_s/60.0;
  }

  stateTrk.WriteTrackToFile(stateFile.c_str());
}

double BioGearsEngineTest::Hill(double x, double& n, double& h)
{
  return (std::pow(x, h)) / (std::pow(x, h) + std::pow(n, h));
}

double BioGearsEngineTest::DownReg(double x, double& cin, double& hin, double& ilbar, double& il)
{
  return x * ((1.0 - cin) / (1.0 + std::pow(il / ilbar, hin)) + cin);
}
}