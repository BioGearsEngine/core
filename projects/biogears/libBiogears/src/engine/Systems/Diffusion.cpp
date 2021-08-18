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
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>

namespace biogears {

auto DiffusionCalculator::make_unique(BioGears& bg) -> std::unique_ptr<DiffusionCalculator>
{
  return std::unique_ptr<DiffusionCalculator>(new DiffusionCalculator(bg));
}
//-------------------------------------------------------------------------------
DiffusionCalculator::DiffusionCalculator(BioGears& bg)
  : Loggable(bg.GetLogger())
  , m_data(bg)
{
  m_dt_s = m_data.GetTimeStep().GetValue(TimeUnit::s);
}
//-------------------------------------------------------------------------------
DiffusionCalculator::~DiffusionCalculator()
{
}
//-------------------------------------------------------------------------------
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
      if (sub->GetMolarMass(MassPerAmountUnit::g_Per_mol) < 1000.0 && sub->GetName() != "Bicarbonate" && sub->GetClassification() != CDM::enumSubstanceClass::WholeBlood) {
        //We elect not to transport HCO3 with simple diffusion because it is a charged substance.   We also need to filter out blood antigens
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculateLinearDiffusionMethods()
{
  Eigen::MatrixXd DeltaMassInstantVE_ug = (m_InstantSubMatrices.vascular - m_InstantSubMatrices.extracellular) * m_VolumeRatiosVascularExtra.asDiagonal();
  Eigen::MatrixXd DeltaMassInstantEI_ug = (m_InstantSubMatrices.extracellular - m_InstantSubMatrices.intracellular) * m_VolumeRatiosExtraIntra.asDiagonal();

  Eigen::MatrixXd DeltaMassSimpleVE_ug = m_dt_s * m_PermeabilityCoefficients.asDiagonal() * (m_SimpleSubMatrices.vascular - m_SimpleSubMatrices.extracellular) * m_TissueMasses_g.asDiagonal();
  Eigen::MatrixXd DeltaMassSimpleEI_ug = m_dt_s * m_PermeabilityCoefficients.asDiagonal() * (m_SimpleSubMatrices.extracellular - m_SimpleSubMatrices.intracellular) * m_TissueMasses_g.asDiagonal();

  double massToMoveVE_ug = 0.0;
  double massToMoveEI_ug = 0.0;
  int colNumber = 0;
  //Containers for substance quantities
  SELiquidSubstanceQuantity* vascularSubQ;
  SELiquidSubstanceQuantity* extracellularSubQ;
  SELiquidSubstanceQuantity* intracellularSubQ;

  for (auto diffSet : m_DiffusionSets) {
    int rowNumber = 0;
    for (auto instantSub : m_InstantDiffusionSubstances) {
      massToMoveVE_ug = DeltaMassInstantVE_ug(rowNumber, colNumber);
      massToMoveEI_ug = DeltaMassInstantEI_ug(rowNumber, colNumber);
      vascularSubQ = diffSet.vascular->GetSubstanceQuantity(*instantSub);
      extracellularSubQ = diffSet.extracellular->GetSubstanceQuantity(*instantSub);
      intracellularSubQ = diffSet.intracellular->GetSubstanceQuantity(*instantSub);

      //Bound massToMoveVE:  >0 --> leaving vascular (bounded by available in vascular), <0 means leaving extracellular (bounded by available in extracellular)
      if (massToMoveVE_ug > 0.0) {
        if (vascularSubQ->GetMass(MassUnit::ug) - massToMoveVE_ug < ZERO_APPROX) {
          massToMoveVE_ug = vascularSubQ->GetMass(MassUnit::ug);
        }
      } else {
        if (extracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveVE_ug) < ZERO_APPROX) {
          massToMoveVE_ug = -extracellularSubQ->GetMass(MassUnit::ug);
        }
      }
      //Bound massToMoveEI:  >0 --> leaving extracellular (bounded by available extracelllar), < 0 means leaving intracellular (bounded by available intracellular)
      if (massToMoveEI_ug > 0.0) {
        if (extracellularSubQ->GetMass(MassUnit::ug) - massToMoveEI_ug < ZERO_APPROX) {
          massToMoveEI_ug = extracellularSubQ->GetMass(MassUnit::ug);
        }
      } else {
        if (intracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveEI_ug) < ZERO_APPROX) {
          massToMoveEI_ug = -intracellularSubQ->GetMass(MassUnit::ug);
        }
      }

      //For all distribute methods, we use VolumeWeighted when mass is entering a compartment and MassWeighted when mass is exiting
      if (!diffSet.vascular->HasChildren()) {
        vascularSubQ->GetMass().IncrementValue(-massToMoveVE_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is leaving vascular, mass < 0 --> mass is entering vascular
        massToMoveVE_ug > 0.0 ? DistributeMassbyMassWeighted(*diffSet.vascular, *instantSub, -massToMoveVE_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(*diffSet.vascular, *instantSub, -massToMoveVE_ug, MassUnit::ug);
      }
      if (!diffSet.extracellular->HasChildren()) {
        extracellularSubQ->GetMass().IncrementValue(massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      } else {
        //(MassVE - MassEI) > 0 --> net mass is entering extracellular, < 0 --> net mass is leaving extracellular
        (massToMoveVE_ug - massToMoveEI_ug) > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.extracellular, *instantSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.extracellular, *instantSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      }
      if (!diffSet.intracellular->HasChildren()) {
        intracellularSubQ->GetMass().IncrementValue(massToMoveEI_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is entering intracellular,  < 0 --> mass is leaving intracellular
        massToMoveEI_ug > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.intracellular, *instantSub, massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.intracellular, *instantSub, massToMoveEI_ug, MassUnit::ug);
      }
      vascularSubQ->Balance(BalanceLiquidBy::Mass);
      extracellularSubQ->Balance(BalanceLiquidBy::Mass);
      intracellularSubQ->Balance(BalanceLiquidBy::Mass);
      rowNumber++;
    }
    rowNumber = 0;
    for (auto simpleSub : m_SimpleDiffusionSubstances) {
      massToMoveVE_ug = DeltaMassSimpleVE_ug(rowNumber, colNumber);
      massToMoveEI_ug = simpleSub->GetClassification() == CDM::enumSubstanceClass::Ion ? 0.0 : DeltaMassSimpleEI_ug(rowNumber, colNumber);
      vascularSubQ = diffSet.vascular->GetSubstanceQuantity(*simpleSub);
      extracellularSubQ = diffSet.extracellular->GetSubstanceQuantity(*simpleSub);
      intracellularSubQ = diffSet.intracellular->GetSubstanceQuantity(*simpleSub);

      //Bound massToMoveVE:  >0 --> leaving vascular (bounded by available in vascular), <0 means leaving extracellular (bounded by available in extracellular)
      if (massToMoveVE_ug > 0.0) {
        if (vascularSubQ->GetMass(MassUnit::ug) - massToMoveVE_ug < ZERO_APPROX) {
          massToMoveVE_ug = vascularSubQ->GetMass(MassUnit::ug);
        }
      } else {
        if (extracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveVE_ug) < ZERO_APPROX) {
          massToMoveVE_ug = -extracellularSubQ->GetMass(MassUnit::ug);
        }
      }
      //Bound massToMoveEI:  >0 --> leaving extracellular (bounded by available extracelllar), < 0 means leaving intracellular (bounded by available intracellular)
      if (massToMoveEI_ug > 0.0) {
        if (extracellularSubQ->GetMass(MassUnit::ug) - massToMoveEI_ug < ZERO_APPROX) {
          massToMoveEI_ug = extracellularSubQ->GetMass(MassUnit::ug);
        }
      } else {
        if (intracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveEI_ug) < ZERO_APPROX) {
          massToMoveEI_ug = -intracellularSubQ->GetMass(MassUnit::ug);
        }
      }

      //For all distribute methods, we use VolumeWeighted when mass is entering a compartment and MassWeighted when mass is exiting
      if (!diffSet.vascular->HasChildren()) {
        vascularSubQ->GetMass().IncrementValue(-massToMoveVE_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is leaving vascular, mass < 0 --> mass is entering vascular
        massToMoveVE_ug > 0.0 ? DistributeMassbyMassWeighted(*diffSet.vascular, *simpleSub, -massToMoveVE_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(*diffSet.vascular, *simpleSub, -massToMoveVE_ug, MassUnit::ug);
      }
      if (!diffSet.extracellular->HasChildren()) {
        extracellularSubQ->GetMass().IncrementValue(massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      } else {
        //(MassVE - MassEI) > 0 --> net mass is entering extracellular, < 0 --> net mass is leaving extracellular
        (massToMoveVE_ug - massToMoveEI_ug) > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.extracellular, *simpleSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.extracellular, *simpleSub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
      }
      //Extracellular->Intracellular transport of ions happens in Tissue::CoupledIonTransport
      //We do need to diffuse extracellular ion mass that transfers with vascular, but we do not want to balance yet
      //Ion balance on both extra and intra happens in CoupledIonTransport
      if (simpleSub->GetClassification() == CDM::enumSubstanceClass::Ion) {
        vascularSubQ->Balance(BalanceLiquidBy::Mass);
        rowNumber++;
        continue;
      }
      //End of ion break
      if (!diffSet.intracellular->HasChildren()) {
        diffSet.intracellular->GetSubstanceQuantity(*simpleSub)->GetMass().IncrementValue(massToMoveEI_ug, MassUnit::ug);
      } else {
        //Mass > 0 --> mass is entering intracellular,  < 0 --> mass is leaving intracellular
        massToMoveEI_ug > 0.0 ? DistributeMassbyVolumeWeighted(*diffSet.intracellular, *simpleSub, massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(*diffSet.intracellular, *simpleSub, massToMoveEI_ug, MassUnit::ug);
      }
      vascularSubQ->Balance(BalanceLiquidBy::Mass);
      extracellularSubQ->Balance(BalanceLiquidBy::Mass);
      intracellularSubQ->Balance(BalanceLiquidBy::Mass);
      rowNumber++;
    }
    colNumber++;
  }
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculateNonLinearDiffusionMethods()
{
  SESubstance* albumin = &m_data.GetSubstances().GetAlbumin(); //Many albumin-specific calls, so make a variable for it
  for (auto diffSet : m_DiffusionSets) {
    CalculateActiveIonDiffusion(diffSet);
    for (auto facilitatedSub : m_FacilitatedDiffusionSubstances) {
      if (facilitatedSub->GetName() == "Triacylglycerol" && diffSet.vascular->GetName() == BGE::VascularCompartment::Brain)
        continue;
      double massToAreaCoefficient_cm2_Per_g = 1.0; /// \todo Define relationship between tissue mass and membrane area.
      double capCoverage_cm2 = massToAreaCoefficient_cm2_Per_g * diffSet.tissue->GetTotalMass(MassUnit::g);
      double maximumMassFlux = facilitatedSub->GetMaximumDiffusionFlux(MassPerAreaTimeUnit::g_Per_cm2_s);
      double combinedCoefficient_g_Per_s = maximumMassFlux * capCoverage_cm2;
      CalculateFacilitatedDiffusion(diffSet, *facilitatedSub, combinedCoefficient_g_Per_s);
    }

    const SESubstanceTissuePharmacokinetics* tissueKinetics;
    for (auto pkSub : m_data.GetSubstances().GetActiveDrugs()) {
      tissueKinetics = &pkSub->GetPK().GetTissueKinetics(diffSet.tissue->GetName());
      //Check to see if substance is a drug with the appropriate parameters to calculate PK diffusion
      // If the substance is a PBPK drug, then diffusion is computed by perfusion limited diffusion, as described in \cite huisinga2012modeling
      if (tissueKinetics != nullptr) {
        if (!tissueKinetics->HasPartitionCoefficient()) {
          Error("Attempted to diffuse a substance with PK that had no partition coefficient available.");
          continue;
        }
        CalculatePerfusionLimitedDiffusion(diffSet, *pkSub, tissueKinetics->GetPartitionCoefficient()); //Balance happens in the method
      }
    }
    //Currently only albumin uses MacromoleculeDiffusion and PassiveLymphDiffusion
    CalculateMacromoleculeDiffusion(diffSet, *albumin);
    CalculatePassiveLymphDiffusion(*diffSet.extracellular, *m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph), *albumin);
    diffSet.vascular->GetSubstanceQuantity(*albumin)->Balance(BalanceLiquidBy::Mass);
    diffSet.extracellular->GetSubstanceQuantity(*albumin)->Balance(BalanceLiquidBy::Mass);
    //Wait to balance lymph until we have calculate lymph -> vena cava transport
  }
  //Finally, we need to move albumin from lymph back to vena cava
  CalculatePassiveLymphDiffusion(*m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph), *m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava), *albumin);
  m_data.GetCompartments().GetLiquidCompartment(BGE::LymphCompartment::Lymph)->GetSubstanceQuantity(*albumin)->Balance(BalanceLiquidBy::Mass);
  m_data.GetCompartments().GetLiquidCompartment(BGE::VascularCompartment::VenaCava)->GetSubstanceQuantity(*albumin)->Balance(BalanceLiquidBy::Mass);
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculateFacilitatedDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub, double combinedCoefficient_g_Per_s)
{
  SELiquidCompartment& vascular = *cmptSet.vascular;
  SELiquidCompartment& extracellular = *cmptSet.extracellular;
  SELiquidCompartment& intracellular = *cmptSet.intracellular;

  SELiquidSubstanceQuantity* vascularSubQ = vascular.GetSubstanceQuantity(sub);
  SELiquidSubstanceQuantity* extracellularSubQ = extracellular.GetSubstanceQuantity(sub);
  SELiquidSubstanceQuantity* intracellularSubQ = intracellular.GetSubstanceQuantity(sub);

  double massToMoveVE_ug = 1.0e6 * combinedCoefficient_g_Per_s * (vascularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - extracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))
    / (sub.GetMichaelisCoefficient() + (vascularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - extracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))) * m_dt_s;

  double massToMoveEI_ug = 1.0e6 * combinedCoefficient_g_Per_s * (extracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - intracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))
    / (sub.GetMichaelisCoefficient() + (extracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL) - intracellularSubQ->GetConcentration(MassPerVolumeUnit::g_Per_mL))) * m_dt_s;

  //Bound massToMoveVE:  >0 --> leaving vascular (bounded by available in vascular), <0 means leaving extracellular (bounded by available in extracellular)
  if (massToMoveVE_ug > 0.0) {
    if (vascularSubQ->GetMass(MassUnit::ug) - massToMoveVE_ug < ZERO_APPROX) {
      massToMoveVE_ug = vascularSubQ->GetMass(MassUnit::ug);
    }
  } else {
    if (extracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveVE_ug) < ZERO_APPROX) {
      massToMoveVE_ug = -extracellularSubQ->GetMass(MassUnit::ug);
    }
  }
  //Bound massToMoveEI:  >0 --> leaving extracellular (bounded by available extracelllar), < 0 means leaving intracellular (bounded by available intracellular)
  if (massToMoveEI_ug > 0.0) {
    if (extracellularSubQ->GetMass(MassUnit::ug) - massToMoveEI_ug < ZERO_APPROX) {
      massToMoveEI_ug = extracellularSubQ->GetMass(MassUnit::ug);
    }
  } else {
    if (intracellularSubQ->GetMass(MassUnit::ug) - (-massToMoveEI_ug) < ZERO_APPROX) {
      massToMoveEI_ug = -intracellularSubQ->GetMass(MassUnit::ug);
    }
  }

  //For all distribute methods, we use VolumeWeighted when mass is entering a compartment and MassWeighted when mass is exiting
  if (!vascular.HasChildren()) {
    vascularSubQ->GetMass().IncrementValue(-massToMoveVE_ug, MassUnit::ug);
  } else {
    //massToMoveVE > 0 --> leaving vascular, < 0 --> entering vascular
    massToMoveVE_ug > 0.0 ? DistributeMassbyMassWeighted(vascular, sub, -massToMoveVE_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(vascular, sub, -massToMoveVE_ug, MassUnit::ug);
  }
  if (!extracellular.HasChildren()) {
    extracellularSubQ->GetMass().IncrementValue(massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
  } else {
    //(massToMoveVE - massToMoveEI) > 0 --> entering extracellular, < 0 --> leaving extracellular
    (massToMoveVE_ug - massToMoveEI_ug) > 0.0 ? DistributeMassbyVolumeWeighted(extracellular, sub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(extracellular, sub, massToMoveVE_ug - massToMoveEI_ug, MassUnit::ug);
  }
  if (!intracellular.HasChildren()) {
    intracellularSubQ->GetMass().IncrementValue(massToMoveEI_ug, MassUnit::ug);
  } else {
    //massToMoveEI > 0 --> entering intracellular, < 0 --> leaving intracellular
    massToMoveEI_ug > 0.0 ? DistributeMassbyVolumeWeighted(intracellular, sub, massToMoveEI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(intracellular, sub, massToMoveEI_ug, MassUnit::ug);
  }

  vascularSubQ->Balance(BalanceLiquidBy::Mass);
  extracellularSubQ->Balance(BalanceLiquidBy::Mass);
  intracellularSubQ->Balance(BalanceLiquidBy::Mass);
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculatePerfusionLimitedDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub, double partitionCoeff)
{
  SETissueCompartment& tissue = *cmptSet.tissue;
  SELiquidCompartment& vascular = *cmptSet.vascular;
  SELiquidCompartment& intracellular = *cmptSet.intracellular;
  SELiquidSubstanceQuantity* vSubQ = vascular.GetSubstanceQuantity(sub);
  if (vSubQ == nullptr)
    throw CommonDataModelException("No Vascular Substance Quantity found for substance " + std::string { sub.GetName() });
  SELiquidSubstanceQuantity* tSubQ = intracellular.GetSubstanceQuantity(sub);
  if (tSubQ == nullptr)
    throw CommonDataModelException("No Tissue-Intracellular Substance Quantity found for substance " + std::string { sub.GetName() });

  double VascularFlow_m_LPer_s = vascular.GetInFlow(VolumePerTimeUnit::mL_Per_s);
  double VascularConcentration_ug_Per_mL = vSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  double tisDensity_kg_Per_L = 1.0;
  if (tissue.GetName() == BGE::TissueCompartment::Fat) {
    tisDensity_kg_Per_L = 0.92;
  }
  if (tissue.GetName() == BGE::TissueCompartment::Bone) {
    tisDensity_kg_Per_L = 1.3;
  }
  //Getting tissue volume by totalMass/density because tissue compartments do not store a volume ("matrix volume" is not the whole fluid volume)
  //Even though we store drug in intracellular cmpt, PK equations are derived assuming total tissue volume
  double tisVolume_mL = tissue.GetTotalMass(MassUnit::kg) / tisDensity_kg_Per_L * 1000.0;
  double TissueConcentration_ug_Per_mL = tSubQ->GetMass(MassUnit::ug) / tisVolume_mL;
  double massToMoveVI_ug = 0;
  if (!(partitionCoeff == 0)) {
    massToMoveVI_ug = VascularFlow_m_LPer_s * m_dt_s * (VascularConcentration_ug_Per_mL - (TissueConcentration_ug_Per_mL / partitionCoeff));
  } else {
    massToMoveVI_ug = 0;
  }

  //Bound massToMoveVI:  >0 --> leaving vascular (bounded by available in vascular), <0 means leaving extracellular (bounded by available in extracellular)
  if (massToMoveVI_ug > 0.0) {
    if (vSubQ->GetMass(MassUnit::ug) - massToMoveVI_ug < ZERO_APPROX) {
      massToMoveVI_ug = vSubQ->GetMass(MassUnit::ug);
    }
  } else {
    if (tSubQ->GetMass(MassUnit::ug) - (-massToMoveVI_ug) < ZERO_APPROX) {
      massToMoveVI_ug = -tSubQ->GetMass(MassUnit::ug);
    }
  }

  //For all distribute methods, we use VolumeWeighted when mass is entering a compartment and MassWeighted when mass is exiting
  if (!vascular.HasChildren()) {
    vSubQ->GetMass().IncrementValue(-massToMoveVI_ug, MassUnit::ug);
  } else {
    //Mass > 0 --> mass is leaving vascular, mass < 0 --> mass is entering vascular
    massToMoveVI_ug > 0.0 ? DistributeMassbyMassWeighted(vascular, sub, -massToMoveVI_ug, MassUnit::ug) : DistributeMassbyVolumeWeighted(vascular, sub, -massToMoveVI_ug, MassUnit::ug);
  }
  if (!intracellular.HasChildren()) {
    tSubQ->GetMass().IncrementValue(massToMoveVI_ug, MassUnit::ug);
  } else {
    //Mass > 0 --> mass is entering intracellular, mass < 0 --> mass is leaving intracellular
    massToMoveVI_ug > 0.0 ? DistributeMassbyVolumeWeighted(intracellular, sub, massToMoveVI_ug, MassUnit::ug) : DistributeMassbyMassWeighted(intracellular, sub, massToMoveVI_ug, MassUnit::ug);
  }

  vSubQ->Balance(BalanceLiquidBy::Mass);
  tSubQ->Balance(BalanceLiquidBy::Mass);
}
//-------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the mass transport of ionic species Na, K, Ca, Cl against their electrochemical gradient using active pumps and cotransport
///
/// \param tissue: tissue compartment
/// \param vascular: vascular space adjoining tissue compartment
/// \param extra: extracellular space in tissue compartment
/// \param intra:  intracellular space in tissue compartment
/// \param timestep_s: the time step
///
/// \details
/// This active transport moves ions to maintain their resting intracellular and extracellular concentrations using the cell membrane potential and active transport.
/// The current experience by each ion is calculated by comparing its nernst potential to the membrane potential.  Active pumps are then called which have been tuned
/// to drive ionic current to maintain homeostatic concentrations.  Since this method replaces diffusion for Na, Ca, K, and Cl, transport from the extracellular space
/// to the vascular space is also handled here and balancing of the ions is also performed.
//--------------------------------------------------------------------------------------------------
void DiffusionCalculator::CalculateActiveIonDiffusion(DiffusionCompartmentSet& cmptSet)
{
  SETissueCompartment& tissue = *cmptSet.tissue;
  SELiquidCompartment& extra = *cmptSet.extracellular;
  SELiquidCompartment& intra = *cmptSet.intracellular;
  //All terms are on the basis of a single cell using model from Yi2003Mathematical--these will be converted to macro level using reported cell volume/surface area ratio from same source
  double faradaysConstant_C_Per_mol = 96485;
  double idealGasConstant = 8.314; //Amazingly, R has same value in J/mol-K and mL-Pa/K-umol
  double temperature_K = 310.0; //Replace w/ call to body temperature
  double membranePotential_V = tissue.GetMembranePotential(ElectricPotentialUnit::V);
  double cellSurfaceArea_cm2 = 7.7e-5; //From Yi2003Mathematical
  double scaleFactor = tissue.GetTotalMass(MassUnit::kg) * 1e6 / cellSurfaceArea_cm2; //Carlson reports 1*10^6 cm^2 / kg tissue relationship

  //Define ions
  SESubstance* sodium = &m_data.GetSubstances().GetSodium();
  SESubstance* potassium = &m_data.GetSubstances().GetPotassium();
  SESubstance* chloride = &m_data.GetSubstances().GetChloride();
  SESubstance* calcium = &m_data.GetSubstances().GetCalcium();

  //Get all ion concentrations
  double naExtracellular_mM = extra.GetSubstanceQuantity(*sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double naIntracellular_mM = intra.GetSubstanceQuantity(*sodium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double kExtracellular_mM = extra.GetSubstanceQuantity(*potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double kIntracellular_mM = intra.GetSubstanceQuantity(*potassium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double clExtracellular_mM = extra.GetSubstanceQuantity(*chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  double clIntracellular_mM = intra.GetSubstanceQuantity(*chloride)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  //double calciumExtracellular_mM = extra.GetSubstanceQuantity(*calcium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);
  //double calciumIntracellular_mM = intra.GetSubstanceQuantity(*calcium)->GetMolarity(AmountPerVolumeUnit::mmol_Per_L);

  /// \ @cite lindblad 1996model
  double naConductance_mC_Per_V_min = 2.925e-5;
  double kConductance_mC_Per_V_min = 7.0e-4;
  double clConductance_mC_Per_V_min = 2.81e-4;
  //double calciumConductance_S_Per_mL = 1.0 / (calcium->GetMembraneResistance(ElectricResistanceUnit::Ohm));

  //Calculate current Nernst potentials for each ion (Faradays constant is multiplied by the charge of the ion--1 for Na and K, -1 for Cl)
  double coreTemp_K = m_data.GetEnergy().GetCoreTemperature(TemperatureUnit::K);
  double naNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, sodium, coreTemp_K);
  double kNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, potassium, coreTemp_K);
  double clNernst_V = GeneralMath::CalculateNernstPotential(extra, intra, chloride, coreTemp_K);
  //double calciumNernst_V = CalculateNernstPotential(extra, intra, calcium, coreTemp_K);

  //Calculate pump rate using information from previous time step--this will lag a time step behind everything but it
  //shouldn't make a huge difference
  double NaKPumpRate_umol_Per_min = SodiumPotassiumPump(naIntracellular_mM, naExtracellular_mM, kExtracellular_mM, membranePotential_V);

  //Calculate new membrane potential (uses previous pump rate)
  membranePotential_V = (naConductance_mC_Per_V_min * naNernst_V + kConductance_mC_Per_V_min * kNernst_V + clConductance_mC_Per_V_min * clNernst_V - faradaysConstant_C_Per_mol * NaKPumpRate_umol_Per_min / 1000.0) / (naConductance_mC_Per_V_min + kConductance_mC_Per_V_min + clConductance_mC_Per_V_min);
  tissue.GetMembranePotential().SetValue(membranePotential_V, ElectricPotentialUnit::V);

  //Calculate diffusion fluxes of each ion through channels in umol/min with respect to intracellular compartment
  //Units:  1000 factor converts mol/C to umol/mC
  double naFlux_umol_Per_min = -((1000.0 * naConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - naNernst_V) + 3 * NaKPumpRate_umol_Per_min);
  double kFlux_umol_Per_min = -((1000.0 * kConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - kNernst_V) - 2 * NaKPumpRate_umol_Per_min);
  double clFlux_umol_Per_min = (1000.0 * clConductance_mC_Per_V_min / faradaysConstant_C_Per_mol) * (membranePotential_V - clNernst_V);
  //double calciumDiffusion_mol_Per_mL_s = calciumConductance_S_Per_mL*(membranePotential_V - calciumNernst_V) / (faradaysConstant_C_Per_mol * 2);

  //This commented out block of code takes regulatory volume increase and decrease into account.  In essence, cells can rapidly transport
  //ions through channels to oppose a change in volume caused by a large change in osmotic environment.  It's not clear if we need this as of now
  //but the code is here if we want to turn it on.

  //These constants from Hernandez1998Modeling are used to determined regulatory volume increase/decrease.
  //Boundaries are from Hoffman1989Membrane

  /*double qNa_mL6_Per_umol_min = 0.1 * 1.0e-6 * 60.0 * cellSurfaceArea_cm2;    //Value reported = 0.1 mL^4/mol-s --> Convert to mL^6/umol-min to fit with units later on
        double qK_mL6_Per_umol_min = 10.0 * 1.0e-6 * 60.0 * cellSurfaceArea_cm2;
        double volLimitUpper_mL = 1.01*intracellularVolumeBaseline_mL;
        double volLimitLower_mL = 0.99*intracellularVolumeBaseline_mL;
        if (intracellularVolume_mL <= volLimitLower_mL)
        {
          qNa_mL6_Per_umol_min *= (volLimitLower_mL - intracellularVolume_mL) / volLimitLower_mL;
          qK_mL6_Per_umol_min = 0.0;
        }
        else if (intracellularVolume_mL >= volLimitUpper_mL)
        {
          qNa_mL6_Per_umol_min = 0.0;
          qK_mL6_Per_umol_min *= (intracellularVolume_mL - volLimitUpper_mL) / volLimitUpper_mL;
        }
        else
        {
          qNa_mL6_Per_umol_min = 0.0;
          qK_mL6_Per_umol_min = 0.0;
        }

        double psiNa_umol_Per_min = qNa_mL6_Per_umol_min*(naExtracellular_mM*clExtracellular_mM - naIntracellular_mM*clIntracellular_mM);
        double psiK_umol_Per_min = qK_mL6_Per_umol_min*(kExtracellular_mM*clExtracellular_mM - kIntracellular_mM*clIntracellular_mM);

        naFlux_umol_Per_min += psiNa_umol_Per_min;
        kFlux_umol_Per_min += psiK_umol_Per_min;
        clFlux_umol_Per_min += (psiNa_umol_Per_min + psiK_umol_Per_min);*/
  //Cl flux is coupled to both of these process to maintain electroneutrality

  //End of optional RVI/RVD functionality

  //Here is what we actually move at each time step, based on the calculated rate, and scaled up from cellular level to tissue
  double naIncrement_ug = (naFlux_umol_Per_min * sodium->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_dt_s / 60.0) * scaleFactor;
  double kIncrement_ug = (kFlux_umol_Per_min * potassium->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_dt_s / 60.0) * scaleFactor;
  double clIncrement_ug = (clFlux_umol_Per_min * chloride->GetMolarMass(MassPerAmountUnit::g_Per_mol)) * (m_dt_s / 60.0) * scaleFactor;

  SESubstance* ion;
  double incrementer_ug = 0.0;
  double toVascular_ug = 0.0;
  std::map<SESubstance*, double> ionIncrements = { { sodium, naIncrement_ug }, { potassium, kIncrement_ug }, { chloride, clIncrement_ug } };
  for (auto ions : ionIncrements) {
    ion = ions.first;
    incrementer_ug = ions.second;

    if (incrementer_ug > 0.0) {
      //Net flux is direction extra-->intra
      if (incrementer_ug > extra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug)) {
        //Make sure we don't take it more than is there (shouldn't happen but you never know)
        incrementer_ug = extra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug);
      }
      DistributeMassbyMassWeighted(extra, *ion, -incrementer_ug, MassUnit::ug);
      DistributeMassbyVolumeWeighted(intra, *ion, incrementer_ug, MassUnit::ug);
    } else {
      //Net flux is direction intra-->extra
      if (-incrementer_ug > intra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug)) {
        incrementer_ug = -intra.GetSubstanceQuantity(*ion)->GetMass(MassUnit::ug);
      }
      DistributeMassbyVolumeWeighted(extra, *ion, -incrementer_ug, MassUnit::ug);
      DistributeMassbyMassWeighted(intra, *ion, incrementer_ug, MassUnit::ug);
    }

    extra.GetSubstanceQuantity(*ion)->Balance(BalanceLiquidBy::Mass);
    intra.GetSubstanceQuantity(*ion)->Balance(BalanceLiquidBy::Mass);
  }
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculateMacromoleculeDiffusion(DiffusionCompartmentSet& cmptSet, const SESubstance& sub)
{
  SETissueCompartment& tissue = *cmptSet.tissue;
  SELiquidCompartment& vascular = *cmptSet.vascular;
  SELiquidCompartment& extra = *cmptSet.extracellular;

  //Parameters are specific to albumin--would need to be put on per substance basis if this method is extended to other macromolecules
  double diffusivityCoefficient_mL_Per_min_kg = 0.03306;
  double reflectionCoefficientSmallBase = 0.954;
  double reflectionCoefficientLargeBase = 0.097;
  double reflectionCoefficientLarge = reflectionCoefficientLargeBase;
  double reflectionCoefficientSmall = reflectionCoefficientSmallBase;
  double tissueMass_kg = tissue.GetTotalMass(MassUnit::kg);
  //Tissues have names like "BrainTissue"--we need to extract the compartment specific tag (e.g. "Brain") to construct circuit path name that we need
  auto tisPosition = tissue.GetName().find("Tissue");
  std::string tisBase = tissue.GetName().substr(0, tisPosition);
  std::string fluidFluxPathName = tisBase + "E2To" + tisBase + "E3";
  double fluidFlux_mL_Per_min = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(fluidFluxPathName)->GetFlow(VolumePerTimeUnit::mL_Per_min);

  //We need to increase albumin permeability when there is inflammation
  if (m_data.GetBloodChemistry().GetInflammatoryResponse().HasInflammationSources()) {
    double tissueIntegrity = m_data.GetBloodChemistry().GetInflammatoryResponse().GetTissueIntegrity().GetValue();
    reflectionCoefficientSmall = reflectionCoefficientSmallBase * tissueIntegrity;
  }

  BLIM(reflectionCoefficientSmall, 0.0, 1.0);
  BLIM(reflectionCoefficientLarge, 0.0, 1.0);

  double vascular_ug_Per_mL = vascular.GetSubstanceQuantity(sub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);
  double extracellular_ug_Per_mL = extra.GetSubstanceQuantity(sub)->GetConcentration(MassPerVolumeUnit::ug_Per_mL);

  double peclet = fluidFlux_mL_Per_min * (1.0 - reflectionCoefficientSmall) / (diffusivityCoefficient_mL_Per_min_kg * tissueMass_kg);
  double soluteFlux_ug_Per_min = fluidFlux_mL_Per_min * (1.0 - reflectionCoefficientSmall) * ((vascular_ug_Per_mL - extracellular_ug_Per_mL * std::exp(-peclet)) / (1.0 - std::exp(-peclet)));

  double moved_ug = soluteFlux_ug_Per_min / 60.0 * m_dt_s;
  if (moved_ug > 0) {
    if (moved_ug > vascular.GetSubstanceQuantity(sub)->GetMass(MassUnit::ug)) {
      moved_ug = vascular.GetSubstanceQuantity(sub)->GetMass(MassUnit::ug);
    }
    DistributeMassbyVolumeWeighted(extra, sub, moved_ug, MassUnit::ug);
    DistributeMassbyMassWeighted(vascular, sub, -moved_ug, MassUnit::ug);
  } else {
    moved_ug = 0.0;
  }
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::CalculatePassiveLymphDiffusion(SELiquidCompartment& source, SELiquidCompartment& target, const SESubstance& sub)
{

  const SELiquidSubstanceQuantity* sSubQ = source.GetSubstanceQuantity(sub);
  const SELiquidSubstanceQuantity* tSubQ = target.GetSubstanceQuantity(sub);

  double fluidFlow_mL_Per_s = 0.0;
  if (target.GetName() == BGE::VascularCompartment::VenaCava) {
    //If we're going to Vena Cava, we must be using lymph vascular return path
    fluidFlow_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(BGE::TissuePath::LymphToVenaCava)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  } else {
    //Otherwise, we are going from an extracellular compartment to the lymph
    //Source extracellular compartments are named like "BrainTissueExtracellular"--we want to extract the cmpt specific tag (e.g. Brain) to construct lymph path name
    auto tisPosition = source.GetName().find("TissueExtracellular");
    std::string tisBase = source.GetName().substr(0, tisPosition);
    std::string fluidPathName = tisBase + "E3To" + tisBase + "L1";
    fluidFlow_mL_Per_s = m_data.GetCircuits().GetActiveCardiovascularCircuit().GetPath(fluidPathName)->GetFlow(VolumePerTimeUnit::mL_Per_s);
  }

  double massToMove_ug = fluidFlow_mL_Per_s * sSubQ->GetConcentration(MassPerVolumeUnit::ug_Per_mL) * m_dt_s;

  if (massToMove_ug > 0.0) {
    if (massToMove_ug > sSubQ->GetMass(MassUnit::ug)) {
      massToMove_ug = sSubQ->GetMass(MassUnit::ug);
    }
    m_data.GetDiffusionCalculator().DistributeMassbyMassWeighted(source, sub, -massToMove_ug, MassUnit::ug);
    m_data.GetDiffusionCalculator().DistributeMassbyVolumeWeighted(target, sub, massToMove_ug, MassUnit::ug);
  } else // negative increment means it is coming from the target and going to the source
  {
    massToMove_ug = 0.0;
  }
}
//-------------------------------------------------------------------------------
void DiffusionCalculator::ClearConcentrations()
{
  m_SimpleSubConcentrations.vascular_ug_Per_mL.clear();
  m_SimpleSubConcentrations.extracellular_ug_Per_mL.clear();
  m_SimpleSubConcentrations.intracellular_ug_Per_mL.clear();

  m_InstantSubConcentrations.vascular_ug_Per_mL.clear();
  m_InstantSubConcentrations.extracellular_ug_Per_mL.clear();
  m_InstantSubConcentrations.intracellular_ug_Per_mL.clear();
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
double DiffusionCalculator::SodiumPotassiumPump(double intraNa_mM, double extraNa_mM, double extraK_mM, double potential_V)
{
  //Formulation of Luo1994Dynamic used in Yi2002Mathematical
  double maxPumpRate_umol_Per_min = 7.15e-8;
  double potassiumMichaelis_mM = 1.0;
  double sodiumMichaelis_mM = 15.0;

  double KTerm = extraK_mM / (extraK_mM + potassiumMichaelis_mM);
  double NaTerm = 1.0 / (1.0 + std::pow((sodiumMichaelis_mM / intraNa_mM), 1.5));
  double sigma = (1.0 / 7.0) * (exp(extraNa_mM / 69.73) - 1);
  double beta = potential_V * 96485.0 / (8.314 * 310.0);
  double f = 1.0 / (1.0 + 0.1245 * exp(-0.1 * beta) + 0.0365 * sigma * exp(-beta));

  double pumpFlux_umol_Per_min = maxPumpRate_umol_Per_min * f * NaTerm * KTerm;

  return pumpFlux_umol_Per_min;
}
//-------------------------------------------------------------------------------
double DiffusionCalculator::CalciumPump(double intraCa_M)
{
  double maxCurrent_A_Per_mL = 0.0033;
  double calciumMichaelis_M = 2.75e-7;

  double calciumCurrent_A_Per_mL = maxCurrent_A_Per_mL * intraCa_M / (intraCa_M + calciumMichaelis_M);

  return calciumCurrent_A_Per_mL;
}
}