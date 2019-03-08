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


#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SESubstanceTissuePharmacokinetics::SESubstanceTissuePharmacokinetics(const std::string& name, Logger* logger)
  : Loggable(logger)
  , m_Name(name)
{
  m_PartitionCoefficient = nullptr;
}
//-----------------------------------------------------------------------------
SESubstanceTissuePharmacokinetics::~SESubstanceTissuePharmacokinetics()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESubstanceTissuePharmacokinetics::Clear()
{
  ;
  SAFE_DELETE(m_PartitionCoefficient)
}
//-----------------------------------------------------------------------------
bool SESubstanceTissuePharmacokinetics::Load(const CDM::SubstanceTissuePharmacokineticsData& in)
{
  Clear();
  if (in.PartitionCoefficient().present())
    GetPartitionCoefficient().Load(in.PartitionCoefficient().get());
  return true;
}
//-----------------------------------------------------------------------------
CDM::SubstanceTissuePharmacokineticsData* SESubstanceTissuePharmacokinetics::Unload() const
{
  CDM::SubstanceTissuePharmacokineticsData* data = new CDM::SubstanceTissuePharmacokineticsData();
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESubstanceTissuePharmacokinetics::Unload(CDM::SubstanceTissuePharmacokineticsData& data) const
{
  data.Name(m_Name);
  if (m_PartitionCoefficient != nullptr)
    data.PartitionCoefficient(std::unique_ptr<CDM::ScalarData>(m_PartitionCoefficient->Unload()));
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceTissuePharmacokinetics::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-----------------------------------------------------------------------------
const SEScalar* SESubstanceTissuePharmacokinetics::GetScalar(const std::string& name)
{
  if (name.compare("PartitionCoefficient") == 0)
    return &GetPartitionCoefficient();

  return nullptr;
}
//-----------------------------------------------------------------------------
std::string SESubstanceTissuePharmacokinetics::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SESubstanceTissuePharmacokinetics::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
bool SESubstanceTissuePharmacokinetics::HasPartitionCoefficient() const
{
  return (m_PartitionCoefficient == nullptr) ? false : m_PartitionCoefficient->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar& SESubstanceTissuePharmacokinetics::GetPartitionCoefficient()
{
  if (m_PartitionCoefficient == nullptr)
    m_PartitionCoefficient = new SEScalar();
  return *m_PartitionCoefficient;
}
//-----------------------------------------------------------------------------
double SESubstanceTissuePharmacokinetics::GetPartitionCoefficient() const
{
  if (m_PartitionCoefficient == nullptr)
    return SEScalar::dNaN();
  return m_PartitionCoefficient->GetValue();
}
//-----------------------------------------------------------------------------
}