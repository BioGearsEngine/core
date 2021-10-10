%module "biogears::SEAnesthesiaMachineOxygenBottle"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
