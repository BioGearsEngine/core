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
#include <biogears/cdm/system/physiology/SEEnergySystem.h>

#include <biogears/cdm/properties/SEScalarAmountPerTime.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/container/Tree.tci.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
constexpr char idAchievedExerciseLevel[] = "AchievedExerciseLevel";
constexpr char idChlorideLostToSweat[] = "ChlorideLostToSweat";
constexpr char idCoreTemperature[] = "CoreTemperature";
constexpr char idCreatinineProductionRate[] = "CreatinineProductionRate";
constexpr char idEnergyDeficit[] = "EnergyDeficit";
constexpr char idExerciseEnergyDemand[] = "ExerciseEnergyDemand";
constexpr char idExerciseMeanArterialPressureDelta[] = "ExerciseMeanArterialPressureDelta";
constexpr char idFatigueLevel[] = "FatigueLevel";
constexpr char idLactateProductionRate[] = "LactateProductionRate";
constexpr char idPotassiumLostToSweat[] = "PotassiumLostToSweat";
constexpr char idSkinTemperature[] = "SkinTemperature";
constexpr char idSodiumLostToSweat[] = "SodiumLostToSweat";
constexpr char idSweatRate[] = "SweatRate";
constexpr char idTotalMetabolicRate[] = "TotalMetabolicRate";
constexpr char idTotalWorkRateLevel[] = "TotalWorkRateLevel";

SEEnergySystem::SEEnergySystem(Logger* logger)
  : SESystem(logger)
{
  m_AchievedExerciseLevel = nullptr;
  m_ChlorideLostToSweat = nullptr;
  m_CoreTemperature = nullptr;
  m_CreatinineProductionRate = nullptr;
  m_EnergyDeficit = nullptr;
  m_ExerciseEnergyDemand = nullptr;
  m_ExerciseMeanArterialPressureDelta = nullptr;
  m_FatigueLevel = nullptr;
  m_LactateProductionRate = nullptr;
  m_PotassiumLostToSweat = nullptr;
  m_SkinTemperature = nullptr;
  m_SodiumLostToSweat = nullptr;
  m_SweatRate = nullptr;
  m_TotalMetabolicRate = nullptr;
  m_TotalWorkRateLevel = nullptr;
}
//-------------------------------------------------------------------------------

SEEnergySystem::~SEEnergySystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEEnergySystem::Clear()
{
  SESystem::Clear();

  SAFE_DELETE(m_AchievedExerciseLevel);
  SAFE_DELETE(m_ChlorideLostToSweat);
  SAFE_DELETE(m_CoreTemperature);
  SAFE_DELETE(m_CreatinineProductionRate);
  SAFE_DELETE(m_EnergyDeficit);
  SAFE_DELETE(m_ExerciseEnergyDemand);
  SAFE_DELETE(m_ExerciseMeanArterialPressureDelta);
  SAFE_DELETE(m_FatigueLevel);
  SAFE_DELETE(m_LactateProductionRate);
  SAFE_DELETE(m_PotassiumLostToSweat);
  SAFE_DELETE(m_SkinTemperature);
  SAFE_DELETE(m_SodiumLostToSweat);
  SAFE_DELETE(m_SweatRate);
  SAFE_DELETE(m_TotalMetabolicRate);
  SAFE_DELETE(m_TotalWorkRateLevel);
} //-------------------------------------------------------------------------------
const SEScalar* SEEnergySystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEEnergySystem::GetScalar(const std::string& name)
{
  if (name == idAchievedExerciseLevel)
    return &GetAchievedExerciseLevel();
  if (name == idChlorideLostToSweat)
    return &GetChlorideLostToSweat();
  if (name == idCoreTemperature)
    return &GetCoreTemperature();
  if (name == idCreatinineProductionRate)
    return &GetCreatinineProductionRate();
  if (name == idEnergyDeficit)
    return &GetEnergyDeficit();
  if (name == idExerciseEnergyDemand)
    return &GetExerciseEnergyDemand();
  if (name == idExerciseMeanArterialPressureDelta)
    return &GetExerciseMeanArterialPressureDelta();
  if (name == idFatigueLevel)
    return &GetFatigueLevel();
  if (name == idLactateProductionRate)
    return &GetLactateProductionRate();
  if (name == idPotassiumLostToSweat)
    return &GetPotassiumLostToSweat();
  if (name == idSkinTemperature)
    return &GetSkinTemperature();
  if (name == idSodiumLostToSweat)
    return &GetSodiumLostToSweat();
  if (name == idSweatRate)
    return &GetSweatRate();
  if (name == idTotalMetabolicRate)
    return &GetTotalMetabolicRate();
  if (name == idTotalWorkRateLevel)
    return &GetTotalWorkRateLevel();
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::Load(const CDM::EnergySystemData& in)
{
  SESystem::Load(in);

  if (in.AchievedExerciseLevel().present())
    GetAchievedExerciseLevel().Load(in.AchievedExerciseLevel().get());
  if (in.ChlorideLostToSweat().present())
    GetChlorideLostToSweat().Load(in.ChlorideLostToSweat().get());
  if (in.CoreTemperature().present())
    GetCoreTemperature().Load(in.CoreTemperature().get());
  if (in.CreatinineProductionRate().present())
    GetCreatinineProductionRate().Load(in.CreatinineProductionRate().get());
  if (in.EnergyDeficit().present())
    GetEnergyDeficit().Load(in.EnergyDeficit().get());
  if (in.ExerciseEnergyDemand().present())
    GetExerciseEnergyDemand().Load(in.ExerciseEnergyDemand().get());
  if (in.ExerciseMeanArterialPressureDelta().present())
    GetExerciseMeanArterialPressureDelta().Load(in.ExerciseMeanArterialPressureDelta().get());
  if (in.FatigueLevel().present())
    GetFatigueLevel().Load(in.FatigueLevel().get());
  if (in.LactateProductionRate().present())
    GetLactateProductionRate().Load(in.LactateProductionRate().get());
  if (in.PotassiumLostToSweat().present())
    GetPotassiumLostToSweat().Load(in.PotassiumLostToSweat().get());
  if (in.SkinTemperature().present())
    GetSkinTemperature().Load(in.SkinTemperature().get());
  if (in.SodiumLostToSweat().present())
    GetSodiumLostToSweat().Load(in.SodiumLostToSweat().get());
  if (in.SweatRate().present())
    GetSweatRate().Load(in.SweatRate().get());
  if (in.TotalMetabolicRate().present())
    GetTotalMetabolicRate().Load(in.TotalMetabolicRate().get());
  if (in.TotalWorkRateLevel().present())
    GetTotalWorkRateLevel().Load(in.TotalWorkRateLevel().get());

  return true;
}
//-------------------------------------------------------------------------------

CDM::EnergySystemData* SEEnergySystem::Unload() const
{
  CDM::EnergySystemData* data(new CDM::EnergySystemData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEEnergySystem::Unload(CDM::EnergySystemData& data) const
{
  SESystem::Unload(data);

  if (m_AchievedExerciseLevel != nullptr)
    data.AchievedExerciseLevel(std::unique_ptr<CDM::ScalarFractionData>(m_AchievedExerciseLevel->Unload()));
  if (m_ChlorideLostToSweat != nullptr)
    data.ChlorideLostToSweat(std::unique_ptr<CDM::ScalarMassData>(m_ChlorideLostToSweat->Unload()));
  if (m_CoreTemperature != nullptr)
    data.CoreTemperature(std::unique_ptr<CDM::ScalarTemperatureData>(m_CoreTemperature->Unload()));
  if (m_CreatinineProductionRate != nullptr)
    data.CreatinineProductionRate(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_CreatinineProductionRate->Unload()));
  if (m_EnergyDeficit != nullptr)
    data.EnergyDeficit(std::unique_ptr<CDM::ScalarPowerData>(m_EnergyDeficit->Unload()));
  if (m_ExerciseEnergyDemand != nullptr)
    data.ExerciseEnergyDemand(std::unique_ptr<CDM::ScalarPowerData>(m_ExerciseEnergyDemand->Unload()));
  if (m_ExerciseMeanArterialPressureDelta != nullptr)
    data.ExerciseMeanArterialPressureDelta(std::unique_ptr<CDM::ScalarPressureData>(m_ExerciseMeanArterialPressureDelta->Unload()));
  if (m_FatigueLevel != nullptr)
    data.FatigueLevel(std::unique_ptr<CDM::ScalarFractionData>(m_FatigueLevel->Unload()));
  if (m_LactateProductionRate != nullptr)
    data.LactateProductionRate(std::unique_ptr<CDM::ScalarAmountPerTimeData>(m_LactateProductionRate->Unload()));
  if (m_PotassiumLostToSweat != nullptr)
    data.PotassiumLostToSweat(std::unique_ptr<CDM::ScalarMassData>(m_PotassiumLostToSweat->Unload()));
  if (m_SkinTemperature != nullptr)
    data.SkinTemperature(std::unique_ptr<CDM::ScalarTemperatureData>(m_SkinTemperature->Unload()));
  if (m_SodiumLostToSweat != nullptr)
    data.SodiumLostToSweat(std::unique_ptr<CDM::ScalarMassData>(m_SodiumLostToSweat->Unload()));
  if (m_SweatRate != nullptr)
    data.SweatRate(std::unique_ptr<CDM::ScalarMassPerTimeData>(m_SweatRate->Unload()));
  if (m_TotalMetabolicRate != nullptr)
    data.TotalMetabolicRate(std::unique_ptr<CDM::ScalarPowerData>(m_TotalMetabolicRate->Unload()));
  if (m_TotalWorkRateLevel != nullptr)
    data.TotalWorkRateLevel(std::unique_ptr<CDM::ScalarFractionData>(m_TotalWorkRateLevel->Unload()));
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasAchievedExerciseLevel() const
{
  return m_AchievedExerciseLevel == nullptr ? false : m_AchievedExerciseLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEEnergySystem::GetAchievedExerciseLevel()
{
  if (m_AchievedExerciseLevel == nullptr)
    m_AchievedExerciseLevel = new SEScalarFraction();
  return *m_AchievedExerciseLevel;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetAchievedExerciseLevel() const
{
  if (m_AchievedExerciseLevel == nullptr)
    return SEScalar::dNaN();
  return m_AchievedExerciseLevel->GetValue();
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasChlorideLostToSweat() const
{
  return m_ChlorideLostToSweat == nullptr ? false : m_ChlorideLostToSweat->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEEnergySystem::GetChlorideLostToSweat()
{
  if (m_ChlorideLostToSweat == nullptr)
    m_ChlorideLostToSweat = new SEScalarMass();
  return *m_ChlorideLostToSweat;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetChlorideLostToSweat(const MassUnit& unit) const
{
  if (m_ChlorideLostToSweat == nullptr)
    return SEScalar::dNaN();
  return m_ChlorideLostToSweat->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasCoreTemperature() const
{
  return m_CoreTemperature == nullptr ? false : m_CoreTemperature->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEEnergySystem::GetCoreTemperature()
{
  if (m_CoreTemperature == nullptr)
    m_CoreTemperature = new SEScalarTemperature();
  return *m_CoreTemperature;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetCoreTemperature(const TemperatureUnit& unit) const
{
  if (m_CoreTemperature == nullptr)
    return SEScalar::dNaN();
  return m_CoreTemperature->GetValue(unit);
}

bool SEEnergySystem::HasCreatinineProductionRate() const
{
  return m_CreatinineProductionRate == nullptr ? false : m_CreatinineProductionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime& SEEnergySystem::GetCreatinineProductionRate()
{
  if (m_CreatinineProductionRate == nullptr)
    m_CreatinineProductionRate = new SEScalarAmountPerTime();
  return *m_CreatinineProductionRate;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetCreatinineProductionRate(const AmountPerTimeUnit& unit) const
{
  if (m_CreatinineProductionRate == nullptr)
    return SEScalar::dNaN();
  return m_CreatinineProductionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEEnergySystem::HasEnergyDeficit() const
{
  return m_EnergyDeficit == nullptr ? false : m_EnergyDeficit->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnergySystem::GetEnergyDeficit()
{
  if (m_EnergyDeficit == nullptr)
    m_EnergyDeficit = new SEScalarPower();
  return *m_EnergyDeficit;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetEnergyDeficit(const PowerUnit& unit) const
{
  if (m_EnergyDeficit == nullptr)
    return SEScalar::dNaN();
  return m_EnergyDeficit->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEEnergySystem::HasExerciseEnergyDemand() const
{
  return m_ExerciseEnergyDemand == nullptr ? false : m_ExerciseEnergyDemand->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnergySystem::GetExerciseEnergyDemand()
{
  if (m_ExerciseEnergyDemand == nullptr)
    m_ExerciseEnergyDemand = new SEScalarPower();
  return *m_ExerciseEnergyDemand;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetExerciseEnergyDemand(const PowerUnit& unit) const
{
  if (m_ExerciseEnergyDemand == nullptr)
    return SEScalar::dNaN();
  return m_ExerciseEnergyDemand->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEEnergySystem::HasExerciseMeanArterialPressureDelta() const
{
  return m_ExerciseMeanArterialPressureDelta == nullptr ? false : m_ExerciseMeanArterialPressureDelta->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEEnergySystem::GetExerciseMeanArterialPressureDelta()
{
  if (m_ExerciseMeanArterialPressureDelta == nullptr)
    m_ExerciseMeanArterialPressureDelta = new SEScalarPressure();
  return *m_ExerciseMeanArterialPressureDelta;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetExerciseMeanArterialPressureDelta(const PressureUnit& unit) const
{
  if (m_ExerciseMeanArterialPressureDelta == nullptr)
    return SEScalar::dNaN();
  return m_ExerciseMeanArterialPressureDelta->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasFatigueLevel() const
{
  return m_FatigueLevel == nullptr ? false : m_FatigueLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEEnergySystem::GetFatigueLevel()
{
  if (m_FatigueLevel == nullptr)
    m_FatigueLevel = new SEScalarFraction();
  return *m_FatigueLevel;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetFatigueLevel() const
{
  if (m_FatigueLevel == nullptr)
    return SEScalar::dNaN();
  return m_FatigueLevel->GetValue();
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasLactateProductionRate() const
{
  return m_LactateProductionRate == nullptr ? false : m_LactateProductionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerTime& SEEnergySystem::GetLactateProductionRate()
{
  if (m_LactateProductionRate == nullptr)
    m_LactateProductionRate = new SEScalarAmountPerTime();
  return *m_LactateProductionRate;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetLactateProductionRate(const AmountPerTimeUnit& unit) const
{
  if (m_LactateProductionRate == nullptr)
    return SEScalar::dNaN();
  return m_LactateProductionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasPotassiumLostToSweat() const
{
  return m_PotassiumLostToSweat == nullptr ? false : m_PotassiumLostToSweat->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEEnergySystem::GetPotassiumLostToSweat()
{
  if (m_PotassiumLostToSweat == nullptr)
    m_PotassiumLostToSweat = new SEScalarMass();
  return *m_PotassiumLostToSweat;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetPotassiumLostToSweat(const MassUnit& unit) const
{
  if (m_PotassiumLostToSweat == nullptr)
    return SEScalar::dNaN();
  return m_PotassiumLostToSweat->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasSkinTemperature() const
{
  return m_SkinTemperature == nullptr ? false : m_SkinTemperature->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTemperature& SEEnergySystem::GetSkinTemperature()
{
  if (m_SkinTemperature == nullptr)
    m_SkinTemperature = new SEScalarTemperature();
  return *m_SkinTemperature;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetSkinTemperature(const TemperatureUnit& unit) const
{
  if (m_SkinTemperature == nullptr)
    return SEScalar::dNaN();
  return m_SkinTemperature->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasSodiumLostToSweat() const
{
  return m_SodiumLostToSweat == nullptr ? false : m_SodiumLostToSweat->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEEnergySystem::GetSodiumLostToSweat()
{
  if (m_SodiumLostToSweat == nullptr)
    m_SodiumLostToSweat = new SEScalarMass();
  return *m_SodiumLostToSweat;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetSodiumLostToSweat(const MassUnit& unit) const
{
  if (m_SodiumLostToSweat == nullptr)
    return SEScalar::dNaN();
  return m_SodiumLostToSweat->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasSweatRate() const
{
  return m_SweatRate == nullptr ? false : m_SweatRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerTime& SEEnergySystem::GetSweatRate()
{
  if (m_SweatRate == nullptr)
    m_SweatRate = new SEScalarMassPerTime();
  return *m_SweatRate;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetSweatRate(const MassPerTimeUnit& unit) const
{
  if (m_SweatRate == nullptr)
    return SEScalar::dNaN();
  return m_SweatRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasTotalMetabolicRate() const
{
  return m_TotalMetabolicRate == nullptr ? false : m_TotalMetabolicRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPower& SEEnergySystem::GetTotalMetabolicRate()
{
  if (m_TotalMetabolicRate == nullptr)
    m_TotalMetabolicRate = new SEScalarPower();
  return *m_TotalMetabolicRate;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetTotalMetabolicRate(const PowerUnit& unit) const
{
  if (m_TotalMetabolicRate == nullptr)
    return SEScalar::dNaN();
  return m_TotalMetabolicRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEEnergySystem::HasTotalWorkRateLevel() const
{
  return m_TotalWorkRateLevel == nullptr ? false : m_TotalWorkRateLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEEnergySystem::GetTotalWorkRateLevel()
{
  if (m_TotalWorkRateLevel == nullptr)
    m_TotalWorkRateLevel = new SEScalarFraction();
  return *m_TotalWorkRateLevel;
}
//-------------------------------------------------------------------------------
double SEEnergySystem::GetTotalWorkRateLevel() const
{
  if (m_TotalWorkRateLevel == nullptr)
    return SEScalar::dNaN();
  return m_TotalWorkRateLevel->GetValue();
}
//-------------------------------------------------------------------------------
Tree<const char*> SEEnergySystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{ classname() }
    .emplace_back(idAchievedExerciseLevel)
    .emplace_back(idChlorideLostToSweat)
    .emplace_back(idCoreTemperature)
    .emplace_back(idCreatinineProductionRate)
    .emplace_back(idExerciseMeanArterialPressureDelta)
    .emplace_back(idFatigueLevel)
    .emplace_back(idLactateProductionRate)
    .emplace_back(idPotassiumLostToSweat)
    .emplace_back(idSkinTemperature)
    .emplace_back(idSodiumLostToSweat)
    .emplace_back(idSweatRate)
    .emplace_back(idTotalMetabolicRate)
    .emplace_back(idTotalWorkRateLevel);
}
}