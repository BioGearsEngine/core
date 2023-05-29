
%{
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatResistanceAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatResistanceAreaUnit&) const;
%rename(NotEquals)  operator!=(const HeatResistanceAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatResistanceArea&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatResistanceArea&) const;  

%ignore BIOGEARS_NAMESPACE HeatResistanceAreaUnit::HeatResistanceAreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatResistanceAreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatResistanceAreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatResistanceArea;
%nspace BIOGEARS_NAMESPACE HeatResistanceAreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatResistanceAreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatResistanceAreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatResistanceAreaUnit >::operator!=;

%template(QuantityHeatResistanceArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatResistanceAreaUnit>;

%include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>




