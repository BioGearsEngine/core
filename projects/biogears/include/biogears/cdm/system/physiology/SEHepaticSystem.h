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
#include <biogears/cdm/system/SESystem.h>
#include <biogears/schema/HepaticSystemData.hxx>

class BIOGEARS_API SEHepaticSystem : public SESystem
{
public:

	SEHepaticSystem(Logger* logger);
	virtual ~SEHepaticSystem();

  virtual void Clear();// Deletes all members
  
	virtual const SEScalar* GetScalar(const std::string& name);
	
  virtual bool Load(const CDM::HepaticSystemData& in);
  virtual CDM::HepaticSystemData* Unload() const;

protected:
  virtual void Unload(CDM::HepaticSystemData& data) const;

public:

  virtual bool HasKetoneProductionRate() const;
  virtual SEScalarAmountPerTime& GetKetoneProductionRate();
  virtual double GetKetoneProductionRate(const AmountPerTimeUnit& unit) const;

  virtual bool HasHepaticGluconeogenesisRate() const;
  virtual SEScalarMassPerTime& GetHepaticGluconeogenesisRate();
  virtual double GetHepaticGluconeogenesisRate(const MassPerTimeUnit& unit) const;

	
protected:
  SEScalarAmountPerTime*	m_KetoneProductionRate;
  SEScalarMassPerTime* m_HepaticGluconeogenesisRate;
	
};