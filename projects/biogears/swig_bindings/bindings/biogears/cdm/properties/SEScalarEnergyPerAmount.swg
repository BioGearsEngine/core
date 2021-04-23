// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<EnergyPerAmountUnit>::ToString;

%rename(Equals)  operator==(const EnergyPerAmountUnit&) const;
%rename(NotEquals)  operator!=(const EnergyPerAmountUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergyPerAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergyPerAmount&) const;  

%ignore biogears::EnergyPerAmountUnit::EnergyPerAmountUnit(std::string const &);
%ignore biogears::EnergyPerAmountUnit::IsValidUnit(std::string const &);
%ignore biogears::EnergyPerAmountUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarEnergyPerAmount;
%nspace biogears::EnergyPerAmountUnit;
%nspace biogears::SEScalarQuantity<EnergyPerAmountUnit>;

namespace biogears {
   class EnergyPerAmountUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::EnergyPerAmountUnit>;

%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< EnergyPerAmountUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< EnergyPerAmountUnit >::operator!=;

%template(QuantityEnergyPerAmount) biogears::SEScalarQuantity<biogears::EnergyPerAmountUnit>;

%include <biogears/cdm/properties/SEScalarEnergyPerAmount.h>




