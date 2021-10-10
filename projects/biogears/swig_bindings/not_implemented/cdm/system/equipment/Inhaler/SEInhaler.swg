%module "biogears::SEInhaler"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Inhaler/SEInhaler.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
