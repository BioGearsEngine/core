
%{
#include <biogears/cdm/properties/SEScalarLength.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<LengthUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const LengthUnit&) const;
%rename(NotEquals)  operator!=(const LengthUnit&) const;
%rename(Equals)  operator==(const SEScalarLength&) const;
%rename(NotEquals)  operator!=(const SEScalarLength&) const;  

%ignore biogears::LengthUnit::LengthUnit(std::string const &);
%ignore biogears::LengthUnit::IsValidUnit(std::string const &);
%ignore biogears::LengthUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarLength;
%nspace biogears::LengthUnit;
%nspace biogears::SEScalarQuantity<LengthUnit>;

%rename(inch)       LengthUnit::in;
namespace biogears {
   class LengthUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::LengthUnit>;

%ignore  biogears::SEScalarQuantity< LengthUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< LengthUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< LengthUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< LengthUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::LengthUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< LengthUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< LengthUnit >::operator!=;
%template(QuantityLength) biogears::SEScalarQuantity<biogears::LengthUnit>;

%include <biogears/cdm/properties/SEScalarLength.h>




