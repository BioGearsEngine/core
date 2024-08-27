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
#include <biogears/cdm/scenario/SECondition.h>

#include "io/cdm/Conditions.h"

#include <biogears/cdm/patient/conditions/SEChronicAnemia.h>
#include <biogears/cdm/patient/conditions/SEChronicHeartFailure.h>
#include <biogears/cdm/patient/conditions/SEChronicObstructivePulmonaryDisease.h>
#include <biogears/cdm/patient/conditions/SEChronicPericardialEffusion.h>
#include <biogears/cdm/patient/conditions/SEChronicRenalStenosis.h>
#include <biogears/cdm/patient/conditions/SEChronicVentricularSystolicDysfunction.h>
#include <biogears/cdm/patient/conditions/SEDehydration.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType1.h>
#include <biogears/cdm/patient/conditions/SEDiabetesType2.h>
#include <biogears/cdm/patient/conditions/SEImpairedAlveolarExchange.h>
#include <biogears/cdm/patient/conditions/SELobarPneumonia.h>
#include <biogears/cdm/patient/conditions/SEStarvation.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

namespace std {
template class vector<biogears::SECondition*>;
}

namespace biogears {
SECondition::SECondition()
  : Loggable()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
SECondition::~SECondition()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECondition::Clear()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
SECondition* SECondition::newFromBind(const CDM::ConditionData& data, SESubstanceManager& substances)
{
  // Could speed up case by testing Patient Conditions vs another type
  // But we only have 1 type at this time, and a few conditions to support
  
  if (const CDM::ChronicAnemiaData* ccAnemiaData = dynamic_cast<const CDM::ChronicAnemiaData*>(&data)) {
    SEChronicAnemia* cc = new SEChronicAnemia();
    io::Conditions::UnMarshall(*ccAnemiaData, *cc);
    return cc;
  }
  
  if (const CDM::ChronicObstructivePulmonaryDiseaseData* ccopdData = dynamic_cast<const CDM::ChronicObstructivePulmonaryDiseaseData*>(&data)) {
    SEChronicObstructivePulmonaryDisease* cc = new SEChronicObstructivePulmonaryDisease();
    io::Conditions::UnMarshall(*ccopdData, *cc);
    return cc;
  }
  
  if (const CDM::ChronicVentricularSystolicDysfunctionData* ccVentSysDysfuncData = dynamic_cast<const CDM::ChronicVentricularSystolicDysfunctionData*>(&data)) {
    SEChronicVentricularSystolicDysfunction* cc = new SEChronicVentricularSystolicDysfunction();
    io::Conditions::UnMarshall(*ccVentSysDysfuncData, *cc);
    return cc;
  }
  
  if (const CDM::ChronicPericardialEffusionData* ccPericardialEffusionData = dynamic_cast<const CDM::ChronicPericardialEffusionData*>(&data)) {
    SEChronicPericardialEffusion* cc = new SEChronicPericardialEffusion();
    io::Conditions::UnMarshall(*ccPericardialEffusionData, *cc);
    return cc;
  }
  
  if (const CDM::ChronicRenalStenosisData* ccRenalStenosisData = dynamic_cast<const CDM::ChronicRenalStenosisData*>(&data)) {
    SEChronicRenalStenosis* cc = new SEChronicRenalStenosis();
    io::Conditions::UnMarshall(*ccRenalStenosisData, *cc);
    return cc;
  }
  
  if (const CDM::DehydrationData* ccDehydrationData = dynamic_cast<const CDM::DehydrationData*>(&data)) {
    SEDehydration* cc = new SEDehydration();
    io::Conditions::UnMarshall(*ccDehydrationData, *cc);
    return cc;
  }
  
  if (const CDM::DiabetesType1Data* ccDiabetesType1Data = dynamic_cast<const CDM::DiabetesType1Data*>(&data)) {
    SEDiabetesType1* cc = new SEDiabetesType1();
    io::Conditions::UnMarshall(*ccDiabetesType1Data, *cc);
    return cc;
  }
  
  if (const CDM::DiabetesType2Data* ccDiabetesType2Data = dynamic_cast<const CDM::DiabetesType2Data*>(&data)) {
    SEDiabetesType2* cc = new SEDiabetesType2();
    io::Conditions::UnMarshall(*ccDiabetesType2Data, *cc);
    return cc;
  }
  
  if (const CDM::StarvationData* ccStarvationData = dynamic_cast<const CDM::StarvationData*>(&data)) {
    SEStarvation* cc = new SEStarvation();
    io::Conditions::UnMarshall(*ccStarvationData, *cc);
    return cc;
  }
  
  if (const CDM::ImpairedAlveolarExchangeData* ccImpairedAlveolarExchangeData = dynamic_cast<const CDM::ImpairedAlveolarExchangeData*>(&data)) {
    SEImpairedAlveolarExchange* cc = new SEImpairedAlveolarExchange();
    io::Conditions::UnMarshall(*ccImpairedAlveolarExchangeData, *cc);
    return cc;
  }
  
  if (const CDM::LobarPneumoniaData* ccLobarPneumoniaData = dynamic_cast<const CDM::LobarPneumoniaData*>(&data)) {
    SELobarPneumonia* cc = new SELobarPneumonia();
    io::Conditions::UnMarshall(*ccLobarPneumoniaData, *cc);
    return cc;
  }
  
  if (const CDM::InitialEnvironmentData* ccInitialEnvironmentData = dynamic_cast<const CDM::InitialEnvironmentData*>(&data)) {
    SEInitialEnvironment* cc = new SEInitialEnvironment(substances);
    io::Conditions::UnMarshall(*ccInitialEnvironmentData, *cc);
    return cc;
  }

  if (substances.GetLogger() != nullptr)
    substances.GetLogger()->Error("Unsupported Condition Received", "SECondition::newFromBind");
  return nullptr;
}
//-------------------------------------------------------------------------------
const char* SECondition::GetComment_cStr() const
{
  return m_Comment.c_str();
}
  //-------------------------------------------------------------------------------
std::string SECondition::GetComment() const
{
  return m_Comment;
}
//-------------------------------------------------------------------------------
void SECondition::SetComment(const char* comment)
{
  m_Comment = comment;
}
//-------------------------------------------------------------------------------
void SECondition::SetComment(const std::string& comment)
{
  m_Comment = comment;
}
//-------------------------------------------------------------------------------
bool SECondition::HasComment() const
{
  return m_Comment.empty() ? false : true;
}
//-------------------------------------------------------------------------------
void SECondition::InvalidateComment()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
}