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

#pragma once
#include "HowToTracker.h"
#include "HowTo-ThreadedBioGears.h"

namespace biogears
{
class SESepsis;
class SESubstance;
class SESubstanceInfusion;
}

void HowToDynamicSepsis();

class DynamicSepsis : public BioGearsThread
{
public:
	DynamicSepsis(const std::string& logfile);
	virtual ~DynamicSepsis();

	void SetAntibiotic();
	void SetNorepinephrine(double& concentration);
	void Status();
	void SetSepsis(std::string& location, double& severity);
	
protected:
	biogears::SESepsis*							m_sepsis;
  biogears::SESubstanceInfusion*				m_pressor;
  biogears::SESubstanceCompoundInfusion*		m_antibiotic;
};
