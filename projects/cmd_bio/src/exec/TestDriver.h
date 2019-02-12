#ifndef BIOGEARS_TEST_DRIVER_H
#define BIOGEARS_TEST_DRIVER_H

#include <biogears/threading/thread_pool.h>
#include <biogears/threading/runnable.h>

namespace biogears
{
  class TestDriver : biogears::Runnable
  {
  public:
    TestDriver( size_t thread_count);
    TestDriver( const TestDriver&) = delete;
    TestDriver(TestDriver&&) = default;
    ~TestDriver() override;

    void RunTests(std::vector<std::string> files);
   
    void run()  override;
    void stop() override;
    bool stop_if_empty();
    void join() override; 
  private:
    ThreadPool _pool;
  };
} //namespace biogears

#endif //BIOGEARS_STATE_GENERATOR_H