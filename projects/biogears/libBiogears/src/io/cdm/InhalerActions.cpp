#include "InhalerActions.h"

#include "Inhaler.h"
#include "Property.h"
#include "Scenario.h"

#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>
#include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>

namespace biogears {
namespace io {
  //class SEInhalerAction
  void InhalerActions::Marshall(const CDM::InhalerActionData& in, SEInhalerAction& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void InhalerActions::UnMarshall(const SEInhalerAction& in, CDM::InhalerActionData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEInhalerConfiguration
  void InhalerActions::Marshall(const CDM::InhalerConfigurationData& in, SEInhalerConfiguration& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
    if (in.ConfigurationFile().present()) {
      out.SetConfigurationFile(in.ConfigurationFile().get());
    }
    io::Inhaler::Marshall(in.Configuration(), out.GetConfiguration());
  }
  //----------------------------------------------------------------------------------
  void InhalerActions::UnMarshall(const SEInhalerConfiguration& in, CDM::InhalerConfigurationData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
    if (in.HasConfiguration()) {
      io::Inhaler::UnMarshall(*in.m_Configuration, out.Configuration());
    } else if (in.HasConfigurationFile()) {
      out.ConfigurationFile(in.m_ConfigurationFile);
    }
  }
  //----------------------------------------------------------------------------------
}
}