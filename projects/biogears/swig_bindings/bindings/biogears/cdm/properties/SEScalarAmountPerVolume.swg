// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<AmountPerVolumeUnit>::ToString;

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
%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AmountPerVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AmountPerVolumeUnit >::operator!=;

%template(QuantityAmountPerVolume) biogears::SEScalarQuantity<biogears::AmountPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarAmountPerVolume.h>




