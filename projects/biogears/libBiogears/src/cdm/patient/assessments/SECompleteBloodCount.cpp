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
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>

#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include "io/cdm/PatientAssessments.h"

namespace biogears {
SECompleteBloodCount::SECompleteBloodCount()
{
  m_Hematocrit = nullptr;
  m_Hemoglobin = nullptr;
  m_NeutrophilCellCount = nullptr;
  m_LymphocyteCellCount = nullptr;
  m_PlateletCount = nullptr;
  m_MeanCorpuscularHemoglobin = nullptr;
  m_MeanCorpuscularHemoglobinConcentration = nullptr;
  m_MeanCorpuscularVolume = nullptr;
  m_NeutrophilCellCount = nullptr;
  m_RedBloodCellCount = nullptr;
  m_WhiteBloodCellCount = nullptr;
}
//-------------------------------------------------------------------------------
SECompleteBloodCount::~SECompleteBloodCount()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SECompleteBloodCount::Clear()
{
  SEPatientAssessment::Clear();
  SAFE_DELETE(m_Hematocrit);
  SAFE_DELETE(m_Hemoglobin);
  SAFE_DELETE(m_LymphocyteCellCount);
  SAFE_DELETE(m_PlateletCount);
  SAFE_DELETE(m_MeanCorpuscularHemoglobin);
  SAFE_DELETE(m_MeanCorpuscularHemoglobinConcentration);
  SAFE_DELETE(m_MeanCorpuscularVolume);
  SAFE_DELETE(m_NeutrophilCellCount);
  SAFE_DELETE(m_RedBloodCellCount);
  SAFE_DELETE(m_WhiteBloodCellCount);
}
//-------------------------------------------------------------------------------
void SECompleteBloodCount::Reset()
{
  SEPatientAssessment::Reset();
  INVALIDATE_PROPERTY(m_Hematocrit);
  INVALIDATE_PROPERTY(m_Hemoglobin);
  INVALIDATE_PROPERTY(m_LymphocyteCellCount);
  INVALIDATE_PROPERTY(m_PlateletCount);
  INVALIDATE_PROPERTY(m_MeanCorpuscularHemoglobin);
  INVALIDATE_PROPERTY(m_MeanCorpuscularHemoglobinConcentration);
  INVALIDATE_PROPERTY(m_MeanCorpuscularVolume);
  INVALIDATE_PROPERTY(m_NeutrophilCellCount);
  INVALIDATE_PROPERTY(m_LymphocyteCellCount);
  INVALIDATE_PROPERTY(m_RedBloodCellCount);
  INVALIDATE_PROPERTY(m_WhiteBloodCellCount);
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasHematocrit() const
{
  return m_Hematocrit == nullptr ? false : m_Hematocrit->IsValid();
}
SEScalarFraction& SECompleteBloodCount::GetHematocrit()
{
  if (m_Hematocrit == nullptr)
    m_Hematocrit = new SEScalarFraction();
  return *m_Hematocrit;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasHemoglobin() const
{
  return m_Hemoglobin == nullptr ? false : m_Hemoglobin->IsValid();
}
SEScalarMassPerVolume& SECompleteBloodCount::GetHemoglobin()
{
  if (m_Hemoglobin == nullptr)
    m_Hemoglobin = new SEScalarMassPerVolume();
  return *m_Hemoglobin;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasLymphocyteCellCount() const
{
  return m_LymphocyteCellCount == nullptr ? false : m_LymphocyteCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetLymphocyteCellCount()
{
  if (m_LymphocyteCellCount == nullptr)
    m_LymphocyteCellCount = new SEScalarAmountPerVolume();
  return *m_LymphocyteCellCount;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasPlateletCount() const
{
  return m_PlateletCount == nullptr ? false : m_PlateletCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetPlateletCount()
{
  if (m_PlateletCount == nullptr)
    m_PlateletCount = new SEScalarAmountPerVolume();
  return *m_PlateletCount;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasMeanCorpuscularHemoglobin() const
{
  return m_MeanCorpuscularHemoglobin == nullptr ? false : m_MeanCorpuscularHemoglobin->IsValid();
}
SEScalarMassPerAmount& SECompleteBloodCount::GetMeanCorpuscularHemoglobin()
{
  if (m_MeanCorpuscularHemoglobin == nullptr)
    m_MeanCorpuscularHemoglobin = new SEScalarMassPerAmount();
  return *m_MeanCorpuscularHemoglobin;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasMeanCorpuscularHemoglobinConcentration() const
{
  return m_MeanCorpuscularHemoglobinConcentration == nullptr ? false : m_MeanCorpuscularHemoglobinConcentration->IsValid();
}
SEScalarMassPerVolume& SECompleteBloodCount::GetMeanCorpuscularHemoglobinConcentration()
{
  if (m_MeanCorpuscularHemoglobinConcentration == nullptr)
    m_MeanCorpuscularHemoglobinConcentration = new SEScalarMassPerVolume();
  return *m_MeanCorpuscularHemoglobinConcentration;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasMeanCorpuscularVolume() const
{
  return m_MeanCorpuscularVolume == nullptr ? false : m_MeanCorpuscularVolume->IsValid();
}
SEScalarVolume& SECompleteBloodCount::GetMeanCorpuscularVolume()
{
  if (m_MeanCorpuscularVolume == nullptr)
    m_MeanCorpuscularVolume = new SEScalarVolume();
  return *m_MeanCorpuscularVolume;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasNeutrophilCount() const
{
  return m_NeutrophilCellCount == nullptr ? false : m_NeutrophilCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetNeutrophilCount()
{
  if (m_NeutrophilCellCount == nullptr)
    m_NeutrophilCellCount = new SEScalarAmountPerVolume();
  return *m_NeutrophilCellCount;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasRedBloodCellCount() const
{
  return m_RedBloodCellCount == nullptr ? false : m_RedBloodCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetRedBloodCellCount()
{
  if (m_RedBloodCellCount == nullptr)
    m_RedBloodCellCount = new SEScalarAmountPerVolume();
  return *m_RedBloodCellCount;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::HasWhiteBloodCellCount() const
{
  return m_WhiteBloodCellCount == nullptr ? false : m_WhiteBloodCellCount->IsValid();
}
SEScalarAmountPerVolume& SECompleteBloodCount::GetWhiteBloodCellCount()
{
  if (m_WhiteBloodCellCount == nullptr)
    m_WhiteBloodCellCount = new SEScalarAmountPerVolume();
  return *m_WhiteBloodCellCount;
}
//-------------------------------------------------------------------------------
bool SECompleteBloodCount::operator==(SECompleteBloodCount const& rhs) const
{
  if (this == &rhs)
    return true;

  return ((m_Hematocrit && rhs.m_Hematocrit) ? m_Hematocrit->operator==(*rhs.m_Hematocrit) : m_Hematocrit == rhs.m_Hematocrit)
    && ((m_Hemoglobin && rhs.m_Hemoglobin) ? m_Hemoglobin->operator==(*rhs.m_Hemoglobin) : m_Hemoglobin == rhs.m_Hemoglobin)
    && ((m_LymphocyteCellCount && rhs.m_LymphocyteCellCount) ? m_LymphocyteCellCount->operator==(*rhs.m_LymphocyteCellCount) : m_LymphocyteCellCount == rhs.m_LymphocyteCellCount)
    && ((m_PlateletCount && rhs.m_PlateletCount) ? m_PlateletCount->operator==(*rhs.m_PlateletCount) : m_PlateletCount == rhs.m_PlateletCount)
    && ((m_MeanCorpuscularHemoglobin && rhs.m_MeanCorpuscularHemoglobin) ? m_MeanCorpuscularHemoglobin->operator==(*rhs.m_MeanCorpuscularHemoglobin) : m_MeanCorpuscularHemoglobin == rhs.m_MeanCorpuscularHemoglobin)
    && ((m_MeanCorpuscularHemoglobinConcentration && rhs.m_MeanCorpuscularHemoglobinConcentration) ? m_MeanCorpuscularHemoglobinConcentration->operator==(*rhs.m_MeanCorpuscularHemoglobinConcentration) : m_MeanCorpuscularHemoglobinConcentration == rhs.m_MeanCorpuscularHemoglobinConcentration)
    && ((m_NeutrophilCellCount && rhs.m_NeutrophilCellCount) ? m_NeutrophilCellCount->operator==(*rhs.m_NeutrophilCellCount) : m_NeutrophilCellCount == rhs.m_NeutrophilCellCount)
    && ((m_RedBloodCellCount && rhs.m_RedBloodCellCount) ? m_RedBloodCellCount->operator==(*rhs.m_RedBloodCellCount) : m_RedBloodCellCount == rhs.m_RedBloodCellCount)
    && ((m_WhiteBloodCellCount && rhs.m_WhiteBloodCellCount) ? m_WhiteBloodCellCount->operator==(*rhs.m_WhiteBloodCellCount) : m_WhiteBloodCellCount == rhs.m_WhiteBloodCellCount)
  ;
}
bool SECompleteBloodCount::operator!=(SECompleteBloodCount const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
}