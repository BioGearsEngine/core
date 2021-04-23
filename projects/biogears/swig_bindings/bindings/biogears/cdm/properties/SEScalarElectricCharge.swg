// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarElectricCharge.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<ElectricChargeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricChargeUnit&) const;
%rename(NotEquals)  operator!=(const ElectricChargeUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricCharge&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricCharge&) const;  

%ignore biogears::ElectricChargeUnit::ElectricChargeUnit(std::string const &);
%ignore biogears::ElectricChargeUnit::IsValidUnit(std::string const &);
%ignore biogears::ElectricChargeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarElectricCharge;
%nspace biogears::ElectricChargeUnit;
%nspace biogears::SEScalarQuantity<ElectricChargeUnit>;

namespace biogears {
   class ElectricChargeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::ElectricChargeUnit>;

%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< ElectricChargeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricChargeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricChargeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricChargeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::ElectricChargeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< ElectricChargeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< ElectricChargeUnit >::operator!=;

%template(QuantityElectricCharge) biogears::SEScalarQuantity<biogears::ElectricChargeUnit>;

%include <biogears/cdm/properties/SEScalarElectricCharge.h>




