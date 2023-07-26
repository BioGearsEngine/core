%{
#include <biogears/cdm/utils/DataTrack.h>
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
) DataTrack
#endif

%include <biogears/exports.swg>
%include <biogears/config.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/substance/SESubstance.swg>
%import <biogears/config.h>

#ifndef BIOGEARS_WITHOUT_NAMESPACE
%nspace BIOGEARS_NAMESPACE DataTrack;
#endif 


OPEN_BIOGEARS_NAMESPACE
  class DataTrack{
    DataTrack(Logger* m_Log);
    double GetTrack(const std::string& name, double time);
  };
CLOSE_BIOGEARS_NAMESPACE
