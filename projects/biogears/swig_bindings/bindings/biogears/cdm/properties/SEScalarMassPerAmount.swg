
%{
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassPerAmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerAmountUnit&) const;
%rename(NotEquals)  operator!=(const MassPerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerAmount&) const;  

%ignore BIOGEARS_NAMESPACE MassPerAmountUnit::MassPerAmountUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMassPerAmount;
%nspace BIOGEARS_NAMESPACE MassPerAmountUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassPerAmountUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassPerAmountUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;
%ignore  SetValue(double);

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAmountUnit >::operator!=;

%template(QuantityMassPerAmount) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAmountUnit>;

%include <biogears/cdm/properties/SEScalarMassPerAmount.h>




