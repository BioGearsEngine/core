%{
#include <biogears/cdm/patient/actions/SEPericardialEffusion.h>
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
) SEPericardialEffusion
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace BIOGEARS_NAMESPACE SEPericardialEffusion;

%ignore BIOGEARS_NAMESPACE SEPericardialEffusion::Unload;
%ignore BIOGEARS_NAMESPACE SEPericardialEffusion::Load;
 
%include <biogears/cdm/patient/actions/SEPericardialEffusion.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEPericardialEffusion*>;
%template (vector_SEPericardialEffusion_ptr)  std::vector<BIOGEARS_NAMESPACE SEPericardialEffusion*>;