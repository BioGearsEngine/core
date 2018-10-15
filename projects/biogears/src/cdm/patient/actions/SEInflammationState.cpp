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

#include "biogears/cdm/patient/actions/SEInflammationState.h"

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>


namespace biogears {

SEInflammationState::SEInflammationState()
  : m_PathogenTissue(nullptr)
  , m_PathogenBlood(nullptr)
  , m_MacrophageRestingTissue(nullptr)
  , m_MacrophageActiveTissue(nullptr)
  , m_NeutrophilRestingBlood(nullptr)
  , m_NeutrophilActiveBlood(nullptr)
  , m_NeutrophilActiveTissue(nullptr)
  , m_TNFBlood(nullptr)
  , m_TNFTissue(nullptr)
  , m_IL10Blood(nullptr)
  , m_IL10Tissue(nullptr)
  , m_TissueIntegrity(nullptr)
  , m_Inflammation(nullptr)
{
}

SEInflammationState::~SEInflammationState()
{
  Clear();
}

void SEInflammationState::Clear()
{
  SAFE_DELETE(m_PathogenTissue);
  SAFE_DELETE(m_PathogenTissue);
  SAFE_DELETE(m_MacrophageActiveTissue);
  SAFE_DELETE(m_MacrophageRestingTissue);
  SAFE_DELETE(m_NeutrophilRestingBlood);
  SAFE_DELETE(m_NeutrophilActiveBlood);
  SAFE_DELETE(m_NeutrophilActiveTissue);
  SAFE_DELETE(m_TNFBlood);
  SAFE_DELETE(m_TNFTissue);
  SAFE_DELETE(m_IL10Blood);
  SAFE_DELETE(m_IL10Tissue);
  SAFE_DELETE(m_TissueIntegrity);
  SAFE_DELETE(m_Inflammation);
}

bool SEInflammationState::Load(const CDM::InflammationStateData& in)
{
  GetPathogenBlood().Load(in.PathogenBlood());
  GetPathogenTissue().Load(in.PathogenTissue());
  GetMacrophageRestingTissue().Load(in.MacrophageRestingTissue());
  GetNeutrophilRestingBlood().Load(in.NeutrophilRestingBlood());
  GetNeutrophilActiveBlood().Load(in.NeutrophilActiveBlood());
  GetNeutrophilActiveTissue().Load(in.NeutrophilActiveTissue());
  GetTNFBlood().Load(in.TNFBlood());
  GetTNFTissue().Load(in.TNFTissue());
  GetIL10Blood().Load(in.IL10Blood());
  GetIL10Tissue().Load(in.IL10Tissue());
  GetTissueIntegrity().Load(in.TissueIntegrity());
  GetInflammation().Load(in.Inflammation());
  return true;
}

CDM::InflammationStateData* SEInflammationState::Unload() const
{
  CDM::InflammationStateData* data = new CDM::InflammationStateData();
  Unload(*data);
  return data;
}

void SEInflammationState::Unload(CDM::InflammationStateData& data) const
{
  data.PathogenBlood(std::unique_ptr<CDM::ScalarData>(m_PathogenBlood->Unload()));
  data.PathogenTissue(std::unique_ptr<CDM::ScalarData>(m_PathogenTissue->Unload()));
  data.MacrophageRestingTissue(std::unique_ptr<CDM::ScalarData>(m_MacrophageRestingTissue->Unload()));
  data.MacrophageActiveTissue(std::unique_ptr<CDM::ScalarData>(m_MacrophageActiveTissue->Unload()));
  data.NeutrophilRestingBlood(std::unique_ptr<CDM::ScalarData>(m_NeutrophilRestingBlood->Unload()));
  data.NeutrophilActiveBlood(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActiveBlood->Unload()));
  data.NeutrophilActiveTissue(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActiveTissue->Unload()));
  data.TNFBlood(std::unique_ptr<CDM::ScalarData>(m_TNFBlood->Unload()));
  data.TNFTissue(std::unique_ptr<CDM::ScalarData>(m_TNFTissue->Unload()));
  data.IL10Blood(std::unique_ptr<CDM::ScalarData>(m_IL10Blood->Unload()));
  data.IL10Tissue(std::unique_ptr<CDM::ScalarData>(m_IL10Tissue->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  data.Inflammation(std::unique_ptr<CDM::Scalar0To1Data>(m_Inflammation->Unload()));
}

bool SEInflammationState::IsValid()
{
  if (HasPathogenBlood() && HasPathogenTissue() && HasMacrophageRestingTissue() && HasMacrophageActiveTissue() && HasNeutrophilRestingBlood() && HasNeutrophilActiveBlood() && HasNeutrophilActiveTissue() && HasTNFBlood() && HasTNFTissue() && HasIL10Blood() && HasIL10Tissue() && HasTissueIntegrity() && HasInflammation())
    return true;
  else
    return false;
}

const SEScalar* SEInflammationState::GetScalar(const std::string& name)
{
  if (name.compare("PathogenBlood") == 0)
    return &GetPathogenBlood();
  if (name.compare("PathogenTissue") == 0)
    return &GetPathogenTissue();
  if (name.compare("MacrophageRestingTissue") == 0)
    return &GetMacrophageRestingTissue();
  if (name.compare("MacrophageActiveTissue") == 0)
    return &GetMacrophageActiveTissue();
  if (name.compare("NeutrophilRestingBlood") == 0)
    return &GetNeutrophilRestingBlood();
  if (name.compare("NeutrophilActiveBlood") == 0)
    return &GetNeutrophilActiveBlood();
  if (name.compare("NeutrophilActiveTissue") == 0)
    return &GetNeutrophilActiveTissue();
  if (name.compare("TNFBlood") == 0)
    return &GetTNFBlood();
  if (name.compare("TNFTissue") == 0)
    return &GetTNFTissue();
  if (name.compare("IL10Blood") == 0)
    return &GetIL10Blood();
  if (name.compare("IL10Tissue") == 0)
    return &GetIL10Tissue();
  if (name.compare("TissueIntegrity") == 0)
    return &GetTissueIntegrity();
  if (name.compare("Inflammation") == 0)
    return &GetInflammation();
  return nullptr;
}

bool SEInflammationState::HasPathogenBlood() const
{
  return m_PathogenBlood == nullptr ? false : m_PathogenBlood->IsValid();
}
SEScalar& SEInflammationState::GetPathogenBlood()
{
  if (m_PathogenBlood == nullptr)
    m_PathogenBlood = new SEScalar();
  return *m_PathogenBlood;
}
double SEInflammationState::GetPathogenBlood() const
{
  if (m_PathogenBlood == nullptr)
    return SEScalar::dNaN();
  return m_PathogenBlood->GetValue();
}

bool SEInflammationState::HasPathogenTissue() const
{
  return m_PathogenTissue == nullptr ? false : m_PathogenTissue->IsValid();
}
SEScalar& SEInflammationState::GetPathogenTissue()
{
  if (m_PathogenTissue == nullptr)
    m_PathogenTissue = new SEScalar();
  return *m_PathogenTissue;
}
double SEInflammationState::GetPathogenTissue() const
{
  if (m_PathogenTissue == nullptr)
    return SEScalar::dNaN();
  return m_PathogenTissue->GetValue();
}

bool SEInflammationState::HasMacrophageRestingTissue() const
{
  return m_MacrophageRestingTissue == nullptr ? false : m_MacrophageRestingTissue->IsValid();
}
SEScalar& SEInflammationState::GetMacrophageRestingTissue()
{
  if (m_MacrophageRestingTissue == nullptr)
    m_MacrophageRestingTissue = new SEScalar();
  return *m_MacrophageRestingTissue;
}
double SEInflammationState::GetMacrophageRestingTissue() const
{
  if (m_MacrophageRestingTissue == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageRestingTissue->GetValue();
}

bool SEInflammationState::HasMacrophageActiveTissue() const
{
  return m_MacrophageActiveTissue == nullptr ? false : m_MacrophageActiveTissue->IsValid();
}
SEScalar& SEInflammationState::GetMacrophageActiveTissue()
{
  if (m_MacrophageActiveTissue == nullptr)
    m_MacrophageActiveTissue = new SEScalar();
  return *m_MacrophageActiveTissue;
}
double SEInflammationState::GetMacrophageActiveTissue() const
{
  if (m_MacrophageActiveTissue == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageActiveTissue->GetValue();
}

bool SEInflammationState::HasNeutrophilRestingBlood() const
{
  return m_NeutrophilRestingBlood == nullptr ? false : m_NeutrophilRestingBlood->IsValid();
}
SEScalar& SEInflammationState::GetNeutrophilRestingBlood()
{
  if (m_NeutrophilRestingBlood == nullptr)
    m_NeutrophilRestingBlood = new SEScalar();
  return *m_NeutrophilRestingBlood;
}
double SEInflammationState::GetNeutrophilRestingBlood() const
{
  if (m_NeutrophilRestingBlood == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilRestingBlood->GetValue();
}

bool SEInflammationState::HasNeutrophilActiveBlood() const
{
  return m_NeutrophilActiveBlood == nullptr ? false : m_NeutrophilActiveBlood->IsValid();
}
SEScalar& SEInflammationState::GetNeutrophilActiveBlood()
{
  if (m_NeutrophilActiveBlood == nullptr)
    m_NeutrophilActiveBlood = new SEScalar();
  return *m_NeutrophilActiveBlood;
}
double SEInflammationState::GetNeutrophilActiveBlood() const
{
  if (m_NeutrophilActiveBlood == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActiveBlood->GetValue();
}

bool SEInflammationState::HasNeutrophilActiveTissue() const
{
  return m_NeutrophilActiveTissue == nullptr ? false : m_NeutrophilActiveTissue->IsValid();
}
SEScalar& SEInflammationState::GetNeutrophilActiveTissue()
{
  if (m_NeutrophilActiveTissue == nullptr)
    m_NeutrophilActiveTissue = new SEScalar();
  return *m_NeutrophilActiveTissue;
}
double SEInflammationState::GetNeutrophilActiveTissue() const
{
  if (m_NeutrophilActiveTissue == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActiveTissue->GetValue();
}

bool SEInflammationState::HasTNFBlood() const
{
  return m_TNFBlood == nullptr ? false : m_TNFBlood->IsValid();
}
SEScalar& SEInflammationState::GetTNFBlood()
{
  if (m_TNFBlood == nullptr)
    m_TNFBlood = new SEScalar();
  return *m_TNFBlood;
}
double SEInflammationState::GetTNFBlood() const
{
  if (m_TNFBlood == nullptr)
    return SEScalar::dNaN();
  return m_TNFBlood->GetValue();
}

bool SEInflammationState::HasTNFTissue() const
{
  return m_TNFTissue == nullptr ? false : m_TNFTissue->IsValid();
}
SEScalar& SEInflammationState::GetTNFTissue()
{
  if (m_TNFTissue == nullptr)
    m_TNFTissue = new SEScalar();
  return *m_TNFTissue;
}
double SEInflammationState::GetTNFTissue() const
{
  if (m_TNFTissue == nullptr)
    return SEScalar::dNaN();
  return m_TNFTissue->GetValue();
}

bool SEInflammationState::HasIL10Blood() const
{
  return m_IL10Blood == nullptr ? false : m_IL10Blood->IsValid();
}
SEScalar& SEInflammationState::GetIL10Blood()
{
  if (m_IL10Blood == nullptr)
    m_IL10Blood = new SEScalar();
  return *m_IL10Blood;
}
double SEInflammationState::GetIL10Blood() const
{
  if (m_IL10Blood == nullptr)
    return SEScalar::dNaN();
  return m_IL10Blood->GetValue();
}

bool SEInflammationState::HasIL10Tissue() const
{
  return m_IL10Tissue == nullptr ? false : m_IL10Tissue->IsValid();
}
SEScalar& SEInflammationState::GetIL10Tissue()
{
  if (m_IL10Tissue == nullptr)
    m_IL10Tissue = new SEScalar();
  return *m_IL10Tissue;
}
double SEInflammationState::GetIL10Tissue() const
{
  if (m_IL10Tissue == nullptr)
    return SEScalar::dNaN();
  return m_IL10Tissue->GetValue();
}

bool SEInflammationState::HasTissueIntegrity() const
{
  return m_TissueIntegrity == nullptr ? false : m_TissueIntegrity->IsValid();
}
SEScalar0To1& SEInflammationState::GetTissueIntegrity()
{
  if (m_TissueIntegrity == nullptr)
    m_TissueIntegrity = new SEScalar0To1();
  return *m_TissueIntegrity;
}
double SEInflammationState::GetTissueIntegrity() const
{
  if (m_TissueIntegrity == nullptr)
    return SEScalar::dNaN();
  return m_TissueIntegrity->GetValue();
}

bool SEInflammationState::HasInflammation() const
{
  return m_Inflammation == nullptr ? false : m_Inflammation->IsValid();
}
SEScalar0To1& SEInflammationState::GetInflammation()
{
  if (m_Inflammation == nullptr)
    m_Inflammation = new SEScalar0To1();
  return *m_Inflammation;
}
double SEInflammationState::GetInflammation() const
{
  if (m_Inflammation == nullptr)
    return SEScalar::dNaN();
  return m_Inflammation->GetValue();
}
}
