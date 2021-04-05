%module "biogears::SEChronicRenalStenosis"

// Add necessary symbols to generated header
%{
#include<cdm/patient/conditions/SEChronicRenalStenosis.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
