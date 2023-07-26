%{
#include <biogears/cdm/properties/SEScalarOsmolarity.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEScalarOsmolarity
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<OsmolarityUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const OsmolarityUnit&) const;
%rename(NotEquals)  operator!=(const OsmolarityUnit&) const;
%rename(Equals)  operator==(const SEScalarOsmolarity&) const;
%rename(NotEquals)  operator!=(const SEScalarOsmolarity&) const;  

%ignore BIOGEARS_NAMESPACE OsmolarityUnit::OsmolarityUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE OsmolarityUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE OsmolarityUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarOsmolarity;
%nspace BIOGEARS_NAMESPACE OsmolarityUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<OsmolarityUnit>;

OPEN_BIOGEARS_NAMESPACE
   class OsmolarityUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< OsmolarityUnit >::operator!=;

%template(QuantityOsmolarity) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE OsmolarityUnit>;

%include <biogears/cdm/properties/SEScalarOsmolarity.h>




