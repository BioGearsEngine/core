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
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>

#include <biogears/cdm/substance/SESubstance.h>

namespace biogears {
SESubstanceQuantity::SESubstanceQuantity(SESubstance& sub)
  : Loggable(sub.GetLogger())
  , m_Substance(sub)
{
}
//-------------------------------------------------------------------------------
SESubstanceQuantity::~SESubstanceQuantity()
{
}
//-------------------------------------------------------------------------------
bool SESubstanceQuantity::Load(const CDM::SubstanceQuantityData& in)
{
  Clear();
  return true;
}
//-------------------------------------------------------------------------------
void SESubstanceQuantity::Unload(CDM::SubstanceQuantityData& data)
{
  data.Substance(m_Substance.GetName());
}
}