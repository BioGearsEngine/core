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

#include <atomic>
#include <chrono>

#include <biogears/config.h>
#include <biogears/filesystem/path.h>
#include <biogears/threading/runnable.h>
#include <biogears/threading/thread_pool.h>

#include "../utils/Config.h"

OPEN_BIOGEARS_NAMESPACE

class Executor;
class Config;

class Driver : BIOGEARS_NAMESPACE Runnable {
public:
  Driver(char* exe_name, size_t thread_count);
  Driver(const Driver&) = delete;
  Driver(Driver&&) = delete;
  ~Driver() override;

  void configure(const Config& config);
  void queue(const Config& runs, bool as_subprocess = false);

  void run() override;
  void stop() override;
  bool stop_if_empty();
  void stop_when_empty();
  void join() override;

  bool has_work() const { return _total_work > 0; }

protected:
  void subprocess_execute(BIOGEARS_NAMESPACE Executor& ex, bool multi_patient_run);
  void async_execute(BIOGEARS_NAMESPACE Executor& ex, bool multi_patient_run);

  void queue_BGEUnitTest(Executor, bool as_subprocess = false);
  void queue_CDMUnitTest(Executor, bool as_subprocess = false);

  void queue_Scenario(Executor, bool as_subprocess = false);
  void queue_from_patient_files(const Executor& exec, const std::vector<filesystem::path>& patient_files, std::function<void(Executor, bool)> scenario_launch_func);
  void queue_from_sate_files(const Executor& exec, const std::vector<filesystem::path>& state_files, std::function<void(Executor, bool)> scenario_launch_func);
  std::vector<filesystem::path> find_matching_files(const std::string& pattern);
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
CLOSE_BIOGEARS_NAMESPACE //namespace biogears

#endif //CMD_BIO_EXEC_DRIVER_H
