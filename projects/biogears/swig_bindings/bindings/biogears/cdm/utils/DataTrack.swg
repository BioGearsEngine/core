%{
#include <biogears/cdm/utils/DataTrack.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
%}

%include <biogears/exports.h>
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
