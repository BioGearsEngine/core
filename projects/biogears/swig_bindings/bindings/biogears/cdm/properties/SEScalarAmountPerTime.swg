
%{
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<AmountPerTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerTime&) const;  

%ignore biogears::AmountPerTimeUnit::AmountPerTimeUnit(std::string const &);
%ignore biogears::AmountPerTimeUnit::IsValidUnit(std::string const &);
%ignore biogears::AmountPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAmountPerTime;
%nspace biogears::AmountPerTimeUnit;
%nspace biogears::SEScalarQuantity<AmountPerTimeUnit>;

namespace biogears {
   class AmountPerTimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>;

%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountPerTimeUnit >::operator!=;

%template(QuantityAmountPerTime) biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerTime.h>




