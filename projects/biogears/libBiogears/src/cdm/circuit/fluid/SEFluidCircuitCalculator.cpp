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

#include <biogears/cdm/circuit/fluid/SEFluidCircuitCalculator.h>

namespace biogears {
template class SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, FlowComplianceUnit, VolumePerTimeUnit, FlowInertanceUnit, PressureUnit, VolumeUnit, FlowResistanceUnit>;
SEFluidCircuitCalculator::SEFluidCircuitCalculator(Logger* logger)
  : SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, FlowComplianceUnit, VolumePerTimeUnit, FlowInertanceUnit, PressureUnit, VolumeUnit, FlowResistanceUnit>(
    FlowComplianceUnit::m3_Per_Pa, VolumePerTimeUnit::m3_Per_s, FlowInertanceUnit::Pa_s2_Per_m3, PressureUnit::Pa, VolumeUnit::m3, FlowResistanceUnit::Pa_s_Per_m3, logger)
{
}
SEFluidCircuitCalculator::SEFluidCircuitCalculator(const FlowComplianceUnit& c, const VolumePerTimeUnit& f, const FlowInertanceUnit& i, const PressureUnit& p, const VolumeUnit& q, const FlowResistanceUnit& r, Logger* logger)
  : SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, FlowComplianceUnit, VolumePerTimeUnit, FlowInertanceUnit, PressureUnit, VolumeUnit, FlowResistanceUnit>(c, f, i, p, q, r, logger)
{
}
SEFluidCircuitCalculator::~SEFluidCircuitCalculator()
{
}
}