%{
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
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
) SEScalarMassPerVolume
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerVolume&) const;  

%ignore BIOGEARS_NAMESPACE MassPerVolumeUnit::MassPerVolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerVolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMassPerVolume;
%nspace BIOGEARS_NAMESPACE MassPerVolumeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassPerVolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassPerVolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerVolumeUnit >::operator!=;

%template(QuantityMassPerVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerVolume.h>;

%nspace  std::vector<BIOGEARS_NAMESPACE SEScalarMassPerVolume*>;
%template (vector_SEScalarMasPerVolume_ptr)  std::vector<BIOGEARS_NAMESPACE SEScalarMassPerVolume*>;
