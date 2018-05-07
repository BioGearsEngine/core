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
#pragma once

#include <string>
#include <set>

namespace
{
    enum class RunMode
    {
        Invalid,
        Scenario,
        Verification
    };
}

class BioGearsScenarioDriver
{
public:
    BioGearsScenarioDriver() = default;
    ~BioGearsScenarioDriver() = default;

    bool Configure(int argc, char* argv[]);
    void Run();

private:
    void RunScenario();
    void RunVerification();

    bool HasArgument(const std::string& argument);

    std::string m_file;
    std::set<std::string> m_arguments;
    RunMode m_mode = RunMode::Invalid;
};