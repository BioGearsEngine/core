%module "biogears::Diffusion"

// Add necessary symbols to generated header
%{
#include<engine/Systems/Diffusion.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
