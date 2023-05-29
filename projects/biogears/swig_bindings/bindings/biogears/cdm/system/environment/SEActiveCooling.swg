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


%nspace BIOGEARS_NAMESPACE SEActiveCooling;

%ignore BIOGEARS_NAMESPACE SEActiveCooling::Unload;
%ignore BIOGEARS_NAMESPACE SEActiveCooling::Load;
%ignore BIOGEARS_NAMESPACE SEActiveCooling::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEActiveCooling::ToString(std::ostream& str) const;

%rename (GetPower_const) BIOGEARS_NAMESPACE SEActiveCooling::GetPower(const PowerUnit& unit) const;
%rename (GetSurfaceArea_const) BIOGEARS_NAMESPACE SEActiveCooling::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) BIOGEARS_NAMESPACE SEActiveCooling::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEActiveCooling.h>