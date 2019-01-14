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
  biogears::SEScalarMassPerAmount MassPerAmount = biogears::SEScalarMassPerAmount();
  auto ptr = MassPerAmount.Unload();
  EXPECT_EQ(ptr, nullptr);
}

TEST_F(TEST_FIXTURE_NAME, IsValidUnit)
{
  bool unit0  = biogears::MassPerAmountUnit::IsValidUnit("g/ct");
  bool unit1  = biogears::MassPerAmountUnit::IsValidUnit("g/mol");
  bool unit2  = biogears::MassPerAmountUnit::IsValidUnit("g/mmol");
  bool unit3  = biogears::MassPerAmountUnit::IsValidUnit("g/umol");
  bool unit4  = biogears::MassPerAmountUnit::IsValidUnit("mg/mol");
  bool unit5  = biogears::MassPerAmountUnit::IsValidUnit("mg/mmol");
  bool unit6  = biogears::MassPerAmountUnit::IsValidUnit("kg/mol");
  bool unit7  = biogears::MassPerAmountUnit::IsValidUnit("ug/mmol");
  bool unit8  = biogears::MassPerAmountUnit::IsValidUnit("ug/mol");
  bool unit9  = biogears::MassPerAmountUnit::IsValidUnit("ug/ct");
  bool unit10 = biogears::MassPerAmountUnit::IsValidUnit("pg/ct");
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
  bool unit11 = biogears::MassPerAmountUnit::IsValidUnit("DEADBEEF");
  EXPECT_EQ(unit11, false);
}

TEST_F(TEST_FIXTURE_NAME, GetCompoundUnit)
{
  biogears::MassPerAmountUnit mu0  = biogears::MassPerAmountUnit::GetCompoundUnit("g/ct");
  biogears::MassPerAmountUnit mu1  = biogears::MassPerAmountUnit::GetCompoundUnit("g/mol");
  biogears::MassPerAmountUnit mu2  = biogears::MassPerAmountUnit::GetCompoundUnit("g/mmol");
  biogears::MassPerAmountUnit mu3  = biogears::MassPerAmountUnit::GetCompoundUnit("g/umol");
  biogears::MassPerAmountUnit mu4  = biogears::MassPerAmountUnit::GetCompoundUnit("mg/mol");
  biogears::MassPerAmountUnit mu5  = biogears::MassPerAmountUnit::GetCompoundUnit("mg/mmol");
  biogears::MassPerAmountUnit mu6  = biogears::MassPerAmountUnit::GetCompoundUnit("kg/mol");
  biogears::MassPerAmountUnit mu7  = biogears::MassPerAmountUnit::GetCompoundUnit("ug/mmol");
  biogears::MassPerAmountUnit mu8  = biogears::MassPerAmountUnit::GetCompoundUnit("ug/mol");
  biogears::MassPerAmountUnit mu9  = biogears::MassPerAmountUnit::GetCompoundUnit("ug/ct");
  biogears::MassPerAmountUnit mu10 = biogears::MassPerAmountUnit::GetCompoundUnit("pg/ct");
  EXPECT_EQ(mu0, biogears::MassPerAmountUnit::g_Per_ct);
  EXPECT_EQ(mu1, biogears::MassPerAmountUnit::g_Per_mol);
  EXPECT_EQ(mu2, biogears::MassPerAmountUnit::g_Per_mmol);
  EXPECT_EQ(mu3, biogears::MassPerAmountUnit::g_Per_umol);
  EXPECT_EQ(mu4, biogears::MassPerAmountUnit::mg_Per_mol);
  EXPECT_EQ(mu5, biogears::MassPerAmountUnit::mg_Per_mmol);
  EXPECT_EQ(mu6, biogears::MassPerAmountUnit::kg_Per_mol);
  EXPECT_EQ(mu7, biogears::MassPerAmountUnit::ug_Per_mmol);
  EXPECT_EQ(mu8, biogears::MassPerAmountUnit::ug_Per_mol);
  EXPECT_EQ(mu9, biogears::MassPerAmountUnit::ug_Per_ct);
  EXPECT_EQ(mu10, biogears::MassPerAmountUnit::pg_Per_ct);
  EXPECT_THROW(biogears::MassPerAmountUnit::GetCompoundUnit("DEADBEEF"),biogears::CommonDataModelException);
}