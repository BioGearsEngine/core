
%{
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/cdm/properties/SEUnitScalar.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AreaUnit&) const;
%rename(NotEquals)  operator!=(const AreaUnit&) const;
%rename(Equals)  operator==(const SEScalarArea&) const;
%rename(NotEquals)  operator!=(const SEScalarArea&) const;  

%ignore BIOGEARS_NAMESPACE AreaUnit::AreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarArea;
%nspace BIOGEARS_NAMESPACE AreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AreaUnit >::operator!=;

%template(QuantityArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaUnit>;

%include <biogears/cdm/properties/SEScalarArea.h>




