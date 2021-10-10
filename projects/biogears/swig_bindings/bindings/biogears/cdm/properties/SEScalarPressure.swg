
%{
#include <biogears/cdm/properties/SEScalarPressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<PressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressureUnit&) const;
%rename(NotEquals)  operator!=(const PressureUnit&) const;
%rename(Equals)  operator==(const SEScalarPressure&) const;
%rename(NotEquals)  operator!=(const SEScalarPressure&) const;  

%ignore biogears::PressureUnit::PressureUnit(std::string const &);
%ignore biogears::PressureUnit::IsValidUnit(std::string const &);
%ignore biogears::PressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPressure;
%nspace biogears::PressureUnit;
%nspace biogears::SEScalarQuantity<PressureUnit>;

namespace biogears {
   class PressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PressureUnit>;

%ignore  biogears::SEScalarQuantity< PressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PressureUnit >::operator!=;

%template(QuantityPressure) biogears::SEScalarQuantity<biogears::PressureUnit>;

%include <biogears/cdm/properties/SEScalarPressure.h>




