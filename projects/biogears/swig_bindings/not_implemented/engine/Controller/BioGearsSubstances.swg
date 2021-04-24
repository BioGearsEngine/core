%module "biogears::BioGearsSubstances"

// Add necessary symbols to generated header
%{
#include<engine/Controller/BioGearsSubstances.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
