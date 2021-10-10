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

CDM_BIND_DECL(ConditionData);

namespace biogears {
class SESubstanceManager;
namespace io {
  class Scenario;
}
}//namespace biogears

namespace biogears {
class BIOGEARS_API SECondition : public Loggable {
  friend io::Scenario;

public:
  SECondition();
  virtual ~SECondition();

  virtual void Clear(); // Deletes all members

  static SECondition* newFromBind(const CDM::ConditionData& condition, SESubstanceManager& substances);

  virtual bool Load(const CDM::ConditionData& in);
  virtual CDM::ConditionData* Unload() const;

protected:
  void Unload(CDM::ConditionData& data) const;

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
}//namespace biogears


namespace std {
BG_EXT template class BIOGEARS_API vector<biogears::SECondition*>;
}