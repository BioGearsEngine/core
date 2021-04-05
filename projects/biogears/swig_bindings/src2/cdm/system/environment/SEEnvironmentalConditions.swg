%module "biogears::SEEnvironmentalConditions"

// Add necessary symbols to generated header
%{
#include<cdm/system/environment/SEEnvironmentalConditions.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
