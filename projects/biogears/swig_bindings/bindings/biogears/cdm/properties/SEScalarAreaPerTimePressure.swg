
%{
#include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<AreaPerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AreaPerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const AreaPerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarAreaPerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarAreaPerTimePressure&) const;  

%ignore biogears::AreaPerTimePressureUnit::AreaPerTimePressureUnit(std::string const &);
%ignore biogears::AreaPerTimePressureUnit::IsValidUnit(std::string const &);
%ignore biogears::AreaPerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarAreaPerTimePressure;
%nspace biogears::AreaPerTimePressureUnit;
%nspace biogears::SEScalarQuantity<AreaPerTimePressureUnit>;

namespace biogears {
   class AreaPerTimePressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>;

%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AreaPerTimePressureUnit >::operator!=;

%template(QuantityAreaPerTimePressure) biogears::SEScalarQuantity<biogears::AreaPerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarAreaPerTimePressure.h>




