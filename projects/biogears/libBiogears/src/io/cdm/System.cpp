#include "System.h"

#include <biogears/cdm/system/SESystem.h>

namespace biogears {
namespace io {
  //class SESystem
  void System::UnMarshall(const CDM::SystemData& in, SESystem& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void System::Marshall(const SESystem& in, CDM::SystemData& out)
  {
  }
  //-----------------------------------------------------------------------------
}
}