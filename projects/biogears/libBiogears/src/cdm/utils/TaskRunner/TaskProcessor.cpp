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
#include <biogears/cdm/utils/TaskRunner/TaskQueue.h>

#include <biogears/cdm/utils/TaskRunner/Task.h>

namespace biogears {
//--------------------------------------------------------------------------------------------------
/// \brief
/// Constructor
///
/// \param  pTaskQueue  Pointer to the TaskQueue to draw Tasks from
//--------------------------------------------------------------------------------------------------
TaskProcessor::TaskProcessor(TaskQueue* pTaskQueue)
  : m_pTaskQueue(pTaskQueue)
{
}
//--------------------------------------------------------------------------------------------------
TaskProcessor::~TaskProcessor(){
}
//--------------------------------------------------------------------------------------------------
/// \brief
/// Parenthesis operator that draws and runs Tasks until the TaskQueue is empty
//--------------------------------------------------------------------------------------------------
void TaskProcessor::operator()()
{
  while (std::unique_ptr<Task> pTask = m_pTaskQueue->GetNextTask()) {
    pTask->Run();
    m_pTaskQueue->ReportTaskComplete();
  }
}
}