%module "biogears::SEDataRequest"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/requests/SEDataRequest.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
