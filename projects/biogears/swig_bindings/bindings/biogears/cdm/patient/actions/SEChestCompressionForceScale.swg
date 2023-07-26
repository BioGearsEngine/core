%{
#include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>
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
) SEChestCompressionForceScale
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>
%import <biogears/cdm/patient/actions/SEChestCompression.swg>

%nspace BIOGEARS_NAMESPACE SEChestCompressionForceScale;

%ignore BIOGEARS_NAMESPACE SEChestCompressionForceScale::Unload;
%ignore BIOGEARS_NAMESPACE SEChestCompressionForceScale::Load;

%include <biogears/cdm/patient/actions/SEChestCompressionForceScale.h>

%nspace  std::vector<BIOGEARS_NAMESPACE SEChestCompressionForceScale*>;
%template (vector_SEChestCompressionForceScale_ptr)  std::vector<BIOGEARS_NAMESPACE SEChestCompressionForceScale*>;