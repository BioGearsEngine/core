
%{
#include <biogears/cdm/properties/SEScalarOsmolality.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<OsmolalityUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const OsmolalityUnit&) const;
%rename(NotEquals)  operator!=(const OsmolalityUnit&) const;
%rename(Equals)  operator==(const SEScalarOsmolality&) const;
%rename(NotEquals)  operator!=(const SEScalarOsmolality&) const;  

%ignore BIOGEARS_NAMESPACE OsmolalityUnit::OsmolalityUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE OsmolalityUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE OsmolalityUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarOsmolality;
%nspace BIOGEARS_NAMESPACE OsmolalityUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<OsmolalityUnit>;

OPEN_BIOGEARS_NAMESPACE
   class OsmolalityUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolalityUnit >::operator!=;

%template(QuantityOsmolality) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolalityUnit>;

%include <biogears/cdm/properties/SEScalarOsmolality.h>




