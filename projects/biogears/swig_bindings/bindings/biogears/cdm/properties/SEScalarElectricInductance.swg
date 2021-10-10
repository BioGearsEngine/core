
%{
#include <biogears/cdm/properties/SEScalarElectricInductance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricInductanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricInductanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricInductanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricInductance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricInductance&) const;  

%ignore biogears::ElectricInductanceUnit::ElectricInductanceUnit(std::string const &);
%ignore biogears::ElectricInductanceUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricInductanceUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricInductance;
%nspace biogears::ElectricInductanceUnit;
%nspace biogears::SEScalarQuantity<ElectricInductanceUnit>;

namespace biogears {
   class ElectricInductanceUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>;

%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricInductanceUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricInductanceUnit >::operator!=;

%template(QuantityElectricInductance) biogears::SEScalarQuantity<biogears::ElectricInductanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricInductance.h>




