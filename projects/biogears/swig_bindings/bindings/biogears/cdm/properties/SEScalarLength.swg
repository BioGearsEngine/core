
%{
#include <biogears/cdm/properties/SEScalarLength.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<LengthUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const LengthUnit&) const;
%rename(NotEquals)  operator!=(const LengthUnit&) const;
%rename(Equals)  operator==(const SEScalarLength&) const;
%rename(NotEquals)  operator!=(const SEScalarLength&) const;  

%ignore BIOGEARS_NAMESPACE LengthUnit::LengthUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE LengthUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarLength;
%nspace BIOGEARS_NAMESPACE LengthUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<LengthUnit>;

%rename(inch)       LengthUnit::in;
OPEN_BIOGEARS_NAMESPACE
   class LengthUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< LengthUnit >::operator!=;
%template(QuantityLength) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE LengthUnit>;

%include <biogears/cdm/properties/SEScalarLength.h>




