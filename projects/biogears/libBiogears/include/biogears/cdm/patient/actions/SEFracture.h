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

#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/cdm/enums/SEPatientActionsEnums.h>


#include <random>

namespace biogears {
class SEScalar0To1;
namespace io {
  class PatientActions;
}

class BIOGEARS_API SEFracture : public SEPatientAction {
  friend io::PatientActions;

public:
  SEFracture();
  virtual ~SEFracture() override;

  static constexpr const char* TypeTag() { return "SEFracture"; };
  const char* classname() const override { return TypeTag(); }
  
  virtual void Invalidate() override; // clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  bool HasFracturedBone() const;
  SEFracturedBone GetFracturedBone() const;
  void SetFracturedBone(SEFracturedBone bone);

  virtual bool HasSide() const;
  virtual SESide GetSide() const;
  virtual void SetSide(SESide LeftOrRight);
  virtual void InvalidateSide();

  bool HasFractureType() const;
  SEFractureType GetFractureType() const;
  void SetFractureType(SEFractureType type);

  virtual bool HasSeverity() const;
  virtual SEScalar0To1& GetSeverity();
  //void SetSeverity(SEFracturedBone bone, SEFractureType type);

  bool HasInflammation() const; //May need; Check back before merging
  void SetInflammation(bool activate);

  void ToString(std::ostream& str) const override;

  bool operator==(const SEFracture& rhs) const;
  bool operator!=(const SEFracture& rhs) const;

protected:

private:
  bool m_Inflammation;
  SEFracturedBone m_FracturedBone;
  SESide m_Side;
  SEFractureType m_FractureType;
  SEScalar0To1* m_Severity;
};
}
