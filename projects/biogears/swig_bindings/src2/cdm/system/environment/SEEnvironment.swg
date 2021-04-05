%module "biogears::SEEnvironment"

// Add necessary symbols to generated header
%{
#include<cdm/system/environment/SEEnvironment.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
