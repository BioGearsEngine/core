%module "biogears::SEConsumeNutrients"

// Add necessary symbols to generated header
%{
#include<cdm/patient/actions/SEConsumeNutrients.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
