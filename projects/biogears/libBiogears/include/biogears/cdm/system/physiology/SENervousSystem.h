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

#include <biogears/cdm/patient/actions/SEPupillaryResponse.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/biogears/BioGearsPhysiology.hxx>

namespace biogears {
class BIOGEARS_API SENervousSystem : public SESystem {
public:
  SENervousSystem(Logger* logger);
  ~SENervousSystem() override;

  static size_t TypeHash() { return reinterpret_cast<size_t>(&TypeHash); }
  static constexpr char const* const TypeTag() { return "SENervousSystem"; }
  const char* classname() const override { return TypeTag(); }
  size_t hash_code() const override { return TypeHash(); }

  void Clear() override; // Deletes all members

  const SEScalar* GetScalar(const char* name) override;
  const SEScalar* GetScalar(const std::string& name) override;

  bool Load(const CDM::NervousSystemData& in);
  CDM::NervousSystemData* Unload() const override;

  Tree<const char*> GetPhysiologyRequestGraph() const override;

protected:
  void Unload(CDM::NervousSystemData& data) const;

public:
  bool HasComplianceScale() const;
  SEScalar& GetComplianceScale();
  double GetComplianceScale() const;

  bool HasHeartRateScale() const;
  SEScalar& GetHeartRateScale();
  double GetHeartRateScale() const;

  bool HasHeartElastanceScale() const;
  SEScalar& GetHeartElastanceScale();
  double GetHeartElastanceScale() const;

  bool HasResistanceScaleExtrasplanchnic() const;
  SEScalar& GetResistanceScaleExtrasplanchnic();
  double GetResistanceScaleExtrasplanchnic() const;

  bool HasResistanceScaleMuscle() const;
  SEScalar& GetResistanceScaleMuscle();
  double GetResistanceScaleMuscle() const;

  bool HasResistanceScaleMyocardium() const;
  SEScalar& GetResistanceScaleMyocardium();
  double GetResistanceScaleMyocardium() const;

  bool HasResistanceScaleSplanchnic() const;
  SEScalar& GetResistanceScaleSplanchnic();
  double GetResistanceScaleSplanchnic() const;

  bool HasLeftEyePupillaryResponse() const;
  SEPupillaryResponse& GetLeftEyePupillaryResponse();
  const SEPupillaryResponse* GetLeftEyePupillaryResponse() const;
  void RemoveLeftEyePupillaryResponse();

  bool HasPainVisualAnalogueScale() const;
  SEScalar& GetPainVisualAnalogueScale();
  double GetPainVisualAnalogueScale() const;

  bool HasRightEyePupillaryResponse() const;
  SEPupillaryResponse& GetRightEyePupillaryResponse();
  const SEPupillaryResponse* GetRightEyePupillaryResponse() const;
  void RemoveRightEyePupillaryResponse();

protected:
  SEScalar* m_ComplianceScale;
  SEScalar* m_HeartRateScale;
  SEScalar* m_HeartElastanceScale;
  SEScalar* m_ResistanceScaleExtrasplanchnic;
  SEScalar* m_ResistanceScaleMuscle;
  SEScalar* m_ResistanceScaleMyocardium;
  SEScalar* m_ResistanceScaleSplanchnic;

  SEPupillaryResponse* m_LeftEyePupillaryResponse;
  SEPupillaryResponse* m_RightEyePupillaryResponse;
  SEScalar* m_PainVisualAnalogueScale;
};
}