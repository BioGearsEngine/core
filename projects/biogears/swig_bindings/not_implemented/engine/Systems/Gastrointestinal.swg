%module "biogears::Gastrointestinal"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Gastrointestinal.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
