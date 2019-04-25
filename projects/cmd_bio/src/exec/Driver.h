#ifndef CMD_BIO_EXEC_DRIVER_H
#define CMD_BIO_EXEC_DRIVER_H

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

#include <biogears/threading/runnable.h>
#include <biogears/threading/thread_pool.h>

#include "../utils/Config.h"

namespace biogears {
class Executor;
class Config;

class Driver : biogears::Runnable {
public:
  Driver(size_t thread_count);
  Driver(const Driver&) = delete;
  Driver(Driver&&) = default;
  ~Driver() override;

  void configure(const Config& config);
  void queue(const Config& runs);

  void run() override;
  void stop() override;
  bool stop_if_empty();
  void stop_when_empty();

  void join() override;

protected:
  void queue_BGEUnitTest(Executor);
  void queue_CDMUnitTest(Executor);
  void queue_Scenario(Executor);
private:
  ThreadPool _pool;
  Config _globals;
};
} //namespace biogears

#endif //CMD_BIO_EXEC_DRIVER_H