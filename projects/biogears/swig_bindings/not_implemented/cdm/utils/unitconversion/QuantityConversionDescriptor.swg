%module "biogears::QuantityConversionDescriptor"

// Add necessary symbols to generated header
%{
#include<cdm/utils/unitconversion/QuantityConversionDescriptor.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
