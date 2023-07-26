%{
#include <biogears/cdm/properties/SEUnitScalar.h>
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
) SEUnitScalar
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/utils/unitconversion/CompoundUnit.swg>

%nspace BIOGEARS_NAMESPACE SEUnitScalar;
 
%ignore BIOGEARS_NAMESPACE SEUnitScalar::GetCompoundUnit(std::string const &) const ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::GetValue(std::string const &) const ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::IncrementValue(double,std::string const &) ;
%ignore BIOGEARS_NAMESPACE SEUnitScalar::DecrementValue(double,std::string const &) ;

%include <biogears/cdm/properties/SEUnitScalar.h>


