// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarHeatConductance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<HeatConductanceUnit>::ToString;

%rename(Equals)  operator==(const HeatConductanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatConductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatConductance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatConductance&) const;  

%ignore biogears::HeatConductanceUnit::HeatConductanceUnit(std::string const &);
%ignore biogears::HeatConductanceUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatConductanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatConductance;
%nspace biogears::HeatConductanceUnit;
%nspace biogears::SEScalarQuantity<HeatConductanceUnit>;

namespace biogears {
   class HeatConductanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatConductanceUnit>;

%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatConductanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatConductanceUnit >::operator!=;

%template(QuantityHeatConductance) biogears::SEScalarQuantity<biogears::HeatConductanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatConductance.h>




