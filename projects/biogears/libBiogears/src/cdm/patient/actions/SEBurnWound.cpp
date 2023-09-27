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
#include <biogears/cdm/patient/actions/SEBurnWound.h>

#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {

SEBurnWound::SEBurnWound()
  : SEPatientAction()
{
  m_Inflammation = false; //When the burn wound is constructed, the corresponding inflammation state has not been established
  m_TBSA = nullptr; //User input, size of wound measured by total body surface area
  m_BurnIntensity = 0.0; //Calculated value of intensity using TBSA and burn degree
  m_DegreeOfBurn = (CDM::enumBurnDegree::value)-1; // User input, burn degree afflicting specified TBSA
}
//-----------------------------------------------------------------------------
SEBurnWound::~SEBurnWound()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SEBurnWound::Clear()
{
  SEPatientAction::Clear();
  m_Inflammation = false;
  m_compartmentsAffected.clear();
  m_DegreeOfBurn = (CDM::enumBurnDegree::value)-1;
  m_BurnIntensity = 0.0;
  SAFE_DELETE(m_TBSA);
}
//-----------------------------------------------------------------------------
bool SEBurnWound::IsValid() const
{
  return SEPatientAction::IsValid() && HasTotalBodySurfaceArea();
}
//-----------------------------------------------------------------------------
bool SEBurnWound::IsActive() const
{
  return IsValid() ? !(m_TBSA->GetValue() < ZERO_APPROX) : false;
}
//-----------------------------------------------------------------------------
bool SEBurnWound::Load(const CDM::BurnWoundData& in)
{
  SEPatientAction::Load(in);
  GetTotalBodySurfaceArea().Load(in.TotalBodySurfaceArea());
  double degreeModifier;
  if (in.DegreeOfBurn().present()) {
    m_DegreeOfBurn = in.DegreeOfBurn().get();
    switch (m_DegreeOfBurn) {
    case CDM::enumBurnDegree::First:
      degreeModifier = 0.05; // Even super small fractions are kicking off inflammation responses and blood volume losses that we do not want. Need to just turn it off until if/when first degree becomes more necessary
      break;
    case CDM::enumBurnDegree::Second:
      degreeModifier = 0.99;
      break;
    case CDM::enumBurnDegree::Third:
      degreeModifier = 1.0;
      break;
    default:
      degreeModifier = 1.0; // Just a default error catch
    }
  } else {
    degreeModifier = 1.0; // If no degree specified, leave unchanged. This is for legacy scenarios. Implemented (September 2023)
  }

  m_compartmentsAffected.clear();
  std::string compt;
  for (const std::string compData : in.Compartments()) {
    compt = compData;
    if (compt != "LeftArm" && compt != "RightArm" && compt != "LeftLeg" && compt != "RightLeg" && compt != "Trunk") {
      Warning("Compartment not found or not valid : " + compt);
    } else {
      std::string compartmentAffected = "";
      compartmentAffected = (compt);
      m_compartmentsAffected.push_back(compartmentAffected);
    }
  }

  if (!HasCompartment()) {
    Warning("No compartment declared for burn. BioGears will assume primarily trunk burn.");
    m_compartmentsAffected.push_back("Trunk");
  }

  double allowableTBSA = 0.0;
  for (const std::string compData : m_compartmentsAffected) {
    if (compData == "Trunk") {
      allowableTBSA += 0.36;
    } else if (compData == "LeftArm") {
      allowableTBSA += 0.09;
    } else if (compData == "RightArm") {
      allowableTBSA += 0.09;
    } else if (compData == "LeftLeg") {
      allowableTBSA += 0.18;
    } else if (compData == "RightLeg") {
      allowableTBSA += 0.18;
    }
  }
  if (allowableTBSA < GetTotalBodySurfaceArea().GetValue()) {
    Info("The TBSA provided exceeds the area permitted using the Rule of 9's and the compartments selected. The TBSA will be changed to:" + std::to_string(allowableTBSA));
    GetTotalBodySurfaceArea().SetValue(allowableTBSA);
  }
  m_BurnIntensity = degreeModifier * GetTotalBodySurfaceArea().GetValue();

  return true;
}
//-----------------------------------------------------------------------------
CDM::BurnWoundData* SEBurnWound::Unload() const
{
  CDM::BurnWoundData* data(new CDM::BurnWoundData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SEBurnWound::Unload(CDM::BurnWoundData& data) const
{
  SEPatientAction::Unload(data);
  if (m_TBSA != nullptr)
    data.TotalBodySurfaceArea(std::unique_ptr<CDM::Scalar0To1Data>(m_TBSA->Unload()));
  if (HasDegreeOfBurn())
    data.DegreeOfBurn(m_DegreeOfBurn);
  for (std::string compData : m_compartmentsAffected) {
    data.Compartments().push_back(compData);
  }
}
//-----------------------------------------------------------------------------
bool SEBurnWound::HasTotalBodySurfaceArea() const
{
  return m_TBSA == nullptr ? false : m_TBSA->IsValid();
}
//-----------------------------------------------------------------------------
SEScalar0To1& SEBurnWound::GetTotalBodySurfaceArea()
{
  if (m_TBSA == nullptr)
    m_TBSA = new SEScalar0To1();
  return *m_TBSA;
}
//-----------------------------------------------------------------------------
bool SEBurnWound::HasDegreeOfBurn() const
{
  return m_DegreeOfBurn == ((CDM::enumBurnDegree::value)-1) ? false : true;
}
//-----------------------------------------------------------------------------
CDM::enumBurnDegree::value SEBurnWound::GetDegreeOfBurn() const
{
  return m_DegreeOfBurn;
}
//-----------------------------------------------------------------------------
double SEBurnWound::GetBurnIntensity() const
{
  return m_BurnIntensity;
}
//-----------------------------------------------------------------------------
bool SEBurnWound::HasInflammation() const
{
  return m_Inflammation;
}
//-----------------------------------------------------------------------------
void SEBurnWound::SetInflammation(bool activate)
{
  m_Inflammation = activate;
}
//-----------------------------------------------------------------------------
bool SEBurnWound::HasCompartment() const
{
  return m_compartmentsAffected.size() == 0 ? false : true;
}
//-----------------------------------------------------------------------------
bool SEBurnWound::HasCompartment(const std::string compartment) const
{
  for (const std::string ca : m_compartmentsAffected) {
    if (compartment == ca)
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
const std::vector<std::string>& SEBurnWound::GetCompartments()
{
  return m_compartmentsAffected;
}
//-----------------------------------------------------------------------------
const std::string SEBurnWound::GetCompartment(const std::string c) const
{
  for (unsigned int i = 0; i < m_compartmentsAffected.size(); i++) {
    if (c == m_compartmentsAffected[i])
      return c;
  }
  return "";
}
//-------------------------------------------------------------------------------
void SEBurnWound::SetCompartment(const char* name)
{
  return SetCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SEBurnWound::SetCompartment(const std::string& name)
{
  //int nextComptAffected = m_compartmentsAffected.size();
  //m_compartmentsAffected[nextComptAffected] = name;
  m_compartmentsAffected.push_back(name);
}
//-----------------------------------------------------------------------------
void SEBurnWound::RemoveCompartment(const std::string c)
{
  std::string ca = "";
  for (unsigned int i = 0; i < m_compartmentsAffected.size(); i++) {
    ca = m_compartmentsAffected[i];
    if (c == ca) {
      m_compartmentsAffected.erase(m_compartmentsAffected.begin() + i);
      ca = "";
    }
  }
}
//-----------------------------------------------------------------------------
void SEBurnWound::RemoveCompartments()
{
  m_compartmentsAffected.clear();
}
//-----------------------------------------------------------------------------

void SEBurnWound::ToString(std::ostream& str) const
{
  str << "Patient Action : Burn Wound";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  str << "\n\tTotal Body Surface Area:  ";
  str << *m_TBSA;
  str << "\n\tDegree of Burn: ";
  HasDegreeOfBurn() ? str << GetDegreeOfBurn() : str << "Not Set";
  str << "\n\tCompartment(s): ";
  if (HasCompartment()) {
    for (unsigned int i = 0; i < m_compartmentsAffected.size(); i++) {
      if (i == 0) {
        str << m_compartmentsAffected[i];
      } else {
        str << ", " << m_compartmentsAffected[i];
      }
    }
  } else {
    str << "No Compartment Set";
  }
  str << std::flush;
}
//-------------------------------------------------------------------------------
bool SEBurnWound::operator==(const SEBurnWound& rhs) const
{
  bool equivilant = m_Comment == rhs.m_Comment;
  equivilant &= (m_TBSA && rhs.m_TBSA) ? m_TBSA->operator==(*rhs.m_TBSA) : m_TBSA == rhs.m_TBSA;
  equivilant &= m_DegreeOfBurn == rhs.m_DegreeOfBurn;
  equivilant &= m_compartmentsAffected == rhs.m_compartmentsAffected;
  return equivilant;
}
//-------------------------------------------------------------------------------
bool SEBurnWound::operator!=(const SEBurnWound& rhs) const
{
  return !(*this == rhs);
}
}