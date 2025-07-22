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
  static size_t constexpr ALL_SYSTEMS_AND_EQUIPMENT_PRESENT = 0x7FFF;

  static size_t constexpr ANESTHESIA_NACHINE_EQUIPMENT = 0x1 << 12;
  static size_t constexpr ELECTRO_CARDIO_GRAM_EQUIPMENT = 0x1 << 13;
  static size_t constexpr INHALER_EQUIPMENT = 0x1 << 14;

  static size_t constexpr ENVIRONMENT_SYSTEM = 0x1 << 11;
  static size_t constexpr TISSUE_SYSTEM = 0x1 << 10;
  static size_t constexpr RESPIRATORY_SYSTEM = 0x1 << 9;
  static size_t constexpr RENAL_SYSTEM = 0x1 << 8;

  static size_t constexpr NERVOUS_SYSTEM = 0x1 << 7;
  static size_t constexpr HEPTIC_SYSTEM = 0x1 << 6;
  static size_t constexpr GASTROINTESTINAL_SYSTEM = 0x1 << 5;
  static size_t constexpr ENERGY_SYSTEM = 0x1 << 4;

  static size_t constexpr ENDOCRINE_SYSTEM = 0x1 << 3;
  static size_t constexpr DRUG_SYSTEM = 0x1 << 2;
  static size_t constexpr CARDIOVASCULAR_SYSTEM = 0x1 << 1;
  static size_t constexpr BLOOD_CHEMISTRY_SYSTEM = 0x1 << 0;

  virtual void Initialize()
  {
    SetUp();
  }
  /// Notify systems that steady state has been achieved
  virtual void AtSteadyState() {};
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
