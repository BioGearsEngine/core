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
#include <biogears/cdm/properties/SEDecimalFormat.h>

//Standard Includes
#include <fstream>
#include <iomanip>
//Project Includes
#include <biogears/schema/cdm/Properties.hxx>

namespace biogears {

template class std::map<std::string, SEDecimalFormat>;

SEDecimalFormat::SEDecimalFormat(const SEDecimalFormat* dfault)
{
  Reset();
  if (dfault != nullptr)
    Set(*dfault);
}

SEDecimalFormat::~SEDecimalFormat()
{
  Reset();
}

void SEDecimalFormat::Reset()
{
  m_Precision = 6;
  m_Notation = DecimalNotation::Default;
}

void SEDecimalFormat::Set(const SEDecimalFormat& f)
{
  m_Precision = f.m_Precision;
  m_Notation = f.m_Notation;
}

bool SEDecimalFormat::Load(const CDM::DecimalFormatData& in)
{
  Reset();
  if (in.DecimalFormat().present()) {
    if (in.DecimalFormat().get() == CDM::enumDecimalFormat::FixedMantissa)
      m_Notation = DecimalNotation::Fixed;
    else if (in.DecimalFormat().get() == CDM::enumDecimalFormat::SignificantDigits)
      m_Notation = DecimalNotation::Scientific;
  }
  if (in.Precision().present())
    m_Precision = in.Precision().get();
  return true;
}
CDM::DecimalFormatData* SEDecimalFormat::Unload()
{
  CDM::DecimalFormatData* to = new CDM::DecimalFormatData();
  Unload(*to);
  return to;
}
void SEDecimalFormat::Unload(CDM::DecimalFormatData& data) const
{
  data.Precision(static_cast<CDM::DecimalFormatData::Precision_type>(m_Precision));
  switch (m_Notation) {
  case DecimalNotation::Default:
  case DecimalNotation::Fixed: {
    data.DecimalFormat(CDM::enumDecimalFormat::FixedMantissa);
    break;
  }
  case DecimalNotation::Scientific: {
    data.DecimalFormat(CDM::enumDecimalFormat::SignificantDigits);
    break;
  }
  }
}

void SEDecimalFormat::SetPrecision(std::streamsize p)
{
  m_Precision = p;
}
std::streamsize SEDecimalFormat::GetPrecision()
{
  return m_Precision;
}

void SEDecimalFormat::SetNotation(DecimalNotation n)
{
  m_Notation = n;
}
DecimalNotation SEDecimalFormat::GetNotation()
{
  return m_Notation;
}

void SEDecimalFormat::SetStream(std::ofstream& s)
{
  switch (m_Notation) {
  case DecimalNotation::Default:
    s
#ifndef ANDROID
      << std::defaultfloat
#endif
      << std::setprecision(m_Precision);
    break;
  case DecimalNotation::Fixed:
    s << std::fixed << std::setprecision(m_Precision);
    break;
  case DecimalNotation::Scientific:
    s << std::scientific << std::setprecision(m_Precision);
  }
}

bool SEDecimalFormat::operator==(const SEDecimalFormat& rhs) const
{

  return m_Precision == rhs.m_Precision
    && m_Notation == rhs.m_Notation;
}
bool SEDecimalFormat::operator!=(const SEDecimalFormat& rhs) const
{
  return !(*this == rhs);
}

}
