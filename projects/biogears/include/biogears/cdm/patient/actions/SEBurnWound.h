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
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
class SEScalar0To1;
class SEScalar;

class BIOGEARS_API SEBurnWound : public SEPatientAction {
public:
  SEBurnWound();
  virtual ~SEBurnWound() override;

  static constexpr const char* TypeTag() { return "SEBurnWound"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::BurnWoundData& in);
  virtual CDM::BurnWoundData* Unload() const override;

protected:
  virtual void Unload(CDM::BurnWoundData& data) const;

public:
  virtual bool HasTotalBodySurfaceArea() const;
  virtual SEScalar0To1& GetTotalBodySurfaceArea();

  virtual bool HasInflammation() const;
  virtual void SetInflammation(bool activate);

  virtual void ToString(std::ostream& str) const override;

protected:
  bool m_Inflammation;
  SEScalar0To1* m_TBSA;
};
}
