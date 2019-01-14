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
#include <biogears/cdm/system/physiology/SEHepaticSystem.h>

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
constexpr char idKetoneProductionRate[] = "KetoneProductionRate";
constexpr char idHepaticGluconeogenesisRate[] = "HepaticGluconeogenesisRate";

SEHepaticSystem::SEHepaticSystem(Logger* logger)
  : SESystem(logger)
{
  m_KetoneProductionRate = nullptr;
  m_HepaticGluconeogenesisRate = nullptr;
}
//-------------------------------------------------------------------------------

SEHepaticSystem::~SEHepaticSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEHepaticSystem::Clear()
{
  SESystem::Clear();

  SAFE_DELETE(m_KetoneProductionRate);
  SAFE_DELETE(m_HepaticGluconeogenesisRate);
}
//-------------------------------------------------------------------------------
const SEScalar* SEHepaticSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEHepaticSystem::GetScalar(const std::string& name)
{
  if (name == idKetoneProductionRate)
    return &GetKetoneProductionRate();
  if (name == idHepaticGluconeogenesisRate)
    return &GetHepaticGluconeogenesisRate();
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEHepaticSystem::Load(const CDM::HepaticSystemData& in)
{
  SESystem::Load(in);

  if (in.KetoneProductionRate().present())
    GetKetoneProductionRate().Load(in.KetoneProductionRate().get());
  if (in.HepaticGluconeogenesisRate().present())
    GetHepaticGluconeogenesisRate().Load(in.HepaticGluconeogenesisRate().get());

  return true;
}
//-------------------------------------------------------------------------------
CDM::HepaticSystemData* SEHepaticSystem::Unload() const
{
  CDM::HepaticSystemData* data = new CDM::HepaticSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEHepaticSystem::Unload(CDM::HepaticSystemData& data) const
{
  SESystem::Unload(data);

  if (m_KetoneProductionRate != nullptr)
    data.KetoneProductionRate(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_KetoneProductionRate->Unload()));
  if (m_HepaticGluconeogenesisRate != nullptr)
    data.HepaticGluconeogenesisRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_HepaticGluconeogenesisRate->Unload()));
}
//-------------------------------------------------------------------------------

bool SEHepaticSystem::HasKetoneProductionRate() const
{
  return m_KetoneProductionRate == nullptr ? false : m_KetoneProductionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime& SEHepaticSystem::GetKetoneProductionRate()
{
  if (m_KetoneProductionRate == nullptr)
    m_KetoneProductionRate = new SEScalarAmountPerTime();
  return *m_KetoneProductionRate;
}
//-------------------------------------------------------------------------------
double SEHepaticSystem::GetKetoneProductionRate(const AmountPerTimeUnit& unit) const
{
  if (m_KetoneProductionRate == nullptr)
    return SEScalar::dNaN();
  return m_KetoneProductionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEHepaticSystem::HasHepaticGluconeogenesisRate() const
{
  return m_HepaticGluconeogenesisRate == nullptr ? false : m_HepaticGluconeogenesisRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerTime& SEHepaticSystem::GetHepaticGluconeogenesisRate()
{
  if (m_HepaticGluconeogenesisRate == nullptr)
    m_HepaticGluconeogenesisRate = new SEScalarMassPerTime();
  return *m_HepaticGluconeogenesisRate;
}
//-------------------------------------------------------------------------------
double SEHepaticSystem::GetHepaticGluconeogenesisRate(const MassPerTimeUnit& unit) const
{
  if (m_HepaticGluconeogenesisRate == nullptr)
    return SEScalar::dNaN();
  return m_HepaticGluconeogenesisRate->GetValue(unit);
}
//-------------------------------------------------------------------------------
Tree<const char*> SEHepaticSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{ classname() }
    .emplace_back(idKetoneProductionRate)
    .emplace_back(idHepaticGluconeogenesisRate);
}
}