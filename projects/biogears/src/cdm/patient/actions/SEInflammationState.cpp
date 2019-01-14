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

#include "biogears/cdm/patient/actions/SEInflammationState.h"

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>

namespace biogears {

SEInflammationState::SEInflammationState()
  : m_Pathogen(nullptr)
  , m_Trauma(nullptr)
  , m_MacrophageResting(nullptr)
  , m_MacrophageActive(nullptr)
  , m_NeutrophilResting(nullptr)
  , m_NeutrophilActive(nullptr)
  , m_InducibleNOSynthasePre(nullptr)
  , m_InducibleNOSynthase(nullptr)
  , m_ConstitutiveNOSynthase(nullptr)
  , m_Nitrate(nullptr)
  , m_NitricOxide(nullptr)
  , m_TumorNecrosisFactor(nullptr)
  , m_Interleukin6(nullptr)
  , m_Interleukin10(nullptr)
  , m_Interleukin12(nullptr)
  , m_Catecholamines(nullptr)
  , m_TissueIntegrity(nullptr)
{
}
//-------------------------------------------------------------------------------
SEInflammationState::~SEInflammationState()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEInflammationState::Clear()
{
  SAFE_DELETE(m_Pathogen);
  SAFE_DELETE(m_Trauma);
  SAFE_DELETE(m_MacrophageActive);
  SAFE_DELETE(m_MacrophageResting);
  SAFE_DELETE(m_NeutrophilResting);
  SAFE_DELETE(m_NeutrophilActive);
  SAFE_DELETE(m_InducibleNOSynthasePre);
  SAFE_DELETE(m_InducibleNOSynthase);
  SAFE_DELETE(m_ConstitutiveNOSynthase);
  SAFE_DELETE(m_Nitrate);
  SAFE_DELETE(m_NitricOxide);
  SAFE_DELETE(m_TumorNecrosisFactor);
  SAFE_DELETE(m_Interleukin6);
  SAFE_DELETE(m_Interleukin10);
  SAFE_DELETE(m_Interleukin12);
  SAFE_DELETE(m_Catecholamines);
  SAFE_DELETE(m_TissueIntegrity);
  m_InflammationSources.clear();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::Load(const CDM::InflammationStateData& in)
{
  GetPathogen().Load(in.Pathogen());
  GetTrauma().Load(in.Trauma());
  GetMacrophageResting().Load(in.MacrophageResting());
  GetMacrophageActive().Load(in.MacrophageActive());
  GetNeutrophilResting().Load(in.NeutrophilResting());
  GetNeutrophilActive().Load(in.NeutrophilActive());
  GetInducibleNOSynthasePre().Load(in.InducibleNOSynthasePre());
  GetInducibleNOSynthase().Load(in.InducibleNOSynthase());
  GetConstitutiveNOSynthase().Load(in.ConstitutiveNOSynthase());
  GetNitrate().Load(in.Nitrate());
  GetNitricOxide().Load(in.NitricOxide());
  GetTumorNecrosisFactor().Load(in.TumorNecrosisFactor());
  GetInterleukin6().Load(in.Interleukin6());
  GetInterleukin10().Load(in.Interleukin10());
  GetInterleukin12().Load(in.Interleukin12());
  GetCatecholamines().Load(in.Catecholamines());
  GetTissueIntegrity().Load(in.TissueIntegrity());
  for (auto src : in.Source()){
    m_InflammationSources.push_back(src);
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::InflammationStateData* SEInflammationState::Unload() const
{
  CDM::InflammationStateData* data = new CDM::InflammationStateData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEInflammationState::Unload(CDM::InflammationStateData& data) const
{
  data.Pathogen(std::unique_ptr<CDM::ScalarData>(m_Pathogen->Unload()));
  data.Trauma(std::unique_ptr<CDM::ScalarData>(m_Trauma->Unload()));
  data.MacrophageResting(std::unique_ptr<CDM::ScalarData>(m_MacrophageResting->Unload()));
  data.MacrophageActive(std::unique_ptr<CDM::ScalarData>(m_MacrophageActive->Unload()));
  data.NeutrophilResting(std::unique_ptr<CDM::ScalarData>(m_NeutrophilResting->Unload()));
  data.NeutrophilActive(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActive->Unload()));
  data.InducibleNOSynthasePre(std::unique_ptr<CDM::ScalarData>(m_InducibleNOSynthasePre->Unload()));
  data.InducibleNOSynthase(std::unique_ptr<CDM::ScalarData>(m_InducibleNOSynthase->Unload()));
  data.ConstitutiveNOSynthase(std::unique_ptr<CDM::ScalarData>(m_ConstitutiveNOSynthase->Unload()));
  data.Nitrate(std::unique_ptr<CDM::ScalarData>(m_Nitrate->Unload()));
  data.NitricOxide(std::unique_ptr<CDM::ScalarData>(m_NitricOxide->Unload()));
  data.TumorNecrosisFactor(std::unique_ptr<CDM::ScalarData>(m_TumorNecrosisFactor->Unload()));
  data.Interleukin6(std::unique_ptr<CDM::ScalarData>(m_Interleukin6->Unload()));
  data.Interleukin10(std::unique_ptr<CDM::ScalarData>(m_Interleukin10->Unload()));
  data.Interleukin12(std::unique_ptr<CDM::ScalarData>(m_Interleukin12->Unload()));
  data.Catecholamines(std::unique_ptr<CDM::ScalarData>(m_Catecholamines->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  for (auto src : m_InflammationSources) {
    data.Source().push_back(src);
  }
}
//-------------------------------------------------------------------------------
void SEInflammationState::InitializeState()
{
  //Values from Chow2005Acute
  GetPathogen().SetValue(0.0);  //Change this back to 0 after testing
  GetTrauma().SetValue(0.0);
  GetMacrophageResting().SetValue(1.0);
  GetMacrophageActive().SetValue(0.0);
  GetNeutrophilResting().SetValue(1.0);
  GetNeutrophilActive().SetValue(0.0);
  GetInducibleNOSynthase().SetValue(0.0);
  GetInducibleNOSynthasePre().SetValue(0.0);
  GetConstitutiveNOSynthase().SetValue(0.05);
  GetNitrate().SetValue(0.0);
  GetNitricOxide().SetValue(0.0);
  GetTumorNecrosisFactor().SetValue(0.0);
  GetInterleukin6().SetValue(17.0);     //Value is in pg/L-->gets scaled down in model
  GetInterleukin10().SetValue(80.0);    //Value is in pg/L-->gets scaled down in model  
  GetInterleukin12().SetValue(0.0);
  GetCatecholamines().SetValue(0.0);
  GetTissueIntegrity().SetValue(1.0);
}
//-------------------------------------------------------------------------------
bool SEInflammationState::IsValid()
{
  if (HasPathogen() && HasTrauma() && HasMacrophageResting() && HasMacrophageActive() && HasNeutrophilResting() && HasNeutrophilActive() && HasInducibleNOSynthasePre() && HasInducibleNOSynthase() && HasConstitutiveNOSynthase() && HasNitrate() && HasNitricOxide() && HasTumorNecrosisFactor() && HasInterleukin6() && HasInterleukin10() && HasInterleukin12() && HasCatecholamines() && HasTissueIntegrity())
    return true;
  else
    return false;
}
//-------------------------------------------------------------------------------
const SEScalar* SEInflammationState::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEInflammationState::GetScalar(const std::string& name)
{
  if (name.compare("Pathogen") == 0)
    return &GetPathogen();
  if (name.compare("Trauma") == 0)
    return &GetTrauma();
  if (name.compare("MacrophageResting") == 0)
    return &GetMacrophageResting();
  if (name.compare("MacrophageActive") == 0)
    return &GetMacrophageActive();
  if (name.compare("NeutrophilResting") == 0)
    return &GetNeutrophilResting();
  if (name.compare("NeutrophilActive") == 0)
    return &GetNeutrophilActive();
  if (name.compare("InducibleNOSynthasePre") == 0)
    return &GetInducibleNOSynthasePre();
  if (name.compare("InducibleNOSynthase") == 0)
    return &GetInducibleNOSynthase();
  if (name.compare("ConstitutiveNOSynthase") == 0)
    return &GetConstitutiveNOSynthase();
  if (name.compare("Nitrate") == 0)
    return &GetNitrate();
  if (name.compare("NitricOxide") == 0)
    return &GetNitricOxide();
  if (name.compare("TumorNecrosisFactor") == 0)
    return &GetTumorNecrosisFactor();
  if (name.compare("Interleukin6") == 0)
    return &GetInterleukin6();
  if (name.compare("Interleukin10") == 0)
    return &GetInterleukin10();
  if (name.compare("Interleukin12") == 0)
    return &GetInterleukin12();
  if (name.compare("Catecholamines") == 0)
    return &GetCatecholamines();
  if (name.compare("TissueIntegrity") == 0)
    return &GetTissueIntegrity();

  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasPathogen() const
{
  return m_Pathogen == nullptr ? false : m_Pathogen->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetPathogen()
{
  if (m_Pathogen == nullptr)
    m_Pathogen = new SEScalar();
  return *m_Pathogen;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetPathogen() const
{
  if (m_Pathogen == nullptr)
    return SEScalar::dNaN();
  return m_Pathogen->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasTrauma() const
{
  return m_Trauma == nullptr ? false : m_Trauma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetTrauma()
{
  if (m_Trauma == nullptr)
    m_Trauma = new SEScalar();
  return *m_Trauma;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetTrauma() const
{
  if (m_Trauma == nullptr)
    return SEScalar::dNaN();
  return m_Trauma->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasMacrophageResting() const
{
  return m_MacrophageResting == nullptr ? false : m_MacrophageResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetMacrophageResting()
{
  if (m_MacrophageResting == nullptr)
    m_MacrophageResting = new SEScalar();
  return *m_MacrophageResting;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetMacrophageResting() const
{
  if (m_MacrophageResting == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasMacrophageActive() const
{
  return m_MacrophageActive == nullptr ? false : m_MacrophageActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetMacrophageActive()
{
  if (m_MacrophageActive == nullptr)
    m_MacrophageActive = new SEScalar();
  return *m_MacrophageActive;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetMacrophageActive() const
{
  if (m_MacrophageActive == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasNeutrophilResting() const
{
  return m_NeutrophilResting == nullptr ? false : m_NeutrophilResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetNeutrophilResting()
{
  if (m_NeutrophilResting == nullptr)
    m_NeutrophilResting = new SEScalar();
  return *m_NeutrophilResting;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetNeutrophilResting() const
{
  if (m_NeutrophilResting == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasNeutrophilActive() const
{
  return m_NeutrophilActive == nullptr ? false : m_NeutrophilActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetNeutrophilActive()
{
  if (m_NeutrophilActive == nullptr)
    m_NeutrophilActive = new SEScalar();
  return *m_NeutrophilActive;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetNeutrophilActive() const
{
  if (m_NeutrophilActive == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInducibleNOSynthasePre() const
{
  return m_InducibleNOSynthasePre == nullptr ? false : m_InducibleNOSynthasePre->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetInducibleNOSynthasePre()
{
  if (m_InducibleNOSynthasePre == nullptr)
    m_InducibleNOSynthasePre = new SEScalar();
  return *m_InducibleNOSynthasePre;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetInducibleNOSynthasePre() const
{
  if (m_InducibleNOSynthasePre == nullptr)
    return SEScalar::dNaN();
  return m_InducibleNOSynthasePre->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInducibleNOSynthase() const
{
  return m_InducibleNOSynthase == nullptr ? false : m_InducibleNOSynthase->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetInducibleNOSynthase()
{
  if (m_InducibleNOSynthase == nullptr)
    m_InducibleNOSynthase = new SEScalar();
  return *m_InducibleNOSynthase;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetInducibleNOSynthase() const
{
  if (m_InducibleNOSynthase == nullptr)
    return SEScalar::dNaN();
  return m_InducibleNOSynthase->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasConstitutiveNOSynthase() const
{
  return m_ConstitutiveNOSynthase == nullptr ? false : m_ConstitutiveNOSynthase->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetConstitutiveNOSynthase()
{
  if (m_ConstitutiveNOSynthase == nullptr)
    m_ConstitutiveNOSynthase = new SEScalar();
  return *m_ConstitutiveNOSynthase;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetConstitutiveNOSynthase() const
{
  if (m_ConstitutiveNOSynthase == nullptr)
    return SEScalar::dNaN();
  return m_ConstitutiveNOSynthase->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasNitrate() const
{
  return m_Nitrate == nullptr ? false : m_Nitrate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetNitrate()
{
  if (m_Nitrate == nullptr)
    m_Nitrate = new SEScalar();
  return *m_Nitrate;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetNitrate() const
{
  if (m_Nitrate == nullptr)
    return SEScalar::dNaN();
  return m_Nitrate->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasNitricOxide() const
{
  return m_NitricOxide == nullptr ? false : m_NitricOxide->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetNitricOxide()
{
  if (m_NitricOxide == nullptr)
    m_NitricOxide = new SEScalar();
  return *m_NitricOxide;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetNitricOxide() const
{
  if (m_NitricOxide == nullptr)
    return SEScalar::dNaN();
  return m_NitricOxide->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasTumorNecrosisFactor() const
{
  return m_TumorNecrosisFactor == nullptr ? false : m_TumorNecrosisFactor->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetTumorNecrosisFactor()
{
  if (m_TumorNecrosisFactor == nullptr)
    m_TumorNecrosisFactor = new SEScalar();
  return *m_TumorNecrosisFactor;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetTumorNecrosisFactor() const
{
  if (m_TumorNecrosisFactor == nullptr)
    return SEScalar::dNaN();
  return m_TumorNecrosisFactor->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInterleukin6() const
{
  return m_Interleukin6 == nullptr ? false : m_Interleukin6->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetInterleukin6()
{
  if (m_Interleukin6 == nullptr)
    m_Interleukin6 = new SEScalar();
  return *m_Interleukin6;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetInterleukin6() const
{
  if (m_Interleukin6 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin6->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInterleukin10() const
{
  return m_Interleukin10 == nullptr ? false : m_Interleukin10->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetInterleukin10()
{
  if (m_Interleukin10 == nullptr)
    m_Interleukin10 = new SEScalar();
  return *m_Interleukin10;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetInterleukin10() const
{
  if (m_Interleukin10 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin10->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInterleukin12() const
{
  return m_Interleukin12 == nullptr ? false : m_Interleukin12->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetInterleukin12()
{
  if (m_Interleukin12 == nullptr)
    m_Interleukin12 = new SEScalar();
  return *m_Interleukin12;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetInterleukin12() const
{
  if (m_Interleukin12 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin12->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasCatecholamines() const
{
  return m_Catecholamines == nullptr ? false : m_Catecholamines->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammationState::GetCatecholamines()
{
  if (m_Catecholamines == nullptr)
    m_Catecholamines = new SEScalar();
  return *m_Catecholamines;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetCatecholamines() const
{
  if (m_Catecholamines == nullptr)
    return SEScalar::dNaN();
  return m_Catecholamines->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasTissueIntegrity() const
{
  return m_TissueIntegrity == nullptr ? false : m_TissueIntegrity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEInflammationState::GetTissueIntegrity()
{
  if (m_TissueIntegrity == nullptr)
    m_TissueIntegrity = new SEScalar0To1();
  return *m_TissueIntegrity;
}
//-------------------------------------------------------------------------------
double SEInflammationState::GetTissueIntegrity() const
{
  if (m_TissueIntegrity == nullptr)
    return SEScalar::dNaN();
  return m_TissueIntegrity->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammationState::HasInflammationSources() const
{
  return !m_InflammationSources.empty();
}
//-------------------------------------------------------------------------------
std::vector<CDM::enumInflammationSource>& SEInflammationState::GetInflammationSources()
{
  return m_InflammationSources;
}
//-------------------------------------------------------------------------------
}
