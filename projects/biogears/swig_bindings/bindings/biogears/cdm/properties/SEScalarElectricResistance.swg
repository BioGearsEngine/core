// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<ElectricResistanceUnit>::ToString;

%rename(Equals)  operator==(const ElectricResistanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricResistance&) const;  

%ignore biogears::ElectricResistanceUnit::ElectricResistanceUnit(std::string const &);
%ignore biogears::ElectricResistanceUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricResistanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricResistance;
%nspace biogears::ElectricResistanceUnit;
%nspace biogears::SEScalarQuantity<ElectricResistanceUnit>;

namespace biogears {
   class ElectricResistanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>;

%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricResistanceUnit >::operator!=;

%template(QuantityElectricResistance) biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricResistance.h>




