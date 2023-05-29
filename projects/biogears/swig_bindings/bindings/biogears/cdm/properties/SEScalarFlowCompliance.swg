
%{
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<FlowComplianceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FlowComplianceUnit&) const;
%rename(NotEquals)  operator!=(const FlowComplianceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowCompliance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowCompliance&) const;  

%ignore BIOGEARS_NAMESPACE FlowComplianceUnit::FlowComplianceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowComplianceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FlowComplianceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarFlowCompliance;
%nspace BIOGEARS_NAMESPACE FlowComplianceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<FlowComplianceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class FlowComplianceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< FlowComplianceUnit >::operator!=;

%template(QuantityFlowCompliance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FlowComplianceUnit>;

%include <biogears/cdm/properties/SEScalarFlowCompliance.h>




