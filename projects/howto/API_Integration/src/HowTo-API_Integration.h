#pragma once

/**************************************************************************************
Copyright 2020 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <memory>
#include <string>

//!
//! \brief Example Integration Plugin for biogears with threading support
//!
class BioGearsPlugin {

public:
  BioGearsPlugin(std::string name);
  ~BioGearsPlugin();

  BioGearsPlugin& operator=(const BioGearsPlugin&) = delete;
  BioGearsPlugin& operator=(BioGearsPlugin&&);

  void run();
  void stop();
  void join();

  bool isRunning();

  private:
  struct Implementation;
  std::unique_ptr<Implementation> _pimpl;
};