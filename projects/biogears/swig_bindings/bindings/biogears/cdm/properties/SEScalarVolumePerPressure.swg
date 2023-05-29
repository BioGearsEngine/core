
%{
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerPressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerPressureUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerPressureUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerPressure&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerPressure&) const;  

%ignore BIOGEARS_NAMESPACE VolumePerPressureUnit::VolumePerPressureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerPressureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerPressureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarVolumePerPressure;
%nspace BIOGEARS_NAMESPACE VolumePerPressureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerPressureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumePerPressureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerPressureUnit >::operator!=;

%template(QuantityVolumePerPressure) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerPressureUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerPressure.h>




