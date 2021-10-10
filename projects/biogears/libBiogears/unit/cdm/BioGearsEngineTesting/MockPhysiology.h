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

#include <gmock/gmock.h>
#include "HumanSimPhysiology.h"

class MockPhysiology : public Physiology
{
	MOCK_METHOD0(GetLogger, Logger*());
	MOCK_METHOD0(GetBody, Body*());
	MOCK_METHOD0(GetPatientProfile, PatientData*());
	MOCK_METHOD0(GetCardioVascularModel, CardioVascularModel*());
	MOCK_METHOD0(GetRespiratoryModel, RespiratoryModel*());
	MOCK_METHOD0(GetAnesthesiaMachine, AnesthesiaMachineDraeger*());
	MOCK_METHOD0(GetECG, ECG*());
	MOCK_METHOD0(GetHormones, Hormones*());
	MOCK_METHOD0(GetCO2, CO2*());
	MOCK_METHOD0(GetO2, O2*());
	MOCK_METHOD0(GetDrugs, Drugs*());
	MOCK_METHOD0(GetBloodChemistry, RenalFunction*());
};