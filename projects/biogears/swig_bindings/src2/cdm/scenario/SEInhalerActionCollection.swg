%module "biogears::SEInhalerActionCollection"

// Add necessary symbols to generated header
%{
#include<cdm/scenario/SEInhalerActionCollection.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
