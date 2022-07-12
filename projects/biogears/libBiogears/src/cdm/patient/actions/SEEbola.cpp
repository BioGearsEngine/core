/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0(the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:       http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/patient/actions/SEEbola.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears
{
SEEbola::SEEbola()
    : SEPatientAction()
  {
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
  }
  //-------------------------------------------------------------------------------
  SEEbola::~SEEbola()
  {
    Clear();
  }
  //-------------------------------------------------------------------------------
  void SEEbola::Clear()
  {
    SEPatientAction::Clear();
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::IsValid() const
  {
    return SEPatientAction::IsValid() && HasSeverity();
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::IsActive() const
  {
    return m_Severity == CDM::enumInfectionSeverity::Eliminated ? false : true;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::Load(const CDM::EbolaData& in)
  {
    SEPatientAction::Load(in);
    m_Severity = in.Severity();
    return true;
  }
  //-------------------------------------------------------------------------------
  CDM::EbolaData* SEEbola::Unload() const
  {
    CDM::EbolaData* data(new CDM::EbolaData());
    Unload(*data);
    return data;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::Unload(CDM::EbolaData& data) const
  {
    SEPatientAction::Unload(data);
    if (HasSeverity())
      data.Severity(m_Severity);
  }
  //-------------------------------------------------------------------------------
  CDM::enumInfectionSeverity::value SEEbola::GetSeverity() const
  {
    return m_Severity;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::SetSeverity(CDM::enumInfectionSeverity::value t)
  {
    m_Severity = t;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::HasSeverity() const
  {
    return m_Severity == ((CDM::enumInfectionSeverity::value)-1) ? false : true;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::InvalidateSeverity()
  {
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::ToString(std::ostream& str) const
  {
    str << "Patient Action : Ebola";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tSeverity:  ";
    str << m_Severity;
    str << std::flush;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::operator==(const SEEbola& rhs) const
  {
bool equivilant = m_Comment == rhs.m_Comment;
  equivilant = m_Severity == rhs.m_Severity;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEEbola::operator!=(const SEEbola& rhs) const
{
  return !(*this == rhs);
}
}
