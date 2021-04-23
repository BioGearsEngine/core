
%{
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<MassPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerVolume&) const;  

%ignore biogears::MassPerVolumeUnit::MassPerVolumeUnit(std::string const &);
%ignore biogears::MassPerVolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::MassPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMassPerVolume;
%nspace biogears::MassPerVolumeUnit;
%nspace biogears::SEScalarQuantity<MassPerVolumeUnit>;

namespace biogears {
   class MassPerVolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>;

%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< MassPerVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassPerVolumeUnit >::operator!=;

%template(QuantityMassPerVolume) biogears::SEScalarQuantity<biogears::MassPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerVolume.h>




