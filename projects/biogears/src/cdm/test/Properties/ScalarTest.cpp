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

// Testing some basic circuit functionality testing
#include <biogears/cdm/properties/SEDecimalFormat.h>
#include <biogears/cdm/properties/SEScalarAmount.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFlowCompliance.h>
#include <biogears/cdm/properties/SEScalarFlowInertance.h>
#include <biogears/cdm/properties/SEScalarFlowResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerPressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
void CommonDataModelTest::ScalarTest(const std::string& sOutputDirectory)
{
  std::ofstream file;
  SEDecimalFormat dfault;
  dfault.SetNotation(DecimalNotation::Default);
  dfault.SetPrecision(6);
  SEDecimalFormat fixed;
  dfault.SetNotation(DecimalNotation::Fixed);
  dfault.SetPrecision(8);
  SEDecimalFormat sci;
  dfault.SetNotation(DecimalNotation::Scientific);
  dfault.SetPrecision(4);

  DataTrack track;
  track.SetFormatting("Default", dfault);
  track.SetFormatting("Fixed", fixed);
  track.SetFormatting("Scientific", sci);
  track.CreateFile("TrackText.csv", file);
  for (int i = 0; i < 10; i++) {
    double d = std::sqrt(i) + 100;
    track.Probe("Default", d);
    track.Probe("Fixed", d);
    track.Probe("Scientific", d);
    track.StreamProbesToFile(i, file);
    std::cout << d << std::endl;
  }

  CCompoundUnit g_Per_m3("g/m^3");
  CCompoundUnit ug_Per_mL("ug/mL");
  Convert(1, g_Per_m3, ug_Per_mL);

  SEScalarLength d;
  d.SetValue(1, LengthUnit::m);
  std::cout << d << std::endl;
  std::cout << d.GetValue(LengthUnit::m) << std::endl;
  std::cout << d.GetValue(LengthUnit::cm) << std::endl;

  SEScalarTemperature t;
  t.SetValue(5, TemperatureUnit::C);
  std::cout << t.GetValue(TemperatureUnit::K) << std::endl;
  t.SetValue(5, TemperatureUnit::K);
  std::cout << t.GetValue(TemperatureUnit::C) << std::endl;

  SEScalarFlowResistance r;
  r.SetValue(0, FlowResistanceUnit::GetCompoundUnit("mmHg s/mL"));

  SEScalarVolume v;
  v.SetValue(1, VolumeUnit::L);
  double m3 = v.GetValue(VolumeUnit::m3); //0.0010000000000000000
  v.IncrementValue(-2 * 6, VolumeUnit::m3);
  m3 = v.GetValue(VolumeUnit::m3); //m3 = -1.9990000000000001

  SEScalarPressure p;
  p.SetValue(1, PressureUnit::Pa);

  //
  //if(!p.IsValidUnit("Pa"))
  //  std::cerr<<"Pa should be a valid Pressure unit"<<std::endl;
  //if(p.IsValidUnit("Pap"))
  //  std::cerr<<"Pap should NOT be a valid Pressure unit"<<std::endl;
  //p.SetValue(5,PressureUnit::Pa);
  //std::cout<<"Testing Scalar "<<p.GetValue(PressureUnit::Pa)<<PressureUnit::Pa<<std::endl;
  //std::cout<<"Testing Scalar "<<p.GetValue(PressureUnit::mmHg)<<PressureUnit::mmHg<<std::endl;
  //std::cout<<"Testing Scalar "<<p.GetValue(PressureUnit::cmH2O)<<PressureUnit::cmH2O<<std::endl;
}
}