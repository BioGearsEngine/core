
%{
#include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<TimeMassPerVolumeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const TimeMassPerVolumeUnit&) const;
%rename(NotEquals)  operator!=(const TimeMassPerVolumeUnit&) const;
%rename(Equals)  operator==(const SEScalarTimeMassPerVolume&) const;
%rename(NotEquals)  operator!=(const SEScalarTimeMassPerVolume&) const;  

%ignore biogears::TimeMassPerVolumeUnit::TimeMassPerVolumeUnit(std::string const &);
%ignore biogears::TimeMassPerVolumeUnit::IsValidUnit(std::string const &);
%ignore biogears::TimeMassPerVolumeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarTimeMassPerVolume;
%nspace biogears::TimeMassPerVolumeUnit;
%nspace biogears::SEScalarQuantity<TimeMassPerVolumeUnit>;

namespace biogears {
   class TimeMassPerVolumeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>;

%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< TimeMassPerVolumeUnit >::operator!=;

%template(QuantityTimeMassPerVolume) biogears::SEScalarQuantity<biogears::TimeMassPerVolumeUnit>;

%include <biogears/cdm/properties/SEScalarTimeMassPerVolume.h>




