// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<AreaPerTimePressureUnit>::ToString;

%rename(Equals)  operator==(const AreaPerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const AreaPerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarAreaPerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarAreaPerTimePressure&) const;  

%ignore biogears::AreaPerTimePressureUnit::AreaPerTimePressureUnit(std::string const &);
%ignore biogears::AreaPerTimePressureUnit::IsValidUnit(std::string const &);
%ignore biogears::AreaPerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAreaPerTimePressure;
%nspace biogears::AreaPerTimePressureUnit;
%nspace biogears::SEScalarQuantity<AreaPerTimePressureUnit>;

namespace biogears {
   class AreaPerTimePressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>;

%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::operator!=;

%template(QuantityAreaPerTimePressure) biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>




