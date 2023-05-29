
%{
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerMassUnit&) const;
%rename(NotEquals)  operator!=(const MassPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerMass&) const;  

%ignore BIOGEARS_NAMESPACE MassPerMassUnit::MassPerMassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerMassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassPerMassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMassPerMass;
%nspace BIOGEARS_NAMESPACE MassPerMassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassPerMassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassPerMassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassPerMassUnit >::operator!=;

%template(QuantityMassPerMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassPerMassUnit>;

%include <biogears/cdm/properties/SEScalarMassPerMass.h>




