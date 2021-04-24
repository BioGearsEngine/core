%module "biogears::SEScenarioAutoSerialization"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEScenarioAutoSerialization.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
