// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<MassPerTimeUnit>::ToString;

%rename(Equals)  operator==(const MassPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerTime&) const;  

%ignore biogears::MassPerTimeUnit::MassPerTimeUnit(std::string const &);
%ignore biogears::MassPerTimeUnit::IsValidUnit(std::string const &);
%ignore biogears::MassPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMassPerTime;
%nspace biogears::MassPerTimeUnit;
%nspace biogears::SEScalarQuantity<MassPerTimeUnit>;

namespace biogears {
   class MassPerTimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassPerTimeUnit>;

%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< MassPerTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassPerTimeUnit >::operator!=;

%template(QuantityMassPerTime) biogears::SEScalarQuantity<biogears::MassPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerTime.h>




