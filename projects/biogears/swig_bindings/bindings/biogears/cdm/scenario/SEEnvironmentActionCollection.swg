%{
#include <biogears/cdm/scenario/SEEnvironmentActionCollection.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEEnvironmentActionCollection
#endif

%include <biogears/exports.swg>


%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/scenario/SEAction.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
%import <biogears/cdm/substance/SESubstanceCompound.swg>
%import <biogears/cdm/system/environment/actions/SEEnvironmentChange.swg>
%import <biogears/cdm/system/environment/actions/SEThermalApplication.swg>

%nspace BIOGEARS_NAMESPACE SEPatientActionCollection;

%ignore BIOGEARS_NAMESPACE SEEnvironmentActionCollection::Unload;
%ignore BIOGEARS_NAMESPACE SEEnvironmentActionCollection::Load;

%ignore ProcessAction(const CDM::EnvironmentActionData& in);

%include <biogears/cdm/scenario/SEEnvironmentActionCollection.h>

