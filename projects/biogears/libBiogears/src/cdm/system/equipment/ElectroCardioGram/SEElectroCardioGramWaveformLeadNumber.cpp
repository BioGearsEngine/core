#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGramWaveformLeadNumber.h>
#include <biogears/string/manipulation.h>

namespace biogears {
SEElectroCardioGramWaveformLeadNumber::SEElectroCardioGramWaveformLeadNumber(Logger* logger)
  : Loggable(logger)
  , m_value(1)
{
}
SEElectroCardioGramWaveformLeadNumber::SEElectroCardioGramWaveformLeadNumber(Logger*, int lead)
  : m_value(1)
{
  SetValue(lead);
}

//-------------------------------------------------------------------------------
SEElectroCardioGramWaveformLeadNumber::~SEElectroCardioGramWaveformLeadNumber()
{
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramWaveformLeadNumber::Load(const CDM::ElectroCardioGramWaveformLeadNumberData& in)
{

  m_value = static_cast<int>(in);
  return true;
}
//-------------------------------------------------------------------------------
CDM::ElectroCardioGramWaveformLeadNumberData* SEElectroCardioGramWaveformLeadNumber::Unload() const
{
  auto data = new CDM::ElectroCardioGramWaveformLeadNumberData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramWaveformLeadNumber::Unload(CDM::ElectroCardioGramWaveformLeadNumberData& data) const
{
  data = m_value;
}
//-------------------------------------------------------------------------------
int SEElectroCardioGramWaveformLeadNumber::GetValue() const
{
  return m_value;
}
//-------------------------------------------------------------------------------
void SEElectroCardioGramWaveformLeadNumber::SetValue(int v)
{
  if (0 < v && v <= 12) {
    m_value = v;
    return;
  }
  Error(biogears::asprintf("Lead number must be beteen {0,12}, but given %d no action taken", v));
}
//-------------------------------------------------------------------------------
SEElectroCardioGramWaveformLeadNumber::operator int&()
{
  return m_value;
}
SEElectroCardioGramWaveformLeadNumber::operator int() const
{
  return m_value;
}
//-------------------------------------------------------------------------------
SEElectroCardioGramWaveformLeadNumber& SEElectroCardioGramWaveformLeadNumber::operator=(SEElectroCardioGramWaveformLeadNumber const& rhs)
{
  this->m_value = rhs.m_value;
  return *this;
}
//-------------------------------------------------------------------------------
bool SEElectroCardioGramWaveformLeadNumber::operator==(SEElectroCardioGramWaveformLeadNumber const& rhs) const
{
  return m_value == rhs.m_value;
}
bool SEElectroCardioGramWaveformLeadNumber::operator!=(SEElectroCardioGramWaveformLeadNumber const& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
bool operator==(SEElectroCardioGramWaveformLeadNumber const& lhs, int const& rhs)
{
  return lhs.m_value == rhs;
}
bool operator!=(SEElectroCardioGramWaveformLeadNumber const& lhs, int const& rhs)
{
  return !operator==(lhs, rhs);
}
bool operator==(int const& lhs, SEElectroCardioGramWaveformLeadNumber const& rhs)
{
  return lhs == rhs.m_value;
}
bool operator!=(int const& lhs, SEElectroCardioGramWaveformLeadNumber const& rhs)
{
  return !operator==(lhs, rhs);
}
}