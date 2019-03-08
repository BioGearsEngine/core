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
#include <biogears/cdm/compartment/fluid/SELiquidCompartmentGraph.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
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

void CommonDataModelTest::TestLiquidFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("VolumesPressuresFlows");

  TimingProfile pTimer;
  pTimer.Start("Test");

  // Volumes and Pressures are just one to one mapping to a single scalar

  SECompartmentManager cmptMgr(subMgr);
  // Left Compartment
  SELiquidCompartment* left = &cmptMgr.CreateLiquidCompartment("Left");
  // Middle Compartment
  SELiquidCompartment* middle = &cmptMgr.CreateLiquidCompartment("Middle");
  // Right Compartment
  SELiquidCompartment* right = &cmptMgr.CreateLiquidCompartment("Right");
  // Compartment Links
  double largeInflow_mL_Per_s = 5.0;
  double smallOutflow_mL_Per_s = 1.0;
  double largeOutflow_mL_Per_s = 7.0;
  double smallInflow_mL_Per_s = 2.0;
  double inflow_mL_Per_s;
  double outflow_mL_Per_s;
  SELiquidCompartmentLink* left2middle = &cmptMgr.CreateLiquidLink(*left, *middle, "Left2Middle");
  left2middle->GetFlow().SetValue(largeInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SELiquidCompartmentLink* middle2right = &cmptMgr.CreateLiquidLink(*middle, *right, "Middle2Right");
  middle2right->GetFlow().SetValue(smallOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SELiquidCompartmentLink* middle2left = &cmptMgr.CreateLiquidLink(*middle, *left, "Middle2Left");
  middle2left->GetFlow().SetValue(largeOutflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  SELiquidCompartmentLink* right2middle = &cmptMgr.CreateLiquidLink(*right, *middle, "Right2Middle");
  right2middle->GetFlow().SetValue(smallInflow_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestLiqiudFlows.xml");
  left = cmptMgr.GetLiquidCompartment("Left");
  middle = cmptMgr.GetLiquidCompartment("Middle");
  right = cmptMgr.GetLiquidCompartment("Right");
  left2middle = cmptMgr.GetLiquidLink("Left2Middle");
  middle2right = cmptMgr.GetLiquidLink("Middle2Right");
  middle2left = cmptMgr.GetLiquidLink("Middle2Left");
  right2middle = cmptMgr.GetLiquidLink("Right2Middle");

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

void CommonDataModelTest::TestLiquidHierarchyFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("LiquidFlowHierarchy");

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
  SELiquidCompartment* L0C0 = &cmptMgr.CreateLiquidCompartment("L0C0");
  SELiquidCompartment* L1C0 = &cmptMgr.CreateLiquidCompartment("L1C0");
  SELiquidCompartment* L1C1 = &cmptMgr.CreateLiquidCompartment("L1C1");
  SELiquidCompartment* L2C0 = &cmptMgr.CreateLiquidCompartment("L2C0");
  SELiquidCompartment* L2C1 = &cmptMgr.CreateLiquidCompartment("L2C1");
  SELiquidCompartment* L2C2 = &cmptMgr.CreateLiquidCompartment("L2C2");
  SELiquidCompartment* L2C3 = &cmptMgr.CreateLiquidCompartment("L2C3");
  SELiquidCompartment* L2C4 = &cmptMgr.CreateLiquidCompartment("L2C4");
  SELiquidCompartment* L2C5 = &cmptMgr.CreateLiquidCompartment("L2C5");
  SELiquidCompartment* L2C6 = &cmptMgr.CreateLiquidCompartment("L2C6");
  SELiquidCompartment* L2C7 = &cmptMgr.CreateLiquidCompartment("L2C7");

  double L2C0toL2C4_mL_Per_s = 2;
  SELiquidCompartmentLink* L2C0toL2C4 = &cmptMgr.CreateLiquidLink(*L2C0, *L2C4, "L2C0toL2C4");
  L2C0toL2C4->GetFlow().SetValue(L2C0toL2C4_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C4toL2C0_mL_Per_s = 4;
  SELiquidCompartmentLink* L2C4toL2C0 = &cmptMgr.CreateLiquidLink(*L2C4, *L2C0, "L2C4toL2C0");
  L2C4toL2C0->GetFlow().SetValue(L2C4toL2C0_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C1toL2C5_mL_Per_s = 6;
  SELiquidCompartmentLink* L2C1toL2C5 = &cmptMgr.CreateLiquidLink(*L2C1, *L2C5, "L2C1toL2C5");
  L2C1toL2C5->GetFlow().SetValue(L2C1toL2C5_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C5toL2C1_mL_Per_s = 8;
  SELiquidCompartmentLink* L2C5toL2C1 = &cmptMgr.CreateLiquidLink(*L2C5, *L2C1, "L2C5toL2C1");
  L2C5toL2C1->GetFlow().SetValue(L2C5toL2C1_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C2toL2C6_mL_Per_s = 10;
  SELiquidCompartmentLink* L2C2toL2C6 = &cmptMgr.CreateLiquidLink(*L2C2, *L2C6, "L2C2toL2C6");
  L2C2toL2C6->GetFlow().SetValue(L2C2toL2C6_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C6toL2C2_mL_Per_s = 12;
  SELiquidCompartmentLink* L2C6toL2C2 = &cmptMgr.CreateLiquidLink(*L2C6, *L2C2, "L2C6toL2C2");
  L2C6toL2C2->GetFlow().SetValue(L2C6toL2C2_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  double L2C3toL2C7_mL_Per_s = 14;
  SELiquidCompartmentLink* L2C3toL2C7 = &cmptMgr.CreateLiquidLink(*L2C3, *L2C7, "L2C3toL2C7");
  L2C3toL2C7->GetFlow().SetValue(L2C3toL2C7_mL_Per_s, VolumePerTimeUnit::mL_Per_s);
  double L2C7toL2C3_mL_Per_s = 18;
  SELiquidCompartmentLink* L2C7toL2C3 = &cmptMgr.CreateLiquidLink(*L2C7, *L2C3, "L2C7toL2C3");
  L2C7toL2C3->GetFlow().SetValue(L2C7toL2C3_mL_Per_s, VolumePerTimeUnit::mL_Per_s);

  // Build up the hierarchy, note L2C4-7 are not in the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange(); // Call this, AFTER YOU SET UP YOUR HIERARCHY, to ensure all parent compartments have their link data

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestLiqiudHierarchyFlows.xml");
  L0C0 = cmptMgr.GetLiquidCompartment("L0C0");
  L1C0 = cmptMgr.GetLiquidCompartment("L1C0");
  L1C1 = cmptMgr.GetLiquidCompartment("L1C1");
  L2C0 = cmptMgr.GetLiquidCompartment("L2C0");
  L2C1 = cmptMgr.GetLiquidCompartment("L2C1");
  L2C2 = cmptMgr.GetLiquidCompartment("L2C2");
  L2C3 = cmptMgr.GetLiquidCompartment("L2C3");
  L2C4 = cmptMgr.GetLiquidCompartment("L2C4");
  L2C5 = cmptMgr.GetLiquidCompartment("L2C5");
  L2C6 = cmptMgr.GetLiquidCompartment("L2C6");
  L2C7 = cmptMgr.GetLiquidCompartment("L2C7");
  L2C0toL2C4 = cmptMgr.GetLiquidLink("L2C0toL2C4");
  L2C4toL2C0 = cmptMgr.GetLiquidLink("L2C4toL2C0");
  L2C1toL2C5 = cmptMgr.GetLiquidLink("L2C1toL2C5");
  L2C5toL2C1 = cmptMgr.GetLiquidLink("L2C5toL2C1");
  L2C2toL2C6 = cmptMgr.GetLiquidLink("L2C2toL2C6");
  L2C6toL2C2 = cmptMgr.GetLiquidLink("L2C6toL2C2");
  L2C3toL2C7 = cmptMgr.GetLiquidLink("L2C3toL2C7");
  L2C7toL2C3 = cmptMgr.GetLiquidLink("L2C7toL2C3");

  //                        Expected Inflow,     Expected Outflow
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

void CommonDataModelTest::TestFlow(SETestCase& testCase, SELiquidCompartment& cmpt, double inflow_mL_Per_s, double outflow_mL_Per_s)
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

void CommonDataModelTest::TestLiquidHierarchy(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HierarchyVolumesPressuresFlows");

  SESubstance* N2 = subMgr.GetSubstance("Nitrogen");

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
  SELiquidCompartment* L0C0 = &cmptMgr.CreateLiquidCompartment("L0C0");
  // Level 1
  SELiquidCompartment* L1C0 = &cmptMgr.CreateLiquidCompartment("L1C0");
  SELiquidCompartment* L1C1 = &cmptMgr.CreateLiquidCompartment("L1C1");
  // Level 2
  double L2C0_mL = 3;
  double L2C0_mmHg = 3;
  double L2C0_H20 = 0.1;
  double L2C0_pH = 7.32;
  SELiquidCompartment* L2C0 = &cmptMgr.CreateLiquidCompartment("L2C0");
  L2C0->GetVolume().SetValue(L2C0_mL, VolumeUnit::mL);
  L2C0->GetPressure().SetValue(L2C0_mmHg, PressureUnit::mmHg);
  L2C0->GetWaterVolumeFraction().SetValue(L2C0_H20);
  L2C0->GetPH().SetValue(L2C0_pH);

  double L2C1_mL = 6;
  double L2C1_mmHg = 6;
  double L2C1_H20 = 0.75;
  double L2C1_pH = 7.8;
  SELiquidCompartment* L2C1 = &cmptMgr.CreateLiquidCompartment("L2C1");
  L2C1->GetVolume().SetValue(L2C1_mL, VolumeUnit::mL);
  L2C1->GetPressure().SetValue(L2C1_mmHg, PressureUnit::mmHg);
  L2C1->GetWaterVolumeFraction().SetValue(L2C1_H20);
  L2C1->GetPH().SetValue(L2C1_pH);

  double L2C2_mL = 9;
  double L2C2_mmHg = 9;
  double L2C2_H20 = 0.25;
  double L2C2_pH = 7.2;
  SELiquidCompartment* L2C2 = &cmptMgr.CreateLiquidCompartment("L2C2");
  L2C2->GetVolume().SetValue(L2C2_mL, VolumeUnit::mL);
  L2C2->GetPressure().SetValue(L2C2_mmHg, PressureUnit::mmHg);
  L2C2->GetWaterVolumeFraction().SetValue(L2C2_H20);
  L2C2->GetPH().SetValue(L2C2_pH);

  double L2C3_mL = 12;
  double L2C3_mmHg = 12;
  double L2C3_H20 = 0.5;
  double L2C3_pH = 7.38;
  SELiquidCompartment* L2C3 = &cmptMgr.CreateLiquidCompartment("L2C3");
  L2C3->GetVolume().SetValue(L2C3_mL, VolumeUnit::mL);
  L2C3->GetPressure().SetValue(L2C3_mmHg, PressureUnit::mmHg);
  L2C3->GetWaterVolumeFraction().SetValue(L2C3_H20);
  L2C3->GetPH().SetValue(L2C3_pH);

  cmptMgr.AddLiquidCompartmentSubstance(*N2);

  SELiquidSubstanceQuantity* L2C0_N2 = L2C0->GetSubstanceQuantity(*N2);
  // Make sure Balance By Concentration works (Note Balance does not compute saturation)
  double L2C0_N2_mg_Per_mL = 1;
  L2C0_N2->GetConcentration().SetValue(L2C0_N2_mg_Per_mL, MassPerVolumeUnit::mg_Per_mL);
  L2C0_N2->Balance(BalanceLiquidBy::Concentration);
  if (!L2C0_N2->HasMass())
    testCase.AddFailure( std::string{"Mass was not set from Concentration Balance"});

  SELiquidSubstanceQuantity* L2C1_N2 = L2C1->GetSubstanceQuantity(*N2);
  double L2C1_N2_mg = 100;
  L2C1_N2->GetMass().SetValue(L2C1_N2_mg, MassUnit::mg);
  L2C1_N2->Balance(BalanceLiquidBy::Mass);
  // Make sure Balance By Mass works (Note Balance does not compute saturation)
  if (!L2C1_N2->HasConcentration())
    testCase.AddFailure( std::string{"Concentration was not set from Mass Balance"});

  SELiquidSubstanceQuantity* L2C2_N2 = L2C2->GetSubstanceQuantity(*N2);
  L2C2_N2->GetConcentration().SetValue(5, MassPerVolumeUnit::mg_Per_mL);
  L2C2_N2->Balance(BalanceLiquidBy::Concentration);

  SELiquidSubstanceQuantity* L2C3_N2 = L2C3->GetSubstanceQuantity(*N2);
  L2C3_N2->GetConcentration().SetValue(1, MassPerVolumeUnit::mg_Per_mL);
  L2C3_N2->Balance(BalanceLiquidBy::Concentration);

  // Build up the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasHierarchy.xml");
  L0C0 = cmptMgr.GetLiquidCompartment("L0C0");
  L1C0 = cmptMgr.GetLiquidCompartment("L1C0");
  L1C1 = cmptMgr.GetLiquidCompartment("L1C1");
  L2C0 = cmptMgr.GetLiquidCompartment("L2C0");
  L2C0_N2 = L2C0->GetSubstanceQuantity(*N2);
  L2C1 = cmptMgr.GetLiquidCompartment("L2C1");
  L2C1_N2 = L2C1->GetSubstanceQuantity(*N2);
  L2C2 = cmptMgr.GetLiquidCompartment("L2C2");
  L2C2_N2 = L2C2->GetSubstanceQuantity(*N2);
  L2C3 = cmptMgr.GetLiquidCompartment("L2C3");
  L2C3_N2 = L2C3->GetSubstanceQuantity(*N2);

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

  // Check our Water Fractions
  double L1C0_expected_H20 = ((L2C0_mL * L2C0_H20) + (L2C1_mL * L2C1_H20)) / (L2C0_mL + L2C1_mL);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L1C0)->GetWaterVolumeFraction(), L1C0_expected_H20) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " const H20 Fraction is not correct : " << const_cast<const SELiquidCompartment*>(L1C0)->GetWaterVolumeFraction() << " expected " << L1C0_expected_H20;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C0->GetWaterVolumeFraction().GetValue(), L1C0_expected_H20) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " H20 Fraction is not correct : " << L1C0->GetWaterVolumeFraction().GetValue() << " expected " << L1C0_expected_H20;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_H20 = ((L2C2_mL * L2C2_H20) + (L2C3_mL * L2C3_H20)) / (L2C2_mL + L2C3_mL);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L1C1)->GetWaterVolumeFraction(), L1C1_expected_H20) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " const H20 Fraction is not correct : " << const_cast<const SELiquidCompartment*>(L1C1)->GetWaterVolumeFraction() << " expected " << L1C1_expected_H20;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C1->GetWaterVolumeFraction().GetValue(), L1C1_expected_H20) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " H20 Fraction is not correct : " << L1C1->GetWaterVolumeFraction().GetValue() << " expected " << L1C1_expected_H20;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_H20 = ((L1C0_expected_mL * L1C0_expected_H20) + (L1C1_expected_mL * L1C1_expected_H20)) / (L1C0_expected_mL + L1C1_expected_mL);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L0C0)->GetWaterVolumeFraction(), L0C0_expected_H20) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " const H20 Fraction is not correct : " << const_cast<const SELiquidCompartment*>(L0C0)->GetWaterVolumeFraction() << " expected " << L0C0_expected_H20;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L0C0->GetWaterVolumeFraction().GetValue(), L0C0_expected_H20) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " H20 Fraction is not correct : " << L0C0->GetWaterVolumeFraction().GetValue() << " expected " << L0C0_expected_H20;
    testCase.AddFailure(m_ss);
  }

  // Check out pH
  double L1C0_expected_pH = -log10(((std::pow(10, -L2C0_pH) * L2C0_mL) + (std::pow(10, -L2C1_pH) * L2C1_mL)) / (L2C0_mL + L2C1_mL));
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L1C0)->GetPH(), L1C0_expected_pH) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " const pH is not correct : " << const_cast<const SELiquidCompartment*>(L1C0)->GetPH() << " expected " << L1C0_expected_pH;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C0->GetPH().GetValue(), L1C0_expected_pH) > m_PercentTolerance) {
    m_ss << L1C0->GetName() << " pH is not correct : " << L1C0->GetPH().GetValue() << " expected " << L1C0_expected_pH;
    testCase.AddFailure(m_ss);
  }
  double L1C1_expected_pH = -log10(((std::pow(10, -L2C2_pH) * L2C2_mL) + (std::pow(10, -L2C3_pH) * L2C3_mL)) / (L2C2_mL + L2C3_mL));
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L1C1)->GetPH(), L1C1_expected_pH) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " const pH is not correct : " << const_cast<const SELiquidCompartment*>(L1C1)->GetPH() << " expected " << L1C1_expected_pH;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L1C1->GetPH().GetValue(), L1C1_expected_pH) > m_PercentTolerance) {
    m_ss << L1C1->GetName() << " pH is not correct : " << L1C1->GetPH().GetValue() << " expected " << L1C1_expected_pH;
    testCase.AddFailure(m_ss);
  }
  double L0C0_expected_pH = -log10(((std::pow(10, -L1C0_expected_pH) * L1C0_expected_mL) + (std::pow(10, -L1C1_expected_pH) * L1C1_expected_mL)) / (L1C0_expected_mL + L1C1_expected_mL));
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidCompartment*>(L0C0)->GetPH(), L0C0_expected_pH) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " const pH is not correct : " << const_cast<const SELiquidCompartment*>(L0C0)->GetPH() << " expected " << L0C0_expected_pH;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L0C0->GetPH().GetValue(), L0C0_expected_pH) > m_PercentTolerance) {
    m_ss << L0C0->GetName() << " pH is not correct : " << L0C0->GetPH().GetValue() << " expected " << L0C0_expected_pH;
    testCase.AddFailure(m_ss);
  }

  // Test substances
  if (GeneralMath::PercentTolerance(L2C0_N2->GetMass(MassUnit::mg), L2C0_mL * L2C0_N2_mg_Per_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const Mass is not correct : " << L2C0_N2->GetMass(MassUnit::mg) << " expected " << L2C0_mL * L2C0_N2_mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_N2->GetMass().GetValue(MassUnit::mg), L2C0_mL * L2C0_N2_mg_Per_mL) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 Mass is not correct : " << L2C0_N2->GetMass(MassUnit::mg) << " expected " << L2C0_mL * L2C0_N2_mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_N2->HasMolarity())
    testCase.AddFailure( std::string{"Molarity was not set from Concentration Balance"});
  double L2C0_N2_mmol_Per_mL = L2C0_N2_mg_Per_mL / N2->GetMolarMass(MassPerAmountUnit::mg_Per_mmol);
  if ((GeneralMath::PercentTolerance(L2C0_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL), L2C0_N2_mmol_Per_mL)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const Molarity is not correct : " << L2C0_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << L2C0_N2_mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if ((GeneralMath::PercentTolerance(L2C0_N2->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_mL), L2C0_N2_mmol_Per_mL)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 Molarity is not correct : " << L2C0_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << L2C0_N2_mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_N2->HasPartialPressure())
    testCase.AddFailure( std::string{"PartialPressure was not set from Concentration Balance"});
  GeneralMath::CalculatePartialPressureInLiquid(*N2, L2C0_N2->GetConcentration(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C0_N2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const PartialPressure is not correct : " << L2C0_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_N2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 PartialPressure is not correct : " << L2C0_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }

  double L2C1_N2_concentration_mg_Per_mL = L2C1_N2_mg / L2C1_mL;
  if (GeneralMath::PercentTolerance(L2C1_N2->GetConcentration(MassPerVolumeUnit::mg_Per_mL), L2C1_N2_concentration_mg_Per_mL) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 const Concentration is not correct : " << L2C1_N2->GetConcentration(MassPerVolumeUnit::mg_Per_mL) << " expected " << L2C1_N2_concentration_mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_N2->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL), L2C1_N2_concentration_mg_Per_mL) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 Concentration is not correct : " << L2C1_N2->GetConcentration(MassPerVolumeUnit::mg_Per_mL) << " expected " << L2C1_N2_concentration_mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_N2->HasMolarity())
    testCase.AddFailure( std::string{"Molarity was not set from Mass Balance"});
  double L2C1_N2_mmol_Per_mL = L2C1_N2_concentration_mg_Per_mL / N2->GetMolarMass(MassPerAmountUnit::mg_Per_mmol);
  if ((GeneralMath::PercentTolerance(L2C1_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL), L2C1_N2_mmol_Per_mL)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 const Molarity is not correct : " << L2C1_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << L2C1_N2_mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if ((GeneralMath::PercentTolerance(L2C1_N2->GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_mL), L2C1_N2_mmol_Per_mL)) > m_PercentTolerance) {
    m_ss << L2C1->GetName() << " N2 Molarity is not correct : " << L2C1_N2->GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << L2C1_N2_mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!L2C1_N2->HasPartialPressure())
    testCase.AddFailure( std::string{"PartialPressure was not set from Mass Balance"});
  GeneralMath::CalculatePartialPressureInLiquid(*N2, L2C1_N2->GetConcentration(), partialPressure);
  if (GeneralMath::PercentTolerance(L2C1_N2->GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 const PartialPressure is not correct : " << L2C1_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C1_N2->GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " N2 PartialPressure is not correct : " << L2C1_N2->GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }

  SELiquidSubstanceQuantity* L1C0_N2 = L1C0->GetSubstanceQuantity(*N2);
  TestLiquidSubstanceQuantity(testCase, *L1C0, *L1C0_N2, (L2C0_N2->GetMass(MassUnit::mg) + L2C1_N2->GetMass(MassUnit::mg)), (L2C0->GetVolume(VolumeUnit::mL) + L2C1->GetVolume(VolumeUnit::mL)));
  SELiquidSubstanceQuantity* L1C1_N2 = L1C1->GetSubstanceQuantity(*N2);
  TestLiquidSubstanceQuantity(testCase, *L1C1, *L1C1_N2, (L2C2_N2->GetMass(MassUnit::mg) + L2C3_N2->GetMass(MassUnit::mg)), (L2C2->GetVolume(VolumeUnit::mL) + L2C3->GetVolume(VolumeUnit::mL)));
  SELiquidSubstanceQuantity* L0C0_N2 = L0C0->GetSubstanceQuantity(*N2);
  TestLiquidSubstanceQuantity(testCase, *L0C0, *L0C0_N2, (L1C0_N2->GetMass(MassUnit::mg) + L1C1_N2->GetMass(MassUnit::mg)), (L1C0->GetVolume(VolumeUnit::mL) + L1C1->GetVolume(VolumeUnit::mL)));

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::TestLiquidCircuitVolumesPressuresAndFlows(SETestSuite& testSuite, SESubstanceManager& subMgr)
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
  SEFluidCircuitNode l2 = circuit.CreateNode("Left2"); // No Volume
  l2.GetNextPressure().SetValue(22., PressureUnit::mmHg);
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
  r1.GetVolumeBaseline().SetValue(10., VolumeUnit::mL);
  SEFluidCircuitNode& r2 = circuit.CreateNode("Right2");
  r2.GetNextPressure().SetValue(22., PressureUnit::mmHg);
  r2.GetVolumeBaseline().SetValue(10., VolumeUnit::mL);
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
  SELiquidCompartment* left = &cmptMgr.CreateLiquidCompartment("Left");
  left->MapNode(l1);
  left->MapNode(l2);
  // Middle Compartment
  SELiquidCompartment* middle = &cmptMgr.CreateLiquidCompartment("Middle");
  middle->MapNode(m1);
  middle->MapNode(m2);
  // Right Compartment
  SELiquidCompartment* right = &cmptMgr.CreateLiquidCompartment("Right");
  right->MapNode(r1);
  right->MapNode(r2);
  // Compartment Links
  SELiquidCompartmentLink* left2middle = &cmptMgr.CreateLiquidLink(*left, *middle, "Left2Middle");
  left2middle->MapPath(p1);
  SELiquidCompartmentLink* middle2right = &cmptMgr.CreateLiquidLink(*middle, *right, "Middle2Right");
  middle2right->MapPath(p2);
  SELiquidCompartmentLink* middle2left = &cmptMgr.CreateLiquidLink(*middle, *left, "Middle2Left");
  middle2left->MapPath(p3);
  SELiquidCompartmentLink* right2middle = &cmptMgr.CreateLiquidLink(*right, *middle, "Right2Middle");
  right2middle->MapPath(p4);
  right2middle->MapPath(p4);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestGasCircuitVolumesPressuresAndFlows.xml");
  left = cmptMgr.GetLiquidCompartment("Left");
  middle = cmptMgr.GetLiquidCompartment("Middle");
  right = cmptMgr.GetLiquidCompartment("Right");
  left2middle = cmptMgr.GetLiquidLink("Left2Middle");
  middle2right = cmptMgr.GetLiquidLink("Middle2Right");
  middle2left = cmptMgr.GetLiquidLink("Middle2Left");
  right2middle = cmptMgr.GetLiquidLink("Right2Middle");

  CheckLiquidPressureAndVolume(testCase, *left);
  CheckLiquidPressureAndVolume(testCase, *middle);
  CheckLiquidPressureAndVolume(testCase, *right);

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

void CommonDataModelTest::TestLiquidHierarchySaturation(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  TimingProfile pTimer;
  pTimer.Start("Test");

  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("HierarchySaturation");

  SESubstance* O2 = subMgr.GetSubstance("Oxygen");
  SESubstance* CO2 = subMgr.GetSubstance("CarbonDioxide");
  SESubstance* Hb = subMgr.GetSubstance("Hemoglobin");
  SESubstance* HbO2 = subMgr.GetSubstance("Oxyhemoglobin");
  SESubstance* HbCO2 = subMgr.GetSubstance("Carbaminohemoglobin");
  SESubstance* HbO2CO2 = subMgr.GetSubstance("OxyCarbaminohemoglobin");

  //         L0C0
  //        /    \
  //    L1C0      L1C1
  //    /  \      /  \
  // L2C0  L2C1 L2C3 L2C4 <-- Only these cmpts have data

  SECompartmentManager cmptMgr(subMgr);
  // Level 0
  SELiquidCompartment* L0C0 = &cmptMgr.CreateLiquidCompartment("Level0Compartment0");
  // Level 1
  SELiquidCompartment* L1C0 = &cmptMgr.CreateLiquidCompartment("Level1Compartment0");
  SELiquidCompartment* L1C1 = &cmptMgr.CreateLiquidCompartment("Level1Compartment1");
  // Level 2
  double L2C0_L = 0.3;
  SELiquidCompartment* L2C0 = &cmptMgr.CreateLiquidCompartment("Level2Compartment0");
  L2C0->GetVolume().SetValue(L2C0_L, VolumeUnit::L);
  double L2C1_L = 0.6;
  SELiquidCompartment* L2C1 = &cmptMgr.CreateLiquidCompartment("Level2Compartment1");
  L2C1->GetVolume().SetValue(L2C1_L, VolumeUnit::L);
  double L2C2_L = 0.9;
  SELiquidCompartment* L2C2 = &cmptMgr.CreateLiquidCompartment("Level2Compartment2");
  L2C2->GetVolume().SetValue(L2C2_L, VolumeUnit::L);
  double L2C3_L = 1.2;
  SELiquidCompartment* L2C3 = &cmptMgr.CreateLiquidCompartment("Level2Compartment3");
  L2C3->GetVolume().SetValue(L2C3_L, VolumeUnit::L);

  cmptMgr.AddLiquidCompartmentSubstance(*O2);
  cmptMgr.AddLiquidCompartmentSubstance(*CO2);
  cmptMgr.AddLiquidCompartmentSubstance(*Hb);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbCO2);
  cmptMgr.AddLiquidCompartmentSubstance(*HbO2CO2);

  // Build up the hierarchy
  L0C0->AddChild(*L1C0);
  L0C0->AddChild(*L1C1);
  L1C0->AddChild(*L2C0);
  L1C0->AddChild(*L2C1);
  L1C1->AddChild(*L2C2);
  L1C1->AddChild(*L2C3);
  cmptMgr.StateChange();

  SELiquidSubstanceQuantity* L1C0_O2 = L1C0->GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* L1C0_CO2 = L1C0->GetSubstanceQuantity(*CO2);

  SELiquidSubstanceQuantity* L1C1_O2 = L1C1->GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* L1C1_CO2 = L1C1->GetSubstanceQuantity(*CO2);

  SELiquidSubstanceQuantity* L0C0_O2 = L0C0->GetSubstanceQuantity(*O2);
  SELiquidSubstanceQuantity* L0C0_CO2 = L0C0->GetSubstanceQuantity(*CO2);

  double L2C0_Hb_mmol_Per_L = 0.5;
  double L2C0_HbO2_mmol_Per_L = 11.5;
  double L2C0_HbCO2_mmol_Per_L = 0.25;
  double L2C0_HbO2CO2_mmol_Per_L = 2.75;
  double L2C0_Hb_mmol = L2C0_Hb_mmol_Per_L * L2C0_L;
  double L2C0_HbO2_mmol = L2C0_HbO2_mmol_Per_L * L2C0_L;
  double L2C0_HbCO2_mmol = L2C0_HbCO2_mmol_Per_L * L2C0_L;
  double L2C0_HbO2CO2_mmol = L2C0_HbO2CO2_mmol_Per_L * L2C0_L;
  double L2C0_total_mmol = (L2C0_Hb_mmol + L2C0_HbO2_mmol + L2C0_HbCO2_mmol + L2C0_HbO2CO2_mmol);
  double L2C0_O2_saturation = (L2C0_HbO2_mmol + L2C0_HbO2CO2_mmol) / L2C0_total_mmol;
  double L2C0_CO2_saturation = (L2C0_HbCO2_mmol + L2C0_HbO2CO2_mmol) / L2C0_total_mmol;
  SELiquidSubstanceQuantity* L2C0_O2 = L2C0->GetSubstanceQuantity(*O2);
  L2C0_O2->GetSaturation().SetValue(L2C0_O2_saturation);
  SELiquidSubstanceQuantity* L2C0_CO2 = L2C0->GetSubstanceQuantity(*CO2);
  L2C0_CO2->GetSaturation().SetValue(L2C0_CO2_saturation);
  SELiquidSubstanceQuantity* L2C0_Hb = L2C0->GetSubstanceQuantity(*Hb);
  L2C0_Hb->GetMolarity().SetValue(L2C0_Hb_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C0_Hb->Balance(BalanceLiquidBy::Molarity);
  // Check Balance worked correctly
  if (!L2C0_Hb->HasMass())
    testCase.AddFailure( std::string{"Mass was not set from Molarity Balance"});
  double L2C0_Hb_mg = L2C0_Hb_mmol_Per_L * Hb->GetMolarMass(MassPerAmountUnit::mg_Per_mmol) * L2C0_L;
  if (GeneralMath::PercentTolerance(L2C0_Hb->GetMass(MassUnit::mg), L2C0_Hb_mg) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " Hb const Mass is not correct : " << L2C0_Hb->GetMass(MassUnit::mg) << " expected " << L2C0_Hb_mg;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_Hb->GetMass().GetValue(MassUnit::mg), L2C0_Hb_mg) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " Hb Mass is not correct : " << L2C0_Hb->GetMass(MassUnit::mg) << " expected " << L2C0_Hb_mg;
    testCase.AddFailure(m_ss);
  }
  if (!L2C0_Hb->HasConcentration())
    testCase.AddFailure( std::string{"Concentration was not set from Molarity Balance"});
  double L2C0_Hb_mg_Per_L = L2C0_Hb_mg / L2C0_L;
  if (GeneralMath::PercentTolerance(L2C0_Hb->GetConcentration(MassPerVolumeUnit::mg_Per_L), L2C0_Hb_mg_Per_L) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " Hb const Concentration is not correct : " << L2C0_Hb->GetConcentration(MassPerVolumeUnit::mg_Per_L) << " expected " << L2C0_Hb_mg_Per_L;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(L2C0_Hb->GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_L), L2C0_Hb_mg_Per_L) > m_PercentTolerance) {
    m_ss << L2C0->GetName() << " Hb Concentration is not correct : " << L2C0_Hb->GetConcentration(MassPerVolumeUnit::mg_Per_L) << " expected " << L2C0_Hb_mg_Per_L;
    testCase.AddFailure(m_ss);
  }
  if (L2C0_Hb->HasPartialPressure())
    testCase.AddFailure( std::string{"Hb is not a Liquid, but has a partial pressure"});

  SELiquidSubstanceQuantity* L2C0_HbO2 = L2C0->GetSubstanceQuantity(*HbO2);
  L2C0_HbO2->GetMolarity().SetValue(L2C0_HbO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C0_HbO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C0_HbCO2 = L2C0->GetSubstanceQuantity(*HbCO2);
  L2C0_HbCO2->GetMolarity().SetValue(L2C0_HbCO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C0_HbCO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C0_HbO2CO2 = L2C0->GetSubstanceQuantity(*HbO2CO2);
  L2C0_HbO2CO2->GetMolarity().SetValue(L2C0_HbO2CO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C0_HbO2CO2->Balance(BalanceLiquidBy::Molarity);

  double L2C1_Hb_mmol_Per_L = 0.4;
  double L2C1_HbO2_mmol_Per_L = 11.0;
  double L2C1_HbCO2_mmol_Per_L = 0.45;
  double L2C1_HbO2CO2_mmol_Per_L = 3.75;
  double L2C1_Hb_mmol = L2C1_Hb_mmol_Per_L * L2C1_L;
  double L2C1_HbO2_mmol = L2C1_HbO2_mmol_Per_L * L2C1_L;
  double L2C1_HbCO2_mmol = L2C1_HbCO2_mmol_Per_L * L2C1_L;
  double L2C1_HbO2CO2_mmol = L2C1_HbO2CO2_mmol_Per_L * L2C1_L;
  double L2C1_total_mmol = (L2C1_Hb_mmol + L2C1_HbO2_mmol + L2C1_HbCO2_mmol + L2C1_HbO2CO2_mmol);
  double L2C1_O2_saturation = (L2C1_HbO2_mmol + L2C1_HbO2CO2_mmol) / L2C1_total_mmol;
  double L2C1_CO2_saturation = (L2C1_HbCO2_mmol + L2C1_HbO2CO2_mmol) / L2C1_total_mmol;
  SELiquidSubstanceQuantity* L2C1_O2 = L2C1->GetSubstanceQuantity(*O2);
  L2C1_O2->GetSaturation().SetValue(L2C1_O2_saturation);
  SELiquidSubstanceQuantity* L2C1_CO2 = L2C1->GetSubstanceQuantity(*CO2);
  L2C1_CO2->GetSaturation().SetValue(L2C1_CO2_saturation);
  SELiquidSubstanceQuantity* L2C1_Hb = L2C1->GetSubstanceQuantity(*Hb);
  L2C1_Hb->GetMolarity().SetValue(L2C1_Hb_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C1_Hb->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C1_HbO2 = L2C1->GetSubstanceQuantity(*HbO2);
  L2C1_HbO2->GetMolarity().SetValue(L2C1_HbO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C1_HbO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C1_HbCO2 = L2C1->GetSubstanceQuantity(*HbCO2);
  L2C1_HbCO2->GetMolarity().SetValue(L2C1_HbCO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C1_HbCO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C1_HbO2CO2 = L2C1->GetSubstanceQuantity(*HbO2CO2);
  L2C1_HbO2CO2->GetMolarity().SetValue(L2C1_HbO2CO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C1_HbO2CO2->Balance(BalanceLiquidBy::Molarity);

  double L2C2_Hb_mmol_Per_L = 0.6;
  double L2C2_HbO2_mmol_Per_L = 12.5;
  double L2C2_HbCO2_mmol_Per_L = 0.15;
  double L2C2_HbO2CO2_mmol_Per_L = 2.25;
  double L2C2_Hb_mmol = L2C2_Hb_mmol_Per_L * L2C2_L;
  double L2C2_HbO2_mmol = L2C2_HbO2_mmol_Per_L * L2C2_L;
  double L2C2_HbCO2_mmol = L2C2_HbCO2_mmol_Per_L * L2C2_L;
  double L2C2_HbO2CO2_mmol = L2C2_HbO2CO2_mmol_Per_L * L2C2_L;
  double L2C2_total_mmol = (L2C2_Hb_mmol + L2C2_HbO2_mmol + L2C2_HbCO2_mmol + L2C2_HbO2CO2_mmol);
  double L2C2_O2_saturation = (L2C2_HbO2_mmol + L2C2_HbO2CO2_mmol) / L2C2_total_mmol;
  double L2C2_CO2_saturation = (L2C2_HbCO2_mmol + L2C2_HbO2CO2_mmol) / L2C2_total_mmol;
  SELiquidSubstanceQuantity* L2C2_O2 = L2C2->GetSubstanceQuantity(*O2);
  L2C2_O2->GetSaturation().SetValue(L2C2_O2_saturation);
  SELiquidSubstanceQuantity* L2C2_CO2 = L2C2->GetSubstanceQuantity(*CO2);
  L2C2_CO2->GetSaturation().SetValue(L2C2_CO2_saturation);
  SELiquidSubstanceQuantity* L2C2_Hb = L2C2->GetSubstanceQuantity(*Hb);
  L2C2_Hb->GetMolarity().SetValue(L2C2_Hb_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C2_Hb->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C2_HbO2 = L2C2->GetSubstanceQuantity(*HbO2);
  L2C2_HbO2->GetMolarity().SetValue(L2C2_HbO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C2_HbO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C2_HbCO2 = L2C2->GetSubstanceQuantity(*HbCO2);
  L2C2_HbCO2->GetMolarity().SetValue(L2C2_HbCO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C2_HbCO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C2_HbO2CO2 = L2C2->GetSubstanceQuantity(*HbO2CO2);
  L2C2_HbO2CO2->GetMolarity().SetValue(L2C2_HbO2CO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C2_HbO2CO2->Balance(BalanceLiquidBy::Molarity);

  double L2C3_Hb_mmol_Per_L = 1.5;
  double L2C3_HbO2_mmol_Per_L = 10.5;
  double L2C3_HbCO2_mmol_Per_L = 0.95;
  double L2C3_HbO2CO2_mmol_Per_L = 2.05;
  double L2C3_Hb_mmol = L2C3_Hb_mmol_Per_L * L2C3_L;
  double L2C3_HbO2_mmol = L2C3_HbO2_mmol_Per_L * L2C3_L;
  double L2C3_HbCO2_mmol = L2C3_HbCO2_mmol_Per_L * L2C3_L;
  double L2C3_HbO2CO2_mmol = L2C3_HbO2CO2_mmol_Per_L * L2C3_L;
  double L2C3_total_mmol = (L2C3_Hb_mmol + L2C3_HbO2_mmol + L2C3_HbCO2_mmol + L2C3_HbO2CO2_mmol);
  double L2C3_O2_saturation = (L2C3_HbO2_mmol + L2C3_HbO2CO2_mmol) / L2C3_total_mmol;
  double L2C3_CO2_saturation = (L2C3_HbCO2_mmol + L2C3_HbO2CO2_mmol) / L2C3_total_mmol;
  SELiquidSubstanceQuantity* L2C3_O2 = L2C3->GetSubstanceQuantity(*O2);
  L2C3_O2->GetSaturation().SetValue(L2C3_O2_saturation);
  SELiquidSubstanceQuantity* L2C3_CO2 = L2C3->GetSubstanceQuantity(*CO2);
  L2C3_CO2->GetSaturation().SetValue(L2C3_CO2_saturation);
  SELiquidSubstanceQuantity* L2C3_Hb = L2C3->GetSubstanceQuantity(*Hb);
  L2C3_Hb->GetMolarity().SetValue(L2C3_Hb_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C3_Hb->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C3_HbO2 = L2C3->GetSubstanceQuantity(*HbO2);
  L2C3_HbO2->GetMolarity().SetValue(L2C3_HbO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C3_HbO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C3_HbCO2 = L2C3->GetSubstanceQuantity(*HbCO2);
  L2C3_HbCO2->GetMolarity().SetValue(L2C3_HbCO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C3_HbCO2->Balance(BalanceLiquidBy::Molarity);
  SELiquidSubstanceQuantity* L2C3_HbO2CO2 = L2C3->GetSubstanceQuantity(*HbO2CO2);
  L2C3_HbO2CO2->GetMolarity().SetValue(L2C3_HbO2CO2_mmol_Per_L, AmountPerVolumeUnit::mmol_Per_L);
  L2C3_HbO2CO2->Balance(BalanceLiquidBy::Molarity);

  double L1C0_O2_saturation = (L2C0_HbO2_mmol + L2C0_HbO2CO2_mmol + L2C1_HbO2_mmol + L2C1_HbO2CO2_mmol) / (L2C0_total_mmol + L2C1_total_mmol);
  if (!L1C0_O2->HasSaturation())
    testCase.AddFailure( std::string{"L1C0_O2 does not have saturation"});
  m_ss << "L1C0_O2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L1C0_O2)->GetSaturation() << " vs. L1C0_O2_saturation " << L1C0_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L1C0_O2)->GetSaturation(), L1C0_O2_saturation) > m_PercentTolerance) {
    m_ss << "L1C0 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L1C0_O2)->GetSaturation() << " expected " << L1C0_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L1C0_O2_saturation : " << L1C0_O2->GetSaturation().GetValue() << " vs. L1C0_O2_saturation " << L1C0_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L1C0_O2->GetSaturation().GetValue(), L1C0_O2_saturation) > m_PercentTolerance) {
    m_ss << "L1C0 Saturation is not correct : " << L1C0_O2->GetSaturation().GetValue() << " expected " << L1C0_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  double L1C0_CO2_saturation = (L2C0_HbCO2_mmol + L2C0_HbO2CO2_mmol + L2C1_HbCO2_mmol + L2C1_HbO2CO2_mmol) / (L2C0_total_mmol + L2C1_total_mmol);
  if (!L1C0_CO2->HasSaturation())
    testCase.AddFailure( std::string{"L1C0_CO2 does not have saturation"});
  m_ss << "L1C0_CO2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L1C0_CO2)->GetSaturation() << " vs. L1C0_CO2_saturation " << L1C0_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L1C0_CO2)->GetSaturation(), L1C0_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L1C0 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L1C0_CO2)->GetSaturation() << " expected " << L1C0_CO2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L1C0_CO2_saturation : " << L1C0_CO2->GetSaturation().GetValue() << " vs. L1C0_CO2_saturation " << L1C0_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L1C0_CO2->GetSaturation().GetValue(), L1C0_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L1C0 Saturation is not correct : " << L1C0_CO2->GetSaturation().GetValue() << " expected " << L1C0_CO2_saturation;
    testCase.AddFailure(m_ss);
  }

  double L1C1_O2_saturation = (L2C2_HbO2_mmol + L2C2_HbO2CO2_mmol + L2C3_HbO2_mmol + L2C3_HbO2CO2_mmol) / (L2C2_total_mmol + L2C3_total_mmol);
  if (!L1C1_O2->HasSaturation())
    testCase.AddFailure( std::string{"L1C1_O2 does not have saturation"});
  m_ss << "L1C1_O2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L1C1_O2)->GetSaturation() << " vs. L1C1_O2_saturation " << L1C1_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L1C1_O2)->GetSaturation(), L1C1_O2_saturation) > m_PercentTolerance) {
    m_ss << "L1C1 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L1C1_O2)->GetSaturation() << " expected " << L1C1_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L1C1_O2_saturation : " << L1C1_O2->GetSaturation().GetValue() << " vs. L1C1_O2_saturation " << L1C1_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L1C1_O2->GetSaturation().GetValue(), L1C1_O2_saturation) > m_PercentTolerance) {
    m_ss << "L1C1 Saturation is not correct : " << L1C1_O2->GetSaturation().GetValue() << " expected " << L1C1_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  double L1C1_CO2_saturation = (L2C2_HbCO2_mmol + L2C2_HbO2CO2_mmol + L2C3_HbCO2_mmol + L2C3_HbO2CO2_mmol) / (L2C2_total_mmol + L2C3_total_mmol);
  if (!L1C1_CO2->HasSaturation())
    testCase.AddFailure( std::string{"L1C1_CO2 does not have saturation"});
  m_ss << "L1C1_CO2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L1C1_CO2)->GetSaturation() << " vs. L1C1_CO2_saturation " << L1C1_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L1C1_CO2)->GetSaturation(), L1C1_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L1C1 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L1C1_CO2)->GetSaturation() << " expected " << L1C1_CO2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L1C1_CO2_saturation : " << L1C1_CO2->GetSaturation().GetValue() << " vs. L1C1_CO2_saturation " << L1C1_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L1C1_CO2->GetSaturation().GetValue(), L1C1_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L1C1 Saturation is not correct : " << L1C1_CO2->GetSaturation().GetValue() << " expected " << L1C1_CO2_saturation;
    testCase.AddFailure(m_ss);
  }

  double L0C0_O2_saturation = (L2C0_HbO2_mmol + L2C0_HbO2CO2_mmol + L2C1_HbO2_mmol + L2C1_HbO2CO2_mmol + L2C2_HbO2_mmol + L2C2_HbO2CO2_mmol + L2C3_HbO2_mmol + L2C3_HbO2CO2_mmol) / (L2C0_total_mmol + L2C1_total_mmol + L2C2_total_mmol + L2C3_total_mmol);
  if (!L0C0_O2->HasSaturation())
    testCase.AddFailure( std::string{"L0C0_O2 does not have saturation"});
  m_ss << "L0C0_O2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L0C0_O2)->GetSaturation() << " vs. L0C0_O2_saturation " << L0C0_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L0C0_O2)->GetSaturation(), L0C0_O2_saturation) > m_PercentTolerance) {
    m_ss << "L0C0 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L0C0_O2)->GetSaturation() << " expected " << L0C0_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L0C0_O2_saturation : " << L0C0_O2->GetSaturation().GetValue() << " vs. L0C0_O2_saturation " << L0C0_O2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L0C0_O2->GetSaturation().GetValue(), L0C0_O2_saturation) > m_PercentTolerance) {
    m_ss << "L0C0 Saturation is not correct : " << L0C0_O2->GetSaturation().GetValue() << " expected " << L0C0_O2_saturation;
    testCase.AddFailure(m_ss);
  }
  double L0C0_CO2_saturation = (L2C0_HbCO2_mmol + L2C0_HbO2CO2_mmol + L2C1_HbCO2_mmol + L2C1_HbO2CO2_mmol + L2C2_HbCO2_mmol + L2C2_HbO2CO2_mmol + L2C3_HbCO2_mmol + L2C3_HbO2CO2_mmol) / (L2C0_total_mmol + L2C1_total_mmol + L2C2_total_mmol + L2C3_total_mmol);
  if (!L0C0_CO2->HasSaturation())
    testCase.AddFailure( std::string{"L0C0_CO2 does not have saturation"});
  m_ss << "L0C0_CO2_saturation : " << const_cast<const SELiquidSubstanceQuantity*>(L0C0_CO2)->GetSaturation() << " vs. L0C0_CO2_saturation " << L0C0_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(const_cast<const SELiquidSubstanceQuantity*>(L0C0_CO2)->GetSaturation(), L0C0_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L0C0 const Saturation is not correct : " << const_cast<const SELiquidSubstanceQuantity*>(L0C0_CO2)->GetSaturation() << " expected " << L0C0_CO2_saturation;
    testCase.AddFailure(m_ss);
  }
  m_ss << "L0C0_CO2_saturation : " << L0C0_CO2->GetSaturation().GetValue() << " vs. L0C0_CO2_saturation " << L0C0_CO2_saturation;
  Info(m_ss);
  if (GeneralMath::PercentTolerance(L0C0_CO2->GetSaturation().GetValue(), L0C0_CO2_saturation) > m_PercentTolerance) {
    m_ss << "L0C0 Saturation is not correct : " << L0C0_CO2->GetSaturation().GetValue() << " expected " << L0C0_CO2_saturation;
    testCase.AddFailure(m_ss);
  }

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::CheckLiquidPressureAndVolume(SETestCase& testCase, SELiquidCompartment& cmpt)
{
  // Check Volume
  const std::vector<SEFluidCircuitNode*>& vNodes = cmpt.GetNodeMapping().GetNodes();
  double nVolume_mL = 0;
  for (SEFluidCircuitNode* n : vNodes) {
    if (n->HasNextVolume())
      nVolume_mL += n->GetNextVolume().GetValue(VolumeUnit::mL);
  }
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

  // We can do pressure in two ways
  // We look for nodes with volume and pressure
  // -- if multiple nodes are found we volume weight the pressure sum
  // If no nodes have volume and pressure, we look for nodes with pressure
  // -- if multiple nodes are found we average the pressure
  bool volumeWeightedPressure = false;
  bool averagePressure = false;
  int pressureNodes = 0;
  for (SEFluidCircuitNode* n : cmpt.GetNodeMapping().GetNodes()) {
    if (n->HasNextPotential()) {
      averagePressure = true;
      if (n->HasNextQuantity())
        volumeWeightedPressure = true;
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

void CommonDataModelTest::TestLiquidSubstanceQuantity(SETestCase& testCase, SELiquidCompartment& cmpt, SELiquidSubstanceQuantity& subQ, double totalMass_mg, double totalVolume_mL)
{
  if (!subQ.HasMass())
    testCase.AddFailure( std::string{"Mass was not set on "} + cmpt.GetName());
  if (GeneralMath::PercentTolerance(subQ.GetMass(MassUnit::mg), totalMass_mg) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const Mass is not correct : " << subQ.GetMass(MassUnit::mg) << " expected " << totalMass_mg;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetMass().GetValue(MassUnit::mg), totalMass_mg) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 Mass is not correct : " << subQ.GetMass(MassUnit::mg) << " expected " << totalMass_mg;
    testCase.AddFailure(m_ss);
  }
  if (!subQ.HasConcentration())
    testCase.AddFailure( std::string{"Concentration was not set on  "} + cmpt.GetName());
  double mg_Per_mL = totalMass_mg / totalVolume_mL;
  if (GeneralMath::PercentTolerance(subQ.GetConcentration(MassPerVolumeUnit::mg_Per_mL), mg_Per_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const Concentration is not correct : " << subQ.GetConcentration(MassPerVolumeUnit::mg_Per_mL) << " expected " << mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetConcentration().GetValue(MassPerVolumeUnit::mg_Per_mL), mg_Per_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 Concentration is not correct : " << subQ.GetConcentration(MassPerVolumeUnit::mg_Per_mL) << " expected " << mg_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!subQ.HasMolarity())
    testCase.AddFailure( std::string{"Molarity was not set on  "} + cmpt.GetName());
  double mmol_Per_mL = mg_Per_mL / subQ.GetSubstance().GetMolarMass(MassPerAmountUnit::mg_Per_mmol);
  if (GeneralMath::PercentTolerance(subQ.GetMolarity(AmountPerVolumeUnit::mmol_Per_mL), mmol_Per_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const Molarity is not correct : " << subQ.GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetMolarity().GetValue(AmountPerVolumeUnit::mmol_Per_mL), mmol_Per_mL) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 Molarity is not correct : " << subQ.GetMolarity(AmountPerVolumeUnit::mmol_Per_mL) << " expected " << mmol_Per_mL;
    testCase.AddFailure(m_ss);
  }
  if (!subQ.HasPartialPressure())
    testCase.AddFailure( std::string{"PartialPressure was not set on  "} + cmpt.GetName());
  SEScalarPressure partialPressure;
  GeneralMath::CalculatePartialPressureInLiquid(subQ.GetSubstance(), subQ.GetConcentration(), partialPressure);
  if (GeneralMath::PercentTolerance(subQ.GetPartialPressure(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 const PartialPressure is not correct : " << subQ.GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(subQ.GetPartialPressure().GetValue(PressureUnit::mmHg), partialPressure.GetValue(PressureUnit::mmHg)) > m_PercentTolerance) {
    m_ss << cmpt.GetName() << " N2 PartialPressure is not correct : " << subQ.GetPartialPressure(PressureUnit::mmHg) << " expected " << partialPressure;
    testCase.AddFailure(m_ss);
  }
}

void CommonDataModelTest::TestUpdateLiquidLinks(SETestSuite& testSuite, SESubstanceManager& subMgr)
{
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("LiquidLinks");

  TimingProfile pTimer;
  pTimer.Start("Test");

  SECompartmentManager cmptMgr(subMgr);

  SELiquidCompartment& venaCava = cmptMgr.CreateLiquidCompartment("Vena Cava");
  SELiquidCompartment& rightHeart = cmptMgr.CreateLiquidCompartment("Right Heart");
  SELiquidCompartment& IVbag = cmptMgr.CreateLiquidCompartment("IV");
  SELiquidCompartment& veins = cmptMgr.CreateLiquidCompartment("Peripheral Veins");
  //SELiquidCompartment& hemorrhage = cmptMgr.CreateLiquidCompartment("Hemorrhage");

  SELiquidCompartmentLink& venaCava2rightHeart = cmptMgr.CreateLiquidLink(venaCava, rightHeart, "VenaCavaToRightHeart");
  SELiquidCompartmentLink& IV2venaCava = cmptMgr.CreateLiquidLink(IVbag, venaCava, "IVToVenaCava");
  SELiquidCompartmentLink& veins2venaCava = cmptMgr.CreateLiquidLink(veins, venaCava, "PeripheralVeinsToVenaCava");
  //SELiquidCompartmentLink& venaCava2hemorrhage = cmptMgr.CreateLiquidLink(venaCava, hemorrhage, "VenaCavaToHemorrhage");

  venaCava2rightHeart.GetFlow().SetValue(40, VolumePerTimeUnit::mL_Per_s);
  //venaCava2hemorrhage.GetFlow().SetValue(20, VolumePerTimeUnit::mL_Per_s);
  IV2venaCava.GetFlow().SetValue(20, VolumePerTimeUnit::mL_Per_s);
  veins2venaCava.GetFlow().SetValue(200, VolumePerTimeUnit::mL_Per_s);
  cmptMgr.StateChange();

  double inflow = venaCava.GetInFlow(VolumePerTimeUnit::mL_Per_s);
  double inflowScalar = venaCava.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s);

  if (GeneralMath::PercentTolerance(inflow, (IV2venaCava.GetFlow(VolumePerTimeUnit::mL_Per_s) + veins2venaCava.GetFlow(VolumePerTimeUnit::mL_Per_s))) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Initial Vena Cava inflow is not the correct sum"});

  if (GeneralMath::PercentTolerance(inflow, inflowScalar) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Inflow retrieved using GetInFlow() and GetValue() are not equal in the initial test"});

  // Now make a graph that doesn't have the IV connected to the Vena Cava
  SELiquidCompartmentGraph& normalGraph = cmptMgr.CreateLiquidGraph("NormalGraph");
  normalGraph.AddCompartment(venaCava);
  normalGraph.AddCompartment(rightHeart);
  normalGraph.AddCompartment(IVbag);
  normalGraph.AddCompartment(veins);
  normalGraph.AddLink(venaCava2rightHeart);
  normalGraph.AddLink(veins2venaCava);
  cmptMgr.UpdateLinks(normalGraph);

  inflow = venaCava.GetInFlow(VolumePerTimeUnit::mL_Per_s);
  inflowScalar = venaCava.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s);

  if (GeneralMath::PercentTolerance(inflow, veins2venaCava.GetFlow(VolumePerTimeUnit::mL_Per_s)) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Vena Cava inflow is not the veinous flow after first graph update"});

  if (GeneralMath::PercentTolerance(inflow, inflowScalar) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Inflow retrieved using GetInFlow() and GetValue() are not equal after the first graph update"});

  // Now make a graph that connects the IV bag to the vena cava
  SELiquidCompartmentGraph& ivGraph = cmptMgr.CreateLiquidGraph("IVGraph");
  ivGraph.AddCompartment(venaCava);
  ivGraph.AddCompartment(rightHeart);
  ivGraph.AddCompartment(IVbag);
  ivGraph.AddCompartment(veins);
  ivGraph.AddLink(venaCava2rightHeart);
  ivGraph.AddLink(IV2venaCava);
  ivGraph.AddLink(veins2venaCava);
  cmptMgr.UpdateLinks(ivGraph);

  inflow = venaCava.GetInFlow(VolumePerTimeUnit::mL_Per_s);
  inflowScalar = venaCava.GetInFlow().GetValue(VolumePerTimeUnit::mL_Per_s);

  if (GeneralMath::PercentTolerance(inflow, (IV2venaCava.GetFlow(VolumePerTimeUnit::mL_Per_s) + veins2venaCava.GetFlow(VolumePerTimeUnit::mL_Per_s))) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Vena Cava inflow is not the sum of IV and veinous flow after second graph update"});

  if (GeneralMath::PercentTolerance(inflow, inflowScalar) > m_PercentTolerance)
    testCase.AddFailure( std::string{"Inflow retrieved using GetInFlow() and GetValue() are not equal after the second graph update"});

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Test"), TimeUnit::s);
}

void CommonDataModelTest::LiquidCompartmentTest(const std::string& rptDirectory)
{
  m_PercentTolerance = 2.0;
  m_OutDirectory = rptDirectory;

  m_Logger->ResetLogFile(rptDirectory + "/LiquidCompartmentTest.log");
  SETestReport testReport(m_Logger);

  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();

  SETestSuite& Flows = testReport.CreateTestSuite();
  Flows.SetName("LiquidCompartmentFlows");
  TestLiquidFlows(Flows, subMgr);

  SETestSuite& HierarchyFlows = testReport.CreateTestSuite();
  HierarchyFlows.SetName("LiquidCompartmentHierarchyFlowsF");
  TestLiquidHierarchyFlows(HierarchyFlows, subMgr);

  SETestSuite& Hierarchy = testReport.CreateTestSuite();
  Hierarchy.SetName("LiquidCompartmentHierarchy");
  TestLiquidHierarchy(Hierarchy, subMgr);

  SETestSuite& HierarchySaturation = testReport.CreateTestSuite();
  HierarchySaturation.SetName("LiquidCompartmentHierarchySaturation");
  TestLiquidHierarchySaturation(HierarchySaturation, subMgr);

  SETestSuite& CircuitVolumesPressuresAndFlows = testReport.CreateTestSuite();
  CircuitVolumesPressuresAndFlows.SetName("LiquidCompartmentVolumesPressuresAndFlows");
  TestLiquidCircuitVolumesPressuresAndFlows(CircuitVolumesPressuresAndFlows, subMgr);

  SETestSuite& UpdateLiquidLinks = testReport.CreateTestSuite();
  UpdateLiquidLinks.SetName("UpdateLiquidLinks");
  TestUpdateLiquidLinks(UpdateLiquidLinks, subMgr);

  testReport.WriteFile(rptDirectory + "/LiquidCompartmentTestReport.xml");
}
}