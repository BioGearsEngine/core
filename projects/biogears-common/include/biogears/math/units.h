#ifndef BIOGEARS_MATH_ANGLES_H
#define BIOGEARS_MATH_ANGLES_H

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//! \file
//! \author Steven A White
//! \date 2019 09 13
//!
//! Mathematical utility code
//!

#include <units.h>

namespace units {
	UNIT_ADD(pressure, milimeters_of_mercury, milimeters_of_mercury, mmHG, units::unit<std::ratio<1000000000, 133322387415>, units::pressure::pascals>)
	UNIT_ADD(pressure, inches_of_mercury, inches_of_mercury, inHG, units::unit<std::ratio<25400, 1000>, milimeters_of_mercury>)

	UNIT_ADD(insulation, r_value, r_values, R, units::compound_unit<squared<units::length::meters>, units::temperature::celsius, inverse<squared<units::power::watts>>>)
	UNIT_ADD(insulation, rsi_value, rsi_values, RSI, units::unit<std::ratio<567826, 100000>, r_values>)
	UNIT_ADD(insulation, clo, clothes, clo, units::unit<std::ratio<86,100>, r_values>)
	UNIT_ADD(insulation, tog, togs, tog, units::unit<std::ratio<100,155>, clo>)
}

#endif //BIOGEARS_MATH_ANGLES_H
