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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Hormones.h"
#include "MockPhysiology.h"

class TestableHormones : Hormones
{
public:
	TestableHormones(Physiology* Phys) : Hormones(Phys){};
	using Hormones::ApplyPainStimulus;
	using Hormones::m_ExperiencedPainValueExt;
	using Hormones::m_ExternalPainStimulusValue;
	using Hormones::m_PainCmpt;
};

TEST(HormonesTest, ApplyPainStim)
{
	MockPhysiology Phys;
	TestableHormones *Hormones = new TestableHormones(&(Physiology)Phys);
	float _externalPain = 5.0;
	int _iCmpt = 1;

	Hormones->ApplyPainStimulus(_externalPain, _iCmpt);

	ASSERT_EQ(_externalPain, Hormones->m_ExperiencedPainValueExt);
	ASSERT_EQ(_externalPain, Hormones->m_ExternalPainStimulusValue);
	ASSERT_EQ(_iCmpt, Hormones->m_PainCmpt);
}