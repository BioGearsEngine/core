%module "biogears::SEInspiratoryValveLeak"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Anesthesia/actions/SEInspiratoryValveLeak.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
