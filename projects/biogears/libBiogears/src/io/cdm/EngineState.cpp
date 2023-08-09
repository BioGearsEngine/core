#include "EngineState.h"

#include <biogears/cdm/engine/PhysiologyEngine.h>

#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

OPEN_BIOGEARS_NAMESPACE
namespace io {

  //TODO: Implement thee as LoadState/SaveState
  void EngineState::Marshall(const CDM::PhysiologyEngineStateData& in, PhysiologyEngine& out)
  {
    return;
  }
  //----------------------------------------------------------------------------------
  void EngineState::UnMarshall(const PhysiologyEngine& in, CDM::PhysiologyEngineStateData& out)
  {
    return;
  }
  //----------------------------------------------------------------------------------
}
  CLOSE_BIOGEARS_NAMESPACE