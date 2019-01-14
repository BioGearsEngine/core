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
#include <biogears/exports.h>

#include <biogears/schema/cdm/Physiology.hxx>
#include <biogears/cdm/utils/Logger.h>

namespace biogears {
class SEScalar;
class SEScalarNeg1To1;

class BIOGEARS_API SEPupillaryResponse {
public:
  SEPupillaryResponse(Logger* logger);
  virtual ~SEPupillaryResponse();

  virtual void Clear(); // Deletes all members

  virtual const SEScalar* GetScalar(const char* name);
  virtual const SEScalar* GetScalar(const std::string& name);

  virtual bool Load(const CDM::PupillaryResponseData& in);
  virtual CDM::PupillaryResponseData* Unload() const;

protected:
  virtual void Unload(CDM::PupillaryResponseData& data) const;

public:
  virtual bool HasReactivityModifier() const;
  virtual SEScalarNeg1To1& GetReactivityModifier();
  virtual double GetReactivityModifier() const;

  virtual bool HasShapeModifier() const;
  virtual SEScalarNeg1To1& GetShapeModifier();
  virtual double GetShapeModifier() const;

  virtual bool HasSizeModifier() const;
  virtual SEScalarNeg1To1& GetSizeModifier();
  virtual double GetSizeModifier() const;

protected:
  SEScalarNeg1To1* m_ReactivityModifier;
  SEScalarNeg1To1* m_ShapeModifier;
  SEScalarNeg1To1* m_SizeModifier;
};
}