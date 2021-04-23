// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<LengthPerTimeUnit>::ToString;

%rename(Equals)  operator==(const LengthPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const LengthPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarLengthPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarLengthPerTime&) const;  

%ignore biogears::LengthPerTimeUnit::LengthPerTimeUnit(std::string const &);
%ignore biogears::LengthPerTimeUnit::IsValidUnit(std::string const &);
%ignore biogears::LengthPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarLengthPerTime;
%nspace biogears::LengthPerTimeUnit;
%nspace biogears::SEScalarQuantity<LengthPerTimeUnit>;

namespace biogears {
   class LengthPerTimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::LengthPerTimeUnit>;

%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< LengthPerTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< LengthPerTimeUnit >::operator!=;

%template(QuantityLengthPerTime) biogears::SEScalarQuantity<biogears::LengthPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarLengthPerTime.h>




