
%{
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<FlowComplianceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FlowComplianceUnit&) const;
%rename(NotEquals)  operator!=(const FlowComplianceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowCompliance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowCompliance&) const;  

%ignore biogears::FlowComplianceUnit::FlowComplianceUnit(std::string const &);
%ignore biogears::FlowComplianceUnit::IsValidUnit(std::string const &);
%ignore biogears::FlowComplianceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarFlowCompliance;
%nspace biogears::FlowComplianceUnit;
%nspace biogears::SEScalarQuantity<FlowComplianceUnit>;

namespace biogears {
   class FlowComplianceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::FlowComplianceUnit>;

%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowComplianceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowComplianceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowComplianceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowComplianceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowComplianceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FlowComplianceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FlowComplianceUnit >::operator!=;

%template(QuantityFlowCompliance) biogears::SEScalarQuantity<biogears::FlowComplianceUnit>;

%include <biogears/cdm/properties/SEScalarFlowCompliance.h>




