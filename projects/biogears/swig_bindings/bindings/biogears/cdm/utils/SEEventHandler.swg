%module(directors="1")SWIG_SEEventHandler

%{
#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

#if defined(SWIGPYTHON)
%module(package="biogears", 
moduleimport=
   "# Import the low-level C/C++ module\n"
    "if __package__ or \".\" in __name__:\n"
    "    from . import _biogears\n"
    "else:\n"
    "    import _biogears\n"
) SEEventHandler
#endif

%include <biogears/exports.swg>

%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <biogears/cdm/properties/SEScalarTime.swg>

%nspace BIOGEARS_NAMESPACE SEEventHandler;

%ignore BIOGEARS_NAMESPACE SEBreathHold::Unload;
%ignore BIOGEARS_NAMESPACE SEBreathHold::Load;

%feature("director") SEEventHandler; 
%include <biogears/cdm/utils/SEEventHandler.h>
%feature("director") SEEventHandler; 