
%{
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<HeatResistanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatResistanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatResistance&) const;  

%ignore biogears::HeatResistanceUnit::HeatResistanceUnit(std::string const &);
%ignore biogears::HeatResistanceUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatResistanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatResistance;
%nspace biogears::HeatResistanceUnit;
%nspace biogears::SEScalarQuantity<HeatResistanceUnit>;

namespace biogears {
   class HeatResistanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatResistanceUnit>;

%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatResistanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatResistanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatResistanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatResistanceUnit >::operator!=;

%template(QuantityHeatResistance) biogears::SEScalarQuantity<biogears::HeatResistanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatResistance.h>




