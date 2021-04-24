%module "biogears::BioGearsConfiguration"

// Add necessary symbols to generated header
%{
#include<engine/Controller/BioGearsConfiguration.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
