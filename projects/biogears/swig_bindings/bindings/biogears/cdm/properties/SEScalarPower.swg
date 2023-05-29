
%{
#include <biogears/cdm/properties/SEScalarPower.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<PowerUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PowerUnit&) const;
%rename(NotEquals)  operator!=(const PowerUnit&) const;
%rename(Equals)  operator==(const SEScalarPower&) const;
%rename(NotEquals)  operator!=(const SEScalarPower&) const;  

%ignore BIOGEARS_NAMESPACE PowerUnit::PowerUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PowerUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PowerUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarPower;
%nspace BIOGEARS_NAMESPACE PowerUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<PowerUnit>;

OPEN_BIOGEARS_NAMESPACE
   class PowerUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< PowerUnit >::operator!=;

%template(QuantityPower) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PowerUnit>;

%include <biogears/cdm/properties/SEScalarPower.h>




