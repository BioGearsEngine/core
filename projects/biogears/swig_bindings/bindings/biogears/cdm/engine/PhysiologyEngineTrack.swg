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
%import <biogears/cdm/properties/SEGenericScalar.swg>
%import <biogears/cdm/utils/DataTrack.swg>
%import <biogears/cdm/scenario/requests/SEDataRequestManager.swg>

%import <biogears/cdm/engine/PhysiologyEngine.h>

%nspace biogears::PhysiologyEngineTrack;
%nspace biogears::SEDataRequestScalar;

#if defined(SWIGCSHARP)
%csmethodmodifiers biogears::SEDataRequestScalar::ToString() const "public override";
#endif 

namespace biogears{
   class SEDataRequestScalar : public SEGenericScalar {
   public:
     SEDataRequestScalar(Logger* logger)
       : SEGenericScalar(logger){}

     void UpdateScalar();
     void SetScalar(const SEScalar* s, SEDataRequest& dr);
     std::string ToString() const;
   };

   class PhysiologyEngine;
   class PhysiologyEngineTrack {
      public:
      PhysiologyEngineTrack( PhysiologyEngine& eng);
      DataTrack& GetDataTrack();
      SEDataRequestManager& GetDataRequestManager() { return m_DataRequestMgr; }

      bool ConnectRequest(SEDataRequest& dr, SEDataRequestScalar& ds);

      SEDataRequestScalar* GetScalar(SEDataRequest* dr); 
      void SetupRequests(bool append = false);
      void TrackData(double currentTime_s, bool append = false);
      void PullData();
      bool TrackRequest(SEDataRequest& dr);
      void ForceConnection();

   };
}
