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

#pragma once
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEConsciousRespirationCommand {
  friend io::PatientActions;

public:
  SEConsciousRespirationCommand();
  virtual ~SEConsciousRespirationCommand();

  virtual void Clear(); //clear memory

  virtual bool IsValid() const;
  virtual bool IsActive() const;

  virtual bool Load(const CDM::ConsciousRespirationCommandData& in);
  virtual CDM::ConsciousRespirationCommandData* Unload() const = 0;

protected:
  virtual void Unload(CDM::ConsciousRespirationCommandData& data) const;

public:
  virtual std::string GetComment() const;
  virtual const char* GetComment_cStr() const;
  virtual void SetComment(const char* comment);
  virtual void SetComment(const std::string& comment);
  virtual bool HasComment() const;
  virtual void InvalidateComment();

  virtual void ToString(std::ostream& str) const = 0;

protected:
  std::string m_Comment;
};

inline std::ostream& operator<<(std::ostream& out, const SEConsciousRespirationCommand& c)
{
  c.ToString(out);
  return out;
}
}
