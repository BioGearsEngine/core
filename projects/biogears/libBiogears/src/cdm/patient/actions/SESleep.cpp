/**************************************************************************************Copyright 2015 Applied Research Associates, Inc.Licensed under the Apache License, Version 2.0(the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include "biogears/cdm/patient/actions/SESleep.h"

#include "io/cdm/PatientActions.h"
namespace biogears
{
  SESleep::SESleep()
    : SEPatientAction()
  {
     m_SleepState = SEOnOff::Off;
  }
  //-------------------------------------------------------------------------------
  SESleep::~SESleep()
  {
    Clear();
  }
  //-------------------------------------------------------------------------------
  void SESleep::Clear()
  {
    SEPatientAction::Clear();
    m_SleepState = SEOnOff::Off;;
  }
  //-------------------------------------------------------------------------------
  bool SESleep::IsValid() const
  {
    return SEPatientAction::IsValid() && HasSleepState();
  }
  //-------------------------------------------------------------------------------
  bool SESleep::IsActive() const
  {
    return m_SleepState == SEOnOff::Off ? false : true;
  }
  //-------------------------------------------------------------------------------
  bool SESleep::Load(const CDM::SleepData& in, std::default_random_engine *rd)
  {
    io::PatientActions::UnMarshall(in, *this, rd);
    return true;
  }
  //-------------------------------------------------------------------------------
  CDM::SleepData* SESleep::Unload() const
  {
    CDM::SleepData* data(new CDM::SleepData());
    Unload(*data);
    return data;
  }
  //-------------------------------------------------------------------------------
  void SESleep::Unload(CDM::SleepData& data) const
  {
    io::PatientActions::Marshall(*this, data);
  }
  //-------------------------------------------------------------------------------
  SEOnOff SESleep::GetSleepState() const
  {
        return m_SleepState;
  }
  //-------------------------------------------------------------------------------
  bool SESleep::HasSleepState() const
  {
      return m_SleepState == ((SEOnOff) - 1) ? false : true;
  }
  //-------------------------------------------------------------------------------
  void SESleep::SetSleepState(SEOnOff t) 
  {
      m_SleepState = t;
  }
  //-------------------------------------------------------------------------------
  void SESleep::ToString(std::ostream & str) const
  {
    str << "Patient Action : Sleep";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tSleepState:  ";
    str << m_SleepState;
    str << std::flush;
  }
  //-------------------------------------------------------------------------------
  bool SESleep::operator==(const SESleep& rhs) const
  {
    bool equivilant = m_Comment == rhs.m_Comment;
    equivilant &= m_SleepState == rhs.m_SleepState;
    return equivilant;
  }
  //-------------------------------------------------------------------------------
  bool SESleep::operator!=(const SESleep& rhs) const
  {
    return !(*this == rhs);
  }
  //-------------------------------------------------------------------------------
  }
