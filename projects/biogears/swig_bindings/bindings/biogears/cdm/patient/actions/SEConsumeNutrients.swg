%{
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/patient/SEPatient.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEConsumeNutrients
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/SENutrition.swg>

%import <biogears/cdm/patient/SEPatient.swg>
%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>

%nspace BIOGEARS_NAMESPACE SEConsumeNutrients;

%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::Unload;
%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::Load;

%rename (GetNutrition_const) BIOGEARS_NAMESPACE SEConsumeNutrients::GetNutrition() const;
%ignore BIOGEARS_NAMESPACE SEConsumeNutrients::SetNutritionFile(std::string const &);

%include <biogears/cdm/patient/actions/SEConsumeNutrients.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEConsumeNutrients*>;
%template (vector_SEConsumeNutrients_ptr)  std::vector<BIOGEARS_NAMESPACE SEConsumeNutrients*>;