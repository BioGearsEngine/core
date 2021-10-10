
%{
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore biogears::SEScalarQuantity::ToString(std::ostream& str);
%ignore biogears::SEScalarQuantity<OsmolarityUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const OsmolarityUnit&) const;
%rename(NotEquals)  operator!=(const OsmolarityUnit&) const;
%rename(Equals)  operator==(const SEScalarOsmolarity&) const;
%rename(NotEquals)  operator!=(const SEScalarOsmolarity&) const;  

%ignore biogears::OsmolarityUnit::OsmolarityUnit(std::string const &);
%ignore biogears::OsmolarityUnit::IsValidUnit(std::string const &);
%ignore biogears::OsmolarityUnit::GetCompoundUnit(std::string const &);

%nspace biogears::SEScalarOsmolarity;
%nspace biogears::OsmolarityUnit;
%nspace biogears::SEScalarQuantity<OsmolarityUnit>;

namespace biogears {
   class OsmolarityUnit;
}

%warnfilter(842) biogears::SEScalarQuantity<biogears::OsmolarityUnit>;

%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::GetValue(std::string const &) const ;
%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::IncrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::IncrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::DecrementValue(double,std::string const &) ;
%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::DecrementValue(double,char const *) ;
%ignore  biogears::SEScalarQuantity< OsmolarityUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolarityUnit>::Multiply "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolarityUnit>::Divide "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolarityUnit>::MultiplyValue "public new";
%csmethodmodifiers biogears::SEScalarQuantity<biogears::OsmolarityUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  biogears::SEScalarQuantity< OsmolarityUnit >::operator==;
%rename(NotEquals)  biogears::SEScalarQuantity< OsmolarityUnit >::operator!=;

%template(QuantityOsmolarity) biogears::SEScalarQuantity<biogears::OsmolarityUnit>;

%include <biogears/cdm/properties/SEScalarOsmolarity.h>




