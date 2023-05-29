
%{
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatCapacitanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitance&) const;  

%ignore BIOGEARS_NAMESPACE HeatCapacitanceUnit::HeatCapacitanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatCapacitance;
%nspace BIOGEARS_NAMESPACE HeatCapacitanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatCapacitanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitanceUnit >::operator!=;

%template(QuantityHeatCapacitance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitance.h>




