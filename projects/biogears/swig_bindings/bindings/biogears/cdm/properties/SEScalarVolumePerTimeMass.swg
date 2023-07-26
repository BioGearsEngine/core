%{
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEScalarVolumePerTimeMass
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimeMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimeMassUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimeMassUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimeMass&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimeMass&) const;  

%ignore BIOGEARS_NAMESPACE VolumePerTimeMassUnit::VolumePerTimeMassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimeMassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimeMassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarVolumePerTimeMass;
%nspace BIOGEARS_NAMESPACE VolumePerTimeMassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimeMassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumePerTimeMassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>;


%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimeMassUnit >::operator!=;

%template(QuantityVolumePerTimeMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimeMassUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>




