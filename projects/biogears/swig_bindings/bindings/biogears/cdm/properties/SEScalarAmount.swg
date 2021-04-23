// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarAmount.h>
%}

%include <biogears/exports.h>

%import "SESCalarQuantity.swg"


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<AmountUnit>::ToString;

%rename(Equals)  operator==(const AmountUnit&) const;
%rename(NotEquals)  operator!=(const AmountUnit&) const;
%rename(Equals)  operator==(const SEScalarAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarAmount&) const;  

%ignore biogears::AmountUnit::AmountUnit(std::string const &);
%ignore biogears::AmountUnit::IsValidUnit(std::string const &);
%ignore biogears::AmountUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAmount;
%nspace biogears::AmountUnit;
%nspace biogears::SEScalarQuantity<AmountUnit>;

namespace biogears {
   class AmountUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AmountUnit>;

%ignore  biogears::SEScalarQuantity< AmountUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountUnit >::GetCompoundUnit(std::string const &) const ;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountUnit >::operator!=;


%template(QuantityAmount) biogears::SEScalarQuantity<biogears::AmountUnit>;

%include <biogears/cdm/properties/SEScalarAmount.h>




