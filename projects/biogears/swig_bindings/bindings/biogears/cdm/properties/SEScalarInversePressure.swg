// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarInversePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<InversePressureUnit>::ToString;

%rename(Equals)  operator==(const InversePressureUnit&) const;
%rename(NotEquals)  operator!=(const InversePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarInversePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarInversePressure&) const;  

%ignore biogears::InversePressureUnit::InversePressureUnit(std::string const &);
%ignore biogears::InversePressureUnit::IsValidUnit(std::string const &);
%ignore biogears::InversePressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarInversePressure;
%nspace biogears::InversePressureUnit;
%nspace biogears::SEScalarQuantity<InversePressureUnit>;

namespace biogears {
   class InversePressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::InversePressureUnit>;

%ignore  biogears::SEScalarQuantity< InversePressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< InversePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< InversePressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< InversePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< InversePressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< InversePressureUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< InversePressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< InversePressureUnit >::operator!=;

%template(QuantityInversePressure) biogears::SEScalarQuantity<biogears::InversePressureUnit>;

%include <biogears/cdm/properties/SEScalarInversePressure.h>




