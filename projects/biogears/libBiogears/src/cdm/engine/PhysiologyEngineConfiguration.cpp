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
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>

#include "io/cdm/EngineConfiguration.h"
#include "io/cdm/Property.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/engine/PhysiologyEngineDynamicStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineTimedStabilization.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
#include <biogears/io/io-manager.h>
#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/config.h>
#endif

namespace biogears {
PhysiologyEngineConfiguration::PhysiologyEngineConfiguration(Logger* logger)
  : Loggable(logger)
{
  m_Merge = false;
  m_ECGInterpolator = nullptr;
  m_StabilizationCriteria = nullptr;
  m_TimedStabilizationCriteria = nullptr;
  m_DynamicStabilizationCriteria = nullptr;
  m_TimeStep = nullptr;
  m_WritePatientBaselineFile = SEOnOff(-1);
}

//-----------------------------------------------------------------------------
PhysiologyEngineConfiguration::~PhysiologyEngineConfiguration()
{
  Clear();
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::Clear()
{
  SAFE_DELETE(m_ECGInterpolator);
  RemoveStabilizationCriteria();
  SAFE_DELETE(m_TimeStep);

  m_WritePatientBaselineFile = SEOnOff(-1);
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::Merge(const PhysiologyEngineConfiguration& from)
{
  m_Merge = true;
  CDM_COPY((&from), this);
  m_Merge = false;
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::Load(const char* file)
{
  return Load(std::string { file });
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::Load(const std::string& file)
{
  // if file does not exist, we stick with defaults

  CDM::PhysiologyEngineConfigurationData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindConfigFile(file.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    auto content = io::get_embedded_config_file(file.c_str(), content_size);
    data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  pData = dynamic_cast<CDM::PhysiologyEngineConfigurationData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Configuration file : " << file << " not found, using default configuration" << std::endl;
    Info(ss);
    return true;
  }
  return Load(*pData);
}
//-----------------------------------------------------------------------------

bool PhysiologyEngineConfiguration::Load(const CDM::PhysiologyEngineConfigurationData& in)
{
  if (!m_Merge)
    Clear(); // Reset only if we are not merging

  if (in.TimeStep().present())
    GetTimeStep().Load(in.TimeStep().get());
  if (in.WritePatientBaselineFile().present())
    io::Property::UnMarshall(in.WritePatientBaselineFile(), m_WritePatientBaselineFile);

  if (in.ElectroCardioGramInterpolatorFile().present()) {
    if (!GetECGInterpolator().LoadWaveforms(in.ElectroCardioGramInterpolatorFile().get())) {
      Error("Unable to load ElectroCardioGram Waveforms file");
      return false;
    }
  } else if (in.ElectroCardioGramInterpolator().present()) {
    if (!GetECGInterpolator().Load(in.ElectroCardioGramInterpolator().get())) {
      Error("Unable to load ElectroCardioGram Waveforms");
      return false;
    }
  }

  std::unique_ptr<CDM::ObjectData> sData;
  const CDM::PhysiologyEngineTimedStabilizationData* tData = nullptr;
  const CDM::PhysiologyEngineDynamicStabilizationData* dData = nullptr;
  if (in.StabilizationCriteriaFile().present()) {
    auto io = m_Logger->GetIoManager().lock();
    auto possible_path = io->FindConfigFile(in.StabilizationCriteriaFile().get().c_str());
    if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
      size_t content_size;
      auto content = io::get_embedded_config_file(in.StabilizationCriteriaFile().get().c_str(), content_size);
      sData = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
    } else {
      sData = Serializer::ReadFile(possible_path, m_Logger);
    }
    if (sData == nullptr) {
      Error("Unable to load Stabilization Criteria file");
      return false;
    }
    tData = dynamic_cast<const CDM::PhysiologyEngineTimedStabilizationData*>(sData.get());
    dData = dynamic_cast<const CDM::PhysiologyEngineDynamicStabilizationData*>(sData.get());
  } else if (in.StabilizationCriteria().present()) {
    tData = dynamic_cast<const CDM::PhysiologyEngineTimedStabilizationData*>(&in.StabilizationCriteria().get());
    dData = dynamic_cast<const CDM::PhysiologyEngineDynamicStabilizationData*>(&in.StabilizationCriteria().get());
  }
  if (tData != nullptr) {
    if (!GetTimedStabilizationCriteria().Load(*tData)) {
      Error("Unable to load Stabilization Criteria");
      return false;
    }
  } else if (dData != nullptr) {
    if (!GetDynamicStabilizationCriteria().Load(*dData)) {
      Error("Unable to load Stabilization Criteria");
      return false;
    }
  }

  return true;
}
//-----------------------------------------------------------------------------
CDM::PhysiologyEngineConfigurationData* PhysiologyEngineConfiguration::Unload() const
{
  CDM::PhysiologyEngineConfigurationData* data(new CDM::PhysiologyEngineConfigurationData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::Unload(CDM::PhysiologyEngineConfigurationData& data) const
{
  if (HasECGInterpolator())
    data.ElectroCardioGramInterpolator(std::unique_ptr<CDM::ElectroCardioGramInterpolatorData>(m_ECGInterpolator->Unload()));
  if (HasStabilizationCriteria())
    data.StabilizationCriteria(std::unique_ptr<CDM::PhysiologyEngineStabilizationData>(m_StabilizationCriteria->Unload()));
  if (HasTimeStep())
    data.TimeStep(std::unique_ptr<CDM::ScalarTimeData>(m_TimeStep->Unload()));
  if (HasWritePatientBaselineFile())
    io::Property::Marshall(m_WritePatientBaselineFile, data.WritePatientBaselineFile());    
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::HasECGInterpolator() const
{
  return m_ECGInterpolator != nullptr;
}
//-----------------------------------------------------------------------------
SEElectroCardioGramInterpolator& PhysiologyEngineConfiguration::GetECGInterpolator()
{
  if (m_ECGInterpolator == nullptr)
    m_ECGInterpolator = new SEElectroCardioGramInterpolator(GetLogger());
  return *m_ECGInterpolator;
}
//-----------------------------------------------------------------------------
const SEElectroCardioGramInterpolator* PhysiologyEngineConfiguration::GetECGInterpolator() const
{
  return m_ECGInterpolator;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::RemoveECGInterpolator()
{
  SAFE_DELETE(m_ECGInterpolator);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::HasStabilizationCriteria() const
{
  return m_StabilizationCriteria != nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineStabilization* PhysiologyEngineConfiguration::GetStabilizationCriteria()
{
  if ( nullptr == m_StabilizationCriteria) {
    m_StabilizationCriteria = &GetDynamicStabilizationCriteria();
  }
  return m_StabilizationCriteria;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::RemoveStabilizationCriteria()
{
  SAFE_DELETE(m_TimedStabilizationCriteria);
  SAFE_DELETE(m_DynamicStabilizationCriteria);
  m_StabilizationCriteria = nullptr; // Generic pointer used to point to one of the above pointers
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::HasTimedStabilizationCriteria() const
{
  return m_TimedStabilizationCriteria != nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineTimedStabilization& PhysiologyEngineConfiguration::GetTimedStabilizationCriteria()
{
  RemoveDynamicStabilizationCriteria();
  if (m_TimedStabilizationCriteria == nullptr) {
    m_TimedStabilizationCriteria = new PhysiologyEngineTimedStabilization(GetLogger());
    m_StabilizationCriteria = m_TimedStabilizationCriteria;
  }
  return *m_TimedStabilizationCriteria;
}
//-----------------------------------------------------------------------------
const PhysiologyEngineTimedStabilization* PhysiologyEngineConfiguration::GetTimedStabilizationCriteria() const
{
  return m_TimedStabilizationCriteria;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::RemoveTimedStabilizationCriteria()
{
  if (m_StabilizationCriteria == m_TimedStabilizationCriteria)
    m_StabilizationCriteria = nullptr;
  SAFE_DELETE(m_TimedStabilizationCriteria);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::HasDynamicStabilizationCriteria() const
{
  return m_DynamicStabilizationCriteria != nullptr;
}
//-----------------------------------------------------------------------------
PhysiologyEngineDynamicStabilization& PhysiologyEngineConfiguration::GetDynamicStabilizationCriteria()
{
  RemoveTimedStabilizationCriteria();
  if (m_DynamicStabilizationCriteria == nullptr) {
    m_DynamicStabilizationCriteria = new PhysiologyEngineDynamicStabilization(GetLogger());
    m_StabilizationCriteria = m_DynamicStabilizationCriteria;
  }
  return *m_DynamicStabilizationCriteria;
}
//-----------------------------------------------------------------------------
const PhysiologyEngineDynamicStabilization* PhysiologyEngineConfiguration::GetDynamicStabilizationCriteria() const
{
  return m_DynamicStabilizationCriteria;
}
//-----------------------------------------------------------------------------
void PhysiologyEngineConfiguration::RemoveDynamicStabilizationCriteria()
{
  if (m_StabilizationCriteria == m_DynamicStabilizationCriteria)
    m_StabilizationCriteria = nullptr;
  SAFE_DELETE(m_DynamicStabilizationCriteria);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::HasTimeStep() const
{
  return m_TimeStep == nullptr ? false : m_TimeStep->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarTime& PhysiologyEngineConfiguration::GetTimeStep()
{
  if (m_TimeStep == nullptr)
    m_TimeStep = new SEScalarTime();
  return *m_TimeStep;
}
//-----------------------------------------------------------------------------
double PhysiologyEngineConfiguration::GetTimeStep(const TimeUnit& unit) const
{
  if (m_TimeStep == nullptr)
    return SEScalar::dNaN();
  return m_TimeStep->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool PhysiologyEngineConfiguration::operator==(PhysiologyEngineConfiguration const& rhs) const
{
  if (this == &rhs)
    return true;

  bool equivilant = ((m_ECGInterpolator && rhs.m_ECGInterpolator) ? m_ECGInterpolator->operator==(*rhs.m_ECGInterpolator) : m_ECGInterpolator == rhs.m_ECGInterpolator) 
    ;equivilant &= ((m_StabilizationCriteria && rhs.m_StabilizationCriteria) ? m_StabilizationCriteria->operator==(*rhs.m_StabilizationCriteria) : m_StabilizationCriteria == rhs.m_StabilizationCriteria)
    ;equivilant &= ((m_TimedStabilizationCriteria && rhs.m_TimedStabilizationCriteria) ? m_TimedStabilizationCriteria->operator==(*rhs.m_TimedStabilizationCriteria) : m_TimedStabilizationCriteria == rhs.m_TimedStabilizationCriteria)
    ;equivilant &= ((m_DynamicStabilizationCriteria && rhs.m_DynamicStabilizationCriteria) ? m_DynamicStabilizationCriteria->operator==(*rhs.m_DynamicStabilizationCriteria) : m_DynamicStabilizationCriteria == rhs.m_DynamicStabilizationCriteria)
    ;equivilant &= ((m_TimeStep && rhs.m_TimeStep) ? m_TimeStep->operator==(*rhs.m_TimeStep) : m_TimeStep == rhs.m_TimeStep)
    ;equivilant &= ((m_Patient && rhs.m_Patient) ? m_Patient->operator==(*rhs.m_Patient) : m_Patient == rhs.m_Patient)
    ;equivilant &= ((m_overrideMode && rhs.m_overrideMode) ? *m_overrideMode == *rhs.m_overrideMode : m_overrideMode == rhs.m_overrideMode)
    ;equivilant &= m_WritePatientBaselineFile == rhs.m_WritePatientBaselineFile;
  ;
  return equivilant;
}
bool PhysiologyEngineConfiguration::operator!=(PhysiologyEngineConfiguration const& rhs) const
{
  return !(*this == rhs);
}
//-----------------------------------------------------------------------------
}