%{
#include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>
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
) SEScalarPressureTimePerVolumeArea
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<PressureTimePerVolumeAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const PressureTimePerVolumeAreaUnit&) const;
%rename(NotEquals)  operator!=(const PressureTimePerVolumeAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarPressureTimePerVolumeArea&) const;
%rename(NotEquals)  operator!=(const SEScalarPressureTimePerVolumeArea&) const;  

%ignore BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit::PressureTimePerVolumeAreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarPressureTimePerVolumeArea;
%nspace BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<PressureTimePerVolumeAreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class PressureTimePerVolumeAreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< PressureTimePerVolumeAreaUnit >::operator!=;

%template(QuantityPressureTimePerVolumeArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE PressureTimePerVolumeAreaUnit>;

%include <biogears/cdm/properties/SEScalarPressureTimePerVolumeArea.h>




