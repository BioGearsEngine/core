%module "biogears::TaskRunner"

// Add necessary symbols to generated header
%{
#include<cdm/utils/TaskRunner/TaskRunner.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
