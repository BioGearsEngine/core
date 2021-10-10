
%{
#include <biogears/cdm/properties/SEScalarPower.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<PowerUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PowerUnit&) const;
%rename(NotEquals)  operator!=(const PowerUnit&) const;
%rename(Equals)  operator==(const SEScalarPower&) const;
%rename(NotEquals)  operator!=(const SEScalarPower&) const;  

%ignore biogears::PowerUnit::PowerUnit(std::string const &);
%ignore biogears::PowerUnit::IsValidUnit(std::string const &);
%ignore biogears::PowerUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPower;
%nspace biogears::PowerUnit;
%nspace biogears::SEScalarQuantity<PowerUnit>;

namespace biogears {
   class PowerUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PowerUnit>;

%ignore  biogears::SEScalarQuantity< PowerUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PowerUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PowerUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PowerUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PowerUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PowerUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PowerUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PowerUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PowerUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PowerUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PowerUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PowerUnit >::operator!=;

%template(QuantityPower) biogears::SEScalarQuantity<biogears::PowerUnit>;

%include <biogears/cdm/properties/SEScalarPower.h>




