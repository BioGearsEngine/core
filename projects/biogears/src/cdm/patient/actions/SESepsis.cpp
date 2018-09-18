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
#include <biogears/cdm/patient/actions/SESepsis.h>

#include <biogears/schema/cdm/PatientActions.hxx>
#include <biogears/cdm/properties/SEScalarTypes.h>

SESepsis::SESepsis()
  : SEPatientAction()
{
  m_Compartment = ""; //User input, location of hemorrhage
  m_Severity = nullptr; //User input, initial rate of bleeding
  m_Pathogen = nullptr; //Pathogen level state variable in Kumar Model
  m_WhiteBloodCell = nullptr; //White blood cell ("early mediator") level state variable in Kumar Model, as fraction of max value = 1
  m_LateMediator = nullptr; //Late immune response level state variable in Kumar Model
}

SESepsis::~SESepsis()
{
  Clear();
}

void SESepsis::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  m_TissueResistorMap.clear();
  SAFE_DELETE(m_Severity);
  SAFE_DELETE(m_Pathogen);
  SAFE_DELETE(m_WhiteBloodCell);
  SAFE_DELETE(m_LateMediator);
}

bool SESepsis::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment() && HasSeverity();
}

bool SESepsis::IsActive() const
{
  return IsValid() ? !(m_Severity->GetValue() < ZERO_APPROX) : false;
}

bool SESepsis::Load(const CDM::SepsisData& in)
{
  SEPatientAction::Load(in);
  m_Compartment = in.Compartment();
  GetSeverity().Load(in.Severity());
  BuildTissueResistorMap();

  return true;
}

CDM::SepsisData* SESepsis::Unload() const
{
  CDM::SepsisData* data(new CDM::SepsisData());
  Unload(*data);
  return data;
}

void SESepsis::Unload(CDM::SepsisData& data) const
{
  SEPatientAction::Unload(data);
  if (HasCompartment())
    data.Compartment(m_Compartment);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}

std::string SESepsis::GetCompartment() const
{
  return m_Compartment;
}
bool SESepsis::HasCompartment() const
{
  return !m_Compartment.empty();
}
void SESepsis::InvalidateCompartment()
{
  m_Compartment = "";
}
void SESepsis::SetCompartment(const std::string& name)
{
  m_Compartment = name;
}

bool SESepsis::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
SEScalar0To1& SESepsis::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}

SEScalar& SESepsis::GetPathogen()
{
  if (m_Pathogen == nullptr) {
    m_Pathogen = new SEScalar();
    m_Pathogen->SetValue(0.05); //Initial value assigned to Pathogen in Kumar Model
  }
  return *m_Pathogen;
}

SEScalar& SESepsis::GetEarlyMediator()
{ //Calling this GetEarlyMediator to not confuse with GetWhiteBloodCellCount in BloodChemistry system
  if (m_WhiteBloodCell == nullptr) {
    m_WhiteBloodCell = new SEScalar();
    m_WhiteBloodCell->SetValue(0.05); //Initial value assigned to White Blood Cell level in Kumar Model
  }
  return *m_WhiteBloodCell;
}

SEScalar& SESepsis::GetLateMediator()
{
  if (m_LateMediator == nullptr) {
    m_LateMediator = new SEScalar();
    m_LateMediator->SetValue(1.0); //Initial value assigned to Late Mediator in Kumar Model
  }
  return *m_LateMediator;
}

const std::map<std::string, std::string>& SESepsis::GetTissueResistorMap() const
{
  return m_TissueResistorMap;
}

void SESepsis::BuildTissueResistorMap()
{
  m_TissueResistorMap["BoneTissue"] = "BoneE1ToBoneE2";
  m_TissueResistorMap["FatTissue"] = "FatE1ToFatE2";
  m_TissueResistorMap["GutTissue"] = "GutE1ToGutE2";
  m_TissueResistorMap["LeftKidneyTissue"] = "LeftKidneyE1ToLeftKidneyE2";
  m_TissueResistorMap["LeftLungTissue"] = "LeftLungE1ToLeftLungE2";
  m_TissueResistorMap["LiverTissue"] = "LiverE1ToLiverE2";
  m_TissueResistorMap["MuscleTissue"] = "MuscleE1ToMuscleE2";
  m_TissueResistorMap["MyocardiumTissue"] = "MyocardiumE1ToMyocardiumE2";
  m_TissueResistorMap["RightKidneyTissue"] = "RightKidneyE1ToRightKidneyE2";
  m_TissueResistorMap["RightLungTissue"] = "RightLungE1ToRightLungE2";
  m_TissueResistorMap["SkinTissue"] = "SkinE1ToSkinE2";
  m_TissueResistorMap["SpleenTissue"] = "SpleenE1ToSpleenE2";
}

void SESepsis::ToString(std::ostream& str) const
{
  str << "Patient Action : Sepsis";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tSeverity:  ";
  str << *m_Severity;
  str << "\n\tCompartment: ";
  HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
  str << std::flush;
}
