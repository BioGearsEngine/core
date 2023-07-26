%{
#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/system/environment/actions/SEEnvironmentAction.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEEnvironment
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/system/SESystem.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>
%import <biogears/cdm/system/environment/SEEnvironmentalConditions.swg>
%import <biogears/cdm/system/environment/actions/SEEnvironmentChange.swg>
%import <biogears/cdm/system/environment/conditions/SEInitialEnvironment.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarHeatConductancePerArea.swg>

%nspace BIOGEARS_NAMESPACE SEEnvironment;

%ignore BIOGEARS_NAMESPACE SEEnvironment::Unload;
%ignore BIOGEARS_NAMESPACE SEEnvironment::Load;

%ignore BIOGEARS_NAMESPACE SEEnvironment::GetScalar(std::string const &);
%ignore BIOGEARS_NAMESPACE SEEnvironment::SetName(const std::string& name);

%rename (GetConditions_const) BIOGEARS_NAMESPACE SEEnvironment::GetConditions() const;

%ignore SEEnvironment::ToString(std::ostream& str) const;

%include <biogears/cdm/system/environment/SEEnvironment.h>


