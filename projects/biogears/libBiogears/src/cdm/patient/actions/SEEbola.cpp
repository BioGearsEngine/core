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

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears
{
SEEbola::SEEbola()
    : SEPatientAction()
  {
    m_Severity = SEInfectionSeverity::Invalid;
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
    m_Severity = SEInfectionSeverity::Invalid;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::IsValid() const
  {
    return SEPatientAction::IsValid() && HasSeverity();
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::IsActive() const
  {
    return m_Severity == SEInfectionSeverity::Eliminated ? false : true;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::Load(const CDM::EbolaData& in, std::default_random_engine *rd)
  {
    io::PatientActions::UnMarshall(in, *this, rd);
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
    io::PatientActions::Marshall(*this, data);
  }
  //-------------------------------------------------------------------------------
  SEInfectionSeverity SEEbola::GetSeverity() const
  {
    return m_Severity;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::SetSeverity(SEInfectionSeverity t)
  {
    m_Severity = t;
  }
  //-------------------------------------------------------------------------------
  bool SEEbola::HasSeverity() const
  {
    return m_Severity == (SEInfectionSeverity::Invalid) ? false : true;
  }
  //-------------------------------------------------------------------------------
  void SEEbola::InvalidateSeverity()
  {
    m_Severity = SEInfectionSeverity::Invalid;
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
  equivilant &= m_Severity == rhs.m_Severity;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEEbola::operator!=(const SEEbola& rhs) const
{
  return !(*this == rhs);
}
}
