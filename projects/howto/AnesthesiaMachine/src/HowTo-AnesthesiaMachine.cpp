/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/



// Include the various types you will be using in your code
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

using namespace biogears;
//---------------------------------------------------------------------------------------------------------------------
/// \brief
/// Usage for connecting the anesthesia machine to a patient
///
/// \details
/// Use an SESubstanceBolus action to stop the patient from breathing on their own
/// Use an SEAnesthesiaMachineConfiguration action to configure and turn on the anesthesia machine
/// Use an SEAnesthesiaMachineVentilatorMask action to place the mash and allow the machine to breath for the patient
/// End of example shows how to turn things off
//---------------------------------------------------------------------------------------------------------------------
void HowToAnesthesiaMachine()
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToAnesthesiaMachine.log");
  bg->GetLogger()->Info("HowToAnesthesiaMachine");
	if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

    // The tracker is responsible for advancing the engine time and outputting the data requests below at each time step
	

	// Create data requests for each value that should be written to the output log as the engine is executing
	// Physiology System Names are defined on the System Objects 
	// defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("RespirationRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TidalVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("TotalLungVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("OxygenSaturation");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SedationLevel");

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToAnesthesiaMachine.csv");

	bg->GetLogger()->Info("The patient is nice and healthy");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	bg->AdvanceModelTime(50, TimeUnit::s);

	// Turn the anesthesia machine on and get it configured for spontaneous breathing
	// Create an Anesthesia Machine and configure it as needed
	SEAnesthesiaMachineConfiguration AMConfig(bg->GetSubstanceManager());

	// You can set configuration by modifing the configuration class directly
	// Or you can point to an XML with configuration data.
	// Modifying the class will keep any old settings that are not provided in the config
	// Using a xml will set the anesthesia machine to only the property states specified in the file
	SEAnesthesiaMachine& config = AMConfig.GetConfiguration();
	config.SetConnection(CDM::enumAnesthesiaMachineConnection::Mask);
	config.GetInletFlow().SetValue(2.0, VolumePerTimeUnit::L_Per_min);
	config.GetInspiratoryExpiratoryRatio().SetValue(.5);
	config.GetOxygenFraction().SetValue(.5);
	config.SetOxygenSource(CDM::enumAnesthesiaMachineOxygenSource::Wall);
	config.GetPositiveEndExpiredPressure().SetValue(0.0, PressureUnit::cmH2O);
	config.SetPrimaryGas(CDM::enumAnesthesiaMachinePrimaryGas::Nitrogen);
	config.GetReliefValvePressure().SetValue(20.0, PressureUnit::cmH2O);
	config.GetRespiratoryRate().SetValue(12, FrequencyUnit::Per_min);
	config.GetVentilatorPressure().SetValue(0.0, PressureUnit::cmH2O);
	config.GetOxygenBottleOne().GetVolume().SetValue(660.0, VolumeUnit::L);
	config.GetOxygenBottleTwo().GetVolume().SetValue(660.0, VolumeUnit::L);

	// Process the action to propagate state into the engine
	bg->ProcessAction(AMConfig);
	bg->GetLogger()->Info(std::stringstream() <<"Turning on the Anesthesia Machine and placing mask on patient for spontaneous breathing with machine connection.");;

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("The patient is attempting to breath normally with Anesthesia Machine connected");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

    // Get the Succinylcholine substance from the substance manager
	const SESubstance* succs = bg->GetSubstanceManager().GetSubstance("Succinylcholine");

    // Create a substance bolus action to administer the substance
	SESubstanceBolus bolus(*succs);
	bolus.GetConcentration().SetValue(4820, MassPerVolumeUnit::ug_Per_mL);
	bolus.GetDose().SetValue(20, VolumeUnit::mL);
	bolus.SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
	bg->ProcessAction(bolus);
	
  bg->GetLogger()->Info("Giving the patient Succinylcholine to test machine-driven ventilation.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("It has been 60s since the Succinylcholine administration.");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	config.GetInletFlow().SetValue(5.0, VolumePerTimeUnit::L_Per_min);
	config.GetPositiveEndExpiredPressure().SetValue(3.0, PressureUnit::cmH2O);
	config.GetVentilatorPressure().SetValue(22.0, PressureUnit::cmH2O);
	bg->ProcessAction(AMConfig);
	bg->GetLogger()->Info("Setting the ventilator pressure to drive the machine. Also increasing the inlet flow and positive end expired pressure to test machine controls.");

	bg->AdvanceModelTime(60, TimeUnit::s);
	 
	bg->GetLogger()->Info("Patient breathing is being controlled by the machine.");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	config.GetInspiratoryExpiratoryRatio().SetValue(1.0);
	config.GetPositiveEndExpiredPressure().SetValue(1.0, PressureUnit::cmH2O);
	config.GetRespiratoryRate().SetValue(18.0, FrequencyUnit::Per_min);
	config.GetVentilatorPressure().SetValue(10.0, PressureUnit::cmH2O);
	bg->ProcessAction(AMConfig);
	bg->GetLogger()->Info("More Anesthesia Machine control manipulation. Increasing respiratory rate, reducing driving pressure and increasing the inspiratory-expiratory ratio.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("Patient breathing is being controlled by the machine.");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	SEMaskLeak AMleak;
	AMleak.GetSeverity().SetValue(0.5);
	bg->ProcessAction(AMleak);
	bg->GetLogger()->Info("Testing an anesthesia machine failure mode. The mask is leaking with a severity of 0.5.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("Patient breathing is being controlled by the machine. The mask has been leaking for 60 seconds.");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());;

	AMleak.GetSeverity().SetValue(0.0);
	bg->ProcessAction(AMleak);
	bg->GetLogger()->Info("Removing the mask leak.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	SEOxygenWallPortPressureLoss AMpressureloss;
	AMpressureloss.SetActive(true);
	bg->ProcessAction(AMpressureloss);
	bg->GetLogger()->Info("Testing the oxygen pressure loss failure mode. The oxygen pressure from the wall source is dropping.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("Patient breathing is being controlled by the machine. The wall oxygen pressure loss occurred 60 seconds ago.");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());

	AMpressureloss.SetActive(false);
	bg->ProcessAction(AMpressureloss);
	bg->GetLogger()->Info("Removing the wall oxygen pressure loss action.");

	bg->AdvanceModelTime(60, TimeUnit::s);

	bg->GetLogger()->Info("The anesthesia machine is operating normally");
	bg->GetLogger()->Info(std::stringstream() <<"Tidal Volume : " << bg->GetRespiratorySystem()->GetTidalVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Respiration Rate : " << bg->GetRespiratorySystem()->GetRespirationRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Oxygen Saturation : " << bg->GetBloodChemistrySystem()->GetOxygenSaturation());
  bg->GetLogger()->Info("Finished");
}
