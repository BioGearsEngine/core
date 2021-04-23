// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<HeatResistanceAreaUnit>::ToString;

%rename(Equals)  operator==(const HeatResistanceAreaUnit&) const;
%rename(NotEquals)  operator!=(const HeatResistanceAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatResistanceArea&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatResistanceArea&) const;  

%ignore biogears::HeatResistanceAreaUnit::HeatResistanceAreaUnit(std::string const &);
%ignore biogears::HeatResistanceAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatResistanceAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatResistanceArea;
%nspace biogears::HeatResistanceAreaUnit;
%nspace biogears::SEScalarQuantity<HeatResistanceAreaUnit>;

namespace biogears {
   class HeatResistanceAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatResistanceAreaUnit>;

%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatResistanceAreaUnit >::operator!=;

%template(QuantityHeatResistanceArea) biogears::SEScalarQuantity<biogears::HeatResistanceAreaUnit>;

%include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>




