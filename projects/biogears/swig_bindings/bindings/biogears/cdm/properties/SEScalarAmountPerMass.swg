
%{
#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<AmountPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerMassUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerMass&) const;  

%ignore biogears::AmountPerMassUnit::AmountPerMassUnit(std::string const &);
%ignore biogears::AmountPerMassUnit::IsValidUnit(std::string const &);
%ignore biogears::AmountPerMassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAmountPerMass;
%nspace biogears::AmountPerMassUnit;
%nspace biogears::SEScalarQuantity<AmountPerMassUnit>;

namespace biogears {
   class AmountPerMassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AmountPerMassUnit>;

%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerMassUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerMassUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountPerMassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountPerMassUnit >::operator!=;

%template(QuantityAmountPerMass) biogears::SEScalarQuantity<biogears::AmountPerMassUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerMass.h>




