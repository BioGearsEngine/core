%module "biogears::FileUtils"

// Add necessary symbols to generated header
%{
#include<cdm/utils/FileUtils.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
