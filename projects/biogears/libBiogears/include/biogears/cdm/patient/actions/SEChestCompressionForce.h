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
#include <biogears/cdm/patient/actions/SEChestCompression.h>
#include <biogears/schema/cdm/PatientActions.hxx>

OPEN_BIOGEARS_NAMESPACE
class SEScalarForce;
namespace io {
  class PatientActions;
}
class BIOGEARS_API SEChestCompressionForce : public SEChestCompression {
  friend io::PatientActions;

public:
  SEChestCompressionForce();
  virtual ~SEChestCompressionForce();

  virtual void Clear() override; //clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::ChestCompressionForceData& in);
  virtual CDM::ChestCompressionForceData* Unload() const override;

  virtual bool HasForce() const;
  virtual SEScalarForce& GetForce();

  virtual void ToString(std::ostream& str) const override;

  bool operator==( const SEChestCompressionForce& rhs) const;
  bool operator!=( const SEChestCompressionForce& rhs) const;

protected:
  virtual void Unload(CDM::ChestCompressionForceData& data) const;

protected:
  SEScalarForce* m_Force;
};
}