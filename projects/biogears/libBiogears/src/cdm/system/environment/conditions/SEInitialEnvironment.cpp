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
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
	SEInitialEnvironment::SEInitialEnvironment(SESubstanceManager& substances)
		: SEEnvironmentCondition()
		, m_Substances(substances)
	{
		m_Conditions = nullptr;
		InvalidateConditionsFile();
	}
  //-----------------------------------------------------------------------------
	SEInitialEnvironment::~SEInitialEnvironment()
	{
		Clear();
	}
  //-----------------------------------------------------------------------------
	void SEInitialEnvironment::Clear()
	{
		SEEnvironmentCondition::Clear();
		InvalidateConditionsFile();
		SAFE_DELETE(m_Conditions);
	}
  //-----------------------------------------------------------------------------
	bool SEInitialEnvironment::IsValid() const
	{
		return SEEnvironmentCondition::IsValid() && (HasConditions() || HasConditionsFile());
	}
  //-----------------------------------------------------------------------------
	bool SEInitialEnvironment::Load(const CDM::InitialEnvironmentData& in)
	{
		SEEnvironmentCondition::Load(in);
		if (in.ConditionsFile().present())
			SetConditionsFile(in.ConditionsFile().get());
		if (in.Conditions().present())
			GetConditions().Load(in.Conditions().get());
		return true;
	}
  //-----------------------------------------------------------------------------
	CDM::InitialEnvironmentData* SEInitialEnvironment::Unload() const
	{
		CDM::InitialEnvironmentData* data = new CDM::InitialEnvironmentData();
		Unload(*data);
		return data;
	}
  //-----------------------------------------------------------------------------
	void SEInitialEnvironment::Unload(CDM::InitialEnvironmentData& data) const
	{
		SEEnvironmentCondition::Unload(data);
		if (HasConditions())
			data.Conditions(std::unique_ptr<CDM::EnvironmentalConditionsData>(m_Conditions->Unload()));
		else if (HasConditionsFile())
			data.ConditionsFile(m_ConditionsFile);
	}
  //-----------------------------------------------------------------------------
	bool SEInitialEnvironment::HasConditions() const
	{
		return m_Conditions != nullptr;
	}
  //-----------------------------------------------------------------------------
	SEEnvironmentalConditions& SEInitialEnvironment::GetConditions()
	{
		m_ConditionsFile = "";
		if (m_Conditions == nullptr)
			m_Conditions = new SEEnvironmentalConditions(m_Substances);
		return *m_Conditions;
	}
  //-----------------------------------------------------------------------------
	const SEEnvironmentalConditions* SEInitialEnvironment::GetConditions() const
	{
		return m_Conditions;
	}
  //-----------------------------------------------------------------------------
	std::string SEInitialEnvironment::GetConditionsFile() const
	{
		return m_ConditionsFile;
	}
  //-----------------------------------------------------------------------------
	const char* SEInitialEnvironment::GetConditionsFile_cStr() const
	{
		return m_ConditionsFile.c_str();
	}
  //-----------------------------------------------------------------------------
  void SEInitialEnvironment::SetConditionsFile(const char* fileName)
	{
    return SetConditionsFile(std::string{ fileName });
	}
  //-----------------------------------------------------------------------------
	void SEInitialEnvironment::SetConditionsFile(const std::string& fileName)
	{
		if (m_Conditions != nullptr)
			SAFE_DELETE(m_Conditions);
		m_ConditionsFile = fileName;
	}
  //-----------------------------------------------------------------------------
	bool SEInitialEnvironment::HasConditionsFile() const
	{
		return m_ConditionsFile.empty() ? false : true;
	}
  //-----------------------------------------------------------------------------
	void SEInitialEnvironment::InvalidateConditionsFile()
	{
		m_ConditionsFile = "";
	}
  //-----------------------------------------------------------------------------
	void SEInitialEnvironment::ToString(std::ostream& str) const
	{
		str << "Environment Change";
		if (HasComment())
			str << "\n\tComment: " << m_Comment;
		if (HasConditionsFile())
			str << "\n\tConditions File: ";
		str << m_ConditionsFile;
		if (HasConditions()) {
			str << "\n\tSurroundingType: ";
			m_Conditions->HasSurroundingType() ? str << m_Conditions->GetSurroundingType() : str << "Not Set";
			str << "\n\tAir Velocity: ";
			m_Conditions->HasAirVelocity() ? str << m_Conditions->GetAirVelocity() : str << "Not Set";
			str << "\n\tAmbient Temperature: ";
			m_Conditions->HasAmbientTemperature() ? str << m_Conditions->GetAmbientTemperature() : str << "Not Set";
			str << "\n\tAtmospheric Pressure: ";
			m_Conditions->HasAtmosphericPressure() ? str << m_Conditions->GetAtmosphericPressure() : str << "Not Set";
			str << "\n\tClothing Resistance: ";
			m_Conditions->HasClothingResistance() ? str << m_Conditions->GetClothingResistance() : str << "Not Set";
			str << "\n\tEmissivity: ";
			m_Conditions->HasEmissivity() ? str << m_Conditions->GetEmissivity() : str << "Not Set";
			str << "\n\tMean Radiant Temperature: ";
			m_Conditions->HasMeanRadiantTemperature() ? str << m_Conditions->GetMeanRadiantTemperature() : str << "Not Set";
			str << "\n\tRelative Humidity: ";
			m_Conditions->HasRelativeHumidity() ? str << m_Conditions->GetRelativeHumidity() : str << "Not Set";
			str << "\n\tRespiration Ambient Temperature: ";
			m_Conditions->HasRespirationAmbientTemperature() ? str << m_Conditions->GetRespirationAmbientTemperature() : str << "Not Set";
			if (m_Conditions->HasAmbientGas()) {
				for (SESubstanceFraction* sf : m_Conditions->GetAmbientGases()) {
					str << "\n\tSubstance : " << sf->GetSubstance().GetName() << " Fraction Amount " << sf->GetFractionAmount();
				}
			}
			if (m_Conditions->HasAmbientAerosol()) {
				for (SESubstanceConcentration* sc : m_Conditions->GetAmbientAerosols()) {
					str << "\n\tSubstance : " << sc->GetSubstance().GetName() << " Concentration " << sc->GetConcentration();
				}
			}
		}
		str << std::flush;
	}
  //-----------------------------------------------------------------------------
}