//%module "biogears_engine_Controller_BioGearsEngine"
%module(package="biogears") "BioGearsEngine"
// Add necessary symbols to generated header
%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
using namespace biogears;
using namespace CDM;
%}
%include <windows.i>
%include <std_shared_ptr.i>

%include <std_string.i>
%include <std_vector.i>

%include <biogears/exports.h>

%import(module="PhysiologyEngine") <biogears/cdm/engine/PhysiologyEngine.h>
%import(module="BioGears") <biogears/engine/Controller/BioGears.h>

%nspace "biogears::BioGearsEngine";
%ignore biogears::BioGearsEngine::SaveState(const char* file);
%ignore biogears::BioGearsEngine::SaveState(const std::string&);
%ignore biogears::BioGearsEngine::SaveState();
%include<biogears/engine/Controller/BioGearsEngine.h>
