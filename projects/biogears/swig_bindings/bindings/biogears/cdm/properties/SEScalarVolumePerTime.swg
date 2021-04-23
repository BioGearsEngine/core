// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<VolumePerTimeUnit>::ToString;

%rename(Equals)  operator==(const VolumePerTimeUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTime&) const;  

%ignore biogears::VolumePerTimeUnit::VolumePerTimeUnit(std::string const &);
%ignore biogears::VolumePerTimeUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerTimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerTime;
%nspace biogears::VolumePerTimeUnit;
%nspace biogears::SEScalarQuantity<VolumePerTimeUnit>;

namespace biogears {
   class VolumePerTimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerTimeUnit>;

%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerTimeUnit >::operator!=;

%template(QuantityVolumePerTime) biogears::SEScalarQuantity<biogears::VolumePerTimeUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTime.h>




