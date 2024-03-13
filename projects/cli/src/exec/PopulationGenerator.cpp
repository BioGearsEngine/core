//**********************************************************************************
// Copyright 2015 Applied Research Associates, Inc.
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use
// this file except in compliance with the License.You may obtain a copy of the License
// at :
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software distributed under
// the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.See the License for the
// specific language governing permissions and limitations under the License.
//**************************************************************************************

#include "PopulationGenerator.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <algorithm>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include <biogears/schema/cdm/Generators.hxx>
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

namespace biogears {

//-------------------------------------------------------------------------------
// Class Scenario Template
struct PopulationProfile {
public:
  std::string name;
  size_t count;
  std::set<std::string> tags;
};

struct DistributionCollection {
  std::map<std::set<std::string>, std::normal_distribution<double>> normals;
  std::map<std::set<std::string>, std::pair<double, double>> limits_of_normals;

  std::map<std::set<std::string>, std::discrete_distribution<int>> weighted_discretes;
  std::map<std::set<std::string>, CDM::WeightedDistributionData> weighted_descrete_values;

  std::map<std::set<std::string>, std::uniform_real_distribution<double>> continuous_uniforms;
  std::map<std::set<std::string>, std::uniform_int_distribution<int>> discrete_uniforms;

  std::map<std::set<std::string>, std::bernoulli_distribution> bernoullis;

  std::map<std::set<std::string>, int> sequences;

  std::map<std::set<std::string>, std::vector<std::string>> cycles;
  std::map<std::set<std::string>, int> cycles_positions;
};

static const std::string fallback = "default";

std::set<std::string> string_to_set(std::string input)
{
  std::stringstream input_strean(input);
  std::string property;
  std::set<std::string> result;
  while (getline(input_strean, property, ';'))
    result.insert(property);
  return result;
}

DistributionCollection process_distribution_collection(CDM::DistributionCollectionData const& collection)
{
  DistributionCollection result;
  for (auto distribution : collection.NormalDistribution()) {
    result.normals[string_to_set(distribution)] = std::normal_distribution(distribution.mean(), distribution.deviation());
    auto a = distribution.min().present() ? distribution.min().get() : std::numeric_limits<double>::min();
    auto b = distribution.max().present() ? distribution.max().get() : std::numeric_limits<double>::max();
    result.limits_of_normals[string_to_set(distribution)] = std::pair<double, double> { a, b };
  }
  for (auto distribution : collection.WeightedDistribution()) {
    std::vector<double> weights;
    for (auto& weight : distribution.Value()) {
      weights.push_back(weight.weight());
    }
    result.weighted_discretes[string_to_set(distribution.name())] = std::discrete_distribution<int>(weights.begin(), weights.end());
    result.weighted_descrete_values[string_to_set(distribution.name())] = distribution;
  }
  for (auto distribution : collection.ContinuousUniformDistribution()) {
    result.continuous_uniforms[string_to_set(distribution)] = std::uniform_real_distribution(distribution.a(), distribution.b());
  }
  for (auto distribution : collection.ContinuousUniformDistribution()) {
    result.discrete_uniforms[string_to_set(distribution)] = std::uniform_int_distribution<int>(distribution.a(), distribution.b());
  }
  for (auto distribution : collection.BernoulliDistribution()) {
    result.bernoullis[string_to_set(distribution.name())] = std::bernoulli_distribution(distribution.p());
  }
  for (auto sequence : collection.Sequence()) {
    result.sequences[string_to_set(sequence)] = sequence.start();
  }

  for (auto cycle : collection.SelectionCycle()) {
    result.cycles_positions[string_to_set(cycle.name())] = 0;

    std::vector<std::string> values;
    for (auto v : cycle.Value()) {
      values.emplace_back(v);
    }
    result.cycles[string_to_set(cycle.name())] = values;
  }

  return result;
}

static const std::string Heterogametic_Sex = "Heterogametic-Sex";
static const std::string Age = "Age";
static const std::string Weight = "Weight";
static const std::string Height = "Height";
static const std::string BodyFatFraction = "BodyFatFraction";
static const std::string MaxWorkRate = "MaxWorkRate";
static const std::string BloodTypeABO = "BloodTypeABO";
static const std::string BloodTypeRh = "BloodTypeRh";
static const std::string AlveoliSurfaceArea = "AlveoliSurfaceArea";
static const std::string Hyperhidrosis = "Hyperhidrosis";
static const std::string RightLungRatio = "RightLungRatio";
static const std::string SkinSurfaceArea = "SkinSurfaceArea";
static const std::string SleepAmount = "SleepAmount";
static const std::string PainSusceptibility = "PainSusceptibility";
static const std::string BasalMetabolicRate = "BasalMetabolicRate";
static const std::string BloodVolumeBaseline = "BloodVolumeBaseline";
static const std::string DiastolicArterialPressureBaseline = "DiastolicArterialPressureBaseline";
static const std::string HeartRateBaseline = "HeartRateBaseline";
static const std::string RespirationRateBaseline = "RespirationRateBaseline";
static const std::string SystolicArterialPressureBaseline = "SystolicArterialPressureBaseline";
static const std::string HeartRateMaximum = "HeartRateMaximum";
static const std::string HeartRateMinimum = "HeartRateMinimum";
static const std::string FunctionalResidualCapacity = "FunctionalResidualCapacity";
static const std::string ResidualVolume = "ResidualVolume";
static const std::string TotalLungCapacity = "TotalLungCapacity";

static std::map<std::string const, DistributionCollection> translate_distributions(CDM::PopulationGeneratorData::Sampling_type& samples)
{

  std::map<std::string const, DistributionCollection> distributions;
  if (samples.Heterogametic_SexDistribution().present()) {
    distributions[Heterogametic_Sex] = process_distribution_collection(samples.Heterogametic_SexDistribution().get());
  }
  if (samples.AgeDistribution().present()) {
    distributions[Age] = process_distribution_collection(samples.AgeDistribution().get());
  }
  if (samples.WeightDistribution().present()) {
    distributions[Weight] = process_distribution_collection(samples.WeightDistribution().get());
  }
  if (samples.HeightDistribution().present()) {
    distributions[Height] = process_distribution_collection(samples.HeightDistribution().get());
  }
  if (samples.BodyFatFractionDistribution().present()) {
    distributions[BodyFatFraction] = process_distribution_collection(samples.BodyFatFractionDistribution().get());
  }
  if (samples.MaxWorkRateDistribution().present()) {
    distributions[MaxWorkRate] = process_distribution_collection(samples.MaxWorkRateDistribution().get());
  }
  if (samples.BloodTypeABODistribution().present()) {
    distributions[BloodTypeABO] = process_distribution_collection(samples.BloodTypeABODistribution().get());
  }
  if (samples.BloodTypeRhDistribution().present()) {
    distributions[BloodTypeRh] = process_distribution_collection(samples.BloodTypeRhDistribution().get());
  }
  if (samples.AlveoliSurfaceAreaDistribution().present()) {
    distributions[AlveoliSurfaceArea] = process_distribution_collection(samples.AlveoliSurfaceAreaDistribution().get());
  }
  if (samples.HyperhidrosisDistribution().present()) {
    distributions[Hyperhidrosis] = process_distribution_collection(samples.HyperhidrosisDistribution().get());
  }
  if (samples.RightLungRatioDistribution().present()) {
    distributions[RightLungRatio] = process_distribution_collection(samples.RightLungRatioDistribution().get());
  }
  if (samples.SkinSurfaceAreaDistribution().present()) {
    distributions[SkinSurfaceArea] = process_distribution_collection(samples.SkinSurfaceAreaDistribution().get());
  }
  if (samples.SleepAmountDistribution().present()) {
    distributions[SleepAmount] = process_distribution_collection(samples.SleepAmountDistribution().get());
  }
  if (samples.PainSusceptibilityDistribution().present()) {
    distributions[PainSusceptibility] = process_distribution_collection(samples.PainSusceptibilityDistribution().get());
  }
  if (samples.BasalMetabolicRateDistribution().present()) {
    distributions[BasalMetabolicRate] = process_distribution_collection(samples.BasalMetabolicRateDistribution().get());
  }
  if (samples.BloodVolumeBaselineDistribution().present()) {
    distributions[BloodVolumeBaseline] = process_distribution_collection(samples.BloodVolumeBaselineDistribution().get());
  }
  if (samples.DiastolicArterialPressureBaselineDistribution().present()) {
    distributions[DiastolicArterialPressureBaseline] = process_distribution_collection(samples.DiastolicArterialPressureBaselineDistribution().get());
  }
  if (samples.HeartRateBaselineDistribution().present()) {
    distributions[HeartRateBaseline] = process_distribution_collection(samples.HeartRateBaselineDistribution().get());
  }
  if (samples.RespirationRateBaselineDistribution().present()) {
    distributions[RespirationRateBaseline] = process_distribution_collection(samples.RespirationRateBaselineDistribution().get());
  }
  if (samples.SystolicArterialPressureBaselineDistribution().present()) {
    distributions[SystolicArterialPressureBaseline] = process_distribution_collection(samples.SystolicArterialPressureBaselineDistribution().get());
  }
  if (samples.HeartRateMaximumDistribution().present()) {
    distributions[HeartRateMaximum] = process_distribution_collection(samples.HeartRateMaximumDistribution().get());
  }
  if (samples.HeartRateMinimumDistribution().present()) {
    distributions[HeartRateMinimum] = process_distribution_collection(samples.HeartRateMinimumDistribution().get());
  }
  if (samples.FunctionalResidualCapacityDistribution().present()) {
    distributions[FunctionalResidualCapacity] = process_distribution_collection(samples.FunctionalResidualCapacityDistribution().get());
  }
  if (samples.ResidualVolumeDistribution().present()) {
    distributions[ResidualVolume] = process_distribution_collection(samples.ResidualVolumeDistribution().get());
  }
  if (samples.TotalLungCapacityDistribution().present()) {
    distributions[TotalLungCapacity] = process_distribution_collection(samples.TotalLungCapacityDistribution().get());
  }
}
//-------------------------------------------------------------------------------
PopulationGenerator::PopulationGenerator(std::vector<std::string> params)
{
  if (params.size() == 0) {
    _runs.emplace_back("PopulationTemplate.xml");
  } else {
    _runs = params;
  }
}
//-------------------------------------------------------------------------------
//!
//! \brief Iterates through patientFiles, creates a lambda function for each item, and passes those functions to a thread pool
//
void PopulationGenerator::Generate()
{

  std::random_device rd {};
  std::mt19937 gen { rd() };
  std::string unit_str = "";

  auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
  std::string population_directory = std::format("Templates_{:%Y%m%dT%H%M}/", time);

  for (auto& run : _runs) {
    auto const& config_file = run;

    try {
      auto population = CDM::PopulationProfile(config_file);
      std::map<std::string const, DistributionCollection> distributions = translate_distributions(population->Sampling());

      for (auto population : population->Populations().Population()) {
        PopulationProfile profile;
        profile.count = population.count();
        profile.name = population.name();
        for (auto tag : population.Property()) {
          profile.tags.insert(tag);
        }

        for (auto i = 0; i < profile.count; ++i) {
          CDM::PatientData patient;
          //          std::normal_distribution<> standard_distribution { 5, 2 };
          //          std::binomial_distribution<> binomial_distribution { 1, population->Heterogametic_SexDistribution() };
          //          std::uniform_int_distribution<> uniform_distribution;
          //
          //          // This is a really complicated way of saying (
          //          patient.Sex().set(binomial_distribution(gen) ? CDM::enumSex::Female : CDM::enumSex::Male);
          //          if (patient.Sex().get() == CDM::enumSex::Male) {
          //            uniform_distribution = std::uniform_int_distribution<>(0, static_cast<int>(_boy_names.size() - 1));
          //            int name_index = uniform_distribution(gen);
          //
          //            patient.Name(_boy_names[name_index]);
          //            _boy_names[name_index] = std::move(_boy_names.back());
          //            _boy_names.pop_back();
          //          } else {
          //            uniform_distribution = std::uniform_int_distribution<>(0, static_cast<int>(_girl_names.size() - 1));
          //            int name_index = uniform_distribution(gen);
          //            patient.Name(_girl_names[name_index]);
          //            _girl_names[name_index] = std::move(_girl_names.back());
          //            _girl_names.pop_back();
          //          }
          //
          //          if (!population->AgeDistribution().empty()) {
          //            unit_str = population->AgeDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->AgeDistribution()[0].mean(),
          //                                                               population->AgeDistribution()[0].diviation());
          //            for (auto& distribution : population->AgeDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "yr";
          //            }
          //            patient.Age(standard_distribution(gen));
          //            patient.Age()->unit(unit_str);
          //          }
          //
          //          if (!population->WeightDistribution().empty()) {
          //            unit_str = population->WeightDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->WeightDistribution()[0].mean(),
          //                                                               population->WeightDistribution()[0].diviation());
          //            for (auto& distribution : population->WeightDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "kg";
          //            }
          //            patient.Weight(standard_distribution(gen));
          //            patient.Weight()->unit(unit_str);
          //          }
          //          if (!population->HeightDistribution().empty()) {
          //            unit_str = population->HeightDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->HeightDistribution()[0].mean(),
          //                                                               population->HeightDistribution()[0].diviation());
          //            for (auto& distribution : population->HeightDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "cm";
          //            }
          //            patient.Height(standard_distribution(gen));
          //            patient.Height()->unit(unit_str);
          //          }
          //
          //          if (!population->BodyFatFractionDistribution().empty()) {
          //            standard_distribution = std::normal_distribution<>(population->BodyFatFractionDistribution()[0].mean(),
          //                                                               population->BodyFatFractionDistribution()[0].diviation());
          //            for (auto& distribution : population->BodyFatFractionDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //
          //            patient.BodyFatFraction(standard_distribution(gen));
          //            patient.BodyFatFraction()->unit("");
          //          }
          //          if (!population->MaxWorkRateDistribution().empty()) {
          //            unit_str = population->MaxWorkRateDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->MaxWorkRateDistribution()[0].mean(),
          //                                                               population->MaxWorkRateDistribution()[0].diviation());
          //            for (auto& distribution : population->MaxWorkRateDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "W";
          //            }
          //            patient.MaxWorkRate(standard_distribution(gen));
          //            patient.MaxWorkRate()->unit(unit_str);
          //          }
          //          if (!population->BloodTypeABODistribution().empty()) {
          //            std::vector<CDM::DiscreteDistributionRangeData::weight_type> weights;
          //            for (auto weight : population->BloodTypeABODistribution()[0].weight()) {
          //              weights.emplace_back(weight);
          //            }
          //            auto discrete_distribution = std::discrete_distribution<>(weights.begin(), weights.end());
          //            for (auto& distribution : population->BloodTypeABODistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                weights.clear();
          //                for (auto weight : distribution.weight()) {
          //                  weights.emplace_back(weight);
          //                }
          //                discrete_distribution = std::discrete_distribution<>(weights.begin(), weights.end());
          //              }
          //            }
          //            // Note: Very easy for the user to key in more weights then blood types and generate
          //            // Non Viable Enum Values
          //            patient.BloodTypeABO(CDM::enumBloodType::value(discrete_distribution(gen)));
          //          }
          //          if (!population->BloodTypeRhDistribution().empty()) {
          //            binomial_distribution = std::binomial_distribution<>(1, population->BloodTypeRhDistribution()[0].weight());
          //            for (auto& distribution : population->BloodTypeRhDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                binomial_distribution = std::binomial_distribution<>(1, distribution.weight());
          //              }
          //            }
          //            patient.BloodTypeRh(binomial_distribution(gen));
          //          }
          //          if (!population->AlveoliSurfaceAreaDistribution().empty()) {
          //            unit_str = population->AlveoliSurfaceAreaDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->AlveoliSurfaceAreaDistribution()[0].mean(),
          //                                                               population->AlveoliSurfaceAreaDistribution()[0].diviation());
          //            for (auto& distribution : population->AlveoliSurfaceAreaDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "m^2";
          //            }
          //            patient.AlveoliSurfaceArea(standard_distribution(gen));
          //            patient.AlveoliSurfaceArea()->unit(unit_str);
          //          }
          //          if (!population->HyperhidrosisDistribution().empty()) {
          //            standard_distribution = std::normal_distribution<>(population->HyperhidrosisDistribution()[0].mean(),
          //                                                               population->HyperhidrosisDistribution()[0].diviation());
          //            for (auto& distribution : population->HyperhidrosisDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            patient.Hyperhidrosis(standard_distribution(gen));
          //            patient.Hyperhidrosis()->unit(unit_str);
          //          }
          //          if (!population->RightLungRatioDistribution().empty()) {
          //            standard_distribution = std::normal_distribution<>(population->RightLungRatioDistribution()[0].mean(),
          //                                                               population->RightLungRatioDistribution()[0].diviation());
          //            for (auto& distribution : population->RightLungRatioDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            patient.RightLungRatio(standard_distribution(gen));
          //          }
          //          if (!population->SkinSurfaceAreaDistribution().empty()) {
          //            unit_str = population->SkinSurfaceAreaDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->SkinSurfaceAreaDistribution()[0].mean(),
          //                                                               population->SkinSurfaceAreaDistribution()[0].diviation());
          //            for (auto& distribution : population->SkinSurfaceAreaDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "m^2";
          //            }
          //            patient.SkinSurfaceArea(standard_distribution(gen));
          //            patient.SkinSurfaceArea()->unit(unit_str);
          //          }
          //          if (!population->SleepAmountDistribution().empty()) {
          //            unit_str = population->SleepAmountDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->SleepAmountDistribution()[0].mean(),
          //                                                               population->SleepAmountDistribution()[0].diviation());
          //            for (auto& distribution : population->SleepAmountDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "hr";
          //            }
          //            patient.SleepAmount(standard_distribution(gen));
          //            patient.SleepAmount()->unit(unit_str);
          //          }
          //          if (!population->PainSusceptibilityDistribution().empty()) {
          //            standard_distribution = std::normal_distribution<>(population->PainSusceptibilityDistribution()[0].mean(),
          //                                                               population->PainSusceptibilityDistribution()[0].diviation());
          //            for (auto& distribution : population->PainSusceptibilityDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            patient.PainSusceptibility(standard_distribution(gen));
          //          }
          //          if (!population->BasalMetabolicRateDistribution().empty()) {
          //            unit_str = population->BasalMetabolicRateDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->BasalMetabolicRateDistribution()[0].mean(),
          //                                                               population->BasalMetabolicRateDistribution()[0].diviation());
          //            for (auto& distribution : population->BasalMetabolicRateDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "kcal/day";
          //            }
          //            patient.BasalMetabolicRate(standard_distribution(gen));
          //            patient.BasalMetabolicRate()->unit(unit_str);
          //          }
          //          if (!population->BloodVolumeBaselineDistribution().empty()) {
          //            unit_str = population->BloodVolumeBaselineDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->BloodVolumeBaselineDistribution()[0].mean(),
          //                                                               population->BloodVolumeBaselineDistribution()[0].diviation());
          //            for (auto& distribution : population->BloodVolumeBaselineDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "mL";
          //            }
          //            patient.BloodVolumeBaseline(standard_distribution(gen));
          //            patient.BloodVolumeBaseline()->unit(unit_str);
          //          }
          //          if (!population->DiastolicArterialPressureBaselineDistribution().empty()) {
          //            unit_str = population->DiastolicArterialPressureBaselineDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->DiastolicArterialPressureBaselineDistribution()[0].mean(),
          //                                                               population->DiastolicArterialPressureBaselineDistribution()[0].diviation());
          //            for (auto& distribution : population->DiastolicArterialPressureBaselineDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "mmHg";
          //            }
          //            patient.DiastolicArterialPressureBaseline(standard_distribution(gen));
          //            patient.DiastolicArterialPressureBaseline()->unit(unit_str);
          //          }
          //          if (!population->HeartRateBaselineDistribution().empty()) {
          //            unit_str = population->HeartRateBaselineDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->HeartRateBaselineDistribution()[0].mean(),
          //                                                               population->HeartRateBaselineDistribution()[0].diviation());
          //            for (auto& distribution : population->HeartRateBaselineDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "1/min";
          //            }
          //            patient.HeartRateBaseline(standard_distribution(gen));
          //            patient.HeartRateBaseline()->unit(unit_str);
          //          }
          //
          //          if (!population->RespirationRateBaselineDistribution().empty()) {
          //            unit_str = population->RespirationRateBaselineDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->RespirationRateBaselineDistribution()[0].mean(),
          //                                                               population->RespirationRateBaselineDistribution()[0].diviation());
          //            for (auto& distribution : population->RespirationRateBaselineDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "1/min";
          //            }
          //            patient.RespirationRateBaseline(standard_distribution(gen));
          //            patient.RespirationRateBaseline()->unit(unit_str);
          //          }
          //          if (!population->SystolicArterialPressureBaselineDistribution().empty()) {
          //            unit_str = population->SystolicArterialPressureBaselineDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->SystolicArterialPressureBaselineDistribution()[0].mean(),
          //                                                               population->SystolicArterialPressureBaselineDistribution()[0].diviation());
          //            for (auto& distribution : population->SystolicArterialPressureBaselineDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "mmHg";
          //            }
          //            patient.SystolicArterialPressureBaseline(standard_distribution(gen));
          //            patient.SystolicArterialPressureBaseline()->unit(unit_str);
          //          }
          //          if (!population->HeartRateMaximumDistribution().empty()) {
          //            unit_str = population->HeartRateMaximumDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->HeartRateMaximumDistribution()[0].mean(),
          //                                                               population->HeartRateMaximumDistribution()[0].diviation());
          //            for (auto& distribution : population->HeartRateMaximumDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "1/min";
          //            }
          //            patient.HeartRateMaximum(standard_distribution(gen));
          //            patient.HeartRateMaximum()->unit(unit_str);
          //          }
          //          if (!population->HeartRateMinimumDistribution().empty()) {
          //            unit_str = population->HeartRateMinimumDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->HeartRateMinimumDistribution()[0].mean(),
          //                                                               population->HeartRateMinimumDistribution()[0].diviation());
          //            for (auto& distribution : population->HeartRateMinimumDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "1/min";
          //            }
          //            patient.HeartRateMinimum(standard_distribution(gen));
          //            patient.HeartRateMinimum()->unit(unit_str);
          //          }
          //          if (!population->FunctionalResidualCapacityDistribution().empty()) {
          //            unit_str = population->FunctionalResidualCapacityDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->FunctionalResidualCapacityDistribution()[0].mean(),
          //                                                               population->FunctionalResidualCapacityDistribution()[0].diviation());
          //            for (auto& distribution : population->FunctionalResidualCapacityDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "L";
          //            }
          //            patient.FunctionalResidualCapacity(standard_distribution(gen));
          //            patient.FunctionalResidualCapacity()->unit(unit_str);
          //          }
          //          if (!population->ResidualVolumeDistribution().empty()) {
          //            unit_str = population->ResidualVolumeDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->ResidualVolumeDistribution()[0].mean(),
          //                                                               population->ResidualVolumeDistribution()[0].diviation());
          //            for (auto& distribution : population->ResidualVolumeDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "L";
          //            }
          //            patient.ResidualVolume(standard_distribution(gen));
          //            patient.ResidualVolume()->unit(unit_str);
          //          }
          //          if (!population->TotalLungCapacityDistribution().empty()) {
          //            unit_str = population->TotalLungCapacityDistribution()[0].unit();
          //            standard_distribution = std::normal_distribution<>(population->TotalLungCapacityDistribution()[0].mean(),
          //                                                               population->TotalLungCapacityDistribution()[0].diviation());
          //            for (auto& distribution : population->TotalLungCapacityDistribution()) {
          //              if (CDM::enumSex(distribution.group()) == patient.Sex().get()) {
          //                unit_str = distribution.unit();
          //                standard_distribution = std::normal_distribution<>(distribution.mean(), distribution.diviation());
          //              }
          //            }
          //            if (unit_str.empty()) {
          //              unit_str = "L";
          //            }
          //            patient.TotalLungCapacity(standard_distribution(gen));
          //            patient.TotalLungCapacity()->unit(unit_str);
          //          }
          //
          //          xml_schema::namespace_infomap info;
          //          info[""].name = "uri:/mil/tatrc/physiology/datamodel";
          //          info[""].schema = "BioGearsDataModel.xsd";
          //
          // #if _WIN32
          //          _mkdir("patients");
          //          _mkdir((std::string("patients/") + population_pool_dir).c_str());
          // #else
          //          mkdir("patients", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
          //          mkdir((std::string("patients/") + population_pool_dir).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
          // #endif
          //
          //          try {
          //            std::string out_file = std::string("patients/") + population_pool_dir.c_str() + "/" + patient.Name() + ".xml";
          //            std::ofstream file;
          //            file.open(out_file.c_str());
          //            if (file.is_open()) {
          //              mil::tatrc::physiology::datamodel::Patient(file, patient, info);
          //              std::cout << std::string("Saved patients/") + population_pool_dir.c_str() + "/" + patient.Name() + ".xml"
          //                        << "\n";
          //            }
          //            file.close();
          //
          //          } catch (std::exception e) {
          //            std::cout << e.what() << std::endl;
          //          }
          //          ++total_population_count;
        }
      }
    } catch (xml_schema::parsing e) {
      std::cerr << "Error opening up " << config_file << "\n";
      std::cerr << e << std::endl;
    }
  }
}
//-------------------------------------------------------------------------------
} // namespace biogears
