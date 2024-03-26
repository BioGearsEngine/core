#ifndef BIOGEARS_SCENARIO_GENERATOR_H
#define BIOGEARS_SCENARIO_GENERATOR_H
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
#include <biogears/threading/runnable.h>
#include <biogears/threading/thread_pool.h>

namespace biogears {
class ScenarioGenerator {
public:
  ScenarioGenerator(std::vector<std::string> params);
  ScenarioGenerator(const ScenarioGenerator&) = delete;
  ScenarioGenerator(ScenarioGenerator&&) = default;
  ~ScenarioGenerator() = default;

  void Generate();

private:
  std::vector<std::string> _runs;
  std::vector<unsigned char> _buffer; // 5MBs of read buffer state
  std::size_t _buffer_size = 5 * 1024 * 1024;
};
} // namespace biogears

#endif // BIOGEARS_SCENARIO_GENERATOR_H