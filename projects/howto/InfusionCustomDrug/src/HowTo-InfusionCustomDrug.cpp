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



// Include the various types you will be using in your code
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>

#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>


using namespace biogears;

#include <atomic>
std::unique_ptr<SESubstanceCompound> makeSubstanceCompound(std::unique_ptr<biogears::PhysiologyEngine>& engine)
{
  static std::atomic<int> counter { 0 };
  std::stringstream ss;
  ss << "Substance_" << ++counter;
  return std::make_unique<SESubstanceCompound>(ss.str().c_str(), engine->GetLogger());
}
bool mixSubstanceCompound(std::unique_ptr<biogears::PhysiologyEngine>& engine, std::unique_ptr<SESubstanceCompound>& compound, const char* substance, double concentration, MassPerVolumeUnit unit)
{
  SESubstance* new_substance = engine->GetSubstanceManager().GetSubstance(substance);
  if (new_substance) {
    auto substance_concentration = SESubstanceConcentration(*new_substance);
    substance_concentration.GetConcentration().SetValue(10.0, MassPerVolumeUnit::mg_Per_mL);
    compound->GetComponents().push_back(substance_concentration);

    //Because the substance isn't register atat this point, it is possible to adjust the name based on the current mix.
    //Active infusions are stored by substance name so once registered and infuse you can no longer modify the mix name;
    return true;
  }
  return false;
}
std::string registerCompoundbiogears(std::unique_ptr<biogears::PhysiologyEngine>& engine, std::unique_ptr<SESubstanceCompound>&& compound)
{
  std::string compound_name = compound->GetName_cStr();
  engine->GetSubstanceManager().AddCompound(*compound.release());
  return compound_name;
}

bool infuseCompound(std::unique_ptr<biogears::PhysiologyEngine>& engine, std::string compound_name, double volume, VolumeUnit volume_unit, double rate, VolumePerTimeUnit rate_unit)
{
  auto& compound = *engine->GetSubstanceManager().GetCompound(compound_name);
  SESubstanceCompoundInfusion infusion{compound};
  infusion.GetBagVolume().SetValue(volume, volume_unit);
  infusion.GetRate().SetValue(rate, rate_unit);
  return engine->ProcessAction(infusion);
}
\
//--------------------------------------------------------------------------------------------------
/// \brief
/// Framework for creating custom SESubstanceCompounds with the BioGears API. Allows Testing
/// of custom IV Bag Mixes with the API.  Because, changing a registered substance while BioGears 
/// is running would cause unexpected behavior each new substance will have a unique name
///
/// \details
/// Refer to the SESubstanceInfusion class
/// Refer to the SESubstanceManager class
//--------------------------------------------------------------------------------------------------
void HowToInfusionDrug()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowTo-InfusionCustomDrug.log");
  bg->GetLogger()->Info("HowTo-InfusionCustomDrug");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

   // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file

  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("SystolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("DiastolicArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::L_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineProductionRate", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("UrineOsmolarity", OsmolalityUnit::mOsm_Per_kg);

  auto customCompound = makeSubstanceCompound(bg);
  auto mixSuccess = mixSubstanceCompound(bg,customCompound,"Saline", 2.17, MassPerVolumeUnit::kg_Per_L );
  mixSuccess &= mixSubstanceCompound(bg, customCompound, "Albumin", 10.0, MassPerVolumeUnit::mg_Per_mL);
  mixSuccess &= mixSubstanceCompound(bg, customCompound, "Morphine", 10.0, MassPerVolumeUnit::mg_Per_mL);

  if (!mixSuccess) {
    bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Saline"), "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
    bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Albumin"), "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);
    bg->GetEngineTrack()->GetDataRequestManager().CreateSubstanceDataRequest().Set(*bg->GetSubstanceManager().GetSubstance("Morphine"), "PlasmaConcentration", MassPerVolumeUnit::ug_Per_L);

    auto compound_name = registerCompoundbiogears(bg, std::move(customCompound)); //< BG takes ownership of customCompound here and will delete it once it is removed (DLL Boundry issue need to create a make_compound function to avoid that in biogears)
    infuseCompound(bg, compound_name, 1.0, VolumeUnit::L, 100., VolumePerTimeUnit::mL_Per_hr);

    bg->AdvanceModelTime(10, TimeUnit::min);
    infuseCompound(bg, compound_name, 1.0, VolumeUnit::L, 0, VolumePerTimeUnit::mL_Per_hr);

    bg->AdvanceModelTime(10, TimeUnit::min);
  } else {
    bg->GetLogger()->Error("Saline,Albumin, and Morphine definitions required for this HowTo please ensure they exist and try again.");
  }
}
