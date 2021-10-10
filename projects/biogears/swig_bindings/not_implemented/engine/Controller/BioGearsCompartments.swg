%module "biogears::BioGearsCompartments"

// Add necessary symbols to generated header
%{
#include<engine/Controller/BioGearsCompartments.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
