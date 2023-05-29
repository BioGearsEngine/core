
%{
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatResistanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatResistanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatResistance&) const;  

%ignore BIOGEARS_NAMESPACE HeatResistanceUnit::HeatResistanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatResistanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatResistanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatResistance;
%nspace BIOGEARS_NAMESPACE HeatResistanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatResistanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatResistanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceUnit >::operator!=;

%template(QuantityHeatResistance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatResistance.h>




