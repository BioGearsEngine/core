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
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEFunctionElectricPotentialVsTime.h>
#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolatorWaveform.h>
#include <biogears/io/io-manager.h>
#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/ecg.h>
#endif

namespace std {
template class map<CDM::enumHeartRhythm, biogears::SEElectroCardioGramInterpolatorWaveform*>;
template class map<CDM::ElectroCardioGramWaveformLeadNumber, biogears::SEScalarElectricPotential*>;
template class map<CDM::ElectroCardioGramWaveformLeadNumber, map<CDM::enumHeartRhythm, biogears::SEElectroCardioGramInterpolatorWaveform*>>;
}

namespace biogears {
SEElectroCardioGramInterpolator::SEElectroCardioGramInterpolator(Logger* logger)
  : Loggable(logger)
{
}
//-------------------------------------------------------------------------------
SEElectroCardioGramInterpolator::~SEElectroCardioGramInterpolator()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::Clear()
{
  for (auto i : m_Waveforms)
    for (auto j : i.second)
      delete j.second;
  m_Waveforms.clear();
  m_Leads.clear();
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::LoadWaveforms(const char* file, const SEScalarTime* timeStep)
{
  return LoadWaveforms(std::string { file }, timeStep);
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::LoadWaveforms(const std::string& given_path, const SEScalarTime* timeStep)
{
  Clear();
  std::stringstream ss;
  if (given_path.empty()) {
    ss << "Waveform file not provided: " << given_path << std::endl;
    Error(ss);
    return false;
  }
  Clear();

  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindEcgFile(given_path.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    std::string resource_id = filesystem::path(given_path).basename();
    auto content = io::get_embedded_ecg_file(resource_id.c_str(), content_size);
    data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  CDM::ElectroCardioGramWaveformInterpolatorData* pData = dynamic_cast<CDM::ElectroCardioGramWaveformInterpolatorData*>(data.get());
  if (pData == nullptr) {
    ss << "Waveform data file could not be read : " << given_path << std::endl;
    Error(ss);
    return false;
  }
  if (!Load(*pData)) {
    ss << "Unable to load waveform file: " << given_path << std::endl;
    Error(ss);
    return false;
  }
  if (timeStep != nullptr)
    Interpolate(*timeStep);
  return true;
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::Load(const CDM::ElectroCardioGramWaveformInterpolatorData& in)
{
  Clear();
  for (auto& w : in.Waveform()) {
    SEElectroCardioGramInterpolatorWaveform* waveform = new SEElectroCardioGramInterpolatorWaveform(GetLogger());
    if (!waveform->Load(w)) {
      Error("Unable to load waveform");
      return false;
    } else
      m_Waveforms[waveform->GetLeadNumber()][waveform->GetRhythm()] = waveform;
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::ElectroCardioGramWaveformInterpolatorData* SEElectroCardioGramInterpolator::Unload() const
{
  CDM::ElectroCardioGramWaveformInterpolatorData* data(new CDM::ElectroCardioGramWaveformInterpolatorData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::Unload(CDM::ElectroCardioGramWaveformInterpolatorData& data) const
{
  for (auto i : m_Waveforms)
    for (auto j : i.second)
      data.Waveform().push_back(std::unique_ptr<CDM::ElectroCardioGramInterpolationWaveformData>(j.second->Unload()));
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::Interpolate(const SEScalarTime& timeStep)
{
  for (auto& l : m_Waveforms)
    for (auto& w : l.second)
      if (w.second != nullptr)
        Interpolate(*w.second, timeStep);
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::Interpolate(SEElectroCardioGramInterpolatorWaveform& w, const SEScalarTime& timeStep)
{
  SEFunctionElectricPotentialVsTime& data = w.GetData();
  SEScalarTime* waveformTimeStep = nullptr;
  if (w.HasTimeStep())
    waveformTimeStep = &w.GetTimeStep();

  bool interpolate = true; // now we need to make the data correspond to our time step.
  if (waveformTimeStep != nullptr) {
    if (waveformTimeStep->Equals(timeStep)) // if the data is already sampled at the engine's time step, no interpolation is needed
      interpolate = false;
  }
  if (interpolate) {
    // NOTE: This assumes that the data is a SINGLE waveform
    std::vector<double> iTime;
    std::vector<double>& wTime = data.GetTime();
    std::vector<double>& wEleP = data.GetElectricPotential();
    double currentTime_s = 0;
    double timeStep_s = timeStep.GetValue(TimeUnit::s);
    double endTime_s = wTime[wTime.size() - 1];
    while (currentTime_s <= endTime_s) // figure out how many data points are needed and populate the new time vector
    {
      iTime.push_back(currentTime_s);
      currentTime_s += timeStep_s;
    }
    SEFunctionElectricPotentialVsTime* iWaveForm = data.InterpolateToTime(iTime, TimeUnit::s); // creates the new waveform data
    CDM_COPY(iWaveForm, (&data));
    delete iWaveForm;
  }
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::CanInterpolateLeadPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const
{
  if (!HasWaveform(lead, rhythm))
    return false;
  auto l = m_Leads.find(lead);
  if (l == m_Leads.end())
    return false;
  return l->second != nullptr;
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::SetLeadElectricPotential(CDM::ElectroCardioGramWaveformLeadNumber lead, SEScalarElectricPotential& ep)
{
  m_Leads[lead] = &ep;
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::StartNewCycle(CDM::enumHeartRhythm::value rhythm)
{
  for (auto l2rw : m_Waveforms) {
    if (m_Leads.find(l2rw.first) == m_Leads.end() && !HasWaveform(l2rw.first, rhythm)) {
      std::stringstream ss;
      ss << "Waveform not provided for Lead " << l2rw.first << " rhythm " << rhythm;
      Error(ss);
      return false;
    }
    l2rw.second[rhythm]->GetActiveIndicies().push_back(0);
  }
  return true;
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Prepares the ECG output data
///
/// \details
/// CalculateWaveform takes data from all active iterators and sums them together. It also assigns
/// the unit of millivolts to the output. If there are no active iterators, the output defaults to 0.
//--------------------------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::CalculateWaveformsElectricPotential()
{
  // Pull Data from our iterators
  int idx;
  double val;
  for (auto& l2s : m_Leads) //Lead# to Scalar
  {
    SEScalarElectricPotential* ep = l2s.second;
    if (ep == nullptr)
      continue;
    ep->SetValue(0, ElectricPotentialUnit::mV);

    for (auto& r2w : m_Waveforms[l2s.first]) //rhythm to indecies
    {
      int i = 0;
      SEElectroCardioGramInterpolatorWaveform* waveform = r2w.second;
      if (waveform == nullptr)
        continue;
      SEFunctionElectricPotentialVsTime& data = waveform->GetData();
      for (unsigned int i = 0; i < waveform->GetActiveIndicies().size(); i++) {
        idx = waveform->GetActiveIndicies()[i];
        val = data.GetElectricPotentialValue(idx++, *data.GetElectricPotentialUnit());
        ep->IncrementValue(val, *data.GetElectricPotentialUnit());
        if (idx == data.Length())
          waveform->GetActiveIndicies().erase(waveform->GetActiveIndicies().begin() + i--); // Remove the iterator if we are at the end
        else
          waveform->GetActiveIndicies()[i] = idx; // Increment iterator
      }
    }
  }
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramInterpolator::HasWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return false;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return false;
  return w->second != nullptr;
}
//-------------------------------------------------------------------------------
SEElectroCardioGramInterpolatorWaveform& SEElectroCardioGramInterpolator::GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm)
{
  SEElectroCardioGramInterpolatorWaveform* w = m_Waveforms[lead][rhythm];
  if (w == nullptr) {
    w = new SEElectroCardioGramInterpolatorWaveform(GetLogger());
    m_Waveforms[lead][rhythm] = w;
  }
  return *w;
}
//-------------------------------------------------------------------------------
const SEElectroCardioGramInterpolatorWaveform* SEElectroCardioGramInterpolator::GetWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm) const
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return nullptr;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return nullptr;
  return w->second;
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramInterpolator::RemoveWaveform(CDM::ElectroCardioGramWaveformLeadNumber lead, CDM::enumHeartRhythm::value rhythm)
{
  auto l = m_Waveforms.find(lead);
  if (l == m_Waveforms.end())
    return;
  auto w = l->second.find(rhythm);
  if (w == l->second.end())
    return;
  delete w->second;
  w->second = nullptr;
}
//-------------------------------------------------------------------------------
}
