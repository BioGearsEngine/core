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
#include <biogears/cdm/system/environment/actions/SEThermalApplication.h>

#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>

namespace biogears {
SEThermalApplication::SEThermalApplication()
  : SEEnvironmentAction()
{
  m_AppendToPrevious = false;
  m_ActiveHeating = nullptr;
  m_ActiveCooling = nullptr;
  m_AppliedTemperature = nullptr;
}
//-------------------------------------------------------------------------------
SEThermalApplication::~SEThermalApplication()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEThermalApplication::Clear()
{
  SEEnvironmentAction::Clear();
  SAFE_DELETE(m_ActiveHeating);
  SAFE_DELETE(m_ActiveCooling);
  SAFE_DELETE(m_AppliedTemperature);
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::IsValid() const
{
  return SEEnvironmentAction::IsValid();
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::IsActive() const
{
  if (HasActiveHeating() && m_ActiveHeating->GetPower().IsPositive())
    return true;
  if (HasActiveCooling() && m_ActiveCooling->GetPower().IsPositive())
    return true;
  if (HasAppliedTemperature() && m_AppliedTemperature->GetState() == CDM::enumOnOff::On)
    return true;
  return false;
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::Load(const CDM::ThermalApplicationData& in)
{
  // Set this before our super class tells us to Clear if the action wants us to keep our current data
  CDM::ActiveHeatingData* ah = HasActiveHeating() ? GetActiveHeating().Unload() : nullptr;
  CDM::ActiveCoolingData* ac = HasActiveCooling() ? GetActiveCooling().Unload() : nullptr;
  CDM::AppliedTemperatureData* at = HasAppliedTemperature() ? GetAppliedTemperature().Unload() : nullptr;

  SEEnvironmentAction::Load(in);

  if (in.AppendToPrevious()) {
    //NOTE: Appending does not merge with the previous action if it has overlapping components.
    //      If a ActiveHeating and ActiveCooling had previously applied and the incoming data had a
    //      all ActiveHeating with an AppliedTemperature component. Append would add the ApliedTemperature
    //      and overwrite the previous ActiveHeating with the inbound. While not appending would clear ActiveCooling.

    if (ah) {
      GetActiveHeating().Load(*ah);
    }
    if (ac) {
      GetActiveCooling().Load(*ac);
    }
    if (at) {
      GetAppliedTemperature().Load(*at);
    }
  }
  if (in.ActiveHeating().present()) {
    GetActiveHeating().Load(in.ActiveHeating().get());
  }
  if (in.ActiveCooling().present()) {
    GetActiveCooling().Load(in.ActiveCooling().get());
  }
  if (in.AppliedTemperature().present()) {
    GetAppliedTemperature().Load(in.AppliedTemperature().get());
  }

  m_AppendToPrevious = in.AppendToPrevious();

  SAFE_DELETE(ah);
  SAFE_DELETE(ac);
  SAFE_DELETE(at);

  return true;
}
//-------------------------------------------------------------------------------
CDM::ThermalApplicationData* SEThermalApplication::Unload() const
{
  CDM::ThermalApplicationData* data = new CDM::ThermalApplicationData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEThermalApplication::Unload(CDM::ThermalApplicationData& data) const
{
  SEEnvironmentAction::Unload(data);
  if (HasActiveHeating()) {
    data.ActiveHeating(std::unique_ptr<CDM::ActiveHeatingData>(m_ActiveHeating->Unload()));
  }
  if (HasActiveCooling()) {
    data.ActiveCooling(std::unique_ptr<CDM::ActiveCoolingData>(m_ActiveCooling->Unload()));
  }
  if (HasAppliedTemperature()) {
    data.AppliedTemperature(std::unique_ptr<CDM::AppliedTemperatureData>(m_AppliedTemperature->Unload()));
  }
  //NOTE: It might be better to always serialize thermal applications to false and allow users to set this in for
  //      Load commands. The problem is we use Load/UnLoad for cloning in BioGears so if keeping this state data for
  //      Serialization is bad then we need to patch all the ProcessAction functions which clone a AppliedTemperatureData and ensure
  //      it is retained after the clone through caching.
  data.AppendToPrevious(m_AppendToPrevious);
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::HasActiveHeating() const
{
  return m_ActiveHeating != nullptr;
}
//-------------------------------------------------------------------------------
SEActiveHeating& SEThermalApplication::GetActiveHeating()
{
  if (m_ActiveHeating == nullptr)
    m_ActiveHeating = new SEActiveHeating(GetLogger());
  return *m_ActiveHeating;
}
//-------------------------------------------------------------------------------
void SEThermalApplication::RemoveActiveHeating()
{
  SAFE_DELETE(m_ActiveHeating);
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::HasActiveCooling() const
{
  return m_ActiveCooling != nullptr;
}
//-------------------------------------------------------------------------------
SEActiveCooling& SEThermalApplication::GetActiveCooling()
{
  if (m_ActiveCooling == nullptr)
    m_ActiveCooling = new SEActiveCooling(GetLogger());
  return *m_ActiveCooling;
}
//-------------------------------------------------------------------------------
void SEThermalApplication::RemoveActiveCooling()
{
  SAFE_DELETE(m_ActiveCooling);
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::HasAppliedTemperature() const
{
  return m_AppliedTemperature != nullptr;
}
//-------------------------------------------------------------------------------
SEAppliedTemperature& SEThermalApplication::GetAppliedTemperature()
{
  if (m_AppliedTemperature == nullptr)
    m_AppliedTemperature = new SEAppliedTemperature(GetLogger());
  return *m_AppliedTemperature;
}
void SEThermalApplication::RemoveAppliedTemperature()
{
  SAFE_DELETE(m_AppliedTemperature);
}
//-------------------------------------------------------------------------------
bool SEThermalApplication::GetAppendToPrevious() const
{
  return m_AppendToPrevious;
};
//-------------------------------------------------------------------------------
void SEThermalApplication::SetAppendToPrevious(bool toAppend)
{
  m_AppendToPrevious = toAppend;
};
//-------------------------------------------------------------------------------
void SEThermalApplication::ToString(std::ostream& str) const
{
  str << "Environment Action : Thermal Application";
  if (HasComment())
    str << "\n\tComment: " << m_Comment;
  if (HasActiveHeating()) {
    str << "\n\t";
    m_ActiveHeating->ToString(str);
  } else
    str << "\n\tNo Active Heating";

  if (HasActiveCooling()) {
    str << "\n\t";
    m_ActiveCooling->ToString(str);
  } else
    str << "\n\tNo Active Cooling";

  if (HasAppliedTemperature()) {
    str << "\n\t";
    m_AppliedTemperature->ToString(str);
  } else
    str << "\n\tNo Applied Temperature";

  str << std::flush;
}
}