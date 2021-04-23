

%{
#include <biogears/cdm/properties/SEScalarVolume.h>
%}

%include <biogears/exports.h>


%import <biogears/cdm/properties/SEScalarQuantity.swg>

%nspace biogears::SEScalarVolume;
%nspace biogears::VolumeUnit;

%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<VolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumeUnit&) const;
%rename(NotEquals)  operator!=(const VolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarVolume&) const;  

%ignore biogears::VolumeUnit::VolumeUnit(std::string const &);
%ignore biogears::VolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarQuantity<VolumeUnit>;

namespace biogears {
   class VolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumeUnit>;

%ignore  biogears::SEScalarQuantity<VolumeUnit>::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity<VolumeUnit>::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity<VolumeUnit>::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity<VolumeUnit>::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity<VolumeUnit>::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity<VolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumeUnit>::DivideValue "public new";
#endif 

%ignore  biogears::SEScalarQuantity<VolumeUnit>::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity<VolumeUnit>::operator!=;

%template(QuantityVolume) biogears::SEScalarQuantity<biogears::VolumeUnit>;

%include <biogears/cdm/properties/SEScalarVolume.h>


