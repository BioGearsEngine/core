%module "biogears::SEPatientDataRequest"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/requests/SEPatientDataRequest.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
