
%{
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<AmountPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AmountPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const AmountPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarAmountPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarAmountPerVolume&) const;  

%ignore biogears::AmountPerVolumeUnit::AmountPerVolumeUnit(std::string const &);
%ignore biogears::AmountPerVolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::AmountPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAmountPerVolume;
%nspace biogears::AmountPerVolumeUnit;
%nspace biogears::SEScalarQuantity<AmountPerVolumeUnit>;

namespace biogears {
   class AmountPerVolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>;

%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountPerVolumeUnit >::operator!=;

%template(QuantityAmountPerVolume) biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerVolume.h>




