
%{
#include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<VolumePerTimeAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const VolumePerTimeAreaUnit&) const;
%rename(NotEquals)  operator!=(const VolumePerTimeAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarVolumePerTimeArea&) const;
%rename(NotEquals)  operator!=(const SEScalarVolumePerTimeArea&) const;  

%ignore biogears::VolumePerTimeAreaUnit::VolumePerTimeAreaUnit(std::string const &);
%ignore biogears::VolumePerTimeAreaUnit::IsValidUnit(std::string const &);
%ignore biogears::VolumePerTimeAreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarVolumePerTimeArea;
%nspace biogears::VolumePerTimeAreaUnit;
%nspace biogears::SEScalarQuantity<VolumePerTimeAreaUnit>;

namespace biogears {
   class VolumePerTimeAreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>;


%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< VolumePerTimeAreaUnit >::operator!=;

%template(QuantityVolumePerTimeArea) biogears::SEScalarQuantity<biogears::VolumePerTimeAreaUnit>;

%include <biogears/cdm/properties/SEScalarVolumePerTimeArea.h>




