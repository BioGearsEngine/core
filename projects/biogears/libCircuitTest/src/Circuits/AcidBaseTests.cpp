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
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/utils/testing/SETestReport.h>

#define VERBOSE
namespace biogears {
void BioGearsEngineTest::AcidBaseMathTest(const std::string& rptDirectory)
{
  // This is a unit test of the blood gas calculation methodology.
  // It isolates the blood gas calculations from any feedback or regulation mechanisms in BioGears.
  // This whole thing will replace in SystemInteraction.cpp:
  // m_data.GetSaturationCalculator().CalculateOxygenSaturation(*node);
  // m_data.GetSaturationCalculator().CalculateCarbonDioxideSaturation(*node);
  // with something like m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*node);
  // so named because "An arterial blood gas (ABG) test measures the acidity (pH) and the levels of oxygen and carbon dioxide in the blood from an artery."
  // Where it goes doesn't matter so much.

  DataTrack trk;

  BioGears bg(rptDirectory + "/AcidBaseMath.log");
  SaturationCalculator& c = bg.GetSaturationCalculator();
  SESubstanceManager& subMgr = bg.GetSubstances();

  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* HCO3 = subMgr.GetSubstance("Bicarbonate");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb); // This is hemoglobin with nothing bound
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HCO3);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  SELiquidCompartment& cmpt = cmptMgr.CreateLiquidCompartment("Blood");
  SELiquidSubstanceQuantity* nO2 = cmpt.GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* nHb = cmpt.GetSubstanceQuantity(*Hb); // This is hemoglobin with nothing bound
  SELiquidSubstanceQuantity* nHbO2 = cmpt.GetSubstanceQuantity(*HbO2);
  SELiquidSubstanceQuantity* nHbO2CO2 = cmpt.GetSubstanceQuantity(*HbO2CO2);
  SELiquidSubstanceQuantity* nCO2 = cmpt.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* nHCO3 = cmpt.GetSubstanceQuantity(*HCO3);
  SELiquidSubstanceQuantity* nHbCO2 = cmpt.GetSubstanceQuantity(*HbCO2);

  double cmptVolume_mL = 100.0;
  double normalTotalO2_mM = 6.2;
  double normalTotalCO2_mM = 28.86;
  double currentTotalO2_mM;
  double currentTotalCO2_mM;
  double normalDissolvedCO2_gPerL = 0.05526; // 0.05526;
  double normalDissolvedO2_gPerL = 0.004287; // 0.004287;
  double normalBicarbonate_gPerL = 0.026 * 61.0168;
  double normalHgb_mM = 1.55;
  double percentNothingBound = 0.01;
  double percentO2OnlyBound = 0.73;
  double percentO2CO2Bound = 0.25;
  double percentCO2Bound = 0.01;
  //double O2ppGuess_mmHg = 100.0;
  //double CO2ppGuess_mmHg = 1000.0*normalDissolvedCO2_gPerL/(44.01*0.0314);
  //normalDissolvedCO2_gPerL = CO2ppGuess_mmHg*(44.01*0.0314) / 1000.0;

  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(0.42);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(28.5, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  double total_O2_iterations = 15;
  double total_CO2_iterations = 15;
  double total_SID_iterations = 15;
  double SID_range_mM = 28.0;
  bool compareBeforeAfter = false;

  std::ofstream file;
  std::string rptFile = rptDirectory + "/AcidBaseMath.csv";

  unsigned int testID = 0;

  for (int i = 0; i < total_O2_iterations; i++) {
    for (int j = 0; j < total_CO2_iterations; j++) {
      strongIonDifference.SetValue(28.5, AmountPerVolumeUnit::mmol_Per_L);
      for (int k = 0; k < total_SID_iterations; k++) {
        //normalDissolvedCO2_gPerL = 0.05526*(0.3 + 0.1*j);
        //normalDissolvedO2_gPerL = 0.004287*(0.3 + 0.1*i);
        currentTotalO2_mM = normalTotalO2_mM * (0.3 + 0.1 * j);
        currentTotalCO2_mM = normalTotalCO2_mM * (0.3 + 0.1 * i);

        double dissolvedCO2_mM = 0.05 * currentTotalCO2_mM;
        double HCO3_mM = 0.9 * currentTotalCO2_mM;
        double HbReq4CO2_mM = 0.05 * currentTotalCO2_mM / 4.0;
        double dissolvedO2_mM = 0.01 * currentTotalO2_mM;
        double HbReq4O2_mM = 0.99 * currentTotalO2_mM / 4.0;
        if (std::max(HbReq4O2_mM, HbReq4CO2_mM) > normalHgb_mM) {
          double dif_mM;
          if (HbReq4O2_mM > normalHgb_mM) {
            // Bound all the hemoglobin and the rest will be dissolved. Error to tell the user that there is way too much O2 in the blood.
            dif_mM = HbReq4O2_mM - normalHgb_mM;
            HbReq4O2_mM = normalHgb_mM;
            dissolvedO2_mM += dif_mM * 4.0; // Remember 4 per site
          }
          if (HbReq4CO2_mM > normalHgb_mM) {
            // This is just madness, but we can make it happen. Bind all CO2, then make 70% of what's left bicarb and the other 10% dissolved
            dif_mM = HbReq4CO2_mM - normalHgb_mM;
            HbReq4CO2_mM = normalHgb_mM;
            dissolvedCO2_mM += 0.3 * dif_mM * 4.0;
            HCO3_mM += 0.7 * dif_mM * 4.0;
          }
          continue;
        }
        double knob = 0.5;
        double HbO2CO2_mM = knob * std::min(HbReq4O2_mM, HbReq4CO2_mM);
        double HbCO2_mM = HbReq4CO2_mM - HbO2CO2_mM;
        double HbO2_mM = HbReq4O2_mM - HbO2CO2_mM;
        double Hb_mM = normalHgb_mM - (HbO2CO2_mM + HbCO2_mM + HbO2_mM);
        if (Hb_mM < 0. || HbCO2_mM < 0. || HbO2_mM < 0. || HbO2CO2_mM < 0.) {
          unsigned int itr = 0;
          while (knob <= 1.0 && itr < 50) {
            itr++;
            knob += 0.2 * (1.0 - knob);
            if (itr == 50)
              knob = 1.0; //Last time through make sure knob is exactly 1.0
            HbO2CO2_mM = knob * std::min(HbReq4O2_mM, HbReq4CO2_mM);
            HbCO2_mM = HbReq4CO2_mM - HbO2CO2_mM;
            HbO2_mM = HbReq4O2_mM - HbO2CO2_mM;
            Hb_mM = normalHgb_mM - (HbO2CO2_mM + HbCO2_mM + HbO2_mM);
            if (Hb_mM > 0. && HbCO2_mM > 0. && HbO2_mM > 0. && HbO2CO2_mM > 0.)
              itr = 50;
          }
        }

        if (Hb_mM < 0. || HbCO2_mM < 0. || HbO2_mM < 0. || HbO2CO2_mM < 0.) {
          Fatal("Negative hemoglobin species.");
        }

        //Guess pH based on SID
        cmpt.GetPH().SetValue(0.0143 * strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L) + 6.7537);
        cmpt.GetVolume().SetValue(cmptVolume_mL, VolumeUnit::mL);
        nO2->GetMolarity().SetValue(dissolvedO2_mM, AmountPerVolumeUnit::mmol_Per_L);
        nHb->GetMolarity().SetValue(Hb_mM, AmountPerVolumeUnit::mmol_Per_L);
        nHbO2->GetMolarity().SetValue(HbO2_mM, AmountPerVolumeUnit::mmol_Per_L);
        nHbO2CO2->GetMolarity().SetValue(HbO2CO2_mM, AmountPerVolumeUnit::mmol_Per_L);
        nCO2->GetMolarity().SetValue(dissolvedCO2_mM, AmountPerVolumeUnit::mmol_Per_L);
        nHCO3->GetMolarity().SetValue(HCO3_mM, AmountPerVolumeUnit::mmol_Per_L);
        nHbCO2->GetMolarity().SetValue(HbCO2_mM, AmountPerVolumeUnit::mmol_Per_L);
        cmpt.Balance(BalanceLiquidBy::Molarity); //Sets Mass and partial pressure

        double hb_mM = nHb->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double hbO2CO2_mM = nHbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double hbO2_mM = nHbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double hbCO2_mM = nHbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double O2_mM = nO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double CO2_mM = nCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double bicarb_mM = nHCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        double totalHb_mM = hb_mM + hbO2CO2_mM + hbO2_mM + hbCO2_mM;
        double totalO2_mM = O2_mM + 4. * hbO2CO2_mM + 4. * hbO2_mM;
        double totalCO2_mM = CO2_mM + 4. * hbO2CO2_mM + 4. * hbCO2_mM + bicarb_mM;

        if (compareBeforeAfter) {
          trk.Probe("BeforeStrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
          //trk.Probe("Phosphate", Phosphate);
          trk.Probe("BeforeOxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
          trk.Probe("BeforeCO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
          trk.Probe("BeforeOxygenSaturation", nO2->GetSaturation().GetValue());
          trk.Probe("BeforeCarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
          trk.Probe("BeforeBicarbonate_mM", nHCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
          trk.Probe("BeforeOxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
          trk.Probe("BeforeCarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
          trk.Probe("BeforepH", cmpt.GetPH().GetValue());
          trk.Probe("BeforeUnBoundHemoglobin_mM", hb_mM);
          trk.Probe("BeforeOxyCarbaminohemoglobin_mM", hbO2CO2_mM);
          trk.Probe("BeforeOxyhemoglobin_mM", hbO2_mM);
          trk.Probe("BeforeCarbaminohemoglobin_mM", hbCO2_mM);
          trk.Probe("BeforetotalHb_mM", totalHb_mM);
          trk.Probe("BeforetotalO2_mM", totalO2_mM);
          trk.Probe("BeforetotalCO2_mM", totalCO2_mM);
        }

        c.SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

        // Run the calculations
        c.CalculateBloodGasDistribution(cmpt);

        ///// Print results
        hb_mM = nHb->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        hbO2CO2_mM = nHbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        hbO2_mM = nHbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        hbCO2_mM = nHbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        O2_mM = nO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        CO2_mM = nCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        bicarb_mM = nHCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
        totalHb_mM = hb_mM + hbO2CO2_mM + hbO2_mM + hbCO2_mM;
        totalO2_mM = O2_mM + 4. * hbO2CO2_mM + 4. * hbO2_mM;
        totalCO2_mM = CO2_mM + 4. * hbO2CO2_mM + 4. * hbCO2_mM + bicarb_mM;

        //trk.Probe("albumin_g_per_L", albumin_g_per_L);
        //trk.Probe("hematocrit", hematocrit);
        //trk.Probe("temperature_C", temperature_C);
        trk.Probe("StrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
        //trk.Probe("Phosphate", Phosphate);
        trk.Probe("OxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
        trk.Probe("CO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
        trk.Probe("OxygenSaturation", nO2->GetSaturation().GetValue());
        trk.Probe("CarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
        trk.Probe("Bicarbonate_mM", nHCO3->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mmol));
        trk.Probe("OxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
        trk.Probe("CarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
        trk.Probe("pH", cmpt.GetPH().GetValue());
        trk.Probe("UnBoundHemoglobin_mM", hb_mM);
        trk.Probe("OxyCarbaminohemoglobin_mM", hbO2CO2_mM);
        trk.Probe("Oxyhemoglobin_mM", hbO2_mM);
        trk.Probe("Carbaminohemoglobin_mM", hbCO2_mM);
        trk.Probe("totalHb_mM", totalHb_mM);
        trk.Probe("totalO2_mM", totalO2_mM);
        trk.Probe("totalCO2_mM", totalCO2_mM);

        // Write data to file
        if (!file.is_open())
          trk.CreateFile(rptFile.c_str(), file);
        trk.StreamProbesToFile(testID, file);
        testID++;

        if (total_SID_iterations > 1)
          strongIonDifference.IncrementValue(SID_range_mM / (total_SID_iterations - 1), AmountPerVolumeUnit::mmol_Per_L);

      } //End SID Loop
    } //End CO2 loop
  } //End O2 loop
}
void BioGearsEngineTest::AcidBaseFeedbackTest(const std::string& rptDirectory)
{
  // This is a unit test of the blood gas calculation methodology.
  // It isolates the blood gas calculations from any feedback or regulation mechanisms in BioGears.
  // This whole thing will replace in SystemInteraction.cpp:
  // m_data.GetSaturationCalculator().CalculateOxygenSaturation(*node);
  // m_data.GetSaturationCalculator().CalculateCarbonDioxideSaturation(*node);
  // with something like m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*node);
  // so named because "An arterial blood gas (ABG) test measures the acidity (pH) and the levels of oxygen and carbon dioxide in the blood from an artery."
  // Where it goes doesn't matter so much.

  // It’s making sure the solver gives the same answer when it's feed back the previous answer.

  DataTrack trk;
  BioGears bg(rptDirectory + "/AcidBaseFeedback.log");
  SaturationCalculator& c = bg.GetSaturationCalculator();
  SESubstanceManager& subMgr = bg.GetSubstances();

  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* HCO3 = subMgr.GetSubstance("Bicarbonate");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");

  SECompartmentManager cmptMgr(subMgr);

  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb); // This is hemoglobin with nothing bound
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HCO3);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  SELiquidCompartment& cmpt = cmptMgr.CreateLiquidCompartment("Blood");
  SELiquidSubstanceQuantity* nO2 = cmpt.GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* nHb = cmpt.GetSubstanceQuantity(*Hb); // This is hemoglobin with nothing bound
  SELiquidSubstanceQuantity* nHbO2 = cmpt.GetSubstanceQuantity(*HbO2);
  SELiquidSubstanceQuantity* nHbO2CO2 = cmpt.GetSubstanceQuantity(*HbO2CO2);
  SELiquidSubstanceQuantity* nCO2 = cmpt.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* nHCO3 = cmpt.GetSubstanceQuantity(*HCO3);
  SELiquidSubstanceQuantity* nHbCO2 = cmpt.GetSubstanceQuantity(*HbCO2);

  double VolumeInThisHereNode = 1.0;
  double normalDissolvedCO2_gPerL = 0.05526;
  double normalDissolvedO2_gPerL = 0.004287;
  double normalBicarbonate_gPerL = 0.026 * 61.0168;
  double normalHgb_gPerL = 150.0;
  double percentNothingBound = 0.01;
  double percentO2OnlyBound = 0.73;
  double percentO2CO2Bound = 0.25;
  double percentCO2Bound = 0.01;

  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(0.42);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(41.0, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  std::ofstream file;
  std::string rptFile = rptDirectory + "/AcidBaseFeedback.csv";

  c.SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

  normalDissolvedCO2_gPerL = 0.05526;
  cmpt.GetPH().SetValue(0.0143 * strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L) + 6.7537);
  cmpt.GetVolume().SetValue(VolumeInThisHereNode, VolumeUnit::L);
  nO2->GetConcentration().SetValue(normalDissolvedO2_gPerL, MassPerVolumeUnit::g_Per_L);
  nHb->GetConcentration().SetValue(normalHgb_gPerL * percentNothingBound, MassPerVolumeUnit::g_Per_L);
  nHbO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2OnlyBound, MassPerVolumeUnit::g_Per_L);
  nHbO2CO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2CO2Bound, MassPerVolumeUnit::g_Per_L);
  nCO2->GetConcentration().SetValue(normalDissolvedCO2_gPerL, MassPerVolumeUnit::g_Per_L);
  nHCO3->GetConcentration().SetValue(normalBicarbonate_gPerL, MassPerVolumeUnit::g_Per_L);
  nHbCO2->GetConcentration().SetValue(normalHgb_gPerL * percentCO2Bound, MassPerVolumeUnit::g_Per_L);
  cmpt.Balance(BalanceLiquidBy::Concentration);

  for (int j = 0; j < 20; j++) {
    c.CalculateBloodGasDistribution(cmpt);

    double finalDissovledCO2_gPerL = nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
    double finalDissovledCO2_mM = finalDissovledCO2_gPerL / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0;
    double finalBicarb_gPerL = nHCO3->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
    double finalBicarbCO2_mM = finalBicarb_gPerL / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0; // Yes this should be m_CO2_g_Per_mmol because 1 mol CO2 per mol bicarb
    double finalBicarbCO2_gPerL = finalBicarbCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
    double finalBoundCO2_mM = 4.0 * (nHbCO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0 + nHbO2CO2->GetMass(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0); //4 moles CO2 per mole Hb
    double finalBoundCO2_gPerL = finalBoundCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
    double finalTotalCO2_mM = finalDissovledCO2_mM + finalBicarbCO2_mM + finalBoundCO2_mM;
    double finalTotalCO2_gPerL = finalDissovledCO2_gPerL + finalBicarbCO2_gPerL + finalBoundCO2_gPerL;

    ///// Print results
    trk.Probe("StrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
    trk.Probe("OxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
    trk.Probe("CO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
    trk.Probe("OxygenSaturation", nO2->GetSaturation().GetValue());
    trk.Probe("CarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
    trk.Probe("Bicarbonate_mM", nHCO3->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L));
    trk.Probe("OxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
    trk.Probe("CarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
    trk.Probe("pH", cmpt.GetPH().GetValue());
    trk.Probe("TotalCO2_mM", finalTotalCO2_mM);
    trk.Probe("TotalCO2_gPerL", finalTotalCO2_gPerL);

    // Write data to file
    if (!file.is_open())
      trk.CreateFile(rptFile.c_str(), file);
    trk.StreamProbesToFile(j + 1, file);
  }
}

void BioGearsEngineTest::AcidBaseLimitsTest(const std::string& rptDirectory)
{
  // This is a unit test of the blood gas calculation methodology.
  // It isolates the blood gas calculations from any feedback or regulation mechanisms in BioGears.
  // This tests the solvers ability to handle all zeros and combinations of zeros as initial conditions.
  // Did not test negatives because the engine already has checks for negative mass and concentrations.

  DataTrack trk;
  BioGears bg(rptDirectory + "/AcidBaseLimits.log");
  SaturationCalculator& c = bg.GetSaturationCalculator();
  SESubstanceManager& subMgr = bg.GetSubstances();

  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* HCO3 = subMgr.GetSubstance("Bicarbonate");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb); // This is hemoglobin with nothing bound
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HCO3);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  SELiquidCompartment& cmpt = cmptMgr.CreateLiquidCompartment("Blood");
  SELiquidSubstanceQuantity* nO2 = cmpt.GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* nHb = cmpt.GetSubstanceQuantity(*Hb); // This is hemoglobin with nothing bound
  SELiquidSubstanceQuantity* nHbO2 = cmpt.GetSubstanceQuantity(*HbO2);
  SELiquidSubstanceQuantity* nHbO2CO2 = cmpt.GetSubstanceQuantity(*HbO2CO2);
  SELiquidSubstanceQuantity* nCO2 = cmpt.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* nHCO3 = cmpt.GetSubstanceQuantity(*HCO3);
  SELiquidSubstanceQuantity* nHbCO2 = cmpt.GetSubstanceQuantity(*HbCO2);

  double VolumeInThisHereNode = 1.0;
  double normalDissolvedCO2_gPerL = 0.0;
  double normalDissolvedO2_gPerL = 0.0;
  double normalBicarbonate_gPerL = 0.0;
  double normalHgb_gPerL = 0.0;
  double percentNothingBound = 0.01;
  double percentO2OnlyBound = 0.73;
  double percentO2CO2Bound = 0.25;
  double percentCO2Bound = 0.01;

  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(0.42);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(41.0, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  std::ofstream file;
  std::string rptFile = rptDirectory + "/AcidBaseLimits.csv";

  c.SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

  cmpt.GetPH().SetValue(0.0143 * strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L) + 6.7537);
  cmpt.GetVolume().SetValue(VolumeInThisHereNode, VolumeUnit::L);
  nO2->GetConcentration().SetValue(normalDissolvedO2_gPerL, MassPerVolumeUnit::g_Per_L);
  nHb->GetConcentration().SetValue(normalHgb_gPerL * percentNothingBound, MassPerVolumeUnit::g_Per_L);
  nHbO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2OnlyBound, MassPerVolumeUnit::g_Per_L);
  nHbO2CO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2CO2Bound, MassPerVolumeUnit::g_Per_L);
  nCO2->GetConcentration().SetValue(normalDissolvedCO2_gPerL, MassPerVolumeUnit::g_Per_L);
  nHCO3->GetConcentration().SetValue(normalBicarbonate_gPerL, MassPerVolumeUnit::g_Per_L);
  nHbCO2->GetConcentration().SetValue(normalHgb_gPerL * percentCO2Bound, MassPerVolumeUnit::g_Per_L);
  cmpt.Balance(BalanceLiquidBy::Concentration);

  // First test all zeros
  c.CalculateBloodGasDistribution(cmpt);

  double finalDissovledCO2_gPerL = nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
  double finalDissovledCO2_mM = finalDissovledCO2_gPerL / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0;
  double finalBicarb_gPerL = nHCO3->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
  double finalBicarbCO2_mM = finalBicarb_gPerL / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0; // Yes this should be m_CO2_g_Per_mmol because 1 mol CO2 per mol bicarb
  double finalBicarbCO2_gPerL = finalBicarbCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
  double finalBoundCO2_mM = 4.0 * (nHbCO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0 + nHbO2CO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0); //4 moles CO2 per mole Hb
  double finalBoundCO2_gPerL = finalBoundCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
  double finalTotalCO2_mM = finalDissovledCO2_mM + finalBicarbCO2_mM + finalBoundCO2_mM;
  double finalTotalCO2_gPerL = finalDissovledCO2_gPerL + finalBicarbCO2_gPerL + finalBoundCO2_gPerL;
  double finalTotalHemoglobin_gPerL = nHb->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2CO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);

  ///// Print results
  trk.Probe("StrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
  trk.Probe("OxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
  trk.Probe("CO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
  trk.Probe("OxygenSaturation", nO2->GetSaturation().GetValue());
  trk.Probe("CarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
  trk.Probe("Bicarbonate_mM", nHCO3->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L));
  trk.Probe("OxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
  trk.Probe("CarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
  trk.Probe("pH", cmpt.GetPH().GetValue());
  trk.Probe("TotalCO2_mM", finalTotalCO2_mM);
  trk.Probe("TotalCO2_gPerL", finalTotalCO2_gPerL);
  trk.Probe("TotalHemoglobin_p_Per_L", finalTotalHemoglobin_gPerL);
  trk.Probe("finalBoundCO2_mM", finalBoundCO2_mM);

  double testID = 1.0;

  // Write data to file
  if (!file.is_open())
    trk.CreateFile(rptFile.c_str(), file);
  trk.StreamProbesToFile(testID, file);

  std::string caseName;

  // Now try combinations of zeros
  for (int i = 0; i < 4; i++) {
    testID = testID + 1.0;

    switch (i) {
    case 0:
      caseName = "noCO2test";
      normalDissolvedCO2_gPerL = 0.0;
      normalDissolvedO2_gPerL = 0.004287;
      normalBicarbonate_gPerL = 0.026 * 61.0168;
      normalHgb_gPerL = 150.0;
      break;
    case 1:
      caseName = "noO2test";
      normalDissolvedCO2_gPerL = 0.05526;
      normalDissolvedO2_gPerL = 0.0;
      normalBicarbonate_gPerL = 0.026 * 61.0168;
      normalHgb_gPerL = 150.0;
      break;
    case 2:
      caseName = "noHCO3test";
      normalDissolvedCO2_gPerL = 0.05526;
      normalDissolvedO2_gPerL = 0.004287;
      normalBicarbonate_gPerL = 0.0;
      normalHgb_gPerL = 150.0;
      break;
    case 3:
      caseName = "noHGBtest";
      normalDissolvedCO2_gPerL = 0.05526;
      normalDissolvedO2_gPerL = 0.004287;
      normalBicarbonate_gPerL = 0.026 * 61.0168;
      normalHgb_gPerL = 0.0;
      break;
    default:
      bg.GetLogger()->Fatal("AcidBaseLimitsTest:: Problem in the switch");
      break;
    }
    nO2->GetConcentration().SetValue(normalDissolvedO2_gPerL, MassPerVolumeUnit::g_Per_L);
    nHb->GetConcentration().SetValue(normalHgb_gPerL * percentNothingBound, MassPerVolumeUnit::g_Per_L);
    nHbO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2OnlyBound, MassPerVolumeUnit::g_Per_L);
    nHbO2CO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2CO2Bound, MassPerVolumeUnit::g_Per_L);
    nCO2->GetConcentration().SetValue(normalDissolvedCO2_gPerL, MassPerVolumeUnit::g_Per_L);
    nHCO3->GetConcentration().SetValue(normalBicarbonate_gPerL, MassPerVolumeUnit::g_Per_L);
    nHbCO2->GetConcentration().SetValue(normalHgb_gPerL * percentCO2Bound, MassPerVolumeUnit::g_Per_L);
    cmpt.Balance(BalanceLiquidBy::Concentration);

    // Test
    c.CalculateBloodGasDistribution(cmpt);

    // Get and print results
    double finalDissovledCO2_gPerL = nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
    double finalDissovledCO2_mM = finalDissovledCO2_gPerL / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol);
    double finalBicarb_gPerL = nHCO3->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
    double finalBicarbCO2_mM = finalBicarb_gPerL / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mmol); // Yes this should be m_CO2_g_Per_mmol because 1 mol CO2 per mol bicarb
    double finalBicarbCO2_gPerL = finalBicarbCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol);
    double finalBoundCO2_mM = 4.0 * (nHbCO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol) + nHbO2CO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol)); //4 moles CO2 per mole Hb
    double finalBoundCO2_gPerL = finalBoundCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol);
    double finalTotalCO2_mM = finalDissovledCO2_mM + finalBicarbCO2_mM + finalBoundCO2_mM;
    double finalTotalCO2_gPerL = finalDissovledCO2_gPerL + finalBicarbCO2_gPerL + finalBoundCO2_gPerL;
    double finalTotalHemoglobin_gPerL = nHb->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2CO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);

    trk.Probe("StrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
    trk.Probe("OxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
    trk.Probe("CO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
    trk.Probe("OxygenSaturation", nO2->GetSaturation().GetValue());
    trk.Probe("CarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
    trk.Probe("Bicarbonate_mM", nHCO3->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L));
    trk.Probe("OxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
    trk.Probe("CarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
    trk.Probe("pH", cmpt.GetPH().GetValue());
    trk.Probe("TotalCO2_mM", finalTotalCO2_mM);
    trk.Probe("TotalCO2_gPerL", finalTotalCO2_gPerL);
    trk.Probe("TotalHemoglobin_p_Per_L", finalTotalHemoglobin_gPerL);
    trk.Probe("finalBoundCO2_mM", finalBoundCO2_mM);

    trk.StreamProbesToFile(testID, file);
  }
}

void BioGearsEngineTest::AcidBaseExtremeTest(const std::string& rptDirectory)
{
  DataTrack trk;
  BioGears bg(rptDirectory + "/AcidBaseExtreme.log");
  SaturationCalculator& c = bg.GetSaturationCalculator();
  SESubstanceManager& subMgr = bg.GetSubstances();

  std::ofstream file;
  std::string rptFile = rptDirectory + "/AcidBaseExtreme.csv";

  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* HCO3 = subMgr.GetSubstance("Bicarbonate");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");

  SECompartmentManager cmptMgr(subMgr);
  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb); // This is hemoglobin with nothing bound
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HCO3);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  SELiquidCompartment& cmpt = cmptMgr.CreateLiquidCompartment("Blood");
  SELiquidSubstanceQuantity* nO2 = cmpt.GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* nHb = cmpt.GetSubstanceQuantity(*Hb); // This is hemoglobin with nothing bound
  SELiquidSubstanceQuantity* nHbO2 = cmpt.GetSubstanceQuantity(*HbO2);
  SELiquidSubstanceQuantity* nHbO2CO2 = cmpt.GetSubstanceQuantity(*HbO2CO2);
  SELiquidSubstanceQuantity* nCO2 = cmpt.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* nHCO3 = cmpt.GetSubstanceQuantity(*HCO3);
  SELiquidSubstanceQuantity* nHbCO2 = cmpt.GetSubstanceQuantity(*HbCO2);

  double finalDissovledCO2_gPerL = 0.0;
  double finalDissovledCO2_mM = 0.0;
  double finalBicarb_gPerL = 0.0;
  double finalBicarbCO2_mM = 0.0; // Yes this should be m_CO2_g_Per_mmol because 1 mol CO2 per mol bicarb
  double finalBicarbCO2_gPerL = 0.0;
  double finalBoundCO2_mM = 0.0; //4 moles CO2 per mole Hb
  double finalBoundCO2_gPerL = 0.0;
  double finalTotalCO2_mM = 0.0;
  double finalTotalCO2_gPerL = 0.0;
  double finalTotalHemoglobin_gPerL = 0.0;

  double VolumeInThisHereNode = 1.0;
  double normalDissolvedCO2_gPerL = 0.0;
  double normalDissolvedO2_gPerL = 0.0;
  double normalBicarbonate_gPerL = 0.0;
  double normalHgb_gPerL = 150.0;
  double percentNothingBound = 0.01;
  double percentO2OnlyBound = 0.73;
  double percentO2CO2Bound = 0.25;
  double percentCO2Bound = 0.01;

  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(0.42);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(41.0, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  double phValue = 0.0;
  double testID = 1.0;

  //extreme values:
  std::vector<double> tempValues_C = { 0.0, 100.0 };
  std::vector<double> pHValues = { 5.0, 9.0 };
  std::vector<double> saturationValues = { 0.0, 100.0 };
  std::vector<double> strongIonValues = { 20.0, 50.0 };
  std::vector<double> PhosphateValues = { 2.0, 6.0 };
  std::vector<double> albuminValues_g_Per_L = { 30.0, 60.0 };
  std::vector<double> normalDissolvedCO2Values_gPerL = { 0.0, (0.05526) * 0.5 };
  std::vector<double> normalDissolvedO2Values_gPerL = { 0.0, (0.004287) * 0.5 }; //increase normal value by a factor of 2
  std::vector<double> normalBicarbonateValues_gPerL = { 0.0, 2.0 };
  std::vector<double> normalHgbValues_gPerL = { 1.0, 200.0 };

  for (int s = 0; s < 2; s++) {

    for (int r = 0; r < 2; r++) {

      for (int q = 0; q < 2; q++) {

        for (int p = 0; p < 2; p++) {

          for (int n = 0; n < 2; n++) {

            for (int m = 0; m < 2; m++) {

              for (int k = 0; k < 2; k++) {

                for (int j = 0; j < 2; j++) {

                  for (int i = 0; i < 2; i++) {
                    //report index values before each run to determine deficiencies:
#ifdef VERBOSE
                    std::cout << " i:" << i << " j:" << j << " k:" << k << " m:" << m << " n:" << n << " p:" << p << " q:" << q << " r:" << r << " s:" << s << std::endl;
#endif
                    albuminConcentration.SetValue(albuminValues_g_Per_L[n], MassPerVolumeUnit::g_Per_L);
                    hematocrit.SetValue(0.42);
                    bodyTemp.SetValue(tempValues_C[i], TemperatureUnit::C);
                    strongIonDifference.SetValue(strongIonValues[k], AmountPerVolumeUnit::mmol_Per_L);
                    phosphate.SetValue(PhosphateValues[m], AmountPerVolumeUnit::mmol_Per_L);
                    phValue = pHValues[j];
                    normalDissolvedCO2_gPerL = normalDissolvedCO2Values_gPerL[p];
                    normalDissolvedO2_gPerL = normalDissolvedO2Values_gPerL[q];
                    normalBicarbonate_gPerL = normalBicarbonateValues_gPerL[r];
                    normalHgb_gPerL = normalHgbValues_gPerL[s];

                    c.SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

                    cmpt.GetPH().SetValue(0.0143 * strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L) + 6.7537);
                    cmpt.GetVolume().SetValue(VolumeInThisHereNode, VolumeUnit::L);
                    nO2->GetConcentration().SetValue(normalDissolvedO2_gPerL, MassPerVolumeUnit::g_Per_L);
                    nHb->GetConcentration().SetValue(normalHgb_gPerL * percentNothingBound, MassPerVolumeUnit::g_Per_L);
                    nHbO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2OnlyBound, MassPerVolumeUnit::g_Per_L);
                    nHbO2CO2->GetConcentration().SetValue(normalHgb_gPerL * percentO2CO2Bound, MassPerVolumeUnit::g_Per_L);
                    nCO2->GetConcentration().SetValue(normalDissolvedCO2_gPerL, MassPerVolumeUnit::g_Per_L);
                    nHCO3->GetConcentration().SetValue(normalBicarbonate_gPerL, MassPerVolumeUnit::g_Per_L);
                    nHbCO2->GetConcentration().SetValue(normalHgb_gPerL * percentCO2Bound, MassPerVolumeUnit::g_Per_L);
                    cmpt.Balance(BalanceLiquidBy::Concentration);
                    //
                    c.CalculateBloodGasDistribution(cmpt);

                    finalDissovledCO2_gPerL = nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
                    finalDissovledCO2_mM = finalDissovledCO2_gPerL / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0;
                    finalBicarb_gPerL = nHCO3->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);
                    finalBicarbCO2_mM = finalBicarb_gPerL / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0; // Yes this should be m_CO2_g_Per_mmol because 1 mol CO2 per mol bicarb
                    finalBicarbCO2_gPerL = finalBicarbCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
                    finalBoundCO2_mM = 4.0 * (nHbCO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0 + nHbO2CO2->GetMass().GetValue(MassUnit::g) / cmpt.GetVolume(VolumeUnit::L) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000.0); //4 moles CO2 per mole Hb
                    finalBoundCO2_gPerL = finalBoundCO2_mM * CO2->GetMolarMass(MassPerAmountUnit::g_Per_mol) / 1000.0;
                    finalTotalCO2_mM = finalDissovledCO2_mM + finalBicarbCO2_mM + finalBoundCO2_mM;
                    finalTotalCO2_gPerL = finalDissovledCO2_gPerL + finalBicarbCO2_gPerL + finalBoundCO2_gPerL;
                    finalTotalHemoglobin_gPerL = nHb->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L) + nHbO2CO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L);

                    ///// Print results
                    trk.Probe("StrongIonDifference", strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
                    trk.Probe("OxygenConcentration_g_Per_L", nO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
                    trk.Probe("CO2Concentration_g_Per_L", nCO2->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_L));
                    trk.Probe("OxygenSaturation", nO2->GetSaturation().GetValue());
                    trk.Probe("CarbonDioxideSaturation", nCO2->GetSaturation().GetValue());
                    trk.Probe("Bicarbonate_mM", nHCO3->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L));
                    trk.Probe("OxygenPartialPressure", nO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
                    trk.Probe("CarbonDioxidePartialPressure", nCO2->GetPartialPressure().GetValue(PressureUnit::mmHg));
                    trk.Probe("pH", cmpt.GetPH().GetValue());
                    trk.Probe("TotalCO2_mM", finalTotalCO2_mM);
                    trk.Probe("TotalCO2_gPerL", finalTotalCO2_gPerL);
                    trk.Probe("TotalHemoglobin_g_Per_L", finalTotalHemoglobin_gPerL);
                    trk.Probe("finalBoundCO2_mM", finalBoundCO2_mM);

                    // Write data to file
                    if (!file.is_open())
                      trk.CreateFile(rptFile.c_str(), file);
                    trk.StreamProbesToFile(testID, file);

                    std::string caseName;

                    testID += 1.0;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void BioGearsEngineTest::AcidBaseBloodGasTest(BioGears& bg, bloodType bloodCompartment, SETestSuite& testSuite)
{
  TimingProfile timer;
  timer.Start("Test");
  // This is a unit test of the blood gas calculation methodology. Inputs are set
  // within normal range specified in \cite van2013davis and outputs are expected
  // to also be within normal range.

  DataTrack& trk = bg.GetDataTrack();
  SESubstanceManager& subMgr = bg.GetSubstances();
  SaturationCalculator& c = bg.GetSaturationCalculator();
  SECompartmentManager& cmptMgr = bg.GetCompartments();
  cmptMgr.Clear();

  SETestCase& testCase = testSuite.CreateTestCase();
  switch (bloodCompartment) {
  case ARTERIAL:
    testCase.SetName("Arterial");
    break;
  case VENOUS:
    testCase.SetName("Venous");
    break;
  case CAPILLARY:
    testCase.SetName("Capillary");
    break;
  case RESPIRATORY_ACIDOSIS:
    testCase.SetName("RespiratoryAcidosis");
    break;
  case METABOLIC_ALKALOSIS:
    testCase.SetName("MetabolicAlkalosis");
    break;
  case METABOLIC_ACIDOSIS:
    testCase.SetName("MetabolicAcidosis");
    break;
  case RESPIRATORY_ALKALOSIS:
    testCase.SetName("RespiratoryAlkalosis");
    break;
  default:
    break;
  };

  //Create the compartment
  SELiquidCompartment& testCompartment = cmptMgr.CreateLiquidCompartment("TestCompartment");

  //Initialize Substances
  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* HCO3 = subMgr.GetSubstance("Bicarbonate");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");

  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb); // This is hemoglobin with nothing bound
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HCO3);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  cmptMgr.StateChange();

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
  double Hb_total_mM = Hb_total_g_Per_dL / Hb->GetMolarMass(MassPerAmountUnit::g_Per_mmol) * 10.0;

  c.SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

  SELiquidSubstanceQuantity* O2q = testCompartment.GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* CO2q = testCompartment.GetSubstanceQuantity(*CO2);
  SELiquidSubstanceQuantity* Hbq = testCompartment.GetSubstanceQuantity(*Hb);
  SELiquidSubstanceQuantity* HbO2q = testCompartment.GetSubstanceQuantity(*HbO2);
  SELiquidSubstanceQuantity* HbCO2q = testCompartment.GetSubstanceQuantity(*HbCO2);
  SELiquidSubstanceQuantity* HbO2CO2q = testCompartment.GetSubstanceQuantity(*HbO2CO2);
  SELiquidSubstanceQuantity* HCO3q = testCompartment.GetSubstanceQuantity(*HCO3);

  //For defining expected ranges
  double PCO2min_mmHg = 0; //Leeuwen
  double PCO2max_mmHg = 0; //Leeuwen
  double PO2min_mmHg = 0; //Leeuwen
  double PO2max_mmHg = 0; //Leeuwen
  double HCO3min_mmol_Per_L = 0; //Leeuwen
  double HCO3max_mmol_Per_L = 0; //Leeuwen
  double O2sat_min = 0; //Leeuwen
  double O2sat_max = 0; //Leeuwen
  double CO2sat_min = 0; //Computed using Dash model
  double CO2sat_max = 0; //Computed using Dash model
  double pH_min = 0; //Leeuwen
  double pH_max = 0; //Leeuwen
  double percentTolerance = .02;

  double O2_sat = 0;
  double CO2_sat = 0;
  double HCO3_mmol_Per_L = 0;
  double pH = 0;
  double CO2PartialPressure_mmHg = 0;
  double O2PartialPressure_mmHg = 0;
  double volume_mL = 0;
  double testID = 1.0;

  if (bloodCompartment == ARTERIAL) {
    testID = 1.0;

    O2_sat = .97;
    CO2_sat = .2224;
    CO2PartialPressure_mmHg = 40.0;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 25;
    pH = 7.4;
    volume_mL = 1150.0;

    PCO2min_mmHg = 35;
    PCO2max_mmHg = 45;
    PO2min_mmHg = 80;
    PO2max_mmHg = 95;
    HCO3min_mmol_Per_L = 22;
    HCO3max_mmol_Per_L = 26;
    O2sat_min = .95;
    O2sat_max = .99;
    CO2sat_min = .1696;
    CO2sat_max = .2752;
    pH_min = 7.35;
    pH_max = 7.45;
  } else if (bloodCompartment == VENOUS) {
    testID = 2.0;

    O2_sat = .725;
    CO2_sat = .2523;
    CO2PartialPressure_mmHg = 46.0;
    O2PartialPressure_mmHg = 34.5;
    HCO3_mmol_Per_L = 25;
    pH = 7.375;
    volume_mL = 3000.0;

    PCO2min_mmHg = 41;
    PCO2max_mmHg = 51;
    PO2min_mmHg = 20;
    PO2max_mmHg = 49;
    HCO3min_mmol_Per_L = 24;
    HCO3max_mmol_Per_L = 28;
    O2sat_min = .7;
    O2sat_max = .75;
    CO2sat_min = .2113;
    CO2sat_max = .2933;
    pH_min = 7.32;
    pH_max = 7.43;
  } else if (bloodCompartment == CAPILLARY) {
    testID = 3.0;

    O2_sat = .965;
    CO2_sat = .19435;
    CO2PartialPressure_mmHg = 33.5;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 20.5;
    pH = 7.4;
    volume_mL = 250.0;

    PCO2min_mmHg = 26;
    PCO2max_mmHg = 41;
    PO2min_mmHg = 80;
    PO2max_mmHg = 95;
    HCO3min_mmol_Per_L = 18;
    HCO3max_mmol_Per_L = 23;
    O2sat_min = .95;
    O2sat_max = .98;
    CO2sat_min = .1317;
    CO2sat_max = .257;
    pH_min = 7.35;
    pH_max = 7.45;
  } else if (bloodCompartment == RESPIRATORY_ACIDOSIS) {
    testID = 4.0;

    O2_sat = .97;
    CO2_sat = .2224;
    CO2PartialPressure_mmHg = 120.0;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 44.0;
    pH = 7.2;
    volume_mL = 1150.0;

    PCO2min_mmHg = 40;
    PCO2max_mmHg = 150;
    PO2min_mmHg = 0.0;
    PO2max_mmHg = 200.0;
    HCO3min_mmol_Per_L = 24;
    HCO3max_mmol_Per_L = 60;
    O2sat_min = 0.0;
    O2sat_max = 1.0;
    CO2sat_min = 0.0;
    CO2sat_max = 1.0;
    pH_min = 7.0;
    pH_max = 7.4;
  } else if (bloodCompartment == METABOLIC_ALKALOSIS) {
    testID = 5.0;

    O2_sat = .97;
    CO2_sat = .2224;
    CO2PartialPressure_mmHg = 40;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 44;
    pH = 7.6;
    volume_mL = 1150.0;

    PCO2min_mmHg = 20;
    PCO2max_mmHg = 90;
    PO2min_mmHg = 0.0;
    PO2max_mmHg = 200.0;
    HCO3min_mmol_Per_L = 24;
    HCO3max_mmol_Per_L = 60;
    O2sat_min = 0.0;
    O2sat_max = 1.0;
    CO2sat_min = 0.0;
    CO2sat_max = 1.0;
    pH_min = 7.4;
    pH_max = 7.8;
  } else if (bloodCompartment == METABOLIC_ACIDOSIS) {
    testID = 6.0;

    O2_sat = .97;
    CO2_sat = .2224;
    CO2PartialPressure_mmHg = 30.0;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 12.0;
    pH = 7.2;
    volume_mL = 1150.0;

    PCO2min_mmHg = 0;
    PCO2max_mmHg = 90;
    PO2min_mmHg = 0.0;
    PO2max_mmHg = 200.0;
    HCO3min_mmol_Per_L = 0;
    HCO3max_mmol_Per_L = 14;
    O2sat_min = 0.0;
    O2sat_max = 1.0;
    CO2sat_min = 0.0;
    CO2sat_max = 1.0;
    pH_min = 7.0;
    pH_max = 7.4;
  } else if (bloodCompartment == RESPIRATORY_ALKALOSIS) {
    testID = 7.0;

    O2_sat = .97;
    CO2_sat = .2224;
    CO2PartialPressure_mmHg = 10;
    O2PartialPressure_mmHg = 87.5;
    HCO3_mmol_Per_L = 12;
    pH = 7.6;
    volume_mL = 1150.0;

    PCO2min_mmHg = 0;
    PCO2max_mmHg = 40;
    PO2min_mmHg = 0.0;
    PO2max_mmHg = 200.0;
    HCO3min_mmol_Per_L = 0;
    HCO3max_mmol_Per_L = 24;
    O2sat_min = 0.0;
    O2sat_max = 1.0;
    CO2sat_min = 0.0;
    CO2sat_max = 1.0;
    pH_min = 7.4;
    pH_max = 7.8;
  } else if (bloodCompartment == CUSTOM) {
    std::vector<std::string> strValues;

    //You can test out the values that WriteBloodGases() spits out
    //O2 Sat, O2 Molarity, CO2 Sat, CO2 Molarity, Bicarb Molarity, pH
    std::stringstream ss("0.784329, 0.0614627, 0.0955322, 1.82692, 26.7283, 7.26525");

    while (ss.good()) {
      std::string substr;
      std::getline(ss, substr, ',');
      strValues.push_back(substr);
    }

#ifndef ANDROID
    O2_sat = std::stod(strValues[0]);
    CO2_sat = std::stod(strValues[2]);
    CO2PartialPressure_mmHg = (std::stod(strValues[3]) * 44.01) / (1000 * 0.0013815);
    O2PartialPressure_mmHg = (std::stod(strValues[1]) * 32) / (1000 * 4.51263E-05);
    HCO3_mmol_Per_L = std::stod(strValues[4]);
    pH = std::stod(strValues[5]);
    volume_mL = 1000.0;
#endif
  }

  //set compartment values (same as InitializeBloodGases):
  testCompartment.GetPH().SetValue(pH);
  testCompartment.GetVolume().SetValue(volume_mL, VolumeUnit::mL);

  //Distribute Hb
  double HbO2CO2_mM = 0.0;
  double HbO2_mM = 0.0;
  double HbCO2_mM = 0.0;
  if (O2_sat >= CO2_sat) {
    HbO2CO2_mM = Hb_total_mM * CO2_sat;
    HbO2_mM = Hb_total_mM * O2_sat - HbO2CO2_mM;
    HbCO2_mM = 0.0;
  } else //CO2_sat greater
  {
    HbO2CO2_mM = Hb_total_mM * O2_sat;
    HbO2_mM = 0.0;
    HbCO2_mM = Hb_total_mM * CO2_sat - HbO2CO2_mM;
  }
  double Hb_mM = Hb_total_mM - HbO2CO2_mM - HbO2_mM - HbCO2_mM;
  Hbq->GetMolarity().SetValue(Hb_mM, AmountPerVolumeUnit::mmol_Per_L);
  Hbq->Balance(BalanceLiquidBy::Molarity);
  HbO2CO2q->GetMolarity().SetValue(HbO2CO2_mM, AmountPerVolumeUnit::mmol_Per_L);
  HbO2CO2q->Balance(BalanceLiquidBy::Molarity);
  HbO2q->GetMolarity().SetValue(HbO2_mM, AmountPerVolumeUnit::mmol_Per_L);
  HbO2q->Balance(BalanceLiquidBy::Molarity);
  HbCO2q->GetMolarity().SetValue(HbCO2_mM, AmountPerVolumeUnit::mmol_Per_L);
  HbCO2q->Balance(BalanceLiquidBy::Molarity);

  //Set dissolved values
  CO2q->GetPartialPressure().SetValue(CO2PartialPressure_mmHg, PressureUnit::mmHg);
  CO2q->Balance(BalanceLiquidBy::PartialPressure);
  CO2q->GetSaturation().SetValue(CO2_sat);
  HCO3q->GetMolarity().SetValue(HCO3_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  HCO3q->Balance(BalanceLiquidBy::Molarity);
  O2q->GetPartialPressure().SetValue(O2PartialPressure_mmHg, PressureUnit::mmHg);
  O2q->Balance(BalanceLiquidBy::PartialPressure);
  O2q->GetSaturation().SetValue(O2_sat);

  double starting_totalAmountO2_mmol = (O2q->GetMass(MassUnit::g) / O2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2q->GetMass(MassUnit::g) / HbO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  double starting_totalAmountCO2_mmol = (CO2q->GetMass(MassUnit::g) / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HCO3q->GetMass(MassUnit::g) / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbCO2q->GetMass(MassUnit::g) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  double starting_totalAmountHb_mmol = (Hbq->GetMass(MassUnit::g) / Hb->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbCO2q->GetMass(MassUnit::g) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbO2q->GetMass(MassUnit::g) / HbO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  //Do the actual distribution using the calculator
  c.CalculateBloodGasDistribution(testCompartment);

  double ending_totalAmountO2_mmol = (O2q->GetMass(MassUnit::g) / O2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2q->GetMass(MassUnit::g) / HbO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  double ending_totalAmountCO2_mmol = (CO2q->GetMass(MassUnit::g) / CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HCO3q->GetMass(MassUnit::g) / HCO3->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + 4 * (HbCO2q->GetMass(MassUnit::g) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  double ending_totalAmountHb_mmol = (Hbq->GetMass(MassUnit::g) / Hb->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbCO2q->GetMass(MassUnit::g) / HbCO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbO2CO2q->GetMass(MassUnit::g) / HbO2CO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol))
    + (HbO2q->GetMass(MassUnit::g) / HbO2->GetMolarMass(MassPerAmountUnit::g_Per_mmol));

  //CO2 pp
  if (CO2q->GetPartialPressure(PressureUnit::mmHg) < PCO2min_mmHg || CO2q->GetPartialPressure(PressureUnit::mmHg) > PCO2max_mmHg) {
    m_ss.str(std::string());
    m_ss << "CO2 partial pressure (mmHg)" << CO2q->GetPartialPressure(PressureUnit::mmHg) << " is not in range [" << PCO2min_mmHg << ", " << PCO2max_mmHg << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //O2 pp
  if (O2q->GetPartialPressure(PressureUnit::mmHg) < PO2min_mmHg || O2q->GetPartialPressure(PressureUnit::mmHg) > PO2max_mmHg) {
    m_ss.str(std::string());
    m_ss << "O2 partial pressure (mmHg) " << O2q->GetPartialPressure(PressureUnit::mmHg) << " is not in range [" << PO2min_mmHg << ", " << PO2max_mmHg << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //HCO3 molarity
  if (HCO3q->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) < HCO3min_mmol_Per_L || HCO3q->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) > HCO3max_mmol_Per_L) {
    m_ss.str(std::string());
    m_ss << "HCO3 molarity (mmol/L) " << HCO3q->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) << " is not in range [" << HCO3min_mmol_Per_L << ", " << HCO3max_mmol_Per_L << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //CO2 sat
  if (CO2q->GetSaturation().GetValue() < CO2sat_min || CO2q->GetSaturation().GetValue() > CO2sat_max) {
    m_ss.str(std::string());
    m_ss << "CO2 saturation " << CO2q->GetSaturation().GetValue() << " is not in range [" << CO2sat_min << ", " << CO2sat_max << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //O2 sat
  if (O2q->GetSaturation().GetValue() < O2sat_min || O2q->GetSaturation().GetValue() > O2sat_max) {
    m_ss.str(std::string());
    m_ss << "O2 saturation " << O2q->GetSaturation().GetValue() << " is not in range [" << O2sat_min << ", " << O2sat_max << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //pH
  if (testCompartment.GetPH().GetValue() < pH_min || testCompartment.GetPH().GetValue() > pH_max) {
    m_ss.str(std::string());
    m_ss << "pH " << testCompartment.GetPH().GetValue() << " is not in range [" << pH_min << ", " << pH_max << "]. Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //total CO2
  double CO2diff = ending_totalAmountCO2_mmol - starting_totalAmountCO2_mmol;
  if (CO2diff > ZERO_APPROX) {
    m_ss.str(std::string());
    m_ss << "CO2 was not conserved; percentage error: " << 100 * CO2diff / starting_totalAmountCO2_mmol << ". Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //total O2
  double O2diff = ending_totalAmountO2_mmol - starting_totalAmountO2_mmol;
  if (O2diff > ZERO_APPROX) {
    m_ss.str(std::string());
    m_ss << "O2 was not conserved; percentage error: " << 100 * O2diff / starting_totalAmountO2_mmol << ". Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }
  //total Hb
  double Hbdiff = ending_totalAmountHb_mmol - starting_totalAmountHb_mmol;
  if (Hbdiff > ZERO_APPROX) {
    m_ss.str(std::string());
    m_ss << "Hb was not conserved; percentage difference: " << 100 * Hbdiff / starting_totalAmountHb_mmol << ". Test case: " << testCase.GetName() << std::endl;
    testCase.AddFailure(m_ss.str());
  }

  //Total CO2
  double finalDissovledCO2_mM = CO2q->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L);
  double finalBicarbCO2_mM = HCO3q->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L);
  double finalBoundCO2_mM = 4.0 * (HbCO2q->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2q->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L)); //4 moles CO2 per mole Hb
  double finalTotalCO2_mM = finalDissovledCO2_mM + finalBicarbCO2_mM + finalBoundCO2_mM;

  ///// Print results
  trk.Track("StrongIonDifference", testID, strongIonDifference.GetValue(AmountPerVolumeUnit::mmol_Per_L));
  trk.Track("Hematocrit", testID, hematocrit.GetValue());
  trk.Track("HemoglobinConcentration_gPerdL", testID, ending_totalAmountHb_mmol * Hb->GetMolarMass(MassPerAmountUnit::g_Per_mmol) / testCompartment.GetVolume().GetValue(VolumeUnit::dL));
  trk.Track("OxygenSaturation", testID, O2q->GetSaturation().GetValue());
  trk.Track("CarbonDioxideSaturation", testID, CO2q->GetSaturation().GetValue());
  trk.Track("BicarbonateMolarity_mM", testID, HCO3q->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_L));
  trk.Track("OxygenPartialPressure", testID, O2q->GetPartialPressure().GetValue(PressureUnit::mmHg));
  trk.Track("CarbonDioxidePartialPressure", testID, CO2q->GetPartialPressure().GetValue(PressureUnit::mmHg));
  trk.Track("TotalCO2Concentration_mM", testID, finalTotalCO2_mM);
  trk.Track("pH", testID, testCompartment.GetPH().GetValue());

  testCase.GetDuration().SetValue(timer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void BioGearsEngineTest::AcidBaseBloodGasTests(const std::string& sOutputDirectory)
{
  BioGears bg(sOutputDirectory + "/AcidBaseBloodGasTests.log");

  // Set up our test report
  SETestReport testReport = SETestReport(bg.GetLogger());
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName("AcidBaseBloodGasCompartmentTests");

  AcidBaseBloodGasTest(bg, ARTERIAL, testSuite);
  AcidBaseBloodGasTest(bg, VENOUS, testSuite);
  AcidBaseBloodGasTest(bg, CAPILLARY, testSuite);
  /// \todo Do these tests with Strong Ion Differences once we ensure the Sodium, Potassium, and Chloride concentrations are correct and being handled properly
  //AcidBaseBloodGasTest(bg, RESPIRATORY_ACIDOSIS, testSuite, sOutputDirectory);
  //AcidBaseBloodGasTest(bg, METABOLIC_ALKALOSIS, testSuite, sOutputDirectory);
  //AcidBaseBloodGasTest(bg, METABOLIC_ACIDOSIS, testSuite, sOutputDirectory);
  //AcidBaseBloodGasTest(bg, RESPIRATORY_ALKALOSIS, testSuite, sOutputDirectory);

  std::string results = sOutputDirectory + "/AcidBaseBloodGasTests.csv";
  bg.GetDataTrack().WriteTrackToFile(results.c_str());

  testReport.WriteFile(sOutputDirectory + "/AcidBaseBloodGasTestsReport.xml");
}
}
