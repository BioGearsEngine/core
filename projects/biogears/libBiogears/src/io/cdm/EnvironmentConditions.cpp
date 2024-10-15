#include "EnvironmentConditions.h"

#include "Environment.h"
#include "Scenario.h"

#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

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

  std::unique_ptr<SECondition> EnvironmentConditions::factory(CDM::EnvironmentConditionData const* environmentConditionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {

    if (const auto initialEnvironmentData = dynamic_cast<const CDM::InitialEnvironmentData*>(environmentConditionData); initialEnvironmentData) {
      auto initialEnvironmentAction = std::make_unique<SEInitialEnvironment>(substances);
      EnvironmentConditions::UnMarshall(*initialEnvironmentData, *initialEnvironmentAction);
      return initialEnvironmentAction;
    }

    throw CommonDataModelException("Unsupported Condition Received in PatientConditions::factory");
  }

  std::unique_ptr<CDM::EnvironmentConditionData> EnvironmentConditions::factory(const SEEnvironmentCondition* conditionData)
  {
    POLYMORPHIC_MARSHALL(conditionData, InitialEnvironment)
    throw biogears::CommonDataModelException("PatientConditions::factory does not support the derived SEPatientCondition. If you are not a developer contact upstream for support.");
  }
  // class SEEnvironmentCondition
  void EnvironmentConditions::UnMarshall(const CDM::EnvironmentConditionData& in, SEEnvironmentCondition& out)
  {
    io::Conditions::UnMarshall(static_cast<const CDM::ConditionData&>(in), static_cast<SECondition&>(out));
  }
  //----------------------------------------------------------------------------------
  void EnvironmentConditions::Marshall(const SEEnvironmentCondition& in, CDM::EnvironmentConditionData& out)
  {
    io::Conditions::Marshall(static_cast<const SECondition&>(in), static_cast<CDM::ConditionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEInitialEnvironment
  void EnvironmentConditions::UnMarshall(const CDM::InitialEnvironmentData& in, SEInitialEnvironment& out)
  {
    io::Conditions::UnMarshall(static_cast<const CDM::EnvironmentConditionData&>(in), static_cast<SEEnvironmentCondition&>(out));
    if (in.ConditionsFile().present())
      out.SetConditionsFile(in.ConditionsFile().get());
    if (in.Conditions().present())
      io::Environment::UnMarshall(in.Conditions(), out.GetConditions());
  }
  //----------------------------------------------------------------------------------
  void EnvironmentConditions::Marshall(const SEInitialEnvironment& in, CDM::InitialEnvironmentData& out)
  {
    io::Conditions::Marshall(static_cast<const SEEnvironmentCondition&>(in), static_cast<CDM::EnvironmentConditionData&>(out));
    if (in.HasConditions())
      io::Environment::Marshall(*in.m_Conditions, out.Conditions());
    else if (in.HasConditionsFile())
      out.ConditionsFile(in.m_ConditionsFile);
  }
  //----------------------------------------------------------------------------------
}
}