
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEUnitScalar.h>
%}


%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>

%nspace biogears::SEUnitScalar;
 
%ignore biogears::SEUnitScalar::GetCompoundUnit(std::string const &) const ;
%ignore biogears::SEUnitScalar::GetValue(std::string const &) const ;
%ignore biogears::SEUnitScalar::IncrementValue(double,std::string const &) ;
%ignore biogears::SEUnitScalar::DecrementValue(double,std::string const &) ;

%include <biogears/cdm/properties/SEUnitScalar.h>


