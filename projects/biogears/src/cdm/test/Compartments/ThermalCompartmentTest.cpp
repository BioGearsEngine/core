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
#include <biogears/cdm/test/CommonDataModelTest.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuit.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitNode.h>
#include <biogears/cdm/circuit/thermal/SEThermalCircuitPath.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartment.h>
#include <biogears/cdm/compartment/thermal/SEThermalCompartmentLink.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/properties/SEScalarEnergy.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
// Maybe we want to test a couple compartements that overlap
// i.e. 2 compartments containing the same node and or path

//void CheckTemperatureAndHeat(SETestCase& testCase, SEThermalCompartment& cmpt);
//void TestFlow(SETestCase& testCase, SEThermalCompartment& cmpt, double inflow_kcal_Per_s, double outflow_kcal_Per_s);

void CommonDataModelTest::TestThermalFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HeatTemperatureFlows");

  TimingProfile pTimer;
  pTimer.Start("Test");

  // Heat and Temperature are just one to one mapping to a single scalar

  SECompartmentManager cmptMgr(subMgr);
  // Left Compartment
  SEThermalCompartment* left = &cmptMgr.CreateThermalCompartment("Left");
  // Middle Compartment
  SEThermalCompartment* middle = &cmptMgr.CreateThermalCompartment("Middle");
  // Right Compartment
  SEThermalCompartment* right = &cmptMgr.CreateThermalCompartment("Right");
  // Compartment Links
  double largeInflow_kcal_Per_s = 5.0;
  double smallOutflow_kcal_Per_s = 1.0;
  double largeOutflow_kcal_Per_s = 7.0;
  double smallInflow_kcal_Per_s = 2.0;
  double inflow_kcal_Per_s;
  double outflow_kcal_Per_s;
  SEThermalCompartmentLink* left2middle = &cmptMgr.CreateThermalLink(*left, *middle, "Left2Middle");
  left2middle->GetHeatTransferRate().SetValue(largeInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCompartmentLink* middle2right = &cmptMgr.CreateThermalLink(*middle, *right, "Middle2Right");
  middle2right->GetHeatTransferRate().SetValue(smallOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCompartmentLink* middle2left = &cmptMgr.CreateThermalLink(*middle, *left, "Middle2Left");
  middle2left->GetHeatTransferRate().SetValue(largeOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCompartmentLink* right2middle = &cmptMgr.CreateThermalLink(*right, *middle, "Right2Middle");
  right2middle->GetHeatTransferRate().SetValue(smallInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestThermalFlows.xml");
  left = cmptMgr.GetThermalCompartment("Left");
  middle = cmptMgr.GetThermalCompartment("Middle");
  right = cmptMgr.GetThermalCompartment("Right");
  left2middle = cmptMgr.GetThermalLink("Left2Middle");
  middle2right = cmptMgr.GetThermalLink("Middle2Right");
  middle2left = cmptMgr.GetThermalLink("Middle2Left");
  right2middle = cmptMgr.GetThermalLink("Right2Middle");

  inflow_kcal_Per_s = largeInflow_kcal_Per_s + smallInflow_kcal_Per_s;
  outflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 small flows
  middle2right->GetHeatTransferRate().SetValue(-smallOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  right2middle->GetHeatTransferRate().SetValue(-smallInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  inflow_kcal_Per_s = largeInflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  outflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallInflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 large flows
  left2middle->GetHeatTransferRate().SetValue(-largeInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  middle2left->GetHeatTransferRate().SetValue(-largeOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  inflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  outflow_kcal_Per_s = largeInflow_kcal_Per_s + smallInflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestThermalFlowHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("ThermalFlowHierarchy");

  TimingProfile pTimer;
  pTimer.Start("Test");

  //                L0C0
  //               /    \
  //           L1C0      L1C1
  //           /  \      /  \
  //        L2C0  L2C1 L2C2 L2C3 <-- Only these cmpts have data
  //        L2C4  L2C5 L2C6 L2C7 <-- Have Data, same level as above, but these will not be in the cmpt hierarchy
  //                                 They define the L0 and L1 Level compartment input/output links

  SECompartmentManager cmptMgr(subMgr);
  SEThermalCompartment* L0C0 = &cmptMgr.CreateThermalCompartment("L0C0");
  SEThermalCompartment* L1C0 = &cmptMgr.CreateThermalCompartment("L1C0");
  SEThermalCompartment* L1C1 = &cmptMgr.CreateThermalCompartment("L1C1");
  SEThermalCompartment* L2C0 = &cmptMgr.CreateThermalCompartment("L2C0");
  SEThermalCompartment* L2C1 = &cmptMgr.CreateThermalCompartment("L2C1");
  SEThermalCompartment* L2C2 = &cmptMgr.CreateThermalCompartment("L2C2");
  SEThermalCompartment* L2C3 = &cmptMgr.CreateThermalCompartment("L2C3");
  SEThermalCompartment* L2C4 = &cmptMgr.CreateThermalCompartment("L2C4");
  SEThermalCompartment* L2C5 = &cmptMgr.CreateThermalCompartment("L2C5");
  SEThermalCompartment* L2C6 = &cmptMgr.CreateThermalCompartment("L2C6");
  SEThermalCompartment* L2C7 = &cmptMgr.CreateThermalCompartment("L2C7");

  double L2C0toL2C4_kcal_Per_s = 2;
  SEThermalCompartmentLink* L2C0toL2C4 = &cmptMgr.CreateThermalLink(*L2C0, *L2C4, "L2C0toL2C4");
  L2C0toL2C4->GetHeatTransferRate().SetValue(L2C0toL2C4_kcal_Per_s, PowerUnit::kcal_Per_s);
  double L2C4toL2C0_kcal_Per_s = 4;
  SEThermalCompartmentLink* L2C4toL2C0 = &cmptMgr.CreateThermalLink(*L2C4, *L2C0, "L2C4toL2C0");
  L2C4toL2C0->GetHeatTransferRate().SetValue(L2C4toL2C0_kcal_Per_s, PowerUnit::kcal_Per_s);

  double L2C1toL2C5_kcal_Per_s = 6;
  SEThermalCompartmentLink* L2C1toL2C5 = &cmptMgr.CreateThermalLink(*L2C1, *L2C5, "L2C1toL2C5");
  L2C1toL2C5->GetHeatTransferRate().SetValue(L2C1toL2C5_kcal_Per_s, PowerUnit::kcal_Per_s);
  double L2C5toL2C1_kcal_Per_s = 8;
  SEThermalCompartmentLink* L2C5toL2C1 = &cmptMgr.CreateThermalLink(*L2C5, *L2C1, "L2C5toL2C1");
  L2C5toL2C1->GetHeatTransferRate().SetValue(L2C5toL2C1_kcal_Per_s, PowerUnit::kcal_Per_s);

  double L2C2toL2C6_kcal_Per_s = 10;
  SEThermalCompartmentLink* L2C2toL2C6 = &cmptMgr.CreateThermalLink(*L2C2, *L2C6, "L2C2toL2C6");
  L2C2toL2C6->GetHeatTransferRate().SetValue(L2C2toL2C6_kcal_Per_s, PowerUnit::kcal_Per_s);
  double L2C6toL2C2_kcal_Per_s = 12;
  SEThermalCompartmentLink* L2C6toL2C2 = &cmptMgr.CreateThermalLink(*L2C6, *L2C2, "L2C6toL2C2");
  L2C6toL2C2->GetHeatTransferRate().SetValue(L2C6toL2C2_kcal_Per_s, PowerUnit::kcal_Per_s);

  double L2C3toL2C7_kcal_Per_s = 14;
  SEThermalCompartmentLink* L2C3toL2C7 = &cmptMgr.CreateThermalLink(*L2C3, *L2C7, "L2C3toL2C7");
  L2C3toL2C7->GetHeatTransferRate().SetValue(L2C3toL2C7_kcal_Per_s, PowerUnit::kcal_Per_s);
  double L2C7toL2C3_kcal_Per_s = 18;
  SEThermalCompartmentLink* L2C7toL2C3 = &cmptMgr.CreateThermalLink(*L2C7, *L2C3, "L2C7toL2C3");
  L2C7toL2C3->GetHeatTransferRate().SetValue(L2C7toL2C3_kcal_Per_s, PowerUnit::kcal_Per_s);

  // Build up the hierarchy, note L2C4-7 are not in the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange(); // Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestThermalFlowHierarchy.xml");
  L0C0 = cmptMgr.GetThermalCompartment("L0C0");
  L1C0 = cmptMgr.GetThermalCompartment("L1C0");
  L1C1 = cmptMgr.GetThermalCompartment("L1C1");
  L2C0 = cmptMgr.GetThermalCompartment("L2C0");
  L2C1 = cmptMgr.GetThermalCompartment("L2C1");
  L2C2 = cmptMgr.GetThermalCompartment("L2C2");
  L2C3 = cmptMgr.GetThermalCompartment("L2C3");
  L2C4 = cmptMgr.GetThermalCompartment("L2C4");
  L2C5 = cmptMgr.GetThermalCompartment("L2C5");
  L2C6 = cmptMgr.GetThermalCompartment("L2C6");
  L2C7 = cmptMgr.GetThermalCompartment("L2C7");

  //                       Expected Inflow,       Expected Outflow
  TestFlow(testCase, *L2C0, L2C4toL2C0_kcal_Per_s, L2C0toL2C4_kcal_Per_s);
  TestFlow(testCase, *L2C1, L2C5toL2C1_kcal_Per_s, L2C1toL2C5_kcal_Per_s);
  TestFlow(testCase, *L2C2, L2C6toL2C2_kcal_Per_s, L2C2toL2C6_kcal_Per_s);
  TestFlow(testCase, *L2C3, L2C7toL2C3_kcal_Per_s, L2C3toL2C7_kcal_Per_s);
  TestFlow(testCase, *L2C4, L2C0toL2C4_kcal_Per_s, L2C4toL2C0_kcal_Per_s);
  TestFlow(testCase, *L2C5, L2C1toL2C5_kcal_Per_s, L2C5toL2C1_kcal_Per_s);
  TestFlow(testCase, *L2C6, L2C2toL2C6_kcal_Per_s, L2C6toL2C2_kcal_Per_s);
  TestFlow(testCase, *L2C7, L2C3toL2C7_kcal_Per_s, L2C7toL2C3_kcal_Per_s);

  TestFlow(testCase, *L1C0, L2C4toL2C0_kcal_Per_s + L2C5toL2C1_kcal_Per_s, L2C0toL2C4_kcal_Per_s + L2C1toL2C5_kcal_Per_s);
  TestFlow(testCase, *L1C1, L2C6toL2C2_kcal_Per_s + L2C7toL2C3_kcal_Per_s, L2C2toL2C6_kcal_Per_s + L2C3toL2C7_kcal_Per_s);
  TestFlow(testCase, *L0C0, L2C4toL2C0_kcal_Per_s + L2C5toL2C1_kcal_Per_s + L2C6toL2C2_kcal_Per_s + L2C7toL2C3_kcal_Per_s, L2C0toL2C4_kcal_Per_s + L2C1toL2C5_kcal_Per_s + L2C2toL2C6_kcal_Per_s + L2C3toL2C7_kcal_Per_s);

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestFlow(SETestCase& testCase, SEThermalCompartment& cmpt, double inflow_kcal_Per_s, double outflow_kcal_Per_s)
{
  if (!cmpt.HasHeatTransferRateIn())
    testCase.AddFailure(std::string{ cmpt.GetName() }+ " does not have Inflow");
  if (!cmpt.HasHeatTransferRateOut())
    testCase.AddFailure(std::string{ cmpt.GetName() }+ " does not have Outflow");
  m_ss << std::string{ cmpt.GetName() }+ " Inflow : " << cmpt.GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const InFlow is not correct : " << cmpt.GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Inflow : " << cmpt.GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const InFlow is not correct : " << cmpt.GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Outflow : " << cmpt.GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const OutFlow is not correct : " << cmpt.GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Outflow : " << cmpt.GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const OutFlow is not correct : " << cmpt.GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::TestThermalHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HierarchyHeatTemperatureFlows");

  SESubstance* N2 = subMgr.GetSubstance("Nitrogen");

  SEScalarTemperature partialTemperature;

  TimingProfile pTimer;
  pTimer.Start("Test");

  //         L0C0
  //        /    \
  //    L1C0      L1C1
  //    /  \      /  \
  // L2C0  L2C1 L2C3 L2C4 <-- Only these cmpts have data

  SECompartmentManager cmptMgr(subMgr);
  // Level 0
  SEThermalCompartment* L0C0 = &cmptMgr.CreateThermalCompartment("L0C0");
  // Level 1
  SEThermalCompartment* L1C0 = &cmptMgr.CreateThermalCompartment("L1C0");
  SEThermalCompartment* L1C1 = &cmptMgr.CreateThermalCompartment("L1C1");
  // Level 2
  double L2C0_kcal = 3;
  double L2C0_C = 3;
  SEThermalCompartment* L2C0 = &cmptMgr.CreateThermalCompartment("L2C0");
  L2C0->GetHeat().SetValue(L2C0_kcal, EnergyUnit::kcal);
  L2C0->GetTemperature().SetValue(L2C0_C, TemperatureUnit::C);

  double L2C1_kcal = 6;
  double L2C1_C = 6;
  SEThermalCompartment* L2C1 = &cmptMgr.CreateThermalCompartment("L2C1");
  L2C1->GetHeat().SetValue(L2C1_kcal, EnergyUnit::kcal);
  L2C1->GetTemperature().SetValue(L2C1_C, TemperatureUnit::C);

  double L2C2_kcal = 9;
  double L2C2_C = 9;
  SEThermalCompartment* L2C2 = &cmptMgr.CreateThermalCompartment("L2C2");
  L2C2->GetHeat().SetValue(L2C2_kcal, EnergyUnit::kcal);
  L2C2->GetTemperature().SetValue(L2C2_C, TemperatureUnit::C);

  double L2C3_kcal = 12;
  double L2C3_C = 12;
  SEThermalCompartment* L2C3 = &cmptMgr.CreateThermalCompartment("L2C3");
  L2C3->GetHeat().SetValue(L2C3_kcal, EnergyUnit::kcal);
  L2C3->GetTemperature().SetValue(L2C3_C, TemperatureUnit::C);

  // Build up the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestThermalHierarchy.xml");
  L0C0 = cmptMgr.GetThermalCompartment("L0C0");
  L1C0 = cmptMgr.GetThermalCompartment("L1C0");
  L1C1 = cmptMgr.GetThermalCompartment("L1C1");
  L2C0 = cmptMgr.GetThermalCompartment("L2C0");
  L2C1 = cmptMgr.GetThermalCompartment("L2C1");
  L2C2 = cmptMgr.GetThermalCompartment("L2C2");
  L2C3 = cmptMgr.GetThermalCompartment("L2C3");

  // Check our Heat
  double L1C0_expected_kcal = L2C0_kcal + L2C1_kcal;
  if (L1C0->GetHeat(EnergyUnit::kcal) != L1C0_expected_kcal) {
    m_ss << L1C0->GetName() << " const Heat is not correct : " << L1C0->GetHeat(EnergyUnit::kcal) << " expected " << L1C0_expected_kcal;
    testCase.AddFailure(m_ss);
  }
  if (L1C0->GetHeat().GetValue(EnergyUnit::kcal) != L1C0_expected_kcal) {
    m_ss << L1C0->GetName() << " Heat is not correct : " << L1C0->GetHeat().GetValue(EnergyUnit::kcal) << " expected " << L1C0_expected_kcal;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_kcal = L2C2_kcal + L2C3_kcal;
  if (L1C1->GetHeat(EnergyUnit::kcal) != L1C1_expected_kcal) {
    m_ss << L1C1->GetName() << " const Heat is not correct : " << L1C1->GetHeat(EnergyUnit::kcal) << " expected " << L1C1_expected_kcal;
    testCase.AddFailure(m_ss);
  }
  if (L1C1->GetHeat().GetValue(EnergyUnit::kcal) != L1C1_expected_kcal) {
    m_ss << L1C1->GetName() << " Heat is not correct : " << L1C1->GetHeat().GetValue(EnergyUnit::kcal) << " expected " << L1C1_expected_kcal;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_kcal = L1C0_expected_kcal + L1C1_expected_kcal;
  if (L0C0->GetHeat(EnergyUnit::kcal) != L0C0_expected_kcal) {
    m_ss << L0C0->GetName() << " const Heat is not correct : " << L0C0->GetHeat(EnergyUnit::kcal) << " expected " << L0C0_expected_kcal;
    testCase.AddFailure(m_ss);
  }
  if (L0C0->GetHeat().GetValue(EnergyUnit::kcal) != L0C0_expected_kcal) {
    m_ss << L0C0->GetName() << " Heat is not correct : " << L0C0->GetHeat().GetValue(EnergyUnit::kcal) << " expected " << L0C0_expected_kcal;
    testCase.AddFailure(m_ss);
  }

  // Check our Temperature
  double L1C0_expected_C = (L2C0_C * (L2C0_kcal / L1C0_expected_kcal)) + (L2C1_C * (L2C1_kcal / L1C0_expected_kcal));
  if (L1C0->GetTemperature(TemperatureUnit::C) != L1C0_expected_C) {
    m_ss << L1C0->GetName() << " const Temperature is not correct : " << L1C0->GetTemperature(TemperatureUnit::C) << " expected " << L1C0_expected_C;
    testCase.AddFailure(m_ss);
  }
  if (L1C0->GetTemperature().GetValue(TemperatureUnit::C) != L1C0_expected_C) {
    m_ss << L1C0->GetName() << " Temperature is not correct : " << L1C0->GetTemperature().GetValue(TemperatureUnit::C) << " expected " << L1C0_expected_C;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_C = (L2C2_C * (L2C2_kcal / L1C1_expected_kcal)) + (L2C3_C * (L2C3_kcal / L1C1_expected_kcal));
  if (L1C1->GetTemperature(TemperatureUnit::C) != L1C1_expected_C) {
    m_ss << L1C1->GetName() << " const Temperature is not correct : " << L1C1->GetTemperature(TemperatureUnit::C) << " expected " << L1C1_expected_C;
    testCase.AddFailure(m_ss);
  }
  if (L1C1->GetTemperature().GetValue(TemperatureUnit::C) != L1C1_expected_C) {
    m_ss << L1C1->GetName() << " Temperature is not correct : " << L1C1->GetTemperature().GetValue(TemperatureUnit::C) << " expected " << L1C1_expected_C;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_C = (L1C0_expected_C * (L1C0_expected_kcal / L0C0_expected_kcal)) + (L1C1_expected_C * (L1C1_expected_kcal / L0C0_expected_kcal));
  if (L0C0->GetTemperature(TemperatureUnit::C) != L0C0_expected_C) {
    m_ss << L0C0->GetName() << " const Temperature is not correct : " << L0C0->GetTemperature(TemperatureUnit::C) << " expected " << L0C0_expected_C;
    testCase.AddFailure(m_ss);
  }
  if (L0C0->GetTemperature().GetValue(TemperatureUnit::C) != L0C0_expected_C) {
    m_ss << L0C0->GetName() << " Temperature is not correct : " << L0C0->GetTemperature().GetValue(TemperatureUnit::C) << " expected " << L0C0_expected_C;
    testCase.AddFailure(m_ss);
  }

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestCircuitHeatTemperatureAndFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HeatTemperatureFlows");

  TimingProfile pTimer;
  pTimer.Start("Test");

  SEThermalCircuit& circuit = m_Circuits.CreateThermalCircuit("TestCircuit");
  // Left Nodes
  SEThermalCircuitNode& l1 = circuit.CreateNode("Left1");
  l1.GetNextTemperature().SetValue(10., TemperatureUnit::C);
  l1.GetHeatBaseline().SetValue(10., EnergyUnit::kcal);
  SEThermalCircuitNode l2 = circuit.CreateNode("Left2"); // No Heat
  l2.GetNextTemperature().SetValue(22., TemperatureUnit::C);
  // Middle Nodes
  SEThermalCircuitNode& m1 = circuit.CreateNode("Middle1");
  m1.GetNextTemperature().SetValue(4.0, TemperatureUnit::C);
  m1.GetHeatBaseline().SetValue(8.0, EnergyUnit::kcal);
  SEThermalCircuitNode& m2 = circuit.CreateNode("Middle2");
  m2.GetNextTemperature().SetValue(5.0, TemperatureUnit::C);
  m2.GetHeatBaseline().SetValue(10., EnergyUnit::kcal);
  // Right Nodes
  SEThermalCircuitNode& r1 = circuit.CreateNode("Right1");
  r1.GetNextTemperature().SetValue(10., TemperatureUnit::C);
  r1.GetHeatBaseline().SetValue(10., EnergyUnit::kcal);
  SEThermalCircuitNode& r2 = circuit.CreateNode("Right2");
  r2.GetNextTemperature().SetValue(22., TemperatureUnit::C);
  r2.GetHeatBaseline().SetValue(10., EnergyUnit::kcal);
  // Paths
  double largeInflow_kcal_Per_s = 5.0;
  double smallOutflow_kcal_Per_s = 1.0;
  double largeOutflow_kcal_Per_s = 7.0;
  double smallInflow_kcal_Per_s = 2.0;
  double inflow_kcal_Per_s;
  double outflow_kcal_Per_s;
  SEThermalCircuitPath& p1 = circuit.CreatePath(l1, m1, "l1->m1"); //Large Inflow
  p1.GetNextHeatTransferRate().SetValue(largeInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCircuitPath& p2 = circuit.CreatePath(m1, r1, "m1->r1"); //Small Outflow
  p2.GetNextHeatTransferRate().SetValue(smallOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCircuitPath& p3 = circuit.CreatePath(m2, l2, "m2->l2"); // Large Outflow
  p3.GetNextHeatTransferRate().SetValue(largeOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  SEThermalCircuitPath& p4 = circuit.CreatePath(r2, m2, "r2->m2"); // Small Inflow
  p4.GetNextHeatTransferRate().SetValue(smallInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  circuit.SetNextAndCurrentFromBaselines();
  circuit.StateChange();

  SECompartmentManager cmptMgr(subMgr);
  // Left Compartment
  SEThermalCompartment* left = &cmptMgr.CreateThermalCompartment("Left");
  left->MapNode(l1);
  left->MapNode(l2);
  // Middle Compartment
  SEThermalCompartment* middle = &cmptMgr.CreateThermalCompartment("Middle");
  middle->MapNode(m1);
  middle->MapNode(m2);
  // Right Compartment
  SEThermalCompartment* right = &cmptMgr.CreateThermalCompartment("Right");
  right->MapNode(r1);
  right->MapNode(r2);
  // Compartment Links
  SEThermalCompartmentLink* left2middle = &cmptMgr.CreateThermalLink(*left, *middle, "Left2Middle");
  left2middle->MapPath(p1);
  SEThermalCompartmentLink* middle2right = &cmptMgr.CreateThermalLink(*middle, *right, "Middle2Right");
  middle2right->MapPath(p2);
  SEThermalCompartmentLink* middle2left = &cmptMgr.CreateThermalLink(*middle, *left, "Middle2Left");
  middle2left->MapPath(p3);
  SEThermalCompartmentLink* right2middle = &cmptMgr.CreateThermalLink(*right, *middle, "Right2Middle");
  right2middle->MapPath(p4);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestThermalCircuitHeatTemperatureAndFlows.xml");
  left = cmptMgr.GetThermalCompartment("Left");
  middle = cmptMgr.GetThermalCompartment("Middle");
  right = cmptMgr.GetThermalCompartment("Right");
  left2middle = cmptMgr.GetThermalLink("Left2Middle");
  middle2right = cmptMgr.GetThermalLink("Middle2Right");
  middle2left = cmptMgr.GetThermalLink("Middle2Left");

  CheckTemperatureAndHeat(testCase, *left);
  CheckTemperatureAndHeat(testCase, *middle);
  CheckTemperatureAndHeat(testCase, *right);

  inflow_kcal_Per_s = largeInflow_kcal_Per_s + smallInflow_kcal_Per_s;
  outflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 small flows
  p2.GetNextHeatTransferRate().SetValue(-smallOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  p4.GetNextHeatTransferRate().SetValue(-smallInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  inflow_kcal_Per_s = largeInflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  outflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallInflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 large flows
  p1.GetNextHeatTransferRate().SetValue(-largeInflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  p3.GetNextHeatTransferRate().SetValue(-largeOutflow_kcal_Per_s, PowerUnit::kcal_Per_s);
  inflow_kcal_Per_s = largeOutflow_kcal_Per_s + smallOutflow_kcal_Per_s;
  outflow_kcal_Per_s = largeInflow_kcal_Per_s + smallInflow_kcal_Per_s;
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " vs. inflow_kcal_Per_s " << inflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s), inflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetHeatTransferRateIn().GetValue(PowerUnit::kcal_Per_s) << " expected " << inflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " vs. outflow_kcal_Per_s " << outflow_kcal_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s), outflow_kcal_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetHeatTransferRateOut().GetValue(PowerUnit::kcal_Per_s) << " expected " << outflow_kcal_Per_s;
    testCase.AddFailure(m_ss);
  }
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::CheckTemperatureAndHeat(SETestCase& testCase, SEThermalCompartment& cmpt)
{
  // Check Heat
  const std::vector<SEThermalCircuitNode*>& vNodes = cmpt.GetNodeMapping().GetNodes();
  double nHeat_kcal = 0;
  for (SEThermalCircuitNode* n : vNodes) {
    if (n->HasNextHeat())
      nHeat_kcal += n->GetNextHeat().GetValue(EnergyUnit::kcal);
  }
  m_ss << "Cmpt Heat : " << cmpt.GetHeat(EnergyUnit::kcal) << " vs. Node Heat " << nHeat_kcal;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeat(EnergyUnit::kcal), nHeat_kcal) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const Heat is not correct : " << cmpt.GetHeat(EnergyUnit::kcal) << " expected " << nHeat_kcal;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Cmpt Heat : " << cmpt.GetHeat().GetValue(EnergyUnit::kcal) << " vs. Node Heat " << nHeat_kcal;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetHeat().GetValue(EnergyUnit::kcal), nHeat_kcal) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " Heat is not correct : " << cmpt.GetHeat().GetValue(EnergyUnit::kcal) << " expected " << nHeat_kcal;
    testCase.AddFailure(m_ss);
  }

  // We can do Temperature in two ways
  // We look for nodes with Heat and Temperature
  // -- if multiple nodes are found we Heat weight the Temperature sum
  // If no nodes have Heat and Temperature, we look for nodes with Temperature
  // -- if multiple nodes are found we average the Temperature
  bool HeatWeightedTemperature = false;
  bool averageTemperature = false;
  int TemperatureNodes = 0;
  for (SEThermalCircuitNode* n : cmpt.GetNodeMapping().GetNodes()) {
    if (n->HasNextPotential()) {
      averageTemperature = true;
      if (n->HasNextQuantity())
        HeatWeightedTemperature = true;
    }
  }
  if (HeatWeightedTemperature == false && averageTemperature == false) {
    Info("No Temperature values found on compartment");
    return;
  }
  const std::vector<SEThermalCircuitNode*>& pNodes = cmpt.GetNodeMapping().GetNodes();
  double nTemperature_C = 0;
  for (SEThermalCircuitNode* n : pNodes) {
    if (HeatWeightedTemperature) {
      if (n->HasNextTemperature() && n->HasNextHeat())
        nTemperature_C += n->GetNextTemperature().GetValue(TemperatureUnit::C) * (n->GetNextHeat(EnergyUnit::kcal) / nHeat_kcal);
    } else {
      if (n->HasNextTemperature()) {
        TemperatureNodes++;
        nTemperature_C += n->GetNextTemperature().GetValue(TemperatureUnit::C);
      }
    }
  }
  if (!HeatWeightedTemperature && averageTemperature)
    nTemperature_C /= TemperatureNodes;
  m_ss << "Cmpt Temperature : " << cmpt.GetTemperature(TemperatureUnit::C) << " vs. Node Temperature " << nTemperature_C;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetTemperature(TemperatureUnit::C), nTemperature_C) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const Temperature is not correct : " << cmpt.GetTemperature(TemperatureUnit::C) << " expected " << nTemperature_C;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Cmpt Temperature : " << cmpt.GetTemperature().GetValue(TemperatureUnit::C) << " vs. Node Temperature " << nTemperature_C;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetTemperature().GetValue(TemperatureUnit::C), nTemperature_C) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " Temperature Get methods are not equal! : " << cmpt.GetTemperature().GetValue(TemperatureUnit::C) << " expected " << nTemperature_C;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::ThermalCompartmentTest(const std::string& rptDirectory)
{
  m_PercentTolerance = 2.0;
  m_OutDirectory = rptDirectory;

  m_Logger->ResetLogFile(rptDirectory + "/ThermalCompartmentTest.log");
  SETestReport testReport(m_Logger);

  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();

  SETestSuite& Flows = testReport.CreateTestSuite();
  Flows.SetName("ThermalCompartmentFlows");
  TestThermalFlows(Flows, subMgr);

  SETestSuite& FlowHierarchy = testReport.CreateTestSuite();
  FlowHierarchy.SetName("ThermalCompartmentFlowHierarchy");
  TestThermalFlowHierarchy(FlowHierarchy, subMgr);

  SETestSuite& Hierarchy = testReport.CreateTestSuite();
  Hierarchy.SetName("ThermalCompartmentHierarchy");
  TestThermalHierarchy(Hierarchy, subMgr);

  SETestSuite& CircuitHeatTemperatureAndFlows = testReport.CreateTestSuite();
  CircuitHeatTemperatureAndFlows.SetName("ThermalCompartmentHeatTemperatureAndFlows");
  TestCircuitHeatTemperatureAndFlows(CircuitHeatTemperatureAndFlows, subMgr);

  testReport.WriteFile(rptDirectory + "/ThermalCompartmentTestReport.xml");
}
}