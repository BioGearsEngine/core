// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarFrequency.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<FrequencyUnit>::ToString;

%rename(Equals)  operator==(const FrequencyUnit&) const;
%rename(NotEquals)  operator!=(const FrequencyUnit&) const;
%rename(Equals)  operator==(const SEScalarFrequency&) const;
%rename(NotEquals)  operator!=(const SEScalarFrequency&) const;  

%ignore biogears::FrequencyUnit::FrequencyUnit(std::string const &);
%ignore biogears::FrequencyUnit::IsValidUnit(std::string const &);
%ignore biogears::FrequencyUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarFrequency;
%nspace biogears::FrequencyUnit;
%nspace biogears::SEScalarQuantity<FrequencyUnit>;

namespace biogears {
   class FrequencyUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::FrequencyUnit>;

%ignore  biogears::SEScalarQuantity< FrequencyUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< FrequencyUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FrequencyUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FrequencyUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< FrequencyUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< FrequencyUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< FrequencyUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< FrequencyUnit >::operator!=;

%template(QuantityFrequency) biogears::SEScalarQuantity<biogears::FrequencyUnit>;

%include <biogears/cdm/properties/SEScalarFrequency.h>




