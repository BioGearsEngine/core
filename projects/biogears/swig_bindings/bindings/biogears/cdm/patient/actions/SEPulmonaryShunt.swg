%{
#include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>
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
) SEPulmonaryShunt
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SEPulmonaryShunt;

%ignore BIOGEARS_NAMESPACE SEPulmonaryShunt::Unload;
%ignore BIOGEARS_NAMESPACE SEPulmonaryShunt::Load;

%include <biogears/cdm/patient/actions/SEPulmonaryShunt.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEPulmonaryShunt*>;
%template (vector_SEPulmonaryShunt_ptr)  std::vector<BIOGEARS_NAMESPACE SEPulmonaryShunt*>;