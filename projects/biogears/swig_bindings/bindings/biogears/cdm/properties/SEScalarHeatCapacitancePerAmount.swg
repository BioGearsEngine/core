
%{
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<HeatCapacitancePerAmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatCapacitancePerAmountUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitancePerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitancePerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitancePerAmount&) const;  

%ignore biogears::HeatCapacitancePerAmountUnit::HeatCapacitancePerAmountUnit(std::string const &);
%ignore biogears::HeatCapacitancePerAmountUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatCapacitancePerAmountUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatCapacitancePerAmount;
%nspace biogears::HeatCapacitancePerAmountUnit;
%nspace biogears::SEScalarQuantity<HeatCapacitancePerAmountUnit>;

namespace biogears {
   class HeatCapacitancePerAmountUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>;

%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatCapacitancePerAmountUnit >::operator!=;

%template(QuantityHeatCapacitancePerAmount) biogears::SEScalarQuantity<biogears::HeatCapacitancePerAmountUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitancePerAmount.h>




