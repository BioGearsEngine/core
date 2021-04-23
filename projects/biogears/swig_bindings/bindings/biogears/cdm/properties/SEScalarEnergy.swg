// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarEnergy.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<EnergyUnit>::ToString;

%rename(Equals)  operator==(const EnergyUnit&) const;
%rename(NotEquals)  operator!=(const EnergyUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergy&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergy&) const;  

%ignore biogears::EnergyUnit::EnergyUnit(std::string const &);
%ignore biogears::EnergyUnit::IsValidUnit(std::string const &);
%ignore biogears::EnergyUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarEnergy;
%nspace biogears::EnergyUnit;
%nspace biogears::SEScalarQuantity<EnergyUnit>;

namespace biogears {
   class EnergyUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::EnergyUnit>;

%ignore  biogears::SEScalarQuantity< EnergyUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< EnergyUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< EnergyUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< EnergyUnit >::operator!=;

%template(QuantityEnergy) biogears::SEScalarQuantity<biogears::EnergyUnit>;

%include <biogears/cdm/properties/SEScalarEnergy.h>




