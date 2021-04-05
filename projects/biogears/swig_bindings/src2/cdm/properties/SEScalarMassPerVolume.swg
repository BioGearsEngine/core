%module "biogears::SEScalarMassPerVolume"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarMassPerVolume.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
