// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<ElectricCapacitanceUnit>::ToString;

%rename(Equals)  operator==(const ElectricCapacitanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricCapacitanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCapacitance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCapacitance&) const;  

%ignore biogears::ElectricCapacitanceUnit::ElectricCapacitanceUnit(std::string const &);
%ignore biogears::ElectricCapacitanceUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricCapacitanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricCapacitance;
%nspace biogears::ElectricCapacitanceUnit;
%nspace biogears::SEScalarQuantity<ElectricCapacitanceUnit>;

namespace biogears {
   class ElectricCapacitanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>;

%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::operator!=;

%template(QuantityElectricCapacitance) biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricCapacitance.h>




