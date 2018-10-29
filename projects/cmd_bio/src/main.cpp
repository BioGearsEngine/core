#include <biogears/exports.h>

#include <iostream>
#include <string>

#include "StateGenerator.h"
#include "GenStates.h"

int main (int argc, char* argv[])
{
  namespace bg = biogears;
  if ( argc > 1 )
  {
    if ( "STATES" == std::string(argv[1]) )
    {
      auto generator = bg::StateGenerator();

      std::string var1{ argv[2] };
      std::string var2{" Foo"};
      generator.runScenario(0, var1, var2 );
    } else
    {
      auto substances = bg::SubstanceGenerator("", "Substances.csv");
      substances.parse();
      substances.print();
    }
  }
  return 0;
}