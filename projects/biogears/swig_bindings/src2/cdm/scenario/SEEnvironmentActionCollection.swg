%module "biogears::SEEnvironmentActionCollection"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEEnvironmentActionCollection.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
