
%{
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<VolumePerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimePressure&) const;  

%ignore biogears::VolumePerTimePressureUnit::VolumePerTimePressureUnit(std::string const &);
%ignore biogears::VolumePerTimePressureUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerTimePressure;
%nspace biogears::VolumePerTimePressureUnit;
%nspace biogears::SEScalarQuantity<VolumePerTimePressureUnit>;

namespace biogears {
   class VolumePerTimePressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>;


%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerTimePressureUnit >::operator!=;

%template(QuantityVolumePerTimePressure) biogears::SEScalarQuantity<biogears::VolumePerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>




