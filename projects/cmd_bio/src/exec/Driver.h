#ifndef CMD_BIO_EXEC_DRIVER_H
#define CMD_BIO_EXEC_DRIVER_H

#include <biogears/threading/runnable.h>
#include <biogears/threading/thread_pool.h>

namespace biogears {
class Executor;
class Config;

class Driver : biogears::Runnable {
public:
  Driver(size_t thread_count);
  Driver(const Driver&) = delete;
  Driver(Driver&&) = default;
  ~Driver() override;

  void queue(const Config& runs);

  void run() override;
  void stop() override;
  bool stop_if_empty();
  void join() override;

protected:
  void queue_BGEUnitTest(const Executor&);
  void queue_CDMUnitTest(const Executor&);
  void queue_Scenario(const Executor&);
private:
  ThreadPool _pool;
};
} //namespace biogears

#endif //CMD_BIO_EXEC_DRIVER_H