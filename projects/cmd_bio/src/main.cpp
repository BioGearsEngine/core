#include <biogears/exports.h>

#include <iostream>
#include <string>

#include "StateGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"
#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

int main(int argc, char* argv[])
{
  namespace bg = biogears;
  if (argc > 1) {
    if ("STATES" == std::string(argv[1])) {
      auto generator = bg::StateGenerator();

      std::string var1{ argv[2] };
      std::string var2{ " Foo" };
      generator.runScenario(0, var1, var2);
    } else if ("SUBS" == std::string(argv[1])) {
      auto substances = bg::SubstanceGenerator();
      substances.parse();
      substances.save();
    } else if ("ENVS" == std::string(argv[1])) {
      auto envs = bg::EnvironmentGenerator();
      envs.parse();
      envs.save();
    } else if ("PATIENTS" == std::string(argv[1])) {
      auto patients = bg::PatientGenerator("");
      patients.parse();
      patients.save();
    } else if ("CONF" == std::string(argv[1])) {
      auto confs = bg::StabilizationGenerator();
      confs.parse();
      confs.save();
    } else {
      std::cout << "Input not recognized" << std::endl;
      std::cout << "Usuage " + std::string(argv[0]) + ": [SUBS|ENVS|PATIENTS|CONF]";
    }
  }
  return 0;
}
