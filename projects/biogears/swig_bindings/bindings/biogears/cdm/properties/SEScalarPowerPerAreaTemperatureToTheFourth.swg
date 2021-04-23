// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::ToString;

%rename(Equals)  operator==(const PowerPerAreaTemperatureToTheFourthUnit&) const;
%rename(NotEquals)  operator!=(const PowerPerAreaTemperatureToTheFourthUnit&) const;
%rename(Equals)  operator==(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;
%rename(NotEquals)  operator!=(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;  

%ignore biogears::PowerPerAreaTemperatureToTheFourthUnit::PowerPerAreaTemperatureToTheFourthUnit(std::string const &);
%ignore biogears::PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(std::string const &);
%ignore biogears::PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPowerPerAreaTemperatureToTheFourth;
%nspace biogears::PowerPerAreaTemperatureToTheFourthUnit;
%nspace biogears::SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;

namespace biogears {
   class PowerPerAreaTemperatureToTheFourthUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PowerPerAreaTemperatureToTheFourthUnit>;

%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::operator!=;

%template(QuantityPowerPerAreaTemperatureToTheFourth) biogears::SEScalarQuantity<biogears::PowerPerAreaTemperatureToTheFourthUnit>;

%include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>




