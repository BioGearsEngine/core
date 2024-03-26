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

#include <biogears/io/io-manager.h>
#include <biogears/schema/cdm/Generators.hxx>
#include <biogears/schema/cdm/Patient.hxx>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <strstream>
#include <vector>

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
  std::map<std::set<std::string> const, CDM::NormalDistributionData> normals;
  std::map<std::set<std::string> const, std::normal_distribution<double>> normal_distributions;

  std::map<std::set<std::string> const, std::discrete_distribution<int>> weighted_discrete_distributions;
  std::map<std::set<std::string> const, CDM::WeightedDistributionData> weighted_discretes;

  std::map<std::set<std::string> const, std::uniform_real_distribution<double>> continuous_uniform_distributions;
  std::map<std::set<std::string> const, CDM::ContinuousUniformDistributionData> continuous_uniforms;

  std::map<std::set<std::string> const, std::uniform_int_distribution<int>> discrete_uniform_distributions;
  std::map<std::set<std::string> const, CDM::DiscreteUniformDistributionData> discrete_uniforms;

  std::map<std::set<std::string> const, std::bernoulli_distribution> bernoulli_distributions;
  std::map<std::set<std::string> const, CDM::BernoulliDistributionData> bernoullis;

  std::map<std::set<std::string> const, CDM::SequenceData> sequences;
  std::map<std::set<std::string> const, int> sequence_values;

  std::map<std::set<std::string> const, CDM::SelectionCycleData> cycles;
  std::map<std::set<std::string> const, int> cycles_positions;
};

static const std::string default_key = "__default__";

std::pair<std::set<std::string>, std::string> find_best_match(std::set<std::string> properties, DistributionCollection const& collection)
{
  std::pair<std::set<std::string>, std::string> result = { {}, "" };

  for (auto const& [key, distribution] : collection.normals) {
    if (result.first.size() <= key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.weighted_discretes) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.continuous_uniforms) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.discrete_uniforms) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.bernoullis) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.sequences) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  for (auto const& [key, distribution] : collection.cycles) {
    if (result.first.size() < key.size()
        && (std::includes(properties.begin(), properties.end(), key.begin(), key.end())
            || key.size() == 1 && key.contains(default_key))) {
      result = { key, distribution.unit() };
    }
  }

  return result;
}

std::set<std::string> string_to_set(std::string input)
{
  std::stringstream input_strean(input);
  std::string property;
  std::set<std::string> result, default_set { default_key };
  while (getline(input_strean, property, ';')) {
    std::transform(property.begin(), property.end(), property.begin(), ::tolower);
    result.insert(property);
  }
  return (result.empty()) ? default_set : result;
}

DistributionCollection process_distribution_collection(CDM::DistributionCollectionData const& collection)
{
  DistributionCollection result;
  for (CDM::NormalDistributionData const& distribution : collection.NormalDistribution()) {
    result.normal_distributions[string_to_set(distribution)] = std::normal_distribution(distribution.mean(), distribution.deviation());
    result.normals[string_to_set(distribution)] = distribution;
  }
  for (CDM::WeightedDistributionData const& distribution : collection.WeightedDistribution()) {
    std::vector<double> weights;
    for (auto& weight : distribution.Value()) {
      weights.push_back(weight.weight());
    }
    result.weighted_discrete_distributions[string_to_set(distribution.name())] = std::discrete_distribution<int>(weights.begin(), weights.end());
    result.weighted_discretes[string_to_set(distribution.name())] = distribution;
  }
  for (CDM::ContinuousUniformDistributionData const& distribution : collection.ContinuousUniformDistribution()) {
    result.continuous_uniform_distributions[string_to_set(distribution)] = std::uniform_real_distribution(distribution.a(), distribution.b());
    result.continuous_uniforms[string_to_set(distribution)] = distribution;
  }
  for (CDM::DiscreteUniformDistributionData const& distribution : collection.DiscreteUniformDistribution()) {
    result.discrete_uniform_distributions[string_to_set(distribution)] = std::uniform_int_distribution<int>(distribution.a(), distribution.b());
    result.discrete_uniforms[string_to_set(distribution)] = distribution;
  }
  for (CDM::BernoulliDistributionData const& distribution : collection.BernoulliDistribution()) {
    result.bernoulli_distributions[string_to_set(distribution)] = std::bernoulli_distribution(distribution.p());
    result.bernoullis[string_to_set(distribution)] = distribution;
  }
  for (CDM::SequenceData const& sequence : collection.Sequence()) {
    result.sequence_values[string_to_set(sequence)] = sequence.start();
    result.sequences[string_to_set(sequence)] = sequence;
  }

  for (CDM::SelectionCycleData const& cycle : collection.SelectionCycle()) {
    result.cycles_positions[string_to_set(cycle.name())] = 0;
    result.cycles[string_to_set(cycle.name())] = cycle;
  }

  return result;
}

static const std::string Heterogametic_Sex = "Heterogametic-Sex";
static const std::string Age = "Age";
static const std::string Weight = "Weight";
static const std::string Height = "Height";
static const std::string BodyMassIndex = "BodyMassIndex";
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
  if (samples.BodyMassIndexDistribution().present()) {
    distributions[BodyMassIndex] = process_distribution_collection(samples.BodyMassIndexDistribution().get());
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
  return distributions;
}
//-------------------------------------------------------------------------------
PopulationGenerator::PopulationGenerator(std::vector<std::string> params)
{
  _buffer.resize(_buffer_size);
  _runs = params;
}
//-------------------------------------------------------------------------------

template <typename numeric_type>
  requires std::integral<numeric_type> || std::floating_point<numeric_type>
numeric_type sample_population(std::set<std::string> key, DistributionCollection& collection, std::mt19937& rd)
{

  if (auto ptr = collection.normal_distributions.find(key); ptr != collection.normal_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);
    double min_value = (collection.normals[key].min().present()) ? collection.normals[key].min().get() : std::numeric_limits<double>::min();
    double max_value = (collection.normals[key].max().present()) ? collection.normals[key].max().get() : std::numeric_limits<double>::max();
    if (collection.normals[key].strategy() == "clamp") {
      // Strategy Clamp
      return std::max(min_value, std::min(max_value, roll_value));
    } else {
      // Strategy Resample - Lets limit ourselves to 1 second of resample to prevent livelock
      //                     when the user gives a bad min and max
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      std::chrono::steady_clock::time_point now = begin;
      while (roll_value < min_value || roll_value > max_value && std::chrono::duration_cast<std::chrono::seconds>(now - begin).count() < 1) {
        roll_value = distribution(rd);
        now = std::chrono::steady_clock::now();
      }
    }
    return roll_value;
  }

  if (auto ptr = collection.weighted_discrete_distributions.find(key); ptr != collection.weighted_discrete_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return std::stod(collection.weighted_discretes[key].Value()[roll_value]);
  }

  if (auto ptr = collection.continuous_uniform_distributions.find(key); ptr != collection.continuous_uniform_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return roll_value;
  }

  if (auto ptr = collection.discrete_uniform_distributions.find(key); ptr != collection.discrete_uniform_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return roll_value;
  }

  if (auto ptr = collection.bernoulli_distributions.find(key); ptr != collection.bernoulli_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return roll_value;
  }

  if (auto ptr = collection.sequence_values.find(key); ptr != collection.sequence_values.end()) {
    auto& [key, count] = *ptr;
    return count++;
  }

  if (auto ptr = collection.cycles_positions.find(key); ptr != collection.cycles_positions.end()) {
    auto& [key, index] = *ptr;
    auto v = std::stod(collection.cycles[key].Value()[index]);
    return 1.0;
  }

  return 0;
}

//-------------------------------------------------------------------------------

template <typename eType>
eType generate_cdm_enum(std::set<std::string> key, DistributionCollection& collection, std::mt19937& rd)
{

  if (auto ptr = collection.normal_distributions.find(key); ptr != collection.normal_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return eType::value(std::round(roll_value));
  }

  if (auto ptr = collection.weighted_discrete_distributions.find(key); ptr != collection.weighted_discrete_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return eType(collection.weighted_discretes[key].Value()[roll_value]);
  }

  if (auto ptr = collection.continuous_uniform_distributions.find(key); ptr != collection.continuous_uniform_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return eType::value(std::round(roll_value));
  }

  if (auto ptr = collection.discrete_uniform_distributions.find(key); ptr != collection.discrete_uniform_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return eType::value(roll_value);
  }

  if (auto ptr = collection.bernoulli_distributions.find(key); ptr != collection.bernoulli_distributions.end()) {
    auto& [key, distribution] = *ptr;
    auto roll_value = distribution(rd);

    return eType::value(roll_value);
  }

  if (auto ptr = collection.sequence_values.find(key); ptr != collection.sequence_values.end()) {
    auto& [key, count] = *ptr;
    return eType::value(count++);
  }

  if (auto ptr = collection.cycles_positions.find(key); ptr != collection.cycles_positions.end()) {
    auto& [key, index] = *ptr;
    auto& v = collection.cycles[key].Value()[index];
    index = ++index % collection.cycles[key].Value().size();
    return eType(v);
  }

  return eType::value(0);
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
  std::string population_pool_dir = std::format("Patients_{:%Y%m%dT%H%M}/", time);

  for (auto& run : _runs) {
    auto const& config_file = run;

    try {
      biogears::IOManager ioManager;

      auto content_size = ioManager.read_resource_file(run.c_str(), reinterpret_cast<char*>(&_buffer[0]), _buffer.size());
      _buffer[content_size] = 0;
      _buffer[content_size + 1] = std::ifstream::traits_type::eof();

      std::istrstream content_stream(reinterpret_cast<const char*>(_buffer.data()), content_size);
      auto population = CDM::PopulationGenerator(content_stream);

      std::map<std::string const, DistributionCollection> distributions = translate_distributions(population->Sampling());

      size_t file_count = 0;
      for (auto& population : population->Populations().Population()) {
        PopulationProfile profile;
        profile.count = population.count();
        profile.name = population.name();
        for (auto& tag : population.Property()) {
          std::transform(tag.begin(), tag.end(), tag.begin(), ::tolower);
          profile.tags.insert(tag);
        }

        for (auto i = 0; i < profile.count; ++i) {
          CDM::PatientData patient;

          patient.Name(std::format("id{1:03}_{0}", profile.name, ++file_count));
          if (distributions.find(Heterogametic_Sex) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[Heterogametic_Sex]);
            if (best_match.size() != 0) {
              patient.Sex(generate_cdm_enum<CDM::enumSex>(best_match, distributions[Heterogametic_Sex], gen));
            }
          }

          if (distributions.find(Age) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[Age]);
            if (best_match.size() != 0) {
              patient.Age(sample_population<double>(best_match, distributions[Age], gen));
              patient.Age().get().unit(unit);
            }
          }

          if (distributions.find(Weight) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[Weight]);
            if (best_match.size() != 0) {
              patient.Weight(sample_population<double>(best_match, distributions[Weight], gen));
              patient.Weight().get().unit(unit);
            }
          }

          if (distributions.find(Height) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[Height]);
            if (best_match.size() != 0) {
              patient.Height(sample_population<double>(best_match, distributions[Height], gen));
              patient.Height().get().unit(unit);
            }
          }

          if (distributions.find(BodyMassIndex) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BodyMassIndex]);
            if (best_match.size() != 0) {
              patient.BodyMassIndex(sample_population<double>(best_match, distributions[BodyMassIndex], gen));
              patient.BodyMassIndex().get().unit(unit);
            }
          }

          if (distributions.find(BodyFatFraction) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BodyFatFraction]);
            if (best_match.size() != 0) {
              patient.BodyFatFraction(sample_population<double>(best_match, distributions[BodyFatFraction], gen));
              patient.BodyFatFraction().get().unit(unit);
            }
          }

          if (distributions.find(MaxWorkRate) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[MaxWorkRate]);
            if (best_match.size() != 0) {
              patient.MaxWorkRate(sample_population<double>(best_match, distributions[MaxWorkRate], gen));
              patient.MaxWorkRate().get().unit(unit);
            }
          }

          if (distributions.find(BloodTypeABO) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BloodTypeABO]);
            if (best_match.size() != 0) {
              patient.BloodTypeABO(generate_cdm_enum<CDM::enumBloodType>(best_match, distributions[BloodTypeABO], gen));
            }
          }

          if (distributions.find(BloodTypeRh) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BloodTypeRh]);
            if (best_match.size() != 0) {
              patient.BloodTypeRh(sample_population<int>(best_match, distributions[BloodTypeRh], gen));
            }
          }

          if (distributions.find(AlveoliSurfaceArea) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[AlveoliSurfaceArea]);
            if (best_match.size() != 0) {
              patient.AlveoliSurfaceArea(sample_population<double>(best_match, distributions[AlveoliSurfaceArea], gen));
              patient.AlveoliSurfaceArea().get().unit(unit);
            }
          }

          if (distributions.find(Hyperhidrosis) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[Hyperhidrosis]);
            if (best_match.size() != 0) {
              patient.Hyperhidrosis(sample_population<double>(best_match, distributions[Hyperhidrosis], gen));
              patient.Hyperhidrosis().get().unit(unit);
            }
          }

          if (distributions.find(RightLungRatio) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[RightLungRatio]);
            if (best_match.size() != 0) {
              patient.RightLungRatio(sample_population<double>(best_match, distributions[RightLungRatio], gen));
              patient.RightLungRatio().get().unit(unit);
            }
          }

          if (distributions.find(SkinSurfaceArea) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[SkinSurfaceArea]);
            if (best_match.size() != 0) {
              patient.SkinSurfaceArea(sample_population<double>(best_match, distributions[SkinSurfaceArea], gen));
              patient.SkinSurfaceArea().get().unit(unit);
            }
          }

          if (distributions.find(SleepAmount) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[SleepAmount]);
            if (best_match.size() != 0) {
              patient.SleepAmount(sample_population<double>(best_match, distributions[SleepAmount], gen));
              patient.SleepAmount().get().unit(unit);
            }
          }

          if (distributions.find(PainSusceptibility) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[PainSusceptibility]);
            if (best_match.size() != 0) {
              patient.PainSusceptibility(sample_population<double>(best_match, distributions[PainSusceptibility], gen));
              patient.PainSusceptibility().get().unit(unit);
            }
          }

          if (distributions.find(BasalMetabolicRate) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BasalMetabolicRate]);
            if (best_match.size() != 0) {
              patient.BasalMetabolicRate(sample_population<double>(best_match, distributions[BasalMetabolicRate], gen));
              patient.BasalMetabolicRate().get().unit(unit);
            }
          }

          if (distributions.find(BloodVolumeBaseline) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[BloodVolumeBaseline]);
            if (best_match.size() != 0) {
              patient.BloodVolumeBaseline(sample_population<double>(best_match, distributions[BloodVolumeBaseline], gen));
              patient.BloodVolumeBaseline().get().unit(unit);
            }
          }

          if (distributions.find(DiastolicArterialPressureBaseline) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[DiastolicArterialPressureBaseline]);
            if (best_match.size() != 0) {
              patient.DiastolicArterialPressureBaseline(sample_population<double>(best_match, distributions[DiastolicArterialPressureBaseline], gen));
              patient.DiastolicArterialPressureBaseline().get().unit(unit);
            }
          }

          if (distributions.find(HeartRateBaseline) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[HeartRateBaseline]);
            if (best_match.size() != 0) {
              patient.HeartRateBaseline(sample_population<double>(best_match, distributions[HeartRateBaseline], gen));
              patient.HeartRateBaseline().get().unit(unit);
            }
          }

          if (distributions.find(RespirationRateBaseline) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[RespirationRateBaseline]);
            if (best_match.size() != 0) {
              patient.RespirationRateBaseline(sample_population<double>(best_match, distributions[RespirationRateBaseline], gen));
              patient.RespirationRateBaseline().get().unit(unit);
            }
          }

          if (distributions.find(SystolicArterialPressureBaseline) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[SystolicArterialPressureBaseline]);
            if (best_match.size() != 0) {
              patient.SystolicArterialPressureBaseline(sample_population<double>(best_match, distributions[SystolicArterialPressureBaseline], gen));
              patient.SystolicArterialPressureBaseline().get().unit(unit);
            }
          }

          if (distributions.find(HeartRateMaximum) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[HeartRateMaximum]);
            if (best_match.size() != 0) {
              patient.HeartRateMaximum(sample_population<double>(best_match, distributions[HeartRateMaximum], gen));
              patient.HeartRateMaximum().get().unit(unit);
            }
          }

          if (distributions.find(HeartRateMinimum) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[HeartRateMinimum]);
            if (best_match.size() != 0) {
              patient.HeartRateMinimum(sample_population<double>(best_match, distributions[HeartRateMinimum], gen));
              patient.HeartRateMinimum().get().unit(unit);
            }
          }

          if (distributions.find(FunctionalResidualCapacity) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[FunctionalResidualCapacity]);
            if (best_match.size() != 0) {
              patient.FunctionalResidualCapacity(sample_population<double>(best_match, distributions[FunctionalResidualCapacity], gen));
              patient.FunctionalResidualCapacity().get().unit(unit);
            }
          }

          if (distributions.find(ResidualVolume) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[ResidualVolume]);
            if (best_match.size() != 0) {
              patient.ResidualVolume(sample_population<double>(best_match, distributions[ResidualVolume], gen));
              patient.ResidualVolume().get().unit(unit);
            }
          }

          if (distributions.find(TotalLungCapacity) != distributions.end()) {
            auto [best_match, unit] = find_best_match(profile.tags, distributions[TotalLungCapacity]);
            if (best_match.size() != 0) {
              patient.TotalLungCapacity(sample_population<double>(best_match, distributions[TotalLungCapacity], gen));
              patient.TotalLungCapacity().get().unit(unit);
            }
          }

          xml_schema::namespace_infomap info;
          info[""].name = "uri:/mil/tatrc/physiology/datamodel";
          info[""].schema = "BioGearsDataModel.xsd";

#if _WIN32
          _mkdir("patients");
          _mkdir((std::string("patients/") + population_pool_dir).c_str());
#else
          mkdir("patients", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
          mkdir((std::string("patients/") + population_pool_dir).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

          try {
            std::string out_file = std::string("patients/") + population_pool_dir.c_str() + "/" + patient.Name() + ".xml";
            std::ofstream file;
            file.open(out_file.c_str());
            if (file.is_open()) {
              mil::tatrc::physiology::datamodel::Patient(file, patient, info);
              std::cout << std::string("Saved patients/") + population_pool_dir.c_str() + "/" + patient.Name() + ".xml"
                        << "\n";
            }
            file.close();

          } catch (std::exception e) {
            std::cout << e.what() << std::endl;
          }
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
