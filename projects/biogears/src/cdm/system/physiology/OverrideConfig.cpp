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

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/schema/cdm/Properties.hxx>
#include "biogears/cdm/system/physiology/OverrideConfig.h"


// Work in Progress
OverrideConfig::OverrideConfig()

{
	m_overrideMode = CDM::enumOnOff::value(-1);
	m_MeanArterialPressureOverride = nullptr;

}

OverrideConfig::~OverrideConfig()
{
  Clear();
}

void OverrideConfig::Clear()
{
  /* Check this function */
	SAFE_DELETE(m_MeanArterialPressureOverride);
	m_overrideMode = CDM::enumOnOff::value(-1); ;
}

bool OverrideConfig::LoadOverride(const std::string& file)
{
  Clear();
  std::stringstream sst;
  if (file.empty()) {
    sst << "Override file not provided: " << file << std::endl;
    Error(sst);
    return false;
  }

  std::unique_ptr<CDM::ObjectData> data = Serializer::ReadFile(file, GetLogger());
  CDM::OverrideConfigData* pData = dynamic_cast<CDM::OverrideConfigData*>(data.get());
  if (pData == nullptr) {
    sst << "Override data file could not be read : " << file << std::endl;
    Error(sst);
    return false;
  }
  if (!Load(*pData)) {
    sst << "Unable to load override file: " << file << std::endl;
    Error(sst);
    return false;
  }
  m_overrideMode = CDM::enumOnOff::On;
  return true;
}

//Need to finish
bool OverrideConfig::Load(const CDM::OverrideConfigData& in)
{
	if (in.CardiovascularOverride().present()) {
		const CDM::CardiovascularOverrideData& config = in.CardiovascularOverride().get();
		if (config.EnableCardiovascularOverride().present())
      EnableCardiovascularOverride(config.EnableCardiovascularOverride().get());
	}

	return true;
}

CDM::OverrideConfigData* OverrideConfig::Unload() const
{
  CDM::OverrideConfigData* data(new CDM::OverrideConfigData());
  Unload(*data);
  return data; 
  
}

void OverrideConfig::Unload(CDM::OverrideConfigData& data) const
{
  /*
	CDM::CardiovascularOverrideData* cardio(new CDM::CardiovascularOverrideData());
	if (HasMeanArterialPressureOverride())
		cardio->MeanArterialPressureOverride(std::unique_ptr<CDM::ScalarPressureData>(m_MeanArterialPressureOverride->Unload()));
  */

  // PhysiologyEngineConfiguration::Unload(data);
  CDM::CardiovascularOverrideData* cardiovascularoverride(new CDM::CardiovascularOverrideData());
  if (HasEnableCardiovascularOverride())
    cardiovascularoverride->EnableCardiovascularOverride(m_overrideMode);
  data.CardiovascularOverride(std::unique_ptr<CDM::CardiovascularOverrideData>(cardiovascularoverride));
}

bool OverrideConfig::ReadOverrideParameters(const std::string& overrideParameterFile)
{
  CDM::OverrideConfigData* pData;
  std::unique_ptr<CDM::ObjectData> data;

  std::string opFile = overrideParameterFile;
  if (opFile.find("/override") == std::string::npos) {
    opFile = "./override/";
    opFile += overrideParameterFile;
  }

  data = Serializer::ReadFile(opFile, GetLogger());
  pData = dynamic_cast<CDM::OverrideConfigData*>(data.get());
  if (pData == nullptr) {
    std::stringstream ss;
    ss << "Override file could not be read : " << overrideParameterFile << std::endl;
    Error(ss);
    return false;
  }
  return Load(*pData);
}

//commenting for now
//bool OverrideConfig::HasOverride() const
//{
//  return m_MeanArterialPressureOverride == nullptr ? false : m_MeanArterialPressureOverride->IsValid();
//}
//
//SEScalarPressure& OverrideConfig::GetOverride()
//{
//  if (m_MeanArterialPressureOverride == nullptr)
//    m_MeanArterialPressureOverride = new SEScalarPressure();
//  return *m_MeanArterialPressureOverride;
//}
//
//double OverrideConfig::GetOverride(const PressureUnit& unit) const
//{
//  if (m_MeanArterialPressureOverride == nullptr)
//  {
//    return SEScalar::dNaN();
//  }
//  return m_MeanArterialPressureOverride->GetValue(unit);
//}

////////////////////
/** CardioVascular */
////////////////////

bool OverrideConfig::HasMeanArterialPressureOverride() const
{
	return m_MeanArterialPressureOverride == nullptr ? false : m_MeanArterialPressureOverride->IsValid();
}
SEScalar& OverrideConfig::GetMeanArterialPressureOverride()
{
	if (m_MeanArterialPressureOverride == nullptr)
		m_MeanArterialPressureOverride = new SEScalarPressure();
	return *m_MeanArterialPressureOverride;
}
double OverrideConfig::GetMeanArterialPressureOverride(const PressureUnit& unit) const
{
	if (m_MeanArterialPressureOverride == nullptr)
		return SEScalar::dNaN();
	return m_MeanArterialPressureOverride->GetValue(unit);
}
