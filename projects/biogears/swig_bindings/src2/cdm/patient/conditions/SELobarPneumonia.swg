%module "biogears::SELobarPneumonia"

// Add necessary symbols to generated header
%{
#include<cdm/patient/conditions/SELobarPneumonia.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
