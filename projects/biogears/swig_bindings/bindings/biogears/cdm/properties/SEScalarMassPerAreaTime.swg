
%{
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassPerAreaTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerAreaTimeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerAreaTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerAreaTime&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerAreaTime&) const;  

%ignore BIOGEARS_NAMESPACE MassPerAreaTimeUnit::MassPerAreaTimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerAreaTimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerAreaTimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMassPerAreaTime;
%nspace BIOGEARS_NAMESPACE MassPerAreaTimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassPerAreaTimeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassPerAreaTimeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerAreaTimeUnit >::operator!=;

%template(QuantityMassPerAreaTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerAreaTimeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>




