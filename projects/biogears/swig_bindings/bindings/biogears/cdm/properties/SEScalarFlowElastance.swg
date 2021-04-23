
%{
#include <biogears/cdm/properties/SEScalarFlowElastance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<FlowElastanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FlowElastanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowElastanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowElastance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowElastance&) const;  

%ignore biogears::FlowElastanceUnit::FlowElastanceUnit(std::string const &);
%ignore biogears::FlowElastanceUnit::IsValidUnit(std::string const &);
%ignore biogears::FlowElastanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarFlowElastance;
%nspace biogears::FlowElastanceUnit;
%nspace biogears::SEScalarQuantity<FlowElastanceUnit>;

namespace biogears {
   class FlowElastanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::FlowElastanceUnit>;

%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowElastanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowElastanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FlowElastanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FlowElastanceUnit >::operator!=;

%template(QuantityFlowElastance) biogears::SEScalarQuantity<biogears::FlowElastanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowElastance.h>




