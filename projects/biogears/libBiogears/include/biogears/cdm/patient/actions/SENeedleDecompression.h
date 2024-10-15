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
namespace io {
  class PatientActions;
}

class BIOGEARS_API SENeedleDecompression : public SEPatientAction {
  friend io::PatientActions;

public:
  SENeedleDecompression();
  virtual ~SENeedleDecompression() override;

  static constexpr const char* TypeTag() { return "SENeedleDecompression"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; // clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;
  virtual void SetActive(bool b);

  virtual SESide GetSide() const;
  virtual void SetSide(SESide LeftOrRight);
  virtual bool HasSide() const;
  virtual void InvalidateSide();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SENeedleDecompression& rhs) const;
  bool operator!=(const SENeedleDecompression& rhs) const;

protected:
  SESide m_Side;
  SEOnOff m_State;
};
}