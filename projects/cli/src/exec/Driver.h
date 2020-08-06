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

#include <chrono>
#include <atomic>

#include <biogears/threading/runnable.h>
#include <biogears/threading/thread_pool.h>

#include "../utils/Config.h"

namespace biogears {
class Executor;
class Config;

class Driver : biogears::Runnable {
public:
  Driver(char* exe_name, size_t thread_count);
  Driver(const Driver&) = delete;
  Driver(Driver&&) = default;
  ~Driver() override;

  void configure(const Config& config);
  void queue(const Config& runs, bool as_subprocess = false);

  void run() override;
  void stop() override;
  bool stop_if_empty();
  void stop_when_empty();
  void join() override;

protected:
  void queue_BGEUnitTest(Executor, bool as_subprocess = false);
  void queue_CDMUnitTest(Executor, bool as_subprocess = false);
  void queue_Scenario(Executor, bool as_subprocess = false);
  void subprocess_execute(biogears::Executor& ex, bool multi_patient_run);
  void async_execute(biogears::Executor& ex, bool multi_patient_run);

  std::string process_str();

private:
  ThreadPool _pool;
  std::thread _join_thread;
  Config _globals;

  std::string _relative_path;
  std::chrono::steady_clock::time_point _start_time;
  std::atomic<int> _thread_count;
  std::atomic<int> _process_count;
  size_t _total_work;
  size_t _jobs;
};
} //namespace biogears

#endif //CMD_BIO_EXEC_DRIVER_H
