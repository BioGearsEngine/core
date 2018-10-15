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

#include <biogears/schema/cdm/Physiology.hxx>

namespace biogears {

class SEScalar;
class SEScalar0To1;

class BIOGEARS_API SEInflammationState {
public:
  SEInflammationState();
  virtual ~SEInflammationState();

  virtual void Clear();

  virtual bool Load(const CDM::InflammationStateData& in);
  virtual CDM::InflammationStateData* Unload() const;
  virtual bool IsValid();

  virtual const SEScalar* GetScalar(const std::string& name);

protected:
  virtual void Unload(CDM::InflammationStateData& data) const;

public:
  virtual bool HasPathogenTissue() const;
  virtual SEScalar& GetPathogenTissue();
  virtual double GetPathogenTissue() const;

  virtual bool HasPathogenBlood() const;
  virtual SEScalar& GetPathogenBlood();
  virtual double GetPathogenBlood() const;

  virtual bool HasMacrophageRestingTissue() const;
  virtual SEScalar& GetMacrophageRestingTissue();
  virtual double GetMacrophageRestingTissue() const;

  virtual bool HasMacrophageActiveTissue() const;
  virtual SEScalar& GetMacrophageActiveTissue();
  virtual double GetMacrophageActiveTissue() const;

  virtual bool HasNeutrophilRestingBlood() const;
  virtual SEScalar& GetNeutrophilRestingBlood();
  virtual double GetNeutrophilRestingBlood() const;

  virtual bool HasNeutrophilActiveBlood() const;
  virtual SEScalar& GetNeutrophilActiveBlood();
  virtual double GetNeutrophilActiveBlood() const;

  virtual bool HasNeutrophilActiveTissue() const;
  virtual SEScalar& GetNeutrophilActiveTissue();
  virtual double GetNeutrophilActiveTissue() const;

  virtual bool HasTNFBlood() const;
  virtual SEScalar& GetTNFBlood();
  virtual double GetTNFBlood() const;

  virtual bool HasTNFTissue() const;
  virtual SEScalar& GetTNFTissue();
  virtual double GetTNFTissue() const;

  virtual bool HasIL10Blood() const;
  virtual SEScalar& GetIL10Blood();
  virtual double GetIL10Blood() const;

  virtual bool HasIL10Tissue() const;
  virtual SEScalar& GetIL10Tissue();
  virtual double GetIL10Tissue() const;

  virtual bool HasTissueIntegrity() const;
  virtual SEScalar0To1& GetTissueIntegrity();
  virtual double GetTissueIntegrity() const;

  virtual bool HasInflammation() const;
  virtual SEScalar0To1& GetInflammation();
  virtual double GetInflammation() const;


protected:
  SEScalar* m_PathogenTissue;
  SEScalar* m_PathogenBlood;
  SEScalar* m_MacrophageRestingTissue;
  SEScalar* m_MacrophageActiveTissue;
  SEScalar* m_NeutrophilRestingBlood;
  SEScalar* m_NeutrophilActiveBlood;
  SEScalar* m_NeutrophilActiveTissue;
  SEScalar* m_TNFBlood;
  SEScalar* m_TNFTissue;
  SEScalar* m_IL10Blood;
  SEScalar* m_IL10Tissue;
  SEScalar0To1* m_TissueIntegrity;
  SEScalar0To1* m_Inflammation;
};
}
