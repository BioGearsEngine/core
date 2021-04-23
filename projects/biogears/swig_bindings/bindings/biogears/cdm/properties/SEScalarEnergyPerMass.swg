
%{
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<EnergyPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const EnergyPerMassUnit&) const;
%rename(NotEquals)  operator!=(const EnergyPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergyPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergyPerMass&) const;  

%ignore biogears::EnergyPerMassUnit::EnergyPerMassUnit(std::string const &);
%ignore biogears::EnergyPerMassUnit::IsValidUnit(std::string const &);
%ignore biogears::EnergyPerMassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarEnergyPerMass;
%nspace biogears::EnergyPerMassUnit;
%nspace biogears::SEScalarQuantity<EnergyPerMassUnit>;

namespace biogears {
   class EnergyPerMassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>;

%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< EnergyPerMassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< EnergyPerMassUnit >::operator!=;

%template(QuantityEnergyPerMass) biogears::SEScalarQuantity<biogears::EnergyPerMassUnit>;

%include <biogears/cdm/properties/SEScalarEnergyPerMass.h>




