%module "biogears::Environment"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Environment.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
