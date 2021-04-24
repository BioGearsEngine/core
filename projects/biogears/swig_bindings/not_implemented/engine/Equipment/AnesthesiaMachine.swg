%module "biogears::AnesthesiaMachine"

// Add necessary symbols to generated header
%{
#include<engine/Equipment/AnesthesiaMachine.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
