%module(directors="1")SWIG_SEEventHandler

%{
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEEventHandler;

%ignore BIOGEARS_NAMESPACE SEBreathHold::Unload;
%ignore BIOGEARS_NAMESPACE SEBreathHold::Load;

%feature("director") SEEventHandler; 
%include <biogears/cdm/utils/SEEventHandler.h>
%feature("director") SEEventHandler; 