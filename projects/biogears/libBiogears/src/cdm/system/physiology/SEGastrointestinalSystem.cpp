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
#include <biogears/cdm/system/physiology/SEGastrointestinalSystem.h>

#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>
#include <biogears/schema/cdm/Properties.hxx>

namespace std{
template class map<const biogears::SESubstance*, biogears::SEDrugTransitState*>;
}

namespace biogears {
constexpr char idChymeAbsorptionRate[] = "ChymeAbsorptionRate";
constexpr char idStomachContents[] = "StomachContents";

SEGastrointestinalSystem::SEGastrointestinalSystem(Logger* logger)
  : SESystem(logger)
{
  m_ChymeAbsorptionRate = nullptr;
  m_StomachContents = nullptr;
}
//-------------------------------------------------------------------------------

SEGastrointestinalSystem::~SEGastrointestinalSystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEGastrointestinalSystem::Clear()
{
  SESystem::Clear();
  SAFE_DELETE(m_ChymeAbsorptionRate);
  SAFE_DELETE(m_StomachContents);
}
//-------------------------------------------------------------------------------
const SEScalar* SEGastrointestinalSystem::GetScalar(const char* name)
{
  return GetScalar(std::string{ name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEGastrointestinalSystem::GetScalar(const std::string& name)
{
  if (name == idChymeAbsorptionRate)
    return &GetChymeAbsorptionRate();

  size_t split = name.find('-');
  if (split != name.npos) {
    std::string child = name.substr(0, split);
    std::string prop = name.substr(split + 1, name.npos);
    if (child == idStomachContents)
      return GetStomachContents().GetScalar(prop);
  }
  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::Load(const CDM::GastrointestinalSystemData& in)
{
  SESystem::Load(in);
  if (in.ChymeAbsorptionRate().present())
    GetChymeAbsorptionRate().Load(in.ChymeAbsorptionRate().get());
  if (in.StomachContents().present())
    GetStomachContents().Load(in.StomachContents().get());

  return true;
}
//-------------------------------------------------------------------------------

CDM::GastrointestinalSystemData* SEGastrointestinalSystem::Unload() const
{
  CDM::GastrointestinalSystemData* data = new CDM::GastrointestinalSystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEGastrointestinalSystem::Unload(CDM::GastrointestinalSystemData& data) const
{
  SESystem::Unload(data);
  if (m_ChymeAbsorptionRate != nullptr)
    data.ChymeAbsorptionRate(std::unique_ptr<CDM::ScalarVolumePerTimeData>(m_ChymeAbsorptionRate->Unload()));
  if (m_StomachContents != nullptr)
    data.StomachContents(std::unique_ptr<CDM::NutritionData>(m_StomachContents->Unload()));
  for (auto itr : m_DrugTransitStates) {
    if (itr.second != nullptr)
      data.DrugTransitStates().push_back(std::unique_ptr<CDM::DrugTransitStateData>(itr.second->Unload()));
  }
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::HasChymeAbsorptionRate() const
{
  return m_ChymeAbsorptionRate == nullptr ? false : m_ChymeAbsorptionRate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolumePerTime& SEGastrointestinalSystem::GetChymeAbsorptionRate()
{
  if (m_ChymeAbsorptionRate == nullptr)
    m_ChymeAbsorptionRate = new SEScalarVolumePerTime();
  return *m_ChymeAbsorptionRate;
}
//-------------------------------------------------------------------------------
double SEGastrointestinalSystem::GetChymeAbsorptionRate(const VolumePerTimeUnit& unit) const
{
  if (m_ChymeAbsorptionRate == nullptr)
    return SEScalar::dNaN();
  return m_ChymeAbsorptionRate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEGastrointestinalSystem::HasStomachContents() const
{
  return m_StomachContents == nullptr ? false : true;
}
//-------------------------------------------------------------------------------
SENutrition& SEGastrointestinalSystem::GetStomachContents()
{
  if (m_StomachContents == nullptr)
    m_StomachContents = new SENutrition(GetLogger());
  return *m_StomachContents;
}
//-------------------------------------------------------------------------------
const SENutrition* SEGastrointestinalSystem::GetStomachContents() const
{
  return m_StomachContents;
}
//-------------------------------------------------------------------------------
void SEGastrointestinalSystem::RemoveStomachContents()
{
  SAFE_DELETE(m_StomachContents);
}
//-------------------------------------------------------------------------------
Tree<const char*> SEGastrointestinalSystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*>{ classname() }
    .emplace_back(idChymeAbsorptionRate)
    .emplace_back(idStomachContents);
}

std::map<const SESubstance*, SEDrugTransitState*> SEGastrointestinalSystem::GetDrugTransitStates()
{
  return m_DrugTransitStates;
}

SEDrugTransitState* SEGastrointestinalSystem::GetDrugTransitState(const SESubstance* sub)
{
  return m_DrugTransitStates[sub];
}

SEDrugTransitState* SEGastrointestinalSystem::NewDrugTransitState(const SESubstance* sub)
{
  SEDrugTransitState* catState = new SEDrugTransitState(*sub);
  m_DrugTransitStates[sub] = catState;
  return catState;
}

SEDrugTransitState::SEDrugTransitState(const SESubstance& sub)
  : m_Substance(&sub)
{
  m_NumTransitMasses = 9; //Hard-coded for current model implementation.  This value includes stomach + 8 small intestine compartments
  m_TotalMassExcreted = nullptr;
  m_TotalMassMetabolized = nullptr;
}

SEDrugTransitState::~SEDrugTransitState()
{
  Clear();
}

void SEDrugTransitState::Clear()
{
  m_LumenSolidMasses.clear();
  m_LumenDissolvedMasses.clear();
  m_EnterocyteMasses.clear();
  SAFE_DELETE(m_TotalMassExcreted);
  SAFE_DELETE(m_TotalMassMetabolized);
}

bool SEDrugTransitState::Load(const CDM::DrugTransitStateData& in)
{
  m_LumenDissolvedMasses.clear();
  for (auto disMass : in.LumenDissolvedMasses()) {
    SEScalarMass* dMass = new SEScalarMass;
    dMass->Load(disMass);
    m_LumenDissolvedMasses.push_back(dMass);
  }
  m_LumenSolidMasses.clear();
  for (auto solMass : in.LumenSolidMasses()) {
    SEScalarMass* sMass = new SEScalarMass;
    sMass->Load(solMass);
    m_LumenSolidMasses.push_back(sMass);
  }
  m_EnterocyteMasses.clear();
  for (auto entMass : in.EnterocyteMasses()) {
    SEScalarMass* eMass = new SEScalarMass;
    eMass->Load(entMass);
    m_EnterocyteMasses.push_back(eMass);
  }
  GetTotalMassExcreted().Load(in.MassExcreted());
  GetTotalMassMetabolized().Load(in.MassMetabolized());
  return true;
}

CDM::DrugTransitStateData* SEDrugTransitState::Unload() const
{
  CDM::DrugTransitStateData* data = new CDM::DrugTransitStateData();
  Unload(*data);
  return data;
}


void SEDrugTransitState::Unload(CDM::DrugTransitStateData& data) const 
{
    for (auto tdMass : m_LumenDissolvedMasses) {
    data.LumenDissolvedMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(tdMass->Unload()));
    }
    for (auto tsMass : m_LumenSolidMasses) {
    data.LumenSolidMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(tsMass->Unload()));
    }
    for (auto eMass : m_EnterocyteMasses) {
    data.EnterocyteMasses().push_back(std::unique_ptr<CDM::ScalarMassData>(eMass->Unload()));
    }
    data.MassMetabolized(std::unique_ptr<CDM::ScalarMassData>(m_TotalMassMetabolized->Unload()));
    data.MassExcreted(std::unique_ptr<CDM::ScalarMassData>(m_TotalMassExcreted->Unload()));
    data.Substance(m_Substance->GetName());
}

bool SEDrugTransitState::Initialize(SEScalarMass& dose, CDM::enumOralAdministration::value route)
{
    std::vector<double> zeroMassVec(m_NumTransitMasses); //All zeros, correct number of elements
    bool trSolSet = SetLumenSolidMasses(zeroMassVec, MassUnit::ug);
    if (route == CDM::enumOralAdministration::Gastrointestinal) {
      m_LumenSolidMasses[0]->Set(dose); //If pill swallowed, put all the mass as solid in to stomach at initialization
    } 
	//If route is transmucosal, we leave everything at 0 because we assume that no drug has been dissolved in saliva and swallowed at first time step
    bool trDisSet = SetLumenDissolvedMasses(zeroMassVec, MassUnit::ug);
	zeroMassVec.pop_back(); //enterocyte vector has one fewer element because there is not stomach component
    bool entSet = SetEnterocyteMasses(zeroMassVec, MassUnit::ug);
	GetTotalMassExcreted().SetValue(0.0, MassUnit::ug);
    GetTotalMassMetabolized().SetValue(0.0, MassUnit::ug);
  
    return (trDisSet && trSolSet && entSet);
}

SEScalarMass& SEDrugTransitState::GetTotalMassExcreted()
{
  if (m_TotalMassExcreted == nullptr)
    m_TotalMassExcreted = new SEScalarMass();
  return *m_TotalMassExcreted;
}

SEScalarMass& SEDrugTransitState::GetTotalMassMetabolized()
{
  if (m_TotalMassMetabolized == nullptr)
    m_TotalMassMetabolized = new SEScalarMass();
  return *m_TotalMassMetabolized;
}

std::vector<double> SEDrugTransitState::GetLumenDissolvedMasses(const MassUnit& unit)
{
   std::vector<double> transitMasses;
   for (auto tData : m_LumenDissolvedMasses) {
      transitMasses.push_back(tData->GetValue(unit));
   }
   return transitMasses;
}
bool SEDrugTransitState::SetLumenDissolvedMasses(std::vector<double>& tMasses, const MassUnit& unit)
{
	if (tMasses.size() != m_NumTransitMasses) {
		//Don't execute if the input vector is not the correct size
		return false;
	}
	if (m_LumenDissolvedMasses.empty()) {
		//The very first time we call this function, the member vector will be empty.  All values should initialize to 0
		for (size_t pos = 0; pos < m_NumTransitMasses; pos++) {
		SEScalarMass* gtMass = new SEScalarMass();
		gtMass->SetValue(tMasses[pos], unit);
		m_LumenDissolvedMasses.push_back(gtMass);
		}
	} else {
		//Vectors should be the same size, so we can iterate over them
		std::vector<double>::iterator dblIt;
		std::vector<SEScalarMass*>::iterator scIt;
		for (dblIt = tMasses.begin(), scIt = m_LumenDissolvedMasses.begin(); dblIt != tMasses.end() && scIt != m_LumenDissolvedMasses.end(); ++dblIt, ++scIt) {
		(*scIt)->SetValue(*dblIt, unit);
		}
	}
	return true;
}

std::vector<double> SEDrugTransitState::GetLumenSolidMasses(const MassUnit& unit)
{
	std::vector<double> transitMasses;
	for (auto tData : m_LumenSolidMasses) {
		transitMasses.push_back(tData->GetValue(unit));
	}
	return transitMasses;
}

bool SEDrugTransitState::SetLumenSolidMasses(std::vector<double>& tMasses, const MassUnit& unit)
{
	if (tMasses.size() != m_NumTransitMasses) {
		//Don't execute if the input vector is not the correct size
		return false;
	}
	if (m_LumenSolidMasses.empty()) {
		//The very first time we call this function, the member vector will be empty.  All values should initialize to 0
		for (size_t pos = 0; pos < m_NumTransitMasses; pos++) {
		SEScalarMass* gtMass = new SEScalarMass();
		gtMass->SetValue(tMasses[pos], unit);
		m_LumenSolidMasses.push_back(gtMass);
		}
	} else {
		//Vectors should be the same size, so we can iterate over them
		std::vector<double>::iterator dblIt;
		std::vector<SEScalarMass*>::iterator scIt;
		for (dblIt = tMasses.begin(), scIt = m_LumenSolidMasses.begin(); dblIt != tMasses.end() && scIt != m_LumenSolidMasses.end(); ++dblIt, ++scIt) {
		(*scIt)->SetValue(*dblIt, unit);
		}
	}
	return true;
}

std::vector<double> SEDrugTransitState::GetEnterocyteMasses(const MassUnit& unit)
{
	std::vector<double> EnterocyteMasses;
	for (auto eData : m_EnterocyteMasses) {
		EnterocyteMasses.push_back(eData->GetValue(unit));
	}
	return EnterocyteMasses;
}

bool SEDrugTransitState::SetEnterocyteMasses(std::vector<double>& entMasses, const MassUnit& unit)
	{
	if (entMasses.size() != m_NumTransitMasses-1) {
		//Don't execute if the input vector is not the correct size--recall that enterocyte vector is one element shorter than the transit vectors
		return false;
	}
	if (m_EnterocyteMasses.empty()) {
		//The very first time we call this function, the member vector will be empty.  All values should initialize to 0
		for (size_t pos = 0; pos < m_NumTransitMasses-1; pos++) {
		SEScalarMass* entMass = new SEScalarMass();
		entMass->SetValue(entMasses[pos], unit);
		m_EnterocyteMasses.push_back(entMass);
		}
	} else {
		//Vectors should be the same size, so we can iterate over them
		std::vector<double>::iterator dblIt;
		std::vector<SEScalarMass*>::iterator scIt;
		for (dblIt = entMasses.begin(), scIt = m_EnterocyteMasses.begin(); dblIt != entMasses.end() && scIt != m_EnterocyteMasses.end(); ++dblIt, ++scIt) {
		(*scIt)->SetValue(*dblIt, unit);
		}
	}
	return true;
}

void SEDrugTransitState::IncrementStomachDissolvedMass(double value, const MassUnit& unit)
{
  m_LumenDissolvedMasses[0]->IncrementValue(value, unit);
}

void SEDrugTransitState::IncrementStomachSolidMass(double value, const MassUnit& unit)
{
  m_LumenSolidMasses[0]->IncrementValue(value, unit);
}

double SEDrugTransitState::GetTotalSolidMassInLumen(const MassUnit& unit)
{
  double totalMass = 0.0;
  for (auto itr : m_LumenSolidMasses) {
    totalMass += itr->GetValue(unit);
  }
  return totalMass;
}
double SEDrugTransitState::GetTotalDissolvedMassInLumen(const MassUnit& unit)
{
  double totalMass = 0.0;
  for (auto itr : m_LumenDissolvedMasses) {
    totalMass += itr->GetValue(unit);
  }
  return totalMass;
}
double SEDrugTransitState::GetTotalMassInEnterocytes(const MassUnit& unit)
{
  double totalMass = 0.0;
  for (auto itr : m_EnterocyteMasses) {
    totalMass += itr->GetValue(unit);
  }
  return totalMass;
}

};
