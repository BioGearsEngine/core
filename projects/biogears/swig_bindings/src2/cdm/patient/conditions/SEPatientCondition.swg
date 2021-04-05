%module "biogears::SEPatientCondition"

// Add necessary symbols to generated header
%{
#include<cdm/patient/conditions/SEPatientCondition.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
