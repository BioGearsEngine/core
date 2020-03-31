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
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalar0To1.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/container/Tree.tci.h>

namespace biogears {
constexpr char idArterialBloodPH[] = "ArterialBloodPH";
constexpr char idArterialBloodPHBaseline[] = "ArterialBloodPHBaseline";
constexpr char idBloodDensity[] = "BloodDensity";
constexpr char idBloodSpecificHeat[] = "BloodSpecificHeat";
constexpr char idBloodUreaNitrogenConcentration[] = "BloodUreaNitrogenConcentration";
constexpr char idCarbonDioxideSaturation[] = "CarbonDioxideSaturation";
constexpr char idCarbonMonoxideSaturation[] = "CarbonMonoxideSaturation";
constexpr char idHematocrit[] = "Hematocrit";
constexpr char idHemoglobinContent[] = "HemoglobinContent";
constexpr char idHemoglobinLostToUrine[] = "HemoglobinLostToUrine";
constexpr char idOxygenSaturation[] = "OxygenSaturation";
constexpr char idOxygenVenousSaturation[] = "OxygenVenousSaturation";
constexpr char idPhosphate[] = "Phosphate";
constexpr char idPlasmaVolume[] = "PlasmaVolume";
constexpr char idPulseOximetry[] = "PulseOximetry";
constexpr char idRedBloodCellAcetylcholinesterase[] = "RedBloodCellAcetylcholinesterase";
constexpr char idRedBloodCellCount[] = "RedBloodCellCount";
constexpr char idRhTransfusionReactionVolume[] = "RhTransfusionReactionVolume";
constexpr char idShuntFraction[] = "ShuntFraction";
constexpr char idStrongIonDifference[] = "StrongIonDifference";
constexpr char idTotalBilirubin[] = "TotalBilirubin";
constexpr char idTotalProteinConcentration[] = "TotalProteinConcentration";
constexpr char idVenousBloodPH[] = "VenousBloodPH";
constexpr char idVolumeFractionNeutralPhospholipidInPlasma[] = "VolumeFractionNeutralPhospholipidInPlasma";
constexpr char idVolumeFractionNeutralLipidInPlasma[] = "VolumeFractionNeutralLipidInPlasma";
constexpr char idWhiteBloodCellCount[] = "WhiteBloodCellCount";
constexpr char idArterialCarbonDioxidePressure[] = "ArterialCarbonDioxidePressure";
constexpr char idArterialOxygenPressure[] = "ArterialOxygenPressure";
constexpr char idPulmonaryArterialCarbonDioxidePressure[] = "PulmonaryArterialCarbonDioxidePressure";
constexpr char idPulmonaryArterialOxygenPressure[] = "PulmonaryArterialOxygenPressure";
constexpr char idPulmonaryVenousCarbonDioxidePressure[] = "PulmonaryVenousCarbonDioxidePressure";
constexpr char idPulmonaryVenousOxygenPressure[] = "PulmonaryVenousOxygenPressure";
constexpr char idVenousCarbonDioxidePressure[] = "VenousCarbonDioxidePressure";
constexpr char idVenousOxygenPressure[] = "VenousOxygenPressure";
constexpr char idInflammtoryRespone[] = "InflammatoryResponse";

SEBloodChemistrySystem::SEBloodChemistrySystem(Logger* logger)
  : SESystem(logger)
{
  m_ArterialBloodPH = nullptr;
  m_ArterialBloodPHBaseline = nullptr;

  m_BloodDensity = nullptr;

  m_BloodSpecificHeat = nullptr;
  m_BloodUreaNitrogenConcentration = nullptr;
  m_CarbonDioxideSaturation = nullptr;
  m_CarbonMonoxideSaturation = nullptr;
  m_Hematocrit = nullptr;
  m_HemoglobinContent = nullptr;
  m_HemoglobinLostToUrine = nullptr;
  m_OxygenSaturation = nullptr;
  m_OxygenVenousSaturation = nullptr;
  m_Phosphate = nullptr;
  m_PlasmaVolume = nullptr;
  m_PulseOximetry = nullptr;
  m_RedBloodCellAcetylcholinesterase = nullptr;
  m_RedBloodCellCount = nullptr;
  m_ShuntFraction = nullptr;
  m_StrongIonDifference = nullptr;
  m_TotalBilirubin = nullptr;
  m_TotalProteinConcentration = nullptr;
  m_RhTransfusionReactionVolume = nullptr;
  m_VenousBloodPH = nullptr;
  m_VolumeFractionNeutralPhospholipidInPlasma = nullptr;
  m_VolumeFractionNeutralLipidInPlasma = nullptr;
  m_WhiteBloodCellCount = nullptr;

  m_ArterialCarbonDioxidePressure = nullptr;
  m_ArterialOxygenPressure = nullptr;
  m_PulmonaryArterialCarbonDioxidePressure = nullptr;
  m_PulmonaryArterialOxygenPressure = nullptr;
  m_PulmonaryVenousCarbonDioxidePressure = nullptr;
  m_PulmonaryVenousOxygenPressure = nullptr;
  m_VenousCarbonDioxidePressure = nullptr;
  m_VenousOxygenPressure = nullptr;
  m_InflammatoryResponse = nullptr;
}
//-------------------------------------------------------------------------------

SEBloodChemistrySystem::~SEBloodChemistrySystem()
{
  Clear();
}
//-------------------------------------------------------------------------------

void SEBloodChemistrySystem::Clear()
{
  SESystem::Clear();

  SAFE_DELETE(m_ArterialBloodPH);
  SAFE_DELETE(m_ArterialBloodPHBaseline);
  SAFE_DELETE(m_BloodDensity);
  SAFE_DELETE(m_BloodSpecificHeat);
  SAFE_DELETE(m_BloodUreaNitrogenConcentration);
  SAFE_DELETE(m_CarbonDioxideSaturation);
  SAFE_DELETE(m_CarbonMonoxideSaturation);
  SAFE_DELETE(m_Hematocrit);
  SAFE_DELETE(m_HemoglobinContent);
  SAFE_DELETE(m_HemoglobinLostToUrine);
  SAFE_DELETE(m_OxygenSaturation);
  SAFE_DELETE(m_OxygenVenousSaturation);
  SAFE_DELETE(m_Phosphate);
  SAFE_DELETE(m_PlasmaVolume);
  SAFE_DELETE(m_PulseOximetry);
  SAFE_DELETE(m_RedBloodCellAcetylcholinesterase);
  SAFE_DELETE(m_RedBloodCellCount);
  SAFE_DELETE(m_RhTransfusionReactionVolume);
  SAFE_DELETE(m_ShuntFraction);
  SAFE_DELETE(m_StrongIonDifference);
  SAFE_DELETE(m_TotalBilirubin);
  SAFE_DELETE(m_TotalProteinConcentration);
  SAFE_DELETE(m_VenousBloodPH);
  SAFE_DELETE(m_VolumeFractionNeutralPhospholipidInPlasma);
  SAFE_DELETE(m_VolumeFractionNeutralLipidInPlasma);
  SAFE_DELETE(m_WhiteBloodCellCount);

  SAFE_DELETE(m_PulmonaryVenousOxygenPressure);
  SAFE_DELETE(m_PulmonaryArterialOxygenPressure);
  SAFE_DELETE(m_PulmonaryVenousCarbonDioxidePressure);
  SAFE_DELETE(m_PulmonaryArterialCarbonDioxidePressure);
  SAFE_DELETE(m_ArterialOxygenPressure);
  SAFE_DELETE(m_VenousOxygenPressure);
  SAFE_DELETE(m_ArterialCarbonDioxidePressure);
  SAFE_DELETE(m_VenousCarbonDioxidePressure);
  SAFE_DELETE(m_InflammatoryResponse);
}
//-------------------------------------------------------------------------------
const SEScalar* SEBloodChemistrySystem::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
const SEScalar* SEBloodChemistrySystem::GetScalar(const std::string& name)
{
  if (name == idArterialBloodPH) {
    return &GetArterialBloodPH();
  }
  if (name == idArterialBloodPHBaseline) {
    return &GetArterialBloodPHBaseline();
  }
  if (name == idBloodDensity) {
    return &GetBloodDensity();
  }
  if (name == idBloodSpecificHeat) {
    return &GetBloodSpecificHeat();
  }
  if (name == idBloodUreaNitrogenConcentration) {
    return &GetBloodUreaNitrogenConcentration();
  }
  if (name == idCarbonDioxideSaturation) {
    return &GetCarbonDioxideSaturation();
  }
  if (name == idCarbonMonoxideSaturation) {
    return &GetCarbonMonoxideSaturation();
  }
  if (name == idHematocrit) {
    return &GetHematocrit();
  }
  if (name == idHemoglobinContent) {
    return &GetHemoglobinContent();
  }
  if (name == idHemoglobinLostToUrine) {
    return &GetHemoglobinLostToUrine();
  }
  if (name == idOxygenSaturation) {
    return &GetOxygenSaturation();
  }
  if (name == idOxygenVenousSaturation) {
    return &GetOxygenVenousSaturation();
  }
  if (name == idPhosphate) {
    return &GetPhosphate();
  }
  if (name == idPlasmaVolume) {
    return &GetPlasmaVolume();
  }
  if (name == idPulseOximetry) {
    return &GetPulseOximetry();
  }
  if (name == idRedBloodCellAcetylcholinesterase) {
    return &GetRedBloodCellAcetylcholinesterase();
  }
  if (name == idRedBloodCellCount) {
    return &GetRedBloodCellCount();
  }
  if (name == idRhTransfusionReactionVolume) {
    return &GetRhTransfusionReactionVolume();
  }
  if (name == idShuntFraction) {
    return &GetShuntFraction();
  }
  if (name == idStrongIonDifference) {
    return &GetStrongIonDifference();
  }
  if (name == idTotalBilirubin) {
    return &GetTotalBilirubin();
  }
  if (name == idTotalProteinConcentration) {
    return &GetTotalProteinConcentration();
  }
  if (name == idVenousBloodPH) {
    return &GetVenousBloodPH();
  }
  if (name == idVolumeFractionNeutralPhospholipidInPlasma) {
    return &GetVolumeFractionNeutralPhospholipidInPlasma();
  }
  if (name == idVolumeFractionNeutralLipidInPlasma) {
    return &GetVolumeFractionNeutralLipidInPlasma();
  }
  if (name == idWhiteBloodCellCount) {
    return &GetWhiteBloodCellCount();
  }
  if (name == idArterialCarbonDioxidePressure) {
    return &GetArterialCarbonDioxidePressure();
  }
  if (name == idArterialOxygenPressure) {
    return &GetArterialOxygenPressure();
  }
  if (name == idPulmonaryArterialCarbonDioxidePressure) {
    return &GetPulmonaryArterialCarbonDioxidePressure();
  }
  if (name == idPulmonaryArterialOxygenPressure) {
    return &GetPulmonaryArterialOxygenPressure();
  }
  if (name == idPulmonaryVenousCarbonDioxidePressure) {
    return &GetPulmonaryVenousCarbonDioxidePressure();
  }
  if (name == idPulmonaryVenousOxygenPressure) {
    return &GetPulmonaryVenousOxygenPressure();
  }
  if (name == idVenousCarbonDioxidePressure) {
    return &GetVenousCarbonDioxidePressure();
  }
  if (name == idVenousOxygenPressure) {
    return &GetVenousOxygenPressure();
  }

  //This applies to InflammationState values, as they are defined AcuteInflammatoryResponse-Pathogen, e.g.
  size_t split = name.find('-');
  if (split != name.npos) {
    std::string prop = name.substr(split + 1, name.npos); //Get property that follows dash
    std::string parent = name.substr(0, split);
    if (parent == idInflammtoryRespone) {
      return GetInflammatoryResponse().GetScalar(prop);
    }
  }

  return nullptr;
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::Load(const CDM::BloodChemistrySystemData& in)
{
  SESystem::Load(in);

  if (in.ArterialBloodPH().present()) {
    GetArterialBloodPH().Load(in.ArterialBloodPH().get());
  }
  if (in.ArterialBloodPHBaseline().present()) {
    GetArterialBloodPHBaseline().Load(in.ArterialBloodPHBaseline().get());
  }
  if (in.BloodDensity().present()) {
    GetBloodDensity().Load(in.BloodDensity().get());
  }
  if (in.BloodSpecificHeat().present()) {
    GetBloodSpecificHeat().Load(in.BloodSpecificHeat().get());
  }
  if (in.BloodUreaNitrogenConcentration().present()) {
    GetBloodUreaNitrogenConcentration().Load(in.BloodUreaNitrogenConcentration().get());
  }
  if (in.CarbonDioxideSaturation().present()) {
    GetCarbonDioxideSaturation().Load(in.CarbonDioxideSaturation().get());
  }
  if (in.CarbonMonoxideSaturation().present()) {
    GetCarbonMonoxideSaturation().Load(in.CarbonMonoxideSaturation().get());
  }
  if (in.Hematocrit().present()) {
    GetHematocrit().Load(in.Hematocrit().get());
  }
  if (in.HemoglobinContent().present()) {
    GetHemoglobinContent().Load(in.HemoglobinContent().get());
  }
  if (in.HemoglobinLostToUrine().present()) {
    GetHemoglobinLostToUrine().Load(in.HemoglobinLostToUrine().get());
  }
  if (in.OxygenSaturation().present()) {
    GetOxygenSaturation().Load(in.OxygenSaturation().get());
  }
  if (in.OxygenVenousSaturation().present()) {
    GetOxygenVenousSaturation().Load(in.OxygenVenousSaturation().get());
  }
  if (in.Phosphate().present()) {
    GetPhosphate().Load(in.Phosphate().get());
  }
  if (in.PlasmaVolume().present()) {
    GetPlasmaVolume().Load(in.PlasmaVolume().get());
  }
  if (in.PulseOximetry().present()) {
    GetPulseOximetry().Load(in.PulseOximetry().get());
  }
  if (in.RedBloodCellAcetylcholinesterase().present()) {
    GetRedBloodCellAcetylcholinesterase().Load(in.RedBloodCellAcetylcholinesterase().get());
  }
  if (in.RedBloodCellCount().present()) {
    GetRedBloodCellCount().Load(in.RedBloodCellCount().get());
  }
  if (in.RhTransfusionReactionVolume().present()) {
    GetRhTransfusionReactionVolume().Load(in.RhTransfusionReactionVolume().get());
  }
  if (in.ShuntFraction().present()) {
    GetShuntFraction().Load(in.ShuntFraction().get());
  }
  if (in.StrongIonDifference().present()) {
    GetStrongIonDifference().Load(in.StrongIonDifference().get());
  }
  if (in.TotalBilirubin().present()) {
    GetTotalBilirubin().Load(in.TotalBilirubin().get());
  }
  if (in.TotalProteinConcentration().present()) {
    GetTotalProteinConcentration().Load(in.TotalProteinConcentration().get());
  }
  if (in.VenousBloodPH().present()) {
    GetVenousBloodPH().Load(in.VenousBloodPH().get());
  }
  if (in.VolumeFractionNeutralPhospholipidInPlasma().present()) {
    GetVolumeFractionNeutralPhospholipidInPlasma().Load(in.VolumeFractionNeutralPhospholipidInPlasma().get());
  }
  if (in.VolumeFractionNeutralLipidInPlasma().present()) {
    GetVolumeFractionNeutralLipidInPlasma().Load(in.VolumeFractionNeutralLipidInPlasma().get());
  }
  if (in.WhiteBloodCellCount().present()) {
    GetWhiteBloodCellCount().Load(in.WhiteBloodCellCount().get());
  }
  if (in.ArterialCarbonDioxidePressure().present()) {
    GetArterialCarbonDioxidePressure().Load(in.ArterialCarbonDioxidePressure().get());
  }
  if (in.ArterialOxygenPressure().present()) {
    GetArterialOxygenPressure().Load(in.ArterialOxygenPressure().get());
  }
  if (in.PulmonaryArterialCarbonDioxidePressure().present()) {
    GetPulmonaryArterialCarbonDioxidePressure().Load(in.PulmonaryArterialCarbonDioxidePressure().get());
  }
  if (in.PulmonaryArterialOxygenPressure().present()) {
    GetPulmonaryArterialOxygenPressure().Load(in.PulmonaryArterialOxygenPressure().get());
  }
  if (in.PulmonaryVenousOxygenPressure().present()) {
    GetPulmonaryVenousOxygenPressure().Load(in.PulmonaryVenousOxygenPressure().get());
  }
  if (in.PulmonaryVenousCarbonDioxidePressure().present()) {
    GetPulmonaryVenousCarbonDioxidePressure().Load(in.PulmonaryVenousCarbonDioxidePressure().get());
  }
  if (in.VenousCarbonDioxidePressure().present()) {
    GetVenousCarbonDioxidePressure().Load(in.VenousCarbonDioxidePressure().get());
  }
  if (in.VenousOxygenPressure().present()) {
    GetVenousOxygenPressure().Load(in.VenousOxygenPressure().get());
  }
  if (in.InflammatoryResponse().present()) {
    GetInflammatoryResponse().Load(in.InflammatoryResponse().get());
  }
  return true;
}
//-------------------------------------------------------------------------------

CDM::BloodChemistrySystemData* SEBloodChemistrySystem::Unload() const
{
  CDM::BloodChemistrySystemData* data = new CDM::BloodChemistrySystemData();
  Unload(*data);
  return data;
}
//-------------------------------------------------------------------------------

void SEBloodChemistrySystem::Unload(CDM::BloodChemistrySystemData& data) const
{
  SESystem::Unload(data);

  if (m_ArterialBloodPH != nullptr) {
    data.ArterialBloodPH(std::unique_ptr<CDM::ScalarData>(m_ArterialBloodPH->Unload()));
  }
  if (m_ArterialBloodPHBaseline != nullptr) {
    data.ArterialBloodPHBaseline(std::unique_ptr<CDM::ScalarData>(m_ArterialBloodPHBaseline->Unload()));
  }
  if (m_BloodDensity != nullptr) {
    data.BloodDensity(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_BloodDensity->Unload()));
  }
  if (m_BloodSpecificHeat != nullptr) {
    data.BloodSpecificHeat(std::unique_ptr<CDM::ScalarHeatCapacitancePerMassData>(m_BloodSpecificHeat->Unload()));
  }
  if (m_BloodUreaNitrogenConcentration != nullptr) {
    data.BloodUreaNitrogenConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_BloodUreaNitrogenConcentration->Unload()));
  }
  if (m_CarbonDioxideSaturation != nullptr) {
    data.CarbonDioxideSaturation(std::unique_ptr<CDM::ScalarFractionData>(m_CarbonDioxideSaturation->Unload()));
  }
  if (m_CarbonMonoxideSaturation != nullptr) {
    data.CarbonMonoxideSaturation(std::unique_ptr<CDM::ScalarFractionData>(m_CarbonMonoxideSaturation->Unload()));
  }
  if (m_Hematocrit != nullptr) {
    data.Hematocrit(std::unique_ptr<CDM::ScalarFractionData>(m_Hematocrit->Unload()));
  }
  if (m_HemoglobinContent != nullptr) {
    data.HemoglobinContent(std::unique_ptr<CDM::ScalarMassData>(m_HemoglobinContent->Unload()));
  }
  if (m_HemoglobinLostToUrine != nullptr) {
    data.HemoglobinLostToUrine(std::unique_ptr<CDM::ScalarMassData>(m_HemoglobinLostToUrine->Unload()));
  }
  if (m_OxygenSaturation != nullptr) {
    data.OxygenSaturation(std::unique_ptr<CDM::ScalarFractionData>(m_OxygenSaturation->Unload()));
  }
  if (m_OxygenVenousSaturation != nullptr) {
    data.OxygenVenousSaturation(std::unique_ptr<CDM::ScalarFractionData>(m_OxygenVenousSaturation->Unload()));
  }
  if (m_Phosphate != nullptr) {
    data.Phosphate(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_Phosphate->Unload()));
  }
  if (m_PlasmaVolume != nullptr) {
    data.PlasmaVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_PlasmaVolume->Unload()));
  }
  if (m_PulseOximetry != nullptr) {
    data.PulseOximetry(std::unique_ptr<CDM::ScalarFractionData>(m_PulseOximetry->Unload()));
  }
  if (m_RedBloodCellAcetylcholinesterase != nullptr) {
    data.RedBloodCellAcetylcholinesterase(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_RedBloodCellAcetylcholinesterase->Unload()));
  }
  if (m_RedBloodCellCount != nullptr) {
    data.RedBloodCellCount(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_RedBloodCellCount->Unload()));
  }
  if (m_RhTransfusionReactionVolume != nullptr) {
    data.RhTransfusionReactionVolume(std::unique_ptr<CDM::ScalarVolumeData>(m_RhTransfusionReactionVolume->Unload()));
  }
  if (m_ShuntFraction != nullptr) {
    data.ShuntFraction(std::unique_ptr<CDM::ScalarFractionData>(m_ShuntFraction->Unload()));
  }
  if (m_StrongIonDifference != nullptr) {
    data.StrongIonDifference(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_StrongIonDifference->Unload()));
  }
  if (m_TotalBilirubin != nullptr) {
    data.TotalBilirubin(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TotalBilirubin->Unload()));
  }
  if (m_TotalProteinConcentration != nullptr) {
    data.TotalProteinConcentration(std::unique_ptr<CDM::ScalarMassPerVolumeData>(m_TotalProteinConcentration->Unload()));
  }
  if (m_VolumeFractionNeutralPhospholipidInPlasma != nullptr) {
    data.VolumeFractionNeutralPhospholipidInPlasma(std::unique_ptr<CDM::ScalarFractionData>(m_VolumeFractionNeutralPhospholipidInPlasma->Unload()));
  }
  if (m_VolumeFractionNeutralLipidInPlasma != nullptr) {
    data.VolumeFractionNeutralLipidInPlasma(std::unique_ptr<CDM::ScalarFractionData>(m_VolumeFractionNeutralLipidInPlasma->Unload()));
  }
  if (m_WhiteBloodCellCount != nullptr) {
    data.WhiteBloodCellCount(std::unique_ptr<CDM::ScalarAmountPerVolumeData>(m_WhiteBloodCellCount->Unload()));
  }
  if (m_ArterialCarbonDioxidePressure != nullptr) {
    data.ArterialCarbonDioxidePressure(std::unique_ptr<CDM::ScalarPressureData>(m_ArterialCarbonDioxidePressure->Unload()));
  }
  if (m_ArterialOxygenPressure != nullptr) {
    data.ArterialOxygenPressure(std::unique_ptr<CDM::ScalarPressureData>(m_ArterialOxygenPressure->Unload()));
  }
  if (m_PulmonaryArterialCarbonDioxidePressure != nullptr) {
    data.PulmonaryArterialCarbonDioxidePressure(std::unique_ptr<CDM::ScalarPressureData>(m_PulmonaryArterialCarbonDioxidePressure->Unload()));
  }
  if (m_PulmonaryArterialOxygenPressure != nullptr) {
    data.PulmonaryArterialOxygenPressure(std::unique_ptr<CDM::ScalarPressureData>(m_PulmonaryArterialOxygenPressure->Unload()));
  }
  if (m_PulmonaryVenousCarbonDioxidePressure != nullptr) {
    data.PulmonaryVenousCarbonDioxidePressure(std::unique_ptr<CDM::ScalarPressureData>(m_PulmonaryVenousCarbonDioxidePressure->Unload()));
  }
  if (m_PulmonaryVenousOxygenPressure != nullptr) {
    data.PulmonaryVenousOxygenPressure(std::unique_ptr<CDM::ScalarPressureData>(m_PulmonaryVenousOxygenPressure->Unload()));
  }
  if (m_VenousCarbonDioxidePressure != nullptr) {
    data.VenousCarbonDioxidePressure(std::unique_ptr<CDM::ScalarPressureData>(m_VenousCarbonDioxidePressure->Unload()));
  }
  if (m_VenousBloodPH != nullptr) {
    data.VenousBloodPH(std::unique_ptr<CDM::ScalarData>(m_VenousBloodPH->Unload()));
  }
  if (m_VenousOxygenPressure != nullptr) {
    data.VenousOxygenPressure(std::unique_ptr<CDM::ScalarPressureData>(m_VenousOxygenPressure->Unload()));
  }
  if (m_InflammatoryResponse != nullptr) {
    data.InflammatoryResponse(std::unique_ptr<CDM::InflammatoryResponseData>(m_InflammatoryResponse->Unload()));
  }
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasBloodDensity() const
{
  return m_BloodDensity == nullptr ? false : m_BloodDensity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SEBloodChemistrySystem::GetBloodDensity()
{
  if (m_BloodDensity == nullptr) {
    m_BloodDensity = new SEScalarMassPerVolume();
  }
  return *m_BloodDensity;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetBloodDensity(const MassPerVolumeUnit& unit) const
{
  if (m_BloodDensity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodDensity->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasArterialBloodPH() const
{
  return m_ArterialBloodPH == nullptr ? false : m_ArterialBloodPH->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEBloodChemistrySystem::GetArterialBloodPH()
{
  if (m_ArterialBloodPH == nullptr) {
    m_ArterialBloodPH = new SEScalar();
  }
  return *m_ArterialBloodPH;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetArterialBloodPH() const
{
  if (m_ArterialBloodPH == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialBloodPH->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasArterialBloodPHBaseline() const
{
  return m_ArterialBloodPHBaseline == nullptr ? false : m_ArterialBloodPHBaseline->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEBloodChemistrySystem::GetArterialBloodPHBaseline()
{
  if (m_ArterialBloodPHBaseline == nullptr) {
    m_ArterialBloodPHBaseline = new SEScalar();
  }
  return *m_ArterialBloodPHBaseline;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetArterialBloodPHBaseline() const
{
  if (m_ArterialBloodPHBaseline == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialBloodPHBaseline->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasVenousBloodPH() const
{
  return m_VenousBloodPH == nullptr ? false : m_VenousBloodPH->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEBloodChemistrySystem::GetVenousBloodPH()
{
  if (m_VenousBloodPH == nullptr) {
    m_VenousBloodPH = new SEScalar();
  }
  return *m_VenousBloodPH;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetVenousBloodPH() const
{
  if (m_VenousBloodPH == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousBloodPH->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasBloodSpecificHeat() const
{
  return m_BloodSpecificHeat == nullptr ? false : m_BloodSpecificHeat->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarHeatCapacitancePerMass& SEBloodChemistrySystem::GetBloodSpecificHeat()
{
  if (m_BloodSpecificHeat == nullptr) {
    m_BloodSpecificHeat = new SEScalarHeatCapacitancePerMass();
  }
  return *m_BloodSpecificHeat;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetBloodSpecificHeat(const HeatCapacitancePerMassUnit& unit) const
{
  if (m_BloodSpecificHeat == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodSpecificHeat->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasBloodUreaNitrogenConcentration() const
{
  return m_BloodUreaNitrogenConcentration == nullptr ? false : m_BloodUreaNitrogenConcentration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SEBloodChemistrySystem::GetBloodUreaNitrogenConcentration()
{
  if (m_BloodUreaNitrogenConcentration == nullptr) {
    m_BloodUreaNitrogenConcentration = new SEScalarMassPerVolume();
  }
  return *m_BloodUreaNitrogenConcentration;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetBloodUreaNitrogenConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_BloodUreaNitrogenConcentration == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodUreaNitrogenConcentration->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasCarbonDioxideSaturation() const
{
  return m_CarbonDioxideSaturation == nullptr ? false : m_CarbonDioxideSaturation->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetCarbonDioxideSaturation()
{
  if (m_CarbonDioxideSaturation == nullptr) {
    m_CarbonDioxideSaturation = new SEScalarFraction();
  }
  return *m_CarbonDioxideSaturation;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetCarbonDioxideSaturation() const
{
  if (m_CarbonDioxideSaturation == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CarbonDioxideSaturation->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasCarbonMonoxideSaturation() const
{
  return m_CarbonMonoxideSaturation == nullptr ? false : m_CarbonMonoxideSaturation->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetCarbonMonoxideSaturation()
{
  if (m_CarbonMonoxideSaturation == nullptr) {
    m_CarbonMonoxideSaturation = new SEScalarFraction();
  }
  return *m_CarbonMonoxideSaturation;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetCarbonMonoxideSaturation() const
{
  if (m_CarbonMonoxideSaturation == nullptr) {
    return SEScalar::dNaN();
  }
  return m_CarbonMonoxideSaturation->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasHematocrit() const
{
  return m_Hematocrit == nullptr ? false : m_Hematocrit->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetHematocrit()
{
  if (m_Hematocrit == nullptr) {
    m_Hematocrit = new SEScalarFraction();
  }
  return *m_Hematocrit;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetHematocrit() const
{
  if (m_Hematocrit == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Hematocrit->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasHemoglobinContent() const
{
  return m_HemoglobinContent == nullptr ? false : m_HemoglobinContent->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEBloodChemistrySystem::GetHemoglobinContent()
{
  if (m_HemoglobinContent == nullptr) {
    m_HemoglobinContent = new SEScalarMass();
  }
  return *m_HemoglobinContent;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetHemoglobinContent(const MassUnit& unit) const
{
  if (m_HemoglobinContent == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HemoglobinContent->GetValue(unit);
}

//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasHemoglobinLostToUrine() const
{
  return m_HemoglobinLostToUrine == nullptr ? false : m_HemoglobinLostToUrine->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMass& SEBloodChemistrySystem::GetHemoglobinLostToUrine()
{
  if (m_HemoglobinLostToUrine == nullptr) {
    m_HemoglobinLostToUrine = new SEScalarMass();
  }
  return *m_HemoglobinLostToUrine;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetHemoglobinLostToUrine(const MassUnit& unit) const
{
  if (m_HemoglobinLostToUrine == nullptr) {
    return SEScalar::dNaN();
  }
  return m_HemoglobinLostToUrine->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasOxygenSaturation() const
{
  return m_OxygenSaturation == nullptr ? false : m_OxygenSaturation->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetOxygenSaturation()
{
  if (m_OxygenSaturation == nullptr) {
    m_OxygenSaturation = new SEScalarFraction();
  }
  return *m_OxygenSaturation;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetOxygenSaturation() const
{
  if (m_OxygenSaturation == nullptr) {
    return SEScalar::dNaN();
  }
  return m_OxygenSaturation->GetValue();
}
//-------------------------------------------------------------------------------
bool SEBloodChemistrySystem::HasOxygenVenousSaturation() const
{
  return m_OxygenVenousSaturation == nullptr ? false : m_OxygenVenousSaturation->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetOxygenVenousSaturation()
{
  if (m_OxygenVenousSaturation == nullptr) {
    m_OxygenVenousSaturation = new SEScalarFraction();
  }
  return *m_OxygenVenousSaturation;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetOxygenVenousSaturation() const
{
  if (m_OxygenVenousSaturation == nullptr) {
    return SEScalar::dNaN();
  }
  return m_OxygenVenousSaturation->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPhosphate() const
{
  return m_Phosphate == nullptr ? false : m_Phosphate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume& SEBloodChemistrySystem::GetPhosphate()
{
  if (m_Phosphate == nullptr) {
    m_Phosphate = new SEScalarAmountPerVolume();
  }
  return *m_Phosphate;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPhosphate(const AmountPerVolumeUnit& unit) const
{
  if (m_Phosphate == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Phosphate->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPlasmaVolume() const
{
  return m_PlasmaVolume == nullptr ? false : m_PlasmaVolume->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolume& SEBloodChemistrySystem::GetPlasmaVolume()
{
  if (m_PlasmaVolume == nullptr) {
    m_PlasmaVolume = new SEScalarVolume();
  }
  return *m_PlasmaVolume;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPlasmaVolume(const VolumeUnit& unit) const
{
  if (m_PlasmaVolume == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PlasmaVolume->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPulseOximetry() const
{
  return m_PulseOximetry == nullptr ? false : m_PulseOximetry->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetPulseOximetry()
{
  if (m_PulseOximetry == nullptr) {
    m_PulseOximetry = new SEScalarFraction();
  }
  return *m_PulseOximetry;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPulseOximetry() const
{
  if (m_PulseOximetry == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulseOximetry->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasRedBloodCellAcetylcholinesterase() const
{
  return m_RedBloodCellAcetylcholinesterase == nullptr ? false : m_RedBloodCellAcetylcholinesterase->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume& SEBloodChemistrySystem::GetRedBloodCellAcetylcholinesterase()
{
  if (m_RedBloodCellAcetylcholinesterase == nullptr) {
    m_RedBloodCellAcetylcholinesterase = new SEScalarAmountPerVolume();
  }
  return *m_RedBloodCellAcetylcholinesterase;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetRedBloodCellAcetylcholinesterase(const AmountPerVolumeUnit& unit) const
{
  if (m_RedBloodCellAcetylcholinesterase == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RedBloodCellAcetylcholinesterase->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasRedBloodCellCount() const
{
  return m_RedBloodCellCount == nullptr ? false : m_RedBloodCellCount->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume& SEBloodChemistrySystem::GetRedBloodCellCount()
{
  if (m_RedBloodCellCount == nullptr) {
    m_RedBloodCellCount = new SEScalarAmountPerVolume();
  }
  return *m_RedBloodCellCount;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetRedBloodCellCount(const AmountPerVolumeUnit& unit) const
{
  if (m_RedBloodCellCount == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RedBloodCellCount->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasShuntFraction() const
{
  return m_ShuntFraction == nullptr ? false : m_ShuntFraction->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetShuntFraction()
{
  if (m_ShuntFraction == nullptr) {
    m_ShuntFraction = new SEScalarFraction();
  }
  return *m_ShuntFraction;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetShuntFraction() const
{
  if (m_ShuntFraction == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ShuntFraction->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasStrongIonDifference() const
{
  return m_StrongIonDifference == nullptr ? false : m_StrongIonDifference->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume& SEBloodChemistrySystem::GetStrongIonDifference()
{
  if (m_StrongIonDifference == nullptr) {
    m_StrongIonDifference = new SEScalarAmountPerVolume();
  }
  return *m_StrongIonDifference;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetStrongIonDifference(const AmountPerVolumeUnit& unit) const
{
  if (m_StrongIonDifference == nullptr) {
    return SEScalar::dNaN();
  }
  return m_StrongIonDifference->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasTotalBilirubin() const
{
  return m_TotalBilirubin == nullptr ? false : m_TotalBilirubin->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SEBloodChemistrySystem::GetTotalBilirubin()
{
  if (m_TotalBilirubin == nullptr) {
    m_TotalBilirubin = new SEScalarMassPerVolume();
  }
  return *m_TotalBilirubin;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetTotalBilirubin(const MassPerVolumeUnit& unit) const
{
  if (m_TotalBilirubin == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalBilirubin->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasTotalProteinConcentration() const
{
  return m_TotalProteinConcentration == nullptr ? false : m_TotalProteinConcentration->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarMassPerVolume& SEBloodChemistrySystem::GetTotalProteinConcentration()
{
  if (m_TotalProteinConcentration == nullptr) {
    m_TotalProteinConcentration = new SEScalarMassPerVolume();
  }
  return *m_TotalProteinConcentration;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetTotalProteinConcentration(const MassPerVolumeUnit& unit) const
{
  if (m_TotalProteinConcentration == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TotalProteinConcentration->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEBloodChemistrySystem::HasRhTransfusionReactionVolume() const
{
  return m_RhTransfusionReactionVolume == nullptr ? false : m_RhTransfusionReactionVolume->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarVolume& SEBloodChemistrySystem::GetRhTransfusionReactionVolume()
{
  if (m_RhTransfusionReactionVolume == nullptr) {
    m_RhTransfusionReactionVolume = new SEScalarVolume();
  }
  return *m_RhTransfusionReactionVolume;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetRhTransfusionReactionVolume(const VolumeUnit& unit) const
{
  if (m_RhTransfusionReactionVolume == nullptr) {
    return SEScalar::dNaN();
  }
  return m_RhTransfusionReactionVolume->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEBloodChemistrySystem::HasVolumeFractionNeutralPhospholipidInPlasma() const
{
  return m_VolumeFractionNeutralPhospholipidInPlasma == nullptr ? false : m_VolumeFractionNeutralPhospholipidInPlasma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetVolumeFractionNeutralPhospholipidInPlasma()
{
  if (m_VolumeFractionNeutralPhospholipidInPlasma == nullptr) {
    m_VolumeFractionNeutralPhospholipidInPlasma = new SEScalarFraction();
  }
  return *m_VolumeFractionNeutralPhospholipidInPlasma;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetVolumeFractionNeutralPhospholipidInPlasma() const
{
  if (m_VolumeFractionNeutralPhospholipidInPlasma == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VolumeFractionNeutralPhospholipidInPlasma->GetValue();
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasVolumeFractionNeutralLipidInPlasma() const
{
  return m_VolumeFractionNeutralLipidInPlasma == nullptr ? false : m_VolumeFractionNeutralLipidInPlasma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarFraction& SEBloodChemistrySystem::GetVolumeFractionNeutralLipidInPlasma()
{
  if (m_VolumeFractionNeutralLipidInPlasma == nullptr) {
    m_VolumeFractionNeutralLipidInPlasma = new SEScalarFraction();
  }
  return *m_VolumeFractionNeutralLipidInPlasma;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetVolumeFractionNeutralLipidInPlasma() const
{
  if (m_VolumeFractionNeutralLipidInPlasma == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VolumeFractionNeutralLipidInPlasma->GetValue();
}

//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasWhiteBloodCellCount() const
{
  return m_WhiteBloodCellCount == nullptr ? false : m_WhiteBloodCellCount->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarAmountPerVolume& SEBloodChemistrySystem::GetWhiteBloodCellCount()
{
  if (m_WhiteBloodCellCount == nullptr)
    m_WhiteBloodCellCount = new SEScalarAmountPerVolume();
  return *m_WhiteBloodCellCount;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetWhiteBloodCellCount(const AmountPerVolumeUnit& unit) const
{
  if (m_WhiteBloodCellCount == nullptr)
    return SEScalar::dNaN();
  return m_WhiteBloodCellCount->GetValue(unit);
}

//-------------------------------------------------------------------------------
bool SEBloodChemistrySystem::HasArterialCarbonDioxidePressure() const
{
  return m_ArterialCarbonDioxidePressure == nullptr ? false : m_ArterialCarbonDioxidePressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetArterialCarbonDioxidePressure()
{
  if (m_ArterialCarbonDioxidePressure == nullptr) {
    m_ArterialCarbonDioxidePressure = new SEScalarPressure();
  }
  return *m_ArterialCarbonDioxidePressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetArterialCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_ArterialCarbonDioxidePressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialCarbonDioxidePressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasArterialOxygenPressure() const
{
  return m_ArterialOxygenPressure == nullptr ? false : m_ArterialOxygenPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetArterialOxygenPressure()
{
  if (m_ArterialOxygenPressure == nullptr) {
    m_ArterialOxygenPressure = new SEScalarPressure();
  }
  return *m_ArterialOxygenPressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetArterialOxygenPressure(const PressureUnit& unit) const
{
  if (m_ArterialOxygenPressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ArterialOxygenPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPulmonaryArterialCarbonDioxidePressure() const
{
  return m_PulmonaryArterialCarbonDioxidePressure == nullptr ? false : m_PulmonaryArterialCarbonDioxidePressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetPulmonaryArterialCarbonDioxidePressure()
{
  if (m_PulmonaryArterialCarbonDioxidePressure == nullptr) {
    m_PulmonaryArterialCarbonDioxidePressure = new SEScalarPressure();
  }
  return *m_PulmonaryArterialCarbonDioxidePressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPulmonaryArterialCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_PulmonaryArterialCarbonDioxidePressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryArterialCarbonDioxidePressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPulmonaryArterialOxygenPressure() const
{
  return m_PulmonaryArterialOxygenPressure == nullptr ? false : m_PulmonaryArterialOxygenPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetPulmonaryArterialOxygenPressure()
{
  if (m_PulmonaryArterialOxygenPressure == nullptr) {
    m_PulmonaryArterialOxygenPressure = new SEScalarPressure();
  }
  return *m_PulmonaryArterialOxygenPressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPulmonaryArterialOxygenPressure(const PressureUnit& unit) const
{
  if (m_PulmonaryArterialOxygenPressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryArterialOxygenPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPulmonaryVenousCarbonDioxidePressure() const
{
  return m_PulmonaryVenousCarbonDioxidePressure == nullptr ? false : m_PulmonaryVenousCarbonDioxidePressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetPulmonaryVenousCarbonDioxidePressure()
{
  if (m_PulmonaryVenousCarbonDioxidePressure == nullptr) {
    m_PulmonaryVenousCarbonDioxidePressure = new SEScalarPressure();
  }
  return *m_PulmonaryVenousCarbonDioxidePressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPulmonaryVenousCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_PulmonaryVenousCarbonDioxidePressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryVenousCarbonDioxidePressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasPulmonaryVenousOxygenPressure() const
{
  return m_PulmonaryVenousOxygenPressure == nullptr ? false : m_PulmonaryVenousOxygenPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetPulmonaryVenousOxygenPressure()
{
  if (m_PulmonaryVenousOxygenPressure == nullptr) {
    m_PulmonaryVenousOxygenPressure = new SEScalarPressure();
  }
  return *m_PulmonaryVenousOxygenPressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetPulmonaryVenousOxygenPressure(const PressureUnit& unit) const
{
  if (m_PulmonaryVenousOxygenPressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_PulmonaryVenousOxygenPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasVenousOxygenPressure() const
{
  return m_VenousOxygenPressure == nullptr ? false : m_VenousOxygenPressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetVenousOxygenPressure()
{
  if (m_VenousOxygenPressure == nullptr) {
    m_VenousOxygenPressure = new SEScalarPressure();
  }
  return *m_VenousOxygenPressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetVenousOxygenPressure(const PressureUnit& unit) const
{
  if (m_VenousOxygenPressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousOxygenPressure->GetValue(unit);
}
//-------------------------------------------------------------------------------

bool SEBloodChemistrySystem::HasVenousCarbonDioxidePressure() const
{
  return m_VenousCarbonDioxidePressure == nullptr ? false : m_VenousCarbonDioxidePressure->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarPressure& SEBloodChemistrySystem::GetVenousCarbonDioxidePressure()
{
  if (m_VenousCarbonDioxidePressure == nullptr) {
    m_VenousCarbonDioxidePressure = new SEScalarPressure();
  }
  return *m_VenousCarbonDioxidePressure;
}
//-------------------------------------------------------------------------------
double SEBloodChemistrySystem::GetVenousCarbonDioxidePressure(const PressureUnit& unit) const
{
  if (m_VenousCarbonDioxidePressure == nullptr) {
    return SEScalar::dNaN();
  }
  return m_VenousCarbonDioxidePressure->GetValue(unit);
}
//-------------------------------------------------------------------------------
bool SEBloodChemistrySystem::HasInflammatoryResponse() const
{
  return m_InflammatoryResponse == nullptr ? false : m_InflammatoryResponse->IsValid();
}
//-------------------------------------------------------------------------------
SEInflammatoryResponse& SEBloodChemistrySystem::GetInflammatoryResponse()
{
  if (m_InflammatoryResponse == nullptr) {
    m_InflammatoryResponse = new SEInflammatoryResponse();
  }
  return *m_InflammatoryResponse;
}
//-------------------------------------------------------------------------------
SEInflammatoryResponse& SEBloodChemistrySystem::GetInflammatoryResponse() const
{
  return *m_InflammatoryResponse;
}
//-------------------------------------------------------------------------------
Tree<const char*> SEBloodChemistrySystem::GetPhysiologyRequestGraph() const
{
  return Tree<const char*> { classname() }
    .emplace_back(idArterialBloodPH)
    .emplace_back(idArterialBloodPHBaseline)
    .emplace_back(idBloodDensity)
    .emplace_back(idBloodSpecificHeat)
    .emplace_back(idBloodUreaNitrogenConcentration)
    .emplace_back(idCarbonDioxideSaturation)
    .emplace_back(idCarbonMonoxideSaturation)
    .emplace_back(idHematocrit)
    .emplace_back(idHemoglobinContent)
    .emplace_back(idOxygenSaturation)
    .emplace_back(idPhosphate)
    .emplace_back(idPlasmaVolume)
    .emplace_back(idPulseOximetry)
    .emplace_back(idRedBloodCellAcetylcholinesterase)
    .emplace_back(idRedBloodCellCount)
    .emplace_back(idShuntFraction)
    .emplace_back(idStrongIonDifference)
    .emplace_back(idTotalBilirubin)
    .emplace_back(idTotalProteinConcentration)
    .emplace_back(idVenousBloodPH)
    .emplace_back(idVolumeFractionNeutralPhospholipidInPlasma)
    .emplace_back(idVolumeFractionNeutralLipidInPlasma)
    .emplace_back(idArterialCarbonDioxidePressure)
    .emplace_back(idArterialOxygenPressure)
    .emplace_back(idPulmonaryArterialCarbonDioxidePressure)
    .emplace_back(idPulmonaryArterialOxygenPressure)
    .emplace_back(idPulmonaryVenousCarbonDioxidePressure)
    .emplace_back(idPulmonaryVenousOxygenPressure)
    .emplace_back(idVenousCarbonDioxidePressure)
    .emplace_back(idVenousOxygenPressure)
    .emplace_back(GetInflammatoryResponse().GetPhysiologyRequestGraph());
  ;
}

SEInflammatoryResponse::SEInflammatoryResponse()
  : m_LocalPathogen(nullptr)
  , m_LocalMacrophage(nullptr)
  , m_LocalNeutrophil(nullptr)
  , m_LocalBarrier(nullptr)
  , m_ActiveTLR(CDM::enumOnOff::value(-1))
  , m_AutonomicResponseLevel(nullptr)
  , m_Catecholamines(nullptr)
  , m_ConstitutiveNOS(nullptr)
  , m_InducibleNOSPre(nullptr)
  , m_InducibleNOS(nullptr)
  , m_InflammationTime(nullptr)
  , m_Interleukin6(nullptr)
  , m_Interleukin10(nullptr)
  , m_Interleukin12(nullptr)
  , m_MacrophageResting(nullptr)
  , m_MacrophageActive(nullptr)
  , m_NeutrophilResting(nullptr)
  , m_NeutrophilActive(nullptr)
  , m_Nitrate(nullptr)
  , m_NitricOxide(nullptr)
  , m_BloodPathogen(nullptr)
  , m_TissueIntegrity(nullptr)
  , m_Trauma(nullptr)
  , m_TumorNecrosisFactor(nullptr)
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
  SAFE_DELETE(m_LocalPathogen);
  SAFE_DELETE(m_LocalMacrophage);
  SAFE_DELETE(m_LocalNeutrophil);
  SAFE_DELETE(m_LocalBarrier);
  m_ActiveTLR = CDM::enumOnOff::value(-1);
  SAFE_DELETE(m_AutonomicResponseLevel);
  SAFE_DELETE(m_Catecholamines);
  SAFE_DELETE(m_ConstitutiveNOS);
  SAFE_DELETE(m_InducibleNOSPre);
  SAFE_DELETE(m_InducibleNOS);
  SAFE_DELETE(m_InflammationTime);
  SAFE_DELETE(m_Interleukin6);
  SAFE_DELETE(m_Interleukin10);
  SAFE_DELETE(m_Interleukin12);
  SAFE_DELETE(m_MacrophageActive);
  SAFE_DELETE(m_MacrophageResting);
  SAFE_DELETE(m_NeutrophilResting);
  SAFE_DELETE(m_NeutrophilActive);
  SAFE_DELETE(m_Nitrate);
  SAFE_DELETE(m_NitricOxide);
  SAFE_DELETE(m_BloodPathogen);
  SAFE_DELETE(m_TissueIntegrity);
  SAFE_DELETE(m_Trauma);
  SAFE_DELETE(m_TumorNecrosisFactor);
  m_InflammationSources.clear();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::Load(const CDM::InflammatoryResponseData& in)
{
  GetLocalPathogen().Load(in.LocalPathogen());
  GetLocalMacrophage().Load(in.LocalMacrophage());
  GetLocalNeutrophil().Load(in.LocalNeutrophil());
  GetLocalBarrier().Load(in.LocalBarrier());
  SetActiveTLR(in.ActiveTLR());
  GetAutonomicResponseLevel().Load(in.AutonomicResponseLevel());
  GetCatecholamines().Load(in.Catecholamines());
  GetConstitutiveNOS().Load(in.ConstitutiveNOS());
  GetInducibleNOSPre().Load(in.InducibleNOSPre());
  GetInducibleNOS().Load(in.InducibleNOS());
  GetInflammationTime().Load(in.InflammationTime());
  GetInterleukin6().Load(in.Interleukin6());
  GetInterleukin10().Load(in.Interleukin10());
  GetInterleukin12().Load(in.Interleukin12());
  GetMacrophageResting().Load(in.MacrophageResting());
  GetMacrophageActive().Load(in.MacrophageActive());
  GetNeutrophilResting().Load(in.NeutrophilResting());
  GetNeutrophilActive().Load(in.NeutrophilActive());
  GetNitrate().Load(in.Nitrate());
  GetNitricOxide().Load(in.NitricOxide());
  GetBloodPathogen().Load(in.BloodPathogen());
  GetTissueIntegrity().Load(in.TissueIntegrity());
  GetTrauma().Load(in.Trauma());
  GetTumorNecrosisFactor().Load(in.TumorNecrosisFactor());

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
  data.LocalPathogen(std::unique_ptr<CDM::ScalarData>(m_LocalPathogen->Unload()));
  data.LocalMacrophage(std::unique_ptr<CDM::ScalarData>(m_LocalMacrophage->Unload()));
  data.LocalNeutrophil(std::unique_ptr<CDM::ScalarData>(m_LocalNeutrophil->Unload()));
  data.LocalBarrier(std::unique_ptr<CDM::ScalarData>(m_LocalBarrier->Unload()));
  data.ActiveTLR(m_ActiveTLR);
  data.AutonomicResponseLevel(std::unique_ptr<CDM::ScalarData>(m_AutonomicResponseLevel->Unload()));
  data.Catecholamines(std::unique_ptr<CDM::ScalarData>(m_Catecholamines->Unload()));
  data.ConstitutiveNOS(std::unique_ptr<CDM::ScalarData>(m_ConstitutiveNOS->Unload()));
  data.InducibleNOSPre(std::unique_ptr<CDM::ScalarData>(m_InducibleNOSPre->Unload()));
  data.InducibleNOS(std::unique_ptr<CDM::ScalarData>(m_InducibleNOS->Unload()));
  data.InflammationTime(std::unique_ptr<CDM::ScalarTimeData>(m_InflammationTime->Unload()));
  data.Interleukin6(std::unique_ptr<CDM::ScalarData>(m_Interleukin6->Unload()));
  data.Interleukin10(std::unique_ptr<CDM::ScalarData>(m_Interleukin10->Unload()));
  data.Interleukin12(std::unique_ptr<CDM::ScalarData>(m_Interleukin12->Unload()));
  data.MacrophageResting(std::unique_ptr<CDM::ScalarData>(m_MacrophageResting->Unload()));
  data.MacrophageActive(std::unique_ptr<CDM::ScalarData>(m_MacrophageActive->Unload()));
  data.NeutrophilResting(std::unique_ptr<CDM::ScalarData>(m_NeutrophilResting->Unload()));
  data.NeutrophilActive(std::unique_ptr<CDM::ScalarData>(m_NeutrophilActive->Unload()));
  data.Nitrate(std::unique_ptr<CDM::ScalarData>(m_Nitrate->Unload()));
  data.NitricOxide(std::unique_ptr<CDM::ScalarData>(m_NitricOxide->Unload()));
  data.BloodPathogen(std::unique_ptr<CDM::ScalarData>(m_BloodPathogen->Unload()));
  data.TissueIntegrity(std::unique_ptr<CDM::Scalar0To1Data>(m_TissueIntegrity->Unload()));
  data.Trauma(std::unique_ptr<CDM::ScalarData>(m_Trauma->Unload()));
  data.TumorNecrosisFactor(std::unique_ptr<CDM::ScalarData>(m_TumorNecrosisFactor->Unload()));

  for (auto src : m_InflammationSources) {
    data.Source().push_back(src);
  }
}
//-------------------------------------------------------------------------------
void SEInflammatoryResponse::Initialize()
{
  //Values from Chow2005Acute
  GetLocalPathogen().SetValue(0.0);
  GetLocalMacrophage().SetValue(10.0);
  GetLocalNeutrophil().SetValue(0.0);
  GetLocalBarrier().SetValue(1.0);
  SetActiveTLR(CDM::enumOnOff::Off);
  GetAutonomicResponseLevel().SetValue(0.0);
  GetBloodPathogen().SetValue(0.0);
  GetTrauma().SetValue(0.0);
  GetMacrophageResting().SetValue(1.0);
  GetMacrophageActive().SetValue(0.0);
  GetNeutrophilResting().SetValue(1.0);
  GetNeutrophilActive().SetValue(0.0);
  GetInducibleNOS().SetValue(0.0);
  GetInducibleNOSPre().SetValue(0.0);
  GetConstitutiveNOS().SetValue(0.05);
  GetNitrate().SetValue(0.0);
  GetNitricOxide().SetValue(0.0);
  GetTumorNecrosisFactor().SetValue(0.0);
  GetInterleukin6().SetValue(0.001);
  GetInterleukin10().SetValue(0.01);
  GetInterleukin12().SetValue(0.0);
  GetCatecholamines().SetValue(0.0);
  GetTissueIntegrity().SetValue(1.0);
  GetInflammationTime().SetValue(0.0, TimeUnit::s);
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::IsValid()
{
  if (HasLocalPathogen() && HasLocalMacrophage() && HasLocalNeutrophil() && HasLocalBarrier() && HasActiveTLR() && HasBloodPathogen() && HasTrauma() && HasAutonomicResponseLevel() && HasMacrophageResting() && HasMacrophageActive() && HasNeutrophilResting() && HasNeutrophilActive() && HasInducibleNOSPre() && HasInducibleNOS() && HasConstitutiveNOS() && HasNitrate() && HasNitricOxide() && HasTumorNecrosisFactor() && HasInterleukin6() && HasInterleukin10() && HasInterleukin12() && HasCatecholamines() && HasTissueIntegrity() && HasInflammationTime()) {
    return true;
  } else {
    return false;
  }
}
//-------------------------------------------------------------------------------
const SEScalar* SEInflammatoryResponse::GetScalar(const char* name)
{
  return GetScalar(std::string { name });
}
//-------------------------------------------------------------------------------
constexpr char idLocalPathogen[] = "LocalPathogen";
constexpr char idLocalMacrophage[] = "LocalMacrophage";
constexpr char idLocalNeutrophil[] = "LocalNeutrophil";
constexpr char idLocalBarrier[] = "LocalBarrier";
constexpr char idBloodPathogen[] = "BloodPathogen";
constexpr char idTrauma[] = "Trauma";
constexpr char idAutonomicResponseLevel[] = "AutonomicResponseLevel";
constexpr char idMacrophageResting[] = "MacrophageResting";
constexpr char idMacrophageActive[] = "MacrophageActive";
constexpr char idNeutrophilResting[] = "NeutrophilResting";
constexpr char idNeutrophilActive[] = "NeutrophilActive";
constexpr char idInducibleNOSPre[] = "InducibleNOSPre";
constexpr char idInducibleNOS[] = "InducibleNOS";
constexpr char idConstitutiveNOS[] = "ConstitutiveNOS";
constexpr char idNitrate[] = "Nitrate";
constexpr char idNitricOxide[] = "NitricOxide";
constexpr char idTumorNecrosisFactor[] = "TumorNecrosisFactor";
constexpr char idInterleukin6[] = "Interleukin6";
constexpr char idInterleukin10[] = "Interleukin10";
constexpr char idInterleukin12[] = "Interleukin12";
constexpr char idCatecholamines[] = "Catecholamines";
constexpr char idTissueIntegrity[] = "TissueIntegrity";
constexpr char idInflammationTime[] = "InflammationTime";

const SEScalar* SEInflammatoryResponse::GetScalar(const std::string& name)
{
  using namespace std::string_literals;
  if (idLocalPathogen == name) {
    return &GetLocalPathogen();
  }
  if (idLocalMacrophage == name) {
    return &GetLocalMacrophage();
  }
  if (idLocalNeutrophil == name) {
    return &GetLocalNeutrophil();
  }
  if (idLocalBarrier == name) {
    return &GetLocalBarrier();
  }
  if (idBloodPathogen == name) {
    return &GetBloodPathogen();
  }
  if (idTrauma == name) {
    return &GetTrauma();
  }
  if (idAutonomicResponseLevel == name) {
    return &GetAutonomicResponseLevel();
  }
  if (idMacrophageResting == name) {
    return &GetMacrophageResting();
  }
  if (idMacrophageActive == name) {
    return &GetMacrophageActive();
  }
  if (idNeutrophilResting == name) {
    return &GetNeutrophilResting();
  }
  if (idNeutrophilActive == name) {
    return &GetNeutrophilActive();
  }
  if (idInducibleNOSPre == name) {
    return &GetInducibleNOSPre();
  }
  if (idInducibleNOS == name) {
    return &GetInducibleNOS();
  }
  if (idConstitutiveNOS == name) {
    return &GetConstitutiveNOS();
  }
  if (idNitrate == name) {
    return &GetNitrate();
  }
  if (idNitricOxide == name) {
    return &GetNitricOxide();
  }
  if (idTumorNecrosisFactor == name) {
    return &GetTumorNecrosisFactor();
  }
  if (idInterleukin6 == name) {
    return &GetInterleukin6();
  }
  if (idInterleukin10 == name) {
    return &GetInterleukin10();
  }
  if (idInterleukin12 == name) {
    return &GetInterleukin12();
  }
  if (idCatecholamines == name) {
    return &GetCatecholamines();
  }
  if (idTissueIntegrity == name) {
    return &GetTissueIntegrity();
  }
  if (idInflammationTime == name) {
    return &GetInflammationTime();
  }

  return nullptr;
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasLocalPathogen() const
{
  return m_LocalPathogen == nullptr ? false : m_LocalPathogen->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetLocalPathogen()
{
  if (m_LocalPathogen == nullptr) {
    m_LocalPathogen = new SEScalar();
  }
  return *m_LocalPathogen;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetLocalPathogen() const
{
  if (m_LocalPathogen == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LocalPathogen->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasLocalMacrophage() const
{
  return m_LocalMacrophage == nullptr ? false : m_LocalMacrophage->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetLocalMacrophage()
{
  if (m_LocalMacrophage == nullptr) {
    m_LocalMacrophage = new SEScalar();
  }
  return *m_LocalMacrophage;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetLocalMacrophage() const
{
  if (m_LocalMacrophage == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LocalMacrophage->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasLocalNeutrophil() const
{
  return m_LocalNeutrophil == nullptr ? false : m_LocalNeutrophil->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetLocalNeutrophil()
{
  if (m_LocalNeutrophil == nullptr) {
    m_LocalNeutrophil = new SEScalar();
  }
  return *m_LocalNeutrophil;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetLocalNeutrophil() const
{
  if (m_LocalNeutrophil == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LocalNeutrophil->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasLocalBarrier() const
{
  return m_LocalBarrier == nullptr ? false : m_LocalBarrier->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetLocalBarrier()
{
  if (m_LocalBarrier == nullptr) {
    m_LocalBarrier = new SEScalar();
  }
  return *m_LocalBarrier;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetLocalBarrier() const
{
  if (m_LocalBarrier == nullptr) {
    return SEScalar::dNaN();
  }
  return m_LocalBarrier->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasBloodPathogen() const
{
  return m_BloodPathogen == nullptr ? false : m_BloodPathogen->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetBloodPathogen()
{
  if (m_BloodPathogen == nullptr) {
    m_BloodPathogen = new SEScalar();
  }
  return *m_BloodPathogen;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetBloodPathogen() const
{
  if (m_BloodPathogen == nullptr) {
    return SEScalar::dNaN();
  }
  return m_BloodPathogen->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTrauma() const
{
  return m_Trauma == nullptr ? false : m_Trauma->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetTrauma()
{
  if (m_Trauma == nullptr) {
    m_Trauma = new SEScalar();
  }
  return *m_Trauma;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetTrauma() const
{
  if (m_Trauma == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Trauma->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasMacrophageResting() const
{
  return m_MacrophageResting == nullptr ? false : m_MacrophageResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetMacrophageResting()
{
  if (m_MacrophageResting == nullptr) {
    m_MacrophageResting = new SEScalar();
  }
  return *m_MacrophageResting;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetMacrophageResting() const
{
  if (m_MacrophageResting == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MacrophageResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasMacrophageActive() const
{
  return m_MacrophageActive == nullptr ? false : m_MacrophageActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetMacrophageActive()
{
  if (m_MacrophageActive == nullptr) {
    m_MacrophageActive = new SEScalar();
  }
  return *m_MacrophageActive;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetMacrophageActive() const
{
  if (m_MacrophageActive == nullptr) {
    return SEScalar::dNaN();
  }
  return m_MacrophageActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasNeutrophilResting() const
{
  return m_NeutrophilResting == nullptr ? false : m_NeutrophilResting->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetNeutrophilResting()
{
  if (m_NeutrophilResting == nullptr) {
    m_NeutrophilResting = new SEScalar();
  }
  return *m_NeutrophilResting;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetNeutrophilResting() const
{
  if (m_NeutrophilResting == nullptr) {
    return SEScalar::dNaN();
  }
  return m_NeutrophilResting->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasNeutrophilActive() const
{
  return m_NeutrophilActive == nullptr ? false : m_NeutrophilActive->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetNeutrophilActive()
{
  if (m_NeutrophilActive == nullptr) {
    m_NeutrophilActive = new SEScalar();
  }
  return *m_NeutrophilActive;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetNeutrophilActive() const
{
  if (m_NeutrophilActive == nullptr) {
    return SEScalar::dNaN();
  }
  return m_NeutrophilActive->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInducibleNOSPre() const
{
  return m_InducibleNOSPre == nullptr ? false : m_InducibleNOSPre->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetInducibleNOSPre()
{
  if (m_InducibleNOSPre == nullptr) {
    m_InducibleNOSPre = new SEScalar();
  }
  return *m_InducibleNOSPre;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInducibleNOSPre() const
{
  if (m_InducibleNOSPre == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InducibleNOSPre->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInducibleNOS() const
{
  return m_InducibleNOS == nullptr ? false : m_InducibleNOS->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetInducibleNOS()
{
  if (m_InducibleNOS == nullptr) {
    m_InducibleNOS = new SEScalar();
  }
  return *m_InducibleNOS;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInducibleNOS() const
{
  if (m_InducibleNOS == nullptr) {
    return SEScalar::dNaN();
  }
  return m_InducibleNOS->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasConstitutiveNOS() const
{
  return m_ConstitutiveNOS == nullptr ? false : m_ConstitutiveNOS->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetConstitutiveNOS()
{
  if (m_ConstitutiveNOS == nullptr) {
    m_ConstitutiveNOS = new SEScalar();
  }
  return *m_ConstitutiveNOS;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetConstitutiveNOS() const
{
  if (m_ConstitutiveNOS == nullptr) {
    return SEScalar::dNaN();
  }
  return m_ConstitutiveNOS->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasNitrate() const
{
  return m_Nitrate == nullptr ? false : m_Nitrate->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetNitrate()
{
  if (m_Nitrate == nullptr) {
    m_Nitrate = new SEScalar();
  }
  return *m_Nitrate;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetNitrate() const
{
  if (m_Nitrate == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Nitrate->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasNitricOxide() const
{
  return m_NitricOxide == nullptr ? false : m_NitricOxide->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetNitricOxide()
{
  if (m_NitricOxide == nullptr) {
    m_NitricOxide = new SEScalar();
  }
  return *m_NitricOxide;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetNitricOxide() const
{
  if (m_NitricOxide == nullptr) {
    return SEScalar::dNaN();
  }
  return m_NitricOxide->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTumorNecrosisFactor() const
{
  return m_TumorNecrosisFactor == nullptr ? false : m_TumorNecrosisFactor->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetTumorNecrosisFactor()
{
  if (m_TumorNecrosisFactor == nullptr) {
    m_TumorNecrosisFactor = new SEScalar();
  }
  return *m_TumorNecrosisFactor;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetTumorNecrosisFactor() const
{
  if (m_TumorNecrosisFactor == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TumorNecrosisFactor->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInterleukin6() const
{
  return m_Interleukin6 == nullptr ? false : m_Interleukin6->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetInterleukin6()
{
  if (m_Interleukin6 == nullptr) {
    m_Interleukin6 = new SEScalar();
  }
  return *m_Interleukin6;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInterleukin6() const
{
  if (m_Interleukin6 == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Interleukin6->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInterleukin10() const
{
  return m_Interleukin10 == nullptr ? false : m_Interleukin10->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetInterleukin10()
{
  if (m_Interleukin10 == nullptr) {
    m_Interleukin10 = new SEScalar();
  }
  return *m_Interleukin10;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInterleukin10() const
{
  if (m_Interleukin10 == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Interleukin10->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInterleukin12() const
{
  return m_Interleukin12 == nullptr ? false : m_Interleukin12->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetInterleukin12()
{
  if (m_Interleukin12 == nullptr) {
    m_Interleukin12 = new SEScalar();
  }
  return *m_Interleukin12;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInterleukin12() const
{
  if (m_Interleukin12 == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Interleukin12->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasCatecholamines() const
{
  return m_Catecholamines == nullptr ? false : m_Catecholamines->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetCatecholamines()
{
  if (m_Catecholamines == nullptr) {
    m_Catecholamines = new SEScalar();
  }
  return *m_Catecholamines;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetCatecholamines() const
{
  if (m_Catecholamines == nullptr) {
    return SEScalar::dNaN();
  }
  return m_Catecholamines->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasAutonomicResponseLevel() const
{
  return m_AutonomicResponseLevel == nullptr ? false : m_AutonomicResponseLevel->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar& SEInflammatoryResponse::GetAutonomicResponseLevel()
{
  if (m_AutonomicResponseLevel == nullptr) {
    m_AutonomicResponseLevel = new SEScalar();
  }
  return *m_AutonomicResponseLevel;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetAutonomicResponseLevel() const
{
  if (m_AutonomicResponseLevel == nullptr) {
    return SEScalar::dNaN();
  }
  return m_AutonomicResponseLevel->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasTissueIntegrity() const
{
  return m_TissueIntegrity == nullptr ? false : m_TissueIntegrity->IsValid();
}
//-------------------------------------------------------------------------------
SEScalar0To1& SEInflammatoryResponse::GetTissueIntegrity()
{
  if (m_TissueIntegrity == nullptr) {
    m_TissueIntegrity = new SEScalar0To1();
  }
  return *m_TissueIntegrity;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetTissueIntegrity() const
{
  if (m_TissueIntegrity == nullptr) {
    return SEScalar::dNaN();
  }
  return m_TissueIntegrity->GetValue();
}
//-------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInflammationTime() const
{
  return m_InflammationTime == nullptr ? false : m_InflammationTime->IsValid();
}
//-------------------------------------------------------------------------------
SEScalarTime& SEInflammatoryResponse::GetInflammationTime()
{
  if (m_InflammationTime == nullptr) {
    m_InflammationTime = new SEScalarTime();
  }
  return *m_InflammationTime;
}
//-------------------------------------------------------------------------------
double SEInflammatoryResponse::GetInflammationTime(const TimeUnit& unit) const
{
  if (m_InflammationTime == nullptr) {
    return SEScalarTime::dNaN();
  }
  return m_InflammationTime->GetValue(unit);
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
//--------------------------------------------------------------------------------
bool SEInflammatoryResponse::HasInflammationSource(CDM::enumInflammationSource::value src)
{
  std::vector<CDM::enumInflammationSource>::iterator srcLoc;
  srcLoc = std::find(m_InflammationSources.begin(), m_InflammationSources.end(), src);
  return srcLoc == m_InflammationSources.end() ? false : true;
}
//---------------------------------------------------------------------------------
Tree<const char*> SEInflammatoryResponse::GetPhysiologyRequestGraph() const
{

  return Tree<const char*> { "InflammatoryResponse" }
    .emplace_back(idLocalPathogen)
    .emplace_back(idLocalMacrophage)
    .emplace_back(idLocalNeutrophil)
    .emplace_back(idLocalBarrier)
    .emplace_back(idBloodPathogen)
    .emplace_back(idTrauma)
    .emplace_back(idMacrophageResting)
    .emplace_back(idMacrophageActive)
    .emplace_back(idNeutrophilResting)
    .emplace_back(idNeutrophilActive)
    .emplace_back(idInducibleNOSPre)
    .emplace_back(idInducibleNOS)
    .emplace_back(idConstitutiveNOS)
    .emplace_back(idNitrate)
    .emplace_back(idNitricOxide)
    .emplace_back(idTumorNecrosisFactor)
    .emplace_back(idInterleukin6)
    .emplace_back(idInterleukin10)
    .emplace_back(idInterleukin12)
    .emplace_back(idCatecholamines)
    .emplace_back(idTissueIntegrity)
    .emplace_back(idAutonomicResponseLevel)
    .emplace_back(idInflammationTime);
}

}
