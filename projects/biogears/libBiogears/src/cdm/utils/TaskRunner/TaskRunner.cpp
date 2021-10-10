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

#include <biogears/cdm/utils/TaskRunner/TaskProcessor.h>
#include <biogears/cdm/utils/TaskRunner/TaskRunner.h>

#include <iostream>

#include <iomanip>
#include <thread>

namespace biogears {
//--------------------------------------------------------------------------------------------------
/// \brief
/// Constructor
///
/// \details
/// The default constructor sets the thread count to the number of cores on the machine as determined
/// by std::thread::hardware_concurrency().  In the case that the number of cores cannot be determined,
/// the TaskRunner runs in single-threaded mode.
//--------------------------------------------------------------------------------------------------
TaskRunner::TaskRunner()
{
  m_threadCount = std::thread::hardware_concurrency();
  std::cout << "System supports " << m_threadCount << " threads." << std::endl;
  if (m_threadCount == 0) {
    m_threadCount = 1;
  }
  std::cout << "Will execute with " << m_threadCount << std::endl;
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Constructor
///
/// \param  threadCount     number of threads to use
//--------------------------------------------------------------------------------------------------
TaskRunner::TaskRunner(unsigned int threadCount)
  : m_threadCount(threadCount)
{
}
//--------------------------------------------------------------------------------------------------
TaskRunner::~TaskRunner()
{
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Adds a task to the task queue.  This should only be called from one thread.
///
/// \param  task    Unique pointer to the task to add
//--------------------------------------------------------------------------------------------------
void TaskRunner::AddTask(std::unique_ptr<Task> task)
{
  m_tasks.push(std::move(task));
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Runs all tasks and waits for completion
//--------------------------------------------------------------------------------------------------
void TaskRunner::Run()
{
  m_totalTaskCount = static_cast<decltype(m_totalTaskCount)>(m_tasks.size());

  if (m_echoPercentComplete) {
    PrintProgress();
  }

  if (m_threadCount == 1) {
    TaskProcessor(this)();
  } else {
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < m_threadCount; ++i) {
      threads.push_back(std::thread(TaskProcessor(this)));
    }

    for (std::thread& thread : threads) {
      thread.join();
    }
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Removes and returns the next task from the task queue.  If the queue is empty, a null pointer
/// is returned.
///
/// \return Pointer to the next task, or a null pointer if there are no tasks
//--------------------------------------------------------------------------------------------------
std::unique_ptr<Task> TaskRunner::GetNextTask()
{
  std::lock_guard<std::recursive_mutex> lock(m_taskQueueMutex);

  if (m_tasks.empty()) {
    return nullptr;
  } else {
    std::unique_ptr<Task> pNextTask = std::move(m_tasks.front());
    m_tasks.pop();

    return pNextTask;
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Notification that a task is complete
//--------------------------------------------------------------------------------------------------
void TaskRunner::ReportTaskComplete()
{
  if (m_echoPercentComplete) {
    std::lock_guard<std::recursive_mutex> lock(m_taskQueueMutex);

    ++m_tasksComplete;

    PrintProgress();
  }
}

//--------------------------------------------------------------------------------------------------
/// \brief
/// Prints a text progress bar based on the number of tasks there were in the queue when execution
/// began and the number of reported task completions.
//--------------------------------------------------------------------------------------------------
void TaskRunner::PrintProgress()
{
  std::lock_guard<std::recursive_mutex> lock(m_taskQueueMutex);

  double percent = static_cast<double>(m_tasksComplete) / static_cast<double>(m_totalTaskCount);
  unsigned int progress = static_cast<unsigned int>(percent * 30.0);

  std::ios::fmtflags flags(std::cout.flags());

  std::cout << "TaskRunner   [";
  for (unsigned int i = 0; i < 30; ++i) {
    if (i < progress) {
      std::cout << "=";
    } else {
      std::cout << " ";
    }
  }

  std::cout << "] " << std::fixed << std::setprecision(2) << (percent * 100.0) << "%\n";

  std::cout.flags(flags);
}
}