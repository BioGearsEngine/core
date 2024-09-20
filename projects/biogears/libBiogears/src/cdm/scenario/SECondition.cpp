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