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
#include <biogears/cdm/enums/SEPatientActionsEnums.h>

#include <random>

namespace biogears {
class SEScalar0To1;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SETensionPneumothorax : public SEPatientAction {
  friend io::PatientActions;

public:
  SETensionPneumothorax();
  virtual ~SETensionPneumothorax() override;

  static constexpr const char* TypeTag() { return "SETensionPneumothorax"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; // clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual SEPneumothoraxType GetType() const;
  virtual void SetType(SEPneumothoraxType name);
  virtual bool HasType() const;
  virtual void InvalidateType();

  virtual SESide GetSide() const;
  virtual void SetSide(SESide name);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SETensionPneumothorax& rhs) const;
  bool operator!=(const SETensionPneumothorax& rhs) const;

protected:
  SEPneumothoraxType m_Type;
  SESide m_Side;
  SEScalar0To1* m_Severity;
};
}