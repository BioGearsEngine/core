
%{
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToStringToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<FlowResistanceUnit>::ToStringToString(std::ostream& str);

%rename(Equals)  operator==(const FlowResistanceUnit&) const;
%rename(NotEquals)  operator!=(const FlowResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarFlowResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarFlowResistance&) const;  

%ignore biogears::FlowResistanceUnit::FlowResistanceUnit(std::string const &);
%ignore biogears::FlowResistanceUnit::IsValidUnit(std::string const &);
%ignore biogears::FlowResistanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarFlowResistance;
%nspace biogears::FlowResistanceUnit;
%nspace biogears::SEScalarQuantity<FlowResistanceUnit>;

namespace biogears {
   class FlowResistanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::FlowResistanceUnit>;

%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FlowResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowResistanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowResistanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::FlowResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FlowResistanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FlowResistanceUnit >::operator!=;

%template(QuantityFlowResistance) biogears::SEScalarQuantity<biogears::FlowResistanceUnit>;

%include <biogears/cdm/properties/SEScalarFlowResistance.h>




