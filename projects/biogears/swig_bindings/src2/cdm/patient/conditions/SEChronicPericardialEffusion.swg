%module "biogears::SEChronicPericardialEffusion"

// Add necessary symbols to generated header
%{
#include<cdm/patient/conditions/SEChronicPericardialEffusion.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
