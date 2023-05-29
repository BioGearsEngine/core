
%{
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricChargeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricChargeUnit&) const;
%rename(NotEquals)  operator!=(const ElectricChargeUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCharge&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCharge&) const;  

%ignore BIOGEARS_NAMESPACE ElectricChargeUnit::ElectricChargeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricChargeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricChargeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricCharge;
%nspace BIOGEARS_NAMESPACE ElectricChargeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricChargeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricChargeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricChargeUnit >::operator!=;

%template(QuantityElectricCharge) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricChargeUnit>;

%include <biogears/cdm/properties/SEScalarElectricCharge.h>




