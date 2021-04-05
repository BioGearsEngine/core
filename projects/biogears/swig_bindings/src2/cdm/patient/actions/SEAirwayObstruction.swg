%module "biogears::SEAirwayObstruction"

// Add necessary symbols to generated header
%{
#include<cdm/patient/actions/SEAirwayObstruction.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>
