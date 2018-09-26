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

//----------------------------------------------------------------------------
/// @file PrefixDescriptor.cpp
//----------------------------------------------------------------------------
#include <biogears/cdm/utils/unitconversion/PrefixDescriptor.h>
namespace biogears {
  CPrefixDescriptor::CPrefixDescriptor(std::string name, std::string sym, double scaleFac)
  : m_strName(name)
  , m_cSym(sym[0])
  , m_dScaleFac(scaleFac)
{
  // All done already
}

CPrefixDescriptor::CPrefixDescriptor(std::string name, char sym, double scaleFac)
  : m_strName(name)
  , m_cSym(sym)
  , m_dScaleFac(scaleFac)
{
  // All done already
}

const std::string& CPrefixDescriptor::GetName() const
{
  return m_strName;
};

char CPrefixDescriptor::GetSymbol() const
{
  return m_cSym;
}

double CPrefixDescriptor::GetScaleFactor() const
{
  return m_dScaleFac;
}
}