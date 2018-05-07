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

#include <vector>

class Task;
class TaskQueue;

//--------------------------------------------------------------------------------------------------
/// \brief
/// This class is a functor used in threads launched by the TaskRunner.  It's responsibility is to
/// pull and run Tasks from the TaskRunner until the Task queue is empty.
//--------------------------------------------------------------------------------------------------
class TaskProcessor
{
public:
    TaskProcessor(TaskQueue* pTaskQueue);
    ~TaskProcessor() = default;

    void operator()();

private:
    TaskQueue* m_pTaskQueue = nullptr;  ///< Pointer to the TaskQueue to draw Tasks from
};
