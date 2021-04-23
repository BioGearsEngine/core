// Add necessary symbols to generated header
%{
#include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<LengthPerTimePressureUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const LengthPerTimePressureUnit&) const;
%rename(NotEquals)  operator!=(const LengthPerTimePressureUnit&) const;
%rename(Equals)  operator==(const SEScalarLengthPerTimePressure&) const;
%rename(NotEquals)  operator!=(const SEScalarLengthPerTimePressure&) const;  

%ignore biogears::LengthPerTimePressureUnit::LengthPerTimePressureUnit(std::string const &);
%ignore biogears::LengthPerTimePressureUnit::IsValidUnit(std::string const &);
%ignore biogears::LengthPerTimePressureUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarLengthPerTimePressure;
%nspace biogears::LengthPerTimePressureUnit;
%nspace biogears::SEScalarQuantity<LengthPerTimePressureUnit>;

namespace biogears {
   class LengthPerTimePressureUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>;

%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< LengthPerTimePressureUnit >::operator!=;

%template(QuantityLengthPerTimePressure) biogears::SEScalarQuantity<biogears::LengthPerTimePressureUnit>;

%include <biogears/cdm/properties/SEScalarLengthPerTimePressure.h>




