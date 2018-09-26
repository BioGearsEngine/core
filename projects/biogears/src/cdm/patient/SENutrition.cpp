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

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/schema/cdm/Properties.hxx>
#include <biogears/schema/cdm/PatientNutrition.hxx>

namespace biogears {
SENutrition::SENutrition(Logger* logger)
  : Loggable(logger)
{
  m_Carbohydrate = nullptr;
  m_CarbohydrateDigestionRate = nullptr;
  m_Fat = nullptr;
  m_FatDigestionRate = nullptr;
  m_Protein = nullptr;
  m_ProteinDigestionRate = nullptr;
  m_Calcium = nullptr;
  m_Sodium = nullptr;
  m_Water = nullptr;
}

SENutrition::~SENutrition()
{
  Clear();
}

void SENutrition::Clear()
{
  SAFE_DELETE(m_Carbohydrate);
  SAFE_DELETE(m_CarbohydrateDigestionRate);
  SAFE_DELETE(m_Fat);
  SAFE_DELETE(m_FatDigestionRate);
  SAFE_DELETE(m_Protein);
  SAFE_DELETE(m_ProteinDigestionRate);
  SAFE_DELETE(m_Calcium);
  SAFE_DELETE(m_Sodium);
  SAFE_DELETE(m_Water);
}

double ComputeWeightedRate(double amt1, double amt2, double rate1, double rate2)
{
  return ((amt1 / (amt1 + amt2)) * rate1) + ((amt2 / (amt1 + amt2)) * rate2);
}
void SENutrition::Increment(const SENutrition& from)
{
  double weightedRate; //Unit independent
  if (from.HasCarbohydrate()) {
    if (HasCarbohydrateDigestionRate()) {
      // Compute the weighted rate (before we increment nutrient)
      weightedRate = ComputeWeightedRate(GetCarbohydrate(MassUnit::g), from.GetCarbohydrate(MassUnit::g),
        GetCarbohydrateDigestionRate(MassPerTimeUnit::g_Per_s), from.GetCarbohydrateDigestionRate(MassPerTimeUnit::g_Per_s));
      GetCarbohydrateDigestionRate().SetValue(weightedRate, MassPerTimeUnit::g_Per_s);
    } else {
      GetCarbohydrateDigestionRate().SetValue(from.GetCarbohydrateDigestionRate(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);
    }
    GetCarbohydrate().Increment(*from.m_Carbohydrate);
  }
  if (from.HasFat()) {
    if (HasFatDigestionRate()) {
      // Compute the weighted rate (before we increment nutrient)
      weightedRate = ComputeWeightedRate(GetFat(MassUnit::g), from.GetFat(MassUnit::g),
        GetFatDigestionRate(MassPerTimeUnit::g_Per_s), from.GetFatDigestionRate(MassPerTimeUnit::g_Per_s));
      GetFatDigestionRate().SetValue(weightedRate, MassPerTimeUnit::g_Per_s);
    } else {
      GetFatDigestionRate().SetValue(from.GetFatDigestionRate(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);
    }
    GetFat().Increment(*from.m_Fat);
  }
  if (from.HasProtein()) {
    if (HasProteinDigestionRate()) {
      // Compute the weighted rate (before we increment nutrient)
      weightedRate = ComputeWeightedRate(GetProtein(MassUnit::g), from.GetProtein(MassUnit::g),
        GetProteinDigestionRate(MassPerTimeUnit::g_Per_s), from.GetProteinDigestionRate(MassPerTimeUnit::g_Per_s));
      GetProteinDigestionRate().SetValue(weightedRate, MassPerTimeUnit::g_Per_s);
    } else {
      GetProteinDigestionRate().SetValue(from.GetProteinDigestionRate(MassPerTimeUnit::g_Per_s), MassPerTimeUnit::g_Per_s);
    }
    GetProtein().Increment(*from.m_Protein);
  }
  if (from.HasCalcium())
    GetCalcium().Increment(*from.m_Calcium);
  if (from.HasSodium())
    GetSodium().Increment(*from.m_Sodium);
  if (from.HasWater())
    GetWater().Increment(*from.m_Water);
}

bool SENutrition::Load(const CDM::NutritionData& in)
{
  Clear();

  if (in.Carbohydrate().present())
    GetCarbohydrate().Load(in.Carbohydrate().get());
  if (in.CarbohydrateDigestionRate().present())
    GetCarbohydrateDigestionRate().Load(in.CarbohydrateDigestionRate().get());
  if (in.Fat().present())
    GetFat().Load(in.Fat().get());
  if (in.FatDigestionRate().present())
    GetFatDigestionRate().Load(in.FatDigestionRate().get());
  if (in.Protein().present())
    GetProtein().Load(in.Protein().get());
  if (in.ProteinDigestionRate().present())
    GetProteinDigestionRate().Load(in.ProteinDigestionRate().get());
  if (in.Calcium().present())
    GetCalcium().Load(in.Calcium().get());
  if (in.Sodium().present())
    GetSodium().Load(in.Sodium().get());
  if (in.Water().present())
    GetWater().Load(in.Water().get());
  return true;
}

CDM::NutritionData* SENutrition::Unload() const
{
  CDM::NutritionData* data = new CDM::NutritionData();
  Unload(*data);
  return data;
}

void SENutrition::Unload(CDM::NutritionData& data) const
{
  if (m_Carbohydrate != nullptr)
    data.Carbohydrate(std::unique_ptr<CDM::ScalarMassData>(m_Carbohydrate->Unload()));
  if (m_CarbohydrateDigestionRate != nullptr)
    data.CarbohydrateDigestionRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_CarbohydrateDigestionRate->Unload()));
  if (m_Fat != nullptr)
    data.Fat(std::unique_ptr<CDM::ScalarMassData>(m_Fat->Unload()));
  if (m_FatDigestionRate != nullptr)
    data.FatDigestionRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_FatDigestionRate->Unload()));
  if (m_Protein != nullptr)
    data.Protein(std::unique_ptr<CDM::ScalarMassData>(m_Protein->Unload()));
  if (m_ProteinDigestionRate != nullptr)
    data.ProteinDigestionRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_ProteinDigestionRate->Unload()));
  if (m_Calcium != nullptr)
    data.Calcium(std::unique_ptr<CDM::ScalarMassData>(m_Calcium->Unload()));
  if (m_Sodium != nullptr)
    data.Sodium(std::unique_ptr<CDM::ScalarMassData>(m_Sodium->Unload()));
  if (m_Water != nullptr)
    data.Water(std::unique_ptr<CDM::ScalarVolumeData>(m_Water->Unload()));
}

const SEScalar* SENutrition::GetScalar(const std::string& name)
{
  if (name.compare("Carbohydrate") == 0)
    return &GetCarbohydrate();
  if (name.compare("CarbohydrateDigestionRate") == 0)
    return &GetCarbohydrateDigestionRate();
  if (name.compare("Fat") == 0)
    return &GetFat();
  if (name.compare("FatDigestionRate") == 0)
    return &GetFatDigestionRate();
  if (name.compare("Protein") == 0)
    return &GetProtein();
  if (name.compare("ProteinDigestionRate") == 0)
    return &GetProteinDigestionRate();
  if (name.compare("Calcium") == 0)
    return &GetCalcium();
  if (name.compare("Sodium") == 0)
    return &GetSodium();
  if (name.compare("Water") == 0)
    return &GetWater();
  return nullptr;
}

bool SENutrition::Load(const std::string& nutritionFile)
{
  CDM::NutritionData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  std::string nFile = nutritionFile;
  if (nFile.find("/nutrition") == std::string::npos) {
    nFile = "./nutrition/";
    nFile += nutritionFile;
  }

  data = Serializer::ReadFile(nFile, GetLogger());
  pData = dynamic_cast<CDM::NutritionData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Nutrition file could not be read : " << nutritionFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}

bool SENutrition::HasCarbohydrate() const
{
  return m_Carbohydrate == nullptr ? false : m_Carbohydrate->IsValid();
}
SEScalarMass& SENutrition::GetCarbohydrate()
{
  if (m_Carbohydrate == nullptr)
    m_Carbohydrate = new SEScalarMass();
  return *m_Carbohydrate;
}
double SENutrition::GetCarbohydrate(const MassUnit& unit) const
{
  if (m_Carbohydrate == nullptr)
    return SEScalar::dNaN();
  return m_Carbohydrate->GetValue(unit);
}

bool SENutrition::HasCarbohydrateDigestionRate() const
{
  return m_CarbohydrateDigestionRate == nullptr ? false : m_CarbohydrateDigestionRate->IsValid();
}
SEScalarMassPerTime& SENutrition::GetCarbohydrateDigestionRate()
{
  if (m_CarbohydrateDigestionRate == nullptr)
    m_CarbohydrateDigestionRate = new SEScalarMassPerTime();
  return *m_CarbohydrateDigestionRate;
}
double SENutrition::GetCarbohydrateDigestionRate(const MassPerTimeUnit& unit) const
{
  if (m_CarbohydrateDigestionRate == nullptr)
    return SEScalar::dNaN();
  return m_CarbohydrateDigestionRate->GetValue(unit);
}

bool SENutrition::HasFat() const
{
  return m_Fat == nullptr ? false : m_Fat->IsValid();
}
SEScalarMass& SENutrition::GetFat()
{
  if (m_Fat == nullptr)
    m_Fat = new SEScalarMass();
  return *m_Fat;
}
double SENutrition::GetFat(const MassUnit& unit) const
{
  if (m_Fat == nullptr)
    return SEScalar::dNaN();
  return m_Fat->GetValue(unit);
}

bool SENutrition::HasFatDigestionRate() const
{
  return m_FatDigestionRate == nullptr ? false : m_FatDigestionRate->IsValid();
}
SEScalarMassPerTime& SENutrition::GetFatDigestionRate()
{
  if (m_FatDigestionRate == nullptr)
    m_FatDigestionRate = new SEScalarMassPerTime();
  return *m_FatDigestionRate;
}
double SENutrition::GetFatDigestionRate(const MassPerTimeUnit& unit) const
{
  if (m_FatDigestionRate == nullptr)
    return SEScalar::dNaN();
  return m_FatDigestionRate->GetValue(unit);
}

bool SENutrition::HasProtein() const
{
  return m_Protein == nullptr ? false : m_Protein->IsValid();
}
SEScalarMass& SENutrition::GetProtein()
{
  if (m_Protein == nullptr)
    m_Protein = new SEScalarMass();
  return *m_Protein;
}
double SENutrition::GetProtein(const MassUnit& unit) const
{
  if (m_Protein == nullptr)
    return SEScalar::dNaN();
  return m_Protein->GetValue(unit);
}

bool SENutrition::HasProteinDigestionRate() const
{
  return m_ProteinDigestionRate == nullptr ? false : m_ProteinDigestionRate->IsValid();
}
SEScalarMassPerTime& SENutrition::GetProteinDigestionRate()
{
  if (m_ProteinDigestionRate == nullptr)
    m_ProteinDigestionRate = new SEScalarMassPerTime();
  return *m_ProteinDigestionRate;
}
double SENutrition::GetProteinDigestionRate(const MassPerTimeUnit& unit) const
{
  if (m_ProteinDigestionRate == nullptr)
    return SEScalar::dNaN();
  return m_ProteinDigestionRate->GetValue(unit);
}

bool SENutrition::HasCalcium() const
{
  return m_Calcium == nullptr ? false : m_Calcium->IsValid();
}
SEScalarMass& SENutrition::GetCalcium()
{
  if (m_Calcium == nullptr)
    m_Calcium = new SEScalarMass();
  return *m_Calcium;
}
double SENutrition::GetCalcium(const MassUnit& unit) const
{
  if (m_Calcium == nullptr)
    return SEScalar::dNaN();
  return m_Calcium->GetValue(unit);
}

bool SENutrition::HasSodium() const
{
  return m_Sodium == nullptr ? false : m_Sodium->IsValid();
}
SEScalarMass& SENutrition::GetSodium()
{
  if (m_Sodium == nullptr)
    m_Sodium = new SEScalarMass();
  return *m_Sodium;
}
double SENutrition::GetSodium(const MassUnit& unit) const
{
  if (m_Sodium == nullptr)
    return SEScalar::dNaN();
  return m_Sodium->GetValue(unit);
}

bool SENutrition::HasWater() const
{
  return m_Water == nullptr ? false : m_Water->IsValid();
}
SEScalarVolume& SENutrition::GetWater()
{
  if (m_Water == nullptr)
    m_Water = new SEScalarVolume();
  return *m_Water;
}
double SENutrition::GetWater(const VolumeUnit& unit) const
{
  if (m_Water == nullptr)
    return SEScalar::dNaN();
  return m_Water->GetValue(unit);
}

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

void SENutrition::ToString(std::ostream& str) const
{
  str << "Nutrient Contents";
  str << "\n\tCharbohydrates: ";
  HasCarbohydrate() ? str << *m_Carbohydrate : str << "None";
  str << "\n\tCharbohydrates Digestion Rate: ";
  HasCarbohydrateDigestionRate() ? str << *m_CarbohydrateDigestionRate : str << "None";
  str << "\n\tFat: ";
  HasFat() ? str << *m_Fat : str << "None";
  str << "\n\tFat Digestion Rate: ";
  HasFatDigestionRate() ? str << *m_FatDigestionRate : str << "None";
  str << "\n\tProtein: ";
  HasProtein() ? str << *m_Protein : str << "None";
  str << "\n\tProtein Digestion Rate: ";
  HasProteinDigestionRate() ? str << *m_ProteinDigestionRate : str << "None";
  str << "\n\tCalcium: ";
  HasCalcium() ? str << *m_Calcium : str << "None";
  str << "\n\tSodium: ";
  HasSodium() ? str << *m_Sodium : str << "None";
  str << "\n\tWater: ";
  HasWater() ? str << *m_Water : str << "None";
  str << std::flush;
}
}