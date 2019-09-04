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
#include <biogears/engine/Controller/BioGearsSubstances.h>

#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentLink.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>

namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
BioGearsSubstances::BioGearsSubstances(BioGears& data)
  : SESubstanceManager(data.GetLogger())
  , m_data(data)
{
  m_isCOActive = false;
}

void BioGearsSubstances::Clear()
{
  SESubstanceManager::Clear();
  m_O2 = nullptr;
  m_CO = nullptr;
  m_CO2 = nullptr;
  m_N2 = nullptr;
  m_Hb = nullptr;
  m_HbO2 = nullptr;
  m_HbCO2 = nullptr;
  m_HbCO = nullptr;
  m_HbO2CO2 = nullptr;
  m_HCO3 = nullptr;
  m_epi = nullptr;

  m_albumin = nullptr;
  m_aminoAcids = nullptr;
  m_calcium = nullptr;
  m_chloride = nullptr;
  m_creatinine = nullptr;
  m_globulin = nullptr;
  m_glucagon = nullptr;
  m_glucose = nullptr;
  m_insulin = nullptr;
  m_ketones = nullptr;
  m_lactate = nullptr;
  m_potassium = nullptr;
  m_triacylglycerol = nullptr;
  m_sodium = nullptr;
  m_urea = nullptr;

  m_platelets = nullptr;
  m_AntigenA = nullptr;
  m_AntigenB = nullptr;
  m_RBC = nullptr;
  m_WBC = nullptr;
}

void BioGearsSubstances::InitializeSubstances()
{
  // NOTE!!
  // The way BioGears sets up,
  // Substance initialization relies on environmental state,
  // so the environment will read the environment file associated
  // with this engine instance, and it will call AddActiveSubstance
  // and those gases will be added in the order they are read from the file
  // So your substance lists are not guaranteed to be in this order.
  // If this is ever a huge problem, or order is necessary for optimization or something
  // Just make an activate substances method that has to be called first,
  // That just makes an extra thing for unit tests and all that jazz to worry about
  // so I chose not to worry about it as I consider it a minor annoyance

  AddActiveSubstance(*m_O2);
  AddActiveSubstance(*m_CO2);
  AddActiveSubstance(*m_N2);

  AddActiveSubstance(*m_Hb);
  AddActiveSubstance(*m_HbO2);
  AddActiveSubstance(*m_HbCO2);
  AddActiveSubstance(*m_HbO2CO2);
  AddActiveSubstance(*m_HCO3);
  AddActiveSubstance(*m_epi);

  AddActiveSubstance(*m_albumin);
  AddActiveSubstance(*m_aminoAcids);
  AddActiveSubstance(*m_calcium);
  AddActiveSubstance(*m_chloride);
  AddActiveSubstance(*m_creatinine);
  //AddActiveSubstance(*m_globulin);//We don't transport this
  AddActiveSubstance(*m_glucagon);
  AddActiveSubstance(*m_glucose);
  AddActiveSubstance(*m_insulin);
  AddActiveSubstance(*m_ketones);
  AddActiveSubstance(*m_lactate);
  AddActiveSubstance(*m_potassium);
  AddActiveSubstance(*m_sodium);
  AddActiveSubstance(*m_triacylglycerol);
  AddActiveSubstance(*m_urea);

  AddActiveSubstance(*m_platelets);
  AddActiveSubstance(*m_AntigenA);
  AddActiveSubstance(*m_AntigenB);
  AddActiveSubstance(*m_RBC);
  AddActiveSubstance(*m_WBC);

  InitializeGasCompartments();
  InitializeLiquidCompartmentGases();
  InitializeLiquidCompartmentNonGases();
}

void BioGearsSubstances::InitializeGasCompartments()
{
  SEGasCompartment* Ambient = m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  double AmbientO2VF = Ambient->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().GetValue();
  double AmbientCO2VF = Ambient->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().GetValue();
  double AmbientN2VF = Ambient->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().GetValue();

  SEGasCompartment* Mouth = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Mouth);
  Mouth->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(AmbientCO2VF);
  Mouth->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(AmbientN2VF);
  Mouth->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(AmbientO2VF);
  SEGasCompartment* Trachea = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea);
  Trachea->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(AmbientCO2VF);
  Trachea->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(AmbientN2VF);
  Trachea->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(AmbientO2VF);
  Trachea->Balance(BalanceGasBy::VolumeFraction);
  SEGasCompartment* LeftBronchi = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftBronchi);
  LeftBronchi->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(0.011);
  LeftBronchi->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(0.195);
  LeftBronchi->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(1 - 0.011 - 0.195);
  LeftBronchi->Balance(BalanceGasBy::VolumeFraction);
  SEGasCompartment* LeftAlveoli = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli);
  LeftAlveoli->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(0.06);
  LeftAlveoli->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(0.155);
  LeftAlveoli->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(1 - 0.06 - 0.155);
  LeftAlveoli->Balance(BalanceGasBy::VolumeFraction);
  SEGasCompartment* RightBronchi = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightBronchi);
  RightBronchi->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(0.011);
  RightBronchi->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(0.195);
  RightBronchi->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(1 - 0.011 - 0.195);
  RightBronchi->Balance(BalanceGasBy::VolumeFraction);
  SEGasCompartment* RightAlveoli = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightAlveoli);
  RightAlveoli->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(0.06);
  RightAlveoli->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(0.155);
  RightAlveoli->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(1 - 0.06 - 0.155);
  RightAlveoli->Balance(BalanceGasBy::VolumeFraction);
  SEGasCompartment* LeftPleuralCavity = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftPleuralCavity);
  LeftPleuralCavity->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(AmbientCO2VF);
  LeftPleuralCavity->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(AmbientN2VF);
  LeftPleuralCavity->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(AmbientO2VF);
  SEGasCompartment* RightPleuralCavity = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightPleuralCavity);
  RightPleuralCavity->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(AmbientCO2VF);
  RightPleuralCavity->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(AmbientN2VF);
  RightPleuralCavity->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(AmbientO2VF);
  SEGasCompartment* Stomach = m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Stomach);
  Stomach->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(0.01);
  Stomach->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(0.99);
  Stomach->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(0.0);
  Stomach->Balance(BalanceGasBy::VolumeFraction);

  //Initialize the compartments to Ambient values
  for (SEGasCompartment* cmpt : m_data.GetCompartments().GetAnesthesiaMachineLeafCompartments()) {
    if (cmpt->HasVolume()) {
      cmpt->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(AmbientO2VF);
      cmpt->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(AmbientCO2VF);
      cmpt->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(AmbientN2VF);
      cmpt->Balance(BalanceGasBy::VolumeFraction);
    }
  }

  for (SEGasCompartment* cmpt : m_data.GetCompartments().GetInhalerLeafCompartments()) {
    if (cmpt->HasVolume()) {
      cmpt->GetSubstanceQuantity(m_data.GetSubstances().GetO2())->GetVolumeFraction().SetValue(AmbientO2VF);
      cmpt->GetSubstanceQuantity(m_data.GetSubstances().GetCO2())->GetVolumeFraction().SetValue(AmbientCO2VF);
      cmpt->GetSubstanceQuantity(m_data.GetSubstances().GetN2())->GetVolumeFraction().SetValue(AmbientN2VF);
      cmpt->Balance(BalanceGasBy::VolumeFraction);
    }
  }
}

void BioGearsSubstances::InitializeLiquidCompartmentGases()
{
  BioGearsCompartments& cmpts = m_data.GetCompartments();

  SEScalarMassPerVolume albuminConcentration;
  SEScalarFraction hematocrit;
  SEScalarTemperature bodyTemp;
  SEScalarAmountPerVolume strongIonDifference;
  SEScalarAmountPerVolume phosphate;

  albuminConcentration.SetValue(45.0, MassPerVolumeUnit::g_Per_L);
  hematocrit.SetValue(m_data.GetPatient().GetGender() == CDM::enumSex::Male ? 0.45 : 0.40);
  bodyTemp.SetValue(37.0, TemperatureUnit::C);
  strongIonDifference.SetValue(40.5, AmountPerVolumeUnit::mmol_Per_L);
  phosphate.SetValue(1.1, AmountPerVolumeUnit::mmol_Per_L);

  m_data.GetSaturationCalculator().SetBodyState(albuminConcentration, hematocrit, bodyTemp, strongIonDifference, phosphate);

  double Hb_total_g_Per_dL = hematocrit.GetValue() * 34.0;

  // Convert to mmol/L
  // Note that we would need to use 1/4 of the molecular weight of the hemoglobin tetramer
  // to compute a site-based concentration in accordance with lab custom.
  // However, we should account with the actual molarity not the "lab" molarity.
  // So we need to note that our hemoglobin molarity is 1/4 times the reference values
  // because we are using the tetramer concentration not the site concentration.
  // For details please see @cite lodemann2010wrong
  double Hb_total_mM = Hb_total_g_Per_dL / m_Hb->GetMolarMass(MassPerAmountUnit::g_Per_mmol) * 10.0;

  //These values were generated from steady state Standard Male run
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Aorta), Hb_total_mM, 0.979121, 0.13668, 0.0286758, 1.24913, 25.8409, 7.4157);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Bone), Hb_total_mM, 0.811429, 0.0586404, 0.119425, 1.38143, 26.0748, 7.37589);
  if (m_data.GetConfiguration().IsCerebralEnabled()) {
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::CerebralArteries), Hb_total_mM, 0.907482, 0.0788763, 0.0562386, 1.35047, 26.0222, 7.38486);
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::CerebralCapillaries), Hb_total_mM, 0.839259, 0.0629336, 0.111903, 1.37794, 26.069, 7.37689);
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::CerebralVeins), Hb_total_mM, 0.679049, 0.0485416, 0.223764, 1.6691, 26.5149, 7.30101);
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::NeckArteries), Hb_total_mM, 0.979125, 0.136688, 0.0286762, 1.24909, 25.8409, 7.41571);
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::NeckVeins), Hb_total_mM, 0.679995, 0.0485553, 0.223991, 1.66353, 26.5071, 7.30233);
  } else {
    InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Brain), Hb_total_mM, 0.907482, 0.0788763, 0.0562386, 1.35047, 26.0222, 7.38486);
  }
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Fat), Hb_total_mM, 0.811675, 0.0587283, 0.125287, 1.38467, 26.0803, 7.37497);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LargeIntestine), Hb_total_mM, 0.80879, 0.0586749, 0.16279, 1.40653, 26.1167, 7.36877);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftArm), Hb_total_mM, 0.979174, 0.136914, 0.0286486, 1.25161, 25.8455, 7.41492);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftLeg), Hb_total_mM, 0.979174, 0.136914, 0.0286486, 1.25161, 25.8455, 7.41492);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftHeart), Hb_total_mM, 0.979185, 0.136723, 0.0287056, 1.24629, 25.8356, 7.4166);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryArteries), Hb_total_mM, 0.767886, 0.0534961, 0.156613, 1.40149, 26.1084, 7.37019);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryCapillaries), Hb_total_mM, 0.985388, 0.155977, 0.0286948, 1.24095, 25.8257, 7.4183);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryVeins), Hb_total_mM, 0.97884, 0.136049, 0.028664, 1.25052, 25.8435, 7.41526);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftRenalArtery), Hb_total_mM, 0.928045, 0.0860069, 0.0287127, 1.29903, 25.9319, 7.40022);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftAfferentArteriole), Hb_total_mM, 0.900918, 0.0762668, 0.0286836, 1.33099, 25.9884, 7.3906);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftGlomerularCapillaries), Hb_total_mM, 0.875895, 0.0696264, 0.0356313, 1.34054, 26.005, 7.38778);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftEfferentArteriole), Hb_total_mM, 0.851963, 0.0647313, 0.0562525, 1.34992, 26.0212, 7.38502);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPeritubularCapillaries), Hb_total_mM, 0.828783, 0.0607991, 0.0672768, 1.35493, 26.0298, 7.38355);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftRenalVein), Hb_total_mM, 0.793614, 0.0558172, 0.0663554, 1.35408, 26.0284, 7.3838);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftBowmansCapsules), 0.0, 0.0, 0.0773075, 0.0, 1.07857, 0.0, 7.47375);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftTubules), 0.0, 0.0, 0.0773075, 0.0, 1.07857, 0.0, 7.47375);
  //----------------------------------------
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftAtrium), Hb_total_mM, 0.974919, 0.129622, 0.0281731, 1.30089, 25.9384, 7.3997);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftVentricle), Hb_total_mM, 0.974919, 0.129622, 0.0281731, 1.30089, 25.9384, 7.3997);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftLeg), Hb_total_mM, 0.974914, 0.129637, 0.0281665, 1.30153, 25.9395, 7.39951);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPeritubularCapillaries), Hb_total_mM, 0.89667, 0.0757891, 0.0873866, 1.36599, 26.0518, 7.38039);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryArteries), Hb_total_mM, 0.813117, 0.0591941, 0.146207, 1.39669, 26.1035, 7.3716);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryCapillaries), Hb_total_mM, 0.983029, 0.149863, 0.0281766, 1.29193, 25.9223, 7.40244);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryVeins), Hb_total_mM, 0.974924, 0.129359, 0.0282471, 1.29371, 25.9255, 7.40189);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftRenalArtery), Hb_total_mM, 0.941145, 0.0945363, 0.0484668, 1.34704, 26.0194, 7.38592);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftRenalVein), Hb_total_mM, 0.859657, 0.0668517, 0.112249, 1.37857, 26.0731, 7.37676);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Liver), Hb_total_mM, 0.729017, 0.0500882, 0.21436, 1.43708, 26.1698, 7.36032);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Muscle), Hb_total_mM, 0.830253, 0.0616652, 0.130118, 1.38785, 26.0887, 7.37411);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Myocardium), Hb_total_mM, 0.829366, 0.0615371, 0.13173, 1.38874, 26.0902, 7.37386);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Pericardium), Hb_total_mM, 0.725881, 0.0491881, 0.142507, 1.39215, 26.0959, 7.37289);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightAfferentArteriole), Hb_total_mM, 0.927186, 0.0870192, 0.0595885, 1.35232, 26.0285, 7.38437);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightArm), Hb_total_mM, 0.974914, 0.129637, 0.0281665, 1.30153, 25.9395, 7.39951);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightEfferentArteriole), Hb_total_mM, 0.907413, 0.0791142, 0.072315, 1.35842, 26.0389, 7.38259);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightGlomerularCapillaries), Hb_total_mM, 0.9209, 0.0773075, 0.031322, 1.07857, 25.5034, 7.47375);
  //No Hb through glomerular ultrafiltration
  //InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightBowmansCapsules), 0.0, 0.0, 0.0773075, 0.0, 1.07857, 25.5034, 7.47375);

  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightBowmansCapsules), 0.0, 0.0, 0.0773075, 0.0, 1.07857, 0.0, 7.47375);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightTubules), 0.0, 0.0, 0.0773075, 0.0, 1.07857, 0.0, 7.47375);
  //----------------------------------------
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightAtrium), Hb_total_mM, 0.813115, 0.0591938, 0.14621, 1.39669, 26.1035, 7.3716);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightVentricle), Hb_total_mM, 0.813115, 0.0591938, 0.14621, 1.39669, 26.1035, 7.3716);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightLeg), Hb_total_mM, 0.974914, 0.129637, 0.0281665, 1.30153, 25.9395, 7.39951);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightPeritubularCapillaries), Hb_total_mM, 0.89667, 0.0757891, 0.0873866, 1.36599, 26.0518, 7.38039);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryArteries), Hb_total_mM, 0.813117, 0.0591941, 0.146207, 1.39669, 26.1035, 7.3716);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryCapillaries), Hb_total_mM, 0.982298, 0.148493, 0.0279498, 1.31484, 25.9632, 7.39548);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightPulmonaryVeins), Hb_total_mM, 0.974936, 0.130199, 0.0280263, 1.31523, 25.9638, 7.39537);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightRenalArtery), Hb_total_mM, 0.941145, 0.0945363, 0.0484668, 1.34704, 26.0194, 7.38592);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightRenalVein), Hb_total_mM, 0.859657, 0.0668517, 0.112249, 1.37857, 26.0731, 7.37676);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Skin), Hb_total_mM, 0.82836, 0.061373, 0.131491, 1.38858, 26.09, 7.3739);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine), Hb_total_mM, 0.842176, 0.0636112, 0.122046, 1.3836, 26.0816, 7.37532);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Splanchnic), Hb_total_mM, 0.817051, 0.0597293, 0.141898, 1.39428, 26.0995, 7.37228);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::Spleen), Hb_total_mM, 0.830039, 0.0616463, 0.131757, 1.38878, 26.0903, 7.37385);
  InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::VenaCava), Hb_total_mM, 0.813122, 0.0591948, 0.146203, 1.39669, 26.1035, 7.3716);


  //Tissues
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Bone), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Bone));
  if (m_data.GetConfiguration().IsCerebralEnabled()) {
    InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Brain), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::CerebralCapillaries));
  } else {
    InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Brain), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Brain));
  }
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Fat), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Fat));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Gut), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Gut));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::LeftKidney), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftKidney));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::LeftLung), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::LeftLung));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Liver), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Liver));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Muscle), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Muscle));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Myocardium), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Myocardium));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::RightKidney), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightKidney));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::RightLung), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::RightLung));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Skin), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Skin));
  InitializeBloodGases(*cmpts.GetTissueCompartment(BGE::TissueCompartment::Spleen), *cmpts.GetLiquidCompartment(BGE::VascularCompartment::Spleen));

  SEScalarMassPerVolume concentration;
  concentration.SetValue(0.146448, MassPerVolumeUnit::g_Per_dL);
  SetSubstanceConcentration(*m_HCO3, cmpts.GetUrineLeafCompartments(), concentration);
}
void BioGearsSubstances::InitializeBloodGases(SELiquidCompartment& cmpt, double Hb_total_mM, double O2_sat, double O2_mmol_Per_L, double CO2_sat, double CO2_mmol_Per_L, double HCO3_mmol_Per_L, double pH, bool distribute)
{
  // N2 is inert
  SEGasCompartment* Ambient = m_data.GetCompartments().GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  double N2_mmHg = Ambient->GetPressure(PressureUnit::mmHg) * Ambient->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().GetValue();
  SELiquidSubstanceQuantity* N2 = cmpt.GetSubstanceQuantity(*m_N2);
  N2->GetPartialPressure().SetValue(N2_mmHg, PressureUnit::mmHg);
  N2->Balance(BalanceLiquidBy::PartialPressure);

  SELiquidSubstanceQuantity* O2 = cmpt.GetSubstanceQuantity(*m_O2);
  SELiquidSubstanceQuantity* CO2 = cmpt.GetSubstanceQuantity(*m_CO2);
  SELiquidSubstanceQuantity* Hb = cmpt.GetSubstanceQuantity(*m_Hb);
  SELiquidSubstanceQuantity* HbO2 = cmpt.GetSubstanceQuantity(*m_HbO2);
  SELiquidSubstanceQuantity* HbCO2 = cmpt.GetSubstanceQuantity(*m_HbCO2);
  SELiquidSubstanceQuantity* HbO2CO2 = cmpt.GetSubstanceQuantity(*m_HbO2CO2);
  SELiquidSubstanceQuantity* HCO3 = cmpt.GetSubstanceQuantity(*m_HCO3);

  //Assume no HbO2CO2 at first (O2sat + CO2sat < 100%)
  double HbUnbound_mM = Hb_total_mM * (1 - O2_sat - CO2_sat);
  if (std::abs(HbUnbound_mM) <= ZERO_APPROX)
    HbUnbound_mM = 0;

  //If our assumption was wrong, that means there was HbO2CO2 contributing to sat values
  //Any negative is due to HbO2CO2
  if (HbUnbound_mM < 0) {
    double HbO2CO2_mM = -HbUnbound_mM;
    HbO2CO2->GetMolarity().SetValue(HbO2CO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbO2CO2->Balance(BalanceLiquidBy::Molarity);

    HbUnbound_mM = 0;
    Hb->GetMolarity().SetValue(HbUnbound_mM, AmountPerVolumeUnit::mmol_Per_L);
    Hb->Balance(BalanceLiquidBy::Molarity);

    //Now we know HbUnbound and HbO2CO2, and we can solve HbO2 and HbCO2 using saturation values
    double HbO2_mM = O2_sat * Hb_total_mM - HbO2CO2_mM;
    double HbCO2_mM = CO2_sat * Hb_total_mM - HbO2CO2_mM;

    if (std::abs(HbCO2_mM) <= ZERO_APPROX)
      HbCO2_mM = 0;
    if (std::abs(HbO2_mM) <= ZERO_APPROX)
      HbO2_mM = 0;

    HbO2->GetMolarity().SetValue(HbO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbO2->Balance(BalanceLiquidBy::Molarity);
    HbCO2->GetMolarity().SetValue(HbCO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbCO2->Balance(BalanceLiquidBy::Molarity);
  }
  //If our assumption was right, we have excess Hb, and need to use the total Hb to solve
  else {
    double HbO2CO2_mM = (O2_sat * Hb_total_mM) - Hb_total_mM + HbUnbound_mM + (CO2_sat * Hb_total_mM);
    double HbO2_mM = O2_sat * Hb_total_mM - HbO2CO2_mM;
    double HbCO2_mM = CO2_sat * Hb_total_mM - HbO2CO2_mM;

    if (std::abs(HbO2CO2_mM) <= ZERO_APPROX)
      HbO2CO2_mM = 0;
    if (std::abs(HbCO2_mM) <= ZERO_APPROX)
      HbCO2_mM = 0;
    if (std::abs(HbO2_mM) <= ZERO_APPROX)
      HbO2_mM = 0;

    Hb->GetMolarity().SetValue(HbUnbound_mM, AmountPerVolumeUnit::mmol_Per_L);
    Hb->Balance(BalanceLiquidBy::Molarity);
    HbO2CO2->GetMolarity().SetValue(HbO2CO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbO2CO2->Balance(BalanceLiquidBy::Molarity);
    HbO2->GetMolarity().SetValue(HbO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbO2->Balance(BalanceLiquidBy::Molarity);
    HbCO2->GetMolarity().SetValue(HbCO2_mM, AmountPerVolumeUnit::mmol_Per_L);
    HbCO2->Balance(BalanceLiquidBy::Molarity);
  }

  CO2->GetMolarity().SetValue(CO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  CO2->Balance(BalanceLiquidBy::Molarity);
  CO2->GetSaturation().SetValue(CO2_sat);
  HCO3->GetMolarity().SetValue(HCO3_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  HCO3->Balance(BalanceLiquidBy::Molarity);
  O2->GetMolarity().SetValue(O2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  O2->Balance(BalanceLiquidBy::Molarity);
  O2->GetSaturation().SetValue(O2_sat);

  cmpt.GetPH().SetValue(pH);

  if (distribute)
    m_data.GetSaturationCalculator().CalculateBloodGasDistribution(cmpt);

  /*std::cout << cmpt.GetName() << " O2 Partial Pressure " << O2->GetPartialPressure() << std::endl;
  std::cout << cmpt.GetName() << " CO2 Partial Pressure  " << CO2->GetPartialPressure() << std::endl;
  std::cout << cmpt.GetName() << " O2 Concentration " << O2->GetConcentration() << std::endl;
  std::cout << cmpt.GetName() << " CO2 Concentration " << CO2->GetConcentration() << std::endl;
  std::cout << cmpt.GetName() << " HCO3 Molarity " << HCO3->GetMolarity() << std::endl;
  std::cout << " " << std::endl;*/
}
void BioGearsSubstances::InitializeBloodGases(SETissueCompartment& tissue, SELiquidCompartment& vascular)
{
  SELiquidCompartment& extracellular = m_data.GetCompartments().GetExtracellularFluid(tissue);
  extracellular.GetSubstanceQuantity(*m_N2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_N2)->GetMolarity());
  extracellular.GetSubstanceQuantity(*m_O2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_O2)->GetMolarity());
  extracellular.GetSubstanceQuantity(*m_CO2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_CO2)->GetMolarity());
  extracellular.Balance(BalanceLiquidBy::Molarity);

  SELiquidCompartment& intracellular = m_data.GetCompartments().GetIntracellularFluid(tissue);
  intracellular.GetSubstanceQuantity(*m_N2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_N2)->GetMolarity());
  intracellular.GetSubstanceQuantity(*m_O2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_O2)->GetMolarity());
  intracellular.GetSubstanceQuantity(*m_CO2)->GetMolarity().Set(vascular.GetSubstanceQuantity(*m_CO2)->GetMolarity());
  intracellular.Balance(BalanceLiquidBy::Molarity);
}

void BioGearsSubstances::WriteBloodGases()
{
  std::stringstream ss;
  for (SELiquidCompartment* cmpt : m_data.GetCompartments().GetVascularLeafCompartments()) {
    if (cmpt->HasVolume()) {
      SELiquidSubstanceQuantity* O2 = cmpt->GetSubstanceQuantity(*m_O2);
      SELiquidSubstanceQuantity* CO2 = cmpt->GetSubstanceQuantity(*m_CO2);
      SELiquidSubstanceQuantity* Hb = cmpt->GetSubstanceQuantity(*m_Hb);
      SELiquidSubstanceQuantity* HbO2 = cmpt->GetSubstanceQuantity(*m_HbO2);
      SELiquidSubstanceQuantity* HbCO2 = cmpt->GetSubstanceQuantity(*m_HbCO2);
      SELiquidSubstanceQuantity* HbO2CO2 = cmpt->GetSubstanceQuantity(*m_HbO2CO2);
      SELiquidSubstanceQuantity* HCO3 = cmpt->GetSubstanceQuantity(*m_HCO3);

      ss << "InitializeBloodGases(*cmpts.GetLiquidCompartment(BGE::VascularCompartment::" << cmpt->GetName() << "), Hb_total_mM, " << O2->GetSaturation() << ", " << O2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) << ", " << CO2->GetSaturation() << ", " << CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) << ", " << HCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) << ", " << cmpt->GetPH().GetValue() << ");";
      Info(ss);
    }
  }
}
void BioGearsSubstances::WritePulmonaryGases()
{
  std::stringstream ss;
  std::vector<SEGasCompartment*> cmpts;
  cmpts.push_back(m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::Trachea));
  cmpts.push_back(m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftBronchi));
  cmpts.push_back(m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli));
  cmpts.push_back(m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightBronchi));
  cmpts.push_back(m_data.GetCompartments().GetGasCompartment(BGE::PulmonaryCompartment::RightAlveoli));
  for (SEGasCompartment* cmpt : cmpts) {
    if (cmpt->HasVolume()) {
      SEGasSubstanceQuantity* O2 = cmpt->GetSubstanceQuantity(*m_O2);
      SEGasSubstanceQuantity* CO2 = cmpt->GetSubstanceQuantity(*m_CO2);
      SEGasSubstanceQuantity* N2 = cmpt->GetSubstanceQuantity(*m_N2);
      ss << cmpt->GetName() << "->GetSubstanceQuantity(*m_CO2)->GetVolumeFraction().SetValue(" << CO2->GetVolumeFraction() << ");" << std::endl;
      ss << cmpt->GetName() << "->GetSubstanceQuantity(*m_O2)->GetVolumeFraction().SetValue(" << O2->GetVolumeFraction() << ");" << std::endl;
      ss << cmpt->GetName() << "->GetSubstanceQuantity(*m_N2)->GetVolumeFraction().SetValue(" << N2->GetVolumeFraction() << ");" << std::endl;
      Info(ss);
    }
  }
}

void BioGearsSubstances::InitializeLiquidCompartmentNonGases()
{
  const std::vector<SELiquidCompartment*>& urine = m_data.GetCompartments().GetUrineLeafCompartments();
  const std::vector<SELiquidCompartment*>& vascular = m_data.GetCompartments().GetVascularLeafCompartments();
  const std::vector<SETissueCompartment*>& tissue = m_data.GetCompartments().GetTissueLeafCompartments();

  SELiquidSubstanceQuantity* subQ;
  // Initialize Substances throughout the body
  SEScalarMassPerVolume concentration;
  SEScalarAmountPerVolume molarity1;
  SEScalarAmountPerVolume molarity2;

  // Urine cmpts have to be explicitly set to values
  // Upper Tract
  SELiquidCompartment* leftBowmansCapsules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftBowmansCapsules);
  SELiquidCompartment* rightBowmansCapsules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightBowmansCapsules);
  SELiquidCompartment* leftTubules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftTubules);
  SELiquidCompartment* rightTubules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightTubules);
  SELiquidCompartment* leftUreter = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::LeftUreter);
  SELiquidCompartment* rightUreter = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::RightUreter);
  // Lower Tract
  // Note I don't modify the urethra, it's just a flow pipe, with no volume, hence, no substance quantities (NaN)
  //SELiquidCompartment* urethra = m_data.GetCompartments().GetUrineCompartment("Urethra");
  SELiquidCompartment* bladder = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::Bladder);
  SELiquidCompartment* lymph = m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph);
  //Right now the lymph is not used, but code is in place and commented out in case we revisit
  SETissueCompartment* brain = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain);

  // ALBUMIN //
  concentration.SetValue(4.5, MassPerVolumeUnit::g_Per_dL);
  SetSubstanceConcentration(*m_albumin, vascular, concentration);
  // Set Urine
  concentration.SetValue(0.155/3.0, MassPerVolumeUnit::mg_Per_dL);
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_albumin);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_albumin);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  leftTubules->GetSubstanceQuantity(*m_albumin)->SetToZero();
  rightTubules->GetSubstanceQuantity(*m_albumin)->SetToZero();
  leftUreter->GetSubstanceQuantity(*m_albumin)->SetToZero();
  rightUreter->GetSubstanceQuantity(*m_albumin)->SetToZero();
  bladder->GetSubstanceQuantity(*m_albumin)->SetToZero();
  // Tissue
  molarity1.SetValue(20.0 / (m_albumin->GetMolarMass(MassPerAmountUnit::g_Per_mmol)), AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_albumin, tissue, molarity1, molarity2);
  //Lymph - should be same as extracellular
  concentration.SetValue(2.0, MassPerVolumeUnit::g_Per_dL);
  lymph->GetSubstanceQuantity(*m_albumin)->GetConcentration().Set(concentration);
  lymph->GetSubstanceQuantity(*m_albumin)->Balance(BalanceLiquidBy::Concentration);

  // AMINOACIDS //
  concentration.SetValue(50.0, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_aminoAcids, vascular, concentration);
  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_aminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_aminoAcids, tissue, molarity1, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_aminoAcids)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_aminoAcids)->Balance(BalanceLiquidBy::Molarity);

  // BICARBONATE IS SET IN GASES SECTION //

  // CALCIUM //
  concentration.SetValue(80.0, MassPerVolumeUnit::mg_Per_L);
  SetSubstanceConcentration(*m_calcium, vascular, concentration);
  // Tissue
  molarity1.SetValue(1.2, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0001, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_calcium, tissue, molarity1, molarity2);
  //Lymph
  lymph->GetSubstanceQuantity(*m_calcium)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_calcium)->Balance(BalanceLiquidBy::Molarity);
  // Set Urine
  concentration.SetValue(80.0, MassPerVolumeUnit::mg_Per_L);
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftTubules->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftUreter->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightUreter->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = bladder->GetSubstanceQuantity(*m_calcium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);

  // CHLORIDE //
  concentration.SetValue(0.362, MassPerVolumeUnit::g_Per_dL);
  SetSubstanceConcentration(*m_chloride, vascular, concentration);
  SetSubstanceConcentration(*m_chloride, urine, concentration);
  // Tissue
  molarity1.SetValue(102.0, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(4.5, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_chloride, tissue, molarity1, molarity2);
  //Lymph
  lymph->GetSubstanceQuantity(*m_chloride)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_chloride)->Balance(BalanceLiquidBy::Molarity);
  // CREATININE //
  concentration.SetValue(1.2, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_creatinine, vascular, concentration);
  // Set Urine
  concentration.SetValue(0.006, MassPerVolumeUnit::mg_Per_dL);
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  concentration.SetValue(1.25, MassPerVolumeUnit::g_Per_L);
  subQ = leftTubules->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftUreter->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightUreter->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = bladder->GetSubstanceQuantity(*m_creatinine);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  // Tissue
  //molarity1.SetValue(0.044, AmountPerVolumeUnit::mmol_Per_L);
  molarity1.SetValue(0.106, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_creatinine, tissue, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_creatinine)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_creatinine)->Balance(BalanceLiquidBy::Molarity);

  // EPINEPHRINE //
  // Initializing to artificial plasma concentration because BG plasma is not totally correct
  //double hematocritGuess = 0.45;
  concentration.SetValue(0.034, MassPerVolumeUnit::ug_Per_L);
  SetSubstanceConcentration(*m_epi, vascular, concentration);
  // Tissue
  molarity1.SetValue(1.8558e-7, AmountPerVolumeUnit::mmol_Per_L); //epinephrine: 183.2044 g/mol
  //molarity1.SetValue(0, AmountPerVolumeUnit::mmol_Per_L); //epinephrine: 183.2044 g/mol
  SetSubstanceMolarity(*m_epi, tissue, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_epi)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_epi)->Balance(BalanceLiquidBy::Molarity);

  // GLUCAGON //
  concentration.SetValue(0.079, MassPerVolumeUnit::ug_Per_L); //We want 70 pg/mL, but it dips in stabilization, so set it higher
  SetSubstanceConcentration(*m_glucagon, vascular, concentration);
  // Tissue
  molarity1.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_glucagon, tissue, molarity1, molarity2);
  //Lymph
  //Don't set since none in tissue and it's too large to diffuse from bloodstream

  // GLUCOSE //
  concentration.SetValue(90, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_glucose, vascular, concentration);
  // Only Bowmans has it
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_glucose);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_glucose);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  leftTubules->GetSubstanceQuantity(*m_glucose)->SetToZero();
  rightTubules->GetSubstanceQuantity(*m_glucose)->SetToZero();
  leftUreter->GetSubstanceQuantity(*m_glucose)->SetToZero();
  rightUreter->GetSubstanceQuantity(*m_glucose)->SetToZero();
  bladder->GetSubstanceQuantity(*m_glucose)->SetToZero();
  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_glucose, tissue, molarity1, molarity1);
  lymph->GetSubstanceQuantity(*m_glucose)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_glucose)->Balance(BalanceLiquidBy::Molarity);

  // INSULIN //
  concentration.SetValue(0.85, MassPerVolumeUnit::ug_Per_L); //118.1 pmol/L is desired (.6859 ug/L), was .85 because of stabilization dip, but it seems okay now
  SetSubstanceConcentration(*m_insulin, vascular, concentration);
  // None in Urine
  leftBowmansCapsules->GetSubstanceQuantity(*m_insulin)->SetToZero();
  rightBowmansCapsules->GetSubstanceQuantity(*m_insulin)->SetToZero();
  leftTubules->GetSubstanceQuantity(*m_insulin)->SetToZero();
  rightTubules->GetSubstanceQuantity(*m_insulin)->SetToZero();
  leftUreter->GetSubstanceQuantity(*m_insulin)->SetToZero();
  rightUreter->GetSubstanceQuantity(*m_insulin)->SetToZero();
  bladder->GetSubstanceQuantity(*m_insulin)->SetToZero();
  // Tissue
  molarity1.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_insulin, tissue, molarity1, molarity2);
  //Lymph--Don't set since there is none in tissue and it's too large to diffuse from bloodstream

  // KETONES //
  concentration.SetValue(9.19, MassPerVolumeUnit::mg_Per_L);
  SetSubstanceConcentration(*m_ketones, vascular, concentration);
  // Set Urine
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_ketones);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_ketones);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  leftTubules->GetSubstanceQuantity(*m_ketones)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::mg_Per_dL);
  leftTubules->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Concentration);
  rightTubules->GetSubstanceQuantity(*m_ketones)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::mg_Per_dL);
  rightTubules->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Concentration);
  leftUreter->GetSubstanceQuantity(*m_ketones)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::mg_Per_dL);
  leftUreter->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Concentration);
  rightUreter->GetSubstanceQuantity(*m_ketones)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::mg_Per_dL);
  rightUreter->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Concentration);
  bladder->GetSubstanceQuantity(*m_ketones)->GetConcentration().SetValue(0.0, MassPerVolumeUnit::mg_Per_dL);
  bladder->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Concentration);
  // Tissue
  molarity1.SetValue(0.09, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_ketones, tissue, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_ketones)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_ketones)->Balance(BalanceLiquidBy::Molarity);

  // LACTATE //
  concentration.SetValue(80.0, MassPerVolumeUnit::mg_Per_L);
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceConcentration(*m_lactate, vascular, concentration);
  //set in tubules zero in urine
  concentration.SetValue(1.5, MassPerVolumeUnit::g_Per_L);
  subQ = leftTubules->GetSubstanceQuantity(*m_lactate);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_lactate);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  bladder->GetSubstanceQuantity(*m_lactate)->SetToZero();
  //Clear Lactate out of the Ureter to initialize for Gluconeogenesis
  leftUreter->GetSubstanceQuantity(*m_lactate)->SetToZero();
  rightUreter->GetSubstanceQuantity(*m_lactate)->SetToZero();
  // Tissue
  SetSubstanceMolarity(*m_lactate, tissue, molarity1, molarity1);
  //set muscle tissue lactate to higher value --> creates gradient from muscle to blood that causes lactate transfer to blood at same rate that
  //lactate is produced basally in muscles and same rate that kidneys filter lactate.  Net result is stable lactate blood concentration
  double lactateGradient_mg_Per_L = 10.0; //Produces 7 mg/min lactate transport, equal to basal tissue production
  double lactateExtracellular_mg_Per_L = molarity1.GetValue(AmountPerVolumeUnit::mol_Per_L) * m_lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol) * 1000. + lactateGradient_mg_Per_L;
  double lactateIntracellular_mg_Per_L = lactateExtracellular_mg_Per_L + lactateGradient_mg_Per_L;
  SELiquidSubstanceQuantity* muscleExtraLactate = m_data.GetCompartments().GetExtracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(*m_lactate);
  SELiquidSubstanceQuantity* muscleIntraLactate = m_data.GetCompartments().GetIntracellularFluid(*m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Muscle)).GetSubstanceQuantity(*m_lactate);
  muscleExtraLactate->GetConcentration().SetValue(lactateExtracellular_mg_Per_L, MassPerVolumeUnit::mg_Per_L);
  muscleIntraLactate->GetConcentration().SetValue(lactateIntracellular_mg_Per_L, MassPerVolumeUnit::mg_Per_L);
  muscleExtraLactate->Balance(BalanceLiquidBy::Concentration);
  muscleIntraLactate->Balance(BalanceLiquidBy::Concentration);
  //Lymph
  lymph->GetSubstanceQuantity(*m_lactate)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_lactate)->Balance(BalanceLiquidBy::Molarity);

  // POTASSIUM //
  concentration.SetValue(150.5, MassPerVolumeUnit::mg_Per_L);
  SetSubstanceConcentration(*m_potassium, vascular, concentration);
  // Set Urine
  concentration.SetValue(150.5, MassPerVolumeUnit::mg_Per_L);
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftTubules->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftUreter->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightUreter->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = bladder->GetSubstanceQuantity(*m_potassium);
  subQ->GetConcentration().Set(concentration);
  // Tissue
  molarity1.SetValue(4.5, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(110, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_potassium, tissue, molarity1, molarity2);
  //Lymph
  lymph->GetSubstanceQuantity(*m_potassium)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_potassium)->Balance(BalanceLiquidBy::Molarity);

  // SODIUM //
  concentration.SetValue(0.335, MassPerVolumeUnit::g_Per_dL);
  SetSubstanceConcentration(*m_sodium, vascular, concentration);
  // Set Urine
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  concentration.SetValue(0.375, MassPerVolumeUnit::g_Per_dL);
  subQ = leftTubules->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftUreter->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightUreter->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = bladder->GetSubstanceQuantity(*m_sodium);
  subQ->GetConcentration().Set(concentration);
  // Tissue
  molarity1.SetValue(145, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(15, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_sodium, tissue, molarity1, molarity2);
  //Lymph
  lymph->GetSubstanceQuantity(*m_sodium)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_sodium)->Balance(BalanceLiquidBy::Molarity);

  // TRIACYLGLYCEROL //
  concentration.SetValue(75.0, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_triacylglycerol, vascular, concentration);
  // None in Urine
  leftBowmansCapsules->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  rightBowmansCapsules->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  leftTubules->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  rightTubules->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  leftUreter->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  rightUreter->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  bladder->GetSubstanceQuantity(*m_triacylglycerol)->SetToZero();
  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_triacylglycerol, tissue, molarity1, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_triacylglycerol)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
  // TAG can't cross blood-brain barrier, so no TAG there
  molarity1.SetValue(0, AmountPerVolumeUnit::mmol_Per_L);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular)->GetSubstanceQuantity(*m_triacylglycerol)->GetMolarity().Set(molarity1);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular)->GetSubstanceQuantity(*m_triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainIntracellular)->GetSubstanceQuantity(*m_triacylglycerol)->GetMolarity().Set(molarity1);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainIntracellular)->GetSubstanceQuantity(*m_triacylglycerol)->Balance(BalanceLiquidBy::Molarity);

  // UREA //
  concentration.SetValue(270.0, MassPerVolumeUnit::mg_Per_L);
  SetSubstanceConcentration(*m_urea, vascular, concentration);
  // Set Urine
  concentration.SetValue(0.2, MassPerVolumeUnit::mg_Per_dL);
  subQ = leftBowmansCapsules->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightBowmansCapsules->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  concentration.SetValue(20.0, MassPerVolumeUnit::g_Per_L);
  subQ = leftTubules->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightTubules->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = leftUreter->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = rightUreter->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  subQ = bladder->GetSubstanceQuantity(*m_urea);
  subQ->GetConcentration().Set(concentration);
  subQ->Balance(BalanceLiquidBy::Concentration);
  // Tissue
  molarity1.SetValue(4.8, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_urea, tissue, molarity1);
  //Lymph
  lymph->GetSubstanceQuantity(*m_urea)->GetMolarity().Set(molarity1);
  lymph->GetSubstanceQuantity(*m_urea)->Balance(BalanceLiquidBy::Molarity);

  //BLOOD COMPONENTS//
  //Initialize Blood Type
  //RBC
  double rbc_ct_per_uL = 5280000.0;
  double antigens_per_rbc = 2000000.0;
  double rbc_concentration_g_Per_dL = 14.256; //Compounds spreadsheet
  concentration.SetValue(rbc_concentration_g_Per_dL, MassPerVolumeUnit::g_Per_dL);
  SetSubstanceConcentration(*m_RBC, vascular, concentration);

  SEScalarMassPerVolume A_concentration;
  SEScalarMassPerVolume B_concentration;
  if (m_data.GetPatient().GetBloodType() == CDM::enumBloodType::A) {
    A_concentration.SetValue(23.0, MassPerVolumeUnit::mg_Per_L); //Compounds spreadsheet
    B_concentration.SetValue(0.0, MassPerVolumeUnit::mg_Per_L);
  } else if (m_data.GetPatient().GetBloodType() == CDM::enumBloodType::B) {
    A_concentration.SetValue(0.0, MassPerVolumeUnit::mg_Per_L);
    B_concentration.SetValue(19.0, MassPerVolumeUnit::mg_Per_L);
  } else if (m_data.GetPatient().GetBloodType() == CDM::enumBloodType::O) {
    A_concentration.SetValue(0.0, MassPerVolumeUnit::mg_Per_L);
    B_concentration.SetValue(0.0, MassPerVolumeUnit::mg_Per_L);
  } else if (m_data.GetPatient().GetBloodType() == CDM::enumBloodType::AB) {
    A_concentration.SetValue(11.5, MassPerVolumeUnit::mg_Per_L);
    B_concentration.SetValue(9.5, MassPerVolumeUnit::mg_Per_L);
  }

  SetSubstanceConcentration(*m_AntigenA, vascular, A_concentration);
  SetSubstanceConcentration(*m_AntigenB, vascular, B_concentration);
  //WBC
  molarity1.SetValue(7000.0, AmountPerVolumeUnit::ct_Per_uL);
  SetSubstanceMolarity(*m_WBC, vascular, molarity1);
  //Platelets
  molarity1.SetValue(300000.0, AmountPerVolumeUnit::ct_Per_uL);
  SetSubstanceMolarity(*m_platelets, vascular, molarity1);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets the status of blood concentrations to appropriate starved values
///
/// \details
/// The blood concentrations of glucose and ketones are set to match literature values. Insulin and
/// glucagon are not set because they react to set glucose quickly. Other metabolites are not set,
/// but they could be in the future if appropriate validation data is found.
//--------------------------------------------------------------------------------------------------
void BioGearsSubstances::SetLiquidCompartmentNonGasesForStarvation(double time_h)
{
  //This function copies InitializeLiquidCompartmentNonGases() in form and is called
  //from Tissue::SetStarvationState() to configure blood and tissue concentrations during
  //the Starvation condition (urine compartments are not currently considered)

  const std::vector<SELiquidCompartment*>& vascular = m_data.GetCompartments().GetVascularLeafCompartments();
  const std::vector<SETissueCompartment*>& tissue = m_data.GetCompartments().GetTissueLeafCompartments();

  // Initialize Substances throughout the body
  SEScalarMassPerVolume concentration;
  SEScalarAmountPerVolume molarity1;
  SEScalarAmountPerVolume molarity2;

  SETissueCompartment* brain = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::Brain);

  // AMINOACIDS //
  //Probably sholdn't be messed with; see elia1984mineral that says total protein stays ~constant
  /*
  concentration.SetValue(50.0, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_aminoAcids, vascular, concentration);
  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_aminoAcids->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_aminoAcids, tissue, molarity1, molarity1);
  */

  // GLUCAGON //
  //Not modified since it will react to glucose quickly
  /*
  concentration.SetValue(0.079, MassPerVolumeUnit::ug_Per_L);  //We want 70 pg/mL, but it dips in stabilization, so set it higher
  SetSubstanceConcentration(*m_glucagon, vascular, concentration);
  // Tissue
  molarity1.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_glucagon, tissue, molarity1, molarity2);
  */

  // GLUCOSE //
  // \cite garber1974hepatic and \cite owen1971human show glucose concentrations from 0-3 days of fasting and then at 24 days
  // https://www.wolframalpha.com/input/?i=y%3D84.3105+-+.39147x+-+.00000434x%5E2+from+0+%3C+x+%3C+80
  // It's very nearly linear up to 3 days, where it stays hovering around 61 mg/dL
  double conc = 0;
  if (time_h < 72)
    conc = 84.3105 - .39147 * time_h - .00000434 * time_h * time_h;
  else
    conc = 61.25;
  concentration.SetValue(conc, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_glucose, vascular, concentration);

  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_glucose->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_glucose, tissue, molarity1, molarity1);

  // INSULIN //
  //Not modified since it reacts to glucose quickly
  /*
  concentration.SetValue(0.85, MassPerVolumeUnit::ug_Per_L);  //118.1 pmol/L is desired (.6859 ug/L), but it dips during stabilization, so start higher
  SetSubstanceConcentration(*m_insulin, vascular, concentration);

  // Tissue
  molarity1.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  molarity2.SetValue(0.0, AmountPerVolumeUnit::mmol_Per_L);
  SetSubstanceMolarity(*m_insulin, tissue, molarity1, molarity2);
  */

  // KETONES //
  // \cite garber1974hepatic shows ketone concentrations from 0-3 days, mentioning that the peak is around 3 days
  // https://www.wolframalpha.com/input/?i=y%3D2.705%2B.0276875x%2B.00398698x%5E2+from+0%3Cx%3C80
  // We'll hold constant after 3 days, though according to Garber, it might decrease a bit after that
  conc = 0;
  if (time_h < 72)
    conc = 2.705 + .0276875 * time_h + .00398698 * time_h * time_h;
  else
    conc = 25.52;
  concentration.SetValue(conc, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_ketones, vascular, concentration);

  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_ketones->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_ketones, tissue, molarity1, molarity1);

  // LACTATE //
  //Modified to match engine state in order to provide adequate substrate for gluconeogenesis

  concentration.SetValue(32.5, MassPerVolumeUnit::mg_Per_dL);
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_lactate->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceConcentration(*m_lactate, vascular, concentration);

  // Tissue
  SetSubstanceMolarity(*m_lactate, tissue, molarity1, molarity1);

  // TRIACYLGLYCEROL //
  //Not modified. \cite zauner2000resting shows it not changing much from basal levels, but since we don't model fatty acids, we'll see it rise over time.
  /*
  concentration.SetValue(75.0, MassPerVolumeUnit::mg_Per_dL);
  SetSubstanceConcentration(*m_triacylglycerol, vascular, concentration);

  // Tissue
  molarity1.SetValue(concentration.GetValue(MassPerVolumeUnit::g_Per_L) / m_triacylglycerol->GetMolarMass(MassPerAmountUnit::g_Per_mol), AmountPerVolumeUnit::mol_Per_L);
  SetSubstanceMolarity(*m_triacylglycerol, tissue, molarity1, molarity1);
  // TAG can't cross blood-brain barrier, so no TAG there
  molarity1.SetValue(0, AmountPerVolumeUnit::mmol_Per_L);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular)->GetSubstanceQuantity(*m_triacylglycerol)->GetMolarity().Set(molarity1);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular)->GetSubstanceQuantity(*m_triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainIntracellular)->GetSubstanceQuantity(*m_triacylglycerol)->GetMolarity().Set(molarity1);
  m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::BrainIntracellular)->GetSubstanceQuantity(*m_triacylglycerol)->Balance(BalanceLiquidBy::Molarity);
  */

  // IONS //
  //Not modified, but \cite elia1984mineral has good data for Na, K, Ca, and Cl (they don't change much during 4 day starvation)
}

bool BioGearsSubstances::LoadSubstanceDirectory()
{
  if (!SESubstanceManager::LoadSubstanceDirectory())
    return false;

  m_O2 = GetSubstance("Oxygen");
  m_CO = GetSubstance("CarbonMonoxide");
  m_CO2 = GetSubstance("CarbonDioxide");
  m_N2 = GetSubstance("Nitrogen");
  m_Hb = GetSubstance("Hemoglobin");
  m_HbO2 = GetSubstance("Oxyhemoglobin");
  m_HbCO2 = GetSubstance("Carbaminohemoglobin");
  m_HbCO = GetSubstance("Carboxyhemoglobin");
  m_HbO2CO2 = GetSubstance("OxyCarbaminohemoglobin");
  m_HCO3 = GetSubstance("Bicarbonate");
  m_epi = GetSubstance("Epinephrine");

  m_platelets = GetSubstance("Platelet");
  m_AntigenA = GetSubstance("Antigen_A");
  m_AntigenB = GetSubstance("Antigen_B");
  m_RBC = GetSubstance("RedBloodCell");
  m_WBC = GetSubstance("WhiteBloodCell");

  if (m_O2 == nullptr)
    Error("Oxygen Definition not found");
  if (m_CO == nullptr)
    Error("CarbonMonoxide Definition not found");
  if (m_CO2 == nullptr)
    Error("CarbonDioxide Definition not found");
  if (m_N2 == nullptr)
    Error("Nitrogen Definition not found");
  if (m_Hb == nullptr)
    Error("Hemoglobin Definition not found");
  if (m_HbO2 == nullptr)
    Error("Oxyhemoglobin Definition not found");
  if (m_HbCO2 == nullptr)
    Error("Carbaminohemoglobin Definition not found");
  if (m_HbCO == nullptr)
    Error("Carboxyhemoglobin Definition not found");
  if (m_HbO2CO2 == nullptr)
    Error("OxyCarbaminohemoglobin Definition not found");
  if (m_HCO3 == nullptr)
    Error("Bicarbonate Definition not found");
  if (m_epi == nullptr)
    Error("Epinephrine Definition not found");
  if (m_platelets == nullptr)
    Error("Platelets Definition not found");
  if (m_AntigenA == nullptr)
    Error("Antigen A Definition not found");
  if (m_AntigenB == nullptr)
    Error("Antigen B Definition not found");
  if (m_RBC == nullptr)
    Error("Red Blood Cell Definition not found");
  if (m_WBC == nullptr)
    Error("White Blood Cell Definition not found");

  if (m_O2 == nullptr || m_CO == nullptr || m_CO2 == nullptr || m_N2 == nullptr || m_Hb == nullptr || m_HbO2 == nullptr
      || m_HbCO2 == nullptr || m_HbCO == nullptr || m_HbO2CO2 == nullptr || m_epi == nullptr || m_HCO3 == nullptr)
    return false;

  m_albumin = GetSubstance("Albumin");
  m_aminoAcids = GetSubstance("AminoAcids");
  m_calcium = GetSubstance("Calcium");
  m_chloride = GetSubstance("Chloride");
  m_creatinine = GetSubstance("Creatinine");
  m_globulin = GetSubstance("Globulin");
  m_glucagon = GetSubstance("Glucagon");
  m_glucose = GetSubstance("Glucose");
  m_insulin = GetSubstance("Insulin");
  m_ketones = GetSubstance("Ketones");
  m_lactate = GetSubstance("Lactate");
  m_potassium = GetSubstance("Potassium");
  m_sodium = GetSubstance("Sodium");
  m_triacylglycerol = GetSubstance("Triacylglycerol");
  m_urea = GetSubstance("Urea");

  if (m_albumin == nullptr)
    Error("Albumin Definition not found");
  if (m_aminoAcids == nullptr)
    Error("AminoAcids Definition not found");
  if (m_calcium == nullptr)
    Error("Calcium Definition not found");
  if (m_chloride == nullptr)
    Error("Chloride Definition not found");
  if (m_creatinine == nullptr)
    Error("Creatinine Definition not found");
  if (m_globulin == nullptr)
    Error("Globulin Definition not found");
  if (m_glucagon == nullptr)
    Error("Glucagon Definition not found");
  if (m_glucose == nullptr)
    Error("Glucose Definition not found");
  if (m_insulin == nullptr)
    Error("Insulin Definition not found");
  if (m_ketones == nullptr)
    Error("Ketones Definition not found");
  if (m_lactate == nullptr)
    Error("Lactate Definition not found");
  if (m_potassium == nullptr)
    Error("Potassium Definition not found");
  if (m_sodium == nullptr)
    Error("Sodium Definition not found");
  if (m_triacylglycerol == nullptr)
    Error("Triacylglycerol Definition not found");
  if (m_urea == nullptr)
    Error("Urea Definition not found");
  // These metabolites will be activated in initialization

  // Check that drugs have what we need
  for (SESubstance* sub : m_Substances) {
    if (sub->HasPD()) {
      if (sub->GetPD().GetEC50().IsZero() || sub->GetPD().GetEC50().IsNegative()) {
        std::stringstream ss;
        ss << sub->GetName() << " cannot have EC50 <= 0";
        Fatal(ss);
      }
    }
  }

  return true;
}

void BioGearsSubstances::AddActiveSubstance(SESubstance& substance)
{
  if (IsActive(substance))
    return; // If its already active, don't do anything

  SESubstanceManager::AddActiveSubstance(substance);
  if (substance.GetState() == CDM::enumSubstanceState::Gas)
    m_data.GetCompartments().AddGasCompartmentSubstance(substance);
  m_data.GetCompartments().AddLiquidCompartmentSubstance(substance);

  if (&substance == m_CO) // We need to put HbCO in the system if CO is in the system
  {
    m_isCOActive = true;
    AddActiveSubstance(*m_HbCO);
  }
}

bool BioGearsSubstances::IsActive(const SESubstance& sub) const
{
  if (&sub == m_CO)
    return m_isCOActive;
  return SESubstanceManager::IsActive(sub);
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the substance mass cleared for a node
///
/// \details
/// The volume cleared, the compartment, and the substance are provided to clear the mass of the substance
/// from the node. This generic methodology can be used by other systems to calculate the mass cleared.
//--------------------------------------------------------------------------------------------------
void BioGearsSubstances::CalculateGenericClearance(double volumeCleared_mL, SELiquidCompartment& cmpt, SESubstance& sub, SEScalarMass* cleared)
{
  SELiquidSubstanceQuantity* subQ = cmpt.GetSubstanceQuantity(sub);
  if (subQ == nullptr)
    throw CommonDataModelException(std::string{ "No Substance Quantity found for substance " } + sub.GetName());
  //GetMass and Concentration from the compartment
  double mass_ug = subQ->GetMass(MassUnit::ug);
  double concentration_ug_Per_mL = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

  //Calculate Mass Cleared
  double MassCleared_ug = volumeCleared_mL * concentration_ug_Per_mL;
  //Ensure mass does not become negative
  mass_ug -= MassCleared_ug;
  if (mass_ug < 0) {
    mass_ug = 0;
  }

  MassCleared_ug = subQ->GetMass(MassUnit::ug) - mass_ug;
  subQ->GetMass().SetValue(mass_ug, MassUnit::ug);
  subQ->Balance(BalanceLiquidBy::Mass);
  subQ->GetMassCleared().IncrementValue(MassCleared_ug, MassUnit::ug);

  sub.GetSystemicMassCleared().IncrementValue(MassCleared_ug, MassUnit::ug);
  if (cleared != nullptr)
    cleared->SetValue(MassCleared_ug, MassUnit::ug);
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the substance mass cleared for a tissue compartment based on a volume
///
/// \param VolumeCleared_mL: the volume of fluid cleared of a substance by some process
/// \param tissue: a tissue compartment
/// \param sub: a substance
/// \param cleared: mass cleared
///
/// \details
/// The volume cleared, the compartment, and the substance are provided to clear the mass of the substance
/// from the node. This generic methodology can be used by other systems to calculate the mass cleared.
//--------------------------------------------------------------------------------------------------
void BioGearsSubstances::CalculateGenericClearance(double VolumeCleared_mL, SETissueCompartment& tissue, SESubstance& sub, SEScalarMass* cleared)
{
  SELiquidSubstanceQuantity* subQ = m_data.GetCompartments().GetIntracellularFluid(tissue).GetSubstanceQuantity(sub);
  if (subQ == nullptr)
    throw CommonDataModelException(std::string{ "No Substance Quantity found for substance" } + sub.GetName());
  //GetMass and Concentration from the compartment
  double mass_ug = subQ->GetMass(MassUnit::ug);
  double concentration_ug_Per_mL;
  SEScalarMassPerVolume concentration;
  if (sub.HasPK()) {
    GeneralMath::CalculateConcentration(subQ->GetMass(), tissue.GetMatrixVolume(), concentration, m_Logger);
    concentration_ug_Per_mL = concentration.GetValue(MassPerVolumeUnit::ug_Per_mL);
  } else {
    concentration_ug_Per_mL = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  }

  //Calculate Mass Cleared
  double MassCleared_ug = VolumeCleared_mL * concentration_ug_Per_mL;
  //Ensure mass does not become negative
  mass_ug -= MassCleared_ug;
  if (mass_ug < 0) {
    mass_ug = 0;
  }

  MassCleared_ug = subQ->GetMass(MassUnit::ug) - mass_ug;
  subQ->GetMass().SetValue(mass_ug, MassUnit::ug);
  subQ->Balance(BalanceLiquidBy::Mass);
  subQ->GetMassCleared().IncrementValue(MassCleared_ug, MassUnit::ug);
  if (cleared != nullptr)
    cleared->SetValue(MassCleared_ug, MassUnit::ug);
}

/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the substance mass excreted for a compartment
///
/// \details
/// The volume cleared, the compartment, and the substance are provided to clear the mass of the substance
/// from the node. This generic methodology can be used by other systems to calculate the mass excreted.
//--------------------------------------------------------------------------------------------------
void BioGearsSubstances::CalculateGenericExcretion(double VascularFlow_mL_Per_s, SETissueCompartment& tissue, SESubstance& sub, double FractionExcreted, double timestep_s, SEScalarMass* excreted)
{
  SELiquidSubstanceQuantity* subQ = m_data.GetCompartments().GetIntracellularFluid(tissue).GetSubstanceQuantity(sub);
  if (subQ == nullptr)
    throw CommonDataModelException(std::string{ "No Substance Quantity found for substance" } + sub.GetName());
  double concentration_ug_Per_mL;
  SEScalarMassPerVolume concentration;
  if (sub.HasPK()) {
    GeneralMath::CalculateConcentration(subQ->GetMass(), tissue.GetMatrixVolume(), concentration, m_Logger);
    concentration_ug_Per_mL = concentration.GetValue(MassPerVolumeUnit::ug_Per_mL);
  } else {
    concentration_ug_Per_mL = subQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  }

  double MassExcreted_ug = VascularFlow_mL_Per_s * concentration_ug_Per_mL * timestep_s * 0.5 * FractionExcreted; //0.5  is the tuning parameter to remove the correct percentage.

  double mass_ug = subQ->GetMass().GetValue(MassUnit::ug);
  mass_ug = subQ->GetMass().GetValue(MassUnit::ug) - MassExcreted_ug;
  if (mass_ug < 0)
    mass_ug = 0;

  MassExcreted_ug = subQ->GetMass().GetValue(MassUnit::ug) - mass_ug;
  subQ->GetMass().SetValue(mass_ug, MassUnit::ug);
  subQ->Balance(BalanceLiquidBy::Mass);
  subQ->GetMassExcreted().IncrementValue(MassExcreted_ug, MassUnit::ug);
  if (excreted != nullptr)
    excreted->SetValue(MassExcreted_ug, MassUnit::ug);
}

void BioGearsSubstances::ProbeBloodGases(SELiquidCompartment& cmpt, const std::string& prefix)
{
  SELiquidSubstanceQuantity* O2 = cmpt.GetSubstanceQuantity(*m_O2);
  SELiquidSubstanceQuantity* Hb = cmpt.GetSubstanceQuantity(*m_Hb);
  SELiquidSubstanceQuantity* HbO2 = cmpt.GetSubstanceQuantity(*m_HbO2);
  SELiquidSubstanceQuantity* HbO2CO2 = cmpt.GetSubstanceQuantity(*m_HbO2CO2);
  SELiquidSubstanceQuantity* CO2 = cmpt.GetSubstanceQuantity(*m_CO2);
  SELiquidSubstanceQuantity* HbCO2 = cmpt.GetSubstanceQuantity(*m_HbCO2);
  SELiquidSubstanceQuantity* HCO3 = cmpt.GetSubstanceQuantity(*m_HCO3);

  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_pH", cmpt.GetPH().GetValue());
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_O2_ug", O2->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_O2_mmol_Per_L", O2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_O2_mmHg", O2->GetPartialPressure(PressureUnit::mmHg));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_O2_sat", O2->GetSaturation().GetValue());
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_CO2_ug", CO2->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_CO2_mmol_Per_L", CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_CO2_mmHg", CO2->GetPartialPressure(PressureUnit::mmHg));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_CO2_sat", CO2->GetSaturation().GetValue());
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_Hb_ug", Hb->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_Hb_mmol_Per_L", Hb->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbO2_ug", HbO2->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbO2_mmol_Per_L", HbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbCO2_ug", HbCO2->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbCO2_mmol_Per_L", HbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbO2CO2_ug", HbO2CO2->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HbO2CO2_mmol_Per_L", HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HCO3_ug", HCO3->GetMass(MassUnit::ug));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_HCO3_mmol_Per_L", HCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_TotalO2_mmol_per_L", O2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_TotalCO2_mmol_per_L", CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + +HbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_TotalHb_mmol_per_L", Hb->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
  m_data.GetDataTrack().Probe(cmpt.GetName() + prefix + "_TotalMass_mmol_per_L", O2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HCO3->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + Hb->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbCO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L) + HbO2CO2->GetMolarity(AmountPerVolumeUnit::mmol_Per_L));
}

double BioGearsSubstances::GetSubstanceMass(SESubstance& sub, const std::vector<SELiquidCompartment*>& cmpts, const MassUnit& unit)
{
  double mass = 0;
  SELiquidSubstanceQuantity* subQ;
  for (SELiquidCompartment* cmpt : cmpts) {
    subQ = cmpt->GetSubstanceQuantity(sub);
    mass += subQ->GetMass(unit);
  }
  return mass;
}
double BioGearsSubstances::GetSubstanceMass(SESubstance& sub, const std::vector<SETissueCompartment*>& cmpts, const MassUnit& unit)
{
  double mass = 0;
  SELiquidSubstanceQuantity* subQ;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    ;
    mass += subQ->GetMass(unit);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    mass += subQ->GetMass(unit);
  }
  return mass;
}

void BioGearsSubstances::SetSubstanceConcentration(SESubstance& sub, const std::vector<SELiquidCompartment*>& cmpts, const SEScalarMassPerVolume& concentration)
{
  SELiquidSubstanceQuantity* subQ;
  for (SELiquidCompartment* cmpt : cmpts) {
    subQ = cmpt->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(concentration);
    subQ->Balance(BalanceLiquidBy::Concentration);
  }
}
void BioGearsSubstances::SetSubstanceConcentration(SESubstance& sub, const std::vector<SETissueCompartment*>& cmpts, const SEScalarMassPerVolume& concentration)
{
  SELiquidSubstanceQuantity* subQ;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(concentration);
    subQ->Balance(BalanceLiquidBy::Concentration);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(concentration);
    subQ->Balance(BalanceLiquidBy::Concentration);
  }
}
void BioGearsSubstances::SetSubstanceConcentration(SESubstance& sub, const std::vector<SETissueCompartment*>& cmpts, const SEScalarMassPerVolume& extracellular, const SEScalarMassPerVolume& intracellular)
{
  SELiquidSubstanceQuantity* subQ;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(extracellular);
    subQ->Balance(BalanceLiquidBy::Concentration);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetConcentration().Set(intracellular);
    subQ->Balance(BalanceLiquidBy::Concentration);
  }
}

void BioGearsSubstances::SetSubstanceMolarity(SESubstance& sub, const std::vector<SELiquidCompartment*>& cmpts, const SEScalarAmountPerVolume& molarity)
{
  SELiquidSubstanceQuantity* subQ;
  for (SELiquidCompartment* cmpt : cmpts) {
    subQ = cmpt->GetSubstanceQuantity(sub);
    subQ->GetMolarity().Set(molarity);
    subQ->Balance(BalanceLiquidBy::Molarity);
  }
}
void BioGearsSubstances::SetSubstanceMolarity(SESubstance& sub, const std::vector<SETissueCompartment*>& cmpts, const SEScalarAmountPerVolume& molarity)
{
  SELiquidSubstanceQuantity* subQ;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetMolarity().Set(molarity);
    subQ->Balance(BalanceLiquidBy::Molarity);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetMolarity().Set(molarity);
    subQ->Balance(BalanceLiquidBy::Molarity);
  }
}
void BioGearsSubstances::SetSubstanceMolarity(SESubstance& sub, const std::vector<SETissueCompartment*>& cmpts, const SEScalarAmountPerVolume& extracellularMolarity, const SEScalarAmountPerVolume& intracellularMolarity)
{
  SELiquidCompartment* intracellular = nullptr;
  SELiquidCompartment* extracellular = nullptr;

  for (SETissueCompartment* cmpt : cmpts) {
    intracellular = m_data.GetCompartments().GetLiquidCompartment(std::string{ cmpt->GetName() } + "Intracellular");
    if (intracellular != nullptr) {
      intracellular->GetSubstanceQuantity(sub)->GetMolarity().Set(intracellularMolarity);
      intracellular->GetSubstanceQuantity(sub)->Balance(BalanceLiquidBy::Molarity);
    }

    extracellular = m_data.GetCompartments().GetLiquidCompartment(std::string{ cmpt->GetName() } + "Extracellular");
    if (extracellular != nullptr) {
      extracellular->GetSubstanceQuantity(sub)->GetMolarity().Set(extracellularMolarity);
      extracellular->GetSubstanceQuantity(sub)->Balance(BalanceLiquidBy::Molarity);
    }
  }
}
void BioGearsSubstances::SetSubstanceMolarity(SESubstance& sub, const SEScalarAmountPerVolume& extracellular, const SEScalarAmountPerVolume& intracellular)
{
  SELiquidSubstanceQuantity* subQ;
  for (auto itr : m_data.GetCompartments().GetExtracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetMolarity().Set(extracellular);
    subQ->Balance(BalanceLiquidBy::Molarity);
  }
  for (auto itr : m_data.GetCompartments().GetIntracellularFluid()) {
    subQ = itr.second->GetSubstanceQuantity(sub);
    subQ->GetMolarity().Set(intracellular);
    subQ->Balance(BalanceLiquidBy::Molarity);
  }
}

const SizeIndependentDepositionEfficencyCoefficient& BioGearsSubstances::GetSizeIndependentDepositionEfficencyCoefficient(SESubstance& substance)
{
  auto itr = m_SIDECoefficients.find(&substance);
  if (itr != m_SIDECoefficients.end())
    return *itr->second;

  if (!substance.HasAerosolization())
    Fatal("Cannot generate a SIDE Coefficient if no aerosolization data is provided");
  if (!substance.GetAerosolization().HasParticulateSizeDistribution())
    Fatal("Cannot generate a SIDE Coefficient if no particulate distribution is provided");

  // This fraction vs. length histogram characterizes the size distribution for a polydispersed aerosol.
  // Length is the aerodynamic diameter of a particle, and Fraction is the fraction of the total
  // that are contained in bin i, which is bounded by boundary i and boundary i+1.
  // The deposition fraction for each bin is computed from the equations fit by Hinds to the ICRP
  // model data @cite Rostami2009computationsal with as much resolution as is provided by the histogram.
  // Regardless of the provided resolution, particles with aerodynamic diameter less than 10^-4 micrometers
  // all deposit in the head and particles greater than 100 micrometers deposit 50% in the head with 50% loss.
  // For more information see the [aerosol](@ref environment-aerosol) section in the @ref EnvironmentMethodology report.
  // A histogram with n bins must contain n+1 boundaries
  SEHistogramFractionVsLength& concentrations = substance.GetAerosolization().GetParticulateSizeDistribution();

  // Check to make sure everything is in the right form.
  if (!concentrations.IsVaild())
    Fatal("Particle distribution histogram is not valid");

  // First we need compartment-specific deposition fractions for each size
  SEHistogramFractionVsLength depositionsMouth;
  SEHistogramFractionVsLength depositionsCarina;
  SEHistogramFractionVsLength depositionsAnatomicalDeadspace;
  SEHistogramFractionVsLength depositionsAlveoli;
  // Copy sizes
  depositionsMouth.GetLength() = concentrations.GetLength();
  depositionsCarina.GetLength() = concentrations.GetLength();
  depositionsAnatomicalDeadspace.GetLength() = concentrations.GetLength();
  depositionsAlveoli.GetLength() = concentrations.GetLength();

  int numPerRegion = 1000; // This is how many times the equations are evaluated in a bin to get a mean deposition fraction
  for (size_t i = 0; i < concentrations.NumberOfBins(); i++) {
    double binLength = concentrations.GetLength()[i + 1] - concentrations.GetLength()[i];
    double stepSize = binLength / double(numPerRegion); //
    double sumHeadAirways = 0.;
    double sumAnatomicalDeadspace = 0.;
    double sumAlveoli = 0.;
    double aerodynamicDiameter;
    for (int j = 0; j < numPerRegion; j++) {
      aerodynamicDiameter = concentrations.GetLength()[i] + stepSize * j; //Start at the bottom of the bin and march towards the top minus one step
      if (aerodynamicDiameter == 0)
        continue;
      double inspirFrac = 1 - 0.5 * (1 - 1 / (1 + 0.00076 * std::pow(aerodynamicDiameter, 2.8)));
      sumHeadAirways += inspirFrac * (1 / (1 + exp(6.84 + 1.183 * log(aerodynamicDiameter))) + 1 / (1 + exp(0.924 - 1.885 * log(aerodynamicDiameter))));
      sumAnatomicalDeadspace += 0.00352 / aerodynamicDiameter * (exp(-0.23 * std::pow((log(aerodynamicDiameter) + 3.4), 2)) + 63.9 * exp(-0.819 * std::pow((log(aerodynamicDiameter) - 1.61), 2)));
      sumAlveoli += 0.0155 / aerodynamicDiameter * (exp(-0.416 * std::pow((log(aerodynamicDiameter) + 2.84), 2)) + 19.11 * exp(-0.482 * std::pow((log(aerodynamicDiameter) - 1.362), 2)));
    }
    // Mean this region
    depositionsMouth.GetFraction().push_back(sumHeadAirways / numPerRegion);
    depositionsAnatomicalDeadspace.GetFraction().push_back(sumAnatomicalDeadspace / numPerRegion);
    depositionsAlveoli.GetFraction().push_back(sumAlveoli / numPerRegion);
    // If any fractions are more than one, weight the error and distribute
    // More than 1.0 can happen with small particles, possibly due to
    // truncation in the equations in @cite Rostami2009computational.
    if (depositionsMouth.GetFraction()[i] + depositionsAnatomicalDeadspace.GetFraction()[i] + depositionsAlveoli.GetFraction()[i] > 1.0) {
      double mag = depositionsMouth.GetFraction()[i] + depositionsAnatomicalDeadspace.GetFraction()[i] + depositionsAlveoli.GetFraction()[i];
      double delta = 1.0 - mag;
      depositionsMouth.GetFraction()[i] = depositionsMouth.GetFraction()[i] * (1. + delta / mag);
      depositionsAnatomicalDeadspace.GetFraction()[i] = depositionsAnatomicalDeadspace.GetFraction()[i] * (1. + delta / mag);
      depositionsAlveoli.GetFraction()[i] = depositionsAlveoli.GetFraction()[i] * (1. + delta / mag);
    }
    // Head airways are split between mouth and carina
    double carinaFraction = 0.5;
    depositionsCarina.GetFraction().push_back(depositionsMouth.GetFraction()[i] * carinaFraction);
    depositionsMouth.GetFraction()[i] *= (1 - carinaFraction);
  }

  // Now we can compute the size-independent deposition efficiencies for each compartment
  // The fraction of the total that deposits in the mouth is equal to the sum of the
  // products of the fraction of each size times the deposition fractions of each size for the mouth.
  // The fraction of the total that deposits in each subsequent windward compartment is the sum of the
  // products of the fraction of each size times the deposition fractions of each size for the compartment
  // divided by the sum of (1-deposition fraction)*concentration fraction for the leeward compartment.
  double sumMouthProducts = 0.;
  double sumMouthOneMinusProducts = 0.;
  double sumCarinaProducts = 0.;
  double sumCarinaOneMinusProducts = 0.;
  double sumDeadspaceProducts = 0.;
  double sumDeadspaceOneMinusProducts = 0.;
  double sumAlveoliProducts = 0.;

  for (size_t i = 0; i < concentrations.NumberOfBins(); i++) {
    sumMouthProducts += concentrations.GetFraction()[i] * depositionsMouth.GetFraction()[i];
    sumMouthOneMinusProducts += concentrations.GetFraction()[i] * (1 - depositionsMouth.GetFraction()[i]);
    sumCarinaProducts += concentrations.GetFraction()[i] * depositionsCarina.GetFraction()[i];
    sumCarinaOneMinusProducts += concentrations.GetFraction()[i] * (1 - depositionsCarina.GetFraction()[i]);
    sumDeadspaceProducts += concentrations.GetFraction()[i] * depositionsAnatomicalDeadspace.GetFraction()[i];
    sumDeadspaceOneMinusProducts += concentrations.GetFraction()[i] * (1 - depositionsAnatomicalDeadspace.GetFraction()[i]);
    sumAlveoliProducts += concentrations.GetFraction()[i] * depositionsAlveoli.GetFraction()[i];
  }

  SizeIndependentDepositionEfficencyCoefficient* SIDECoefficients = new SizeIndependentDepositionEfficencyCoefficient();
  SIDECoefficients->m_mouth = sumMouthProducts;
  SIDECoefficients->m_trachea = (sumMouthOneMinusProducts < 1.0e-12) ? 0 : sumCarinaProducts / sumMouthOneMinusProducts;
  SIDECoefficients->m_bronchi = (sumCarinaOneMinusProducts < 1.0e-12) ? 0 : sumDeadspaceProducts / sumCarinaOneMinusProducts;
  SIDECoefficients->m_alveoli = (sumDeadspaceOneMinusProducts < 1.0e-12) ? 0 : sumAlveoliProducts / sumDeadspaceOneMinusProducts;
  m_SIDECoefficients[&substance] = SIDECoefficients;
  // Now we do not need to track size distributions, only mass (via concentration).

  return *SIDECoefficients;
}
}