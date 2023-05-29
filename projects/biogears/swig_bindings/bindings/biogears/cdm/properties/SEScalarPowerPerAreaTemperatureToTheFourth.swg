
%{
#include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PowerPerAreaTemperatureToTheFourthUnit&) const;
%rename(NotEquals)  operator!=(const PowerPerAreaTemperatureToTheFourthUnit&) const;
%rename(Equals)  operator==(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;
%rename(NotEquals)  operator!=(const SEScalarPowerPerAreaTemperatureToTheFourth&) const;  

%ignore BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit::PowerPerAreaTemperatureToTheFourthUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarPowerPerAreaTemperatureToTheFourth;
%nspace BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<PowerPerAreaTemperatureToTheFourthUnit>;

OPEN_BIOGEARS_NAMESPACE
   class PowerPerAreaTemperatureToTheFourthUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< PowerPerAreaTemperatureToTheFourthUnit >::operator!=;

%template(QuantityPowerPerAreaTemperatureToTheFourth) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerPerAreaTemperatureToTheFourthUnit>;

%include <biogears/cdm/properties/SEScalarPowerPerAreaTemperatureToTheFourth.h>




