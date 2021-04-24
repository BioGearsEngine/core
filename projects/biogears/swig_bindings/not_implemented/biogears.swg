%module "biogears"

// Add necessary symbols to generated header
%{
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
%}

%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
%include <biogears/cdm/engine/PhysiologyEngine.h>
%include <biogears/engine/Controller/BioGears.h>
%include <biogears/engine/Controller/BioGearsEngine.h>

