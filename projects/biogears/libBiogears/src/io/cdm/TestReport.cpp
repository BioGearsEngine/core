
#include "TestReport.h"

#include "Property.h"

#include <biogears/cdm/properties/SEFunction.h>
#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestErrorStatistics.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

namespace biogears {
namespace io {
  // class SETestErrorStatistics
  void TestReport::UnMarshall(const CDM::TestErrorStatisticsData& in, SETestErrorStatistics& out)
  {
    out.Reset();
    out.m_MinimumError = in.MinimumError().get();
    out.m_MaximumError = in.MaximumError().get();
    out.m_AverageError = in.AverageError().get();
    out.m_StandardDeviation = in.StandardDeviation().get();

    std::string dData;
    for (unsigned int i = 0; i < in.Differences().size(); i++) {
      dData = (std::string)in.Differences().at(i);
      if (!dData.empty()) {
        out.m_Differences.push_back(dData);
      }
    }
    out.m_ComputedPropertyID = in.ComputedPropertyID();
    out.m_ExpectedPropertyID = in.ExpectedPropertyID();
    out.m_NumberOfErrors = in.NumberOfErrors().get();
    out.m_PercentTolerance = in.PercentTolerance();
    out.m_PropertyName = in.PropertyName();

    io::Property::UnMarshall(in.PercentToleranceVsNumErrors(), out.GetPercentToleranceVsNumErrors());

    if (!out.IsValid()) {
      throw CommonDataModelException("Unable to Marhsall SETestErrorStatistics from the given TestErrorStatisticsData");
    }
  }
  //-----------------------------------------------------------------------------
  void TestReport::Marshall(const SETestErrorStatistics& in, CDM::TestErrorStatisticsData& out)
  {
    if (!std::isnan(in.m_MinimumError)) {
      out.MinimumError(in.m_MinimumError);
    }
    if (!std::isnan(in.m_MaximumError)) {
      out.MaximumError(in.m_MaximumError);
    }
    if (!std::isnan(in.m_AverageError)) {
      out.AverageError(in.m_AverageError);
    }
    if (!std::isnan(in.m_StandardDeviation)) {
      out.StandardDeviation(in.m_StandardDeviation);
    }

    std::string dData;
    for (unsigned int i = 0; i < in.m_Differences.size(); i++) {
      dData = in.m_Differences.at(i);
      if (!dData.empty()) {
        out.Differences().push_back(dData);
      }
    }

    out.ComputedPropertyID(in.m_ComputedPropertyID);
    out.ExpectedPropertyID(in.m_ExpectedPropertyID);

    out.NumberOfErrors(in.m_NumberOfErrors);
    out.PercentTolerance(in.m_PercentTolerance);
    out.PropertyName(in.m_PropertyName);

    CDM_OPTIONAL_PROPERTY_PTR_MARSHALL_HELPER(in, out, PercentToleranceVsNumErrors);
  }
  //-----------------------------------------------------------------------------
  // class SETestReport
  void TestReport::UnMarshall(const CDM::TestReportData& in, SETestReport& out)
  {
    out.Reset();

    SETestSuite* sx;
    CDM::TestSuiteData* sData;
    for (unsigned int i = 0; i < in.TestSuite().size(); i++) {
      sData = (CDM::TestSuiteData*)&in.TestSuite().at(i);
      if (sData != nullptr) {
        sx = new SETestSuite(out.GetLogger());
        UnMarshall(*sData, *sx);
      }
      out.m_testSuite.push_back(sx);
    }
  }
  //-----------------------------------------------------------------------------
  void TestReport::Marshall(const SETestReport& in, CDM::TestReportData& out)
  {
    for (unsigned int i = 0; i < in.m_testSuite.size(); i++) {
      auto data = CDM::TestSuiteData();
      Marshall(*in.m_testSuite.at(i), data);
      out.TestSuite().push_back(data);
    }
  }
  //-----------------------------------------------------------------------------
  // class SETestSuite
  void TestReport::UnMarshall(const CDM::TestSuiteData& in, SETestSuite& out)
  {
    out.Reset();
    out.m_Performed = in.Performed();
    out.m_Name = in.Name();

    std::string sData;
    for (unsigned int i = 0; i < in.Requirement().size(); i++) {
      sData = (std::string)in.Requirement().at(i);
      if (!sData.empty()) {
        out.m_Requirements.push_back(sData);
      }
    }

    SETestErrorStatistics* ex;
    CDM::TestErrorStatisticsData* eData;
    for (unsigned int i = 0; i < in.SuiteEqualError().size(); i++) {
      eData = (CDM::TestErrorStatisticsData*)&in.SuiteEqualError().at(i);
      if (eData != nullptr) {
        ex = new SETestErrorStatistics(out.GetLogger());
        TestReport::UnMarshall(*eData, *ex);
      }
      out.m_SuiteEqualError.push_back(ex);
    }

    SETestCase* tx;
    CDM::TestCaseData* tData;
    for (unsigned int i = 0; i < in.TestCase().size(); i++) {
      tData = (CDM::TestCaseData*)&in.TestCase().at(i);
      if (tData != nullptr) {
        tx = new SETestCase(out.m_Name, out.GetLogger());
        UnMarshall(*tData, *tx);
      }
      out.m_TestCase.push_back(tx);
    }
  }
  //-----------------------------------------------------------------------------
  void TestReport::Marshall(const SETestSuite& in, CDM::TestSuiteData& out)
  {
    std::string sData;
    for (unsigned int i = 0; i < in.m_Requirements.size(); i++) {
      sData = in.m_Requirements.at(i);
      if (!sData.empty())
        out.Requirement().push_back(sData);
    }
    for (unsigned int i = 0; i < in.m_SuiteEqualError.size(); i++) {
      auto tesData = CDM::TestErrorStatisticsData();
      Marshall(*in.m_SuiteEqualError.at(i), tesData);
      out.SuiteEqualError().push_back(tesData);
    }
    for (unsigned int i = 0; i < in.m_TestCase.size(); i++) {
      auto data = CDM::TestCaseData();
      Marshall(*in.m_TestCase.at(i), data);
      out.TestCase().push_back(data);
    }

    out.Performed(in.m_Performed);
    out.Duration(std::make_unique<std::remove_reference<decltype(out.Duration())>::type>());
    out.Duration().value(0);
    out.Duration().unit("s");
    if (in.GetNumberOfTests() != 0) {
      out.Tests(in.GetNumberOfTests());

      if (in.m_Duration.IsValid()) {
        io::Property::Marshall(in.m_Duration, out.Duration());
      }
      out.Errors(in.GetNumberOfErrors());
    }

    out.Name(in.m_Name);
  }
  //-----------------------------------------------------------------------------
  // class SETestCase
  void TestReport::UnMarshall(const CDM::TestCaseData& in, SETestCase& out)
  {
    SETestErrorStatistics* ex;
    CDM::TestErrorStatisticsData* eData;

    out.Reset();

    out.m_Name = in.Name();

    io::Property::UnMarshall(in.Duration(), out.GetDuration());

    for (unsigned int i = 0; i < in.CaseEqualError().size(); i++) {
      eData = (CDM::TestErrorStatisticsData*)&in.CaseEqualError().at(i);
      if (eData != nullptr) {
        ex = new SETestErrorStatistics(out.GetLogger());
        UnMarshall(*eData, *ex);
        out.m_CaseEqualsErrors.push_back(ex);
      }
    }

    for (unsigned int i = 0; i < in.Failure().size(); i++) {
      out.m_Failure.push_back(in.Failure().at(i));
    }
  }
  //-----------------------------------------------------------------------------
  void TestReport::Marshall(const SETestCase& in, CDM::TestCaseData& out)
  {
    out.Name(in.m_Name);

    // Duration is not optional in the format
    out.Duration(std::make_unique<std::remove_reference<decltype(out.Duration())>::type>());
    out.Duration().value(0);
    out.Duration().unit("s");
    if (in.m_Duration.IsValid()) {
      io::Property::Marshall(in.m_Duration, out.Duration());
    }

    for (unsigned int i = 0; i < in.m_Failure.size(); i++) {
      out.Failure().push_back(in.m_Failure.at(i));
    }

    for (unsigned int i = 0; i < in.m_CaseEqualsErrors.size(); i++) {
      auto data = std::make_unique<CDM::TestErrorStatisticsData>();
      Marshall(*in.m_CaseEqualsErrors.at(i), *data);
      out.CaseEqualError().push_back(std::move(data));
    }
  }
  //-----------------------------------------------------------------------------
}
}