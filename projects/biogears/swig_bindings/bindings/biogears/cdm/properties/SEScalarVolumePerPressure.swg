// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString;
%ignore biogears::SEScalarQuantity<VolumePerPressureUnit>::ToString;

%rename(Equals)  operator==(const VolumePerPressureUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerPressureUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerPressure&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerPressure&) const;  

%ignore biogears::VolumePerPressureUnit::VolumePerPressureUnit(std::string const &);
%ignore biogears::VolumePerPressureUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerPressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerPressure;
%nspace biogears::VolumePerPressureUnit;
%nspace biogears::SEScalarQuantity<VolumePerPressureUnit>;

namespace biogears {
   class VolumePerPressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerPressureUnit>;

%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::GetCompoundUnit(std::string const &) const ;
%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerPressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerPressureUnit >::operator!=;

%template(QuantityVolumePerPressure) biogears::SEScalarQuantity<biogears::VolumePerPressureUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerPressure.h>




