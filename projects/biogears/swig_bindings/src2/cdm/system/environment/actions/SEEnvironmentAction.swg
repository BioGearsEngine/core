%module "biogears::SEEnvironmentAction"

// Add necessary symbols to generated header
%{
#include<cdm/system/environment/actions/SEEnvironmentAction.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
