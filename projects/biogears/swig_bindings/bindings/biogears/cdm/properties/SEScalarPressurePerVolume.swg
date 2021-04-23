// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<PressurePerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressurePerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const PressurePerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarPressurePerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarPressurePerVolume&) const;  

%ignore biogears::PressurePerVolumeUnit::PressurePerVolumeUnit(std::string const &);
%ignore biogears::PressurePerVolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::PressurePerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarPressurePerVolume;
%nspace biogears::PressurePerVolumeUnit;
%nspace biogears::SEScalarQuantity<PressurePerVolumeUnit>;

namespace biogears {
   class PressurePerVolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>;

%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< PressurePerVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< PressurePerVolumeUnit >::operator!=;

%template(QuantityPressurePerVolume) biogears::SEScalarQuantity<biogears::PressurePerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarPressurePerVolume.h>




