%{
#include <biogears/cdm/patient/actions/SEInfection.h>
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
) SEInfection
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SEInfection;

%ignore BIOGEARS_NAMESPACE SEInfection::Unload;
%ignore BIOGEARS_NAMESPACE SEInfection::Load;

%ignore BIOGEARS_NAMESPACE SEInfection::SetLocation(const std::string& name);

%include <biogears/cdm/patient/actions/SEInfection.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEInfection*>;
%template (vector_SEInfection_ptr)  std::vector<BIOGEARS_NAMESPACE SEInfection*>;