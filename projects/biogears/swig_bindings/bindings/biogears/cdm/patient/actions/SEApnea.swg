%{
#include <biogears/cdm/patient/actions/SEApnea.h>
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
) SEApnea
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEApnea;

%ignore BIOGEARS_NAMESPACE SEApnea::Unload;
%ignore BIOGEARS_NAMESPACE SEApnea::Load;

%include <biogears/cdm/patient/actions/SEApnea.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEApnea*>;
%template (vector_SEApnea_ptr)  std::vector<BIOGEARS_NAMESPACE SEApnea*>;