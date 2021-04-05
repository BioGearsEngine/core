%module "biogears::SEScalarVolumePerPressure"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarVolumePerPressure.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
