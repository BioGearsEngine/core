
%{
#include <biogears/cdm/properties/SEScalarPressurePerVolume.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<PressurePerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressurePerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const PressurePerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarPressurePerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarPressurePerVolume&) const;  

%ignore BIOGEARS_NAMESPACE PressurePerVolumeUnit::PressurePerVolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressurePerVolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressurePerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarPressurePerVolume;
%nspace BIOGEARS_NAMESPACE PressurePerVolumeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<PressurePerVolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class PressurePerVolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< PressurePerVolumeUnit >::operator!=;

%template(QuantityPressurePerVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressurePerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarPressurePerVolume.h>




