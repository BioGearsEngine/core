%{
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<MassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassUnit&) const;
%rename(NotEquals)  operator!=(const MassUnit&) const;
%rename(Equals)  operator==(const SEScalarMass&) const;
%rename(NotEquals)  operator!=(const SEScalarMass&) const;  

%ignore BIOGEARS_NAMESPACE MassUnit::MassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE MassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarMass;
%nspace BIOGEARS_NAMESPACE MassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<MassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class MassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::GetValue(std::string const &) const;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::IncrementValue(double,std::string const &);
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::IncrementValue(double,char const *);
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::DecrementValue(double,std::string const &);
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::DecrementValue(double,char const *);
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit>::GetCompoundUnit(std::string const &) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;
%ignore  SetValue(double);

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< MassUnit >::operator!=;

%template(QuantityMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE MassUnit>;

%include <biogears/cdm/properties/SEScalarMass.h>

%nspace std::vector<BIOGEARS_NAMESPACE SEScalarMass*>;
%template (vector_SEScalarMass_ptr) std::vector<BIOGEARS_NAMESPACE SEScalarMass*>;


