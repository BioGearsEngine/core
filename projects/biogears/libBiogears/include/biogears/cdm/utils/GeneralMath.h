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
#include <vector>

#include <biogears/exports.h>
#include <biogears/cdm/circuit/SECircuitNode.h>

namespace biogears {

class SEScalarOsmolality;
class SEScalarOsmolarity;
class SEScalarTemperature;
class SEScalar;
class SEScalarAmountPerVolume;
class SEScalarPressure;
class SEScalarFraction;
class SEScalarMassPerVolume;
class SEScalarVolume;
class SEScalarMass;
class SESubstance;
class SESubstanceManager;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

class BIOGEARS_API GeneralMath {
public:
  static void CalculateConcentration(const SEScalarMass& mass, const SEScalarVolume& volume, SEScalarMassPerVolume& concentration, Logger* logger = nullptr);
  static void CalculateMass(const SEScalarVolume& volume, const SEScalarMassPerVolume& concentration, SEScalarMass& mass, Logger* logger = nullptr);
  static void CalculateHenrysLawConcentration(const SESubstance& substance, const SEScalarPressure& partialPressure, SEScalarMassPerVolume& concentration, Logger* logger = nullptr);
  static void CalculatePartialPressureInGas(const SEScalarFraction& volumeFraction, const SEScalarPressure& pressure, SEScalarPressure& partialPressure, Logger* logger = nullptr);
  static void CalculatePartialPressureInLiquid(const SESubstance& substance, const SEScalarMassPerVolume& concentration, SEScalarPressure& partialPressure, Logger* logger = nullptr);
  static void CalculateOsmolarity(const SEScalarAmountPerVolume& sodiumMolarity, const SEScalarAmountPerVolume& potassiumMolarity, const SEScalarAmountPerVolume& glucoseMolarity, const SEScalarAmountPerVolume& ureaMolarity, SEScalarOsmolarity& fluidOsmolarity);
  static void CalculateOsmolality(const SEScalarAmountPerVolume& sodiumMolarity, const SEScalarAmountPerVolume& potassiumMolarity, const SEScalarAmountPerVolume& glucoseMolarity, const SEScalarAmountPerVolume& ureaMolarity, const SEScalar& specificGravity, SEScalarOsmolality& fluidOsmolality);
  static void CalculateSpecificGravity(const SEScalarMass& mass, const SEScalarVolume& volume, SEScalar& specificGravity, Logger* logger = nullptr);
  static void CalculateWaterDensity(const SEScalarTemperature& temp, SEScalarMassPerVolume& density);

  static void Combinations(::std::vector<int> maxValues, ::std::vector<::std::vector<int>>& permutations);
  static double AntoineEquation(double dTemperature_C);
  static double LinearInterpolator(double x1, double x2, double y1, double y2, double xPrime);
  static double PercentDifference(double expected, double calculated);
  static double PercentTolerance(double expected, double calculated, double epsilon = 1e-20);
  static double ResistanceFunction(double dbase, double dmin, double dmax, double dx);
  static double LogisticFunction(double a, double x50, double k, double x);
  static double CalculateNernstPotential(SELiquidCompartment& extra, SELiquidCompartment& intra, SESubstance* ion, double& coreTemp_K);
  static double HillActivation(double x, double n, double h);
  static double HillInhibition(double x, double n, double h);
  static double VectorSum(std::vector<double>& vec);
};
}