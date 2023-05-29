//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------
//!
//! @author David Lee
//! @date   2017 Aug 3rd
//!
//! Unit Test for Biogears-common Config
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEScalarMassPerAmount.h>


#ifdef DISABLE_BIOGEARS_SEScalarMassPerAmount_TEST
#define TEST_FIXTURE_NAME DISABLED_SEScalarMassPerAmount_Fixture
#else
#define TEST_FIXTURE_NAME SEScalarMassPerAmount_Fixture
#endif

// The fixture for testing class Foo.
class TEST_FIXTURE_NAME : public ::testing::Test {
protected:
  // You can do set-up work for each test here.
  TEST_FIXTURE_NAME() = default;

  // You can do clean-up work that doesn't throw exceptions here.
  virtual ~TEST_FIXTURE_NAME() = default;

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  // Code here will be called immediately after the constructor (right
  // before each test).
  virtual void SetUp() override;

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown() override;
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

TEST_F(TEST_FIXTURE_NAME, Unload)
{
  BIOGEARS_NAMESPACE SEScalarMassPerAmount MassPerAmount = BIOGEARS_NAMESPACE SEScalarMassPerAmount();
  auto ptr = MassPerAmount.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("g/ct");
  bool unit1  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("g/mol");
  bool unit2  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("g/mmol");
  bool unit3  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("g/umol");
  bool unit4  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("mg/mol");
  bool unit5  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("mg/mmol");
  bool unit6  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("kg/mol");
  bool unit7  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("ug/mmol");
  bool unit8  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("ug/mol");
  bool unit9  = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("ug/ct");
  bool unit10 = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("pg/ct");
  EXPECT_EQ(unit0, true);
  EXPECT_EQ(unit1, true);
  EXPECT_EQ(unit2, true);
  EXPECT_EQ(unit3, true);
  EXPECT_EQ(unit4, true);
  EXPECT_EQ(unit5, true);
  EXPECT_EQ(unit6, true);
  EXPECT_EQ(unit7, true);
  EXPECT_EQ(unit8, true);
  EXPECT_EQ(unit9, true);
  EXPECT_EQ(unit10, true);
  bool unit11 = BIOGEARS_NAMESPACE MassPerAmountUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit11, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  BIOGEARS_NAMESPACE MassPerAmountUnit mu0  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("g/ct");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu1  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("g/mol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu2  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("g/mmol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu3  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("g/umol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu4  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("mg/mol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu5  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("mg/mmol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu6  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("kg/mol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu7  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("ug/mmol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu8  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("ug/mol");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu9  = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("ug/ct");
  BIOGEARS_NAMESPACE MassPerAmountUnit mu10 = BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("pg/ct");
  EXPECT_EQ(mu0, BIOGEARS_NAMESPACE MassPerAmountUnit::g_Per_ct);
  EXPECT_EQ(mu1, BIOGEARS_NAMESPACE MassPerAmountUnit::g_Per_mol);
  EXPECT_EQ(mu2, BIOGEARS_NAMESPACE MassPerAmountUnit::g_Per_mmol);
  EXPECT_EQ(mu3, BIOGEARS_NAMESPACE MassPerAmountUnit::g_Per_umol);
  EXPECT_EQ(mu4, BIOGEARS_NAMESPACE MassPerAmountUnit::mg_Per_mol);
  EXPECT_EQ(mu5, BIOGEARS_NAMESPACE MassPerAmountUnit::mg_Per_mmol);
  EXPECT_EQ(mu6, BIOGEARS_NAMESPACE MassPerAmountUnit::kg_Per_mol);
  EXPECT_EQ(mu7, BIOGEARS_NAMESPACE MassPerAmountUnit::ug_Per_mmol);
  EXPECT_EQ(mu8, BIOGEARS_NAMESPACE MassPerAmountUnit::ug_Per_mol);
  EXPECT_EQ(mu9, BIOGEARS_NAMESPACE MassPerAmountUnit::ug_Per_ct);
  EXPECT_EQ(mu10, BIOGEARS_NAMESPACE MassPerAmountUnit::pg_Per_ct);
  EXPECT_THROW(BIOGEARS_NAMESPACE MassPerAmountUnit::GetCompoundUnit("DEADBEEF"),BIOGEARS_NAMESPACE CommonDataModelException);
}