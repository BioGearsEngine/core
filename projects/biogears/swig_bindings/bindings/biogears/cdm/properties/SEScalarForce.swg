// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarForce.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<ForceUnit>::ToString;

%rename(Equals)  operator==(const ForceUnit&) const;
%rename(NotEquals)  operator!=(const ForceUnit&) const;
%rename(Equals)  operator==(const SEScalarForce&) const;
%rename(NotEquals)  operator!=(const SEScalarForce&) const;  

%ignore biogears::ForceUnit::ForceUnit(std::string const &);
%ignore biogears::ForceUnit::IsValidUnit(std::string const &);
%ignore biogears::ForceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarForce;
%nspace biogears::ForceUnit;
%nspace biogears::SEScalarQuantity<ForceUnit>;

namespace biogears {
   class ForceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ForceUnit>;

%ignore  biogears::SEScalarQuantity< ForceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ForceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ForceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ForceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ForceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ForceUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ForceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ForceUnit >::operator!=;

%template(QuantityForce) biogears::SEScalarQuantity<biogears::ForceUnit>;

%include <biogears/cdm/properties/SEScalarForce.h>




