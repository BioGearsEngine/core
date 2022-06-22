%{
#include <biogears/cdm/properties/SEScalarMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<MassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassUnit&) const;
%rename(NotEquals)  operator!=(const MassUnit&) const;
%rename(Equals)  operator==(const SEScalarMass&) const;
%rename(NotEquals)  operator!=(const SEScalarMass&) const;  

%ignore biogears::MassUnit::MassUnit(std::string const &);
%ignore biogears::MassUnit::IsValidUnit(std::string const &);
%ignore biogears::MassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMass;
%nspace biogears::MassUnit;
%nspace biogears::SEScalarQuantity<MassUnit>;

namespace biogears {
   class MassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassUnit>;

%ignore  biogears::SEScalarQuantity< MassUnit >::GetValue(std::string const &) const;
%ignore  biogears::SEScalarQuantity< MassUnit >::IncrementValue(double,std::string const &);
%ignore  biogears::SEScalarQuantity< MassUnit >::IncrementValue(double,char const *);
%ignore  biogears::SEScalarQuantity< MassUnit >::DecrementValue(double,std::string const &);
%ignore  biogears::SEScalarQuantity< MassUnit >::DecrementValue(double,char const *);
%ignore  biogears::SEScalarQuantity< MassUnit>::GetCompoundUnit(std::string const &) const;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;
%ignore  SetValue(double);

%ignore  biogears::SEScalarQuantity< MassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassUnit >::operator!=;

%template(QuantityMass) biogears::SEScalarQuantity<biogears::MassUnit>;

%include <biogears/cdm/properties/SEScalarMass.h>

%nspace std::vector<biogears::SEScalarMass*>;
%template (vector_SEScalarMass_ptr) std::vector<biogears::SEScalarMass*>;


