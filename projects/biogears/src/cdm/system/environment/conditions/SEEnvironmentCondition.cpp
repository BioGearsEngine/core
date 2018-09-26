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
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
namespace biogears {
	SEEnvironmentCondition::SEEnvironmentCondition()
		: SECondition()
	{
	}

	SEEnvironmentCondition::~SEEnvironmentCondition()
	{
		Clear();
	}

	void SEEnvironmentCondition::Clear()
	{
		SECondition::Clear();
	}

	bool SEEnvironmentCondition::IsValid() const
	{
		return SECondition::IsValid();
	}

	bool SEEnvironmentCondition::Load(const CDM::EnvironmentConditionData& in)
	{
		SECondition::Load(in);
		return true;
	}

	CDM::EnvironmentConditionData* SEEnvironmentCondition::Unload() const
	{
		CDM::EnvironmentConditionData* data = new CDM::EnvironmentConditionData();
		Unload(*data);
		return data;
	}

	void SEEnvironmentCondition::Unload(CDM::EnvironmentConditionData& data) const
	{
		SECondition::Unload(data);
	}
}