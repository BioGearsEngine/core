
%{
#include <biogears/cdm/properties/SEScalarForce.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToStringToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ForceUnit>::ToStringToString(std::ostream& str);

%rename(Equals)  operator==(const ForceUnit&) const;
%rename(NotEquals)  operator!=(const ForceUnit&) const;
%rename(Equals)  operator==(const SEScalarForce&) const;
%rename(NotEquals)  operator!=(const SEScalarForce&) const;  

%ignore BIOGEARS_NAMESPACE ForceUnit::ForceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ForceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ForceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarForce;
%nspace BIOGEARS_NAMESPACE ForceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ForceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ForceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ForceUnit >::operator!=;

%template(QuantityForce) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ForceUnit>;

%include <biogears/cdm/properties/SEScalarForce.h>




