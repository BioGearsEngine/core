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
//! @author
//! @date   2021/03/22
//!
//! Unit Test for BioGears Seralization
//!
#include <thread>

#include <gtest/gtest.h>

#include <biogears/cdm/properties/SEProperties.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/utils/Logger.h>

#include <io/cdm/ElectroCardioGram.h>
#include <io/cdm/Property.h>

#ifdef DISABLE_BIOGEARS_ElectroCadioGram_TEST
#define TEST_FIXTURE_NAME DISABLED_ElectroCadioGramFixture
#else
#define TEST_FIXTURE_NAME IO_ElectroCadioGramFixture
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
  virtual void SetUp();

  // Code here will be called immediately after each test (right
  // before the destructor).
  virtual void TearDown();
};

void TEST_FIXTURE_NAME::SetUp()
{
}

void TEST_FIXTURE_NAME::TearDown()
{
}

#define USING_TYPES(name)             \
  using namespace biogears::io;       \
  using SEType = biogears ::SE##name; \
  using CDMType = CDM ::name##Data;

#include <biogears/cdm/substance/SESubstanceManager.h>

// class SEElectroCardioGram;
//!
//! TYPE ElectroCardioGram
//! static void Marshall(const CDM::ElectroCardioGram& in, SEElectroCardioGram& out);
//! static void UnMarshall(const SEElectroCardioGram& in, CDM::ElectroCardioGram& out);
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>
TEST_F(TEST_FIXTURE_NAME, ElectroCardioGram)
{
  USING_TYPES(ElectroCardioGram)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.GetLead1ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead2ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead3ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead4ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead5ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead6ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead7ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead8ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead9ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead10ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead11ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);
  source.GetLead12ElectricPotential().SetValue(1.0, biogears::ElectricPotentialUnit::mV);

  EXPECT_NE(source, sink);

  ElectroCardioGram::Marshall(source, data);
  ElectroCardioGram::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}


// class SEElectroCardioGramInterpolationWaveform;
//!
//! TYPE ElectroCardioGramInterpolationWaveform
//! static void Marshall(const CDM::ElectroCardioGramInterpolationWaveform& in, SEElectroCardioGramInterpolationWaveform& out);
//! static void UnMarshall(const SEElectroCardioGramInterpolationWaveform& in, CDM::ElectroCardioGramInterpolationWaveform& out);
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolationWaveform.h>
TEST_F(TEST_FIXTURE_NAME, ElectroCardioGramInterpolationWaveform)
{
  USING_TYPES(ElectroCardioGramInterpolationWaveform)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.SetLeadNumber(2);
  source.SetRhythm(CDM::enumHeartRhythm::NormalSinus);

  source.GetTimeStep().SetValue(1.0, biogears::TimeUnit::yr);
  auto& epVt = source.GetData();

  epVt.GetTime().push_back(1.0);
  epVt.GetTime().push_back(2.0);
  epVt.GetTime().push_back(3.0);
  epVt.GetTime().push_back(4.0);

  epVt.SetTimeUnit(biogears::TimeUnit::hr);

  epVt.GetElectricPotential().push_back(2);
  epVt.GetElectricPotential().push_back(3);
  epVt.GetElectricPotential().push_back(4);
  epVt.GetElectricPotential().push_back(5);

  epVt.SetElectricPotentialUnit(biogears::ElectricPotentialUnit::mV);

  source.GetActiveIndicies().push_back(4);
  source.GetActiveIndicies().push_back(2);

  EXPECT_NE(source, sink);

  ElectroCardioGram::Marshall(source, data);
  ElectroCardioGram::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}


// class SEElectroCardioGramInterpolator;
//!
//! TYPE ElectroCardioGramInterpolator
//! static void Marshall(const CDM::ElectroCardioGramInterpolator& in, SEElectroCardioGramInterpolator& out);
//! static void UnMarshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramInterpolator& out);
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramWaveformLeadNumber.h>
TEST_F(TEST_FIXTURE_NAME, ElectroCardioGramWaveformLeadNumber)
{
  USING_TYPES(ElectroCardioGramWaveformLeadNumber)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  source.SetValue(10);

  EXPECT_NE(source, sink);

  ElectroCardioGram::Marshall(source, data);
  ElectroCardioGram::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}

// class SEElectroCardioGramInterpolator;
//!
//! TYPE ElectroCardioGramInterpolator
//! static void Marshall(const CDM::ElectroCardioGramInterpolator& in, SEElectroCardioGramInterpolator& out);
//! static void UnMarshall(const SEElectroCardioGramInterpolator& in, CDM::ElectroCardioGramInterpolator& out);
#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramInterpolator.h>
TEST_F(TEST_FIXTURE_NAME, ElectroCardioGramInterpolator)
{
  USING_TYPES(ElectroCardioGramInterpolator)

  biogears::Logger logger;
  biogears::SESubstanceManager subMgr { &logger };
  ASSERT_TRUE(subMgr.LoadSubstanceDirectory());
  SEType source { &logger }, sink { &logger };
  CDMType data;

  auto potential = biogears::SEScalarElectricPotential { 10, biogears::ElectricPotentialUnit::mV };
  source.SetLeadElectricPotential(1, potential);
  auto& waveform = source.GetWaveform(10, CDM::enumHeartRhythm::Asystole);

  //LeadNumber an Rythem set by GetWaveForm of Coherence.
  //waveform.SetLeadNumber(10) 
  //waveform.SetRhythm(CDM::enumHeartRhythm::NormalSinus);

  waveform.GetTimeStep().SetValue(1.0, biogears::TimeUnit::yr);
  auto& epVt = waveform.GetData();

  epVt.GetTime().push_back(1.0);
  epVt.GetTime().push_back(2.0);
  epVt.GetTime().push_back(3.0);
  epVt.GetTime().push_back(4.0);

  epVt.SetTimeUnit(biogears::TimeUnit::hr);

  epVt.GetElectricPotential().push_back(2);
  epVt.GetElectricPotential().push_back(3);
  epVt.GetElectricPotential().push_back(4);
  epVt.GetElectricPotential().push_back(5);

  epVt.SetElectricPotentialUnit(biogears::ElectricPotentialUnit::mV);

  waveform.GetActiveIndicies().push_back(4);
  waveform.GetActiveIndicies().push_back(2);

  EXPECT_NE(source, sink);

  ElectroCardioGram::Marshall(source, data);
  ElectroCardioGram::UnMarshall(data, sink);

  EXPECT_EQ(source, sink);
}