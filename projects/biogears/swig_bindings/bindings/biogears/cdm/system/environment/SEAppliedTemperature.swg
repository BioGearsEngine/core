%{
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>

%nspace biogears::SEAppliedTemperature;

%ignore biogears::SEAppliedTemperature::Unload;
%ignore biogears::SEAppliedTemperature::Load;
%ignore biogears::SEAppliedTemperature::GetScalar(const std::string& name);
%ignore biogears::SEAppliedTemperature::ToString(std::ostream& str) const;

%rename (GetPower_const) biogears::SEAppliedTemperature:: GetTemperature(const TemperatureUnit& unit) const;
%rename (GetSurfaceArea_const) biogears::SEAppliedTemperature::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) biogears::SEAppliedTemperature::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEAppliedTemperature.h>