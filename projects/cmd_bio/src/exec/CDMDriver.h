#ifndef CDM_DRIVER_H
#define CDM_DRIVER_H

#include <biogears/threading/thread_pool.h>
#include <biogears/threading/runnable.h>

#include "../utils/Config.h"

namespace biogears
{
  class CDMDriver : biogears::Runnable
  {
  public:
    CDMDriver( size_t thread_count);
    CDMDriver( const CDMDriver&) = delete;
    CDMDriver(CDMDriver&&) = default;
    ~CDMDriver() override;

    void RunCDMs(const Config& runs);
   
    void run()  override;
    void stop() override;
    bool stop_if_empty();
    void join() override; 
  private:
    ThreadPool _pool;
  };
} //namespace biogears

#endif //CDM_DRIVER_H