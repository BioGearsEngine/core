%{
#include <biogears/schema/cdm/ElectroCardioGram.hxx>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
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
) SEScalarElectricPotential
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalarQuantity.swg>


%ignore BIOGEARS_NAMESPACE SEScalarQuantity::ToString(std::ostream& str);
%ignore BIOGEARS_NAMESPACE SEScalarQuantity<ElectricPotentialUnit>::ToString(std::ostream& str);

%rename(Equals)  operator==(const ElectricPotentialUnit&) const;
%rename(NotEquals)  operator!=(const ElectricPotentialUnit&) const;
%rename(Equals)  operator==(const SEScalarElectricPotential&) const;
%rename(NotEquals)  operator!=(const SEScalarElectricPotential&) const;  

%ignore BIOGEARS_NAMESPACE ElectricPotentialUnit::ElectricPotentialUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricPotentialUnit::IsValidUnit(std::string const &);
%ignore BIOGEARS_NAMESPACE ElectricPotentialUnit::GetCompoundUnit(std::string const &);

%nspace BIOGEARS_NAMESPACE SEScalarElectricPotential;
%nspace BIOGEARS_NAMESPACE ElectricPotentialUnit;
%nspace BIOGEARS_NAMESPACE SEScalarQuantity<ElectricPotentialUnit>;

OPEN_BIOGEARS_NAMESPACE
   class ElectricPotentialUnit;
CLOSE_BIOGEARS_NAMESPACE

%warnfilter(842) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::GetValue(std::string const &) const ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::IncrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::IncrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::DecrementValue(double,std::string const &) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::DecrementValue(double,char const *) ;
%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit>::GetCompoundUnit(std::string const &) const ;
#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>::Multiply "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>::Divide "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>::MultiplyValue "public new";
%csmethodmodifiers BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>::DivideValue "public new";
#endif 

%ignore  GetValue(std::string const &) const ;

%ignore  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::operator==;
%rename(NotEquals)  BIOGEARS_NAMESPACE SEScalarQuantity< ElectricPotentialUnit >::operator!=;

%template(QuantityElectricPotential) BIOGEARS_NAMESPACE SEScalarQuantity<BIOGEARS_NAMESPACE ElectricPotentialUnit>;

%include <biogears/cdm/properties/SEScalarElectricPotential.h>

%nspace std::vector<BIOGEARS_NAMESPACE SEScalarElectricPotential*>;
%template (vector_SEScalarElectricPotential_ptr) std::vector<BIOGEARS_NAMESPACE SEScalarElectricPotential*>;

%nspace std::map<CDM::ElectroCardioGramWaveformLeadNumber, BIOGEARS_NAMESPACE SEScalarElectricPotential*>;
%template (map_ElectroCardioGramWaveformLeadNumber__SEScalarElectricPotential_ptr) std::map<CDM::ElectroCardioGramWaveformLeadNumber, BIOGEARS_NAMESPACE SEScalarElectricPotential*>;
