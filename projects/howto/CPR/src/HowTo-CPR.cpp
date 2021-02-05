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
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/patient/actions/SECardiacArrest.h>
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// A custom class for end users to listen to patient events
///
/// \details
/// It is possible during the course of the simulation for the patient or equipment to reach certain
/// states (hypoxia, tachycardia, etc.).  When a state change occurs, a physiology engine event
/// describing the state change is sent to the appropriate handlers.  Below is an example of a custom
/// event handler which we use to listen for patient and anesthesia machine events.
//--------------------------------------------------------------------------------------------------
class MyListener : public SEEventHandler
{
public:
  MyListener(Logger* logger)
    : SEEventHandler()
    , log ( logger )
  {};
	virtual void HandlePatientEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime* time) override
	{
		log->Info(std::stringstream() <<"Recieved Patient Event : " << type);
	}

	virtual void HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const SEScalarTime* time) override
	{
		log->Info(std::stringstream() <<"Recieved Anesthesia Machine Event : " << type);
	}
private:
  Logger *log;
};

//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for adminstering CPR to a patient
///
/// \details
/// Give patient Succinylcholine to stop heart, then give CPR
/// Refer to the SESubstanceBolus class
/// Refer to the SESubstanceManager class
/// This example also shows how to listen to patient events.
//--------------------------------------------------------------------------------------------------
void HowToCPR()
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToCPR.log");
  bg->GetLogger()->Info("HowToCPR");
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
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartStrokeVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartEjectionFraction");
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput",VolumePerTimeUnit::mL_Per_min);
	bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodChemistry-InflammatoryResponse-Interleukin6",VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreateLiquidCompartmentDataRequest().Set(BGE::VascularCompartment::Brain, "InFlow", VolumePerTimeUnit::mL_Per_min);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToCPR.csv");

  // This is the total amount of time that CPR will be administered in seconds
	double durationOfCPR_Seconds = 120;
	
  // This is the frequency at which CPR is administered
  double compressionRate_BeatsPerMinute = 100;

  // This is where you specify how much force to apply to the chest. We have capped the applicable force at 600 N.
	double compressionForce_Newtons = 400;

  // This is the percent of time per period that the chest will be compressed e.g. if I have a 1 second period
  // (60 beats per minute) the chest will be compressed for 0.3 seconds
	double percentOn = .3;

	bg->GetLogger()->Info("The patient is nice and healthy");
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << bg->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << bg->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << bg->GetCardiovascularSystem()->GetHeartEjectionFraction());;

	bg->AdvanceModelTime(50, TimeUnit::s);

	// Put the patient into cardiac arrest
	SECardiacArrest c;
	bg->ProcessAction(c);
	
  bg->GetLogger()->Info("Giving the patient Cardiac Arrest.");

	// Let's add a listener which will print any state changes that patient undergoes
	MyListener l(bg->GetLogger());
	bg->GetPatient().ForwardEvents(&l);
	
	bg->AdvanceModelTime(10, TimeUnit::s);

	bg->GetLogger()->Info("It has been 10s since the administration, not doing well...");
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << bg->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << bg->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << bg->GetCardiovascularSystem()->GetHeartEjectionFraction());;


	// After patient's heart is not beating, start doing CPR
	SEChestCompressionForce chestCompression;

  // The period is calculated via 1 / compressionRate.  Because the compression rate is given
  // in beats per minute it is divided by 60 to give a period in seconds.
	double pulsePeriod_s = 1.0 / (compressionRate_BeatsPerMinute / 60.0);

  // The amount of time the chest will be compressed, calculated from the period and percentOn
	double timeOn = percentOn * pulsePeriod_s;

  // The rest of the time there will be no force on the chest, this is calculated from the
	double timeOff = pulsePeriod_s - timeOn;

  // This timer is used to keep track of how long CPR has been administered
	double timer1 = 0;

  // This boolean is used to determine if the chest is compressed or not. It starts as true
  // so the chest will be compressed on the next calculation from the engine.
	bool pulseState = true;
	
  bg->GetLogger()->Info("Patient is in asystole. Begin performing CPR");
  while (timer1 < durationOfCPR_Seconds) // CPR is administered in this loop. It is time based, so after timer1 has exceeded the specified duration of CPR it will stop. set pulseState to true so that it will only exit AFTER a compression has been removed
	{
		if (pulseState) // check if the chest is supposed to be compressed. If yes...
		{
            // This calls the CPR function in the Cardiovascular system.  It sets the chest compression at the specified force.
			chestCompression.GetForce().SetValue(compressionForce_Newtons, ForceUnit::N);
			bg->ProcessAction(chestCompression);

            // Time is advanced until it is time to remove the compression
			bg->AdvanceModelTime(timeOn, TimeUnit::s);
            
            // Increment timer1 by the time the chest was compressed
			timer1 += timeOn;

            // Specify that the compression is to now be removed.
			pulseState = false;
		}
		else
		{
            // This removes the chest compression by specifying the applied force as 0 N
			chestCompression.GetForce().SetValue(0, ForceUnit::N);
			bg->ProcessAction(chestCompression);
            
            // Time is advanced until it is time to compress the chest again
			bg->AdvanceModelTime(timeOff, TimeUnit::s);
            
            // Increment timer1 by the time the chest was no compressed
			timer1 += timeOff;

            // Set pulse state back to true.
			pulseState = true;
		}
	}

	// Make sure that the chest is no longer being compressed
	if (chestCompression.GetForce().GetValue(ForceUnit::N) != 0)
	{
        // If it is compressed, set force to 0 to turn off
		chestCompression.GetForce().SetValue(0, ForceUnit::N);
		bg->ProcessAction(chestCompression);
	}

	// Do one last output to show status after CPR.
	bg->GetLogger()->Info("Check on the patient's status after CPR has been performed");
	bg->GetLogger()->Info(std::stringstream() <<"Systolic Pressure : " << bg->GetCardiovascularSystem()->GetSystolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Diastolic Pressure : " << bg->GetCardiovascularSystem()->GetDiastolicArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
	bg->GetLogger()->Info(std::stringstream() <<"Stroke Volume : " << bg->GetCardiovascularSystem()->GetHeartStrokeVolume(VolumeUnit::mL) << VolumeUnit::mL);
	bg->GetLogger()->Info(std::stringstream() <<"Cardiac Output : " << bg->GetCardiovascularSystem()->GetCardiacOutput(VolumePerTimeUnit::mL_Per_min) << VolumePerTimeUnit::mL_Per_min);
	bg->GetLogger()->Info(std::stringstream() <<"Arterial Pressure : " << bg->GetCardiovascularSystem()->GetArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
	bg->GetLogger()->Info(std::stringstream() <<"Heart Ejection Fraction : " << bg->GetCardiovascularSystem()->GetHeartEjectionFraction());
  bg->GetLogger()->Info("Finished");
}
