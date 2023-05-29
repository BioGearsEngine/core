
%{
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const TimeUnit&) const;
%rename(NotEquals)  operator!=(const TimeUnit&) const;
%rename(Equals)  operator==(const SEScalarTime&) const;
%rename(NotEquals)  operator!=(const SEScalarTime&) const;  

%ignore BIOGEARS_NAMESPACE TimeUnit::TimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE TimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarTime;
%nspace BIOGEARS_NAMESPACE TimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<TimeUnit>;

   class TimeUnit;

%warnfilter(842)  SEScalarQuantity< TimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< TimeUnit >::operator!=;

%template(QuantityTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE TimeUnit>;

%include <biogears/cdm/properties/SEScalarTime.h>




