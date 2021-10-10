%module "biogears::Cardiovascular"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Cardiovascular.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
