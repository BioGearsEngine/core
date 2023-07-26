%{
#include <biogears/cdm/patient/actions/SEIntubation.h>
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
) SEIntubation
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEIntubation;

%ignore BIOGEARS_NAMESPACE SEIntubation::Unload;
%ignore BIOGEARS_NAMESPACE SEIntubation::Load;

%rename (SetIntubationType) BIOGEARS_NAMESPACE SEIntubation::SetType;
%rename (GetIntubationType) BIOGEARS_NAMESPACE SEIntubation::GetType;

%include <biogears/cdm/patient/actions/SEIntubation.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEIntubation*>;
%template (vector_SEIntubation_ptr)  std::vector<BIOGEARS_NAMESPACE SEIntubation*>;