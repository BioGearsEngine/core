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
#include <biogears/cdm/patient/actions/SEAmputation.h>

#include "io/cdm/PatientActions.h"
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEAmputation::SEAmputation()
  : SEPatientAction()
  , m_BleedResistance(nullptr)
{
  m_Compartment = ""; // User input, location of Amputation
  m_WeightPercent = "WeightNotLost";
}
//-----------------------------------------------------------------------------
SEAmputation::~SEAmputation()
{
  Invalidate();
}
//-----------------------------------------------------------------------------
void SEAmputation::Invalidate()
{
  SEPatientAction::Invalidate();
  m_Compartment = "";
  SAFE_DELETE(m_BleedResistance);
}
//-----------------------------------------------------------------------------
bool SEAmputation::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment();
}
//-----------------------------------------------------------------------------
const char* SEAmputation::GetCompartment_cStr() const
{
  return m_Compartment.c_str();
}
//-----------------------------------------------------------------------------
//!
//!  \brief String representing the location of the event.
//!  \return std::string
//!
//!  Only a single Amputation can exist at the exsact same compartment string
//!  Any string value can be used. Submitting the same value overwrites the previous event.
std::string SEAmputation::GetCompartment() const
{
  return m_Compartment;
}
//-----------------------------------------------------------------------------
bool SEAmputation::HasCompartment() const
{
  return !m_Compartment.empty();
}
//-----------------------------------------------------------------------------
void SEAmputation::SetCompartment(const char* name)
{
  return SetCompartment(std::string { name });
}
//-----------------------------------------------------------------------------
void SEAmputation::SetCompartment(const std::string& name)
{
  m_Compartment = name;
}
//-----------------------------------------------------------------------------
void SEAmputation::InvalidateCompartment()
{
  m_Compartment = "";
}
//-----------------------------------------------------------------------------
bool SEAmputation::HasBleedResistance() const
{
  return m_BleedResistance == nullptr ? false : m_BleedResistance->IsValid();
}
//-----------------------------------------------------------------------------
SEScalarFlowResistance& SEAmputation::GetBleedResistance()
{
  if (m_BleedResistance == nullptr) {
    m_BleedResistance = new SEScalarFlowResistance();
  }
  return *m_BleedResistance;
}
SEScalarFlowResistance const& SEAmputation::GetBleedResistance() const
{
  if (m_BleedResistance == nullptr) {
    const_cast<SEAmputation*>(this)->m_BleedResistance = new SEScalarFlowResistance();
  }
  return *m_BleedResistance;
}
//-----------------------------------------------------------------------------
const char* SEAmputation::GetWeightPercent_cStr() const
{
  return m_WeightPercent.c_str();
}
std::string SEAmputation::GetWeightPercent() const
{
  return m_WeightPercent;
}
//-----------------------------------------------------------------------------
void SEAmputation::SetWeightPercent(const char* name)
{
  return SetWeightPercent(std::string { name });
}
//-----------------------------------------------------------------------------
void SEAmputation::SetWeightPercent(const std::string& name)
{
  m_WeightPercent = name;
}
//-----------------------------------------------------------------------------
void SEAmputation::ToString(std::ostream& str) const
{
    str << "Patient Action : Amputation";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tCompartment: ";
    HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
    str << std::flush;

  
}
//-------------------------------------------------------------------------------
bool SEAmputation::operator==(const SEAmputation& rhs) const
{
  return m_Comment == rhs.m_Comment
    && m_Compartment == rhs.m_Compartment;
}
//-------------------------------------------------------------------------------
bool SEAmputation::operator!=(const SEAmputation& rhs) const
{
  return !(*this == rhs);
}
}