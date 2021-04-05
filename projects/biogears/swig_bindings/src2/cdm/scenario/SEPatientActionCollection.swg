%module "biogears::SEPatientActionCollection"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEPatientActionCollection.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
