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

#include <biogears/cdm/circuit/SECircuitCalculator.h>

OPEN_BIOGEARS_NAMESPACE
// Note this must match the enum values
template <>
char const* enumStrings<EigenCircuitSolver>::values[] = { "Direct", "PartialPivLu", "FullPivLu", "JacobiSvd", "HouseholderQr", "Ldlt", "Llt", "SparseLU", "SparseQR", "BiCGSTAB", "ConjugateGradient" };
char const* EigenCircuitSolver::Value(size_t idx)
{
  return enumStrings<EigenCircuitSolver>::values[idx];
}
CLOSE_BIOGEARS_NAMESPACE
