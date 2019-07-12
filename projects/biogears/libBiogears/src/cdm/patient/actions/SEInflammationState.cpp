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
  for (auto src : in.Source()) {
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
  GetPathogen().SetValue(0.0); //Change this back to 0 after testing
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
  GetInterleukin6().SetValue(17.0); //Value is in pg/L-->gets scaled down in model
  GetInterleukin10().SetValue(80.0); //Value is in pg/L-->gets scaled down in model
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
//////////////////////////////////--SEInflammatoryResponse--//////////////////////////////////
SEInflammatoryResponse::SEInflammatoryResponse()
  : m_PathogenTissue(nullptr)
  , m_PathogenBlood(nullptr)
  , m_Trauma(nullptr)
  , m_BloodMediators(nullptr)
  , m_TissueMediators(nullptr)
  , m_TissueIntegrity(nullptr)
  , m_InflammationLevel(nullptr)
{
}
//-------------------------------------------------------------------------------
SEInflammatoryResponse::~SEInflammatoryResponse()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::Clear()
{
  SAFE_DELETE(m_PathogenTissue);
  SAFE_DELETE(m_PathogenBlood);
  SAFE_DELETE(m_Trauma);
  SAFE_DELETE(m_BloodMediators);
  SAFE_DELETE(m_TissueMediators);
  SAFE_DELETE(m_TissueIntegrity);
  SAFE_DELETE(m_InflammationLevel);
  m_InflammationSources.clear();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::Load(const CDM::InflammatoryResponseData& in)
{
  GetPathogenTissue().Load(in.PathogenTissue());
  GetPathogenBlood().Load(in.PathogenBlood());
  GetTrauma().Load(in.Trauma());
  GetBloodMediators().Load(in.BloodMediators());
  GetTissueMediators().Load(in.TissueMediators());
  GetInflammationLevel().Load(in.InflammationLevel());
  GetTissueIntegrity().Load(in.TissueIntegrity());
  for (auto src : in.Source()) {
    m_InflammationSources.push_back(src);
  }
  return true;
}
//-------------------------------------------------------------------------------
CDM::InflammatoryResponseData* SEInflammatoryResponse::Unload() const
{
  CDM::InflammatoryResponseData* data = new CDM::InflammatoryResponseData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::Unload(CDM::InflammatoryResponseData& data) const
{
  data.PathogenTissue(std::unique_ptr<CDM::ScalarData>(m_PathogenTissue->Unload()));
  data.PathogenBlood(std::unique_ptr<CDM::ScalarData>(m_PathogenBlood->Unload()));
  data.Trauma(std::unique_ptr<CDM::ScalarData>(m_Trauma->Unload()));
  data.BloodMediators(std::unique_ptr<CDM::ImmuneMediatorData>(m_BloodMediators->Unload()));
  data.TissueMediators(std::unique_ptr<CDM::ImmuneMediatorData>(m_TissueMediators->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  data.InflammationLevel(std::unique_ptr<CDM::Scalar0To1Data>(m_InflammationLevel->Unload()));
  for (auto src : m_InflammationSources) {
    data.Source().push_back(src);
  }
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::Initialize()
{
  //Values from Chow2005Acute
  GetPathogenTissue().SetValue(0.0); //Change this back to 0 after testing
  GetPathogenBlood().SetValue(0.0);
  GetTrauma().SetValue(0.0);
  GetTissueMediators().Initialize(true);
  GetBloodMediators().Initialize(false);
  GetTissueIntegrity().SetValue(1.0);
  GetInflammationLevel().SetValue(0.0);
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::IsValid()
{
  if (HasPathogenTissue() && HasPathogenBlood() && HasTrauma() && HasTissueIntegrity() && HasInflammationLevel() && HasBloodMediators() && HasTissueMediators())
    return true;
  else
    return false;
}
//-------------------------------------------------------------------------------
const SEScalar* SEInflammatoryResponse::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEInflammatoryResponse::GetScalar(const std::string& name)
{
  if (name.compare("PathogenTissue") == 0)
    return &GetPathogenTissue();
  if (name.compare("PathogenBlood") == 0)
    return &GetPathogenBlood();
  if (name.compare("Trauma") == 0)
    return &GetTrauma();
  if (name.compare("TissueIntegrity") == 0)
    return &GetTissueIntegrity();
  if (name.compare("InflammationLevel") == 0)
    return &GetInflammationLevel();
  size_t split = name.find('-');
  if (split != name.npos) {
    std::string prop = name.substr(split + 1, name.npos);
    std::string parent = name.substr(0, split);
    if (parent.compare("BloodMediators") == 0)
      return GetBloodMediators().GetScalar(prop);
    if (parent.compare("TissueMediators") == 0)
      return GetTissueMediators().GetScalar(prop);
  }

  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasPathogenTissue() const
{
  return m_PathogenTissue == nullptr ? false : m_PathogenTissue->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetPathogenTissue()
{
  if (m_PathogenTissue == nullptr)
    m_PathogenTissue = new SEScalar();
  return *m_PathogenTissue;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetPathogenTissue() const
{
  if (m_PathogenTissue == nullptr)
    return SEScalar::dNaN();
  return m_PathogenTissue->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasPathogenBlood() const
{
  return m_PathogenBlood == nullptr ? false : m_PathogenBlood->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetPathogenBlood()
{
  if (m_PathogenBlood == nullptr)
    m_PathogenBlood = new SEScalar();
  return *m_PathogenBlood;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetPathogenBlood() const
{
  if (m_PathogenBlood == nullptr)
    return SEScalar::dNaN();
  return m_PathogenBlood->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTrauma() const
{
  return m_Trauma == nullptr ? false : m_Trauma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetTrauma()
{
  if (m_Trauma == nullptr)
    m_Trauma = new SEScalar();
  return *m_Trauma;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetTrauma() const
{
  if (m_Trauma == nullptr)
    return SEScalar::dNaN();
  return m_Trauma->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTissueIntegrity() const
{
  return m_TissueIntegrity == nullptr ? false : m_TissueIntegrity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEInflammatoryResponse::GetTissueIntegrity()
{
  if (m_TissueIntegrity == nullptr)
    m_TissueIntegrity = new SEScalar0To1();
  return *m_TissueIntegrity;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetTissueIntegrity() const
{
  if (m_TissueIntegrity == nullptr)
    return SEScalar::dNaN();
  return m_TissueIntegrity->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInflammationLevel() const
{
  return m_InflammationLevel == nullptr ? false : m_InflammationLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEInflammatoryResponse::GetInflammationLevel()
{
  if (m_InflammationLevel == nullptr)
    m_InflammationLevel = new SEScalar0To1();
  return *m_InflammationLevel;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInflammationLevel() const
{
  if (m_InflammationLevel == nullptr)
    return SEScalar::dNaN();
  return m_InflammationLevel->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInflammationSources() const
{
  return !m_InflammationSources.empty();
}
//-------------------------------------------------------------------------------
std::vector<CDM::enumInflammationSource>& SEInflammatoryResponse::GetInflammationSources()
{
  return m_InflammationSources;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasBloodMediators() const
{
  return m_BloodMediators == nullptr ? false : m_BloodMediators->IsValid();
}
//-------------------------------------------------------------------------------
SEImmuneMediators& SEInflammatoryResponse::GetBloodMediators()
{
  if (m_BloodMediators == nullptr)
    m_BloodMediators = new SEImmuneMediators();
  return *m_BloodMediators;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTissueMediators() const
{
  return m_TissueMediators == nullptr ? false : m_TissueMediators->IsValid();
}
//-------------------------------------------------------------------------------
SEImmuneMediators& SEInflammatoryResponse::GetTissueMediators()
{
  if (m_TissueMediators == nullptr)
    m_TissueMediators = new SEImmuneMediators();
  return *m_TissueMediators;
}
//-------------------------------------------------------------------------------

/////////////////////////////////--SEImmuneMediators--////////////////////////////////////////
SEImmuneMediators::SEImmuneMediators()
  : m_Antibodies(nullptr)
  , m_Interleukin10(nullptr)
  , m_MacrophageResting(nullptr)
  , m_MacrophageActive(nullptr)
  , m_NeutrophilResting(nullptr)
  , m_NeutrophilActive(nullptr)
  , m_NitricOxide(nullptr)
  , m_TumorNecrosisFactor(nullptr)
{
}

//-------------------------------------------------------------------------------
SEImmuneMediators::~SEImmuneMediators()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEImmuneMediators::Clear()
{
  SAFE_DELETE(m_Antibodies);
  SAFE_DELETE(m_Interleukin10);
  SAFE_DELETE(m_MacrophageActive);
  SAFE_DELETE(m_MacrophageResting);
  SAFE_DELETE(m_NeutrophilResting);
  SAFE_DELETE(m_NeutrophilActive);
  SAFE_DELETE(m_NitricOxide);
  SAFE_DELETE(m_TumorNecrosisFactor);
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::Load(const CDM::ImmuneMediatorData& in)
{
  GetAntibodies().Load(in.Antibodies());
  GetInterleukin10().Load(in.Interleukin10());
  GetMacrophageResting().Load(in.MacrophageResting());
  GetMacrophageActive().Load(in.MacrophageActive());
  GetNeutrophilResting().Load(in.NeutrophilResting());
  GetNeutrophilActive().Load(in.NeutrophilActive());
  GetNitricOxide().Load(in.NitricOxide());
  GetTumorNecrosisFactor().Load(in.TumorNecrosisFactor());

  return true;
}
//-------------------------------------------------------------------------------
CDM::ImmuneMediatorData* SEImmuneMediators::Unload() const
{
  CDM::ImmuneMediatorData* data = new CDM::ImmuneMediatorData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEImmuneMediators::Unload(CDM::ImmuneMediatorData& data) const
{
  data.Antibodies(std::unique_ptr<CDM::ScalarData> (m_Antibodies->Unload()));
  data.Interleukin10(std::unique_ptr<CDM::ScalarData>(m_Interleukin10->Unload()));
  data.MacrophageResting(std::unique_ptr<CDM::ScalarData>(m_MacrophageResting->Unload()));
  data.MacrophageActive(std::unique_ptr<CDM::ScalarData>(m_MacrophageActive->Unload()));
  data.NeutrophilResting(std::unique_ptr<CDM::ScalarData>(m_NeutrophilResting->Unload()));
  data.NeutrophilActive(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActive->Unload()));
  data.NitricOxide(std::unique_ptr<CDM::ScalarData>(m_NitricOxide->Unload()));
  data.TumorNecrosisFactor(std::unique_ptr<CDM::ScalarData>(m_TumorNecrosisFactor->Unload()));
}
//-------------------------------------------------------------------------------
void SEImmuneMediators::Initialize(bool TissueSpace)
{
  //Values from Reynolds2008Mathematical
  if (TissueSpace) {
    GetAntibodies().SetValue(0.0075 / 0.0023);		//Ratio of resting synthesis rate to resting decay rate
    GetMacrophageResting().SetValue(10.0 / 0.12); //Ratio of resting synthesis rate to resting decay rate
    GetMacrophageActive().SetValue(0.0);
    GetNeutrophilResting().SetValue(0.0);
    GetNeutrophilActive().SetValue(0.0);
    GetNitricOxide().SetValue(0.0);
    GetTumorNecrosisFactor().SetValue(0.0);
    GetInterleukin10().SetValue(0.0);
  } else {
    GetAntibodies().SetValue(0.0075 / 0.0023);    //Ratio of resting synthesis rate to resting decay rate
    GetMacrophageResting().SetValue(10.0 / 0.12); //Ratio of resting synthesis rate to resting decay rate
    GetMacrophageActive().SetValue(0.0);
    GetNeutrophilResting().SetValue(10.0 / 0.12); //Ratio of resting synthesis rate to resting decay rate
    GetNeutrophilActive().SetValue(0.0);
    GetNitricOxide().SetValue(0.0);
    GetTumorNecrosisFactor().SetValue(0.0);
    GetInterleukin10().SetValue(0.0);
  }
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::IsValid()
{
  if (HasAntibodies() && HasMacrophageResting() && HasMacrophageActive() && HasNeutrophilResting() && HasNeutrophilActive() && HasNitricOxide() && HasTumorNecrosisFactor() && HasInterleukin10())
    return true;
  else
    return false;
}
//-------------------------------------------------------------------------------
const SEScalar* SEImmuneMediators::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEImmuneMediators::GetScalar(const std::string& name)
{
  if (name.compare("Antibodies") == 0)
    return &GetAntibodies();
  if (name.compare("MacrophageResting") == 0)
    return &GetMacrophageResting();
  if (name.compare("MacrophageActive") == 0)
    return &GetMacrophageActive();
  if (name.compare("NeutrophilResting") == 0)
    return &GetNeutrophilResting();
  if (name.compare("NeutrophilActive") == 0)
    return &GetNeutrophilActive();
  if (name.compare("NitricOxide") == 0)
    return &GetNitricOxide();
  if (name.compare("TumorNecrosisFactor") == 0)
    return &GetTumorNecrosisFactor();
  if (name.compare("Interleukin10") == 0)
    return &GetInterleukin10();

  return nullptr;
}

//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasAntibodies() const
{
  return m_Antibodies == nullptr ? false : m_Antibodies->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetAntibodies()
{
  if (m_Antibodies == nullptr)
    m_Antibodies = new SEScalar();
  return *m_Antibodies;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetAntibodies() const
{
  if (m_Antibodies == nullptr)
    return SEScalar::dNaN();
  return m_Antibodies->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasInterleukin10() const
{
  return m_Interleukin10 == nullptr ? false : m_Interleukin10->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetInterleukin10()
{
  if (m_Interleukin10 == nullptr)
    m_Interleukin10 = new SEScalar();
  return *m_Interleukin10;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetInterleukin10() const
{
  if (m_Interleukin10 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin10->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasMacrophageResting() const
{
  return m_MacrophageResting == nullptr ? false : m_MacrophageResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetMacrophageResting()
{
  if (m_MacrophageResting == nullptr)
    m_MacrophageResting = new SEScalar();
  return *m_MacrophageResting;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetMacrophageResting() const
{
  if (m_MacrophageResting == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasMacrophageActive() const
{
  return m_MacrophageActive == nullptr ? false : m_MacrophageActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetMacrophageActive()
{
  if (m_MacrophageActive == nullptr)
    m_MacrophageActive = new SEScalar();
  return *m_MacrophageActive;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetMacrophageActive() const
{
  if (m_MacrophageActive == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasNeutrophilResting() const
{
  return m_NeutrophilResting == nullptr ? false : m_NeutrophilResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetNeutrophilResting()
{
  if (m_NeutrophilResting == nullptr)
    m_NeutrophilResting = new SEScalar();
  return *m_NeutrophilResting;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetNeutrophilResting() const
{
  if (m_NeutrophilResting == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasNeutrophilActive() const
{
  return m_NeutrophilActive == nullptr ? false : m_NeutrophilActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetNeutrophilActive()
{
  if (m_NeutrophilActive == nullptr)
    m_NeutrophilActive = new SEScalar();
  return *m_NeutrophilActive;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetNeutrophilActive() const
{
  if (m_NeutrophilActive == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasNitricOxide() const
{
  return m_NitricOxide == nullptr ? false : m_NitricOxide->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetNitricOxide()
{
  if (m_NitricOxide == nullptr)
    m_NitricOxide = new SEScalar();
  return *m_NitricOxide;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetNitricOxide() const
{
  if (m_NitricOxide == nullptr)
    return SEScalar::dNaN();
  return m_NitricOxide->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneMediators::HasTumorNecrosisFactor() const
{
  return m_TumorNecrosisFactor == nullptr ? false : m_TumorNecrosisFactor->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneMediators::GetTumorNecrosisFactor()
{
  if (m_TumorNecrosisFactor == nullptr)
    m_TumorNecrosisFactor = new SEScalar();
  return *m_TumorNecrosisFactor;
}
//-------------------------------------------------------------------------------
double SEImmuneMediators::GetTumorNecrosisFactor() const
{
  if (m_TumorNecrosisFactor == nullptr)
    return SEScalar::dNaN();
  return m_TumorNecrosisFactor->GetValue();
}
//-------------------------------------------------------------------------------
}
