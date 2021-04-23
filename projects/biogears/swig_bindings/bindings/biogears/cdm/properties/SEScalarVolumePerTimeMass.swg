// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<VolumePerTimeMassUnit>::ToString;

%rename(Equals)  operator==(const VolumePerTimeMassUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimeMassUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimeMass&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimeMass&) const;  

%ignore biogears::VolumePerTimeMassUnit::VolumePerTimeMassUnit(std::string const &);
%ignore biogears::VolumePerTimeMassUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerTimeMassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerTimeMass;
%nspace biogears::VolumePerTimeMassUnit;
%nspace biogears::SEScalarQuantity<VolumePerTimeMassUnit>;

namespace biogears {
   class VolumePerTimeMassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerTimeMassUnit>;


%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerTimeMassUnit >::operator!=;

%template(QuantityVolumePerTimeMass) biogears::SEScalarQuantity<biogears::VolumePerTimeMassUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>




