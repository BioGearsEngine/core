
%{
#include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<PressureTimePerAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressureTimePerAreaUnit&) const;
%rename(NotEquals)  operator!=(const PressureTimePerAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarPressureTimePerArea&) const;
%rename(NotEquals)  operator!=(const SEScalarPressureTimePerArea&) const;  

%ignore biogears::PressureTimePerAreaUnit::PressureTimePerAreaUnit(std::string const &);
%ignore biogears::PressureTimePerAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::PressureTimePerAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPressureTimePerArea;
%nspace biogears::PressureTimePerAreaUnit;
%nspace biogears::SEScalarQuantity<PressureTimePerAreaUnit>;

namespace biogears {
   class PressureTimePerAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>;

%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PressureTimePerAreaUnit >::operator!=;

%template(QuantityPressureTimePerArea) biogears::SEScalarQuantity<biogears::PressureTimePerAreaUnit>;

%include <biogears/cdm/properties/SEScalarPressureTimePerArea.h>




