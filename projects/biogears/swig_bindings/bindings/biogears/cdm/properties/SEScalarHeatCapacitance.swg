
%{
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<HeatCapacitanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatCapacitanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitance&) const;  

%ignore biogears::HeatCapacitanceUnit::HeatCapacitanceUnit(std::string const &);
%ignore biogears::HeatCapacitanceUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatCapacitanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatCapacitance;
%nspace biogears::HeatCapacitanceUnit;
%nspace biogears::SEScalarQuantity<HeatCapacitanceUnit>;

namespace biogears {
   class HeatCapacitanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>;

%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatCapacitanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatCapacitanceUnit >::operator!=;

%template(QuantityHeatCapacitance) biogears::SEScalarQuantity<biogears::HeatCapacitanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitance.h>




