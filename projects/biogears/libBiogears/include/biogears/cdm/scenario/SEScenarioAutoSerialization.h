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

#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/enums/SEPropertyEnums.h>
#include <biogears/schema/cdm/DataRequests.hxx>

CDM_BIND_DECL(ScenarioAutoSerializationData)
namespace biogears {
namespace io {
  class Scenario;
}
class BIOGEARS_API SEScenarioAutoSerialization : public Loggable {
  friend io::Scenario;

public:
  SEScenarioAutoSerialization(Logger* logger);
  virtual ~SEScenarioAutoSerialization();

  virtual void Clear();
  virtual bool IsValid() const;

  bool operator==(SEScenarioAutoSerialization const&) const;
  bool operator!=(SEScenarioAutoSerialization const&) const;

public:
  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

  virtual bool HasPeriodTimeStamps() const;
  virtual SEOnOff GetPeriodTimeStamps() const;
  virtual void SetPeriodTimeStamps(SEOnOff v);
  virtual void InvalidatePeriodTimeStamps();

  virtual bool HasAfterActions() const;
  virtual SEOnOff GetAfterActions() const;
  virtual void SetAfterActions(SEOnOff v);
  virtual void InvalidateAfterActions();

  virtual bool HasReloadState() const;
  virtual SEOnOff GetReloadState() const;
  virtual void SetReloadState(SEOnOff v);
  virtual void InvalidateReloadState();

  virtual bool HasDirectory() const;
  virtual const char* GetDirectory() const;
  virtual void SetDirectory(const char* dir);
  virtual void SetDirectory(const std::string& dir);
  virtual void InvalidateDirectory();

  virtual bool HasFileName() const;
  virtual const char* GetFileName() const;
  virtual void SetFileName(const char* dir);
  virtual void SetFileName(const std::string& dir);
  virtual void InvalidateFileName();

protected:
  SEScalarTime* m_Period;
  SEOnOff m_PeriodTimeStamps;
  SEOnOff m_AfterActions;
  SEOnOff m_ReloadState;
  std::string m_Directory;
  std::string m_FileName;
};
}