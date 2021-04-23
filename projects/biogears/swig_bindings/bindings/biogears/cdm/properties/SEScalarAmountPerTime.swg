// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<AmountPerTimeUnit>::ToString;

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
%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountPerTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountPerTimeUnit >::operator!=;

%template(QuantityAmountPerTime) biogears::SEScalarQuantity<biogears::AmountPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerTime.h>




