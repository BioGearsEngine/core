
// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarQuantity.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEUnitScalar.swg>

%nspace biogears::SEScalarQuantity;
%nspace biogears::QuantityUnit;

%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<TimeUnit>::ToString;

%rename(Equals)  operator==(const SEScalarQuantity&) const;
%rename(NotEquals)  operator!=(const SEScalarQuantity&) const;

%ignore  SEScalarQuantity::GetValue(std::string const &) const ;
%ignore  SEScalarQuantity::IncrementValue(double,std::string const &) ;
%ignore  SEScalarQuantity::IncrementValue(double,char const *) ;
%ignore  SEScalarQuantity::DecrementValue(double,std::string const &) ;
%ignore  SEScalarQuantity::DecrementValue(double,char const *) ;
%ignore  SEScalarQuantity::GetCompoundUnit(std::string const &) const ;

%include <biogears/cdm/properties/SEScalarQuantity.h>


