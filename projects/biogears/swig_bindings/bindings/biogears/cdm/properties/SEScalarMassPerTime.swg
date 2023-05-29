
%{
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassPerTimeUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerTimeUnit&) const;
%rename(NotEquals)  operator!=(const MassPerTimeUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerTime&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerTime&) const;  

%ignore BIOGEARS_NAMESPACE MassPerTimeUnit::MassPerTimeUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerTimeUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerTimeUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMassPerTime;
%nspace BIOGEARS_NAMESPACE MassPerTimeUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassPerTimeUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassPerTimeUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerTimeUnit >::operator!=;

%template(QuantityMassPerTime) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerTimeUnit>;

%include <biogears/cdm/properties/SEScalarMassPerTime.h>




