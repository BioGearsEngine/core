
%{
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<EnergyPerAmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const EnergyPerAmountUnit&) const;
%rename(NotEquals)  operator!=(const EnergyPerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergyPerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergyPerAmount&) const;  

%ignore BIOGEARS_NAMESPACE EnergyPerAmountUnit::EnergyPerAmountUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyPerAmountUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyPerAmountUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarEnergyPerAmount;
%nspace BIOGEARS_NAMESPACE EnergyPerAmountUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<EnergyPerAmountUnit>;

OPEN_BIOGEARS_NAMESPACE
   class EnergyPerAmountUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerAmountUnit >::operator!=;

%template(QuantityEnergyPerAmount) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerAmountUnit>;

%include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>




