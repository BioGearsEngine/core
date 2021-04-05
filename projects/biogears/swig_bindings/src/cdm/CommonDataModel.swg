%module(package="biogears") "CommonDataModel"

// Add necessary symbols to generated header
%{
#include <exception>
#include<biogears/cdm/CommonDataModel.h>
%}
%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>

%include <exception.i>
%nspace "biogears::CommonDataModelException"

%include <biogears/cdm/CommonDataModel.h>
