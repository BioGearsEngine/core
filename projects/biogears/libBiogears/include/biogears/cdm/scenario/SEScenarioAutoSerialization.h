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
#include <biogears/schema/cdm/Scenario.hxx>

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

  virtual bool Load(const CDM::ScenarioAutoSerializationData& in);
  virtual CDM::ScenarioAutoSerializationData* Unload() const;

protected:
  virtual void Unload(CDM::ScenarioAutoSerializationData& data) const;

public:
  virtual bool HasPeriod() const;
  virtual SEScalarTime& GetPeriod();
  virtual double GetPeriod(const TimeUnit& unit) const;

  virtual bool HasPeriodTimeStamps() const;
  virtual CDM::enumOnOff::value GetPeriodTimeStamps() const;
  virtual void SetPeriodTimeStamps(CDM::enumOnOff::value v);
  virtual void InvalidatePeriodTimeStamps();

  virtual bool HasAfterActions() const;
  virtual CDM::enumOnOff::value GetAfterActions() const;
  virtual void SetAfterActions(CDM::enumOnOff::value v);
  virtual void InvalidateAfterActions();

  virtual bool HasReloadState() const;
  virtual CDM::enumOnOff::value GetReloadState() const;
  virtual void SetReloadState(CDM::enumOnOff::value v);
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
  CDM::enumOnOff::value m_PeriodTimeStamps;
  CDM::enumOnOff::value m_AfterActions;
  CDM::enumOnOff::value m_ReloadState;
  std::string m_Directory;
  std::string m_FileName;
};
}