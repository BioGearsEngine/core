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

#include <biogears/cdm/patient/actions/SEConsciousRespirationCommand.h>

namespace biogears {
SEConsciousRespirationCommand::SEConsciousRespirationCommand()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
SEConsciousRespirationCommand::~SEConsciousRespirationCommand()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEConsciousRespirationCommand::Clear()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
bool SEConsciousRespirationCommand::Load(const CDM::ConsciousRespirationCommandData& in)
{
  if (in.Comment().present())
    m_Comment = in.Comment().get();
  return true;
}
//-------------------------------------------------------------------------------
void SEConsciousRespirationCommand::Unload(CDM::ConsciousRespirationCommandData& data) const
{
  if (HasComment())
    data.Comment(m_Comment);
}
//-------------------------------------------------------------------------------
bool SEConsciousRespirationCommand::IsValid() const
{
  return true;
}
//-------------------------------------------------------------------------------
bool SEConsciousRespirationCommand::IsActive() const
{
  return true;
}
//-------------------------------------------------------------------------------
std::string SEConsciousRespirationCommand::GetComment() const
{
  return m_Comment;
}
  //-------------------------------------------------------------------------------
const char* SEConsciousRespirationCommand::GetComment_cStr() const
{
  return m_Comment.c_str();
}
//-------------------------------------------------------------------------------
void SEConsciousRespirationCommand::SetComment(const char* comment)
{
  m_Comment = comment;
}
//-------------------------------------------------------------------------------
void SEConsciousRespirationCommand::SetComment(const std::string& comment)
{
  m_Comment = comment;
}
//-------------------------------------------------------------------------------
bool SEConsciousRespirationCommand::HasComment() const
{
  return m_Comment.empty() ? false : true;
}
//-------------------------------------------------------------------------------
void SEConsciousRespirationCommand::InvalidateComment()
{
  m_Comment = "";
}
//-------------------------------------------------------------------------------
}