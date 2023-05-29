
%{
#include <biogears/cdm/properties/SEScalarInversePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<InversePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const InversePressureUnit&) const;
%rename(NotEquals)  operator!=(const InversePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarInversePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarInversePressure&) const;  

%ignore BIOGEARS_NAMESPACE InversePressureUnit::InversePressureUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE InversePressureUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE InversePressureUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarInversePressure;
%nspace BIOGEARS_NAMESPACE InversePressureUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<InversePressureUnit>;

OPEN_BIOGEARS_NAMESPACE
   class InversePressureUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< InversePressureUnit >::operator!=;

%template(QuantityInversePressure) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE InversePressureUnit>;

%include <biogears/cdm/properties/SEScalarInversePressure.h>




