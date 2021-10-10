
%{
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<MassPerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const MassPerMassUnit&) const;
%rename(NotEquals)  operator!=(const MassPerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarMassPerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarMassPerMass&) const;  

%ignore biogears::MassPerMassUnit::MassPerMassUnit(std::string const &);
%ignore biogears::MassPerMassUnit::IsValidUnit(std::string const &);
%ignore biogears::MassPerMassUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarMassPerMass;
%nspace biogears::MassPerMassUnit;
%nspace biogears::SEScalarQuantity<MassPerMassUnit>;

namespace biogears {
   class MassPerMassUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::MassPerMassUnit>;

%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< MassPerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerMassUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerMassUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::MassPerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< MassPerMassUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< MassPerMassUnit >::operator!=;

%template(QuantityMassPerMass) biogears::SEScalarQuantity<biogears::MassPerMassUnit>;

%include <biogears/cdm/properties/SEScalarMassPerMass.h>




