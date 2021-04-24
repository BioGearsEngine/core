%module "biogears::BioGearsScenarioExec"

// Add necessary symbols to generated header
%{
#include<engine/Controller/Scenario/BioGearsScenarioExec.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
