#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/utils/Logger.h>

namespace biogears {

PhysiologyEngine::PhysiologyEngine(Logger const* log)
  : Loggable(log, Loggable::empty)
{
}
PhysiologyEngine::PhysiologyEngine()
  : Loggable()
{
}
PhysiologyEngine::~PhysiologyEngine()
{
}

} // namespace biogears
