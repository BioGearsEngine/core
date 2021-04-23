
%{
#include <biogears/cdm/properties/SEScalarTemperature.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<TemperatureUnit>::ToString(std::ostream& str);

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
%ignore  biogears::SEScalarQuantity< TemperatureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TemperatureUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TemperatureUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TemperatureUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TemperatureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< TemperatureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< TemperatureUnit >::operator!=;

%template(QuantityTemperature) biogears::SEScalarQuantity<biogears::TemperatureUnit>;

%include <biogears/cdm/properties/SEScalarTemperature.h>




