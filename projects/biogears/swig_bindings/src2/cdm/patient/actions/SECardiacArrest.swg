%module "biogears::SECardiacArrest"

// Add necessary symbols to generated header
%{
#include<cdm/patient/actions/SECardiacArrest.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
