%module "biogears::SESubstancePharmacodynamics"

// Add necessary symbols to generated header
%{
#include<cdm/substance/SESubstancePharmacodynamics.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
