%module "biogears::Nervous"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Nervous.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
