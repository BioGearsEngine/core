// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<PressureTimePerVolumeAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressureTimePerVolumeAreaUnit&) const;
%rename(NotEquals)  operator!=(const PressureTimePerVolumeAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarPressureTimePerVolumeArea&) const;
%rename(NotEquals)  operator!=(const SEScalarPressureTimePerVolumeArea&) const;  

%ignore biogears::PressureTimePerVolumeAreaUnit::PressureTimePerVolumeAreaUnit(std::string const &);
%ignore biogears::PressureTimePerVolumeAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::PressureTimePerVolumeAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPressureTimePerVolumeArea;
%nspace biogears::PressureTimePerVolumeAreaUnit;
%nspace biogears::SEScalarQuantity<PressureTimePerVolumeAreaUnit>;

namespace biogears {
   class PressureTimePerVolumeAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>;

%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PressureTimePerVolumeAreaUnit >::operator!=;

%template(QuantityPressureTimePerVolumeArea) biogears::SEScalarQuantity<biogears::PressureTimePerVolumeAreaUnit>;

%include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>




