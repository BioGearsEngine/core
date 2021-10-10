#include "System.h"

#include <biogears/cdm/system/SESystem.h>

namespace biogears {
namespace io {
  //class SESystem
  void System::Marshall(const CDM::SystemData& in, SESystem& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void System::UnMarshall(const SESystem& in, CDM::SystemData& out)
  {
  }
  //-----------------------------------------------------------------------------
}
}