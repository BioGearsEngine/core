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

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {

SESepsis::SESepsis()
  : SEPatientAction()
{
  m_Compartment = ""; //User input, location of hemorrhage
  m_Severity = nullptr; //User input, initial rate of bleeding
}
//-------------------------------------------------------------------------------
SESepsis::~SESepsis()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SESepsis::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  SAFE_DELETE(m_Severity);
}
//-------------------------------------------------------------------------------
bool SESepsis::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment() && HasSeverity();
}
//-------------------------------------------------------------------------------
bool SESepsis::IsActive() const
{
  return IsValid() ? !(m_Severity->GetValue() < ZERO_APPROX) : false;
}
//-------------------------------------------------------------------------------
bool SESepsis::Load(const CDM::SepsisData& in)
{
  SEPatientAction::Load(in);
  m_Compartment = in.Compartment();
  GetSeverity().Load(in.Severity());
  return true;
}
//-------------------------------------------------------------------------------
CDM::SepsisData* SESepsis::Unload() const
{
  CDM::SepsisData* data(new CDM::SepsisData());
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SESepsis::Unload(CDM::SepsisData& data) const
{
  SEPatientAction::Unload(data);
  if (HasCompartment())
    data.Compartment(m_Compartment);
  if (m_Severity != nullptr)
    data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
}
//-------------------------------------------------------------------------------
const char* SESepsis::GetCompartment_cStr() const
{
  return m_Compartment.c_str();
}
//-------------------------------------------------------------------------------
std::string SESepsis::GetCompartment() const
{
  return m_Compartment;
}
//-------------------------------------------------------------------------------
bool SESepsis::HasCompartment() const
{
  return !m_Compartment.empty();
}
//-------------------------------------------------------------------------------
void SESepsis::InvalidateCompartment()
{
  m_Compartment = "";
}
//-------------------------------------------------------------------------------
void SESepsis::SetCompartment(const char* name)
{
  return SetCompartment(std::string{ name });
}
//-------------------------------------------------------------------------------
void SESepsis::SetCompartment(const std::string& name)
{
  m_Compartment = name;
}
//-------------------------------------------------------------------------------
bool SESepsis::HasSeverity() const
{
  return m_Severity == nullptr ? false : m_Severity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SESepsis::GetSeverity()
{
  if (m_Severity == nullptr)
    m_Severity = new SEScalar0To1();
  return *m_Severity;
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
}
