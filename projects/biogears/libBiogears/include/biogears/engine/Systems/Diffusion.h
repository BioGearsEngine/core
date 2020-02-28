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
#include <memory>

#include <Eigen/Core>

#include <biogears/cdm/CommonDataModel.h>
#include <biogears/exports.h>
#include <vector>
#include <memory>
#include <Eigen/Core>


namespace biogears {
class MassUnit;
class BioGears;
class SESubstanceManager;
class SESubstance;
class SETissueCompartment;
class SELiquidCompartment;

class BIOGEARS_API DiffusionCalculator : public Loggable {
  friend class BioGears;
  friend class BioGearsEngineTest;

protected:
  static auto make_unique(BioGears& bg) -> std::unique_ptr<DiffusionCalculator>;
  DiffusionCalculator(BioGears& bg);

  void ClearConcentrations();
  BioGears& m_data;

  struct DiffusionCompartmentSet {
    SETissueCompartment* tissue;
    SELiquidCompartment* vascular;
    SELiquidCompartment* extracellular;
    SELiquidCompartment* intracellular;
  };

  struct DiffusionMatrixSet {
    Eigen::MatrixXd vascular;
    Eigen::MatrixXd extracellular;
    Eigen::MatrixXd intracellular;
  };

  struct ConcentrationSet {
    std::vector<double> vascular_ug_Per_mL;
    std::vector<double> extracellular_ug_Per_mL;
    std::vector<double> intracellular_ug_Per_mL;
  };

public:
  ~DiffusionCalculator() = default;
  void Initialize(SESubstanceManager& subMgr);
  void SetDiffusionState();
  void CalculateLinearDiffusionMethods();
  void CalculateNonLinearDiffusionMethods();

  std::vector<DiffusionCompartmentSet>& GetDiffusionSets() { return m_DiffusionSets; };
  std::vector<SESubstance*>& GetFacilitatedDiffusionSubstances() { return m_FacilitatedDiffusionSubstances; };
  std::vector<SESubstance*>& GetInstantDiffusionSubstances() { return m_InstantDiffusionSubstances; };
  std::vector<SESubstance*>& GetSimpleDiffusionSubstances() { return m_SimpleDiffusionSubstances; };

protected:
  void CalculateActiveIonDiffusion(DiffusionCompartmentSet& cmptSet);
  void CalculateMacromoleculeDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub);
  void CalculatePassiveLymphDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub);
  void CalculateFacilitatedDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub, double combinedCoefficient_g_Per_s);
  void CalculatePerfusionLimitedDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub, double partitionCoeff);

private:
  void DistributeMassbyVolumeWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit);
  void DistributeMassbyMassWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit);
  double SodiumPotassiumPump(double intraNa_mM, double extraNa_mM, double extraK_mM, double potential_V);
  double CalciumPump(double intraCa_M);

  double m_dt_s;

  std::vector<SESubstance*> m_SimpleDiffusionSubstances;
  std::vector<SESubstance*> m_InstantDiffusionSubstances;
  std::vector<SESubstance*> m_FacilitatedDiffusionSubstances;

  ConcentrationSet m_InstantSubConcentrations;
  ConcentrationSet m_SimpleSubConcentrations;
  DiffusionMatrixSet m_InstantSubMatrices;
  DiffusionMatrixSet m_SimpleSubMatrices;

  std::vector<DiffusionCompartmentSet> m_DiffusionSets;

  Eigen::VectorXd m_PermeabilityCoefficients;
  Eigen::VectorXd m_TissueMasses_g;
  Eigen::VectorXd m_VolumeRatiosVascularExtra;
  Eigen::VectorXd m_VolumeRatiosExtraIntra;
};
}
