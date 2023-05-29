
%{
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimePressure&) const;  

%ignore BIOGEARS_NAMESPACE VolumePerTimePressureUnit::VolumePerTimePressureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimePressureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarVolumePerTimePressure;
%nspace BIOGEARS_NAMESPACE VolumePerTimePressureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimePressureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumePerTimePressureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>;


%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureUnit >::operator!=;

%template(QuantityVolumePerTimePressure) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>




