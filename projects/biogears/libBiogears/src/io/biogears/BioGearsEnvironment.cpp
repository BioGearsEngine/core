#include "BioGearsEnvironment.h"

#include <memory>

#include <biogears/cdm/system/SESystem.h>
#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/engine/Systems/Environment.h>

#include "../cdm/Environment.h"
#include "../cdm/System.h"

namespace biogears {
namespace io {
  std::unique_ptr<SESystem> BiogearsEnvironment::factory(CDM::SystemData const* systemData, biogears::BioGears& bgData)
  {
    if (auto environmentData = dynamic_cast<CDM::BioGearsEnvironmentData const*>(systemData)) {
      auto environment = biogears::Environment::make_unique(bgData);
      UnMarshall(*environmentData, *environment);
      return environment;
    }


    throw biogears::CommonDataModelException("BioGearsPhysiology:Factory - Unsupported BioGearsSystem Received.");
  }

  std::unique_ptr<CDM::SystemData> BiogearsEnvironment::factory(const SESystem* system)
  {
    if (auto biogearsSystem = dynamic_cast<biogears::Environment const*>(system)) {
      auto systemData = std::make_unique<CDM::BioGearsEnvironmentData>();
      Marshall(*biogearsSystem, *systemData);
      return systemData;
    }

    throw biogears::CommonDataModelException("BioGearsPhysiology:Factory - Unsupported BioGearsSystemData Received.");
  }
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