%{
#include <biogears/cdm/utils/DataTrack.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/substance/SESubstance.swg>

%nspace biogears::DataTrack;

namespace biogears{
  class DataTrack{
    DataTrack(Logger* m_Log);
    double GetTrack(const std::string& name, double time);
  };
}
