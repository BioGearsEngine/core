
%{
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<MassPerAmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerAmountUnit&) const;
%rename(NotEquals)  operator!=(const MassPerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerAmount&) const;  

%ignore biogears::MassPerAmountUnit::MassPerAmountUnit(std::string const &);
%ignore biogears::MassPerAmountUnit::IsValidUnit(std::string const &);
%ignore biogears::MassPerAmountUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMassPerAmount;
%nspace biogears::MassPerAmountUnit;
%nspace biogears::SEScalarQuantity<MassPerAmountUnit>;

namespace biogears {
   class MassPerAmountUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassPerAmountUnit>;

%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerAmountUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAmountUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAmountUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;
%ignore  SetValue(double);

%ignore  biogears::SEScalarQuantity< MassPerAmountUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassPerAmountUnit >::operator!=;

%template(QuantityMassPerAmount) biogears::SEScalarQuantity<biogears::MassPerAmountUnit>;

%include <biogears/cdm/properties/SEScalarMassPerAmount.h>




