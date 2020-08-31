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
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>

#include <biogears/cdm/properties/SEScalarAmount.h>

namespace biogears {
SEUrinalysisMicroscopic::SEUrinalysisMicroscopic()

{
  m_ObservationType = CDM::enumMicroscopicObservationType::value(-1);
  m_RedBloodCells = nullptr;
  m_WhiteBloodCells = nullptr;
  m_EpithelialCells = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Casts = nullptr;
  m_Crystals = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Bacteria = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Trichomonads = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Yeast = CDM::enumMicroscopicObservationAmount::value(-1);
}

SEUrinalysisMicroscopic::~SEUrinalysisMicroscopic()
{
  Clear();
}

void SEUrinalysisMicroscopic::Clear()
{
  SEPatientAssessment::Clear();
  m_ObservationType = CDM::enumMicroscopicObservationType::value(-1);
  SAFE_DELETE(m_RedBloodCells);
  SAFE_DELETE(m_WhiteBloodCells);
  m_EpithelialCells = CDM::enumMicroscopicObservationAmount::value(-1);
  SAFE_DELETE(m_Casts);
  m_Crystals = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Bacteria = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Trichomonads = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Yeast = CDM::enumMicroscopicObservationAmount::value(-1);
}

void SEUrinalysisMicroscopic::Reset()
{
  SEPatientAssessment::Reset();
  m_ObservationType = CDM::enumMicroscopicObservationType::value(-1);
  INVALIDATE_PROPERTY(m_RedBloodCells);
  INVALIDATE_PROPERTY(m_WhiteBloodCells);
  m_EpithelialCells = CDM::enumMicroscopicObservationAmount::value(-1);
  INVALIDATE_PROPERTY(m_Casts);
  m_Crystals = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Bacteria = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Trichomonads = CDM::enumMicroscopicObservationAmount::value(-1);
  m_Yeast = CDM::enumMicroscopicObservationAmount::value(-1);
}

bool SEUrinalysisMicroscopic::Load(const CDM::UrinalysisMicroscopicData& in)
{
  SEPatientAssessment::Load(in);
  return true;
}

CDM::UrinalysisMicroscopicData* SEUrinalysisMicroscopic::Unload()
{
  CDM::UrinalysisMicroscopicData* data = new CDM::UrinalysisMicroscopicData();
  Unload(*data);
  return data;
}

void SEUrinalysisMicroscopic::Unload(CDM::UrinalysisMicroscopicData& data)
{
  if (HasObservationType())
    data.ObservationType(m_ObservationType);
  if (HasRedBloodCellsResult())
    data.RedBloodCells(std::unique_ptr<CDM::ScalarAmountData>(m_RedBloodCells->Unload()));
  if (HasWhiteBloodCellsResult())
    data.WhiteBloodCells(std::unique_ptr<CDM::ScalarAmountData>(m_WhiteBloodCells->Unload()));
  if (HasEpithelialCellsResult())
    data.EpithelialCells(m_EpithelialCells);
  if (HasCastsResult())
    data.Casts(std::unique_ptr<CDM::ScalarAmountData>(m_Casts->Unload()));
  if (HasCrystalsResult())
    data.Crystals(m_Crystals);
  if (HasBacteriaResult())
    data.Bacteria(m_Bacteria);
  if (HasTrichomonadsResult())
    data.Trichomonads(m_Trichomonads);
  if (HasYeastResult())
    data.Yeast(m_Yeast);
}

bool SEUrinalysisMicroscopic::HasRedBloodCellsResult() const
{
  return m_RedBloodCells == nullptr ? false : m_RedBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetRedBloodCellsResult()
{
  if (m_RedBloodCells == nullptr)
    m_RedBloodCells = new SEScalarAmount();
  return *m_RedBloodCells;
}

bool SEUrinalysisMicroscopic::HasObservationType() const
{
  return m_ObservationType != CDM::enumMicroscopicObservationType::value(-1);
}
CDM::enumMicroscopicObservationType::value SEUrinalysisMicroscopic::GetObservationType() const
{
  return m_ObservationType;
}
void SEUrinalysisMicroscopic::SetObservationType(CDM::enumMicroscopicObservationType::value c)
{
  m_ObservationType = c;
}
void SEUrinalysisMicroscopic::InvalidateObservationType()
{
  m_ObservationType = CDM::enumMicroscopicObservationType::value(-1);
}

bool SEUrinalysisMicroscopic::HasWhiteBloodCellsResult() const
{
  return m_WhiteBloodCells == nullptr ? false : m_WhiteBloodCells->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetWhiteBloodCellsResult()
{
  if (m_WhiteBloodCells == nullptr)
    m_WhiteBloodCells = new SEScalarAmount();
  return *m_WhiteBloodCells;
}

bool SEUrinalysisMicroscopic::HasEpithelialCellsResult() const
{
  return m_EpithelialCells != CDM::enumMicroscopicObservationAmount::value(-1);
}
CDM::enumMicroscopicObservationAmount::value SEUrinalysisMicroscopic::GetEpithelialCellsResult() const
{
  return m_EpithelialCells;
}
void SEUrinalysisMicroscopic::SetEpithelialCellsResult(CDM::enumMicroscopicObservationAmount::value c)
{
  m_EpithelialCells = c;
}
void SEUrinalysisMicroscopic::InvalidateEpithelialCellsResult()
{
  m_EpithelialCells = CDM::enumMicroscopicObservationAmount::value(-1);
}

bool SEUrinalysisMicroscopic::HasCastsResult() const
{
  return m_Casts == nullptr ? false : m_Casts->IsValid();
}
SEScalarAmount& SEUrinalysisMicroscopic::GetCastsResult()
{
  if (m_Casts == nullptr)
    m_Casts = new SEScalarAmount();
  return *m_Casts;
}

bool SEUrinalysisMicroscopic::HasCrystalsResult() const
{
  return m_Crystals != CDM::enumMicroscopicObservationAmount::value(-1);
}
CDM::enumMicroscopicObservationAmount::value SEUrinalysisMicroscopic::GetCrystalsResult() const
{
  return m_Crystals;
}
void SEUrinalysisMicroscopic::SetCrystalsResult(CDM::enumMicroscopicObservationAmount::value c)
{
  m_Crystals = c;
}
void SEUrinalysisMicroscopic::InvalidateCrystalsResult()
{
  m_Crystals = CDM::enumMicroscopicObservationAmount::value(-1);
}

bool SEUrinalysisMicroscopic::HasBacteriaResult() const
{
  return m_Bacteria != CDM::enumMicroscopicObservationAmount::value(-1);
}
CDM::enumMicroscopicObservationAmount::value SEUrinalysisMicroscopic::GetBacteriaResult() const
{
  return m_Bacteria;
}
void SEUrinalysisMicroscopic::SetBacteriaResult(CDM::enumMicroscopicObservationAmount::value c)
{
  m_Bacteria = c;
}
void SEUrinalysisMicroscopic::InvalidateBacteriaResult()
{
  m_Bacteria = CDM::enumMicroscopicObservationAmount::value(-1);
}

bool SEUrinalysisMicroscopic::HasTrichomonadsResult() const
{
  return m_Trichomonads != CDM::enumMicroscopicObservationAmount::value(-1);
}
CDM::enumMicroscopicObservationAmount::value SEUrinalysisMicroscopic::GetTrichomonadsResult() const
{
  return m_Trichomonads;
}
void SEUrinalysisMicroscopic::SetTrichomonadsResult(CDM::enumMicroscopicObservationAmount::value c)
{
  m_Trichomonads = c;
}
void SEUrinalysisMicroscopic::InvalidateTrichomonadsResult()
{
  m_Trichomonads = CDM::enumMicroscopicObservationAmount::value(-1);
}

bool SEUrinalysisMicroscopic::HasYeastResult() const
{
  return m_Yeast != CDM::enumMicroscopicObservationAmount::value(-1);
}
CDM::enumMicroscopicObservationAmount::value SEUrinalysisMicroscopic::GetYeastResult() const
{
  return m_Yeast;
}
void SEUrinalysisMicroscopic::SetYeastResult(CDM::enumMicroscopicObservationAmount::value c)
{
  m_Yeast = c;
}
void SEUrinalysisMicroscopic::InvalidateYeastResult()
{
  m_Yeast = CDM::enumMicroscopicObservationAmount::value(-1);
}
}