
%{
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<FlowInertanceUnit>::ToString(std::ostream& str);

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
%ignore  biogears::SEScalarQuantity< FlowInertanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowInertanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowInertanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowInertanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowInertanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FlowInertanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FlowInertanceUnit >::operator!=;

%template(QuantityFlowInertance) biogears::SEScalarQuantity<biogears::FlowInertanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowInertance.h>




