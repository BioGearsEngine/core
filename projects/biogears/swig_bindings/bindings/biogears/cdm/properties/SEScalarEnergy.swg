%{
#include <biogears/cdm/properties/SEScalarEnergy.h>
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
) SEScalarEnergy
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<EnergyUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const EnergyUnit&) const;
%rename(NotEquals)  operator!=(const EnergyUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergy&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergy&) const;  

%ignore BIOGEARS_NAMESPACE EnergyUnit::EnergyUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarEnergy;
%nspace BIOGEARS_NAMESPACE EnergyUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<EnergyUnit>;

OPEN_BIOGEARS_NAMESPACE
   class EnergyUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyUnit >::operator!=;

%template(QuantityEnergy) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyUnit>;

%include <biogears/cdm/properties/SEScalarEnergy.h>




