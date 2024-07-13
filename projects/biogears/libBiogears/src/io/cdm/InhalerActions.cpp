#include "InhalerActions.h"

#include "Inhaler.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>

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
    if (auto inhalerConfiguration = dynamic_cast<SEInhalerConfiguration const*>(inhalerAction); inhalerConfiguration) {
      auto inhalerConfigurationData = std::make_unique<CDM::InhalerConfigurationData>();
      Marshall(*inhalerConfiguration, *inhalerConfigurationData);
      return std::move(inhalerConfigurationData);
    }
    throw biogears::CommonDataModelException("InhalerActions::factory does not support the derived SEInhalerAction. If you are not a developer contact upstream for support.");
  }
}
}