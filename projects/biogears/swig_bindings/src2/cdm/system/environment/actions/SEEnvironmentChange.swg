%module "biogears::SEEnvironmentChange"

// Add necessary symbols to generated header
%{
#include<cdm/system/environment/actions/SEEnvironmentChange.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
