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
#include <biogears/cdm/patient/actions/SEChestCompressionForce.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalarForce.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEChestCompressionForce::SEChestCompressionForce()
  : SEChestCompression()
{
  m_Force = nullptr;
}
//-------------------------------------------------------------------------------
SEChestCompressionForce::~SEChestCompressionForce()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEChestCompressionForce::Clear()
{
  SEChestCompression::Clear();
  SAFE_DELETE(m_Force);
}
//-------------------------------------------------------------------------------
bool SEChestCompressionForce::IsValid() const
{
  return SEChestCompression::IsValid() && HasForce();
}
//-------------------------------------------------------------------------------
bool SEChestCompressionForce::IsActive() const
{
  return IsValid() ? !m_Force->IsZero() : false;
}
//-------------------------------------------------------------------------------
bool SEChestCompressionForce::HasForce() const
{
  return m_Force == nullptr ? false : m_Force->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarForce& SEChestCompressionForce::GetForce()
{
  if (m_Force == nullptr)
    m_Force = new SEScalarForce();
  return *m_Force;
}
//-------------------------------------------------------------------------------
void SEChestCompressionForce::ToString(std::ostream& str) const
{
  str << "Patient Action : Chest Compression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tForce: ";
  HasForce() ? str << *m_Force : str << "NaN";
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEChestCompressionForce::operator==( const SEChestCompressionForce& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_Force && rhs.m_Force) ? m_Force->operator==(*rhs.m_Force) : m_Force == rhs.m_Force;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEChestCompressionForce::operator!=( const SEChestCompressionForce& rhs) const
{
  return !(*this == rhs);
}
}