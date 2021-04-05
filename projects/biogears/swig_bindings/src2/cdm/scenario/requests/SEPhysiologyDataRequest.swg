%module "biogears::SEPhysiologyDataRequest"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/requests/SEPhysiologyDataRequest.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
