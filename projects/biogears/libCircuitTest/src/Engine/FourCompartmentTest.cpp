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
double TotalHbMols(SELiquidCompartmentGraph& Graph, SESubstance& Hb, SESubstance& HbO2, SESubstance& HbO2CO2, SESubstance& HbCO2)
{
  double Hb_g_Per_mol = Hb.GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double HbO2_g_Per_mol = HbO2.GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double HbCO2_g_Per_mol = HbCO2.GetMolarMass(MassPerAmountUnit::g_Per_mol);
  double HbO2CO2_g_Per_mol = HbO2CO2.GetMolarMass(MassPerAmountUnit::g_Per_mol);

  double totalHb_g = 0.0;
  double totalHbO2_g = 0.0;
  double totalHbCO2_g = 0.0;
  double totalHBO2CO2_g = 0.0;

  for (SELiquidCompartment* cmpt : Graph.GetCompartments()) {
    totalHb_g += cmpt->GetSubstanceQuantity(Hb)->GetMass(MassUnit::g);
    totalHbO2_g += cmpt->GetSubstanceQuantity(HbO2)->GetMass(MassUnit::g);
    totalHbCO2_g += cmpt->GetSubstanceQuantity(HbCO2)->GetMass(MassUnit::g);
    totalHBO2CO2_g += cmpt->GetSubstanceQuantity(HbO2CO2)->GetMass(MassUnit::g);
  }

  return totalHb_g / Hb_g_Per_mol + totalHbO2_g / HbO2_g_Per_mol + totalHbCO2_g / HbCO2_g_Per_mol + totalHBO2CO2_g / HbO2CO2_g_Per_mol;
}

void BioGearsEngineTest::FourCompartmentTest(bool usingAcidBase, bool usingProductionConsumption, bool usingDiffusion, bool activeDiffusion, const std::string& rptDirectory)
{
  DataTrack trk;
  std::string outputName;
  if (!usingAcidBase && !usingProductionConsumption && !usingDiffusion && !activeDiffusion) {
    outputName = "/FourCompartmentTestSimple";
  } else if (usingAcidBase && !usingProductionConsumption && !usingDiffusion && !activeDiffusion) {
    outputName = "/AcidBaseFourCompartmentTest";
  } else if (!usingAcidBase && !usingProductionConsumption && usingDiffusion && !activeDiffusion) {
    outputName = "/FiveCompartmentTestWithDiffusion";
  } else if (usingAcidBase && usingProductionConsumption && !usingDiffusion && !activeDiffusion) {
    outputName = "/AcidBaseFourCompartmentTestWithProductionConsumption";
  } else if (usingAcidBase && !usingProductionConsumption && usingDiffusion && !activeDiffusion) {
    outputName = "/AcidBaseFiveCompartmentTestWithDiffusion";
  } else if (usingAcidBase && usingProductionConsumption && usingDiffusion && !activeDiffusion) {
    outputName = "/AcidBaseFiveCompartmentTestWithProductionConsumptionAndDiffusion";
  } else if (!usingAcidBase && !usingProductionConsumption && usingDiffusion && activeDiffusion) {
    outputName = "/FiveCompartmentTestWithActiveDiffusion";
  }
  m_Logger->ResetLogFile(rptDirectory + outputName + ".log");
  std::ofstream file;
  SELiquidTransporter txpt(VolumePerTimeUnit::mL_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL, m_Logger);
  SEFluidCircuitCalculator calc(FlowComplianceUnit::mL_Per_mmHg, VolumePerTimeUnit::mL_Per_s, FlowInertanceUnit::mmHg_s2_Per_mL, PressureUnit::mmHg, VolumeUnit::mL, FlowResistanceUnit::mmHg_s_Per_mL, m_Logger);
  BioGears bg(m_Logger);
  Tissue& tsu = (Tissue&)bg.GetTissue();
  bg.GetPatient().Load("./patients/StandardMale.xml");
  bg.SetupPatient();
  bg.m_Config->EnableRenal(CDM::enumOnOff::Off);
  bg.m_Config->EnableTissue(CDM::enumOnOff::Off);
  bg.CreateCircuitsAndCompartments();
  bg.GetSubstances().InitializeGasCompartments();

  //Create a circuit
  double veinsPressure_mmHg = 4.0; //Central Venous Pressure
  double pulmonaryPressure_mmHg = 10.0; //Average Pressure
  double arteriesPressure_mmHg = 90.0; //Mean Arterial Pressure
  double capillariesPressure_mmHg = 30.0; //http://www.cvphysiology.com/Blood%20Pressure/BP019.htm
  double flow_mL_Per_min = 5600.0;

  //http://droualb.faculty.mjc.edu/Course%20Materials/Physiology%20101/Chapter%20Notes/Fall%202011/chapter_14%20Fall%202011.htm
  double totalVolume_L = 5.0;
  double arteriesVolume = 0.23 * totalVolume_L;
  double veinsVolume_L = 0.60 * totalVolume_L;
  double pulmonaryVolume = 0.12 * totalVolume_L;
  double capillariesVolume = 0.05 * totalVolume_L;

  SECircuitManager circuits(m_Logger);
  SEFluidCircuit* Circuit = &circuits.CreateFluidCircuit("Circuit");

  SEFluidCircuitNode& nPulmonary = Circuit->CreateNode("Pulmonary");
  nPulmonary.GetVolumeBaseline().SetValue(pulmonaryVolume, VolumeUnit::L);
  SEFluidCircuitNode& nVeins = Circuit->CreateNode("Veins");
  nVeins.GetVolumeBaseline().SetValue(veinsVolume_L, VolumeUnit::L);
  SEFluidCircuitNode& nCapillaries = Circuit->CreateNode("Capillaries");
  nCapillaries.GetVolumeBaseline().SetValue(capillariesVolume, VolumeUnit::L);
  SEFluidCircuitNode& nArteries = Circuit->CreateNode("Arteries");
  nArteries.GetVolumeBaseline().SetValue(arteriesVolume, VolumeUnit::L);
  SEFluidCircuitNode& nGround = Circuit->CreateNode("Ground");
  Circuit->AddReferenceNode(nGround);
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitPath& pPulmonaryToArteries = Circuit->CreatePath(nPulmonary, nArteries, "PulmonaryToArteries");
  pPulmonaryToArteries.GetFlowSourceBaseline().SetValue(flow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pArteriesToCapillaries = Circuit->CreatePath(nArteries, nCapillaries, "ArteriesToCapillaries");
  pArteriesToCapillaries.GetFlowSourceBaseline().SetValue(flow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pCapillariesToVeins = Circuit->CreatePath(nCapillaries, nVeins, "CapillariesToVeins");
  pCapillariesToVeins.GetFlowSourceBaseline().SetValue(flow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& pVeinsToPulmonary = Circuit->CreatePath(nVeins, nPulmonary, "VeinsToPulmonary");
  pVeinsToPulmonary.GetFlowSourceBaseline().SetValue(flow_mL_Per_min, VolumePerTimeUnit::mL_Per_min);

  SEFluidCircuitPath& pGroundToArteries = Circuit->CreatePath(nGround, nArteries, "GroundToArteries");
  pGroundToArteries.GetPressureSourceBaseline().SetValue(arteriesPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pGroundToPulmonary = Circuit->CreatePath(nGround, nPulmonary, "GroundToPulmonary");
  pGroundToPulmonary.GetPressureSourceBaseline().SetValue(pulmonaryPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pGroundToCapillaries = Circuit->CreatePath(nGround, nCapillaries, "GroundToCapillaries");
  pGroundToCapillaries.GetPressureSourceBaseline().SetValue(capillariesPressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& pGroundToVeins = Circuit->CreatePath(nGround, nVeins, "GroundToVeins");
  pGroundToVeins.GetPressureSourceBaseline().SetValue(veinsPressure_mmHg, PressureUnit::mmHg);

  Circuit->SetNextAndCurrentFromBaselines();
  Circuit->StateChange();

  SELiquidCompartment& cPulmonary = bg.GetCompartments().CreateLiquidCompartment("Pulmonary");
  cPulmonary.MapNode(nPulmonary);
  SELiquidCompartment& cVeins = bg.GetCompartments().CreateLiquidCompartment("Veins");
  cVeins.MapNode(nVeins);
  SELiquidCompartment& cArteries = bg.GetCompartments().CreateLiquidCompartment("Arteries");
  cArteries.MapNode(nArteries);
  SELiquidCompartment& cCapillaries = bg.GetCompartments().CreateLiquidCompartment("Capillaries");
  cCapillaries.MapNode(nCapillaries);

  SELiquidCompartment& cTissue = bg.GetCompartments().CreateLiquidCompartment("Tissue"); //for simple diffusion

  SELiquidCompartment& tisExtra = bg.GetCompartments().CreateLiquidCompartment("TissueExtracellular"); //for active diffusion with nernst potentials
  SELiquidCompartment& tisIntra = bg.GetCompartments().CreateLiquidCompartment("TissueIntracellular"); //for active diffusion with nernst potentials

  //Create the links
  SELiquidCompartmentLink& lPulmonaryToArteries = bg.GetCompartments().CreateLiquidLink(cPulmonary, cArteries, "PulmonaryToArteries");
  lPulmonaryToArteries.MapPath(pPulmonaryToArteries);
  SELiquidCompartmentLink& lArteriesToCapillaries = bg.GetCompartments().CreateLiquidLink(cArteries, cCapillaries, "ArteriesToCapillaries");
  lArteriesToCapillaries.MapPath(pArteriesToCapillaries);
  SELiquidCompartmentLink& lCapillariesToVeins = bg.GetCompartments().CreateLiquidLink(cCapillaries, cVeins, "CapillariesToVeins");
  lCapillariesToVeins.MapPath(pCapillariesToVeins);
  SELiquidCompartmentLink& lVeinsToPulmonary = bg.GetCompartments().CreateLiquidLink(cVeins, cPulmonary, "VeinsToPulmonary");
  lVeinsToPulmonary.MapPath(pVeinsToPulmonary);

  //Create the graph
  SELiquidCompartmentGraph& Graph = bg.GetCompartments().CreateLiquidGraph("Graph");
  Graph.AddCompartment(cPulmonary);
  Graph.AddCompartment(cArteries);
  Graph.AddCompartment(cVeins);
  Graph.AddCompartment(cCapillaries);
  Graph.AddLink(lPulmonaryToArteries);
  Graph.AddLink(lArteriesToCapillaries);
  Graph.AddLink(lCapillariesToVeins);
  Graph.AddLink(lVeinsToPulmonary);
  Graph.StateChange();

  //Initialize blood gas substances
  SESubstance& O2 = bg.GetSubstances().GetO2();
  SESubstance& Hb = bg.GetSubstances().GetHb();
  SESubstance& HbO2 = bg.GetSubstances().GetHbO2();
  SESubstance& HbO2CO2 = bg.GetSubstances().GetHbO2CO2();
  SESubstance& CO2 = bg.GetSubstances().GetCO2();
  SESubstance& HCO3 = bg.GetSubstances().GetHCO3();
  SESubstance& HbCO2 = bg.GetSubstances().GetHbCO2();
  SESubstance& N2 = bg.GetSubstances().GetN2();

  //Intialize ions for active transport test
  SESubstance& Sodium = bg.GetSubstances().GetSodium();
  SESubstance& Potassium = bg.GetSubstances().GetPotassium();
  SESubstance& Chloride = bg.GetSubstances().GetChloride();
  SESubstance& Calcium = bg.GetSubstances().GetCalcium();
  std::vector<SESubstance*> ions = { &Sodium, &Potassium, &Chloride, &Calcium };

  bg.GetSubstances().AddActiveSubstance(O2);
  bg.GetSubstances().AddActiveSubstance(Hb);
  bg.GetSubstances().AddActiveSubstance(HbO2);
  bg.GetSubstances().AddActiveSubstance(HbO2CO2);
  bg.GetSubstances().AddActiveSubstance(CO2);
  bg.GetSubstances().AddActiveSubstance(HCO3);
  bg.GetSubstances().AddActiveSubstance(HbCO2);
  bg.GetSubstances().AddActiveSubstance(N2);
  bg.GetSubstances().AddActiveSubstance(Sodium);
  bg.GetSubstances().AddActiveSubstance(Potassium);
  bg.GetSubstances().AddActiveSubstance(Chloride);
  bg.GetSubstances().AddActiveSubstance(Calcium);

  //Other substances
  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(0.45);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(40.5, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  double Hb_total_g_Per_dL = hematocrit.GetValue() * 34.0;
  double Hb_total_mM = Hb_total_g_Per_dL / Hb.GetMolarMass(MassPerAmountUnit::g_Per_mmol) * 10.0;

  bg.GetSaturationCalculator().SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

  // Highly Oxygenated (Aorta/PulmCaps) (90.5/40 O2/CO2 PP)
  double High_CO2_sat = 0.0282123;
  double High_O2_sat = 0.974759;
  double High_CO2_mmol_Per_L = 1.30049;
  double High_HCO3_mmol_Per_L = 25.9377;
  double High_O2_mmol_Per_L = 0.129065;
  double High_pH = 7.39982;

  // Low Oxygenated (Everywhere else) (36.5/45.5 O2/CO2 PP)
  double Low_CO2_sat = 0.162382;
  double Low_O2_sat = 0.789701;
  double Low_CO2_mmol_Per_L = 1.40557;
  double Low_HCO3_mmol_Per_L = 26.1182;
  double Low_O2_mmol_Per_L = 0.0560892;
  double Low_pH = 7.36909;

  bg.GetSubstances().InitializeBloodGases(cPulmonary, Hb_total_mM, High_O2_sat, High_O2_mmol_Per_L, High_CO2_sat, High_CO2_mmol_Per_L, High_HCO3_mmol_Per_L, High_pH, false);
  bg.GetSubstances().InitializeBloodGases(cArteries, Hb_total_mM, High_O2_sat, High_O2_mmol_Per_L, High_CO2_sat, High_CO2_mmol_Per_L, High_HCO3_mmol_Per_L, High_pH, false);
  bg.GetSubstances().InitializeBloodGases(cCapillaries, Hb_total_mM, Low_O2_sat, Low_O2_mmol_Per_L, Low_CO2_sat, Low_CO2_mmol_Per_L, Low_HCO3_mmol_Per_L, Low_pH, false);
  bg.GetSubstances().InitializeBloodGases(cVeins, Hb_total_mM, Low_O2_sat, Low_O2_mmol_Per_L, Low_CO2_sat, Low_CO2_mmol_Per_L, Low_HCO3_mmol_Per_L, Low_pH, false);

  if (usingAcidBase) {
    bg.GetSaturationCalculator().CalculateBloodGasDistribution(cPulmonary);
    bg.GetSaturationCalculator().CalculateBloodGasDistribution(cArteries);
    bg.GetSaturationCalculator().CalculateBloodGasDistribution(cCapillaries);
    bg.GetSaturationCalculator().CalculateBloodGasDistribution(cVeins);
  }

  //Tissue diffusion values for simple diffusion test
  double tissueVolume_L = 61.7; //sum of all tissue volumes
  double tissueO2Mass_g = tissueVolume_L * cCapillaries.GetSubstanceQuantity(O2)->GetConcentration(MassPerVolumeUnit::g_Per_L);
  double tissueCO2Mass_g = tissueVolume_L * cCapillaries.GetSubstanceQuantity(CO2)->GetConcentration(MassPerVolumeUnit::g_Per_L);

  //Tissue volume and Ion concentration setpoints for active diffusion test
  tisIntra.GetVolume().SetValue(tissueVolume_L * 0.7, VolumeUnit::L); //Use ~2.5:1 ratio of intra to extracellular tissue volume
  tisExtra.GetVolume().SetValue(tissueVolume_L * 0.3, VolumeUnit::L);
  Sodium.GetBloodConcentration().SetValue(145.0 * Sodium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Sodium.GetTissueConcentration().SetValue(15.0 * Sodium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Potassium.GetBloodConcentration().SetValue(4.4 * Potassium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Potassium.GetTissueConcentration().SetValue(120 * Potassium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Calcium.GetBloodConcentration().SetValue(1.2 * Calcium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Calcium.GetTissueConcentration().SetValue(0.0001 * Calcium.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Chloride.GetBloodConcentration().SetValue(110 * Chloride.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  Chloride.GetTissueConcentration().SetValue(20.0 * Chloride.GetMolarMass(MassPerAmountUnit::g_Per_mmol), MassPerVolumeUnit::g_Per_L);
  if (usingDiffusion) {
    if (!activeDiffusion) {
      cTissue.GetVolume().SetValue(tissueVolume_L, VolumeUnit::L);
      cTissue.GetSubstanceQuantity(O2)->GetMass().SetValue(tissueO2Mass_g, MassUnit::g);
      cTissue.GetSubstanceQuantity(Hb)->GetMass().SetValue(0, MassUnit::g);
      cTissue.GetSubstanceQuantity(HbO2)->GetMass().SetValue(0, MassUnit::g);
      cTissue.GetSubstanceQuantity(HbO2CO2)->GetMass().SetValue(0, MassUnit::g);
      cTissue.GetSubstanceQuantity(CO2)->GetMass().SetValue(tissueCO2Mass_g, MassUnit::g);
      cTissue.GetSubstanceQuantity(HCO3)->GetMass().SetValue(0, MassUnit::g);
      cTissue.GetSubstanceQuantity(HbCO2)->GetMass().SetValue(0, MassUnit::g);
      cTissue.Balance(BalanceLiquidBy::Mass);
    } else {
      for (auto ion : ions) {
        //Vascular and extracellular compartments have approximately equal concentrations of respective ions
        cPulmonary.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetBloodConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
        cArteries.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetBloodConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
        cCapillaries.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetBloodConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
        cVeins.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetBloodConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
        tisExtra.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetBloodConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
        tisIntra.GetSubstanceQuantity(*ion)->GetConcentration().SetValue(ion->GetTissueConcentration(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
      }
      cPulmonary.Balance(BalanceLiquidBy::Concentration);
      cArteries.Balance(BalanceLiquidBy::Concentration);
      cCapillaries.Balance(BalanceLiquidBy::Concentration);
      cVeins.Balance(BalanceLiquidBy::Concentration);
      tisExtra.Balance(BalanceLiquidBy::Concentration);
      tisIntra.Balance(BalanceLiquidBy::Concentration);
    }
  }

  //Production, Consumption / Alveolar Transfer values
  double O2Consumption_ugPerS = 5954.2;
  double CO2Production_ugPerS = 6140.4;

  //Execution parameters
  double time = 0;
  double deltaT_s = 1.0 / 90.0;
  double runTime_min = 3.0;
  std::stringstream ss;

  for (unsigned int i = 0; i < runTime_min * 60.0 / deltaT_s; i++) {
    //PreProcess - Mimic Produce, Consume, and Alveolar Transfer
    if (usingProductionConsumption) {
      if (usingDiffusion) {
        if (!activeDiffusion) {
          //Do it in the tissues and let it diffuse
          cPulmonary.GetSubstanceQuantity(O2)->GetMass().IncrementValue(O2Consumption_ugPerS * deltaT_s, MassUnit::ug);
          cPulmonary.GetSubstanceQuantity(CO2)->GetMass().IncrementValue(-CO2Production_ugPerS * deltaT_s, MassUnit::ug);
          cTissue.GetSubstanceQuantity(O2)->GetMass().IncrementValue(-O2Consumption_ugPerS * deltaT_s, MassUnit::ug);
          cTissue.GetSubstanceQuantity(CO2)->GetMass().IncrementValue(CO2Production_ugPerS * deltaT_s, MassUnit::ug);

          //See if we took too much out
          double pulmonaryCO2Mass_ug = cPulmonary.GetSubstanceQuantity(CO2)->GetMass(MassUnit::ug);
          double tissueO2Mass_ug = cCapillaries.GetSubstanceQuantity(O2)->GetMass(MassUnit::ug);
          if (pulmonaryCO2Mass_ug < 0.0) {
            ss << "Not Enough Pulmonary CO2 at time " << i * deltaT_s;
            cPulmonary.GetSubstanceQuantity(CO2)->GetMass().SetValue(0.0, MassUnit::ug);
            Info(ss.str());
            ss.str("");
            ss.clear();
          }
          if (tissueO2Mass_ug < 0.0) {
            ss << "Not Enough Tissue O2 at time " << i * deltaT_s;
            cTissue.GetSubstanceQuantity(O2)->GetMass().SetValue(0.0, MassUnit::ug);
            Info(ss.str());
            ss.str("");
            ss.clear();
          }

          cPulmonary.Balance(BalanceLiquidBy::Mass);
          cTissue.Balance(BalanceLiquidBy::Mass);
        } else {
          //Stub in the event anyone wants to do a test with production consumption and active ion diffusion
        }

      } else {
        //No tissues, so put straight into the blood
        cPulmonary.GetSubstanceQuantity(O2)->GetMass().IncrementValue(O2Consumption_ugPerS * deltaT_s, MassUnit::ug);
        cPulmonary.GetSubstanceQuantity(CO2)->GetMass().IncrementValue(-CO2Production_ugPerS * deltaT_s, MassUnit::ug);
        cCapillaries.GetSubstanceQuantity(O2)->GetMass().IncrementValue(-O2Consumption_ugPerS * deltaT_s, MassUnit::ug);
        cCapillaries.GetSubstanceQuantity(CO2)->GetMass().IncrementValue(CO2Production_ugPerS * deltaT_s, MassUnit::ug);

        //See if we took too much out
        double pulmonaryCO2Mass_ug = cPulmonary.GetSubstanceQuantity(CO2)->GetMass(MassUnit::ug);
        double capillaryO2Mass_ug = cCapillaries.GetSubstanceQuantity(O2)->GetMass(MassUnit::ug);
        if (pulmonaryCO2Mass_ug < 0.0) {
          ss << "Not Enough Pulmonary CO2 at time " << i * deltaT_s;
          cPulmonary.GetSubstanceQuantity(CO2)->GetMass().SetValue(0.0, MassUnit::ug);
          Info(ss.str());
          ss.str("");
          ss.clear();
        }
        if (capillaryO2Mass_ug < 0.0) {
          ss << "Not Enough Capillary O2 at time " << i * deltaT_s;
          cCapillaries.GetSubstanceQuantity(O2)->GetMass().SetValue(0.0, MassUnit::ug);
          Info(ss.str());
          ss.str("");
          ss.clear();
        }

        cPulmonary.Balance(BalanceLiquidBy::Mass);
        cCapillaries.Balance(BalanceLiquidBy::Mass);
      }
    }

    //Process - Execute the circuit
    calc.Process(*Circuit, deltaT_s);
    //Execute the substance transport function
    txpt.Transport(Graph, deltaT_s);

    if (usingAcidBase) {
      for (SELiquidCompartment* cmpt : Graph.GetCompartments()) {
        bg.GetSaturationCalculator().CalculateBloodGasDistribution(*cmpt);
      }
    }

    //convert 'Next' values to current
    calc.PostProcess(*Circuit);

    trk.Track(time, *Circuit);
    trk.Track(time, Graph);

    //Total masses
    for (SESubstance* sub : bg.GetCompartments().GetLiquidCompartmentSubstances()) {
      double totalMass = 0.0;
      std::string name;
      for (SELiquidCompartment* cmpt : Graph.GetCompartments()) {
        totalMass += cmpt->GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
        name = cmpt->GetName();
      }
      if (usingDiffusion && activeDiffusion) {
        totalMass += tisExtra.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
        totalMass += tisIntra.GetSubstanceQuantity(*sub)->GetMass(MassUnit::g);
      }
      if (sub->GetName() == "Hemoglobin")
        trk.Track(std::string{ "Unbound" } +sub->GetName() + "_TotalMass_" + "g", time, totalMass);
      else
        trk.Track(std::string{ sub->GetName() } +"_TotalMass_" + "g", time, totalMass);
    }

    //Total Hb
    double Hb_Mols = TotalHbMols(Graph, Hb, HbO2, HbO2CO2, HbCO2);
    trk.Track("HbTotal_mols", time, Hb_Mols);

    //Total molarity of blood gases
    for (SELiquidCompartment* cmpt : Graph.GetCompartments()) {
      SELiquidSubstanceQuantity* sqO2 = cmpt->GetSubstanceQuantity(O2);
      SELiquidSubstanceQuantity* sqHbO2 = cmpt->GetSubstanceQuantity(HbO2);
      SELiquidSubstanceQuantity* sqHbO2CO2 = cmpt->GetSubstanceQuantity(HbO2CO2);

      SELiquidSubstanceQuantity* sqCO2 = cmpt->GetSubstanceQuantity(CO2);
      SELiquidSubstanceQuantity* sqHbCO2 = cmpt->GetSubstanceQuantity(HbCO2);
      SELiquidSubstanceQuantity* sqHCO3 = cmpt->GetSubstanceQuantity(HCO3);
      trk.Track(std::string{ cmpt->GetName() } +"_TotalOxygenMolarConcentration_mmol_per_L", time, sqO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + sqHbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + sqHbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
      trk.Track(std::string{ cmpt->GetName() } +"_TotalCarbonDioxideMolarConcentration_mmol_per_L", time, sqCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + sqHCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + sqHbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + sqHbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
    }

    if (usingDiffusion) {
      if (!activeDiffusion) //Simple diffusion test tracks oxygen species
      {
        cTissue.Balance(BalanceLiquidBy::Mass);
        cCapillaries.Balance(BalanceLiquidBy::Mass);
        SELiquidSubstanceQuantity* sqO2Tissue = cTissue.GetSubstanceQuantity(O2);
        trk.Track("Tissue_OxygenMass_ug", time, sqO2Tissue->GetMass(MassUnit::ug));
        trk.Track("Tissue_OxygenConcentration_ug_per_mL", time, sqO2Tissue->GetMass(MassUnit::ug) / cTissue.GetVolume(VolumeUnit::mL));

        double totalMassO2 = 0;

        for (SELiquidCompartment* cmpt : Graph.GetCompartments()) {
          totalMassO2 += cmpt->GetSubstanceQuantity(O2)->GetMass(MassUnit::ug);
        }

        totalMassO2 += sqO2Tissue->GetMass(MassUnit::ug);
        trk.Track("xTotalOxygenMass_ug", time, totalMassO2);
      } else //Active diffusion test tracks ions
      {
        double intraConcentration_mM;
        double extraConcentration_mM;
        for (auto ion : ions) {
          intraConcentration_mM = tisIntra.GetSubstanceQuantity(*ion)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
          extraConcentration_mM = tisExtra.GetSubstanceQuantity(*ion)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
          trk.Track(std::string{ion->GetName()} + "_IntracellularConcentration_mmol_Per_L", time, intraConcentration_mM);
          trk.Track(std::string{ion->GetName()} + "_ExtracellularConcentration_mmol_Per_L", time, extraConcentration_mM);
        }
      }
    }

    if (i == 0) {
      trk.CreateFile(std::string(rptDirectory + outputName + ".csv").c_str(), file);
    }
    trk.StreamTrackToFile(file);

    time += deltaT_s;
  }
  file.close();
}

void BioGearsEngineTest::FourCompartmentTestSimple(const std::string& sOutputDirectory)
{
  FourCompartmentTest(false, false, false, false, sOutputDirectory);
}
void BioGearsEngineTest::AcidBaseFourCompartmentTest(const std::string& sOutputDirectory)
{
  FourCompartmentTest(true, false, false, false, sOutputDirectory);
}
void BioGearsEngineTest::AcidBaseFourCompartmentTestWithProductionConsumption(const std::string& sOutputDirectory)
{
  FourCompartmentTest(true, true, false, false, sOutputDirectory);
}
}