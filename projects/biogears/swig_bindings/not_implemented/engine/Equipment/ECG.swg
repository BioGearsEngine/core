%module "biogears::ECG"

// Add necessary symbols to generated header
%{
#include<engine/Equipment/ECG.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
