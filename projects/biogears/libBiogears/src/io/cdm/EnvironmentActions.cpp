#include "EnvironmentActions.h"

#include "Scenario.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>
namespace biogears {
namespace io {
  //class SEEnvironmentAction
  void EnvironmentActions::Marshall(const CDM::EnvironmentActionData& in, SEEnvironmentAction& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::ActionData&>(in), static_cast<SEAction&>(out));
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::UnMarshall(const SEEnvironmentAction& in, CDM::EnvironmentActionData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEAction&>(in), static_cast<CDM::ActionData&>(out));
  }
  //----------------------------------------------------------------------------------
  //class SEEnvironmentChange
  void EnvironmentActions::Marshall(const CDM::EnvironmentChangeData& in, SEEnvironmentChange& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::EnvironmentActionData&>(in), static_cast<SEEnvironmentAction&>(out));
    if (in.ConditionsFile().present())
      out.SetConditionsFile(in.ConditionsFile().get());
    else if (in.Conditions().present())
      out.GetConditions().Load(in.Conditions().get());
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::UnMarshall(const SEEnvironmentChange& in, CDM::EnvironmentChangeData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEEnvironmentAction&>(in), static_cast<CDM::EnvironmentActionData&>(out));
    if (in.HasConditions())
      out.Conditions(std::unique_ptr<CDM::EnvironmentalConditionsData>(in.m_Conditions->Unload()));
    else if (in.HasConditionsFile())
      out.ConditionsFile(in.m_ConditionsFile);
  }
  //----------------------------------------------------------------------------------
  //class SEThermalApplication
  void EnvironmentActions::Marshall(const CDM::ThermalApplicationData& in, SEThermalApplication& out)
  {
    io::Scenario::Marshall(static_cast<const CDM::EnvironmentActionData&>(in), static_cast<SEEnvironmentAction&>(out));

    if (in.ActiveHeating().present())
      out.GetActiveHeating().Load(in.ActiveHeating().get());
    if (in.ActiveCooling().present())
      out.GetActiveCooling().Load(in.ActiveCooling().get());
    if (in.AppliedTemperature().present())
      out.GetAppliedTemperature().Load(in.AppliedTemperature().get());
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::UnMarshall(const SEThermalApplication& in, CDM::ThermalApplicationData& out)
  {
    io::Scenario::UnMarshall(static_cast<const SEEnvironmentAction&>(in), static_cast<CDM::EnvironmentActionData&>(out));
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
    EnvironmentActions::UnMarshall(in, data);
    EnvironmentActions::Marshall(data, out);
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
}
}