%{
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
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
) SEScalarInverseVolume
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<InverseVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const InverseVolumeUnit&) const;
%rename(NotEquals)  operator!=(const InverseVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarInverseVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarInverseVolume&) const;  

%ignore BIOGEARS_NAMESPACE InverseVolumeUnit::InverseVolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE InverseVolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE InverseVolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarInverseVolume;
%nspace BIOGEARS_NAMESPACE InverseVolumeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<InverseVolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class InverseVolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< InverseVolumeUnit >::operator!=;

%template(QuantityInverseVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InverseVolumeUnit>;

%include <biogears/cdm/properties/SEScalarInverseVolume.h>




