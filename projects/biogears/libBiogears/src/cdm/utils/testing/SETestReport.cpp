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
#include <algorithm>
#include <fstream>
#include <functional>
#include <numeric>

#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/TestReport.hxx>

#include "io/cdm/TestReport.h"

namespace biogears {
SETestReport::SETestReport(Logger* logger)
  : Loggable(logger)
{
  assert(m_Logger != nullptr);
}
//-------------------------------------------------------------------------------
SETestReport::~SETestReport()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SETestReport::Clear()
{
  DELETE_VECTOR(m_testSuite);
}
//-------------------------------------------------------------------------------
void SETestReport::Reset()
{
}

//-------------------------------------------------------------------------------
bool SETestReport::WriteFile(const std::string& fileName)
{
  xml_schema::namespace_infomap map;
  map[""].name = "uri:/mil/tatrc/physiology/datamodel";
  map[""].schema = "BioGears.xsd";

  try {
    std::ofstream outFile;
    auto io = m_Logger->GetIoManager().lock();
    outFile.open(io->ResolveLogFileLocation(fileName));
    auto unloaded = std::make_unique<CDM::TestReportData>();
    io::TestReport::Marshall(*this, *unloaded);
    CDM::TestReport(outFile, *unloaded, map);
  } catch (const xml_schema::exception& e) {
    Error(e.what());
    return false;
  }
  return true;
}
//-------------------------------------------------------------------------------
SETestSuite& SETestReport::CreateTestSuite()
{
  SETestSuite* suite = new SETestSuite(GetLogger());
  m_testSuite.push_back(suite);
  return *suite;
}
//-------------------------------------------------------------------------------
const std::vector<SETestSuite*>& SETestReport::GetTestSuites() const
{
  return m_testSuite;
}
//-------------------------------------------------------------------------------
bool SETestReport::operator==(SETestReport const& rhs) const
{

  if (this == &rhs)
    return true;

  bool equivilant = m_testSuite.size() == rhs.m_testSuite.size();
  for (auto i = 0; equivilant && i < m_testSuite.size(); ++i) {
    equivilant &= (m_testSuite[i] && rhs.m_testSuite[i])
      ? m_testSuite[i]->operator==(*rhs.m_testSuite[i])
      : m_testSuite[i] == rhs.m_testSuite[i];
  }

  return equivilant;
}
//-------------------------------------------------------------------------------
bool SETestReport::operator!=(SETestReport const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}
