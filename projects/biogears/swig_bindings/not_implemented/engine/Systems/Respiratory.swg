%module "biogears::Respiratory"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Respiratory.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
