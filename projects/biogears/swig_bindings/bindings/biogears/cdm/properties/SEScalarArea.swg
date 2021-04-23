
%{
#include <biogears/cdm/properties/SEScalarArea.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SESCalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<AreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const AreaUnit&) const;
%rename(NotEquals)  operator!=(const AreaUnit&) const;
%rename(Equals)  operator==(const SEScalarArea&) const;
%rename(NotEquals)  operator!=(const SEScalarArea&) const;  

%ignore biogears::AreaUnit::AreaUnit(std::string const &);
%ignore biogears::AreaUnit::IsValidUnit(std::string const &);
%ignore biogears::AreaUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarArea;
%nspace biogears::AreaUnit;
%nspace biogears::SEScalarQuantity<AreaUnit>;

namespace biogears {
   class AreaUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::AreaUnit>;

%ignore  biogears::SEScalarQuantity< AreaUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< AreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< AreaUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< AreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::AreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< AreaUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< AreaUnit >::operator!=;

%template(QuantityArea) biogears::SEScalarQuantity<biogears::AreaUnit>;

%include <biogears/cdm/properties/SEScalarArea.h>




