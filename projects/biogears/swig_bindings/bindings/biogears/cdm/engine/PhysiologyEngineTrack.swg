%{
#include  <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>

%}

%include <biogears/exports.h>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%include <std_map.i>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/utils/DataTrack.swg>
%import <biogears/cdm/scenario/requests/SEDataRequestManager.swg>

%import <biogears/cdm/engine/PhysiologyEngine.h>
%nspace biogears::PhysiologyEngineTrack;

namespace biogears{
   class PhysiologyEngine;
   class PhysiologyEngineTrack {
      public:
      PhysiologyEngineTrack( PhysiologyEngine& eng);
      DataTrack& GetDataTrack();
      SEDataRequestManager& GetDataRequestManager() { return m_DataRequestMgr; }
   };
}
