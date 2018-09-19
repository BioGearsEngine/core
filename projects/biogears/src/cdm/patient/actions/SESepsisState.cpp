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

#include <biogears/cdm/patient/actions/SESepsisState.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>

SESepsisState::SESepsisState()
  : m_Pathogen(nullptr)
  , m_Macrophage(nullptr)
  , m_NeutrophilResting(nullptr)
  , m_NeutrophilActive(nullptr)
  , m_TissueIntegrity(nullptr)
  , m_Antiinflammation(nullptr)
{
}

SESepsisState::~SESepsisState()
{
  Clear();
}

void SESepsisState::Clear()
{
  SAFE_DELETE(m_Pathogen);
  SAFE_DELETE(m_Macrophage);
  SAFE_DELETE(m_NeutrophilResting);
  SAFE_DELETE(m_NeutrophilActive);
  SAFE_DELETE(m_TissueIntegrity);
  SAFE_DELETE(m_Antiinflammation);
}

bool SESepsisState::Load(const CDM::SepsisStateData& in)
{
  GetPathogen().Load(in.Pathogen());
  GetMacrophage().Load(in.Macrophage());
  GetNeutrophilResting().Load(in.NeutrophilResting());
  GetNeutrophilActive().Load(in.NeutrophilActive());
  GetTissueIntegrity().Load(in.TissueIntegrity());
  GetAntiinflammation().Load(in.Antiinflammation());
  return true;
}

CDM::SepsisStateData* SESepsisState::Unload() const
{
  CDM::SepsisStateData* data = new CDM::SepsisStateData();
  Unload(*data);
  return data;
}

void SESepsisState::Unload(CDM::SepsisStateData& data) const
{
  data.Pathogen(std::unique_ptr<CDM::ScalarData>(m_Pathogen->Unload()));
  data.Macrophage(std::unique_ptr<CDM::ScalarData>(m_Macrophage->Unload()));
  data.NeutrophilResting(std::unique_ptr<CDM::ScalarData>(m_NeutrophilResting->Unload()));
  data.NeutrophilActive(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActive->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  data.Antiinflammation(std::unique_ptr<CDM::ScalarData>(m_Antiinflammation->Unload()));
}

bool SESepsisState::IsValid()
{
  if (HasPathogen() && HasMacrophage() && HasNeutrophilResting() && HasNeutrophilActive() && HasTissueIntegrity() && HasAntiinflammation())
    return true;
  else
    return false;
}

const SEScalar* SESepsisState::GetScalar(const std::string& name)
{
  if (name.compare("Pathogen") == 0)
    return &GetPathogen();
  if (name.compare("Macrophage") == 0)
    return &GetMacrophage();
  if (name.compare("NeutrophilResting") == 0)
    return &GetNeutrophilResting();
  if (name.compare("NeutrophilActive") == 0)
    return &GetNeutrophilActive();
  if (name.compare("TissueIntegrity") == 0)
    return &GetTissueIntegrity();
  if (name.compare("Antiinflammation") == 0)
    return &GetAntiinflammation();
  return nullptr;
}

bool SESepsisState::HasPathogen() const
{
  return m_Pathogen == nullptr ? false : m_Pathogen->IsValid();
}
SEScalar& SESepsisState::GetPathogen()
{
  if (m_Pathogen == nullptr)
    m_Pathogen = new SEScalar();
  return *m_Pathogen;
}
double SESepsisState::GetPathogen() const
{
  if (m_Pathogen == nullptr)
    return SEScalar::dNaN();
  return m_Pathogen->GetValue();
}

bool SESepsisState::HasMacrophage() const
{
  return m_Macrophage == nullptr ? false : m_Macrophage->IsValid();
}
SEScalar& SESepsisState::GetMacrophage()
{
  if (m_Macrophage == nullptr)
    m_Macrophage = new SEScalar();
  return *m_Macrophage;
}
double SESepsisState::GetMacrophage() const
{
  if (m_Macrophage == nullptr)
    return SEScalar::dNaN();
  return m_Macrophage->GetValue();
}

bool SESepsisState::HasNeutrophilResting() const
{
  return m_NeutrophilResting == nullptr ? false : m_NeutrophilResting->IsValid();
}
SEScalar& SESepsisState::GetNeutrophilResting()
{
  if (m_NeutrophilResting == nullptr)
    m_NeutrophilResting = new SEScalar();
  return *m_NeutrophilResting;
}
double SESepsisState::GetNeutrophilResting() const
{
  if (m_NeutrophilResting == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilResting->GetValue();
}

bool SESepsisState::HasNeutrophilActive() const
{
  return m_NeutrophilActive == nullptr ? false : m_NeutrophilActive->IsValid();
}
SEScalar& SESepsisState::GetNeutrophilActive()
{
  if (m_NeutrophilActive == nullptr)
    m_NeutrophilActive = new SEScalar();
  return *m_NeutrophilActive;
}
double SESepsisState::GetNeutrophilActive() const
{
  if (m_NeutrophilActive == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActive->GetValue();
}

bool SESepsisState::HasTissueIntegrity() const
{
  return m_TissueIntegrity == nullptr ? false : m_TissueIntegrity->IsValid();
}
SEScalar0To1& SESepsisState::GetTissueIntegrity()
{
  if (m_TissueIntegrity == nullptr)
    m_TissueIntegrity = new SEScalar0To1();
  return *m_TissueIntegrity;
}
double SESepsisState::GetTissueIntegrity() const
{
  if (m_TissueIntegrity == nullptr)
    return SEScalar::dNaN();
  return m_TissueIntegrity->GetValue();
}

bool SESepsisState::HasAntiinflammation() const
{
  return m_Antiinflammation == nullptr ? false : m_Antiinflammation->IsValid();
}
SEScalar& SESepsisState::GetAntiinflammation()
{
  if (m_Antiinflammation == nullptr)
    m_Antiinflammation = new SEScalar();
  return *m_Antiinflammation;
}
double SESepsisState::GetAntiinflammation() const
{
  if (m_Antiinflammation == nullptr)
    return SEScalar::dNaN();
  return m_Antiinflammation->GetValue();
}