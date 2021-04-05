%module "biogears::SEAnesthesiaMachine"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
