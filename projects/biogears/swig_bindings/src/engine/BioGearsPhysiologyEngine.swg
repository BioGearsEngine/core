//%module(package="biogears") "biogears_controller_BioGearsPhysiologyEngine"
%module(package="biogears") "BioGearsPhysiologyEngine"
// Add necessary symbols to generated header
%{
#include <biogears/cdm/utils/Logger.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>

biogears::CreateBioGearsEngine(const std::string working_dir, biogears::Logger* logger);