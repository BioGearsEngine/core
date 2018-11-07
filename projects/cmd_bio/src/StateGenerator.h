#ifndef BIOGEARS_STATE_GENERATOR_H
#define BIOGEARS_STATE_GENERATOR_H

#include <string>

#include <biogears/threading/thread_pool.h>
#include <biogears/threading/runnable.h>

namespace biogears
{
  class StateGenerator : biogears::Runnable
  {
  public:
    StateGenerator( size_t thread_count);
    StateGenerator( const StateGenerator&) = delete;
    StateGenerator(StateGenerator&&) = default;
    ~StateGenerator() override;

    void GenStates();
   
    void run()  override;
    void stop() override;
    bool stop_if_empty();
    void join() override; 
  private:
    ThreadPool _pool;
  };
} //namespace biogears

#endif //BIOGEARS_STATE_GENERATOR_H