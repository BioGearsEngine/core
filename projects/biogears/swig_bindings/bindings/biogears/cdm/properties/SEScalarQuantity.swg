

%{
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEUnitScalar.swg>

%nspace BIOGEARS_NAMESPACE SEScalarQuantity;
%nspace BIOGEARS_NAMESPACE QuantityUnit;

%ignore BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const SEScalarQuantity&) const;
%rename(NotEquals)  operator!=(const SEScalarQuantity&) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity::ToString() const "public override";
#endif 

%ignore  SEScalarQuantity::GetValue(std::string const &) const ;
%ignore  SEScalarQuantity::IncrementValue(double,std::string const &) ;
%ignore  SEScalarQuantity::IncrementValue(double,char const *) ;
%ignore  SEScalarQuantity::DecrementValue(double,std::string const &) ;
%ignore  SEScalarQuantity::DecrementValue(double,char const *) ;
%ignore  SEScalarQuantity::GetCompoundUnit(std::string const &) const ;

%include <biogears/cdm/properties/SEScalarQuantity.h>


