
%{
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<HeatConductancePerAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatConductancePerAreaUnit&) const;
%rename(NotEquals)  operator!=(const HeatConductancePerAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatConductancePerArea&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatConductancePerArea&) const;  

%ignore biogears::HeatConductancePerAreaUnit::HeatConductancePerAreaUnit(std::string const &);
%ignore biogears::HeatConductancePerAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatConductancePerAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatConductancePerArea;
%nspace biogears::HeatConductancePerAreaUnit;
%nspace biogears::SEScalarQuantity<HeatConductancePerAreaUnit>;

namespace biogears {
   class HeatConductancePerAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>;

%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatConductancePerAreaUnit >::operator!=;

%template(QuantityHeatConductancePerArea) biogears::SEScalarQuantity<biogears::HeatConductancePerAreaUnit>;

%include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>




