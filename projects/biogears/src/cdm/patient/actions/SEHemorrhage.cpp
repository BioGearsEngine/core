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

#include <biogears/cdm/stdafx.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/schema/HemorrhageData.hxx>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/schema/ScalarVolumePerTimeData.hxx>
#include <biogears/schema/IntegerArray.hxx>
#include <biogears/schema/IntegerList.hxx>
#include <biogears/cdm/properties/SEScalar0To1.h>

SEHemorrhage::SEHemorrhage() : SEPatientAction()
{
  m_Compartment = ""; //This is the compartment we use to store information about hemorrhage
  m_MCIS;
  m_BleedName = "";	  //This is the name of the pathway in circuit that will have its resistance changed
  m_Severity = nullptr;

  //Place paths in torso in a map so that we don't get too messy with nested conditionals.  Each vector is digits 2-4 of the MCIS code
  organMap["Vena Cava"] = std::make_pair("VenaCavaBleed", std::vector<unsigned int>{6, 6, 0});
  organMap["Lung"] = std::make_pair("LungBleed", std::vector<unsigned int>{7, 1, 0});
  organMap["Myocardium"] = std::make_pair("HeartBleed", std::vector<unsigned int>{7, 2, 0});
  organMap["Liver"] = std::make_pair("LiverBleed", std::vector<unsigned int>{8, 1, 0});
  organMap["Spleen"] = std::make_pair("SpleenBleed", std::vector<unsigned int>{8, 2, 0});
  organMap["Splanchnic"] = std::make_pair("SplanchnicBleed", std::vector<unsigned int>{8, 3, 0});
  organMap["Kidney"] = std::make_pair("KidneyBleed", std::vector<unsigned int>{8, 4, 0});
  organMap["Small Intestine"] = std::make_pair("SmallIntestineBleed", std::vector<unsigned int>{8, 5, 0});
  organMap["Large Intestine"] = std::make_pair("LargeIntestineBleed", std::vector<unsigned int>{8, 6, 0});
}

SEHemorrhage::~SEHemorrhage()
{
	Clear();
}

void SEHemorrhage::Clear()
{
	SEPatientAction::Clear();
	SAFE_DELETE(m_Severity);
    m_Compartment = "";
	m_MCIS.clear();
	m_BleedName = "";
}

bool SEHemorrhage::IsValid() const
{
	return SEPatientAction::IsValid() && HasCompartment() && HasBleedName();
}

bool SEHemorrhage::IsActive() const
{
	return IsValid() ? !(m_Severity->GetValue()<=ZERO_APPROX) : false;
}

bool SEHemorrhage::Load(const CDM::HemorrhageData& in)
{
	SEPatientAction::Load(in);
	GetSeverity().Load(in.Severity());
	m_Compartment = in.Compartment();
	SetBleedPath();

	return true;
}

CDM::HemorrhageData* SEHemorrhage::Unload() const
{
	CDM::HemorrhageData*data(new CDM::HemorrhageData());
	Unload(*data);
	return data;
}

void SEHemorrhage::Unload(CDM::HemorrhageData& data) const
{
	SEPatientAction::Unload(data);
	if(HasSeverity())
		data.Severity(std::unique_ptr<CDM::Scalar0To1Data>(m_Severity->Unload()));
	if (HasCompartment())
		data.Compartment(m_Compartment);
}

void SEHemorrhage::SetBleedPath()
{
	bool found = false;
	int sev = (int)ceil(5.0*m_Severity->GetValue());
	m_MCIS.push_back(sev);
	if (m_Compartment == "Head")
	{
		m_MCIS.insert(m_MCIS.end(), { 1,6,1,0 });
		m_BleedName = "BrainBleed";
		found = true;
	}
	else if (m_Compartment == "Major Artery")
	{
		m_MCIS.insert(m_MCIS.end(), { 2,6,4,0 });
		m_BleedName = "AortaBleed";
		found = true;
	}
	else if (m_Compartment == "Arm")
	{
		m_MCIS.insert(m_MCIS.end(), { 3,0,0,0 });
		m_BleedName = "ArmBleed";
		found = true;
	}
	else if (m_Compartment == "Leg")
	{
		m_MCIS.insert(m_MCIS.end(), { 4,0,0,0 });
		m_BleedName = "LegBleed";
		found = true;
	}
	else
	{
		m_MCIS.push_back(2);
		//This inserts the code of integers stored in the organ map (associated with the compartment) at the end of the mcis vector
		m_MCIS.insert(m_MCIS.end(), organMap[m_Compartment].second.begin(),organMap[m_Compartment].second.end());
		m_BleedName = organMap[m_Compartment].first;
		found = true;
	}
	if (!found)
	{
		SetComment("Could not find compartment, defaulting to Aorta");
		m_MCIS.insert(m_MCIS.end(), { 2,6,4,0 });
		m_BleedName = "AortaBleed";
	}
}
bool SEHemorrhage::HasMCIS() const
{
	return !m_MCIS.empty();
}

std::string SEHemorrhage::GetBleedName() const
{
	return m_BleedName;
}
bool SEHemorrhage::HasBleedName() const
{
	return !m_BleedName.empty();
}

std::string SEHemorrhage::GetCompartment() const
{
	return m_Compartment;
}
bool SEHemorrhage::HasCompartment() const
{
	return !m_Compartment.empty();
}
void SEHemorrhage::SetCompartment(const std::string& name)
{
	m_Compartment = name;
}
void SEHemorrhage::InvalidateCompartment()
{
  m_Compartment = "";
}

bool SEHemorrhage::HasSeverity() const
{
	return m_Severity == nullptr ? false : true;
}
SEScalar0To1& SEHemorrhage::GetSeverity()
{
	if (m_Severity == nullptr)
		m_Severity = new SEScalar0To1();
	return *m_Severity;
}


void SEHemorrhage::ToString(std::ostream &str) const
{
	if (m_Severity->GetValue() == 0.0)
	{
		str << "Patient Action : Stop Hemorrhage";
		if (HasComment())
			str << "\n\tComment: "; str << m_Comment;
		str << "\n\tCompartment: "; HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
	}
	else
	{
		str << "Patient Action : Hemorrhage";
		if (HasComment())
			str << "\n\tComment: " << m_Comment;
		str << "\n\tSeverity:  ";  str << *m_Severity;
		str << "\n\tCompartment: "; HasCompartment() ? str << GetCompartment() : str << "No Compartment Set";
		str << "\n\tInjury Code: ";
		for (int i : m_MCIS)
			str << i;
		str << std::flush;
	}
}
