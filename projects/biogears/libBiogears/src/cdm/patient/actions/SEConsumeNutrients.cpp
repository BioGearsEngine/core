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
#include <biogears/cdm/patient/actions/SEConsumeNutrients.h>

#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

namespace biogears {
  SEConsumeNutrients::SEConsumeNutrients()
    : SEPatientAction()
  {
    m_Nutrition = nullptr;
    InvalidateNutritionFile();
  }
  //-----------------------------------------------------------------------------
  SEConsumeNutrients::~SEConsumeNutrients()
  {
    Clear();
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::Clear()
  {
    SEPatientAction::Clear();
    SAFE_DELETE(m_Nutrition);
    InvalidateNutritionFile();
  }
  //-----------------------------------------------------------------------------
  bool SEConsumeNutrients::IsValid() const
  {
    return SEPatientAction::IsValid() && (HasNutrition() || HasNutritionFile());
  }
  //-----------------------------------------------------------------------------
  bool SEConsumeNutrients::IsActive() const
  {
    return IsValid();
  }
  //-----------------------------------------------------------------------------
  bool SEConsumeNutrients::Load(const CDM::ConsumeNutrientsData& in)
  {
    SEPatientAction::Load(in);
    if (in.Nutrition().present())
      GetNutrition().Load(in.Nutrition().get());
    else if (in.NutritionFile().present())
      SetNutritionFile(in.NutritionFile().get());
    return true;
  }
  //-----------------------------------------------------------------------------
  CDM::ConsumeNutrientsData* SEConsumeNutrients::Unload() const
  {
    CDM::ConsumeNutrientsData* data(new CDM::ConsumeNutrientsData());
    Unload(*data);
    return data;
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::Unload(CDM::ConsumeNutrientsData& data) const
  {
    SEPatientAction::Unload(data);
    if (HasNutrition())
      data.Nutrition(std::unique_ptr<CDM::NutritionData>(m_Nutrition->Unload()));
    if (HasNutritionFile())
      data.NutritionFile(m_NutritionFile);
  }
  //-----------------------------------------------------------------------------
  bool SEConsumeNutrients::HasNutrition() const
  {
    return m_Nutrition != nullptr;
  }
  //-----------------------------------------------------------------------------
  SENutrition& SEConsumeNutrients::GetNutrition()
  {
    m_NutritionFile = "";
    if (m_Nutrition == nullptr)
      m_Nutrition = new SENutrition(nullptr);
    return *m_Nutrition;
  }
  //-----------------------------------------------------------------------------
  const SENutrition* SEConsumeNutrients::GetNutrition() const
  {
    return m_Nutrition;
  }
  //-----------------------------------------------------------------------------
  std::string SEConsumeNutrients::GetNutritionFile() const
  {
    return m_NutritionFile;
  }
  //-----------------------------------------------------------------------------
  const char* SEConsumeNutrients::GetNutritionFile_cStr() const
  {
    return m_NutritionFile.c_str();
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::SetNutritionFile(const char* fileName)
  {
    SetNutritionFile(std::string(fileName));
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::SetNutritionFile(const std::string& fileName)
  {
    m_NutritionFile = fileName;
    if (m_Nutrition != nullptr)
      SAFE_DELETE(m_Nutrition);
  }
  //-----------------------------------------------------------------------------
  bool SEConsumeNutrients::HasNutritionFile() const
  {
    return m_NutritionFile.empty() ? false : true;
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::InvalidateNutritionFile()
  {
    m_NutritionFile = "";
  }
  //-----------------------------------------------------------------------------
  void SEConsumeNutrients::ToString(std::ostream& str) const
  {
    str << "Patient Action : Consume Nutrients";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    if (HasNutrition()) {
      str << "\n\tCharbohydrates: ";
      m_Nutrition->HasCarbohydrate() ? str << m_Nutrition->GetCarbohydrate() : str << "None";
      str << "\n\tFat: ";
      m_Nutrition->HasFat() ? str << m_Nutrition->GetFat() : str << "None";
      str << "\n\tProtein: ";
      m_Nutrition->HasProtein() ? str << m_Nutrition->GetProtein() : str << "None";
      str << "\n\tCalcium: ";
      m_Nutrition->HasCalcium() ? str << m_Nutrition->GetCalcium() : str << "None";
      str << "\n\tSodium: ";
      m_Nutrition->HasSodium() ? str << m_Nutrition->GetSodium() : str << "None";
      str << "\n\tWater: ";
      m_Nutrition->HasWater() ? str << m_Nutrition->GetWater() : str << "None";
    }
    if (HasNutritionFile())
      str << "\n\tNutrition File: ";
    str << m_NutritionFile;
    str << std::flush;
  }
  //-----------------------------------------------------------------------------
}