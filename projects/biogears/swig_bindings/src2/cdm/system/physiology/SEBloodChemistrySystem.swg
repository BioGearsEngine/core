%module "biogears::SEBloodChemistrySystem"

// Add necessary symbols to generated header
%{
#include<cdm/system/physiology/SEBloodChemistrySystem.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
