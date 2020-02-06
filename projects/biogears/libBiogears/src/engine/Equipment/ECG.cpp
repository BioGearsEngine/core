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
#include <biogears/engine/Controller/BioGears.h>

#include <biogears/engine/Equipment/ECG.h>
#include <biogears/engine/Systems/Cardiovascular.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>
#include <biogears/engine/Controller/BioGearsSystem.h>

#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
/*
========================
Constructors
========================
*/

auto ECG::make_unique(BioGears& bg) -> std::unique_ptr<ECG>
{
  return std::unique_ptr<ECG>(new ECG(bg));
}

ECG::ECG(BioGears& bg)
  : SEElectroCardioGram(bg.GetLogger())
  , m_data(bg)
  , m_interpolator(bg.GetLogger())
{
  Clear();
}

ECG::~ECG()
{
  Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief Cleans up any memory this class is responsible for
//--------------------------------------------------------------------------------------------------
void ECG::Clear()
{
  SEElectroCardioGram::Clear();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Initializes the ECG specific quantities
///
/// \details
/// Local member variables used in the ECG class are initialized here. These variables are used to
/// keep track of the heart cycle and to create the ECG machine's output.
/// SEElectrocardioGramInterpolater also loads in the data needed to create the output and
/// interpolates it to match our time step. Placing the variables in this function allows for the
/// system to be easy initialized at the beginning of a simulation or after a crash.
//--------------------------------------------------------------------------------------------------
void ECG::Initialize()
{
  BioGearsSystem::Initialize();

  m_heartRhythmTime.SetValue(0, TimeUnit::s);
  m_heartRhythmPeriod.SetValue(0, TimeUnit::s);
  CDM_COPY(m_data.GetConfiguration().GetECGInterpolator(), (&m_interpolator));
  // You can uncomment this code to compare the original waveform to the interpolated waveform and make sure you are capturing the data properly
  /* Code to write out the ECG data in a format easy to view in plotting tools 
  std::vector<double> original_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
  std::vector<double> original_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
  DataTrack Original;  
  for (size_t i = 0; i < original_s.size(); i++)
    Original.Track("Original_ECG",original_s[i], original_mV[i]);
  Original.WriteTrackToFile("OriginalECG.csv");
*/
  m_interpolator.Interpolate(m_data.GetTimeStep());
  /* Code to write out the Interpolated ECG data in a format easy to view in plotting tools
  std::vector<double> interpolated_s = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetTime();
  std::vector<double> interpolated_mV = m_interpolator.GetWaveform(3, CDM::enumHeartRhythm::NormalSinus).GetData().GetElectricPotential();
  DataTrack Interpolated;
  for (size_t i = 0; i < interpolated_s.size(); i++)
    Interpolated.Track("Interpolated_ECG", interpolated_s[i], interpolated_mV[i]);
  Interpolated.WriteTrackToFile("InterpolatedECG.csv");
*/
  m_interpolator.SetLeadElectricPotential(3, GetLead3ElectricPotential());
}

bool ECG::Load(const CDM::BioGearsElectroCardioGramData& in)
{
  if (!SEElectroCardioGram::Load(in))
    return false;
  BioGearsSystem::LoadState();
  m_heartRhythmTime.Load(in.HeartRythmTime());
  m_heartRhythmPeriod.Load(in.HeartRythmPeriod());
  m_interpolator.Load(in.Waveforms());
  m_interpolator.SetLeadElectricPotential(3, GetLead3ElectricPotential());
  return true;
}
CDM::BioGearsElectroCardioGramData* ECG::Unload() const
{
  CDM::BioGearsElectroCardioGramData* data = new CDM::BioGearsElectroCardioGramData();
  Unload(*data);
  return data;
}
void ECG::Unload(CDM::BioGearsElectroCardioGramData& data) const
{
  SEElectroCardioGram::Unload(data);
  data.HeartRythmTime(std::unique_ptr<CDM::ScalarTimeData>(m_heartRhythmTime.Unload()));
  data.HeartRythmPeriod(std::unique_ptr<CDM::ScalarTimeData>(m_heartRhythmPeriod.Unload()));
  data.Waveforms(std::unique_ptr<CDM::ElectroCardioGramWaveformInterpolatorData>(m_interpolator.Unload()));
}

void ECG::SetUp()
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Prepares the ECG machine for the circuit solver
///
/// \details
/// At this time, we do not have a circuit model for the ECG machine so this function is left empty.
/// The PreProcess function is included to preserve our system methodology and as a placeholder for
/// models developed in the future.
//--------------------------------------------------------------------------------------------------
void ECG::PreProcess()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Process creates the ECG output
///
/// \details
/// The process function queries the heart rhythm and heart rate from the cardiovascular system. The
/// time period of the heart beat is determined from the heart rate and a new iterator on the waveform
/// is initialized if the queried rhythm is Normal Sinus. The iterator holds the value of the waveform
/// data at a specific time point on the interpolated waveform data. Process increments the time point
/// and sums up the value of any active iterators. The iterators continue being incremented until they
/// reach the end of the interpolated waveform data. If there are no active iterators, the output
/// defaults to 0 mV.
//--------------------------------------------------------------------------------------------------
void ECG::Process()
{
  m_heartRhythmTime.IncrementValue(m_dt_s, TimeUnit::s);
  if (m_heartRhythmTime.GetValue(TimeUnit::s) >= m_heartRhythmPeriod.GetValue(TimeUnit::s)) {
    m_heartRhythmTime.SetValue(0, TimeUnit::s);
    m_heartRhythmPeriod.SetValue(1 / m_data.GetCardiovascular().GetHeartRate(FrequencyUnit::Per_s), TimeUnit::s);
    // Currently we  have one data set for all currently supported Heart Rhythms
    // Eventually we will support multiple rhythmic data
    if (m_data.GetCardiovascular().GetHeartRhythm() == CDM::enumHeartRhythm::NormalSinus)
      m_interpolator.StartNewCycle(CDM::enumHeartRhythm::NormalSinus);
    else {
      m_ss << m_data.GetCardiovascular().GetHeartRhythm() << " is not a supported Heart Rhythm for ECG";
      Error(m_ss);
    }
  }
  m_interpolator.CalculateWaveformsElectricPotential();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Post process updates the math after the circuit calculation are done
///
/// \details
/// At this time, we do not have a circuit model for the ECG machine so this function is left empty.
/// The PostProcess function is included to preserve our system methodology and as a placeholder for
/// models developed in the future.
//--------------------------------------------------------------------------------------------------
void ECG::PostProcess()
{
}
}