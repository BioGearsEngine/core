// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<FlowInertanceUnit>::ToString;

%rename(Equals)  operator==(const FlowInertanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowInertanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowInertance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowInertance&) const;  

%ignore biogears::FlowInertanceUnit::FlowInertanceUnit(std::string const &);
%ignore biogears::FlowInertanceUnit::IsValidUnit(std::string const &);
%ignore biogears::FlowInertanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarFlowInertance;
%nspace biogears::FlowInertanceUnit;
%nspace biogears::SEScalarQuantity<FlowInertanceUnit>;

namespace biogears {
   class FlowInertanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::FlowInertanceUnit>;

%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FlowInertanceUnit >::operator!=;

%template(QuantityFlowInertance) biogears::SEScalarQuantity<biogears::FlowInertanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowInertance.h>




