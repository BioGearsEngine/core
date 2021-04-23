// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<HeatCapacitancePerMassUnit>::ToString;

%rename(Equals)  operator==(const HeatCapacitancePerMassUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitancePerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitancePerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitancePerMass&) const;  

%ignore biogears::HeatCapacitancePerMassUnit::HeatCapacitancePerMassUnit(std::string const &);
%ignore biogears::HeatCapacitancePerMassUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatCapacitancePerMassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatCapacitancePerMass;
%nspace biogears::HeatCapacitancePerMassUnit;
%nspace biogears::SEScalarQuantity<HeatCapacitancePerMassUnit>;

namespace biogears {
   class HeatCapacitancePerMassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatCapacitancePerMassUnit>;

%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatCapacitancePerMassUnit >::operator!=;

%template(QuantityHeatCapacitancePerMass) biogears::SEScalarQuantity<biogears::HeatCapacitancePerMassUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>




