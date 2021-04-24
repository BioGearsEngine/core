%module "biogears::SEInhalerConfiguration"

// Add necessary symbols to generated header
%{
#include<cdm/system/equipment/Inhaler/actions/SEInhalerConfiguration.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
