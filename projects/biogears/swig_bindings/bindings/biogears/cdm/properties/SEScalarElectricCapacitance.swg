
%{
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricCapacitanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricCapacitanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricCapacitanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCapacitance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCapacitance&) const;  

%ignore biogears::ElectricCapacitanceUnit::ElectricCapacitanceUnit(std::string const &);
%ignore biogears::ElectricCapacitanceUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricCapacitanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricCapacitance;
%nspace biogears::ElectricCapacitanceUnit;
%nspace biogears::SEScalarQuantity<ElectricCapacitanceUnit>;

namespace biogears {
   class ElectricCapacitanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>;

%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricCapacitanceUnit >::operator!=;

%template(QuantityElectricCapacitance) biogears::SEScalarQuantity<biogears::ElectricCapacitanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricCapacitance.h>




