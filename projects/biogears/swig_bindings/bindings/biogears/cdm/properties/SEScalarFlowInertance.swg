
%{
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<FlowInertanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FlowInertanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowInertanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowInertance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowInertance&) const;  

%ignore BIOGEARS_NAMESPACE FlowInertanceUnit::FlowInertanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowInertanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowInertanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarFlowInertance;
%nspace BIOGEARS_NAMESPACE FlowInertanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<FlowInertanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class FlowInertanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< FlowInertanceUnit >::operator!=;

%template(QuantityFlowInertance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowInertanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowInertance.h>




