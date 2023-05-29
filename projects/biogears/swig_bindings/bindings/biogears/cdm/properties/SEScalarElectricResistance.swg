
%{
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricResistanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricResistanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricResistanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricResistance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricResistance&) const;  

%ignore BIOGEARS_NAMESPACE ElectricResistanceUnit::ElectricResistanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricResistanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricResistanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricResistance;
%nspace BIOGEARS_NAMESPACE ElectricResistanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricResistanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricResistanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricResistanceUnit >::operator!=;

%template(QuantityElectricResistance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricResistanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricResistance.h>




