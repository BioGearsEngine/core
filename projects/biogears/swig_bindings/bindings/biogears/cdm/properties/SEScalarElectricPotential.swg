// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricPotentialUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricPotentialUnit&) const;
%rename(NotEquals)  operator!=(const ElectricPotentialUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricPotential&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricPotential&) const;  

%ignore biogears::ElectricPotentialUnit::ElectricPotentialUnit(std::string const &);
%ignore biogears::ElectricPotentialUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricPotentialUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricPotential;
%nspace biogears::ElectricPotentialUnit;
%nspace biogears::SEScalarQuantity<ElectricPotentialUnit>;

namespace biogears {
   class ElectricPotentialUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>;

%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricPotentialUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricPotentialUnit >::operator!=;

%template(QuantityElectricPotential) biogears::SEScalarQuantity<biogears::ElectricPotentialUnit>;

%include <biogears/cdm/properties/SEScalarElectricPotential.h>




