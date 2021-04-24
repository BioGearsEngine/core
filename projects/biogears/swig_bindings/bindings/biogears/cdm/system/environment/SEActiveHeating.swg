%{
#include <biogears/cdm/system/environment/SEActiveHeating.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>


%nspace biogears::SEActiveHeating;

%ignore biogears::SEActiveHeating::Unload;
%ignore biogears::SEActiveHeating::Load;
%ignore biogears::SEActiveHeating::GetScalar(const std::string& name);
%ignore biogears::SEActiveHeating::ToString(std::ostream& str) const;

%rename (GetPower_const) biogears::SEActiveHeating::GetPower(const PowerUnit& unit) const;
%rename (GetSurfaceArea_const) biogears::SEActiveHeating::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) biogears::SEActiveHeating::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEActiveHeating.h>