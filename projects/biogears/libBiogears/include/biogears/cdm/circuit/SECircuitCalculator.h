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
#include <Eigen/Core>
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/utils/SmartEnum.h>
#include <biogears/cdm/utils/TimingProfile.h>

#define CIRCUIT_CALCULATOR_TEMPLATE typename CircuitType, typename NodeType, typename PathType, typename CapacitanceUnit, typename FluxUnit, typename InductanceUnit, typename PotentialUnit, typename QuantityUnit, typename ResistanceUnit
#define CIRCUIT_CALCULATOR_TYPES CircuitType, NodeType, PathType, CapacitanceUnit, FluxUnit, InductanceUnit, PotentialUnit, QuantityUnit, ResistanceUnit

namespace biogears {
// These are the Eigen Solvers we can use for solving our circuits
struct BIOGEARS_API EigenCircuitSolver {
  enum Type { Direct,
    PartialPivLu,
    FullPivLu,
    JacobiSvd,
    HouseholderQr,
    Ldlt,
    Llt,
    SparseLU,
    SparseQR,
    BiCGSTAB,
    ConjugateGradient };
  SMART_ENUM(EigenCircuitSolver, Type, 11);
  static char const* Value(size_t idx);
};

template <CIRCUIT_CALCULATOR_TEMPLATE>
class SECircuitCalculator : public Loggable {
public:
  SECircuitCalculator(const CapacitanceUnit& c, const FluxUnit& f, const InductanceUnit& i, const PotentialUnit& p, const QuantityUnit& q, const ResistanceUnit& r, Logger* logger);
  virtual ~SECircuitCalculator();

  virtual void SetEigenSolver(EigenCircuitSolver s) { m_solver = s; };
  virtual void SetEigenSolver(EigenCircuitSolver::Type t) { m_solver.set(t); };

  // Analysis of the Circuit dynamics
  virtual void Process(CircuitType& circuit, double timeStep_s);
  virtual void PostProcess(CircuitType& circuit);

protected:
  /**/ virtual void ParseIn();
  /**/ virtual void Solve();
  /**/ virtual void ParseOut();
  /**/ virtual void CalculateFluxes();
  /**/ virtual void CalculateQuantities();

  // Valve Support
  virtual bool CheckAndModifyValves();
  virtual bool IsCurrentValveStateUnique();
  // Eigen Support
  virtual void PopulateAMatrix(NodeType& nKCL, PathType& p, double dMultiplier, bool hasPotentialSource = false);

  // These are all transient and cleared/set at the start of the process call

  std::stringstream m_ss;
  //Ax=b
  Eigen::MatrixXd m_AMatrix; //A
  Eigen::VectorXd m_xVector; //x
  Eigen::VectorXd m_bVector; //b

  double m_dT_s;
  CircuitType* m_circuit;
  EigenCircuitSolver m_solver;
  std::set<uint64_t> m_valveStates;
  std::map<PathType*, size_t> m_potentialSources;

  const CapacitanceUnit& m_CapacitanceUnit;
  const FluxUnit& m_FluxUnit;
  const InductanceUnit& m_InductanceUnit;
  const PotentialUnit& m_PotentialUnit;
  const QuantityUnit& m_QuantityUnit;
  const ResistanceUnit& m_ResistanceUnit;
};
}
#include <biogears/cdm/circuit/SECircuitCalculator.inl>