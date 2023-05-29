%{
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>


%nspace BIOGEARS_NAMESPACE SEActiveHeating;

%ignore BIOGEARS_NAMESPACE SEActiveHeating::Unload;
%ignore BIOGEARS_NAMESPACE SEActiveHeating::Load;
%ignore BIOGEARS_NAMESPACE SEActiveHeating::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEActiveHeating::ToString(std::ostream& str) const;

%rename (GetPower_const) BIOGEARS_NAMESPACE SEActiveHeating::GetPower(const PowerUnit& unit) const;
%rename (GetSurfaceArea_const) BIOGEARS_NAMESPACE SEActiveHeating::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) BIOGEARS_NAMESPACE SEActiveHeating::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEActiveHeating.h>