//**********************************************************************************
//Copyright 2015 Applied Research Associates, Inc.
//Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//this file except in compliance with the License.You may obtain a copy of the License
//at :
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software distributed under
//the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//CONDITIONS OF ANY KIND, either express or implied.See the License for the
//specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "PopulationGenerator.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <biogears/schema/cdm/Patient.hxx>
#include <biogears/string/manipulation.h>

namespace mil {
namespace tatrc {
  namespace physiology {
    namespace datamodel {
    }
  }
}
}
namespace CDM = mil::tatrc::physiology::datamodel;

std::string dateString()
{
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  std::string s(30, '\0');
  std::strftime(&s[0], s.size(), "%Y-%m-%dT%H_%M_%S", std::localtime(&now));
  return s;
}

namespace biogears {
std::vector<std::string> generate_boy_names();
std::vector<std::string> generate_girl_names();
//-------------------------------------------------------------------------------
PopulationGenerator::PopulationGenerator(std::vector<std::string> params)
{
  if (params.size() == 1) {
    _runs.emplace_back("PatientTemplate.xml", 10);
  } else {
    for (auto i = 0; i < params.size(); ++i) {
      if (i + 2 <= params.size()) {
        _runs.emplace_back(params[i], std::stoi(params[i + 1]));
      }
      i += 2;
    }
  }

  _boy_names = generate_boy_names();
  _girl_names = generate_girl_names();
}
//-------------------------------------------------------------------------------
void PopulationGenerator::SetGirlNames(std::vector<std::string>&& names)
{
  _girl_names = std::move(names);
}
//-------------------------------------------------------------------------------
void PopulationGenerator::SetBoyNames(std::vector<std::string>&& names)
{
  _boy_names = std::move(names);
}
//-------------------------------------------------------------------------------
void PopulationGenerator::AddBoyName(std::string name)
{
  _boy_names.emplace_back(name);
}
//-------------------------------------------------------------------------------
void PopulationGenerator::AddGirlName(std::string name)
{
  _boy_names.emplace_back(name);
}
//-------------------------------------------------------------------------------
void PopulationGenerator::ClearBoyNames()
{
  _boy_names.clear();
}
//-------------------------------------------------------------------------------
void PopulationGenerator::ClearGirlNames()
{
  _girl_names.clear();
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//!
void PopulationGenerator::Generate()
{

  std::random_device rd {};
  std::mt19937 gen { rd() };
  static int total_population_count = 1;
  std::string unit_str = "";
  std::string population_pool_dir = std::string("population_") + dateString();

  for (auto& run : _runs) {
    auto const& config_file = run.first;
    auto const& population_size = run.second;

    
    
    try {
      auto population = CDM::PopulationProfile(config_file);
      for (auto i = 0; i < population_size; ++i) {
        CDM::PatientData patient;
        std::normal_distribution<> standard_distribution { 5, 2 };
        std::binomial_distribution<> binomial_distribution { 1, population->Heterogametic_SexDistribution() };
        std::uniform_int_distribution<> uniform_distribution;

        if (_girl_names.empty()) {
          _girl_names.emplace_back("MalePatient_" + std::to_string(total_population_count));
        }
        if (_boy_names.empty()) {
          _boy_names.emplace_back("FemalePatient_" + std::to_string(total_population_count));
        }


        //This is a really complicated way of saying (
        patient.Sex().set(binomial_distribution(gen) ? CDM::enumSex::Female : CDM::enumSex::Male);
        if (patient.Sex().get() == CDM::enumSex::Male) {
          uniform_distribution = std::uniform_int_distribution<>(0, static_cast<int>(_boy_names.size() - 1));
          int name_index = uniform_distribution(gen);
          
          patient.Name(_boy_names[name_index]);
          _boy_names[name_index] = std::move(_boy_names.back());
          _boy_names.pop_back();
        } else {
          uniform_distribution = std::uniform_int_distribution<>(0, static_cast<int> (_girl_names.size() - 1));
          int name_index = uniform_distribution(gen);
          patient.Name(_girl_names[name_index]);
          _girl_names[name_index] = std::move(_girl_names.back());
          _girl_names.pop_back();
        }

        if (!population->AgeDistribution().empty()) {
          unit_str = population->AgeDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->AgeDistribution()[0].mean(),
                                                             population->AgeDistribution()[0].diviation());
          for (auto& distribution : population->AgeDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "yr";
          }
          patient.Age(standard_distribution(gen));
          patient.Age()->unit(unit_str);
        }
        if (!population->WeightDistribution().empty()) {
          unit_str = population->WeightDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->WeightDistribution()[0].mean(),
                                                             population->WeightDistribution()[0].diviation());
          for (auto& distribution : population->WeightDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "kg";
          }
          patient.Weight(standard_distribution(gen));
          patient.Weight()->unit(unit_str);
        }
        if (!population->HeightDistribution().empty()) {
          unit_str = population->HeightDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->HeightDistribution()[0].mean(),
                                                             population->HeightDistribution()[0].diviation());
          for (auto& distribution : population->HeightDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "cm";
          }
          patient.Height(standard_distribution(gen));
          patient.Height()->unit(unit_str);
        }

        if (!population->BodyFatFractionDistribution().empty()) {
          standard_distribution = std::normal_distribution<>(population->BodyFatFractionDistribution()[0].mean(),
                                                             population->BodyFatFractionDistribution()[0].diviation());
          for (auto& distribution : population->BodyFatFractionDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }

          patient.BodyFatFraction(standard_distribution(gen));
          patient.BodyFatFraction()->unit("");
        }
        if (!population->MaxWorkRateDistribution().empty()) {
          unit_str = population->MaxWorkRateDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->MaxWorkRateDistribution()[0].mean(),
                                                             population->MaxWorkRateDistribution()[0].diviation());
          for (auto& distribution : population->MaxWorkRateDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "W";
          }
          patient.MaxWorkRate(standard_distribution(gen));
          patient.MaxWorkRate()->unit(unit_str);
        }
        if (!population->BloodTypeABODistribution().empty()) {
          std::vector<CDM::DiscreteDistributionRangeData::weight_type> weights;
          for (auto weight : population->BloodTypeABODistribution()[0].weight()) {
            weights.emplace_back(weight);
          }
          auto discrete_distribution = std::discrete_distribution<>(weights.begin(), weights.end());
          for (auto& distribution : population->BloodTypeABODistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              weights.clear();
              for (auto weight : distribution.weight()) {
                weights.emplace_back(weight);
              }
              discrete_distribution = std::discrete_distribution<>(weights.begin(), weights.end());
            }
          }
          //Note: Very easy for the user to key in more weights then blood types and generate
          //Non Viable Enum Values
          patient.BloodTypeABO(CDM::enumBloodType::value(discrete_distribution(gen)));
        }
        if (!population->BloodTypeRhDistribution().empty()) {
          binomial_distribution = std::binomial_distribution<>(1, population->BloodTypeRhDistribution()[0].weight());
          for (auto& distribution : population->BloodTypeRhDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              binomial_distribution = std::binomial_distribution<>(1, distribution.weight());
            }
          }
          patient.BloodTypeRh(binomial_distribution(gen));
        }
        if (!population->AlveoliSurfaceAreaDistribution().empty()) {
          unit_str = population->AlveoliSurfaceAreaDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->AlveoliSurfaceAreaDistribution()[0].mean(),
                                                             population->AlveoliSurfaceAreaDistribution()[0].diviation());
          for (auto& distribution : population->AlveoliSurfaceAreaDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "m^2";
          }
          patient.AlveoliSurfaceArea(standard_distribution(gen));
          patient.AlveoliSurfaceArea()->unit(unit_str);
        }
        if (!population->HyperhidrosisDistribution().empty()) {
          standard_distribution = std::normal_distribution<>(population->HyperhidrosisDistribution()[0].mean(),
                                                             population->HyperhidrosisDistribution()[0].diviation());
          for (auto& distribution : population->HyperhidrosisDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          patient.Hyperhidrosis(standard_distribution(gen));
          patient.Hyperhidrosis()->unit(unit_str);
        }
        if (!population->RightLungRatioDistribution().empty()) {
          standard_distribution = std::normal_distribution<>(population->RightLungRatioDistribution()[0].mean(),
                                                             population->RightLungRatioDistribution()[0].diviation());
          for (auto& distribution : population->RightLungRatioDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          patient.RightLungRatio(standard_distribution(gen));
        }
        if (!population->SkinSurfaceAreaDistribution().empty()) {
          unit_str = population->SkinSurfaceAreaDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->SkinSurfaceAreaDistribution()[0].mean(),
                                                             population->SkinSurfaceAreaDistribution()[0].diviation());
          for (auto& distribution : population->SkinSurfaceAreaDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "m^2";
          }
          patient.SkinSurfaceArea(standard_distribution(gen));
          patient.SkinSurfaceArea()->unit(unit_str);
        }
        if (!population->SleepAmountDistribution().empty()) {
          unit_str = population->SleepAmountDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->SleepAmountDistribution()[0].mean(),
                                                             population->SleepAmountDistribution()[0].diviation());
          for (auto& distribution : population->SleepAmountDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "hr";
          }
          patient.SleepAmount(standard_distribution(gen));
          patient.SleepAmount()->unit(unit_str);
        }
        if (!population->PainSusceptibilityDistribution().empty()) {
          standard_distribution = std::normal_distribution<>(population->PainSusceptibilityDistribution()[0].mean(),
                                                             population->PainSusceptibilityDistribution()[0].diviation());
          for (auto& distribution : population->PainSusceptibilityDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          patient.PainSusceptibility(standard_distribution(gen));
        }
        if (!population->BasalMetabolicRateDistribution().empty()) {
          unit_str = population->BasalMetabolicRateDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->BasalMetabolicRateDistribution()[0].mean(),
                                                             population->BasalMetabolicRateDistribution()[0].diviation());
          for (auto& distribution : population->BasalMetabolicRateDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "kcal/day";
          }
          patient.BasalMetabolicRate(standard_distribution(gen));
          patient.BasalMetabolicRate()->unit(unit_str);
        }
        if (!population->BloodVolumeBaselineDistribution().empty()) {
          unit_str = population->BloodVolumeBaselineDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->BloodVolumeBaselineDistribution()[0].mean(),
                                                             population->BloodVolumeBaselineDistribution()[0].diviation());
          for (auto& distribution : population->BloodVolumeBaselineDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "mL";
          }
          patient.BloodVolumeBaseline(standard_distribution(gen));
          patient.BloodVolumeBaseline()->unit(unit_str);
        }
        if (!population->DiastolicArterialPressureBaselineDistribution().empty()) {
          unit_str = population->DiastolicArterialPressureBaselineDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->DiastolicArterialPressureBaselineDistribution()[0].mean(),
                                                             population->DiastolicArterialPressureBaselineDistribution()[0].diviation());
          for (auto& distribution : population->DiastolicArterialPressureBaselineDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "mmHg";
          }
          patient.DiastolicArterialPressureBaseline(standard_distribution(gen));
          patient.DiastolicArterialPressureBaseline()->unit(unit_str);
        }
        if (!population->HeartRateBaselineDistribution().empty()) {
          unit_str = population->HeartRateBaselineDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->HeartRateBaselineDistribution()[0].mean(),
                                                             population->HeartRateBaselineDistribution()[0].diviation());
          for (auto& distribution : population->HeartRateBaselineDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "1/min";
          }
          patient.HeartRateBaseline(standard_distribution(gen));
          patient.HeartRateBaseline()->unit(unit_str);
        }

        if (!population->RespirationRateBaselineDistribution().empty()) {
          unit_str = population->RespirationRateBaselineDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->RespirationRateBaselineDistribution()[0].mean(),
                                                             population->RespirationRateBaselineDistribution()[0].diviation());
          for (auto& distribution : population->RespirationRateBaselineDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "1/min";
          }
          patient.RespirationRateBaseline(standard_distribution(gen));
          patient.RespirationRateBaseline()->unit(unit_str);
        }
        if (!population->SystolicArterialPressureBaselineDistribution().empty()) {
          unit_str = population->SystolicArterialPressureBaselineDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->SystolicArterialPressureBaselineDistribution()[0].mean(),
                                                             population->SystolicArterialPressureBaselineDistribution()[0].diviation());
          for (auto& distribution : population->SystolicArterialPressureBaselineDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "mmHg";
          }
          patient.SystolicArterialPressureBaseline(standard_distribution(gen));
          patient.SystolicArterialPressureBaseline()->unit(unit_str);
        }
        if (!population->HeartRateMaximumDistribution().empty()) {
          unit_str = population->HeartRateMaximumDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->HeartRateMaximumDistribution()[0].mean(),
                                                             population->HeartRateMaximumDistribution()[0].diviation());
          for (auto& distribution : population->HeartRateMaximumDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "1/min";
          }
          patient.HeartRateMaximum(standard_distribution(gen));
          patient.HeartRateMaximum()->unit(unit_str);
        }
        if (!population->HeartRateMinimumDistribution().empty()) {
          unit_str = population->HeartRateMinimumDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->HeartRateMinimumDistribution()[0].mean(),
                                                             population->HeartRateMinimumDistribution()[0].diviation());
          for (auto& distribution : population->HeartRateMinimumDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "1/min";
          }
          patient.HeartRateMinimum(standard_distribution(gen));
          patient.HeartRateMinimum()->unit(unit_str);
        }
        if (!population->FunctionalResidualCapacityDistribution().empty()) {
          unit_str = population->FunctionalResidualCapacityDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->FunctionalResidualCapacityDistribution()[0].mean(),
                                                             population->FunctionalResidualCapacityDistribution()[0].diviation());
          for (auto& distribution : population->FunctionalResidualCapacityDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "L";
          }
          patient.FunctionalResidualCapacity(standard_distribution(gen));
          patient.FunctionalResidualCapacity()->unit(unit_str);
        }
        if (!population->ResidualVolumeDistribution().empty()) {
          unit_str = population->ResidualVolumeDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->ResidualVolumeDistribution()[0].mean(),
                                                             population->ResidualVolumeDistribution()[0].diviation());
          for (auto& distribution : population->ResidualVolumeDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "L";
          }
          patient.ResidualVolume(standard_distribution(gen));
          patient.ResidualVolume()->unit(unit_str);
        }
        if (!population->TotalLungCapacityDistribution().empty()) {
          unit_str = population->TotalLungCapacityDistribution()[0].unit();
          standard_distribution = std::normal_distribution<>(population->TotalLungCapacityDistribution()[0].mean(),
                                                             population->TotalLungCapacityDistribution()[0].diviation());
          for (auto& distribution : population->TotalLungCapacityDistribution()) {
            if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
              unit_str = distribution.unit();
              standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
            }
          }
          if (unit_str.empty()) {
            unit_str = "L";
          }
          patient.TotalLungCapacity(standard_distribution(gen));
          patient.TotalLungCapacity()->unit(unit_str);
        }

        xml_schema::namespace_infomap info;
        info[""].name = "uri:/mil/tatrc/physiology/datamodel";
        info[""].schema = "BioGearsDataModel.xsd";

#if _WIN32
        _mkdir("patients");
        _mkdir((std::string("patients/") + population_pool_dir).c_str());
#else
        mkdir("patients", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        mkdir(std::string("patients/") + population_pool_dir).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

        try {
          std::string out_file = std::string("patients/") + population_pool_dir.c_str() + "/" + patient.Name() + ".xml";
          std::ofstream file;
          file.open(out_file.c_str());
          if (file.is_open()) {
            mil::tatrc::physiology::datamodel::Patient(file, patient, info);
            std::cout << std::string("Saved patients/") + population_pool_dir.c_str() + "/"+ patient.Name() + ".xml"
                    << "\n";
          }
          file.close();

        } catch (std::exception e) {
          std::cout << e.what() << std::endl;
        }
        ++total_population_count;
      }
    } catch (xml_schema::parsing e) {
      std::cerr << "Error opening up " << config_file << "\n";
      std::cerr << e << std::endl;
    }
  }
}
//-------------------------------------------------------------------------------
//!
//!        Top 1,000 US names for girls born in 2019
//!        Used to su give population files some unique names
//!
//-------------------------------------------------------------------------------
std::vector<std::string> generate_girl_names()
{
  return {
    "Olivia", "Emma", "Ava", "Sophia", "Isabella", "Charlotte", "Amelia", "Mia", "Harper", "Evelyn", "Abigail", "Emily", "Ella", "Elizabeth", "Camila",
    "Luna", "Sofia", "Avery", "Mila", "Aria", "Scarlett", "Penelope", "Layla", "Chloe", "Victoria", "Madison", "Eleanor", "Grace", "Nora", "Riley", "Zoey",
    "Hannah", "Hazel", "Lily", "Ellie", "Violet", "Lillian", "Zoe", "Stella", "Aurora", "Natalie", "Emilia", "Everly", "Leah", "Aubrey", "Willow", "Addison",
    "Lucy", "Audrey", "Bella", "Nova", "Brooklyn", "Paisley", "Savannah", "Claire", "Skylar", "Isla", "Genesis", "Naomi", "Elena", "Caroline", "Eliana",
    "Anna", "Maya", "Valentina", "Ruby", "Kennedy", "Ivy", "Ariana", "Aaliyah", "Cora", "Madelyn", "Alice", "Kinsley", "Hailey", "Gabriella", "Allison",
    "Gianna", "Serenity", "Samantha", "Sarah", "Autumn", "Quinn", "Eva", "Piper", "Sophie", "Sadie", "Delilah", "Josephine", "Nevaeh", "Adeline", "Arya",
    "Emery", "Lydia", "Clara", "Vivian", "Madeline", "Peyton", "Julia", "Rylee", "Brielle", "Reagan", "Natalia", "Jade", "Athena", "Maria", "Leilani",
    "Everleigh", "Liliana", "Melanie", "Mackenzie", "Hadley", "Raelynn", "Kaylee", "Rose", "Arianna", "Isabelle", "Melody", "Eliza", "Lyla", "Katherine", "Aubree",
    "Adalynn", "Kylie", "Faith", "Mary", "Margaret", "Ximena", "Iris", "Alexandra", "Jasmine", "Charlie", "Amaya", "Taylor", "Isabel", "Ashley", "Khloe",
    "Ryleigh", "Alexa", "Amara", "Valeria", "Andrea", "Parker", "Norah", "Eden", "Elliana", "Brianna", "Emersyn", "Valerie", "Anastasia", "Eloise", "Emerson",
    "Cecilia", "Remi", "Josie", "Alina", "Reese", "Bailey", "Lucia", "Adalyn", "Molly", "Ayla", "Sara", "Daisy", "London", "Jordyn", "Esther",
    "Genevieve", "Harmony", "Annabelle", "Alyssa", "Ariel", "Aliyah", "Londyn", "Juliana", "Morgan", "Summer", "Juliette", "Trinity", "Callie", "Sienna", "Blakely",
    "Alaia", "Kayla", "Teagan", "Alaina", "Brynlee", "Finley", "Catalina", "Sloane", "Rachel", "Lilly", "Ember", "Kimberly", "Juniper", "Sydney", "Arabella",
    "Gemma", "Jocelyn", "Freya", "June", "Lauren", "Amy", "Presley", "Georgia", "Journee", "Elise", "Rosalie", "Ada", "Laila", "Brooke", "Diana",
    "Olive", "River", "Payton", "Ariella", "Daniela", "Raegan", "Alayna", "Gracie", "Mya", "Blake", "Noelle", "Ana", "Leila", "Paige", "Lila",
    "Nicole", "Rowan", "Hope", "Ruth", "Alana", "Selena", "Marley", "Kamila", "Alexis", "Mckenzie", "Zara", "Millie", "Magnolia", "Kali", "Kehlani",
    "Catherine", "Maeve", "Adelyn", "Sawyer", "Elsie", "Lola", "Jayla", "Adriana", "Journey", "Vera", "Aspen", "Joanna", "Alivia", "Angela", "Dakota",
    "Camille", "Nyla", "Tessa", "Brooklynn", "Malia", "Makayla", "Rebecca", "Fiona", "Mariana", "Lena", "Julianna", "Vanessa", "Juliet", "Camilla", "Kendall",
    "Harley", "Cali", "Evangeline", "Mariah", "Jane", "Zuri", "Elaina", "Sage", "Amira", "Adaline", "Lia", "Charlee", "Delaney", "Lilah", "Miriam",
    "Angelina", "Mckenna", "Aniyah", "Phoebe", "Michelle", "Thea", "Hayden", "Maggie", "Lucille", "Amiyah", "Annie", "Alexandria", "Myla", "Vivienne", "Kiara",
    "Alani", "Margot", "Adelaide", "Briella", "Brynn", "Saylor", "Destiny", "Amari", "Evelynn", "Haven", "Phoenix", "Izabella", "Kaia", "Lilliana", "Harlow",
    "Alessandra", "Madilyn", "Nina", "Logan", "Adelynn", "Amina", "Kate", "Fatima", "Samara", "Winter", "Giselle", "Evie", "Arielle", "Jessica", "Talia",
    "Leia", "Gabriela", "Gracelyn", "Lexi", "Laura", "Makenzie", "Melissa", "Royalty", "Rylie", "Raelyn", "Gabrielle", "Paris", "Daleyza", "Joy", "Maisie",
    "Oakley", "Ariyah", "Kailani", "Alayah", "Stephanie", "Amora", "Willa", "Gracelynn", "Elle", "Keira", "Tatum", "Veronica", "Milani", "Felicity", "Paislee",
    "Allie", "Nylah", "Ariah", "Cassidy", "Lyric", "Madeleine", "Miracle", "Gwendolyn", "Octavia", "Dahlia", "Heidi", "Celeste", "Remington", "Makenna", "Everlee",
    "Scarlet", "Esmeralda", "Maci", "Lainey", "Jacqueline", "Kira", "Lana", "Brinley", "Demi", "Ophelia", "Lennon", "Reign", "Bristol", "Sabrina", "Alaya",
    "Jennifer", "Kenzie", "Angel", "Luciana", "Anaya", "Hallie", "Ryan", "Camryn", "Kinley", "Daniella", "Lilith", "Blair", "Amanda", "Collins", "Jordan",
    "Maliyah", "Rosemary", "Cataleya", "Kaylani", "Gia", "Alison", "Leighton", "Nadia", "Sutton", "Carolina", "Skye", "Alicia", "Regina", "Viviana", "Yaretzi",
    "Heaven", "Serena", "Raven", "Emely", "Carmen", "Wren", "Helen", "Charleigh", "Danielle", "Daphne", "Esme", "Nayeli", "Maddison", "Sarai", "Dylan",
    "Frances", "Elisa", "Mabel", "Skyler", "Jenna", "Emelia", "Kaitlyn", "Miranda", "Marlee", "Matilda", "Selah", "Jolene", "Wynter", "Hattie", "Bianca",
    "Haley", "Lorelei", "Mira", "Braelynn", "Annalise", "Madelynn", "Katie", "Palmer", "Aylin", "Elliott", "Kyla", "Rory", "Avianna", "Liana", "Shiloh",
    "Kalani", "Jada", "Kelsey", "Elianna", "Jimena", "Kora", "Kamryn", "Ainsley", "Averie", "Kensley", "Helena", "Holly", "Emory", "Macie", "Amber",
    "Zariah", "Erin", "Eve", "Kathryn", "Renata", "Kayleigh", "Emmy", "Celine", "Francesca", "Fernanda", "April", "Shelby", "Poppy", "Colette", "Meadow",
    "Nia", "Sierra", "Cheyenne", "Edith", "Oaklynn", "Kennedi", "Abby", "Danna", "Jazlyn", "Alessia", "Mikayla", "Alondra", "Addilyn", "Leona", "Mckinley",
    "Carter", "Maren", "Sylvia", "Alejandra", "Ariya", "Astrid", "Adrianna", "Charli", "Imani", "Maryam", "Christina", "Stevie", "Maia", "Adelina", "Dream",
    "Aisha", "Alanna", "Itzel", "Azalea", "Katelyn", "Kylee", "Leslie", "Madilynn", "Myra", "Virginia", "Remy", "Hanna", "Aleah", "Jaliyah", "Antonella",
    "Aviana", "Cameron", "Chelsea", "Cecelia", "Alia", "Mae", "Cadence", "Emberly", "Charley", "Janelle", "Mallory", "Kaliyah", "Elaine", "Gloria", "Jayleen",
    "Lorelai", "Malaysia", "Bethany", "Briana", "Beatrice", "Dorothy", "Rosie", "Jemma", "Noa", "Carly", "Mariam", "Anne", "Karina", "Emmalyn", "Ivory",
    "Ivanna", "Jamie", "Kara", "Aitana", "Jayda", "Justice", "Meredith", "Briar", "Skyla", "Khaleesi", "Dayana", "Julieta", "Katalina", "Kendra", "Oaklyn",
    "Ashlyn", "Armani", "Jazmin", "Kyra", "Angelica", "Zahra", "Dallas", "Johanna", "Elliot", "Macy", "Monroe", "Kimber", "Henley", "Ari", "Karsyn",
    "Lyanna", "Lilian", "Amalia", "Nola", "Dior", "Aleena", "Megan", "Michaela", "Amirah", "Cassandra", "Melany", "Legacy", "Reyna", "Alma", "Emmie",
    "Melina", "Siena", "Priscilla", "Ashlynn", "Savanna", "Sloan", "Tiana", "Aubrie", "Coraline", "Reina", "Allyson", "Kaydence", "Sasha", "Julie", "Alexia",
    "Irene", "Marilyn", "Greta", "Braelyn", "Emerie", "Lylah", "Nalani", "Monica", "Aileen", "Lauryn", "Anahi", "Aurelia", "Kassidy", "Rayna", "Romina",
    "Lillie", "Marie", "Rosa", "Saige", "Bonnie", "Kelly", "Xiomara", "Annabella", "Avah", "Lacey", "Anya", "Liberty", "Karen", "Mercy", "Zelda",
    "Baylee", "Chaya", "Kenna", "Roselyn", "Liv", "Mara", "Ensley", "Malani", "Malaya", "Hadassah", "Lyra", "Adley", "Galilea", "Jaylah", "Karla",
    "Nala", "Opal", "Aliza", "Milena", "Ailani", "Louisa", "Mina", "Kairi", "Clementine", "Louise", "Maleah", "Janiyah", "Marina", "Anika", "Julissa",
    "Bailee", "Hayley", "Jessie", "Laney", "Eileen", "Faye", "Kynlee", "Tiffany", "Lara", "Angie", "Joelle", "Rhea", "Calliope", "Jazmine", "Amani",
    "Haylee", "Aliana", "Leyla", "Jolie", "Kinslee", "Ryann", "Simone", "Milan", "Lennox", "Treasure", "Alora", "Ellis", "Rebekah", "Mikaela", "Lina",
    "Harmoni", "Yareli", "Giuliana", "Lea", "Harlee", "Elyse", "Frida", "Blaire", "Aya", "Laurel", "Meghan", "Pearl", "Zaylee", "Alena", "Holland",
    "Bria", "Rayne", "Bridget", "Zariyah", "Kori", "Frankie", "Clarissa", "Brylee", "Davina", "Rivka", "Cynthia", "Zaria", "Madalyn", "Paula", "Salem",
    "Amelie", "Madisyn", "Vienna", "Haisley", "Ainhoa", "Journi", "Karter", "Oaklee", "Livia", "Miley", "Adele", "Amaia", "Yara", "Averi", "Emmeline",
    "Kyleigh", "Princess", "Penny", "Sariyah", "Amayah", "Crystal", "Keyla", "Lilyana", "Linda", "Aniya", "Marianna", "Alaiya", "Noemi", "Chanel", "Estella",
    "Isabela", "Jillian", "Kallie", "Ellianna", "Elsa", "Itzayana", "Zora", "Estelle", "Chana", "Raina", "Royal", "Sunny", "Estrella", "Martha", "Ellen",
    "Kailey", "Maxine", "Clare", "Teresa", "Annika", "Kamilah", "Azariah", "Della", "Addyson", "Kai", "Lilianna", "Tinsley", "Yaritza", "Navy", "Winnie",
    "Andi", "Kamiyah", "Waverly", "Sky", "Amaris", "Ramona", "Saoirse", "Hana", "Judith", "Halle", "Laylah", "Novalee", "Jaycee", "Zaniyah", "Alianna",
    "Paulina", "Jayde", "Thalia", "Giovanna", "Gwen", "Iliana", "Elora", "Ezra", "Kaylie", "Braylee", "Mavis", "Ellison", "Margo", "Mylah", "Paisleigh",
    "Analia", "August", "Brittany", "Kaisley", "Belen", "Promise", "Amiya", "Dalary", "Veda", "Alisson", "Keilani", "Oakleigh", "Guadalupe", "Leanna", "Rosalyn",
    "Selene", "Theodora", "Kamari", "Anais", "Elodie", "Celia", "Dani", "Hunter", "Indie", "Kenia", "Nellie", "Belle", "Kataleya", "Lexie", "Miah",
    "Rylan", "Sylvie", "Valery", "Addilynn", "Dulce", "Marissa", "Meilani", "Natasha", "Jaylee", "Kimora", "Raquel", "Scarlette", "Aliya", "Nataly", "Whitney",
    "Corinne", "Denver", "Nathalie", "Kiera", "Milana", "Vada", "Violeta", "Luz", "Addisyn", "Casey", "Deborah", "Tori", "Zainab", "Erika", "Jenesis",
    "Avalynn", "Nancy", "Emmalynn", "Hadlee", "Heavenly", "Aubrielle", "Elisabeth", "Salma", "Adalee", "Landry", "Malayah", "Novah", "Egypt", "Ayleen", "Blessing",
    "Elina", "Joyce", "Myah", "Zoie", "Christine", "Jaelynn", "Persephone", "Chandler", "Emmaline", "Paloma", "Harleigh", "Noor", "Paola", "India", "Madalynn",
    "Rosalee", "Florence", "Maliah", "Flora", "Luella", "Patricia", "Whitley", "Carolyn", "Kathleen", "Keily", "Kiana", "Tenley", "Alyson", "Barbara", "Dana",
    "Yasmin", "Bexley", "Micah", "Tatiana", "Arden", "Aubriella", "Lindsey", "Emani", "Hailee", "Lisa", "Sevyn", "Fallon", "Magdalena", "Tinley", "Halo",
    "Lailah", "Arlette", "Ansley", "Esperanza", "Cleo", "Aila", "Emerald", "Jaelyn", "Karlee", "Kaya", "Ingrid", "Jewel", "Emilee", "Giana", "Paityn",
    "Zola", "Amoura", "Renee", "Ann", "Berkley", "Harriet", "Queen", "Sariah", "Beatrix", "Sandra", "Alannah", "Austyn", "Freyja", "Kaylin", "Samira",
    "Taliyah", "Hadleigh", "Kaiya", "Robin", "Luisa", "Zendaya", "Ariadne", "Dixie"
  };
}
//-------------------------------------------------------------------------------
//!
//!        Top 1,000 US names for boy's born in 2019
//!        Used to su give population files some unique names
//!

std::vector<std::string> generate_boy_names()
{
  return {
    "Liam", "Noah", "Oliver", "William", "Elijah", "James", "Benjamin", "Lucas", "Mason", "Ethan", "Alexander", "Henry", "Jacob", "Michael", "Daniel",
    "Logan", "Jackson", "Sebastian", "Jack", "Aiden", "Owen", "Samuel", "Matthew", "Joseph", "Levi", "Mateo", "David", "John", "Wyatt", "Carter",
    "Julian", "Luke", "Grayson", "Isaac", "Jayden", "Theodore", "Gabriel", "Anthony", "Dylan", "Leo", "Lincoln", "Jaxon", "Asher", "Christopher", "Josiah",
    "Andrew", "Thomas", "Joshua", "Ezra", "Hudson", "Charles", "Caleb", "Isaiah", "Ryan", "Nathan", "Adrian", "Christian", "Maverick", "Colton", "Elias",
    "Aaron", "Eli", "Landon", "Jonathan", "Nolan", "Hunter", "Cameron", "Connor", "Santiago", "Jeremiah", "Ezekiel", "Angel", "Roman", "Easton", "Miles",
    "Robert", "Jameson", "Nicholas", "Greyson", "Cooper", "Ian", "Carson", "Axel", "Jaxson", "Dominic", "Leonardo", "Luca", "Austin", "Jordan", "Adam",
    "Xavier", "Jose", "Jace", "Everett", "Declan", "Evan", "Kayden", "Parker", "Wesley", "Kai", "Brayden", "Bryson", "Weston", "Jason", "Emmett",
    "Sawyer", "Silas", "Bennett", "Brooks", "Micah", "Damian", "Harrison", "Waylon", "Ayden", "Vincent", "Ryder", "Kingston", "Rowan", "George", "Luis",
    "Chase", "Cole", "Nathaniel", "Zachary", "Ashton", "Braxton", "Gavin", "Tyler", "Diego", "Bentley", "Amir", "Beau", "Gael", "Carlos", "Ryker",
    "Jasper", "Max", "Juan", "Ivan", "Brandon", "Jonah", "Giovanni", "Kaiden", "Myles", "Calvin", "Lorenzo", "Maxwell", "Jayce", "Kevin", "Legend",
    "Tristan", "Jesus", "Jude", "Zion", "Justin", "Maddox", "Abel", "King", "Camden", "Elliott", "Malachi", "Milo", "Emmanuel", "Karter", "Rhett",
    "Alex", "August", "River", "Xander", "Antonio", "Brody", "Finn", "Elliot", "Dean", "Emiliano", "Eric", "Miguel", "Arthur", "Matteo", "Graham",
    "Alan", "Nicolas", "Blake", "Thiago", "Adriel", "Victor", "Joel", "Timothy", "Hayden", "Judah", "Abraham", "Edward", "Messiah", "Zayden", "Theo",
    "Tucker", "Grant", "Richard", "Alejandro", "Steven", "Jesse", "Dawson", "Bryce", "Avery", "Oscar", "Patrick", "Archer", "Barrett", "Leon", "Colt",
    "Charlie", "Peter", "Kaleb", "Lukas", "Beckett", "Jeremy", "Preston", "Enzo", "Luka", "Andres", "Marcus", "Felix", "Mark", "Ace", "Brantley",
    "Atlas", "Remington", "Maximus", "Matias", "Walker", "Kyrie", "Griffin", "Kenneth", "Israel", "Javier", "Kyler", "Jax", "Amari", "Zane", "Emilio",
    "Knox", "Adonis", "Aidan", "Kaden", "Paul", "Omar", "Brian", "Louis", "Caden", "Maximiliano", "Holden", "Paxton", "Nash", "Bradley", "Bryan",
    "Simon", "Phoenix", "Lane", "Josue", "Colin", "Rafael", "Kyle", "Riley", "Jorge", "Beckham", "Cayden", "Jaden", "Emerson", "Ronan", "Karson",
    "Arlo", "Tobias", "Brady", "Clayton", "Francisco", "Zander", "Erick", "Walter", "Daxton", "Cash", "Martin", "Damien", "Dallas", "Cody", "Chance",
    "Jensen", "Finley", "Jett", "Corbin", "Kash", "Reid", "Kameron", "Andre", "Gunner", "Jake", "Hayes", "Manuel", "Prince", "Bodhi", "Cohen",
    "Sean", "Khalil", "Hendrix", "Derek", "Cristian", "Cruz", "Kairo", "Dante", "Atticus", "Killian", "Stephen", "Orion", "Malakai", "Ali", "Eduardo",
    "Fernando", "Anderson", "Angelo", "Spencer", "Gideon", "Mario", "Titus", "Travis", "Rylan", "Kayson", "Ricardo", "Tanner", "Malcolm", "Raymond", "Odin",
    "Cesar", "Lennox", "Joaquin", "Kane", "Wade", "Muhammad", "Iker", "Jaylen", "Crew", "Zayn", "Hector", "Ellis", "Leonel", "Cairo", "Garrett",
    "Romeo", "Dakota", "Edwin", "Warren", "Julius", "Major", "Donovan", "Caiden", "Tyson", "Nico", "Sergio", "Nasir", "Rory", "Devin", "Jaiden",
    "Jared", "Kason", "Malik", "Jeffrey", "Ismael", "Elian", "Marshall", "Lawson", "Desmond", "Winston", "Nehemiah", "Ari", "Conner", "Jay", "Kade",
    "Andy", "Johnny", "Jayceon", "Marco", "Seth", "Ibrahim", "Raiden", "Collin", "Edgar", "Erik", "Troy", "Clark", "Jaxton", "Johnathan", "Gregory",
    "Russell", "Royce", "Fabian", "Ezequiel", "Noel", "Pablo", "Cade", "Pedro", "Sullivan", "Trevor", "Reed", "Quinn", "Frank", "Harvey", "Princeton",
    "Zayne", "Matthias", "Conor", "Sterling", "Dax", "Grady", "Cyrus", "Gage", "Leland", "Solomon", "Emanuel", "Niko", "Ruben", "Kasen", "Mathias",
    "Kashton", "Franklin", "Remy", "Shane", "Kendrick", "Shawn", "Otto", "Armani", "Keegan", "Finnegan", "Memphis", "Bowen", "Dominick", "Kolton", "Jamison",
    "Allen", "Philip", "Tate", "Peyton", "Jase", "Oakley", "Rhys", "Kyson", "Adan", "Esteban", "Dalton", "Gianni", "Callum", "Sage", "Alexis",
    "Milan", "Moises", "Jonas", "Uriel", "Colson", "Marcos", "Zaiden", "Hank", "Damon", "Hugo", "Ronin", "Royal", "Kamden", "Dexter", "Luciano",
    "Alonzo", "Augustus", "Kamari", "Eden", "Roberto", "Baker", "Bruce", "Kian", "Albert", "Frederick", "Mohamed", "Abram", "Omari", "Porter", "Enrique",
    "Alijah", "Francis", "Leonidas", "Zachariah", "Landen", "Wilder", "Apollo", "Santino", "Tatum", "Pierce", "Forrest", "Corey", "Derrick", "Isaias", "Kaison",
    "Kieran", "Arjun", "Gunnar", "Rocco", "Emmitt", "Abdiel", "Braylen", "Maximilian", "Skyler", "Phillip", "Benson", "Cannon", "Deacon", "Dorian", "Asa",
    "Moses", "Ayaan", "Jayson", "Raul", "Briggs", "Armando", "Nikolai", "Cassius", "Drew", "Rodrigo", "Raphael", "Danny", "Conrad", "Moshe", "Zyaire",
    "Julio", "Casey", "Ronald", "Scott", "Callan", "Roland", "Saul", "Jalen", "Brycen", "Ryland", "Lawrence", "Davis", "Rowen", "Zain", "Ermias",
    "Jaime", "Duke", "Stetson", "Alec", "Yusuf", "Case", "Trenton", "Callen", "Ariel", "Jasiah", "Soren", "Dennis", "Donald", "Keith", "Izaiah",
    "Lewis", "Kylan", "Kobe", "Makai", "Rayan", "Ford", "Zaire", "Landyn", "Roy", "Bo", "Chris", "Jamari", "Ares", "Mohammad", "Darius",
    "Drake", "Tripp", "Marcelo", "Samson", "Dustin", "Layton", "Gerardo", "Johan", "Kaysen", "Keaton", "Reece", "Chandler", "Lucca", "Mack", "Baylor",
    "Kannon", "Marvin", "Huxley", "Nixon", "Tony", "Cason", "Mauricio", "Quentin", "Edison", "Quincy", "Ahmed", "Finnley", "Justice", "Taylor", "Gustavo",
    "Brock", "Ahmad", "Kyree", "Arturo", "Nikolas", "Boston", "Sincere", "Alessandro", "Braylon", "Colby", "Leonard", "Ridge", "Trey", "Aden", "Leandro",
    "Sam", "Uriah", "Ty", "Sylas", "Axton", "Issac", "Fletcher", "Julien", "Wells", "Alden", "Vihaan", "Jamir", "Valentino", "Shepherd", "Keanu",
    "Hezekiah", "Lionel", "Kohen", "Zaid", "Alberto", "Neil", "Denver", "Aarav", "Brendan", "Dillon", "Koda", "Sutton", "Kingsley", "Sonny", "Alfredo",
    "Wilson", "Harry", "Jaziel", "Salvador", "Cullen", "Hamza", "Dariel", "Rex", "Zeke", "Mohammed", "Nelson", "Boone", "Ricky", "Santana", "Cayson",
    "Lance", "Raylan", "Lucian", "Eliel", "Alvin", "Jagger", "Braden", "Curtis", "Mathew", "Jimmy", "Kareem", "Archie", "Amos", "Quinton", "Yosef",
    "Bodie", "Jerry", "Langston", "Axl", "Stanley", "Clay", "Douglas", "Layne", "Titan", "Tomas", "Houston", "Darren", "Lachlan", "Kase", "Korbin",
    "Leighton", "Joziah", "Samir", "Watson", "Colten", "Roger", "Shiloh", "Tommy", "Mitchell", "Azariah", "Noe", "Talon", "Deandre", "Lochlan", "Joe",
    "Carmelo", "Otis", "Randy", "Byron", "Chaim", "Lennon", "Devon", "Nathanael", "Bruno", "Aryan", "Flynn", "Vicente", "Brixton", "Kyro", "Brennan",
    "Casen", "Kenzo", "Orlando", "Castiel", "Rayden", "Ben", "Grey", "Jedidiah", "Tadeo", "Morgan", "Augustine", "Mekhi", "Abdullah", "Ramon", "Saint",
    "Emery", "Maurice", "Jefferson", "Maximo", "Koa", "Ray", "Jamie", "Eddie", "Guillermo", "Onyx", "Thaddeus", "Wayne", "Hassan", "Alonso", "Dash",
    "Elisha", "Jaxxon", "Rohan", "Carl", "Kelvin", "Jon", "Larry", "Reese", "Aldo", "Marcel", "Melvin", "Yousef", "Aron", "Kace", "Vincenzo",
    "Kellan", "Miller", "Jakob", "Reign", "Kellen", "Kristopher", "Ernesto", "Briar", "Gary", "Trace", "Joey", "Clyde", "Enoch", "Jaxx", "Crosby",
    "Magnus", "Fisher", "Jadiel", "Bronson", "Eugene", "Lee", "Brecken", "Atreus", "Madden", "Khari", "Caspian", "Ishaan", "Kristian", "Westley", "Hugh",
    "Kamryn", "Musa", "Rey", "Thatcher", "Alfred", "Emory", "Kye", "Reyansh", "Yahir", "Cain", "Mordechai", "Zayd", "Demetrius", "Harley", "Felipe",
    "Louie", "Branson", "Graysen", "Allan", "Kole", "Harold", "Alvaro", "Harlan", "Amias", "Brett", "Khalid", "Misael", "Westin", "Zechariah", "Aydin",
    "Kaiser", "Lian", "Bryant", "Junior", "Legacy", "Ulises", "Bellamy", "Brayan", "Kody", "Ledger", "Eliseo", "Gordon", "London", "Rocky", "Valentin",
    "Terry", "Damari", "Trent", "Bentlee", "Canaan", "Gatlin", "Kiaan", "Franco", "Eithan", "Idris", "Krew", "Yehuda", "Marlon", "Rodney", "Creed",
    "Salvatore", "Stefan", "Tristen", "Adrien", "Jamal", "Judson", "Camilo", "Kenny", "Nova", "Robin", "Rudy", "Van", "Bjorn", "Brodie", "Mac",
    "Jacoby", "Sekani", "Vivaan", "Blaine", "Ira", "Ameer", "Dominik", "Alaric", "Dane", "Jeremias", "Kyng", "Reginald", "Bobby", "Kabir", "Jairo",
    "Alexzander", "Benicio", "Vance", "Wallace", "Zavier", "Billy", "Callahan", "Dakari", "Gerald", "Turner", "Bear", "Jabari", "Cory", "Fox", "Harlem",
    "Jakari", "Jeffery", "Maxton", "Ronnie", "Yisroel", "Zakai", "Bridger", "Remi", "Arian", "Blaze", "Forest", "Genesis", "Jerome", "Reuben", "Wesson",
    "Anders", "Banks", "Calum", "Dayton", "Kylen", "Dangelo", "Emir", "Malakhi", "Salem", "Blaise", "Tru", "Boden", "Kolten", "Kylo", "Aries",
    "Henrik", "Kalel", "Landry", "Marcellus", "Zahir", "Lyle", "Dario", "Rene", "Terrance", "Xzavier", "Alfonso", "Darian", "Kylian", "Maison", "Foster",
    "Keenan", "Yahya", "Heath", "Javion", "Jericho", "Aziel", "Darwin", "Marquis", "Mylo", "Ambrose", "Anakin", "Jordy", "Juelz", "Toby", "Yael",
    "Azrael", "Brentley", "Tristian", "Bode", "Jovanni", "Santos", "Alistair", "Braydon", "Kamdyn", "Marc", "Mayson", "Niklaus", "Simeon", "Colter", "Davion",
    "Leroy", "Ayan", "Dilan", "Ephraim", "Anson", "Merrick", "Wes", "Will", "Jaxen", "Maxim", "Howard", "Jad", "Jesiah", "Ignacio", "Zyon",
    "Ahmir", "Jair", "Mustafa", "Jermaine", "Yadiel", "Aayan", "Dhruv", "Seven", "Stone", "Rome"
  };
}
//-------------------------------------------------------------------------------
} // namespace biogears
