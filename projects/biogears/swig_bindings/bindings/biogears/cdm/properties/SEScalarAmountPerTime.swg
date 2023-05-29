
%{
#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerTime&) const;  

%ignore BIOGEARS_NAMESPACE AmountPerTimeUnit::AmountPerTimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerTimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarAmountPerTime;
%nspace BIOGEARS_NAMESPACE AmountPerTimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerTimeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AmountPerTimeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerTimeUnit >::operator!=;

%template(QuantityAmountPerTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerTime.h>




