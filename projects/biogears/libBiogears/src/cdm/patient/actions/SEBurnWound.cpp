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
  , m_DegreeModifier(1.0)
  , m_burnInitiationTime(0.0) // Automatically set on burn instantiation. Exists in xsd for state loading/unloading 
  , m_TBSA(new SEScalar0To1()) // User input, size of wound measured by total body surface area
  , m_compartments(5)
{
  m_Inflammation = false; // When the burn wound is constructed, the corresponding inflammation state has not been established
  m_DegreeOfBurn = (CDM::enumBurnDegree::value)-1; // User input, burn degree afflicting specified TBSA
  for (auto& sa : m_compartments) {
    sa.SetValue(0.0);
  }
}
//-----------------------------------------------------------------------------
SEBurnWound::~SEBurnWound()
{
  Clear();
  SAFE_DELETE(m_TBSA);
}
//-----------------------------------------------------------------------------
void SEBurnWound::Clear()
{
  SEPatientAction::Clear();
  m_Inflammation = false;
  m_compartmentsAffected.clear();
  m_DegreeOfBurn = (CDM::enumBurnDegree::value)-1;
  m_burnInitiationTime = 0.;
  m_TBSA->Clear();
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
  SetTotalBodySurfaceArea(0.);
  // The degree modifier is a v1.0 methodology for scaling burn response based on instantiated degree of burn specific to the minimal physiology response of first degree as compared to second and third.
  // Minimal validation data exists differentiating the physiology of the three in the hours following a burn, so inital values for second and third are tuned to mimic TBSA data of both.

  if (in.DegreeOfBurn().present()) {
    SetDegreeOfBurn(in.DegreeOfBurn().get());
  } else {
    SetDegreeOfBurn(CDM::enumBurnDegree::Third);
  }

  if (in.BurnInitiationTime().present()) {
    SetTimeOfBurn(in.BurnInitiationTime().get());
  }

  m_compartmentsAffected.clear();
  std::string compt;
  for (const std::string compData : in.Compartments()) {
    AddCompartment(compData);
  }

  if (!HasCompartment()) {
    Warning("No compartment declared for burn. BioGears will assume primarily trunk burn.");
    m_compartmentsAffected.push_back("Trunk");
  }

  SetTotalBodySurfaceArea(in.TotalBodySurfaceArea().value());


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
  if (m_burnInitiationTime != 0.) {
    data.BurnInitiationTime(m_burnInitiationTime);
  }
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
double SEBurnWound::GetTotalBodySurfaceArea() const
{
  return m_TBSA->GetValue();
}
//-----------------------------------------------------------------------------
void SEBurnWound::SetTotalBodySurfaceArea(double rhs)
{
  double allowableTBSA = 0.0;
  for (auto& compData : m_compartmentsAffected) {
    if (compData == "Trunk") {
      allowableTBSA += MAXIMUM_TRUNK;
    } else if (compData == "LeftArm") {
      allowableTBSA += MAXIMUM_LEFT_ARM;
    } else if (compData == "RightArm") {
      allowableTBSA += MAXIMUM_RIGHT_ARM;
    } else if (compData == "LeftLeg") {
      allowableTBSA += MAXIMUM_LEFT_LEG;
    } else if (compData == "RightLeg") {
      allowableTBSA += MAXIMUM_RIGHT_LEG;
    }
  }
  if (allowableTBSA < rhs) {
    Info("The TBSA provided exceeds the area permitted using the Rule of 9's and the compartments selected. The TBSA will be changed to:" + std::to_string(allowableTBSA));
    m_TBSA->SetValue(allowableTBSA);
  } else {
    m_TBSA->SetValue(rhs);
  }

  calculateCompartmentDistribution();
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
void SEBurnWound::SetDegreeOfBurn(CDM::enumBurnDegree::value bd)
{
  m_DegreeOfBurn = bd;
  switch (bd) {
  case CDM::enumBurnDegree::First:
    m_DegreeModifier = 0.05;
    break;
  case CDM::enumBurnDegree::Second:
    m_DegreeModifier = 0.99;
    break;
  case CDM::enumBurnDegree::Third:
    m_DegreeModifier = 1.0;
    break;
  default:
    m_DegreeModifier = 1.0; // Just a default error catch
  }
}
//-----------------------------------------------------------------------------
void SEBurnWound::SetTimeOfBurn(double burnTime)
{
  m_burnInitiationTime = burnTime;
}
//-----------------------------------------------------------------------------
double SEBurnWound::GetTimeOfBurn() const
{
  return m_burnInitiationTime;
}
//-----------------------------------------------------------------------------
double SEBurnWound::GetBurnIntensity() const
{
    return m_DegreeModifier * GetTotalBodySurfaceArea();
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
    if (compartment == ca && m_DegreeOfBurn != CDM::enumBurnDegree::First)
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
void SEBurnWound::AddCompartment(const char* name)
{
  return AddCompartment(std::string { name });
}
//-------------------------------------------------------------------------------
void SEBurnWound::AddCompartment(const std::string& name)
{
  auto not_found = std::find(m_compartmentsAffected.begin(), m_compartmentsAffected.end(), name)
    == m_compartmentsAffected.end();
  if (name == "Trunk"
      || name == "LeftArm"
      || name == "RightArm"
      || name == "LeftLeg"
      || name == "RightLeg") {
    if (not_found) {
      m_compartmentsAffected.push_back(name);
    }
  } else {
    Warning("Compartment not found or not valid : " + name);
  }
  calculateCompartmentDistribution();
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
  calculateCompartmentDistribution();
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
//-------------------------------------------------------------------------------
//!
//!   Evenly distributes the TBSA across the given compartments.
//!   This is done using a drop method which maximizes each compartment and
//!   puts remainder across compartments with sufficent remaing area.
void SEBurnWound::calculateCompartmentDistribution()
{
  if (!m_TBSA->IsValid()) {
    return;
  }
  double remaining_tbsa = m_TBSA->GetValue();

  for (auto& compartment : m_compartments) {
    compartment.SetValue(0);
  }
  size_t full_compartments = 0;
  while (remaining_tbsa > 0.001 && m_compartmentsAffected.size() > 0 && m_compartmentsAffected.size() > full_compartments) {
    double even_divide = remaining_tbsa / (m_compartmentsAffected.size() - full_compartments);
    for (auto compartment : m_compartmentsAffected) {
      if (compartment == "Trunk" && m_compartments[2].GetValue() < MAXIMUM_TRUNK) {
        if (even_divide < MAXIMUM_TRUNK - m_compartments[2].GetValue()) {
          remaining_tbsa -= even_divide;
          m_compartments[2].IncrementValue(even_divide);
        } else {
          remaining_tbsa -= MAXIMUM_TRUNK - m_compartments[2].GetValue();
          m_compartments[2].IncrementValue(MAXIMUM_TRUNK - m_compartments[2].GetValue());
        }
        if (m_compartments[2].GetValue() >= MAXIMUM_TRUNK) {
          full_compartments += 1;
        }
      } else if (compartment == "LeftArm" && m_compartments[0].GetValue() < MAXIMUM_LEFT_ARM) {
        if (even_divide < MAXIMUM_LEFT_ARM - m_compartments[0].GetValue()) {
          remaining_tbsa -= even_divide;
          m_compartments[0].IncrementValue(even_divide);
        } else {
          remaining_tbsa -= MAXIMUM_LEFT_ARM - m_compartments[0].GetValue();
          m_compartments[0].IncrementValue(MAXIMUM_LEFT_ARM - m_compartments[0].GetValue());
        }
        if (m_compartments[0].GetValue() >= MAXIMUM_LEFT_ARM) {
          full_compartments += 1;
        }
      } else if (compartment == "RightArm" && m_compartments[1].GetValue() < MAXIMUM_RIGHT_ARM) {
        if (even_divide < MAXIMUM_RIGHT_ARM - m_compartments[1].GetValue()) {
          remaining_tbsa -= even_divide;
          m_compartments[1].IncrementValue(even_divide);
        } else {
          remaining_tbsa -= MAXIMUM_RIGHT_ARM - m_compartments[1].GetValue();
          m_compartments[1].IncrementValue(MAXIMUM_RIGHT_ARM - m_compartments[1].GetValue());
        }
        if (m_compartments[1].GetValue() >= MAXIMUM_RIGHT_ARM) {
          full_compartments += 1;
        }
      } else if (compartment == "LeftLeg" && m_compartments[3].GetValue() < MAXIMUM_LEFT_LEG) {
        if (even_divide < MAXIMUM_LEFT_LEG - m_compartments[3].GetValue()) {
          remaining_tbsa -= even_divide;
          m_compartments[3].IncrementValue(even_divide);
        } else {
          remaining_tbsa -= MAXIMUM_LEFT_LEG - m_compartments[3].GetValue();
          m_compartments[3].IncrementValue(MAXIMUM_LEFT_LEG - m_compartments[3].GetValue());
        }
        if (m_compartments[3].GetValue() >= MAXIMUM_LEFT_LEG) {
          full_compartments += 1;
        }
      } else if (compartment == "RightLeg" && m_compartments[4].GetValue() < MAXIMUM_RIGHT_LEG) {
        if (even_divide < MAXIMUM_RIGHT_LEG - m_compartments[4].GetValue()) {
          remaining_tbsa -= even_divide;
          m_compartments[4].IncrementValue(even_divide);
        } else {
          remaining_tbsa -= MAXIMUM_RIGHT_LEG - m_compartments[4].GetValue();
          m_compartments[4].IncrementValue(MAXIMUM_RIGHT_LEG - m_compartments[4].GetValue());
        }
        if (m_compartments[4].GetValue() >= MAXIMUM_RIGHT_LEG) {
          full_compartments += 1;
        }
      }
    }
  }
}
//-------------------------------------------------------------------------------
std::vector<double> SEBurnWound::GetTBSACompartmentDistribution() const
{

  return { m_compartments[0].GetValue(), m_compartments[1].GetValue(),
           m_compartments[2].GetValue(), m_compartments[3].GetValue(),
           m_compartments[4].GetValue() };
}
//-------------------------------------------------------------------------------
double SEBurnWound::getLeftArmSA() const { return m_compartments[0].GetValue(); }
double SEBurnWound::getRightArmSA() const { return m_compartments[1].GetValue(); }
double SEBurnWound::getTrunkSA() const { return m_compartments[2].GetValue(); }
double SEBurnWound::getLeftLegSA() const { return m_compartments[3].GetValue(); }
double SEBurnWound::getRightLegSA() const { return m_compartments[4].GetValue(); }
//-------------------------------------------------------------------------------
double SEBurnWound::getLeftArmBurnIntensity() const { return m_DegreeModifier * m_compartments[0].GetValue() / MAXIMUM_LEFT_ARM; }
double SEBurnWound::getRightArmBurnIntensity() const { return m_DegreeModifier * m_compartments[1].GetValue() / MAXIMUM_RIGHT_ARM; }
double SEBurnWound::getTrunkBurnIntensity() const { return m_DegreeModifier * m_compartments[2].GetValue() / MAXIMUM_TRUNK; }
double SEBurnWound::getLeftLegBurnIntensity() const { return m_DegreeModifier * m_compartments[3].GetValue() / MAXIMUM_LEFT_LEG; }
double SEBurnWound::getRightLegBurnIntensity() const { return m_DegreeModifier * m_compartments[4].GetValue() / MAXIMUM_RIGHT_LEG; }
}
