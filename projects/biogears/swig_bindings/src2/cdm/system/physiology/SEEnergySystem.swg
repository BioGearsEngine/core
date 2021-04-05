%module "biogears::SEEnergySystem"

// Add necessary symbols to generated header
%{
#include<cdm/system/physiology/SEEnergySystem.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
