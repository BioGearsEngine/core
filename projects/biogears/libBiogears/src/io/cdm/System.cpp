#include "System.h"

#include <biogears/cdm/system/SESystem.h>

OPEN_BIOGEARS_NAMESPACE
namespace io {
  //class SESystem
  void System::Marshall(const CDM::SystemData& in, SESystem& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void System::UnMarshall(const SESystem& in, CDM::SystemData& out)
  {
      //Nothing todo.
  }
  //-----------------------------------------------------------------------------
}
  CLOSE_BIOGEARS_NAMESPACE