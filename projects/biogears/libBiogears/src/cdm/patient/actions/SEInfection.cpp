/**************************************************************************************Copyright 2015 Applied Research Associates, Inc.Licensed under the Apache License, Version 2.0(the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/patient/actions/SEInfection.h>

#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears
{
  SEInfection::SEInfection()
    : SEPatientAction()
  {
    m_Location = ""; //User input, location of infection
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
    m_MinimumInhibitoryConcentration = nullptr;
  }
  //-------------------------------------------------------------------------------
  SEInfection::~SEInfection()
  {
    Clear();
  }
  //-------------------------------------------------------------------------------
  void SEInfection::Clear()
  {
    SEPatientAction::Clear();
    m_Location = "";
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
    SAFE_DELETE(m_MinimumInhibitoryConcentration);
  }
  //-------------------------------------------------------------------------------
  bool SEInfection::IsValid() const
  {
    return SEPatientAction::IsValid() && HasLocation() && HasSeverity() && HasMinimumInhibitoryConcentration();
  }
  //-------------------------------------------------------------------------------
  bool SEInfection::IsActive() const
  {
    return m_Severity == CDM::enumInfectionSeverity::Eliminated ? false : true;
  }
  //-------------------------------------------------------------------------------
  bool SEInfection::Load(const CDM::InfectionData& in)
  {
    SEPatientAction::Load(in);
    m_Location = in.Location();
    m_Severity = in.Severity();
    GetMinimumInhibitoryConcentration().Load(in.MinimumInhibitoryConcentration());
    return true;
  }
  //-------------------------------------------------------------------------------
  CDM::InfectionData* SEInfection::Unload() const
  {
    CDM::InfectionData* data(new CDM::InfectionData());
    Unload(*data);
    return data;
  }
  //-------------------------------------------------------------------------------
  void SEInfection::Unload(CDM::InfectionData& data) const
  {
    SEPatientAction::Unload(data);
    if (HasLocation())
      data.Location(m_Location);
    if (HasSeverity())
      data.Severity(m_Severity);
    if (m_MinimumInhibitoryConcentration != nullptr)
      data.MinimumInhibitoryConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_MinimumInhibitoryConcentration->Unload()));
  }
  //-------------------------------------------------------------------------------
  const char* SEInfection::GetLocation_cStr() const
  {
    return m_Location.c_str();
  }
  //-------------------------------------------------------------------------------
  std::string SEInfection::GetLocation() const
  {
    return m_Location;
  }
  //-------------------------------------------------------------------------------
  bool SEInfection::HasLocation() const
  {
    return !m_Location.empty();
  }
  //-------------------------------------------------------------------------------
  void SEInfection::InvalidateLocation()
  {
    m_Location = "";
  }
  //-------------------------------------------------------------------------------
  void SEInfection::SetLocation(const char* name)
  {
    return SetLocation(std::string{ name });
  }
  //-------------------------------------------------------------------------------
  void SEInfection::SetLocation(const std::string& name)
  {
    m_Location = name;
  }
  //-------------------------------------------------------------------------------
  CDM::enumInfectionSeverity::value SEInfection::GetSeverity() const
  {
    return m_Severity;
  }
  //-------------------------------------------------------------------------------
  void SEInfection::SetSeverity(CDM::enumInfectionSeverity::value t)
  {
    m_Severity = t;
  }
  //-------------------------------------------------------------------------------
  bool SEInfection::HasSeverity() const
  {
    return m_Severity == ((CDM::enumInfectionSeverity::value)-1) ? false : true;
  }
  //-------------------------------------------------------------------------------
  void SEInfection::InvalidateSeverity()
  {
    m_Severity = (CDM::enumInfectionSeverity::value)-1;
  }
  //-----------------------------------------------------------------------------
  bool SEInfection::HasMinimumInhibitoryConcentration() const
  {
    return m_MinimumInhibitoryConcentration == nullptr ? false : true;
  }
  //-----------------------------------------------------------------------------
  SEScalarMassPerVolume& SEInfection::GetMinimumInhibitoryConcentration()
  {
    if (m_MinimumInhibitoryConcentration == nullptr)
      m_MinimumInhibitoryConcentration = new SEScalarMassPerVolume();
    return *m_MinimumInhibitoryConcentration;
  }
  //-------------------------------------------------------------------------------
  void SEInfection::ToString(std::ostream & str) const
  {
    str << "Patient Action : Infection";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tSeverity:  ";
    str << m_Severity;
    str << "\n\tMinimum Inhibitory Concentration: ";
    str << *m_MinimumInhibitoryConcentration;
    str << "\n\tLocation: ";
    HasLocation() ? str << GetLocation() : str << "No Location Set";
    str << std::flush;
  }
  //-------------------------------------------------------------------------------
}
