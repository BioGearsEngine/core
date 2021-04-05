%module "biogears::SEComprehensiveMetabolicPanel"

// Add necessary symbols to generated header
%{
#include<cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
