%{
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
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
) SEScalarElectricInductance
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricInductanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricInductanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricInductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricInductance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricInductance&) const;  

%ignore BIOGEARS_NAMESPACE ElectricInductanceUnit::ElectricInductanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricInductanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricInductanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricInductance;
%nspace BIOGEARS_NAMESPACE ElectricInductanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricInductanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricInductanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricInductanceUnit >::operator!=;

%template(QuantityElectricInductance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricInductanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricInductance.h>




