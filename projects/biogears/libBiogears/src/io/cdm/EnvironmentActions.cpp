#include "EnvironmentActions.h"

#include "Actions.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
namespace biogears {
namespace io {
  // class SEEnvironmentAction
  void EnvironmentActions::UnMarshall(const CDM::EnvironmentActionData& in, SEEnvironmentAction& out)
  {
    io::Actions::UnMarshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::Marshall(const SEEnvironmentAction& in, CDM::EnvironmentActionData& out)
  {
    io::Actions::Marshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  // class SEEnvironmentChange
  void EnvironmentActions::UnMarshall(const CDM::EnvironmentChangeData& in, SEEnvironmentChange& out)
  {
    UnMarshall(static_cast<const CDM::EnvironmentActionData&>(in), static_cast<SEEnvironmentAction&>(out));
    if (in.ConditionsFile().present())
      out.SetConditionsFile(in.ConditionsFile().get());
    else if (in.Conditions().present())
      out.GetConditions().Load(in.Conditions().get());
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::Marshall(const SEEnvironmentChange& in, CDM::EnvironmentChangeData& out)
  {
    io::Actions::Marshall(static_cast<const SEEnvironmentAction&>(in), static_cast<CDM::EnvironmentActionData&>(out));
    if (in.HasConditions())
      out.Conditions(std::unique_ptr<CDM::EnvironmentalConditionsData>(in.m_Conditions->Unload()));
    else if (in.HasConditionsFile())
      out.ConditionsFile(in.m_ConditionsFile);
  }
  //----------------------------------------------------------------------------------
  // class SEThermalApplication
  void EnvironmentActions::UnMarshall(const CDM::ThermalApplicationData& in, SEThermalApplication& out)
  {
    io::Actions::UnMarshall(static_cast<const CDM::EnvironmentActionData&>(in), static_cast<SEEnvironmentAction&>(out));

    if (in.ActiveHeating().present())
      out.GetActiveHeating().Load(in.ActiveHeating().get());
    if (in.ActiveCooling().present())
      out.GetActiveCooling().Load(in.ActiveCooling().get());
    if (in.AppliedTemperature().present())
      out.GetAppliedTemperature().Load(in.AppliedTemperature().get());
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::Marshall(const SEThermalApplication& in, CDM::ThermalApplicationData& out)
  {
    Marshall(static_cast<const SEEnvironmentAction&>(in), static_cast<CDM::EnvironmentActionData&>(out));
    if (in.HasActiveHeating())
      out.ActiveHeating(std::unique_ptr<CDM::ActiveHeatingData>(in.m_ActiveHeating->Unload()));
    if (in.HasActiveCooling())
      out.ActiveCooling(std::unique_ptr<CDM::ActiveCoolingData>(in.m_ActiveCooling->Unload()));
    if (in.HasAppliedTemperature())
      out.AppliedTemperature(std::unique_ptr<CDM::AppliedTemperatureData>(in.m_AppliedTemperature->Unload()));
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Copy(const SE& in, SE& out)
  {
    XSD data;
    EnvironmentActions::Marshall(in, data);
    EnvironmentActions::UnMarshall(data, out);
  }
  //-----------------------------------------------------------------------------
  void EnvironmentActions::Copy(const SEEnvironmentAction& in, SEEnvironmentAction& out)
  {
    ::biogears::io::Copy<SEEnvironmentAction, CDM::EnvironmentActionData>(in, out);
  }
  void EnvironmentActions::Copy(const SEEnvironmentChange& in, SEEnvironmentChange& out)
  {
    ::biogears::io::Copy<SEEnvironmentChange, CDM::EnvironmentChangeData>(in, out);
  }
  void EnvironmentActions::Copy(const SEThermalApplication& in, SEThermalApplication& out)
  {
    ::biogears::io::Copy<SEThermalApplication, CDM::ThermalApplicationData>(in, out);
  }
  //-----------------------------------------------------------------------------
  //! Can throw NULLPTR to indicate a failure
  std::unique_ptr<CDM::EnvironmentActionData> EnvironmentActions::factory(const SEEnvironmentAction* environmentAction)
  {
    if (auto environmentChangeAction = dynamic_cast<SEEnvironmentChange const*>(environmentAction); environmentChangeAction) {
      auto environmentChangeActionData = std::make_unique<CDM::EnvironmentChangeData>();
      Marshall(*environmentChangeAction, *environmentChangeActionData);
      return std::move(environmentChangeActionData);
    }

    if (auto thermalApplication = dynamic_cast<SEThermalApplication const*>(environmentAction); thermalApplication) {
      auto thermalApplicationData = std::make_unique<CDM::EnvironmentChangeData>();
      Marshall(*thermalApplication, *thermalApplicationData);
      return std::move(thermalApplicationData);
    }
    throw biogears::CommonDataModelException("EnvironmentActions::factory does not support the derived SEEnvironmentAction. If you are not a developer contact upstream for support.");
  }
}
}