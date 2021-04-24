%module "biogears::SEAnesthesiaMachineConfiguration"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
