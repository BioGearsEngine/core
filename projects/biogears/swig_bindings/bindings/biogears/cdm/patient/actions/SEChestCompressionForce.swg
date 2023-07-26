%{
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
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
) SEChestCompressionForce
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalarForce.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEChestcompression.swg>

%nspace BIOGEARS_NAMESPACE SEChestCompressionForce;

%ignore BIOGEARS_NAMESPACE SEChestCompressionForce::Unload;
%ignore BIOGEARS_NAMESPACE SEChestCompressionForce::Load;

%include <biogears/cdm/patient/actions/SEChestCompressionForce.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEChestCompressionForce*>;
%template (vector_SEChestCompressionForce_ptr)  std::vector<BIOGEARS_NAMESPACE SEChestCompressionForce*>;