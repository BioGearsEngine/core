%module "biogears::DataPeak"

// Add necessary symbols to generated header
%{
#include<cdm/utils/DataPeak.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
