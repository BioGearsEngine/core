%{
#include <biogears/cdm/system/physiology/SERenalSystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SERenalSystem
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/properties/SEScalar.swg>

%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressure.swg>
%import <biogears/cdm/properties/SEScalarArea.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTimePressureArea.swg>
%import <biogears/cdm/properties/SEScalarOsmolarity.swg>
%import <biogears/cdm/properties/SEScalarOsmolality.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>

%nspace BIOGEARS_NAMESPACE SERenalSystem;

%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::GetScalar(const std::string& name);
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Unload;
%ignore BIOGEARS_NAMESPACE SESubstancePharmacokinetics::Load;

%rename (GetFiltrationFraction_const) BIOGEARS_NAMESPACE SERenalSystem::GetFiltrationFraction() const;
%rename (GetLeftFiltrationFraction_const) BIOGEARS_NAMESPACE SERenalSystem::GetLeftFiltrationFraction() const ;
%rename (GetRightFiltrationFraction_const) BIOGEARS_NAMESPACE SERenalSystem::GetRightFiltrationFraction() const;
%rename (GetUrineSpecificGravity_const) BIOGEARS_NAMESPACE SERenalSystem::GetUrineSpecificGravity() const ;

%include <biogears/cdm/system/physiology/SERenalSystem.h>