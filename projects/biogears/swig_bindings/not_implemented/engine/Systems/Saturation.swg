%module "biogears::Saturation"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Saturation.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
