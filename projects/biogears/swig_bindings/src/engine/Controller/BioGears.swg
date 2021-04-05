//%module "biogears_engine_Controller_BioGears"
%module(package="biogears") "BioGears"
// Add necessary symbols to generated header
%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/Controller/BioGears.h>
using namespace biogears;
using namespace CDM;
%}
%include <windows.i>
%include <std_shared_ptr.i>

%include <std_string.i>
%include <std_vector.i>

%include <biogears/exports.h>

%nspace "biogears::BioGears";
%ignore biogears::BioGears::SaveState(const char* file);
%ignore biogears::BioGears::SaveState(const std::string&);
%ignore biogears::BioGears::SaveState();
%include<biogears/engine/Controller/BioGears.h>
