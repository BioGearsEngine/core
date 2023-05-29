
%{
#include <biogears/cdm/properties/SEScalarFrequency.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<FrequencyUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const FrequencyUnit&) const;
%rename(NotEquals)  operator!=(const FrequencyUnit&) const;
%rename(Equals)  operator==(const SEScalarFrequency&) const;
%rename(NotEquals)  operator!=(const SEScalarFrequency&) const;  

%ignore BIOGEARS_NAMESPACE FrequencyUnit::FrequencyUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FrequencyUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE FrequencyUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarFrequency;
%nspace BIOGEARS_NAMESPACE FrequencyUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<FrequencyUnit>;

OPEN_BIOGEARS_NAMESPACE
   class FrequencyUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< FrequencyUnit >::operator!=;

%template(QuantityFrequency) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE FrequencyUnit>;

%include <biogears/cdm/properties/SEScalarFrequency.h>




