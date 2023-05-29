
%{
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimeUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTime&) const;  

%ignore BIOGEARS_NAMESPACE VolumePerTimeUnit::VolumePerTimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarVolumePerTime;
%nspace BIOGEARS_NAMESPACE VolumePerTimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumePerTimeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeUnit >::operator!=;

%template(QuantityVolumePerTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTime.h>




