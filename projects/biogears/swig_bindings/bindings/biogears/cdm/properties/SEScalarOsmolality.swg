
%{
#include <biogears/cdm/properties/SEScalarOsmolality.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<OsmolalityUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const OsmolalityUnit&) const;
%rename(NotEquals)  operator!=(const OsmolalityUnit&) const;
%rename(Equals)  operator==(const SEScalarOsmolality&) const;
%rename(NotEquals)  operator!=(const SEScalarOsmolality&) const;  

%ignore biogears::OsmolalityUnit::OsmolalityUnit(std::string const &);
%ignore biogears::OsmolalityUnit::IsValidUnit(std::string const &);
%ignore biogears::OsmolalityUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarOsmolality;
%nspace biogears::OsmolalityUnit;
%nspace biogears::SEScalarQuantity<OsmolalityUnit>;

namespace biogears {
   class OsmolalityUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::OsmolalityUnit>;

%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< OsmolalityUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolalityUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolalityUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolalityUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolalityUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< OsmolalityUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< OsmolalityUnit >::operator!=;

%template(QuantityOsmolality) biogears::SEScalarQuantity<biogears::OsmolalityUnit>;

%include <biogears/cdm/properties/SEScalarOsmolality.h>




