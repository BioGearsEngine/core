#include "BioGearsEnvironment.h"

#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/engine/Systems/Environment.h>

#include "../cdm/Environment.h"
#include "../cdm/System.h"

namespace biogears {
namespace io {
  // class SEAnesthesiaMachine
  void BiogearsEnvironment::UnMarshall(const CDM::BioGearsEnvironmentData& in, biogears::Environment& out)
  {
    io::Environment::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    out.m_PatientEquivalentDiameter_m = in.PatientEquivalentDiameter_m();
    out.StateChange();
  }
  void BiogearsEnvironment::Marshall(const biogears::Environment& in, CDM::BioGearsEnvironmentData& out)
  {
    io::Environment::Marshall(in, out);
    out.PatientEquivalentDiameter_m(in.m_PatientEquivalentDiameter_m);
  }
}
}