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
#include <biogears/engine/Controller/BioGears.h>

#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFlowElastance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitance.h>
#include <biogears/cdm/properties/SEScalarHeatCapacitancePerMass.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarNeg1To1.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/utils/FileUtils.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Equipment/AnesthesiaMachine.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/engine/Systems/Environment.h>
#include <biogears/engine/Systems/Gastrointestinal.h>
#include <biogears/schema/cdm/EnvironmentConditions.hxx>
#include <biogears/schema/cdm/Patient.hxx>

namespace BGE = mil::tatrc::physiology::biogears;

namespace biogears {
BioGears::BioGears(const std::string& logFileName)
  : BioGears(new Logger(logFileName))
{

  myLogger = true;
  m_DataTrack = nullptr;
}

BioGears::BioGears(Logger* logger)
  : Loggable(logger)
{

  myLogger = false;
  m_DataTrack = nullptr;
  if (!m_Logger->HasForward()) { // Don't override a forwarder, if there already is one there
    m_Logger->SetForward(this);
  }

  SetUp();
}

BioGears::BioGears(const std::string& logFileName, const std::string& working_dir)
  : BioGears(new Logger(logFileName, working_dir), working_dir)
{
  myLogger = true;
  m_DataTrack = nullptr;
}

BioGears::BioGears(Logger* logger, const std::string& working_dir)
  : Loggable(logger)
{
  SetCurrentWorkingDirectory(working_dir);
  myLogger = false;
  m_DataTrack = nullptr;
  if (!m_Logger->HasForward()) { // Don't override a forwarder, if there already is one there
    m_Logger->SetForward(this);
  }

  SetUp();
}

DataTrack& BioGears::GetDataTrack()
{
  if (m_DataTrack == nullptr) {
    m_DataTrack = new DataTrack();
  }
  return *m_DataTrack;
}

void BioGears::SetUp()
{
  m_CurrentTime = std::make_unique<SEScalarTime>();
  m_SimulationTime = std::make_unique<SEScalarTime>();
  m_CurrentTime->SetValue(0, TimeUnit::s);
  m_SimulationTime->SetValue(0, TimeUnit::s);
  m_Logger->SetLogTime(m_SimulationTime.get());

  m_Substances = std::make_unique<BioGearsSubstances>(*this);
  m_Substances->LoadSubstanceDirectory();

  m_Patient = std::make_unique<SEPatient>(GetLogger());

  m_Config = std::make_unique<BioGearsConfiguration>(*m_Substances);
  m_Config->Initialize();

  m_SaturationCalculator = SaturationCalculator::make_unique(*this);

  m_Actions = std::make_unique<SEActionManager>(*m_Substances);
  m_Conditions = std::make_unique<SEConditionManager>(*m_Substances);

  m_Environment = Environment::make_unique(*this);

  m_BloodChemistrySystem = BloodChemistry::make_unique(*this);
  m_CardiovascularSystem = Cardiovascular::make_unique(*this);
  m_EndocrineSystem = Endocrine::make_unique(*this);
  m_EnergySystem = Energy::make_unique(*this);
  m_GastrointestinalSystem = Gastrointestinal::make_unique(*this);
  m_HepaticSystem = Hepatic::make_unique(*this);
  m_NervousSystem = Nervous::make_unique(*this);
  m_RenalSystem = Renal::make_unique(*this);
  m_RespiratorySystem = Respiratory::make_unique(*this);
  m_DrugSystem = Drugs::make_unique(*this);
  m_TissueSystem = Tissue::make_unique(*this);

  m_ECG = ECG::make_unique(*this);

  m_AnesthesiaMachine = AnesthesiaMachine::make_unique(*this);

  m_Inhaler = Inhaler::make_unique(*this);

  m_Compartments = BioGearsCompartments::make_unique(*this);

  m_Circuits = BioGearsCircuits::make_unique(*this);

  m_DiffusionCalculator = DiffusionCalculator::make_unique(*this);
}

bool BioGears::Initialize(const PhysiologyEngineConfiguration* config)
{
  m_State = EngineState::NotReady;
  Info("Configuring patient");
  if (!SetupPatient()) {
    return false;
  }

  Info("Resetting Substances");
  m_Substances->Reset();

  // Clear all substances and reload the original data
  // This clears out all engine specific data stored in the substance
  // This will preserve the pointer to the substance, but not any pointers
  // to any substance child objects, those will need to be fixed up, if they exist

  Info("Initializing Configuration");
  m_Config->Initialize(); // Load up Defaults
  if (config != nullptr) {
    Info("Merging Provided Configuration");
    m_Config->Merge(*config);
  }
  // Now, Let's see if there is anything to merge into our base configuration
  Info("Merging OnDisk Configuration");
  BioGearsConfiguration cFile(*m_Substances);
  cFile.Load("BioGearsConfiguration.xml");
  m_Config->Merge(cFile);

  // Now we can check the config
  if (m_Config->WritePatientBaselineFile()) {
    std::string stableDir = "./stable/";
    MakeDirectory(stableDir);
    CDM::PatientData* pData = m_Patient->Unload();
    pData->contentVersion(BGE::Version);
    // Write out the stable patient state
    std::ofstream stream(stableDir + m_Patient->GetName() + ".xml");
    // Write out the xml file
    xml_schema::namespace_infomap map;
    map[""].name = "uri:/mil/tatrc/physiology/datamodel";
    Patient(stream, *pData, map);
    stream.close();
    SAFE_DELETE(pData);
  }

  m_SaturationCalculator->Initialize(*m_Substances);

  m_Actions->Clear();
  m_Conditions->Clear();

  // This will also Initialize the environment
  // Due to needing the initial environment values for circuits to construct properly
  Info("Creating Circuits and Compartments");
  CreateCircuitsAndCompartments();

  m_AirwayMode = CDM::enumBioGearsAirwayMode::Free;
  m_Intubation = CDM::enumOnOff::Off;
  m_CurrentTime->SetValue(0, TimeUnit::s);
  m_SimulationTime->SetValue(0, TimeUnit::s);
  m_Logger->SetLogTime(m_SimulationTime.get());

  Info("Initializing Substances");
  m_Substances->InitializeSubstances(); // Sets all concentrations and such of all substances for all compartments, need to do this after we figure out what's in the environment

  //Note:  Diffusion Calculator is initialized in Tissue::SetUp because it depends on so many Tissue parameters

  Info("Initializing Systems");
  m_CardiovascularSystem->Initialize();
  m_RespiratorySystem->Initialize();
  m_AnesthesiaMachine->Initialize();
  m_GastrointestinalSystem->Initialize();
  m_HepaticSystem->Initialize();
  m_RenalSystem->Initialize();
  m_NervousSystem->Initialize();
  m_EndocrineSystem->Initialize();
  m_DrugSystem->Initialize();
  m_EnergySystem->Initialize();
  m_BloodChemistrySystem->Initialize();
  m_TissueSystem->Initialize(); // Depends on some parameters that Blood Chemistry initializes,needs to be after
  m_ECG->Initialize();
  m_Inhaler->Initialize();

  return true;
}

void BioGears::SetAirwayMode(CDM::enumBioGearsAirwayMode::value mode)
{
  if (mode == m_AirwayMode) {
    return; // do nazing!
  }
  if (mode == CDM::enumBioGearsAirwayMode::Inhaler && m_AirwayMode != CDM::enumBioGearsAirwayMode::Free) {
    throw CommonDataModelException("Can only change airway mode to Inhaler from the Free mode, Disable other equipment first.");
  }
  if (mode == CDM::enumBioGearsAirwayMode::AnesthesiaMachine && m_AirwayMode != CDM::enumBioGearsAirwayMode::Free) {
    throw CommonDataModelException("Can only change airway mode to Anesthesia Machine from the Free mode, Disable other equipment first.");
  }
  if (mode == CDM::enumBioGearsAirwayMode::MechanicalVentilator && m_AirwayMode != CDM::enumBioGearsAirwayMode::Free) {
    throw CommonDataModelException("Can only change airway mode to Mechanical Ventilator from the Free mode, Disable other equipment first.");
  }
  if (mode != m_AirwayMode) {
    m_Compartments->UpdateAirwayGraph();
  }
  m_AirwayMode = mode;
  std::stringstream ss;
  ss << "Airway Mode : " << m_AirwayMode;
  Info(ss);
}

void BioGears::SetIntubation(CDM::enumOnOff::value s)
{
  if (m_Intubation == s) {
    return; // do nazing!
  }
  if (m_AirwayMode == CDM::enumBioGearsAirwayMode::Inhaler) {
    throw CommonDataModelException("Cannot intubate if the inhaler is active.");
  }
  m_Intubation = s;
}

bool BioGears::SetupPatient()
{
  bool err = false;
  std::stringstream ss;

  //Gender is the only thing we absolutely need to be defined
  //Everything else is either derived or assumed to be a "standard" value
  if (!m_Patient->HasGender()) {
    Error("Patient must provide a gender.");
    err = true;
  }

  //AGE ---------------------------------------------------------------
  double age_yr;
  double ageMin_yr = 18.0;
  double ageMax_yr = 65.0;
  double ageStandard_yr = 44.0;
  if (!m_Patient->HasAge()) {
    m_Patient->GetAge().SetValue(ageStandard_yr, TimeUnit::yr);
    ss << "No patient age set. Using the standard value of " << ageStandard_yr << " years.";
    Info(ss);
  }
  age_yr = m_Patient->GetAge().GetValue(TimeUnit::yr);
  if (age_yr < ageMin_yr) {
    ss << "Patient age of " << age_yr << " years is too young. We do not model pediatrics. Minimum age allowed is " << ageMin_yr << " years.";
    Error(ss);
    err = true;
  } else if (age_yr > ageMax_yr) {
    ss << "Patient age of " << age_yr << " years is too old. We do not model geriatrics. Maximum age allowed is " << ageMax_yr << " years.";
    Error(ss);
    err = true;
  }

  //PAIN SUSCEPTIBILITY -----------------------------------------------------------------------------------
  double painStandard = 0.0;
  if (!m_Patient->HasPainSusceptibility()) {
    m_Patient->GetPainSusceptibility().SetValue(painStandard);
    ss << "No patient pain susceptibility set " << painStandard << " being used.";
    Info(ss);
  }

  //SWEAT SUSCEPTIBILITY -----------------------------------------------------------------------------------
  double sweatStandard = 0.0;
  if (!m_Patient->HasHyperhidrosis()) {
    m_Patient->GetHyperhidrosis().SetValue(sweatStandard);
    ss << "No patient sweat susceptibility set " << sweatStandard << " being used.";
    Info(ss);
  }

  //HEIGHT ---------------------------------------------------------------
  //From CDC values for 20 year olds
  //Minimums are 3rd percentile, Maximums are 97th percentile, and standard is 50th percentile
  /// \cite Centers2016clinical
  double heightMinMale_cm = 163.0;
  double heightMaxMale_cm = 190.0;
  double heightStandardMale_cm = 177.0;
  double heightMinFemale_cm = 151.0;
  double heightMaxFemale_cm = 175.5;
  double heightStandardFemale_cm = 163.0;
  //Male
  double heightMin_cm = heightMinMale_cm;
  double heightMax_cm = heightMaxMale_cm;
  double heightStandard_cm = heightStandardMale_cm;
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    //Female
    heightMin_cm = heightMinFemale_cm;
    heightMax_cm = heightMaxFemale_cm;
    heightStandard_cm = heightStandardFemale_cm;
  }
  if (!m_Patient->HasHeight()) {
    m_Patient->GetHeight().SetValue(heightStandard_cm, LengthUnit::cm);
    ss << "No patient height set. Using the standard value of " << heightStandard_cm << " cm.";
    Info(ss);
  }
  double height_cm = m_Patient->GetHeight().GetValue(LengthUnit::cm);
  double height_ft = Convert(height_cm, LengthUnit::cm, LengthUnit::ft);
  //Check for outrageous values
  if (height_ft < 4.5 || height_ft > 7.0) {
    Error("Patient height setting is outrageous. It must be between 4.5 and 7.0 ft");
    err = true;
  }
  if (height_cm < heightMin_cm) {
    ss << "Patient height of " << height_cm << " cm is outside of typical ranges - below 3rd percentile (" << heightMax_cm << " cm). No guarantees of model validity.";
    Warning(ss);
  } else if (height_cm > heightMax_cm) {
    ss << "Patient height of " << height_cm << " cm is outside of typical ranges - above 97th percentile(" << heightMin_cm << " cm). No guarantees of model validity.";
    Warning(ss);
  }

  //WEIGHT ---------------------------------------------------------------
  /// \cite World2006bmi
  double weight_kg;
  double BMI_kg_per_m2;
  double BMIStandard_kg_per_m2 = 21.75;
  double BMIObese_kg_per_m2 = 30.0;
  double BMIOverweight_kg_per_m2 = 25.0;
  double BMIUnderweight_kg_per_m2 = 18.5;
  double BMISeverelyUnderweight_kg_per_m2 = 16.0;
  if (!m_Patient->HasWeight()) {
    weight_kg = BMIStandard_kg_per_m2 * std::pow(m_Patient->GetHeight().GetValue(LengthUnit::m), 2);
    m_Patient->GetWeight().SetValue(weight_kg, MassUnit::kg);
    ss << "No patient weight set. Using the standard BMI value of 21.75 kg/m^2, resulting in a weight of " << weight_kg << " kg.";
    Info(ss);
  }
  weight_kg = m_Patient->GetWeight(MassUnit::kg);
  BMI_kg_per_m2 = weight_kg / std::pow(m_Patient->GetHeight().GetValue(LengthUnit::m), 2);
  if (BMI_kg_per_m2 > BMIObese_kg_per_m2) {
    ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << "  kg/m^2 is too high. Obese patients must be modeled by adding/using a condition. Maximum BMI allowed is " << BMIObese_kg_per_m2 << " kg/m^2.";
    Error(ss);
    err = true;
  }
  if (BMI_kg_per_m2 > BMIOverweight_kg_per_m2) {
    ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is overweight. No guarantees of model validity.";
    Warning(ss);
  }
  if (BMI_kg_per_m2 < BMIUnderweight_kg_per_m2) {
    ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is underweight. No guarantees of model validity.";
    Warning(ss);
  }
  if (BMI_kg_per_m2 < BMISeverelyUnderweight_kg_per_m2) {
    ss << "Patient Body Mass Index (BMI) of " << BMI_kg_per_m2 << " kg/m^2 is too low. Severly underweight patients must be modeled by adding/using a condition. Maximum BMI allowed is " << BMISeverelyUnderweight_kg_per_m2 << " kg/m^2.";
    Error(ss);
    err = true;
  }

  //BODY FAT FRACTION ---------------------------------------------------------------
  //From American Council on Exercise
  /// \cite muth2009what
  double fatFraction;
  double fatFractionStandardMale = 0.21;
  double fatFractionStandardFemale = 0.28;
  double fatFractionMaxMale = 0.25; //Obese
  double fatFractionMaxFemale = 0.32; //Obese
  double fatFractionMinMale = 0.02; //Essential fat
  double fatFractionMinFemale = 0.10; //Essential fat
  //Male
  double fatFractionMin = fatFractionMinMale;
  double fatFractionMax = fatFractionMaxMale;
  double fatFractionStandard = fatFractionStandardMale;
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    //Female
    fatFractionMin = fatFractionMinFemale;
    fatFractionMax = fatFractionMaxFemale;
    fatFractionStandard = fatFractionStandardFemale;
  }

  if (!m_Patient->HasBodyFatFraction()) {
    fatFraction = fatFractionStandard;
    m_Patient->GetBodyFatFraction().SetValue(fatFraction);
    ss << "No patient body fat fraction set. Using the standard value of " << fatFraction << ".";
    Info(ss);
  }
  fatFraction = m_Patient->GetBodyFatFraction().GetValue();
  if (fatFraction > fatFractionMax) {
    ss << "Patient body fat fraction of " << fatFraction << " is too high. Obese patients must be modeled by adding/using a condition. Maximum body fat fraction allowed is " << fatFractionMax << ".";
    Error(ss);
    err = true;
  } else if (fatFraction < fatFractionMin) {
    ss << "Patient body fat fraction  " << fatFraction << " is too low. Patients must have essential fat. Minimum body fat fraction allowed is " << fatFractionMin << ".";
    Error(ss);
    err = true;
  }

  //Lean Body Mass ---------------------------------------------------------------
  if (m_Patient->HasLeanBodyMass()) {
    ss << "Patient lean body mass cannot be set. It is determined by weight and body fat fraction.";
    Error(ss);
    err = true;
  }
  double leanBodyMass_kg = weight_kg * (1.0 - fatFraction);
  m_Patient->GetLeanBodyMass().SetValue(leanBodyMass_kg, MassUnit::kg);
  ss << "Patient lean body mass computed and set to " << leanBodyMass_kg << " kg.";
  Info(ss);

  //Muscle Mass ---------------------------------------------------------------
  // \cite janssen2000skeletal
  if (m_Patient->HasMuscleMass()) {
    ss << "Patient muscle mass cannot be set directly. It is determined by a percentage of weight.";
    Error(ss);
    err = true;
  }

  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    m_Patient->GetMuscleMass().SetValue(weight_kg * .306, MassUnit::kg);
  } else {
    m_Patient->GetMuscleMass().SetValue(weight_kg * .384, MassUnit::kg);
  }

  ss << "Patient muscle mass computed and set to " << m_Patient->GetMuscleMass().GetValue(MassUnit::kg) << " kg.";
  Info(ss);

  //Body Density ---------------------------------------------------------------
  if (m_Patient->HasBodyDensity()) {
    ss << "Patient body density cannot be set. It is determined using body fat fraction.";
    Error(ss);
    err = true;
  }
  //Using the average of Siri and Brozek formulas
  /// \cite siri1961body
  /// \cite brovzek1963densitometric
  double SiriBodyDensity_g_Per_cm3 = 4.95 / (fatFraction + 4.50);
  double BrozekBodyDensity_g_Per_cm3 = 4.57 / (fatFraction + 4.142);
  double bodyDensity_g_Per_cm3 = (SiriBodyDensity_g_Per_cm3 + BrozekBodyDensity_g_Per_cm3) / 2.0;
  m_Patient->GetBodyDensity().SetValue(bodyDensity_g_Per_cm3, MassPerVolumeUnit::g_Per_cm3);
  ss << "Patient body density computed and set to " << bodyDensity_g_Per_cm3 << " g/cm^3.";
  Info(ss);

  //Heart Rate ---------------------------------------------------------------
  double heartRate_bpm;
  double heartStandard_bpm = 72.0;
  double heartRateMax_bpm = 109.0;
  double heartRateTachycardia_bpm = 100.0;
  double heartRateMin_bpm = 50.0;
  double heartRateBradycardia_bpm = 60.0;
  if (!m_Patient->HasHeartRateBaseline()) {
    heartRate_bpm = heartStandard_bpm;
    m_Patient->GetHeartRateBaseline().SetValue(heartRate_bpm, FrequencyUnit::Per_min);
    ss << "No patient heart rate baseline set. Using the standard value of " << heartRate_bpm << " bpm.";
    Info(ss);
  }
  heartRate_bpm = m_Patient->GetHeartRateBaseline(FrequencyUnit::Per_min);
  if (heartRateTachycardia_bpm < heartRate_bpm) {
    if (heartRate_bpm <= heartRateMax_bpm) {
      ss << "Patient heart rate baseline of " << heartRate_bpm << " bpm is tachycardic";
      Info(ss);
    } else {
      ss << "Patient heart rate baseline of " << heartRate_bpm << " exceeds maximum stable value of " << heartRateMax_bpm << " bpm.  Resetting heart rate baseline to " << heartRateMax_bpm;
      m_Patient->GetHeartRateBaseline().SetValue(heartRateMax_bpm, FrequencyUnit::Per_min);
      Info(ss);
    }
  } else if (heartRate_bpm < heartRateBradycardia_bpm) {
    if (heartRateMin_bpm <= heartRate_bpm) {
      ss << "Patient heart rate baseline of " << heartRate_bpm << " bpm is bradycardic";
      Info(ss);
    } else {
      ss << "Patient heart rate baseline of " << heartRate_bpm << " exceeds minimum stable value of " << heartRateMin_bpm << " bpm.  Resetting heart rate baseline to " << heartRateMin_bpm;
      m_Patient->GetHeartRateBaseline().SetValue(heartRateMin_bpm, FrequencyUnit::Per_min);
      Info(ss);
    }
  }

  //Tanaka H, Monahan KD, Seals DR (January 2001). "Age-predicted maximal heart rate revisited". J. Am. Coll. Cardiol. 37(1): 153–6. doi:10.1016/S0735-1097(00)01054-8.PMID 11153730.
  double computedHeartRateMaximum_bpm = 208.0 - (0.7 * m_Patient->GetAge(TimeUnit::yr));
  if (!m_Patient->HasHeartRateMaximum()) {
    m_Patient->GetHeartRateMaximum().SetValue(computedHeartRateMaximum_bpm, FrequencyUnit::Per_min);
    ss << "No patient heart rate maximum set. Using a computed value of " << computedHeartRateMaximum_bpm << " bpm.";
    Info(ss);
  } else {
    if (m_Patient->GetHeartRateMaximum(FrequencyUnit::Per_min) < heartRate_bpm) {
      ss << "Patient heart rate maximum must be greater than the baseline heart rate.";
      Error(ss);
      err = true;
    }
    ss << "Specified patient heart rate maximum of " << m_Patient->GetHeartRateMaximum(FrequencyUnit::Per_min) << " bpm differs from computed value of " << computedHeartRateMaximum_bpm << " bpm. No guarantees of model validity.";
    Warning(ss);
  }
  if (!m_Patient->HasHeartRateMinimum()) {
    m_Patient->GetHeartRateMinimum().SetValue(0.001, FrequencyUnit::Per_min);
    ss << "No patient heart rate minimum set. Using a default value of " << 0.001 << " bpm.";
    Info(ss);
  }
  if (m_Patient->GetHeartRateMinimum(FrequencyUnit::Per_min) > heartRate_bpm) {
    ss << "Patient heart rate minimum must be less than the baseline heart rate.";
    Error(ss);
    err = true;
  }

  //Arterial Pressures ---------------------------------------------------------------
  double systolic_mmHg;
  double diastolic_mmHg;
  double systolicStandard_mmHg = 114.0;
  double diastolicStandard_mmHg = 73.5;
  double systolicMax_mmHg = 120.0; //Hypertension
  double diastolicMax_mmHg = 80.0; //Hypertension
  double systolicMin_mmHg = 90.0; //Hypotension
  double diastolicMin_mmHg = 60.0; //Hypotension
  double narrowestPulseFactor = 0.75; //From Wikipedia: Pulse Pressure
  if (!m_Patient->HasSystolicArterialPressureBaseline()) {
    systolic_mmHg = systolicStandard_mmHg;
    m_Patient->GetSystolicArterialPressureBaseline().SetValue(systolic_mmHg, PressureUnit::mmHg);
    ss << "No patient systolic pressure baseline set. Using the standard value of " << systolic_mmHg << " mmHg.";
    Info(ss);
  }
  systolic_mmHg = m_Patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  if (systolic_mmHg < systolicMin_mmHg) {
    ss << "Patient systolic pressure baseline of " << systolic_mmHg << " mmHg is too low. Hypotension must be modeled by adding/using a condition. Minimum systolic pressure baseline allowed is " << systolicMin_mmHg << " mmHg.";
    Error(ss);
    err = true;
  } else if (systolic_mmHg > systolicMax_mmHg) {
    ss << "Patient systolic pressure baseline of " << systolic_mmHg << " mmHg is too high. Hypertension must be modeled by adding/using a condition. Maximum systolic pressure baseline allowed is " << systolicMax_mmHg << " mmHg.";
    Error(ss);
    err = true;
  }

  if (!m_Patient->HasDiastolicArterialPressureBaseline()) {
    diastolic_mmHg = diastolicStandard_mmHg;
    m_Patient->GetDiastolicArterialPressureBaseline().SetValue(diastolic_mmHg, PressureUnit::mmHg);
    ss << "No patient diastolic pressure baseline set. Using the standard value of " << diastolic_mmHg << " mmHg.";
    Info(ss);
  }
  diastolic_mmHg = m_Patient->GetDiastolicArterialPressureBaseline(PressureUnit::mmHg);
  if (diastolic_mmHg < diastolicMin_mmHg) {
    ss << "Patient diastolic pressure baseline of " << diastolic_mmHg << " mmHg is too low. Hypotension must be modeled by adding/using a condition. Minimum diastolic pressure baseline allowed is " << diastolicMin_mmHg << " mmHg.";
    Error(ss);
    err = true;
  } else if (diastolic_mmHg > diastolicMax_mmHg) {
    ss << "Patient diastolic pressure baseline of " << diastolic_mmHg << " mmHg is too high. Hypertension must be modeled by adding/using a condition. Maximum diastolic pressure baseline allowed is " << diastolicMax_mmHg << " mmHg.";
    Error(ss);
    err = true;
  }

  if (diastolic_mmHg > 0.75 * systolic_mmHg) {
    ss << "Patient baseline pulse pressure (systolic vs. diastolic pressure fraction) of " << diastolic_mmHg / systolic_mmHg << " is abnormally narrow. Minimum fraction allowed is " << narrowestPulseFactor << " .";
    Error(ss);
    err = true;
  }

  if (m_Patient->HasMeanArterialPressureBaseline()) {
    ss << "Patient mean arterial pressure baseline cannot be set. It is determined through homeostatic simulation.";
    Error(ss);
    err = true;
  }
  double MAP_mmHg = 1.0 / 3.0 * systolic_mmHg + 2.0 / 3.0 * diastolic_mmHg;
  m_Patient->GetMeanArterialPressureBaseline().SetValue(MAP_mmHg, PressureUnit::mmHg);

  //Blood Type -----------------------------------------------------------------
  //default blood type based on universal acceptor
  CDM::enumBloodType::value defaultBloodType_ABO = CDM::enumBloodType::AB;
  bool defaultBloodRh = true;
  if (!m_Patient->HasBloodRh()) {
    m_Patient->SetBloodRh(defaultBloodRh);
    ss << "Patient's blood Rh factor has not been set. Defaulting to  " << defaultBloodRh;
    Info(ss);
  }

  if (!m_Patient->HasBloodType()) {
    m_Patient->SetBloodType(defaultBloodType_ABO);
    ss << "Patient's blood type antigen has not been set. Defaulting to  " << defaultBloodType_ABO;
    Info(ss);
  }

  //Blood Volume ---------------------------------------------------------------
  /// \cite Morgan2006Clinical
  double bloodVolume_mL;
  double computedBloodVolume_mL = 65.6 * std::pow(weight_kg, 1.02);
  double bloodVolumeMin_mL = computedBloodVolume_mL * 0.85; //Stage 1 Hypovolemia
  double bloodVolumeMax_mL = computedBloodVolume_mL * 1.15; //Just go the same distance on the other side
  if (!m_Patient->HasBloodVolumeBaseline()) {
    bloodVolume_mL = computedBloodVolume_mL;
    m_Patient->GetBloodVolumeBaseline().SetValue(bloodVolume_mL, VolumeUnit::mL);
    ss << "No patient blood volume baseline set. Using a computed value of " << computedBloodVolume_mL << " mL.";
    Info(ss);
  }
  bloodVolume_mL = m_Patient->GetBloodVolumeBaseline(VolumeUnit::mL);
  if (bloodVolume_mL != computedBloodVolume_mL) {
    ss << "Specified patient blood volume baseline of " << bloodVolume_mL << " mL differs from computed value of " << computedBloodVolume_mL << " mL. No guarantees of model validity and there is a good chance the patient will not reach a starting homeostatic point.";
    Warning(ss);
  }
  if (bloodVolume_mL < bloodVolumeMin_mL) {
    ss << "Patient blood volume baseline of " << bloodVolume_mL << " mL is too low. Hypovolemia must be modeled by adding/using a condition. Minimum blood volume baseline allowed is " << bloodVolumeMin_mL << " mL.";
    Error(ss);
    err = true;
  } else if (bloodVolume_mL > bloodVolumeMax_mL) {
    ss << "Patient blood volume baseline of " << bloodVolume_mL << " mL is too high. Excessive volume must be modeled by adding/using a condition. Maximum blood volume baseline allowed is " << bloodVolumeMax_mL << " mL.";
    Error(ss);
    err = true;
  }

  //Respiration Rate ---------------------------------------------------------------
  //Note: This is overwritten after stabilization
  double respirationRate_bpm;
  double respirationRateStandard_bpm = 14.0;
  double respirationRateMax_bpm = 20.0;
  double respirationRateMin_bpm = 12.0;
  if (!m_Patient->HasRespirationRateBaseline()) {
    respirationRate_bpm = respirationRateStandard_bpm;
    m_Patient->GetRespirationRateBaseline().SetValue(respirationRate_bpm, FrequencyUnit::Per_min);
    ss << "No patient respiration rate baseline set. Using the standard value of " << respirationRate_bpm << " bpm.";
    Info(ss);
  }

  respirationRate_bpm = m_Patient->GetRespirationRateBaseline(FrequencyUnit::Per_min);

  if (respirationRate_bpm > respirationRateMax_bpm) {
    ss << "Patient respiration rate baseline of " << respirationRate_bpm << " bpm is too high. Non-healthy values must be modeled by adding/using a condition. Maximum respiration rate baseline allowed is " << respirationRateMax_bpm << " bpm.";
    Error(ss);
    err = true;
  } else if (respirationRate_bpm < respirationRateMin_bpm) {
    ss << "Patient respiration rate baseline of " << respirationRate_bpm << " bpm is too low. Non-healthy values must be modeled by adding/using a condition. Minimum respiration rate baseline allowed is " << respirationRateMin_bpm << " bpm.";
    Error(ss);
    err = true;
  }

  //Right Lung Ratio ---------------------------------------------------------------
  double rightLungRatio;
  double rightLungRatioStandard = 0.525;
  double rightLungRatioMax = 0.60;
  double rightLungRatioMin = 0.50;
  if (!m_Patient->HasRightLungRatio()) {
    rightLungRatio = rightLungRatioStandard;
    m_Patient->GetRightLungRatio().SetValue(rightLungRatio);
    ss << "No patient right lung ratio set. Using the standard value of " << rightLungRatio << ".";
    Info(ss);
  }
  rightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  if (rightLungRatio > rightLungRatioMax) {
    ss << "Patient right lung ratio of " << rightLungRatio << " is too high. Non-healthy values must be modeled by adding/using a condition. Maximum right lung ratio allowed is " << rightLungRatioMax << ".";
    Error(ss);
    err = true;
  } else if (rightLungRatio < rightLungRatioMin) {
    ss << "Patient right lung ratio of " << rightLungRatio << " is too low. Non-healthy values must be modeled by adding/using a condition. Minimum right lung ratio allowed is " << rightLungRatioMin << ".";
    Error(ss);
    err = true;
  }

  //Respiratory Volumes ---------------------------------------------------------------
  //These are based on weight
  /// \cite ganong1995review
  double totalLungCapacity_L;
  double computedTotalLungCapacity_L = 80.0 * weight_kg / 1000.0;
  if (!m_Patient->HasTotalLungCapacity()) {
    totalLungCapacity_L = computedTotalLungCapacity_L;
    m_Patient->GetTotalLungCapacity().SetValue(totalLungCapacity_L, VolumeUnit::L);
    ss << "No patient total lung capacity set. Using a computed value of " << computedTotalLungCapacity_L << " L.";
    Info(ss);
  }
  totalLungCapacity_L = m_Patient->GetTotalLungCapacity(VolumeUnit::L);
  if (totalLungCapacity_L != computedTotalLungCapacity_L) {
    ss << "Specified total lung capacity of " << totalLungCapacity_L << " L differs from computed value of " << computedTotalLungCapacity_L << " L. No guarantees of model validity.";
    Warning(ss);
  }

  double functionalResidualCapacity_L;
  double computedFunctionalResidualCapacity_L = 30.0 * weight_kg / 1000.0;
  if (!m_Patient->HasFunctionalResidualCapacity()) {
    functionalResidualCapacity_L = computedFunctionalResidualCapacity_L;
    m_Patient->GetFunctionalResidualCapacity().SetValue(functionalResidualCapacity_L, VolumeUnit::L);
    ss << "No patient functional residual capacity set. Using a computed value of " << computedFunctionalResidualCapacity_L << " L.";
    Info(ss);
  }
  functionalResidualCapacity_L = m_Patient->GetFunctionalResidualCapacity(VolumeUnit::L);
  if (functionalResidualCapacity_L != computedFunctionalResidualCapacity_L) {
    ss << "Specified functional residual capacity of " << functionalResidualCapacity_L << " L differs from computed value of " << computedFunctionalResidualCapacity_L << " L. No guarantees of model validity.";
    Warning(ss);
  }

  double residualVolume_L;
  double computRedesidualVolume_L = 20.0 * weight_kg / 1000.0;
  if (!m_Patient->HasResidualVolume()) {
    residualVolume_L = computRedesidualVolume_L;
    m_Patient->GetResidualVolume().SetValue(residualVolume_L, VolumeUnit::L);
    ss << "No patient residual volume set. Using a computed value of " << computRedesidualVolume_L << " L.";
    Info(ss);
  }
  residualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  if (residualVolume_L != computRedesidualVolume_L) {
    ss << "Specified residual volume of " << residualVolume_L << " L differs from computed value of " << computRedesidualVolume_L << " L. No guarantees of model validity.";
    Warning(ss);
  }

  if (m_Patient->HasTidalVolumeBaseline()) {
    ss << "Patient tidal volume baseline cannot be set. It is determined through homeostatic simulation.";
    Error(ss);
    err = true;
  }
  if (m_Patient->HasVitalCapacity()) {
    ss << "Patient vital capacity cannot be set. It is directly computed via other lung volume patient parameters.";
    Error(ss);
    err = true;
  }
  if (m_Patient->HasExpiratoryReserveVolume()) {
    ss << "Patient expiratory reserve volume cannot be set. It is directly computed via other lung volume patient parameters.";
    Error(ss);
    err = true;
  }
  if (m_Patient->HasInspiratoryReserveVolume()) {
    ss << "Patient inspiratory reserve volume cannot be set. It is directly computed via other lung volume patient parameters.";
    Error(ss);
    err = true;
  }
  if (m_Patient->HasInspiratoryCapacity()) {
    ss << "Patient inspiratory capacity cannot be set. It is directly computed via other lung volume patient parameters.";
    Error(ss);
    err = true;
  }

  double tidalVolume_L = 7.0 * weight_kg / 1000.0;
  double targetVent_L_Per_min = tidalVolume_L * respirationRate_bpm;
  m_Patient->GetTotalVentilationBaseline().SetValue(targetVent_L_Per_min, VolumePerTimeUnit::L_Per_min);

  //Stabilization goes faster if we start the driver with a good amplitude that pushes blood gas levels to setpoint.
  //Based off testing, this relationship holds up well between RR = 12 and RR = 16 for Standard Male.
  double baselineDriverPressure_cmH2O = -6.2 + 0.25 * (respirationRate_bpm - 12.0);
  //Adjust driver pressure relationship for respiration rates > 16 (slope of driver - RR line decreases)
  if (respirationRate_bpm > 16.0) {
    //-5.2 = driver pressure at 16 bpm.
    baselineDriverPressure_cmH2O = -5.2 + 0.125 * (respirationRate_bpm - 16);
  }
  //Scale target pressure as ratio of calculated FRC to Standard Male FRC
  double standardFRC_L = 2.31332;
  baselineDriverPressure_cmH2O *= functionalResidualCapacity_L / standardFRC_L;

  BLIM(baselineDriverPressure_cmH2O, -6.5, -3.5);
  m_Patient->GetRespiratoryDriverAmplitudeBaseline().SetValue(baselineDriverPressure_cmH2O, PressureUnit::cmH2O);

  double vitalCapacity = totalLungCapacity_L - residualVolume_L;
  double expiratoryReserveVolume = functionalResidualCapacity_L - residualVolume_L;
  double inspiratoryReserveVolume = totalLungCapacity_L - functionalResidualCapacity_L - tidalVolume_L;
  double inspiratoryCapacity = totalLungCapacity_L - functionalResidualCapacity_L;
  //No negative volumes
  if (totalLungCapacity_L < 0.0 || functionalResidualCapacity_L < 0.0 || residualVolume_L < 0.0 || tidalVolume_L < 0.0 || vitalCapacity < 0.0 || expiratoryReserveVolume < 0.0 || inspiratoryReserveVolume < 0.0 || inspiratoryCapacity < 0.0) {
    ss << "All patient lung volumes must be positive.";
    Error(ss);
    err = true;
  }
  m_Patient->GetTidalVolumeBaseline().SetValue(tidalVolume_L, VolumeUnit::L); //This is overwritten after stabilization
  ss << "Patient tidal volume computed and set to " << tidalVolume_L << " L.";
  Info(ss);

  m_Patient->GetVitalCapacity().SetValue(vitalCapacity, VolumeUnit::L);
  ss << "Patient vital capacity computed and set to " << vitalCapacity << " L.";
  Info(ss);

  m_Patient->GetExpiratoryReserveVolume().SetValue(expiratoryReserveVolume, VolumeUnit::L);
  ss << "Patient expiratory reserve volume computed and set to " << expiratoryReserveVolume << " L.";
  Info(ss);

  m_Patient->GetInspiratoryReserveVolume().SetValue(inspiratoryReserveVolume, VolumeUnit::L);
  ss << "Patient inspiratory reserve volume computed and set to " << inspiratoryReserveVolume << " L.";
  Info(ss);

  m_Patient->GetInspiratoryCapacity().SetValue(inspiratoryCapacity, VolumeUnit::L);
  ss << "Patient inspiratory capacity computed and set to " << inspiratoryCapacity << " L.";
  Info(ss);

  //Alveoli Surface Area ---------------------------------------------------------------
  /// \cite roberts2000gaseous
  double standardAlveoliSurfaceArea_m2 = 70.0;
  double alveoliSurfaceArea_m2;
  //Scale the alveoli surface area based on the size of the patient’s lungs
  /// cite ganong1995review
  double standardTotalLungCapacity_L = 6.17; //This is the Total Lung Capacity of our standard patient
  double computedAlveoliSurfaceArea_m2 = totalLungCapacity_L / standardTotalLungCapacity_L * standardAlveoliSurfaceArea_m2;
  if (!m_Patient->HasAlveoliSurfaceArea()) {
    alveoliSurfaceArea_m2 = computedAlveoliSurfaceArea_m2;
    m_Patient->GetAlveoliSurfaceArea().SetValue(alveoliSurfaceArea_m2, AreaUnit::m2);
    ss << "No patient alveoli surface area set. Using a computed value of " << computedAlveoliSurfaceArea_m2 << " m^2.";
    Info(ss);
  }
  alveoliSurfaceArea_m2 = m_Patient->GetAlveoliSurfaceArea(AreaUnit::m2);
  if (alveoliSurfaceArea_m2 != computedAlveoliSurfaceArea_m2) {
    ss << "Specified alveoli surface area of " << alveoliSurfaceArea_m2 << " m^2 differs from computed value of " << computedAlveoliSurfaceArea_m2 << " m^2. No guarantees of model validity.";
    Warning(ss);
  }

  //Skin Surface Area ---------------------------------------------------------------
  /// \cite du1989formula
  double skinSurfaceArea_m2;
  double computSkinSurfaceArea_m2 = 0.20247 * std::pow(weight_kg, 0.425) * std::pow(Convert(height_cm, LengthUnit::cm, LengthUnit::m), 0.725);
  if (!m_Patient->HasSkinSurfaceArea()) {
    skinSurfaceArea_m2 = computSkinSurfaceArea_m2;
    m_Patient->GetSkinSurfaceArea().SetValue(skinSurfaceArea_m2, AreaUnit::m2);
    ss << "No patient skin surface area set. Using a computed value of " << computSkinSurfaceArea_m2 << " m^2.";
    Info(ss);
  }
  skinSurfaceArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);
  if (skinSurfaceArea_m2 != computSkinSurfaceArea_m2) {
    ss << "Specified skin surface area of " << skinSurfaceArea_m2 << " cm differs from computed value of " << computSkinSurfaceArea_m2 << " cm. No guarantees of model validity.";
    Warning(ss);
  }

  //Basal Metabolic Rate ---------------------------------------------------------------
  //The basal metabolic rate is determined from the Harris-Benedict formula, with differences dependent on sex, age, height and mass
  /// \cite roza1984metabolic
  double BMR_kcal_Per_day;
  double computBMR_kcal_Per_day = 88.632 + 13.397 * weight_kg + 4.799 * height_cm - 5.677 * age_yr; //Male
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    computBMR_kcal_Per_day = 447.593 + 9.247 * weight_kg + 3.098 * height_cm - 4.330 * age_yr; //Female
  }
  if (!m_Patient->HasBasalMetabolicRate()) {
    BMR_kcal_Per_day = computBMR_kcal_Per_day;
    m_Patient->GetBasalMetabolicRate().SetValue(BMR_kcal_Per_day, PowerUnit::kcal_Per_day);

    ss << "No patient basal metabolic rate set. Using a computed value of " << computBMR_kcal_Per_day << " kcal/day.";
    Info(ss);
  }
  BMR_kcal_Per_day = m_Patient->GetBasalMetabolicRate(PowerUnit::kcal_Per_day);
  if (BMR_kcal_Per_day != computBMR_kcal_Per_day) {
    ss << "Specified basal metabolic rate of " << BMR_kcal_Per_day << " kcal/day differs from computed value of " << computBMR_kcal_Per_day << " kcal/day. No guarantees of model validity.";
    Warning(ss);
  }

  //Maximum Work Rate ---------------------------------------------------------------
  //The max work rate is determined from linear regressions of research by Plowman et al., with differences dependent on sex and age
  /// \cite plowman2013exercise
  double maxWorkRate_W;
  double computedMaxWorkRate_W;

  if (m_Patient->GetGender() == CDM::enumSex::Male) {
    if (age_yr >= 60.) {
      computedMaxWorkRate_W = ((-24.3 * 60.) + 2070.);
    } else {
      computedMaxWorkRate_W = ((-24.3 * age_yr) + 2070.);
    }
  } else {
    if (age_yr >= 60.) {
      computedMaxWorkRate_W = ((-20.7 * 60.) + 1673.);
    } else {
      computedMaxWorkRate_W = ((-20.7 * age_yr) + 1673.);
    }
  }

  if (!m_Patient->HasMaxWorkRate()) {
    maxWorkRate_W = computedMaxWorkRate_W;
    m_Patient->GetMaxWorkRate().SetValue(maxWorkRate_W, PowerUnit::W);

    ss << "No patient maximum work rate set. Using a computed value of " << computedMaxWorkRate_W << " Watts.";
    Info(ss);
  }
  maxWorkRate_W = m_Patient->GetMaxWorkRate(PowerUnit::W);
  if (maxWorkRate_W != computedMaxWorkRate_W) {
    ss << "Specified maximum work rate of " << maxWorkRate_W << " Watts differs from computed value of " << computedMaxWorkRate_W << " Watts. No guarantees of model validity.";
    Warning(ss);
  }

  if (err) {
    return false;
  }
  return true;
}

BioGears::~BioGears()
{
  if (myLogger) {
    SAFE_DELETE(m_Logger);
  } else { //Turn off forwarding for this logger
    m_Logger->SetForward(nullptr);
  }
}

EngineState BioGears::GetState() { return m_State; }
SaturationCalculator& BioGears::GetSaturationCalculator() { return *m_SaturationCalculator; }
DiffusionCalculator& BioGears::GetDiffusionCalculator() { return *m_DiffusionCalculator; }
BioGearsSubstances& BioGears::GetSubstances() { return *m_Substances; }
SEPatient& BioGears::GetPatient() { return *m_Patient; }
SEBloodChemistrySystem& BioGears::GetBloodChemistry() { return *m_BloodChemistrySystem; }
SECardiovascularSystem& BioGears::GetCardiovascular() { return *m_CardiovascularSystem; }
SEDrugSystem& BioGears::GetDrugs() { return *m_DrugSystem; }
SEEndocrineSystem& BioGears::GetEndocrine() { return *m_EndocrineSystem; }
SEEnergySystem& BioGears::GetEnergy() { return *m_EnergySystem; }
SEGastrointestinalSystem& BioGears::GetGastrointestinal() { return *m_GastrointestinalSystem; }
SEHepaticSystem& BioGears::GetHepatic() { return *m_HepaticSystem; }
SENervousSystem& BioGears::GetNervous() { return *m_NervousSystem; }
SERenalSystem& BioGears::GetRenal() { return *m_RenalSystem; }
SERespiratorySystem& BioGears::GetRespiratory() { return *m_RespiratorySystem; }
SETissueSystem& BioGears::GetTissue() { return *m_TissueSystem; }
SEEnvironment& BioGears::GetEnvironment() { return *m_Environment; }

const EngineState BioGears::GetState() const { return m_State; }
const SaturationCalculator& BioGears::GetSaturationCalculator() const { return *m_SaturationCalculator; }
const DiffusionCalculator& BioGears::GetDiffusionCalculator() const { return *m_DiffusionCalculator; }
const BioGearsSubstances& BioGears::GetSubstances() const { return *m_Substances; }
const SEPatient& BioGears::GetPatient() const { return *m_Patient; }
const SEBloodChemistrySystem& BioGears::GetBloodChemistry() const { return *m_BloodChemistrySystem; }
const SECardiovascularSystem& BioGears::GetCardiovascular() const { return *m_CardiovascularSystem; }
const SEDrugSystem& BioGears::GetDrugs() const { return *m_DrugSystem; }
const SEEndocrineSystem& BioGears::GetEndocrine() const { return *m_EndocrineSystem; }
const SEEnergySystem& BioGears::GetEnergy() const { return *m_EnergySystem; }
const SEGastrointestinalSystem& BioGears::GetGastrointestinal() const { return *m_GastrointestinalSystem; }
const SEHepaticSystem& BioGears::GetHepatic() const { return *m_HepaticSystem; }
const SENervousSystem& BioGears::GetNervous() const { return *m_NervousSystem; }
const SERenalSystem& BioGears::GetRenal() const { return *m_RenalSystem; }
const SERespiratorySystem& BioGears::GetRespiratory() const { return *m_RespiratorySystem; }
const SETissueSystem& BioGears::GetTissue() const { return *m_TissueSystem; }
const SEEnvironment& BioGears::GetEnvironment() const { return *m_Environment; }

SEAnesthesiaMachine& BioGears::GetAnesthesiaMachine() { return *m_AnesthesiaMachine; }
SEElectroCardioGram& BioGears::GetECG() { return *m_ECG; }
SEInhaler& BioGears::GetInhaler() { return *m_Inhaler; }
SEActionManager& BioGears::GetActions() { return *m_Actions; }
SEConditionManager& BioGears::GetConditions() { return *m_Conditions; }
BioGearsCircuits& BioGears::GetCircuits() { return *m_Circuits; }
BioGearsCompartments& BioGears::GetCompartments() { return *m_Compartments; }
const BioGearsConfiguration& BioGears::GetConfiguration() { return *m_Config; }
const SEScalarTime& BioGears::GetEngineTime() { return *m_CurrentTime; }
const SEScalarTime& BioGears::GetSimulationTime() { return *m_SimulationTime; }
const SEScalarTime& BioGears::GetTimeStep() { return m_Config->GetTimeStep(); }
CDM::enumBioGearsAirwayMode::value BioGears::GetAirwayMode() { return m_AirwayMode; }
CDM::enumOnOff::value BioGears::GetIntubation() { return m_Intubation; }

const SEAnesthesiaMachine& BioGears::GetAnesthesiaMachine() const { return *m_AnesthesiaMachine; }
const SEElectroCardioGram& BioGears::GetECG() const { return *m_ECG; }
const SEInhaler& BioGears::GetInhaler() const { return *m_Inhaler; }
const SEActionManager& BioGears::GetActions() const { return *m_Actions; }
const SEConditionManager& BioGears::GetConditions() const { return *m_Conditions; }
const BioGearsCircuits& BioGears::GetCircuits() const { return *m_Circuits; }
const BioGearsCompartments& BioGears::GetCompartments() const { return *m_Compartments; }
const BioGearsConfiguration& BioGears::GetConfiguration() const { return *m_Config; }
const SEScalarTime& BioGears::GetEngineTime() const { return *m_CurrentTime; }
const SEScalarTime& BioGears::GetSimulationTime() const { return *m_SimulationTime; }
const SEScalarTime& BioGears::GetTimeStep() const { return m_Config->GetTimeStep(); }
const CDM::enumBioGearsAirwayMode::value BioGears::GetAirwayMode() const { return m_AirwayMode; }
const CDM::enumOnOff::value BioGears::GetIntubation() const { return m_Intubation; }

void BioGears::AtSteadyState(EngineState state)
{
  m_State = state;
  m_Environment->AtSteadyState();
  m_CardiovascularSystem->AtSteadyState();
  m_Inhaler->AtSteadyState();
  m_RespiratorySystem->AtSteadyState();
  m_AnesthesiaMachine->AtSteadyState();
  m_GastrointestinalSystem->AtSteadyState();
  m_HepaticSystem->AtSteadyState();
  m_RenalSystem->AtSteadyState();
  m_EnergySystem->AtSteadyState();
  m_EndocrineSystem->AtSteadyState();
  m_DrugSystem->AtSteadyState();
  m_TissueSystem->AtSteadyState();
  m_BloodChemistrySystem->AtSteadyState();
  m_NervousSystem->AtSteadyState();
  m_ECG->AtSteadyState();
}

void BioGears::PreProcess()
{
  m_Environment->PreProcess();
  m_CardiovascularSystem->PreProcess();
  m_Inhaler->PreProcess();
  m_RespiratorySystem->PreProcess();
  m_AnesthesiaMachine->PreProcess();
  m_GastrointestinalSystem->PreProcess();
  m_HepaticSystem->PreProcess();
  m_RenalSystem->PreProcess();
  m_EnergySystem->PreProcess();
  m_EndocrineSystem->PreProcess();
  m_DrugSystem->PreProcess();
  m_TissueSystem->PreProcess();
  m_BloodChemistrySystem->PreProcess();
  m_NervousSystem->PreProcess();
  m_ECG->PreProcess();
}
void BioGears::Process()
{
  m_Environment->Process();
  m_CardiovascularSystem->Process();
  m_Inhaler->Process();
  m_RespiratorySystem->Process();
  m_AnesthesiaMachine->Process();
  m_GastrointestinalSystem->Process();
  m_HepaticSystem->Process();
  m_RenalSystem->Process();
  m_EnergySystem->Process();
  m_EndocrineSystem->Process();
  m_DrugSystem->Process();
  m_TissueSystem->Process();
  m_BloodChemistrySystem->Process();
  m_NervousSystem->Process();
  m_ECG->Process();
}
void BioGears::PostProcess()
{
  m_Environment->PostProcess();
  m_CardiovascularSystem->PostProcess();
  m_Inhaler->PostProcess();
  m_RespiratorySystem->PostProcess();
  m_AnesthesiaMachine->PostProcess();
  m_GastrointestinalSystem->PostProcess();
  m_HepaticSystem->PostProcess();
  m_RenalSystem->PostProcess();
  m_EnergySystem->PostProcess();
  m_EndocrineSystem->PostProcess();
  m_DrugSystem->PostProcess();
  m_TissueSystem->PostProcess();
  m_BloodChemistrySystem->PostProcess();
  m_NervousSystem->PostProcess();
  m_ECG->PostProcess();
}

bool BioGears::GetPatientAssessment(SEPatientAssessment& assessment)
{
  SEPulmonaryFunctionTest* pft = dynamic_cast<SEPulmonaryFunctionTest*>(&assessment);
  if (pft != nullptr) {
    return m_RespiratorySystem->CalculatePulmonaryFunctionTest(*pft);
  }

  SECompleteBloodCount* cbc = dynamic_cast<SECompleteBloodCount*>(&assessment);
  if (cbc != nullptr) {
    return m_BloodChemistrySystem->CalculateCompleteBloodCount(*cbc);
  }

  SEComprehensiveMetabolicPanel* cmp = dynamic_cast<SEComprehensiveMetabolicPanel*>(&assessment);
  if (cmp != nullptr) {
    return m_BloodChemistrySystem->CalculateComprehensiveMetabolicPanel(*cmp);
  }

  SEUrinalysis* u = dynamic_cast<SEUrinalysis*>(&assessment);
  if (u != nullptr) {
    return m_RenalSystem->CalculateUrinalysis(*u);
  }

  Error("Unsupported patient assessment");
  return false;
}

void BioGears::ForwardFatal(const std::string& msg, const std::string& origin)
{
  std::string err;
  err.append(msg);
  err.append(" ");
  err.append(origin);
  throw PhysiologyEngineException(err);
}

bool BioGears::CreateCircuitsAndCompartments()
{
  m_Circuits->Clear();
  m_Compartments->Clear();

  SetupCardiovascular();
  if (m_Config->IsCerebralEnabled()) {
    SetupCerebral();
  }
  if (m_Config->IsRenalEnabled()) {
    SetupRenal();
  }
  if (m_Config->IsTissueEnabled()) {
    SetupTissue();
  }
  SetupGastrointestinal();

  ///////////////////////////////////////////////////////////////////
  // Create and Combine External and Internal Temperature Circuits //
  ///////////////////////////////////////////////////////////////////
  SetupExternalTemperature();
  SetupInternalTemperature();
  SEThermalCircuit& cThermal = m_Circuits->GetTemperatureCircuit();
  SEThermalCircuit& CInthermal = m_Circuits->GetInternalTemperatureCircuit();
  SEThermalCircuit& cExthermal = m_Circuits->GetExternalTemperatureCircuit();
  cThermal.AddCircuit(CInthermal);
  cThermal.AddCircuit(cExthermal);
  SEThermalCircuitNode* exCore = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalCore);
  SEThermalCircuitNode* exSkin = cExthermal.GetNode(BGE::ExternalTemperatureNode::ExternalSkin);
  SEThermalCircuitNode* inCore = CInthermal.GetNode(BGE::InternalTemperatureNode::InternalCore);
  SEThermalCircuitNode* inSkin = CInthermal.GetNode(BGE::InternalTemperatureNode::InternalSkin);
  SEThermalCircuitPath& CoreTemperatureConnection = cThermal.CreatePath(*inCore, *exCore, BGE::CombinedTemperaturePath::InternalCoreToExternalCore);
  SEThermalCircuitPath& SkinTemperatureConnection = cThermal.CreatePath(*inSkin, *exSkin, BGE::CombinedTemperaturePath::InternalSkinToExternalSkin);
  cThermal.SetNextAndCurrentFromBaselines();
  cThermal.StateChange();

  SEThermalCompartment* cExCore = m_Compartments->GetThermalCompartment(BGE::TemperatureCompartment::ExternalCore);
  SEThermalCompartment* cExSkin = m_Compartments->GetThermalCompartment(BGE::TemperatureCompartment::ExternalSkin);
  SEThermalCompartment* cInCore = m_Compartments->GetThermalCompartment(BGE::TemperatureCompartment::InternalCore);
  SEThermalCompartment* cInSkin = m_Compartments->GetThermalCompartment(BGE::TemperatureCompartment::InternalSkin);
  SEThermalCompartmentLink& InternalCoreToExternalCore = m_Compartments->CreateThermalLink(*cInCore, *cExCore, BGE::TemperatureLink::InternalCoreToExternalCore);
  InternalCoreToExternalCore.MapPath(CoreTemperatureConnection);
  SEThermalCompartmentLink& InternalSkinToExternalSkin = m_Compartments->CreateThermalLink(*cInSkin, *cExSkin, BGE::TemperatureLink::InternalSkinToExternalSkin);
  InternalSkinToExternalSkin.MapPath(SkinTemperatureConnection);

  // This node is shared between the respiratory, anesthesia, and inhaler circuits
  SEFluidCircuitNode& Ambient = m_Circuits->CreateFluidNode(BGE::EnvironmentNode::Ambient);
  Ambient.GetNextVolume().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  Ambient.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::L);
  SEGasCompartment& gEnvironment = m_Compartments->CreateGasCompartment(BGE::EnvironmentCompartment::Ambient);
  gEnvironment.MapNode(Ambient);
  SELiquidCompartment& lEnvironment = m_Compartments->CreateLiquidCompartment(BGE::EnvironmentCompartment::Ambient);
  lEnvironment.MapNode(Ambient);

  m_Environment->Initialize();
  CDM_COPY((&m_Config->GetInitialEnvironmentalConditions()), (&m_Environment->GetConditions()));
  m_Environment->StateChange();
  // Update the environment pressures on all the 'air' circuits to match what the environment was set to
  gEnvironment.GetPressure().Set(m_Environment->GetConditions().GetAtmosphericPressure());

  SetupRespiratory();
  SetupAnesthesiaMachine();
  SetupInhaler();
  SetupMechanicalVentilator();

  m_Compartments->StateChange();
  return true;
}

void BioGears::SetupCardiovascular()
{
  Info("Setting Up Cardiovascular");
  bool male = m_Patient->GetGender() == CDM::enumSex::Male ? true : false;
  double RightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1 - RightLungRatio;
  double bloodVolume_mL = m_Patient->GetBloodVolumeBaseline(VolumeUnit::mL);

  double systolicPressureTarget_mmHg = m_Patient->GetSystolicArterialPressureBaseline(PressureUnit::mmHg);
  double heartRate_bpm = m_Patient->GetHeartRateBaseline(FrequencyUnit::Per_min);
  double strokeVolumeTarget_mL = 81.0; //Note:  Had set this to 87 in previous commit when vascular->interstitium resistances were different
  double cardiacOutputTarget_mL_Per_s = heartRate_bpm / 60.0 * strokeVolumeTarget_mL;
  double diastolicPressureTarget_mmHg = 80.0;
  double centralVenousPressureTarget_mmHg = 4.0;
  double pulmonaryShuntFractionFactor = 0.009; // Used to set the pulmonary shunt fraction. Actual shunt will be roughly double this value (two lungs).
  // The way this works is we compute resistances and compliances based on the hemodynamic variables above that are either in the patient
  // file or we use the defaults if nothing is there. Because the actual impedance depends on the frequency, the computations assume a resting heart rate.
  // So if a user needs to put pressures in the patient file assuming that the pts baseline hr is in the normal range (around 72).
  // If someone wants a patient with a high hr because s/he is exercising or something, then they need to use the action.
  // If a user wants a patient with a ridiculously high resting hr, then they will need to estimate what the pressures and CO would be if the pt had a normal resting hr.

  // We compute a tuning modifier to adjust some baseline resistances and compliances to get closer to the target systolic and diastolic pressures from the patient file
  // The tuning method in cardiovascular will do the fine tuning. This just speeds up the process.
  /// \todo Make these a function of the systolic and diastolic pressure by fitting a curve to the data from the variations test
  double systemicResistanceModifier = 0.849;
  double largeArteriesComplianceModifier = 0.4333;

  // Volume fractions and flow rates from \cite valtin1995renal
  // Pressure targets derived from information available in \cite guyton2006medical and \cite van2013davis
  double VolumeFractionAorta = 0.05, VascularPressureTargetAorta = 1.0 * systolicPressureTarget_mmHg, VascularFlowTargetAorta = 1.0 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionArmLeft = 0.01, VascularPressureTargetArmLeft = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetArmLeft = male ? 0.00724 * cardiacOutputTarget_mL_Per_s : 0.0083 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionArmRight = VolumeFractionArmLeft, VascularPressureTargetArmRight = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetArmRight = VascularFlowTargetArmLeft;
  double VolumeFractionBone = 0.07, VascularPressureTargetBone = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetBone = 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionBrain = 0.012, VascularPressureTargetBrain = 0.08 * systolicPressureTarget_mmHg, VascularFlowTargetBrain = 0.12 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionFat = male ? 0.05 : 0.085, VascularPressureTargetFat = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetFat = male ? 0.05 * cardiacOutputTarget_mL_Per_s : 0.085 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionHeartLeft = 0.0025, VascularPressureTargetHeartLeft = 1.06667 * systolicPressureTarget_mmHg; /*No flow targets heart right*/
  double VolumeFractionHeartRight = 0.0025, VascularPressureTargetHeartRight = 0.16667 * systolicPressureTarget_mmHg; /*No flow targets heart left*/
  double VolumeFractionKidney = 0.019, VascularPressureTargetKidney = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetKidney = male ? 0.098 * cardiacOutputTarget_mL_Per_s : 0.088 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLargeIntestine = 0.019, VascularPressureTargetLargeIntestine = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetLargeIntestine = male ? 0.04 * cardiacOutputTarget_mL_Per_s : 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLegLeft = 0.0151, VascularPressureTargetLegLeft = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetLegLeft = male ? 0.01086 * cardiacOutputTarget_mL_Per_s : 0.01245 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionLegRight = VolumeFractionLegLeft, VascularPressureTargetLegRight = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetLegRight = VascularFlowTargetLegLeft;
  double VolumeFractionLiver = 0.106, VascularPressureTargetLiver = 0.25 * systolicPressureTarget_mmHg, VascularFlowTargetLiver = 0.075 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionMuscle = male ? 0.14 : 0.105, VascularPressureTargetMuscle = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetMuscle = male ? 0.17 * cardiacOutputTarget_mL_Per_s : 0.12 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionMyocardium = 0.007, VascularPressureTargetMyocardium = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetMyocardium = male ? 0.04 * cardiacOutputTarget_mL_Per_s : 0.05 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionPulmArtRight = 0.034 * RightLungRatio, VascularPressureTargetPulmArtRight = 0.13333 * systolicPressureTarget_mmHg, VascularFlowTargetPulmArtRight = RightLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmCapRight = 0.023 * RightLungRatio, VascularPressureTargetPulmCapRight = 0.0650 * systolicPressureTarget_mmHg, VascularFlowTargetPulmCapRight = RightLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmVeinsRight = 0.068 * RightLungRatio, VascularPressureTargetPulmVeinsRight = 0.03846 * systolicPressureTarget_mmHg, VascularFlowTargetPulmVeinsRight = RightLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmArtLeft = 0.034 * LeftLungRatio, VascularPressureTargetPulmArtLeft = 0.13333 * systolicPressureTarget_mmHg, VascularFlowTargetPulmArtLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmCapLeft = 0.023 * LeftLungRatio, VascularPressureTargetPulmCapLeft = 0.0650 * systolicPressureTarget_mmHg, VascularFlowTargetPulmCapLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionPulmVeinsLeft = 0.068 * LeftLungRatio, VascularPressureTargetPulmVeinsLeft = 0.03846 * systolicPressureTarget_mmHg, VascularFlowTargetPulmVeinsLeft = LeftLungRatio * cardiacOutputTarget_mL_Per_s * (1 - pulmonaryShuntFractionFactor);
  double VolumeFractionSkin = 0.032, VascularPressureTargetSkin = 0.0833 * systolicPressureTarget_mmHg, VascularFlowTargetSkin = 0.067 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSmallIntestine = 0.038, VascularPressureTargetSmallIntestine = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSmallIntestine = male ? 0.1 * cardiacOutputTarget_mL_Per_s : 0.11 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSplanchnic = 0.0116, VascularPressureTargetSplanchnic = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSplanchnic = male ? 0.0258 * cardiacOutputTarget_mL_Per_s : 0.0255 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionSpleen = 0.014, VascularPressureTargetSpleen = 0.33 * systolicPressureTarget_mmHg, VascularFlowTargetSpleen = 0.03 * cardiacOutputTarget_mL_Per_s;
  double VolumeFractionVenaCava = 0.247, VascularPressureTargetVenaCava = 0.0333 * systolicPressureTarget_mmHg, VascularFlowTargetVenaCava = 1.0 * cardiacOutputTarget_mL_Per_s;
  /*Portal Vein is path only*/ double VascularPressureTargetPortalVein = 0.25 * systolicPressureTarget_mmHg, VascularFlowTargetPortalVein = VascularFlowTargetLargeIntestine + VascularFlowTargetSmallIntestine + VascularFlowTargetSplanchnic + VascularFlowTargetSpleen;

  // Compute resistances from mean flow rates and pressure targets
  double ResistanceAorta = (VascularPressureTargetHeartLeft - systolicPressureTarget_mmHg) / VascularFlowTargetAorta; /*No Downstream Resistance Aorta*/
  double ResistanceArmLeft = (systolicPressureTarget_mmHg - VascularPressureTargetArmLeft) / VascularFlowTargetArmLeft, ResistanceArmLeftVenous = (VascularPressureTargetArmLeft - VascularPressureTargetVenaCava) / VascularFlowTargetArmLeft;
  double ResistanceArmRight = ResistanceArmLeft, ResistanceArmRightVenous = ResistanceArmLeftVenous;
  double ResistanceBone = (systolicPressureTarget_mmHg - VascularPressureTargetBone) / VascularFlowTargetBone, ResistanceBoneVenous = (VascularPressureTargetBone - VascularPressureTargetVenaCava) / VascularFlowTargetBone;
  double ResistanceBrain = 0.94 * ((systolicPressureTarget_mmHg - VascularPressureTargetBrain) / VascularFlowTargetBrain), ResistanceBrainVenous = (VascularPressureTargetBrain - VascularPressureTargetVenaCava) / VascularFlowTargetBrain;
  double ResistanceFat = (systolicPressureTarget_mmHg - VascularPressureTargetFat) / VascularFlowTargetFat, ResistanceFatVenous = (VascularPressureTargetFat - VascularPressureTargetVenaCava) / VascularFlowTargetFat;
  double ResistanceHeartLeft = 0.000002; /*No Downstream Resistance HeartLeft*/
  double ResistanceHeartRight = (0.04225 * systolicPressureTarget_mmHg - VascularPressureTargetVenaCava) / cardiacOutputTarget_mL_Per_s; // Describes the flow resistance between the systemic vasculature and the right atrium    /*No Downstream Resistance Heart Right*/
  double ResistanceKidney = (systolicPressureTarget_mmHg - VascularPressureTargetKidney) / VascularFlowTargetKidney, ResistanceKidneyVenous = (VascularPressureTargetKidney - VascularPressureTargetVenaCava) / VascularFlowTargetKidney;
  double ResistanceLargeIntestine = (systolicPressureTarget_mmHg - VascularPressureTargetLargeIntestine) / VascularFlowTargetLargeIntestine, ResistanceLargeIntestineVenous = (VascularPressureTargetLargeIntestine - VascularPressureTargetLiver) / VascularFlowTargetLargeIntestine;
  double ResistanceLegLeft = (systolicPressureTarget_mmHg - VascularPressureTargetLegLeft) / VascularFlowTargetLegLeft, ResistanceLegLeftVenous = (VascularPressureTargetLegLeft - VascularPressureTargetVenaCava) / VascularFlowTargetLegLeft;
  double ResistanceLegRight = ResistanceLegLeft, ResistanceLegRightVenous = ResistanceLegLeftVenous;
  double ResistanceLiver = (systolicPressureTarget_mmHg - VascularPressureTargetLiver) / VascularFlowTargetLiver, ResistanceLiverVenous = (VascularPressureTargetLiver - VascularPressureTargetVenaCava) / (VascularFlowTargetLiver + VascularFlowTargetPortalVein);
  double ResistanceMuscle = (systolicPressureTarget_mmHg - VascularPressureTargetMuscle) / VascularFlowTargetMuscle, ResistanceMuscleVenous = (VascularPressureTargetMuscle - VascularPressureTargetVenaCava) / VascularFlowTargetMuscle;
  double ResistanceMyocardium = (systolicPressureTarget_mmHg - VascularPressureTargetMyocardium) / VascularFlowTargetMyocardium, ResistanceMyocardiumVenous = (VascularPressureTargetMyocardium - VascularPressureTargetVenaCava) / VascularFlowTargetMyocardium;
  double ResistancePulmArtRight = (VascularPressureTargetHeartRight - VascularPressureTargetPulmArtRight) / VascularFlowTargetPulmArtRight; /*No Downstream Resistance PulmArt*/
  double ResistancePulmCapRight = (VascularPressureTargetPulmArtRight - VascularPressureTargetPulmCapRight) / VascularFlowTargetPulmCapRight; /*No Downstream Resistance PulmCap*/
  double ResistancePulmVeinsRight = (VascularPressureTargetPulmCapRight - VascularPressureTargetPulmVeinsRight) / VascularFlowTargetPulmVeinsRight; /*No Downstream Resistance PulmVeins*/
  double ResistancePulmArtLeft = (VascularPressureTargetHeartRight - VascularPressureTargetPulmArtLeft) / VascularFlowTargetPulmArtLeft; /*No Downstream Resistance PulmArt*/
  double ResistancePulmCapLeft = (VascularPressureTargetPulmArtLeft - VascularPressureTargetPulmCapLeft) / VascularFlowTargetPulmCapLeft; /*No Downstream Resistance PulmCap*/
  double ResistancePulmVeinsLeft = (VascularPressureTargetPulmCapLeft - VascularPressureTargetPulmVeinsLeft) / VascularFlowTargetPulmVeinsLeft; /*No Downstream Resistance PulmVeins*/
  double ResistanceSkin = (systolicPressureTarget_mmHg - VascularPressureTargetSkin) / VascularFlowTargetSkin, ResistanceSkinVenous = (VascularPressureTargetSkin - VascularPressureTargetVenaCava) / VascularFlowTargetSkin;
  double ResistanceSmallIntestine = (systolicPressureTarget_mmHg - VascularPressureTargetSmallIntestine) / VascularFlowTargetSmallIntestine, ResistanceSmallIntestineVenous = (VascularPressureTargetSmallIntestine - VascularPressureTargetLiver) / VascularFlowTargetSmallIntestine;
  double ResistanceSplanchnic = (systolicPressureTarget_mmHg - VascularPressureTargetSplanchnic) / VascularFlowTargetSplanchnic, ResistanceSplanchnicVenous = (VascularPressureTargetSplanchnic - VascularPressureTargetLiver) / VascularFlowTargetSplanchnic;
  double ResistanceSpleen = (systolicPressureTarget_mmHg - VascularPressureTargetSpleen) / VascularFlowTargetSpleen, ResistanceSpleenVenous = (VascularPressureTargetSpleen - VascularPressureTargetLiver) / VascularFlowTargetSpleen;

  // Portal vein and shunt are just paths - only have resistance
  double ResistancePortalVein = 0.001; // The portal vein is just a pathway in BioGears. The pressure across this path does not represent portal vein pressure (if it did our patient would always be portal hypertensive)
  double ResistanceShuntRight = (VascularPressureTargetPulmArtRight - VascularPressureTargetPulmCapRight) / (cardiacOutputTarget_mL_Per_s * pulmonaryShuntFractionFactor);
  double ResistanceShuntLeft = (VascularPressureTargetPulmArtLeft - VascularPressureTargetPulmCapLeft) / (cardiacOutputTarget_mL_Per_s * pulmonaryShuntFractionFactor);

  // Make a circuit
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();

  // Create Nodes, set volume baselines and pressures where appropriate
  SEFluidCircuitNode& RightAtrium1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightAtrium1);
  RightAtrium1.GetVolumeBaseline().SetValue(VolumeFractionHeartRight * 0.45 * bloodVolume_mL, VolumeUnit::mL);
  RightAtrium1.GetPressure().SetValue(2.9, PressureUnit::mmHg);

  SEFluidCircuitNode& RightAtrium2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightAtrium2);
  RightAtrium2.GetPressure().SetValue(2.9 - ResistanceHeartRight * cardiacOutputTarget_mL_Per_s, PressureUnit::mmHg);

  SEFluidCircuitNode& RightVentricle1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightVentricle1);
  RightVentricle1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  RightVentricle1.GetVolumeBaseline().SetValue(VolumeFractionHeartRight * 0.55 * bloodVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& RightVentricle2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightVentricle2);
  RightVentricle2.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitNode& MainPulmonaryArteries = cCardiovascular.CreateNode(BGE::CardiovascularNode::MainPulmonaryArteries);

  SEFluidCircuitNode& RightIntermediatePulmonaryArteries = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightIntermediatePulmonaryArteries);
  SEFluidCircuitNode& RightPulmonaryArteries = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightPulmonaryArteries);
  RightPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeFractionPulmArtRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryArteries.GetPressure().SetValue(VascularPressureTargetPulmArtRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftIntermediatePulmonaryArteries = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftIntermediatePulmonaryArteries);
  SEFluidCircuitNode& LeftPulmonaryArteries = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftPulmonaryArteries);
  LeftPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeFractionPulmArtLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryArteries.GetPressure().SetValue(VascularPressureTargetPulmArtLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& RightPulmonaryCapillaries = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightPulmonaryCapillaries);
  RightPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeFractionPulmCapRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryCapillaries.GetPressure().SetValue(VascularPressureTargetPulmCapRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftPulmonaryCapillaries = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftPulmonaryCapillaries);
  LeftPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeFractionPulmCapLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryCapillaries.GetPressure().SetValue(VascularPressureTargetPulmCapLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& RightIntermediatePulmonaryVeins = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightIntermediatePulmonaryVeins);
  SEFluidCircuitNode& RightPulmonaryVeins = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightPulmonaryVeins);
  RightPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeFractionPulmVeinsRight * bloodVolume_mL, VolumeUnit::mL);
  RightPulmonaryVeins.GetPressure().SetValue(VascularPressureTargetPulmVeinsRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftIntermediatePulmonaryVeins = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftIntermediatePulmonaryVeins);
  SEFluidCircuitNode& LeftPulmonaryVeins = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftPulmonaryVeins);
  LeftPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeFractionPulmVeinsLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftPulmonaryVeins.GetPressure().SetValue(VascularPressureTargetPulmVeinsLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftAtrium1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftAtrium1);
  LeftAtrium1.GetVolumeBaseline().SetValue(VolumeFractionHeartLeft * 0.45 * bloodVolume_mL, VolumeUnit::mL);
  LeftAtrium1.GetPressure().SetValue(4.9, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftAtrium2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftAtrium2);
  LeftAtrium2.GetPressure().SetValue(4.9 - ResistanceHeartLeft * cardiacOutputTarget_mL_Per_s, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftVentricle1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftVentricle1);
  LeftVentricle1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  LeftVentricle1.GetVolumeBaseline().SetValue(VolumeFractionHeartLeft * 0.55 * bloodVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& LeftVentricle2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftVentricle2);
  LeftVentricle2.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  SEFluidCircuitNode& Aorta1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Aorta1);
  SEFluidCircuitNode& Aorta2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Aorta2);
  SEFluidCircuitNode& Aorta3 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Aorta3);
  Aorta1.GetVolumeBaseline().SetValue(VolumeFractionAorta * bloodVolume_mL, VolumeUnit::mL);
  Aorta1.GetPressure().SetValue(VascularPressureTargetAorta, PressureUnit::mmHg);

  SEFluidCircuitNode& Brain1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Brain1);
  SEFluidCircuitNode& Brain2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Brain2);
  Brain1.GetVolumeBaseline().SetValue(VolumeFractionBrain * bloodVolume_mL, VolumeUnit::mL);
  Brain1.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  Brain1.GetPressure().SetValue(VascularPressureTargetBrain, PressureUnit::mmHg);

  SEFluidCircuitNode& Bone1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Bone1);
  SEFluidCircuitNode& Bone2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Bone2);
  Bone1.GetVolumeBaseline().SetValue(VolumeFractionBone * bloodVolume_mL, VolumeUnit::mL);
  Bone1.GetPressure().SetValue(VascularPressureTargetBone, PressureUnit::mmHg);

  SEFluidCircuitNode& Fat1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Fat1);
  SEFluidCircuitNode& Fat2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Fat2);
  Fat1.GetVolumeBaseline().SetValue(VolumeFractionFat * bloodVolume_mL, VolumeUnit::mL);
  Fat1.GetPressure().SetValue(VascularPressureTargetFat, PressureUnit::mmHg);

  SEFluidCircuitNode& LargeIntestine = cCardiovascular.CreateNode(BGE::CardiovascularNode::LargeIntestine1);
  LargeIntestine.GetVolumeBaseline().SetValue(VolumeFractionLargeIntestine * bloodVolume_mL, VolumeUnit::mL);
  LargeIntestine.GetPressure().SetValue(VascularPressureTargetLargeIntestine, PressureUnit::mmHg);

  SEFluidCircuitNode& Liver1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Liver1);
  SEFluidCircuitNode& Liver2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Liver2);
  Liver1.GetVolumeBaseline().SetValue(VolumeFractionLiver * bloodVolume_mL, VolumeUnit::mL);
  Liver1.GetPressure().SetValue(VascularPressureTargetLiver, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftArm1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftArm1);
  SEFluidCircuitNode& LeftArm2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftArm2);
  LeftArm1.GetVolumeBaseline().SetValue(VolumeFractionArmLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftArm1.GetPressure().SetValue(VascularPressureTargetArmRight, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftKidney1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftKidney1);
  SEFluidCircuitNode& LeftKidney2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftKidney2);
  LeftKidney1.GetVolumeBaseline().SetValue(0.5 * VolumeFractionKidney * bloodVolume_mL, VolumeUnit::mL);
  LeftKidney1.GetPressure().SetValue(VascularPressureTargetKidney, PressureUnit::mmHg);

  SEFluidCircuitNode& LeftLeg1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftLeg1);
  SEFluidCircuitNode& LeftLeg2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::LeftLeg2);
  LeftLeg1.GetVolumeBaseline().SetValue(VolumeFractionLegLeft * bloodVolume_mL, VolumeUnit::mL);
  LeftLeg1.GetPressure().SetValue(VascularPressureTargetLegLeft, PressureUnit::mmHg);

  SEFluidCircuitNode& Muscle1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Muscle1);
  SEFluidCircuitNode& Muscle2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Muscle2);
  Muscle1.GetVolumeBaseline().SetValue(VolumeFractionMuscle * bloodVolume_mL, VolumeUnit::mL);
  Muscle1.GetPressure().SetValue(VascularPressureTargetMuscle, PressureUnit::mmHg);

  SEFluidCircuitNode& Myocardium1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Myocardium1);
  SEFluidCircuitNode& Myocardium2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Myocardium2);
  Myocardium1.GetVolumeBaseline().SetValue(VolumeFractionMyocardium * bloodVolume_mL, VolumeUnit::mL);
  Myocardium1.GetPressure().SetValue(VascularPressureTargetMyocardium, PressureUnit::mmHg);

  SEFluidCircuitNode& PortalVein = cCardiovascular.CreateNode(BGE::CardiovascularNode::PortalVein1);

  SEFluidCircuitNode& RightArm1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightArm1);
  SEFluidCircuitNode& RightArm2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightArm2);
  RightArm1.GetVolumeBaseline().SetValue(VolumeFractionArmRight * bloodVolume_mL, VolumeUnit::mL);
  RightArm1.GetPressure().SetValue(VascularPressureTargetArmRight, PressureUnit::mmHg);

  SEFluidCircuitNode& RightKidney1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightKidney1);
  SEFluidCircuitNode& RightKidney2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightKidney2);
  RightKidney1.GetVolumeBaseline().SetValue(0.5 * VolumeFractionKidney * bloodVolume_mL, VolumeUnit::mL);
  RightKidney1.GetPressure().SetValue(VascularPressureTargetKidney, PressureUnit::mmHg);

  SEFluidCircuitNode& RightLeg1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightLeg1);
  SEFluidCircuitNode& RightLeg2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::RightLeg2);
  RightLeg1.GetVolumeBaseline().SetValue(VolumeFractionLegRight * bloodVolume_mL, VolumeUnit::mL);
  RightLeg1.GetPressure().SetValue(VascularPressureTargetLegRight, PressureUnit::mmHg);

  SEFluidCircuitNode& Skin1 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Skin1);
  SEFluidCircuitNode& Skin2 = cCardiovascular.CreateNode(BGE::CardiovascularNode::Skin2);
  Skin1.GetVolumeBaseline().SetValue(VolumeFractionSkin * bloodVolume_mL, VolumeUnit::mL);
  Skin1.GetPressure().SetValue(VascularPressureTargetSkin, PressureUnit::mmHg);

  SEFluidCircuitNode& SmallIntestine = cCardiovascular.CreateNode(BGE::CardiovascularNode::SmallIntestine1);
  SmallIntestine.GetVolumeBaseline().SetValue(VolumeFractionSmallIntestine * bloodVolume_mL, VolumeUnit::mL);
  SmallIntestine.GetPressure().SetValue(VascularPressureTargetSmallIntestine, PressureUnit::mmHg);

  SEFluidCircuitNode& Splanchnic = cCardiovascular.CreateNode(BGE::CardiovascularNode::Splanchnic1);
  Splanchnic.GetVolumeBaseline().SetValue(VolumeFractionSplanchnic * bloodVolume_mL, VolumeUnit::mL);
  Splanchnic.GetPressure().SetValue(VascularPressureTargetSplanchnic, PressureUnit::mmHg);

  SEFluidCircuitNode& Spleen = cCardiovascular.CreateNode(BGE::CardiovascularNode::Spleen1);
  Spleen.GetVolumeBaseline().SetValue(VolumeFractionSpleen * bloodVolume_mL, VolumeUnit::mL);
  Spleen.GetPressure().SetValue(VascularPressureTargetSpleen, PressureUnit::mmHg);

  SEFluidCircuitNode& VenaCava = cCardiovascular.CreateNode(BGE::CardiovascularNode::VenaCava);
  VenaCava.GetVolumeBaseline().SetValue(VolumeFractionVenaCava * bloodVolume_mL, VolumeUnit::mL);
  VenaCava.GetPressure().SetValue(VascularPressureTargetVenaCava, PressureUnit::mmHg);

  SEFluidCircuitNode& Ground = cCardiovascular.CreateNode(BGE::CardiovascularNode::Ground);
  cCardiovascular.AddReferenceNode(Ground);
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  double blood_mL = 0;
  for (SEFluidCircuitNode* n : cCardiovascular.GetNodes()) {
    if (n->HasVolumeBaseline()) {
      blood_mL += n->GetVolumeBaseline(VolumeUnit::mL);
    }
  }
  if (blood_mL > bloodVolume_mL) {
    Error("Blood volume greater than total blood volume");
  }

  SEFluidCircuitNode& Pericardium = cCardiovascular.CreateNode(BGE::CardiovascularNode::Pericardium1);
  Pericardium.GetVolumeBaseline().SetValue(15.0, VolumeUnit::mL);
  Pericardium.GetPressure().SetValue(1.0, PressureUnit::mmHg);

  // Create Paths, set switch (diodes), compliances, and resistances where appropriate
  SEFluidCircuitPath& VenaCavaToRightAtrium1 = cCardiovascular.CreatePath(VenaCava, RightAtrium1, BGE::CardiovascularPath::VenaCavaToRightAtrium1);
  VenaCavaToRightAtrium1.GetResistanceBaseline().SetValue(ResistanceHeartRight, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightAtrium1ToGround = cCardiovascular.CreatePath(RightAtrium1, Ground, BGE::CardiovascularPath::RightAtrium1ToGround);
  RightAtrium1ToGround.GetComplianceBaseline().SetValue(1.0 / 0.06, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightAtrium1ToRightAtrium2 = cCardiovascular.CreatePath(RightAtrium1, RightAtrium2, BGE::CardiovascularPath::RightAtrium1ToRightAtrium2);
  RightAtrium1ToRightAtrium2.GetResistanceBaseline().SetValue(5.94e-5, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightAtrium2ToRightVentricle1 = cCardiovascular.CreatePath(RightAtrium2, RightVentricle1, BGE::CardiovascularPath::RightAtrium2ToRightVentricle1);
  RightAtrium2ToRightVentricle1.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& RightVentricle1ToRightVentricle2 = cCardiovascular.CreatePath(RightVentricle1, RightVentricle2, BGE::CardiovascularPath::RightVentricle1ToRightVentricle2);
  SEFluidCircuitPath& RightVentricle2ToGround = cCardiovascular.CreatePath(Ground, RightVentricle2, BGE::CardiovascularPath::RightVentricle2ToGround);
  RightVentricle2ToGround.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& RightVentricle1ToMainPulmonaryArteries = cCardiovascular.CreatePath(RightVentricle1, MainPulmonaryArteries, BGE::CardiovascularPath::RightVentricle1ToMainPulmonaryArteries);
  RightVentricle1ToMainPulmonaryArteries.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& MainPulmonaryArteriesToRightIntermediatePulmonaryArteries = cCardiovascular.CreatePath(MainPulmonaryArteries, RightIntermediatePulmonaryArteries, BGE::CardiovascularPath::MainPulmonaryArteriesToRightIntermediatePulmonaryArteries);
  //MainPulmonaryArteriesToRightIntermediatePulmonaryArteries.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& RightIntermediatePulmonaryArteriesToRightPulmonaryArteries = cCardiovascular.CreatePath(RightIntermediatePulmonaryArteries, RightPulmonaryArteries, BGE::CardiovascularPath::RightIntermediatePulmonaryArteriesToRightPulmonaryArteries);
  RightIntermediatePulmonaryArteriesToRightPulmonaryArteries.GetResistanceBaseline().SetValue(ResistancePulmArtLeft, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& RightPulmonaryArteriesToRightPulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryArteries, RightPulmonaryVeins, BGE::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryVeins);
  RightPulmonaryArteriesToRightPulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceShuntRight, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryArteriesToRightPulmonaryCapillaries = cCardiovascular.CreatePath(RightPulmonaryArteries, RightPulmonaryCapillaries, BGE::CardiovascularPath::RightPulmonaryArteriesToRightPulmonaryCapillaries);
  RightPulmonaryArteriesToRightPulmonaryCapillaries.GetResistanceBaseline().SetValue(ResistancePulmCapRight, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryArteriesToGround = cCardiovascular.CreatePath(RightPulmonaryArteries, Ground, BGE::CardiovascularPath::RightPulmonaryArteriesToGround);
  RightPulmonaryArteriesToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightPulmonaryCapillariesToRightPulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryCapillaries, RightPulmonaryVeins, BGE::CardiovascularPath::RightPulmonaryCapillariesToRightPulmonaryVeins);
  RightPulmonaryCapillariesToRightPulmonaryVeins.GetResistanceBaseline().SetValue(ResistancePulmVeinsRight, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryCapillariesToGround = cCardiovascular.CreatePath(RightPulmonaryCapillaries, Ground, BGE::CardiovascularPath::RightPulmonaryCapillariesToGround);
  RightPulmonaryCapillariesToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& RightPulmonaryVeinsToRightIntermediatePulmonaryVeins = cCardiovascular.CreatePath(RightPulmonaryVeins, RightIntermediatePulmonaryVeins, BGE::CardiovascularPath::RightPulmonaryVeinsToRightIntermediatePulmonaryVeins);
  RightPulmonaryVeinsToRightIntermediatePulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceHeartLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightPulmonaryVeinsToGround = cCardiovascular.CreatePath(RightPulmonaryVeins, Ground, BGE::CardiovascularPath::RightPulmonaryVeinsToGround);
  RightPulmonaryVeinsToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightIntermediatePulmonaryVeinsToLeftAtrium1 = cCardiovascular.CreatePath(RightIntermediatePulmonaryVeins, LeftAtrium1, BGE::CardiovascularPath::RightIntermediatePulmonaryVeinsToLeftAtrium1);
  //RightIntermediatePulmonaryVeinsToLeftAtrium2.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries = cCardiovascular.CreatePath(MainPulmonaryArteries, LeftIntermediatePulmonaryArteries, BGE::CardiovascularPath::MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries);
  //MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries = cCardiovascular.CreatePath(LeftIntermediatePulmonaryArteries, LeftPulmonaryArteries, BGE::CardiovascularPath::LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries);
  LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries.GetResistanceBaseline().SetValue(ResistancePulmArtLeft, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& LeftPulmonaryArteriesToLeftPulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryArteries, LeftPulmonaryVeins, BGE::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryVeins);
  LeftPulmonaryArteriesToLeftPulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceShuntLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryArteriesToLeftPulmonaryCapillaries = cCardiovascular.CreatePath(LeftPulmonaryArteries, LeftPulmonaryCapillaries, BGE::CardiovascularPath::LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
  LeftPulmonaryArteriesToLeftPulmonaryCapillaries.GetResistanceBaseline().SetValue(ResistancePulmCapLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryArteriesToGround = cCardiovascular.CreatePath(LeftPulmonaryArteries, Ground, BGE::CardiovascularPath::LeftPulmonaryArteriesToGround);
  LeftPulmonaryArteriesToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftPulmonaryCapillariesToGround = cCardiovascular.CreatePath(LeftPulmonaryCapillaries, Ground, BGE::CardiovascularPath::LeftPulmonaryCapillariesToGround);
  LeftPulmonaryCapillariesToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftPulmonaryCapillariesToLeftPulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryCapillaries, LeftPulmonaryVeins, BGE::CardiovascularPath::LeftPulmonaryCapillariesToLeftPulmonaryVeins);
  LeftPulmonaryCapillariesToLeftPulmonaryVeins.GetResistanceBaseline().SetValue(ResistancePulmVeinsLeft, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins = cCardiovascular.CreatePath(LeftPulmonaryVeins, LeftIntermediatePulmonaryVeins, BGE::CardiovascularPath::LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins);
  LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins.GetResistanceBaseline().SetValue(ResistanceHeartLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftPulmonaryVeinsToGround = cCardiovascular.CreatePath(LeftPulmonaryVeins, Ground, BGE::CardiovascularPath::LeftPulmonaryVeinsToGround);
  LeftPulmonaryVeinsToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftIntermediatePulmonaryVeinsToLeftAtrium1 = cCardiovascular.CreatePath(LeftIntermediatePulmonaryVeins, LeftAtrium1, BGE::CardiovascularPath::LeftIntermediatePulmonaryVeinsToLeftAtrium1);
  //LeftIntermediatePulmonaryVeinsToLeftAtrium2.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& LeftAtrium1ToGround = cCardiovascular.CreatePath(LeftAtrium1, Ground, BGE::CardiovascularPath::LeftAtrium1ToGround);
  LeftAtrium1ToGround.GetComplianceBaseline().SetValue(1.0 / 0.075, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftAtrium1ToLeftAtrium2 = cCardiovascular.CreatePath(LeftAtrium1, LeftAtrium2, BGE::CardiovascularPath::LeftAtrium1ToLeftAtrium2);
  LeftAtrium1ToLeftAtrium2.GetResistanceBaseline().SetValue(8.9e-5, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftAtrium2ToLeftVentricle1 = cCardiovascular.CreatePath(LeftAtrium2, LeftVentricle1, BGE::CardiovascularPath::LeftAtrium2ToLeftVentricle1);
  LeftAtrium2ToLeftVentricle1.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& LeftVentricle1ToLeftVentricle2 = cCardiovascular.CreatePath(LeftVentricle1, LeftVentricle2, BGE::CardiovascularPath::LeftVentricle1ToLeftVentricle2);
  SEFluidCircuitPath& LeftVentricle2ToGround = cCardiovascular.CreatePath(Ground, LeftVentricle2, BGE::CardiovascularPath::LeftVentricle2ToGround);
  LeftVentricle2ToGround.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftVentricle1ToAorta2 = cCardiovascular.CreatePath(LeftVentricle1, Aorta2, BGE::CardiovascularPath::LeftVentricle1ToAorta2);
  LeftVentricle1ToAorta2.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& Aorta2ToAorta3 = cCardiovascular.CreatePath(Aorta2, Aorta3, BGE::CardiovascularPath::Aorta2ToAorta3);
  SEFluidCircuitPath& Aorta3ToAorta1 = cCardiovascular.CreatePath(Aorta3, Aorta1, BGE::CardiovascularPath::Aorta3ToAorta1);
  Aorta3ToAorta1.GetResistanceBaseline().SetValue(ResistanceAorta, FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& Aorta1ToGround = cCardiovascular.CreatePath(Aorta1, Ground, BGE::CardiovascularPath::Aorta1ToGround);
  Aorta1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& Aorta1ToBrain1 = cCardiovascular.CreatePath(Aorta1, Brain1, BGE::CardiovascularPath::Aorta1ToBrain1);
  Aorta1ToBrain1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBrain, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToBrain1.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);
  SEFluidCircuitPath& Brain1ToGround = cCardiovascular.CreatePath(Brain1, Ground, BGE::CardiovascularPath::Brain1ToGround);
  Brain1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Brain1ToBrain2 = cCardiovascular.CreatePath(Brain1, Brain2, BGE::CardiovascularPath::Brain1ToBrain2);
  Brain1ToBrain2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBrainVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Brain1ToBrain2.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);
  SEFluidCircuitPath& Brain2ToVenaCava = cCardiovascular.CreatePath(Brain2, VenaCava, BGE::CardiovascularPath::Brain2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToBone1 = cCardiovascular.CreatePath(Aorta1, Bone1, BGE::CardiovascularPath::Aorta1ToBone1);
  Aorta1ToBone1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBone, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToBone1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Bone1ToGround = cCardiovascular.CreatePath(Bone1, Ground, BGE::CardiovascularPath::Bone1ToGround);
  Bone1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Bone1ToBone2 = cCardiovascular.CreatePath(Bone1, Bone2, BGE::CardiovascularPath::Bone1ToBone2);
  Bone1ToBone2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceBoneVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Bone1ToBone2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Bone2ToVenaCava = cCardiovascular.CreatePath(Bone2, VenaCava, BGE::CardiovascularPath::Bone2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToFat1 = cCardiovascular.CreatePath(Aorta1, Fat1, BGE::CardiovascularPath::Aorta1ToFat1);
  Aorta1ToFat1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceFat, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToFat1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Fat1ToGround = cCardiovascular.CreatePath(Fat1, Ground, BGE::CardiovascularPath::Fat1ToGround);
  Fat1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Fat1ToFat2 = cCardiovascular.CreatePath(Fat1, Fat2, BGE::CardiovascularPath::Fat1ToFat2);
  Fat1ToFat2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceFatVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Fat1ToFat2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Fat2ToVenaCava = cCardiovascular.CreatePath(Fat2, VenaCava, BGE::CardiovascularPath::Fat2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLargeIntestine = cCardiovascular.CreatePath(Aorta1, LargeIntestine, BGE::CardiovascularPath::Aorta1ToLargeIntestine);
  Aorta1ToLargeIntestine.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLargeIntestine, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToLargeIntestine.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& LargeIntestineToGround = cCardiovascular.CreatePath(LargeIntestine, Ground, BGE::CardiovascularPath::LargeIntestineToGround);
  LargeIntestineToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LargeIntestineToPortalVein = cCardiovascular.CreatePath(LargeIntestine, PortalVein, BGE::CardiovascularPath::LargeIntestineToPortalVein);
  LargeIntestineToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLargeIntestineVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  LargeIntestineToPortalVein.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);

  SEFluidCircuitPath& Aorta1ToLeftArm1 = cCardiovascular.CreatePath(Aorta1, LeftArm1, BGE::CardiovascularPath::Aorta1ToLeftArm1);
  Aorta1ToLeftArm1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToLeftArm1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftArm1ToGround = cCardiovascular.CreatePath(LeftArm1, Ground, BGE::CardiovascularPath::LeftArm1ToGround);
  LeftArm1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftArm1ToLeftArm2 = cCardiovascular.CreatePath(LeftArm1, LeftArm2, BGE::CardiovascularPath::LeftArm1ToLeftArm2);
  LeftArm1ToLeftArm2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmLeftVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  LeftArm1ToLeftArm2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftArm2ToVenaCava = cCardiovascular.CreatePath(LeftArm2, VenaCava, BGE::CardiovascularPath::LeftArm2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLeftKidney1 = cCardiovascular.CreatePath(Aorta1, LeftKidney1, BGE::CardiovascularPath::Aorta1ToLeftKidney1);
  Aorta1ToLeftKidney1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceKidney, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToLeftKidney1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftKidney1ToGround = cCardiovascular.CreatePath(LeftKidney1, Ground, BGE::CardiovascularPath::LeftKidney1ToGround);
  LeftKidney1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftKidney1ToLeftKidney2 = cCardiovascular.CreatePath(LeftKidney1, LeftKidney2, BGE::CardiovascularPath::LeftKidney1ToLeftKidney2);
  LeftKidney1ToLeftKidney2.GetResistanceBaseline().SetValue(ResistanceKidneyVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  LeftKidney1ToLeftKidney2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftKidney2ToVenaCava = cCardiovascular.CreatePath(LeftKidney2, VenaCava, BGE::CardiovascularPath::LeftKidney2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLeftLeg1 = cCardiovascular.CreatePath(Aorta1, LeftLeg1, BGE::CardiovascularPath::Aorta1ToLeftLeg1);
  Aorta1ToLeftLeg1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegLeft, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToLeftLeg1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftLeg1ToGround = cCardiovascular.CreatePath(LeftLeg1, Ground, BGE::CardiovascularPath::LeftLeg1ToGround);
  LeftLeg1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& LeftLeg1ToLeftLeg2 = cCardiovascular.CreatePath(LeftLeg1, LeftLeg2, BGE::CardiovascularPath::LeftLeg1ToLeftLeg2);
  LeftLeg1ToLeftLeg2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegLeftVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  LeftLeg1ToLeftLeg2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& LeftLeg2ToVenaCava = cCardiovascular.CreatePath(LeftLeg2, VenaCava, BGE::CardiovascularPath::LeftLeg2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToLiver1 = cCardiovascular.CreatePath(Aorta1, Liver1, BGE::CardiovascularPath::Aorta1ToLiver1);
  Aorta1ToLiver1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLiver, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToLiver1.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& Liver1ToGround = cCardiovascular.CreatePath(Liver1, Ground, BGE::CardiovascularPath::Liver1ToGround);
  Liver1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& PortalVeinToLiver1 = cCardiovascular.CreatePath(PortalVein, Liver1, BGE::CardiovascularPath::PortalVeinToLiver1);
  PortalVeinToLiver1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistancePortalVein, FlowResistanceUnit::mmHg_s_Per_mL);
  PortalVeinToLiver1.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& Liver1ToLiver2 = cCardiovascular.CreatePath(Liver1, Liver2, BGE::CardiovascularPath::Liver1ToLiver2);
  Liver1ToLiver2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLiverVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Liver1ToLiver2.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& Liver2ToVenaCava = cCardiovascular.CreatePath(Liver2, VenaCava, BGE::CardiovascularPath::Liver2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToMuscle1 = cCardiovascular.CreatePath(Aorta1, Muscle1, BGE::CardiovascularPath::Aorta1ToMuscle1);
  Aorta1ToMuscle1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMuscle, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToMuscle1.SetCardiovascularRegion(CDM::enumResistancePathType::Muscle);
  SEFluidCircuitPath& Muscle1ToGround = cCardiovascular.CreatePath(Muscle1, Ground, BGE::CardiovascularPath::Muscle1ToGround);
  Muscle1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Muscle1ToMuscle2 = cCardiovascular.CreatePath(Muscle1, Muscle2, BGE::CardiovascularPath::Muscle1ToMuscle2);
  Muscle1ToMuscle2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMuscleVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Muscle1ToMuscle2.SetCardiovascularRegion(CDM::enumResistancePathType::Muscle);
  SEFluidCircuitPath& Muscle2ToVenaCava = cCardiovascular.CreatePath(Muscle2, VenaCava, BGE::CardiovascularPath::Muscle2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToMyocardium1 = cCardiovascular.CreatePath(Aorta1, Myocardium1, BGE::CardiovascularPath::Aorta1ToMyocardium1);
  Aorta1ToMyocardium1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMyocardium, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToMyocardium1.SetCardiovascularRegion(CDM::enumResistancePathType::Ventricle);
  SEFluidCircuitPath& Myocardium1ToGround = cCardiovascular.CreatePath(Myocardium1, Ground, BGE::CardiovascularPath::Myocardium1ToGround);
  Myocardium1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Myocardium1ToMyocardium2 = cCardiovascular.CreatePath(Myocardium1, Myocardium2, BGE::CardiovascularPath::Myocardium1ToMyocardium2);
  Myocardium1ToMyocardium2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceMyocardiumVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Myocardium1ToMyocardium2.SetCardiovascularRegion(CDM::enumResistancePathType::Ventricle);
  SEFluidCircuitPath& Myocardium2ToVenaCava = cCardiovascular.CreatePath(Myocardium2, VenaCava, BGE::CardiovascularPath::Myocardium2ToVenaCava);

  SEFluidCircuitPath& PericardiumToGround = cCardiovascular.CreatePath(Pericardium, Ground, BGE::CardiovascularPath::Pericardium1ToGround);
  SEFluidCircuitPath& GroundToPericardium = cCardiovascular.CreatePath(Ground, Pericardium, BGE::CardiovascularPath::GroundToPericardium1);
  GroundToPericardium.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SEFluidCircuitPath& Aorta1ToRightArm1 = cCardiovascular.CreatePath(Aorta1, RightArm1, BGE::CardiovascularPath::Aorta1ToRightArm1);
  Aorta1ToRightArm1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmRight, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToRightArm1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightArm1ToGround = cCardiovascular.CreatePath(RightArm1, Ground, BGE::CardiovascularPath::RightArm1ToGround);
  RightArm1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightArm1ToRightArm2 = cCardiovascular.CreatePath(RightArm1, RightArm2, BGE::CardiovascularPath::RightArm1ToRightArm2);
  RightArm1ToRightArm2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceArmRightVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  RightArm1ToRightArm2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightArm2ToVenaCava = cCardiovascular.CreatePath(RightArm2, VenaCava, BGE::CardiovascularPath::RightArm2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToRightKidney1 = cCardiovascular.CreatePath(Aorta1, RightKidney1, BGE::CardiovascularPath::Aorta1ToRightKidney1);
  Aorta1ToRightKidney1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceKidney, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToRightKidney1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightKidney1ToGround = cCardiovascular.CreatePath(RightKidney1, Ground, BGE::CardiovascularPath::RightKidney1ToGround);
  RightKidney1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightKidney1ToRightKidney2 = cCardiovascular.CreatePath(RightKidney1, RightKidney2, BGE::CardiovascularPath::RightKidney1ToRightKidney2);
  RightKidney1ToRightKidney2.GetResistanceBaseline().SetValue(ResistanceKidneyVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  RightKidney1ToRightKidney2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightKidney2ToVenaCava = cCardiovascular.CreatePath(RightKidney2, VenaCava, BGE::CardiovascularPath::RightKidney2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToRightLeg1 = cCardiovascular.CreatePath(Aorta1, RightLeg1, BGE::CardiovascularPath::Aorta1ToRightLeg1);
  Aorta1ToRightLeg1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegRight, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToRightLeg1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightLeg1ToGround = cCardiovascular.CreatePath(RightLeg1, Ground, BGE::CardiovascularPath::RightLeg1ToGround);
  RightLeg1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightLeg1ToRightLeg2 = cCardiovascular.CreatePath(RightLeg1, RightLeg2, BGE::CardiovascularPath::RightLeg1ToRightLeg2);
  RightLeg1ToRightLeg2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceLegRightVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  RightLeg1ToRightLeg2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& RightLeg2ToVenaCava = cCardiovascular.CreatePath(RightLeg2, VenaCava, BGE::CardiovascularPath::RightLeg2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToSkin1 = cCardiovascular.CreatePath(Aorta1, Skin1, BGE::CardiovascularPath::Aorta1ToSkin1);
  Aorta1ToSkin1.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSkin, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToSkin1.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Skin1ToGround = cCardiovascular.CreatePath(Skin1, Ground, BGE::CardiovascularPath::Skin1ToGround);
  Skin1ToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& Skin1ToSkin2 = cCardiovascular.CreatePath(Skin1, Skin2, BGE::CardiovascularPath::Skin1ToSkin2);
  Skin1ToSkin2.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSkinVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  Skin1ToSkin2.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  SEFluidCircuitPath& Skin2ToVenaCava = cCardiovascular.CreatePath(Skin2, VenaCava, BGE::CardiovascularPath::Skin2ToVenaCava);

  SEFluidCircuitPath& Aorta1ToSmallIntestine = cCardiovascular.CreatePath(Aorta1, SmallIntestine, BGE::CardiovascularPath::Aorta1ToSmallIntestine);
  Aorta1ToSmallIntestine.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSmallIntestine, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToSmallIntestine.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& SmallIntestineToGround = cCardiovascular.CreatePath(SmallIntestine, Ground, BGE::CardiovascularPath::SmallIntestineToGround);
  SmallIntestineToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SmallIntestineToPortalVein = cCardiovascular.CreatePath(SmallIntestine, PortalVein, BGE::CardiovascularPath::SmallIntestineToPortalVein);
  SmallIntestineToPortalVein.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SmallIntestineToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSmallIntestineVenous, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& Aorta1ToSplanchnic = cCardiovascular.CreatePath(Aorta1, Splanchnic, BGE::CardiovascularPath::Aorta1ToSplanchnic);
  Aorta1ToSplanchnic.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSplanchnic, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToSplanchnic.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& SplanchnicToGround = cCardiovascular.CreatePath(Splanchnic, Ground, BGE::CardiovascularPath::SplanchnicToGround);
  SplanchnicToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SplanchnicToPortalVein = cCardiovascular.CreatePath(Splanchnic, PortalVein, BGE::CardiovascularPath::SplanchnicToPortalVein);
  SplanchnicToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSplanchnicVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  SplanchnicToPortalVein.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);

  SEFluidCircuitPath& Aorta1ToSpleen = cCardiovascular.CreatePath(Aorta1, Spleen, BGE::CardiovascularPath::Aorta1ToSpleen);
  Aorta1ToSpleen.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSpleen, FlowResistanceUnit::mmHg_s_Per_mL);
  Aorta1ToSpleen.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);
  SEFluidCircuitPath& SpleenToGround = cCardiovascular.CreatePath(Spleen, Ground, BGE::CardiovascularPath::SpleenToGround);
  SpleenToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& SpleenToPortalVein = cCardiovascular.CreatePath(Spleen, PortalVein, BGE::CardiovascularPath::SpleenToPortalVein);
  SpleenToPortalVein.GetResistanceBaseline().SetValue(systemicResistanceModifier * ResistanceSpleenVenous, FlowResistanceUnit::mmHg_s_Per_mL);
  SpleenToPortalVein.SetCardiovascularRegion(CDM::enumResistancePathType::Splanchnic);

  SEFluidCircuitPath& VenaCavaToGround = cCardiovascular.CreatePath(VenaCava, Ground, BGE::CardiovascularPath::VenaCavaToGround);
  VenaCavaToGround.GetComplianceBaseline().SetValue(0.0, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& VenaCavaBleed = cCardiovascular.CreatePath(VenaCava, Ground, BGE::CardiovascularPath::VenaCavaBleed);
  VenaCavaBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& IVToVenaCava = cCardiovascular.CreatePath(Ground, VenaCava, BGE::CardiovascularPath::IVToVenaCava);
  IVToVenaCava.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  //Hemorrhage--Include major organs, if there is a right/left side just adjust the resistance of the right side
  SEFluidCircuitPath& AortaBleed = cCardiovascular.CreatePath(Aorta1, Ground, BGE::CardiovascularPath::AortaBleed);
  AortaBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& BrainBleed = cCardiovascular.CreatePath(Brain1, Ground, BGE::CardiovascularPath::BrainBleed);
  BrainBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& HeartBleed = cCardiovascular.CreatePath(Myocardium1, Ground, BGE::CardiovascularPath::MyocardiumBleed);
  HeartBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftLungBleed = cCardiovascular.CreatePath(LeftPulmonaryArteries, Ground, BGE::CardiovascularPath::LeftLungBleed);
  LeftLungBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightLungBleed = cCardiovascular.CreatePath(RightPulmonaryArteries, Ground, BGE::CardiovascularPath::RightLungBleed);
  RightLungBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LeftArmBleed = cCardiovascular.CreatePath(LeftArm1, Ground, BGE::CardiovascularPath::LeftArmBleed);
  LeftArmBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightArmBleed = cCardiovascular.CreatePath(RightArm1, Ground, BGE::CardiovascularPath::RightArmBleed);
  RightArmBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SpleenBleed = cCardiovascular.CreatePath(Spleen, Ground, BGE::CardiovascularPath::SpleenBleed);
  SpleenBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SmallIntestineBleed = cCardiovascular.CreatePath(SmallIntestine, Ground, BGE::CardiovascularPath::SmallIntestineBleed);
  SmallIntestineBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LargeIntestineBleed = cCardiovascular.CreatePath(LargeIntestine, Ground, BGE::CardiovascularPath::LargeIntestineBleed);
  LargeIntestineBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& SplanchnicBleed = cCardiovascular.CreatePath(Splanchnic, Ground, BGE::CardiovascularPath::SplanchnicBleed);
  SplanchnicBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& LiverBleed = cCardiovascular.CreatePath(Liver1, Ground, BGE::CardiovascularPath::LiverBleed);
  LiverBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& PortalBleed = cCardiovascular.CreatePath(PortalVein, Ground, BGE::CardiovascularPath::PortalBleed);
  PortalBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_min_Per_L);
  //Note: kidney hemorrhage path is set below in renal circuit
  SEFluidCircuitPath& LeftLegBleed = cCardiovascular.CreatePath(LeftLeg1, Ground, BGE::CardiovascularPath::LeftLegBleed);
  LeftLegBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  SEFluidCircuitPath& RightLegBleed = cCardiovascular.CreatePath(RightLeg1, Ground, BGE::CardiovascularPath::RightLegBleed);
  RightLegBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);

  // Compute compliances from target pressures and baseline volumes
  for (SEFluidCircuitPath* p : cCardiovascular.GetPaths()) {
    if (p->HasCapacitanceBaseline()) {
      SEFluidCircuitNode& src = p->GetSourceNode();
      if (!src.HasVolumeBaseline()) {
        Fatal("Compliance paths must have a volume baseline.");
      }
      double pressure = src.GetPressure(PressureUnit::mmHg);
      double volume = src.GetVolumeBaseline(VolumeUnit::mL);
      p->GetComplianceBaseline().SetValue(volume / pressure, FlowComplianceUnit::mL_Per_mmHg);
    }
  }
  // The vena cava compliance needs to be decreased to ensure proper return
  double venaCavaComplianceTuning = 1.0;
  VenaCavaToGround.GetCapacitanceBaseline().SetValue(venaCavaComplianceTuning * VenaCavaToGround.GetComplianceBaseline().GetValue(FlowComplianceUnit::mL_Per_mmHg), FlowComplianceUnit::mL_Per_mmHg);

  // Hearts and pericardium have special compliance computations
  double InitialComplianceHeartRight = 1.0 / 0.0243;
  double InitialComplianceHeartLeft = 1.0 / 0.049;
  // Volumes are initialized from the volume baselines. The heart volume initialization is a little tricky. To much prime and the
  // initial pressure wave will be devastating to the rest of the CV system during the first contraction phase. Too little prime
  // and there will be issues with available flow as the elastance decreases during the first relaxation phase.
  // The 1/4 full initialization gives decent results.
  // RightHeart1ToRightHeart3.GetComplianceBaseline().SetValue(InitialComplianceHeartRight, FlowComplianceUnit::mL_Per_mmHg);
  //LeftHeart1ToLeftHeart3.GetComplianceBaseline().SetValue(InitialComplianceHeartLeft, FlowComplianceUnit::mL_Per_mmHg);
  LeftVentricle1ToLeftVentricle2.GetComplianceBaseline().SetValue(1.0 / 0.049, FlowComplianceUnit::mL_Per_mmHg);
  RightVentricle1ToRightVentricle2.GetComplianceBaseline().SetValue(1.0 / 0.0243, FlowComplianceUnit::mL_Per_mmHg);
  PericardiumToGround.GetComplianceBaseline().SetValue(100.0, FlowComplianceUnit::mL_Per_mmHg);

  double VolumeModifierAorta = 1.16722 * 1.018749, VolumeModifierBrain = 0.998011 * 1.038409, VolumeModifierBone = 1.175574 * 0.985629, VolumeModifierFat = 1.175573 * 0.986527;
  double VolumeModifierLargeIntestine = 1.17528 * 0.985609, VolumeModifierArmL = 1.175573 * 0.986529, VolumeModifierKidneyL = 0.737649 * 0.954339, VolumeModifierLegL = 1.175573 * 0.986529;
  double VolumeModifierPulmArtL = 0.855566 * 1.095697, VolumeModifierPulmCapL = 0.724704 * 1.079139, VolumeModifierPulmVeinL = 0.548452 * 1.056844 * 1.062, VolumeModifierLiver = 1.157475 * 0.991848;
  double VolumeModifierMuscle = 1.175573 * 0.986529, VolumeModifierMyocard = 1.175564 * 0.986531, VolumeModifierArmR = 1.175573 * 0.986529, VolumeModifierKidneyR = 0.737649 * 0.954339;
  double VolumeModifierLegR = 1.175573 * 0.986529, VolumeModifierPulmArtR = 0.756158 * 1.121167, VolumeModifierPulmCapR = 0.602545 * 1.118213, VolumeModifierPulmVeinR = 0.395656 * 1.11424 * 1.11;
  double VolumeModifierSkin = 1.007306 * 1.035695, VolumeModifierSmallIntestine = 1.17528 * 0.986509, VolumeModifierSplanchnic = 1.17528 * 0.986509, VolumeModifierSpleen = 1.17528 * 0.986509;
  double VolumeModifierVenaCava = 0.66932 * 1.134447;

  //And also modify the compliances
  Aorta1ToGround.GetComplianceBaseline().SetValue(largeArteriesComplianceModifier * Aorta1ToGround.GetComplianceBaseline(FlowComplianceUnit::mL_Per_mmHg), FlowComplianceUnit::mL_Per_mmHg);

  RightPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeModifierPulmArtR * RightPulmonaryArteries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryArteries.GetVolumeBaseline().SetValue(VolumeModifierPulmArtL * LeftPulmonaryArteries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeModifierPulmCapR * RightPulmonaryCapillaries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryCapillaries.GetVolumeBaseline().SetValue(VolumeModifierPulmCapL * LeftPulmonaryCapillaries.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeModifierPulmVeinR * RightPulmonaryVeins.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftPulmonaryVeins.GetVolumeBaseline().SetValue(VolumeModifierPulmVeinL * LeftPulmonaryVeins.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Aorta1.GetVolumeBaseline().SetValue(VolumeModifierAorta * Aorta1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Brain1.GetVolumeBaseline().SetValue(VolumeModifierBrain * Brain1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Bone1.GetVolumeBaseline().SetValue(VolumeModifierBone * Bone1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Fat1.GetVolumeBaseline().SetValue(VolumeModifierFat * Fat1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LargeIntestine.GetVolumeBaseline().SetValue(VolumeModifierLargeIntestine * LargeIntestine.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Liver1.GetVolumeBaseline().SetValue(VolumeModifierLiver * Liver1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftArm1.GetVolumeBaseline().SetValue(VolumeModifierArmL * LeftArm1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftKidney1.GetVolumeBaseline().SetValue(VolumeModifierKidneyL * LeftKidney1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  LeftLeg1.GetVolumeBaseline().SetValue(VolumeModifierLegL * LeftLeg1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Muscle1.GetVolumeBaseline().SetValue(VolumeModifierMuscle * Muscle1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Myocardium1.GetVolumeBaseline().SetValue(VolumeModifierMyocard * Myocardium1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightArm1.GetVolumeBaseline().SetValue(VolumeModifierArmR * RightArm1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightKidney1.GetVolumeBaseline().SetValue(VolumeModifierKidneyR * RightKidney1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  RightLeg1.GetVolumeBaseline().SetValue(VolumeModifierLegR * RightLeg1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Skin1.GetVolumeBaseline().SetValue(VolumeModifierSkin * Skin1.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  SmallIntestine.GetVolumeBaseline().SetValue(VolumeModifierSmallIntestine * SmallIntestine.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Splanchnic.GetVolumeBaseline().SetValue(VolumeModifierSplanchnic * Splanchnic.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  Spleen.GetVolumeBaseline().SetValue(VolumeModifierSpleen * Spleen.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);
  VenaCava.GetVolumeBaseline().SetValue(VolumeModifierVenaCava * VenaCava.GetVolumeBaseline(VolumeUnit::mL), VolumeUnit::mL);

  // Prepare circuit for compartment creation
  cCardiovascular.SetNextAndCurrentFromBaselines();
  cCardiovascular.StateChange();

  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  cCombinedCardiovascular.AddCircuit(cCardiovascular);
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  /////////////////////////
  // Create Compartments //
  /////////////////////////

  /////////////////
  // Right Heart //
  SELiquidCompartment& vRightAtrium = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightAtrium);
  vRightAtrium.MapNode(RightAtrium1);
  vRightAtrium.MapNode(RightAtrium2);
  SELiquidCompartment& vRightVentricle = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightVentricle);
  vRightVentricle.MapNode(RightVentricle1);
  vRightVentricle.MapNode(RightVentricle2);
  vRightVentricle.MapNode(MainPulmonaryArteries);
  //////////////////////////////
  // Right Pulmonary Arteries //
  SELiquidCompartment& vRightPulmonaryArteries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightPulmonaryArteries);
  vRightPulmonaryArteries.MapNode(RightPulmonaryArteries);
  vRightPulmonaryArteries.MapNode(RightIntermediatePulmonaryArteries);
  /////////////////////////////
  // Left Pulmonary Arteries //
  SELiquidCompartment& vLeftPulmonaryArteries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryArteries);
  vLeftPulmonaryArteries.MapNode(LeftPulmonaryArteries);
  vLeftPulmonaryArteries.MapNode(LeftIntermediatePulmonaryArteries);
  ////////////////////////
  // Pulmonary Arteries //
  SELiquidCompartment& vPulmonaryArteries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::PulmonaryArteries);
  vPulmonaryArteries.AddChild(vRightPulmonaryArteries);
  vPulmonaryArteries.AddChild(vLeftPulmonaryArteries);
  /////////////////////////////////
  // Right Pulmonary Capillaries //
  SELiquidCompartment& vRightPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightPulmonaryCapillaries);
  vRightPulmonaryCapillaries.MapNode(RightPulmonaryCapillaries);
  ////////////////////////////////
  // Left Pulmonary Capillaries //
  SELiquidCompartment& vLeftPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryCapillaries);
  vLeftPulmonaryCapillaries.MapNode(LeftPulmonaryCapillaries);
  ///////////////////////////
  // Pulmonary Capillaries //
  SELiquidCompartment& vPulmonaryCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::PulmonaryCapillaries);
  vPulmonaryCapillaries.AddChild(vRightPulmonaryCapillaries);
  vPulmonaryCapillaries.AddChild(vLeftPulmonaryCapillaries);
  ///////////////////////////
  // Right Pulmonary Veins //
  SELiquidCompartment& vRightPulmonaryVeins = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightPulmonaryVeins);
  vRightPulmonaryVeins.MapNode(RightPulmonaryVeins);
  //////////////////////////
  // Left Pulmonary Veins //
  SELiquidCompartment& vLeftPulmonaryVeins = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftPulmonaryVeins);
  vLeftPulmonaryVeins.MapNode(LeftPulmonaryVeins);
  /////////////////////
  // Pulmonary Veins //
  SELiquidCompartment& vPulmonaryVeins = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::PulmonaryVeins);
  vPulmonaryVeins.AddChild(vRightPulmonaryVeins);
  vPulmonaryVeins.AddChild(vLeftPulmonaryVeins);
  ////////////////
  // Left Heart //
  SELiquidCompartment& vLeftAtrium = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftAtrium);
  vLeftAtrium.MapNode(LeftAtrium1);
  vLeftAtrium.MapNode(LeftAtrium2);
  SELiquidCompartment& vLeftVentricle = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftVentricle);
  vLeftVentricle.MapNode(LeftVentricle1);
  vLeftVentricle.MapNode(LeftVentricle2);
  ///////////
  // Aorta //
  SELiquidCompartment& vAorta = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Aorta);
  vAorta.MapNode(Aorta1);
  vAorta.MapNode(Aorta2);
  vAorta.MapNode(Aorta3);
  ///////////
  // Brain //
  SELiquidCompartment& vBrain = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Brain);
  vBrain.MapNode(Brain1);
  vBrain.MapNode(Brain2);
  //////////
  // Bone //
  SELiquidCompartment& vBone = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Bone);
  vBone.MapNode(Bone1);
  vBone.MapNode(Bone2);
  /////////
  // Fat //
  SELiquidCompartment& vFat = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Fat);
  vFat.MapNode(Fat1);
  vFat.MapNode(Fat2);
  /////////////////////
  // Large Intestine //
  SELiquidCompartment& vLargeIntestine = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LargeIntestine);
  vLargeIntestine.MapNode(LargeIntestine);
  ///////////
  // Liver //
  SELiquidCompartment& vLiver = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Liver);
  vLiver.MapNode(Liver1);
  vLiver.MapNode(Liver2);
  vLiver.MapNode(PortalVein);
  //////////////
  // Left Arm //
  SELiquidCompartment& vLeftArm = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftArm);
  vLeftArm.MapNode(LeftArm1);
  vLeftArm.MapNode(LeftArm2);
  /////////////////
  // Left Kidney //
  SELiquidCompartment& vLeftKidney = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftKidney);
  vLeftKidney.MapNode(LeftKidney1);
  vLeftKidney.MapNode(LeftKidney2);
  //////////////
  // Left Leg //
  SELiquidCompartment& vLeftLeg = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftLeg);
  vLeftLeg.MapNode(LeftLeg1);
  vLeftLeg.MapNode(LeftLeg2);
  ////////////
  // Muscle //
  SELiquidCompartment& vMuscle = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Muscle);
  vMuscle.MapNode(Muscle1);
  vMuscle.MapNode(Muscle2);
  ////////////////
  // Myocardium //
  SELiquidCompartment& vMyocardium = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Myocardium);
  vMyocardium.MapNode(Myocardium1);
  vMyocardium.MapNode(Myocardium2);
  /////////////////
  // Pericardium //
  SELiquidCompartment& vPericardium = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Pericardium);
  vPericardium.MapNode(Pericardium);
  ///////////////
  // Right Arm //
  SELiquidCompartment& vRightArm = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightArm);
  vRightArm.MapNode(RightArm1);
  vRightArm.MapNode(RightArm2);
  //////////////////
  // Right Kidney //
  SELiquidCompartment& vRightKidney = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightKidney);
  vRightKidney.MapNode(RightKidney1);
  vRightKidney.MapNode(RightKidney2);
  ///////////////
  // Right Leg //
  SELiquidCompartment& vRightLeg = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightLeg);
  vRightLeg.MapNode(RightLeg1);
  vRightLeg.MapNode(RightLeg2);
  //////////
  // Skin //
  SELiquidCompartment& vSkin = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Skin);
  vSkin.MapNode(Skin1);
  vSkin.MapNode(Skin2);
  /////////////////////
  // Small Intestine //
  SELiquidCompartment& vSmallIntestine = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::SmallIntestine);
  vSmallIntestine.MapNode(SmallIntestine);
  ////////////////
  // Splanchnic //
  SELiquidCompartment& vSplanchnic = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Splanchnic);
  vSplanchnic.MapNode(Splanchnic);
  ////////////
  // Spleen //
  SELiquidCompartment& vSpleen = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Spleen);
  vSpleen.MapNode(Spleen);
  //////////////
  // VenaCava //
  SELiquidCompartment& vVenaCava = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::VenaCava);
  vVenaCava.MapNode(VenaCava);
  ////////////
  // Ground //
  SELiquidCompartment& vGround = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Ground);
  vGround.MapNode(Ground);

  //////////////////////////
  // Set up our hierarchy //
  //////////////////////////
  SELiquidCompartment& vKidneys = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Kidneys);
  vKidneys.AddChild(vLeftKidney);
  vKidneys.AddChild(vRightKidney);
  SELiquidCompartment& vRightHeart = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightHeart);
  vRightHeart.AddChild(vRightAtrium);
  vRightHeart.AddChild(vRightVentricle);
  SELiquidCompartment& vLeftHeart = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftHeart);
  vLeftHeart.AddChild(vLeftAtrium);
  vLeftHeart.AddChild(vRightAtrium);
  SELiquidCompartment& vHeart = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Heart);
  vHeart.AddChild(vMyocardium);
  vHeart.AddChild(vLeftHeart);
  vHeart.AddChild(vRightHeart);
  vHeart.AddChild(vPericardium);
  SELiquidCompartment& vLeftLung = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftLung);
  vLeftLung.AddChild(vLeftPulmonaryArteries);
  vLeftLung.AddChild(vLeftPulmonaryCapillaries);
  vLeftLung.AddChild(vLeftPulmonaryVeins);
  SELiquidCompartment& vRightLung = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightLung);
  vRightLung.AddChild(vRightPulmonaryArteries);
  vRightLung.AddChild(vRightPulmonaryCapillaries);
  vRightLung.AddChild(vRightPulmonaryVeins);
  SELiquidCompartment& vLungs = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Lungs);
  vLungs.AddChild(vLeftLung);
  vLungs.AddChild(vRightLung);
  SELiquidCompartment& vGut = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::Gut);
  vGut.AddChild(vSplanchnic);
  vGut.AddChild(vSmallIntestine);
  vGut.AddChild(vLargeIntestine);

  //////////////////
  // Create Links //
  //////////////////

  /////////////////////
  // Heart and Lungs //
  SELiquidCompartmentLink& vVenaCavaToRightAtrium = m_Compartments->CreateLiquidLink(vVenaCava, vRightAtrium, BGE::VascularLink::VenaCavaToRightAtrium);
  //vVenaCavaToRightHeart.MapPath(VenaCavaToRightHeart2);
  vVenaCavaToRightAtrium.MapPath(VenaCavaToRightAtrium1);
  SELiquidCompartmentLink& vRightAtriumToRightVentricle = m_Compartments->CreateLiquidLink(vRightAtrium, vRightVentricle, BGE::VascularLink::RightAtriumToRightVentricle);
  vRightAtriumToRightVentricle.MapPath(RightAtrium2ToRightVentricle1);
  SELiquidCompartmentLink& vRightVentricleToLeftPulmonaryArteries = m_Compartments->CreateLiquidLink(vRightVentricle, vLeftPulmonaryArteries, BGE::VascularLink::RightVentricleToLeftPulmonaryArteries);
  vRightVentricleToLeftPulmonaryArteries.MapPath(MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries);
  SELiquidCompartmentLink& vLeftPulmonaryArteriesToCapillaries = m_Compartments->CreateLiquidLink(vLeftPulmonaryArteries, vLeftPulmonaryCapillaries, BGE::VascularLink::LeftPulmonaryArteriesToCapillaries);
  vLeftPulmonaryArteriesToCapillaries.MapPath(LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
  SELiquidCompartmentLink& vLeftPulmonaryArteriesToVeins = m_Compartments->CreateLiquidLink(vLeftPulmonaryArteries, vLeftPulmonaryVeins, BGE::VascularLink::LeftPulmonaryArteriesToVeins);
  vLeftPulmonaryArteriesToVeins.MapPath(LeftPulmonaryArteriesToLeftPulmonaryVeins);
  SELiquidCompartmentLink& vLeftPulmonaryCapillariesToVeins = m_Compartments->CreateLiquidLink(vLeftPulmonaryCapillaries, vLeftPulmonaryVeins, BGE::VascularLink::LeftPulmonaryCapillariesToVeins);
  vLeftPulmonaryCapillariesToVeins.MapPath(LeftPulmonaryCapillariesToLeftPulmonaryVeins);
  SELiquidCompartmentLink& vLeftPulmonaryVeinsToLeftAtrium = m_Compartments->CreateLiquidLink(vLeftPulmonaryVeins, vLeftAtrium, BGE::VascularLink::LeftPulmonaryVeinsToLeftAtrium);
  vLeftPulmonaryVeinsToLeftAtrium.MapPath(LeftIntermediatePulmonaryVeinsToLeftAtrium1);
  SELiquidCompartmentLink& vRightVentricleToRightPulmonaryArteries = m_Compartments->CreateLiquidLink(vRightVentricle, vRightPulmonaryArteries, BGE::VascularLink::RightVentricleToRightPulmonaryArteries);
  vRightVentricleToRightPulmonaryArteries.MapPath(MainPulmonaryArteriesToRightIntermediatePulmonaryArteries);
  SELiquidCompartmentLink& vRightPulmonaryArteriesToCapillaries = m_Compartments->CreateLiquidLink(vRightPulmonaryArteries, vRightPulmonaryCapillaries, BGE::VascularLink::RightPulmonaryArteriesToCapillaries);
  vRightPulmonaryArteriesToCapillaries.MapPath(RightPulmonaryArteriesToRightPulmonaryCapillaries);
  SELiquidCompartmentLink& vRightPulmonaryArteriesToVeins = m_Compartments->CreateLiquidLink(vRightPulmonaryArteries, vRightPulmonaryVeins, BGE::VascularLink::RightPulmonaryArteriesToVeins);
  vRightPulmonaryArteriesToVeins.MapPath(RightPulmonaryArteriesToRightPulmonaryVeins);
  SELiquidCompartmentLink& vRightPulmonaryCapillariesToVeins = m_Compartments->CreateLiquidLink(vRightPulmonaryCapillaries, vRightPulmonaryVeins, BGE::VascularLink::RightPulmonaryCapillariesToVeins);
  vRightPulmonaryCapillariesToVeins.MapPath(RightPulmonaryCapillariesToRightPulmonaryVeins);
  SELiquidCompartmentLink& vRightPulmonaryVeinsToLeftAtrium = m_Compartments->CreateLiquidLink(vRightPulmonaryVeins, vLeftAtrium, BGE::VascularLink::RightPulmonaryVeinsToLeftAtrium);
  vRightPulmonaryVeinsToLeftAtrium.MapPath(RightIntermediatePulmonaryVeinsToLeftAtrium1);
  SELiquidCompartmentLink& vLeftAtriumToLeftVentricle = m_Compartments->CreateLiquidLink(vLeftAtrium, vLeftVentricle, BGE::VascularLink::LeftAtriumToLeftVentricle);
  vLeftAtriumToLeftVentricle.MapPath(LeftAtrium2ToLeftVentricle1);
  SELiquidCompartmentLink& vLeftVentricleToAorta = m_Compartments->CreateLiquidLink(vLeftVentricle, vAorta, BGE::VascularLink::LeftVentricleToAorta);
  vLeftVentricleToAorta.MapPath(LeftVentricle1ToAorta2);
  //////////
  // Bone //
  SELiquidCompartmentLink& vAortaToBone = m_Compartments->CreateLiquidLink(vAorta, vBone, BGE::VascularLink::AortaToBone);
  vAortaToBone.MapPath(Aorta1ToBone1);
  SELiquidCompartmentLink& vBoneToVenaCava = m_Compartments->CreateLiquidLink(vBone, vVenaCava, BGE::VascularLink::BoneToVenaCava);
  vBoneToVenaCava.MapPath(Bone2ToVenaCava);
  ///////////
  // Brain //
  SELiquidCompartmentLink& vAortaToBrain = m_Compartments->CreateLiquidLink(vAorta, vBrain, BGE::VascularLink::AortaToBrain);
  vAortaToBrain.MapPath(Aorta1ToBrain1);
  SELiquidCompartmentLink& vBrainToVenaCava = m_Compartments->CreateLiquidLink(vBrain, vVenaCava, BGE::VascularLink::BrainToVenaCava);
  vBrainToVenaCava.MapPath(Brain2ToVenaCava);
  /////////
  // Fat //
  SELiquidCompartmentLink& vAortaToFat = m_Compartments->CreateLiquidLink(vAorta, vFat, BGE::VascularLink::AortaToFat);
  vAortaToFat.MapPath(Aorta1ToFat1);
  SELiquidCompartmentLink& vFatToVenaCava = m_Compartments->CreateLiquidLink(vFat, vVenaCava, BGE::VascularLink::FatToVenaCava);
  vFatToVenaCava.MapPath(Fat2ToVenaCava);
  /////////////////////
  // Large Intestine //
  SELiquidCompartmentLink& vAortaToLargeIntestine = m_Compartments->CreateLiquidLink(vAorta, vLargeIntestine, BGE::VascularLink::AortaToLargeIntestine);
  vAortaToLargeIntestine.MapPath(Aorta1ToLargeIntestine);
  SELiquidCompartmentLink& vLargeIntestineToLiver = m_Compartments->CreateLiquidLink(vLargeIntestine, vLiver, BGE::VascularLink::LargeIntestineToLiver);
  vLargeIntestineToLiver.MapPath(LargeIntestineToPortalVein);
  ///////////
  // Liver //
  SELiquidCompartmentLink& vAortaToLiver = m_Compartments->CreateLiquidLink(vAorta, vLiver, BGE::VascularLink::AortaToLiver);
  vAortaToLiver.MapPath(Aorta1ToLiver1);
  SELiquidCompartmentLink& vLiverToVenaCava = m_Compartments->CreateLiquidLink(vLiver, vVenaCava, BGE::VascularLink::LiverToVenaCava);
  vLiverToVenaCava.MapPath(Liver2ToVenaCava);
  //////////////
  // Left Arm //
  SELiquidCompartmentLink& vAortaToLeftArm = m_Compartments->CreateLiquidLink(vAorta, vLeftArm, BGE::VascularLink::AortaToLeftArm);
  vAortaToLeftArm.MapPath(Aorta1ToLeftArm1);
  SELiquidCompartmentLink& vLeftArmToVenaCava = m_Compartments->CreateLiquidLink(vLeftArm, vVenaCava, BGE::VascularLink::LeftArmToVenaCava);
  vLeftArmToVenaCava.MapPath(LeftArm2ToVenaCava);
  /////////////////
  // Left Kidney //
  SELiquidCompartmentLink& vAortaToLeftKidney = m_Compartments->CreateLiquidLink(vAorta, vLeftKidney, BGE::VascularLink::AortaToLeftKidney);
  vAortaToLeftKidney.MapPath(Aorta1ToLeftKidney1);
  SELiquidCompartmentLink& vLeftKidneyToVenaCava = m_Compartments->CreateLiquidLink(vLeftKidney, vVenaCava, BGE::VascularLink::LeftKidneyToVenaCava);
  vLeftKidneyToVenaCava.MapPath(LeftKidney2ToVenaCava);
  //////////////
  // Left Leg //
  SELiquidCompartmentLink& vAortaToLeftLeg = m_Compartments->CreateLiquidLink(vAorta, vLeftLeg, BGE::VascularLink::AortaToLeftLeg);
  vAortaToLeftLeg.MapPath(Aorta1ToLeftLeg1);
  SELiquidCompartmentLink& vLeftLegToVenaCava = m_Compartments->CreateLiquidLink(vLeftLeg, vVenaCava, BGE::VascularLink::LeftLegToVenaCava);
  vLeftLegToVenaCava.MapPath(LeftLeg2ToVenaCava);
  ////////////
  // Muscle //
  SELiquidCompartmentLink& vAortaToMuscle = m_Compartments->CreateLiquidLink(vAorta, vMuscle, BGE::VascularLink::AortaToMuscle);
  vAortaToMuscle.MapPath(Aorta1ToMuscle1);
  SELiquidCompartmentLink& vMuscleToVenaCava = m_Compartments->CreateLiquidLink(vMuscle, vVenaCava, BGE::VascularLink::MuscleToVenaCava);
  vMuscleToVenaCava.MapPath(Muscle2ToVenaCava);
  ////////////////
  // Myocardium //
  SELiquidCompartmentLink& vAortaToMyocardium = m_Compartments->CreateLiquidLink(vAorta, vMyocardium, BGE::VascularLink::AortaToMyocardium);
  vAortaToMyocardium.MapPath(Aorta1ToMyocardium1);
  SELiquidCompartmentLink& vMyocardiumToVenaCava = m_Compartments->CreateLiquidLink(vMyocardium, vVenaCava, BGE::VascularLink::MyocardiumToVenaCava);
  vMyocardiumToVenaCava.MapPath(Myocardium2ToVenaCava);
  ///////////////
  // Right Arm //
  SELiquidCompartmentLink& vAortaToRightArm = m_Compartments->CreateLiquidLink(vAorta, vRightArm, BGE::VascularLink::AortaToRightArm);
  vAortaToRightArm.MapPath(Aorta1ToRightArm1);
  SELiquidCompartmentLink& vRightArmToVenaCava = m_Compartments->CreateLiquidLink(vRightArm, vVenaCava, BGE::VascularLink::RightArmToVenaCava);
  vRightArmToVenaCava.MapPath(RightArm2ToVenaCava);
  //////////////////
  // Right Kidney //
  SELiquidCompartmentLink& vAortaToRightKidney = m_Compartments->CreateLiquidLink(vAorta, vRightKidney, BGE::VascularLink::AortaToRightKidney);
  vAortaToRightKidney.MapPath(Aorta1ToRightKidney1);
  SELiquidCompartmentLink& vRightKidneyToVenaCava = m_Compartments->CreateLiquidLink(vRightKidney, vVenaCava, BGE::VascularLink::RightKidneyToVenaCava);
  vRightKidneyToVenaCava.MapPath(RightKidney2ToVenaCava);
  ///////////////
  // Right Leg //
  SELiquidCompartmentLink& vAortaToRightLeg = m_Compartments->CreateLiquidLink(vAorta, vRightLeg, BGE::VascularLink::AortaToRightLeg);
  vAortaToRightLeg.MapPath(Aorta1ToRightLeg1);
  SELiquidCompartmentLink& vRightLegToVenaCava = m_Compartments->CreateLiquidLink(vRightLeg, vVenaCava, BGE::VascularLink::RightLegToVenaCava);
  vRightLegToVenaCava.MapPath(RightLeg2ToVenaCava);
  //////////
  // Skin //
  SELiquidCompartmentLink& vAortaToSkin = m_Compartments->CreateLiquidLink(vAorta, vSkin, BGE::VascularLink::AortaToSkin);
  vAortaToSkin.MapPath(Aorta1ToSkin1);
  SELiquidCompartmentLink& vSkinToVenaCava = m_Compartments->CreateLiquidLink(vSkin, vVenaCava, BGE::VascularLink::SkinToVenaCava);
  vSkinToVenaCava.MapPath(Skin2ToVenaCava);
  /////////////////////
  // Small Intestine //
  SELiquidCompartmentLink& vAortaToSmallIntestine = m_Compartments->CreateLiquidLink(vAorta, vSmallIntestine, BGE::VascularLink::AortaToSmallIntestine);
  vAortaToSmallIntestine.MapPath(Aorta1ToSmallIntestine);
  SELiquidCompartmentLink& vSmallIntestineToLiver = m_Compartments->CreateLiquidLink(vSmallIntestine, vLiver, BGE::VascularLink::SmallIntestineToLiver);
  vSmallIntestineToLiver.MapPath(SmallIntestineToPortalVein);
  ////////////////
  // Splanchnic //
  SELiquidCompartmentLink& vAortaToSplanchnic = m_Compartments->CreateLiquidLink(vAorta, vSplanchnic, BGE::VascularLink::AortaToSplanchnic);
  vAortaToSplanchnic.MapPath(Aorta1ToSplanchnic);
  SELiquidCompartmentLink& vSplanchnicToLiver = m_Compartments->CreateLiquidLink(vSplanchnic, vLiver, BGE::VascularLink::SplanchnicToLiver);
  vSplanchnicToLiver.MapPath(SplanchnicToPortalVein);
  ////////////
  // Spleen //
  SELiquidCompartmentLink& vAortaToSpleen = m_Compartments->CreateLiquidLink(vAorta, vSpleen, BGE::VascularLink::AortaToSpleen);
  vAortaToSpleen.MapPath(Aorta1ToSpleen);
  SELiquidCompartmentLink& vSpleenToLiver = m_Compartments->CreateLiquidLink(vSpleen, vLiver, BGE::VascularLink::SpleenToLiver);
  vSpleenToLiver.MapPath(SpleenToPortalVein);

  /////////////////////
  // Bleeds and IV's //
  SELiquidCompartmentLink& vVenaCavaIV = m_Compartments->CreateLiquidLink(vGround, vVenaCava, BGE::VascularLink::VenaCavaIV);
  vVenaCavaIV.MapPath(IVToVenaCava);
  SELiquidCompartmentLink& vVenaCavaHemorrhage = m_Compartments->CreateLiquidLink(vVenaCava, vGround, BGE::VascularLink::VenaCavaHemorrhage);
  vVenaCavaHemorrhage.MapPath(VenaCavaBleed);
  SELiquidCompartmentLink& vAortaHemorrhage = m_Compartments->CreateLiquidLink(vAorta, vGround, BGE::VascularLink::AortaHemorrhage);
  vAortaHemorrhage.MapPath(AortaBleed);
  SELiquidCompartmentLink& vBrainHemorrhage = m_Compartments->CreateLiquidLink(vBrain, vGround, BGE::VascularLink::BrainHemorrhage);
  vBrainHemorrhage.MapPath(BrainBleed);
  SELiquidCompartmentLink& vHeartHemorrhage = m_Compartments->CreateLiquidLink(vMyocardium, vGround, BGE::VascularLink::HeartHemorrhage);
  vHeartHemorrhage.MapPath(HeartBleed);
  SELiquidCompartmentLink& vLeftLungHemorrhage = m_Compartments->CreateLiquidLink(vLeftPulmonaryArteries, vGround, BGE::VascularLink::LeftLungHemorrhage);
  vLeftLungHemorrhage.MapPath(LeftLungBleed);
  SELiquidCompartmentLink& vRightLungHemorrhage = m_Compartments->CreateLiquidLink(vRightPulmonaryArteries, vGround, BGE::VascularLink::RightLungHemorrhage);
  vRightLungHemorrhage.MapPath(RightLungBleed);
  SELiquidCompartmentLink& vLeftArmHemorrhage = m_Compartments->CreateLiquidLink(vLeftArm, vGround, BGE::VascularLink::LeftArmHemorrhage);
  vLeftArmHemorrhage.MapPath(LeftArmBleed);
  SELiquidCompartmentLink& vRightArmHemorrhage = m_Compartments->CreateLiquidLink(vRightArm, vGround, BGE::VascularLink::RightArmHemorrhage);
  vRightArmHemorrhage.MapPath(RightArmBleed);
  SELiquidCompartmentLink& vSpleenHemorrhage = m_Compartments->CreateLiquidLink(vSpleen, vGround, BGE::VascularLink::SpleenHemorrhage);
  vSpleenHemorrhage.MapPath(SpleenBleed);
  SELiquidCompartmentLink& vSmallIntestineHemorrhage = m_Compartments->CreateLiquidLink(vSmallIntestine, vGround, BGE::VascularLink::SmallIntestineHemorrhage);
  vSmallIntestineHemorrhage.MapPath(SmallIntestineBleed);
  SELiquidCompartmentLink& vLargeIntestineHemorrhage = m_Compartments->CreateLiquidLink(vLargeIntestine, vGround, BGE::VascularLink::LargeIntestineHemorrhage);
  vLargeIntestineHemorrhage.MapPath(LargeIntestineBleed);
  SELiquidCompartmentLink& vSplanchnicHemorrhage = m_Compartments->CreateLiquidLink(vSplanchnic, vGround, BGE::VascularLink::SplanchnicHemorrhage);
  vSplanchnicHemorrhage.MapPath(SplanchnicBleed);
  //Take care of kidney hemorrhage in renal circuit set up below
  SELiquidCompartmentLink& vLiverHemorrhage = m_Compartments->CreateLiquidLink(vLiver, vGround, BGE::VascularLink::LiverHemorrhage);
  vLiverHemorrhage.MapPath(LiverBleed);
  SELiquidCompartmentLink& vLeftLegHemorrhage = m_Compartments->CreateLiquidLink(vLeftLeg, vGround, BGE::VascularLink::LeftLegHemorrhage);
  vLeftLegHemorrhage.MapPath(LeftLegBleed);
  SELiquidCompartmentLink& vRightLegHemorrhage = m_Compartments->CreateLiquidLink(vRightLeg, vGround, BGE::VascularLink::RightLegHemorrhage);
  vRightLegHemorrhage.MapPath(RightLegBleed);

  SELiquidCompartmentGraph& gCardiovascular = m_Compartments->GetCardiovascularGraph();
  gCardiovascular.AddCompartment(vRightAtrium);
  gCardiovascular.AddCompartment(vRightVentricle);
  gCardiovascular.AddCompartment(vRightPulmonaryArteries);
  gCardiovascular.AddCompartment(vLeftPulmonaryArteries);
  gCardiovascular.AddCompartment(vRightPulmonaryCapillaries);
  gCardiovascular.AddCompartment(vLeftPulmonaryCapillaries);
  gCardiovascular.AddCompartment(vRightPulmonaryVeins);
  gCardiovascular.AddCompartment(vLeftPulmonaryVeins);
  gCardiovascular.AddCompartment(vLeftAtrium);
  gCardiovascular.AddCompartment(vLeftVentricle);
  gCardiovascular.AddCompartment(vAorta);
  gCardiovascular.AddCompartment(vBrain);
  gCardiovascular.AddCompartment(vBone);
  gCardiovascular.AddCompartment(vFat);
  gCardiovascular.AddCompartment(vLargeIntestine);
  gCardiovascular.AddCompartment(vLiver);
  gCardiovascular.AddCompartment(vLeftArm);
  gCardiovascular.AddCompartment(vLeftKidney);
  gCardiovascular.AddCompartment(vLeftLeg);
  gCardiovascular.AddCompartment(vMuscle);
  gCardiovascular.AddCompartment(vMyocardium);
  gCardiovascular.AddCompartment(vRightArm);
  gCardiovascular.AddCompartment(vRightKidney);
  gCardiovascular.AddCompartment(vRightLeg);
  gCardiovascular.AddCompartment(vSkin);
  gCardiovascular.AddCompartment(vSmallIntestine);
  gCardiovascular.AddCompartment(vSplanchnic);
  gCardiovascular.AddCompartment(vSpleen);
  gCardiovascular.AddCompartment(vVenaCava);
  gCardiovascular.AddLink(vVenaCavaToRightAtrium);
  gCardiovascular.AddLink(vRightAtriumToRightVentricle);
  gCardiovascular.AddLink(vRightVentricleToLeftPulmonaryArteries);
  gCardiovascular.AddLink(vLeftPulmonaryArteriesToCapillaries);
  gCardiovascular.AddLink(vLeftPulmonaryArteriesToVeins);
  gCardiovascular.AddLink(vLeftPulmonaryCapillariesToVeins);
  gCardiovascular.AddLink(vLeftPulmonaryVeinsToLeftAtrium);
  gCardiovascular.AddLink(vRightVentricleToRightPulmonaryArteries);
  gCardiovascular.AddLink(vRightPulmonaryArteriesToCapillaries);
  gCardiovascular.AddLink(vRightPulmonaryArteriesToVeins);
  gCardiovascular.AddLink(vRightPulmonaryCapillariesToVeins);
  gCardiovascular.AddLink(vRightPulmonaryVeinsToLeftAtrium);
  gCardiovascular.AddLink(vLeftAtriumToLeftVentricle);
  gCardiovascular.AddLink(vLeftVentricleToAorta);
  gCardiovascular.AddLink(vAortaToBone);
  gCardiovascular.AddLink(vBoneToVenaCava);
  gCardiovascular.AddLink(vAortaToBrain);
  gCardiovascular.AddLink(vBrainToVenaCava);
  gCardiovascular.AddLink(vAortaToFat);
  gCardiovascular.AddLink(vFatToVenaCava);
  gCardiovascular.AddLink(vAortaToLargeIntestine);
  gCardiovascular.AddLink(vLargeIntestineToLiver);
  gCardiovascular.AddLink(vAortaToLeftArm);
  gCardiovascular.AddLink(vLeftArmToVenaCava);
  gCardiovascular.AddLink(vAortaToLeftKidney);
  gCardiovascular.AddLink(vLeftKidneyToVenaCava);
  gCardiovascular.AddLink(vAortaToLeftLeg);
  gCardiovascular.AddLink(vLeftLegToVenaCava);
  gCardiovascular.AddLink(vAortaToLiver);
  gCardiovascular.AddLink(vLiverToVenaCava);
  gCardiovascular.AddLink(vAortaToMuscle);
  gCardiovascular.AddLink(vMuscleToVenaCava);
  gCardiovascular.AddLink(vAortaToMyocardium);
  gCardiovascular.AddLink(vMyocardiumToVenaCava);
  gCardiovascular.AddLink(vAortaToRightArm);
  gCardiovascular.AddLink(vRightArmToVenaCava);
  gCardiovascular.AddLink(vAortaToRightKidney);
  gCardiovascular.AddLink(vRightKidneyToVenaCava);
  gCardiovascular.AddLink(vAortaToRightLeg);
  gCardiovascular.AddLink(vRightLegToVenaCava);
  gCardiovascular.AddLink(vAortaToSkin);
  gCardiovascular.AddLink(vSkinToVenaCava);
  gCardiovascular.AddLink(vAortaToSmallIntestine);
  gCardiovascular.AddLink(vSmallIntestineToLiver);
  gCardiovascular.AddLink(vAortaToSplanchnic);
  gCardiovascular.AddLink(vSplanchnicToLiver);
  gCardiovascular.AddLink(vAortaToSpleen);
  gCardiovascular.AddLink(vSpleenToLiver);
  gCardiovascular.AddLink(vVenaCavaHemorrhage);
  gCardiovascular.AddLink(vVenaCavaIV);
  gCardiovascular.AddLink(vAortaHemorrhage);
  gCardiovascular.AddLink(vBrainHemorrhage);
  gCardiovascular.AddLink(vHeartHemorrhage);
  gCardiovascular.AddLink(vLeftLungHemorrhage);
  gCardiovascular.AddLink(vRightLungHemorrhage);
  gCardiovascular.AddLink(vLeftArmHemorrhage);
  gCardiovascular.AddLink(vRightArmHemorrhage);
  gCardiovascular.AddLink(vSpleenHemorrhage);
  gCardiovascular.AddLink(vSmallIntestineHemorrhage);
  gCardiovascular.AddLink(vLargeIntestineHemorrhage);
  gCardiovascular.AddLink(vSplanchnicHemorrhage);
  gCardiovascular.AddLink(vLiverHemorrhage);
  gCardiovascular.AddLink(vLeftLegHemorrhage);
  gCardiovascular.AddLink(vRightLegHemorrhage);
  gCardiovascular.StateChange();

  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddGraph(gCardiovascular);
  gCombinedCardiovascular.StateChange();
}

void BioGears::SetupCerebral()
{
  Info("Setting up Cerebral");

  SEFluidCircuit& CerebralCircuit = m_Circuits->GetCerebralCircuit();
  SEFluidCircuit& CardioCircuit = m_Circuits->GetCardiovascularCircuit();

  //--------------------Define target circuit values---------------------------------------
  //Get target flow from cardiovascular circuit--probably seems circuitous but I'd rather this function know if the target in SetupCardiovascular changes
  double cardioBrain1Pressure = CardioCircuit.GetNode(BGE::CardiovascularNode::Brain1)->GetPressure(PressureUnit::mmHg);
  double cardioAorta1Pressure = CardioCircuit.GetNode(BGE::CardiovascularNode::Aorta1)->GetPressure(PressureUnit::mmHg);
  double cardioAortaToBrainResistance = CardioCircuit.GetPath(BGE::CardiovascularPath::Aorta1ToBrain1)->GetResistanceBaseline(FlowResistanceUnit::mmHg_s_Per_mL);

  double cerebralTargetFlow_mL_Per_s = (cardioAorta1Pressure - cardioBrain1Pressure) / cardioAortaToBrainResistance;
  double cerebralTargetVolume_mL = CardioCircuit.GetNode(BGE::CardiovascularNode::Brain1)->GetVolumeBaseline(VolumeUnit::mL);
  //Remaining parameters derived from Lu2003Cerebral
  //Target cerebral spinal fluid flow--same target for both formation and absorption since net = 0 at steady state
  double cerebroSpinalFluidFlow_mL_Per_s = Convert(560.0, VolumePerTimeUnit::mL_Per_day, VolumePerTimeUnit::mL_Per_s);
  //Target volumes--Lu assumes 100 mL of brain vasculature disctributed 20/20/60 (discounting neck), so scale reported values by BioGears target
  //Factor in volume modifier applied to vascular nodes in Setup Cardiovascular since this will affect the stabilized pressure after tuning
  double VolumeModifierBrain = 0.998011 * 1.038409;
  double neckArteriesVolume_mL = 15.0 * VolumeModifierBrain;
  double cerebralArteriesVolume_mL = 0.2 * cerebralTargetVolume_mL * VolumeModifierBrain;
  double cerebralCapillariesVolume_mL = 0.2 * cerebralTargetVolume_mL * VolumeModifierBrain;
  double cerebralVeinsVolume_mL = 0.6 * cerebralTargetVolume_mL * VolumeModifierBrain;
  double neckVeinsVolume_mL = 35.0 * VolumeModifierBrain;
  double intracranialVolume_mL = 150.0 * VolumeModifierBrain;
  //Target pressures
  double neckArteriesPressure_mmHg = 85.0; //This pressure is below enforced patient systolic baseline of 90mmHg, so we can safely hard-code this value
  double cerebralArteries1Pressure_mmHg = 65.0;
  double cerebralAreteries2Pressure_mmHg = cerebralArteries1Pressure_mmHg;
  double cerebralCapillariesPressure_mmHg = 25.0;
  double cerebralVeins1Pressure_mmHg = 14.0;
  double cerebralVeins2Pressure_mmHg = 7.5; //Assumed--value should be < ICP (10.0) but greater than vena cava pressure (~4)
  double neckVeinsPressure_mmHg = cerebralVeins2Pressure_mmHg;
  double intracranialPressure_mmHg = 15.0;
  //Derive restistance elements
  double aorta1ToNeckArteriesResitance_mmHg_s_Per_mL = (CardioCircuit.GetNode(BGE::CardiovascularNode::Aorta1)->GetPressure(PressureUnit::mmHg) - neckArteriesPressure_mmHg) / cerebralTargetFlow_mL_Per_s;
  double neckToCerebralArteries1Resistance_mmHg_s_Per_mL = (neckArteriesPressure_mmHg - cerebralArteries1Pressure_mmHg) / cerebralTargetFlow_mL_Per_s;
  double cerebralArteries2ToCapillariesResistance_mmHg_s_Per_mL = (cerebralAreteries2Pressure_mmHg - cerebralCapillariesPressure_mmHg) / cerebralTargetFlow_mL_Per_s;
  double cerebralCapillariesToVeins1Resistance_mmHg_s_Per_mL = (cerebralCapillariesPressure_mmHg - cerebralVeins1Pressure_mmHg) / cerebralTargetFlow_mL_Per_s;
  double cerebralVeins1ToVeins2Resistance_mmHg_s_Per_mL = (cerebralVeins1Pressure_mmHg - cerebralVeins2Pressure_mmHg) / cerebralTargetFlow_mL_Per_s;
  double neckVeinsToVenaCavaResistance_mmHg_s_Per_mL = (neckVeinsPressure_mmHg - CardioCircuit.GetNode(BGE::CardiovascularNode::VenaCava)->GetPressure(PressureUnit::mmHg)) / cerebralTargetFlow_mL_Per_s;
  double cerebralArteries2ToSpinalFluidResistance_mmHg_s_Per_mL = (cerebralAreteries2Pressure_mmHg - intracranialPressure_mmHg) / cerebroSpinalFluidFlow_mL_Per_s;
  double spinalFluidToVeinsCheckResistance_mmHg_s_Per_mL = (intracranialPressure_mmHg - cerebralVeins2Pressure_mmHg) / cerebroSpinalFluidFlow_mL_Per_s;
  //Compliance elements
  double neckArteriesToGroundCompliance_mL_Per_mmHg = 0.20;
  double cerebralArteries1ToSpinalFluidCompliance_mL_Per_mmHg = 0.20;
  double cerebralCapillariesToSpinalFluidCompliance_mL_Per_mmHg = 0.22;
  double cerebralVeins1ToSpinalFluidCompliance_mL_Per_mmHg = 1.0;
  double intracranialCompliance_mL_Per_mmHg = 1.0 / (0.11 * intracranialPressure_mmHg); //Lu cites 0.11 as an "intracranial rigidity" factor

  //------------------------Cerebral Circuit (n = nodes, p = paths)-----------------------
  //Create circuit nodes
  SEFluidCircuitNode& nGround = CerebralCircuit.CreateNode(BGE::CerebralNode::Ground);
  nGround.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  CerebralCircuit.AddReferenceNode(nGround);

  SEFluidCircuitNode& nNeckArteries = CerebralCircuit.CreateNode(BGE::CerebralNode::NeckArteries);
  nNeckArteries.GetPressure().SetValue(neckArteriesPressure_mmHg, PressureUnit::mmHg);
  nNeckArteries.GetVolumeBaseline().SetValue(neckArteriesVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nCerebralArteries1 = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralArteries1);
  nCerebralArteries1.GetPressure().SetValue(cerebralArteries1Pressure_mmHg, PressureUnit::mmHg);
  nCerebralArteries1.GetVolumeBaseline().SetValue(cerebralArteriesVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nCerebralArteries2 = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralArteries2);
  nCerebralArteries2.GetPressure().SetValue(cerebralAreteries2Pressure_mmHg, PressureUnit::mmHg);

  SEFluidCircuitNode& nCerebralCapillaries = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralCapillaries);
  nCerebralCapillaries.GetPressure().SetValue(cerebralCapillariesPressure_mmHg, PressureUnit::mmHg);
  nCerebralCapillaries.GetVolumeBaseline().SetValue(cerebralCapillariesVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nCerebralVeins1 = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralVeins1);
  nCerebralVeins1.GetPressure().SetValue(cerebralVeins1Pressure_mmHg, PressureUnit::mmHg);
  nCerebralVeins1.GetVolumeBaseline().SetValue(cerebralVeinsVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nCerebralVeins2 = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralVeins2);
  nCerebralVeins2.GetPressure().SetValue(cerebralVeins2Pressure_mmHg, PressureUnit::mmHg);

  SEFluidCircuitNode& nCerebralVeinsCheck = CerebralCircuit.CreateNode(BGE::CerebralNode::CerebralVeinsCheck);
  nCerebralVeinsCheck.GetPressure().SetValue(cerebralVeins2Pressure_mmHg, PressureUnit::mmHg);

  SEFluidCircuitNode& nNeckVeins = CerebralCircuit.CreateNode(BGE::CerebralNode::NeckVeins);
  nNeckVeins.GetPressure().SetValue(neckVeinsPressure_mmHg, PressureUnit::mmHg);
  nNeckVeins.GetVolumeBaseline().SetValue(neckVeinsVolume_mL, VolumeUnit::mL);

  SEFluidCircuitNode& nSpinalFluid = CerebralCircuit.CreateNode(BGE::CerebralNode::SpinalFluid);
  nSpinalFluid.GetPressure().SetValue(intracranialPressure_mmHg, PressureUnit::mmHg);
  nSpinalFluid.GetVolumeBaseline().SetValue(intracranialVolume_mL, VolumeUnit::mL);

  //Paths
  SEFluidCircuitPath& pNeckArteriesToCerebralArteries1 = CerebralCircuit.CreatePath(nNeckArteries, nCerebralArteries1, BGE::CerebralPath::NeckArteriesToCerebralArteries1);
  pNeckArteriesToCerebralArteries1.GetResistanceBaseline().SetValue(neckToCerebralArteries1Resistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pNeckArteriesToCerebralArteries1.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);

  SEFluidCircuitPath& pNeckArteriesToGround = CerebralCircuit.CreatePath(nNeckArteries, nGround, BGE::CerebralPath::NeckArteriesToGround);
  pNeckArteriesToGround.GetComplianceBaseline().SetValue(neckArteriesToGroundCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pCerebralArteries1ToArteries2 = CerebralCircuit.CreatePath(nCerebralArteries1, nCerebralArteries2, BGE::CerebralPath::CerebralArteries1ToCerebralArteries2);

  SEFluidCircuitPath& pCerebralArteries1ToSpinalFluid = CerebralCircuit.CreatePath(nCerebralArteries1, nSpinalFluid, BGE::CerebralPath::CerebralArteries1ToSpinalFluid);
  pCerebralArteries1ToSpinalFluid.GetComplianceBaseline().SetValue(cerebralArteries1ToSpinalFluidCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pCerebralArteries2ToSpinalFluid = CerebralCircuit.CreatePath(nCerebralArteries2, nSpinalFluid, BGE::CerebralPath::CerebralArteries2ToSpinalFluid);
  pCerebralArteries2ToSpinalFluid.GetResistanceBaseline().SetValue(cerebralArteries2ToSpinalFluidResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& pCerebralArteries2ToCapillaries = CerebralCircuit.CreatePath(nCerebralArteries2, nCerebralCapillaries, BGE::CerebralPath::CerebralArteries2ToCapillaries);
  pCerebralArteries2ToCapillaries.GetResistanceBaseline().SetValue(cerebralArteries2ToCapillariesResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pCerebralArteries2ToCapillaries.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);

  SEFluidCircuitPath& pCerebralCapillariesToSpinalFluid = CerebralCircuit.CreatePath(nCerebralCapillaries, nSpinalFluid, BGE::CerebralPath::CerebralCapillariesToSpinalFluid);
  pCerebralCapillariesToSpinalFluid.GetComplianceBaseline().SetValue(cerebralCapillariesToSpinalFluidCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pCerebralCapillariesToCerebralVeins1 = CerebralCircuit.CreatePath(nCerebralCapillaries, nCerebralVeins1, BGE::CerebralPath::CerebralCapillariesToCerebralVeins1);
  pCerebralCapillariesToCerebralVeins1.GetResistanceBaseline().SetValue(cerebralCapillariesToVeins1Resistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pCerebralCapillariesToCerebralVeins1.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);

  SEFluidCircuitPath& pCerebralVeins1ToVeins2 = CerebralCircuit.CreatePath(nCerebralVeins1, nCerebralVeins2, BGE::CerebralPath::CerebralVeins1ToCerebralVeins2);
  pCerebralVeins1ToVeins2.GetResistanceBaseline().SetValue(cerebralVeins1ToVeins2Resistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pCerebralVeins1ToVeins2.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);

  SEFluidCircuitPath& pCerebralVeins2ToNeckVeins = CerebralCircuit.CreatePath(nCerebralVeins2, nNeckVeins, BGE::CerebralPath::CerebralVeins2ToNeckVeins);

  SEFluidCircuitPath& pNeckVeinsToGround = CerebralCircuit.CreatePath(nNeckVeins, nGround, BGE::CerebralPath::NeckVeinsToGround);
  pNeckVeinsToGround.GetComplianceBaseline().SetValue(neckVeinsPressure_mmHg / neckVeinsVolume_mL, FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& pSpinalFluidToCerebralVeinsCheck = CerebralCircuit.CreatePath(nSpinalFluid, nCerebralVeinsCheck, BGE::CerebralPath::SpinalFluidToCerebralVeinsCheck);
  pSpinalFluidToCerebralVeinsCheck.GetResistanceBaseline().SetValue(spinalFluidToVeinsCheckResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

  SEFluidCircuitPath& pCerebralVeinsCheckToVeins2 = CerebralCircuit.CreatePath(nCerebralVeinsCheck, nCerebralVeins2, BGE::CerebralPath::CerebralVeinsCheckToCerebralVeins2);
  pCerebralVeinsCheckToVeins2.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& pSpinalFluidToGround = CerebralCircuit.CreatePath(nSpinalFluid, nGround, BGE::CerebralPath::SpinalFluidToGround);
  pSpinalFluidToGround.GetComplianceBaseline().SetValue(intracranialCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);

  CerebralCircuit.SetNextAndCurrentFromBaselines();
  CerebralCircuit.StateChange();

  // Delete the three-element Windkessel brain and add the cerebral circuit to the active combined CV circuit
  SEFluidCircuit& CombinedCardioCircuit = m_Circuits->GetActiveCardiovascularCircuit();
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::Brain1);
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::Brain2);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Aorta1ToBrain1);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Brain1ToBrain2);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Brain1ToGround);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Brain2ToVenaCava);

  CombinedCardioCircuit.AddCircuit(CerebralCircuit);
  // Grab the nodes that we will be connecting between the 2 circuits
  SEFluidCircuitNode* nAorta1 = CardioCircuit.GetNode(BGE::CardiovascularNode::Aorta1);
  SEFluidCircuitNode* nVenaCava = CardioCircuit.GetNode(BGE::CardiovascularNode::VenaCava);
  // Add the new connection paths
  SEFluidCircuitPath& pAorta1ToNeckArteries = CombinedCardioCircuit.CreatePath(*nAorta1, nNeckArteries, BGE::CardiovascularPath::Aorta1ToNeckArteries);
  pAorta1ToNeckArteries.GetResistanceBaseline().SetValue(aorta1ToNeckArteriesResitance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pAorta1ToNeckArteries.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);
  SEFluidCircuitPath& pNeckVeinsToVenaCava = CombinedCardioCircuit.CreatePath(nNeckVeins, *nVenaCava, BGE::CardiovascularPath::NeckVeinsToVenaCava);
  pNeckVeinsToVenaCava.GetResistanceBaseline().SetValue(neckVeinsToVenaCavaResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  pNeckVeinsToVenaCava.SetCardiovascularRegion(CDM::enumResistancePathType::Cerebral);

  // Update the circuit
  CombinedCardioCircuit.SetNextAndCurrentFromBaselines();
  CombinedCardioCircuit.StateChange();

  //---------------------Set up Cerebral Graph (c = compartment, l = liquid link)-------------------
  //Compartments

  SELiquidCompartment& cNeckArteries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::NeckArteries);
  cNeckArteries.MapNode(nNeckArteries);

  SELiquidCompartment& cCerebralArteries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::CerebralArteries);
  cCerebralArteries.MapNode(nCerebralArteries1);
  cCerebralArteries.MapNode(nCerebralArteries2);

  SELiquidCompartment& cCerebralCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::CerebralCapillaries);
  cCerebralCapillaries.MapNode(nCerebralCapillaries);

  SELiquidCompartment& cCerebralVeins = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::CerebralVeins);
  cCerebralVeins.MapNode(nCerebralVeins1);
  cCerebralVeins.MapNode(nCerebralVeins2);
  cCerebralVeins.MapNode(nCerebralVeinsCheck);

  SELiquidCompartment& cNeckVeins = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::NeckVeins);
  cNeckVeins.MapNode(nNeckVeins);

  SELiquidCompartment& cSpinalFluid = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::CerebralSpinalFluid);
  cSpinalFluid.MapNode(nSpinalFluid);

  SELiquidCompartment& cBrain = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::Brain);
  cBrain.GetNodeMapping().Clear();
  cBrain.AddChild(cCerebralArteries);
  cBrain.AddChild(cCerebralCapillaries);
  cBrain.AddChild(cCerebralVeins);

  //Existing compartments to link with cerebral graph
  SELiquidCompartment& cAorta = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  SELiquidCompartment& cVenaCava = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::VenaCava);

  //Liquid Links
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::AortaToBrain); //Replace this link

  SELiquidCompartmentLink& lAortaToNeckArteries = m_Compartments->CreateLiquidLink(cAorta, cNeckArteries, BGE::VascularLink::AortaToNeckArteries);
  lAortaToNeckArteries.MapPath(pAorta1ToNeckArteries);

  SELiquidCompartmentLink& lNeckArteriesToCerebralArteries = m_Compartments->CreateLiquidLink(cNeckArteries, cCerebralArteries, BGE::VascularLink::NeckArteriesToCerebralArteries);
  lNeckArteriesToCerebralArteries.MapPath(pNeckArteriesToCerebralArteries1);

  SELiquidCompartmentLink& lCerebralArteriesToCapillaries = m_Compartments->CreateLiquidLink(cCerebralArteries, cCerebralCapillaries, BGE::VascularLink::CerebralArteriesToCerebralCapillaries);
  lCerebralArteriesToCapillaries.MapPath(pCerebralArteries2ToCapillaries);

  SELiquidCompartmentLink& lCerebralCapillariesToVeins = m_Compartments->CreateLiquidLink(cCerebralCapillaries, cCerebralVeins, BGE::VascularLink::CerebralCapillariesToCerebralVeins);
  lCerebralCapillariesToVeins.MapPath(pCerebralCapillariesToCerebralVeins1);

  SELiquidCompartmentLink& lCerebralVeinsToNeckVeins = m_Compartments->CreateLiquidLink(cCerebralVeins, cNeckVeins, BGE::VascularLink::CerebralVeinsToNeckVeins);
  lCerebralVeinsToNeckVeins.MapPath(pCerebralVeins2ToNeckVeins);

  m_Compartments->DeleteLiquidLink(BGE::VascularLink::BrainToVenaCava); //Replace this link
  SELiquidCompartmentLink& lNeckVeinsToVenaCava = m_Compartments->CreateLiquidLink(cNeckVeins, cVenaCava, BGE::VascularLink::NeckVeinsToVenaCava);
  lNeckVeinsToVenaCava.MapPath(pNeckVeinsToVenaCava);

  //Delete cerebral hemorrhage link (re-define after some more testing), otherwise graph will go searching for old BrainVasculature cmpt, which is called by this link
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::BrainHemorrhage);

  //Add compartments and links to cerebral graph
  SELiquidCompartmentGraph& gCerebral = m_Compartments->GetCerebralGraph();
  gCerebral.AddCompartment(cAorta);
  gCerebral.AddCompartment(cVenaCava);
  gCerebral.AddCompartment(cNeckArteries);
  gCerebral.AddCompartment(cCerebralArteries);
  gCerebral.AddCompartment(cCerebralCapillaries);
  gCerebral.AddCompartment(cCerebralVeins);
  gCerebral.AddCompartment(cNeckVeins);
  gCerebral.AddCompartment(cSpinalFluid);
  gCerebral.AddLink(lAortaToNeckArteries);
  gCerebral.AddLink(lNeckArteriesToCerebralArteries);
  gCerebral.AddLink(lCerebralArteriesToCapillaries);
  gCerebral.AddLink(lCerebralCapillariesToVeins);
  gCerebral.AddLink(lCerebralVeinsToNeckVeins);
  gCerebral.AddLink(lNeckVeinsToVenaCava);

  gCerebral.StateChange();

  //Remove brain cmpt from active CV graph (because it's discretized into smaller cmpts) and then add cerebral graph
  m_Compartments->GetActiveCardiovascularGraph().RemoveCompartment(*m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::Brain));
  m_Compartments->GetActiveCardiovascularGraph().AddGraph(gCerebral);
  m_Compartments->GetActiveCardiovascularGraph().StateChange();
}

void BioGears::SetupRenal()
{
  Info("Setting Up Renal");
  //////////////////////////
  // Circuit Interdependence
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();

  //assuming there is a left and right kidney node in cardiovascular AND that a baseline volume is set (as a function of patient mass):
  double leftKidneyFluidVolume_mL = cCardiovascular.GetNode(BGE::CardiovascularNode::LeftKidney1)->GetVolumeBaseline(VolumeUnit::mL);
  double rightKidneyFluidVolume_mL = cCardiovascular.GetNode(BGE::CardiovascularNode::RightKidney1)->GetVolumeBaseline(VolumeUnit::mL);
  double singleKidneyLargeVasculatureFluidVolume_mL = leftKidneyFluidVolume_mL / 2; //Total large vasculature fluid volume
  double singleKidneySmallVasculatureFluidVolume_mL = leftKidneyFluidVolume_mL / 2; //Total small vasculature fluid volume

  //////////////////////////
  ///// Circuit Parameters//////
  double openSwitch_mmHg_s_Per_mL = m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL);
  //Resistances with some tuning multipliers
  double urineTuningMultiplier = 1.0;
  double arteryTuningMultiplier = 0.6;
  double reabsorptionTuningMultiplier = 1.1;
  double gfrTuning = 1.0;
  double efferentTuning = 1.1;

  double renalArteryResistance_mmHg_s_Per_mL = Convert(0.0250 * arteryTuningMultiplier, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double afferentResistance_mmHg_s_Per_mL = Convert(0.0417 * arteryTuningMultiplier, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double efferentResistance_mmHg_s_Per_mL = Convert(0.0763 * efferentTuning, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double glomerularResistance_mmHg_s_Per_mL = Convert(0.0019 * gfrTuning, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double peritubularResistance_mmHg_s_Per_mL = Convert(0.0167, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double renalVeinResistance_mmHg_s_Per_mL = Convert(0.0066, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double glomerularFilterResistance_mmHg_s_Per_mL = Convert(0.1600 * gfrTuning, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double tubulesResistance_mmHg_s_Per_mL = Convert(0.1920 * urineTuningMultiplier, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double reabsoprtionResistance_mmHg_s_Per_mL = Convert(0.1613 * reabsorptionTuningMultiplier, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //This one is tuned
  double ureterTuningMultiplier = 0.59;
  double ureterResistance_mmHg_s_Per_mL = Convert(30.0 * ureterTuningMultiplier, FlowResistanceUnit::mmHg_min_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  double urethraResistance_mmHg_s_Per_mL = openSwitch_mmHg_s_Per_mL;
  //Compliances
  //0.5 * CapacitanceKidney is the per-kidney value from 3 element Windkessel
  double totalCompliance = (0.5 * (0.91 * 1.7560) * 0.02);
  //The fractions here should add to 1.0;
  double renalArteryCompliance_mL_Per_mmHg = totalCompliance * 0.11;
  double renalVeinCompliance_mL_Per_mmHg = totalCompliance * 0.78;
  double glomerularCompliance_mL_Per_mmHg = totalCompliance * 0.11;
  ///\todo The bladder is currently not being modeled as a compliance
  //double bladderCompliance_mL_Per_mmHg = Convert(38.3, FlowComplianceUnit::mL_Per_cmH2O, FlowComplianceUnit::mL_Per_mmHg);

  //Large Vasculature (divide total large vasculature fluid volume three ways):
  double tubulesVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;
  double renalArteryVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;
  double renalVeinVolume_mL = singleKidneyLargeVasculatureFluidVolume_mL / 3.0;

  //Small vasculature (divide total small vasculature fluid volume five ways):
  double peritubularVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double efferentVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double afferentVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double bowmansVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;
  double glomerularVolume_mL = singleKidneySmallVasculatureFluidVolume_mL / 5.0;

  //Using width = 1.8 mm and length = 11 inches => 710.6 mm^3
  double ureterVolume_mL = 0.71;

  //Tuned constants
  double bladderVolume_mL = 1.0;
  //Unstressed Pressures - set to zero to use unstressed properly
  double renalArteryPressure_mmHg = 0.0;
  double renalVeinPressure_mmHg = 0.0;
  double glomerularPressure_mmHg = cCardiovascular.GetNode(BGE::CardiovascularNode::LeftKidney1)->GetPressure(PressureUnit::mmHg);
  //double bladderPressure_mmHg = bladderVolume_mL / bladderCompliance_mL_Per_mmHg;
  //Pressure Sources
  double glomerularOsmoticPressure_mmHg = -32.0;
  double bowmansOsmoticPressure_mmHg = 0.0;
  double tubulesOsmoticPressure_mmHg = -15.0;
  double peritubularOsmoticPressure_mmHg = -32.0;

  SEFluidCircuit& cRenal = m_Circuits->GetRenalCircuit();

  ////////////
  // Ground //
  SEFluidCircuitNode& Ground = cRenal.CreateNode(BGE::RenalNode::Ground);
  Ground.GetPressure().SetValue(0.0, PressureUnit::mmHg);
  cRenal.AddReferenceNode(Ground);

  //////////////////
  // Create Nodes //
  //////////////////

  /////////////////
  // Right Blood //
  /////////////////
  //////////////////////////
  // RightAortaConnection //
  SEFluidCircuitNode& RightAortaConnection = cRenal.CreateNode(BGE::RenalNode::RightAortaConnection);
  //////////////////////
  // RightRenalArtery //
  SEFluidCircuitNode& RightRenalArtery = cRenal.CreateNode(BGE::RenalNode::RightRenalArtery);
  RightRenalArtery.GetVolumeBaseline().SetValue(renalArteryVolume_mL, VolumeUnit::mL);
  RightRenalArtery.GetPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  RightRenalArtery.GetNextPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // RightAfferentArteriole //
  SEFluidCircuitNode& RightAfferentArteriole = cRenal.CreateNode(BGE::RenalNode::RightAfferentArteriole);
  RightAfferentArteriole.GetVolumeBaseline().SetValue(afferentVolume_mL, VolumeUnit::mL);
  ////////////////////////////////
  // RightGlomerularCapillaries //
  SEFluidCircuitNode& RightGlomerularCapillaries = cRenal.CreateNode(BGE::RenalNode::RightGlomerularCapillaries);
  RightGlomerularCapillaries.GetVolumeBaseline().SetValue(glomerularVolume_mL, VolumeUnit::mL);
  RightGlomerularCapillaries.GetPressure().SetValue(glomerularPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // RightEfferentArteriole //
  SEFluidCircuitNode& RightEfferentArteriole = cRenal.CreateNode(BGE::RenalNode::RightEfferentArteriole);
  RightEfferentArteriole.GetVolumeBaseline().SetValue(efferentVolume_mL, VolumeUnit::mL);
  /////////////////////////////////
  // RightPeritubularCapillaries //
  SEFluidCircuitNode& RightPeritubularCapillaries = cRenal.CreateNode(BGE::RenalNode::RightPeritubularCapillaries);
  RightPeritubularCapillaries.GetVolumeBaseline().SetValue(peritubularVolume_mL, VolumeUnit::mL);
  ////////////////////
  // RightRenalVein //
  SEFluidCircuitNode& RightRenalVein = cRenal.CreateNode(BGE::RenalNode::RightRenalVein);
  RightRenalVein.GetVolumeBaseline().SetValue(renalVeinVolume_mL, VolumeUnit::mL);
  RightRenalVein.GetPressure().SetValue(renalVeinPressure_mmHg, PressureUnit::mmHg);
  /////////////////////////////
  // RightVenaCavaConnection //
  SEFluidCircuitNode& RightVenaCavaConnection = cRenal.CreateNode(BGE::RenalNode::RightVenaCavaConnection);
  //////////////////////////
  // RightBowmansCapsules //
  SEFluidCircuitNode& RightBowmansCapsules = cRenal.CreateNode(BGE::RenalNode::RightBowmansCapsules);
  RightBowmansCapsules.GetVolumeBaseline().SetValue(bowmansVolume_mL, VolumeUnit::mL);
  /////////////////////////////
  // RightNetBowmansCapsules //
  SEFluidCircuitNode& RightNetBowmansCapsules = cRenal.CreateNode(BGE::RenalNode::RightNetBowmansCapsules);
  ///////////////////////////////////
  // RightNetGlomerularCapillaries //
  SEFluidCircuitNode& RightNetGlomerularCapillaries = cRenal.CreateNode(BGE::RenalNode::RightNetGlomerularCapillaries);
  ////////////////////////////////////
  // RightNetPeritubularCapillaries //
  SEFluidCircuitNode& RightNetPeritubularCapillaries = cRenal.CreateNode(BGE::RenalNode::RightNetPeritubularCapillaries);
  /////////////////
  // Right Urine //
  /////////////////
  //////////////////
  // RightTubules //
  SEFluidCircuitNode& RightTubules = cRenal.CreateNode(BGE::RenalNode::RightTubules);
  RightTubules.GetVolumeBaseline().SetValue(tubulesVolume_mL, VolumeUnit::mL);
  /////////////////////
  // RightNetTubules //
  SEFluidCircuitNode& RightNetTubules = cRenal.CreateNode(BGE::RenalNode::RightNetTubules);
  /////////////////
  // RightUreter //
  SEFluidCircuitNode& RightUreter = cRenal.CreateNode(BGE::RenalNode::RightUreter);
  RightUreter.GetVolumeBaseline().SetValue(ureterVolume_mL, VolumeUnit::mL);

  /////////////////
  // Left Blood //
  /////////////////
  //////////////////////////
  // LeftAortaConnection //
  SEFluidCircuitNode& LeftAortaConnection = cRenal.CreateNode(BGE::RenalNode::LeftAortaConnection);
  /////////////////////
  // LeftRenalArtery //
  SEFluidCircuitNode& LeftRenalArtery = cRenal.CreateNode(BGE::RenalNode::LeftRenalArtery);
  LeftRenalArtery.GetVolumeBaseline().SetValue(renalArteryVolume_mL, VolumeUnit::mL);
  LeftRenalArtery.GetPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  LeftRenalArtery.GetNextPressure().SetValue(renalArteryPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // LeftAfferentArteriole //
  SEFluidCircuitNode& LeftAfferentArteriole = cRenal.CreateNode(BGE::RenalNode::LeftAfferentArteriole);
  LeftAfferentArteriole.GetVolumeBaseline().SetValue(afferentVolume_mL, VolumeUnit::mL);
  ////////////////////////////////
  // LeftGlomerularCapillaries //
  SEFluidCircuitNode& LeftGlomerularCapillaries = cRenal.CreateNode(BGE::RenalNode::LeftGlomerularCapillaries);
  LeftGlomerularCapillaries.GetVolumeBaseline().SetValue(glomerularVolume_mL, VolumeUnit::mL);
  LeftGlomerularCapillaries.GetPressure().SetValue(glomerularPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////
  // LeftEfferentArteriole //
  SEFluidCircuitNode& LeftEfferentArteriole = cRenal.CreateNode(BGE::RenalNode::LeftEfferentArteriole);
  LeftEfferentArteriole.GetVolumeBaseline().SetValue(efferentVolume_mL, VolumeUnit::mL);
  /////////////////////////////////
  // LeftPeritubularCapillaries //
  SEFluidCircuitNode& LeftPeritubularCapillaries = cRenal.CreateNode(BGE::RenalNode::LeftPeritubularCapillaries);
  LeftPeritubularCapillaries.GetVolumeBaseline().SetValue(peritubularVolume_mL, VolumeUnit::mL);
  ////////////////////
  // LeftRenalVein //
  SEFluidCircuitNode& LeftRenalVein = cRenal.CreateNode(BGE::RenalNode::LeftRenalVein);
  LeftRenalVein.GetVolumeBaseline().SetValue(renalVeinVolume_mL, VolumeUnit::mL);
  LeftRenalVein.GetPressure().SetValue(renalVeinPressure_mmHg, PressureUnit::mmHg);
  /////////////////////////////
  // LeftVenaCavaConnection //
  SEFluidCircuitNode& LeftVenaCavaConnection = cRenal.CreateNode(BGE::RenalNode::LeftVenaCavaConnection);
  //////////////////////////
  // LeftBowmansCapsules //
  SEFluidCircuitNode& LeftBowmansCapsules = cRenal.CreateNode(BGE::RenalNode::LeftBowmansCapsules);
  LeftBowmansCapsules.GetVolumeBaseline().SetValue(bowmansVolume_mL, VolumeUnit::mL);
  /////////////////////////////
  // LeftNetBowmansCapsules //
  SEFluidCircuitNode& LeftNetBowmansCapsules = cRenal.CreateNode(BGE::RenalNode::LeftNetBowmansCapsules);
  ///////////////////////////////////
  // LeftNetGlomerularCapillaries //
  SEFluidCircuitNode& LeftNetGlomerularCapillaries = cRenal.CreateNode(BGE::RenalNode::LeftNetGlomerularCapillaries);
  ////////////////////////////////////
  // LeftNetPeritubularCapillaries //
  SEFluidCircuitNode& LeftNetPeritubularCapillaries = cRenal.CreateNode(BGE::RenalNode::LeftNetPeritubularCapillaries);
  /////////////////
  // Left Urine //
  /////////////////
  //////////////////
  // LeftTubules //
  SEFluidCircuitNode& LeftTubules = cRenal.CreateNode(BGE::RenalNode::LeftTubules);
  LeftTubules.GetVolumeBaseline().SetValue(tubulesVolume_mL, VolumeUnit::mL);
  /////////////////////
  // LeftNetTubules //
  SEFluidCircuitNode& LeftNetTubules = cRenal.CreateNode(BGE::RenalNode::LeftNetTubules);
  /////////////////
  // LeftUreter //
  SEFluidCircuitNode& LeftUreter = cRenal.CreateNode(BGE::RenalNode::LeftUreter);
  LeftUreter.GetVolumeBaseline().SetValue(ureterVolume_mL, VolumeUnit::mL);

  /////////////
  // Bladder //
  SEFluidCircuitNode& Bladder = cRenal.CreateNode(BGE::RenalNode::Bladder);
  Bladder.GetVolumeBaseline().SetValue(bladderVolume_mL, VolumeUnit::mL);
  //Bladder.GetPressure().SetValue(0.0, PressureUnit::mmHg);

  //////////////////
  // Create Paths //
  //////////////////

  /////////////////
  // Right Blood //
  /////////////////
  ///////////////////////////////////////
  // RightAortaConnectionToRenalArtery //
  SEFluidCircuitPath& RightAortaConnectionToRenalArtery = cRenal.CreatePath(RightAortaConnection, RightRenalArtery, BGE::RenalPath::RightAortaConnectionToRenalArtery);
  //////////////////////
  // RightRenalArteryToAfferentArteriole //
  SEFluidCircuitPath& RightRenalArteryToAfferentArteriole = cRenal.CreatePath(RightRenalArtery, RightAfferentArteriole, BGE::RenalPath::RightRenalArteryToAfferentArteriole);
  RightRenalArteryToAfferentArteriole.GetResistanceBaseline().SetValue(renalArteryResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////
  // RightRenalArteryCompliance //
  SEFluidCircuitPath& RightRenalArteryCompliance = cRenal.CreatePath(RightRenalArtery, Ground, BGE::RenalPath::RightRenalArteryCompliance);
  RightRenalArteryCompliance.GetComplianceBaseline().SetValue(renalArteryCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  ///////////////////////////////////////////////////
  // RightAfferentArterioleToGlomerularCapillaries //
  SEFluidCircuitPath& RightAfferentArterioleToGlomerularCapillaries = cRenal.CreatePath(RightAfferentArteriole, RightGlomerularCapillaries, BGE::RenalPath::RightAfferentArterioleToGlomerularCapillaries);
  RightAfferentArterioleToGlomerularCapillaries.GetResistanceBaseline().SetValue(afferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ///////////////////////////////////////////////////
  // RightGlomerularCapillariesToEfferentArteriole //
  SEFluidCircuitPath& RightGlomerularCapillariesToEfferentArteriole = cRenal.CreatePath(RightGlomerularCapillaries, RightEfferentArteriole, BGE::RenalPath::RightGlomerularCapillariesToEfferentArteriole);
  RightGlomerularCapillariesToEfferentArteriole.GetResistanceBaseline().SetValue(glomerularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////
  // RightGlomerularCapillariesCompliance //
  SEFluidCircuitPath& RightGlomerularCapillariesCompliance = cRenal.CreatePath(RightGlomerularCapillaries, Ground, BGE::RenalPath::RightGlomerularCapillariesCompliance);
  RightGlomerularCapillariesCompliance.GetComplianceBaseline().SetValue(glomerularCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  ////////////////////////////////////////////////////
  // RightEfferentArterioleToPeritubularCapillaries //
  SEFluidCircuitPath& RightEfferentArterioleToPeritubularCapillaries = cRenal.CreatePath(RightEfferentArteriole, RightPeritubularCapillaries, BGE::RenalPath::RightEfferentArterioleToPeritubularCapillaries);
  RightEfferentArterioleToPeritubularCapillaries.GetResistanceBaseline().SetValue(efferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////
  // RightPeritubularCapillariesToRenalVein //
  SEFluidCircuitPath& RightPeritubularCapillariesToRenalVein = cRenal.CreatePath(RightPeritubularCapillaries, RightRenalVein, BGE::RenalPath::RightPeritubularCapillariesToRenalVein);
  RightPeritubularCapillariesToRenalVein.GetResistanceBaseline().SetValue(peritubularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////
  // RightRenalVeinToVenaCavaConnection //
  SEFluidCircuitPath& RightRenalVeinToVenaCavaConnection = cRenal.CreatePath(RightRenalVein, RightVenaCavaConnection, BGE::RenalPath::RightRenalVeinToVenaCavaConnection);
  RightRenalVeinToVenaCavaConnection.GetResistanceBaseline().SetValue(renalVeinResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////////
  // RightRenalVeinCompliance //
  SEFluidCircuitPath& RightRenalVeinCompliance = cRenal.CreatePath(RightRenalVein, Ground, BGE::RenalPath::RightRenalVeinCompliance);
  RightRenalVeinCompliance.GetComplianceBaseline().SetValue(renalVeinCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  //////////////////////////////////////////////////////////
  // RightGlomerularCapillariesToNetGlomerularCapillaries //
  SEFluidCircuitPath& RightGlomerularCapillariesToNetGlomerularCapillaries = cRenal.CreatePath(RightGlomerularCapillaries, RightNetGlomerularCapillaries, BGE::RenalPath::RightGlomerularCapillariesToNetGlomerularCapillaries);
  RightGlomerularCapillariesToNetGlomerularCapillaries.GetPressureSourceBaseline().SetValue(glomerularOsmoticPressure_mmHg, PressureUnit::mmHg);
  ///////////////////////////////////////////////////////
  // RightNetGlomerularCapillariesToNetBowmansCapsules //
  SEFluidCircuitPath& RightNetGlomerularCapillariesToNetBowmansCapsules = cRenal.CreatePath(RightNetGlomerularCapillaries, RightNetBowmansCapsules, BGE::RenalPath::RightNetGlomerularCapillariesToNetBowmansCapsules);
  RightNetGlomerularCapillariesToNetBowmansCapsules.GetResistanceBaseline().SetValue(glomerularFilterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  RightNetGlomerularCapillariesToNetBowmansCapsules.SetNextPolarizedState(CDM::enumOpenClosed::Open);
  //////////////////////////////////////////////
  // RightBowmansCapsulesToNetBowmansCapsules //
  SEFluidCircuitPath& RightBowmansCapsulesToNetBowmansCapsules = cRenal.CreatePath(RightBowmansCapsules, RightNetBowmansCapsules, BGE::RenalPath::RightBowmansCapsulesToNetBowmansCapsules);
  RightBowmansCapsulesToNetBowmansCapsules.GetPressureSourceBaseline().SetValue(bowmansOsmoticPressure_mmHg, PressureUnit::mmHg);
  /////////////////
  // Hemorrhage from right kidney//
  SEFluidCircuitPath& RightKidneyBleed = cRenal.CreatePath(RightRenalVein, Ground, BGE::CardiovascularPath::RightKidneyBleed);
  RightKidneyBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  ///////////////////////////////////
  /////////////////
  // Hemorrhage from left kidney//
  SEFluidCircuitPath& LeftKidneyBleed = cRenal.CreatePath(LeftRenalVein, Ground, BGE::CardiovascularPath::LeftKidneyBleed);
  LeftKidneyBleed.GetResistanceBaseline().SetValue(m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::mmHg_s_Per_mL), FlowResistanceUnit::mmHg_s_Per_mL);
  ///////////////////////////////////
  // Right Urine //
  /////////////////
  ///////////////////////////////////
  // RightBowmansCapsulesToTubules //
  SEFluidCircuitPath& RightBowmansCapsulesToTubules = cRenal.CreatePath(RightBowmansCapsules, RightTubules, BGE::RenalPath::RightBowmansCapsulesToTubules);
  RightBowmansCapsulesToTubules.GetResistanceBaseline().SetValue(tubulesResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////
  // RightTubulesToUreter //
  SEFluidCircuitPath& RightTubulesToUreter = cRenal.CreatePath(RightTubules, RightUreter, BGE::RenalPath::RightTubulesToUreter);
  RightTubulesToUreter.GetResistanceBaseline().SetValue(ureterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  /////////////////////////////
  // RighTubulesToNetTubules //
  SEFluidCircuitPath& RightTubulesToNetTubules = cRenal.CreatePath(RightTubules, RightNetTubules, BGE::RenalPath::RightTubulesToNetTubules);
  RightTubulesToNetTubules.GetPressureSourceBaseline().SetValue(tubulesOsmoticPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////////////////////////
  // RightNetTubulesToNetPeritubularCapillaries //
  SEFluidCircuitPath& RightNetTubulesToNetPeritubularCapillaries = cRenal.CreatePath(RightNetTubules, RightNetPeritubularCapillaries, BGE::RenalPath::RightNetTubulesToNetPeritubularCapillaries);
  RightNetTubulesToNetPeritubularCapillaries.GetResistanceBaseline().SetValue(reabsoprtionResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////////////////////
  // RightPeritubularCapillariesToNetPeritubularCapillaries //
  SEFluidCircuitPath& RightPeritubularCapillariesToNetPeritubularCapillaries = cRenal.CreatePath(RightPeritubularCapillaries, RightNetPeritubularCapillaries, BGE::RenalPath::RightPeritubularCapillariesToNetPeritubularCapillaries);
  RightPeritubularCapillariesToNetPeritubularCapillaries.GetPressureSourceBaseline().SetValue(peritubularOsmoticPressure_mmHg, PressureUnit::mmHg);
  //////////////////////////
  // RightUreterToBladder //
  SEFluidCircuitPath& RightUreterToBladder = cRenal.CreatePath(RightUreter, Bladder, BGE::RenalPath::RightUreterToBladder);
  RightUreterToBladder.SetNextValve(CDM::enumOpenClosed::Closed);

  /////////////////
  // Left Blood //
  /////////////////
  ///////////////////////////////////////
  // LeftAortaConnectionToRenalArtery //
  SEFluidCircuitPath& LeftAortaConnectionToRenalArtery = cRenal.CreatePath(LeftAortaConnection, LeftRenalArtery, BGE::RenalPath::LeftAortaConnectionToRenalArtery);
  //////////////////////
  // LeftRenalArteryToAfferentArteriole //
  SEFluidCircuitPath& LeftRenalArteryToAfferentArteriole = cRenal.CreatePath(LeftRenalArtery, LeftAfferentArteriole, BGE::RenalPath::LeftRenalArteryToAfferentArteriole);
  LeftRenalArteryToAfferentArteriole.GetResistanceBaseline().SetValue(renalArteryResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////
  // LeftRenalArteryCompliance //
  SEFluidCircuitPath& LeftRenalArteryCompliance = cRenal.CreatePath(LeftRenalArtery, Ground, BGE::RenalPath::LeftRenalArteryCompliance);
  LeftRenalArteryCompliance.GetComplianceBaseline().SetValue(renalArteryCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  ///////////////////////////////////////////////////
  // LeftAfferentArterioleToGlomerularCapillaries //
  SEFluidCircuitPath& LeftAfferentArterioleToGlomerularCapillaries = cRenal.CreatePath(LeftAfferentArteriole, LeftGlomerularCapillaries, BGE::RenalPath::LeftAfferentArterioleToGlomerularCapillaries);
  LeftAfferentArterioleToGlomerularCapillaries.GetResistanceBaseline().SetValue(afferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ///////////////////////////////////////////////////
  // LeftGlomerularCapillariesToEfferentArteriole //
  SEFluidCircuitPath& LeftGlomerularCapillariesToEfferentArteriole = cRenal.CreatePath(LeftGlomerularCapillaries, LeftEfferentArteriole, BGE::RenalPath::LeftGlomerularCapillariesToEfferentArteriole);
  LeftGlomerularCapillariesToEfferentArteriole.GetResistanceBaseline().SetValue(glomerularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////////////////////
  // LeftGlomerularCapillariesCompliance //
  SEFluidCircuitPath& LeftGlomerularCapillariesCompliance = cRenal.CreatePath(LeftGlomerularCapillaries, Ground, BGE::RenalPath::LeftGlomerularCapillariesCompliance);
  LeftGlomerularCapillariesCompliance.GetComplianceBaseline().SetValue(glomerularCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  ////////////////////////////////////////////////////
  // LeftEfferentArterioleToPeritubularCapillaries //
  SEFluidCircuitPath& LeftEfferentArterioleToPeritubularCapillaries = cRenal.CreatePath(LeftEfferentArteriole, LeftPeritubularCapillaries, BGE::RenalPath::LeftEfferentArterioleToPeritubularCapillaries);
  LeftEfferentArterioleToPeritubularCapillaries.GetResistanceBaseline().SetValue(efferentResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////
  // LeftPeritubularCapillariesToRenalVein //
  SEFluidCircuitPath& LeftPeritubularCapillariesToRenalVein = cRenal.CreatePath(LeftPeritubularCapillaries, LeftRenalVein, BGE::RenalPath::LeftPeritubularCapillariesToRenalVein);
  LeftPeritubularCapillariesToRenalVein.GetResistanceBaseline().SetValue(peritubularResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////
  // LeftRenalVeinToVenaCavaConnection //
  SEFluidCircuitPath& LeftRenalVeinToVenaCavaConnection = cRenal.CreatePath(LeftRenalVein, LeftVenaCavaConnection, BGE::RenalPath::LeftRenalVeinToVenaCavaConnection);
  LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline().SetValue(renalVeinResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////////
  // LeftRenalVeinCompliance //
  SEFluidCircuitPath& LeftRenalVeinCompliance = cRenal.CreatePath(LeftRenalVein, Ground, BGE::RenalPath::LeftRenalVeinCompliance);
  LeftRenalVeinCompliance.GetComplianceBaseline().SetValue(renalVeinCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  //////////////////////////////////////////////////////////
  // LeftGlomerularCapillariesToNetGlomerularCapillaries //
  SEFluidCircuitPath& LeftGlomerularCapillariesToNetGlomerularCapillaries = cRenal.CreatePath(LeftGlomerularCapillaries, LeftNetGlomerularCapillaries, BGE::RenalPath::LeftGlomerularCapillariesToNetGlomerularCapillaries);
  LeftGlomerularCapillariesToNetGlomerularCapillaries.GetPressureSourceBaseline().SetValue(glomerularOsmoticPressure_mmHg, PressureUnit::mmHg);
  ///////////////////////////////////////////////////////
  // LeftNetGlomerularCapillariesToNetBowmansCapsules //
  SEFluidCircuitPath& LeftNetGlomerularCapillariesToNetBowmansCapsules = cRenal.CreatePath(LeftNetGlomerularCapillaries, LeftNetBowmansCapsules, BGE::RenalPath::LeftNetGlomerularCapillariesToNetBowmansCapsules);
  LeftNetGlomerularCapillariesToNetBowmansCapsules.GetResistanceBaseline().SetValue(glomerularFilterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  LeftNetGlomerularCapillariesToNetBowmansCapsules.SetNextPolarizedState(CDM::enumOpenClosed::Open);
  //////////////////////////////////////////////
  // LeftBowmansCapsulesToNetBowmansCapsules //
  SEFluidCircuitPath& LeftBowmansCapsulesToNetBowmansCapsules = cRenal.CreatePath(LeftBowmansCapsules, LeftNetBowmansCapsules, BGE::RenalPath::LeftBowmansCapsulesToNetBowmansCapsules);
  LeftBowmansCapsulesToNetBowmansCapsules.GetPressureSourceBaseline().SetValue(bowmansOsmoticPressure_mmHg, PressureUnit::mmHg);
  /////////////////
  // Left Urine //
  /////////////////
  ///////////////////////////////////
  // LeftBowmansCapsulesToTubules //
  SEFluidCircuitPath& LeftBowmansCapsulesToTubules = cRenal.CreatePath(LeftBowmansCapsules, LeftTubules, BGE::RenalPath::LeftBowmansCapsulesToTubules);
  LeftBowmansCapsulesToTubules.GetResistanceBaseline().SetValue(tubulesResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  //////////////////////////
  // LeftTubulesToUreter //
  SEFluidCircuitPath& LeftTubulesToUreter = cRenal.CreatePath(LeftTubules, LeftUreter, BGE::RenalPath::LeftTubulesToUreter);
  LeftTubulesToUreter.GetResistanceBaseline().SetValue(ureterResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  /////////////////////////////
  // LeftubulesToNetTubules //
  SEFluidCircuitPath& LeftTubulesToNetTubules = cRenal.CreatePath(LeftTubules, LeftNetTubules, BGE::RenalPath::LeftTubulesToNetTubules);
  LeftTubulesToNetTubules.GetPressureSourceBaseline().SetValue(tubulesOsmoticPressure_mmHg, PressureUnit::mmHg);
  ////////////////////////////////////////////////
  // LeftNetTubulesToNetPeritubularCapillaries //
  SEFluidCircuitPath& LeftNetTubulesToNetPeritubularCapillaries = cRenal.CreatePath(LeftNetTubules, LeftNetPeritubularCapillaries, BGE::RenalPath::LeftNetTubulesToNetPeritubularCapillaries);
  LeftNetTubulesToNetPeritubularCapillaries.GetResistanceBaseline().SetValue(reabsoprtionResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);
  ////////////////////////////////////////////////////////////
  // LeftPeritubularCapillariesToNetPeritubularCapillaries //
  SEFluidCircuitPath& LeftPeritubularCapillariesToNetPeritubularCapillaries = cRenal.CreatePath(LeftPeritubularCapillaries, LeftNetPeritubularCapillaries, BGE::RenalPath::LeftPeritubularCapillariesToNetPeritubularCapillaries);
  LeftPeritubularCapillariesToNetPeritubularCapillaries.GetPressureSourceBaseline().SetValue(peritubularOsmoticPressure_mmHg, PressureUnit::mmHg);
  //////////////////////////
  // LeftUreterToBladder //
  SEFluidCircuitPath& LeftUreterToBladder = cRenal.CreatePath(LeftUreter, Bladder, BGE::RenalPath::LeftUreterToBladder);
  LeftUreterToBladder.SetNextValve(CDM::enumOpenClosed::Closed);

  ///////////////////////
  // BladderCompliance //
  SEFluidCircuitPath& BladderToGroundPressure = cRenal.CreatePath(Bladder, Ground, BGE::RenalPath::BladderToGroundPressure);
  /// \todo Use a compliance here - make sure you remove the current handling of bladder volume in the renal system as a pressure source
  //BladderCompliance.GetComplianceBaseline().SetValue(bladderCompliance_mL_Per_mmHg, FlowComplianceUnit::mL_Per_mmHg);
  BladderToGroundPressure.GetPressureSourceBaseline().SetValue(-4.0, PressureUnit::mmHg); //Negative because source-target is for compliance
  //////////////
  // BladderGround //
  SEFluidCircuitPath& BladderToGroundUrinate = cRenal.CreatePath(Bladder, Ground, BGE::RenalPath::BladderToGroundUrinate);
  BladderToGroundUrinate.GetResistanceBaseline().SetValue(urethraResistance_mmHg_s_Per_mL, FlowResistanceUnit::mmHg_s_Per_mL);

  cRenal.SetNextAndCurrentFromBaselines();
  cRenal.StateChange();

  // Delete the three-element Windkessel kidney model
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::LeftKidney1);
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::LeftKidney2);
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::RightKidney1);
  m_Circuits->DeleteFluidNode(BGE::CardiovascularNode::RightKidney2);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Aorta1ToLeftKidney1);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::Aorta1ToRightKidney1);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::LeftKidney1ToLeftKidney2);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::RightKidney1ToRightKidney2);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::LeftKidney1ToGround);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::RightKidney1ToGround);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::LeftKidney2ToVenaCava);
  m_Circuits->DeleteFluidPath(BGE::CardiovascularPath::RightKidney2ToVenaCava);
  cCombinedCardiovascular.AddCircuit(cRenal);
  // Grab the nodes that we will be connecting between the 2 circuits
  SEFluidCircuitNode* Aorta1 = cCardiovascular.GetNode(BGE::CardiovascularNode::Aorta1);
  SEFluidCircuitNode* VenaCava = cCardiovascular.GetNode(BGE::CardiovascularNode::VenaCava);
  // Add the new connection paths
  SEFluidCircuitPath& NewAorta1ToRightKidney = cCombinedCardiovascular.CreatePath(*Aorta1, RightAortaConnection, BGE::CardiovascularPath::Aorta1ToRightKidney1);
  SEFluidCircuitPath& NewRightKidneyToVenaCava = cCombinedCardiovascular.CreatePath(RightVenaCavaConnection, *VenaCava, BGE::CardiovascularPath::RightKidney2ToVenaCava);
  SEFluidCircuitPath& NewAorta1ToLeftKidney = cCombinedCardiovascular.CreatePath(*Aorta1, LeftAortaConnection, BGE::CardiovascularPath::Aorta1ToLeftKidney1);
  SEFluidCircuitPath& NewLeftKidneyToVenaCava = cCombinedCardiovascular.CreatePath(LeftVenaCavaConnection, *VenaCava, BGE::CardiovascularPath::LeftKidney2ToVenaCava);
  // We need to move the resistances
  NewAorta1ToRightKidney.GetResistanceBaseline().Set(RightAortaConnectionToRenalArtery.GetResistanceBaseline());
  NewAorta1ToRightKidney.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  RightAortaConnectionToRenalArtery.GetResistanceBaseline().Invalidate();
  NewRightKidneyToVenaCava.GetResistanceBaseline().Set(RightRenalVeinToVenaCavaConnection.GetResistanceBaseline());
  RightRenalVeinToVenaCavaConnection.GetResistanceBaseline().Invalidate();
  NewAorta1ToLeftKidney.GetResistanceBaseline().Set(LeftAortaConnectionToRenalArtery.GetResistanceBaseline());
  NewAorta1ToLeftKidney.SetCardiovascularRegion(CDM::enumResistancePathType::Extrasplanchnic);
  LeftAortaConnectionToRenalArtery.GetResistanceBaseline().Invalidate();
  NewLeftKidneyToVenaCava.GetResistanceBaseline().Set(LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline());
  LeftRenalVeinToVenaCavaConnection.GetResistanceBaseline().Invalidate();
  // Update the circuit
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  ////////////////////////
  // Renal Compartments //
  ////////////////////////

  ///////////
  // Blood //
  ///////////

  ///////////////////////
  // RightRenalArtery //
  SELiquidCompartment& vRightRenalArtery = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightRenalArtery);
  vRightRenalArtery.MapNode(RightRenalArtery);
  //////////////////////////////
  // RightAfferentArteriole //
  SELiquidCompartment& vRightAfferentArteriole = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightAfferentArteriole);
  vRightAfferentArteriole.MapNode(RightAfferentArteriole);
  ////////////////////////////////
  // RightGlomerularCapillaries //
  SELiquidCompartment& vRightGlomerularCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightGlomerularCapillaries);
  vRightGlomerularCapillaries.MapNode(RightGlomerularCapillaries);
  vRightGlomerularCapillaries.MapNode(RightNetGlomerularCapillaries);
  ////////////////////////////
  // RightEfferentArteriole //
  SELiquidCompartment& vRightEfferentArteriole = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightEfferentArteriole);
  vRightEfferentArteriole.MapNode(RightEfferentArteriole);
  /////////////////////////////////
  // RightPeritubularCapillaries //
  SELiquidCompartment& vRightPeritubularCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightPeritubularCapillaries);
  vRightPeritubularCapillaries.MapNode(RightPeritubularCapillaries);
  vRightPeritubularCapillaries.MapNode(RightNetPeritubularCapillaries);
  ///////////////////
  // RightRenalVein //
  SELiquidCompartment& vRightRenalVein = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightRenalVein);
  vRightRenalVein.MapNode(RightRenalVein);
  //////////////////////////
  // RightBowmansCapsules //
  SELiquidCompartment& vRightBowmansCapsules = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightBowmansCapsules);
  vRightBowmansCapsules.MapNode(RightBowmansCapsules);
  vRightBowmansCapsules.MapNode(RightNetBowmansCapsules);
  //////////////////
  // RightTubules //
  SELiquidCompartment& vRightTubules = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightTubules);
  vRightTubules.MapNode(RightTubules);
  vRightTubules.MapNode(RightNetTubules);

  /////////////////////
  // LeftRenalArtery //
  SELiquidCompartment& vLeftRenalArtery = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftRenalArtery);
  vLeftRenalArtery.MapNode(LeftRenalArtery);
  //////////////////////////////
  // LeftAfferentArteriole //
  SELiquidCompartment& vLeftAfferentArteriole = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftAfferentArteriole);
  vLeftAfferentArteriole.MapNode(LeftAfferentArteriole);
  ////////////////////////////////
  // LeftGlomerularCapillaries //
  SELiquidCompartment& vLeftGlomerularCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftGlomerularCapillaries);
  vLeftGlomerularCapillaries.MapNode(LeftGlomerularCapillaries);
  vLeftGlomerularCapillaries.MapNode(LeftNetGlomerularCapillaries);
  ////////////////////////////
  // LeftEfferentArteriole //
  SELiquidCompartment& vLeftEfferentArteriole = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftEfferentArteriole);
  vLeftEfferentArteriole.MapNode(LeftEfferentArteriole);
  ////////////////////////////////
  // LeftPeritubularCapillaries //
  SELiquidCompartment& vLeftPeritubularCapillaries = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftPeritubularCapillaries);
  vLeftPeritubularCapillaries.MapNode(LeftPeritubularCapillaries);
  vLeftPeritubularCapillaries.MapNode(LeftNetPeritubularCapillaries);
  ///////////////////
  // LeftRenalVein //
  SELiquidCompartment& vLeftRenalVein = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftRenalVein);
  vLeftRenalVein.MapNode(LeftRenalVein);
  /////////////////////////
  // LeftBowmansCapsules //
  SELiquidCompartment& vLeftBowmansCapsules = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftBowmansCapsules);
  vLeftBowmansCapsules.MapNode(LeftBowmansCapsules);
  vLeftBowmansCapsules.MapNode(LeftNetBowmansCapsules);
  /////////////////
  // LeftTubules //
  SELiquidCompartment& vLeftTubules = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftTubules);
  vLeftTubules.MapNode(LeftTubules);
  vLeftTubules.MapNode(LeftNetTubules);

  // Let's build out the hierarchy
  // Grab these, as cardiovascular already made them
  SELiquidCompartment* vLeftKidney = m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::LeftKidney);
  SELiquidCompartment& vLeftNephron = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::LeftNephron);
  vLeftKidney->GetNodeMapping().Clear(); // Remove the nodes the cardiovascular was using to model the kidney
  vLeftKidney->AddChild(vLeftRenalArtery);
  vLeftKidney->AddChild(vLeftNephron);
  vLeftNephron.AddChild(vLeftAfferentArteriole);
  vLeftNephron.AddChild(vLeftGlomerularCapillaries);
  vLeftNephron.AddChild(vLeftEfferentArteriole);
  vLeftNephron.AddChild(vLeftPeritubularCapillaries);
  vLeftNephron.AddChild(vLeftBowmansCapsules);
  vLeftNephron.AddChild(vLeftTubules);
  vLeftKidney->AddChild(vLeftRenalVein);
  SELiquidCompartment* vRightKidney = m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::RightKidney);
  SELiquidCompartment& vRightNephron = m_Compartments->CreateLiquidCompartment(BGE::VascularCompartment::RightNephron);
  vRightKidney->GetNodeMapping().Clear(); // Remove the nodes the cardiovascular was using to model the kidney
  vRightKidney->AddChild(vRightRenalArtery);
  vRightKidney->AddChild(vRightNephron);
  vRightNephron.AddChild(vRightAfferentArteriole);
  vRightNephron.AddChild(vRightGlomerularCapillaries);
  vRightNephron.AddChild(vRightEfferentArteriole);
  vRightNephron.AddChild(vRightPeritubularCapillaries);
  vRightNephron.AddChild(vRightBowmansCapsules);
  vRightNephron.AddChild(vRightTubules);
  vRightKidney->AddChild(vRightRenalVein);

  ///////////
  // Urine //
  ///////////

  /////////////////
  // RightUreter //
  SELiquidCompartment& uRightUreter = m_Compartments->CreateLiquidCompartment(BGE::UrineCompartment::RightUreter);
  uRightUreter.MapNode(RightUreter);
  ////////////////
  // LeftUreter //
  SELiquidCompartment& uLeftUreter = m_Compartments->CreateLiquidCompartment(BGE::UrineCompartment::LeftUreter);
  uLeftUreter.MapNode(LeftUreter);
  ////////////////
  // Ureters //
  SELiquidCompartment& uUreters = m_Compartments->CreateLiquidCompartment(BGE::UrineCompartment::Ureters);
  uUreters.AddChild(uRightUreter);
  uUreters.AddChild(uLeftUreter);
  /////////////
  // Bladder //
  SELiquidCompartment& uBladder = m_Compartments->CreateLiquidCompartment(BGE::UrineCompartment::Bladder);
  uBladder.MapNode(Bladder);

  /////////////////////////////
  // Renal Compartment Links //
  /////////////////////////////

  // Graph Dependencies
  SELiquidCompartment& vAorta = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::Aorta);
  SELiquidCompartment& vVenaCava = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  SELiquidCompartment& vGround = *m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::Ground);

  ///////////
  // Blood //
  ///////////

  ////////////////////////////
  // AortaToRightRenalArtery //
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::AortaToRightKidney); // Replace this link
  SELiquidCompartmentLink& vAortaToRightRenalArtery = m_Compartments->CreateLiquidLink(vAorta, vRightRenalArtery, BGE::VascularLink::AortaToRightKidney);
  vAortaToRightRenalArtery.MapPath(RightAortaConnectionToRenalArtery);
  ////////////////////////////////////////
  // RightRenalArteryToAfferentArteriole //
  SELiquidCompartmentLink& vRightRenalArteryToAfferentArteriole = m_Compartments->CreateLiquidLink(vRightRenalArtery, vRightAfferentArteriole, BGE::VascularLink::RightRenalArteryToAfferentArteriole);
  vRightRenalArteryToAfferentArteriole.MapPath(RightRenalArteryToAfferentArteriole);
  //////////////////////////////////////////////////
  // RightAfferentArterioleToGlomerularCapillaries //
  SELiquidCompartmentLink& vRightAfferentArterioleToGlomerularCapillaries = m_Compartments->CreateLiquidLink(vRightAfferentArteriole, vRightGlomerularCapillaries, BGE::VascularLink::RightAfferentArterioleToGlomerularCapillaries);
  vRightAfferentArterioleToGlomerularCapillaries.MapPath(RightAfferentArterioleToGlomerularCapillaries);
  //////////////////////////////////////////////////
  // RightGlomerularCapillariesToEfferentArteriole //
  SELiquidCompartmentLink& vRightGlomerularCapillariesToEfferentArteriole = m_Compartments->CreateLiquidLink(vRightGlomerularCapillaries, vRightEfferentArteriole, BGE::VascularLink::RightGlomerularCapillariesToEfferentArteriole);
  vRightGlomerularCapillariesToEfferentArteriole.MapPath(RightGlomerularCapillariesToEfferentArteriole);
  /////////////////////////////////////////////////
  // RightGlomerularCapillariesToBowmansCapsules //
  SELiquidCompartmentLink& vRightGlomerularCapillariesToBowmansCapsules = m_Compartments->CreateLiquidLink(vRightGlomerularCapillaries, vRightBowmansCapsules, BGE::VascularLink::RightGlomerularCapillariesToBowmansCapsules);
  vRightGlomerularCapillariesToBowmansCapsules.MapPath(RightNetGlomerularCapillariesToNetBowmansCapsules);
  ///////////////////////////////////
  // RightBowmansCapsulesToTubules //
  SELiquidCompartmentLink& vRightBowmansCapsulesToTubules = m_Compartments->CreateLiquidLink(vRightBowmansCapsules, vRightTubules, BGE::VascularLink::RightBowmansCapsulesToTubules);
  vRightBowmansCapsulesToTubules.MapPath(RightBowmansCapsulesToTubules);
  //////////////////////////////////////////
  // RightTubulesToPeritubularCapillaries //
  SELiquidCompartmentLink& vRightTubulesToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vRightTubules, vRightPeritubularCapillaries, BGE::VascularLink::RightTubulesToPeritubularCapillaries);
  vRightTubulesToPeritubularCapillaries.MapPath(RightNetTubulesToNetPeritubularCapillaries);
  ///////////////////////////////////////////////////
  // RightEfferentArterioleToPeritubularCapillaries //
  SELiquidCompartmentLink& vRightEfferentArterioleToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vRightEfferentArteriole, vRightPeritubularCapillaries, BGE::VascularLink::RightEfferentArterioleToPeritubularCapillaries);
  vRightEfferentArterioleToPeritubularCapillaries.MapPath(RightEfferentArterioleToPeritubularCapillaries);
  ////////////////////////////////////////////
  // RightPeritubularCapillariesToRenalVein //
  SELiquidCompartmentLink& vRightPeritubularCapillariesToRenalVein = m_Compartments->CreateLiquidLink(vRightPeritubularCapillaries, vRightRenalVein, BGE::VascularLink::RightPeritubularCapillariesToRenalVein);
  vRightPeritubularCapillariesToRenalVein.MapPath(RightPeritubularCapillariesToRenalVein);
  /////////////////////////////
  // RightRenalVeinToVenaCava //
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::RightKidneyToVenaCava); // Replace this vink
  SELiquidCompartmentLink& vRightRenalVeinToVenaCava = m_Compartments->CreateLiquidLink(vRightRenalVein, vVenaCava, BGE::VascularLink::RightKidneyToVenaCava);
  vRightRenalVeinToVenaCava.MapPath(RightRenalVeinToVenaCavaConnection);
  /////////////////////////////
  // Hemorrhage //
  SELiquidCompartmentLink& vLeftKidneyHemorrhage = m_Compartments->CreateLiquidLink(vLeftRenalVein, vGround, BGE::VascularLink::LeftKidneyHemorrhage);
  vLeftKidneyHemorrhage.MapPath(LeftKidneyBleed);
  // Hemorrhage //
  SELiquidCompartmentLink& vRightKidneyHemorrhage = m_Compartments->CreateLiquidLink(vRightRenalVein, vGround, BGE::VascularLink::RightKidneyHemorrhage);
  vRightKidneyHemorrhage.MapPath(RightKidneyBleed);

  ////////////////////////////
  // AortaToLeftRenalArtery //
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::AortaToLeftKidney); // Replace this link
  SELiquidCompartmentLink& vAortaToLeftRenalArtery = m_Compartments->CreateLiquidLink(vAorta, vLeftRenalArtery, BGE::VascularLink::AortaToLeftKidney);
  vAortaToLeftRenalArtery.MapPath(LeftAortaConnectionToRenalArtery);
  ////////////////////////////////////////
  // LeftRenalArteryToAfferentArteriole //
  SELiquidCompartmentLink& vLeftRenalArteryToAfferentArteriole = m_Compartments->CreateLiquidLink(vLeftRenalArtery, vLeftAfferentArteriole, BGE::VascularLink::LeftRenalArteryToAfferentArteriole);
  vLeftRenalArteryToAfferentArteriole.MapPath(LeftRenalArteryToAfferentArteriole);
  //////////////////////////////////////////////////
  // LeftAfferentArterioleToGlomerularCapillaries //
  SELiquidCompartmentLink& vLeftAfferentArterioleToGlomerularCapillaries = m_Compartments->CreateLiquidLink(vLeftAfferentArteriole, vLeftGlomerularCapillaries, BGE::VascularLink::LeftAfferentArterioleToGlomerularCapillaries);
  vLeftAfferentArterioleToGlomerularCapillaries.MapPath(LeftAfferentArterioleToGlomerularCapillaries);
  //////////////////////////////////////////////////
  // LeftGlomerularCapillariesToEfferentArteriole //
  SELiquidCompartmentLink& vLeftGlomerularCapillariesToEfferentArteriole = m_Compartments->CreateLiquidLink(vLeftGlomerularCapillaries, vLeftEfferentArteriole, BGE::VascularLink::LeftGlomerularCapillariesToEfferentArteriole);
  vLeftGlomerularCapillariesToEfferentArteriole.MapPath(LeftGlomerularCapillariesToEfferentArteriole);
  ////////////////////////////////////////////////
  // LeftGlomerularCapillariesToBowmansCapsules //
  SELiquidCompartmentLink& vLeftGlomerularCapillariesToBowmansCapsules = m_Compartments->CreateLiquidLink(vLeftGlomerularCapillaries, vLeftBowmansCapsules, BGE::VascularLink::LeftGlomerularCapillariesToBowmansCapsules);
  vLeftGlomerularCapillariesToBowmansCapsules.MapPath(LeftNetGlomerularCapillariesToNetBowmansCapsules);
  //////////////////////////////////
  // LeftBowmansCapsulesToTubules //
  SELiquidCompartmentLink& vLeftBowmansCapsulesToTubules = m_Compartments->CreateLiquidLink(vLeftBowmansCapsules, vLeftTubules, BGE::VascularLink::LeftBowmansCapsulesToTubules);
  vLeftBowmansCapsulesToTubules.MapPath(LeftBowmansCapsulesToTubules);
  /////////////////////////////////////////
  // LeftTubulesToPeritubularCapillaries //
  SELiquidCompartmentLink& vLeftTubulesToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vLeftTubules, vLeftPeritubularCapillaries, BGE::VascularLink::LeftTubulesToPeritubularCapillaries);
  vLeftTubulesToPeritubularCapillaries.MapPath(LeftNetTubulesToNetPeritubularCapillaries);
  ///////////////////////////////////////////////////
  // LeftEfferentArterioleToPeritubularCapillaries //
  SELiquidCompartmentLink& vLeftEfferentArterioleToPeritubularCapillaries = m_Compartments->CreateLiquidLink(vLeftEfferentArteriole, vLeftPeritubularCapillaries, BGE::VascularLink::LeftEfferentArterioleToPeritubularCapillaries);
  vLeftEfferentArterioleToPeritubularCapillaries.MapPath(LeftEfferentArterioleToPeritubularCapillaries);
  ///////////////////////////////////////////
  // LeftPeritubularCapillariesToRenalVein //
  SELiquidCompartmentLink& vLeftPeritubularCapillariesToRenalVein = m_Compartments->CreateLiquidLink(vLeftPeritubularCapillaries, vLeftRenalVein, BGE::VascularLink::LeftPeritubularCapillariesToRenalVein);
  vLeftPeritubularCapillariesToRenalVein.MapPath(LeftPeritubularCapillariesToRenalVein);
  /////////////////////////////
  // LeftRenalVeinToVenaCava //
  m_Compartments->DeleteLiquidLink(BGE::VascularLink::LeftKidneyToVenaCava); // Replace this link
  SELiquidCompartmentLink& vLeftRenalVeinToVenaCava = m_Compartments->CreateLiquidLink(vLeftRenalVein, vVenaCava, BGE::VascularLink::LeftKidneyToVenaCava);
  vLeftRenalVeinToVenaCava.MapPath(LeftRenalVeinToVenaCavaConnection);

  ///////////
  // Urine //
  ///////////

  //////////////////////////
  // RightTubulesToUreter //
  SELiquidCompartmentLink& uRightTubulesToUreter = m_Compartments->CreateLiquidLink(vRightTubules, uRightUreter, BGE::UrineLink::RightTubulesToUreter);
  uRightTubulesToUreter.MapPath(RightTubulesToUreter);
  //////////////////////////
  // RightUreterToBladder //
  SELiquidCompartmentLink& uRightUreterToBladder = m_Compartments->CreateLiquidLink(uRightUreter, uBladder, BGE::UrineLink::RightUreterToBladder);
  uRightUreterToBladder.MapPath(RightUreterToBladder);

  /////////////////////////
  // LeftTubulesToUreter //
  SELiquidCompartmentLink& uLeftTubulesToUreter = m_Compartments->CreateLiquidLink(vLeftTubules, uLeftUreter, BGE::UrineLink::LeftTubulesToUreter);
  uLeftTubulesToUreter.MapPath(LeftTubulesToUreter);
  /////////////////////////
  // LeftUreterToBladder //
  SELiquidCompartmentLink& uLeftUreterToBladder = m_Compartments->CreateLiquidLink(uLeftUreter, uBladder, BGE::UrineLink::LeftUreterToBladder);
  uLeftUreterToBladder.MapPath(LeftUreterToBladder);

  /////////////////////
  // BladderToGround //
  SELiquidCompartmentLink& uBladderToGround = m_Compartments->CreateLiquidLink(uBladder, vGround, BGE::UrineLink::BladderToGround);
  uBladderToGround.MapPath(BladderToGroundUrinate);
  SELiquidCompartmentLink& uBladderToGroundSource = m_Compartments->CreateLiquidLink(uBladder, vGround, BGE::UrineLink::BladderToGroundSource);
  uBladderToGroundSource.MapPath(BladderToGroundPressure);

  SELiquidCompartmentGraph& gRenal = m_Compartments->GetRenalGraph();
  gRenal.AddCompartment(vAorta);
  gRenal.AddCompartment(vVenaCava);
  // Left Blood
  gRenal.AddCompartment(vLeftRenalArtery);
  gRenal.AddCompartment(vLeftAfferentArteriole);
  gRenal.AddCompartment(vLeftGlomerularCapillaries);
  gRenal.AddCompartment(vLeftEfferentArteriole);
  gRenal.AddCompartment(vLeftPeritubularCapillaries);
  gRenal.AddCompartment(vLeftBowmansCapsules);
  gRenal.AddCompartment(vLeftTubules);
  gRenal.AddCompartment(vLeftRenalVein);
  gRenal.AddLink(vAortaToLeftRenalArtery);
  gRenal.AddLink(vLeftRenalArteryToAfferentArteriole);
  gRenal.AddLink(vLeftAfferentArterioleToGlomerularCapillaries);
  gRenal.AddLink(vLeftGlomerularCapillariesToEfferentArteriole);
  //gRenal.AddLink(vLeftGlomerularCapillariesToBowmansCapsules); //Active transport only
  gRenal.AddLink(vLeftBowmansCapsulesToTubules);
  //gRenal.AddLink(vLeftTubulesToPeritubularCapillaries); //Active transport only
  gRenal.AddLink(vLeftEfferentArterioleToPeritubularCapillaries);
  gRenal.AddLink(vLeftPeritubularCapillariesToRenalVein);
  gRenal.AddLink(vLeftRenalVeinToVenaCava);
  // Right Blood
  gRenal.AddCompartment(vRightRenalArtery);
  gRenal.AddCompartment(vRightAfferentArteriole);
  gRenal.AddCompartment(vRightGlomerularCapillaries);
  gRenal.AddCompartment(vRightEfferentArteriole);
  gRenal.AddCompartment(vRightPeritubularCapillaries);
  gRenal.AddCompartment(vRightBowmansCapsules);
  gRenal.AddCompartment(vRightTubules);
  gRenal.AddCompartment(vRightRenalVein);
  gRenal.AddLink(vAortaToRightRenalArtery);
  gRenal.AddLink(vRightRenalArteryToAfferentArteriole);
  gRenal.AddLink(vRightAfferentArterioleToGlomerularCapillaries);
  gRenal.AddLink(vRightGlomerularCapillariesToEfferentArteriole);
  gRenal.AddLink(vLeftKidneyHemorrhage);
  gRenal.AddLink(vRightKidneyHemorrhage);
  //gRenal.AddLink(vRightGlomerularCapillariesToBowmansCapsules); //Active transport only
  gRenal.AddLink(vRightBowmansCapsulesToTubules);
  //gRenal.AddLink(vRightTubulesToPeritubularCapillaries); //Active transport only
  gRenal.AddLink(vRightEfferentArterioleToPeritubularCapillaries);
  gRenal.AddLink(vRightPeritubularCapillariesToRenalVein);
  gRenal.AddLink(vRightRenalVeinToVenaCava);
  // Left Urine
  gRenal.AddCompartment(uLeftUreter);
  gRenal.AddLink(uLeftTubulesToUreter);
  gRenal.AddLink(uLeftUreterToBladder);
  // Right Urine
  gRenal.AddCompartment(uRightUreter);
  gRenal.AddLink(uRightTubulesToUreter);
  gRenal.AddLink(uRightUreterToBladder);
  // Shared
  gRenal.AddCompartment(uBladder);
  gRenal.AddCompartment(vGround);
  gRenal.AddLink(uBladderToGround);
  gRenal.AddLink(uBladderToGroundSource);
  gRenal.StateChange();

  // We have discretized these compartments, so remove them
  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.RemoveCompartment(*vLeftKidney);
  gCombinedCardiovascular.RemoveCompartment(*vRightKidney);
  gCombinedCardiovascular.AddGraph(gRenal);
  gCombinedCardiovascular.StateChange();
}

void BioGears::SetupTissue()
{
  Info("Setting Up Tissue");
  SEFluidCircuit& cCardiovascular = m_Circuits->GetCardiovascularCircuit();
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();
  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();

  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Ground);
  ///////////
  // Lymph //
  SEFluidCircuitNode& Lymph = cCombinedCardiovascular.CreateNode(BGE::TissueNode::Lymph);
  Lymph.GetPressure().SetValue(5.0, PressureUnit::mmHg);
  //If we break out Lymph, Table 8.5 in Nanomedicine: Volume 1 (found @ http://www.nanomedicine.com/NMI/8.2.1.3.htm) has a total capacity of ~2.0 L.
  Lymph.GetVolumeBaseline().SetValue(2.0, VolumeUnit::L);
  double lymphTotalBody_mL_Per_min = 3.5; //This corresponds to ~ 5 L/day of lymph flow in body

  SEFluidCircuitNode* VenaCava = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::VenaCava);
  SEFluidCircuitPath& LymphToVenaCava = cCombinedCardiovascular.CreatePath(Lymph, *VenaCava, BGE::TissuePath::LymphToVenaCava);
  LymphToVenaCava.GetResistanceBaseline().SetValue((Lymph.GetPressure(PressureUnit::mmHg) - VenaCava->GetPressure(PressureUnit::mmHg)) / lymphTotalBody_mL_Per_min, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LymphToGround = cCombinedCardiovascular.CreatePath(Lymph, *Ground, BGE::TissuePath::LymphToGround);
  LymphToGround.GetComplianceBaseline().SetValue(Lymph.GetVolumeBaseline(VolumeUnit::mL) / Lymph.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);
  ///\ToDo:  Use P-V relationship in Himeno2015Mechanisms to get better initial compliance estimate

  SELiquidCompartment* cVenaCava = m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::VenaCava);
  SELiquidCompartment& cLymph = m_Compartments->CreateLiquidCompartment(BGE::LymphCompartment::Lymph);
  cLymph.MapNode(Lymph);

  gCombinedCardiovascular.AddCompartment(cLymph);

  /// \todo Put Initial Circuit/Compartment data values into the configuration file.

  //Density (kg/L)
  double AdiposeTissueDensity = 0.92;
  double BoneTissueDensity = 1.3;
  double BrainTissueDensity = 1.0;
  double GutTissueDensity = 1.0;
  double RKidneyTissueDensity = 1.0;
  double LKidneyTissueDensity = 1.0;
  double LiverTissueDensity = 1.0;
  double RLungTissueDensity = 1.0;
  double LLungTissueDensity = 1.0;
  double MuscleTissueDensity = 1.0;
  double MyocardiumTissueDensity = 1.0;
  double SkinTissueDensity = 1.0;
  double SpleenTissueDensity = 1.0;

  // ExtracellcularWaterFraction        IntracellularWaterFraction    NeutralLipid                   NeutralPhospolipid             AlbuminRatio              AlphaAcidGlycoprotein       PlasmaLipoprotein        AcidicPhospohlipidConcentration
  double AdiposeEWFraction = 0.135, AdiposeIWFraction = 0.017, AdiposeNLFraction = 0.79, AdiposeNPFraction = 0.002, AdiposeARatio = 0.049, AdiposeAAGRatio = 0.049, AdiposeLRatio = 0.068, AdiposeAPL = 0.4;
  double BoneEWFraction = 0.1, BoneIWFraction = 0.346, BoneNLFraction = 0.074, BoneNPFraction = 0.0011, BoneARatio = 0.1, BoneAAGRatio = 0.1, BoneLRatio = 0.05, BoneAPL = 0.67;
  double BrainEWFraction = 0.162, BrainIWFraction = 0.62, BrainNLFraction = 0.051, BrainNPFraction = 0.0565, BrainARatio = 0.048, BrainAAGRatio = 0.048, BrainLRatio = 0.041, BrainAPL = 0.4;
  double GutEWFraction = 0.282, GutIWFraction = 0.475, GutNLFraction = 0.0487, GutNPFraction = 0.0163, GutARatio = 0.158, GutAAGRatio = 0.158, GutLRatio = 0.0141, GutAPL = 2.41;
  double RKidneyEWFraction = 0.273, RKidneyIWFraction = 0.483, RKidneyNLFraction = 0.0207, RKidneyNPFraction = 0.0162, RKidneyARatio = 0.13, RKidneyAAGRatio = 0.13, RKidneyLRatio = 0.137, RKidneyAPL = 5.03;
  double LKidneyEWFraction = 0.273, LKidneyIWFraction = 0.483, LKidneyNLFraction = 0.0207, LKidneyNPFraction = 0.0162, LKidneyARatio = 0.13, LKidneyAAGRatio = 0.13, LKidneyLRatio = 0.137, LKidneyAPL = 5.03;
  double LiverEWFraction = 0.161, LiverIWFraction = 0.573, LiverNLFraction = 0.0348, LiverNPFraction = 0.0252, LiverARatio = 0.086, LiverAAGRatio = 0.086, LiverLRatio = 0.161, LiverAPL = 4.56;
  double RLungEWFraction = 0.336, RLungIWFraction = 0.446, RLungNLFraction = 0.003, RLungNPFraction = 0.009, RLungARatio = 0.212, RLungAAGRatio = 0.212, RLungLRatio = 0.168, RLungAPL = 3.91;
  double LLungEWFraction = 0.336, LLungIWFraction = 0.446, LLungNLFraction = 0.003, LLungNPFraction = 0.009, LLungARatio = 0.212, LLungAAGRatio = 0.212, LLungLRatio = 0.168, LLungAPL = 3.91;
  double MuscleEWFraction = 0.118, MuscleIWFraction = 0.63, MuscleNLFraction = 0.0238, MuscleNPFraction = 0.0072, MuscleARatio = 0.064, MuscleAAGRatio = 0.064, MuscleLRatio = 0.059, MuscleAPL = 1.53;
  double MyocardiumEWFraction = 0.32, MyocardiumIWFraction = 0.456, MyocardiumNLFraction = 0.0115, MyocardiumNPFraction = 0.0166, MyocardiumARatio = 0.157, MyocardiumAAGRatio = 0.157, MyocardiumLRatio = 0.16, MyocardiumAPL = 2.25;
  double SkinEWFraction = 0.382, SkinIWFraction = 0.291, SkinNLFraction = 0.0284, SkinNPFraction = 0.0111, SkinARatio = 0.277, SkinAAGRatio = 0.277, SkinLRatio = 0.096, SkinAPL = 1.32;
  double SpleenEWFraction = 0.207, SpleenIWFraction = 0.579, SpleenNLFraction = 0.0201, SpleenNPFraction = 0.0198, SpleenARatio = 0.277, SpleenAAGRatio = 0.277, SpleenLRatio = 0.096, SpleenAPL = 3.18;

  //Typical ICRP Male
  //Total Mass (kg)
  double AdiposeTissueMass = 14.5;
  double BoneTissueMass = 10.5;
  double BrainTissueMass = 1.45;
  double GutTissueMass = 1.02;
  double RKidneyTissueMass = 0.155;
  double LKidneyTissueMass = 0.155;
  double LiverTissueMass = 1.8;
  double RLungTissueMass = 0.25;
  double LLungTissueMass = 0.25;
  double MuscleTissueMass = 29.0;
  double MyocardiumTissueMass = 0.33;
  double SkinTissueMass = 3.3;
  double SpleenTissueMass = 0.15;

  //Typical ICRP Female - From ICRP
  //Total Mass (kg)
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    AdiposeTissueMass = 19.0;
    BoneTissueMass = 7.8;
    BrainTissueMass = 1.3;
    GutTissueMass = 0.96;
    RKidneyTissueMass = 0.1375;
    LKidneyTissueMass = 0.1375;
    LiverTissueMass = 1.4;
    RLungTissueMass = 0.21;
    LLungTissueMass = 0.21;
    MuscleTissueMass = 17.5;
    MyocardiumTissueMass = 0.25;
    SkinTissueMass = 2.3;
    SpleenTissueMass = 0.13;
  }

  //Scale things based on patient parameters -------------------------------

  //Modify adipose (i.e. fat) directly using the body fat fraction
  AdiposeTissueMass = m_Patient->GetBodyFatFraction().GetValue() * m_Patient->GetWeight().GetValue(MassUnit::kg);

  //Modify skin based on total surface area
  //Male
  double standardPatientWeight_lb = 170.0;
  double standardPatientHeight_in = 71.0;
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    //Female
    standardPatientWeight_lb = 130.0;
    standardPatientHeight_in = 64.0;
  }
  double typicalSkinSurfaceArea_m2 = 0.20247 * std::pow(Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg), 0.425) * std::pow(Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::m), 0.725);
  double patientSkinArea_m2 = m_Patient->GetSkinSurfaceArea(AreaUnit::m2);
  SkinTissueMass = SkinTissueMass * patientSkinArea_m2 / typicalSkinSurfaceArea_m2;

  //Modify most based on lean body mass
  //Hume, R (Jul 1966). "Prediction of lean body mass from height and weight." Journal of clinical pathology. 19 (4): 389–91. doi:10.1136/jcp.19.4.389. PMC 473290. PMID 5929341.
  //double typicalLeanBodyMass_kg = 0.32810 * Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) + 0.33929 * Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::cm) - 29.5336; //Male
  //if (m_Patient->GetGender() == CDM::enumSex::Female)
  //{
  // typicalLeanBodyMass_kg = 0.29569 * Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) + 0.41813 * Convert(standardPatientHeight_in, LengthUnit::in, LengthUnit::cm) - 43.2933; //Female
  //}

  //Male
  double standardFatFraction = 0.21;
  if (m_Patient->GetGender() == CDM::enumSex::Female) {
    //Female
    standardFatFraction = 0.28;
  }
  double standardLeanBodyMass_kg = Convert(standardPatientWeight_lb, MassUnit::lb, MassUnit::kg) * (1.0 - standardFatFraction);
  double patientLeanBodyMass_kg = m_Patient->GetLeanBodyMass(MassUnit::kg);
  double leanBodyMassFractionOfTypical = patientLeanBodyMass_kg / standardLeanBodyMass_kg;

  BoneTissueMass *= leanBodyMassFractionOfTypical;
  GutTissueMass *= leanBodyMassFractionOfTypical;
  RKidneyTissueMass *= leanBodyMassFractionOfTypical;
  LKidneyTissueMass *= leanBodyMassFractionOfTypical;
  LiverTissueMass *= leanBodyMassFractionOfTypical;
  RLungTissueMass *= leanBodyMassFractionOfTypical;
  LLungTissueMass *= leanBodyMassFractionOfTypical;
  MuscleTissueMass *= leanBodyMassFractionOfTypical;
  MyocardiumTissueMass *= leanBodyMassFractionOfTypical;
  SpleenTissueMass *= leanBodyMassFractionOfTypical;

  //Note: Brain doesn't change

  //Total Volume(L)
  double AdiposeTissueVolume = AdiposeTissueMass / AdiposeTissueDensity;
  double BoneTissueVolume = BoneTissueMass / BoneTissueDensity;
  double BrainTissueVolume = BrainTissueMass / BrainTissueDensity;
  double GutTissueVolume = GutTissueMass / GutTissueDensity;
  double RKidneyTissueVolume = RKidneyTissueMass / RKidneyTissueDensity;
  double LKidneyTissueVolume = LKidneyTissueMass / LKidneyTissueDensity;
  double LiverTissueVolume = LiverTissueMass / LiverTissueDensity;
  double RLungTissueVolume = RLungTissueMass / RLungTissueDensity;
  double LLungTissueVolume = LLungTissueMass / LLungTissueDensity;
  double MuscleTissueVolume = MuscleTissueMass / MuscleTissueDensity;
  double MyocardiumTissueVolume = MyocardiumTissueMass / MyocardiumTissueDensity;
  double SkinTissueVolume = SkinTissueMass / SkinTissueDensity;
  double SpleenTissueVolume = SpleenTissueMass / SpleenTissueDensity;

  double totalECWater_L = AdiposeEWFraction * AdiposeTissueVolume + BoneEWFraction * BoneTissueVolume + BrainEWFraction * BrainTissueVolume + GutEWFraction * GutTissueVolume
    + LiverEWFraction * LiverTissueVolume + RLungEWFraction * RLungTissueVolume + LLungEWFraction * LLungTissueVolume + MuscleEWFraction * MuscleTissueVolume + MyocardiumEWFraction * MyocardiumTissueVolume + SkinEWFraction * SkinTissueVolume
    + SpleenEWFraction * SpleenTissueVolume + LKidneyTissueVolume * LKidneyEWFraction + RKidneyTissueVolume * RKidneyEWFraction;

  //Create the circuit -------------------------------

  // Colloid Osmotic Pressure--establish here so we can define pressure sources on tissue circuit (substances haven't been set up yet) and, if we need to, change them without jacking everything up
  /// \cite Mazonni1988Dynamic
  double albuminVascular_g_Per_dL = 4.5;
  double albuminExtracell_g_Per_dL = 2.0;
  double totalPlasamaProtein_g_Per_dL = 1.6 * albuminVascular_g_Per_dL; //Relationship between albumin and total plasma protein
  double totalInterstitialProtein_g_Per_dL = 1.6 * albuminExtracell_g_Per_dL;
  double copVascular_mmHg = 2.1 * totalPlasamaProtein_g_Per_dL + 0.16 * std::pow(totalPlasamaProtein_g_Per_dL, 2) + 0.009 * std::pow(totalPlasamaProtein_g_Per_dL, 3); //Use Landis-Pappenheimer equation to get plasma colloid oncotic pressure
  double copExtracell_mmHg = 2.1 * totalInterstitialProtein_g_Per_dL + 0.16 * std::pow(totalInterstitialProtein_g_Per_dL, 2) + 0.009 * std::pow(totalInterstitialProtein_g_Per_dL, 3); //If we assume only albumin leaks across membrame, use relationshp for albumin colloid pressure from Mazzoni1988Dynamic

  double targetPressureGradient_mmHg = 5.0;
  //Boron: Medical Physiology has total pressure gradient from capillary to interstitium as 12 mmHg at arterial end and -5 mmHg at venous end
  //So there should be a small net gradient towards interstitium.
  double targetHydrostaticGradient_mmHg = targetPressureGradient_mmHg + (copVascular_mmHg - copExtracell_mmHg); //COP gradient opposes flow into capillaries.  Thus, to get to target total gradient we need a hydrostatic gradient to oppose it

  //These modifiers are copied from the SetUp Cardiovascular function.  The volume modification helps tune the vascular node volumes, but it also causes pressure drops
  //on those nodes.  This is a problem because the tissue nodes below have their pressures set based on the pressures set on the CV nodes.  If the pressure in the vasculature
  //drops too much, the gradient favoring filtration to the tissue is lost.  Factoring in these modifiers prevents this.  Note: Did not include modifier for brain, lungs, or kidneys.
  double VolumeModifierBone = 1.175574 * 0.985629, VolumeModifierFat = 1.175573 * 0.986527;
  double VolumeModifierGut = 1.17528 * 0.985609, VolumeModifierLiver = 1.157475 * 0.991848;
  double VolumeModifierMuscle = 1.175573 * 0.986529, VolumeModifierMyocardium = 1.175564 * 0.986531;
  double VolumeModifierSpleen = 1.17528 * 0.986509, VolumeModifierSkin = 1.007306 * 1.035695;

  //Use these and keep recycling for each tissue to help define node and path baselines
  double vNodePressure = 0.0;
  double e1NodePressure = 0.0;
  double e2NodePressure = 0.0;
  double e3NodePressure = 0.0;
  double l1NodePressure = 0.0;
  double l2NodePressure = 0.0;
  double preLymphaticPressureMin_mmHg = 6.0;
  double filteredFlow_mL_Per_min = 0.0;
  double capillaryResistance_mmHg_min_Per_mL = 0.0;
  double lymphResistance_mmHg_min_Per_mL = 0.0;
  double lymphDrivePressure_mmHg = 0.0;

  //Circuit Set-Up
  /*
   V = vascular, E = tissue extracellular, I = tissue intracellular
  >> = pressure source (and direction), ~~~ = resistor, || = capacitor, ( = flow source
  
                  I---||----Ground
                  (
                  (
    V--<<---E1--~~~~-E2--->>--E3--||----Ground
                  (
                  (
      Back to Vena cava <--Lymph
  
  */

  /////////
  // Fat //
  SEFluidCircuitNode* FatV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Fat1);
  SEFluidCircuitNode& FatE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatE1);
  SEFluidCircuitNode& FatE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatE2);
  SEFluidCircuitNode& FatE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatE3);
  SEFluidCircuitNode& FatI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatI);
  SEFluidCircuitNode& FatL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& FatL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::FatL2); //Pre-lymphatic node 2--valve check

  vNodePressure = FatV->GetPressure(PressureUnit::mmHg) / VolumeModifierFat;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (AdiposeTissueVolume * AdiposeEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  FatE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  FatE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  FatE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  FatE3.GetVolumeBaseline().SetValue(AdiposeEWFraction * AdiposeTissueVolume * 1000.0, VolumeUnit::mL);
  FatI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  FatI.GetVolumeBaseline().SetValue(AdiposeIWFraction * AdiposeTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  FatL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  FatL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& FatVToFatE1 = cCombinedCardiovascular.CreatePath(*FatV, FatE1, BGE::TissuePath::FatVToFatE1);
  FatVToFatE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg); // < 0 because directed from extracellular to vascular
  SEFluidCircuitPath& FatE1ToFatE2 = cCombinedCardiovascular.CreatePath(FatE1, FatE2, BGE::TissuePath::FatE1ToFatE2);
  FatE1ToFatE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& FatE2ToFatE3 = cCombinedCardiovascular.CreatePath(FatE2, FatE3, BGE::TissuePath::FatE2ToFatE3);
  FatE2ToFatE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& FatE3ToGround = cCombinedCardiovascular.CreatePath(FatE3, *Ground, BGE::TissuePath::FatE3ToGround);
  FatE3ToGround.GetComplianceBaseline().SetValue(FatE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& FatE3ToFatI = cCombinedCardiovascular.CreatePath(FatE3, FatI, BGE::TissuePath::FatE3ToFatI);
  FatE3ToFatI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& FatIToGround = cCombinedCardiovascular.CreatePath(FatI, *Ground, BGE::TissuePath::FatIToGround);
  FatIToGround.GetComplianceBaseline().SetValue(FatI.GetVolumeBaseline(VolumeUnit::mL) / FatI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& FatE3ToFatL1 = cCombinedCardiovascular.CreatePath(FatE3, FatL1, BGE::TissuePath::FatE3ToFatL1);
  FatE3ToFatL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& FatL1ToFatL2 = cCombinedCardiovascular.CreatePath(FatL1, FatL2, BGE::TissuePath::FatL1ToFatL2);
  FatL1ToFatL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& FatL2ToLymph = cCombinedCardiovascular.CreatePath(FatL2, Lymph, BGE::TissuePath::FatToLymphValve);
  FatL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& FatTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Fat);
  SELiquidCompartment& FatExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::FatExtracellular);
  SELiquidCompartment& FatIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::FatIntracellular);
  FatTissue.GetMatrixVolume().SetValue((1 - AdiposeEWFraction - AdiposeIWFraction) * AdiposeTissueVolume * 1000.0, VolumeUnit::mL);
  FatExtracellular.MapNode(FatE1);
  FatExtracellular.MapNode(FatE2);
  FatExtracellular.MapNode(FatE3);
  FatIntracellular.MapNode(FatI);
  FatExtracellular.MapNode(FatL1);
  FatExtracellular.MapNode(FatL2);
  FatExtracellular.GetWaterVolumeFraction().SetValue(AdiposeEWFraction);
  FatIntracellular.GetWaterVolumeFraction().SetValue(AdiposeIWFraction);
  FatTissue.GetAcidicPhospohlipidConcentration().SetValue(AdiposeAPL, MassPerMassUnit::mg_Per_g);
  FatTissue.GetNeutralLipidsVolumeFraction().SetValue(AdiposeNLFraction);
  FatTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(AdiposeNPFraction);
  FatTissue.GetTissueToPlasmaAlbuminRatio().SetValue(AdiposeARatio);
  FatTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(AdiposeAAGRatio);
  FatTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(AdiposeLRatio);
  FatTissue.GetTotalMass().SetValue(AdiposeTissueMass, MassUnit::kg);
  FatTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  FatTissue.GetReflectionCoefficient().SetValue(1.0);

  //////////
  // Bone //
  SEFluidCircuitNode* BoneV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Bone1);
  SEFluidCircuitNode& BoneE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneE1);
  SEFluidCircuitNode& BoneE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneE2);
  SEFluidCircuitNode& BoneE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneE3);
  SEFluidCircuitNode& BoneI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneI);
  SEFluidCircuitNode& BoneL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& BoneL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BoneL2); //Pre-lymphatic node 2--valve check

  vNodePressure = BoneV->GetPressure(PressureUnit::mmHg) / VolumeModifierBone;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (BoneTissueVolume * BoneEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  BoneE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  BoneE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  BoneE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  BoneE3.GetVolumeBaseline().SetValue(BoneEWFraction * BoneTissueVolume * 1000.0, VolumeUnit::mL);
  BoneI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  BoneI.GetVolumeBaseline().SetValue(BoneIWFraction * BoneTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  BoneL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  BoneL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& BoneVToBoneE1 = cCombinedCardiovascular.CreatePath(*BoneV, BoneE1, BGE::TissuePath::BoneVToBoneE1);
  BoneVToBoneE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BoneE1ToBoneE2 = cCombinedCardiovascular.CreatePath(BoneE1, BoneE2, BGE::TissuePath::BoneE1ToBoneE2);
  BoneE1ToBoneE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& BoneE2ToBoneE3 = cCombinedCardiovascular.CreatePath(BoneE2, BoneE3, BGE::TissuePath::BoneE2ToBoneE3);
  BoneE2ToBoneE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BoneE3ToGround = cCombinedCardiovascular.CreatePath(BoneE3, *Ground, BGE::TissuePath::BoneE3ToGround);
  BoneE3ToGround.GetComplianceBaseline().SetValue(BoneE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& BoneE3ToBoneI = cCombinedCardiovascular.CreatePath(BoneE3, BoneI, BGE::TissuePath::BoneE3ToBoneI);
  BoneE3ToBoneI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& BoneIToGround = cCombinedCardiovascular.CreatePath(BoneI, *Ground, BGE::TissuePath::BoneIToGround);
  BoneIToGround.GetComplianceBaseline().SetValue(BoneI.GetVolumeBaseline(VolumeUnit::mL) / BoneI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& BoneE3ToBoneL1 = cCombinedCardiovascular.CreatePath(BoneE3, BoneL1, BGE::TissuePath::BoneE3ToBoneL1);
  BoneE3ToBoneL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BoneL1ToBoneL2 = cCombinedCardiovascular.CreatePath(BoneL1, BoneL2, BGE::TissuePath::BoneL1ToBoneL2);
  BoneL1ToBoneL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& BoneL2ToLymph = cCombinedCardiovascular.CreatePath(BoneL2, Lymph, BGE::TissuePath::BoneToLymphValve);
  BoneL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& BoneTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Bone);
  SELiquidCompartment& BoneExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::BoneExtracellular);
  SELiquidCompartment& BoneIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::BoneIntracellular);
  BoneTissue.GetMatrixVolume().SetValue((1 - BoneEWFraction - BoneIWFraction) * BoneTissueVolume * 1000.0, VolumeUnit::mL);
  BoneExtracellular.MapNode(BoneE1);
  BoneExtracellular.MapNode(BoneE2);
  BoneExtracellular.MapNode(BoneE3);
  BoneIntracellular.MapNode(BoneI);
  BoneExtracellular.MapNode(BoneL1);
  BoneExtracellular.MapNode(BoneL2);
  BoneExtracellular.GetWaterVolumeFraction().SetValue(BoneEWFraction);
  BoneIntracellular.GetWaterVolumeFraction().SetValue(BoneIWFraction);
  BoneTissue.GetAcidicPhospohlipidConcentration().SetValue(BoneAPL, MassPerMassUnit::mg_Per_g);
  BoneTissue.GetNeutralLipidsVolumeFraction().SetValue(BoneNLFraction);
  BoneTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(BoneNPFraction);
  BoneTissue.GetTissueToPlasmaAlbuminRatio().SetValue(BoneARatio);
  BoneTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(BoneAAGRatio);
  BoneTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(BoneLRatio);
  BoneTissue.GetTotalMass().SetValue(BoneTissueMass, MassUnit::kg);
  BoneTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  BoneTissue.GetReflectionCoefficient().SetValue(1.0);

  ///////////
  // Brain //
  SEFluidCircuitNode* BrainV;
  if (GetConfiguration().IsCerebralEnabled()) {
    BrainV = cCombinedCardiovascular.GetNode(BGE::CerebralNode::CerebralCapillaries);
  } else {
    BrainV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Brain1);
  }

  SEFluidCircuitNode& BrainE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainE1);
  SEFluidCircuitNode& BrainE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainE2);
  SEFluidCircuitNode& BrainE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainE3);
  SEFluidCircuitNode& BrainI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainI);
  SEFluidCircuitNode& BrainL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& BrainL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::BrainL2); //Pre-lymphatic node 2--valve check

  vNodePressure = BrainV->GetPressure(PressureUnit::mmHg); // VolumeModifierBrain;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (BrainTissueVolume * BrainEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  BrainE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  BrainE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  BrainE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  BrainE3.GetVolumeBaseline().SetValue(BrainEWFraction * BrainTissueVolume * 1000.0, VolumeUnit::mL);
  BrainI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  BrainI.GetVolumeBaseline().SetValue(BrainIWFraction * BrainTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  BrainL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  BrainL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& BrainVToBrainE1 = cCombinedCardiovascular.CreatePath(*BrainV, BrainE1, BGE::TissuePath::BrainVToBrainE1);
  BrainVToBrainE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BrainE1ToBrainE2 = cCombinedCardiovascular.CreatePath(BrainE1, BrainE2, BGE::TissuePath::BrainE1ToBrainE2);
  BrainE1ToBrainE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& BrainE2ToBrainE3 = cCombinedCardiovascular.CreatePath(BrainE2, BrainE3, BGE::TissuePath::BrainE2ToBrainE3);
  BrainE2ToBrainE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BrainE3ToGround = cCombinedCardiovascular.CreatePath(BrainE3, *Ground, BGE::TissuePath::BrainE3ToGround);
  //Minus sign in compliance baselines are because the interstitial pressure is negative with respect to atmosphere and we don't want negative compliance
  BrainE3ToGround.GetComplianceBaseline().SetValue(BrainE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& BrainE3ToBrainI = cCombinedCardiovascular.CreatePath(BrainE3, BrainI, BGE::TissuePath::BrainE3ToBrainI);
  BrainE3ToBrainI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& BrainIToGround = cCombinedCardiovascular.CreatePath(BrainI, *Ground, BGE::TissuePath::BrainIToGround);
  BrainIToGround.GetComplianceBaseline().SetValue(-BrainI.GetVolumeBaseline(VolumeUnit::mL) / BrainI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& BrainE3ToBrainL1 = cCombinedCardiovascular.CreatePath(BrainE3, BrainL1, BGE::TissuePath::BrainE3ToBrainL1);
  BrainE3ToBrainL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& BrainL1ToBrainL2 = cCombinedCardiovascular.CreatePath(BrainL1, BrainL2, BGE::TissuePath::BrainL1ToBrainL2);
  BrainL1ToBrainL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& BrainL2ToLymph = cCombinedCardiovascular.CreatePath(BrainL2, Lymph, BGE::TissuePath::BrainToLymphValve);
  BrainL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& BrainTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Brain);
  SELiquidCompartment& BrainExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::BrainExtracellular);
  SELiquidCompartment& BrainIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::BrainIntracellular);
  BrainTissue.GetMatrixVolume().SetValue((1 - BrainEWFraction - BrainIWFraction) * BrainTissueVolume * 1000.0, VolumeUnit::mL);
  BrainExtracellular.MapNode(BrainE1);
  BrainExtracellular.MapNode(BrainE2);
  BrainExtracellular.MapNode(BrainE3);
  BrainIntracellular.MapNode(BrainI);
  BrainExtracellular.MapNode(BrainL1);
  BrainExtracellular.MapNode(BrainL2);
  BrainExtracellular.GetWaterVolumeFraction().SetValue(BrainEWFraction);
  BrainIntracellular.GetWaterVolumeFraction().SetValue(BrainIWFraction);
  BrainTissue.GetAcidicPhospohlipidConcentration().SetValue(BrainAPL, MassPerMassUnit::mg_Per_g);
  BrainTissue.GetNeutralLipidsVolumeFraction().SetValue(BrainNLFraction);
  BrainTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(BrainNPFraction);
  BrainTissue.GetTissueToPlasmaAlbuminRatio().SetValue(BrainARatio);
  BrainTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(BrainAAGRatio);
  BrainTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(BrainLRatio);
  BrainTissue.GetTotalMass().SetValue(BrainTissueMass, MassUnit::kg);
  BrainTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  BrainTissue.GetReflectionCoefficient().SetValue(1.0);

  /////////
  // Gut //
  SEFluidCircuitNode* SmallIntestineV = cCardiovascular.GetNode(BGE::CardiovascularNode::SmallIntestine1);
  SEFluidCircuitNode* LargeIntestineV = cCardiovascular.GetNode(BGE::CardiovascularNode::LargeIntestine1);
  SEFluidCircuitNode* SplanchnicV = cCardiovascular.GetNode(BGE::CardiovascularNode::Splanchnic1);
  SEFluidCircuitNode& GutE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutE1);
  SEFluidCircuitNode& GutE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutE2);
  SEFluidCircuitNode& GutE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutE3);
  SEFluidCircuitNode& GutI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutI);
  SEFluidCircuitNode& GutL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& GutL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::GutL2); //Pre-lymphatic node 2--valve check

  //Gut tissue takes fluid from large and small intestines and splanchnic
  //These vascular compartments have the same pressure setpoints, but take an average just in case they were changed during tuning

  vNodePressure = (SmallIntestineV->GetPressure(PressureUnit::mmHg) + LargeIntestineV->GetPressure(PressureUnit::mmHg) + SplanchnicV->GetPressure(PressureUnit::mmHg)) / 3.0 / VolumeModifierGut;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (GutTissueVolume * GutEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  GutE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  GutE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  GutE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  GutE3.GetVolumeBaseline().SetValue(GutEWFraction * GutTissueVolume * 1000.0, VolumeUnit::mL);
  GutI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  GutI.GetVolumeBaseline().SetValue(GutIWFraction * GutTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  GutL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  GutL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& SmallIntestineVToGutE1 = cCombinedCardiovascular.CreatePath(*SmallIntestineV, GutE1, BGE::TissuePath::SmallIntestineVToGutE1);
  SmallIntestineVToGutE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LargeIntestineVToGutE1 = cCombinedCardiovascular.CreatePath(*LargeIntestineV, GutE1, BGE::TissuePath::LargeIntestineVToGutE1);
  LargeIntestineVToGutE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SplanchnicVToGutE1 = cCombinedCardiovascular.CreatePath(*SplanchnicV, GutE1, BGE::TissuePath::SplanchnicVToGutE1);
  SplanchnicVToGutE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);

  SEFluidCircuitPath& GutE1ToGutE2 = cCombinedCardiovascular.CreatePath(GutE1, GutE2, BGE::TissuePath::GutE1ToGutE2);
  GutE1ToGutE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& GutE2ToGutE3 = cCombinedCardiovascular.CreatePath(GutE2, GutE3, BGE::TissuePath::GutE2ToGutE3);
  GutE2ToGutE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& GutE3ToGround = cCombinedCardiovascular.CreatePath(GutE3, *Ground, BGE::TissuePath::GutE3ToGround);
  GutE3ToGround.GetComplianceBaseline().SetValue(GutE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& GutE3ToGutI = cCombinedCardiovascular.CreatePath(GutE3, GutI, BGE::TissuePath::GutE3ToGutI);
  GutE3ToGutI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& GutIToGround = cCombinedCardiovascular.CreatePath(GutI, *Ground, BGE::TissuePath::GutIToGround);
  GutIToGround.GetComplianceBaseline().SetValue(GutI.GetVolumeBaseline(VolumeUnit::mL) / GutI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& GutE3ToGutL1 = cCombinedCardiovascular.CreatePath(GutE3, GutL1, BGE::TissuePath::GutE3ToGutL1);
  GutE3ToGutL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& GutL1ToGutL2 = cCombinedCardiovascular.CreatePath(GutL1, GutL2, BGE::TissuePath::GutL1ToGutL2);
  GutL1ToGutL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& GutL2ToLymph = cCombinedCardiovascular.CreatePath(GutL2, Lymph, BGE::TissuePath::GutToLymphValve);
  GutL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& GutTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Gut);
  SELiquidCompartment& GutExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::GutExtracellular);
  SELiquidCompartment& GutIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::GutIntracellular);
  GutTissue.GetMatrixVolume().SetValue((1 - GutEWFraction - GutIWFraction) * GutTissueVolume * 1000.0, VolumeUnit::mL);
  GutExtracellular.MapNode(GutE1);
  GutExtracellular.MapNode(GutE2);
  GutExtracellular.MapNode(GutE3);
  GutIntracellular.MapNode(GutI);
  GutExtracellular.MapNode(GutL1);
  GutExtracellular.MapNode(GutL2);
  GutExtracellular.GetWaterVolumeFraction().SetValue(GutEWFraction);
  GutIntracellular.GetWaterVolumeFraction().SetValue(GutIWFraction);
  GutTissue.GetAcidicPhospohlipidConcentration().SetValue(GutAPL, MassPerMassUnit::mg_Per_g);
  GutTissue.GetNeutralLipidsVolumeFraction().SetValue(GutNLFraction);
  GutTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(GutNPFraction);
  GutTissue.GetTissueToPlasmaAlbuminRatio().SetValue(GutARatio);
  GutTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(GutAAGRatio);
  GutTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(GutLRatio);
  GutTissue.GetTotalMass().SetValue(GutTissueMass, MassUnit::kg);
  GutTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  GutTissue.GetReflectionCoefficient().SetValue(1.0);

  /////////////////
  // Left Kidney //
  SEFluidCircuitNode* LeftKidneyV;
  if (!m_Config->IsRenalEnabled()) {
    LeftKidneyV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::LeftKidney1);
  } else {
    LeftKidneyV = cCombinedCardiovascular.GetNode(BGE::RenalNode::LeftGlomerularCapillaries);
  }

  SEFluidCircuitNode& LeftKidneyE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyE1);
  SEFluidCircuitNode& LeftKidneyE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyE2);
  SEFluidCircuitNode& LeftKidneyE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyE3);
  SEFluidCircuitNode& LeftKidneyI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyI);
  SEFluidCircuitNode& LeftKidneyL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& LeftKidneyL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftKidneyL2); //Pre-lymphatic node 2--valve check

  //Kidneys are a little bit different because there is an oncotic pressure source set against glomerular capillaries that increases
  //effective pressure on node quite a bit.  This value is derived from average hydrostatic pressure on node and glomerular oncotic pressure source.
  vNodePressure = 65.474;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (LKidneyTissueVolume * LKidneyEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  LeftKidneyE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  LeftKidneyE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  LeftKidneyE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  LeftKidneyE3.GetVolumeBaseline().SetValue(LKidneyEWFraction * LKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  LeftKidneyI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  LeftKidneyI.GetVolumeBaseline().SetValue(LKidneyIWFraction * LKidneyTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  LeftKidneyL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  LeftKidneyL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& LeftKidneyVToLeftKidneyE1 = cCombinedCardiovascular.CreatePath(*LeftKidneyV, LeftKidneyE1, BGE::TissuePath::LeftKidneyVToLeftKidneyE1);
  LeftKidneyVToLeftKidneyE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftKidneyE1ToLeftKidneyE2 = cCombinedCardiovascular.CreatePath(LeftKidneyE1, LeftKidneyE2, BGE::TissuePath::LeftKidneyE1ToLeftKidneyE2);
  LeftKidneyE1ToLeftKidneyE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LeftKidneyE2ToLeftKidneyE3 = cCombinedCardiovascular.CreatePath(LeftKidneyE2, LeftKidneyE3, BGE::TissuePath::LeftKidneyE2ToLeftKidneyE3);
  LeftKidneyE2ToLeftKidneyE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftKidneyE3ToGround = cCombinedCardiovascular.CreatePath(LeftKidneyE3, *Ground, BGE::TissuePath::LeftKidneyE3ToGround);
  LeftKidneyE3ToGround.GetComplianceBaseline().SetValue(LeftKidneyE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& LeftKidneyE3ToLeftKidneyI = cCombinedCardiovascular.CreatePath(LeftKidneyE3, LeftKidneyI, BGE::TissuePath::LeftKidneyE3ToLeftKidneyI);
  LeftKidneyE3ToLeftKidneyI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& LeftKidneyIToGround = cCombinedCardiovascular.CreatePath(LeftKidneyI, *Ground, BGE::TissuePath::LeftKidneyIToGround);
  LeftKidneyIToGround.GetComplianceBaseline().SetValue(LeftKidneyI.GetVolumeBaseline(VolumeUnit::mL) / LeftKidneyI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& LeftKidneyE3ToLeftKidneyL1 = cCombinedCardiovascular.CreatePath(LeftKidneyE3, LeftKidneyL1, BGE::TissuePath::LeftKidneyE3ToLeftKidneyL1);
  LeftKidneyE3ToLeftKidneyL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftKidneyL1ToLeftKidneyL2 = cCombinedCardiovascular.CreatePath(LeftKidneyL1, LeftKidneyL2, BGE::TissuePath::LeftKidneyL1ToLeftKidneyL2);
  LeftKidneyL1ToLeftKidneyL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LeftKidneyL2ToLymph = cCombinedCardiovascular.CreatePath(LeftKidneyL2, Lymph, BGE::TissuePath::LeftKidneyToLymphValve);
  LeftKidneyL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& LeftKidneyTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::LeftKidney);
  SELiquidCompartment& LeftKidneyExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LeftKidneyExtracellular);
  SELiquidCompartment& LeftKidneyIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LeftKidneyIntracellular);
  LeftKidneyTissue.GetMatrixVolume().SetValue((1 - LKidneyEWFraction - LKidneyIWFraction) * LKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  LeftKidneyExtracellular.MapNode(LeftKidneyE1);
  LeftKidneyExtracellular.MapNode(LeftKidneyE2);
  LeftKidneyExtracellular.MapNode(LeftKidneyE3);
  LeftKidneyIntracellular.MapNode(LeftKidneyI);
  LeftKidneyExtracellular.MapNode(LeftKidneyL1);
  LeftKidneyExtracellular.MapNode(LeftKidneyL2);
  LeftKidneyExtracellular.GetWaterVolumeFraction().SetValue(LKidneyEWFraction);
  LeftKidneyIntracellular.GetWaterVolumeFraction().SetValue(LKidneyIWFraction);
  LeftKidneyTissue.GetAcidicPhospohlipidConcentration().SetValue(LKidneyAPL, MassPerMassUnit::mg_Per_g);
  LeftKidneyTissue.GetNeutralLipidsVolumeFraction().SetValue(LKidneyNLFraction);
  LeftKidneyTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LKidneyNPFraction);
  LeftKidneyTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LKidneyARatio);
  LeftKidneyTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LKidneyAAGRatio);
  LeftKidneyTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LKidneyLRatio);
  LeftKidneyTissue.GetTotalMass().SetValue(LKidneyTissueMass, MassUnit::kg);
  LeftKidneyTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  LeftKidneyTissue.GetReflectionCoefficient().SetValue(1.0);

  ///////////////
  // Left Lung //
  SEFluidCircuitNode* LeftLungV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::LeftPulmonaryCapillaries);
  SEFluidCircuitNode& LeftLungE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungE1);
  SEFluidCircuitNode& LeftLungE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungE2);
  SEFluidCircuitNode& LeftLungE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungE3);
  SEFluidCircuitNode& LeftLungI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungI);
  SEFluidCircuitNode& LeftLungL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& LeftLungL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LeftLungL2); //Pre-lymphatic node 2--valve check

  //vNodePressure = LeftLungV->GetPressure(PressureUnit::mmHg);
  //Using empirical value from previous iteration of tissue circuit because the extracellular lung volume was increasing too much
  //When we revisit oncotic pressure calculations, this might be attributate to different concentrations of albumin in lungs than rest of body
  vNodePressure = 9.339;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (LLungTissueVolume * LLungEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  LeftLungE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  LeftLungE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  LeftLungE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  LeftLungE3.GetVolumeBaseline().SetValue(LLungEWFraction * LLungTissueVolume * 1000.0, VolumeUnit::mL);
  LeftLungI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  LeftLungI.GetVolumeBaseline().SetValue(LLungIWFraction * LLungTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  LeftLungL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  LeftLungL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& LeftLungVToLeftLungE1 = cCombinedCardiovascular.CreatePath(*LeftLungV, LeftLungE1, BGE::TissuePath::LeftLungVToLeftLungE1);
  LeftLungVToLeftLungE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftLungE1ToLeftLungE2 = cCombinedCardiovascular.CreatePath(LeftLungE1, LeftLungE2, BGE::TissuePath::LeftLungE1ToLeftLungE2);
  LeftLungE1ToLeftLungE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LeftLungE2ToLeftLungE3 = cCombinedCardiovascular.CreatePath(LeftLungE2, LeftLungE3, BGE::TissuePath::LeftLungE2ToLeftLungE3);
  LeftLungE2ToLeftLungE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftLungE3ToGround = cCombinedCardiovascular.CreatePath(LeftLungE3, *Ground, BGE::TissuePath::LeftLungE3ToGround);
  //Lung interstitial compliance based of value in Miserocchi1993Pulmonary
  LeftLungE3ToGround.GetComplianceBaseline().SetValue(0.00544 * LeftLungE3.GetVolumeBaseline(VolumeUnit::mL), FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& LeftLungE3ToLeftLungI = cCombinedCardiovascular.CreatePath(LeftLungE3, LeftLungI, BGE::TissuePath::LeftLungE3ToLeftLungI);
  LeftLungE3ToLeftLungI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& LeftLungIToGround = cCombinedCardiovascular.CreatePath(LeftLungI, *Ground, BGE::TissuePath::LeftLungIToGround);
  LeftLungIToGround.GetComplianceBaseline().SetValue(-LeftLungI.GetVolumeBaseline(VolumeUnit::mL) / LeftLungI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& LeftLungE3ToLeftLungL1 = cCombinedCardiovascular.CreatePath(LeftLungE3, LeftLungL1, BGE::TissuePath::LeftLungE3ToLeftLungL1);
  LeftLungE3ToLeftLungL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LeftLungL1ToLeftLungL2 = cCombinedCardiovascular.CreatePath(LeftLungL1, LeftLungL2, BGE::TissuePath::LeftLungL1ToLeftLungL2);
  LeftLungL1ToLeftLungL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LeftLungL2ToLymph = cCombinedCardiovascular.CreatePath(LeftLungL2, Lymph, BGE::TissuePath::LeftLungToLymphValve);
  LeftLungL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& LeftLungTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::LeftLung);
  SELiquidCompartment& LeftLungExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LeftLungExtracellular);
  SELiquidCompartment& LeftLungIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LeftLungIntracellular);
  LeftLungTissue.GetMatrixVolume().SetValue((1 - LLungEWFraction - LLungIWFraction) * LLungTissueVolume * 1000.0, VolumeUnit::mL);
  LeftLungExtracellular.MapNode(LeftLungE1);
  LeftLungExtracellular.MapNode(LeftLungE2);
  LeftLungExtracellular.MapNode(LeftLungE3);
  LeftLungIntracellular.MapNode(LeftLungI);
  LeftLungExtracellular.MapNode(LeftLungL1);
  LeftLungExtracellular.MapNode(LeftLungL2);
  LeftLungExtracellular.GetWaterVolumeFraction().SetValue(LLungEWFraction);
  LeftLungIntracellular.GetWaterVolumeFraction().SetValue(LLungIWFraction);
  LeftLungTissue.GetAcidicPhospohlipidConcentration().SetValue(LLungAPL, MassPerMassUnit::mg_Per_g);
  LeftLungTissue.GetNeutralLipidsVolumeFraction().SetValue(LLungNLFraction);
  LeftLungTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LLungNPFraction);
  LeftLungTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LLungARatio);
  LeftLungTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LLungAAGRatio);
  LeftLungTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LLungLRatio);
  LeftLungTissue.GetTotalMass().SetValue(LLungTissueMass, MassUnit::kg);
  LeftLungTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  LeftLungTissue.GetReflectionCoefficient().SetValue(1.0);

  SELiquidCompartmentLink& LeftLungVascularToTissue = m_Compartments->CreateLiquidLink(*m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::LeftLung),
                                                                                       LeftLungExtracellular, BGE::VascularLink::LeftLungVascularToTissue);
  LeftLungVascularToTissue.MapPath(LeftLungVToLeftLungE1);

  //SELiquidCompartmentLink& LeftLungTissueToLymph = m_Compartments->CreateLiquidLink(LeftLungExtracellular, cLymph, BGE::LymphLink::LeftLungTissueToLymph);
  //LeftLungTissueToLymph.MapPath(LeftLungL2ToLymph);

  ///////////
  // Liver //
  SEFluidCircuitNode* LiverV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Liver1);
  SEFluidCircuitNode& LiverE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverE1);
  SEFluidCircuitNode& LiverE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverE2);
  SEFluidCircuitNode& LiverE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverE3);
  SEFluidCircuitNode& LiverI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverI);
  SEFluidCircuitNode& LiverL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& LiverL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::LiverL2); //Pre-lymphatic node 2--valve check

  vNodePressure = LiverV->GetPressure(PressureUnit::mmHg) / VolumeModifierLiver;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (LiverTissueVolume * LiverEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  LiverE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  LiverE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  LiverE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  LiverE3.GetVolumeBaseline().SetValue(LiverEWFraction * LiverTissueVolume * 1000.0, VolumeUnit::mL);
  LiverI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  LiverI.GetVolumeBaseline().SetValue(LiverIWFraction * LiverTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  LiverL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  LiverL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& LiverVToLiverE1 = cCombinedCardiovascular.CreatePath(*LiverV, LiverE1, BGE::TissuePath::LiverVToLiverE1);
  LiverVToLiverE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LiverE1ToLiverE2 = cCombinedCardiovascular.CreatePath(LiverE1, LiverE2, BGE::TissuePath::LiverE1ToLiverE2);
  LiverE1ToLiverE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LiverE2ToLiverE3 = cCombinedCardiovascular.CreatePath(LiverE2, LiverE3, BGE::TissuePath::LiverE2ToLiverE3);
  LiverE2ToLiverE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LiverE3ToGround = cCombinedCardiovascular.CreatePath(LiverE3, *Ground, BGE::TissuePath::LiverE3ToGround);
  LiverE3ToGround.GetComplianceBaseline().SetValue(LiverE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& LiverE3ToLiverI = cCombinedCardiovascular.CreatePath(LiverE3, LiverI, BGE::TissuePath::LiverE3ToLiverI);
  LiverE3ToLiverI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& LiverIToGround = cCombinedCardiovascular.CreatePath(LiverI, *Ground, BGE::TissuePath::LiverIToGround);
  LiverIToGround.GetComplianceBaseline().SetValue(LiverI.GetVolumeBaseline(VolumeUnit::mL) / LiverI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& LiverE3ToLiverL1 = cCombinedCardiovascular.CreatePath(LiverE3, LiverL1, BGE::TissuePath::LiverE3ToLiverL1);
  LiverE3ToLiverL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& LiverL1ToLiverL2 = cCombinedCardiovascular.CreatePath(LiverL1, LiverL2, BGE::TissuePath::LiverL1ToLiverL2);
  LiverL1ToLiverL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& LiverL2ToLymph = cCombinedCardiovascular.CreatePath(LiverL2, Lymph, BGE::TissuePath::LiverToLymphValve);
  LiverL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& LiverTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Liver);
  SELiquidCompartment& LiverExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LiverExtracellular);
  SELiquidCompartment& LiverIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::LiverIntracellular);
  LiverTissue.GetMatrixVolume().SetValue((1 - LiverEWFraction - LiverIWFraction) * LiverTissueVolume * 1000.0, VolumeUnit::mL);
  LiverExtracellular.MapNode(LiverE1);
  LiverExtracellular.MapNode(LiverE2);
  LiverExtracellular.MapNode(LiverE3);
  LiverIntracellular.MapNode(LiverI);
  LiverExtracellular.MapNode(LiverL1);
  LiverExtracellular.MapNode(LiverL2);
  LiverExtracellular.GetWaterVolumeFraction().SetValue(LiverEWFraction);
  LiverIntracellular.GetWaterVolumeFraction().SetValue(LiverIWFraction);
  LiverTissue.GetAcidicPhospohlipidConcentration().SetValue(LiverAPL, MassPerMassUnit::mg_Per_g);
  LiverTissue.GetNeutralLipidsVolumeFraction().SetValue(LiverNLFraction);
  LiverTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(LiverNPFraction);
  LiverTissue.GetTissueToPlasmaAlbuminRatio().SetValue(LiverARatio);
  LiverTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(LiverAAGRatio);
  LiverTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(LiverLRatio);
  LiverTissue.GetTotalMass().SetValue(LiverTissueMass, MassUnit::kg);
  LiverTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  LiverTissue.GetReflectionCoefficient().SetValue(1.0);

  ////////////
  // Muscle //
  SEFluidCircuitNode* MuscleV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Muscle1);
  SEFluidCircuitNode& MuscleE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleE1);
  SEFluidCircuitNode& MuscleE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleE2);
  SEFluidCircuitNode& MuscleE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleE3);
  SEFluidCircuitNode& MuscleI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleI);
  SEFluidCircuitNode& MuscleL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& MuscleL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MuscleL2); //Pre-lymphatic node 2--valve check

  vNodePressure = MuscleV->GetPressure(PressureUnit::mmHg) / VolumeModifierMuscle;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (MuscleTissueVolume * MuscleEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  MuscleE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  MuscleE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  MuscleE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  MuscleE3.GetVolumeBaseline().SetValue(MuscleEWFraction * MuscleTissueVolume * 1000.0, VolumeUnit::mL);
  MuscleI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  MuscleI.GetVolumeBaseline().SetValue(MuscleIWFraction * MuscleTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  MuscleL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  MuscleL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& MuscleVToMuscleE1 = cCombinedCardiovascular.CreatePath(*MuscleV, MuscleE1, BGE::TissuePath::MuscleVToMuscleE1);
  MuscleVToMuscleE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MuscleE1ToMuscleE2 = cCombinedCardiovascular.CreatePath(MuscleE1, MuscleE2, BGE::TissuePath::MuscleE1ToMuscleE2);
  MuscleE1ToMuscleE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& MuscleE2ToMuscleE3 = cCombinedCardiovascular.CreatePath(MuscleE2, MuscleE3, BGE::TissuePath::MuscleE2ToMuscleE3);
  MuscleE2ToMuscleE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MuscleE3ToGround = cCombinedCardiovascular.CreatePath(MuscleE3, *Ground, BGE::TissuePath::MuscleE3ToGround);
  MuscleE3ToGround.GetComplianceBaseline().SetValue(200.0, FlowComplianceUnit::mL_Per_mmHg); //From Reisner2012Computational
  SEFluidCircuitPath& MuscleE3ToMuscleI = cCombinedCardiovascular.CreatePath(MuscleE3, MuscleI, BGE::TissuePath::MuscleE3ToMuscleI);
  MuscleE3ToMuscleI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& MuscleIToGround = cCombinedCardiovascular.CreatePath(MuscleI, *Ground, BGE::TissuePath::MuscleIToGround);
  MuscleIToGround.GetComplianceBaseline().SetValue(MuscleI.GetVolumeBaseline(VolumeUnit::mL) / MuscleI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& MuscleE3ToMuscleL1 = cCombinedCardiovascular.CreatePath(MuscleE3, MuscleL1, BGE::TissuePath::MuscleE3ToMuscleL1);
  MuscleE3ToMuscleL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MuscleL1ToMuscleL2 = cCombinedCardiovascular.CreatePath(MuscleL1, MuscleL2, BGE::TissuePath::MuscleL1ToMuscleL2);
  MuscleL1ToMuscleL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& MuscleL2ToLymph = cCombinedCardiovascular.CreatePath(MuscleL2, Lymph, BGE::TissuePath::MuscleToLymphValve);
  MuscleL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& MuscleTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Muscle);
  SELiquidCompartment& MuscleExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::MuscleExtracellular);
  SELiquidCompartment& MuscleIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::MuscleIntracellular);
  MuscleTissue.GetMatrixVolume().SetValue((1 - MuscleEWFraction - MuscleIWFraction) * MuscleTissueVolume * 1000.0, VolumeUnit::mL);
  MuscleExtracellular.MapNode(MuscleE1);
  MuscleExtracellular.MapNode(MuscleE2);
  MuscleExtracellular.MapNode(MuscleE3);
  MuscleIntracellular.MapNode(MuscleI);
  MuscleExtracellular.MapNode(MuscleL1);
  MuscleExtracellular.MapNode(MuscleL2);
  MuscleExtracellular.GetWaterVolumeFraction().SetValue(MuscleEWFraction);
  MuscleIntracellular.GetWaterVolumeFraction().SetValue(MuscleIWFraction);
  MuscleTissue.GetAcidicPhospohlipidConcentration().SetValue(MuscleAPL, MassPerMassUnit::mg_Per_g);
  MuscleTissue.GetNeutralLipidsVolumeFraction().SetValue(MuscleNLFraction);
  MuscleTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(MuscleNPFraction);
  MuscleTissue.GetTissueToPlasmaAlbuminRatio().SetValue(MuscleARatio);
  MuscleTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(MuscleAAGRatio);
  MuscleTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(MuscleLRatio);
  MuscleTissue.GetTotalMass().SetValue(MuscleTissueMass, MassUnit::kg);
  MuscleTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  MuscleTissue.GetReflectionCoefficient().SetValue(1.0);

  ////////////////
  // Myocardium //
  SEFluidCircuitNode* MyocardiumV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Myocardium1);
  SEFluidCircuitNode& MyocardiumE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumE1);
  SEFluidCircuitNode& MyocardiumE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumE2);
  SEFluidCircuitNode& MyocardiumE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumE3);
  SEFluidCircuitNode& MyocardiumI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumI);
  SEFluidCircuitNode& MyocardiumL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& MyocardiumL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::MyocardiumL2); //Pre-lymphatic node 2--valve check

  vNodePressure = MyocardiumV->GetPressure(PressureUnit::mmHg) / VolumeModifierMyocardium;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (MyocardiumTissueVolume * MyocardiumEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  MyocardiumE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  MyocardiumE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  MyocardiumE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  MyocardiumE3.GetVolumeBaseline().SetValue(MyocardiumEWFraction * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL);
  MyocardiumI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  MyocardiumI.GetVolumeBaseline().SetValue(MyocardiumIWFraction * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  MyocardiumL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  MyocardiumL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& MyocardiumVToMyocardiumE1 = cCombinedCardiovascular.CreatePath(*MyocardiumV, MyocardiumE1, BGE::TissuePath::MyocardiumVToMyocardiumE1);
  MyocardiumVToMyocardiumE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MyocardiumE1ToMyocardiumE2 = cCombinedCardiovascular.CreatePath(MyocardiumE1, MyocardiumE2, BGE::TissuePath::MyocardiumE1ToMyocardiumE2);
  MyocardiumE1ToMyocardiumE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& MyocardiumE2ToMyocardiumE3 = cCombinedCardiovascular.CreatePath(MyocardiumE2, MyocardiumE3, BGE::TissuePath::MyocardiumE2ToMyocardiumE3);
  MyocardiumE2ToMyocardiumE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MyocardiumE3ToGround = cCombinedCardiovascular.CreatePath(MyocardiumE3, *Ground, BGE::TissuePath::MyocardiumE3ToGround);
  MyocardiumE3ToGround.GetComplianceBaseline().SetValue(MyocardiumE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& MyocardiumE3ToMyocardiumI = cCombinedCardiovascular.CreatePath(MyocardiumE3, MyocardiumI, BGE::TissuePath::MyocardiumE3ToMyocardiumI);
  MyocardiumE3ToMyocardiumI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& MyocardiumIToGround = cCombinedCardiovascular.CreatePath(MyocardiumI, *Ground, BGE::TissuePath::MyocardiumIToGround);
  MyocardiumIToGround.GetComplianceBaseline().SetValue(MyocardiumI.GetVolumeBaseline(VolumeUnit::mL) / MyocardiumI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& MyocardiumE3ToMyocardiumL1 = cCombinedCardiovascular.CreatePath(MyocardiumE3, MyocardiumL1, BGE::TissuePath::MyocardiumE3ToMyocardiumL1);
  MyocardiumE3ToMyocardiumL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& MyocardiumL1ToMyocardiumL2 = cCombinedCardiovascular.CreatePath(MyocardiumL1, MyocardiumL2, BGE::TissuePath::MyocardiumL1ToMyocardiumL2);
  MyocardiumL1ToMyocardiumL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& MyocardiumL2ToLymph = cCombinedCardiovascular.CreatePath(MyocardiumL2, Lymph, BGE::TissuePath::MyocardiumToLymphValve);
  MyocardiumL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& MyocardiumTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Myocardium);
  SELiquidCompartment& MyocardiumExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::MyocardiumExtracellular);
  SELiquidCompartment& MyocardiumIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::MyocardiumIntracellular);
  MyocardiumTissue.GetMatrixVolume().SetValue((1 - MyocardiumEWFraction - MyocardiumIWFraction) * MyocardiumTissueVolume * 1000.0, VolumeUnit::mL);
  MyocardiumExtracellular.MapNode(MyocardiumE1);
  MyocardiumExtracellular.MapNode(MyocardiumE2);
  MyocardiumExtracellular.MapNode(MyocardiumE3);
  MyocardiumIntracellular.MapNode(MyocardiumI);
  MyocardiumExtracellular.MapNode(MyocardiumL1);
  MyocardiumExtracellular.MapNode(MyocardiumL2);
  MyocardiumExtracellular.GetWaterVolumeFraction().SetValue(MyocardiumEWFraction);
  MyocardiumIntracellular.GetWaterVolumeFraction().SetValue(MyocardiumIWFraction);
  MyocardiumTissue.GetAcidicPhospohlipidConcentration().SetValue(MyocardiumAPL, MassPerMassUnit::mg_Per_g);
  MyocardiumTissue.GetNeutralLipidsVolumeFraction().SetValue(MyocardiumNLFraction);
  MyocardiumTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(MyocardiumNPFraction);
  MyocardiumTissue.GetTissueToPlasmaAlbuminRatio().SetValue(MyocardiumARatio);
  MyocardiumTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(MyocardiumAAGRatio);
  MyocardiumTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(MyocardiumLRatio);
  MyocardiumTissue.GetTotalMass().SetValue(MyocardiumTissueMass, MassUnit::kg);
  MyocardiumTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  MyocardiumTissue.GetReflectionCoefficient().SetValue(1.0);

  //////////////////
  // Right Kidney //
  SEFluidCircuitNode* RightKidneyV;
  if (!m_Config->IsRenalEnabled()) {
    RightKidneyV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::RightKidney1);
  } else {
    RightKidneyV = cCombinedCardiovascular.GetNode(BGE::RenalNode::RightGlomerularCapillaries);
  }

  SEFluidCircuitNode& RightKidneyE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyE1);
  SEFluidCircuitNode& RightKidneyE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyE2);
  SEFluidCircuitNode& RightKidneyE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyE3);
  SEFluidCircuitNode& RightKidneyI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyI);
  SEFluidCircuitNode& RightKidneyL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& RightKidneyL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightKidneyL2); //Pre-lymphatic node 2--valve check

  //Kidneys are a little bit different because there is an oncotic pressure source set against glomerular capillaries that increases
  //effective pressure on node quite a bit.  This value is derived from average hydrostatic pressure on node and glomerular oncotic pressure source.
  vNodePressure = 65.474;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (RKidneyTissueVolume * RKidneyEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  RightKidneyE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  RightKidneyE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  RightKidneyE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  RightKidneyE3.GetVolumeBaseline().SetValue(RKidneyEWFraction * RKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  RightKidneyI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  RightKidneyI.GetVolumeBaseline().SetValue(RKidneyIWFraction * RKidneyTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  RightKidneyL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  RightKidneyL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& RightKidneyVToRightKidneyE1 = cCombinedCardiovascular.CreatePath(*RightKidneyV, RightKidneyE1, BGE::TissuePath::RightKidneyVToRightKidneyE1);
  RightKidneyVToRightKidneyE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightKidneyE1ToRightKidneyE2 = cCombinedCardiovascular.CreatePath(RightKidneyE1, RightKidneyE2, BGE::TissuePath::RightKidneyE1ToRightKidneyE2);
  RightKidneyE1ToRightKidneyE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& RightKidneyE2ToRightKidneyE3 = cCombinedCardiovascular.CreatePath(RightKidneyE2, RightKidneyE3, BGE::TissuePath::RightKidneyE2ToRightKidneyE3);
  RightKidneyE2ToRightKidneyE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightKidneyE3ToGround = cCombinedCardiovascular.CreatePath(RightKidneyE3, *Ground, BGE::TissuePath::RightKidneyE3ToGround);
  RightKidneyE3ToGround.GetComplianceBaseline().SetValue(RightKidneyE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& RightKidneyE3ToRightKidneyI = cCombinedCardiovascular.CreatePath(RightKidneyE3, RightKidneyI, BGE::TissuePath::RightKidneyE3ToRightKidneyI);
  RightKidneyE3ToRightKidneyI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& RightKidneyIToGround = cCombinedCardiovascular.CreatePath(RightKidneyI, *Ground, BGE::TissuePath::RightKidneyIToGround);
  RightKidneyIToGround.GetComplianceBaseline().SetValue(RightKidneyI.GetVolumeBaseline(VolumeUnit::mL) / RightKidneyI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& RightKidneyE3ToRightKidneyL1 = cCombinedCardiovascular.CreatePath(RightKidneyE3, RightKidneyL1, BGE::TissuePath::RightKidneyE3ToRightKidneyL1);
  RightKidneyE3ToRightKidneyL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightKidneyL1ToRightKidneyL2 = cCombinedCardiovascular.CreatePath(RightKidneyL1, RightKidneyL2, BGE::TissuePath::RightKidneyL1ToRightKidneyL2);
  RightKidneyL1ToRightKidneyL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& RightKidneyL2ToLymph = cCombinedCardiovascular.CreatePath(RightKidneyL2, Lymph, BGE::TissuePath::RightKidneyToLymphValve);
  RightKidneyL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& RightKidneyTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::RightKidney);
  SELiquidCompartment& RightKidneyExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::RightKidneyExtracellular);
  SELiquidCompartment& RightKidneyIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::RightKidneyIntracellular);
  RightKidneyTissue.GetMatrixVolume().SetValue((1 - RKidneyEWFraction - RKidneyIWFraction) * RKidneyTissueVolume * 1000.0, VolumeUnit::mL);
  RightKidneyExtracellular.MapNode(RightKidneyE1);
  RightKidneyExtracellular.MapNode(RightKidneyE2);
  RightKidneyExtracellular.MapNode(RightKidneyE3);
  RightKidneyIntracellular.MapNode(RightKidneyI);
  RightKidneyExtracellular.MapNode(RightKidneyL1);
  RightKidneyExtracellular.MapNode(RightKidneyL2);
  RightKidneyExtracellular.GetWaterVolumeFraction().SetValue(RKidneyEWFraction);
  RightKidneyIntracellular.GetWaterVolumeFraction().SetValue(RKidneyIWFraction);
  RightKidneyTissue.GetAcidicPhospohlipidConcentration().SetValue(RKidneyAPL, MassPerMassUnit::mg_Per_g);
  RightKidneyTissue.GetNeutralLipidsVolumeFraction().SetValue(RKidneyNLFraction);
  RightKidneyTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(RKidneyNPFraction);
  RightKidneyTissue.GetTissueToPlasmaAlbuminRatio().SetValue(RKidneyARatio);
  RightKidneyTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(RKidneyAAGRatio);
  RightKidneyTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(RKidneyLRatio);
  RightKidneyTissue.GetTotalMass().SetValue(RKidneyTissueMass, MassUnit::kg);
  RightKidneyTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  RightKidneyTissue.GetReflectionCoefficient().SetValue(1.0);

  ////////////////
  // Right Lung //
  SEFluidCircuitNode* RightLungV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::RightPulmonaryCapillaries);
  SEFluidCircuitNode& RightLungE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungE1);
  SEFluidCircuitNode& RightLungE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungE2);
  SEFluidCircuitNode& RightLungE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungE3);
  SEFluidCircuitNode& RightLungI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungI);
  SEFluidCircuitNode& RightLungL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& RightLungL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::RightLungL2); //Pre-lymphatic node 2--valve check

  //vNodePressure = RightLungV->GetPressure(PressureUnit::mmHg);
  //Using empirical value from previous iteration of tissue circuit because the extracellular lung volume was increasing too much
  //When we revisit oncotic pressure calculations, this might be attributate to different concentrations of albumin in lungs than rest of body
  vNodePressure = 9.339;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (RLungTissueVolume * RLungEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  RightLungE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  RightLungE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  RightLungE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  RightLungE3.GetVolumeBaseline().SetValue(RLungEWFraction * RLungTissueVolume * 1000.0, VolumeUnit::mL);
  RightLungI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  RightLungI.GetVolumeBaseline().SetValue(RLungIWFraction * RLungTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  RightLungL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  RightLungL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& RightLungVToRightLungE1 = cCombinedCardiovascular.CreatePath(*RightLungV, RightLungE1, BGE::TissuePath::RightLungVToRightLungE1);
  RightLungVToRightLungE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightLungE1ToRightLungE2 = cCombinedCardiovascular.CreatePath(RightLungE1, RightLungE2, BGE::TissuePath::RightLungE1ToRightLungE2);
  RightLungE1ToRightLungE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& RightLungE2ToRightLungE3 = cCombinedCardiovascular.CreatePath(RightLungE2, RightLungE3, BGE::TissuePath::RightLungE2ToRightLungE3);
  RightLungE2ToRightLungE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightLungE3ToGround = cCombinedCardiovascular.CreatePath(RightLungE3, *Ground, BGE::TissuePath::RightLungE3ToGround);
  //Lung interstitial compliance based of value in Miserocchi1993Pulmonary
  RightLungE3ToGround.GetComplianceBaseline().SetValue(0.00544 * RightLungE3.GetVolumeBaseline(VolumeUnit::mL), FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  // RightLungE3ToGround.GetComplianceBaseline().SetValue(RightLungE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg);
  SEFluidCircuitPath& RightLungE3ToRightLungI = cCombinedCardiovascular.CreatePath(RightLungE3, RightLungI, BGE::TissuePath::RightLungE3ToRightLungI);
  RightLungE3ToRightLungI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& RightLungIToGround = cCombinedCardiovascular.CreatePath(RightLungI, *Ground, BGE::TissuePath::RightLungIToGround);
  RightLungIToGround.GetComplianceBaseline().SetValue(-RightLungI.GetVolumeBaseline(VolumeUnit::mL) / RightLungI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& RightLungE3ToRightLungL1 = cCombinedCardiovascular.CreatePath(RightLungE3, RightLungL1, BGE::TissuePath::RightLungE3ToRightLungL1);
  RightLungE3ToRightLungL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& RightLungL1ToRightLungL2 = cCombinedCardiovascular.CreatePath(RightLungL1, RightLungL2, BGE::TissuePath::RightLungL1ToRightLungL2);
  RightLungL1ToRightLungL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& RightLungL2ToLymph = cCombinedCardiovascular.CreatePath(RightLungL2, Lymph, BGE::TissuePath::RightLungToLymphValve);
  RightLungL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& RightLungTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::RightLung);
  SELiquidCompartment& RightLungExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::RightLungExtracellular);
  SELiquidCompartment& RightLungIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::RightLungIntracellular);
  RightLungTissue.GetMatrixVolume().SetValue((1 - RLungEWFraction - RLungIWFraction) * RLungTissueVolume * 1000.0, VolumeUnit::mL);
  RightLungExtracellular.MapNode(RightLungE1);
  RightLungExtracellular.MapNode(RightLungE2);
  RightLungExtracellular.MapNode(RightLungE3);
  RightLungIntracellular.MapNode(RightLungI);
  RightLungExtracellular.MapNode(RightLungL1);
  RightLungExtracellular.MapNode(RightLungL2);
  RightLungExtracellular.GetWaterVolumeFraction().SetValue(RLungEWFraction);
  RightLungIntracellular.GetWaterVolumeFraction().SetValue(RLungIWFraction);
  RightLungTissue.GetAcidicPhospohlipidConcentration().SetValue(RLungAPL, MassPerMassUnit::mg_Per_g);
  RightLungTissue.GetNeutralLipidsVolumeFraction().SetValue(RLungNLFraction);
  RightLungTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(RLungNPFraction);
  RightLungTissue.GetTissueToPlasmaAlbuminRatio().SetValue(RLungARatio);
  RightLungTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(RLungAAGRatio);
  RightLungTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(RLungLRatio);
  RightLungTissue.GetTotalMass().SetValue(RLungTissueMass, MassUnit::kg);
  RightLungTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  RightLungTissue.GetReflectionCoefficient().SetValue(1.0);

  //////////
  // Skin //
  SEFluidCircuitNode* SkinV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Skin1);
  SEFluidCircuitNode& SkinE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinE1);
  SEFluidCircuitNode& SkinE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinE2);
  SEFluidCircuitNode& SkinE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinE3);
  SEFluidCircuitNode& SkinI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinI);
  SEFluidCircuitNode& SkinL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& SkinL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SkinL2); //Pre-lymphatic node 2--valve check

  vNodePressure = SkinV->GetPressure(PressureUnit::mmHg) / VolumeModifierSkin;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (SkinTissueVolume * SkinEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  SkinE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  SkinE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  SkinE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  SkinE3.GetVolumeBaseline().SetValue(SkinEWFraction * SkinTissueVolume * 1000.0, VolumeUnit::mL);
  SkinI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  SkinI.GetVolumeBaseline().SetValue(SkinIWFraction * SkinTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  SkinL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  SkinL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& SkinVToSkinE1 = cCombinedCardiovascular.CreatePath(*SkinV, SkinE1, BGE::TissuePath::SkinVToSkinE1);
  SkinVToSkinE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SkinE1ToSkinE2 = cCombinedCardiovascular.CreatePath(SkinE1, SkinE2, BGE::TissuePath::SkinE1ToSkinE2);
  SkinE1ToSkinE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& SkinE2ToSkinE3 = cCombinedCardiovascular.CreatePath(SkinE2, SkinE3, BGE::TissuePath::SkinE2ToSkinE3);
  SkinE2ToSkinE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SkinE3ToGround = cCombinedCardiovascular.CreatePath(SkinE3, *Ground, BGE::TissuePath::SkinE3ToGround);
  //Minus sign in compliance baselines are because the interstitial pressure is negative with respect to atmosphere and we don't want negative compliance
  SkinE3ToGround.GetComplianceBaseline().SetValue(SkinE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& SkinE3ToSkinI = cCombinedCardiovascular.CreatePath(SkinE3, SkinI, BGE::TissuePath::SkinE3ToSkinI);
  SkinE3ToSkinI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& SkinIToGround = cCombinedCardiovascular.CreatePath(SkinI, *Ground, BGE::TissuePath::SkinIToGround);
  SkinIToGround.GetComplianceBaseline().SetValue(-SkinI.GetVolumeBaseline(VolumeUnit::mL) / SkinI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& SkinE3ToSkinL1 = cCombinedCardiovascular.CreatePath(SkinE3, SkinL1, BGE::TissuePath::SkinE3ToSkinL1);
  SkinE3ToSkinL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SkinL1ToSkinL2 = cCombinedCardiovascular.CreatePath(SkinL1, SkinL2, BGE::TissuePath::SkinL1ToSkinL2);
  SkinL1ToSkinL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& SkinL2ToLymph = cCombinedCardiovascular.CreatePath(SkinL2, Lymph, BGE::TissuePath::SkinToLymphValve);
  SkinL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SEFluidCircuitPath& SkinSweatLossToGround = cCombinedCardiovascular.CreatePath(SkinE3, *Ground, BGE::TissuePath::SkinSweating);
  SkinSweatLossToGround.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);

  SETissueCompartment& SkinTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Skin);
  SELiquidCompartment& SkinExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::SkinExtracellular);
  SELiquidCompartment& SkinIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::SkinIntracellular);
  SkinTissue.GetMatrixVolume().SetValue((1 - SkinEWFraction - SkinIWFraction) * SkinTissueVolume * 1000.0, VolumeUnit::mL);
  SkinExtracellular.MapNode(SkinE1);
  SkinExtracellular.MapNode(SkinE2);
  SkinExtracellular.MapNode(SkinE3);
  SkinIntracellular.MapNode(SkinI);
  SkinExtracellular.MapNode(SkinL1);
  SkinExtracellular.MapNode(SkinL2);
  SkinExtracellular.GetWaterVolumeFraction().SetValue(SkinEWFraction);
  SkinIntracellular.GetWaterVolumeFraction().SetValue(SkinIWFraction);
  SkinTissue.GetAcidicPhospohlipidConcentration().SetValue(SkinAPL, MassPerMassUnit::mg_Per_g);
  SkinTissue.GetNeutralLipidsVolumeFraction().SetValue(SkinNLFraction);
  SkinTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(SkinNPFraction);
  SkinTissue.GetTissueToPlasmaAlbuminRatio().SetValue(SkinARatio);
  SkinTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(SkinAAGRatio);
  SkinTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(SkinLRatio);
  SkinTissue.GetTotalMass().SetValue(SkinTissueMass, MassUnit::kg);
  SkinTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  SkinTissue.GetReflectionCoefficient().SetValue(1.0);

  ////////////
  // Spleen //
  SEFluidCircuitNode* SpleenV = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Spleen1);
  SEFluidCircuitNode& SpleenE1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenE1);
  SEFluidCircuitNode& SpleenE2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenE2);
  SEFluidCircuitNode& SpleenE3 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenE3);
  SEFluidCircuitNode& SpleenI = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenI);
  SEFluidCircuitNode& SpleenL1 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenL1); //Pre-lymphatic node 1
  SEFluidCircuitNode& SpleenL2 = cCombinedCardiovascular.CreateNode(BGE::TissueNode::SpleenL2); //Pre-lymphatic node 2--valve check

  vNodePressure = SpleenV->GetPressure(PressureUnit::mmHg) / VolumeModifierSpleen;
  e1NodePressure = vNodePressure - copVascular_mmHg; //Plasma colloid osmotic pressure opposes flow into tissue space (i.e. favor E1 to V)
  e3NodePressure = vNodePressure - targetHydrostaticGradient_mmHg;
  e2NodePressure = e3NodePressure - copExtracell_mmHg; //Extracellular colloid osmotic pressure promotes flow from E2 to E3
  if (e3NodePressure > preLymphaticPressureMin_mmHg) {
    l1NodePressure = e3NodePressure;
  } else {
    l1NodePressure = preLymphaticPressureMin_mmHg;
  }
  l2NodePressure = Lymph.GetPressure(PressureUnit::mmHg);

  filteredFlow_mL_Per_min = (SpleenTissueVolume * SpleenEWFraction) / totalECWater_L * lymphTotalBody_mL_Per_min;
  capillaryResistance_mmHg_min_Per_mL = (e1NodePressure - e2NodePressure) / filteredFlow_mL_Per_min;
  lymphDrivePressure_mmHg = l1NodePressure - e3NodePressure;
  lymphResistance_mmHg_min_Per_mL = (l1NodePressure - l2NodePressure) / filteredFlow_mL_Per_min;

  SpleenE1.GetPressure().SetValue(e1NodePressure, PressureUnit::mmHg);
  SpleenE2.GetPressure().SetValue(e2NodePressure, PressureUnit::mmHg);
  SpleenE3.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg);
  SpleenE3.GetVolumeBaseline().SetValue(SpleenEWFraction * SpleenTissueVolume * 1000.0, VolumeUnit::mL);
  SpleenI.GetPressure().SetValue(e3NodePressure, PressureUnit::mmHg); //No hydrostatic pressure difference between intra/extra
  SpleenI.GetVolumeBaseline().SetValue(SpleenIWFraction * SpleenTissueVolume * 1000.0, VolumeUnit::mL); //intracellular node
  SpleenL1.GetPressure().SetValue(l1NodePressure, PressureUnit::mmHg);
  SpleenL2.GetPressure().SetValue(l2NodePressure, PressureUnit::mmHg);

  SEFluidCircuitPath& SpleenVToSpleenE1 = cCombinedCardiovascular.CreatePath(*SpleenV, SpleenE1, BGE::TissuePath::SpleenVToSpleenE1);
  SpleenVToSpleenE1.GetPressureSourceBaseline().SetValue(-copVascular_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SpleenE1ToSpleenE2 = cCombinedCardiovascular.CreatePath(SpleenE1, SpleenE2, BGE::TissuePath::SpleenE1ToSpleenE2);
  SpleenE1ToSpleenE2.GetResistanceBaseline().SetValue(capillaryResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& SpleenE2ToSpleenE3 = cCombinedCardiovascular.CreatePath(SpleenE2, SpleenE3, BGE::TissuePath::SpleenE2ToSpleenE3);
  SpleenE2ToSpleenE3.GetPressureSourceBaseline().SetValue(copExtracell_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SpleenE3ToGround = cCombinedCardiovascular.CreatePath(SpleenE3, *Ground, BGE::TissuePath::SpleenE3ToGround);
  SpleenE3ToGround.GetComplianceBaseline().SetValue(SpleenE3.GetVolumeBaseline(VolumeUnit::mL) / vNodePressure, FlowComplianceUnit::mL_Per_mmHg); //Might need to change this
  SEFluidCircuitPath& SpleenE3ToSpleenI = cCombinedCardiovascular.CreatePath(SpleenE3, SpleenI, BGE::TissuePath::SpleenE3ToSpleenI);
  SpleenE3ToSpleenI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  SEFluidCircuitPath& SpleenIToGround = cCombinedCardiovascular.CreatePath(SpleenI, *Ground, BGE::TissuePath::SpleenIToGround);
  SpleenIToGround.GetComplianceBaseline().SetValue(SpleenI.GetVolumeBaseline(VolumeUnit::mL) / SpleenI.GetPressure(PressureUnit::mmHg), FlowComplianceUnit::mL_Per_mmHg);

  SEFluidCircuitPath& SpleenE3ToSpleenL1 = cCombinedCardiovascular.CreatePath(SpleenE3, SpleenL1, BGE::TissuePath::SpleenE3ToSpleenL1);
  SpleenE3ToSpleenL1.GetPressureSourceBaseline().SetValue(lymphDrivePressure_mmHg, PressureUnit::mmHg);
  SEFluidCircuitPath& SpleenL1ToSpleenL2 = cCombinedCardiovascular.CreatePath(SpleenL1, SpleenL2, BGE::TissuePath::SpleenL1ToSpleenL2);
  SpleenL1ToSpleenL2.GetResistanceBaseline().SetValue(lymphResistance_mmHg_min_Per_mL, FlowResistanceUnit::mmHg_min_Per_mL);
  SEFluidCircuitPath& SpleenL2ToLymph = cCombinedCardiovascular.CreatePath(SpleenL2, Lymph, BGE::TissuePath::SpleenToLymphValve);
  SpleenL2ToLymph.SetNextValve(CDM::enumOpenClosed::Closed);

  SETissueCompartment& SpleenTissue = m_Compartments->CreateTissueCompartment(BGE::TissueCompartment::Spleen);
  SELiquidCompartment& SpleenExtracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::SpleenExtracellular);
  SELiquidCompartment& SpleenIntracellular = m_Compartments->CreateLiquidCompartment(BGE::ExtravascularCompartment::SpleenIntracellular);
  SpleenTissue.GetMatrixVolume().SetValue((1 - SpleenEWFraction - SpleenIWFraction) * SpleenTissueVolume * 1000.0, VolumeUnit::mL);
  SpleenExtracellular.MapNode(SpleenE1);
  SpleenExtracellular.MapNode(SpleenE2);
  SpleenExtracellular.MapNode(SpleenE3);
  SpleenIntracellular.MapNode(SpleenI);
  SpleenExtracellular.MapNode(SpleenL1);
  SpleenExtracellular.MapNode(SpleenL2);
  SpleenExtracellular.GetWaterVolumeFraction().SetValue(SpleenEWFraction);
  SpleenIntracellular.GetWaterVolumeFraction().SetValue(SpleenIWFraction);
  SpleenTissue.GetAcidicPhospohlipidConcentration().SetValue(SpleenAPL, MassPerMassUnit::mg_Per_g);
  SpleenTissue.GetNeutralLipidsVolumeFraction().SetValue(SpleenNLFraction);
  SpleenTissue.GetNeutralPhospholipidsVolumeFraction().SetValue(SpleenNPFraction);
  SpleenTissue.GetTissueToPlasmaAlbuminRatio().SetValue(SpleenARatio);
  SpleenTissue.GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(SpleenAAGRatio);
  SpleenTissue.GetTissueToPlasmaLipoproteinRatio().SetValue(SpleenLRatio);
  SpleenTissue.GetTotalMass().SetValue(SpleenTissueMass, MassUnit::kg);
  SpleenTissue.GetMembranePotential().SetValue(-84.8, ElectricPotentialUnit::mV);
  SpleenTissue.GetReflectionCoefficient().SetValue(1.0);

  ////Finalize Circuit Changes
  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  ////Add compartments to graph, but not links because all substance transport involving extracellular is performed manually in Tissue::CalculateDiffusion
  gCombinedCardiovascular.AddCompartment(FatExtracellular);
  gCombinedCardiovascular.AddCompartment(FatIntracellular);
  gCombinedCardiovascular.AddCompartment(BoneExtracellular);
  gCombinedCardiovascular.AddCompartment(BoneIntracellular);
  gCombinedCardiovascular.AddCompartment(BrainExtracellular);
  gCombinedCardiovascular.AddCompartment(BrainIntracellular);
  gCombinedCardiovascular.AddCompartment(GutExtracellular);
  gCombinedCardiovascular.AddCompartment(GutIntracellular);
  gCombinedCardiovascular.AddCompartment(LeftKidneyExtracellular);
  gCombinedCardiovascular.AddCompartment(LeftLungExtracellular);
  gCombinedCardiovascular.AddCompartment(LiverExtracellular);
  gCombinedCardiovascular.AddCompartment(LiverIntracellular);
  gCombinedCardiovascular.AddCompartment(MuscleExtracellular);
  gCombinedCardiovascular.AddCompartment(MuscleIntracellular);
  gCombinedCardiovascular.AddCompartment(MyocardiumExtracellular);
  gCombinedCardiovascular.AddCompartment(MyocardiumIntracellular);
  gCombinedCardiovascular.AddCompartment(RightKidneyExtracellular);
  gCombinedCardiovascular.AddCompartment(RightKidneyIntracellular);
  gCombinedCardiovascular.AddCompartment(RightLungExtracellular);
  gCombinedCardiovascular.AddCompartment(RightLungIntracellular);
  gCombinedCardiovascular.AddCompartment(SkinExtracellular);
  gCombinedCardiovascular.AddCompartment(SkinIntracellular);
  gCombinedCardiovascular.AddCompartment(SpleenExtracellular);
  gCombinedCardiovascular.AddCompartment(SpleenIntracellular);

  gCombinedCardiovascular.StateChange();
}

void BioGears::SetupRespiratory()
{
  Info("Setting Up Respiratory");
  double RightLungRatio = m_Patient->GetRightLungRatio().GetValue();
  double LeftLungRatio = 1 - RightLungRatio;

  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEFluidCircuitNode* Ambient = m_Circuits->GetFluidNode(BGE::EnvironmentNode::Ambient);
  cRespiratory.AddReferenceNode(*Ambient);
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  SEGasCompartment* gEnvironment = m_Compartments->GetGasCompartment(BGE::EnvironmentCompartment::Ambient);

  //Standard lung parameters
  double AmbientPressure_cmH2O = 1033.23; //1 atm
  double FunctionalResidualCapacity_L = 2.313;
  double LungResidualVolume_L = m_Patient->GetResidualVolume(VolumeUnit::L);
  double OpenResistance_cmH2O_s_Per_L = m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::cmH2O_s_Per_L);

  //Compliances
  double tracheaCompliance_L_Per_cmH2O = 0.00238;
  double bronchiCompliance_L_Per_cmH2O = 0.0305;
  double alveoliCompliance_L_Per_cmH2O = 0.175;
  double pleuralCompliance_L_Per_cmH2O = 0.21;
  //Resistance
  double totalPulmonaryResistance_cmH2O_s_Per_L = 1.75;
  double tracheaResistancePercent = 0.6;
  double bronchiResistancePercent = 0.3;
  double alveoliResistancePercent = 0.1;
  double mouthToTracheaResistance_cmH2O_s_Per_L = tracheaResistancePercent * totalPulmonaryResistance_cmH2O_s_Per_L;
  double tracheaToBronchiResistance_cmH2O_s_Per_L = bronchiResistancePercent * totalPulmonaryResistance_cmH2O_s_Per_L;
  double bronchiToAlveoliResistance_cmH2O_s_Per_L = alveoliResistancePercent * totalPulmonaryResistance_cmH2O_s_Per_L;

  //Target volumes are end-expiratory (i.e. bottom of breathing cycle, pressure = ambient pressure)
  double larynxVolume_mL = 34.4;
  double tracheaVolume_mL = 6.63;
  double totalBronchiVolume_mL = 15.0;
  double totalAlveoliVolume_L = LungResidualVolume_L;
  //-----Set up nodes---------------------
  // Mouth
  SEFluidCircuitNode& Mouth = cRespiratory.CreateNode(BGE::RespiratoryNode::Mouth);
  Mouth.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Mouth.GetVolumeBaseline().SetValue(20.6, VolumeUnit::mL);
  // Carina
  SEFluidCircuitNode& Trachea = cRespiratory.CreateNode(BGE::RespiratoryNode::Trachea);
  Trachea.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Trachea.GetVolumeBaseline().SetValue(tracheaVolume_mL + larynxVolume_mL, VolumeUnit::mL);
  // Right Dead Space
  SEFluidCircuitNode& RightBronchi = cRespiratory.CreateNode(BGE::RespiratoryNode::RightBronchi);
  RightBronchi.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  RightBronchi.GetVolumeBaseline().SetValue(RightLungRatio * totalBronchiVolume_mL, VolumeUnit::mL);
  // Left Dead Space
  SEFluidCircuitNode& LeftBronchi = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftBronchi);
  LeftBronchi.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  LeftBronchi.GetVolumeBaseline().SetValue(LeftLungRatio * totalBronchiVolume_mL, VolumeUnit::mL);
  // Right Alveoli
  SEFluidCircuitNode& RightAlveoli = cRespiratory.CreateNode(BGE::RespiratoryNode::RightAlveoli);
  RightAlveoli.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  RightAlveoli.GetVolumeBaseline().SetValue(RightLungRatio * totalAlveoliVolume_L, VolumeUnit::L);
  // Left Alveoli
  SEFluidCircuitNode& LeftAlveoli = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftAlveoli);
  LeftAlveoli.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  LeftAlveoli.GetVolumeBaseline().SetValue(LeftLungRatio * totalAlveoliVolume_L, VolumeUnit::L);
  // Node for right alveoli leak
  SEFluidCircuitNode& RightAlveoliLeak = cRespiratory.CreateNode(BGE::RespiratoryNode::RightAlveoliLeak);
  RightAlveoliLeak.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Node for left alveoli leak
  SEFluidCircuitNode& LeftAlveoliLeak = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftAlveoliLeak);
  LeftAlveoliLeak.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Right Pleural Connection - no volume, so it doesn't get modified by compliances
  SEFluidCircuitNode& RightPleuralConnection = cRespiratory.CreateNode(BGE::RespiratoryNode::RightPleuralConnection);
  RightPleuralConnection.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Left Pleural Connection - no volume, so it doesn't get modified by compliances
  SEFluidCircuitNode& LeftPleuralConnection = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftPleuralConnection);
  LeftPleuralConnection.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Right Pleural
  SEFluidCircuitNode& RightPleuralCavity = cRespiratory.CreateNode(BGE::RespiratoryNode::RightPleuralCavity);
  RightPleuralCavity.GetPressure().SetValue(AmbientPressure_cmH2O - 5.0, PressureUnit::cmH2O);
  RightPleuralCavity.GetVolumeBaseline().SetValue(0.0085, VolumeUnit::L);
  // Left Pleural
  SEFluidCircuitNode& LeftPleuralCavity = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftPleuralCavity);
  LeftPleuralCavity.GetPressure().SetValue(AmbientPressure_cmH2O - 5.0, PressureUnit::cmH2O);
  LeftPleuralCavity.GetVolumeBaseline().SetValue(0.0085, VolumeUnit::L);
  // Node for left chest leak
  SEFluidCircuitNode& LeftChestLeak = cRespiratory.CreateNode(BGE::RespiratoryNode::LeftChestLeak);
  LeftChestLeak.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Node for right Alveoli leak
  SEFluidCircuitNode& RightChestLeak = cRespiratory.CreateNode(BGE::RespiratoryNode::RightChestLeak);
  RightChestLeak.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  // Stomach
  SEFluidCircuitNode& Stomach = cRespiratory.CreateNode(BGE::RespiratoryNode::Stomach);
  Stomach.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Stomach.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  // Respiratory Muscle - corresponds to a node representing the inspiratory muscles, particularly diaphragm
  SEFluidCircuitNode& RespiratoryMuscle = cRespiratory.CreateNode(BGE::RespiratoryNode::RespiratoryMuscle);
  RespiratoryMuscle.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);

  //---------Paths-------------------------------------------------
  SEFluidCircuitPath& EnvironmentToMouth = cRespiratory.CreatePath(*Ambient, Mouth, BGE::RespiratoryPath::EnvironmentToMouth);
  SEFluidCircuitPath& MouthToTrachea = cRespiratory.CreatePath(Mouth, Trachea, BGE::RespiratoryPath::MouthToTrachea);
  MouthToTrachea.GetResistanceBaseline().SetValue(mouthToTracheaResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& TracheaToRightBronchi = cRespiratory.CreatePath(Trachea, RightBronchi, BGE::RespiratoryPath::TracheaToRightBronchi);
  TracheaToRightBronchi.GetResistanceBaseline().SetValue(tracheaToBronchiResistance_cmH2O_s_Per_L / 2.0, FlowResistanceUnit::cmH2O_s_Per_L); //Circuit math assumes equal resistances left/right
  SEFluidCircuitPath& TracheaToLeftBronchi = cRespiratory.CreatePath(Trachea, LeftBronchi, BGE::RespiratoryPath::TracheaToLeftBronchi);
  TracheaToLeftBronchi.GetResistanceBaseline().SetValue(tracheaToBronchiResistance_cmH2O_s_Per_L / 2.0, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightBronchiToRightPleuralConnection = cRespiratory.CreatePath(RightBronchi, RightPleuralConnection, BGE::RespiratoryPath::RightBronchiToRightPleuralConnection);
  RightBronchiToRightPleuralConnection.GetComplianceBaseline().SetValue(bronchiCompliance_L_Per_cmH2O * RightLungRatio, FlowComplianceUnit::L_Per_cmH2O);
  RightBronchiToRightPleuralConnection.SetNextPolarizedState(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& LeftBronchiToLeftPleuralConnection = cRespiratory.CreatePath(LeftBronchi, LeftPleuralConnection, BGE::RespiratoryPath::LeftBronchiToLeftPleuralConnection);
  LeftBronchiToLeftPleuralConnection.GetComplianceBaseline().SetValue(bronchiCompliance_L_Per_cmH2O * LeftLungRatio, FlowComplianceUnit::L_Per_cmH2O);
  LeftBronchiToLeftPleuralConnection.SetNextPolarizedState(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& RightBronchiToRightAlveoli = cRespiratory.CreatePath(RightBronchi, RightAlveoli, BGE::RespiratoryPath::RightBronchiToRightAlveoli);
  RightBronchiToRightAlveoli.GetResistanceBaseline().SetValue(bronchiToAlveoliResistance_cmH2O_s_Per_L / 2.0, FlowResistanceUnit::cmH2O_s_Per_L); //Circuit math assumes equal resistances left/right
  SEFluidCircuitPath& LeftBronchiToLeftAlveoli = cRespiratory.CreatePath(LeftBronchi, LeftAlveoli, BGE::RespiratoryPath::LeftBronchiToLeftAlveoli);
  LeftBronchiToLeftAlveoli.GetResistanceBaseline().SetValue(bronchiToAlveoliResistance_cmH2O_s_Per_L / 2.0, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightAlveoliToRightPleuralConnection = cRespiratory.CreatePath(RightAlveoli, RightPleuralConnection, BGE::RespiratoryPath::RightAlveoliToRightPleuralConnection);
  RightAlveoliToRightPleuralConnection.GetComplianceBaseline().SetValue(alveoliCompliance_L_Per_cmH2O * RightLungRatio, FlowComplianceUnit::L_Per_cmH2O);
  RightAlveoliToRightPleuralConnection.SetNextPolarizedState(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& LeftAlveoliToLeftPleuralConnection = cRespiratory.CreatePath(LeftAlveoli, LeftPleuralConnection, BGE::RespiratoryPath::LeftAlveoliToLeftPleuralConnection);
  LeftAlveoliToLeftPleuralConnection.GetComplianceBaseline().SetValue(alveoliCompliance_L_Per_cmH2O * LeftLungRatio, FlowComplianceUnit::L_Per_cmH2O);
  LeftAlveoliToLeftPleuralConnection.SetNextPolarizedState(CDM::enumOpenClosed::Closed);
  //Need a no element path to be able to include a node with no volume, so it doesn't get modified by compliances
  SEFluidCircuitPath& RightPleuralConnectionToRightPleuralCavity = cRespiratory.CreatePath(RightPleuralConnection, RightPleuralCavity, BGE::RespiratoryPath::RightPleuralConnectionToRightPleuralCavity);
  SEFluidCircuitPath& LeftPleuralConnectionToLeftPleuralCavity = cRespiratory.CreatePath(LeftPleuralConnection, LeftPleuralCavity, BGE::RespiratoryPath::LeftPleuralConnectionToLeftPleuralCavity);
  //----------------------------------------------------------------------------------------------------------------------------------------------
  // Path between alveoli and pleural - for right pleural leak
  SEFluidCircuitPath& RightAlveoliToRightAlveoliLeak = cRespiratory.CreatePath(RightAlveoli, RightAlveoliLeak, BGE::RespiratoryPath::RightAlveoliToRightAlveoliLeak);
  RightAlveoliToRightAlveoliLeak.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& RightAlveoliLeakToRightPleuralCavity = cRespiratory.CreatePath(RightAlveoliLeak, RightPleuralCavity, BGE::RespiratoryPath::RightAlveoliLeakToRightPleuralCavity);
  RightAlveoliLeakToRightPleuralCavity.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  // path between alveoli and pleural - for left pleural leak
  SEFluidCircuitPath& LeftAlveoliToLeftAlveoliLeak = cRespiratory.CreatePath(LeftAlveoli, LeftAlveoliLeak, BGE::RespiratoryPath::LeftAlveoliToLeftAlveoliLeak);
  LeftAlveoliToLeftAlveoliLeak.SetNextValve(CDM::enumOpenClosed::Closed);
  SEFluidCircuitPath& LeftAlveoliLeakToLeftPleuralCavity = cRespiratory.CreatePath(LeftAlveoliLeak, LeftPleuralCavity, BGE::RespiratoryPath::LeftAlveoliLeakToLeftPleuralCavity);
  LeftAlveoliLeakToLeftPleuralCavity.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  // Path for needle decompression - right side
  SEFluidCircuitPath& RightPleuralCavityToEnvironment = cRespiratory.CreatePath(RightPleuralCavity, *Ambient, BGE::RespiratoryPath::RightPleuralCavityToEnvironment);
  RightPleuralCavityToEnvironment.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  //Path for needle decompression - left side
  SEFluidCircuitPath& LeftPleuralCavityToEnvironment = cRespiratory.CreatePath(LeftPleuralCavity, *Ambient, BGE::RespiratoryPath::LeftPleuralCavityToEnvironment);
  LeftPleuralCavityToEnvironment.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  // Path for open (chest wound) pneumothorax circuit  - right side
  SEFluidCircuitPath& EnvironmentToRightChestLeak = cRespiratory.CreatePath(*Ambient, RightChestLeak, BGE::RespiratoryPath::EnvironmentToRightChestLeak);
  EnvironmentToRightChestLeak.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& RightChestLeakToRightPleuralCavity = cRespiratory.CreatePath(RightChestLeak, RightPleuralCavity, BGE::RespiratoryPath::RightChestLeakToRightPleuralCavity);
  RightChestLeakToRightPleuralCavity.SetNextValve(CDM::enumOpenClosed::Closed);
  // Path for open (chest wound) pneumothorax circuit - left side
  SEFluidCircuitPath& EnvironmentToLeftChestLeak = cRespiratory.CreatePath(*Ambient, LeftChestLeak, BGE::RespiratoryPath::EnvironmentToLeftChestLeak);
  EnvironmentToLeftChestLeak.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& LeftChestLeakToLeftPleuralCavity = cRespiratory.CreatePath(LeftChestLeak, LeftPleuralCavity, BGE::RespiratoryPath::LeftChestLeakToLeftPleuralCavity);
  LeftChestLeakToLeftPleuralCavity.SetNextValve(CDM::enumOpenClosed::Closed);
  // Paths for the Driver
  SEFluidCircuitPath& EnvironmentToRespiratoryMuscle = cRespiratory.CreatePath(*Ambient, RespiratoryMuscle, BGE::RespiratoryPath::EnvironmentToRespiratoryMuscle);
  EnvironmentToRespiratoryMuscle.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);
  // Esophageal (Stomach) path
  SEFluidCircuitPath& MouthToStomach = cRespiratory.CreatePath(Mouth, Stomach, BGE::RespiratoryPath::MouthToStomach);
  MouthToStomach.GetResistanceBaseline().SetValue(OpenResistance_cmH2O_s_Per_L, FlowResistanceUnit::cmH2O_s_Per_L);
  SEFluidCircuitPath& StomachToEnvironment = cRespiratory.CreatePath(Stomach, *Ambient, BGE::RespiratoryPath::StomachToEnvironment);
  StomachToEnvironment.GetComplianceBaseline().SetValue(0.05, FlowComplianceUnit::L_Per_cmH2O);
  // Paths to RespiratoryMuscle
  SEFluidCircuitPath& RightPleuralCavityToRespiratoryMuscle = cRespiratory.CreatePath(RightPleuralCavity, RespiratoryMuscle, BGE::RespiratoryPath::RightPleuralCavityToRespiratoryMuscle);
  RightPleuralCavityToRespiratoryMuscle.GetComplianceBaseline().SetValue(pleuralCompliance_L_Per_cmH2O * RightLungRatio, FlowComplianceUnit::L_Per_cmH2O);
  SEFluidCircuitPath& LeftPleuralCavityToRespiratoryMuscle = cRespiratory.CreatePath(LeftPleuralCavity, RespiratoryMuscle, BGE::RespiratoryPath::LeftPleuralCavityToRespiratoryMuscle);
  LeftPleuralCavityToRespiratoryMuscle.GetComplianceBaseline().SetValue(pleuralCompliance_L_Per_cmH2O * LeftLungRatio, FlowComplianceUnit::L_Per_cmH2O);

  cRespiratory.SetNextAndCurrentFromBaselines();
  cRespiratory.StateChange();

  // Setup Compartments //

  // Pulmonary Compartments
  SEGasCompartment& pMouth = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::Mouth);
  pMouth.MapNode(Mouth);
  SEGasCompartment& pStomach = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::Stomach);
  pStomach.MapNode(Stomach);
  SEGasCompartment& pTrachea = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::Trachea);
  pTrachea.MapNode(Trachea);
  SEGasCompartment& pLeftBronchi = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftBronchi);
  pLeftBronchi.MapNode(LeftBronchi);
  SEGasCompartment& pLeftAlveoli = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftAlveoli);
  pLeftAlveoli.MapNode(LeftAlveoli);
  SEGasCompartment& pLeftPleuralCavity = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftPleuralCavity);
  pLeftPleuralCavity.MapNode(LeftPleuralCavity);
  pLeftPleuralCavity.MapNode(LeftPleuralConnection);
  SEGasCompartment& pRightBronchi = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightBronchi);
  pRightBronchi.MapNode(RightBronchi);
  SEGasCompartment& pRightAlveoli = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightAlveoli);
  pRightAlveoli.MapNode(RightAlveoli);
  SEGasCompartment& pRightPleuralCavity = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightPleuralCavity);
  pRightPleuralCavity.MapNode(RightPleuralCavity);
  pRightPleuralCavity.MapNode(RightPleuralConnection);
  SEGasCompartment& pLeftAlveoliLeak = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftAlveoliLeak);
  pLeftAlveoliLeak.MapNode(LeftAlveoliLeak);
  SEGasCompartment& pLeftChestLeak = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftChestLeak);
  pLeftChestLeak.MapNode(LeftChestLeak);
  SEGasCompartment& pRightAlveoliLeak = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightAlveoliLeak);
  pRightAlveoliLeak.MapNode(RightAlveoliLeak);
  SEGasCompartment& pRightChestLeak = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightChestLeak);
  pRightChestLeak.MapNode(RightChestLeak);

  // Set up hierarchy
  SEGasCompartment& pLeftLung = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::LeftLung);
  pLeftLung.AddChild(pLeftBronchi);
  pLeftLung.AddChild(pLeftAlveoli);
  SEGasCompartment& pRightLung = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::RightLung);
  pRightLung.AddChild(pRightBronchi);
  pRightLung.AddChild(pRightAlveoli);
  SEGasCompartment& pLungs = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::Lungs);
  pLungs.AddChild(pLeftLung);
  pLungs.AddChild(pRightLung);
  SEGasCompartment& pPleuralCavity = m_Compartments->CreateGasCompartment(BGE::PulmonaryCompartment::PleuralCavity);
  pPleuralCavity.AddChild(pLeftPleuralCavity);
  pPleuralCavity.AddChild(pRightPleuralCavity);

  SEGasCompartmentLink& pEnvironmentToMouth = m_Compartments->CreateGasLink(*gEnvironment, pMouth, BGE::PulmonaryLink::EnvironmentToMouth);
  pEnvironmentToMouth.MapPath(EnvironmentToMouth);
  SEGasCompartmentLink& pMouthToTrachea = m_Compartments->CreateGasLink(pMouth, pTrachea, BGE::PulmonaryLink::MouthToTrachea);
  pMouthToTrachea.MapPath(MouthToTrachea);
  SEGasCompartmentLink& pMouthToStomach = m_Compartments->CreateGasLink(pMouth, pStomach, BGE::PulmonaryLink::MouthToStomach);
  pMouthToStomach.MapPath(MouthToStomach);
  SEGasCompartmentLink& pTracheaToLeftBronchi = m_Compartments->CreateGasLink(pTrachea, pLeftBronchi, BGE::PulmonaryLink::TracheaToLeftBronchi);
  pTracheaToLeftBronchi.MapPath(TracheaToLeftBronchi);
  SEGasCompartmentLink& pLeftBronchiToAlveoli = m_Compartments->CreateGasLink(pLeftBronchi, pLeftAlveoli, BGE::PulmonaryLink::LeftBronchiToAlveoli);
  pLeftBronchiToAlveoli.MapPath(LeftBronchiToLeftAlveoli);
  SEGasCompartmentLink& pTracheaToRightBronchi = m_Compartments->CreateGasLink(pTrachea, pRightBronchi, BGE::PulmonaryLink::TracheaToRightBronchi);
  pTracheaToRightBronchi.MapPath(TracheaToRightBronchi);
  SEGasCompartmentLink& pRightBronchiToAlveoli = m_Compartments->CreateGasLink(pRightBronchi, pRightAlveoli, BGE::PulmonaryLink::RightBronchiToAlveoli);
  pRightBronchiToAlveoli.MapPath(RightBronchiToRightAlveoli);
  SEGasCompartmentLink& pEnvironmentToLeftChestLeak = m_Compartments->CreateGasLink(*gEnvironment, pLeftChestLeak, BGE::PulmonaryLink::EnvironmentToLeftChestLeak);
  pEnvironmentToLeftChestLeak.MapPath(EnvironmentToLeftChestLeak);
  SEGasCompartmentLink& pEnvironmentToRightChestLeak = m_Compartments->CreateGasLink(*gEnvironment, pRightChestLeak, BGE::PulmonaryLink::EnvironmentToRightChestLeak);
  pEnvironmentToRightChestLeak.MapPath(EnvironmentToRightChestLeak);
  SEGasCompartmentLink& pLeftAlveoliLeakToLeftPleuralCavity = m_Compartments->CreateGasLink(pLeftAlveoliLeak, pLeftPleuralCavity, BGE::PulmonaryLink::LeftAlveoliLeakToLeftPleuralCavity);
  pLeftAlveoliLeakToLeftPleuralCavity.MapPath(LeftAlveoliLeakToLeftPleuralCavity);
  SEGasCompartmentLink& pLeftAlveoliToLeftAlveoliLeak = m_Compartments->CreateGasLink(pLeftAlveoli, pLeftAlveoliLeak, BGE::PulmonaryLink::LeftAlveoliToLeftAlveoliLeak);
  pLeftAlveoliToLeftAlveoliLeak.MapPath(LeftAlveoliToLeftAlveoliLeak);
  SEGasCompartmentLink& pLeftChestLeakToLeftPleuralCavity = m_Compartments->CreateGasLink(pLeftChestLeak, pLeftPleuralCavity, BGE::PulmonaryLink::LeftChestLeakToLeftPleuralCavity);
  pLeftChestLeakToLeftPleuralCavity.MapPath(LeftChestLeakToLeftPleuralCavity);
  SEGasCompartmentLink& pLeftPleuralCavityToEnvironment = m_Compartments->CreateGasLink(pLeftPleuralCavity, *gEnvironment, BGE::PulmonaryLink::LeftPleuralCavityToEnvironment);
  pLeftPleuralCavityToEnvironment.MapPath(LeftPleuralCavityToEnvironment);
  SEGasCompartmentLink& pRightAlveoliLeakToRightPleuralCavity = m_Compartments->CreateGasLink(pRightAlveoliLeak, pRightPleuralCavity, BGE::PulmonaryLink::RightAlveoliLeakToRightPleuralCavity);
  pRightAlveoliLeakToRightPleuralCavity.MapPath(RightAlveoliLeakToRightPleuralCavity);
  SEGasCompartmentLink& pRightAlveoliToRightAlveoliLeak = m_Compartments->CreateGasLink(pRightAlveoli, pRightAlveoliLeak, BGE::PulmonaryLink::RightAlveoliToRightAlveoliLeak);
  pRightAlveoliToRightAlveoliLeak.MapPath(RightAlveoliToRightAlveoliLeak);
  SEGasCompartmentLink& pRightChestLeakToRightPleuralCavity = m_Compartments->CreateGasLink(pRightChestLeak, pRightPleuralCavity, BGE::PulmonaryLink::RightChestLeakToRightPleuralCavity);
  pRightChestLeakToRightPleuralCavity.MapPath(RightChestLeakToRightPleuralCavity);
  SEGasCompartmentLink& pRightPleuralCavityToEnvironment = m_Compartments->CreateGasLink(pRightPleuralCavity, *gEnvironment, BGE::PulmonaryLink::RightPleuralCavityToEnvironment);
  pRightPleuralCavityToEnvironment.MapPath(RightPleuralCavityToEnvironment);

  // Create the respiratory graph for transport //
  gRespiratory.AddCompartment(*gEnvironment);
  gRespiratory.AddCompartment(pMouth);
  gRespiratory.AddCompartment(pStomach);
  gRespiratory.AddCompartment(pTrachea);
  gRespiratory.AddCompartment(pLeftBronchi);
  gRespiratory.AddCompartment(pLeftAlveoli);
  gRespiratory.AddCompartment(pRightBronchi);
  gRespiratory.AddCompartment(pRightPleuralCavity);
  gRespiratory.AddCompartment(pLeftPleuralCavity);
  gRespiratory.AddCompartment(pRightAlveoli);
  gRespiratory.AddCompartment(pLeftAlveoliLeak);
  gRespiratory.AddCompartment(pLeftChestLeak);
  gRespiratory.AddCompartment(pRightAlveoliLeak);
  gRespiratory.AddCompartment(pRightChestLeak);
  gRespiratory.AddLink(pEnvironmentToMouth);
  gRespiratory.AddLink(pMouthToTrachea);
  gRespiratory.AddLink(pMouthToStomach);
  gRespiratory.AddLink(pTracheaToLeftBronchi);
  gRespiratory.AddLink(pLeftBronchiToAlveoli);
  gRespiratory.AddLink(pTracheaToRightBronchi);
  gRespiratory.AddLink(pRightBronchiToAlveoli);
  gRespiratory.AddLink(pEnvironmentToLeftChestLeak);
  gRespiratory.AddLink(pEnvironmentToRightChestLeak);
  gRespiratory.AddLink(pLeftAlveoliLeakToLeftPleuralCavity);
  gRespiratory.AddLink(pLeftAlveoliToLeftAlveoliLeak);
  gRespiratory.AddLink(pLeftChestLeakToLeftPleuralCavity);
  gRespiratory.AddLink(pLeftPleuralCavityToEnvironment);
  gRespiratory.AddLink(pRightAlveoliLeakToRightPleuralCavity);
  gRespiratory.AddLink(pRightAlveoliToRightAlveoliLeak);
  gRespiratory.AddLink(pRightChestLeakToRightPleuralCavity);
  gRespiratory.AddLink(pRightPleuralCavityToEnvironment);
  gRespiratory.StateChange();

  // Generically set up the Aerosol Graph, this is a mirror of the Respiratory Gas Graph, only it's a liquid graph
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  SELiquidCompartment* lEnvironment = m_Compartments->GetLiquidCompartment(BGE::EnvironmentCompartment::Ambient);
  lAerosol.AddCompartment(*lEnvironment);
  // First Create the compartments and map the same nodes
  for (auto name : BGE::PulmonaryCompartment::GetValues()) {
    SEGasCompartment* gasCmpt = m_Compartments->GetGasCompartment(name);
    SELiquidCompartment& liquidCmpt = m_Compartments->CreateLiquidCompartment(name);
    if (gasCmpt->HasNodeMapping()) {
      for (auto node : gasCmpt->GetNodeMapping().GetNodes()) {
        liquidCmpt.MapNode(*node);
      }
    }
  }
  // Hook up any hierarchies
  for (auto name : BGE::PulmonaryCompartment::GetValues()) {
    SEGasCompartment* gasCmpt = m_Compartments->GetGasCompartment(name);
    SELiquidCompartment* liquidCmpt = m_Compartments->GetLiquidCompartment(name);
    if (gasCmpt->HasChildren()) {
      for (auto child : gasCmpt->GetChildren()) {
        liquidCmpt->AddChild(*m_Compartments->GetLiquidCompartment(child->GetName()));
      }
    }
  }
  // Add leaf compartments to the graph
  for (auto name : BGE::PulmonaryCompartment::GetValues()) {
    SELiquidCompartment* liquidCmpt = m_Compartments->GetLiquidCompartment(name);
    if (!liquidCmpt->HasChildren()) {
      lAerosol.AddCompartment(*liquidCmpt);
    }
  }
  // Create Links
  for (auto name : BGE::PulmonaryLink::GetValues()) {
    SEGasCompartmentLink* gasLink = m_Compartments->GetGasLink(name);
    SELiquidCompartment* src = m_Compartments->GetLiquidCompartment(gasLink->GetSourceCompartment().GetName());
    SELiquidCompartment* tgt = m_Compartments->GetLiquidCompartment(gasLink->GetTargetCompartment().GetName());
    SELiquidCompartmentLink& liquidLink = m_Compartments->CreateLiquidLink(*src, *tgt, name);
    if (gasLink->HasPath()) {
      liquidLink.MapPath(*gasLink->GetPath());
    }
    lAerosol.AddLink(liquidLink);
  }
  lAerosol.StateChange();
}

void BioGears::SetupGastrointestinal()
{
  Info("Setting Up Gastrointestinal");
  // Circuit
  SEFluidCircuit& cCombinedCardiovascular = m_Circuits->GetActiveCardiovascularCircuit();

  SEFluidCircuitNode& SmallIntestineC1 = cCombinedCardiovascular.CreateNode(BGE::ChymeNode::SmallIntestineC1);
  SmallIntestineC1.GetPressure().SetValue(0, PressureUnit::mmHg);
  SmallIntestineC1.GetVolumeBaseline().SetValue(100, VolumeUnit::mL);

  SEFluidCircuitNode* SmallIntestine1 = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::SmallIntestine1);
  SEFluidCircuitNode* Ground = cCombinedCardiovascular.GetNode(BGE::CardiovascularNode::Ground);

  SEFluidCircuitPath& SmallIntestineC1ToSmallIntestine1 = cCombinedCardiovascular.CreatePath(SmallIntestineC1, *SmallIntestine1, BGE::ChymePath::SmallIntestineC1ToSmallIntestine1);
  SmallIntestineC1ToSmallIntestine1.GetFlowSourceBaseline().SetValue(0, VolumePerTimeUnit::mL_Per_min);
  SEFluidCircuitPath& GroundToSmallIntestineC1 = cCombinedCardiovascular.CreatePath(*Ground, SmallIntestineC1, BGE::ChymePath::GroundToSmallIntestineC1);

  if (m_Config->IsTissueEnabled()) {
    SEFluidCircuitNode* GutE3 = cCombinedCardiovascular.GetNode(BGE::TissueNode::GutE3);
    SEFluidCircuitPath& GutE3ToGroundGI = cCombinedCardiovascular.CreatePath(*GutE3, *Ground, BGE::ChymePath::GutE3ToGroundGI);
    GutE3ToGroundGI.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::mL_Per_s);
  }

  cCombinedCardiovascular.SetNextAndCurrentFromBaselines();
  cCombinedCardiovascular.StateChange();

  // Compartment
  SELiquidCompartment& cSmallIntestine = m_Compartments->CreateLiquidCompartment(BGE::ChymeCompartment::SmallIntestine);
  cSmallIntestine.MapNode(SmallIntestineC1);

  //remove the link because substances are handled manually in model design
  SELiquidCompartment* vSmallIntestine = m_Compartments->GetLiquidCompartment(BGE::VascularCompartment::SmallIntestine);
  //SELiquidCompartmentLink& lSmallIntestineChymeToVasculature = m_Compartments->CreateLiquidLink(cSmallIntestine, *vSmallIntestine, BGE::ChymeLink::SmallIntestineChymeToVasculature);
  //lSmallIntestineChymeToVasculature.MapPath(SmallIntestineC1ToSmallIntestine1);

  SELiquidCompartmentGraph& gCombinedCardiovascular = m_Compartments->GetActiveCardiovascularGraph();
  gCombinedCardiovascular.AddCompartment(cSmallIntestine);
  //gCombinedCardiovascular.AddLink(lSmallIntestineChymeToVasculature);
  gCombinedCardiovascular.StateChange();
}

void BioGears::SetupAnesthesiaMachine()
{
  Info("Setting Up Anesthesia Machine");
  /////////////////////// Circuit Interdependencies
  double AmbientPressure_cmH2O = 1033.23; // = 1 atm // Also defined in SetupRespiratoryCircuit
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  ///////////////////////

  double ventilatorVolume_L = 1.0;
  double ventilatorCompliance_L_Per_cmH2O = 0.5;
  double dValveOpenResistance = m_Config->GetMachineOpenResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  double dValveClosedResistance = m_Config->GetMachineClosedResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  double dSwitchOpenResistance = m_Config->GetDefaultOpenFlowResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  double dSwitchClosedResistance = m_Config->GetDefaultClosedFlowResistance(FlowResistanceUnit::cmH2O_s_Per_L);
  double dLowResistance = 0.01;

  SEFluidCircuit& cAnesthesia = m_Circuits->GetAnesthesiaMachineCircuit();
  SEFluidCircuitNode* Ambient = m_Circuits->GetFluidNode(BGE::EnvironmentNode::Ambient);
  cAnesthesia.AddReferenceNode(*Ambient);

  ////////////////
  // Ventilator //
  SEFluidCircuitNode& Ventilator = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::Ventilator);
  Ventilator.GetVolumeBaseline().SetValue(ventilatorVolume_L, VolumeUnit::L);
  Ventilator.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  //////////////////////////
  // VentilatorConnection //
  SEFluidCircuitNode& VentilatorConnection = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::VentilatorConnection);
  VentilatorConnection.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  /////////////////
  // ReliefValve //
  SEFluidCircuitNode& ReliefValve = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::ReliefValve);
  ReliefValve.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  //////////////
  // Selector //
  SEFluidCircuitNode& Selector = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::Selector);
  Selector.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Selector.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  //////////////
  // Scrubber //
  SEFluidCircuitNode& Scrubber = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::Scrubber);
  Scrubber.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Scrubber.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ////////////
  // YPiece //
  SEFluidCircuitNode& Ypiece = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::YPiece);
  Ypiece.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  Ypiece.GetVolumeBaseline().SetValue(0.01, VolumeUnit::L);
  //////////////
  // GasInlet //
  SEFluidCircuitNode& GasInlet = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::GasInlet);
  GasInlet.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  GasInlet.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ///////////////
  // GasSource //
  SEFluidCircuitNode& GasSource = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::GasSource);
  GasSource.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  GasSource.GetVolumeBaseline().SetValue(std::numeric_limits<double>::infinity(), VolumeUnit::mL);
  //////////////////////////
  // AnesthesiaConnection //
  SEFluidCircuitNode& AnesthesiaConnection = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::AnesthesiaConnection);
  AnesthesiaConnection.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  AnesthesiaConnection.GetVolumeBaseline().SetValue(0.01, VolumeUnit::L);
  /////////////////////
  // InspiratoryLimb //
  SEFluidCircuitNode& InspiratoryLimb = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::InspiratoryLimb);
  InspiratoryLimb.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  InspiratoryLimb.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);
  ////////////////////
  // ExpiratoryLimb //
  SEFluidCircuitNode& ExpiratoryLimb = cAnesthesia.CreateNode(BGE::AnesthesiaMachineNode::ExpiratoryLimb);
  ExpiratoryLimb.GetPressure().SetValue(AmbientPressure_cmH2O, PressureUnit::cmH2O);
  ExpiratoryLimb.GetVolumeBaseline().SetValue(0.1, VolumeUnit::L);

  /////////////////////////////
  // EnvironmentToVentilator //
  SEFluidCircuitPath& EnvironmentToVentilator = cAnesthesia.CreatePath(*Ambient, Ventilator, BGE::AnesthesiaMachinePath::EnvironmentToVentilator);
  EnvironmentToVentilator.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);
  //////////////////////////////
  // EnvironmentToReliefValve //
  SEFluidCircuitPath& EnvironmentToReliefValve = cAnesthesia.CreatePath(*Ambient, ReliefValve, BGE::AnesthesiaMachinePath::EnvironmentToReliefValve);
  EnvironmentToReliefValve.GetPressureSourceBaseline().SetValue(100.0, PressureUnit::cmH2O);
  //////////////////////////////////////
  // VentilatorConnectionToVentilator //
  SEFluidCircuitPath& VentilatorToVentilatorConnection = cAnesthesia.CreatePath(Ventilator, VentilatorConnection, BGE::AnesthesiaMachinePath::VentilatorToVentilatorConnection);
  VentilatorToVentilatorConnection.GetComplianceBaseline().SetValue(ventilatorCompliance_L_Per_cmH2O, FlowComplianceUnit::L_Per_cmH2O);
  VentilatorToVentilatorConnection.SetNextPolarizedState(CDM::enumOpenClosed::Closed);
  ////////////////////////////////////
  // VentilatorConnectionToSelector //
  SEFluidCircuitPath& VentilatorConnectionToSelector = cAnesthesia.CreatePath(VentilatorConnection, Selector, BGE::AnesthesiaMachinePath::VentilatorConnectionToSelector);
  ///////////////////////////
  // SelectorToReliefValve //
  SEFluidCircuitPath& SelectorToReliefValve = cAnesthesia.CreatePath(Selector, ReliefValve, BGE::AnesthesiaMachinePath::SelectorToReliefValve);
  SelectorToReliefValve.SetNextValve(CDM::enumOpenClosed::Open);
  ////////////////////////
  // SelectorToScrubber //
  SEFluidCircuitPath& SelectorToScrubber = cAnesthesia.CreatePath(Selector, Scrubber, BGE::AnesthesiaMachinePath::SelectorToScrubber);
  SelectorToScrubber.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  ////////////////////////
  // ScrubberToGasInlet //
  SEFluidCircuitPath& ScrubberToGasInlet = cAnesthesia.CreatePath(Scrubber, GasInlet, BGE::AnesthesiaMachinePath::ScrubberToGasInlet);
  ScrubberToGasInlet.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  ////////////////////////////
  // EnvironmentToGasSource //
  SEFluidCircuitPath& EnvironmentToGasSource = cAnesthesia.CreatePath(*Ambient, GasSource, BGE::AnesthesiaMachinePath::EnvironmentToGasSource);
  ///////////////////////////
  // SelectorToEnvironment //
  SEFluidCircuitPath& SelectorToEnvironment = cAnesthesia.CreatePath(Selector, *Ambient, BGE::AnesthesiaMachinePath::SelectorToEnvironment);
  SelectorToEnvironment.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::L_Per_s); //Exhaust
  /////////////////////////
  // GasSourceToGasInlet //
  SEFluidCircuitPath& GasSourceToGasInlet = cAnesthesia.CreatePath(GasSource, GasInlet, BGE::AnesthesiaMachinePath::GasSourceToGasInlet);
  GasSourceToGasInlet.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::L_Per_s); //Fresh gas source
  ///////////////////////////////
  // GasInletToInspiratoryLimb //
  SEFluidCircuitPath& GasInletToInspiratoryLimb = cAnesthesia.CreatePath(GasInlet, InspiratoryLimb, BGE::AnesthesiaMachinePath::GasInletToInspiratoryLimb);
  GasInletToInspiratoryLimb.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  /////////////////////////////
  // InspiratoryLimbToYPiece //
  SEFluidCircuitPath& InspiratoryLimbToYPiece = cAnesthesia.CreatePath(InspiratoryLimb, Ypiece, BGE::AnesthesiaMachinePath::InspiratoryLimbToYPiece);
  InspiratoryLimbToYPiece.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  ////////////////////////////
  // YPieceToExpiratoryLimb //
  SEFluidCircuitPath& YPieceToExpiratoryLimb = cAnesthesia.CreatePath(Ypiece, ExpiratoryLimb, BGE::AnesthesiaMachinePath::YPieceToExpiratoryLimb);
  YPieceToExpiratoryLimb.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  //////////////////////////////
  // ExpiratoryLimbToSelector //
  SEFluidCircuitPath& ExpiratoryLimbToSelector = cAnesthesia.CreatePath(ExpiratoryLimb, Selector, BGE::AnesthesiaMachinePath::ExpiratoryLimbToSelector);
  ExpiratoryLimbToSelector.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  //////////////////////////////////
  // YPieceToAnesthesiaConnection //
  SEFluidCircuitPath& YPieceToAnesthesiaConnection = cAnesthesia.CreatePath(Ypiece, AnesthesiaConnection, BGE::AnesthesiaMachinePath::YPieceToAnesthesiaConnection);
  ///////////////////////////////////////
  // AnesthesiaConnectionToEnvironment //
  SEFluidCircuitPath& AnesthesiaConnectionToEnvironment = cAnesthesia.CreatePath(AnesthesiaConnection, *Ambient, BGE::AnesthesiaMachinePath::AnesthesiaConnectionToEnvironment);
  AnesthesiaConnectionToEnvironment.GetResistanceBaseline().SetValue(dSwitchOpenResistance, FlowResistanceUnit::cmH2O_s_Per_L);

  cAnesthesia.SetNextAndCurrentFromBaselines();
  cAnesthesia.StateChange();

  //Combined Respiratory and Anesthesia Machine Circuit
  SEFluidCircuit& cCombinedAnesthesia = m_Circuits->GetRespiratoryAndAnesthesiaMachineCircuit();
  cCombinedAnesthesia.AddCircuit(cRespiratory);
  cCombinedAnesthesia.AddCircuit(cAnesthesia);
  SEFluidCircuitNode& Mouth = *cCombinedAnesthesia.GetNode(BGE::RespiratoryNode::Mouth);
  SEFluidCircuitPath& AnesthesiaConnectionToMouth = cCombinedAnesthesia.CreatePath(AnesthesiaConnection, Mouth, "AnesthesiaConnectionToMouth");
  cCombinedAnesthesia.RemovePath(BGE::RespiratoryPath::EnvironmentToMouth);
  cCombinedAnesthesia.SetNextAndCurrentFromBaselines();
  cCombinedAnesthesia.StateChange();

  // Grab the Environment Compartment
  SEGasCompartment* eEnvironment = m_Compartments->GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  // Anesthesia Machine Compartments
  SEGasCompartment& aAnesthesiaConnection = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::AnesthesiaConnection);
  aAnesthesiaConnection.MapNode(AnesthesiaConnection);
  SEGasCompartment& aExpiratoryLimb = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::ExpiratoryLimb);
  aExpiratoryLimb.MapNode(ExpiratoryLimb);
  SEGasCompartment& aGasInlet = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::GasInlet);
  aGasInlet.MapNode(GasInlet);
  SEGasCompartment& aGasSource = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::GasSource);
  aGasSource.MapNode(GasSource);
  SEGasCompartment& aInspiratoryLimb = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::InspiratoryLimb);
  aInspiratoryLimb.MapNode(InspiratoryLimb);
  SEGasCompartment& aReliefValve = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::ReliefValve);
  aReliefValve.MapNode(ReliefValve);
  SEGasCompartment& aScrubber = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::Scrubber);
  aScrubber.MapNode(Scrubber);
  SEGasCompartment& aSelector = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::Selector);
  aSelector.MapNode(Selector);
  SEGasCompartment& aVentilator = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::Ventilator);
  aVentilator.MapNode(Ventilator);
  SEGasCompartment& aVentilatorConnection = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::VentilatorConnection);
  aVentilator.MapNode(VentilatorConnection);
  SEGasCompartment& aYPiece = m_Compartments->CreateGasCompartment(BGE::AnesthesiaMachineCompartment::YPiece);
  aYPiece.MapNode(Ypiece);

  // Setup Links //
  SEGasCompartmentLink& aVentilatorToSelector = m_Compartments->CreateGasLink(aVentilator, aSelector, BGE::AnesthesiaMachineLink::VentilatorToSelector);
  aVentilatorToSelector.MapPath(VentilatorConnectionToSelector);
  SEGasCompartmentLink& aSelectorToReliefValve = m_Compartments->CreateGasLink(aSelector, aReliefValve, BGE::AnesthesiaMachineLink::SelectorToReliefValve);
  aSelectorToReliefValve.MapPath(SelectorToReliefValve);
  SEGasCompartmentLink& aSelectorToScrubber = m_Compartments->CreateGasLink(aSelector, aScrubber, BGE::AnesthesiaMachineLink::SelectorToScrubber);
  aSelectorToScrubber.MapPath(SelectorToScrubber);
  SEGasCompartmentLink& aScrubberToGasInlet = m_Compartments->CreateGasLink(aScrubber, aGasInlet, BGE::AnesthesiaMachineLink::ScrubberToGasInlet);
  aScrubberToGasInlet.MapPath(ScrubberToGasInlet);
  SEGasCompartmentLink& aExhaust = m_Compartments->CreateGasLink(aSelector, *eEnvironment, BGE::AnesthesiaMachineLink::Exhaust);
  aExhaust.MapPath(SelectorToEnvironment);
  SEGasCompartmentLink& aGasSourceToGasInlet = m_Compartments->CreateGasLink(aGasSource, aGasInlet, BGE::AnesthesiaMachineLink::GasSourceToGasInlet);
  aGasSourceToGasInlet.MapPath(GasSourceToGasInlet);
  SEGasCompartmentLink& aGasInletToInspiratoryLimb = m_Compartments->CreateGasLink(aGasInlet, aInspiratoryLimb, BGE::AnesthesiaMachineLink::GasInletToInspiratoryLimb);
  aGasInletToInspiratoryLimb.MapPath(GasInletToInspiratoryLimb);
  SEGasCompartmentLink& aInspiratoryLimbToYPiece = m_Compartments->CreateGasLink(aInspiratoryLimb, aYPiece, BGE::AnesthesiaMachineLink::InspiratoryLimbToYPiece);
  aInspiratoryLimbToYPiece.MapPath(InspiratoryLimbToYPiece);
  SEGasCompartmentLink& aYPieceToExpiratoryLimb = m_Compartments->CreateGasLink(aYPiece, aExpiratoryLimb, BGE::AnesthesiaMachineLink::YPieceToExpiratoryLimb);
  aYPieceToExpiratoryLimb.MapPath(YPieceToExpiratoryLimb);
  SEGasCompartmentLink& aExpiratoryLimbToSelector = m_Compartments->CreateGasLink(aExpiratoryLimb, aSelector, BGE::AnesthesiaMachineLink::ExpiratoryLimbToSelector);
  aExpiratoryLimbToSelector.MapPath(ExpiratoryLimbToSelector);
  SEGasCompartmentLink& aYPieceToAnesthesiaConnection = m_Compartments->CreateGasLink(aYPiece, aAnesthesiaConnection, BGE::AnesthesiaMachineLink::YPieceToAnesthesiaConnection);
  aYPieceToAnesthesiaConnection.MapPath(YPieceToAnesthesiaConnection);
  SEGasCompartmentLink& aAnesthesiaConnectionLeak = m_Compartments->CreateGasLink(aAnesthesiaConnection, *eEnvironment, BGE::AnesthesiaMachineLink::AnesthesiaConnectionLeak);
  aAnesthesiaConnectionLeak.MapPath(AnesthesiaConnectionToEnvironment);

  SEGasCompartmentGraph& gAnesthesia = m_Compartments->GetAnesthesiaMachineGraph();
  gAnesthesia.AddCompartment(*eEnvironment);
  gAnesthesia.AddCompartment(aAnesthesiaConnection);
  gAnesthesia.AddCompartment(aExpiratoryLimb);
  gAnesthesia.AddCompartment(aGasInlet);
  gAnesthesia.AddCompartment(aGasSource);
  gAnesthesia.AddCompartment(aInspiratoryLimb);
  gAnesthesia.AddCompartment(aReliefValve);
  gAnesthesia.AddCompartment(aScrubber);
  gAnesthesia.AddCompartment(aSelector);
  gAnesthesia.AddCompartment(aVentilator);
  gAnesthesia.AddCompartment(aYPiece);
  gAnesthesia.AddLink(aVentilatorToSelector);
  gAnesthesia.AddLink(aSelectorToReliefValve);
  gAnesthesia.AddLink(aSelectorToScrubber);
  gAnesthesia.AddLink(aScrubberToGasInlet);
  gAnesthesia.AddLink(aExhaust);
  gAnesthesia.AddLink(aGasSourceToGasInlet);
  gAnesthesia.AddLink(aGasInletToInspiratoryLimb);
  gAnesthesia.AddLink(aInspiratoryLimbToYPiece);
  gAnesthesia.AddLink(aYPieceToExpiratoryLimb);
  gAnesthesia.AddLink(aExpiratoryLimbToSelector);
  gAnesthesia.AddLink(aYPieceToAnesthesiaConnection);
  gAnesthesia.AddLink(aAnesthesiaConnectionLeak);
  gAnesthesia.StateChange();

  //Now do the combined transport setup
  // Grab the mouth from pulmonary
  SEGasCompartment* pMouth = m_Compartments->GetGasCompartment(BGE::PulmonaryCompartment::Mouth);
  SEGasCompartmentLink& aMask = m_Compartments->CreateGasLink(aAnesthesiaConnection, *pMouth, BGE::AnesthesiaMachineLink::Mask);
  aMask.MapPath(AnesthesiaConnectionToMouth);

  SEGasCompartmentGraph& gCombinedRespiratoryAnesthesia = m_Compartments->GetRespiratoryAndAnesthesiaMachineGraph();
  gCombinedRespiratoryAnesthesia.AddGraph(gRespiratory);
  gCombinedRespiratoryAnesthesia.AddGraph(gAnesthesia);
  gCombinedRespiratoryAnesthesia.RemoveLink(BGE::PulmonaryLink::EnvironmentToMouth);
  gCombinedRespiratoryAnesthesia.AddLink(aMask);
  gCombinedRespiratoryAnesthesia.StateChange();
}

void BioGears::SetupInhaler()
{
  Info("Setting Up Inhaler");
  /////////////////////// Circuit Interdependencies
  double dLowResistance = 0.01; // Also defined in SetupRespiratoryCircuit
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  SELiquidCompartmentGraph& lAerosol = m_Compartments->GetAerosolGraph();
  ///////////////////////

  //Combined Respiratory and Inhaler Circuit
  SEFluidCircuit& m_CombinedInhaler = m_Circuits->GetRespiratoryAndInhalerCircuit();
  m_CombinedInhaler.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Mouth = *cRespiratory.GetNode(BGE::RespiratoryNode::Mouth);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(BGE::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Mouthpiece = m_CombinedInhaler.CreateNode(BGE::InhalerNode::Mouthpiece);
  Mouthpiece.GetPressure().SetValue(0.0, PressureUnit::cmH2O);
  Mouthpiece.GetNextPressure().SetValue(0.0, PressureUnit::cmH2O);
  double dInhalerBaseVolume_L = 0.030; // 30 milliliters
  Mouthpiece.GetVolumeBaseline().SetValue(dInhalerBaseVolume_L, VolumeUnit::L);
  // Define path on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitPath& EnvironmentToMouthpiece = m_CombinedInhaler.CreatePath(Ambient, Mouthpiece, BGE::InhalerPath::EnvironmentToMouthpiece);
  // Connect Path
  SEFluidCircuitPath& MouthpieceToMouth = m_CombinedInhaler.CreatePath(Mouthpiece, Mouth, BGE::InhalerPath::MouthpieceToMouth);
  MouthpieceToMouth.GetResistanceBaseline().SetValue(dLowResistance, FlowResistanceUnit::cmH2O_s_Per_L);
  m_CombinedInhaler.RemovePath(BGE::RespiratoryPath::EnvironmentToMouth);
  m_CombinedInhaler.SetNextAndCurrentFromBaselines();
  m_CombinedInhaler.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gMouth = m_Compartments->GetGasCompartment(BGE::PulmonaryCompartment::Mouth);
  SEGasCompartment* gAmbient = m_Compartments->GetGasCompartment(BGE::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SEGasCompartment& gMouthpiece = m_Compartments->CreateGasCompartment(BGE::InhalerCompartment::Mouthpiece);
  gMouthpiece.MapNode(Mouthpiece);
  ///////////
  // Links //
  SEGasCompartmentLink& gEnvironmentToMouthpiece = m_Compartments->CreateGasLink(*gAmbient, gMouthpiece, BGE::InhalerLink::EnvironmentToMouthpiece);
  gEnvironmentToMouthpiece.MapPath(EnvironmentToMouthpiece);
  SEGasCompartmentLink& gMouthpieceToMouth = m_Compartments->CreateGasLink(gMouthpiece, *gMouth, BGE::InhalerLink::MouthpieceToMouth);
  gMouthpieceToMouth.MapPath(MouthpieceToMouth);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedInhaler = m_Compartments->GetRespiratoryAndInhalerGraph();
  gCombinedInhaler.AddGraph(gRespiratory);
  gCombinedInhaler.RemoveLink(BGE::PulmonaryLink::EnvironmentToMouth);
  gCombinedInhaler.AddCompartment(gMouthpiece);
  gCombinedInhaler.AddLink(gEnvironmentToMouthpiece);
  gCombinedInhaler.AddLink(gMouthpieceToMouth);
  gCombinedInhaler.StateChange();

  // I could probably take the generic code I wrote in SetupRespiratory to clone the gas setup into a liquid setup
  // and then call that method here, but this is such a simple circuit... I will leave that as an exercise for somebody else...

  ///////////////////////////////////
  // LIQUID (AEROSOL) COMPARTMENTS //
  SELiquidCompartment* lMouth = m_Compartments->GetLiquidCompartment(BGE::PulmonaryCompartment::Mouth);
  SELiquidCompartment* lAmbient = m_Compartments->GetLiquidCompartment(BGE::EnvironmentCompartment::Ambient);
  //////////////////
  // Compartments //
  SELiquidCompartment& lMouthpiece = m_Compartments->CreateLiquidCompartment(BGE::InhalerCompartment::Mouthpiece);
  lMouthpiece.MapNode(Mouthpiece);
  ///////////
  // Links //
  SELiquidCompartmentLink& lEnvironmentToMouthpiece = m_Compartments->CreateLiquidLink(*lAmbient, lMouthpiece, BGE::InhalerLink::EnvironmentToMouthpiece);
  lEnvironmentToMouthpiece.MapPath(EnvironmentToMouthpiece);
  SELiquidCompartmentLink& lMouthpieceToMouth = m_Compartments->CreateLiquidLink(lMouthpiece, *lMouth, BGE::InhalerLink::MouthpieceToMouth);
  lMouthpieceToMouth.MapPath(MouthpieceToMouth);
  ///////////
  // Graph //
  SELiquidCompartmentGraph& lCombinedInhaler = m_Compartments->GetAerosolAndInhalerGraph();
  lCombinedInhaler.AddGraph(lAerosol);
  lCombinedInhaler.RemoveLink(BGE::PulmonaryLink::EnvironmentToMouth);
  lCombinedInhaler.AddCompartment(lMouthpiece);
  lCombinedInhaler.AddLink(lEnvironmentToMouthpiece);
  lCombinedInhaler.AddLink(lMouthpieceToMouth);
  lCombinedInhaler.StateChange();
}

void BioGears::SetupMechanicalVentilator()
{
  Info("Setting Up MechanicalVentilator");
  /////////////////////// Circuit Interdependencies
  SEFluidCircuit& cRespiratory = m_Circuits->GetRespiratoryCircuit();
  SEGasCompartmentGraph& gRespiratory = m_Compartments->GetRespiratoryGraph();
  ///////////////////////

  //Combined Respiratory and Mechanical Ventilator Circuit
  SEFluidCircuit& m_CombinedMechanicalVentilator = m_Circuits->GetRespiratoryAndMechanicalVentilatorCircuit();
  m_CombinedMechanicalVentilator.AddCircuit(cRespiratory);
  // Grab connection points/nodes
  SEFluidCircuitNode& Mouth = *cRespiratory.GetNode(BGE::RespiratoryNode::Mouth);
  SEFluidCircuitNode& Ambient = *cRespiratory.GetNode(BGE::EnvironmentNode::Ambient);
  // Define node on the combined graph, this is a simple circuit, no reason to make a independent circuit at this point
  SEFluidCircuitNode& Connection = m_CombinedMechanicalVentilator.CreateNode(BGE::MechanicalVentilatorNode::Connection);
  Connection.GetPressure().Set(Ambient.GetPressure());
  Connection.GetNextPressure().Set(Ambient.GetNextPressure());
  // No connection volume, so volume fractions work properly
  // Paths
  SEFluidCircuitPath& ConnectionToMouth = m_CombinedMechanicalVentilator.CreatePath(Connection, Mouth, BGE::MechanicalVentilatorPath::ConnectionToMouth);
  ConnectionToMouth.GetFlowSourceBaseline().SetValue(0.0, VolumePerTimeUnit::L_Per_s);
  SEFluidCircuitPath& GroundToConnection = m_CombinedMechanicalVentilator.CreatePath(Ambient, Connection, BGE::MechanicalVentilatorPath::GroundToConnection);
  GroundToConnection.GetPressureSourceBaseline().SetValue(0.0, PressureUnit::cmH2O);
  m_CombinedMechanicalVentilator.RemovePath(BGE::RespiratoryPath::EnvironmentToMouth);
  m_CombinedMechanicalVentilator.SetNextAndCurrentFromBaselines();
  m_CombinedMechanicalVentilator.StateChange();

  //////////////////////
  // GAS COMPARTMENTS //
  SEGasCompartment* gMouth = m_Compartments->GetGasCompartment(BGE::PulmonaryCompartment::Mouth);
  //////////////////
  // Compartments //
  SEGasCompartment& gConnection = m_Compartments->CreateGasCompartment(BGE::MechanicalVentilatorCompartment::Connection);
  gConnection.MapNode(Connection);
  ///////////
  // Links //
  SEGasCompartmentLink& gConnectionToMouth = m_Compartments->CreateGasLink(gConnection, *gMouth, BGE::MechanicalVentilatorLink::ConnectionToMouth);
  gConnectionToMouth.MapPath(ConnectionToMouth);
  ///////////
  // Graph //
  SEGasCompartmentGraph& gCombinedMechanicalVentilator = m_Compartments->GetRespiratoryAndMechanicalVentilatorGraph();
  gCombinedMechanicalVentilator.AddGraph(gRespiratory);
  gCombinedMechanicalVentilator.RemoveLink(BGE::PulmonaryLink::EnvironmentToMouth);
  gCombinedMechanicalVentilator.AddCompartment(gConnection);
  gCombinedMechanicalVentilator.AddLink(gConnectionToMouth);
  gCombinedMechanicalVentilator.StateChange();
}

void BioGears::SetupExternalTemperature()
{
  Info("Setting Up External Temperature");
  SEThermalCircuit& exthermal = m_Circuits->GetExternalTemperatureCircuit();

  double dNoResistance = m_Config->GetDefaultClosedHeatResistance(HeatResistanceUnit::K_Per_W);
  double dMaxResistance = m_Config->GetDefaultOpenHeatResistance(HeatResistanceUnit::K_Per_W);
  //Define Nodes
  //Initialize temperatures to a reasonable value (ambient temperature hasn't been read in yet)
  double dAmbientTemperature_K = 295.4; //~72F

  //Note: All resistances are bound by the default open and closed switch resistances
  SEThermalCircuitNode& ThermalEnvironment = exthermal.CreateNode(BGE::ExternalTemperatureNode::Ambient);
  ThermalEnvironment.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Skin = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalSkin);
  Skin.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Core = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalCore);
  Core.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Clothing = exthermal.CreateNode(BGE::ExternalTemperatureNode::Clothing);
  Clothing.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Enclosure = exthermal.CreateNode(BGE::ExternalTemperatureNode::Enclosure);
  Enclosure.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Active = exthermal.CreateNode(BGE::ExternalTemperatureNode::Active);
  Active.GetTemperature().SetValue(dAmbientTemperature_K, TemperatureUnit::K);
  SEThermalCircuitNode& Ground = exthermal.CreateNode(BGE::ExternalTemperatureNode::ExternalGround);
  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
  exthermal.AddReferenceNode(Ground);

  //Define Paths
  //Everything will be properly initialized in Evironment::Reset
  SEThermalCircuitPath& EnvironmentCoreToAbsoluteReferencePath = exthermal.CreatePath(Core, Ground, BGE::ExternalTemperaturePath::ExternalCoreToGround);
  EnvironmentCoreToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToAbsoluteReferencePath = exthermal.CreatePath(Skin, Ground, BGE::ExternalTemperaturePath::ExternalSkinToGround);
  EnvironmentSkinToAbsoluteReferencePath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& EnvironmentSkinToClothingPath = exthermal.CreatePath(Skin, Clothing, BGE::ExternalTemperaturePath::ExternalSkinToClothing);
  EnvironmentSkinToClothingPath.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& AbsoluteReferenceToClothingPath = exthermal.CreatePath(Ground, Clothing, BGE::ExternalTemperaturePath::GroundToClothing);
  AbsoluteReferenceToClothingPath.GetNextHeatSource().SetValue(0.0, PowerUnit::W);
  SEThermalCircuitPath& ClothingToAmbientEnvironmentPath = exthermal.CreatePath(Clothing, ThermalEnvironment, BGE::ExternalTemperaturePath::ClothingToEnvironment);
  ClothingToAmbientEnvironmentPath.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& ClothingToEnclosure = exthermal.CreatePath(Clothing, Enclosure, BGE::ExternalTemperaturePath::ClothingToEnclosure);
  ClothingToEnclosure.GetNextResistance().SetValue(dNoResistance, HeatResistanceUnit::K_Per_W);
  SEThermalCircuitPath& AbsoluteReferenceToAmbientEnvironmentPath = exthermal.CreatePath(Ground, ThermalEnvironment, BGE::ExternalTemperaturePath::GroundToEnvironment);
  AbsoluteReferenceToAmbientEnvironmentPath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToEnclosurePath = exthermal.CreatePath(Ground, Enclosure, BGE::ExternalTemperaturePath::GroundToEnclosure);
  AbsoluteReferenceToEnclosurePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& AbsoluteReferenceToActivePath = exthermal.CreatePath(Ground, Active, BGE::ExternalTemperaturePath::GroundToActive);
  AbsoluteReferenceToActivePath.GetNextTemperatureSource().SetValue(0.0, TemperatureUnit::K);
  SEThermalCircuitPath& ActiveToClothing = exthermal.CreatePath(Active, Clothing, BGE::ExternalTemperaturePath::ActiveToClothing);
  ActiveToClothing.SetNextSwitch(CDM::enumOpenClosed::Open);

  exthermal.SetNextAndCurrentFromBaselines();
  exthermal.StateChange();

  SEThermalCompartment& cActive = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Active);
  cActive.MapNode(Active);
  SEThermalCompartment& cClothing = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Clothing);
  cClothing.MapNode(Clothing);
  SEThermalCompartment& cEnclosure = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Enclosure);
  cEnclosure.MapNode(Enclosure);
  SEThermalCompartment& cExternalCore = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalCore);
  cExternalCore.MapNode(Core);
  SEThermalCompartment& cExternalSkin = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalSkin);
  cExternalSkin.MapNode(Skin);
  SEThermalCompartment& cExternalGround = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::ExternalGround);
  cExternalGround.MapNode(Ground);
  SEThermalCompartment& cAmbient = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::Ambient);
  cAmbient.MapNode(ThermalEnvironment);

  SEThermalCompartmentLink& lActiveToClothing = m_Compartments->CreateThermalLink(cActive, cClothing, BGE::TemperatureLink::ActiveToClothing);
  lActiveToClothing.MapPath(ActiveToClothing);
  SEThermalCompartmentLink& lClothingToEnclosure = m_Compartments->CreateThermalLink(cClothing, cEnclosure, BGE::TemperatureLink::ClothingToEnclosure);
  lClothingToEnclosure.MapPath(ClothingToEnclosure);
  SEThermalCompartmentLink& lClothingToEnvironment = m_Compartments->CreateThermalLink(cClothing, cAmbient, BGE::TemperatureLink::ClothingToEnvironment);
  lClothingToEnvironment.MapPath(ClothingToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalCoreToGround = m_Compartments->CreateThermalLink(cExternalCore, cExternalGround, BGE::TemperatureLink::ExternalCoreToGround);
  lExternalCoreToGround.MapPath(EnvironmentCoreToAbsoluteReferencePath);
  SEThermalCompartmentLink& lGroundToActive = m_Compartments->CreateThermalLink(cExternalGround, cActive, BGE::TemperatureLink::GroundToActive);
  lGroundToActive.MapPath(AbsoluteReferenceToActivePath);
  SEThermalCompartmentLink& lGroundToClothing = m_Compartments->CreateThermalLink(cExternalGround, cClothing, BGE::TemperatureLink::GroundToClothing);
  lGroundToClothing.MapPath(AbsoluteReferenceToClothingPath);
  SEThermalCompartmentLink& lGroundToEnclosure = m_Compartments->CreateThermalLink(cExternalGround, cEnclosure, BGE::TemperatureLink::GroundToEnclosure);
  lGroundToEnclosure.MapPath(AbsoluteReferenceToEnclosurePath);
  SEThermalCompartmentLink& lGroundToEnvironment = m_Compartments->CreateThermalLink(cExternalGround, cAmbient, BGE::TemperatureLink::GroundToEnvironment);
  lGroundToEnvironment.MapPath(AbsoluteReferenceToAmbientEnvironmentPath);
  SEThermalCompartmentLink& lExternalSkinToGround = m_Compartments->CreateThermalLink(cExternalSkin, cExternalGround, BGE::TemperatureLink::ExternalSkinToGround);
  lExternalSkinToGround.MapPath(EnvironmentSkinToAbsoluteReferencePath);
  SEThermalCompartmentLink& lExternalSkinToClothing = m_Compartments->CreateThermalLink(cExternalSkin, cClothing, BGE::TemperatureLink::ExternalSkinToClothing);
  lExternalSkinToClothing.MapPath(EnvironmentSkinToClothingPath);
}

void BioGears::SetupInternalTemperature()
{
  Info("Setting Up Internal Temperature");
  SEThermalCircuit& cIntemperature = m_Circuits->GetInternalTemperatureCircuit();

  SEThermalCircuitNode& Core = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalCore);
  Core.GetTemperature().SetValue(37.0, TemperatureUnit::C);
  // Peripheral Temperature
  SEThermalCircuitNode& Skin = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalSkin);
  Skin.GetTemperature().SetValue(33.0, TemperatureUnit::C);
  SEThermalCircuitNode& Ground = cIntemperature.CreateNode(BGE::InternalTemperatureNode::InternalGround);
  Ground.GetTemperature().SetValue(0.0, TemperatureUnit::K);
  Ground.GetNextTemperature().SetValue(0.0, TemperatureUnit::K);
  cIntemperature.AddReferenceNode(Ground);

  /*Metabolic heat generation, leading to a heat source into the core*/
  SEThermalCircuitPath& TemperatureGroundToCore = cIntemperature.CreatePath(Ground, Core, BGE::InternalTemperaturePath::GroundToInternalCore);
  TemperatureGroundToCore.GetHeatSourceBaseline().SetValue(0.0, PowerUnit::W);
  /*Conduction and convection leading a resistance of heat flow from the core to the skin*/
  SEThermalCircuitPath& CoreToSkin = cIntemperature.CreatePath(Core, Skin, BGE::InternalTemperaturePath::InternalCoreToInternalSkin);
  CoreToSkin.GetResistanceBaseline().SetValue(0.056, HeatResistanceUnit::K_Per_W);

  double skinMassFraction = 0.09; //0.09 is fraction of mass that the skin takes up in a typical human /cite herman2006physics
  SEThermalCircuitPath& CoreToTemperatureGround = cIntemperature.CreatePath(Core, Ground, BGE::InternalTemperaturePath::InternalCoreToGround);
  CoreToTemperatureGround.GetCapacitanceBaseline().SetValue((1.0 - skinMassFraction) * m_Patient->GetWeight(MassUnit::kg) * GetConfiguration().GetBodySpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg), HeatCapacitanceUnit::J_Per_K);
  Core.GetHeatBaseline().SetValue(CoreToTemperatureGround.GetCapacitanceBaseline().GetValue(HeatCapacitanceUnit::J_Per_K) * Core.GetTemperature().GetValue(TemperatureUnit::K), EnergyUnit::J);

  SEThermalCircuitPath& SkinToTemperatureGround = cIntemperature.CreatePath(Skin, Ground, BGE::InternalTemperaturePath::InternalSkinToGround);
  SkinToTemperatureGround.GetCapacitanceBaseline().SetValue(skinMassFraction * m_Patient->GetWeight(MassUnit::kg) * GetConfiguration().GetBodySpecificHeat(HeatCapacitancePerMassUnit::J_Per_K_kg), HeatCapacitanceUnit::J_Per_K);
  Skin.GetHeatBaseline().SetValue(SkinToTemperatureGround.GetCapacitanceBaseline().GetValue(HeatCapacitanceUnit::J_Per_K) * Skin.GetTemperature().GetValue(TemperatureUnit::K), EnergyUnit::J);

  cIntemperature.SetNextAndCurrentFromBaselines();
  cIntemperature.StateChange();

  SEThermalCompartment& InternalCore = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::InternalCore);
  InternalCore.MapNode(Core);
  SEThermalCompartment& InternalSkin = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::InternalSkin);
  InternalSkin.MapNode(Skin);
  SEThermalCompartment& InternalGround = m_Compartments->CreateThermalCompartment(BGE::TemperatureCompartment::InternalGround);
  InternalCore.MapNode(Ground);

  SEThermalCompartmentLink& GroundToInternalCore = m_Compartments->CreateThermalLink(InternalGround, InternalCore, BGE::TemperatureLink::GroundToInternalCore);
  GroundToInternalCore.MapPath(TemperatureGroundToCore);
  SEThermalCompartmentLink& InternalCoreToInternalSkin = m_Compartments->CreateThermalLink(InternalGround, InternalCore, BGE::TemperatureLink::InternalCoreToInternalSkin);
  InternalCoreToInternalSkin.MapPath(CoreToSkin);
  SEThermalCompartmentLink& InternalCoreToGround = m_Compartments->CreateThermalLink(InternalGround, InternalCore, BGE::TemperatureLink::InternalCoreToGround);
  InternalCoreToGround.MapPath(CoreToTemperatureGround);
  SEThermalCompartmentLink& InternalSkinToGround = m_Compartments->CreateThermalLink(InternalGround, InternalCore, BGE::TemperatureLink::InternalSkinToGround);
  InternalSkinToGround.MapPath(SkinToTemperatureGround);
}
}
