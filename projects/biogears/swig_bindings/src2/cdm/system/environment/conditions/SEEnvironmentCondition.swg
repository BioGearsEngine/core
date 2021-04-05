%module "biogears::SEEnvironmentCondition"

// Add necessary symbols to generated header
%{
#include<cdm/system/environment/conditions/SEEnvironmentCondition.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
