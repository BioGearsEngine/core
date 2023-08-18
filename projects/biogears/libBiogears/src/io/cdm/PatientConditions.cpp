#include "PatientConditions.h"

#include "EnvironmentConditions.h"
#include "Property.h"
#include "Scenario.h"

#include "biogears/cdm/system/environment/conditions/SEInitialEnvironment.h"
#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/patient/conditions/SEPatientCondition.h>
#include <biogears/cdm/patient/conditions/SEStarvation.h>

#include <biogears/cdm/scenario/SECondition.h>

#include <biogears/cdm/properties/SEProperties.h>

#include "biogears/cdm/substance/SESubstanceManager.h"

#define PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(paramName, typeName)           \
  if (auto typeName = dynamic_cast<SE##typeName const*>(paramName); typeName) { \
    auto typeName##Data = std::make_unique<CDM::typeName##Data>();              \
    UnMarshall(*typeName, *typeName##Data);                                     \
    return std::move(typeName##Data);                                           \
  }

#define PATIENT_CONDITION_POLYMORPHIC_MARSHALL(paramName, typeName, schema)                        \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::Marshall(*typeName##Data, *typeName);                                                  \
    return std::move(typeName);                                                                    \
  }

namespace biogears {
namespace io {
  // class SEConditionList
  std::vector<std::unique_ptr<SECondition>> PatientConditions::condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances)
  {
    std::vector<std::unique_ptr<SECondition>> r_vec;
    for (const auto condition_data : in.Condition()) {
      r_vec.emplace_back(factory(&condition_data, substances));
    }
    return std::move(r_vec);
  }
  //----------------------------------------------------------------------------------
  std::unique_ptr<SECondition> PatientConditions::factory(const CDM::ConditionData* conditionData, SESubstanceManager& substances)
  {
    // Could speed up case by testing Patient Conditions vs another type
    // But we only have 1 type at this time, and a few conditions to support

    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ChronicAnemia, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ChronicObstructivePulmonaryDisease, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ChronicVentricularSystolicDysfunction, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ChronicPericardialEffusion, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ChronicRenalStenosis, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, Dehydration, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, DiabetesType1, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, DiabetesType2, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, Starvation, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, ImpairedAlveolarExchange, PatientConditions)
    PATIENT_CONDITION_POLYMORPHIC_MARSHALL(conditionData, LobarPneumonia, PatientConditions)

    if (const auto initialEnvironmentData = dynamic_cast<const CDM::InitialEnvironmentData*>(conditionData); initialEnvironmentData) {
      auto initialEnvironmentAction = std::make_unique<SEInitialEnvironment>(substances);
      EnvironmentConditions::Marshall(*initialEnvironmentData, *initialEnvironmentAction);
      return initialEnvironmentAction;
    }

    throw CommonDataModelException("Unsupported Condition Received in PatientConditions::factory");
  }

  std::unique_ptr<CDM::ConditionData> PatientConditions::factory(const SECondition* conditionData)
  {
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicAnemia)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicHeartFailure)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicVentricularSystolicDysfunction)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicObstructivePulmonaryDisease)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicPericardialEffusion)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ChronicRenalStenosis)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, Dehydration)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, DiabetesType1)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, DiabetesType2)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, ImpairedAlveolarExchange)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, LobarPneumonia)
    PATIENT_CONDITION_POLYMORPHIC_UNMARSHALL(conditionData, Starvation)
    throw biogears::CommonDataModelException("PatientConditions::factory does not support the derived SEPatientCondition. If you are not a developer contact upstream for support.");
  }
  //----------------------------------------------------------------------------------
  // SEPatientCondition
  void PatientConditions::Marshall(const CDM::PatientConditionData& in, SEPatientCondition& out)
  {
    Scenario::Marshall(static_cast<const CDM::ConditionData&>(in), static_cast<SECondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEPatientCondition& in, CDM::PatientConditionData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  // SEChronicAnemia
  void PatientConditions::Marshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.ReductionFactor(), out.GetReductionFactor());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out)
  {
    PatientConditions::UnMarshall(static_cast<const SEPatientCondition&>(in), static_cast<CDM::PatientConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, ReductionFactor)
  }
  //----------------------------------------------------------------------------------
  // SEChronicHeartFailure
  void PatientConditions::Marshall(const CDM::ChronicHeartFailureData& in, SEChronicHeartFailure& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicHeartFailure& in, CDM::ChronicHeartFailureData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // SEChronicVentricularSystolicDysfunction
  void PatientConditions::Marshall(const CDM::ChronicVentricularSystolicDysfunctionData& in, SEChronicVentricularSystolicDysfunction& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicVentricularSystolicDysfunction& in, CDM::ChronicVentricularSystolicDysfunctionData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // SEChronicObstructivePulmonaryDisease
  void PatientConditions::Marshall(const CDM::ChronicObstructivePulmonaryDiseaseData& in, SEChronicObstructivePulmonaryDisease& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.BronchitisSeverity(), out.GetBronchitisSeverity());
    io::Property::Marshall(in.EmphysemaSeverity(), out.GetEmphysemaSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicObstructivePulmonaryDisease& in, CDM::ChronicObstructivePulmonaryDiseaseData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));

    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, BronchitisSeverity)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, EmphysemaSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEChronicPericardialEffusion
  void PatientConditions::Marshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.AccumulatedVolume(), out.GetAccumulatedVolume());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, AccumulatedVolume)
  }
  //----------------------------------------------------------------------------------
  // SEChronicRenalStenosis
  void PatientConditions::Marshall(const CDM::ChronicRenalStenosisData& in, SEChronicRenalStenosis& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.LeftKidneySeverity(), out.GetLeftKidneySeverity());
    io::Property::Marshall(in.RightKidneySeverity(), out.GetRightKidneySeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicRenalStenosis& in, CDM::ChronicRenalStenosisData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, LeftKidneySeverity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RightKidneySeverity)
  }
  //----------------------------------------------------------------------------------
  // SEDehydration
  void PatientConditions::Marshall(const CDM::DehydrationData& in, SEDehydration& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.DehydrationFraction(), out.GetDehydrationFraction());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEDehydration& in, CDM::DehydrationData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, DehydrationFraction)
  }
  //----------------------------------------------------------------------------------
  // SEDiabetesType1
  void PatientConditions::Marshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.InsulinProductionSeverity(), out.GetInsulinProductionSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InsulinProductionSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEDiabetesType2
  void PatientConditions::Marshall(const CDM::DiabetesType2Data& in, SEDiabetesType2& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.InsulinProductionSeverity(), out.GetInsulinProductionSeverity());
    io::Property::Marshall(in.InsulinResistanceSeverity(), out.GetInsulinResistanceSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEDiabetesType2& in, CDM::DiabetesType2Data& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InsulinProductionSeverity)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InsulinResistanceSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEImpairedAlveolarExchange
  void PatientConditions::Marshall(const CDM::ImpairedAlveolarExchangeData& in, SEImpairedAlveolarExchange& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.ImpairedSurfaceArea(), out.GetImpairedSurfaceArea());
    io::Property::Marshall(in.ImpairedFraction(), out.GetImpairedFraction());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEImpairedAlveolarExchange& in, CDM::ImpairedAlveolarExchangeData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ImpairedSurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ImpairedFraction)
  }
  //----------------------------------------------------------------------------------
  // SELobarPneumonia
  void PatientConditions::Marshall(const CDM::LobarPneumoniaData& in, SELobarPneumonia& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.Severity(), out.GetSeverity());
    io::Property::Marshall(in.LeftLungAffected(), out.GetLeftLungAffected());
    io::Property::Marshall(in.RightLungAffected(), out.GetRightLungAffected());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SELobarPneumonia& in, CDM::LobarPneumoniaData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Severity)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LeftLungAffected)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, RightLungAffected)
  }
  //----------------------------------------------------------------------------------
  // SEStarvation
  void PatientConditions::Marshall(const CDM::StarvationData& in, SEStarvation& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.TimeSinceMeal(), out.GetTimeSinceMeal());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEStarvation& in, CDM::StarvationData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, TimeSinceMeal)
  }
  //----------------------------------------------------------------------------------
}
}
