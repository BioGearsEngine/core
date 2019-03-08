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
#include <biogears/cdm/properties/SEFunction.h>

#include <biogears/cdm/properties/SEScalar.h> //Utils

namespace biogears {
static std::stringstream err;

SEFunction::SEFunction()
  : SEProperty()
{
}

SEFunction::~SEFunction()
{
  Clear();
}

void SEFunction::Clear()
{
  m_Dependent.clear();
  m_Independent.clear();
}

bool SEFunction::IsValid() const
{
  if (m_Dependent.size() == 0 || m_Independent.size() == 0)
    return false;
  if (m_Dependent.size() != m_Independent.size())
    return false;
  return true;
}

void SEFunction::Invalidate()
{
  Clear();
}

bool SEFunction::Load(const CDM::FunctionData& in)
{
  Clear();
  for (unsigned int i = 0; i < in.Dependent().DoubleList().size(); i++)
    m_Dependent.push_back(in.Dependent().DoubleList()[i]);
  for (unsigned int i = 0; i < in.Independent().DoubleList().size(); i++)
    m_Independent.push_back(in.Independent().DoubleList()[i]);
  return IsValid();
}

CDM::FunctionData* SEFunction::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::FunctionData* data(new CDM::FunctionData());
  Unload(*data);
  return data;
}

void SEFunction::Unload(CDM::FunctionData& data) const
{
  data.Dependent(std::unique_ptr<CDM::DoubleArray>(new CDM::DoubleArray()));
  data.Dependent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  data.Independent(std::unique_ptr<CDM::DoubleArray>(new CDM::DoubleArray()));
  data.Independent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  for (unsigned int i = 0; i < m_Dependent.size(); i++) {
    data.Dependent().DoubleList().push_back(m_Dependent[i]);
    data.Independent().DoubleList().push_back(m_Independent[i]);
  }
}

unsigned int SEFunction::Length()
{
  if (IsValid())
    return static_cast<unsigned int>(m_Independent.size());
  return 0;
}

double SEFunction::GetDependentValue(unsigned int index)
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}

std::vector<double>& SEFunction::GetDependent()
{
  return m_Dependent;
}

double SEFunction::GetIndependentValue(unsigned int index)
{
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return m_Independent[index];
}

std::vector<double>& SEFunction::GetIndependent()
{
  return m_Independent;
}
}