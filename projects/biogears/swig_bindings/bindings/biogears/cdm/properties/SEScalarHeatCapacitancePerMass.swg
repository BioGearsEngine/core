%{
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
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
) SEScalarHeatCapacitancePerMass
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitancePerMassUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const HeatCapacitancePerMassUnit&) const;
%rename(NotEquals)  operator!=(const HeatCapacitancePerMassUnit&) const;
%rename(Equals)  operator==(const SEScalarHeatCapacitancePerMass&) const;
%rename(NotEquals)  operator!=(const SEScalarHeatCapacitancePerMass&) const;  

%ignore BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit::HeatCapacitancePerMassUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarHeatCapacitancePerMass;
%nspace BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<HeatCapacitancePerMassUnit>;

OPEN_BIOGEARS_NAMESPACE
   class HeatCapacitancePerMassUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::DivideValue "public new";
#endif 

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< HeatCapacitancePerMassUnit >::operator!=;

%template(QuantityHeatCapacitancePerMass) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE HeatCapacitancePerMassUnit>;

%include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>




