
%{
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatInductanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatInductanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatInductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatInductance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatInductance&) const;  

%ignore BIOGEARS_NAMESPACE HeatInductanceUnit::HeatInductanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatInductanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatInductanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatInductance;
%nspace BIOGEARS_NAMESPACE HeatInductanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatInductanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatInductanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatInductanceUnit >::operator!=;

%template(QuantityHeatInductance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatInductanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatInductance.h>




