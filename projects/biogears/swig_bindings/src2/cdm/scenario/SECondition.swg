%module "biogears::SECondition"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SECondition.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
