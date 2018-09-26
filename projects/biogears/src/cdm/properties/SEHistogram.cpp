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
#include <biogears/cdm/properties/SEHistogram.h>

#include <biogears/cdm/properties/SEScalar.h> //Utils

namespace biogears {
static std::stringstream err;

SEHistogram::SEHistogram()
  : SEProperty()
{
}

SEHistogram::~SEHistogram()
{
  Clear();
}

void SEHistogram::Clear()
{
  m_Dependent.clear();
  m_Independent.clear();
}

bool SEHistogram::IsValid() const
{
  if (m_Dependent.size() == 0 || m_Independent.size() == 0) {
    std::cerr << "Histogram: No data provided." << std::endl; //todo make properties have a logger
    return false;
  }
  if (m_Dependent.size() + 1 != m_Independent.size()) {
    std::cerr << "Histogram: Size mismatch between boundaries and bins." << std::endl; //todo make properties have a logger
    return false;
  }
  return true;
}

void SEHistogram::Invalidate()
{
  Clear();
}

bool SEHistogram::Load(const CDM::HistogramData& in)
{
  Clear();
  for (unsigned int i = 0; i < in.Dependent().DoubleList().size(); i++)
    m_Dependent.push_back(in.Dependent().DoubleList()[i]);
  for (unsigned int i = 0; i < in.Independent().DoubleList().size(); i++)
    m_Independent.push_back(in.Independent().DoubleList()[i]);
  return IsValid();
}

CDM::HistogramData* SEHistogram::Unload() const
{
  if (!IsValid())
    return nullptr;
  CDM::HistogramData* data(new CDM::HistogramData());
  Unload(*data);
  return data;
}

void SEHistogram::Unload(CDM::HistogramData& data) const
{
  data.Dependent(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  data.Dependent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  data.Independent(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  data.Independent().DoubleList(std::unique_ptr<CDM::DoubleList>(new CDM::DoubleList()));
  for (unsigned int i = 0; i < m_Dependent.size(); i++)
    data.Dependent().DoubleList().push_back(m_Dependent[i]);
  for (unsigned int i = 0; i < m_Independent.size(); i++)
    data.Independent().DoubleList().push_back(m_Independent[i]);
}

unsigned int SEHistogram::NumberOfBins() const
{
  if (IsValid())
    return static_cast<unsigned int>(m_Dependent.size());
  return 0;
}

unsigned int SEHistogram::NumberOfBoundaries() const
{
  if (IsValid())
    return static_cast<unsigned int>(m_Independent.size());
  return 0;
}

double SEHistogram::GetDependentValue(unsigned int index) const
{
  if (index >= m_Dependent.size())
    throw CommonDataModelException("Dependent index out of bounds");
  return m_Dependent[index];
}

std::vector<double>& SEHistogram::GetDependent()
{
  return m_Dependent;
}
const std::vector<double>& SEHistogram::GetDependent() const
{
  return m_Dependent;
}

double SEHistogram::GetIndependentValue(unsigned int index) const
{
  if (index >= m_Independent.size())
    throw CommonDataModelException("Independent index out of bounds");
  return m_Independent[index];
}

std::vector<double>& SEHistogram::GetIndependent()
{
  return m_Independent;
}
const std::vector<double>& SEHistogram::GetIndependent() const
{
  return m_Independent;
}
}