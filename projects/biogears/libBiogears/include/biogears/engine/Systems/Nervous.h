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

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>
#include <biogears/exports.h>
#include <biogears/cdm/utils/RunningAverage.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class SESubstance;
class SEPsychomotorVigilanceTask;
class BioGears;
class SEPatient;
/**
 * @brief 
 * The nervous class holds models of the peripheral and central nervous system. Currently, on the baroreceptor reflex is modeled.
 */
class BIOGEARS_API Nervous : public SENervousSystem, public BioGearsSystem {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<Nervous>;
  Nervous(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~Nervous() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "Nervous"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override;

  // Set members to a stable homeostatic state
  void Initialize() override;

  // Load a state
  virtual bool Load(const CDM::BioGearsNervousSystemData& in);
  virtual CDM::BioGearsNervousSystemData* Unload() const override;

  //Assessments
  SEScalar& CalculateCentralNervousSOFA();

protected:
  virtual void Unload(CDM::BioGearsNervousSystemData& data) const;

  // Set pointers and other member variables common to both homeostatic initialization and loading a state
  void SetUp() override;

public:
  void AtSteadyState() override;
  void PreProcess() override;
  void Process() override;
  void PostProcess() override;
  bool CalculatePsychomotorVigilanceTask(SEPsychomotorVigilanceTask& pvt);


protected:
  void BaroreceptorFeedback();
  void CheckPainStimulus();
  void CheckNervousStatus();
  void ChemoreceptorFeedback();
  void SetPupilEffects();
  void CalculateSleepEffects();
  void UpdateSleepState();
  void AfferentResponse();
  void CentralSignalProcess();
  void EfferentResponse();
  void LocalAutoregulation();

  // Serializable member variables (Set in Initialize and in schema)
  double m_AfferentChemoreceptor_Hz; //*AC
  double m_AfferentPulmonaryStretchReceptor_Hz; //*AP
  double m_AorticBaroreceptorStrain;
  double m_AttentionLapses;
  double m_ArterialOxygenBaseline_mmHg;
  double m_ArterialCarbonDioxideBaseline_mmHg;
  double m_BaroreceptorOperatingPoint_mmHg;
  double m_BiologicalDebt;
  double m_CardiopulmonaryInputBaseline_mmHg;
  double m_CardiopulmonaryInput_mmHg;
  double m_CarotidBaroreceptorStrain;
  double m_CentralFrequencyDelta_Per_min;
  double m_CentralPressureDelta_cmH2O;
  std::vector<double> m_CerebralArteriesEffectors_Large;
  std::vector<double> m_CerebralArteriesEffectors_Small;
  double m_CerebralOxygenSaturationBaseline;
  double m_CerebralPerfusionPressureBaseline_mmHg;
  double m_ChemoreceptorFiringRateSetPoint_Hz;
  double m_CerebralBloodFlowBaseline_mL_Per_s;
  double m_CerebralBloodFlowInput_mL_Per_s;
  double m_ComplianceModifier;
  double m_HeartElastanceModifier;
  double m_HeartOxygenBaseline;
  double m_HeartRateModifierSympathetic;
  double m_HeartRateModifierVagal;
  double m_HypercapniaThresholdHeart;
  double m_HypercapniaThresholdPeripheral;
  double m_HypoxiaThresholdHeart;
  double m_HypoxiaThresholdPeripheral;
  double m_MeanLungVolume_L;
  double m_MuscleOxygenBaseline;
  double m_OxygenAutoregulatorHeart;
  double m_OxygenAutoregulatorMuscle;
  double m_PeripheralBloodGasInteractionBaseline_Hz;
  double m_PeripheralFrequencyDelta_Per_min;
  double m_PeripheralPressureDelta_cmH2O;
  double m_ReactionTime_s;
  double m_ResistanceModifierExtrasplanchnic;
  double m_ResistanceModifierMuscle;
  double m_ResistanceModifierSplanchnic;
  double m_SympatheticPeripheralSignalBaseline_Hz;
  double m_SympatheticSinoatrialSignalBaseline_Hz;
  double m_SympatheticPeripheralSignalFatigue;
  double m_TiredTime_hr;
  double m_VagalSignalBaseline_Hz;



  // Stateless member variable (Set in SetUp())
  double m_AfferentBaroreceptorAortic_Hz; //*ABA
  double m_AfferentBaroreceptorCarotid_Hz; //*ABC
  double m_AfferentCardiopulmonary_Hz;  //*ACP
  double m_DrugRespirationEffects;
  bool m_FeedbackActive;
  double m_dt_s;
  SESubstance* m_Succinylcholine;
  SESubstance* m_Sarin;
  SEPatient* m_Patient;
  bool m_blockActive;
  double m_painStimulusDuration_s;
  double m_painVASDuration_s;
  double m_painVAS;
  double m_painStimulus;
  double m_SympatheticSinoatrialSignal_Hz;
  double m_SympatheticPeripheralSignal_Hz;
  double m_VagalSignal_Hz;

};
}