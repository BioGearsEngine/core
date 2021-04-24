%{
#include <biogears/cdm/system/environment/SEActiveCooling.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>


%nspace biogears::SEActiveCooling;

%ignore biogears::SEActiveCooling::Unload;
%ignore biogears::SEActiveCooling::Load;
%ignore biogears::SEActiveCooling::GetScalar(const std::string& name);
%ignore biogears::SEActiveCooling::ToString(std::ostream& str) const;

%rename (GetPower_const) biogears::SEActiveCooling::GetPower(const PowerUnit& unit) const;
%rename (GetSurfaceArea_const) biogears::SEActiveCooling::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) biogears::SEActiveCooling::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEActiveCooling.h>