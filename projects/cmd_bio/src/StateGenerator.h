#ifndef BIOGEARS_STATE_GENERATOR_H
#define BIOGEARS_STATE_GENERATOR_H

#include <string>

namespace biogears
{
  class StateGenerator
  {
  public:
    StateGenerator();
    ~StateGenerator();

    int runScenario(int patientNum, std::string& XMLFileName, std::string& XMLString);
  };
} //namespace biogears

#endif //BIOGEARS_STATE_GENERATOR_H