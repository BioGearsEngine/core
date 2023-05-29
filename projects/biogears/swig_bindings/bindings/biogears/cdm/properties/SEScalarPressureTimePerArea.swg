
%{
#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<PressureTimePerAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressureTimePerAreaUnit&) const;
%rename(NotEquals)  operator!=(const PressureTimePerAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarPressureTimePerArea&) const;
%rename(NotEquals)  operator!=(const SEScalarPressureTimePerArea&) const;  

%ignore BIOGEARS_NAMESPACE PressureTimePerAreaUnit::PressureTimePerAreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressureTimePerAreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressureTimePerAreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarPressureTimePerArea;
%nspace BIOGEARS_NAMESPACE PressureTimePerAreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<PressureTimePerAreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class PressureTimePerAreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerAreaUnit >::operator!=;

%template(QuantityPressureTimePerArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerAreaUnit>;

%include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>




