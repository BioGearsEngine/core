%module "biogears::SEDrugSystem"

// Add necessary symbols to generated header
%{
#include<cdm/system/physiology/SEDrugSystem.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
