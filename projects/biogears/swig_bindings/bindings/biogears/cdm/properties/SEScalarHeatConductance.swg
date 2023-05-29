
%{
#include <biogears/cdm/properties/SEScalarHeatConductance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatConductanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatConductanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatConductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatConductance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatConductance&) const;  

%ignore BIOGEARS_NAMESPACE HeatConductanceUnit::HeatConductanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatConductanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatConductanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatConductance;
%nspace BIOGEARS_NAMESPACE HeatConductanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatConductanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatConductanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductanceUnit >::operator!=;

%template(QuantityHeatConductance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatConductance.h>




