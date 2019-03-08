/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License..
**************************************************************************************/

#pragma once
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>

CDM_BIND_DECL(SubstanceFractionData)

namespace biogears {
class SESubstance;
class SESubstanceManager;
class SEEnvironmentalConditions;
class SEScalarFraction;

class BIOGEARS_API SESubstanceFraction : public Loggable {
protected:
  friend SEEnvironmentalConditions; // So it can add substances to the manager
public:
  SESubstanceFraction(SESubstance&);
  virtual ~SESubstanceFraction();

  virtual void Clear();

  virtual bool Load(const CDM::SubstanceFractionData& in);
  virtual CDM::SubstanceFractionData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceFractionData& data) const;

public:
  virtual bool HasFractionAmount() const;
  virtual SEScalarFraction& GetFractionAmount();
  virtual double GetFractionAmount() const;

  virtual SESubstance& GetSubstance() const;

protected:
  SESubstance& m_Substance;
  SEScalarFraction* m_FractionAmount;
};
}