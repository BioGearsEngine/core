%module "biogears::Energy"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Energy.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
