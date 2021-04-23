// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricCurrentUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricCurrentUnit&) const;
%rename(NotEquals)  operator!=(const ElectricCurrentUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCurrent&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCurrent&) const;  

%ignore biogears::ElectricCurrentUnit::ElectricCurrentUnit(std::string const &);
%ignore biogears::ElectricCurrentUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricCurrentUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricCurrent;
%nspace biogears::ElectricCurrentUnit;
%nspace biogears::SEScalarQuantity<ElectricCurrentUnit>;

namespace biogears {
   class ElectricCurrentUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>;

%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricCurrentUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricCurrentUnit >::operator!=;

%template(QuantityElectricCurrent) biogears::SEScalarQuantity<biogears::ElectricCurrentUnit>;

%include <biogears/cdm/properties/SEScalarElectricCurrent.h>




