%{
#include  <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>

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
) PhysiologyEngineTrack
#endif

%include <biogears/exports.swg>

%import <biogears/container/Tree.swg>
%import <biogears/cdm/utils/Logger.swg>
%import <biogears/cdm/CommonDataModel.swg>

%import <std_map.i>

%import <biogears/cdm/properties/SEScalar.swg>
%import <biogears/cdm/properties/SEGenericScalar.swg>
%import <biogears/cdm/utils/DataTrack.swg>
%import <biogears/cdm/scenario/requests/SEDataRequestManager.swg>

%import <biogears/cdm/engine/PhysiologyEngine.h>

%nspace BIOGEARS_NAMESPACE PhysiologyEngineTrack;
%nspace BIOGEARS_NAMESPACE SEDataRequestScalar;

#if defined(SWIGCSHARP)
%csmethodmodifiers BIOGEARS_NAMESPACE SEDataRequestScalar::ToString() const "public override";
#endif 

OPEN_BIOGEARS_NAMESPACE 

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

CLOSE_BIOGEARS_NAMESPACE
