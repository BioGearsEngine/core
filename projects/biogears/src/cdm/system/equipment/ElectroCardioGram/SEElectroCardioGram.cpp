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
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>

#include <biogears/cdm/properties/SEScalarElectricPotential.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
SEElectroCardioGram::SEElectroCardioGram(Logger* logger)
  : SESystem(logger)
{
  m_Lead1ElectricPotential = nullptr;
  m_Lead2ElectricPotential = nullptr;
  m_Lead3ElectricPotential = nullptr;
  m_Lead4ElectricPotential = nullptr;
  m_Lead5ElectricPotential = nullptr;
  m_Lead6ElectricPotential = nullptr;
  m_Lead7ElectricPotential = nullptr;
  m_Lead8ElectricPotential = nullptr;
  m_Lead9ElectricPotential = nullptr;
  m_Lead10ElectricPotential = nullptr;
  m_Lead11ElectricPotential = nullptr;
  m_Lead12ElectricPotential = nullptr;
}
//-------------------------------------------------------------------------------

SEElectroCardioGram::~SEElectroCardioGram()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEElectroCardioGram::Clear()
{
  SAFE_DELETE(m_Lead1ElectricPotential);
  SAFE_DELETE(m_Lead2ElectricPotential);
  SAFE_DELETE(m_Lead3ElectricPotential);
  SAFE_DELETE(m_Lead4ElectricPotential);
  SAFE_DELETE(m_Lead5ElectricPotential);
  SAFE_DELETE(m_Lead6ElectricPotential);
  SAFE_DELETE(m_Lead7ElectricPotential);
  SAFE_DELETE(m_Lead8ElectricPotential);
  SAFE_DELETE(m_Lead9ElectricPotential);
  SAFE_DELETE(m_Lead10ElectricPotential);
  SAFE_DELETE(m_Lead11ElectricPotential);
  SAFE_DELETE(m_Lead12ElectricPotential);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::Load(const CDM::ElectroCardioGramData& in)
{
  if (in.Lead1ElectricPotential().present())
    GetLead1ElectricPotential().Load(in.Lead1ElectricPotential().get());
  if (in.Lead2ElectricPotential().present())
    GetLead2ElectricPotential().Load(in.Lead2ElectricPotential().get());
  if (in.Lead3ElectricPotential().present())
    GetLead3ElectricPotential().Load(in.Lead3ElectricPotential().get());
  if (in.Lead4ElectricPotential().present())
    GetLead4ElectricPotential().Load(in.Lead4ElectricPotential().get());
  if (in.Lead5ElectricPotential().present())
    GetLead5ElectricPotential().Load(in.Lead5ElectricPotential().get());
  if (in.Lead6ElectricPotential().present())
    GetLead6ElectricPotential().Load(in.Lead6ElectricPotential().get());
  if (in.Lead7ElectricPotential().present())
    GetLead7ElectricPotential().Load(in.Lead7ElectricPotential().get());
  if (in.Lead8ElectricPotential().present())
    GetLead8ElectricPotential().Load(in.Lead8ElectricPotential().get());
  if (in.Lead9ElectricPotential().present())
    GetLead9ElectricPotential().Load(in.Lead9ElectricPotential().get());
  if (in.Lead10ElectricPotential().present())
    GetLead10ElectricPotential().Load(in.Lead10ElectricPotential().get());
  if (in.Lead11ElectricPotential().present())
    GetLead11ElectricPotential().Load(in.Lead11ElectricPotential().get());
  if (in.Lead12ElectricPotential().present())
    GetLead12ElectricPotential().Load(in.Lead12ElectricPotential().get());
  return true;
}
//-------------------------------------------------------------------------------

CDM::ElectroCardioGramData* SEElectroCardioGram::Unload() const
{
  CDM::ElectroCardioGramData* data = new CDM::ElectroCardioGramData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEElectroCardioGram::Unload(CDM::ElectroCardioGramData& data) const
{
  if (m_Lead1ElectricPotential != nullptr)
    data.Lead1ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead1ElectricPotential->Unload()));
  if (m_Lead2ElectricPotential != nullptr)
    data.Lead2ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead2ElectricPotential->Unload()));
  if (m_Lead3ElectricPotential != nullptr)
    data.Lead3ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead3ElectricPotential->Unload()));
  if (m_Lead4ElectricPotential != nullptr)
    data.Lead4ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead4ElectricPotential->Unload()));
  if (m_Lead5ElectricPotential != nullptr)
    data.Lead5ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead5ElectricPotential->Unload()));
  if (m_Lead6ElectricPotential != nullptr)
    data.Lead6ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead6ElectricPotential->Unload()));
  if (m_Lead7ElectricPotential != nullptr)
    data.Lead7ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead7ElectricPotential->Unload()));
  if (m_Lead8ElectricPotential != nullptr)
    data.Lead8ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead8ElectricPotential->Unload()));
  if (m_Lead9ElectricPotential != nullptr)
    data.Lead9ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead9ElectricPotential->Unload()));
  if (m_Lead10ElectricPotential != nullptr)
    data.Lead10ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead10ElectricPotential->Unload()));
  if (m_Lead11ElectricPotential != nullptr)
    data.Lead11ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead11ElectricPotential->Unload()));
  if (m_Lead12ElectricPotential != nullptr)
    data.Lead12ElectricPotential(std::unique_ptr<CDM::ScalarElectricPotentialData>(m_Lead12ElectricPotential->Unload()));
}
//-------------------------------------------------------------------------------
const SEScalar* SEElectroCardioGram::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEElectroCardioGram::GetScalar(const std::string& name)
{
  if (name.compare("Lead1ElectricPotential") == 0)
    return &GetLead1ElectricPotential();
  if (name.compare("Lead2ElectricPotential") == 0)
    return &GetLead2ElectricPotential();
  if (name.compare("Lead3ElectricPotential") == 0)
    return &GetLead3ElectricPotential();
  if (name.compare("Lead4ElectricPotential") == 0)
    return &GetLead4ElectricPotential();
  if (name.compare("Lead5ElectricPotential") == 0)
    return &GetLead5ElectricPotential();
  if (name.compare("Lead6ElectricPotential") == 0)
    return &GetLead6ElectricPotential();
  if (name.compare("Lead7ElectricPotential") == 0)
    return &GetLead7ElectricPotential();
  if (name.compare("Lead8ElectricPotential") == 0)
    return &GetLead8ElectricPotential();
  if (name.compare("Lead9ElectricPotential") == 0)
    return &GetLead9ElectricPotential();
  if (name.compare("Lead10ElectricPotential") == 0)
    return &GetLead10ElectricPotential();
  if (name.compare("Lead11ElectricPotential") == 0)
    return &GetLead11ElectricPotential();
  if (name.compare("Lead12ElectricPotential") == 0)
    return &GetLead12ElectricPotential();
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead1ElectricPotential() const
{
  return m_Lead1ElectricPotential == nullptr ? false : m_Lead1ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead1ElectricPotential()
{
  if (m_Lead1ElectricPotential == nullptr)
    m_Lead1ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead1ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead1ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead1ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead2ElectricPotential() const
{
  return m_Lead2ElectricPotential == nullptr ? false : m_Lead2ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead2ElectricPotential()
{
  if (m_Lead2ElectricPotential == nullptr)
    m_Lead2ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead2ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead2ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead2ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead3ElectricPotential() const
{
  return m_Lead3ElectricPotential == nullptr ? false : m_Lead3ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead3ElectricPotential()
{
  if (m_Lead3ElectricPotential == nullptr)
    m_Lead3ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead3ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead3ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead3ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead4ElectricPotential() const
{
  return m_Lead4ElectricPotential == nullptr ? false : m_Lead4ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead4ElectricPotential()
{
  if (m_Lead4ElectricPotential == nullptr)
    m_Lead4ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead4ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead4ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead4ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead5ElectricPotential() const
{
  return m_Lead5ElectricPotential == nullptr ? false : m_Lead5ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead5ElectricPotential()
{
  if (m_Lead5ElectricPotential == nullptr)
    m_Lead5ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead5ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead5ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead5ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead6ElectricPotential() const
{
  return m_Lead6ElectricPotential == nullptr ? false : m_Lead6ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead6ElectricPotential()
{
  if (m_Lead6ElectricPotential == nullptr)
    m_Lead6ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead6ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead6ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead6ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead7ElectricPotential() const
{
  return m_Lead7ElectricPotential == nullptr ? false : m_Lead7ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead7ElectricPotential()
{
  if (m_Lead7ElectricPotential == nullptr)
    m_Lead7ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead7ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead7ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead7ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead8ElectricPotential() const
{
  return m_Lead8ElectricPotential == nullptr ? false : m_Lead8ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead8ElectricPotential()
{
  if (m_Lead8ElectricPotential == nullptr)
    m_Lead8ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead8ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead8ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead8ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead9ElectricPotential() const
{
  return m_Lead9ElectricPotential == nullptr ? false : m_Lead9ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead9ElectricPotential()
{
  if (m_Lead9ElectricPotential == nullptr)
    m_Lead9ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead9ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead9ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead9ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead10ElectricPotential() const
{
  return m_Lead10ElectricPotential == nullptr ? false : m_Lead10ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead10ElectricPotential()
{
  if (m_Lead10ElectricPotential == nullptr)
    m_Lead10ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead10ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead10ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead10ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead11ElectricPotential() const
{
  return m_Lead11ElectricPotential == nullptr ? false : m_Lead11ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead11ElectricPotential()
{
  if (m_Lead11ElectricPotential == nullptr)
    m_Lead11ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead11ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead11ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead11ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEElectroCardioGram::HasLead12ElectricPotential() const
{
  return m_Lead12ElectricPotential == nullptr ? false : m_Lead12ElectricPotential->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarElectricPotential& SEElectroCardioGram::GetLead12ElectricPotential()
{
  if (m_Lead12ElectricPotential == nullptr)
    m_Lead12ElectricPotential = new SEScalarElectricPotential();
  return *m_Lead12ElectricPotential;
}
//-------------------------------------------------------------------------------
double SEElectroCardioGram::GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const
{
  if (m_Lead12ElectricPotential == nullptr)
    return SEScalar::dNaN();
  return m_Lead12ElectricPotential->GetValue(unit);
}
//-------------------------------------------------------------------------------
Tree<const char*> SEElectroCardioGram::GetPhysiologyRequestGraph() const
{
  return {""};
}
}