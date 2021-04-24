%module "biogears::Renal"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Renal.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
