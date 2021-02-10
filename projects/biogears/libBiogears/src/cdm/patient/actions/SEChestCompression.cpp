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
#include <biogears/cdm/patient/actions/SEChestCompression.h>

namespace biogears {
SEChestCompression::SEChestCompression()
  : SEPatientAction()
{
}

SEChestCompression::~SEChestCompression()
{
  Clear();
}

void SEChestCompression::Clear()
{
  SEPatientAction::Clear();
}

bool SEChestCompression::IsValid() const
{
  return SEPatientAction::IsValid();
}

bool SEChestCompression::IsActive() const
{
  return IsValid();
}

bool SEChestCompression::Load(const CDM::ChestCompressionData& in)
{
  SEPatientAction::Load(in);
  return true;
}

CDM::ChestCompressionData* SEChestCompression::Unload() const
{
  CDM::ChestCompressionData* data(new CDM::ChestCompressionData());
  Unload(*data);
  return data;
}

void SEChestCompression::Unload(CDM::ChestCompressionData& data) const
{
  SEPatientAction::Unload(data);
}

void SEChestCompression::ToString(std::ostream& str) const
{
  str << "Patient Action : Chest Compression";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\nIsActive: "  << ((IsActive()) ? "True" : "False");
  str << std::flush;
}

}