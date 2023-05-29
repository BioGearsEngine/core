
%{
#include <biogears/cdm/properties/SEScalarFlowElastance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<FlowElastanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FlowElastanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowElastanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowElastance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowElastance&) const;  

%ignore BIOGEARS_NAMESPACE FlowElastanceUnit::FlowElastanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowElastanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowElastanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarFlowElastance;
%nspace BIOGEARS_NAMESPACE FlowElastanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<FlowElastanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class FlowElastanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< FlowElastanceUnit >::operator!=;

%template(QuantityFlowElastance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowElastanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowElastance.h>




