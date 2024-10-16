#include "EnvironmentActions.h"

#include "Actions.h"
#include "Environment.h"

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentChange.h>
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>

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
    if (in.ConditionsFile().present()) {
      out.SetConditionsFile(in.ConditionsFile().get());
    } else if (in.Conditions().present()) {
      Environment::UnMarshall(in.Conditions(), out.GetConditions());
    }
  }
  //----------------------------------------------------------------------------------
  void EnvironmentActions::Marshall(const SEEnvironmentChange& in, CDM::EnvironmentChangeData& out)
  {
    io::Actions::Marshall(static_cast<const SEEnvironmentAction&>(in), static_cast<CDM::EnvironmentActionData&>(out));
  
    if (in.m_Conditions && in.m_Conditions->IsValid()) {
      io::Environment::Marshall(*in.m_Conditions, out.Conditions());
    }
    else if (in.HasConditionsFile()) {
      out.ConditionsFile(in.m_ConditionsFile);
    }
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
    POLYMORPHIC_MARSHALL(environmentAction, EnvironmentChange)
    POLYMORPHIC_MARSHALL(environmentAction, ThermalApplication)

    throw biogears::CommonDataModelException("EnvironmentActions::factory does not support the derived SEEnvironmentAction. If you are not a developer contact upstream for support.");
  }
} // namespace io
} // namespace biogears