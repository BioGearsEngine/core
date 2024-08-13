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

#include <biogears/cdm/engine/PhysiologyEngineStabilization.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/schema/biogears/BioGears.hxx>

namespace biogears {
class TimeUnit;
class PhysiologyEngineDynamicStabilizationCriteria;
namespace io {
  class EngineConfiguration;
}

class BIOGEARS_API PropertyConvergence : public Loggable {
  friend PhysiologyEngineDynamicStabilizationCriteria;
  friend io::EngineConfiguration;

protected:
  PropertyConvergence(SEDataRequest& dr, Logger* logger);

public:
  virtual ~PropertyConvergence();

  bool Test(double time_s);

  double GetPercentError() const;
  double GetLastPercentError() const;
  double GetLastErrorTime_s() const;
  double GetCurrentTarget() const;
  SEDataRequest& GetDataRequest() const;
  bool IsOptional() const;
  void SetOptional(bool b);

  void TrackScalar(const SEScalar& s);
  SEDataRequestScalar& GetDataRequestScalar();

protected:
  SEDataRequest& m_DataRequest;
  SEDataRequestScalar m_DataRequestScalar;

  bool m_Optional;
  double m_Target;
  double m_Error;
  double m_LastError;
  double m_LastErrorTime_s;
  std::stringstream m_ss;
};

class PhysiologyEngineDynamicStabilization;
CDM_BIND_DECL(PhysiologyEngineDynamicStabilizationCriteriaData)
class BIOGEARS_API PhysiologyEngineDynamicStabilizationCriteria : public Loggable {
  friend PhysiologyEngineDynamicStabilization;
  friend io::EngineConfiguration;

public:
  PhysiologyEngineDynamicStabilizationCriteria(Logger* logger);

  virtual ~PhysiologyEngineDynamicStabilizationCriteria();

  virtual void Clear();

public:
  virtual std::string GetName() const;
  virtual const char* GetName_cStr() const;
  virtual void SetName(const char* name);
  virtual void SetName(const std::string& name);
  virtual bool HasName() const;
  virtual void InvalidateName();

  virtual bool HasConvergenceTime() const;
  virtual SEScalarTime& GetConvergenceTime();
  virtual double GetConvergenceTime(const TimeUnit& unit) const;

  virtual bool HasMinimumReactionTime() const;
  virtual SEScalarTime& GetMinimumReactionTime();
  virtual double GetMinimumReactionTime(const TimeUnit& unit) const;

  virtual bool HasMaximumAllowedStabilizationTime() const;
  virtual SEScalarTime& GetMaximumAllowedStabilizationTime();
  virtual double GetMaximumAllowedStabilizationTime(const TimeUnit& unit) const;

  virtual const std::vector<PropertyConvergence*>& GetPropertyConvergence() const;
  virtual PropertyConvergence& CreateSystemPropertyConvergence(double percentError, const char* name);
  virtual PropertyConvergence& CreateSystemPropertyConvergence(double percentError, const std::string& name);

  virtual PropertyConvergence& CreateGasCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name);
  virtual PropertyConvergence& CreateGasCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name);
  virtual PropertyConvergence& CreateGasCompartmentPropertyConvergence(double percentError, const char* cmpt, SESubstance& substance, const char* name);
  virtual PropertyConvergence& CreateGasCompartmentPropertyConvergence(double percentError, const std::string& cmpt, SESubstance& substance, const std::string& name);
  virtual PropertyConvergence& CreateLiquidCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name);
  virtual PropertyConvergence& CreateLiquidCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name);
  virtual PropertyConvergence& CreateLiquidCompartmentPropertyConvergence(double percentError, const char* cmpt, SESubstance& substance, const char* name);
  virtual PropertyConvergence& CreateLiquidCompartmentPropertyConvergence(double percentError, const std::string& cmpt, SESubstance& substance, const std::string& name);
  virtual PropertyConvergence& CreateThermalCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name);
  virtual PropertyConvergence& CreateThermalCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name);
  virtual PropertyConvergence& CreateTissueCompartmentPropertyConvergence(double percentError, const char* cmpt, const char* name);
  virtual PropertyConvergence& CreateTissueCompartmentPropertyConvergence(double percentError, const std::string& cmpt, const std::string& name);

protected:
  std::string m_Name;
  SEScalarTime* m_ConvergenceTime;
  SEScalarTime* m_MinimumReactionTime;
  SEScalarTime* m_MaximumAllowedStabilizationTime;

  SEDataRequestManager m_DataRequestMgr;
  std::vector<PropertyConvergence*> m_PropertyConvergence;
};

class BIOGEARS_API PhysiologyEngineDynamicStabilizer : public Loggable {
public:
  friend io::EngineConfiguration;
  PhysiologyEngineDynamicStabilizer(double timeStep_s, const PhysiologyEngineDynamicStabilizationCriteria& criteria);
  virtual ~PhysiologyEngineDynamicStabilizer() {};

  virtual void Converge();
  virtual bool HasConverged() { return m_converged; }
  virtual bool HasConvergedOptional() { return m_convergedOptional; }
  virtual bool HasExceededTime() { return m_exceededTime; }

protected:
  bool m_converged;
  bool m_convergedOptional;
  bool m_exceededTime;
  bool m_hasOptionalProperties;
  double m_dT_s;
  double m_totTime_s; // Total time we have been converging
  double m_covTime_s; // Convergence Window Length
  double m_minTime_s; // Minimum amount of time to run before testing
  double m_maxTime_s; // Maximum amount of time to run before we quit
  double m_passTime_s;
  double m_optsPassTime_s;

  const std::vector<PropertyConvergence*>& m_properties;
};

CDM_BIND_DECL(PhysiologyEngineDynamicStabilizationData)
class PhysiologyEngineDynamicStabilization : public PhysiologyEngineStabilization {
public:
  friend io::EngineConfiguration;
  BIOGEARS_API PhysiologyEngineDynamicStabilization(Logger* logger);
  BIOGEARS_API virtual ~PhysiologyEngineDynamicStabilization();

  BIOGEARS_API virtual void Clear() override;

public:
  BIOGEARS_API virtual bool Load(const char* file) override;
  BIOGEARS_API virtual bool Load(const std::string& file) override;

  BIOGEARS_API virtual bool StabilizeRestingState(PhysiologyEngine& engine) override;
  BIOGEARS_API virtual bool StabilizeFeedbackState(PhysiologyEngine& engine) override;
  BIOGEARS_API virtual bool StabilizeConditions(PhysiologyEngine& engine, const std::vector<const SECondition*>& conditions) override;

  BIOGEARS_API virtual PhysiologyEngineDynamicStabilizationCriteria& GetRestingCriteria();
  BIOGEARS_API virtual const PhysiologyEngineDynamicStabilizationCriteria& GetRestingCriteria() const;

  BIOGEARS_API virtual bool HasFeedbackCriteria() const;
  BIOGEARS_API virtual PhysiologyEngineDynamicStabilizationCriteria& GetFeedbackCriteria();
  BIOGEARS_API virtual const PhysiologyEngineDynamicStabilizationCriteria* GetFeedbackCriteria() const;

  BIOGEARS_API virtual void RemoveConditionCriteria(const char* name);
  BIOGEARS_API virtual void RemoveConditionCriteria(const std::string& name);
  BIOGEARS_API virtual void AddConditionCriteria(PhysiologyEngineDynamicStabilizationCriteria& criteria);
  BIOGEARS_API virtual const std::vector<PhysiologyEngineDynamicStabilizationCriteria*>& GetConditionCriteria() const;
  BIOGEARS_API virtual PhysiologyEngineDynamicStabilizationCriteria* GetConditionCriteria(const char* name) const;
  BIOGEARS_API virtual PhysiologyEngineDynamicStabilizationCriteria* GetConditionCriteria(const std::string& name) const;

protected:
  BIOGEARS_API virtual bool Stabilize(PhysiologyEngine& engine, const PhysiologyEngineDynamicStabilizationCriteria& criteria);

  PhysiologyEngineDynamicStabilizationCriteria m_RestingCriteria;
  PhysiologyEngineDynamicStabilizationCriteria* m_FeedbackCriteria;
  std::vector<PhysiologyEngineDynamicStabilizationCriteria*> m_ConditionCriteria;

  BIOGEARS_API bool Merge();
  PhysiologyEngineDynamicStabilizationCriteria m_MergedConditions;
  std::vector<PhysiologyEngineDynamicStabilizationCriteria*> m_ActiveConditions;
};
}
