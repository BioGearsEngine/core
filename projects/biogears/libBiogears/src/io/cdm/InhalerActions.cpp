#include "InhalerActions.h"

#include "Inhaler.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
#include <biogears/cdm/scenario/SEInhalerActionCollection.h>

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
  // class SEInhalerAction
  void InhalerActions::UnMarshall(const CDM::InhalerActionData& in, SEInhalerAction& out)
  {
    io::Actions::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void InhalerActions::Marshall(const SEInhalerAction& in, CDM::InhalerActionData& out)
  {
    io::Actions::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEInhalerConfiguration
  void InhalerActions::UnMarshall(const CDM::InhalerConfigurationData& in, SEInhalerConfiguration& out)
  {
    io::Actions::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
    if (in.ConfigurationFile().present()) {
      out.SetConfigurationFile(in.ConfigurationFile().get());
    }
    io::Inhaler::UnMarshall(in.Configuration(), out.GetConfiguration());
  }
  //----------------------------------------------------------------------------------
  void InhalerActions::Marshall(const SEInhalerConfiguration& in, CDM::InhalerConfigurationData& out)
  {
    io::Actions::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
    if (in.HasConfiguration()) {
      io::Inhaler::Marshall(*in.m_Configuration, out.Configuration());
    } else if (in.HasConfigurationFile()) {
      out.ConfigurationFile(in.m_ConfigurationFile);
    }
  }
  //----------------------------------------------------------------------------------
  // Factories
  std::unique_ptr<CDM::InhalerActionData> InhalerActions::factory(const SEInhalerAction* inhalerAction)
  {
    POLYMORPHIC_MARSHALL(inhalerAction, InhalerConfiguration)

    throw biogears::CommonDataModelException("InhalerActions::factory does not support the derived SEInhalerAction. If you are not a developer contact upstream for support.");
  }

  std::unique_ptr<SEAction> InhalerActions::factory(CDM::InhalerActionData const* inhalerActionData, SESubstanceManager& substances, std::default_random_engine* rd)
  {
    if (auto InhalerConfigurationData = dynamic_cast<CDM::InhalerConfigurationData const*>(inhalerActionData); InhalerConfigurationData) {
      auto InhalerConfiguration = std::make_unique<SEInhalerConfiguration>(substances);
      InhalerActions::UnMarshall(*InhalerConfigurationData, *InhalerConfiguration);
      return std::move(InhalerConfiguration);
    }
    throw biogears::CommonDataModelException("PatientActions:Factory - Unsupported Inhaler Action Received.");
  }

  void InhalerActions::Marshall(const SEInhalerActionCollection& in, std::vector<std::unique_ptr<CDM::ActionData>>& out) {
    if (nullptr != in.m_Configuration) {
      out.push_back(InhalerActions::factory(in.m_Configuration));
    }
  }


} // namespace io
} // namespace biogears