%{
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEAppliedTemperature
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>

%nspace BIOGEARS_NAMESPACE SEAppliedTemperature;

%ignore BIOGEARS_NAMESPACE SEAppliedTemperature::Unload;
%ignore BIOGEARS_NAMESPACE SEAppliedTemperature::Load;
%ignore BIOGEARS_NAMESPACE SEAppliedTemperature::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SEAppliedTemperature::ToString(std::ostream& str) const;

%rename (GetPower_const) BIOGEARS_NAMESPACE SEAppliedTemperature:: GetTemperature(const TemperatureUnit& unit) const;
%rename (GetSurfaceArea_const) BIOGEARS_NAMESPACE SEAppliedTemperature::GetSurfaceArea(const AreaUnit& unit) const;
%rename (GetSurfaceAreaFraction_const) BIOGEARS_NAMESPACE SEAppliedTemperature::GetSurfaceAreaFraction() const;

%include <biogears/cdm/system/environment/SEAppliedTemperature.h>