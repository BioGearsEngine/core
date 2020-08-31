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
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for performing a Pulmonary Function Test (PFT) on the patient
///
/// \details
/// Refer to the SEPulmonaryFunctionTest class
/// Refer to the SEPatient class
/// Refer to the SERespiratory class
//--------------------------------------------------------------------------------------------------
void HowToPulmonaryFunction()
{
  // Create the engine and load the patient
	std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToPulmonaryFunctionTest.log");
  bg->GetLogger()->Info("HowToPulmonaryFunctionTest");
	if (!bg->LoadState("./states/StandardMale@0s.xml"))
  {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

	// Let's analyze the respiratory system more specifically by performing a Pulmonary Function Test (PFT)
	// The PFT Calculates a wave form representing total lung volume during a normal breathing cycle, 
	// forced inhalation and exhalation from current tidal volume and engine parameters

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

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToPulmonaryFunctionTest.csv");

	bg->AdvanceModelTime(5, TimeUnit::s);

	SEPulmonaryFunctionTest pft;
	bg->GetPatientAssessment(pft);

	bg->GetLogger()->Info("Performing PFT at time 0s");
	bg->GetLogger()->Info("Currently these are the PFT properties computed by the BioGears engine");
	bg->GetLogger()->Info(std::stringstream() <<"Expiratory Reserve Volume"    << pft.GetExpiratoryReserveVolume()  ); 
	bg->GetLogger()->Info(std::stringstream() <<"Functional Residual Capacity" << pft.GetFunctionalResidualCapacity());
	bg->GetLogger()->Info(std::stringstream() <<"Inspiratory Capacity"         << pft.GetInspiratoryCapacity()      );
	bg->GetLogger()->Info(std::stringstream() <<"Inspiratory Reserve Volume"   << pft.GetInspiratoryReserveVolume() );
	bg->GetLogger()->Info(std::stringstream() <<"Residual Volume"              << pft.GetResidualVolume()           );
	bg->GetLogger()->Info(std::stringstream() <<"Total Lung Capacity"          << pft.GetTotalLungCapacity()        );
	bg->GetLogger()->Info(std::stringstream() <<"Vital Capacity"               << pft.GetVitalCapacity()            );
	bg->GetLogger()->Info("Currently, BioGears does not support calculation of the following values:");

	// Values will be NaN
	bg->GetLogger()->Info(std::stringstream() <<"Forced Vital Capacity"        << pft.GetForcedVitalCapacity()       );
	bg->GetLogger()->Info(std::stringstream() <<"Forced Expiratory Volume"     << pft.GetForcedExpiratoryVolume()    );
	bg->GetLogger()->Info(std::stringstream() <<"Forced Expiratory Flow"       << pft.GetForcedExpiratoryFlow()      );
	bg->GetLogger()->Info(std::stringstream() <<"Maximum Voluntary Ventilation"<< pft.GetMaximumVoluntaryVentilation());
	bg->GetLogger()->Info(std::stringstream() <<"Peak Expiratory Flow"         << pft.GetPeakExpiratoryFlow()        );
	bg->GetLogger()->Info(std::stringstream() <<"Slow Vital Capacity"          << pft.GetSlowVitalCapacity()         );
	
	// BioGears does compute the LungVolumePlot Data
	//The resulting plot is obtained which displays lung volume as a function of time 
	SEFunctionVolumeVsTime& lungVolumePlot = pft.GetLungVolumePlot();

	//The results may be displayed from from the abscissa (time) and ordinate (lung volume) of the lung volume plot
	lungVolumePlot.GetTime(); //This is the time component of the pulmonary function test
	lungVolumePlot.GetVolume(); //This is the lung volume component of the pulmonary function test
	// This is intended to be a a data form that can easily be plotted.
  bg->GetLogger()->Info("Finished");
}
