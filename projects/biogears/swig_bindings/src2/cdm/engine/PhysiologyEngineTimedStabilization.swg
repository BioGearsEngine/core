%module "biogears::PhysiologyEngineTimedStabilization"

// Add necessary symbols to generated header
%{
#include<cdm/engine/PhysiologyEngineTimedStabilization.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
