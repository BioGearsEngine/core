%module(package="biogears") "Logger"
// Add necessary symbols to generated header
%{
#include <biogears/cdm/utils/Logger.h>
%}

%include <windows.i>
%include <std_string.i>
%include <std_vector.i>
%include <biogears/exports.h>

%nspace "biogears::Logger";
%nspace "biogears::Loggable";
%nspace "biogears::LoggerForward";

%include <biogears/cdm/utils/Logger.h>

