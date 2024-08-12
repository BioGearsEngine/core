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
#include <biogears/cdm/patient/SENutrition.h>

#include "io/cdm/Patient.h"
#include "io/cdm/PatientNutrition.h"

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/PatientNutrition.hxx>
#include <biogears/schema/cdm/Properties.hxx>

#ifdef BIOGEARS_IO_PRESENT
#include <biogears/io/directories/nutrition.h>
#endif

namespace biogears {
SENutrition::SENutrition(Logger* logger)
  : Loggable(logger)
{
  m_Name = "Standard Meal";
  m_Carbohydrate = nullptr;
  m_Fat = nullptr;
  m_Protein = nullptr;
  m_Calcium = nullptr;
  m_Sodium = nullptr;
  m_Water = nullptr;
}
//-----------------------------------------------------------------------------
SENutrition::~SENutrition()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SENutrition::Clear()
{
  m_Name = "";
  SAFE_DELETE(m_Carbohydrate);
  SAFE_DELETE(m_Fat);
  SAFE_DELETE(m_Protein);
  SAFE_DELETE(m_Calcium);
  SAFE_DELETE(m_Sodium);
  SAFE_DELETE(m_Water);
}
//-----------------------------------------------------------------------------
double ComputeWeightedRate(double amt1, double amt2, double rate1, double rate2)
{
  return ((amt1 / (amt1 + amt2)) * rate1) + ((amt2 / (amt1 + amt2)) * rate2);
}
//-----------------------------------------------------------------------------
void SENutrition::Increment(const SENutrition& from)
{
  if (from.HasCarbohydrate()) {
    GetCarbohydrate().Increment(*from.m_Carbohydrate);
  }
  if (from.HasFat()) {
    GetFat().Increment(*from.m_Fat);
  }
  if (from.HasProtein()) {
    GetProtein().Increment(*from.m_Protein);
  }
  if (from.HasCalcium())
    GetCalcium().Increment(*from.m_Calcium);
  if (from.HasSodium())
    GetSodium().Increment(*from.m_Sodium);
  if (from.HasWater())
    GetWater().Increment(*from.m_Water);
}
//-----------------------------------------------------------------------------
const SEScalar* SENutrition::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-----------------------------------------------------------------------------
const SEScalar* SENutrition::GetScalar(const std::string& name)
{
  if (name.compare("Carbohydrate") == 0)
    return &GetCarbohydrate();
  if (name.compare("Fat") == 0)
    return &GetFat();
  if (name.compare("Protein") == 0)
    return &GetProtein();
  if (name.compare("Calcium") == 0)
    return &GetCalcium();
  if (name.compare("Sodium") == 0)
    return &GetSodium();
  if (name.compare("Water") == 0)
    return &GetWater();
  return nullptr;
}
//-----------------------------------------------------------------------------
bool SENutrition::Load(const char* nutritionFile)
{
  return Load(std::string { nutritionFile });
}
//-----------------------------------------------------------------------------
bool SENutrition::Load(const std::string& given)
{
  CDM::NutritionData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  auto io = m_Logger->GetIoManager().lock();
  auto possible_path = io->FindNutritionFile(given.c_str());
  if (possible_path.empty()) {
#ifdef BIOGEARS_IO_PRESENT
    size_t content_size;
    auto content = io::get_embedded_nutrition_file(given.c_str(), content_size);
    data = Serializer::ReadBuffer((XMLByte*)content, content_size, m_Logger);
#endif
  } else {
    data = Serializer::ReadFile(possible_path, m_Logger);
  }

  pData = dynamic_cast<CDM::NutritionData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Nutrition file could not be read : " << io->find_resource_file(given.c_str()) << std::endl;
    Error(ss);
    return false;
  }
  io::PatientNutrition::UnMarshall(*pData, *this);
  return true;
}
//-----------------------------------------------------------------------------
std::string SENutrition::GetName() const
{
  return m_Name;
}
//-----------------------------------------------------------------------------
const char* SENutrition::GetName_cStr() const
{
  return m_Name.c_str();
}
//-----------------------------------------------------------------------------
void SENutrition::SetName(const char* name)
{
  return SetName(std::string { name });
}
//-----------------------------------------------------------------------------
void SENutrition::SetName(const std::string& name)
{
  m_Name = name;
}
//-----------------------------------------------------------------------------
bool SENutrition::HasName() const
{
  return m_Name.empty() ? false : true;
}
//-----------------------------------------------------------------------------
void SENutrition::InvalidateName()
{
  m_Name = "";
}
//-----------------------------------------------------------------------------
bool SENutrition::HasCarbohydrate() const
{
  return m_Carbohydrate == nullptr ? false : m_Carbohydrate->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SENutrition::GetCarbohydrate()
{
  if (m_Carbohydrate == nullptr)
    m_Carbohydrate = new SEScalarMass();
  return *m_Carbohydrate;
}
//-----------------------------------------------------------------------------
double SENutrition::GetCarbohydrate(const MassUnit& unit) const
{
  if (m_Carbohydrate == nullptr)
    return SEScalar::dNaN();
  return m_Carbohydrate->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SENutrition::HasFat() const
{
  return m_Fat == nullptr ? false : m_Fat->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SENutrition::GetFat()
{
  if (m_Fat == nullptr)
    m_Fat = new SEScalarMass();
  return *m_Fat;
}
//-----------------------------------------------------------------------------
double SENutrition::GetFat(const MassUnit& unit) const
{
  if (m_Fat == nullptr)
    return SEScalar::dNaN();
  return m_Fat->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SENutrition::HasProtein() const
{
  return m_Protein == nullptr ? false : m_Protein->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SENutrition::GetProtein()
{
  if (m_Protein == nullptr)
    m_Protein = new SEScalarMass();
  return *m_Protein;
}
//-----------------------------------------------------------------------------
double SENutrition::GetProtein(const MassUnit& unit) const
{
  if (m_Protein == nullptr)
    return SEScalar::dNaN();
  return m_Protein->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SENutrition::HasCalcium() const
{
  return m_Calcium == nullptr ? false : m_Calcium->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SENutrition::GetCalcium()
{
  if (m_Calcium == nullptr)
    m_Calcium = new SEScalarMass();
  return *m_Calcium;
}
//-----------------------------------------------------------------------------
double SENutrition::GetCalcium(const MassUnit& unit) const
{
  if (m_Calcium == nullptr)
    return SEScalar::dNaN();
  return m_Calcium->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SENutrition::HasSodium() const
{
  return m_Sodium == nullptr ? false : m_Sodium->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarMass& SENutrition::GetSodium()
{
  if (m_Sodium == nullptr)
    m_Sodium = new SEScalarMass();
  return *m_Sodium;
}
//-----------------------------------------------------------------------------
double SENutrition::GetSodium(const MassUnit& unit) const
{
  if (m_Sodium == nullptr)
    return SEScalar::dNaN();
  return m_Sodium->GetValue(unit);
}
//-----------------------------------------------------------------------------
bool SENutrition::HasWater() const
{
  return m_Water == nullptr ? false : m_Water->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarVolume& SENutrition::GetWater()
{
  if (m_Water == nullptr)
    m_Water = new SEScalarVolume();
  return *m_Water;
}
//-----------------------------------------------------------------------------
double SENutrition::GetWater(const VolumeUnit& unit) const
{
  if (m_Water == nullptr)
    return SEScalar::dNaN();
  return m_Water->GetValue(unit);
}
//-----------------------------------------------------------------------------
double SENutrition::GetWeight(const MassUnit& unit) const
{
  double w = 0;
  if (HasCarbohydrate())
    w += GetCarbohydrate(unit);
  if (HasFat())
    w += GetFat(unit);
  if (HasProtein())
    w += GetProtein(unit);
  if (HasCalcium())
    w += GetCalcium(unit);
  if (HasSodium())
    w += GetSodium(unit);
  if (HasWater())
    w += Convert(GetWater(VolumeUnit::mL), MassUnit::g, unit);
  return w;
}
//-----------------------------------------------------------------------------
bool SENutrition::IsValid() const {
  //Todo: Refine the meaning if IsValid nutrition. Is it a meal with all members at least at Zero 
  //      or is it a meal with a Name and at least one member.
  return HasName()
    && (HasCarbohydrate()
    || HasFat()
    || HasProtein()
    || HasCalcium()
    || HasSodium()
    || HasWater());
}
//-----------------------------------------------------------------------------
void SENutrition::ToString(std::ostream& str) const
{
  str << "Nutrient Contents";
  str << "\n\tCharbohydrates: ";
  HasCarbohydrate() ? str << *m_Carbohydrate : str << "None";
  str << "\n\tFat: ";
  HasFat() ? str << *m_Fat : str << "None";
  str << "\n\tProtein: ";
  HasProtein() ? str << *m_Protein : str << "None";
  str << "\n\tCalcium: ";
  HasCalcium() ? str << *m_Calcium : str << "None";
  str << "\n\tSodium: ";
  HasSodium() ? str << *m_Sodium : str << "None";
  str << "\n\tWater: ";
  HasWater() ? str << *m_Water : str << "None";
  str << std::flush;
}
//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
bool SENutrition::operator==(const SENutrition& rhs) const
{
  bool equivilant = m_Name == rhs.m_Name;
  equivilant &= (m_Carbohydrate && rhs.m_Carbohydrate) ? m_Carbohydrate->operator==(*rhs.m_Carbohydrate) : m_Carbohydrate == rhs.m_Carbohydrate;
  equivilant &= (m_Fat && rhs.m_Fat) ? m_Fat->operator==(*rhs.m_Fat) : m_Fat == rhs.m_Fat;
  equivilant &= (m_Protein && rhs.m_Protein) ? m_Protein->operator==(*rhs.m_Protein) : m_Protein == rhs.m_Protein;
  equivilant &= (m_Calcium && rhs.m_Calcium) ? m_Calcium->operator==(*rhs.m_Calcium) : m_Calcium == rhs.m_Calcium;
  equivilant &= (m_Sodium && rhs.m_Sodium) ? m_Sodium->operator==(*rhs.m_Sodium) : m_Sodium == rhs.m_Sodium;
  equivilant &= (m_Water && rhs.m_Water) ? m_Water->operator==(*rhs.m_Water) : m_Water == rhs.m_Water;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SENutrition::operator!=(const SENutrition& rhs) const
{
  return !(*this == rhs);
}
}
