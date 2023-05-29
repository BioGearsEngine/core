
%{
#include <biogears/cdm/properties/SEScalarEnergyPerMass.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<EnergyPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const EnergyPerMassUnit&) const;
%rename(NotEquals)  operator!=(const EnergyPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarEnergyPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarEnergyPerMass&) const;  

%ignore BIOGEARS_NAMESPACE EnergyPerMassUnit::EnergyPerMassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyPerMassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE EnergyPerMassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarEnergyPerMass;
%nspace BIOGEARS_NAMESPACE EnergyPerMassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<EnergyPerMassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class EnergyPerMassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< EnergyPerMassUnit >::operator!=;

%template(QuantityEnergyPerMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE EnergyPerMassUnit>;

%include <biogears/cdm/properties/SEScalarEnergyPerMass.h>




