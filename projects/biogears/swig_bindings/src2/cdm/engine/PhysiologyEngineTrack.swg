%module "biogears::PhysiologyEngineTrack"

// Add necessary symbols to generated header
%{
#include<cdm/engine/PhysiologyEngineTrack.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
