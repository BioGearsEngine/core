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
#include <biogears/cdm/patient/assessments/SEArterialBloodGasAnalysis.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/schema/cdm/Properties.hxx>

OPEN_BIOGEARS_NAMESPACE
SEArterialBloodGasAnalysis::SEArterialBloodGasAnalysis()
{
  m_pH = nullptr;
  m_PartialPressureOxygen = nullptr;
  m_PartialPressureCarbonDioxide = nullptr;
  m_BaseExcess = nullptr;
  m_StandardBicarbonate = nullptr;
  m_OxygenSaturation = nullptr;
  
}

SEArterialBloodGasAnalysis::~SEArterialBloodGasAnalysis()
{
  Clear();
}

void SEArterialBloodGasAnalysis::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_pH);
  SAFE_DELETE(m_PartialPressureOxygen);
  SAFE_DELETE(m_PartialPressureCarbonDioxide);
  SAFE_DELETE(m_BaseExcess);
  SAFE_DELETE(m_StandardBicarbonate);
  SAFE_DELETE(m_OxygenSaturation);
}

void SEArterialBloodGasAnalysis::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_pH);
  INVALIDATE_PROPERTY(m_PartialPressureOxygen);
  INVALIDATE_PROPERTY(m_PartialPressureCarbonDioxide);
  INVALIDATE_PROPERTY(m_BaseExcess);
  INVALIDATE_PROPERTY(m_StandardBicarbonate);
  INVALIDATE_PROPERTY(m_OxygenSaturation);
}

bool SEArterialBloodGasAnalysis::Load(const CDM::ArterialBloodGasAnalysisData& in)
{
  SEPatientAssessment::Load(in);
  return true;
}

CDM::ArterialBloodGasAnalysisData* SEArterialBloodGasAnalysis::Unload()
{
  CDM::ArterialBloodGasAnalysisData* data = new CDM::ArterialBloodGasAnalysisData();
  Unload(*data);
  return data;
}

void SEArterialBloodGasAnalysis::Unload(CDM::ArterialBloodGasAnalysisData& data)
{
  SEPatientAssessment::Unload(data);
  if (m_pH != nullptr)
    data.pH(std::unique_ptr<CDM::ScalarData>(m_pH->Unload()));
  if (m_PartialPressureOxygen != nullptr)
    data.pO2(std::unique_ptr<CDM::ScalarPressureData>(m_PartialPressureOxygen->Unload()));
  if (m_PartialPressureCarbonDioxide != nullptr)
    data.pCO2(std::unique_ptr<CDM::ScalarPressureData>(m_PartialPressureCarbonDioxide->Unload()));
  if (m_BaseExcess != nullptr)
    data.BaseExcess(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_BaseExcess->Unload()));
  if (m_StandardBicarbonate != nullptr)
    data.StandardBicarbonate(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_StandardBicarbonate->Unload()));
  if (m_OxygenSaturation != nullptr)
    data.OxygenSaturation(std::unique_ptr<CDM::ScalarData>(m_OxygenSaturation->Unload()));
}

bool SEArterialBloodGasAnalysis::HaspH()
{
  return m_pH == nullptr ? false : m_pH->IsValid();
}
SEScalar& SEArterialBloodGasAnalysis::GetpH()
{
  if (m_pH == nullptr)
    m_pH = new SEScalar();
  return *m_pH;
}

bool SEArterialBloodGasAnalysis::HasPartialPressureOxygen()
{
  return m_PartialPressureOxygen == nullptr ? false : m_PartialPressureOxygen->IsValid();
}
SEScalarPressure& SEArterialBloodGasAnalysis::GetPartialPressureOxygen()
{
  if (m_PartialPressureOxygen == nullptr)
    m_PartialPressureOxygen = new SEScalarPressure();
  return *m_PartialPressureOxygen;
}

bool SEArterialBloodGasAnalysis::HasPartialPressureCarbonDioxide()
{
  return m_PartialPressureCarbonDioxide == nullptr ? false : m_PartialPressureCarbonDioxide->IsValid();
}
SEScalarPressure& SEArterialBloodGasAnalysis::GetPartialPressureCarbonDioxide()
{
  if (m_PartialPressureCarbonDioxide == nullptr)
    m_PartialPressureCarbonDioxide = new SEScalarPressure();
  return *m_PartialPressureCarbonDioxide;
}

bool SEArterialBloodGasAnalysis::HasBaseExcess()
{
  return m_BaseExcess == nullptr ? false : m_BaseExcess->IsValid();
}
SEScalarAmountPerVolume& SEArterialBloodGasAnalysis::GetBaseExcess()
{
  if (m_BaseExcess == nullptr)
    m_BaseExcess = new SEScalarAmountPerVolume();
  return *m_BaseExcess;
}

bool SEArterialBloodGasAnalysis::HasStandardBicarbonate()
{
  return m_StandardBicarbonate == nullptr ? false : m_StandardBicarbonate->IsValid();
}
SEScalarAmountPerVolume& SEArterialBloodGasAnalysis::GetStandardBicarbonate()
{
  if (m_StandardBicarbonate == nullptr)
    m_StandardBicarbonate = new SEScalarAmountPerVolume();
  return *m_StandardBicarbonate;
}

bool SEArterialBloodGasAnalysis::HasOxygenSaturation()
{
  return m_OxygenSaturation == nullptr ? false : m_OxygenSaturation->IsValid();
}
SEScalar& SEArterialBloodGasAnalysis::GetOxygenSaturation()
{
  if (m_OxygenSaturation == nullptr)
    m_OxygenSaturation = new SEScalar();
  return *m_OxygenSaturation;
}
CLOSE_BIOGEARS_NAMESPACE