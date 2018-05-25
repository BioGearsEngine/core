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

#include <biogears/exports.h>

#if (0)
#define DEBUGOUT(x) x
#else
#define DEBUGOUT(x) 
#endif
namespace mil {  namespace tatrc { namespace physiology { namespace datamodel {  } } } }
//using namespace mil::tatrc::phsyiology::datamodel;
#define CDM_BIND_DECL(type) namespace mil{ namespace tatrc{ namespace physiology{ namespace datamodel { class type; } } } }
namespace CDM = mil::tatrc::physiology::datamodel;

#include <memory>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <dirent.h>
#endif

#include <math.h>
#include <vector>
#include <stack>
#include <map>

#if defined(_MSC_VER)
	#include <unordered_map>
	using namespace stdext;
#else//if (__GCC__) || (__GNUC__)
	#include <unordered_map>
#endif

 /*
// Take from : https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers/blob/master/README.md
namespace xsd
{
  template<typename T>
  struct Serializable
  {
    using base_type = T;

    virtual ~Serializable() = default;

    virtual bool Load(const T& in) = 0;
  protected:
    virtual T* Unload() const
    {
      using base_type = typename T::base_type;
      static_assert(std::is_base_of<base_type, T>::value, "T object has to derived from T::base_type");
      auto ptr = static_cast<const base_type&>(object).NewBindType();
      ptr->Unload(static_cast<T*>(ptr));
      return std::unique_ptr<T>(static_cast<T*>(ptr));
    }
    virtual T* NewBindType() const = 0;
    virtual void Unload(T& object) const = 0;
  };
}
*/

/*
template class __declspec(dllexport) std::basic_string<char, 
                                                      std::char_traits<char>, 
                                                      std::allocator<char> >;

template class __declspec(dllexport) std::basic_stringstream<char, 
                                                            std::char_traits<char>, 
                                                            std::allocator<char> >;

template class __declspec(dllexport) std::vector<std::string>;
*/

// Disabling the waring about STL classes used have 
// to have a dll interface to be used by clients
// From what I have read STL is compiler dependent
// But since we are releasing source, you can build
// the project neccessary to ensure proper linkage
// If anyone else has opinions on this, let me know
// abray@ara.com
#pragma warning(disable : 4251)
//Utilities

struct CommonDataModelException : public std::runtime_error
{
	CommonDataModelException()
		: std::runtime_error( "Common Data Model Error" ) {}

	CommonDataModelException(const std::string& _Message)
		: std::runtime_error(_Message) {}
};
// Basics
#include <biogears/cdm/Macros.h>
#include <biogears/cdm/utils/Logger.h>

class CCompoundUnit;

class SEScalar; class SEUnitScalar; class SEGenericScalar; class NoUnit;
class SEScalar0To1; 
class SEScalarNeg1To1;
class SEScalarAmount; class AmountUnit;
class SEScalarAmountPerMass; class AmountPerMassUnit;
class SEScalarAmountPerTime; class AmountPerTimeUnit;
class SEScalarAmountPerVolume; class AmountPerVolumeUnit;
class SEScalarArea; class AreaUnit;
class SEScalarAreaPerTimePressure; class AreaPerTimePressureUnit;
class SEScalarElectricCapacitance; class ElectricCapacitanceUnit;
class SEScalarElectricCharge; class ElectricChargeUnit;
class SEScalarElectricCurrent; class ElectricCurrentUnit;
class SEScalarElectricInductance; class ElectricInductanceUnit;
class SEScalarElectricPotential; class ElectricPotentialUnit;
class SEScalarElectricResistance; class ElectricResistanceUnit;
class SEScalarEnergy; class EnergyUnit;
class SEScalarEnergyPerAmount; class EnergyPerAmountUnit;
class SEScalarEnergyPerMass; class EnergyPerMassUnit;
class SEScalarFlowCompliance; class FlowComplianceUnit;
class SEScalarFlowElastance; class FlowElastanceUnit;
class SEScalarFlowInertance; class FlowInertanceUnit;
class SEScalarFlowResistance; class FlowResistanceUnit;
class SEScalarForce; class ForceUnit;
class SEScalarFraction;
class SEScalarFrequency; class FrequencyUnit;
class SEScalarHeatCapacitance; class HeatCapacitanceUnit;
class SEScalarHeatCapacitancePerAmount; class HeatCapacitancePerAmountUnit;
class SEScalarHeatCapacitancePerMass; class HeatCapacitancePerMassUnit;
class SEScalarHeatConductance; class HeatConductanceUnit;
class SEScalarHeatConductancePerArea; class HeatConductancePerAreaUnit;
class SEScalarHeatResistance; class HeatResistanceUnit;
class SEScalarHeatResistanceArea; class HeatResistanceAreaUnit;
class SEScalarHeatInductance; class HeatInductanceUnit;
class SEScalarInversePressure; class InversePressureUnit; 
class SEScalarInverseVolume; class InverseVolumeUnit;
class SEScalarLength; class LengthUnit;
class SEScalarLengthPerTime; class LengthPerTimeUnit;
class SEScalarLengthPerTimePressure; class LengthPerTimePressureUnit;
class SEScalarMass; class MassUnit;
class SEScalarMassPerAmount; class MassPerAmountUnit;
class SEScalarMassPerAreaTime; class MassPerAreaTimeUnit;
class SEScalarMassPerMass; class MassPerMassUnit;
class SEScalarMassPerTime; class MassPerTimeUnit;
class SEScalarMassPerVolume; class MassPerVolumeUnit;
class SEScalarOsmolality; class OsmolalityUnit;
class SEScalarOsmolarity; class OsmolarityUnit;
class SEScalarPower; class PowerUnit;
class SEScalarPowerPerAreaTemperatureToTheFourth; class PowerPerAreaTemperatureToTheFourthUnit;
class SEScalarPressure; class PressureUnit;
class SEScalarPressurePerVolume; class PressurePerVolumeUnit;
class SEScalarPressureTimePerVolumeArea; class PressureTimePerVolumeAreaUnit;
class SEScalarPressureTimePerArea; class PressureTimePerAreaUnit;
class SEScalarTime; class TimeUnit;
class SEScalarTemperature; class TemperatureUnit;
class SEScalarVolume; class VolumeUnit;
class SEScalarVolumePerPressure; class VolumePerPressureUnit;
class SEScalarVolumePerTime; class VolumePerTimeUnit;
class SEScalarVolumePerTimeArea; class VolumePerTimeAreaUnit;
class SEScalarVolumePerTimePressureArea; class VolumePerTimePressureAreaUnit;
class SEScalarVolumePerTimeMass; class VolumePerTimeMassUnit;
class SEScalarVolumePerTimePressure; class VolumePerTimePressureUnit;

class SEFunction;
class SEFunctionVolumeVsTime;
class SEFunctionElectricPotentialVsTime;

class SEHistogram;
class SEHistogramFractionVsLength;
