// Add necessary symbols to generated header
%{
#include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>
#include <biogears/cdm/properties/SEScalarTypes.h>

#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>
%import <biogears/container/Tree.swg>

%import <biogears/cdm/properties/SEScalar0To1.swg>

%import <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.swg>


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

%include <biogears/cdm/system/equipment/Anesthesia/actions/SESodaLimeFailure.h>