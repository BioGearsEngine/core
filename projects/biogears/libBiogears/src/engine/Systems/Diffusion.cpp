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
#include <biogears/engine/Systems/Diffusion.h>

#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/engine/Controller/BioGears.h>

namespace biogears {

DiffusionCalculator::DiffusionCalculator(BioGears& bg)
  : Loggable(bg.GetLogger())
  , m_data(bg)
{
}

void DiffusionCalculator::Initialize(SESubstanceManager& subMgr)
{
  for (auto sub : subMgr.GetActiveSubstances()) {
    if (sub->HasPK()) {
      //Drugs are handled by ActiveDrugs vector in SubManager
      continue;
    }
    if (sub->GetState() == CDM::enumSubstanceState::Gas) {
      m_InstantDiffusionSubstances.emplace_back(sub);
      continue;
    }
    if (sub->HasMaximumDiffusionFlux()) {
      m_FacilitatedDiffusionSubstances.emplace_back(sub);
    } else {
      if (sub->GetMolarMass(MassPerAmountUnit::g_Per_mol) < 1000.0 && sub->GetName() != "Bicarbonate" && sub->GetClassification()!=CDM::enumSubstanceClass::WholeBlood) {
        //We elect not to transport HCO3 with simple diffusion because it is a charged substance and we need to filter out blood antigens
        m_SimpleDiffusionSubstances.emplace_back(sub);
      }
    }
  }

  size_t numSimpleSubs = m_SimpleDiffusionSubstances.size();
  size_t numInstantSubs = m_InstantDiffusionSubstances.size();
  size_t numTissues = m_data.GetCompartments().GetTissueCompartments().size();

  m_PermeabilityCoefficients = Eigen::VectorXd::Zero(numSimpleSubs);
  m_TissueMasses_g = Eigen::VectorXd::Zero(numTissues);
  m_VolumeRatiosVascularExtra = Eigen::VectorXd::Zero(numTissues);
  m_VolumeRatiosExtraIntra = Eigen::VectorXd::Zero(numTissues);

  m_InstantSubMatrices.vascular = Eigen::MatrixXd::Zero(numInstantSubs, numTissues);
  m_InstantSubMatrices.extracellular = Eigen::MatrixXd::Zero(numInstantSubs, numTissues);
  m_InstantSubMatrices.intracellular = Eigen::MatrixXd::Zero(numInstantSubs, numTissues);

  m_SimpleSubMatrices.vascular = Eigen::MatrixXd::Zero(numSimpleSubs, numTissues);
  m_SimpleSubMatrices.extracellular = Eigen::MatrixXd::Zero(numSimpleSubs, numTissues);
  m_SimpleSubMatrices.intracellular = Eigen::MatrixXd::Zero(numSimpleSubs, numTissues);

  SELiquidCompartment* vascular;
  SELiquidCompartment* extracellular;
  SELiquidCompartment* intracellular;
  std::string vascularName;
  int vectorCount = 0;
  for (auto tissue : m_data.GetCompartments().GetTissueCompartments()) {
    extracellular = &m_data.GetCompartments().GetExtracellularFluid(*tissue);
    intracellular = &m_data.GetCompartments().GetIntracellularFluid(*tissue);
    auto tisPosition = tissue->GetName().find("Tissue");
    vascularName = tissue->GetName().replace(tisPosition, tissue->GetName().size() - tisPosition, "Vasculature");
    vascular = m_data.GetCompartments().GetLiquidCompartment(vascularName);
    DiffusionCompartmentSet temp = { tissue, vascular, extracellular, intracellular };
    m_DiffusionSets.emplace_back(temp);
    m_TissueMasses_g(vectorCount) = tissue->GetTotalMass(MassUnit::g);
    vectorCount++;
  }

  int simpleSubCount = 0;
  for (auto simpleSub : m_SimpleDiffusionSubstances) {
    double molarMass_g_Per_mol = simpleSub->GetMolarMass(MassPerAmountUnit::g_Per_mol);
    double molecularRadius_nm = 0.0348 * std::pow(molarMass_g_Per_mol, 0.4175);
    double vToECpermeabilityCoefficient_mL_Per_s_g = 0.0287 * std::pow(molecularRadius_nm, -2.920) / 100.0; // This is only valid if the molecular radius is > 1.0 nm.
    if (molecularRadius_nm < 1.0)
      vToECpermeabilityCoefficient_mL_Per_s_g = 0.0184 * std::pow(molecularRadius_nm, -1.223) / 100.0;
    m_PermeabilityCoefficients(simpleSubCount) = vToECpermeabilityCoefficient_mL_Per_s_g;
    simpleSubCount++;
  }
}

void DiffusionCalculator::SetDiffusionState()
{
  ClearConcentrations();

  size_t simpleRows = m_SimpleDiffusionSubstances.size();
  size_t instantRows = m_InstantDiffusionSubstances.size();
  size_t columns = m_DiffusionSets.size();

  int tissueCount = 0;
  for (auto diffSet : m_DiffusionSets) {
    for (auto instantSub : m_InstantDiffusionSubstances) {
      m_InstantSubConcentrations.vascular_ug_Per_mL.emplace_back(diffSet.vascular->GetSubstanceQuantity(*instantSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
      m_InstantSubConcentrations.extracellular_ug_Per_mL.emplace_back(diffSet.extracellular->GetSubstanceQuantity(*instantSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
      m_InstantSubConcentrations.intracellular_ug_Per_mL.emplace_back(diffSet.intracellular->GetSubstanceQuantity(*instantSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
    }
    for (auto simpleSub : m_SimpleDiffusionSubstances) {
      m_SimpleSubConcentrations.vascular_ug_Per_mL.emplace_back(diffSet.vascular->GetSubstanceQuantity(*simpleSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
      m_SimpleSubConcentrations.extracellular_ug_Per_mL.emplace_back(diffSet.extracellular->GetSubstanceQuantity(*simpleSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
      m_SimpleSubConcentrations.intracellular_ug_Per_mL.emplace_back(diffSet.intracellular->GetSubstanceQuantity(*simpleSub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL));
    }
    m_VolumeRatiosVascularExtra(tissueCount) = (diffSet.vascular->GetVolume(VolumeUnit::mL) * diffSet.extracellular->GetVolume(VolumeUnit::mL)) / (diffSet.vascular->GetVolume(VolumeUnit::mL) + diffSet.extracellular->GetVolume(VolumeUnit::mL));
    m_VolumeRatiosExtraIntra(tissueCount) = (diffSet.extracellular->GetVolume(VolumeUnit::mL) * diffSet.intracellular->GetVolume(VolumeUnit::mL)) / (diffSet.extracellular->GetVolume(VolumeUnit::mL) + diffSet.intracellular->GetVolume(VolumeUnit::mL));
    tissueCount++;
  }
  m_InstantSubMatrices.vascular = Eigen::Map<Eigen::MatrixXd>(m_InstantSubConcentrations.vascular_ug_Per_mL.data(), instantRows, columns);
  m_InstantSubMatrices.extracellular = Eigen::Map<Eigen::MatrixXd>(m_InstantSubConcentrations.extracellular_ug_Per_mL.data(), instantRows, columns);
  m_InstantSubMatrices.intracellular = Eigen::Map<Eigen::MatrixXd>(m_InstantSubConcentrations.intracellular_ug_Per_mL.data(), instantRows, columns);

  m_SimpleSubMatrices.vascular = Eigen::Map<Eigen::MatrixXd>(m_SimpleSubConcentrations.vascular_ug_Per_mL.data(), simpleRows, columns);
  m_SimpleSubMatrices.extracellular = Eigen::Map<Eigen::MatrixXd>(m_SimpleSubConcentrations.extracellular_ug_Per_mL.data(), simpleRows, columns);
  m_SimpleSubMatrices.intracellular = Eigen::Map<Eigen::MatrixXd>(m_SimpleSubConcentrations.intracellular_ug_Per_mL.data(), simpleRows, columns);
}

void DiffusionCalculator::CalculateInstantAndSimpleDiffusion()
{

  double timeStep_s = m_data.GetTimeStep().GetValue(TimeUnit::s);

  Eigen::MatrixXd DeltaMassInstantVE_ug = (m_InstantSubMatrices.vascular - m_InstantSubMatrices.extracellular) * m_VolumeRatiosVascularExtra.asDiagonal();
  Eigen::MatrixXd DeltaMassInstantEI_ug = (m_InstantSubMatrices.extracellular - m_InstantSubMatrices.intracellular) * m_VolumeRatiosExtraIntra.asDiagonal();

  Eigen::MatrixXd DeltaMassSimpleVE_ug = timeStep_s * m_PermeabilityCoefficients.asDiagonal() * (m_SimpleSubMatrices.vascular - m_SimpleSubMatrices.extracellular) * m_TissueMasses_g.asDiagonal();
  Eigen::MatrixXd DeltaMassSimpleEI_ug = timeStep_s * m_PermeabilityCoefficients.asDiagonal() * (m_SimpleSubMatrices.extracellular - m_SimpleSubMatrices.intracellular) * m_TissueMasses_g.asDiagonal();

  double massToMoveVE_ug = 0.0;
  double massToMoveEI_ug = 0.0;
  int colNumber = 0;
  for (auto diffSet : m_DiffusionSets) {
    int rowNumber = 0;
    for (auto instantSub : m_InstantDiffusionSubstances) {
      massToMoveVE_ug = DeltaMassInstantVE_ug(rowNumber, colNumber);
      massToMoveEI_ug = DeltaMassInstantEI_ug(rowNumber, colNumber);
      if (!diffSet.vascular->HasChildren()) {
        diffSet.vascular->GetSubstanceQuantity(*instantSub)->GetMass().IncrementValue(-massToMoveVE_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is leaving vascular, mass < 0 --> mass is entering vascular
        massToMoveVE_ug > 0.0 ? DistributeMassbyMassWeighted(*diffSet.vascular, *instantSub, -massToMoveVE_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(*diffSet.vascular, *instantSub, -massToMoveVE_ug, MassUnit::ug);
      }
      if (!diffSet.extracellular->HasChildren()) {
        diffSet.extracellular->GetSubstanceQuantity(*instantSub)->GetMass().IncrementValue(massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      } else {
        (massToMoveVE_ug - massToMoveEI_ug) > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.extracellular, *instantSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.extracellular, *instantSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      }
      if (!diffSet.intracellular->HasChildren()) {
        diffSet.intracellular->GetSubstanceQuantity(*instantSub)->GetMass().IncrementValue(massToMoveEI_ug, MassUnit::ug);
      } else {
        massToMoveEI_ug > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.intracellular, *instantSub, massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.intracellular, *instantSub, massToMoveEI_ug, MassUnit::ug);
      }
      diffSet.vascular->GetSubstanceQuantity(*instantSub)->Balance(BalanceLiquidBy::Mass);
      diffSet.extracellular->GetSubstanceQuantity(*instantSub)->Balance(BalanceLiquidBy::Mass);
      diffSet.intracellular->GetSubstanceQuantity(*instantSub)->Balance(BalanceLiquidBy::Mass);
      rowNumber++;
    }
    rowNumber = 0;
    for (auto simpleSub : m_SimpleDiffusionSubstances) {
      massToMoveVE_ug = DeltaMassSimpleVE_ug(rowNumber, colNumber);
      massToMoveEI_ug = simpleSub->GetClassification() == CDM::enumSubstanceClass::Ion ? 0.0 : DeltaMassSimpleEI_ug(rowNumber, colNumber);
      if (!diffSet.vascular->HasChildren()) {
        diffSet.vascular->GetSubstanceQuantity(*simpleSub)->GetMass().IncrementValue(-massToMoveVE_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is leaving vascular, mass < 0 --> mass is entering vascular
        massToMoveVE_ug > 0.0 ? DistributeMassbyMassWeighted(*diffSet.vascular, *simpleSub, -massToMoveVE_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(*diffSet.vascular, *simpleSub, -massToMoveVE_ug, MassUnit::ug);
      }
      if (!diffSet.extracellular->HasChildren()) {
        diffSet.extracellular->GetSubstanceQuantity(*simpleSub)->GetMass().IncrementValue(massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      } else {
        (massToMoveVE_ug - massToMoveEI_ug) > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.extracellular, *simpleSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.extracellular, *simpleSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      }
      //Extracellular->Intracellular transport of ions happens in Tissue::CoupledIonTransport
      //We do need to diffuse extracellular ion mass that transfers with vascular, but we do not want to balance yet
      //Ion balance on both extra and intra happens in CoupledIonTransport
      if (simpleSub->GetClassification() == CDM::enumSubstanceClass::Ion) {
        diffSet.vascular->GetSubstanceQuantity(*simpleSub)->Balance(BalanceLiquidBy::Mass);
        rowNumber++;
        continue;
      }
      //End of ion break
      if (!diffSet.intracellular->HasChildren()) {
        diffSet.intracellular->GetSubstanceQuantity(*simpleSub)->GetMass().IncrementValue(massToMoveEI_ug, MassUnit::ug);
      } else {
        massToMoveEI_ug > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.intracellular, *simpleSub, massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.intracellular, *simpleSub, massToMoveEI_ug, MassUnit::ug);
      }
      diffSet.vascular->GetSubstanceQuantity(*simpleSub)->Balance(BalanceLiquidBy::Mass);
      diffSet.extracellular->GetSubstanceQuantity(*simpleSub)->Balance(BalanceLiquidBy::Mass);
      diffSet.intracellular->GetSubstanceQuantity(*simpleSub)->Balance(BalanceLiquidBy::Mass);
      rowNumber++;
    }
    colNumber++;
  }
}

void DiffusionCalculator::ClearConcentrations()
{
  m_SimpleSubConcentrations.vascular_ug_Per_mL.clear();
  m_SimpleSubConcentrations.extracellular_ug_Per_mL.clear();
  m_SimpleSubConcentrations.intracellular_ug_Per_mL.clear();

  m_InstantSubConcentrations.vascular_ug_Per_mL.clear();
  m_InstantSubConcentrations.extracellular_ug_Per_mL.clear();
  m_InstantSubConcentrations.intracellular_ug_Per_mL.clear();
}

void DiffusionCalculator::DistributeMassbyMassWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit)
{
  SELiquidSubstanceQuantity* subQ = cmpt.GetSubstanceQuantity(sub);
  if (mass < 0.0) {
    mass = -mass > subQ->GetMass(unit) ? -subQ->GetMass(unit) : mass;
  }

  if (!cmpt.HasChildren()) {
    subQ->GetMass().IncrementValue(mass, unit);
    if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
      subQ->GetMass().SetValue(0.0, MassUnit::ug);
    }
  } else {
    double mass_ug = subQ->GetMass(MassUnit::ug);
    for (SELiquidCompartment* leaf : cmpt.GetLeaves()) {
      SELiquidSubstanceQuantity* subQ = leaf->GetSubstanceQuantity(sub);
      double leafMass = 0.0;
      if (mass_ug != 0.0) {
        leafMass = mass * (subQ->GetMass(MassUnit::ug) / mass_ug);
      }
      subQ->GetMass().IncrementValue(leafMass, unit);
      if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
        subQ->GetMass().SetValue(0.0, MassUnit::ug);
      }
    }
  }
}

void DiffusionCalculator::DistributeMassbyVolumeWeighted(SELiquidCompartment& cmpt, const SESubstance& sub, double mass, const MassUnit& unit)
{
  SELiquidSubstanceQuantity* subQ = cmpt.GetSubstanceQuantity(sub);
  if (mass < 0.0) {
    if (-mass > subQ->GetMass(unit)) {
      mass = -subQ->GetMass(unit);
      Info("The amount of mass decrement to distribute by volume weighted was greater than available. High probability of negative mass. DistributeMassbyMassWeighted is preferred for decrements.");
    }
  }

  if (!cmpt.HasChildren()) {
    subQ->GetMass().IncrementValue(mass, unit);
    if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
      subQ->GetMass().SetValue(0.0, MassUnit::ug);
    }
  } else {
    double volume_mL = cmpt.GetVolume(VolumeUnit::mL);
    for (SELiquidCompartment* leaf : cmpt.GetLeaves()) {
      double leafMass = mass * (leaf->GetVolume(VolumeUnit::mL) / volume_mL);
      SELiquidSubstanceQuantity* subQ = leaf->GetSubstanceQuantity(sub);
      subQ->GetMass().IncrementValue(leafMass, unit);
      if (std::abs(subQ->GetMass(MassUnit::ug)) < ZERO_APPROX) {
        subQ->GetMass().SetValue(0.0, MassUnit::ug);
      }
    }
  }
}
}