%{
#include <biogears/cdm/patient/actions/SETensionPneumothorax.h>
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
) SETensionPneumothorax
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%nspace BIOGEARS_NAMESPACE SETensionPneumothorax;

%ignore BIOGEARS_NAMESPACE SETensionPneumothorax::Unload;
%ignore BIOGEARS_NAMESPACE SETensionPneumothorax::Load;

%rename (SetTensionPneumothoraxType) BIOGEARS_NAMESPACE SETensionPneumothorax::SetType;
%rename (GetTensionPneumothoraxType) BIOGEARS_NAMESPACE SETensionPneumothorax::GetType;

%include <biogears/cdm/patient/actions/SETensionPneumothorax.h>


%nsspace  std::vector<BIOGEARS_NAMESPACE SETensionPneumothorax*>;
%template (vector_SETensionPneumothorax_ptr)  std::vector<BIOGEARS_NAMESPACE SETensionPneumothorax*>;