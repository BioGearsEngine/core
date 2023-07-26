%{
#include <biogears/cdm/patient/SENutrition.h>
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
) SENutrition
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SENutrition;

%ignore BIOGEARS_NAMESPACE SENutrition::Unload;
%ignore BIOGEARS_NAMESPACE SENutrition::Load;

%ignore BIOGEARS_NAMESPACE SENutrition::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SENutrition::SetName(std::string const &);
%ignore BIOGEARS_NAMESPACE SENutrition::SetAnnotation(std::string const &);

%ignore BIOGEARS_NAMESPACE operator<<(std::ostream& out, const SENutrition& n);

%include <biogears/cdm/patient/SENutrition.h>
