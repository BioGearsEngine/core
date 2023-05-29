
%{
#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<LengthPerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const LengthPerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const LengthPerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarLengthPerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarLengthPerTimePressure&) const;  

%ignore BIOGEARS_NAMESPACE LengthPerTimePressureUnit::LengthPerTimePressureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthPerTimePressureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthPerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarLengthPerTimePressure;
%nspace BIOGEARS_NAMESPACE LengthPerTimePressureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<LengthPerTimePressureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class LengthPerTimePressureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< LengthPerTimePressureUnit >::operator!=;

%template(QuantityLengthPerTimePressure) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthPerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>




