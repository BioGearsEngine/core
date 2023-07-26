%{
#include <biogears/cdm/patient/actions/SEOverride.h>
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
) SEOverride
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/cdm/patient/actions/SEPatientAction.swg>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEScalarAmountPerTime.swg>
%import <biogears/cdm/properties/SEScalarAmountPerVolume.swg>
%import <biogears/cdm/properties/SEScalarFlowCompliance.swg>
%import <biogears/cdm/properties/SEScalarFlowResistance.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarMass.swg>
%import <biogears/cdm/properties/SEScalarMassPerTime.swg>
%import <biogears/cdm/properties/SEScalarMassPerVolume.swg>
%import <biogears/cdm/properties/SEScalarOsmolality.swg>
%import <biogears/cdm/properties/SEScalarPower.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarTemperature.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%nspace BIOGEARS_NAMESPACE SEOverride;

%ignore BIOGEARS_NAMESPACE SEOverride::Unload;
%ignore BIOGEARS_NAMESPACE SEOverride::Load;

%rename (GetArterialPHOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetArterialPHOverride() const ;
%rename (GetVenousPHOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetVenousPHOverride() const ;
%rename (GetCO2SaturationOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetCO2SaturationOverride() const ;
%rename (GetCOSaturationOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetCOSaturationOverride() const ;
%rename (GetO2SaturationOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetO2SaturationOverride() const ;
%rename (GetAchievedExerciseLevelOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetAchievedExerciseLevelOverride() const ;
%rename (GetFatigueLevelOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetFatigueLevelOverride() const ;
%rename (GetTotalWorkRateLevelOverride_const)  BIOGEARS_NAMESPACE SEOverride::GetTotalWorkRateLevelOverride() const ;

%include <biogears/cdm/patient/actions/SEOverride.h>

%nsspace  std::vector<BIOGEARS_NAMESPACE SEOverride*>;
%template (vector_SEOverride_ptr)  std::vector<BIOGEARS_NAMESPACE SEOverride*>;