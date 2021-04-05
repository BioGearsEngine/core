%module "biogears::SEDiabetesType2"

// Add necessary symbols to generated header
%{
#include<cdm/patient/conditions/SEDiabetesType2.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
