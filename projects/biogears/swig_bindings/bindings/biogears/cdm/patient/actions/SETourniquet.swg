%{
#include <biogears/cdm/patient/actions/SETourniquet.h>
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
) SETourniquet
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SETourniquet;

%ignore BIOGEARS_NAMESPACE SETourniquet::Unload;
%ignore BIOGEARS_NAMESPACE SETourniquet::Load;

%ignore BIOGEARS_NAMESPACE SETourniquet::SetCompartment(const std::string& name);

%include <biogears/cdm/patient/actions/SETourniquet.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SETourniquet*>;
%template (vector_SETourniquet_ptr)  std::vector<BIOGEARS_NAMESPACE SETourniquet*>;
