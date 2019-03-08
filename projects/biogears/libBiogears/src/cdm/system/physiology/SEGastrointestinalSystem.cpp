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
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/container/Tree.tci.h>

namespace biogears {
  constexpr char idChymeAbsorptionRate[] = "ChymeAbsorptionRate";
  constexpr char idStomachContents[] = "StomachContents";

SEGastrointestinalSystem::SEGastrointestinalSystem(Logger* logger)
  : SESystem(logger)
{
  m_ChymeAbsorptionRate = nullptr;
  m_StomachContents = nullptr;
}
//-------------------------------------------------------------------------------

SEGastrointestinalSystem::~SEGastrointestinalSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEGastrointestinalSystem::Clear()
{
  SESystem::Clear();
  SAFE_DELETE(m_ChymeAbsorptionRate);
  SAFE_DELETE(m_StomachContents);
}
//-------------------------------------------------------------------------------
const SEScalar* SEGastrointestinalSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEGastrointestinalSystem::GetScalar(const std::string& name)
{
  if (name == idChymeAbsorptionRate)
    return &GetChymeAbsorptionRate();

  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == idStomachContents)
      return GetStomachContents().GetScalar(prop);
  }
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::Load(const CDM::GastrointestinalSystemData& in)
{
  SESystem::Load(in);
  if (in.ChymeAbsorptionRate().present())
    GetChymeAbsorptionRate().Load(in.ChymeAbsorptionRate().get());
  if (in.StomachContents().present())
    GetStomachContents().Load(in.StomachContents().get());
  return true;
}
//-------------------------------------------------------------------------------

CDM::GastrointestinalSystemData* SEGastrointestinalSystem::Unload() const
{
  CDM::GastrointestinalSystemData* data = new CDM::GastrointestinalSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEGastrointestinalSystem::Unload(CDM::GastrointestinalSystemData& data) const
{
  SESystem::Unload(data);
  if (m_ChymeAbsorptionRate != nullptr)
    data.ChymeAbsorptionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_ChymeAbsorptionRate->Unload()));
  if (m_StomachContents != nullptr)
    data.StomachContents(std::unique_ptr<CDM::NutritionData>(m_StomachContents->Unload()));
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::HasChymeAbsorptionRate() const
{
  return m_ChymeAbsorptionRate == nullptr ? false : m_ChymeAbsorptionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SEGastrointestinalSystem::GetChymeAbsorptionRate()
{
  if (m_ChymeAbsorptionRate == nullptr)
    m_ChymeAbsorptionRate = new SEScalarVolumePerTime();
  return *m_ChymeAbsorptionRate;
}
//-------------------------------------------------------------------------------
double SEGastrointestinalSystem::GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_ChymeAbsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_ChymeAbsorptionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::HasStomachContents() const
{
  return m_StomachContents == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SENutrition& SEGastrointestinalSystem::GetStomachContents()
{
  if (m_StomachContents == nullptr)
    m_StomachContents = new SENutrition(GetLogger());
  return *m_StomachContents;
}
//-------------------------------------------------------------------------------
const SENutrition* SEGastrointestinalSystem::GetStomachContents() const
{
  return m_StomachContents;
}
//-------------------------------------------------------------------------------
void SEGastrointestinalSystem::RemoveStomachContents()
{
  SAFE_DELETE(m_StomachContents);
}
//-------------------------------------------------------------------------------
Tree<const char*> SEGastrointestinalSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*> {classname()}
    .emplace_back(idChymeAbsorptionRate)
    .emplace_back(idStomachContents)
  ;
}
//-------------------------------------------------------------------------------
}