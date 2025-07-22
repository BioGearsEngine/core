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

#define POLYMORPHIC_MARSHALL(paramName, typeName)                               \
  if (auto typeName = dynamic_cast<SE##typeName const*>(paramName); typeName) { \
    auto typeName##Data = std::make_unique<CDM::typeName##Data>();              \
    Marshall(*typeName, *typeName##Data);                                       \
    return std::move(typeName##Data);                                           \
  }

#define POLYMORPHIC_UNMARSHALL(paramName, typeName, schema)                                        \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName);                                                \
    return std::move(typeName);                                                                    \
  }

#define STOCASTIC_POLYMORPHIC_UNMARSHALL(paramName, typeName, schema, engine)                      \
  if (auto typeName##Data = dynamic_cast<CDM::typeName##Data const*>(paramName); typeName##Data) { \
    auto typeName = std::make_unique<SE##typeName>();                                              \
    schema::UnMarshall(*typeName##Data, *typeName, engine);                                        \
    return std::move(typeName);                                                                    \
  }

namespace biogears {
namespace io {
  //----------------------------------------------------------------------------------
  std::unique_ptr<SECondition> PatientConditions::factory(CDM::PatientConditionData const* patientConditionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {
    // Could speed up case by testing Patient Conditions vs another type
    // But we only have 1 type at this time, and a few conditions to support

    POLYMORPHIC_UNMARSHALL(patientConditionData, ChronicAnemia, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, ChronicObstructivePulmonaryDisease, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, ChronicVentricularSystolicDysfunction, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, ChronicPericardialEffusion, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, ChronicRenalStenosis, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, Dehydration, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, DiabetesType1, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, DiabetesType2, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, Starvation, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, ImpairedAlveolarExchange, PatientConditions)
    POLYMORPHIC_UNMARSHALL(patientConditionData, LobarPneumonia, PatientConditions)

    throw CommonDataModelException("Unsupported Condition Received in PatientConditions::factory");
  }

  std::unique_ptr<CDM::PatientConditionData> PatientConditions::factory(const SEPatientCondition* patientConditionData)
  {
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicAnemia)
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicHeartFailure)
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicVentricularSystolicDysfunction)
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicObstructivePulmonaryDisease)
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicPericardialEffusion)
    POLYMORPHIC_MARSHALL(patientConditionData, ChronicRenalStenosis)
    POLYMORPHIC_MARSHALL(patientConditionData, Dehydration)
    POLYMORPHIC_MARSHALL(patientConditionData, DiabetesType1)
    POLYMORPHIC_MARSHALL(patientConditionData, DiabetesType2)
    POLYMORPHIC_MARSHALL(patientConditionData, ImpairedAlveolarExchange)
    POLYMORPHIC_MARSHALL(patientConditionData, LobarPneumonia)
    POLYMORPHIC_MARSHALL(patientConditionData, Starvation)
    throw biogears::CommonDataModelException("PatientConditions::factory does not support the derived SEPatientCondition. If you are not a developer contact upstream for support.");
  }
  //----------------------------------------------------------------------------------
  // SEPatientCondition
  void PatientConditions::UnMarshall(const CDM::PatientConditionData& in, SEPatientCondition& out)
  {
    Conditions::UnMarshall(static_cast<const CDM::ConditionData&>(in), static_cast<SECondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEPatientCondition& in, CDM::PatientConditionData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  // SEChronicAnemia
  void PatientConditions::UnMarshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.ReductionFactor(), out.GetReductionFactor());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out)
  {
    PatientConditions::Marshall(static_cast<const SEPatientCondition&>(in), static_cast<CDM::PatientConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, ReductionFactor)
  }
  //----------------------------------------------------------------------------------
  // SEChronicHeartFailure
  void PatientConditions::UnMarshall(const CDM::ChronicHeartFailureData& in, SEChronicHeartFailure& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicHeartFailure& in, CDM::ChronicHeartFailureData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // SEChronicVentricularSystolicDysfunction
  void PatientConditions::UnMarshall(const CDM::ChronicVentricularSystolicDysfunctionData& in, SEChronicVentricularSystolicDysfunction& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicVentricularSystolicDysfunction& in, CDM::ChronicVentricularSystolicDysfunctionData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // SEChronicObstructivePulmonaryDisease
  void PatientConditions::UnMarshall(const CDM::ChronicObstructivePulmonaryDiseaseData& in, SEChronicObstructivePulmonaryDisease& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.BronchitisSeverity(), out.GetBronchitisSeverity());
    io::Property::UnMarshall(in.EmphysemaSeverity(), out.GetEmphysemaSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicObstructivePulmonaryDisease& in, CDM::ChronicObstructivePulmonaryDiseaseData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));

    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, BronchitisSeverity)
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, EmphysemaSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEChronicPericardialEffusion
  void PatientConditions::UnMarshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.AccumulatedVolume(), out.GetAccumulatedVolume());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, AccumulatedVolume)
  }
  //----------------------------------------------------------------------------------
  // SEChronicRenalStenosis
  void PatientConditions::UnMarshall(const CDM::ChronicRenalStenosisData& in, SEChronicRenalStenosis& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.LeftKidneySeverity(), out.GetLeftKidneySeverity());
    io::Property::UnMarshall(in.RightKidneySeverity(), out.GetRightKidneySeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEChronicRenalStenosis& in, CDM::ChronicRenalStenosisData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_OPTIONAL_PROPERTY_PTR_MARSHALL_HELPER(in, out, LeftKidneySeverity)
    CDM_OPTIONAL_PROPERTY_PTR_MARSHALL_HELPER(in, out, RightKidneySeverity)
  }
  //----------------------------------------------------------------------------------
  // SEDehydration
  void PatientConditions::UnMarshall(const CDM::DehydrationData& in, SEDehydration& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.DehydrationFraction(), out.GetDehydrationFraction());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEDehydration& in, CDM::DehydrationData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, DehydrationFraction)
  }
  //----------------------------------------------------------------------------------
  // SEDiabetesType1
  void PatientConditions::UnMarshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.InsulinProductionSeverity(), out.GetInsulinProductionSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, InsulinProductionSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEDiabetesType2
  void PatientConditions::UnMarshall(const CDM::DiabetesType2Data& in, SEDiabetesType2& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.InsulinProductionSeverity(), out.GetInsulinProductionSeverity());
    io::Property::UnMarshall(in.InsulinResistanceSeverity(), out.GetInsulinResistanceSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEDiabetesType2& in, CDM::DiabetesType2Data& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, InsulinProductionSeverity)
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, InsulinResistanceSeverity)
  }
  //----------------------------------------------------------------------------------
  // SEImpairedAlveolarExchange
  void PatientConditions::UnMarshall(const CDM::ImpairedAlveolarExchangeData& in, SEImpairedAlveolarExchange& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.ImpairedSurfaceArea(), out.GetImpairedSurfaceArea());
    io::Property::UnMarshall(in.ImpairedFraction(), out.GetImpairedFraction());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEImpairedAlveolarExchange& in, CDM::ImpairedAlveolarExchangeData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_OPTIONAL_PROPERTY_PTR_MARSHALL_HELPER(in, out, ImpairedSurfaceArea)
    CDM_OPTIONAL_PROPERTY_PTR_MARSHALL_HELPER(in, out, ImpairedFraction)
  }
  //----------------------------------------------------------------------------------
  // SELobarPneumonia
  void PatientConditions::UnMarshall(const CDM::LobarPneumoniaData& in, SELobarPneumonia& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.Severity(), out.GetSeverity());
    io::Property::UnMarshall(in.LeftLungAffected(), out.GetLeftLungAffected());
    io::Property::UnMarshall(in.RightLungAffected(), out.GetRightLungAffected());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SELobarPneumonia& in, CDM::LobarPneumoniaData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, Severity)
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, LeftLungAffected)
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, RightLungAffected)
  }
  //----------------------------------------------------------------------------------
  // SEStarvation
  void PatientConditions::UnMarshall(const CDM::StarvationData& in, SEStarvation& out)
  {
    PatientConditions::UnMarshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::UnMarshall(in.TimeSinceMeal(), out.GetTimeSinceMeal());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::Marshall(const SEStarvation& in, CDM::StarvationData& out)
  {
    Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    CDM_PROPERTY_PTR_MARSHALL_HELPER(in, out, TimeSinceMeal)
  }
  //----------------------------------------------------------------------------------
}
}
