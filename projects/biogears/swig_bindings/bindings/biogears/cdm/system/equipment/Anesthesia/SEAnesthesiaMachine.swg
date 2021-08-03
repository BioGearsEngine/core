// Add necessary symbols to generated header
%{
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
%}

%include <biogears/exports.h>

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