
%{
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToStringToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<FlowResistanceUnit>::ToStringToString(std::ostream& str);

%rename(Equals)  operator==(const FlowResistanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowResistance&) const;  

%ignore BIOGEARS_NAMESPACE FlowResistanceUnit::FlowResistanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowResistanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowResistanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarFlowResistance;
%nspace BIOGEARS_NAMESPACE FlowResistanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<FlowResistanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class FlowResistanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< FlowResistanceUnit >::operator!=;

%template(QuantityFlowResistance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowResistanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowResistance.h>




