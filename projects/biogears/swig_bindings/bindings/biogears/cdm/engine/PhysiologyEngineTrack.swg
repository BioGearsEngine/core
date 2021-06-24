%{
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%include <std_map.i>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/engine/PhysiologyEngine.swg>
%import <biogears/cdm/utils/DataTrack.swg>
%import <biogears/cdm/scenario/requests/SEDataRequestManager.swg>

%nspace biogears::PhysiologyEngineTrack;

namespace biogears {
  class PhysiologyEngineTrack {
  public: 
    PhysiologyEngineTrack(PhysiologyEngine& engine);
    DataTrack& GetDataTrack();
    SEDataRequestManager& GetDataRequestManager();
  };
}
