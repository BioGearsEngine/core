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

#include <biogears/cdm/utils/TaskRunner/TaskQueue.h>

#include <biogears/cdm/CommonDataModel.h>

#include <memory>
#include <mutex>
#include <queue>

namespace biogears {
//--------------------------------------------------------------------------------------------------
/// \brief
/// This class handles concurrent execution of tasks
//--------------------------------------------------------------------------------------------------
class BIOGEARS_API TaskRunner : public TaskQueue {
public:
  TaskRunner();
  TaskRunner(unsigned int threadCount);
  virtual ~TaskRunner();

  TaskRunner(const TaskRunner& other) = delete;
  TaskRunner& operator=(const TaskRunner& other) = delete;

  void AddTask(std::unique_ptr<Task> task);
  void Run();

  virtual std::unique_ptr<Task> GetNextTask() override;

  void EchoProgress(bool echo) { m_echoPercentComplete = echo; }
  virtual void ReportTaskComplete() override;

private:
  void PrintProgress();

  mutable std::recursive_mutex m_taskQueueMutex; ///< Mutex used to synchronize concurrent access to the task queue
  std::queue<std::unique_ptr<Task>> m_tasks; ///< Queue of tasks to be run

  unsigned int m_threadCount = 1; ///< The number of threads to spawn when running tasks

  bool m_echoPercentComplete = false; ///< Whether or not the task runner should output percent complete as it executes
  unsigned int m_totalTaskCount = 0; ///< Total number of tasks in the queue when the task running begins to execute
  unsigned int m_tasksComplete = 0; ///< Number of completed tasks
};
}