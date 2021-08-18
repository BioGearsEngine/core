
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
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {

class SELiquidCompartment;
class SETissueCompartment;
class SEScalarMass;
class SEScalarAmountPerVolume;
class AmountPerVolumeUnit;
class MassUnit;
class SEScalarMassPerVolume;
class MassPerVolumeUnit;

class BioGears;

class BIOGEARS_API SizeIndependentDepositionEfficencyCoefficient {
  friend class BioGearsSubstances;

public:
  SizeIndependentDepositionEfficencyCoefficient();
  ~SizeIndependentDepositionEfficencyCoefficient();

  double GetMouth() const; 
  double GetTrachea() const;
  double GetBronchi() const;
  double GetAlveoli() const;

protected:
  double m_mouth = 0;
  double m_trachea = 0;
  double m_bronchi = 0;
  double m_alveoli = 0;
};

/**
* @brief Manages and initializes all systems with substances needed by %BioGears
*/
class BIOGEARS_API BioGearsSubstances : public SESubstanceManager {
  friend class BioGearsEngineTest;

public:
  BioGearsSubstances(BioGears& data);
  virtual ~BioGearsSubstances();

  virtual void Clear();

  virtual bool LoadSubstanceDirectory();
  virtual void InitializeSubstances();

  virtual void AddActiveSubstance(SESubstance& substance);
  virtual bool IsActive(const SESubstance& sub) const;

  inline SESubstance& GetO2() { return *m_O2; }
  inline SESubstance& GetCO() { return *m_CO; }
  inline SESubstance& GetCO2() { return *m_CO2; }
  inline SESubstance& GetN2() { return *m_N2; }
  inline SESubstance& GetHb() { return *m_Hb; }
  inline SESubstance& GetHbO2() { return *m_HbO2; }
  inline SESubstance& GetHbCO2() { return *m_HbCO2; }
  inline SESubstance& GetHbCO() { return *m_HbCO; }
  inline SESubstance& GetHbO2CO2() { return *m_HbO2CO2; }
  inline SESubstance& GetHCO3() { return *m_HCO3; }
  inline SESubstance& GetEpi() { return *m_epi; }

  inline SESubstance& GetAlbumin() { return *m_albumin; }
  inline SESubstance& GetAminoAcids() { return *m_aminoAcids; }
  inline SESubstance& GetCalcium() { return *m_calcium; }
  inline SESubstance& GetChloride() { return *m_chloride; }
  inline SESubstance& GetCreatinine() { return *m_creatinine; }
  inline SESubstance& GetGlobulin() { return *m_globulin; }
  inline SESubstance& GetGlucagon() { return *m_glucagon; }
  inline SESubstance& GetGlucose() { return *m_glucose; }
  inline SESubstance& GetInsulin() { return *m_insulin; }
  inline SESubstance& GetKetones() { return *m_ketones; }
  inline SESubstance& GetLactate() { return *m_lactate; }
  inline SESubstance& GetPotassium() { return *m_potassium; }
  inline SESubstance& GetTriacylglycerol() { return *m_triacylglycerol; }
  inline SESubstance& GetSodium() { return *m_sodium; }
  inline SESubstance& GetUrea() { return *m_urea; }

  inline SESubstance& GetPlatelets() { return *m_platelets; }
  inline SESubstance& GetAntigen_A() { return *m_AntigenA; }
  inline SESubstance& GetAntigen_B() { return *m_AntigenB; }
  inline SESubstance& GetRBC() { return *m_RBC; }
  inline SESubstance& GetWBC() { return *m_WBC; }

  void CalculateGenericClearance(double volumeCleared_mL, SELiquidCompartment& cmpt, SESubstance& sub, SEScalarMass* cleared = nullptr);
  void CalculateGenericClearance(double volumeCleared_mL, SETissueCompartment& cmpt, SESubstance& sub, SEScalarMass* cleared = nullptr);
  void CalculateGenericExcretion(double VascularFlow_mL_Per_s, SETissueCompartment& cmpt, SESubstance& sub, double FractionExcreted, double timestep_s, SEScalarMass* excreted = nullptr);
  void SetLiquidCompartmentNonGasesForStarvation(double time_h);

  const SizeIndependentDepositionEfficencyCoefficient& GetSizeIndependentDepositionEfficencyCoefficient(SESubstance& substance);

  double GetSubstanceMass(SESubstance&, const std::vector<SELiquidCompartment*>&, const MassUnit& unit);
  double GetSubstanceMass(SESubstance&, const std::vector<SETissueCompartment*>&, const MassUnit& unit);

  void WriteBloodGases();
  void WritePulmonaryGases();
  void ProbeBloodGases(SELiquidCompartment& cmpt, const std::string& prefix = "");

protected:
  virtual void InitializeGasCompartments();
  virtual void InitializeLiquidCompartmentGases();
  virtual void InitializeLiquidCompartmentNonGases();

  virtual void InitializeBloodGases(SETissueCompartment& tissue, SELiquidCompartment& vascular);
  virtual void InitializeBloodGases(SELiquidCompartment& cmpt, double Hb_total_mM, double O2_sat, double O2_mmol_Per_L, double CO2_sat, double CO2_mmol_Per_L, double HCO3_mmol_Per_L, double pH, bool distribute = true);

  void SetSubstanceMolarity(SESubstance&, const std::vector<SELiquidCompartment*>&, const SEScalarAmountPerVolume& molarity);
  void SetSubstanceMolarity(SESubstance&, const std::vector<SETissueCompartment*>&, const SEScalarAmountPerVolume& molarity);
  void SetSubstanceMolarity(SESubstance&, const std::vector<SETissueCompartment*>&, const SEScalarAmountPerVolume& extracellular, const SEScalarAmountPerVolume& intracellular);
  void SetSubstanceMolarity(SESubstance&, const SEScalarAmountPerVolume& extracellular, const SEScalarAmountPerVolume& intracellular);

  void SetSubstanceConcentration(SESubstance&, const std::vector<SELiquidCompartment*>&, const SEScalarMassPerVolume& concentration);
  void SetSubstanceConcentration(SESubstance&, const std::vector<SETissueCompartment*>&, const SEScalarMassPerVolume& concentration);
  void SetSubstanceConcentration(SESubstance&, const std::vector<SETissueCompartment*>&, const SEScalarMassPerVolume& extracellular, const SEScalarMassPerVolume& intracellular);

  // Common Substances
  //Substances
  SESubstance* m_O2;
  SESubstance* m_CO;
  SESubstance* m_CO2;
  SESubstance* m_N2;
  SESubstance* m_Hb;
  SESubstance* m_HbO2;
  SESubstance* m_HbCO2;
  SESubstance* m_HbCO;
  SESubstance* m_HbO2CO2;
  SESubstance* m_HCO3;
  SESubstance* m_epi;

  SESubstance* m_albumin;
  SESubstance* m_aminoAcids;
  SESubstance* m_calcium;
  SESubstance* m_chloride;
  SESubstance* m_creatinine;
  SESubstance* m_globulin;
  SESubstance* m_glucagon;
  SESubstance* m_glucose;
  SESubstance* m_insulin;
  SESubstance* m_ketones;
  SESubstance* m_lactate;
  SESubstance* m_triacylglycerol;
  SESubstance* m_potassium;
  SESubstance* m_sodium;
  SESubstance* m_urea;

  SESubstance* m_platelets;
  SESubstance* m_AntigenA;
  SESubstance* m_AntigenB;
  SESubstance* m_RBC;
  SESubstance* m_WBC;

  bool m_isCOActive;

  std::map<SESubstance*, SizeIndependentDepositionEfficencyCoefficient*> m_SIDECoefficients;

  BioGears& m_data;
};
}