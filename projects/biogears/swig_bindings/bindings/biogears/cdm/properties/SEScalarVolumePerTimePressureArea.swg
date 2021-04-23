
%{
#include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<VolumePerTimePressureAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimePressureAreaUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimePressureAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimePressureArea&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimePressureArea&) const;  

%ignore biogears::VolumePerTimePressureAreaUnit::VolumePerTimePressureAreaUnit(std::string const &);
%ignore biogears::VolumePerTimePressureAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerTimePressureAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerTimePressureArea;
%nspace biogears::VolumePerTimePressureAreaUnit;
%nspace biogears::SEScalarQuantity<VolumePerTimePressureAreaUnit>;

namespace biogears {
   class VolumePerTimePressureAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>;


%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerTimePressureAreaUnit >::operator!=;

%template(QuantityVolumePerTimePressureArea) biogears::SEScalarQuantity<biogears::VolumePerTimePressureAreaUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.h>




