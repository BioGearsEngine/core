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

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/patient/actions/SESepsisState.h>

SESepsisState::SESepsisState()
  : m_Pathogen(nullptr)
  , m_Neutrophil(nullptr)
  , m_TissueDamage(nullptr)
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
  SAFE_DELETE(m_Neutrophil);
  SAFE_DELETE(m_TissueDamage);
  SAFE_DELETE(m_Antiinflammation);
}

bool SESepsisState::Load(const CDM::SepsisStateData& in)
{
  GetPathogen().Load(in.Pathogen());
  GetNeutrophil().Load(in.Neutrophil());
  GetTissueDamage().Load(in.TissueDamage());
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
  data.Neutrophil(std::unique_ptr<CDM::ScalarData>(m_Neutrophil->Unload()));
  data.TissueDamage(std::unique_ptr<CDM::ScalarData>(m_TissueDamage->Unload()));
  data.Antiinflammation(std::unique_ptr<CDM::ScalarData>(m_Antiinflammation->Unload()));
}

bool SESepsisState::IsValid()
{
  if (HasPathogen() && HasNeutrophil() && HasTissueDamage() && HasAntiinflammation())
    return true;
  else
    return false;
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

bool SESepsisState::HasNeutrophil() const
{
  return m_Neutrophil == nullptr ? false : m_Neutrophil->IsValid();
}
SEScalar& SESepsisState::GetNeutrophil()
{
  if (m_Neutrophil == nullptr)
    m_Neutrophil = new SEScalar();
  return *m_Neutrophil;
}
double SESepsisState::GetNeutrophil() const
{
  if (m_Neutrophil == nullptr)
    return SEScalar::dNaN();
  return m_Neutrophil->GetValue();
}

bool SESepsisState::HasTissueDamage() const
{
  return m_TissueDamage == nullptr ? false : m_TissueDamage->IsValid();
}
SEScalar& SESepsisState::GetTissueDamage()
{
  if (m_TissueDamage == nullptr)
    m_TissueDamage = new SEScalar();
  return *m_TissueDamage;
}
double SESepsisState::GetTissueDamage() const
{
  if (m_TissueDamage == nullptr)
    return SEScalar::dNaN();
  return m_TissueDamage->GetValue();
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