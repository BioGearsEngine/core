%module "biogears::SEDataRequestManager"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/requests/SEDataRequestManager.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
