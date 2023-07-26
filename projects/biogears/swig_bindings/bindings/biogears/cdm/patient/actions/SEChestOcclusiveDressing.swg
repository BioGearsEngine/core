%{
#include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>
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
) SEChestOcclusiveDressing
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEChestOcclusiveDressing;

%ignore BIOGEARS_NAMESPACE SEChestOcclusiveDressing::Unload;
%ignore BIOGEARS_NAMESPACE SEChestOcclusiveDressing::Load;

%include <biogears/cdm/patient/actions/SEChestOcclusiveDressing.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEChestOcclusiveDressing*>;
%template (vector_SEChestOcclusiveDressing_ptr)  std::vector<BIOGEARS_NAMESPACE SEChestOcclusiveDressing*>;