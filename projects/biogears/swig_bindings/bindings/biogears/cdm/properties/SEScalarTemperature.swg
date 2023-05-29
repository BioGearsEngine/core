
%{
#include <biogears/cdm/properties/SEScalarTemperature.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<TemperatureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const TemperatureUnit&) const;
%rename(NotEquals)  operator!=(const TemperatureUnit&) const;
%rename(Equals)  operator==(const SEScalarTemperature&) const;
%rename(NotEquals)  operator!=(const SEScalarTemperature&) const;  

%ignore BIOGEARS_NAMESPACE TemperatureUnit::TemperatureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TemperatureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TemperatureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarTemperature;
%nspace BIOGEARS_NAMESPACE TemperatureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TemperatureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class TemperatureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< TemperatureUnit >::operator!=;

%template(QuantityTemperature) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TemperatureUnit>;

%include <biogears/cdm/properties/SEScalarTemperature.h>




