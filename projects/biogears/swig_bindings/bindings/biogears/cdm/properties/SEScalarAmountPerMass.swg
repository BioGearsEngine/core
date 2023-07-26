%{
#include <biogears/cdm/properties/SEScalarAmountPerMass.h>
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
) SEScalarAmountPerMass
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerMassUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerMass&) const;  

%ignore BIOGEARS_NAMESPACE AmountPerMassUnit::AmountPerMassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerMassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountPerMassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarAmountPerMass;
%nspace BIOGEARS_NAMESPACE AmountPerMassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AmountPerMassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AmountPerMassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AmountPerMassUnit >::operator!=;

%template(QuantityAmountPerMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountPerMassUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerMass.h>




