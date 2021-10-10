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

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarArea.h>

#include "biogears/cdm/substance/SESubstanceManager.h"

namespace biogears {
namespace io {
  //class SEConditionList
  std::vector<std::unique_ptr<SECondition>> PatientConditions::condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances)
  {
    std::vector<std::unique_ptr<SECondition>> r_vec;
    for (const auto condition_data : in.Condition()) {
      r_vec.emplace_back(factory(condition_data, substances));
    }
    return std::move(r_vec);
  }
  //----------------------------------------------------------------------------------
  std::unique_ptr<SECondition> PatientConditions::factory(const CDM::ConditionData& data, SESubstanceManager& substances)
  {
    // Could speed up case by testing Patient Conditions vs another type
    // But we only have 1 type at this time, and a few conditions to support
    const auto ccAnemiaData = dynamic_cast<const CDM::ChronicAnemiaData*>(&data);
    if (ccAnemiaData != nullptr) {
      auto cc = std::make_unique<SEChronicAnemia>();
      PatientConditions::Marshall(*ccAnemiaData, *cc);
      return cc;
    }
    const auto ccopdData = dynamic_cast<const CDM::ChronicObstructivePulmonaryDiseaseData*>(&data);
    if (ccopdData != nullptr) {
      auto cc = std::make_unique<SEChronicObstructivePulmonaryDisease>();
      PatientConditions::Marshall(*ccopdData, *cc);
      return cc;
    }
    const auto ccVentSysDysfuncData = dynamic_cast<const CDM::ChronicVentricularSystolicDysfunctionData*>(&data);
    if (ccVentSysDysfuncData != nullptr) {
      auto cc = std::make_unique<SEChronicVentricularSystolicDysfunction>();
      PatientConditions::Marshall(*ccVentSysDysfuncData, *cc);
      return cc;
    }
    const auto ccPericardialEffusionData = dynamic_cast<const CDM::ChronicPericardialEffusionData*>(&data);
    if (ccPericardialEffusionData != nullptr) {
      auto cc = std::make_unique<SEChronicPericardialEffusion>();
      PatientConditions::Marshall(*ccPericardialEffusionData, *cc);
      return cc;
    }
    const auto ccRenalStenosisData = dynamic_cast<const CDM::ChronicRenalStenosisData*>(&data);
    if (ccRenalStenosisData != nullptr) {
      auto cc = std::make_unique<SEChronicRenalStenosis>();
      PatientConditions::Marshall(*ccRenalStenosisData, *cc);
      return cc;
    }
    const auto ccDehydrationData = dynamic_cast<const CDM::DehydrationData*>(&data);
    if (ccDehydrationData != nullptr) {
      auto cc = std::make_unique<SEDehydration>();
      PatientConditions::Marshall(*ccDehydrationData, *cc);
      return cc;
    }
    const auto ccDiabetesType1Data = dynamic_cast<const CDM::DiabetesType1Data*>(&data);
    if (ccDiabetesType1Data != nullptr) {
      auto cc = std::make_unique<SEDiabetesType1>();
      PatientConditions::Marshall(*ccDiabetesType1Data, *cc);
      return cc;
    }
    const auto ccDiabetesType2Data = dynamic_cast<const CDM::DiabetesType2Data*>(&data);
    if (ccDiabetesType2Data != nullptr) {
      auto cc = std::make_unique<SEDiabetesType2>();
      PatientConditions::Marshall(*ccDiabetesType2Data, *cc);
      return cc;
    }
    const auto ccStarvationData = dynamic_cast<const CDM::StarvationData*>(&data);
    if (ccStarvationData != nullptr) {
      auto cc = std::make_unique<SEStarvation>();
      PatientConditions::Marshall(*ccStarvationData, *cc);
      return cc;
    }
    const auto ccImpairedAlveolarExchangeData = dynamic_cast<const CDM::ImpairedAlveolarExchangeData*>(&data);
    if (ccImpairedAlveolarExchangeData != nullptr) {
      auto cc = std::make_unique<SEImpairedAlveolarExchange>();
      PatientConditions::Marshall(*ccImpairedAlveolarExchangeData, *cc);
      return cc;
    }
    const auto ccLobarPneumoniaData = dynamic_cast<const CDM::LobarPneumoniaData*>(&data);
    if (ccLobarPneumoniaData != nullptr) {
      auto cc = std::make_unique<SELobarPneumonia>();
      PatientConditions::Marshall(*ccLobarPneumoniaData, *cc);
      return cc;
    }
    const auto ccInitialEnvironmentData = dynamic_cast<const CDM::InitialEnvironmentData*>(&data);
    if (ccInitialEnvironmentData != nullptr) {
      auto cc = std::make_unique<SEInitialEnvironment>(substances);
      EnvironmentConditions::Marshall(*ccInitialEnvironmentData, *cc);
      return cc;
    }

    if (substances.GetLogger() != nullptr) {
      substances.GetLogger()->Error("Unsupported Condition Received", "PatientConditions::factory");
      throw CommonDataModelException("Unsupported Condition Received in PatientConditions::factory");
    }
    return nullptr;
  }
  //----------------------------------------------------------------------------------
  //SEPatientCondition
  void PatientConditions::Marshall(const CDM::PatientConditionData& in, SEPatientCondition& out)
  {
    Scenario::Marshall(static_cast<const CDM::ConditionData&>(in), static_cast<SECondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEPatientCondition& in, CDM::PatientConditionData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //SEChronicAnemia
  void PatientConditions::Marshall(const CDM::ChronicAnemiaData& in, SEChronicAnemia& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.ReductionFactor(), out.GetReductionFactor());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicAnemia& in, CDM::ChronicAnemiaData& out)
  {
    PatientConditions::UnMarshall(static_cast<const SEPatientCondition&>(in), static_cast<CDM::PatientConditionData&>(out));
    if (in.m_ReductionFactor != nullptr) {
      io::Property::UnMarshall(*in.m_ReductionFactor, out.ReductionFactor());
    }
  }
  //----------------------------------------------------------------------------------
  //SEChronicHeartFailure
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
  //SEChronicVentricularSystolicDysfunction
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
  //SEChronicObstructivePulmonaryDisease
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

    if (in.m_BronchitisSeverity != nullptr) {
      io::Property::UnMarshall(*in.m_BronchitisSeverity, out.BronchitisSeverity());
    }
    if (in.m_EmphysemaSeverity != nullptr) {
      io::Property::UnMarshall(*in.m_EmphysemaSeverity, out.BronchitisSeverity());
    }
  }
  //----------------------------------------------------------------------------------
  //SEChronicPericardialEffusion
  void PatientConditions::Marshall(const CDM::ChronicPericardialEffusionData& in, SEChronicPericardialEffusion& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.AccumulatedVolume(), out.GetAccumulatedVolume());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEChronicPericardialEffusion& in, CDM::ChronicPericardialEffusionData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    if (in.m_AccumulatedVolume != nullptr) {
      io::Property::UnMarshall(*in.m_AccumulatedVolume, out.AccumulatedVolume());
    }
  }
  //----------------------------------------------------------------------------------
  //SEChronicRenalStenosis
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
    if (in.HasLeftKidneySeverity()) {
      io::Property::UnMarshall(*in.m_LeftKidneySeverity, out.LeftKidneySeverity());
    }
    if (in.HasRightKidneySeverity()) {
      io::Property::UnMarshall(*in.m_RightKidneySeverity, out.RightKidneySeverity());
    }
  }
  //----------------------------------------------------------------------------------
  //SEDehydration
  void PatientConditions::Marshall(const CDM::DehydrationData& in, SEDehydration& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.DehydrationFraction(), out.GetDehydrationFraction());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEDehydration& in, CDM::DehydrationData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    if (in.m_DehydrationFraction != nullptr) {
      io::Property::UnMarshall(*in.m_DehydrationFraction, out.DehydrationFraction());
      ;
    }
  }
  //----------------------------------------------------------------------------------
  //SEDiabetesType1
  void PatientConditions::Marshall(const CDM::DiabetesType1Data& in, SEDiabetesType1& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.InsulinProductionSeverity(), out.GetInsulinProductionSeverity());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEDiabetesType1& in, CDM::DiabetesType1Data& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    if (in.m_InsulinProductionSeverity != nullptr) {
      io::Property::UnMarshall(*in.m_InsulinProductionSeverity, out.InsulinProductionSeverity());
    }
  }
  //----------------------------------------------------------------------------------
  //SEDiabetesType2
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
    if (in.m_InsulinProductionSeverity != nullptr) {
      io::Property::UnMarshall(*in.m_InsulinProductionSeverity, out.InsulinProductionSeverity());
    }
    if (in.m_InsulinResistanceSeverity != nullptr) {
      io::Property::UnMarshall(*in.m_InsulinResistanceSeverity, out.InsulinResistanceSeverity());
    }
  }
  //----------------------------------------------------------------------------------
  //SEImpairedAlveolarExchange
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
    if (in.HasImpairedSurfaceArea()) {
      io::Property::UnMarshall(*in.m_ImpairedSurfaceArea, out.ImpairedSurfaceArea());
    }
    if (in.HasImpairedFraction()) {
      io::Property::UnMarshall(*in.m_ImpairedFraction, out.ImpairedFraction());
    }
  }
  //----------------------------------------------------------------------------------
  //SELobarPneumonia
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
    if (in.m_Severity != nullptr) {
      io::Property::UnMarshall(*in.m_Severity, out.Severity());
    }
    if (in.m_LeftLungAffected != nullptr) {
      io::Property::UnMarshall(*in.m_LeftLungAffected, out.LeftLungAffected());
    }
    if (in.m_RightLungAffected != nullptr) {
      io::Property::UnMarshall(*in.m_RightLungAffected, out.RightLungAffected());
    }
  }
  //----------------------------------------------------------------------------------
  //SEStarvation
  void PatientConditions::Marshall(const CDM::StarvationData& in, SEStarvation& out)
  {
    PatientConditions::Marshall(static_cast<const CDM::PatientConditionData&>(in), static_cast<SEPatientCondition&>(out));
    io::Property::Marshall(in.TimeSinceMeal(), out.GetTimeSinceMeal());
  }
  //----------------------------------------------------------------------------------
  void PatientConditions::UnMarshall(const SEStarvation& in, CDM::StarvationData& out)
  {
    Scenario::UnMarshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
    if (in.m_TimeSinceMeal != nullptr)
      io::Property::UnMarshall(*in.m_TimeSinceMeal, out.TimeSinceMeal());
  }
  //----------------------------------------------------------------------------------
}
}
