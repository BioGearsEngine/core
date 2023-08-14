/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#pragma once
#include <memory>

#include "biogears/cdm/CommonDataModel.h"
#include <biogears/exports.h>

#include <biogears/schema/cdm/TestReport.hxx>

#define CDM_TEST_REPORT_UNMARSHAL_HELPER(in, out, func)                              \
  if (in.m_##func) {                                                                 \
    out.func(std::make_unique<std::remove_reference<decltype(out.func())>::type>()); \
    io::TestReport::UnMarshall(*in.m_##func, out.func());                            \
  }

#define CDM_OPTIONAL_TEST_REPORT_UNMARSHAL_HELPER(in, out, func) \
  if (in.m_##func) {                                             \
    io::TestReport::UnMarshall(*in.m_##func, out.func());        \
  }

namespace biogears {
class SETestErrorStatistics;
class SETestReport;
class SETestSuite;
class SETestCase;

namespace io {
  class BIOGEARS_PRIVATE_API TestReport {
  public:
    //template <typename SE, typename XSD>  option
    template <typename SE, typename XSD>
    static void Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out);
    template <typename SE, typename XSD>
    static void UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out);
    //class SETestErrorStatistics
    static void Marshall(const CDM::TestErrorStatisticsData& in, SETestErrorStatistics& out);
    static void UnMarshall(const SETestErrorStatistics& in, CDM::TestErrorStatisticsData& out);
    //class SETestReport
    static void Marshall(const CDM::TestReportData& in, SETestReport& out);
    static void UnMarshall(const SETestReport& in, CDM::TestReportData& out);
    //class SETestSuite
    static void Marshall(const CDM::TestSuite& in, SETestSuite& out);
    static void UnMarshall(const SETestSuite& in, CDM::TestSuite& out);
    //class SETestCase
    static void Marshall(const CDM::TestCase& in, SETestCase& out);
    static void UnMarshall(const SETestCase& in, CDM::TestCase& out);
  };
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void TestReport::Marshall(xsd::cxx::tree::optional<XSD> const& option_in, SE& out)
  {
    if (!option_in.present()) {
      out.Invalidate();
    } else {
      Marshall(option_in.get(), out);
    }
  }
  //----------------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void TestReport::UnMarshall(const SE& in, xsd::cxx::tree::optional<XSD>& option_out)
  {
    auto item = std::make_unique<XSD>();
    UnMarshall(in, *item);
    option_out.set(*item);
  }
} // Namespace IO
} //Namespace Biogears
