
%{
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<MassPerAreaTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerAreaTimeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerAreaTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerAreaTime&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerAreaTime&) const;  

%ignore biogears::MassPerAreaTimeUnit::MassPerAreaTimeUnit(std::string const &);
%ignore biogears::MassPerAreaTimeUnit::IsValidUnit(std::string const &);
%ignore biogears::MassPerAreaTimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMassPerAreaTime;
%nspace biogears::MassPerAreaTimeUnit;
%nspace biogears::SEScalarQuantity<MassPerAreaTimeUnit>;

namespace biogears {
   class MassPerAreaTimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>;

%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassPerAreaTimeUnit >::operator!=;

%template(QuantityMassPerAreaTime) biogears::SEScalarQuantity<biogears::MassPerAreaTimeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>




