%{
#include <biogears/cdm/patient/actions/SEChestCompression.h>
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
) SEChestCompression
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%nspace BIOGEARS_NAMESPACE SEChestCompression;

%ignore BIOGEARS_NAMESPACE SEChestCompression::Unload;
%ignore BIOGEARS_NAMESPACE SEChestCompression::Load;

%include <biogears/cdm/patient/actions/SEChestCompression.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEChestCompression*>;
%template (vector_SEChestCompression_ptr)  std::vector<BIOGEARS_NAMESPACE SEChestCompression*>;