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
#include <biogears/cdm/patient/actions/SEHemorrhage.h>

#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {
SEHemorrhage::SEHemorrhage()
  : SEPatientAction()
  , m_InitialRate(new SEScalarVolumePerTime())
  , m_BleedResistance(nullptr)

{
  m_Compartment = ""; //User input, location of hemorrhage
  //m_MCIS;
  m_InitialRate->SetValue(0.0, biogears::VolumePerTimeUnit::mL_Per_min); //User input, initial rate of bleeding

  //Place compartments in torso in a map so that we don't get too messy with nested conditionals.  Each vector is digits 2-4 of the MCIS code
  m_OrganMap["VenaCava"] = std::vector<unsigned int> { 6, 6, 0 };
  m_OrganMap["LeftLung"] = std::vector<unsigned int> { 7, 1, 0 };
  m_OrganMap["RightLung"] = std::vector<unsigned int> { 7, 1, 0 };
  m_OrganMap["Myocardium"] = std::vector<unsigned int> { 7, 2, 0 };
  m_OrganMap["Liver"] = std::vector<unsigned int> { 8, 1, 0 };
  m_OrganMap["Spleen"] = std::vector<unsigned int> { 8, 2, 0 };
  m_OrganMap["Splanchnic"] = std::vector<unsigned int> { 8, 3, 0 };
  m_OrganMap["LeftKidney"] = std::vector<unsigned int> { 8, 4, 0 };
  m_OrganMap["RightKidney"] = std::vector<unsigned int> { 8, 4, 0 };
  m_OrganMap["SmallIntestine"] = std::vector<unsigned int> { 8, 5, 0 };
  m_OrganMap["LargeIntestine"] = std::vector<unsigned int> { 8, 6, 0 };
}
//-----------------------------------------------------------------------------
SEHemorrhage::~SEHemorrhage()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEHemorrhage::Clear()
{
  SEPatientAction::Clear();
  m_Compartment = "";
  m_MCIS.clear();
  SAFE_DELETE(m_InitialRate);
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::IsValid() const
{
  return SEPatientAction::IsValid() && HasCompartment() && HasInitialRate();
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::IsActive() const
{
  return IsValid() ? !(m_InitialRate->GetValue(VolumePerTimeUnit::mL_Per_min) <= ZERO_APPROX) : false;
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::Load(const CDM::HemorrhageData& in)
{
  SEPatientAction::Load(in);
  m_Compartment = in.Compartment();
  GetInitialRate().Load(in.InitialRate());
  if ( in.BleedResistance().present() ){
    GetBleedResistance().Load(in.BleedResistance().get());  
  }
  SetMCIS();

  return true;
}
//-----------------------------------------------------------------------------
CDM::HemorrhageData* SEHemorrhage::Unload() const
{
  CDM::HemorrhageData* data(new CDM::HemorrhageData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEHemorrhage::Unload(CDM::HemorrhageData& data) const
{
  SEPatientAction::Unload(data);
  if (HasCompartment())
    data.Compartment(m_Compartment);
  if (HasInitialRate())
    data.InitialRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_InitialRate->Unload()));
  if (HasBleedResistance())
    data.BleedResistance(std::unique_ptr<CDM::ScalarFlowResistanceData>(m_BleedResistance->Unload()));
}
//-----------------------------------------------------------------------------
void SEHemorrhage::SetMCIS()
{
  m_MCIS.clear();
  bool found = false;
  //Scale initial bleeding rate by a max value of 250 mL/min-->this would cause hypovolemic shock in 10 minutes assuming blood volume = 5L
  //Max this our "worst case"
  double flowScale_mL_Per_min = 250.0;
  int sev = 0;
  if (m_InitialRate->GetValue(VolumePerTimeUnit::mL_Per_min) > flowScale_mL_Per_min) {
    sev = 5;
  } else {
    sev = (int)ceil(5.0 * m_InitialRate->GetValue(VolumePerTimeUnit::mL_Per_min) / flowScale_mL_Per_min);
  }

  m_MCIS.push_back(sev);
  if (m_Compartment == "Brain") {
    m_MCIS.insert(m_MCIS.end(), { 1, 6, 1, 0 });
    found = true;
  } else if (m_Compartment == "Aorta") {
    m_MCIS.insert(m_MCIS.end(), { 2, 6, 4, 0 });
    found = true;
  } else if (m_Compartment == "LeftArm" || m_Compartment == "RightArm") {
    m_MCIS.insert(m_MCIS.end(), { 3, 0, 0, 0 });
    found = true;
  } else if (m_Compartment == "LeftLeg" || m_Compartment == "RightLeg") {
    m_MCIS.insert(m_MCIS.end(), { 4, 0, 0, 0 });
    found = true;
  } else if (m_OrganMap.find(m_Compartment) != m_OrganMap.end()) {
    m_MCIS.push_back(2);
    //This inserts the code of integers stored in the organ map (associated with the compartment) at the end of the mcis vector
    m_MCIS.insert(m_MCIS.end(), m_OrganMap[m_Compartment].begin(), m_OrganMap[m_Compartment].end());
    found = true;
  }
  if (!found) {
    SetComment("Could not find compartment, defaulting to Aorta");
    m_MCIS.insert(m_MCIS.end(), { 2, 6, 4, 0 });
  }
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::HasMCIS() const
{
  return !m_MCIS.empty();
}
//-----------------------------------------------------------------------------
const char* SEHemorrhage::GetCompartment_cStr() const
{
  return m_Compartment.c_str();
}
//-----------------------------------------------------------------------------
//!
//!  \brief String representing the location of the event.
//!  \return std::string
//!
//!  Only a single hemorrhage can exist at the exsact same compartment string
//!  Any string value can be used. Submitting the same value overwrites the previous event.
std::string SEHemorrhage::GetCompartment() const
{
  return m_Compartment;
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::HasCompartment() const
{
  return !m_Compartment.empty();
}
//-----------------------------------------------------------------------------
void SEHemorrhage::SetCompartment(const char* name)
{
  return SetCompartment(std::string { name });
}
//-----------------------------------------------------------------------------
void SEHemorrhage::SetCompartment(const std::string& name)
{
  m_Compartment = name;
  SetMCIS();
}
//-----------------------------------------------------------------------------
void SEHemorrhage::InvalidateCompartment()
{
  m_Compartment = "";
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::HasInitialRate() const
{
  return m_InitialRate == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
//!
//!  \brief SEScalarVolumePerTime flow rate of the hemorrhage at occurance
//!  \return SEScalarVolumePerTime
//!  Setting Initial Rate to 0 is the effective way of removing/curing a hemorrhage.
SEScalarVolumePerTime& SEHemorrhage::GetInitialRate()
{
  if (m_InitialRate == nullptr)
    m_InitialRate = new SEScalarVolumePerTime();
  return *m_InitialRate;
}
SEScalarVolumePerTime const & SEHemorrhage::GetInitialRate() const
{
  if (m_InitialRate == nullptr)
    const_cast<SEHemorrhage*>(this)->m_InitialRate = new SEScalarVolumePerTime();
  return *m_InitialRate;
}
//-----------------------------------------------------------------------------
bool SEHemorrhage::HasBleedResistance() const
{
  return m_BleedResistance == nullptr ? false : true;
}
//-----------------------------------------------------------------------------
SEScalarFlowResistance& SEHemorrhage::GetBleedResistance()
{
  if (m_BleedResistance == nullptr) {
    m_BleedResistance = new SEScalarFlowResistance();
  }
  return *m_BleedResistance;
}
SEScalarFlowResistance const& SEHemorrhage::GetBleedResistance() const
{
  if (m_BleedResistance == nullptr) {
    const_cast<SEHemorrhage*>(this)->m_BleedResistance = new SEScalarFlowResistance();
  }
  return *m_BleedResistance;
}
//-----------------------------------------------------------------------------
void SEHemorrhage::ToString(std::ostream& str) const
{
  if (m_InitialRate->GetValue(VolumePerTimeUnit::mL_Per_min) < ZERO_APPROX) {
    str << "Patient Action : Stop Hemorrhage";
    if (HasComment())
      str << "\n\tComment: ";
    str << m_Comment;
    str << "\n\tCompartment: ";
    HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
  } else {
    str << "Patient Action : Hemorrhage";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tInitial Bleeding Rate:  ";
    str << *m_InitialRate;
    str << "\n\tCompartment: ";
    HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
    str << "\n\tInjury Code: ";
    for (int i : m_MCIS)
      str << i;
  }
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEHemorrhage::operator==(const SEHemorrhage& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= m_MCIS == rhs.m_MCIS;
  equivilant &= (m_InitialRate && rhs.m_InitialRate) ? m_InitialRate->operator==(*rhs.m_InitialRate) : m_InitialRate == rhs.m_InitialRate;
  equivilant &= (m_BleedResistance && rhs.m_BleedResistance) ? m_BleedResistance->operator==(*rhs.m_BleedResistance) : m_BleedResistance == rhs.m_BleedResistance;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEHemorrhage::operator!=(const SEHemorrhage& rhs) const
{
  return !(*this == rhs);
}
}