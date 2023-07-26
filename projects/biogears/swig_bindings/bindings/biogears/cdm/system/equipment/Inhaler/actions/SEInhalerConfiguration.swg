// Add necessary symbols to generated header
%{
#include<biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/system/SESystem.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEInhalerConfiguration
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>


%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/system/equipment/Inhaler/SEInhaler.swg>
%import <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerAction.swg>


%rename (GetConfiguration_const) BIOGEARS_NAMESPACE SEInhalerConfiguration::GetConfiguration() const;
%rename (SetConfigurationFile_const) BIOGEARS_NAMESPACE SEInhalerConfiguration::SetConfigurationFile(std::string const &);

%include <biogears/cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
