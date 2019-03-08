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

namespace biogears {
class BioGearsEngine;

/**
 * @brief Generic class for handling the %BioGears stabilization methodology
 */
class BIOGEARS_API BioGearsSystem {
  friend BioGearsEngine;

public:
  /**
  * @brief - Default system values to their homeostatic values
  */
  virtual void Initialize()
  {
    SetUp();
  }
  /// Notify systems that steady state has been achieved
  virtual void AtSteadyState(){};
  virtual void PreProcess() = 0;
  virtual void Process() = 0;
  virtual void PostProcess() = 0;

protected:
  virtual void SetUp() = 0;
  virtual void LoadState()
  {
    SetUp();
  }
};
}