//%module "biogears_engine_Controller_PhysiologyEngine"
%module(package="biogears") "PhysiologyEngine"
// Add necessary symbols to generated header
%{
#include <biogears/container/Tree.tci.h>
#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/engine/PhysiologyEngine.h>
using namespace biogears;
using namespace CDM;
%}
%include <windows.i>
%include <std_shared_ptr.i>

%include <std_string.i>
%include <std_vector.i>

%include <biogears/exports.h>

%import(module="CommonDataModel") <biogears/cdm/CommonDataModel.h>

%nspace "biogears::PhysiologyEngine";
%ignore biogears::PhysiologyEngine::SaveState(const char* file);
%ignore biogears::PhysiologyEngine::SaveState(const std::string&);
%ignore biogears::PhysiologyEngine::SaveState();
%include<biogears/cdm/engine/PhysiologyEngine.h>
