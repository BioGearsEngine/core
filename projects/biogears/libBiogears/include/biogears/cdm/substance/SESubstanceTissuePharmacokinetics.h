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

#include <biogears/schema/cdm/Substance.hxx>

namespace biogears {
class SEScalar;

class BIOGEARS_API SESubstanceTissuePharmacokinetics : public Loggable {
public:
  SESubstanceTissuePharmacokinetics(const std::string& name, Logger* logger);
  virtual ~SESubstanceTissuePharmacokinetics();

  virtual void Clear();

  virtual bool Load(const CDM::SubstanceTissuePharmacokineticsData& in);
  virtual CDM::SubstanceTissuePharmacokineticsData* Unload() const;

protected:
  virtual void Unload(CDM::SubstanceTissuePharmacokineticsData& data) const;

public:
  const SEScalar* GetScalar(const char* name);
  const SEScalar* GetScalar(const std::string& name);

  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;

  virtual bool HasPartitionCoefficient() const;
  virtual SEScalar& GetPartitionCoefficient();
  virtual double GetPartitionCoefficient() const;

protected:
  std::string m_Name;
  SEScalar* m_PartitionCoefficient;
};
}