
%{
#include <biogears/cdm/properties/SEScalarElectricCapacitance.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricCapacitanceUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricCapacitanceUnit&) const;
%rename(NotEquals)  operator!=(const ElectricCapacitanceUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCapacitance&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCapacitance&) const;  

%ignore BIOGEARS_NAMESPACE ElectricCapacitanceUnit::ElectricCapacitanceUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricCapacitanceUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricCapacitanceUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricCapacitance;
%nspace BIOGEARS_NAMESPACE ElectricCapacitanceUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricCapacitanceUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricCapacitanceUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCapacitanceUnit >::operator!=;

%template(QuantityElectricCapacitance) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCapacitanceUnit>;

%include <biogears/cdm/properties/SEScalarElectricCapacitance.h>




