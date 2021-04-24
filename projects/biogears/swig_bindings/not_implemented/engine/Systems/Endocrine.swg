%module "biogears::Endocrine"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Endocrine.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
