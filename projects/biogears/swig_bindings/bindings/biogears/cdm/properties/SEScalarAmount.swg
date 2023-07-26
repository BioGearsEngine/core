%{
#include <biogears/cdm/properties/SEScalarAmount.h>
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
) SEScalarAmount
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AmountUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountUnit&) const;
%rename(NotEquals)  operator!=(const AmountUnit&) const;
%rename(Equals)  operator==(const SEScalarAmount&) const;
%rename(NotEquals)  operator!=(const SEScalarAmount&) const;  

%ignore BIOGEARS_NAMESPACE AmountUnit::AmountUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AmountUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarAmount;
%nspace BIOGEARS_NAMESPACE AmountUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AmountUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AmountUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AmountUnit >::operator!=;


%template(QuantityAmount) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AmountUnit>;

%include <biogears/cdm/properties/SEScalarAmount.h>




