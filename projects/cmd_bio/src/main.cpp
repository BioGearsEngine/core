#include "argumentparser.hpp"

#include <biogears/exports.h>

#include <iostream>
#include <string>

#include "StateGenerator.h"
#include "data/EnvironmentGenerator.h"
#include "data/PatientGenerator.h"

#include "data/StabilizationGenerator.h"
#include "data/SubstanceGenerator.h"

#include "data/NutritionGenerator.h"
#include "data/CompoundGenerator.h"

int main(int argc, char** argv)
{
  namespace bg = biogears;
  ArgumentParser parser;
  parser.parse(argc,argv);
  if (argc > 1) {
	if (parser.exists("STATES")) {
      auto generator = bg::StateGenerator();

      std::string var1{ argv[2] };
      std::string var2{ " Foo" };
      generator.runScenario(0, var1, var2);
    } else if (parser.exists("SUBS")) {
      auto substances = bg::SubstanceGenerator();
      substances.parse();
      substances.save();
    } else if (parser.exists("ENVS")) {
      auto envs = bg::EnvironmentGenerator();
      envs.parse();
      envs.save();
    } else if (parser.exists("PATIENTS")) {
      auto patients = bg::PatientGenerator();
      patients.parse();
      patients.save();
    } else if (parser.exists("CONF")) {
      auto confs = bg::StabilizationGenerator();
      confs.parse();
      confs.save();
    } else if (parser.exists("NUTRITION")) {
      auto confs = bg::NutritionGenerator();
      confs.parse();
      confs.save();
    } else if (parser.exists("COMPOUNDS")) {
      auto comps = bg::CompoundGenerator();
	  comps.parse();
	  comps.save();
	} else {
      std::cout << "Input not recognized" << std::endl;
      std::cout << "Usuage " + std::string(argv[0]) + ": [SUBS|ENVS|PATIENTS|CONF|NEUTRITION]";
    }
  }
  return 0;
}
