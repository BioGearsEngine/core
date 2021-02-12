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
#include <string>
#include <typeinfo>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/container/Tree.h>
#include <biogears/exports.h>

CDM_BIND_DECL(SystemData)

namespace biogears {
class SESubstanceManager;
class SEScalar;
   namespace io{
   class Environment;}
class BIOGEARS_API SESystem : public Loggable {
public:
  friend io::Environment;
  SESystem(Logger* logger);
  virtual ~SESystem();

  virtual const char* classname() const = 0;
  virtual size_t hash_code() const = 0;

  virtual void Clear(); // Deletes all members

  /** @name GetScalar
  *   @brief - A reflextion type call that will return the Scalar associated
  *            with the string. ex. GetScalar("Hematocrit") will return the0
  *            SEScalarPercent object associated with Hematocrit
  *   @details - Note this is an expensive call as it will string compare many times
  *              This is best used, and intended for, you to dynamically prepopulate
  *              a mapping data structure that will help access what you need
  */
  virtual const SEScalar* GetScalar(const char* name) = 0;
  virtual const SEScalar* GetScalar(const std::string& name) = 0;

  /** @name GetScalar
  *   @brief - Look for the Scalar property in the systems contained in the provided vector
  */
  static const SEScalar* GetScalar(const char* name, std::vector<SESystem*>* systems);
  static const SEScalar* GetScalar(const std::string& name, std::vector<SESystem*>* systems);

  bool Load(const CDM::SystemData& in);
  virtual CDM::SystemData* Unload() const = 0;

  virtual Tree<const char*> GetPhysiologyRequestGraph() const = 0;

protected:
  void Unload(CDM::SystemData& data) const;

  std::stringstream m_ss;
};
}