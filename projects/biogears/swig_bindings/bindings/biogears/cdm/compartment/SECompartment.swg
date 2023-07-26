%{
#include <biogears/cdm/compartment/SECompartment.h>
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
) SECompartment
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/utils/Logger.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%nspace BIOGEARS_NAMESPACE SECompartment;

%ignore BIOGEARS_NAMESPACE SECompartment::Unload;
%ignore BIOGEARS_NAMESPACE SECompartment::Load;

%ignore BIOGEARS_NAMESPACE SECompartment::FindLeaves;
%ignore BIOGEARS_NAMESPACE SECompartment::GetScalar(const std::string& name);

%include <biogears/cdm/compartment/SECompartment.h>

%nspace std::vector<BIOGEARS_NAMESPACE SECompartment*>;
%template (vector_SECompartment_ptr) std::vector<BIOGEARS_NAMESPACE SECompartment*>;
