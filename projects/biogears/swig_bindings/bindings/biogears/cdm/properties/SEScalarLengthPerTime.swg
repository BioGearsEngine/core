%{
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
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
) SEScalarLengthPerTime
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<LengthPerTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const LengthPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const LengthPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarLengthPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarLengthPerTime&) const;  

%ignore BIOGEARS_NAMESPACE LengthPerTimeUnit::LengthPerTimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthPerTimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarLengthPerTime;
%nspace BIOGEARS_NAMESPACE LengthPerTimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<LengthPerTimeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class LengthPerTimeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimeUnit >::operator!=;

%template(QuantityLengthPerTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarLengthPerTime.h>




