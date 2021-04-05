%module "biogears::SEScalarMassPerMass"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarMassPerMass.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
