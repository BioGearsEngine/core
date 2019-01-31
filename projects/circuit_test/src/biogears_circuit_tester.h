#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/circuit/SECircuit.h>
#include <biogears/cdm/circuit/SECircuitManager.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsCircuits.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/BioGearsSubstances.h>
#include <biogears/cdm/system/environment/SEEnvironment.h>

#include <string>
#include <vector>

namespace biogears {
class BioGearsCircuitTester : public BioGears {
public:
  BioGearsCircuitTester(std::string logFileName);
  ~BioGearsCircuitTester();
  void TemperatureCircuitTest();
  void AnesthesiaCircuitTest();
  void CardiovascularCircuitTest();
  void EnvironmentCircuitTest();
  void RespiratoryCircuitTest();
  void RenalCircuitTest();

protected:
  std::ofstream file;
};

} //end namespace biogears
