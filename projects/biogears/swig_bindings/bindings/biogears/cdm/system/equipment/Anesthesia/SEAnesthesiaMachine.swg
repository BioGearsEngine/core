// Add necessary symbols to generated header
%{
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEAnesthesiaMachine
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>
%import <biogears/cdm/properties/SEScalarFraction.swg>
%import <biogears/cdm/properties/SEScalarFrequency.swg>
%import <biogears/cdm/properties/SEScalarPressure.swg>
%import <biogears/cdm/properties/SEScalarTime.swg>
%import <biogears/cdm/properties/SEScalarVolume.swg>
%import <biogears/cdm/properties/SEScalarVolumePerTime.swg>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/cdm/substance/SESubstanceManager.swg>

%import <biogears/cdm/utils/SEEventHandler.swg>
%import <biogears/cdm/system/SESystem.swg>

%import <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.swg>
%import <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.swg>

 %rename(GetInletFlow_const)  GetInletFlow(const VolumePerTimeUnit& unit) const;
 %rename(GetInspiratoryExpiratoryRatio_const)  GetInspiratoryExpiratoryRatio() const;
 %rename(GetOxygenFraction_const)  GetOxygenFraction() const;
 %rename(GetPositiveEndExpiredPressure_const)  GetPositiveEndExpiredPressure(const PressureUnit& unit) const;
 %rename(GetRespiratoryRate_const)  GetRespiratoryRate(const FrequencyUnit& unit) const;
 %rename(GetReliefValvePressure_const)  GetReliefValvePressure(const PressureUnit& unit) const;
 %rename(GetVentilatorPressure_const)  GetVentilatorPressure(const PressureUnit& unit) const;
 %rename(GetLeftChamber_const)  GetLeftChamber() const;
 %rename(GetRightChamber_const)  GetRightChamber() const;
 %rename(GetOxygenBottleOne_const)  GetOxygenBottleOne() const;
 %rename(GetOxygenBottleTwo_const)  GetOxygenBottleTwo() const;

%include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>