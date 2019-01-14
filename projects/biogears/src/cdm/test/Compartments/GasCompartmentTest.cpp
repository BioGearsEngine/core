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
#include <biogears/cdm/circuit/fluid/SEFluidCircuit.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitNode.h>
#include <biogears/cdm/circuit/fluid/SEFluidCircuitPath.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartment.h>
#include <biogears/cdm/compartment/fluid/SEFluidCompartmentLink.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/GeneralMath.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
// Maybe we want to test a couple compartments that overlap
// i.e. 2 compartments containing the same node and or path

void CommonDataModelTest::TestGasFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("VolumesPressuresFlows");

  TimingProfile pTimer;
  pTimer.Start("Test");

  // Volumes and Pressures are just one to one mapping to a single scalar

  SECompartmentManager cmptMgr(subMgr);
  // Left Compartment
  SEGasCompartment* left = &cmptMgr.CreateGasCompartment("Left");
  // Middle Compartment
  SEGasCompartment* middle = &cmptMgr.CreateGasCompartment("Middle");
  // Right Compartment
  SEGasCompartment* right = &cmptMgr.CreateGasCompartment("Right");
  // Compartment Links
  double largeInflow_mL_Per_s = 5.0;
  double smallOutflow_mL_Per_s = 1.0;
  double largeOutflow_mL_Per_s = 7.0;
  double smallInflow_mL_Per_s = 2.0;
  double inflow_mL_Per_s;
  double outflow_mL_Per_s;
  SEGasCompartmentLink* left2middle = &cmptMgr.CreateGasLink(*left, *middle, "Left2Middle");
  left2middle->GetFlow().SetValue(largeInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEGasCompartmentLink* middle2right = &cmptMgr.CreateGasLink(*middle, *right, "Middle2Right");
  middle2right->GetFlow().SetValue(smallOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEGasCompartmentLink* middle2left = &cmptMgr.CreateGasLink(*middle, *left, "Middle2Left");
  middle2left->GetFlow().SetValue(largeOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEGasCompartmentLink* right2middle = &cmptMgr.CreateGasLink(*right, *middle, "Right2Middle");
  right2middle->GetFlow().SetValue(smallInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasFlows.xml");
  left = cmptMgr.GetGasCompartment("Left");
  middle = cmptMgr.GetGasCompartment("Middle");
  right = cmptMgr.GetGasCompartment("Right");
  left2middle = cmptMgr.GetGasLink("Left2Middle");
  middle2right = cmptMgr.GetGasLink("Middle2Right");
  middle2left = cmptMgr.GetGasLink("Middle2Left");
  right2middle = cmptMgr.GetGasLink("Right2Middle");

  inflow_mL_Per_s = largeInflow_mL_Per_s + smallInflow_mL_Per_s;
  outflow_mL_Per_s = largeOutflow_mL_Per_s + smallOutflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 small flows
  middle2right->GetFlow().SetValue(-smallOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  right2middle->GetFlow().SetValue(-smallInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  inflow_mL_Per_s = largeInflow_mL_Per_s + smallOutflow_mL_Per_s;
  outflow_mL_Per_s = largeOutflow_mL_Per_s + smallInflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 large flows
  left2middle->GetFlow().SetValue(-largeInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  middle2left->GetFlow().SetValue(-largeOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  inflow_mL_Per_s = largeOutflow_mL_Per_s + smallOutflow_mL_Per_s;
  outflow_mL_Per_s = largeInflow_mL_Per_s + smallInflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestGasHierarchyFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("GasFlowHierarchy");

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
  SEGasCompartment* L0C0 = &cmptMgr.CreateGasCompartment("L0C0");
  SEGasCompartment* L1C0 = &cmptMgr.CreateGasCompartment("L1C0");
  SEGasCompartment* L1C1 = &cmptMgr.CreateGasCompartment("L1C1");
  SEGasCompartment* L2C0 = &cmptMgr.CreateGasCompartment("L2C0");
  SEGasCompartment* L2C1 = &cmptMgr.CreateGasCompartment("L2C1");
  SEGasCompartment* L2C2 = &cmptMgr.CreateGasCompartment("L2C2");
  SEGasCompartment* L2C3 = &cmptMgr.CreateGasCompartment("L2C3");
  SEGasCompartment* L2C4 = &cmptMgr.CreateGasCompartment("L2C4");
  SEGasCompartment* L2C5 = &cmptMgr.CreateGasCompartment("L2C5");
  SEGasCompartment* L2C6 = &cmptMgr.CreateGasCompartment("L2C6");
  SEGasCompartment* L2C7 = &cmptMgr.CreateGasCompartment("L2C7");

  double L2C0toL2C4_mL_Per_s = 2;
  SEGasCompartmentLink* L2C0toL2C4 = &cmptMgr.CreateGasLink(*L2C0, *L2C4, "L2C0toL2C4");
  L2C0toL2C4->GetFlow().SetValue(L2C0toL2C4_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C4toL2C0_mL_Per_s = 4;
  SEGasCompartmentLink* L2C4toL2C0 = &cmptMgr.CreateGasLink(*L2C4, *L2C0, "L2C4toL2C0");
  L2C4toL2C0->GetFlow().SetValue(L2C4toL2C0_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C1toL2C5_mL_Per_s = 6;
  SEGasCompartmentLink* L2C1toL2C5 = &cmptMgr.CreateGasLink(*L2C1, *L2C5, "L2C1toL2C5");
  L2C1toL2C5->GetFlow().SetValue(L2C1toL2C5_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C5toL2C1_mL_Per_s = 8;
  SEGasCompartmentLink* L2C5toL2C1 = &cmptMgr.CreateGasLink(*L2C5, *L2C1, "L2C5toL2C1");
  L2C5toL2C1->GetFlow().SetValue(L2C5toL2C1_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C2toL2C6_mL_Per_s = 10;
  SEGasCompartmentLink* L2C2toL2C6 = &cmptMgr.CreateGasLink(*L2C2, *L2C6, "L2C2toL2C6");
  L2C2toL2C6->GetFlow().SetValue(L2C2toL2C6_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C6toL2C2_mL_Per_s = 12;
  SEGasCompartmentLink* L2C6toL2C2 = &cmptMgr.CreateGasLink(*L2C6, *L2C2, "L2C6toL2C2");
  L2C6toL2C2->GetFlow().SetValue(L2C6toL2C2_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C3toL2C7_mL_Per_s = 14;
  SEGasCompartmentLink* L2C3toL2C7 = &cmptMgr.CreateGasLink(*L2C3, *L2C7, "L2C3toL2C7");
  L2C3toL2C7->GetFlow().SetValue(L2C3toL2C7_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C7toL2C3_mL_Per_s = 18;
  SEGasCompartmentLink* L2C7toL2C3 = &cmptMgr.CreateGasLink(*L2C7, *L2C3, "L2C7toL2C3");
  L2C7toL2C3->GetFlow().SetValue(L2C7toL2C3_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  // Build up the hierarchy, note L2C4-7 are not in the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange(); // Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasHierarchyFlows.xml");
  L0C0 = cmptMgr.GetGasCompartment("L0C0");
  L1C0 = cmptMgr.GetGasCompartment("L1C0");
  L1C1 = cmptMgr.GetGasCompartment("L1C1");
  L2C0 = cmptMgr.GetGasCompartment("L2C0");
  L2C1 = cmptMgr.GetGasCompartment("L2C1");
  L2C2 = cmptMgr.GetGasCompartment("L2C2");
  L2C3 = cmptMgr.GetGasCompartment("L2C3");
  L2C4 = cmptMgr.GetGasCompartment("L2C4");
  L2C5 = cmptMgr.GetGasCompartment("L2C5");
  L2C6 = cmptMgr.GetGasCompartment("L2C6");
  L2C7 = cmptMgr.GetGasCompartment("L2C7");
  L2C0toL2C4 = cmptMgr.GetGasLink("L2C0toL2C4");
  L2C4toL2C0 = cmptMgr.GetGasLink("L2C4toL2C0");
  L2C1toL2C5 = cmptMgr.GetGasLink("L2C1toL2C5");
  L2C5toL2C1 = cmptMgr.GetGasLink("L2C5toL2C1");
  L2C2toL2C6 = cmptMgr.GetGasLink("L2C2toL2C6");
  L2C6toL2C2 = cmptMgr.GetGasLink("L2C6toL2C2");
  L2C3toL2C7 = cmptMgr.GetGasLink("L2C3toL2C7");
  L2C7toL2C3 = cmptMgr.GetGasLink("L2C7toL2C3");

  //                       Expected Inflow,       Expected Outflow
  TestFlow(testCase, *L2C0, L2C4toL2C0_mL_Per_s, L2C0toL2C4_mL_Per_s);
  TestFlow(testCase, *L2C1, L2C5toL2C1_mL_Per_s, L2C1toL2C5_mL_Per_s);
  TestFlow(testCase, *L2C2, L2C6toL2C2_mL_Per_s, L2C2toL2C6_mL_Per_s);
  TestFlow(testCase, *L2C3, L2C7toL2C3_mL_Per_s, L2C3toL2C7_mL_Per_s);
  TestFlow(testCase, *L2C4, L2C0toL2C4_mL_Per_s, L2C4toL2C0_mL_Per_s);
  TestFlow(testCase, *L2C5, L2C1toL2C5_mL_Per_s, L2C5toL2C1_mL_Per_s);
  TestFlow(testCase, *L2C6, L2C2toL2C6_mL_Per_s, L2C6toL2C2_mL_Per_s);
  TestFlow(testCase, *L2C7, L2C3toL2C7_mL_Per_s, L2C7toL2C3_mL_Per_s);

  TestFlow(testCase, *L1C0, L2C4toL2C0_mL_Per_s + L2C5toL2C1_mL_Per_s, L2C0toL2C4_mL_Per_s + L2C1toL2C5_mL_Per_s);
  TestFlow(testCase, *L1C1, L2C6toL2C2_mL_Per_s + L2C7toL2C3_mL_Per_s, L2C2toL2C6_mL_Per_s + L2C3toL2C7_mL_Per_s);
  TestFlow(testCase, *L0C0, L2C4toL2C0_mL_Per_s + L2C5toL2C1_mL_Per_s + L2C6toL2C2_mL_Per_s + L2C7toL2C3_mL_Per_s, L2C0toL2C4_mL_Per_s + L2C1toL2C5_mL_Per_s + L2C2toL2C6_mL_Per_s + L2C3toL2C7_mL_Per_s);

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestFlow(SETestCase& testCase, SEGasCompartment& cmpt, double inflow_mL_Per_s, double outflow_mL_Per_s)
{
  if (!cmpt.HasInFlow())
    testCase.AddFailure(std::string{ cmpt.GetName() }+ " does not have Inflow");
  if (!cmpt.HasOutFlow())
    testCase.AddFailure(std::string{ cmpt.GetName() }+ " does not have Outflow");
  m_ss << std::string{ cmpt.GetName() }+ " Inflow : " << cmpt.GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const InFlow is not correct : " << cmpt.GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Inflow : " << cmpt.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const InFlow is not correct : " << cmpt.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Outflow : " << cmpt.GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const OutFlow is not correct : " << cmpt.GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << std::string{ cmpt.GetName() }+ " Outflow : " << cmpt.GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const OutFlow is not correct : " << cmpt.GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::TestGasHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HierarchyVolumesPressuresFlows");

  SESubstance* N2 = subMgr.GetSubstance("Nitrogen");
  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");

  SEScalarPressure partialPressure;

  TimingProfile pTimer;
  pTimer.Start("Test");

  //         L0C0
  //        /    \
  //    L1C0      L1C1
  //    /  \      /  \
  // L2C0  L2C1 L2C3 L2C4 <-- Only these cmpts have data

  SECompartmentManager cmptMgr(subMgr);
  // Level 0
  SEGasCompartment* L0C0 = &cmptMgr.CreateGasCompartment("L0C0");
  // Level 1
  SEGasCompartment* L1C0 = &cmptMgr.CreateGasCompartment("L1C0");
  SEGasCompartment* L1C1 = &cmptMgr.CreateGasCompartment("L1C1");
  // Level 2
  double L2C0_mL = 3;
  double L2C0_mmHg = 3;
  SEGasCompartment* L2C0 = &cmptMgr.CreateGasCompartment("L2C0");
  L2C0->GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
  L2C0->GetPressure().SetValue(L2C0_mmHg, PressureUnit::mmHg);

  double L2C1_mL = 6;
  double L2C1_mmHg = 6;
  SEGasCompartment* L2C1 = &cmptMgr.CreateGasCompartment("L2C1");
  L2C1->GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
  L2C1->GetPressure().SetValue(L2C1_mmHg, PressureUnit::mmHg);

  double L2C2_mL = 9;
  double L2C2_mmHg = 9;
  SEGasCompartment* L2C2 = &cmptMgr.CreateGasCompartment("L2C2");
  L2C2->GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
  L2C2->GetPressure().SetValue(L2C2_mmHg, PressureUnit::mmHg);

  double L2C3_mL = 12;
  double L2C3_mmHg = 12;
  double L2C3_H20 = 0.5;
  double L2C3_pH = 7.38;
  SEGasCompartment* L2C3 = &cmptMgr.CreateGasCompartment("L2C3");
  L2C3->GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
  L2C3->GetPressure().SetValue(L2C3_mmHg, PressureUnit::mmHg);

  // Set up and test substances
  cmptMgr.AddGasCompartmentSubstance(*N2);
  cmptMgr.AddGasCompartmentSubstance(*O2);
  cmptMgr.AddGasCompartmentSubstance(*CO2);

  // Make sure Balance By VolumeFraction works
  SEGasSubstanceQuantity* L2C0_N2 = L2C0->GetSubstanceQuantity(*N2);
  double L2C0_N2_volFrac = 0.1;
  L2C0_N2->GetVolumeFraction().SetValue(L2C0_N2_volFrac);
  SEGasSubstanceQuantity* L2C0_O2 = L2C0->GetSubstanceQuantity(*O2);
  double L2C0_O2_volFrac = 0.7;
  L2C0_O2->GetVolumeFraction().SetValue(L2C0_O2_volFrac);
  SEGasSubstanceQuantity* L2C0_CO2 = L2C0->GetSubstanceQuantity(*CO2);
  double L2C0_CO2_volFrac = 0.2;
  L2C0_CO2->GetVolumeFraction().SetValue(L2C0_CO2_volFrac);
  L2C0->Balance(BalanceGasBy::VolumeFraction);

  // Make sure Balance By Volume works (Note Balance does not compute saturation)
  SEGasSubstanceQuantity* L2C1_N2 = L2C1->GetSubstanceQuantity(*N2);
  double L2C1_N2_mL = 1;
  L2C1_N2->GetVolume().SetValue(L2C1_N2_mL, VolumeUnit::mL);
  SEGasSubstanceQuantity* L2C1_O2 = L2C1->GetSubstanceQuantity(*O2);
  double L2C1_O2_mL = 2;
  L2C1_O2->GetVolume().SetValue(L2C1_O2_mL, VolumeUnit::mL);
  SEGasSubstanceQuantity* L2C1_CO2 = L2C1->GetSubstanceQuantity(*CO2);
  double L2C1_CO2_mL = 3;
  L2C1_CO2->GetVolume().SetValue(L2C1_CO2_mL, VolumeUnit::mL);
  L2C1->Balance(BalanceGasBy::Volume);

  SEGasSubstanceQuantity* L2C2_N2 = L2C2->GetSubstanceQuantity(*N2);
  L2C2_N2->GetVolumeFraction().SetValue(0.15);
  SEGasSubstanceQuantity* L2C2_O2 = L2C2->GetSubstanceQuantity(*O2);
  L2C2_O2->GetVolumeFraction().SetValue(0.25);
  SEGasSubstanceQuantity* L2C2_CO2 = L2C2->GetSubstanceQuantity(*CO2);
  L2C2_CO2->GetVolumeFraction().SetValue(0.60);
  L2C2->Balance(BalanceGasBy::VolumeFraction);

  SEGasSubstanceQuantity* L2C3_N2 = L2C3->GetSubstanceQuantity(*N2);
  L2C3_N2->GetVolumeFraction().SetValue(0.05);
  SEGasSubstanceQuantity* L2C3_O2 = L2C3->GetSubstanceQuantity(*O2);
  L2C3_O2->GetVolumeFraction().SetValue(0.15);
  SEGasSubstanceQuantity* L2C3_CO2 = L2C3->GetSubstanceQuantity(*CO2);
  L2C3_CO2->GetVolumeFraction().SetValue(0.80);
  L2C3->Balance(BalanceGasBy::VolumeFraction);

  // Build up the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasHierarchy.xml");
  L0C0 = cmptMgr.GetGasCompartment("L0C0");
  L1C0 = cmptMgr.GetGasCompartment("L1C0");
  L1C1 = cmptMgr.GetGasCompartment("L1C1");
  L2C0 = cmptMgr.GetGasCompartment("L2C0");
  L2C0_N2 = L2C0->GetSubstanceQuantity(*N2);
  L2C0_O2 = L2C0->GetSubstanceQuantity(*O2);
  L2C0_CO2 = L2C0->GetSubstanceQuantity(*CO2);
  L2C1 = cmptMgr.GetGasCompartment("L2C1");
  L2C1_N2 = L2C1->GetSubstanceQuantity(*N2);
  L2C1_O2 = L2C1->GetSubstanceQuantity(*O2);
  L2C1_CO2 = L2C1->GetSubstanceQuantity(*CO2);
  L2C2 = cmptMgr.GetGasCompartment("L2C2");
  L2C2_N2 = L2C2->GetSubstanceQuantity(*N2);
  L2C2_O2 = L2C2->GetSubstanceQuantity(*O2);
  L2C2_CO2 = L2C2->GetSubstanceQuantity(*CO2);
  L2C3 = cmptMgr.GetGasCompartment("L2C3");
  L2C3_N2 = L2C3->GetSubstanceQuantity(*N2);
  L2C3_O2 = L2C3->GetSubstanceQuantity(*O2);
  L2C3_CO2 = L2C3->GetSubstanceQuantity(*CO2);

  // Check our Volumes
  double L1C0_expected_mL = L2C0_mL + L2C1_mL;
  if (GeneralMath::PercentTolerance(L1C0->GetVolume(VolumeUnit::mL), L1C0_expected_mL) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " const Volume is not correct : " << L1C0->GetVolume(VolumeUnit::mL) << " expected " << L1C0_expected_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C0->GetVolume().GetValue(VolumeUnit::mL), L1C0_expected_mL) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " Volume is not correct : " << L1C0->GetVolume().GetValue(VolumeUnit::mL) << " expected " << L1C0_expected_mL;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_mL = L2C2_mL + L2C3_mL;
  if (GeneralMath::PercentTolerance(L1C1->GetVolume(VolumeUnit::mL), L1C1_expected_mL) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " const Volume is not correct : " << L1C1->GetVolume(VolumeUnit::mL) << " expected " << L1C1_expected_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C1->GetVolume().GetValue(VolumeUnit::mL), L1C1_expected_mL) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " Volume is not correct : " << L1C1->GetVolume().GetValue(VolumeUnit::mL) << " expected " << L1C1_expected_mL;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_mL = L1C0_expected_mL + L1C1_expected_mL;
  if (GeneralMath::PercentTolerance(L0C0->GetVolume(VolumeUnit::mL), L0C0_expected_mL) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " const Volume is not correct : " << L0C0->GetVolume(VolumeUnit::mL) << " expected " << L0C0_expected_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L0C0->GetVolume().GetValue(VolumeUnit::mL), L0C0_expected_mL) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " Volume is not correct : " << L0C0->GetVolume().GetValue(VolumeUnit::mL) << " expected " << L0C0_expected_mL;
    testCase.AddFailure(m_ss);
  }

  // Check our pressures
  double L1C0_expected_mmHg = (L2C0_mmHg * (L2C0_mL / L1C0_expected_mL)) + (L2C1_mmHg * (L2C1_mL / L1C0_expected_mL));
  if (GeneralMath::PercentTolerance(L1C0->GetPressure(PressureUnit::mmHg), L1C0_expected_mmHg) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " const Pressure is not correct : " << L1C0->GetPressure(PressureUnit::mmHg) << " expected " << L1C0_expected_mmHg;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C0->GetPressure().GetValue(PressureUnit::mmHg), L1C0_expected_mmHg) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " Pressure is not correct : " << L1C0->GetPressure().GetValue(PressureUnit::mmHg) << " expected " << L1C0_expected_mmHg;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_mmHg = (L2C2_mmHg * (L2C2_mL / L1C1_expected_mL)) + (L2C3_mmHg * (L2C3_mL / L1C1_expected_mL));
  if (GeneralMath::PercentTolerance(L1C1->GetPressure(PressureUnit::mmHg), L1C1_expected_mmHg) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " const Pressure is not correct : " << L1C1->GetPressure(PressureUnit::mmHg) << " expected " << L1C1_expected_mmHg;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C1->GetPressure().GetValue(PressureUnit::mmHg), L1C1_expected_mmHg) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " Pressure is not correct : " << L1C1->GetPressure().GetValue(PressureUnit::mmHg) << " expected " << L1C1_expected_mmHg;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_mmHg = (L1C0_expected_mmHg * (L1C0_expected_mL / L0C0_expected_mL)) + (L1C1_expected_mmHg * (L1C1_expected_mL / L0C0_expected_mL));
  if (GeneralMath::PercentTolerance(L0C0->GetPressure(PressureUnit::mmHg), L0C0_expected_mmHg) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " const Pressure is not correct : " << L0C0->GetPressure(PressureUnit::mmHg) << " expected " << L0C0_expected_mmHg;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L0C0->GetPressure().GetValue(PressureUnit::mmHg), L0C0_expected_mmHg) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " Pressure is not correct : " << L0C0->GetPressure().GetValue(PressureUnit::mmHg) << " expected " << L0C0_expected_mmHg;
    testCase.AddFailure(m_ss);
  }

  // Check substance volumes
  if (!L2C0_N2->HasVolume())
    testCase.AddFailure( std::string{"N2 Volume was not set from VolumeFraction Balance"});
  double L2CO_N2_vol_mL = L2C0_mL * L2C0_N2_volFrac;
  if (GeneralMath::PercentTolerance(L2C0_N2->GetVolume(VolumeUnit::mL), L2CO_N2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const Volume is not correct : " << L2C0_N2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_N2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_N2->GetVolume().GetValue(VolumeUnit::mL), L2CO_N2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 Volume is not correct : " << L2C0_N2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_N2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_O2->HasVolume())
    testCase.AddFailure( std::string{"O2 Volume was not set from VolumeFraction Balance"});
  double L2CO_O2_vol_mL = L2C0_mL * L2C0_O2_volFrac;
  if (GeneralMath::PercentTolerance(L2C0_O2->GetVolume(VolumeUnit::mL), L2CO_O2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " O2 const Volume is not correct : " << L2C0_O2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_O2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_O2->GetVolume().GetValue(VolumeUnit::mL), L2CO_O2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " O2 Volume is not correct : " << L2C0_O2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_O2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_CO2->HasVolume())
    testCase.AddFailure( std::string{"CO2 Volume was not set from VolumeFraction Balance"});
  double L2CO_CO2_vol_mL = L2C0_mL * L2C0_CO2_volFrac;
  if (GeneralMath::PercentTolerance(L2C0_CO2->GetVolume(VolumeUnit::mL), L2CO_CO2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " CO2 const Volume is not correct : " << L2C0_CO2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_CO2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_CO2->GetVolume().GetValue(VolumeUnit::mL), L2CO_CO2_vol_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " CO2 Volume is not correct : " << L2C0_CO2->GetVolume(VolumeUnit::mL) << " expected " << L2CO_CO2_vol_mL;
    testCase.AddFailure(m_ss);
  }
  // Check substance partial pressures
  if (!L2C0_N2->HasPartialPressure())
    testCase.AddFailure( std::string{"N2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C0_N2->GetVolumeFraction(), L2C0->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C0_N2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const PartialPressure is not correct : " << L2C0_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_N2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 PartialPressure is not correct : " << L2C0_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_O2->HasPartialPressure())
    testCase.AddFailure( std::string{"O2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C0_O2->GetVolumeFraction(), L2C0->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C0_O2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " O2 const PartialPressure is not correct : " << L2C0_O2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_O2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " O2 PartialPressure is not correct : " << L2C0_O2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_CO2->HasPartialPressure())
    testCase.AddFailure( std::string{"CO2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C0_CO2->GetVolumeFraction(), L2C0->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C0_CO2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " CO2 const PartialPressure is not correct : " << L2C0_CO2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_CO2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " CO2 PartialPressure is not correct : " << L2C0_CO2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }

  // Check substance volume fractions
  if (!L2C1_N2->HasVolumeFraction())
    testCase.AddFailure( std::string{"N2 VolumeFraction was not set from Volume Balance"});
  double L2C1_N2_VolumeFraction = L2C1_N2_mL / L2C1_mL;
  if (GeneralMath::PercentTolerance(const_cast<const SEGasSubstanceQuantity*>(L2C1_N2)->GetVolumeFraction(), L2C1_N2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 const VolumeFraction is not correct : " << const_cast<const SEGasSubstanceQuantity*>(L2C1_N2)->GetVolumeFraction() << " expected " << L2C1_N2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_N2->GetVolumeFraction().GetValue(), L2C1_N2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 VolumeFraction is not correct : " << L2C1_N2->GetVolumeFraction() << " expected " << L2C1_N2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_O2->HasVolumeFraction())
    testCase.AddFailure( std::string{"O2 VolumeFraction was not set from Volume Balance"});
  double L2C1_O2_VolumeFraction = L2C1_O2_mL / L2C1_mL;
  if (GeneralMath::PercentTolerance(const_cast<const SEGasSubstanceQuantity*>(L2C1_O2)->GetVolumeFraction(), L2C1_O2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " O2 const VolumeFraction is not correct : " << const_cast<const SEGasSubstanceQuantity*>(L2C1_O2)->GetVolumeFraction() << " expected " << L2C1_O2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_O2->GetVolumeFraction().GetValue(), L2C1_O2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " O2 VolumeFraction is not correct : " << L2C1_O2->GetVolumeFraction() << " expected " << L2C1_O2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_CO2->HasVolumeFraction())
    testCase.AddFailure( std::string{"CO2 VolumeFraction was not set from Volume Balance"});
  double L2C1_CO2_VolumeFraction = L2C1_CO2_mL / L2C1_mL;
  if (GeneralMath::PercentTolerance(const_cast<const SEGasSubstanceQuantity*>(L2C1_CO2)->GetVolumeFraction(), L2C1_CO2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " CO2 const VolumeFraction is not correct : " << const_cast<const SEGasSubstanceQuantity*>(L2C1_CO2)->GetVolumeFraction() << " expected " << L2C1_CO2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_CO2->GetVolumeFraction().GetValue(), L2C1_CO2_VolumeFraction) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " CO2 VolumeFraction is not correct : " << L2C1_CO2->GetVolumeFraction() << " expected " << L2C1_CO2_VolumeFraction;
    testCase.AddFailure(m_ss);
  }
  // Check substance partial pressures
  if (!L2C0_N2->HasPartialPressure())
    testCase.AddFailure( std::string{"N2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C1_N2->GetVolumeFraction(), L2C1->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C1_N2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 const PartialPressure is not correct : " << L2C1_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_N2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 PartialPressure is not correct : " << L2C1_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_O2->HasPartialPressure())
    testCase.AddFailure( std::string{"O2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C1_O2->GetVolumeFraction(), L2C1->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C1_O2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " O2 const PartialPressure is not correct : " << L2C1_O2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_O2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " O2 PartialPressure is not correct : " << L2C1_O2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_CO2->HasPartialPressure())
    testCase.AddFailure( std::string{"CO2 PartialPressure was not set from VolumeFraction Balance"});
  GeneralMath::CalculatePartialPressureInGas(L2C1_CO2->GetVolumeFraction(), L2C1->GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C1_CO2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " CO2 const PartialPressure is not correct : " << L2C1_CO2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_CO2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " CO2 PartialPressure is not correct : " << L2C1_CO2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }

  SEGasSubstanceQuantity* L1C0_N2 = L1C0->GetSubstanceQuantity(*N2);
  TestGasSubstanceQuantity(testCase, *L1C0, *L1C0_N2, (L2C0_N2->GetVolume(VolumeUnit::mL) + L2C1_N2->GetVolume(VolumeUnit::mL)), (L2C0->GetVolume(VolumeUnit::mL) + L2C1->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L1C0_O2 = L1C0->GetSubstanceQuantity(*O2);
  TestGasSubstanceQuantity(testCase, *L1C0, *L1C0_O2, (L2C0_O2->GetVolume(VolumeUnit::mL) + L2C1_O2->GetVolume(VolumeUnit::mL)), (L2C0->GetVolume(VolumeUnit::mL) + L2C1->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L1C0_CO2 = L1C0->GetSubstanceQuantity(*CO2);
  TestGasSubstanceQuantity(testCase, *L1C0, *L1C0_CO2, (L2C0_CO2->GetVolume(VolumeUnit::mL) + L2C1_CO2->GetVolume(VolumeUnit::mL)), (L2C0->GetVolume(VolumeUnit::mL) + L2C1->GetVolume(VolumeUnit::mL)));

  SEGasSubstanceQuantity* L1C1_N2 = L1C1->GetSubstanceQuantity(*N2);
  TestGasSubstanceQuantity(testCase, *L1C1, *L1C1_N2, (L2C2_N2->GetVolume(VolumeUnit::mL) + L2C3_N2->GetVolume(VolumeUnit::mL)), (L2C2->GetVolume(VolumeUnit::mL) + L2C3->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L1C1_O2 = L1C1->GetSubstanceQuantity(*O2);
  TestGasSubstanceQuantity(testCase, *L1C1, *L1C1_O2, (L2C2_O2->GetVolume(VolumeUnit::mL) + L2C3_O2->GetVolume(VolumeUnit::mL)), (L2C2->GetVolume(VolumeUnit::mL) + L2C3->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L1C1_CO2 = L1C1->GetSubstanceQuantity(*CO2);
  TestGasSubstanceQuantity(testCase, *L1C1, *L1C1_CO2, (L2C2_CO2->GetVolume(VolumeUnit::mL) + L2C3_CO2->GetVolume(VolumeUnit::mL)), (L2C2->GetVolume(VolumeUnit::mL) + L2C3->GetVolume(VolumeUnit::mL)));

  SEGasSubstanceQuantity* L0C0_N2 = L0C0->GetSubstanceQuantity(*N2);
  TestGasSubstanceQuantity(testCase, *L0C0, *L0C0_N2, (L1C0_N2->GetVolume(VolumeUnit::mL) + L1C1_N2->GetVolume(VolumeUnit::mL)), (L1C0->GetVolume(VolumeUnit::mL) + L1C1->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L0C0_O2 = L0C0->GetSubstanceQuantity(*O2);
  TestGasSubstanceQuantity(testCase, *L0C0, *L0C0_O2, (L1C0_O2->GetVolume(VolumeUnit::mL) + L1C1_O2->GetVolume(VolumeUnit::mL)), (L1C0->GetVolume(VolumeUnit::mL) + L1C1->GetVolume(VolumeUnit::mL)));
  SEGasSubstanceQuantity* L0C0_CO2 = L0C0->GetSubstanceQuantity(*CO2);
  TestGasSubstanceQuantity(testCase, *L0C0, *L0C0_CO2, (L1C0_CO2->GetVolume(VolumeUnit::mL) + L1C1_CO2->GetVolume(VolumeUnit::mL)), (L1C0->GetVolume(VolumeUnit::mL) + L1C1->GetVolume(VolumeUnit::mL)));

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestGasCircuitVolumesPressuresAndFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("VolumesPressuresFlows");

  TimingProfile pTimer;
  pTimer.Start("Test");

  m_Circuits.Clear();
  SEFluidCircuit& circuit = m_Circuits.CreateFluidCircuit("TestCircuit");
  // Left Nodes
  SEFluidCircuitNode& l1 = circuit.CreateNode("Left1");
  l1.GetNextPressure().SetValue(10., PressureUnit::mmHg);
  l1.GetVolumeBaseline().SetValue(10., VolumeUnit::mL);
  SEFluidCircuitNode& l2 = circuit.CreateNode("Left2");
  l2.GetNextPressure().SetValue(22., PressureUnit::mmHg);
  // No Volume
  SEFluidCircuitNode& l3 = circuit.CreateNode("Left3");
  l3.GetVolumeBaseline().SetValue(30., VolumeUnit::mL);
  SEFluidCircuitNode& l4 = circuit.CreateNode("Left4");
  l4.GetNextPressure().SetValue(3., PressureUnit::mmHg);
  l4.GetVolumeBaseline().SetValue(8., VolumeUnit::mL);

  // Middle Nodes
  SEFluidCircuitNode& m1 = circuit.CreateNode("Middle1");
  m1.GetNextPressure().SetValue(4.0, PressureUnit::mmHg);
  m1.GetVolumeBaseline().SetValue(8.0, VolumeUnit::mL);
  SEFluidCircuitNode& m2 = circuit.CreateNode("Middle2");
  m2.GetNextPressure().SetValue(5.0, PressureUnit::mmHg);
  m2.GetVolumeBaseline().SetValue(10., VolumeUnit::mL);

  // Right Nodes
  SEFluidCircuitNode& r1 = circuit.CreateNode("Right1");
  r1.GetNextPressure().SetValue(10., PressureUnit::mmHg);
  //No Volume
  SEFluidCircuitNode& r2 = circuit.CreateNode("Right2");
  r2.GetNextPressure().SetValue(22., PressureUnit::mmHg);
  //No Volume
  // Paths
  double largeInflow_mL_Per_s = 5.0;
  double smallOutflow_mL_Per_s = 1.0;
  double largeOutflow_mL_Per_s = 7.0;
  double smallInflow_mL_Per_s = 2.0;
  double inflow_mL_Per_s;
  double outflow_mL_Per_s;
  SEFluidCircuitPath& p1 = circuit.CreatePath(l1, m1, "l1->m1"); //Large Inflow
  p1.GetNextFlow().SetValue(largeInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& p2 = circuit.CreatePath(m1, r1, "m1->r1"); //Small Outflow
  p2.GetNextFlow().SetValue(smallOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& p3 = circuit.CreatePath(m2, l2, "m2->l2"); // Large Outflow
  p3.GetNextFlow().SetValue(largeOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& p4 = circuit.CreatePath(r2, m2, "r2->m2"); // Small Inflow
  p4.GetNextFlow().SetValue(smallInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  circuit.SetNextAndCurrentFromBaselines();
  circuit.StateChange();

  SECompartmentManager cmptMgr(subMgr);
  // Left Compartment
  SEGasCompartment* left = &cmptMgr.CreateGasCompartment("Left");
  left->MapNode(l1);
  left->MapNode(l2);
  left->MapNode(l3);
  left->MapNode(l4);
  // Middle Compartment
  SEGasCompartment* middle = &cmptMgr.CreateGasCompartment("Middle");
  middle->MapNode(m1);
  middle->MapNode(m2);
  // Right Compartment
  SEGasCompartment* right = &cmptMgr.CreateGasCompartment("Right");
  right->MapNode(r1);
  right->MapNode(r2);
  // Compartment Links
  SEGasCompartmentLink* left2middle = &cmptMgr.CreateGasLink(*left, *middle, "Left2Middle");
  left2middle->MapPath(p1);
  SEGasCompartmentLink* middle2right = &cmptMgr.CreateGasLink(*middle, *right, "Middle2Right");
  middle2right->MapPath(p2);
  SEGasCompartmentLink* middle2left = &cmptMgr.CreateGasLink(*middle, *left, "Middle2Left");
  middle2left->MapPath(p3);
  SEGasCompartmentLink* right2middle = &cmptMgr.CreateGasLink(*right, *middle, "Right2Middle");
  right2middle->MapPath(p4);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasCircuitVolumesPressuresAndFlows.xml");
  left = cmptMgr.GetGasCompartment("Left");
  middle = cmptMgr.GetGasCompartment("Middle");
  right = cmptMgr.GetGasCompartment("Right");
  left2middle = cmptMgr.GetGasLink("Left2Middle");
  middle2right = cmptMgr.GetGasLink("Middle2Right");
  middle2left = cmptMgr.GetGasLink("Middle2Left");
  right2middle = cmptMgr.GetGasLink("Right2Middle");

  CheckGasPressureAndVolume(testCase, *left);
  CheckGasPressureAndVolume(testCase, *middle);
  CheckGasPressureAndVolume(testCase, *right);

  l1.GetNextPressure().SetValue(15., PressureUnit::mmHg); // <-- Compartment should return this pressure
  l1.GetVolumeBaseline().SetValue(7, VolumeUnit::mL);
  l2.GetNextPressure().SetValue(28., PressureUnit::mmHg);
  l3.GetVolumeBaseline().SetValue(70., VolumeUnit::mL);
  l4.GetNextPressure().SetValue(9., PressureUnit::mmHg);
  l4.GetVolumeBaseline().SetValue(13., VolumeUnit::mL);
  CheckGasPressureAndVolume(testCase, *left);

  m1.GetNextPressure().SetValue(1.0, PressureUnit::mmHg);
  m1.GetVolumeBaseline().SetValue(9.0, VolumeUnit::mL);
  m2.GetNextPressure().SetValue(2.0, PressureUnit::mmHg);
  m2.GetVolumeBaseline().SetValue(3., VolumeUnit::mL);
  CheckGasPressureAndVolume(testCase, *middle);

  r1.GetNextPressure().SetValue(101., PressureUnit::mmHg);
  r2.GetNextPressure().SetValue(227., PressureUnit::mmHg);
  CheckGasPressureAndVolume(testCase, *right);

  inflow_mL_Per_s = largeInflow_mL_Per_s + smallInflow_mL_Per_s;
  outflow_mL_Per_s = largeOutflow_mL_Per_s + smallOutflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 small flows
  p2.GetNextFlow().SetValue(-smallOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  p4.GetNextFlow().SetValue(-smallInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  inflow_mL_Per_s = largeInflow_mL_Per_s + smallOutflow_mL_Per_s;
  outflow_mL_Per_s = largeOutflow_mL_Per_s + smallInflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }

  // Change direction of the 2 large flows
  p1.GetNextFlow().SetValue(-largeInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  p3.GetNextFlow().SetValue(-largeOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  inflow_mL_Per_s = largeOutflow_mL_Per_s + smallOutflow_mL_Per_s;
  outflow_mL_Per_s = largeInflow_mL_Per_s + smallInflow_mL_Per_s;
  m_ss << "Middle Inflow : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Inflow : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. inflow_mL_Per_s " << inflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s), inflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const InFlow is not correct : " << middle->GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << inflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Middle Outflow : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " vs. outflow_mL_Per_s " << outflow_mL_Per_s;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s), outflow_mL_Per_s) > m_PercentTolerance) {
    m_ss << middle->GetName() << " const OutFlow is not correct : " << middle->GetOutFlow().GetValue(VolumePerTimeUnit::mL_Per_s) << " expected " << outflow_mL_Per_s;
    testCase.AddFailure(m_ss);
  }
  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::CheckGasPressureAndVolume(SETestCase& testCase, SEGasCompartment& cmpt)
{
  // Check Volume
  const std::vector<SEFluidCircuitNode*>& vNodes = cmpt.GetNodeMapping().GetNodes();
  bool hasVolume = false;
  double nVolume_mL = 0;
  for (SEFluidCircuitNode* n : vNodes) {
    if (n->HasNextVolume()) {
      hasVolume = true;
      nVolume_mL += n->GetNextVolume().GetValue(VolumeUnit::mL);
    }
  }
  if (hasVolume && cmpt.HasVolume()) {
    m_ss << "Cmpt Volume : " << cmpt.GetVolume(VolumeUnit::mL) << " vs. Node Volume " << nVolume_mL;
    Info(m_ss);
    if (GeneralMath::PercentTolerance(cmpt.GetVolume(VolumeUnit::mL), nVolume_mL) > m_PercentTolerance) {
      m_ss << cmpt.GetName() << " const Volume is not correct : " << cmpt.GetVolume(VolumeUnit::mL) << " expected " << nVolume_mL;
      testCase.AddFailure(m_ss);
    }
    m_ss << "Cmpt Volume : " << cmpt.GetVolume().GetValue(VolumeUnit::mL) << " vs. Node Volume " << nVolume_mL;
    Info(m_ss);
    if (GeneralMath::PercentTolerance(cmpt.GetVolume().GetValue(VolumeUnit::mL), nVolume_mL) > m_PercentTolerance) {
      m_ss << cmpt.GetName() << " Volume is not correct : " << cmpt.GetVolume().GetValue(VolumeUnit::mL) << " expected " << nVolume_mL;
      testCase.AddFailure(m_ss);
    }
  } else {
    if (hasVolume && !cmpt.HasVolume())
      Error("Nodes have volume, but compartment does not");
    else if (!hasVolume && cmpt.HasVolume())
      Error("Nodes does not have volume, but compartment does");
  }

  // We can do pressure in two ways
  // We look for nodes with volume and pressure
  // -- if multiple nodes with both are found we volume weight the pressure sum
  // If no nodes in the cmpt have volume, we look for nodes with pressure
  // -- if multiple nodes are found we average the pressure
  bool volumeWeightedPressure = false;
  bool averagePressure = false;
  int pressureNodes = 0;
  nVolume_mL = 0;
  for (SEFluidCircuitNode* n : cmpt.GetNodeMapping().GetNodes()) {
    if (n->HasNextPressure()) {
      averagePressure = true;
      if (n->HasNextVolume()) {
        volumeWeightedPressure = true;
        nVolume_mL += n->GetNextVolume().GetValue(VolumeUnit::mL);
      }
    }
  }
  if (volumeWeightedPressure == false && averagePressure == false) {
    Info("No pressure values found on compartment");
    return;
  }
  const std::vector<SEFluidCircuitNode*>& pNodes = cmpt.GetNodeMapping().GetNodes();
  double nPressure_cmH2O = 0;
  for (SEFluidCircuitNode* n : pNodes) {
    if (volumeWeightedPressure) {
      if (n->HasNextPressure() && n->HasNextVolume())
        nPressure_cmH2O += n->GetNextPressure().GetValue(PressureUnit::cmH2O) * (n->GetNextVolume(VolumeUnit::mL) / nVolume_mL);
    } else {
      if (n->HasNextPressure()) {
        pressureNodes++;
        nPressure_cmH2O += n->GetNextPressure().GetValue(PressureUnit::cmH2O);
      }
    }
  }
  if (!volumeWeightedPressure && averagePressure)
    nPressure_cmH2O /= pressureNodes;
  m_ss << "Cmpt Pressure : " << cmpt.GetPressure(PressureUnit::cmH2O) << " vs. Node Pressure " << nPressure_cmH2O;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetPressure(PressureUnit::cmH2O), nPressure_cmH2O) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " const Pressure is not correct : " << cmpt.GetPressure(PressureUnit::cmH2O) << " expected " << nPressure_cmH2O;
    testCase.AddFailure(m_ss);
  }
  m_ss << "Cmpt Pressure : " << cmpt.GetPressure().GetValue(PressureUnit::cmH2O) << " vs. Node Pressure " << nPressure_cmH2O;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(cmpt.GetPressure().GetValue(PressureUnit::cmH2O), nPressure_cmH2O) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " Pressure Get methods are not equal! : " << cmpt.GetPressure().GetValue(PressureUnit::cmH2O) << " expected " << nPressure_cmH2O;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::TestGasSubstanceQuantity(SETestCase& testCase, SEGasCompartment& cmpt, SEGasSubstanceQuantity& subQ, double totalSubVolume_mL, double totalVolume_mL)
{
  if (!subQ.HasVolume())
    testCase.AddFailure( std::string{"Volume was not set on "} + cmpt.GetName());
  if (GeneralMath::PercentTolerance(subQ.GetVolume(VolumeUnit::mL), totalSubVolume_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const Volume is not correct : " << subQ.GetVolume(VolumeUnit::mL) << " expected " << totalSubVolume_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetVolume().GetValue(VolumeUnit::mL), totalSubVolume_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 Volume is not correct : " << subQ.GetVolume(VolumeUnit::mL) << " expected " << totalSubVolume_mL;
    testCase.AddFailure(m_ss);
  }
  if (!subQ.HasVolumeFraction())
    testCase.AddFailure( std::string{"VolumeFraction was not set on  "} + cmpt.GetName());
  double subVolumeFraction = totalSubVolume_mL / totalVolume_mL;
  if (GeneralMath::PercentTolerance(const_cast<const SEGasSubstanceQuantity&>(subQ).GetVolumeFraction(), subVolumeFraction) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const VolumeFraction is not correct : " << const_cast<const SEGasSubstanceQuantity&>(subQ).GetVolumeFraction() << " expected " << subVolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetVolumeFraction().GetValue(), subVolumeFraction) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 VolumeFraction is not correct : " << subQ.GetVolumeFraction() << " expected " << subVolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (!subQ.HasPartialPressure())
    testCase.AddFailure( std::string{"PartialPressure was not set on  "} + cmpt.GetName());
  SEScalarPressure partialPressure;
  GeneralMath::CalculatePartialPressureInGas(subQ.GetVolumeFraction(), cmpt.GetPressure(), partialPressure);
  if (GeneralMath::PercentTolerance(subQ.GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const PartialPressure is not correct : " << subQ.GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 PartialPressure is not correct : " << subQ.GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::TestUpdateGasLinks(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("GasLinks");

  TimingProfile pTimer;
  pTimer.Start("Test");

  SECompartmentManager cmptMgr(subMgr);

  SEGasCompartment& carina = cmptMgr.CreateGasCompartment("Carina");
  SEGasCompartment& stomach = cmptMgr.CreateGasCompartment("Stomach");
  SEGasCompartment& mouth = cmptMgr.CreateGasCompartment("Mouth");
  SEGasCompartment& env = cmptMgr.CreateGasCompartment("Environment");
  SEGasCompartment& equip = cmptMgr.CreateGasCompartment("Equipment");

  SEGasCompartmentLink& mouth2carina = cmptMgr.CreateGasLink(mouth, carina, "MouthToCarina");
  SEGasCompartmentLink& mouth2stomach = cmptMgr.CreateGasLink(mouth, stomach, "MouthToStomach");
  SEGasCompartmentLink& env2mouth = cmptMgr.CreateGasLink(env, mouth, "EnvironmentToMouth");
  SEGasCompartmentLink& equip2mouth = cmptMgr.CreateGasLink(equip, mouth, "EquipmentToMouth");

  mouth2carina.GetFlow().SetValue(25, VolumePerTimeUnit::mL_Per_s);
  mouth2stomach.GetFlow().SetValue(33, VolumePerTimeUnit::mL_Per_s);
  env2mouth.GetFlow().SetValue(50, VolumePerTimeUnit::mL_Per_s);
  equip2mouth.GetFlow().SetValue(200, VolumePerTimeUnit::mL_Per_s);
  cmptMgr.StateChange();

  if (mouth.GetInFlow(VolumePerTimeUnit::mL_Per_s) != (env2mouth.GetFlow(VolumePerTimeUnit::mL_Per_s) + equip2mouth.GetFlow(VolumePerTimeUnit::mL_Per_s)))
    testCase.AddFailure( std::string{"Initial Mouth inflow is not the correct sum"});

  // Now make a graph that connects the environment to the mouth
  SEGasCompartmentGraph& envGraph = cmptMgr.CreateGasGraph("EnvironmentGraph");
  envGraph.AddCompartment(carina);
  envGraph.AddCompartment(stomach);
  envGraph.AddCompartment(mouth);
  envGraph.AddCompartment(env);
  envGraph.AddCompartment(equip);
  envGraph.AddLink(mouth2carina);
  envGraph.AddLink(mouth2stomach);
  envGraph.AddLink(env2mouth);
  cmptMgr.UpdateLinks(envGraph);

  if (mouth.GetInFlow(VolumePerTimeUnit::mL_Per_s) != env2mouth.GetFlow(VolumePerTimeUnit::mL_Per_s))
    testCase.AddFailure( std::string{"Initial Mouth inflow is not the environment flow"});

  // Now make a graph that connects the equipment to the mouth
  SEGasCompartmentGraph& equipGraph = cmptMgr.CreateGasGraph("EquipmentGraph");
  equipGraph.AddCompartment(carina);
  equipGraph.AddCompartment(stomach);
  equipGraph.AddCompartment(mouth);
  equipGraph.AddCompartment(env);
  equipGraph.AddCompartment(equip);
  equipGraph.AddLink(mouth2carina);
  equipGraph.AddLink(mouth2stomach);
  equipGraph.AddLink(equip2mouth);
  cmptMgr.UpdateLinks(equipGraph);

  if (mouth.GetInFlow(VolumePerTimeUnit::mL_Per_s) != equip2mouth.GetFlow(VolumePerTimeUnit::mL_Per_s))
    testCase.AddFailure( std::string{"Initial Mouth inflow is not the equipment flow"});

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::GasCompartmentTest(const std::string& rptDirectory)
{
  m_PercentTolerance = 2.0;
  m_OutDirectory = rptDirectory;

  m_Logger->ResetLogFile(rptDirectory + "/GasCompartmentTest.log");
  SETestReport testReport(m_Logger);

  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();

  SETestSuite& Flows = testReport.CreateTestSuite();
  Flows.SetName("GasCompartmentFlows");
  TestGasFlows(Flows, subMgr);

  SETestSuite& HierarchyFlows = testReport.CreateTestSuite();
  HierarchyFlows.SetName("GasCompartmentHierarchyFlows");
  TestGasHierarchyFlows(HierarchyFlows, subMgr);

  SETestSuite& Hierarchy = testReport.CreateTestSuite();
  Hierarchy.SetName("GasCompartmentHierarchy");
  TestGasHierarchy(Hierarchy, subMgr);

  SETestSuite& CircuitVolumesPressuresAndFlows = testReport.CreateTestSuite();
  CircuitVolumesPressuresAndFlows.SetName("GasCompartmentVolumesPressuresAndFlows");
  TestGasCircuitVolumesPressuresAndFlows(CircuitVolumesPressuresAndFlows, subMgr);

  SETestSuite& UpdateGasLinks = testReport.CreateTestSuite();
  UpdateGasLinks.SetName("UpdateGasLinks");
  TestUpdateGasLinks(UpdateGasLinks, subMgr);

  testReport.WriteFile(rptDirectory + "/GasCompartmentTestReport.xml");
}
}