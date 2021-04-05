%module "biogears::SEEventHandler"

// Add necessary symbols to generated header
%{
#include<cdm/utils/SEEventHandler.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
