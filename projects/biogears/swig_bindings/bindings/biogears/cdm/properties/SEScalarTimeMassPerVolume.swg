
%{
#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<TimeMassPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const TimeMassPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const TimeMassPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarTimeMassPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarTimeMassPerVolume&) const;  

%ignore BIOGEARS_NAMESPACE TimeMassPerVolumeUnit::TimeMassPerVolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TimeMassPerVolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TimeMassPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarTimeMassPerVolume;
%nspace BIOGEARS_NAMESPACE TimeMassPerVolumeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TimeMassPerVolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class TimeMassPerVolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< TimeMassPerVolumeUnit >::operator!=;

%template(QuantityTimeMassPerVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeMassPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>




