

%{
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>


%import <biogears/cdm/properties/SEScalarQuantity.swg>

%nspace BIOGEARS_NAMESPACE SEScalarVolume;
%nspace BIOGEARS_NAMESPACE VolumeUnit;

%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumeUnit&) const;
%rename(NotEquals)  operator!=(const VolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarVolume&) const;  

%ignore BIOGEARS_NAMESPACE VolumeUnit::VolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>::DivideValue "public new";
#endif 

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity<VolumeUnit>::operator!=;

%template(QuantityVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumeUnit>;

%include <biogears/cdm/properties/SEScalarVolume.h>


