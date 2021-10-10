
%{
#include <biogears/cdm/properties/SEScalarHeatInductance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<HeatInductanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatInductanceUnit&) const;
%rename(NotEquals)  operator!=(const HeatInductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatInductance&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatInductance&) const;  

%ignore biogears::HeatInductanceUnit::HeatInductanceUnit(std::string const &);
%ignore biogears::HeatInductanceUnit::IsValidUnit(std::string const &);
%ignore biogears::HeatInductanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarHeatInductance;
%nspace biogears::HeatInductanceUnit;
%nspace biogears::SEScalarQuantity<HeatInductanceUnit>;

namespace biogears {
   class HeatInductanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::HeatInductanceUnit>;

%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< HeatInductanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatInductanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatInductanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatInductanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::HeatInductanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< HeatInductanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< HeatInductanceUnit >::operator!=;

%template(QuantityHeatInductance) biogears::SEScalarQuantity<biogears::HeatInductanceUnit>;

%include <biogears/cdm/properties/SEScalarHeatInductance.h>




