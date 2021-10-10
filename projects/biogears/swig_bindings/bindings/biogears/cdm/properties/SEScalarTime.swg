
%{
#include <biogears/cdm/properties/SEScalarTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<TimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const TimeUnit&) const;
%rename(NotEquals)  operator!=(const TimeUnit&) const;
%rename(Equals)  operator==(const SEScalarTime&) const;
%rename(NotEquals)  operator!=(const SEScalarTime&) const;  

%ignore biogears::TimeUnit::TimeUnit(std::string const &);
%ignore biogears::TimeUnit::IsValidUnit(std::string const &);
%ignore biogears::TimeUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarTime;
%nspace biogears::TimeUnit;
%nspace biogears::SEScalarQuantity<TimeUnit>;

namespace biogears {
   class TimeUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::TimeUnit>;

%ignore  biogears::SEScalarQuantity< TimeUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< TimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TimeUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< TimeUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< TimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::TimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< TimeUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< TimeUnit >::operator!=;

%template(QuantityTime) biogears::SEScalarQuantity<biogears::TimeUnit>;

%include <biogears/cdm/properties/SEScalarTime.h>




