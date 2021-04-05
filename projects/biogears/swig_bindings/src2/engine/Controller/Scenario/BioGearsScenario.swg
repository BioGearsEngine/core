%module "biogears::BioGearsScenario"

// Add necessary symbols to generated header
%{
#include<engine/Controller/Scenario/BioGearsScenario.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
