%{
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
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
) SEScalarHeatConductancePerArea
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatConductancePerAreaUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatConductancePerAreaUnit&) const;
%rename(NotEquals)  operator!=(const HeatConductancePerAreaUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatConductancePerArea&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatConductancePerArea&) const;  

%ignore BIOGEARS_NAMESPACE HeatConductancePerAreaUnit::HeatConductancePerAreaUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatConductancePerAreaUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatConductancePerAreaUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatConductancePerArea;
%nspace BIOGEARS_NAMESPACE HeatConductancePerAreaUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatConductancePerAreaUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatConductancePerAreaUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatConductancePerAreaUnit >::operator!=;

%template(QuantityHeatConductancePerArea) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatConductancePerAreaUnit>;

%include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>




