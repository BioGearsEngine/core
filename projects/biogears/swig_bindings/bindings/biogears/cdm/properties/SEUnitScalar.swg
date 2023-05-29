

%{
#include <biogears/cdm/properties/SEUnitScalar.h>
%}


%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>

%nspace BIOGEARS_NAMESPACE SEUnitScalar;
 
%ignore BIOGEARS_NAMESPACE SEUnitScalar::GetCompoundUnit(std::string const &) const ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::GetValue(std::string const &) const ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::IncrementValue(double,std::string const &) ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::DecrementValue(double,std::string const &) ;

%include <biogears/cdm/properties/SEUnitScalar.h>


