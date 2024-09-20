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
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

namespace biogears {
class SESubstanceManager;
namespace io {
  class Conditions;
}
} // namespace biogears

namespace biogears {
class BIOGEARS_API SECondition : public Loggable {
  friend io::Conditions;

public:
  SECondition();
  virtual ~SECondition();

  virtual void Clear(); // Deletes all members

  virtual bool operator==(SECondition const&) const = 0;
  virtual bool operator!=(SECondition const&) const = 0;

public:
  /** Test if the action has all data it needs */
  virtual bool IsValid() const { return true; }

  virtual std::string GetName() const = 0;
  virtual const char* GetName_cStr() const = 0;

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

inline std::ostream& operator<<(std::ostream& out, const SECondition& a)
{
  a.ToString(out);
  return out;
}
} // namespace biogears

#pragma warning(disable : 4661)

namespace std {
extern template class vector<biogears::SECondition*>;
}
#pragma warning(default : 4661)
