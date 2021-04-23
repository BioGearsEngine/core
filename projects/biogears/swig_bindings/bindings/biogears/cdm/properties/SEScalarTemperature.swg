// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarTemperature.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<TemperatureUnit>::ToString;

%rename(Equals)  operator==(const TemperatureUnit&) const;
%rename(NotEquals)  operator!=(const TemperatureUnit&) const;
%rename(Equals)  operator==(const SEScalarTemperature&) const;
%rename(NotEquals)  operator!=(const SEScalarTemperature&) const;  

%ignore biogears::TemperatureUnit::TemperatureUnit(std::string const &);
%ignore biogears::TemperatureUnit::IsValidUnit(std::string const &);
%ignore biogears::TemperatureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarTemperature;
%nspace biogears::TemperatureUnit;
%nspace biogears::SEScalarQuantity<TemperatureUnit>;

namespace biogears {
   class TemperatureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::TemperatureUnit>;

%ignore  biogears::SEScalarQuantity< TemperatureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< TemperatureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TemperatureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TemperatureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TemperatureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TemperatureUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< TemperatureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< TemperatureUnit >::operator!=;

%template(QuantityTemperature) biogears::SEScalarQuantity<biogears::TemperatureUnit>;

%include <biogears/cdm/properties/SEScalarTemperature.h>




