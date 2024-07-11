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

#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitCalculator.h>
#include <biogears/cdm/circuit/SECircuitNode.h>
#include <biogears/cdm/circuit/SECircuitPath.h>
#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarQuantity.h>
#include <biogears/cdm/utils/GeneralMath.h>

#pragma warning(push, 0)  
#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
//#include <Eigen/SparseCholesky>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseQR>
#pragma warning(pop)
#include <bitset>
#include <numeric>

//#define VERBOSE
//#define TIMING
#define OPEN_RESISTANCE 1e100

namespace biogears {
template <CIRCUIT_CALCULATOR_TEMPLATE>
SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::SECircuitCalculator(const CapacitanceUnit& c, const FluxUnit& f, const InductanceUnit& i, const PotentialUnit& p, const QuantityUnit& q, const ResistanceUnit& r, Logger* logger)
  : Loggable(logger)
  , m_CapacitanceUnit(c)
  , m_FluxUnit(f)
  , m_InductanceUnit(i)
  , m_PotentialUnit(p)
  , m_QuantityUnit(q)
  , m_ResistanceUnit(r)
{
  m_solver.set(EigenCircuitSolver::SparseLU);

  //Make sure the base units are compatible
  if (GeneralMath::PercentTolerance(m_ResistanceUnit.GetBigness(), (m_PotentialUnit.GetBigness() / m_FluxUnit.GetBigness())) > 0.001 || GeneralMath::PercentTolerance(m_CapacitanceUnit.GetBigness(), (m_QuantityUnit.GetBigness() / m_PotentialUnit.GetBigness())) > 0.001 || GeneralMath::PercentTolerance(m_InductanceUnit.GetBigness(), (m_ResistanceUnit.GetBigness() * m_QuantityUnit.GetBigness() / m_FluxUnit.GetBigness())) > 0.001) {
    m_ss << "Incompatible base units." << m_CapacitanceUnit << "(" << c.GetBigness() << ") " << m_FluxUnit << "(" << f.GetBigness() << ") " << m_InductanceUnit << "(" << i.GetBigness() << ") " << m_PotentialUnit << "(" << p.GetBigness() << ") " << m_QuantityUnit << "(" << q.GetBigness() << ") " << m_ResistanceUnit << "(" << r.GetBigness() << ") ";
    Fatal(m_ss);
  }
}
template <CIRCUIT_CALCULATOR_TEMPLATE>
SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::~SECircuitCalculator()
{
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculates the Next circuit values.
///
/// \param  Circuit The circuit in question.
///
/// \details
/// Take the Element values and calculate the Node and Path variables for the Next time-step.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::Process(CircuitType& circuit, double timeStep_s)
{
  m_circuit = &circuit;
  m_dT_s = timeStep_s;
  m_valveStates.clear();

  //Reset all Polarized Elements to be shorted.
  for (PathType* p : circuit.GetPolarizedElementPaths()) {
    if (p->HasNextPolarizedState())
      p->SetNextPolarizedState(SEOpenClosed::Closed);
  }

    //When we parse everything into our Ax=b matrices/vectors for the linear solver,
    //the address (row,column) is based on indexes.
    // The circuit has a method for getting a Jacobian index for all nodes except the reference node
    //We'll solve circuits with switches using Assumed Valve (i.e. Diode) States model by thinking of them as switches
    //and checking the resulting Flow and Pressure difference.
    //We'll keep looping until we've either found a solution or determined that it cannot be solved in the current configuration.
#ifdef VERBOSE
  int i = 0;
#endif
  do {
#ifdef VERBOSE
    i++;
#endif
    //We solve for the unknown circuit values this time-step by using Modified Nodal Analysis and linear algebra.
    //All of the source (i.e. Pressure and Flow) values are known, as well as all element (i.e. Resistance, Compliance, Inertance, Switch on/off, Valve direction) values.
    //We'll solve for all of the circuit's node pressures and Pressure Source Flows simultaneously using an error minimization numerical solver.
    //Then we calculate the flows and voltages based on those Pressures and Element values.
    ParseIn();
    Solve();
    ParseOut();
    CalculateFluxes();
  } while (!CheckAndModifyValves());
#ifdef VERBOSE
  std::cout << "Number of Valve Loops = " << i << std::endl;
#endif
  CalculateQuantities();
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Set up the equations to solve the circuit
///
/// \details
/// Define the state-space system, by creating the A Jacobian matrix and b solution vector (Ax=b).
/// We do all of the Modified Nodal Analysis setup steps all at once below. There are (using electrical terminology):
/// 1) Select a reference node
/// 2) Assign a name to the current through each voltage source.  We will use the convention that the current flows from the positive node to the negative node of the source.
/// 3) Apply Kirchoff's current law to each node.  We will take currents out of the node to be positive.
/// 4) Write an equation for the voltage each voltage source.
/// 5) Solve the system of n-1 unknowns.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::ParseIn()
{
  // Check the Reference Node
  if (!m_circuit->HasReferenceNode()) {
    ///\error Fatal: Circuit must have at least 1 reference node
    Fatal("Circuit must have at least 1 reference node");
  }

  { // Create some scope for r
    NodeType* r = nullptr;
    for (NodeType* ref : m_circuit->GetReferenceNodes()) {
      if (!ref->HasPotential() && !ref->HasNextPotential()) {
        ///\error Warning: Reference pressure is not defined - setting it to 0.
        Warning("Reference pressure is not defined - setting it to 0.", "SECircuitCalculator::ParseIn");
        ValueOverride<PotentialUnit>(ref->GetNextPotential(), 0, m_PotentialUnit);
        Override<PotentialUnit>(ref->GetNextPotential(), ref->GetPotential());
      } else if (!ref->HasPotential())
        Override<PotentialUnit>(ref->GetNextPotential(), ref->GetPotential());
      if (!ref->HasNextPotential())
        Override<PotentialUnit>(ref->GetPotential(), ref->GetNextPotential());
      if (r == nullptr)
        r = ref;
      else if (!r->GetPotential().Equals(r->GetPotential())) {
        ///\error Fatal: Multiple Reference Potentials must be equal
        Fatal("Multiple Reference Potentials must be equal");
      }
    }
  }

  size_t idx = 0;
  size_t numNodes = m_circuit->GetNodes().size() - m_circuit->GetReferenceNodes().size();
  m_potentialSources.clear();
  for (PathType* p : m_circuit->GetPaths()) {
    //Set aside the pressure sources, since the Flow through them will be directly solved by adding them to the bottom of the matrix.
    //We have to do this outside of the KCL loop below because we only want to account for each one once.
    if (p->HasNextPotentialSource() || (p->NumberOfNextElements() < 1) || (p->HasNextValve() && p->GetNextValve() == SEOpenClosed::Closed) || (p->HasNextSwitch() && p->GetNextSwitch() == SEOpenClosed::Closed)) {
      m_potentialSources[p] = numNodes + idx++;
    }
  }

  size_t numSources = m_potentialSources.size();
  size_t numVars = numNodes + numSources;
  //Set the size of the matrix and initialize all elements to zero - we'll populate it later.
  //subtract the known reference - we don't need to solve for that pressure
  //(and it has to be known or we'll have too many unknowns).
  m_AMatrix = Eigen::MatrixXd::Zero(numVars, numVars);
  //Set the right side b vector
  //All zeros - we'll modify the values later
  m_bVector = Eigen::VectorXd::Zero(numVars);

  //Build the A matrix to solve using linear algebra
  //We basically walk through all of the nodes and do the KCL for each
  //The A matrix will have all of the left side values (variable multipliers) from the KCL equation
  //The b matrix will have all of the right side values (known values) from the KCL equation
  //Variables used in the loop
  double dStartingCompliance = 0.0;
  double dStartingInertance = 0.0;
  for (NodeType* n : m_circuit->GetNodes()) {
    //Sum of the flows at each node is 0
    //Skip known reference node (see comment above)
    if (m_circuit->IsReferenceNode(*n))
      continue;

    for (PathType* p : *m_circuit->GetConnectedPaths(*n)) {
      NodeType* nSrc = &p->GetSourceNode();
      NodeType* nTgt = &p->GetTargetNode();

      if (p->HasNextPolarizedState() && p->GetNextPolarizedState() == SEOpenClosed::Open) { //Polarized elements that are open are done exactly the same as a open switch.
        //We'll check to see if the resulting pressure difference is valid later.
        //Model as an open switch
        double dMultiplier = 1.0 / OPEN_RESISTANCE;
        PopulateAMatrix(*n, *p, dMultiplier);
        //Don't allow to function normally - i.e. continue to the next path
        continue;
      }

      //Each Path has only one Element (or none at all) and each type is handled differently.
      //The variables in the x vector are the unknown Node Pressures and Flows for Pressure Sources.
      if (p->HasNextSwitch()) {
        if (p->GetNextSwitch() == SEOpenClosed::Open) {
          //Model as a resistor with a ridiculously high resistance (basically an open circuit)
          double dMultiplier = 1.0 / OPEN_RESISTANCE;
          PopulateAMatrix(*n, *p, dMultiplier);
        } else {
          //Model as a zero pressure source
          PopulateAMatrix(*n, *p, 1, true);
        }
      } else if (p->HasNextResistance()) {
        double r = p->GetNextResistance().GetValue(m_ResistanceUnit);
        if (r < 0.0) {
          /// \error Fatal: Resistance cannot be negative
          Fatal("Resistance cannot be negative.", p->GetName());
        }
        double dMultiplier = 1.0 / r;
        PopulateAMatrix(*n, *p, dMultiplier);
      } else if (p->HasNextCapacitance()) {
        //Capacitors use initial voltage
        if (!nSrc->HasPotential()) {
          //Initial source pressure is not defined, assume it's the same as the reference
          ValueOverride<PotentialUnit>(nSrc->GetPotential(), 0.0, m_PotentialUnit);
          ///\error Warning: Initial compliance source pressure is not defined, assuming it is the reference pressure.
          Warning("Initial capacitance source potential is not defined for " + std::string{ nSrc->GetName() } +", assuming it is the reference potential.");
        }

        if (!nTgt->HasPotential()) {
          //Initial target pressure is not defined, assume it's the same as the reference
          ValueOverride<PotentialUnit>(nTgt->GetPotential(), 0.0, m_PotentialUnit);
          ///\error Warning: Initial compliance target pressure is not defined, assuming it is the reference pressure.
          Warning("Initial capacitance target potential is not defined for " + std::string{ nTgt->GetName() }+", assuming it is the reference potential.");
        }

        //We need to handle the first time through when there is no Current value
        if (p->HasCapacitance()) {
          dStartingCompliance = p->GetCapacitance().GetValue(m_CapacitanceUnit);
        } else {
          dStartingCompliance = p->GetNextCapacitance().GetValue(m_CapacitanceUnit);
          if (dStartingCompliance < 0.0) {
            /// \error Fatal: Capacitance cannot be negative
            Fatal("Capacitance is negative for " + std::string{ p->GetName() });
          }
        }

        //Add left side of the equation
        double dMultiplier = p->GetNextCapacitance().GetValue(m_CapacitanceUnit) / m_dT_s;
        PopulateAMatrix(*n, *p, dMultiplier);

        //Add the known initial time values to the right side of the equation
        //C(0)/dt*(dV(0))
        dMultiplier = dStartingCompliance / m_dT_s;

        double dLastPressureDiff = 0;
        if (n == nSrc)
          dLastPressureDiff = nSrc->GetPotential().GetValue(m_PotentialUnit) - nTgt->GetPotential().GetValue(m_PotentialUnit);
        else if (n == nTgt)
          dLastPressureDiff = nTgt->GetPotential().GetValue(m_PotentialUnit) - nSrc->GetPotential().GetValue(m_PotentialUnit);

        m_bVector(m_circuit->GetCalculatorIndex(*n)) += (dMultiplier * dLastPressureDiff);
      } else if (p->HasNextInductance()) {
        //Inductors use initial current
        if (!p->HasFlux()) {
          //Initial flow is not defined, assume it's 0
          ValueOverride<FluxUnit>(p->GetFlux(), 0.0, m_FluxUnit);
          ///\error Warning: Initial inductance is not defined, assuming it is 0.
          Warning("Initial inductance is not defined for " + std::string{ p->GetName() }+", assuming it is 0.");
        }

        if (!nSrc->HasPotential()) {
          //Initial source pressure is not defined, assume it's the same as the reference
          ValueOverride<PotentialUnit>(nSrc->GetPotential(), 0.0, m_PotentialUnit);
          ///\error Warning: Initial inductance source pressure is not defined, assuming it is the reference pressure.
          Warning("Initial inductance source pressure is not defined " + std::string{ nSrc->GetName() }+", assuming it is the reference pressure.");
        }
        if (!nTgt->HasPotential()) {
          //Initial source pressure is not defined, assume it's the same as the reference
          ValueOverride<PotentialUnit>(nTgt->GetPotential(), 0.0, m_PotentialUnit);
          ///\error Warning: Initial inductance target pressure is not defined, assuming it is the reference pressure.
          Warning("Initial inductance target pressure is not defined " + std::string{ nTgt->GetName() } +", assuming it is the reference pressure.");
        }

        double pInductance = p->GetNextInductance().GetValue(m_InductanceUnit);
        if (pInductance < 0.0) {
          /// \error Fatal: Inductance cannot be negative
          Fatal("Inductance is negative for " + std::string{ p->GetName() });
        }

        double dMultiplier = m_dT_s / (2 * pInductance);
        PopulateAMatrix(*n, *p, dMultiplier);

        double dLastFlow = 0.0;
        double dLastPressureDiff = 0.0;
        if (n == nSrc) {
          dLastFlow = -1.0 * p->GetFlux().GetValue(m_FluxUnit);
          dLastPressureDiff = n->GetPotential().GetValue(m_PotentialUnit) - nTgt->GetPotential().GetValue(m_PotentialUnit);
        } else if (n == nTgt) {
          dLastFlow = p->GetFlux().GetValue(m_FluxUnit);
          dLastPressureDiff = n->GetPotential().GetValue(m_PotentialUnit) - nSrc->GetPotential().GetValue(m_PotentialUnit);
        }
        m_bVector(m_circuit->GetCalculatorIndex(*n)) += dLastFlow - (dMultiplier * dLastPressureDiff);
      } else if (p->HasNextValve()) {
        //Valves are done exactly the same as switches.
        //We'll check to see if the resulting flow and pressure difference is valid later.
        if (p->GetNextValve() == SEOpenClosed::Open) {
          //Model as a resistor with a ridiculously high resistance (basically an open circuit)
          double dMultiplier = 1.0 / OPEN_RESISTANCE;
          PopulateAMatrix(*n, *p, dMultiplier);
        } else {
          //Model as a zero pressure source
          PopulateAMatrix(*n, *p, 1, true);
        }
      } else if (p->HasNextFluxSource()) {
        //Currents out of the node are assumed positive and the sign is switched when moving from the left side of the equation to the right.
        //Therefore, out of the Node we're current analyzing (i.e. Source) reverses the sign when it goes into the right side vector.
        double sign = (n == nSrc) ? -1 : 1;
        double dFlow = p->GetNextFluxSource().GetValue(m_FluxUnit);
        m_bVector(m_circuit->GetCalculatorIndex(*n)) += (sign * dFlow);
      } else if (p->HasNextPotentialSource()) {
        PopulateAMatrix(*n, *p, 1, true);
      } else {
        //No element
        //Model as a 0v voltage source
        PopulateAMatrix(*n, *p, 1, true);
      }
    }
  }

#ifdef VERBOSE
  //std::cout << "#iterations:     " << Solver.iterations() << std::endl;
  //std::cout << "estimated error: " << Solver.error()      << std::endl;
  std::cout << "PrePotential" << std::endl;
  std::cout << "A = " << std::endl
            << m_AMatrix << std::endl;
  std::cout << "b = " << std::endl
            << m_bVector << std::endl;
  std::cout << "x = " << std::endl
            << m_xVector << std::endl;
#endif

  //Deal with pressure sources
  //We also model closed Switches, "closed" Valves (those allowing flow), and shorts (paths without an element) as 0Pa Pressure Sources.
  //All pressure sources will have their Flow directly solved by adding equations for known Node pressure differences caused by them.
  //We add rows for these equations after the KCL equations (hence the iNodeSize+i).
  for (auto itr : m_potentialSources) {
    PathType* p = itr.first;
    NodeType& nSrc = p->GetSourceNode();
    NodeType& nTgt = p->GetTargetNode();

    if (!m_circuit->IsReferenceNode(nSrc)) {
      m_AMatrix(itr.second, m_circuit->GetCalculatorIndex(nSrc)) = -1;
    }
    if (!m_circuit->IsReferenceNode(nTgt)) {
      m_AMatrix(itr.second, m_circuit->GetCalculatorIndex(nTgt)) = 1;
    }

    if (p->HasNextSwitch() || p->HasNextValve() || p->NumberOfNextElements() < 1) {
      m_bVector(itr.second) = 0.0;
    } else {
      m_bVector(itr.second) += p->GetNextPotentialSource().GetValue(m_PotentialUnit);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Solve for unknowns.
///
/// \details
/// Use the Eigen solver to find the unknowns.
/// Make sure the solution is correct.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::Solve()
{
#ifdef VERBOSE
  //std::cout << "#iterations:     " << Solver.iterations() << std::endl;
  //std::cout << "estimated error: " << Solver.error()      << std::endl;
  std::cout << "PreSolve" << std::endl;
  std::cout << "A = " << std::endl
            << m_AMatrix << std::endl;
  std::cout << "b = " << std::endl
            << m_bVector << std::endl;
  std::cout << "x = " << std::endl
            << m_xVector << std::endl;
#endif

  bool sparseFailed = false;
  switch (m_solver.value()) {
    //Use the slow direct-solve method
  case EigenCircuitSolver::Direct:
    m_xVector = m_AMatrix.inverse() * m_bVector;
    break;
    //Dense methods
  case EigenCircuitSolver::PartialPivLu:
    m_xVector = m_AMatrix.partialPivLu().solve(m_bVector);
    break;
  case EigenCircuitSolver::FullPivLu:
    m_xVector = m_AMatrix.fullPivLu().solve(m_bVector);
    break;
  case EigenCircuitSolver::JacobiSvd:
    m_xVector = m_AMatrix.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(m_bVector);
    break;
  case EigenCircuitSolver::HouseholderQr:
    m_xVector = m_xVector = m_AMatrix.householderQr().solve(m_bVector);
    break;
  case EigenCircuitSolver::Ldlt:
    m_xVector = m_AMatrix.ldlt().solve(m_bVector);
    break;
  case EigenCircuitSolver::Llt:
    m_xVector = m_AMatrix.llt().solve(m_bVector);
    break;
    //Sparse methods
  case EigenCircuitSolver::SparseLU: {
    Eigen::SparseMatrix<double> sparse = m_AMatrix.sparseView();
    //TODO:sawhite:Upgrade SparseMatrix to Eigen::SparseLU<Eigen::SparseMatrix<double, Eigen::ColMajor, int64_t>>
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(sparse);
    //Check to see if we have a good solution.
    if (solver.info() == Eigen::Success) {
      m_xVector = solver.solve(m_bVector);
    } else

      sparseFailed = true;
    break;
  }
  case EigenCircuitSolver::SparseQR: {
    Eigen::SparseMatrix<double> sparse = m_AMatrix.sparseView();
    //sparse.makeCompressed();
    Eigen::SparseQR<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> solver;
    solver.compute(sparse);
    //Check to see if we have a good solution.
    if (solver.info() == Eigen::Success)
      m_xVector = solver.solve(m_bVector);
    else
      sparseFailed = true;
    break;
  }
  case EigenCircuitSolver::BiCGSTAB: {
    Eigen::SparseMatrix<double> sparse = m_AMatrix.sparseView();
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver;
    solver.compute(sparse);
    //Check to see if we have a good solution.
    if (solver.info() == Eigen::Success)
      m_xVector = solver.solve(m_bVector);
    else
      sparseFailed = true;
    break;
  }
  case EigenCircuitSolver::ConjugateGradient: {
    Eigen::SparseMatrix<double> sparse = m_AMatrix.sparseView();
    Eigen::ConjugateGradient<Eigen::SparseMatrix<double>> solver;
    solver.compute(sparse);
    //Check to see if we have a good solution.
    if (solver.info() == Eigen::Success)
      m_xVector = solver.solve(m_bVector);
    else
      sparseFailed = true;
    break;
  }
  default: {
    Warning("Invalid solver type requested; using backup method");
    sparseFailed = true;
  }
  };

  if (sparseFailed /*|| !(m_AMatrix*m_xVector).isApprox(m_bVector, 1.0e-10)*/) {
    //The faster sparse solver should almost always work
    //If it didn't, do it dense and make sure we get an answer (if possible)
    m_xVector = m_AMatrix.fullPivLu().solve(m_bVector);

    if (!(m_AMatrix * m_xVector).isApprox(m_bVector, 1.0e-11)) {
      double absolute_error = (m_AMatrix * m_xVector - m_bVector).norm();
      double relative_error = absolute_error / m_bVector.norm();
      std::stringstream ss;

      if (!(m_AMatrix * m_xVector).isApprox(m_bVector, 1.0e-8)) {
        ss << "The solver was unable to determine a solution for the circuit. Relative error = " << relative_error
           << ". Absolute error = " << absolute_error;
        ///\error Fatal: The solver was unable to determine a solution for the circuit.
        Fatal(ss);
      } else {
        ss << "Circuit solution from secondary solver with maximum relative error greater than 1.0e-12 but less than 1.0e-8. Relative error = "
           << relative_error << ". Absolute error = " << absolute_error;
        ///\warning Warning: Circuit solution from secondary solver with maximum relative error greater than 1.0e-12 but less than 1.0e-8
        Warning(ss);
      }
    }
  }
#ifdef VERBOSE
  //std::cout << "#iterations:     " << Solver.iterations() << std::endl;
  //std::cout << "estimated error: " << Solver.error()      << std::endl;
  std::cout << "PostSolve" << std::endl;
  std::cout << "A = " << std::endl
            << m_AMatrix << std::endl;
  std::cout << "b = " << std::endl
            << m_bVector << std::endl;
  std::cout << "x = " << std::endl
            << m_xVector << std::endl;
  double relative_error = (m_AMatrix * m_xVector - m_bVector).norm() / m_bVector.norm(); // norm() is L2 norm
  std::cout << "The relative error is: " << relative_error << std::endl;
#endif
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Assign the Solver solution vector values to the appropriate places.
///
/// \details
/// Set all Node Pressures.
/// Set Pressure Source Flows.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::ParseOut()
{
  double refPotential = m_circuit->GetReferenceNodes()[0]->GetPotential().GetValue(m_PotentialUnit);
  for (NodeType* n : m_circuit->GetNodes()) {
    if (!m_circuit->IsReferenceNode(*n)) {
      //Add the reference potential
      //For the calculations, we assume the reference potential is zero
      //When it's not zero, all potentials are just offset by that amount
      double potential = m_xVector(m_circuit->GetCalculatorIndex(*n)) + refPotential;
      ValueOverride<PotentialUnit>(n->GetNextPotential(), potential, m_PotentialUnit);
    }
  }
  for (auto itr : m_potentialSources) {
    //The Pressure Source Flows are all in order after the Node Pressures.
    //The total number of unknown node pressures are one less than the total number of nodes because the reference node pressure is known.
    double dFlow = m_xVector(itr.second);
    //We define pressure sources as the difference in pressure from source to target.
    //Therefore, a positive value means the target pressure is greater than the source (and reverse for negative values).
    //This means flow would go Target to Source (high to low pressure), and we need to reverse the sign.
    ValueOverride<FluxUnit>(itr.first->GetNextFlux(), -dFlow, m_FluxUnit);
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate and set all Path Flow values.
///
/// \details
/// Set flows that were given for Flow Sources.
/// Set flows that were directly solved for Pressure Sources.
/// Calculate and set Flows for Paths with other Elements.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::CalculateFluxes()
{
  //Calculate flows (currents)
  //Note: flows use source->target convention for positive flow, so the source pressure needs to be larger than the target pressure for positive flow
  for (PathType* p : m_circuit->GetPaths()) {
    if (p->HasNextFluxSource()) {
      Override<FluxUnit>(p->GetNextFluxSource(), p->GetNextFlux());
    } else if ((p->HasNextSwitch() && p->GetNextSwitch() == SEOpenClosed::Open) || (p->HasNextValve() && p->GetNextValve() == SEOpenClosed::Open) || (p->HasNextPolarizedState() && p->GetNextPolarizedState() == SEOpenClosed::Open)) {
      ValueOverride<FluxUnit>(p->GetNextFlux(), 0, m_FluxUnit);
    } else if (p->HasNextResistance()) {
      //I = V/R
      double dResistance;
      dResistance = p->GetNextResistance().GetValue(m_ResistanceUnit);
      double dPressDiff = p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit);
      double dFlow = dPressDiff / dResistance;
      ValueOverride<FluxUnit>(p->GetNextFlux(), -dFlow, m_FluxUnit);
    } else if (p->HasNextCapacitance()) {
      //Positive flow is from source to target (i.e. SourcePressure > TargetPressure = Positive Flow)
      //dStartingCompliance is at time = T
      double dStartingCompliance = 0.0;
      if (p->HasCapacitance())
        dStartingCompliance = p->GetCapacitance().GetValue(m_CapacitanceUnit);
      else
        dStartingCompliance = p->GetNextCapacitance().GetValue(m_CapacitanceUnit);
      //dEndingCompliance is at time = T + deltaT
      double dEndingCompliance = p->GetNextCapacitance().GetValue(m_CapacitanceUnit);
      //dStartingPressDiff is at time = T
      double dStartingPressDiff = p->GetSourceNode().GetPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetPotential().GetValue(m_PotentialUnit);
      //dStartingPressDiff is at time = T + deltaT
      double dEndingPressDiff = p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit);
      double dFlow = (dEndingCompliance * dEndingPressDiff - dStartingCompliance * dStartingPressDiff) / m_dT_s;
      ValueOverride<FluxUnit>(p->GetNextFlux(), dFlow, m_FluxUnit);
    } else if (p->HasNextInductance()) {
      //V = L*dI/dt
      //I = dt/L*V+I(0)
      double dPressDiff = p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit);
      double dInitialPressDiff = p->GetTargetNode().GetPotential().GetValue(m_PotentialUnit) - p->GetSourceNode().GetPotential().GetValue(m_PotentialUnit);
      double dInitialFlow = -1.0 * p->GetFlux().GetValue(m_FluxUnit);
      double dFlow = m_dT_s / (2 * p->GetNextInductance().GetValue(m_InductanceUnit)) * (dPressDiff + dInitialPressDiff) + dInitialFlow;
      ValueOverride<FluxUnit>(p->GetNextFlux(), -dFlow, m_FluxUnit);
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Calculate and set all Path Volumes.
///
/// \details
/// Calculate and set the Volumes for all Paths with Compliance Elements.
/// You need a compliance because volume can't change in a rigid pipe.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::CalculateQuantities()
{
  //Calculate volume changes due to compliance.
  //You need a compliance because volume can't change in a rigid pipe.

  //Volumes are only calculated for paths with a compliance, and each model is responsible for populating node volumes based on those values.
  for (PathType* p : m_circuit->GetPaths()) {
    // Calculate Quantities
    if (p->HasNextCapacitance()) {
      //Charge is analogues to volume
      double flux = p->GetNextFlux().GetValue(m_FluxUnit);
      double dVolumeIncrement = flux * m_dT_s;

      //Handle polarized elements special-like
      if (p->HasNextPolarizedState()) {
        double dStartingCompliance = 0.0;
        if (p->HasCapacitance())
          dStartingCompliance = p->GetCapacitance().GetValue(m_CapacitanceUnit);
        else
          dStartingCompliance = p->GetNextCapacitance().GetValue(m_CapacitanceUnit);
        //dEndingCompliance is at time = T + deltaT
        double dEndingCompliance = p->GetNextCapacitance().GetValue(m_CapacitanceUnit);
        //dStartingPressDiff is at time = T
        double dStartingPressDiff = std::abs(p->GetSourceNode().GetPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetPotential().GetValue(m_PotentialUnit));
        //dStartingPressDiff is at time = T + deltaT
        double dEndingPressDiff = std::abs(p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit));

        if (p->GetPolarizedState() == SEOpenClosed::Open) {
          //If this was a shorted polarized element last time-step, we need to make the starting difference zero
          //Otherwise, it will possibly think it was already charged to a certain point
          dStartingPressDiff = 0.0;
        }
        if (p->GetNextPolarizedState() == SEOpenClosed::Open) {
          //If it is currently a shorted polarized element, we need to make the starting difference zero
          //This will make it go to the non-charged volume
          dEndingPressDiff = 0.0;
        }

        dVolumeIncrement = dEndingCompliance * dEndingPressDiff - dStartingCompliance * dStartingPressDiff;
      }

      if (p->GetSourceNode().HasNextQuantity() && !p->GetSourceNode().GetNextQuantity().IsInfinity()) {
        IncrementOverride<QuantityUnit>(p->GetSourceNode().GetNextQuantity(), dVolumeIncrement, m_QuantityUnit);
      }

      if (p->GetTargetNode().HasNextQuantity() && !p->GetTargetNode().GetNextQuantity().IsInfinity()) {
        IncrementOverride<QuantityUnit>(p->GetTargetNode().GetNextQuantity(), -dVolumeIncrement, m_QuantityUnit);
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Check if the solutions meets all Valve criteria, and modify a Valve state if not.
///
/// \return true if the solution meets all Valve criteria, false if not
///
/// \details
/// The solution passes if:
/// 1) There is a path with a valve and flow in the wrong direction.
/// 2) Check reverse bias (i.e. voltage must be positive at the cathode).
/// Look at the pressures to ensure flow would be going against the diode if the "switch" (ideal diode is modeled as a switch) was closed.
/// Since flow goes from low to high pressure, the pressure at the Target must be higher or equal to the Source for negative flow (against valve).
///
/// If it does not pass, change the state (Open/Close) of the first Valve that fails.
/// The circuit will be re-solved with the new Valve state, and will iterate until a solution is found.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
bool SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::CheckAndModifyValves()
{
  if (m_circuit->GetPolarizedElementPaths().empty() && m_circuit->GetValvePaths().empty())
    return true; //There aren't any valves to worry about

  if (m_valveStates.size() >= ((uint64_t)(std::pow(2.0, int(m_valveStates.size()))))) {
    ///\error Fatal: The combination of valves precludes the circuit from being solved.
    Fatal("The combination of valves precludes the circuit from being solved.");
  }

  for (PathType* p : m_circuit->GetValvePaths()) {
    if ((p->GetNextValve() == SEOpenClosed::Closed && p->GetNextFlux().GetValue(m_FluxUnit) < -ZERO_APPROX)
      || (p->GetNextValve() == SEOpenClosed::Open && (p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit)) > ZERO_APPROX)) {
      p->FlipNextValve();
      if (IsCurrentValveStateUnique()) {
        return false;
      }
      p->FlipNextValve(); // Not unique, flip it back
    }
  }

  for (PathType* p : m_circuit->GetPolarizedElementPaths()) {
    if (p->GetNextPolarizedState() == SEOpenClosed::Closed && (p->GetSourceNode().GetNextPotential().GetValue(m_PotentialUnit) - p->GetTargetNode().GetNextPotential().GetValue(m_PotentialUnit)) < -ZERO_APPROX) {
      p->FlipNextPolarizedState();
      if (IsCurrentValveStateUnique()) {
        return false;
      }
      p->FlipNextPolarizedState(); // Not unique, flip it back
    }
  }
  return true;
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Check if the current Valve state combination has already been analyzed.
///
/// \return true if the Valve combination is unique, false if it is not.
///
/// \details
/// Loop through all of the previously analyzed Valve combinations this time-step and check if the current combination is the same.
/// If this combination is unique, store it in a vector for the next go-round to check.
/// Valve combinations are stored as integers that use their index and state (Open = 0, Closed = 1) to calculate a unique value for every combination.
/// The total number of possible combinations (i.e. highest value possible) is 2^(# of Valves in the Circuit).
/// For example, if all open will be 0, no matter how many Valves are in the circuit
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
bool SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::IsCurrentValveStateUnique()
{
  uint64_t index = 1;
  uint64_t currentState = 0;
  /// The lower bits are used to represent the states of the non-polarized valves.  In the lexicographic
  /// ordering of all state representations, all non-polarized valve combinations are enumerated first before
  /// the polarized element combinations.  This is the order in which we want to evaluate the states
  /// so this bit order is important.
  for (PathType* pValve : m_circuit->GetValvePaths()) {
    if (pValve->GetNextValve() == SEOpenClosed::Closed)
      currentState |= index;
    index = index << static_cast<uint64_t>(1);
  }
  for (PathType* pPolarizedElement : m_circuit->GetPolarizedElementPaths()) {
    if (pPolarizedElement->GetNextPolarizedState() == SEOpenClosed::Closed)
      currentState |= index;
    index = index << static_cast<uint64_t>(1);
  }
  if (m_valveStates.find(currentState) != end(m_valveStates))
    return false;
  m_valveStates.insert(currentState);
  return true;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Add a value to the appropriate location in the Jacobian matrix.
///
/// \param  nKCL              The Node that is currently being analyzed.
/// \param  p                 The Path that is currently being analyzed.
/// \param  dMultiplier       The value to insert in the Jacobian matrix.
/// \param  hasPressureSource True if the path has a Pressure Source.
///
/// \details
/// Each row in the square A matrix is for the KCL analysis of a single Node.
/// Each column is for the variables (i.e. other Nodes) in that KCL equation, and is in the same order as the rows.
/// These should include all of the multipliers on the left side of the KCL equations.
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::PopulateAMatrix(NodeType& n, PathType& p, double dMultiplier, bool hasPotentialSource)
{
  NodeType& nSrc = p.GetSourceNode();
  NodeType& nTgt = p.GetTargetNode();

  if (std::isinf(dMultiplier)) {
    //Someone screwed up
    //Probably a divide by zero and most likely from a Resistance
    m_ss << "Attempting to populate a matrix with an infinite value.  Check " << p.GetName() << " path circuit element.";
    Fatal(m_ss);
  }

  if (hasPotentialSource) {
    //Handle Pressure Sources here.
    //The Jacobian Matrix uses their Flow as a variable.
    double sign = 0;
    if (&nSrc == &n)
      sign = -1;
    else if (&nTgt == &n)
      sign = 1;
    m_AMatrix(m_circuit->GetCalculatorIndex(n), m_potentialSources[&p]) += sign;
  } else {
    //We use the convention that the Node we're sitting on is always the positive multiplier in the current equation.
    //Therefore, it's positive as either the Source or the Target.
    //Nodes on the other end of the path have a negative multiplier.

    if (m_circuit->IsReferenceNode(nSrc)) {
      m_bVector(m_circuit->GetCalculatorIndex(n)) += 0;
    } else if (&nSrc == &n) {
      //If the Source Node is the Node we're sitting on, our convention is positive.
      m_AMatrix(m_circuit->GetCalculatorIndex(n), m_circuit->GetCalculatorIndex(nSrc)) += dMultiplier;
    } else {
      //The Source Node is not the Node we're sitting on for KCL analysis.
      m_AMatrix(m_circuit->GetCalculatorIndex(n), m_circuit->GetCalculatorIndex(nSrc)) -= dMultiplier;
    }

    if (m_circuit->IsReferenceNode(nTgt)) {
      m_bVector(m_circuit->GetCalculatorIndex(n)) += 0;
    } else if (&nTgt == &n) {
      //If the Target Node is the Node we're sitting on, our convention is positive.
      m_AMatrix(m_circuit->GetCalculatorIndex(n), m_circuit->GetCalculatorIndex(nTgt)) += dMultiplier;
    } else {
      //The Target Node is not the Node we're sitting on for KCL analysis.
      m_AMatrix(m_circuit->GetCalculatorIndex(n), m_circuit->GetCalculatorIndex(nTgt)) -= dMultiplier;
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Advances time
///
/// \param  Circuit The circuit in question.
/// \param  vSubstances List with the substances that are currently present in the circuit
///
/// \details
/// Move the Next circuit values to Current, and sets the Next values to the Baselines.
/// This effectively advances time for the circuit
//--------------------------------------------------------------------------------------------------
template <CIRCUIT_CALCULATOR_TEMPLATE>
void SECircuitCalculator<CIRCUIT_CALCULATOR_TYPES>::PostProcess(CircuitType& circuit)
{
  // 1) Push Next pressures, flows, and modified path elements (the only elements that will have a value in Next are those that are modified since the last time step) to Current
  for (NodeType* n : circuit.GetNodes()) {
    Override<PotentialUnit>(n->GetNextPotential(), n->GetPotential());
    //Volume is only incremented, not overwritten by the circuit math.
    Override<QuantityUnit>(n->GetNextQuantity(), n->GetQuantity());
  }
  //The current values stay the same, unless there's a next value to modify it
  //This goes along with the circuit "alter" methodology
  for (PathType* p : circuit.GetPaths()) {
    if (p->HasNextValve())
      p->SetValve(p->GetNextValve());
    if (p->HasNextSwitch())
      p->SetSwitch(p->GetNextSwitch());
    if (p->HasNextPolarizedState())
      p->SetPolarizedState(p->GetNextPolarizedState());

    if (p->HasNextFlux())
      Override<FluxUnit>(p->GetNextFlux(), p->GetFlux());
    if (p->HasNextFluxSource())
      Override<FluxUnit>(p->GetNextFluxSource(), p->GetFluxSource());
    if (p->HasNextResistance())
      Override<ResistanceUnit>(p->GetNextResistance(), p->GetResistance());
    if (p->HasNextCapacitance())
      Override<CapacitanceUnit>(p->GetNextCapacitance(), p->GetCapacitance());
    if (p->HasNextInductance())
      Override<InductanceUnit>(p->GetNextInductance(), p->GetInductance());
    if (p->HasNextPotentialSource())
      Override<PotentialUnit>(p->GetNextPotentialSource(), p->GetPotentialSource());

    // 2) Set Next Path elements to the Baseline values
    //    We won't touch Valves or Switches.  Valves should keep their state to efficiently solve based on assumed states
    p->GetNextFluxSource().Set(p->GetFluxSourceBaseline());
    p->GetNextResistance().Set(p->GetResistanceBaseline());
    p->GetNextCapacitance().Set(p->GetCapacitanceBaseline());
    p->GetNextInductance().Set(p->GetInductanceBaseline());
    p->GetNextPotentialSource().Set(p->GetPotentialSourceBaseline());
  }
}
}
