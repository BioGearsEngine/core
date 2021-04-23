
%{
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricResistanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricResistanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricResistance&) const;  

%ignore biogears::ElectricResistanceUnit::ElectricResistanceUnit(std::string const &);
%ignore biogears::ElectricResistanceUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricResistanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricResistance;
%nspace biogears::ElectricResistanceUnit;
%nspace biogears::SEScalarQuantity<ElectricResistanceUnit>;

namespace biogears {
   class ElectricResistanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>;

%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricResistanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricResistanceUnit >::operator!=;

%template(QuantityElectricResistance) biogears::SEScalarQuantity<biogears::ElectricResistanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricResistance.h>




