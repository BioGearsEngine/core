%module "biogears::SEScalarOsmolarity"

// Add necessary symbols to generated header
%{
#include<cdm/properties/SEScalarOsmolarity.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
