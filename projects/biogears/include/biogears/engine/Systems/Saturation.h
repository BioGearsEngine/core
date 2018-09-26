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

#include <biogears/cdm/system/physiology/SETissueSystem.h>
#include <biogears/engine/Controller/BioGearsSystem.h>

namespace biogears {
struct error_functor;
class SESubstance;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;
class BioGears;
class SEScalarFraction;
class SEScalarTemperature;

/**
* @brief
* The %SaturationCalculator class holds the blood gas distribution model.
*/
class BIOGEARS_API SaturationCalculator : public Loggable {
protected:
  friend struct error_functor;
  friend class BioGears;
  friend class BioGearsEngineTest;

  SaturationCalculator(BioGears& bg);
  BioGears& m_data;

public:
  virtual ~SaturationCalculator();

  void Initialize(SESubstanceManager& substances);

  void SetBodyState(const SEScalarMassPerVolume& AlbuminConcentration, const SEScalarFraction& Hematocrit, const SEScalarTemperature& Temperature, const SEScalarAmountPerVolume& StrongIonDifference, const SEScalarAmountPerVolume& Phosphate);
  void CalculateBloodGasDistribution(SELiquidCompartment& cmpt);
  void CalculateCarbonMonoxideSpeciesDistribution(SELiquidCompartment& cmpt);

protected: // Stewart Model + Dash-Bassingthwaighte Model + Henderson-Hasselbach Model
  void CalculateHemoglobinSaturations(double O2PartialPressureGuess_mmHg, double CO2PartialPressureGuess_mmHg, double pH, double temperature_C, double hematocrit, double& OxygenSaturation, double& CarbonDioxideSaturation, double CO2_scaling_factor);
  bool DistributeHemoglobinBySaturation();

  // All properties are stateless and are set by either the Initialize method or SetBodyState method
  SESubstance* m_O2;
  SESubstance* m_Hb;
  SESubstance* m_HbO2;
  SESubstance* m_HbO2CO2;
  SESubstance* m_CO2;
  SESubstance* m_CO; //Remember to tell amb I added these CO members
  SESubstance* m_HbCO;
  SESubstance* m_HCO3;
  SESubstance* m_HbCO2;
  // Used for conversions
  double m_O2_g_Per_mol;
  double m_CO2_g_Per_mol;
  double m_HCO3_g_Per_mol;
  double m_Hb_g_Per_mol;
  double m_HbO2_g_Per_mol;
  double m_HbCO2_g_Per_mol;
  double m_HbO2CO2_g_Per_mol;
  // This is the current compartment and the quantities we are balancing
  SELiquidCompartment* m_cmpt;
  SELiquidSubstanceQuantity* m_subO2Q;
  SELiquidSubstanceQuantity* m_subCO2Q;
  SELiquidSubstanceQuantity* m_subCOQ; //Remember to tell amb I added these CO members
  SELiquidSubstanceQuantity* m_subHbCOQ;
  SELiquidSubstanceQuantity* m_subHCO3Q;
  SELiquidSubstanceQuantity* m_subHbQ;
  SELiquidSubstanceQuantity* m_subHbO2Q;
  SELiquidSubstanceQuantity* m_subHbCO2Q;
  SELiquidSubstanceQuantity* m_subHbO2CO2Q;
  // The current state of the body to balance to
  double m_albumin_g_per_L;
  double m_hematocrit;
  double m_temperature_C;
  // Here is the stuff that will need to go into the CDM
  double m_StrongIonDifference_mmol_Per_L; // BloodChemistrySystemData mmol/L
  double m_Phosphate_mmol_Per_L; //BloodChemistryData mmol/L
};
}