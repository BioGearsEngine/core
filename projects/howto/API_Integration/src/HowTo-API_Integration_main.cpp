#include <iostream>

#include "HowTo-API_Integration.h"
#include "HowToTracker.h"

int main( int argc, char* argv[] )
{
  BioGearsPlugin plugin("HowTo-API_Integration");
  plugin.run();
  plugin.stop();
  auto start_time = std::chrono::steady_clock::now();
  std::stringstream ss;
  while (plugin.isRunning()) {
    using namespace std::chrono_literals;
    auto duration = std::chrono::steady_clock::now() - start_time;

    ss.str("");
    ss << "Biogears has been running for " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << "\n";
    std::cout << ss.str();

    std::this_thread::sleep_for(15s);
  }
  plugin.join();
}