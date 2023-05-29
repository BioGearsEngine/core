
%{
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitancePerAmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatCapacitancePerAmountUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitancePerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitancePerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitancePerAmount&) const;  

%ignore BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit::HeatCapacitancePerAmountUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatCapacitancePerAmount;
%nspace BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitancePerAmountUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatCapacitancePerAmountUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerAmountUnit >::operator!=;

%template(QuantityHeatCapacitancePerAmount) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerAmountUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>




