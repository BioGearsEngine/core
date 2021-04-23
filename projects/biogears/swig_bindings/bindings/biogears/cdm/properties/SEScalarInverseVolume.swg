// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarInverseVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<InverseVolumeUnit>::ToString;

%rename(Equals)  operator==(const InverseVolumeUnit&) const;
%rename(NotEquals)  operator!=(const InverseVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarInverseVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarInverseVolume&) const;  

%ignore biogears::InverseVolumeUnit::InverseVolumeUnit(std::string const &);
%ignore biogears::InverseVolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::InverseVolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarInverseVolume;
%nspace biogears::InverseVolumeUnit;
%nspace biogears::SEScalarQuantity<InverseVolumeUnit>;

namespace biogears {
   class InverseVolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::InverseVolumeUnit>;

%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< InverseVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< InverseVolumeUnit >::operator!=;

%template(QuantityInverseVolume) biogears::SEScalarQuantity<biogears::InverseVolumeUnit>;

%include <biogears/cdm/properties/SEScalarInverseVolume.h>




