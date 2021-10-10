%module "biogears::SETestSuite"

// Add necessary symbols to generated header
%{
#include<cdm/utils/testing/SETestSuite.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
