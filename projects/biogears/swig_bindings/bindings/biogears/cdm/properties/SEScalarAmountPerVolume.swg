%{
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
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
) SEScalarAmountPerVolume
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerVolume&) const;  

%ignore BIOGEARS_NAMESPACE AmountPerVolumeUnit::AmountPerVolumeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerVolumeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarAmountPerVolume;
%nspace BIOGEARS_NAMESPACE AmountPerVolumeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerVolumeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AmountPerVolumeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerVolumeUnit >::operator!=;

%template(QuantityAmountPerVolume) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerVolume.h>




