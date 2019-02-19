#include <fstream>
#include <iostream>
#include <string>

#include <biogears/cdm/Serializer.h>
#include <biogears/engine/Controller/BioGears.h>


namespace biogears {
class BioGearsCircuitProfiler {
public:
  BioGearsCircuitProfiler();
  ~BioGearsCircuitProfiler();
  void TemperatureCircuitTest();
  void AnesthesiaCircuitTest();
  void CardiovascularCircuitTest();
  void RespiratoryCircuitTest();
  void RenalCircuitTest();

  void ProfileThermalCircuit(SEThermalCircuit& thermal, std::string circuit_name);
  void ProfileFluidCircuit(SEFluidCircuit fluid, std::string circuit_name);

protected:

};

} //end namespace biogears
