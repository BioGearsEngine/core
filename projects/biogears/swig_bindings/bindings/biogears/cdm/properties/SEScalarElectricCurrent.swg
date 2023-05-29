
%{
#include <biogears/cdm/properties/SEScalarElectricCurrent.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricCurrentUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricCurrentUnit&) const;
%rename(NotEquals)  operator!=(const ElectricCurrentUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCurrent&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCurrent&) const;  

%ignore BIOGEARS_NAMESPACE ElectricCurrentUnit::ElectricCurrentUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricCurrentUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricCurrentUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricCurrent;
%nspace BIOGEARS_NAMESPACE ElectricCurrentUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricCurrentUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricCurrentUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricCurrentUnit >::operator!=;

%template(QuantityElectricCurrent) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricCurrentUnit>;

%include <biogears/cdm/properties/SEScalarElectricCurrent.h>




