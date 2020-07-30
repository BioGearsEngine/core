#ifndef BIOGEARS_COMMON_THREAD_POOL_IMPL
#define BIOGEARS_COMMON_THREAD_POOL_IMPL

//-------------------------------------------------------------------------------------------
//- Copyright 2017 Applied Research Associates, Inc.
//- Licensed under the Apache License, Version 2.0 (the "License"); you may not use
//- this file except in compliance with the License. You may obtain a copy of the License
//- at:
//- http://www.apache.org/licenses/LICENSE-2.0
//- Unless required by applicable law or agreed to in writing, software distributed under
//- the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//- CONDITIONS OF ANY KIND, either express or implied. See the License for the
//-  specific language governing permissions and limitations under the License.
//-------------------------------------------------------------------------------------------

//! \author Steven A White
//! \date   Sept 7 2017
//! \copyright Copyright 2017 Applied Research Associates, Inc.
//! \license This project is released under the Apache 2.0 license.
//! \file
//!
//!  Concurrent Queue using std::queue std::mutex.
//!
#include <atomic>

#include <biogears/threading/thread_pool.h>
#include <biogears/threading/thread_work_channel.tci.h>
namespace biogears {

struct ThreadPool::Implementation {
  std::vector<std::thread> pool;
  std::mutex poolMutex;

  std::condition_variable poolWorkSignal;
  std::condition_variable poolPauseSignal;

  std::atomic<bool> poolRunning;
  std::atomic<bool> poolProcessing;
  std::atomic<bool> joined;
  std::shared_ptr<ThreadWorkChannel<void(void)>> workQueue;
  Implementation(size_t);

  void pool_process_func();
};
//-------------------------------------------------------------------------------
ThreadPool::Implementation::Implementation(size_t pool_size)
  : pool(pool_size)
  , workQueue(ThreadWorkChannel<void(void)>::make_shared())
  , poolRunning(false)
  , poolProcessing(true)
  , joined(false)

{
}
//------------------------------------------------------------------------------
ThreadPool::ThreadPool(size_t pool_size)
  : _impl(std::make_unique<Implementation>(pool_size))
{
}
//-------------------------------------------------------------------------------
ThreadPool::ThreadPool(ThreadPool&& obj)
  : _impl(std::move(obj._impl))
{
}
//-------------------------------------------------------------------------------
void ThreadPool::Implementation::pool_process_func()
{
  while (poolRunning.load()) {
    if (poolProcessing.load()) {
      auto work = workQueue->consume();
      if (work) {
        work();
      }
    } else {
      std::unique_lock<std::mutex> lock(poolMutex);
      poolPauseSignal.wait(lock);
    }
  }
}
//-------------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
  _impl->poolProcessing.store(false);
  _impl->poolRunning.store(false);

  for (size_t i = 0; i < _impl->pool.size(); ++i) {
    _impl->workQueue->insert(std::function<void(void)>());
  }

  _impl->poolWorkSignal.notify_all();
  _impl->poolPauseSignal.notify_all();

  for (auto& thread : _impl->pool) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  _impl = nullptr;
}
//-------------------------------------------------------------------------------
void ThreadPool::start()
{
  std::lock_guard<std::mutex>(_impl->poolMutex);
  if (!_impl->poolRunning) {
    _impl->poolRunning.store(true);
    for (auto& thread : _impl->pool) {
      thread = std::thread(&Implementation::pool_process_func, _impl.get());
    }
  }
}
//-------------------------------------------------------------------------------
void ThreadPool::stop()
{
  _impl->poolRunning.store(false);
  _impl->poolProcessing.store(false);

  for (auto& thread : _impl->pool) {
    _impl->workQueue->insert({});
  }

  _impl->poolWorkSignal.notify_all();
  _impl->poolPauseSignal.notify_all();
}
//-------------------------------------------------------------------------------
bool ThreadPool::stop_if_empty()
{
  if (0 == _impl->workQueue->unsafe_size()) {
    stop();
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------
void ThreadPool::join()
{
  for (auto& thread : _impl->pool) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  _impl->joined = true;
}
//-------------------------------------------------------------------------------
bool ThreadPool::joined()
{
  return _impl->joined;
}
//-------------------------------------------------------------------------------
bool ThreadPool::suspend()
{
  bool success = false;
  if (_impl->poolRunning.load()) {
    _impl->poolProcessing.store(false);
    success = true;
  }
  return success;
}
//-------------------------------------------------------------------------------
bool ThreadPool::resume()
{
  bool success = false;
  if (_impl->poolRunning.load()) {
    _impl->poolProcessing.store(true);
    _impl->poolPauseSignal.notify_all();
    success = true;
  }
  return success;
}
//-------------------------------------------------------------------------------
void ThreadPool::queue_work(std::function<void(void)> func)
{
  _impl->workQueue->insert(std::move(func));
}
//-------------------------------------------------------------------------------
void ThreadPool::queue_work(Steppable<void(void)>& steppable)
{
  _impl->workQueue->insert(steppable.step_as_func());
}
//-------------------------------------------------------------------------------
ThreadPool::SourcePtr const ThreadPool::get_source()
{
  return _impl->workQueue->as_source();
}
//-------------------------------------------------------------------------------
ThreadPool::SinkPtr const ThreadPool::get_sink()
{
  return _impl->workQueue->as_sink();
}

}

#endif //BIOGEARS_COMMON_THREAD_POOL_IMPL
