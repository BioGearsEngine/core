%module "biogears::SEInhalerAction"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Inhaler/actions/SEInhalerAction.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
