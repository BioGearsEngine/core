#ifndef BIOGEARS_POPULATION_GENERATOR_H
#define BIOGEARS_POPULATION_GENERATOR_H

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
#include <biogears/threading/thread_pool.h>
#include <biogears/threading/runnable.h>

namespace biogears
{
  class PopulationGenerator 
  {
  public:
    PopulationGenerator(std::vector<std::string> params);
    PopulationGenerator( const PopulationGenerator&) = delete;
    PopulationGenerator(PopulationGenerator&&) = default;
    ~PopulationGenerator() = default;

    void Generate();

    void SetGirlNames(std::vector<std::string>&& names);
    void SetBoyNames(std::vector<std::string>&& names);

    void AddBoyName(std::string);
    void AddGirlName(std::string);

    void ClearBoyNames();
    void ClearGirlNames();

  private:
    std::vector<std::string> _runs;
  };
} //namespace biogears

#endif //BIOGEARS_POPULATION_GENERATOR_H