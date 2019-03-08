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

#include <biogears/cdm/patient/actions/SENeedleDecompression.h>

namespace biogears {
SENeedleDecompression::SENeedleDecompression()
  : SEPatientAction()
{
  m_State = CDM::enumOnOff::Off;
  m_Side = (CDM::enumSide::value)-1;
}

SENeedleDecompression::~SENeedleDecompression()
{
  Clear();
}

void SENeedleDecompression::Clear()
{
  SEPatientAction::Clear();
  m_State = CDM::enumOnOff::Off;
  m_Side = (CDM::enumSide::value)-1;
}

bool SENeedleDecompression::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SENeedleDecompression::IsActive() const
{
  return IsValid() && m_State == CDM::enumOnOff::On;
}

void SENeedleDecompression::SetActive(bool b)
{
  m_State = b ? CDM::enumOnOff::On : CDM::enumOnOff::Off;
}

bool SENeedleDecompression::Load(const CDM::NeedleDecompressionData& in)
{
  SEPatientAction::Load(in);
  m_Side = in.Side();
  m_State = in.State();
  return true;
}

CDM::NeedleDecompressionData* SENeedleDecompression::Unload() const
{
  CDM::NeedleDecompressionData* data(new CDM::NeedleDecompressionData());
  Unload(*data);
  return data;
}

void SENeedleDecompression::Unload(CDM::NeedleDecompressionData& data) const
{
  SEPatientAction::Unload(data);
  data.State(m_State);
  if (HasSide())
    data.Side(m_Side);
}

CDM::enumSide::value SENeedleDecompression::GetSide() const
{
  return m_Side;
}
void SENeedleDecompression::SetSide(CDM::enumSide::value Side)
{
  m_Side = Side;
}
bool SENeedleDecompression::HasSide() const
{
  return m_Side == ((CDM::enumSide::value)-1) ? false : true;
}
void SENeedleDecompression::InvalidateSide()
{
  m_Side = (CDM::enumSide::value)-1;
}

void SENeedleDecompression::ToString(std::ostream& str) const
{
  str << "Patient Action : Needle Decompression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tState: " << IsActive();
  str << "\n\tSide: ";
  HasSide() ? str << GetSide() : str << "No Side Set";
  str << std::flush;
}
}