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
#include <biogears/engine/Systems/Renal.h>

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarOsmolality.h>
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/engine/Systems/Drugs.h>

#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
Renal::Renal(BioGears& bg)
  : SERenalSystem(bg.GetLogger())
  , m_data(bg)
{
  Clear();
}

Renal::~Renal()
{
  Clear();
}

void Renal::Clear()
{
  SERenalSystem::Clear();

  m_patient = nullptr;
  m_RenalCircuit = nullptr;
  m_leftGlomerularNode = nullptr;
  m_leftBowmansNode = nullptr;
  m_leftPeritubularNode = nullptr;
  m_leftTubulesNode = nullptr;
  m_leftRenalArteryNode = nullptr;
  m_rightGlomerularNode = nullptr;
  m_rightBowmansNode = nullptr;
  m_rightPeritubularNode = nullptr;
  m_rightTubulesNode = nullptr;
  m_rightRenalArteryNode = nullptr;
  m_bladderNode = nullptr;
  m_leftNetGlomerularCapillariesNode = nullptr;
  m_leftNetBowmansCapsulesNode = nullptr;
  m_leftNetPeritubularCapillariesNode = nullptr;
  m_leftNetTubulesNode = nullptr;
  m_rightNetGlomerularCapillariesNode = nullptr;
  m_rightNetBowmansCapsulesNode = nullptr;
  m_rightNetPeritubularCapillariesNode = nullptr;
  m_rightNetTubulesNode = nullptr;
  m_leftGlomerularOsmoticSourcePath = nullptr;
  m_leftBowmansOsmoticSourcePath = nullptr;
  m_leftReabsorptionResistancePath = nullptr;
  m_leftTubulesOsmoticSourcePath = nullptr;
  m_leftPeritubularOsmoticSourcePath = nullptr;
  m_leftUreterPath = nullptr;
  m_leftGlomerularFilterResistancePath = nullptr;
  m_leftAfferentArteriolePath = nullptr;
  m_rightReabsorptionResistancePath = nullptr;
  m_rightGlomerularOsmoticSourcePath = nullptr;
  m_rightBowmansOsmoticSourcePath = nullptr;
  m_rightTubulesOsmoticSourcePath = nullptr;
  m_rightPeritubularOsmoticSourcePath = nullptr;
  m_rightUreterPath = nullptr;
  m_rightGlomerularFilterResistancePath = nullptr;
  m_rightAfferentArteriolePath = nullptr;
  m_bladderToGroundPressurePath = nullptr;
  m_urethraPath = nullptr;
  m_leftTubulesPath = nullptr;
  m_rightTubulesPath = nullptr;
  m_leftEfferentArteriolePath = nullptr;
  m_rightEfferentArteriolePath = nullptr;

  m_sodium = nullptr;
  m_urea = nullptr;
  m_glucose = nullptr;
  m_lactate = nullptr;
  m_potassium = nullptr;
  m_aorta = nullptr;
  m_venaCava = nullptr;
  m_bladder = nullptr;
  m_rightKidneyTissue = nullptr;
  m_leftKidneyTissue = nullptr;
  m_leftUreter = nullptr;
  m_leftPeritubular = nullptr;
  m_rightUreter = nullptr;
  m_rightPeritubular = nullptr;
  m_leftGlomerular = nullptr;
  m_leftBowmans = nullptr;
  m_rightGlomerular = nullptr;
  m_rightBowmans = nullptr;
  m_leftTubules = nullptr;
  m_rightTubules = nullptr;

  m_aortaLactate = nullptr;

  m_leftPeritubularGlucose = nullptr;
  m_leftPeritubularPotassium = nullptr;
  m_leftUreterLactate = nullptr;
  m_leftUreterPotassium = nullptr;

  m_rightPeritubularGlucose = nullptr;
  m_rightPeritubularPotassium = nullptr;
  m_rightUreterLactate = nullptr;
  m_rightUreterPotassium = nullptr;

  m_bladderGlucose = nullptr;
  m_bladderPotassium = nullptr;
  m_bladderSodium = nullptr;
  m_bladderUrea = nullptr;
  m_leftTubulesSodium = nullptr;
  m_rightTubulesSodium = nullptr;

  m_leftKidneyIntracellularLactate = nullptr;
  m_rightKidneyIntracellularLactate = nullptr;

  m_urineProductionRate_mL_Per_min_runningAvg.Reset();
  m_urineOsmolarity_mOsm_Per_L_runningAvg.Reset();
  m_sodiumExcretionRate_mg_Per_min_runningAvg.Reset();
  m_rightSodiumFlow_mg_Per_s_runningAvg.Reset();
  m_leftSodiumFlow_mg_Per_s_runningAvg.Reset();
  m_leftRenalArterialPressure_mmHg_runningAvg.Reset();
  m_rightRenalArterialPressure_mmHg_runningAvg.Reset();
  m_sodiumConcentration_mg_Per_mL_runningAvg.Reset();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes system properties to valid homeostatic values.
//--------------------------------------------------------------------------------------------------
void Renal::Initialize()
{
  BioGearsSystem::Initialize();

  m_Urinating = false;
  m_leftAfferentResistance_mmHg_s_Per_mL = m_leftAfferentArteriolePath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  m_rightAfferentResistance_mmHg_s_Per_mL = m_rightAfferentArteriolePath->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);
  m_leftSodiumFlowSetPoint_mg_Per_s = 4.7;
  m_rightSodiumFlowSetPoint_mg_Per_s = 4.7;

  //Initialize system data
  GetLeftGlomerularFluidPermeability().SetValue(m_data.GetConfiguration().GetLeftGlomerularFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2), VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  GetLeftGlomerularFiltrationSurfaceArea().SetValue(m_data.GetConfiguration().GetLeftGlomerularFilteringSurfaceAreaBaseline(AreaUnit::m2), AreaUnit::m2);
  GetLeftTubularReabsorptionFluidPermeability().SetValue(m_data.GetConfiguration().GetLeftTubularReabsorptionFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2), VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  GetLeftTubularReabsorptionFiltrationSurfaceArea().SetValue(m_data.GetConfiguration().GetLeftTubularReabsorptionFilteringSurfaceAreaBaseline(AreaUnit::m2), AreaUnit::m2);

  GetRightGlomerularFluidPermeability().SetValue(m_data.GetConfiguration().GetRightGlomerularFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2), VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  GetRightGlomerularFiltrationSurfaceArea().SetValue(m_data.GetConfiguration().GetRightGlomerularFilteringSurfaceAreaBaseline(AreaUnit::m2), AreaUnit::m2);
  GetRightTubularReabsorptionFluidPermeability().SetValue(m_data.GetConfiguration().GetRightTubularReabsorptionFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2), VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  GetRightTubularReabsorptionFiltrationSurfaceArea().SetValue(m_data.GetConfiguration().GetRightTubularReabsorptionFilteringSurfaceAreaBaseline(AreaUnit::m2), AreaUnit::m2);

  GetFiltrationFraction().SetValue(0.2);
  GetGlomerularFiltrationRate().SetValue(180.0, VolumePerTimeUnit::L_Per_day);

  GetLeftBowmansCapsulesHydrostaticPressure().SetValue(18.0, PressureUnit::mmHg);
  GetLeftBowmansCapsulesOsmoticPressure().SetValue(m_leftBowmansOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetLeftGlomerularCapillariesHydrostaticPressure().SetValue(60.0, PressureUnit::mmHg);
  GetLeftGlomerularCapillariesOsmoticPressure().SetValue(m_leftGlomerularOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg); // circuit pressure source baseline
  GetLeftGlomerularFiltrationCoefficient().SetValue(12.5, VolumePerTimePressureUnit::mL_Per_min_mmHg);
  GetLeftGlomerularFiltrationRate().SetValue(90.0, VolumePerTimeUnit::L_Per_day);
  GetLeftNetFiltrationPressure().SetValue(10.0, PressureUnit::mmHg);
  GetLeftNetReabsorptionPressure().SetValue(10.0, PressureUnit::mmHg);
  GetLeftPeritubularCapillariesHydrostaticPressure().SetValue(13.0, PressureUnit::mmHg);
  GetLeftPeritubularCapillariesOsmoticPressure().SetValue(m_leftPeritubularOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetLeftReabsorptionFiltrationCoefficient().SetValue(12.4, VolumePerTimePressureUnit::mL_Per_min_mmHg);
  GetLeftReabsorptionRate().SetValue(62.0, VolumePerTimeUnit::mL_Per_min);
  GetLeftTubularHydrostaticPressure().SetValue(6.0, PressureUnit::mmHg);
  GetLeftTubularOsmoticPressure().SetValue(m_leftTubulesOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetLeftFiltrationFraction().SetValue(0.2);
  GetLeftReabsorptionRate().SetValue(62.0, VolumePerTimeUnit::mL_Per_min);
  GetLeftFiltrationFraction().SetValue(0.2);
  GetLeftAfferentArterioleResistance().Set(m_leftAfferentArteriolePath->GetResistanceBaseline());
  GetLeftEfferentArterioleResistance().Set(m_leftEfferentArteriolePath->GetResistanceBaseline());

  GetRightBowmansCapsulesHydrostaticPressure().SetValue(18.0, PressureUnit::mmHg);
  GetRightBowmansCapsulesOsmoticPressure().SetValue(m_rightBowmansOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetRightGlomerularCapillariesHydrostaticPressure().SetValue(60.0, PressureUnit::mmHg);
  GetRightGlomerularCapillariesOsmoticPressure().SetValue(m_rightGlomerularOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetRightGlomerularFiltrationCoefficient().SetValue(12.5, VolumePerTimePressureUnit::mL_Per_min_mmHg);
  GetRightGlomerularFiltrationRate().SetValue(90.0, VolumePerTimeUnit::L_Per_day);
  GetRightNetFiltrationPressure().SetValue(10.0, PressureUnit::mmHg);
  GetRightNetReabsorptionPressure().SetValue(10.0, PressureUnit::mmHg);
  GetRightPeritubularCapillariesHydrostaticPressure().SetValue(13.0, PressureUnit::mmHg);
  GetRightPeritubularCapillariesOsmoticPressure().SetValue(m_rightPeritubularOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetRightReabsorptionFiltrationCoefficient().SetValue(12.4, VolumePerTimePressureUnit::mL_Per_min_mmHg);
  GetRightTubularHydrostaticPressure().SetValue(6.0, PressureUnit::mmHg);
  GetRightTubularOsmoticPressure().SetValue(m_rightTubulesOsmoticSourcePath->GetPressureSourceBaseline().GetValue(PressureUnit::mmHg), PressureUnit::mmHg);
  GetRightFiltrationFraction().SetValue(0.2);
  GetRightReabsorptionRate().SetValue(62.0, VolumePerTimeUnit::mL_Per_min);
  GetRightFiltrationFraction().SetValue(0.2);
  GetRightAfferentArterioleResistance().Set(m_rightAfferentArteriolePath->GetResistanceBaseline());
  GetRightEfferentArterioleResistance().Set(m_rightEfferentArteriolePath->GetResistanceBaseline());

  GetRenalBloodFlow().SetValue(1132.0, VolumePerTimeUnit::mL_Per_min);
  GetRenalPlasmaFlow().SetValue(660.0, VolumePerTimeUnit::mL_Per_min);
  GetRenalVascularResistance().SetValue(0.081, FlowResistanceUnit::mmHg_min_Per_mL); //  (100-8)/1132
  GetUrinationRate().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  GetUrineOsmolality().SetValue(625.0, OsmolalityUnit::mOsm_Per_kg);
  GetUrineOsmolarity().SetValue(625.0, OsmolarityUnit::mOsm_Per_L);
  GetUrineProductionRate().SetValue(1500.0, VolumePerTimeUnit::mL_Per_day);
  GetUrineSpecificGravity().SetValue(1.016);
  GetUrineVolume().SetValue(0.0, VolumeUnit::mL);
  // Missing Name="UrineUreaConcentration" Unit="g/L"/>
  GetUrineUreaNitrogenConcentration().SetValue(7.5, MassPerVolumeUnit::g_Per_L);

  // Set a Clearance of zero for ALL substances
  // Determine substance filterability based on other parameters (if provided)
  // This won't change, unless the substance parameters change
  for (SESubstance* sub : m_data.GetSubstances().GetSubstances()) {
    CalculateFilterability(*sub);
    if (!sub->GetClearance().HasRenalDynamic())
      sub->GetClearance().SetRenalDynamic(RenalDynamic::Clearance);
    if (!sub->GetClearance().HasRenalClearance())
      sub->GetClearance().GetRenalClearance().SetValue(0.0, VolumePerTimeMassUnit::mL_Per_min_kg);
    if (sub->GetClearance().GetRenalDynamic() == RenalDynamic::Regulation) {
      sub->GetClearance().GetRenalFiltrationRate().SetValue(0.0, MassPerTimeUnit::g_Per_min);
      sub->GetClearance().GetRenalReabsorptionRate().SetValue(0.0, MassPerTimeUnit::g_Per_min);
      sub->GetClearance().GetRenalExcretionRate().SetValue(0.0, MassPerTimeUnit::g_Per_min);
      sub->GetClearance().GetRenalClearance().SetValue(0.0, VolumePerTimeMassUnit::mL_Per_min_kg);
    }
  }
}

bool Renal::Load(const CDM::BioGearsRenalSystemData& in)
{
  if (!SERenalSystem::Load(in))
    return false;

  m_Urinating = in.Urinating();
  m_leftAfferentResistance_mmHg_s_Per_mL = in.LeftAfferentResistance_mmHg_s_Per_mL();
  m_rightAfferentResistance_mmHg_s_Per_mL = in.RightAfferentResistance_mmHg_s_Per_mL();
  m_leftSodiumFlowSetPoint_mg_Per_s = in.LeftSodiumFlowSetPoint_mg_Per_s();
  m_rightSodiumFlowSetPoint_mg_Per_s = in.RightSodiumFlowSetPoint_mg_Per_s();

  m_urineProductionRate_mL_Per_min_runningAvg.Load(in.UrineProductionRate_mL_Per_min());
  m_urineOsmolarity_mOsm_Per_L_runningAvg.Load(in.UrineOsmolarity_mOsm_Per_L());
  m_sodiumConcentration_mg_Per_mL_runningAvg.Load(in.SodiumConcentration_mg_Per_mL());
  m_sodiumExcretionRate_mg_Per_min_runningAvg.Load(in.SodiumExcretionRate_mg_Per_min());
  m_leftSodiumFlow_mg_Per_s_runningAvg.Load(in.LeftSodiumFlow_mg_Per_s());
  m_rightSodiumFlow_mg_Per_s_runningAvg.Load(in.RightSodiumFlow_mg_Per_s());
  m_leftRenalArterialPressure_mmHg_runningAvg.Load(in.LeftRenalArterialPressure_mmHg());
  m_rightRenalArterialPressure_mmHg_runningAvg.Load(in.RightRenalArterialPressure_mmHg());

  BioGearsSystem::LoadState();
  return true;
}
CDM::BioGearsRenalSystemData* Renal::Unload() const
{
  CDM::BioGearsRenalSystemData* data = new CDM::BioGearsRenalSystemData();
  Unload(*data);
  return data;
}
void Renal::Unload(CDM::BioGearsRenalSystemData& data) const
{
  SERenalSystem::Unload(data);

  data.Urinating(m_Urinating);
  data.LeftAfferentResistance_mmHg_s_Per_mL(m_leftAfferentResistance_mmHg_s_Per_mL);
  data.RightAfferentResistance_mmHg_s_Per_mL(m_rightAfferentResistance_mmHg_s_Per_mL);
  data.LeftSodiumFlowSetPoint_mg_Per_s(m_leftSodiumFlowSetPoint_mg_Per_s);
  data.RightSodiumFlowSetPoint_mg_Per_s(m_rightSodiumFlowSetPoint_mg_Per_s);

  data.UrineProductionRate_mL_Per_min(std::unique_ptr<CDM::RunningAverageData>(m_urineProductionRate_mL_Per_min_runningAvg.Unload()));
  data.UrineOsmolarity_mOsm_Per_L(std::unique_ptr<CDM::RunningAverageData>(m_urineOsmolarity_mOsm_Per_L_runningAvg.Unload()));
  data.SodiumConcentration_mg_Per_mL(std::unique_ptr<CDM::RunningAverageData>(m_sodiumConcentration_mg_Per_mL_runningAvg.Unload()));
  data.SodiumExcretionRate_mg_Per_min(std::unique_ptr<CDM::RunningAverageData>(m_sodiumExcretionRate_mg_Per_min_runningAvg.Unload()));
  data.LeftSodiumFlow_mg_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_leftSodiumFlow_mg_Per_s_runningAvg.Unload()));
  data.RightSodiumFlow_mg_Per_s(std::unique_ptr<CDM::RunningAverageData>(m_rightSodiumFlow_mg_Per_s_runningAvg.Unload()));
  data.LeftRenalArterialPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_leftRenalArterialPressure_mmHg_runningAvg.Unload()));
  data.RightRenalArterialPressure_mmHg(std::unique_ptr<CDM::RunningAverageData>(m_rightRenalArterialPressure_mmHg_runningAvg.Unload()));
}

void Renal::SetUp()
{
  m_dt = m_data.GetTimeStep().GetValue(TimeUnit::s);
  m_patient = &m_data.GetPatient();

  //Substances
  m_sodium = &m_data.GetSubstances().GetSodium();
  m_urea = &m_data.GetSubstances().GetUrea();
  m_glucose = &m_data.GetSubstances().GetGlucose();
  m_lactate = &m_data.GetSubstances().GetLactate();
  m_potassium = &m_data.GetSubstances().GetPotassium();

  //Substance quantities

  //Compartments
  m_aorta = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  m_venaCava = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava);

  m_leftKidneyTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::LeftKidney);
  m_leftGlomerular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftGlomerularCapillaries);
  m_leftPeritubular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftPeritubularCapillaries);
  m_leftBowmans = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftBowmansCapsules);
  m_leftTubules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::LeftTubules);
  m_rightKidneyTissue = m_data.GetCompartments().GetTissueCompartment(BGE::TissueCompartment::RightKidney);
  m_rightGlomerular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightGlomerularCapillaries);
  m_rightPeritubular = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightPeritubularCapillaries);
  m_rightBowmans = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightBowmansCapsules);
  m_rightTubules = m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::RightTubules);

  m_bladder = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::Bladder);
  m_leftUreter = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::LeftUreter);
  m_rightUreter = m_data.GetCompartments().GetLiquidCompartment(BGE::UrineCompartment::RightUreter);

  //Configuration parameters
  m_defaultOpenResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_defaultClosedResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetDefaultClosedFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_maxLeftAfferentResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetMaximumAfferentResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_minLeftAfferentResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetMinimumAfferentResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_maxRightAfferentResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetMaximumAfferentResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_minRightAfferentResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetMinimumAfferentResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_sodiumPlasmaConcentrationSetpoint_mg_Per_mL = m_data.GetConfiguration().GetPlasmaSodiumConcentrationSetPoint(MassPerVolumeUnit::mg_Per_mL);
  m_CVOpenResistance_mmHg_s_Per_mL = m_data.GetConfiguration().GetCardiovascularOpenResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  m_baselinePotassiumConcentration_g_Per_dL = m_data.GetConfiguration().GetPeritubularPotassiumConcentrationSetPoint(MassPerVolumeUnit::g_Per_dL);
  m_leftReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2 = m_data.GetConfiguration().GetLeftTubularReabsorptionFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
  m_rightReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2 = m_data.GetConfiguration().GetRightTubularReabsorptionFluidPermeabilityBaseline(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

  m_RenalCircuit = &m_data.GetCircuits().GetRenalCircuit();
  //Left
  m_leftGlomerularNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftGlomerularCapillaries);
  m_leftNetGlomerularCapillariesNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftNetGlomerularCapillaries);
  m_leftBowmansNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftBowmansCapsules);
  m_leftNetBowmansCapsulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftNetBowmansCapsules);
  m_leftPeritubularNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftPeritubularCapillaries);
  m_leftNetPeritubularCapillariesNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftNetPeritubularCapillaries);
  m_leftTubulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftTubules);
  m_leftNetTubulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftNetTubules);
  m_leftRenalArteryNode = m_RenalCircuit->GetNode(BGE::RenalNode::LeftRenalArtery);
  //Right
  m_rightGlomerularNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightGlomerularCapillaries);
  m_rightNetGlomerularCapillariesNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightNetGlomerularCapillaries);
  m_rightBowmansNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightBowmansCapsules);
  m_rightNetBowmansCapsulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightNetBowmansCapsules);
  m_rightPeritubularNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightPeritubularCapillaries);
  m_rightNetPeritubularCapillariesNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightNetPeritubularCapillaries);
  m_rightTubulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightTubules);
  m_rightNetTubulesNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightNetTubules);
  m_rightRenalArteryNode = m_RenalCircuit->GetNode(BGE::RenalNode::RightRenalArtery);
  //Individual
  m_bladderNode = m_RenalCircuit->GetNode(BGE::RenalNode::Bladder);
  //Left
  m_leftGlomerularOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftGlomerularCapillariesToNetGlomerularCapillaries);
  m_leftBowmansOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftBowmansCapsulesToNetBowmansCapsules);
  m_leftReabsorptionResistancePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftNetTubulesToNetPeritubularCapillaries);
  m_leftTubulesOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftTubulesToNetTubules);
  m_leftPeritubularOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftPeritubularCapillariesToNetPeritubularCapillaries);
  m_leftUreterPath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftTubulesToUreter);
  m_leftGlomerularFilterResistancePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftNetGlomerularCapillariesToNetBowmansCapsules);
  m_leftAfferentArteriolePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftAfferentArterioleToGlomerularCapillaries);
  m_leftTubulesPath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftBowmansCapsulesToTubules);
  m_leftEfferentArteriolePath = m_RenalCircuit->GetPath(BGE::RenalPath::LeftEfferentArterioleToPeritubularCapillaries);

  //Right
  m_rightGlomerularOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightGlomerularCapillariesToNetGlomerularCapillaries);
  m_rightBowmansOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightBowmansCapsulesToNetBowmansCapsules);
  m_rightReabsorptionResistancePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightNetTubulesToNetPeritubularCapillaries);
  m_rightTubulesOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightTubulesToNetTubules);
  m_rightPeritubularOsmoticSourcePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightPeritubularCapillariesToNetPeritubularCapillaries);
  m_rightUreterPath = m_RenalCircuit->GetPath(BGE::RenalPath::RightTubulesToUreter);
  m_rightGlomerularFilterResistancePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightNetGlomerularCapillariesToNetBowmansCapsules);
  m_rightAfferentArteriolePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightAfferentArterioleToGlomerularCapillaries);
  m_rightTubulesPath = m_RenalCircuit->GetPath(BGE::RenalPath::RightBowmansCapsulesToTubules);
  m_rightEfferentArteriolePath = m_RenalCircuit->GetPath(BGE::RenalPath::RightEfferentArterioleToPeritubularCapillaries);
  //Individual
  m_bladderToGroundPressurePath = m_RenalCircuit->GetPath(BGE::RenalPath::BladderToGroundPressure);
  m_urethraPath = m_RenalCircuit->GetPath(BGE::RenalPath::BladderToGroundUrinate);

  m_aortaLactate = m_aorta->GetSubstanceQuantity(*m_lactate);

  m_leftPeritubularGlucose = m_leftPeritubular->GetSubstanceQuantity(*m_glucose);
  m_leftPeritubularPotassium = m_leftPeritubular->GetSubstanceQuantity(*m_potassium);
  m_leftUreterLactate = m_leftUreter->GetSubstanceQuantity(*m_lactate);
  m_leftUreterPotassium = m_leftUreter->GetSubstanceQuantity(*m_potassium);

  m_rightPeritubularGlucose = m_rightPeritubular->GetSubstanceQuantity(*m_glucose);
  m_rightPeritubularPotassium = m_rightPeritubular->GetSubstanceQuantity(*m_potassium);
  m_rightUreterLactate = m_rightUreter->GetSubstanceQuantity(*m_lactate);
  m_rightUreterPotassium = m_rightUreter->GetSubstanceQuantity(*m_potassium);

  m_bladderGlucose = m_bladder->GetSubstanceQuantity(*m_glucose);
  m_bladderPotassium = m_bladder->GetSubstanceQuantity(*m_potassium);
  m_bladderSodium = m_bladder->GetSubstanceQuantity(*m_sodium);
  m_bladderUrea = m_bladder->GetSubstanceQuantity(*m_urea);
  m_leftTubulesSodium = m_leftTubules->GetSubstanceQuantity(*m_sodium);
  m_rightTubulesSodium = m_rightTubules->GetSubstanceQuantity(*m_sodium);

  m_leftKidneyIntracellularLactate = m_data.GetCompartments().GetIntracellularFluid(*m_leftKidneyTissue).GetSubstanceQuantity(*m_lactate);
  m_rightKidneyIntracellularLactate = m_data.GetCompartments().GetIntracellularFluid(*m_rightKidneyTissue).GetSubstanceQuantity(*m_lactate);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calls any functions to be run after stabilization
///
/// \param  type    Which version of stabilization has finished
///
/// \details
/// After stabilization is completed, the renal system resets the bladder contents to clear out any errant
/// values that may occur during stabilization. When the consume meal condition is available, the renal system
/// will adjust blood contents as necessary via calculate substance state.
//--------------------------------------------------------------------------------------------------
void Renal::AtSteadyState()
{
  if (m_data.GetState() == EngineState::AtInitialStableState) {
    /*
    if (m_data.GetConditions().HasConsumeMeal())
    {
      double elapsedTime_s = m_data.GetConditions().GetConsumeMeal()->GetMeal().GetElapsedTime(TimeUnit::s);
      ConsumeMeal(elapsedTime_s);
    }
    */
  }

  if (m_data.GetState() == EngineState::AtSecondaryStableState) {
    //We were letting the substances flow out to get the initial concentrations correct
    //We want to do this out of the pressure source, not the urethra to ensure the flow bringing substances into the compartment is the same as the flow taking it out
    //But now we're stable and want to start filling the bladder, so make substances stay in bladder as they come in with the fluid
    SELiquidCompartmentGraph* renalGraph = &m_data.GetCompartments().GetRenalGraph();
    SELiquidCompartmentGraph* combinedCardiovascularGraph = &m_data.GetCompartments().GetActiveCardiovascularGraph();
    renalGraph->RemoveLink(BGE::UrineLink::BladderToGroundSource);
    renalGraph->StateChange();
    combinedCardiovascularGraph->RemoveLink(BGE::UrineLink::BladderToGroundSource);
    combinedCardiovascularGraph->StateChange();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// The calculations to prepare the renal system for the circuit to be processed
///
/// \details
/// Renal preprocess prepares the circuit components and the clearance rates. The substance clearance rates
/// are initialized to the values defined in BioGears.xlsx so all calculations are relative to the initial values.
/// Then clearance rate adjustments occur for specific substances. The circuit is adjusted to adjust urine production
/// rate and to maintain the glomerular pressure, and the urinate function is called in the case of an overfull bladder or
/// a urinate action.
//--------------------------------------------------------------------------------------------------
void Renal::PreProcess()
{
  CalculateUltrafiltrationFeedback();
  CalculateReabsorptionFeedback();
  CalculateTubuloglomerularFeedback();
  UpdateBladderVolume();
  ProcessActions();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Conducts the substance transport for renal
///
/// \details
/// The renal system must run its substance transport independently from the circuit calculations, which
/// occur in Cardiovascular::Process. This series of functions clears all of the necessary substances to
/// the bladder, restores bicarbonate if necessary and calculates the renal systemic outputs.
//--------------------------------------------------------------------------------------------------
void Renal::Process()
{
  //Circuit Processing is done on the entire circulatory circuit elsewhere
  CalculateActiveTransport();  
  CalculateVitalSigns();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Conducts the post processing for the renal system
///
/// \details
/// The renal circuit post processing occurs with the cardiovascular system's post process. There is
/// currently no other functionality needed for renal post process.
//--------------------------------------------------------------------------------------------------
void Renal::PostProcess()
{
  //Circuit PostProcessing is done on the entire circulatory circuit elsewhere
  if (m_data.GetActions().GetPatientActions().HasOverride()
      && m_data.GetState() == EngineState::Active) {
    if (m_data.GetActions().GetPatientActions().GetOverride()->HasRenalOverride()) {
      ProcessOverride();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Adjusts filtration rate of fluid
///
/// \details
/// This function adjusts the filtration resistance of the glomerular capillaries based on any changes in
/// permeability or glomerular capillary surface area. CalculateColloidOsmoticPressure is then called
/// to adjust the pressure gradient from the glomerular capillaries to the bowman's space.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateUltrafiltrationFeedback()
{
  //Tuning parameters
  double glomerularOsmoticSensitivity = 1.0;
  double bowmansOsmoticSensitivity = 1.0;

  //Get substances
  SEFluidCircuitPath* glomerularOsmoticSourcePath = nullptr;
  SEFluidCircuitPath* bowmansOsmoticSourcePath = nullptr;
  SEFluidCircuitPath* filterResistancePath = nullptr;
  SELiquidCompartment* glomerularCapillaries = nullptr;
  double permeability_mL_Per_s_Per_mmHg_Per_m2 = 0.0;
  double surfaceArea_m2 = 0.0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      filterResistancePath = m_leftGlomerularFilterResistancePath;
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetLeftGlomerularFluidPermeability().GetValue(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      surfaceArea_m2 = GetLeftGlomerularFiltrationSurfaceArea().GetValue(AreaUnit::m2);

      glomerularOsmoticSourcePath = m_leftGlomerularOsmoticSourcePath;
      bowmansOsmoticSourcePath = m_leftBowmansOsmoticSourcePath;
      glomerularCapillaries = m_leftGlomerular;
    } else {
      //RIGHT
      filterResistancePath = m_rightGlomerularFilterResistancePath;
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetRightGlomerularFluidPermeability().GetValue(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      surfaceArea_m2 = GetRightGlomerularFiltrationSurfaceArea().GetValue(AreaUnit::m2);

      glomerularOsmoticSourcePath = m_rightGlomerularOsmoticSourcePath;
      bowmansOsmoticSourcePath = m_rightBowmansOsmoticSourcePath;
      glomerularCapillaries = m_rightGlomerular;
    }

    //Set the filter resistance based on its physical properties
    //This is the Capillary Filtration Coefficient
    double filterResistance_mmHg_s_Per_mL = filterResistancePath->GetNextResistance().GetValue(FlowResistanceUnit::mmHg_s_Per_mL);
    if (permeability_mL_Per_s_Per_mmHg_Per_m2 != 0 && surfaceArea_m2 != 0)
      filterResistance_mmHg_s_Per_mL = 1 / (permeability_mL_Per_s_Per_mmHg_Per_m2 * surfaceArea_m2);
    else
      filterResistance_mmHg_s_Per_mL = m_CVOpenResistance_mmHg_s_Per_mL;

    // Bounding the resistance in case the math starts to shoot the value above feasible resistances.
    filterResistance_mmHg_s_Per_mL = std::min(filterResistance_mmHg_s_Per_mL, m_CVOpenResistance_mmHg_s_Per_mL);

    filterResistancePath->GetNextResistance().SetValue(filterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

    //Modify the pressure on both sides of the filter based on the protein (Albumin) concentration
    //This is the osmotic pressure effect
    ///\todo turn on colloid osmotic pressure once substances have been handled properly (and GI)
    // CACHE THIS SUBSTANCE QUANTITY IN SETUP
    //CalculateColloidOsmoticPressure(glomerularCapillaries->GetSubstanceQuantity(m_data.GetSubstances().GetAlbumin())->GetConcentration(), glomerularOsmoticSourcePath->GetNextPressureSource());
    //CalculateColloidOsmoticPressure(bowmansNode->GetSubstanceQuantity(m_albumin)->GetNextConcentration(), bowmansOsmoticSourcePath->GetNextPressureSource());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates changes in fluid reabsorption
///
/// \details
/// This function determines the feedback on reabsorption from changes in oncotic pressure, permeability
/// or the surface area of the renal tubules.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateReabsorptionFeedback()
{
  //Tuning parameters
  double peritubularOsmoticSensitivity = 1.0;
  double tubulesOsmoticSensitivity = 1.0;

  //Determine the permeability
  //Only allow water to be reabsorbed more easily
  //adjust permeability base upon arterial pressure
  CalculateFluidPermeability();
  //Modify the permeability based on plasma sodium concentration
  //This needs to come after CalculateFluidPermeability
  CalculateOsmoreceptorFeedback();

  //Get substances
  SEFluidCircuitPath* peritubularOsmoticSourcePath = nullptr;
  SEFluidCircuitPath* tubulesOsmoticSourcePath = nullptr;
  SEFluidCircuitPath* filterResistancePath = nullptr;
  SELiquidCompartment* peritubularCapillaries = nullptr;
  SELiquidCompartment* renalInterstitial = nullptr;
  double filterResistance_mmHg_s_Per_mL = 0.0;
  double permeability_mL_Per_s_Per_mmHg_Per_m2 = 0.0;
  double surfaceArea_m2 = 0.0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      filterResistancePath = m_leftReabsorptionResistancePath;
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetLeftTubularReabsorptionFluidPermeability().GetValue(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      surfaceArea_m2 = GetLeftTubularReabsorptionFiltrationSurfaceArea().GetValue(AreaUnit::m2);

      peritubularOsmoticSourcePath = m_leftPeritubularOsmoticSourcePath;
      tubulesOsmoticSourcePath = m_leftTubulesOsmoticSourcePath;
      peritubularCapillaries = m_leftPeritubular;
      renalInterstitial = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::LeftKidneyExtracellular);
    } else {
      //RIGHT
      filterResistancePath = m_rightReabsorptionResistancePath;
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetRightTubularReabsorptionFluidPermeability().GetValue(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      surfaceArea_m2 = GetRightTubularReabsorptionFiltrationSurfaceArea().GetValue(AreaUnit::m2);

      peritubularOsmoticSourcePath = m_rightPeritubularOsmoticSourcePath;
      tubulesOsmoticSourcePath = m_rightTubulesOsmoticSourcePath;
      peritubularCapillaries = m_rightPeritubular;
      renalInterstitial = m_data.GetCompartments().GetLiquidCompartment(BGE::ExtravascularCompartment::RightKidneyExtracellular);
    }

    //Set the filter resistance based on its physical properties
    //This is the Capillary Filtration Coefficient
    //We'll just assume this linear relationship for now
    double filterResistance_mmHg_s_Per_mL = filterResistancePath->GetNextResistance().GetValue(FlowResistanceUnit::mmHg_s_Per_mL);
    if (permeability_mL_Per_s_Per_mmHg_Per_m2 != 0 && surfaceArea_m2 != 0)
      filterResistance_mmHg_s_Per_mL = 1 / (permeability_mL_Per_s_Per_mmHg_Per_m2 * surfaceArea_m2);
    else
      filterResistance_mmHg_s_Per_mL = m_CVOpenResistance_mmHg_s_Per_mL;
    filterResistancePath->GetNextResistance().SetValue(filterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

    //Modify the pressure on both sides of the filter based on the protein (Albumin) concentration
    //This is the osmotic pressure effect
    ///\todo turn on colloid osmotic pressure once substances have been handled properly (and GI)
    // CACHE THIS SUBSTANCE QUANTITY IN SETUP
    //CalculateColloidOsmoticPressure(peritubularCapillaries->GetSubstanceQuantity(m_data.GetSubstances().GetAlbumin())->GetConcentration(), peritubularOsmoticSourcePath->GetNextPressureSource());
    //Since we're not modeling the interstitial space, we'll just always keep this side constant
    //We just won't touch it and let it use the baseline value
    //CalculateColloidOsmoticPressure(renalInterstitial->GetSubstanceQuantity(m_data.GetSubstances().GetAlbumin())->GetConcentration(), tubulesOsmoticSourcePath->GetNextPressureSource());
    tubulesOsmoticSourcePath->GetNextPressureSource().SetValue(-15.0, PressureUnit::mmHg);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Turns cleared lactate into glucose
///
/// \details
/// This function performs the renal contribution to gluconeogenesis. The lactate is converted at a
/// 1 to 1 mass ratio into glucose. This will effectively remove all lactate from the urine up until
/// the transport maximum. If the converted mass exceeds the transport maximum, it is capped at the max
/// and the remainder continues to the urine.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateGluconeogenesis()
{
  //Whatever Lactate shows up in the Ureter (i.e. what's excreted) is converted to Glucose and put in the TubularCapillaries (i.e. reabsorbed)

  SEFluidCircuitNode* tubulesNode = nullptr;
  SELiquidSubstanceQuantity* peritubularGlucose = nullptr;
  SELiquidSubstanceQuantity* ureterLactate = nullptr;
  SEFluidCircuitPath* reabsorptionResistancePath = nullptr;

  double totalReabsorptionRate_mg_Per_s = 0.0;
  double totalLactateExcretionRate_mg_Per_s = 0.0;
  double lactateExcreted_mg = 0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    double glucoseReabsorptionMass_mg = 0.0;
    if (kidney == 0) {
      //LEFT
      ureterLactate = m_leftUreterLactate;
      peritubularGlucose = m_leftPeritubularGlucose;
      glucoseReabsorptionMass_mg = m_SubstanceTransport.leftGlucoseReabsorptionMass_mg;
      lactateExcreted_mg = m_SubstanceTransport.leftLactateExcretedMass_mg;
    } else {
      //RIGHT
      ureterLactate = m_rightUreterLactate;
      peritubularGlucose = m_rightPeritubularGlucose;
      glucoseReabsorptionMass_mg = m_SubstanceTransport.rightGlucoseReabsorptionMass_mg;
      lactateExcreted_mg = m_SubstanceTransport.rightLactateExcretedMass_mg;
    }

    double reabsorptionRate_mg_Per_s = (lactateExcreted_mg + glucoseReabsorptionMass_mg) / m_dt;

    //Convert 1-to-1 Lactate to Glucose and put in PeritubularCapillaries
    //If Converted Glucose + Reabsorbed Glucose > TM, the difference is excreted as Lactate
    if (!m_glucose->GetClearance().GetRenalTransportMaximum().IsInfinity()) {
      double transportMaximum_mg_Per_s = m_glucose->GetClearance().GetRenalTransportMaximum(MassPerTimeUnit::mg_Per_s);
      reabsorptionRate_mg_Per_s = std::min(reabsorptionRate_mg_Per_s, transportMaximum_mg_Per_s);
    }

    double massToMove_mg = reabsorptionRate_mg_Per_s * m_dt;
    massToMove_mg = std::max(massToMove_mg, 0.0);
    double lactateConverted_mg = massToMove_mg - glucoseReabsorptionMass_mg;
    lactateExcreted_mg = massToMove_mg - (lactateExcreted_mg + glucoseReabsorptionMass_mg);

    //Increment & decrement
    ureterLactate->GetMass().IncrementValue(-lactateConverted_mg, MassUnit::mg);
    peritubularGlucose->GetMass().IncrementValue(lactateConverted_mg, MassUnit::mg);

    //Sometimes we pull everything out of the ureterNode, but get a super small negative mass
    //Just make that super small negative mass zero
    if (ureterLactate->GetMass().IsNegative()) {
      ureterLactate->GetMass().SetValue(0.0, MassUnit::mg);
    }

    //Calculate new concentrations
    ureterLactate->Balance(BalanceLiquidBy::Mass);
    peritubularGlucose->Balance(BalanceLiquidBy::Mass);

    //Set the substance output values
    totalReabsorptionRate_mg_Per_s += reabsorptionRate_mg_Per_s;
    totalLactateExcretionRate_mg_Per_s += lactateExcreted_mg / m_dt;
  }

  //Set the substance output values
  m_lactate->GetClearance().GetRenalExcretionRate().SetValue(totalLactateExcretionRate_mg_Per_s, MassPerTimeUnit::mg_Per_s);

  double plasmaConcentration_mg_Per_mL = m_aortaLactate->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
  double patientWeight_kg = m_patient->GetWeight(MassUnit::kg);
  m_lactate->GetClearance().GetRenalClearance().SetValue(totalLactateExcretionRate_mg_Per_s / plasmaConcentration_mg_Per_mL / patientWeight_kg, VolumePerTimeMassUnit::mL_Per_s_kg);

  double singleExcreted_mg = totalLactateExcretionRate_mg_Per_s * m_dt * 0.5; // We are assuming the kidneys are each doing the same amount of work
  m_leftKidneyIntracellularLactate->GetMassExcreted().IncrementValue(singleExcreted_mg, MassUnit::mg);
  m_leftKidneyIntracellularLactate->GetMassCleared().IncrementValue(singleExcreted_mg, MassUnit::mg);
  m_rightKidneyIntracellularLactate->GetMassExcreted().IncrementValue(singleExcreted_mg, MassUnit::mg);
  m_rightKidneyIntracellularLactate->GetMassCleared().IncrementValue(singleExcreted_mg, MassUnit::mg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Executes renal actions
///
/// \details
/// This function is called to run through all renal actions in the event that one has been called.
/// Currently the only renal action is Urinate.
//--------------------------------------------------------------------------------------------------
void Renal::ProcessActions()
{
  Urinate();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calls the necessary transport methodology for each substance
///
/// \details
/// BioGears has two types of renal substance handling, Regulation and Clearance. Clearance is generally
/// used for drugs, but can be specified for any substance. It does not support calculating filtration,
/// reabsorption and excretion, but rather determines how much mass should be excreted as urine based on
/// preset parameters. Regulation is used to model filtration, reabsorption and excretion. This function
/// also calculates average clearance rates for substances to be used in the consume meal condition.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateActiveTransport()
{
  m_SubstanceTransport.leftLactateExcretedMass_mg = 0;
  m_SubstanceTransport.rightLactateExcretedMass_mg = 0;
  m_SubstanceTransport.leftGlucoseReabsorptionMass_mg = 0.0;
  m_SubstanceTransport.rightGlucoseReabsorptionMass_mg = 0.0;

  unsigned int i = 0;
  for (SESubstance* sub : m_data.GetCompartments().GetLiquidCompartmentSubstances()) {
    if (!sub->HasClearance())
      continue;
    if (!sub->GetClearance().HasRenalDynamic())
      continue;
    if (sub->GetClearance().GetRenalDynamic() == RenalDynamic::Regulation) {
      //This is the generic methodology
      CalculateGlomerularTransport(*sub);
      CalculateReabsorptionTransport(*sub);
      if (sub == m_potassium) {
        CalculateSecretion();
      }

      CalculateExcretion(*sub);
    } else if (sub->GetClearance().GetRenalDynamic() == RenalDynamic::Clearance) {
      //This bypasses the generic methodology and just automatically clears
      CalculateAutomaticClearance(*sub);
    } else {
      /// \error Fatal: Unrecognized renal clearance type
      Fatal("Unrecognized renal clearance type.");
    }
  }

  //Convert excreted Lactate to Glucose
  CalculateGluconeogenesis();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines filtered mass flow
///
/// \param  sub Substance to be calculated
///
/// \details
/// This function calculates how much mass of a substance makes it through the glomerular capillaries
/// to the Bowman's space. It does this based on the fluid flow, the filterability of the substance and
/// the fraction unbound in plasma.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateGlomerularTransport(SESubstance& sub)
{
  SELiquidCompartment* glomerular = nullptr;
  SELiquidCompartment* bowmans = nullptr;
  SEFluidCircuitPath* filterResistancePath = nullptr;

  double filtrationRate_mg_Per_s = 0.0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      glomerular = m_leftGlomerular;
      bowmans = m_leftBowmans;
      filterResistancePath = m_leftGlomerularFilterResistancePath;
    } else {
      //RIGHT
      glomerular = m_rightGlomerular;
      bowmans = m_rightBowmans;
      filterResistancePath = m_rightGlomerularFilterResistancePath;
    }
    if (!sub.GetClearance().HasGlomerularFilterability()) {
      CalculateFilterability(sub);
    }
    double filterability = sub.GetClearance().GetGlomerularFilterability().GetValue();

    SELiquidSubstanceQuantity* bowmansSubQ = bowmans->GetSubstanceQuantity(sub);
    SELiquidSubstanceQuantity* glomerularSubQ = glomerular->GetSubstanceQuantity(sub);

    //Now do the transport
    double concentration_mg_Per_mL = glomerularSubQ->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
    double flow_mL_Per_s = filterResistancePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);

    //Don't allow back flow
    if (flow_mL_Per_s < 0.0) {
      continue;
    }

    //Determine how much is unbound - i.e. available to move
    double fractionUnbound = sub.GetClearance().GetFractionUnboundInPlasma().GetValue();

    double massToMove_mg = concentration_mg_Per_mL * flow_mL_Per_s * m_dt * filterability * fractionUnbound;

    //Make sure we don't try to move too much
    massToMove_mg = std::min(massToMove_mg, glomerularSubQ->GetMass().GetValue(MassUnit::mg));

    //Increment & decrement
    glomerularSubQ->GetMass().IncrementValue(-massToMove_mg, MassUnit::mg);
    bowmansSubQ->GetMass().IncrementValue(massToMove_mg, MassUnit::mg);

    //Sometimes we pull everything out of the Glomerular, but get a super small negative mass
    //Just make that super small negative mass zero
    if (glomerularSubQ->GetMass().GetValue(MassUnit::mg) < 0.0) {
      glomerularSubQ->GetMass().SetValue(0.0, MassUnit::mg);
    }

    //Calculate new concentrations
    glomerularSubQ->Balance(BalanceLiquidBy::Mass);
    bowmansSubQ->Balance(BalanceLiquidBy::Mass);

    //Set the substance output values
    filtrationRate_mg_Per_s += massToMove_mg / m_dt;
  }

  //Set the substance output values
  sub.GetClearance().GetRenalFiltrationRate().SetValue(filtrationRate_mg_Per_s, MassPerTimeUnit::mg_Per_s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines secretion of substance from peritubular capillaries to ureter
///
/// \param  sub Substance to be secreted
///
/// \details
/// This function calculates how much of a substance is secreted into the distal tubules and collecting ducts
/// from the peritubular capillaries in the Kidney. We allow this process to take place after reabsorption and
/// place the substance directly into the ureter node as a means to simulate the distal and collecting ducts.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateSecretion()
{
  SELiquidSubstanceQuantity* peritubularPotassium = nullptr;
  SELiquidSubstanceQuantity* ureterPotassium = nullptr;

  double massPotassiumToMove_mg = 0.0;
  double potassiumConcentration_g_Per_dL = 0.0;
  double peritubularVolume_dL = 0.0;

  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      ureterPotassium = m_leftUreterPotassium;
      peritubularPotassium = m_leftPeritubularPotassium;
      peritubularVolume_dL = m_leftPeritubular->GetVolume().GetValue(VolumeUnit::dL);
    } else {
      //RIGHT
      ureterPotassium = m_rightUreterPotassium;
      peritubularPotassium = m_rightPeritubularPotassium;
      peritubularVolume_dL = m_rightPeritubular->GetVolume().GetValue(VolumeUnit::dL);
    }

    //grab current concentration and volume,
    potassiumConcentration_g_Per_dL = peritubularPotassium->GetConcentration().GetValue(MassPerVolumeUnit::g_Per_dL);

    //only do if current levels of potassium are too high:
    if (potassiumConcentration_g_Per_dL > m_baselinePotassiumConcentration_g_Per_dL) {
      //calculate mass to move in mg:
      massPotassiumToMove_mg = (potassiumConcentration_g_Per_dL - m_baselinePotassiumConcentration_g_Per_dL) * peritubularVolume_dL;

      //Increment & decrement
      peritubularPotassium->GetMass().IncrementValue(-massPotassiumToMove_mg, MassUnit::mg);
      ureterPotassium->GetMass().IncrementValue(massPotassiumToMove_mg, MassUnit::mg);

      // if its super small negative mass set to zero:
      if (peritubularPotassium->GetMass().IsNegative())
        peritubularPotassium->GetMass().SetValue(0.0, MassUnit::mg);

      //Calculate new concentrations
      ureterPotassium->Balance(BalanceLiquidBy::Mass);
      peritubularPotassium->Balance(BalanceLiquidBy::Mass);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Determines filterability for a substance
///
/// \param  sub Substance to be calculated
///
/// \details
/// This function calculates the filterability for a substance based on its molar mass and charge.
/// the calculation for filterability derives from a curve fit set to curves in @cite valtin1995renal .
//--------------------------------------------------------------------------------------------------
void Renal::CalculateFilterability(SESubstance& sub)
{
  double molarMass_g_Per_mol = sub.GetMolarMass(MassPerAmountUnit::g_Per_mol);
  //Determine the molecular radius using a best fit equation
  double molecularRadius_nm = 0.0348 * std::pow(molarMass_g_Per_mol, 0.4175);
  //Determine how well this substance transports with respect to water
  //There are three best fit curves that vary by molecule charge
  double filterability = 0.0;

  //Everything below a certain size is 1.0, and everything above is 0.0
  if (molecularRadius_nm < 1.8) {
    filterability = 1.0;
  } else if (molecularRadius_nm > 4.4) {
    filterability = 0.0;
  } else {
    switch (sub.GetClearance().GetChargeInBlood()) {
    case CDM::enumCharge::Positive:
      filterability = 0.0386 * std::pow(molecularRadius_nm, 4.0) - 0.431 * std::pow(molecularRadius_nm, 3.0)
        + 1.61 * std::pow(molecularRadius_nm, 2.0) - 2.6162 * molecularRadius_nm + 2.607;
      break;
    case CDM::enumCharge::Neutral:
      filterability = -0.0908 * std::pow(molecularRadius_nm, 4.0) + 1.2135 * std::pow(molecularRadius_nm, 3.0)
        - 5.76 * std::pow(molecularRadius_nm, 2.0) + 11.013 * molecularRadius_nm - 6.2792;
      break;
    case CDM::enumCharge::Negative:
      //Subtracting 0.01 to account for not enough significant digits given by the best fit - tuned looking at data table from report and confirmed for Albumin
      filterability = 0.0616 * std::pow(molecularRadius_nm, 4.0) - 0.8781 * std::pow(molecularRadius_nm, 3.0)
        + 4.6699 * std::pow(molecularRadius_nm, 2.0) - 10.995 * molecularRadius_nm + 9.6959 - 0.01;
      break;
    default:
      break;
    }
  }

  //Bound it
  filterability = LIMIT(filterability, 0.0, 1.0);

  //Set the substance output values
  sub.GetClearance().GetGlomerularFilterability().SetValue(filterability);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates reabsorption mass flow
///
/// \param  sub Substance to be calculated
///
/// \details
/// This function determines how much of a substance is reabsorbed each time step. This is primarily
/// determined through the fluid flow and reabsorption ratio set in the substance file. If the reabsorbed
/// mass exceeds the transport maximum, the reabsorbed mass is capped at the max and the remainder continues
/// to the bladder.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateReabsorptionTransport(SESubstance& sub)
{
  //Tuning parameters
  //Lower number = more massed moved
  //double permeabilitySensitivity = 1.0;

  SELiquidSubstanceQuantity* tubulesSubQ = nullptr;
  SELiquidSubstanceQuantity* peritubularSubQ = nullptr;
  SEFluidCircuitPath* reabsorptionResistancePath = nullptr;

  double totalReabsorptionRate_mg_Per_s = 0.0;
  //We'll apply the factor to the effectively make the FiltrationFraction change by the same amount
  //This is determined in Osmoreceptor Feedback
  double permeabilityModificationFactor = 1.0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT

      tubulesSubQ = m_leftTubules->GetSubstanceQuantity(sub);
      peritubularSubQ = m_leftPeritubular->GetSubstanceQuantity(sub);
      reabsorptionResistancePath = m_leftReabsorptionResistancePath;
      //Add a factor to make substances reabsorb more or less avidly
      permeabilityModificationFactor = m_leftReabsorptionPermeabilityModificationFactor;
    } else {
      //RIGHT
      tubulesSubQ = m_rightTubules->GetSubstanceQuantity(sub);
      peritubularSubQ = m_rightPeritubular->GetSubstanceQuantity(sub);
      reabsorptionResistancePath = m_rightReabsorptionResistancePath;
      //Add a factor to make substances reabsorb more or less avidly
      permeabilityModificationFactor = m_rightReabsorptionPermeabilityModificationFactor;
    }

    //Now do the transport
    double concentration_mg_Per_mL = tubulesSubQ->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
    double flow_mL_Per_s = reabsorptionResistancePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);

    //Don't allow back flow
    if (flow_mL_Per_s < 0.0) {
      continue;
    }

    //Get the ratio of how this substance moves with water flow
    double massToMove_mg = 0.0;
    if (sub.GetClearance().GetRenalReabsorptionRatio().IsInfinity()) {
      //Infinity, so move all the mass
      massToMove_mg = tubulesSubQ->GetMass().GetValue(MassUnit::mg);
    } else //Not Infinity
    {

      double reabsorptionRatio = sub.GetClearance().GetRenalReabsorptionRatio().GetValue();
      double massModification = 1.0 / permeabilityModificationFactor;
      //limit the ratio to 1 to allow for concentrated urine
      massModification = std::min(massModification, 1.0);
      massToMove_mg = concentration_mg_Per_mL * flow_mL_Per_s * m_dt * reabsorptionRatio * massModification;
    }

    //Make sure we don't try to move too much
    massToMove_mg = std::min(massToMove_mg, tubulesSubQ->GetMass().GetValue(MassUnit::mg));

    double reabsorptionRate_mg_Per_s = massToMove_mg / m_dt;
    //Stay below the maximum allowable transport
    if (!sub.GetClearance().GetRenalTransportMaximum().IsInfinity()) {
      double transportMaximum_mg_Per_s = sub.GetClearance().GetRenalTransportMaximum().GetValue(MassPerTimeUnit::mg_Per_s);
      reabsorptionRate_mg_Per_s = std::min(reabsorptionRate_mg_Per_s, transportMaximum_mg_Per_s);
    }

    massToMove_mg = reabsorptionRate_mg_Per_s * m_dt;

    //Store information about glucose to be used later in Gluconeogenesis
    if (&sub == m_glucose) {
      if (kidney == 0) {
        //Left
        m_SubstanceTransport.leftGlucoseReabsorptionMass_mg = massToMove_mg;
      } else {
        //Right
        m_SubstanceTransport.rightGlucoseReabsorptionMass_mg = massToMove_mg;
      }
    }

    //Increment & decrement
    tubulesSubQ->GetMass().IncrementValue(-massToMove_mg, MassUnit::mg);
    peritubularSubQ->GetMass().IncrementValue(massToMove_mg, MassUnit::mg);

    //Sometimes we pull everything out of the Tubules, but get a super small negative mass
    //Just make that super small negative mass zero
    if (tubulesSubQ->GetMass().IsNegative())
      tubulesSubQ->GetMass().SetValue(0.0, MassUnit::mg);

    //Calculate new concentrations
    tubulesSubQ->Balance(BalanceLiquidBy::Mass);
    peritubularSubQ->Balance(BalanceLiquidBy::Mass);

    //Set the substance output values
    totalReabsorptionRate_mg_Per_s += reabsorptionRate_mg_Per_s;
  }

  //Set the substance output values
  sub.GetClearance().GetRenalReabsorptionRate().SetValue(totalReabsorptionRate_mg_Per_s, MassPerTimeUnit::mg_Per_s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates mass excreted for a substance
///
/// \param  sub Substance to be calculated
///
/// \details
/// This function calculates the mass flow from the tubules to the bladder for each substance. It also
/// back calculates the renal clearance rate for the substance.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateExcretion(SESubstance& sub)
{
  //This will always be a time-step behind

  SELiquidCompartment* tubules = nullptr;
  SEFluidCircuitPath* excretionPath = nullptr;
  double totalExcretionRate_mg_Per_s = 0.0;

  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      tubules = m_leftTubules;
      excretionPath = m_leftUreterPath;
    } else {
      //RIGHT
      tubules = m_rightTubules;
      excretionPath = m_rightUreterPath;
    }

    //Note: this will be off a little because the concentration is a time-step behind
    //We should use the "current" concentration and "next" flow, since that's what was just moved by the transporter
    double tubulesConcentration_mg_Per_mL = tubules->GetSubstanceQuantity(sub)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
    double excretionFlow_mL_Per_s = excretionPath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
    double excretionRate_mg_Per_s = tubulesConcentration_mg_Per_mL * excretionFlow_mL_Per_s;
    //Make sure it's not a super small negative number
    totalExcretionRate_mg_Per_s += std::max(excretionRate_mg_Per_s, 0.0);

    if (&sub == m_lactate) {
      if (kidney == 0) {
        //Left
        m_SubstanceTransport.leftLactateExcretedMass_mg = excretionRate_mg_Per_s * m_dt;
      } else {
        //Right
        m_SubstanceTransport.rightLactateExcretedMass_mg = excretionRate_mg_Per_s * m_dt;
      }
    }
  }

  sub.GetClearance().GetRenalExcretionRate().SetValue(totalExcretionRate_mg_Per_s, MassPerTimeUnit::mg_Per_s);

  double plasmaConcentration_mg_Per_mL = m_aorta->GetSubstanceQuantity(sub)->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
  double patientWeight_kg = m_patient->GetWeight(MassUnit::kg);

  if (totalExcretionRate_mg_Per_s <= 0.0 || patientWeight_kg <= 0.0) {
    sub.GetClearance().GetRenalClearance().SetValue(0.0, VolumePerTimeMassUnit::mL_Per_s_kg);
  } else
    sub.GetClearance().GetRenalClearance().SetValue(totalExcretionRate_mg_Per_s / plasmaConcentration_mg_Per_mL / patientWeight_kg, VolumePerTimeMassUnit::mL_Per_s_kg);

  //Set substance compartment effects
  //Gluconeogenesis calculates it for Lactate later
  if (&sub != m_lactate) {
    SELiquidSubstanceQuantity* leftKidneySubQ = m_data.GetCompartments().GetIntracellularFluid(*m_leftKidneyTissue).GetSubstanceQuantity(sub);
    SELiquidSubstanceQuantity* rightKidneySubQ = m_data.GetCompartments().GetIntracellularFluid(*m_rightKidneyTissue).GetSubstanceQuantity(sub);

    double singleExcreted_mg = totalExcretionRate_mg_Per_s * m_dt * 0.5; // We are assuming the kindneys are doing the same amount of work
    leftKidneySubQ->GetMassExcreted().IncrementValue(singleExcreted_mg, MassUnit::mg);
    leftKidneySubQ->GetMassCleared().IncrementValue(singleExcreted_mg, MassUnit::mg);
    rightKidneySubQ->GetMassExcreted().IncrementValue(singleExcreted_mg, MassUnit::mg);
    rightKidneySubQ->GetMassCleared().IncrementValue(singleExcreted_mg, MassUnit::mg);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Clears substances from the body
///
/// \details
/// This function calculates how much of each substance to remove via the kidneys. Drugs are removed from
/// the tissue compartment in keeping with the PK methodology, while the remaining substances are cleared
/// directly from the vascular compartments. The removed mass of the substance is then added to the
/// bladder compartment and the total body mass of the substance is adjusted accordingly.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateAutomaticClearance(SESubstance& sub)
{
  double patientWeight_kg = m_patient->GetWeight(MassUnit::kg);
  double renalVolumeCleared_mL = 0.0;

  SESubstanceClearance& clearance = sub.GetClearance();

  if (clearance.GetRenalClearance().IsZero())
    return; //nothing to do

  //Renal Volume Cleared - Clearance happens through the renal system
  renalVolumeCleared_mL = (clearance.GetRenalClearance().GetValue(VolumePerTimeMassUnit::mL_Per_s_kg) * patientWeight_kg * m_dt) / 2;

  double massCleared_ug = 0.0;

  //Right Kidney Clearance
  m_data.GetSubstances().CalculateGenericClearance(renalVolumeCleared_mL, *m_rightKidneyTissue, sub, &m_spCleared);
  massCleared_ug += m_spCleared.GetValue(MassUnit::ug);

  //Left Kidney Clearance
  m_data.GetSubstances().CalculateGenericClearance(renalVolumeCleared_mL, *m_leftKidneyTissue, sub, &m_spCleared);
  massCleared_ug += m_spCleared.GetValue(MassUnit::ug);

  //Put it in the bladder
  //We don't have to determine RenalClearance, because we just implement the already determined value
  //We won't set the excretion, since we don't know filtration or reabsorption
  SELiquidSubstanceQuantity* subQ = m_bladder->GetSubstanceQuantity(sub);
  subQ->GetMass().IncrementValue(massCleared_ug, MassUnit::ug);
  subQ->Balance(BalanceLiquidBy::Mass);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Sets all of the system outputs calculated in the renal system
///
/// \details
/// This function calculates the Urine's volume, specific gravity, osmolarity, osmolality and production rate.
/// It also calculates the creatinine clearance rate from the m_CreatinineMassCleared_ug member variable.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateVitalSigns()
{
  //A lot of the system data has already been set elsewhere
  //Set the stuff that was not

  double hematocrit = m_data.GetBloodChemistry().GetHematocrit().GetValue();

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    SEFluidCircuitPath* glomerularOsmoticSourcePath = nullptr;
    SEFluidCircuitPath* bowmansOsmoticSourcePath = nullptr;
    SEFluidCircuitNode* glomerularNode = nullptr;
    SEFluidCircuitNode* bowmansNode = nullptr;
    SEFluidCircuitPath* filterResistancePath = nullptr;
    SEFluidCircuitPath* peritubularOsmoticSourcePath = nullptr;
    SEFluidCircuitPath* tubulesOsmoticSourcePath = nullptr;
    SEFluidCircuitNode* peritubularNode = nullptr;
    SEFluidCircuitNode* tubulesNode = nullptr;
    SEFluidCircuitPath* reabsorptionResistancePath = nullptr;
    SEFluidCircuitPath* afferentArteriolePath = nullptr;
    SEFluidCircuitPath* efferentArteriolePath = nullptr;
    SEFluidCircuitNode* netGlomerularCapillariesNode = nullptr;
    SEFluidCircuitNode* netBowmansCapsulesNode = nullptr;
    SEFluidCircuitNode* netPeritubularNode = nullptr;
    SEFluidCircuitNode* netTubulesNode = nullptr;

    if (kidney == 0) {
      //LEFT
      filterResistancePath = m_leftGlomerularFilterResistancePath;
      glomerularOsmoticSourcePath = m_leftPeritubularOsmoticSourcePath;
      bowmansOsmoticSourcePath = m_leftBowmansOsmoticSourcePath;
      glomerularNode = m_leftGlomerularNode;
      bowmansNode = m_leftBowmansNode;
      netGlomerularCapillariesNode = m_leftNetGlomerularCapillariesNode;
      netBowmansCapsulesNode = m_leftNetBowmansCapsulesNode;
      netPeritubularNode = m_leftNetPeritubularCapillariesNode;
      netTubulesNode = m_leftNetTubulesNode;
      reabsorptionResistancePath = m_leftReabsorptionResistancePath;
      peritubularOsmoticSourcePath = m_leftPeritubularOsmoticSourcePath;
      tubulesOsmoticSourcePath = m_leftTubulesOsmoticSourcePath;
      peritubularNode = m_leftPeritubularNode;
      tubulesNode = m_leftTubulesNode;
      afferentArteriolePath = m_leftAfferentArteriolePath;
      efferentArteriolePath = m_leftEfferentArteriolePath;

      GetLeftAfferentArterioleResistance().Set(afferentArteriolePath->GetNextResistance());
      GetLeftEfferentArterioleResistance().Set(efferentArteriolePath->GetNextResistance());

      GetLeftBowmansCapsulesHydrostaticPressure().Set(bowmansNode->GetNextPressure());
      GetLeftBowmansCapsulesOsmoticPressure().Set(bowmansOsmoticSourcePath->GetNextPressureSource());
      GetLeftGlomerularCapillariesHydrostaticPressure().Set(glomerularNode->GetNextPressure());
      GetLeftGlomerularCapillariesOsmoticPressure().Set(glomerularOsmoticSourcePath->GetNextPressureSource());

      GetLeftGlomerularFiltrationRate().Set(filterResistancePath->GetNextFlow());
      double pressureDiff_mmHg = netGlomerularCapillariesNode->GetNextPressure().GetValue(PressureUnit::mmHg) - netBowmansCapsulesNode->GetNextPressure().GetValue(PressureUnit::mmHg);
      GetLeftNetFiltrationPressure().SetValue(pressureDiff_mmHg, PressureUnit::mmHg);
      double filtrationCoefficient = 0.0;
      if (GetLeftNetFiltrationPressure().GetValue(PressureUnit::mmHg) != 0)
        filtrationCoefficient = GetLeftGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s) / GetLeftNetFiltrationPressure().GetValue(PressureUnit::mmHg);

      GetLeftGlomerularFiltrationCoefficient().SetValue(filtrationCoefficient, VolumePerTimePressureUnit::mL_Per_s_mmHg);

      GetLeftPeritubularCapillariesHydrostaticPressure().Set(peritubularNode->GetNextPressure());
      GetLeftPeritubularCapillariesOsmoticPressure().Set(peritubularOsmoticSourcePath->GetNextPressureSource());
      GetLeftTubularHydrostaticPressure().Set(tubulesNode->GetNextPressure());
      GetLeftTubularOsmoticPressure().Set(tubulesOsmoticSourcePath->GetNextPressureSource());

      GetLeftReabsorptionRate().Set(reabsorptionResistancePath->GetNextFlow());
      pressureDiff_mmHg = netTubulesNode->GetNextPressure().GetValue(PressureUnit::mmHg) - netPeritubularNode->GetNextPressure().GetValue(PressureUnit::mmHg);
      GetLeftNetReabsorptionPressure().SetValue(pressureDiff_mmHg, PressureUnit::mmHg);
      filtrationCoefficient = 0.0;
      if (GetLeftNetReabsorptionPressure().GetValue(PressureUnit::mmHg) != 0.0)
        filtrationCoefficient = GetLeftReabsorptionRate().GetValue(VolumePerTimeUnit::mL_Per_s) / GetLeftNetReabsorptionPressure().GetValue(PressureUnit::mmHg);
      GetLeftReabsorptionFiltrationCoefficient().SetValue(filtrationCoefficient, VolumePerTimePressureUnit::mL_Per_s_mmHg);

      afferentArteriolePath = m_leftAfferentArteriolePath;
      double renalBloodFlow_mL_Per_s = afferentArteriolePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
      double renalPlasmaFlow_mL_Per_s = renalBloodFlow_mL_Per_s * (1.0 - hematocrit);

      double filtrationFraction = 0.0;
      if (renalPlasmaFlow_mL_Per_s != 0)
        filtrationFraction = GetLeftGlomerularFiltrationRate(VolumePerTimeUnit::mL_Per_s) / renalPlasmaFlow_mL_Per_s;

      GetLeftFiltrationFraction().SetValue(filtrationFraction);
    } else {
      //RIGHT
      filterResistancePath = m_rightGlomerularFilterResistancePath;
      glomerularOsmoticSourcePath = m_rightPeritubularOsmoticSourcePath;
      bowmansOsmoticSourcePath = m_rightBowmansOsmoticSourcePath;
      glomerularNode = m_rightGlomerularNode;
      bowmansNode = m_rightBowmansNode;
      netGlomerularCapillariesNode = m_rightNetGlomerularCapillariesNode;
      netBowmansCapsulesNode = m_rightNetBowmansCapsulesNode;
      netPeritubularNode = m_rightNetPeritubularCapillariesNode;
      netTubulesNode = m_rightNetTubulesNode;
      reabsorptionResistancePath = m_rightReabsorptionResistancePath;
      peritubularOsmoticSourcePath = m_rightPeritubularOsmoticSourcePath;
      tubulesOsmoticSourcePath = m_rightTubulesOsmoticSourcePath;
      peritubularNode = m_rightPeritubularNode;
      tubulesNode = m_rightTubulesNode;
      afferentArteriolePath = m_rightAfferentArteriolePath;
      efferentArteriolePath = m_rightEfferentArteriolePath;

      GetRightAfferentArterioleResistance().Set(afferentArteriolePath->GetNextResistance());
      GetRightEfferentArterioleResistance().Set(efferentArteriolePath->GetNextResistance());

      GetRightBowmansCapsulesHydrostaticPressure().Set(bowmansNode->GetNextPressure());
      GetRightBowmansCapsulesOsmoticPressure().Set(bowmansOsmoticSourcePath->GetNextPressureSource());
      GetRightGlomerularCapillariesHydrostaticPressure().Set(glomerularNode->GetNextPressure());
      GetRightGlomerularCapillariesOsmoticPressure().Set(glomerularOsmoticSourcePath->GetNextPressureSource());

      GetRightGlomerularFiltrationRate().Set(filterResistancePath->GetNextFlow());
      double pressureDiff_mmHg = netGlomerularCapillariesNode->GetNextPressure().GetValue(PressureUnit::mmHg) - netBowmansCapsulesNode->GetNextPressure().GetValue(PressureUnit::mmHg);
      GetRightNetFiltrationPressure().SetValue(pressureDiff_mmHg, PressureUnit::mmHg);
      double filtrationCoefficient = 0.0;
      if (GetRightNetFiltrationPressure().GetValue(PressureUnit::mmHg) != 0.0)
        filtrationCoefficient = GetRightGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s) / GetRightNetFiltrationPressure().GetValue(PressureUnit::mmHg);
      GetRightGlomerularFiltrationCoefficient().SetValue(filtrationCoefficient, VolumePerTimePressureUnit::mL_Per_s_mmHg);

      GetRightPeritubularCapillariesHydrostaticPressure().Set(peritubularNode->GetNextPressure());
      GetRightPeritubularCapillariesOsmoticPressure().Set(peritubularOsmoticSourcePath->GetNextPressureSource());
      GetRightTubularHydrostaticPressure().Set(tubulesNode->GetNextPressure());
      GetRightTubularOsmoticPressure().Set(tubulesOsmoticSourcePath->GetNextPressureSource());

      GetRightReabsorptionRate().Set(reabsorptionResistancePath->GetNextFlow());
      pressureDiff_mmHg = netTubulesNode->GetNextPressure().GetValue(PressureUnit::mmHg) - netPeritubularNode->GetNextPressure().GetValue(PressureUnit::mmHg);
      GetRightNetReabsorptionPressure().SetValue(pressureDiff_mmHg, PressureUnit::mmHg);

      filtrationCoefficient = 0.0;
      if (GetRightNetReabsorptionPressure().GetValue(PressureUnit::mmHg) != 0.0)
        filtrationCoefficient = GetRightReabsorptionRate().GetValue(VolumePerTimeUnit::mL_Per_s) / GetRightNetReabsorptionPressure().GetValue(PressureUnit::mmHg);
      GetRightReabsorptionFiltrationCoefficient().SetValue(filtrationCoefficient, VolumePerTimePressureUnit::mL_Per_s_mmHg);

      afferentArteriolePath = m_rightAfferentArteriolePath;
      double renalBloodFlow_mL_Per_s = afferentArteriolePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
      double renalPlasmaFlow_mL_Per_s = renalBloodFlow_mL_Per_s * (1.0 - hematocrit);
      double filtrationFraction = 0.0;
      if (renalPlasmaFlow_mL_Per_s != 0.0)
        filtrationFraction = GetRightGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s) / renalPlasmaFlow_mL_Per_s;
      GetRightFiltrationFraction().SetValue(filtrationFraction);
    }
  }

  GetGlomerularFiltrationRate().SetValue(GetLeftGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s) + GetRightGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s), VolumePerTimeUnit::mL_Per_s);

  double renalBloodFlow_mL_Per_s = m_rightAfferentArteriolePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s) + m_leftAfferentArteriolePath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
  GetRenalBloodFlow().SetValue(renalBloodFlow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  GetRenalPlasmaFlow().SetValue(renalBloodFlow_mL_Per_s * (1.0 - hematocrit), VolumePerTimeUnit::mL_Per_s);

  double pressureDiff_mmHg = m_aorta->GetPressure(PressureUnit::mmHg) - m_venaCava->GetPressure(PressureUnit::mmHg);
  if (renalBloodFlow_mL_Per_s != 0.0)
    GetRenalVascularResistance().SetValue(pressureDiff_mmHg / renalBloodFlow_mL_Per_s, FlowResistanceUnit::mmHg_s_Per_mL);
  else
    GetRenalVascularResistance().SetValue(m_CVOpenResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

  //Do the urine specific values
  GetUrineVolume().SetValue(m_bladderNode->GetNextVolume().GetValue(VolumeUnit::mL), VolumeUnit::mL);
  double urineProductionRate_mL_Per_s = m_leftUreterPath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s) + m_rightUreterPath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
  GetUrineProductionRate().SetValue(urineProductionRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  // Urine specific gravity is calculated by dividing the mass of all the substances in a fluid & the fluid itself by the weight of just the fluid
  SEScalarMass substanceMass;
  for (SELiquidSubstanceQuantity* subQ : m_bladder->GetSubstanceQuantities()) {
    if (subQ->HasMass())
      substanceMass.Increment(subQ->GetMass());
  }

  //increment water mass onto substance mass to get total mass:
  GeneralMath::CalculateSpecificGravity(substanceMass, GetUrineVolume(), GetUrineSpecificGravity());

  // Urine osmolality is the osmotic pressure of sodium, glucose and urea over the weight of the fluid

  GeneralMath::CalculateOsmolality(m_bladderSodium->GetMolarity(), m_bladderPotassium->GetMolarity(), m_bladderGlucose->GetMolarity(), m_bladderUrea->GetMolarity(), GetUrineSpecificGravity(), GetUrineOsmolality());
  GeneralMath::CalculateOsmolarity(m_bladderSodium->GetMolarity(), m_bladderPotassium->GetMolarity(), m_bladderGlucose->GetMolarity(), m_bladderUrea->GetMolarity(), GetUrineOsmolarity());

  double urineUreaConcentration_g_Per_L = m_bladderUrea->GetConcentration(MassPerVolumeUnit::g_Per_L);
  //2.14 = MW Urea(60) / MW N2 (2*14)
  GetUrineUreaNitrogenConcentration().SetValue(urineUreaConcentration_g_Per_L / 2.14, MassPerVolumeUnit::g_Per_L);

  double filtrationFraction = 0.0;
  if (GetRenalPlasmaFlow().GetValue(VolumePerTimeUnit::mL_Per_s) != 0.0)
    filtrationFraction = GetGlomerularFiltrationRate().GetValue(VolumePerTimeUnit::mL_Per_s) / GetRenalPlasmaFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
  GetFiltrationFraction().SetValue(filtrationFraction);

  // Do running averages for events
  // Only calculate the running average when not urinating, since the production rate shuts off during urination
  // This will keep the events from freaking out when urinating
  if (m_Urinating == false) {
    m_urineProductionRate_mL_Per_min_runningAvg.Sample(Convert(urineProductionRate_mL_Per_s, VolumePerTimeUnit::mL_Per_s, VolumePerTimeUnit::mL_Per_min));
    m_urineOsmolarity_mOsm_Per_L_runningAvg.Sample(GetUrineOsmolarity(OsmolarityUnit::mOsm_Per_L));
    m_sodiumExcretionRate_mg_Per_min_runningAvg.Sample(m_sodium->GetClearance().GetRenalExcretionRate(MassPerTimeUnit::mg_Per_min));
  }

  //Only check these once each cardiac cycle (using running average for entire cycle)
  //Otherwise, they could turn on and off like crazy as the flows fluctuate throughout the cycle
  if (m_data.GetPatient().IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle) && m_Urinating == false) {
    if (m_data.GetState() > EngineState::InitialStabilization) { // Don't throw events if we are initializing
      //Handle Events
      /// \cite lahav1992intermittent
      /// 2.5 mL/min
      if (m_urineProductionRate_mL_Per_min_runningAvg.Value() > 2.5) {
        /// \event Patient: Diuresis. Occurs when the urine production rate double to around 2.5 ml/min. \cite lahav1992intermittent
        m_patient->SetEvent(CDM::enumPatientEvent::Diuresis, true, m_data.GetSimulationTime());
      } else if (m_urineProductionRate_mL_Per_min_runningAvg.Value() < 1.0) {
        /// \event Patient: Ends when the urine production rate falls below 1.0 mL/min (near normal urine production). \cite lahav1992intermittent
        m_patient->SetEvent(CDM::enumPatientEvent::Diuresis, false, m_data.GetSimulationTime());
      }

      /// \cite valtin1995renal
      /// p. 116
      /// urine osmolarity must be hyperosmotic relative to plasma and urine production rate must be less than 0.5 mL/min
      if (m_urineProductionRate_mL_Per_min_runningAvg.Value() < 0.5 && m_urineOsmolarity_mOsm_Per_L_runningAvg.Value() > 280) {
        /// \event Patient: Antidiuresis occurs when urine production rate is less than 0.5 mL/min and the urine osmolarity is hyperosmotic to the plasma \cite valtin1995renal
        m_patient->SetEvent(CDM::enumPatientEvent::Antidiuresis, true, m_data.GetSimulationTime());
      } else if ((m_urineProductionRate_mL_Per_min_runningAvg.Value() > 0.55 || m_urineOsmolarity_mOsm_Per_L_runningAvg.Value() < 275)) {
        /// \event Patient: Antidiuresis. Ends when urine production rate rises back above 0.55 mL/min or the urine osmolarity falls below that of the plasma \cite valtin1995renal
        m_patient->SetEvent(CDM::enumPatientEvent::Antidiuresis, false, m_data.GetSimulationTime());
      }

      /// \cite Zager1988HypoperfusionRate
      /// Computing percent decrease as (1-1.6/11.2)*100 = 85 percent decrease or 15% total flow (using 20ml/s as "normal" value, below 3ml/s):
      if (renalBloodFlow_mL_Per_s < 3.0) {
        /// \event Patient: hypoperfusion occurs when renal blood flow decreases below 3 ml/s
        m_patient->SetEvent(CDM::enumPatientEvent::RenalHypoperfusion, true, m_data.GetSimulationTime());
      } else if (renalBloodFlow_mL_Per_s > 4.0) {
        /// \event Patient: hypoperfusion ends when blood flow recovers above 4 ml/s
        m_patient->SetEvent(CDM::enumPatientEvent::RenalHypoperfusion, false, m_data.GetSimulationTime());
      }

      /// \cite moss2014hormonal
      ///  1:6 ratio for sodium excretion in pressure natriuresis in rats, validation for sodium excretion = 2.4 mg/min
      //sub->GetClearance()->GetRenalExcretionRate().SetValue(excretionRate_mg_Per_s, MassPerTimeUnit::mg_Per_s);

      if (m_sodiumExcretionRate_mg_Per_min_runningAvg.Value() > 14.4) {
        /// \event Patient: Natriuresis. Occurs when the sodium excretion rate rises above 14.4 mg/min \cite moss2013hormonal
        m_patient->SetEvent(CDM::enumPatientEvent::Natriuresis, true, m_data.GetSimulationTime());
      } else if (m_sodiumExcretionRate_mg_Per_min_runningAvg.Value() < 14.0) {
        /// \event Patient: Ends when the sodium excretion rate falls below 14.0 mg/min \cite moss2013hormonal
        m_patient->SetEvent(CDM::enumPatientEvent::Natriuresis, false, m_data.GetSimulationTime());
      }

      if (m_data.GetActions().GetPatientActions().HasSepsis()) {
        double systolicBP = m_data.GetCardiovascular().GetSystolicArterialPressure(PressureUnit::mmHg);
        if (systolicBP <= 100.0 && m_urineProductionRate_mL_Per_min_runningAvg.Value() <= 0.5) {
          m_patient->SetEvent(CDM::enumPatientEvent::SevereSepsis, true, m_data.GetSimulationTime());
        }
        if (m_patient->IsEventActive(CDM::enumPatientEvent::SevereSepsis) && m_urineProductionRate_mL_Per_min_runningAvg.Value() >= 0.55 && systolicBP > 90.0) {
          m_patient->SetEvent(CDM::enumPatientEvent::SevereSepsis, false, m_data.GetSimulationTime());
        }
      }
    }

    //reset at start of cardiac cycle
    m_urineProductionRate_mL_Per_min_runningAvg.Reset();
    m_urineOsmolarity_mOsm_Per_L_runningAvg.Reset();
    m_sodiumExcretionRate_mg_Per_min_runningAvg.Reset();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Empties the bladder of fluid and substances.
///
/// \details
/// Urination empties the bladder.This can be called as an action or will automatically occur if the bladder
/// volume exceeds the maximum volume. Currently the outflow portion is disabled due to a bug in the generic
/// transporter. This has been replaced by setting the bladder volume and substance quantities to 1 mL &
/// 0 ug respectively.
//--------------------------------------------------------------------------------------------------
void Renal::Urinate()
{
  double bladderMaxVolume_mL = 400.0;
  GetUrinationRate().SetValue(0.0, VolumePerTimeUnit::mL_Per_min);

  //Check and see if the bladder is overfull or if there is an action called
  if (m_bladderNode->GetNextVolume().GetValue(VolumeUnit::mL) > bladderMaxVolume_mL) {
    /// \event Patient: FunctionalIncontinence: The patient's bladder has reached a maximum
    m_patient->SetEvent(CDM::enumPatientEvent::FunctionalIncontinence, true, m_data.GetSimulationTime());
    m_Urinating = true;
  }

  if (m_data.GetActions().GetPatientActions().HasUrinate()) {
    m_data.GetActions().GetPatientActions().RemoveUrinate();
    m_Urinating = true;
  }

  //Now deal with the action
  if (m_Urinating) {
    ////Stop urinating when the bladder volume drops below 1.0 mL
    //if (m_bladderNode->GetNextVolume().GetValue(VolumeUnit::mL) < 1.0) {
    //  m_Urinating = false;
    //  //The urethra resistances will use the baselines value of an open switch to stop the flow

    //  //Turn off the event
    //  if (m_patient->IsEventActive(CDM::enumPatientEvent::FunctionalIncontinence)) {
    //    m_patient->SetEvent(CDM::enumPatientEvent::FunctionalIncontinence, false, m_data.GetSimulationTime());
    //  }
    //} else {
    //  //Prevent anything from leaving except for what's in the bladder
    //  m_leftUreterPath->GetNextResistance().SetValue(m_defaultOpenResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    //  m_rightUreterPath->GetNextResistance().SetValue(m_defaultOpenResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    //  //Reduce the urethra resistance to urinate
    //  //Use a urethra resistance based on the validated urination flow
    //  //R = (4 mmHg - 0 mmHg) / 22 mL/s = 0.182
    //  m_urethraPath->GetNextResistance().SetValue(0.01, FlowResistanceUnit::mmHg_s_Per_mL); //0.182
    //}
    m_bladderNode->GetNextVolume().SetValue(1.0, VolumeUnit::mL);
    for (auto sub : m_data.GetSubstances().GetActiveSubstances()) {
      if (m_bladder->GetSubstanceQuantity(*sub)->GetMass(MassUnit::ug) > ZERO_APPROX) {
        m_bladder->GetSubstanceQuantity(*sub)->Balance(BalanceLiquidBy::Concentration);
      }
    }
    m_Urinating = false;

    GetUrinationRate().Set(m_urethraPath->GetNextFlow());
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Manually increments/decrements bladder volume
///
/// \details
/// The current renal model does not use a compliance to represent the bladder. This function therefore
/// sums the inflow and outflow to the bladder node each time step and updates the bladder volume accordingly.
//--------------------------------------------------------------------------------------------------
void Renal::UpdateBladderVolume()
{
  /// \todo Eventually replace this entire thing with a compliance and model peristaltic flow

  //Don't fill the bladder during stabilization
  if (m_data.GetState() != EngineState::Active)
    return;

  //Manually modify the bladder volume based on flow
  //This will work for both filling the bladder and urinating
  double bladderFlow_mL_Per_s = m_bladderToGroundPressurePath->GetNextFlow(VolumePerTimeUnit::mL_Per_s);
  double volumeIncrement_mL = bladderFlow_mL_Per_s * m_dt;
  double bladderVolume_mL = m_bladderNode->GetNextVolume().GetValue(VolumeUnit::mL) + volumeIncrement_mL;

  //Don't let this get below zero during urination
  //The urination action will catch it next time around, so this shouldn't be hit more than once (and likely never)
  bladderVolume_mL = std::max(bladderVolume_mL, 0.0);
  m_bladderNode->GetNextVolume().SetValue(bladderVolume_mL, VolumeUnit::mL);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates and sets osmotic pressure
///
/// \param  albuminConcentration    The concentration of albumin (protein) in the fluid
/// \param  osmoticPressure         The current osmotic pressure of the fluid
///
/// \details
/// The Landis-Pappenheimer equation is used to determine and set the colloid osmotic pressure of a fluid.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateColloidOsmoticPressure(SEScalarMassPerVolume& albuminConcentration, SEScalarPressure& osmoticPressure)
{
  //We're using the Landis-Pappenheimer Equation

  //Assume a typical Albumin to total protein ratio
  double totalProteinConentration_g_Per_dL = 1.6 * albuminConcentration.GetValue(MassPerVolumeUnit::g_Per_dL);
  double osmoticPressure_mmHg = 2.1 * totalProteinConentration_g_Per_dL + 0.16 * std::pow(totalProteinConentration_g_Per_dL, 2.0) + 0.009 * std::pow(totalProteinConentration_g_Per_dL, 3.0);
  osmoticPressure.SetValue(-osmoticPressure_mmHg, PressureUnit::mmHg);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the urinalysis outputs
///
/// \param  u   urinalysis assessment
///
/// \return returns true upon the successful calculation of the urinalysis outputs
///
/// \details
/// This function calculates the outputs requested from the urinalysis assessment. Currently only color,
/// glucose, ketones, specific gravity, blood and protein are supported. Specific gravity is pulled directly
/// from the renal system data and color is derived from the osmolality system data. The others are true/false
/// outputs that are set based on the urine concentrations of various substances.
//--------------------------------------------------------------------------------------------------
bool Renal::CalculateUrinalysis(SEUrinalysis& u)
{
  u.Reset();

  double urineOsm_Per_kg = GetUrineOsmolality(OsmolalityUnit::mOsm_Per_kg);
  if (urineOsm_Per_kg <= 400) // Need cite for this
    u.SetColorResult(CDM::enumUrineColor::PaleYellow);
  else if (urineOsm_Per_kg > 400 && urineOsm_Per_kg <= 750)
    u.SetColorResult(CDM::enumUrineColor::Yellow);
  else
    u.SetColorResult(CDM::enumUrineColor::DarkYellow);

  //u.SetApperanceResult();
  double bladder_glucose_mg_Per_dL = m_bladderGlucose->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_dL);

  if (bladder_glucose_mg_Per_dL >= 100.0) /// \cite roxe1990urinalysis
    u.SetGlucoseResult(CDM::enumPresenceIndicator::Positive);
  else
    u.SetGlucoseResult(CDM::enumPresenceIndicator::Negative);

  if (bladder_glucose_mg_Per_dL >= 5.0) /// \cite roxe1990urinalysis
    u.SetKetoneResult(CDM::enumPresenceIndicator::Positive);
  else
    u.SetKetoneResult(CDM::enumPresenceIndicator::Negative);

  //u.SetBilirubinResult();

  u.GetSpecificGravityResult().Set(GetUrineSpecificGravity());
  if (bladder_glucose_mg_Per_dL > 0.15) /// \cite roxe1990urinalysis
    u.SetBloodResult(CDM::enumPresenceIndicator::Positive);
  else
    u.SetBloodResult(CDM::enumPresenceIndicator::Negative);

  //u.GetPHResult().Set();

  if (bladder_glucose_mg_Per_dL > 30.0) /// \cite roxe1990urinalysis
    u.SetProteinResult(CDM::enumPresenceIndicator::Positive);
  else
    u.SetProteinResult(CDM::enumPresenceIndicator::Negative);

  //u.SetUrobilinogen();
  //u.SetNitrite();
  //u.SetLeukocyteEsterase();

  // We do not support Microscopic analysis at this time

  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies permeability of the tubules
///
/// \details
/// This function modifies the permeability of the tubules based on plasma sodium. The plasma sodium
/// is representative of the plasma osmolarity. As the concentration of sodium rises, the osmoreceptors
/// attempt to reabsorb more fluid to offset the rising sodium. The inverse occurs when the sodium concentration
/// is low.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateOsmoreceptorFeedback()
{
  //Modify the reabsorption permeability based on plasma Sodium concentration
  //Note: the permeability feedback due to local pressure change has already occurred

  //Tuning parameters
  double sodiumSensitivity = 10.0;

  double permeability_mL_Per_s_Per_mmHg_Per_m2 = 0.0;

  ///\todo get the aorta osmolarity instead of sodium concentration
  double sodiumConcentration_mg_Per_mL = m_data.GetSubstances().GetSodium().GetBloodConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);
  sodiumConcentration_mg_Per_mL = m_sodiumConcentration_mg_Per_mL_runningAvg.Sample(sodiumConcentration_mg_Per_mL);

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

      m_leftReabsorptionPermeabilityModificationFactor = std::pow(sodiumConcentration_mg_Per_mL, sodiumSensitivity) / std::pow(m_sodiumPlasmaConcentrationSetpoint_mg_Per_mL, sodiumSensitivity);

      if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        permeability_mL_Per_s_Per_mmHg_Per_m2 *= m_leftReabsorptionPermeabilityModificationFactor;

        //Modify reabsorption resistance
        GetLeftTubularReabsorptionFluidPermeability().SetValue(permeability_mL_Per_s_Per_mmHg_Per_m2, VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      }
    } else {
      //RIGHT
      permeability_mL_Per_s_Per_mmHg_Per_m2 = GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

      m_rightReabsorptionPermeabilityModificationFactor = std::pow(sodiumConcentration_mg_Per_mL, sodiumSensitivity) / std::pow(m_sodiumPlasmaConcentrationSetpoint_mg_Per_mL, sodiumSensitivity);
      if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        permeability_mL_Per_s_Per_mmHg_Per_m2 *= m_rightReabsorptionPermeabilityModificationFactor;

        //Modify reabsorption resistance
        GetRightTubularReabsorptionFluidPermeability().SetValue(permeability_mL_Per_s_Per_mmHg_Per_m2, VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
      }
    }
  }
  if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_sodiumConcentration_mg_Per_mL_runningAvg.Reset();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies the afferent resistance due to TGF
///
/// \details
/// Updates the afferent resistance as a function of sodium delivery into the tubules node.
/// This function drives the afferent resistance along the
/// proper path as a response to increased or decreased sodium delivery.
//--------------------------------------------------------------------------------------------------
void Renal::CalculateTubuloglomerularFeedback()
{
  //Get substances and appropriate paths and node which will be utilized in this implementation
  SEFluidCircuitPath* tubulesPath = nullptr;
  SEFluidCircuitPath* afferentResistancePath = nullptr;
  SEFluidCircuitNode* renalArteryNode = nullptr;

  //set sodium flow to initially be zero
  double sodiumFlow_mg_Per_s = 0.0;
  double sodiumFlowSetPoint_mg_Per_s = 0.0;
  double sodiumConcentration_mg_Per_mL = 0.0;
  double tubulesFlow_mL_Per_s = 0.0;

  //set max/min afferent resistance to be zero initially
  double maxAfferentResistance_mmHg_s_Per_mL = 0.0;
  double minAfferentResistance_mmHg_s_Per_mL = 0.0;

  //Do it separate for both kidneys
  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      afferentResistancePath = m_leftAfferentArteriolePath;
      tubulesPath = m_leftTubulesPath;

      //set max/min afferent equal to the member variable
      maxAfferentResistance_mmHg_s_Per_mL = m_maxLeftAfferentResistance_mmHg_s_Per_mL;
      minAfferentResistance_mmHg_s_Per_mL = m_minLeftAfferentResistance_mmHg_s_Per_mL;

      //Get the concentration and flow rate on tubules path to compute sodium flow rate
      sodiumConcentration_mg_Per_mL = m_leftTubulesSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);

      if (tubulesPath->HasNextFlow()) {
        tubulesFlow_mL_Per_s = tubulesPath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
      }
      sodiumFlow_mg_Per_s = tubulesFlow_mL_Per_s * sodiumConcentration_mg_Per_mL;

      //On the off chance it's negative (like the first time-step) don't do anything
      if (sodiumFlow_mg_Per_s < 0.0 && m_data.GetState() <= EngineState::InitialStabilization)
        continue;

      //Keep a running average, so the resistance doesn't go crazy
      sodiumFlow_mg_Per_s = m_leftSodiumFlow_mg_Per_s_runningAvg.Sample(sodiumFlow_mg_Per_s);

      // Save off the last set point from initial stabilization for use after stabilization
      if (m_data.GetState() == EngineState::InitialStabilization && m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        //Don't change the resistance - just figure out what it is
        m_leftSodiumFlowSetPoint_mg_Per_s = sodiumFlow_mg_Per_s;
      }
      sodiumFlowSetPoint_mg_Per_s = m_leftSodiumFlowSetPoint_mg_Per_s;
    } else {
      //RIGHT
      afferentResistancePath = m_rightAfferentArteriolePath;
      tubulesPath = m_rightTubulesPath;

      maxAfferentResistance_mmHg_s_Per_mL = m_maxRightAfferentResistance_mmHg_s_Per_mL;
      minAfferentResistance_mmHg_s_Per_mL = m_minRightAfferentResistance_mmHg_s_Per_mL;

      //Get the concentration and flow rate on tubules path to compute sodium flow rate
      sodiumConcentration_mg_Per_mL = m_rightTubulesSodium->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL);

      if (tubulesPath->HasNextFlow()) {
        tubulesFlow_mL_Per_s = tubulesPath->GetNextFlow().GetValue(VolumePerTimeUnit::mL_Per_s);
      }
      sodiumFlow_mg_Per_s = tubulesFlow_mL_Per_s * sodiumConcentration_mg_Per_mL;

      //On the off chance it's negative (like the first time-step) don't do anything
      if (sodiumFlow_mg_Per_s < 0.0 && m_data.GetState() < EngineState::InitialStabilization)
        continue;

      //Keep a running average, so the resistance doesn't go crazy
      sodiumFlow_mg_Per_s = m_rightSodiumFlow_mg_Per_s_runningAvg.Sample(sodiumFlow_mg_Per_s);

      // Save off the last set point from initial stabilization for use after stabilization
      if (m_data.GetState() == EngineState::InitialStabilization && m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        //Don't change the resistance - just figure out what it is
        m_rightSodiumFlowSetPoint_mg_Per_s = sodiumFlow_mg_Per_s;
      }
      sodiumFlowSetPoint_mg_Per_s = m_rightSodiumFlowSetPoint_mg_Per_s;
    }

    if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
      //Us the "current" resistance, to continually drive towards the response we want - the next value is overwritten by the baseline during postprocess
      double currentAfferentResistance_mmHg_s_Per_mL = afferentResistancePath->GetResistance().GetValue(FlowResistanceUnit::mmHg_s_Per_mL);
      double nextAfferentResistance_mmHg_s_Per_mL = afferentResistancePath->GetNextResistance().GetValue(FlowResistanceUnit::mmHg_s_Per_mL);

      //Get the amount off we are from normal
      double sodiumChange = sodiumFlow_mg_Per_s - sodiumFlowSetPoint_mg_Per_s;
      //normalize to reduce how dramatic the changes are (may need to add a tuning factor)
      double sodiumChangeNormal = 0.0;
      //First time through this will be zero
      if (sodiumFlowSetPoint_mg_Per_s > 0.0) {
        sodiumChangeNormal = sodiumChange / sodiumFlowSetPoint_mg_Per_s;
      }

      //create control statement to drive resistance to get desired sodium flow rate, damping needed to get steady flow rate
      double dampingFactor = 0.001;
      if (m_data.GetState() < EngineState::Active) {
        //Make the damping factor higher to get to homeostasis faster
        dampingFactor = 0.005;
      }

      nextAfferentResistance_mmHg_s_Per_mL *= currentAfferentResistance_mmHg_s_Per_mL / nextAfferentResistance_mmHg_s_Per_mL + dampingFactor * sodiumChangeNormal;
      BLIM(nextAfferentResistance_mmHg_s_Per_mL, minAfferentResistance_mmHg_s_Per_mL, maxAfferentResistance_mmHg_s_Per_mL);
      if (kidney == 0) {
        //LEFT
        m_leftAfferentResistance_mmHg_s_Per_mL = nextAfferentResistance_mmHg_s_Per_mL;
      } else {
        //RIGHT
        m_rightAfferentResistance_mmHg_s_Per_mL = nextAfferentResistance_mmHg_s_Per_mL;
      }
    }

    //set the resistance each time step:
    if (kidney == 0) {
      //LEFT
      afferentResistancePath->GetNextResistance().SetValue(m_leftAfferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    } else {
      //RIGHT
      afferentResistancePath->GetNextResistance().SetValue(m_rightAfferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
    }
  }
  //reset sodium flow at start of cardiac cycle
  if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_leftSodiumFlow_mg_Per_s_runningAvg.Reset();
    m_rightSodiumFlow_mg_Per_s_runningAvg.Reset();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Modifies the tubule fluid permeability as a function of arterial pressure
///
/// \details
///  A rise in arterial pressure results in an increase in fluid permeability of the tubules.
///  As arterial pressure rises
///
//--------------------------------------------------------------------------------------------------
void Renal::CalculateFluidPermeability()
{
  //fit paramerters for the third order fit
  double a = 2.00943182e-06;
  double b = -8.09932500e-04;
  double c = 9.37727091e-02;

  double permeability_mL_Per_s_mmHg_m2 = 0.0;
  double leftArterialPressure_mmHg = 0.0;
  double rightArterialPressure_mmHg = 0.0;
  double tubularPermeabilityModifier = 1.0;

  if (m_data.GetDrugs().HasTubularPermeabilityChange())
    tubularPermeabilityModifier -= m_data.GetDrugs().GetTubularPermeabilityChange().GetValue();

  for (unsigned int kidney = 0; kidney < 2; kidney++) {
    if (kidney == 0) {
      //LEFT
      permeability_mL_Per_s_mmHg_m2 = GetLeftTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

      //get the renal arterial pressure:
      leftArterialPressure_mmHg = m_leftRenalArteryNode->GetNextPressure().GetValue(PressureUnit::mmHg);

      //take a sample so that permeability doesn't go crazy:
      leftArterialPressure_mmHg = m_leftRenalArterialPressure_mmHg_runningAvg.Sample(leftArterialPressure_mmHg);

      //compute desired permeability as a function of arterial pressure, else set as baseline
      if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        if (round(leftArterialPressure_mmHg) >= 80.0) {
          permeability_mL_Per_s_mmHg_m2 = a * std::pow(leftArterialPressure_mmHg, 2) + b * leftArterialPressure_mmHg + c;
        } else {
          permeability_mL_Per_s_mmHg_m2 = m_leftReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2;
        }
      }

      //cap permeability to bound tubules reabsorption resistance:
      if (permeability_mL_Per_s_mmHg_m2 < 0.01) {
        permeability_mL_Per_s_mmHg_m2 = 0.01;
      }

      //set the permeability (modifier should only change computed permeability if diuretics are circulating):
      GetLeftTubularReabsorptionFluidPermeability().SetValue(tubularPermeabilityModifier * permeability_mL_Per_s_mmHg_m2, VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
    } else {
      //RIGHT
      permeability_mL_Per_s_mmHg_m2 = GetRightTubularReabsorptionFluidPermeability(VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);

      //get the renal arterial pressure:
      rightArterialPressure_mmHg = m_rightRenalArteryNode->GetNextPressure().GetValue(PressureUnit::mmHg);

      //take a sample so that permeability doesn't go crazy:
      rightArterialPressure_mmHg = m_rightRenalArterialPressure_mmHg_runningAvg.Sample(rightArterialPressure_mmHg);

      //compute desired permeability as a function of arterial pressure, else set as baseline
      if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
        if (round(rightArterialPressure_mmHg) >= 80.0) {
          permeability_mL_Per_s_mmHg_m2 = a * std::pow(rightArterialPressure_mmHg, 2) + b * rightArterialPressure_mmHg + c;
        } else {
          permeability_mL_Per_s_mmHg_m2 = m_rightReabsorptionPermeabilitySetpoint_mL_Per_s_mmHg_m2;
        }
      }

      //cap permeability to bound tubules reabsorption resistance:
      if (permeability_mL_Per_s_mmHg_m2 < 0.01) {
        permeability_mL_Per_s_mmHg_m2 = 0.01;
      }

      //set the permeability:
      GetRightTubularReabsorptionFluidPermeability().SetValue(tubularPermeabilityModifier * permeability_mL_Per_s_mmHg_m2, VolumePerTimePressureAreaUnit::mL_Per_s_mmHg_m2);
    }
  }
  //reset average at start of cardiac cycle
  if (m_patient->IsEventActive(CDM::enumPatientEvent::StartOfCardiacCycle)) {
    m_leftRenalArterialPressure_mmHg_runningAvg.Reset();
    m_rightRenalArterialPressure_mmHg_runningAvg.Reset();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// determine override requirements from user defined inputs
///
/// \details
/// User specified override outputs that are specific to the cardiovascular system are implemented here.
/// If overrides aren't present for this system then this function will not be called during preprocess.
//--------------------------------------------------------------------------------------------------
void Renal::ProcessOverride()
{
  auto override = m_data.GetActions().GetPatientActions().GetOverride();

#ifdef BIOGEARS_USE_OVERRIDE_CONTROL
  OverrideControlLoop();
#endif

  if (override->HasLeftAfferentArterioleResistanceOverride()) {
    GetLeftAfferentArterioleResistance().SetValue(override->GetLeftAfferentArterioleResistanceOverride(FlowResistanceUnit::mmHg_min_Per_mL), FlowResistanceUnit::mmHg_min_Per_mL);
  }
  if (override->HasLeftGlomerularFiltrationRateOverride()) {
    GetLeftGlomerularFiltrationRate().SetValue(override->GetLeftGlomerularFiltrationRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasLeftReaborptionRateOverride()) {
    GetLeftReabsorptionRate().SetValue(override->GetLeftReaborptionRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasRenalBloodFlowOverride()) {
    GetRenalBloodFlow().SetValue(override->GetRenalBloodFlowOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasRenalPlasmaFlowOverride()) {
    GetRenalPlasmaFlow().SetValue(override->GetRenalPlasmaFlowOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasRightAfferentArterioleResistanceOverride()) {
    GetRightAfferentArterioleResistance().SetValue(override->GetRightAfferentArterioleResistanceOverride(FlowResistanceUnit::mmHg_min_Per_mL), FlowResistanceUnit::mmHg_min_Per_mL);
  }
  if (override->HasRightGlomerularFiltrationRateOverride()) {
    GetRightGlomerularFiltrationRate().SetValue(override->GetRightGlomerularFiltrationRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasRightReaborptionRateOverride()) {
    GetRightReabsorptionRate().SetValue(override->GetRightReaborptionRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasUrinationRateOverride()) {
    GetUrinationRate().SetValue(override->GetUrinationRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasUrineProductionRateOverride()) {
    GetUrineProductionRate().SetValue(override->GetUrineProductionRateOverride(VolumePerTimeUnit::mL_Per_min), VolumePerTimeUnit::mL_Per_min);
  }
  if (override->HasUrineOsmolalityOverride()) {
    GetUrineOsmolality().SetValue(override->GetUrineOsmolalityOverride(OsmolalityUnit::mOsm_Per_kg), OsmolalityUnit::mOsm_Per_kg);
  }
  if (override->HasUrineVolumeOverride()) {
    GetUrineVolume().SetValue(override->GetUrineVolumeOverride(VolumeUnit::mL), VolumeUnit::mL);
  }
  if (override->HasUrineUreaNitrogenConcentrationOverride()) {
    GetUrineUreaNitrogenConcentration().SetValue(override->GetUrineUreaNitrogenConcentrationOverride(MassPerVolumeUnit::g_Per_L), MassPerVolumeUnit::g_Per_L);
  }
}

//// Can be turned on or off (for debugging purposes) using the Biogears_USE_OVERRIDE_CONTROL external in CMake
  void Renal::OverrideControlLoop()
  {
    auto override = m_data.GetActions().GetPatientActions().GetOverride();

    constexpr double maxLeftAAROverride = 1.0; //mmHg_min_Per_mL
    constexpr double minLeftAAROverride = 0.0; //mmHg_min_Per_mL
    constexpr double maxLeftGFROverride = 1000.0; //mL/min
    constexpr double minLeftGFROverride = 0.0; //mL/min
    constexpr double maxLeftReabsorRateOverride = 1000.0; //mL/min
    constexpr double minLeftReabsorRateOverride = 0.0; //mL/min
    constexpr double maxRenalBloodFlowOverride = 3000.0; //mL/min
    constexpr double minRenalBloodFlowOverride = 0.0; //mL/min
    constexpr double maxRenalPlasmaFlowOverride = 3000.0; //mL/min
    constexpr double minRenalPlasmaFlowOverride = 0.0; //mL/min
    constexpr double maxRightAAROverride = 1.0; //mmHg_min_Per_mL
    constexpr double minRightAAROverride = 0.0; //mmHg_min_Per_mL
    constexpr double maxRightGFROverride = 1000.0; //mL/min
    constexpr double minRightGFROverride = 0.0; //mL/min
    constexpr double maxRightReabsorRateOverride = 1000.0; //mL/min
    constexpr double minRightReabsorRateOverride = 0.0; //mL/min
    constexpr double maxUrinationRateOverride = 1000.0; //mL/min
    constexpr double minUrinationRateOverride = 0.0; //mL/min
    constexpr double maxUrineProductionOverride = 100.0; //mL/min
    constexpr double minUrineProductionOverride = 0.0; //mL/min
    constexpr double maxUrineOsmolalityOverride = 2000.0; //mOsm/kg
    constexpr double minUrineOsmolalityOverride = 0.0; //mOsm/kg
    constexpr double maxUrineVolumeOverride = 1000.0; // mL
    constexpr double minUrineVolumeOverride = 0.0; // mL
    constexpr double maxUrineUreaNitrogenOverride = 100.0; // g/L
    constexpr double minUrineUreaNitrogenOverride = 0.0; // g/L

    double currentLeftAAROverride = 0.0; //value gets changed in next check
    double currentLeftGFROverride = 0.0; //value gets changed in next check
    double currentLeftReabsorRateOverride = 0.0; //value gets changed in next check
    double currentRenalBloodFlowOverride = 0.0; //value gets changed in next check
    double currentRenalPlasmaFlowOverride = 0.0; //value gets changed in next check
    double currentRightAAROverride = 0.0; //value gets changed in next check
    double currentRightGFROverride = 0.0; //value gets changed in next check
    double currentRightReabsorRateOverride = 0.0; //value gets changed in next check
    double currentUrinationRateOverride = 0.0; //value gets changed in next check
    double currentUrineProductionOverride = 0.0; //value gets changed in next check
    double currentUrineOsmolalityOverride = 0.0; //value gets changed in next check
    double currentUrineVolumeOverride = 0.0; //value gets changed in next check
    double currentUrineUreaNitrogenOverride = 0.0; //value gets changed in next check

    if (override->HasLeftAfferentArterioleResistanceOverride()) {
      currentLeftAAROverride = override->GetLeftAfferentArterioleResistanceOverride(FlowResistanceUnit::mmHg_min_Per_mL);
    }
    if (override->HasLeftGlomerularFiltrationRateOverride()) {
      currentLeftGFROverride = override->GetLeftGlomerularFiltrationRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasLeftReaborptionRateOverride()) {
      currentLeftReabsorRateOverride = override->GetLeftReaborptionRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasRenalBloodFlowOverride()) {
      currentRenalBloodFlowOverride = override->GetRenalBloodFlowOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasRenalPlasmaFlowOverride()) {
      currentRenalPlasmaFlowOverride = override->GetRenalPlasmaFlowOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasRightAfferentArterioleResistanceOverride()) {
      currentRightAAROverride = override->GetRightAfferentArterioleResistanceOverride(FlowResistanceUnit::mmHg_min_Per_mL);
    }
    if (override->HasRightGlomerularFiltrationRateOverride()) {
      currentRightGFROverride = override->GetRightGlomerularFiltrationRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasRightReaborptionRateOverride()) {
      currentRightReabsorRateOverride = override->GetRightReaborptionRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasUrinationRateOverride()) {
      currentUrinationRateOverride = override->GetUrinationRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasUrineProductionRateOverride()) {
      currentUrineProductionOverride = override->GetUrineProductionRateOverride(VolumePerTimeUnit::mL_Per_min);
    }
    if (override->HasUrineOsmolalityOverride()) {
      currentUrineOsmolalityOverride = override->GetUrineOsmolalityOverride(OsmolalityUnit::mOsm_Per_kg);
    }
    if (override->HasUrineVolumeOverride()) {
      currentUrineVolumeOverride = override->GetUrineVolumeOverride(VolumeUnit::mL);
    }
    if (override->HasUrineUreaNitrogenConcentrationOverride()) {
      currentUrineUreaNitrogenOverride = override->GetUrineUreaNitrogenConcentrationOverride(MassPerVolumeUnit::g_Per_L);
    }

    if ((currentLeftAAROverride < minLeftAAROverride || currentLeftAAROverride > maxLeftAAROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Left Afferent Arteriole Resistance Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentLeftGFROverride < minLeftGFROverride || currentLeftGFROverride > maxLeftGFROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Left Glomerular Filtration Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentLeftReabsorRateOverride < minLeftReabsorRateOverride || currentLeftReabsorRateOverride > maxLeftReabsorRateOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Left Reabsorption Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentRenalBloodFlowOverride < minRenalBloodFlowOverride || currentRenalBloodFlowOverride > maxRenalBloodFlowOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Renal Blood Flow (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentRenalPlasmaFlowOverride < minRenalPlasmaFlowOverride || currentRenalPlasmaFlowOverride > maxRenalPlasmaFlowOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Renal Plasma Flow Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentRightAAROverride < minRightAAROverride || currentRightAAROverride > maxRightAAROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Right Afferent Arteriole Resistance Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentRightGFROverride < minRightGFROverride || currentRightGFROverride > maxRightGFROverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Right Glomerular Filtration Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentRightReabsorRateOverride < minRightReabsorRateOverride || currentRightReabsorRateOverride > maxRightReabsorRateOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Right Reabsorption Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentUrinationRateOverride < minUrinationRateOverride || currentUrinationRateOverride > maxUrinationRateOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Urination Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentUrineProductionOverride < minUrineProductionOverride || currentUrineProductionOverride > maxUrineProductionOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Urine Production Rate Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentUrineOsmolalityOverride < minUrineOsmolalityOverride || currentUrineOsmolalityOverride > maxUrineOsmolalityOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Urine Osmolality Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentUrineVolumeOverride < minUrineVolumeOverride || currentUrineVolumeOverride > maxUrineVolumeOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Urine Volume Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    if ((currentUrineUreaNitrogenOverride < minUrineUreaNitrogenOverride || currentUrineUreaNitrogenOverride > maxUrineUreaNitrogenOverride) && (override->GetOverrideConformance() == CDM::enumOnOff::On)) {
      m_ss << "Urine Urea Nitrogen Concentration Override (Renal) set outside of bounds of validated parameter override. BioGears is no longer conformant.";
      Info(m_ss);
      override->SetOverrideConformance(CDM::enumOnOff::Off);
    }
    return;
  }
}