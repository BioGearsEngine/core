
%{
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<AreaPerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AreaPerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const AreaPerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarAreaPerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarAreaPerTimePressure&) const;  

%ignore BIOGEARS_NAMESPACE AreaPerTimePressureUnit::AreaPerTimePressureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AreaPerTimePressureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE AreaPerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarAreaPerTimePressure;
%nspace BIOGEARS_NAMESPACE AreaPerTimePressureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<AreaPerTimePressureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class AreaPerTimePressureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< AreaPerTimePressureUnit >::operator!=;

%template(QuantityAreaPerTimePressure) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE AreaPerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>




