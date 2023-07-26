%{
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>
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
) SEScalarVolumePerTimePressureArea
#endif


%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimePressureAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimePressureAreaUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimePressureAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimePressureArea&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimePressureArea&) const;  

%ignore BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit::VolumePerTimePressureAreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarVolumePerTimePressureArea;
%nspace BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<VolumePerTimePressureAreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class VolumePerTimePressureAreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>;


%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< VolumePerTimePressureAreaUnit >::operator!=;

%template(QuantityVolumePerTimePressureArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE VolumePerTimePressureAreaUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>




