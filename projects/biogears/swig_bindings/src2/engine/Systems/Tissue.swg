%module "biogears::Tissue"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Tissue.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
