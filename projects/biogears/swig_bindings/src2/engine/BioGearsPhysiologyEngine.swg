%module "biogears::BioGearsPhysiologyEngine"

// Add necessary symbols to generated header
%{
#include <biogears/engine/BioGearsPhysiologyEngine.h>
%}

%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
%include <biogears/engine/BioGearsPhysiologyEngine.h>

