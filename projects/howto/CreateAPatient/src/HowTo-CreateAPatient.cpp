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
#include <biogears/cdm/enums/SEPatientEnums.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Creating a patient
///
/// \details
/// Creating a customized patient in BioGears
//--------------------------------------------------------------------------------------------------
int HowToCreateAPatient()
{
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToEngineUse.log");
  bg->GetLogger()->Info("HowToCreateAPatient");

  SEPatient patient(bg->GetLogger());
  patient.SetName("HowToCreateAPatient");
  //Patient Gender is the only thing that is absolutely required to be set.
  //All value not explicitly set based or standard values or calculations.
  //If you do something out of bounds or set something you're not allowed to, it will alert you with a warning/error.
  patient.SetSex(biogears::SESex::Male);
  patient.GetAge().SetValue(44, TimeUnit::yr);
  patient.GetWeight().SetValue(170, MassUnit::lb);
  patient.GetHeight().SetValue(71, LengthUnit::inch);
  patient.GetBodyFatFraction().SetValue(0.21);
  patient.GetDiastolicArterialPressureBaseline().SetValue(74, PressureUnit::mmHg);
  patient.GetHeartRateBaseline().SetValue(72, FrequencyUnit::Per_min);
  patient.GetRespirationRateBaseline().SetValue(16, FrequencyUnit::Per_min);
  patient.GetSystolicArterialPressureBaseline().SetValue(114, PressureUnit::mmHg);

  if (!bg->InitializeEngine(patient)) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
  }

  // You can save off the initial patient state if you want to use it later
  bg->SaveStateToFile("./states/HowToCreateAPatient@0s.xml");
  return 0;
}

int main ( int argc, char* argv[] ) {
  return HowToCreateAPatient();
}
