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
  : m_BloodResponse(nullptr)
  , m_TissueResponse(nullptr)
  , m_TissueIntegrity(nullptr)
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
  SAFE_DELETE(m_BloodResponse);
  SAFE_DELETE(m_TissueResponse);
  SAFE_DELETE(m_TissueIntegrity);
  m_ImmuneResponses.clear();
  m_InflammationSources.clear();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::Load(const CDM::InflammatoryResponseData& in)
{
  GetBloodResponse().Load(in.BloodResponse());
  GetTissueResponse().Load(in.TissueResponse());
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
  data.BloodResponse(std::unique_ptr<CDM::ImmuneCompartmentData>(m_BloodResponse->Unload()));
  data.TissueResponse(std::unique_ptr<CDM::ImmuneCompartmentData>(m_TissueResponse->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  for (auto src : m_InflammationSources) {
    data.Source().push_back(src);
  }
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::Initialize()
{
  //Values from Chow2005Acute
  GetTissueResponse().Initialize();
  GetBloodResponse().Initialize();
  GetTissueIntegrity().SetValue(1.0);
  m_ImmuneResponses.push_back(std::make_pair("Blood", m_BloodResponse));
  m_ImmuneResponses.push_back(std::make_pair("Tissue", m_TissueResponse));
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::IsValid()
{
  if (HasTissueIntegrity() && HasTissueResponse() && HasBloodResponse())
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

  if (name.compare("TissueIntegrity") == 0)
    return &GetTissueIntegrity();
  size_t split = name.find('-');
  if (split != name.npos) {
    std::string prop = name.substr(split + 1, name.npos);
    std::string parent = name.substr(0, split);
    if (parent.compare("BloodResponse") == 0)
      return GetBloodResponse().GetScalar(prop);
    if (parent.compare("TissueResponse") == 0)
      return GetTissueResponse().GetScalar(prop);
  }

  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasImmuneResponses() const
{
  return m_ImmuneResponses.size() != 2 ? false : true;
}
//-------------------------------------------------------------------------------
std::vector<std::pair<std::string, SEImmuneCompartment*>> SEInflammatoryResponse::GetImmuneResponses()
{
  return m_ImmuneResponses;
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::SetImmuneResponses()
{
  m_ImmuneResponses.clear();
  m_ImmuneResponses.push_back(std::make_pair("Blood", m_BloodResponse));
  m_ImmuneResponses.push_back(std::make_pair("Tissue", m_TissueResponse));
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
bool SEInflammatoryResponse::HasBloodResponse() const
{
  return m_BloodResponse == nullptr ? false : m_BloodResponse->IsValid();
}
//-------------------------------------------------------------------------------
SEImmuneCompartment& SEInflammatoryResponse::GetBloodResponse()
{
  if (m_BloodResponse == nullptr)
    m_BloodResponse = new SEImmuneCompartment();
  return *m_BloodResponse;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTissueResponse() const
{
  return m_TissueResponse == nullptr ? false : m_TissueResponse->IsValid();
}
//-------------------------------------------------------------------------------
SEImmuneCompartment& SEInflammatoryResponse::GetTissueResponse()
{
  if (m_TissueResponse == nullptr)
    m_TissueResponse = new SEImmuneCompartment();
  return *m_TissueResponse;
}
//-------------------------------------------------------------------------------

/////////////////////////////////--SEImmuneCompartment--////////////////////////////////////////
SEImmuneCompartment::SEImmuneCompartment()
  : m_ConstitutiveNOS(nullptr)
  , m_EpitheliumResting(nullptr)
  , m_EpitheliumActive(nullptr)
  , m_InducibleNOSPre(nullptr)
  , m_InducibleNOS(nullptr)
  , m_Interleukin6(nullptr)
  , m_Interleukin10(nullptr)
  , m_Interleukin12(nullptr)
  , m_MacrophageResting(nullptr)
  , m_MacrophageActive(nullptr)
  , m_NeutrophilResting(nullptr)
  , m_NeutrophilActive(nullptr)
  , m_Nitrate(nullptr)
  , m_NitricOxide(nullptr)
  , m_Pathogen(nullptr)
  , m_Trauma(nullptr)
  , m_TumorNecrosisFactor(nullptr)
{
}

//-------------------------------------------------------------------------------
SEImmuneCompartment::~SEImmuneCompartment()
{
  Clear();
}
//-------------------------------------------------------------------------------
void SEImmuneCompartment::Clear()
{
  SAFE_DELETE(m_ConstitutiveNOS);
  SAFE_DELETE(m_EpitheliumResting);
  SAFE_DELETE(m_EpitheliumActive);
  SAFE_DELETE(m_InducibleNOSPre);
  SAFE_DELETE(m_InducibleNOS);
  SAFE_DELETE(m_Interleukin6);
  SAFE_DELETE(m_Interleukin10);
  SAFE_DELETE(m_Interleukin12);
  SAFE_DELETE(m_MacrophageActive);
  SAFE_DELETE(m_MacrophageResting);
  SAFE_DELETE(m_NeutrophilResting);
  SAFE_DELETE(m_NeutrophilActive);
  SAFE_DELETE(m_Nitrate);
  SAFE_DELETE(m_NitricOxide);
  SAFE_DELETE(m_Pathogen);
  SAFE_DELETE(m_Trauma);
  SAFE_DELETE(m_TumorNecrosisFactor);
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::Load(const CDM::ImmuneCompartmentData& in)
{
  GetConstitutiveNOS().Load(in.ConstitutiveNOS());
  GetEpitheliumResting().Load(in.EpitheliumResting());
  GetEpitheliumActive().Load(in.EpitheliumActive());
  GetInducibleNOSPre().Load(in.InducibleNOSPre());
  GetInducibleNOS().Load(in.InducibleNOS());
  GetInterleukin6().Load(in.Interleukin6());
  GetInterleukin10().Load(in.Interleukin10());
  GetInterleukin12().Load(in.Interleukin12());
  GetMacrophageResting().Load(in.MacrophageResting());
  GetMacrophageActive().Load(in.MacrophageActive());
  GetNeutrophilResting().Load(in.NeutrophilResting());
  GetNeutrophilActive().Load(in.NeutrophilActive());
  GetNitrate().Load(in.Nitrate());
  GetNitricOxide().Load(in.NitricOxide());
  GetPathogen().Load(in.Pathogen());
  GetTrauma().Load(in.Trauma());
  GetTumorNecrosisFactor().Load(in.TumorNecrosisFactor());

  return true;
}
//-------------------------------------------------------------------------------
CDM::ImmuneCompartmentData* SEImmuneCompartment::Unload() const
{
  CDM::ImmuneCompartmentData* data = new CDM::ImmuneCompartmentData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------
void SEImmuneCompartment::Unload(CDM::ImmuneCompartmentData& data) const
{
  data.ConstitutiveNOS(std::unique_ptr<CDM::ScalarData>(m_ConstitutiveNOS->Unload()));
  data.EpitheliumResting(std::unique_ptr<CDM::ScalarData>(m_EpitheliumResting->Unload()));
  data.EpitheliumActive(std::unique_ptr<CDM::ScalarData>(m_EpitheliumActive->Unload()));
  data.InducibleNOSPre(std::unique_ptr<CDM::ScalarData>(m_InducibleNOSPre->Unload()));
  data.InducibleNOS(std::unique_ptr<CDM::ScalarData>(m_InducibleNOS->Unload()));
  data.Interleukin6(std::unique_ptr<CDM::ScalarData>(m_Interleukin6->Unload()));
  data.Interleukin10(std::unique_ptr<CDM::ScalarData>(m_Interleukin10->Unload()));
  data.Interleukin12(std::unique_ptr<CDM::ScalarData>(m_Interleukin12->Unload()));
  data.MacrophageResting(std::unique_ptr<CDM::ScalarData>(m_MacrophageResting->Unload()));
  data.MacrophageActive(std::unique_ptr<CDM::ScalarData>(m_MacrophageActive->Unload()));
  data.NeutrophilResting(std::unique_ptr<CDM::ScalarData>(m_NeutrophilResting->Unload()));
  data.NeutrophilActive(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActive->Unload()));
  data.Nitrate(std::unique_ptr<CDM::ScalarData>(m_Nitrate->Unload()));
  data.NitricOxide(std::unique_ptr<CDM::ScalarData>(m_NitricOxide->Unload()));
  data.Pathogen(std::unique_ptr<CDM::ScalarData>(m_Pathogen->Unload()));
  data.Trauma(std::unique_ptr<CDM::ScalarData>(m_Trauma->Unload()));
  data.TumorNecrosisFactor(std::unique_ptr<CDM::ScalarData>(m_TumorNecrosisFactor->Unload()));
}
//-------------------------------------------------------------------------------
void SEImmuneCompartment::Initialize()
{
  GetConstitutiveNOS().SetValue(0.05);
  GetEpitheliumResting().SetValue(1.0);
  GetEpitheliumActive().SetValue(0.0);
  GetInducibleNOSPre().SetValue(0.0);
  GetInducibleNOS().SetValue(0.0);
  GetInterleukin6().SetValue(0.001);
  GetInterleukin10().SetValue(0.01);
  GetInterleukin12().SetValue(0.0);
  GetMacrophageResting().SetValue(1.0);
  GetMacrophageActive().SetValue(0.0);
  GetNeutrophilResting().SetValue(1.0);
  GetNeutrophilActive().SetValue(0.0);
  GetNitrate().SetValue(0.0);
  GetNitricOxide().SetValue(0.0);
  GetPathogen().SetValue(0.0);
  GetTrauma().SetValue(0.0);
  GetTumorNecrosisFactor().SetValue(0.0);
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::IsValid()
{
  if (HasConstitutiveNOS() && HasEpitheliumResting() && HasEpitheliumActive() && HasInducibleNOSPre() && HasInducibleNOS() && HasInterleukin6() && HasInterleukin10() && HasInterleukin12() && HasMacrophageResting() && HasMacrophageActive() && HasNeutrophilResting() && HasNeutrophilActive() && HasNitrate() && HasNitricOxide() && HasPathogen() && HasTrauma() && HasTumorNecrosisFactor())
    return true;
  else
    return false;
}
//-------------------------------------------------------------------------------
const SEScalar* SEImmuneCompartment::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEImmuneCompartment::GetScalar(const std::string& name)
{
  if (name.compare("ConstitutiveNOS") == 0)
    return &GetConstitutiveNOS();
  if (name.compare("EpitheliumResting") == 0)
    return &GetEpitheliumResting();
  if (name.compare("EpitheliumActive") == 0)
    return &GetEpitheliumActive();
  if (name.compare("InducibleNOSPre") == 0)
    return &GetInducibleNOSPre();
  if (name.compare("InducibleNOS") == 0)
    return &GetInducibleNOS();
  if (name.compare("Interleukin6") == 0)
    return &GetInterleukin6();
  if (name.compare("Interleukin10") == 0)
    return &GetInterleukin10();
  if (name.compare("Interleukin12") == 0)
    return &GetInterleukin12();
  if (name.compare("MacrophageResting") == 0)
    return &GetMacrophageResting();
  if (name.compare("MacrophageActive") == 0)
    return &GetMacrophageActive();
  if (name.compare("NeutrophilResting") == 0)
    return &GetNeutrophilResting();
  if (name.compare("NeutrophilActive") == 0)
    return &GetNeutrophilActive();
  if (name.compare("Nitrate") == 0)
    return &GetNitrate();
  if (name.compare("NitricOxide") == 0)
    return &GetNitricOxide();
  if (name.compare("Pathogen") == 0)
    return &GetPathogen();
  if (name.compare("Trauma") == 0)
    return &GetTrauma();
  if (name.compare("TumorNecrosisFactor") == 0)
    return &GetTumorNecrosisFactor();

  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasConstitutiveNOS() const
{
  return m_ConstitutiveNOS == nullptr ? false : m_ConstitutiveNOS->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetConstitutiveNOS()
{
  if (m_ConstitutiveNOS == nullptr)
    m_ConstitutiveNOS = new SEScalar();
  return *m_ConstitutiveNOS;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetConstitutiveNOS() const
{
  if (m_ConstitutiveNOS == nullptr)
    return SEScalar::dNaN();
  return m_ConstitutiveNOS->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasEpitheliumResting() const
{
  return m_EpitheliumResting == nullptr ? false : m_EpitheliumResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetEpitheliumResting()
{
  if (m_EpitheliumResting == nullptr)
    m_EpitheliumResting = new SEScalar();
  return *m_EpitheliumResting;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetEpitheliumResting() const
{
  if (m_EpitheliumResting == nullptr)
    return SEScalar::dNaN();
  return m_EpitheliumResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasEpitheliumActive() const
{
  return m_EpitheliumActive == nullptr ? false : m_EpitheliumActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetEpitheliumActive()
{
  if (m_EpitheliumActive == nullptr)
    m_EpitheliumActive = new SEScalar();
  return *m_EpitheliumActive;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetEpitheliumActive() const
{
  if (m_EpitheliumActive == nullptr)
    return SEScalar::dNaN();
  return m_EpitheliumActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasInducibleNOSPre() const
{
  return m_InducibleNOSPre == nullptr ? false : m_InducibleNOSPre->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetInducibleNOSPre()
{
  if (m_InducibleNOSPre == nullptr)
    m_InducibleNOSPre = new SEScalar();
  return *m_InducibleNOSPre;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetInducibleNOSPre() const
{
  if (m_InducibleNOSPre == nullptr)
    return SEScalar::dNaN();
  return m_InducibleNOSPre->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasInducibleNOS() const
{
  return m_InducibleNOS == nullptr ? false : m_InducibleNOS->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetInducibleNOS()
{
  if (m_InducibleNOS == nullptr)
    m_InducibleNOS = new SEScalar();
  return *m_InducibleNOS;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetInducibleNOS() const
{
  if (m_InducibleNOS == nullptr)
    return SEScalar::dNaN();
  return m_InducibleNOS->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasInterleukin6() const
{
  return m_Interleukin6 == nullptr ? false : m_Interleukin6->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetInterleukin6()
{
  if (m_Interleukin6 == nullptr)
    m_Interleukin6 = new SEScalar();
  return *m_Interleukin6;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetInterleukin6() const
{
  if (m_Interleukin6 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin6->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasInterleukin10() const
{
  return m_Interleukin10 == nullptr ? false : m_Interleukin10->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetInterleukin10()
{
  if (m_Interleukin10 == nullptr)
    m_Interleukin10 = new SEScalar();
  return *m_Interleukin10;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetInterleukin10() const
{
  if (m_Interleukin10 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin10->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasInterleukin12() const
{
  return m_Interleukin12 == nullptr ? false : m_Interleukin12->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetInterleukin12()
{
  if (m_Interleukin12 == nullptr)
    m_Interleukin12 = new SEScalar();
  return *m_Interleukin12;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetInterleukin12() const
{
  if (m_Interleukin12 == nullptr)
    return SEScalar::dNaN();
  return m_Interleukin12->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasMacrophageResting() const
{
  return m_MacrophageResting == nullptr ? false : m_MacrophageResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetMacrophageResting()
{
  if (m_MacrophageResting == nullptr)
    m_MacrophageResting = new SEScalar();
  return *m_MacrophageResting;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetMacrophageResting() const
{
  if (m_MacrophageResting == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasMacrophageActive() const
{
  return m_MacrophageActive == nullptr ? false : m_MacrophageActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetMacrophageActive()
{
  if (m_MacrophageActive == nullptr)
    m_MacrophageActive = new SEScalar();
  return *m_MacrophageActive;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetMacrophageActive() const
{
  if (m_MacrophageActive == nullptr)
    return SEScalar::dNaN();
  return m_MacrophageActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasNeutrophilResting() const
{
  return m_NeutrophilResting == nullptr ? false : m_NeutrophilResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetNeutrophilResting()
{
  if (m_NeutrophilResting == nullptr)
    m_NeutrophilResting = new SEScalar();
  return *m_NeutrophilResting;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetNeutrophilResting() const
{
  if (m_NeutrophilResting == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasNeutrophilActive() const
{
  return m_NeutrophilActive == nullptr ? false : m_NeutrophilActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetNeutrophilActive()
{
  if (m_NeutrophilActive == nullptr)
    m_NeutrophilActive = new SEScalar();
  return *m_NeutrophilActive;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetNeutrophilActive() const
{
  if (m_NeutrophilActive == nullptr)
    return SEScalar::dNaN();
  return m_NeutrophilActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasNitrate() const
{
  return m_Nitrate == nullptr ? false : m_Nitrate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetNitrate()
{
  if (m_Nitrate == nullptr)
    m_Nitrate = new SEScalar();
  return *m_Nitrate;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetNitrate() const
{
  if (m_Nitrate == nullptr)
    return SEScalar::dNaN();
  return m_Nitrate->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasNitricOxide() const
{
  return m_NitricOxide == nullptr ? false : m_NitricOxide->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetNitricOxide()
{
  if (m_NitricOxide == nullptr)
    m_NitricOxide = new SEScalar();
  return *m_NitricOxide;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetNitricOxide() const
{
  if (m_NitricOxide == nullptr)
    return SEScalar::dNaN();
  return m_NitricOxide->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasPathogen() const
{
  return m_Pathogen == nullptr ? false : m_Pathogen->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetPathogen()
{
  if (m_Pathogen == nullptr)
    m_Pathogen = new SEScalar();
  return *m_Pathogen;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetPathogen() const
{
  if (m_Pathogen == nullptr)
    return SEScalar::dNaN();
  return m_Pathogen->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasTrauma() const
{
  return m_Trauma == nullptr ? false : m_Trauma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetTrauma()
{
  if (m_Trauma == nullptr)
    m_Trauma = new SEScalar();
  return *m_Trauma;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetTrauma() const
{
  if (m_Trauma == nullptr)
    return SEScalar::dNaN();
  return m_Trauma->GetValue();
}
//-------------------------------------------------------------------------------
bool SEImmuneCompartment::HasTumorNecrosisFactor() const
{
  return m_TumorNecrosisFactor == nullptr ? false : m_TumorNecrosisFactor->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEImmuneCompartment::GetTumorNecrosisFactor()
{
  if (m_TumorNecrosisFactor == nullptr)
    m_TumorNecrosisFactor = new SEScalar();
  return *m_TumorNecrosisFactor;
}
//-------------------------------------------------------------------------------
double SEImmuneCompartment::GetTumorNecrosisFactor() const
{
  if (m_TumorNecrosisFactor == nullptr)
    return SEScalar::dNaN();
  return m_TumorNecrosisFactor->GetValue();
}
//-------------------------------------------------------------------------------
}
