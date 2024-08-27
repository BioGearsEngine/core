#include "Conditions.h"

#include "Actions.h"
#include "EnvironmentConditions.h"
#include "PatientConditions.h"
#include "Property.h"

#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/SEConditionManager.h>

#include <biogears/BiogearsEnums.h>

namespace biogears {
namespace io {

  std::vector<std::unique_ptr<SECondition>> Conditions::condition_factory(const CDM::ConditionListData& in, SESubstanceManager& substances, std::default_random_engine* rd)
  {
    std::vector<std::unique_ptr<SECondition>> r_vec;
    for (auto condition_data : in.Condition()) {
      r_vec.emplace_back(factory(&condition_data, substances, rd));
    }
    return std::move(r_vec);
  }

  std::vector<std::unique_ptr<CDM::ConditionData>> Conditions::condition_data_factory(SEConditionManager const& conditionManager)
  {
    auto conditions = std::vector<std::unique_ptr<CDM::ConditionData>>();
    for (auto condition : conditionManager.m_Conditions) {
      conditions.push_back(factory(condition));
    }
    return conditions;
  }
  std::unique_ptr<SECondition> Conditions::factory(CDM::ConditionData const* conditionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {

    if (auto patient_condition_data = dynamic_cast<const CDM::PatientConditionData*>(conditionData)) {
      return io::PatientConditions::factory(patient_condition_data, substances, rd);
    }

    if (auto environment_condition_data = dynamic_cast<const CDM::EnvironmentConditionData*>(conditionData)) {
      return io::EnvironmentConditions::factory(environment_condition_data, substances, rd);
    }

    throw biogears::CommonDataModelException("Conditions:Factory - Unsupported Condition Received.");
  }

  std::unique_ptr<CDM::ConditionData> Conditions::factory(const SECondition* data)
  {
    if (auto patient_condition_data = dynamic_cast<SEPatientCondition const*>(data)) {
      return io::PatientConditions::factory(patient_condition_data);
    }

    if (auto environment_condition_data = dynamic_cast<SEEnvironmentCondition const*>(data)) {
      return io::EnvironmentConditions::factory(environment_condition_data);
    }

    throw biogears::CommonDataModelException("Conditions::factory does not support the derived Condition. If you are not a developer contact upstream for support.");
  }

  // class SECondition;
  void Conditions::UnMarshall(const CDM::ConditionData& in, SECondition& out)
  {
    out.Clear();
    if (in.Comment().present()) {
      out.m_Comment = in.Comment().get();
    }
  }
  void Conditions::Marshall(const SECondition& in, CDM::ConditionData& out)
  {
    if (in.HasComment()) {
      out.Comment(in.m_Comment);
    }
  }
} // namespace io
} // namespace biogears