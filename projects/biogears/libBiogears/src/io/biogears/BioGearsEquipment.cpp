#include "BioGearsEquipment.h"

#include <memory>

#include <biogears/engine/Equipment/AnesthesiaMachine.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/engine/Equipment/Inhaler.h>

#include "../cdm/Anesthesia.h"
#include "../cdm/ElectroCardioGram.h"
#include "../cdm/Inhaler.h"
#include "../cdm/Property.h"

namespace biogears {
namespace io {
  std::unique_ptr<SESystem> BiogearsEquipment::factory(CDM::SystemData const* systemData, biogears::BioGears& bgData)
  {
    if (auto environmentData = dynamic_cast<CDM::BioGearsAnesthesiaMachineData const*>(systemData)) {
      auto environment = biogears::AnesthesiaMachine::make_unique(bgData);
      UnMarshall(*environmentData, *environment);
      return environment;
    }

    if (auto environmentData = dynamic_cast<CDM::BioGearsElectroCardioGramData const*>(systemData)) {
      auto environment = biogears::ECG::make_unique(bgData);
      UnMarshall(*environmentData, *environment);
      return environment;
    }

    if (auto environmentData = dynamic_cast<CDM::BioGearsInhalerData const*>(systemData)) {
      auto environment = biogears::Inhaler::make_unique(bgData);
      UnMarshall(*environmentData, *environment);
      return environment;
    }

    throw biogears::CommonDataModelException("BioGearsPhysiology:Factory - Unsupported BioGearsSystem Received.");
  }

  std::unique_ptr<CDM::SystemData> BiogearsEquipment::factory(const SESystem* system)
  {
    if (auto biogearsSystem = dynamic_cast<biogears::AnesthesiaMachine const*>(system)) {
      auto systemData = std::make_unique<CDM::BioGearsAnesthesiaMachineData>();
      Marshall(*biogearsSystem, *systemData);
      return systemData;
    }

    if (auto biogearsSystem = dynamic_cast<biogears::ECG const*>(system)) {
      auto systemData = std::make_unique<CDM::BioGearsElectroCardioGramData>();
      Marshall(*biogearsSystem, *systemData);
      return systemData;
    }

    if (auto biogearsSystem = dynamic_cast<biogears::Inhaler const*>(system)) {
      auto systemData = std::make_unique<CDM::BioGearsInhalerData>();
      Marshall(*biogearsSystem, *systemData);
      return systemData;
    }

    throw biogears::CommonDataModelException("BioGearsPhysiology:Factory - Unsupported BioGearsSystemData Received.");
  }

  // class SEAnesthesiaMachine
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsAnesthesiaMachineData& in, AnesthesiaMachine& out)
  {
    io::Anesthesia::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    out.m_Inhaling = in.Inhaling();
    io::Property::UnMarshall(in.CurrentBreathingCycleTime(), out.m_CurrentBreathingCycleTime);
    io::Property::UnMarshall(in.InspirationTime(), out.m_InspirationTime);
    io::Property::UnMarshall(in.OxygenInletVolumeFraction(), out.m_OxygenInletVolumeFraction);
    io::Property::UnMarshall(in.TotalBreathingCycleTime(), out.m_TotalBreathingCycleTime);
  }

  //-----------------------------------------------------------------------------
  void BiogearsEquipment::Marshall(const AnesthesiaMachine& in, CDM::BioGearsAnesthesiaMachineData& out)
  {
    io::Anesthesia::Marshall(in, out);

    out.Inhaling(in.m_Inhaling);

    if (in.m_CurrentBreathingCycleTime.IsValid()) {
      out.CurrentBreathingCycleTime(std::make_unique<std::remove_reference<decltype(out.CurrentBreathingCycleTime())>::type>());
      io::Property::Marshall(in.m_CurrentBreathingCycleTime, out.CurrentBreathingCycleTime());
    }

    if (in.m_InspirationTime.IsValid()) {
      out.InspirationTime(std::make_unique<std::remove_reference<decltype(out.InspirationTime())>::type>());
      io::Property::Marshall(in.m_InspirationTime, out.InspirationTime());
    }

    if (in.m_OxygenInletVolumeFraction.IsValid()) {
      out.OxygenInletVolumeFraction(std::make_unique<std::remove_reference<decltype(out.OxygenInletVolumeFraction())>::type>());
      io::Property::Marshall(in.m_OxygenInletVolumeFraction, out.OxygenInletVolumeFraction());
    }

    if (in.m_TotalBreathingCycleTime.IsValid()) {
      out.TotalBreathingCycleTime(std::make_unique<std::remove_reference<decltype(out.TotalBreathingCycleTime())>::type>());
      io::Property::Marshall(in.m_TotalBreathingCycleTime, out.TotalBreathingCycleTime());
    }
  };

  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineChamber
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsElectroCardioGramData& in, ECG& out)
  {
    io::ElectroCardioGram::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    io::Property::UnMarshall(in.HeartRhythmTime(), out.m_HeartRhythmTime);
    io::Property::UnMarshall(in.HeartRhythmPeriod(), out.m_HeartRhythmPeriod);
    io::ElectroCardioGram::UnMarshall(in.Waveforms(), out.m_Waveforms);

    out.m_Waveforms.SetLeadElectricPotential(3, out.GetLead3ElectricPotential());
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::Marshall(const ECG& in, CDM::BioGearsElectroCardioGramData& out)
  {
    io::ElectroCardioGram::Marshall(in, out);

    if ( in.m_HeartRhythmTime.IsValid()) {
      out.HeartRhythmTime(std::make_unique<std::remove_reference<decltype(out.HeartRhythmTime())>::type>());
      io::Property::Marshall(in.m_HeartRhythmTime, out.HeartRhythmTime());
    };
    if (in.m_HeartRhythmPeriod.IsValid()) {
      out.HeartRhythmPeriod(std::make_unique<std::remove_reference<decltype(out.HeartRhythmPeriod())>::type>());
      io::Property::Marshall(in.m_HeartRhythmPeriod, out.HeartRhythmPeriod());
    };
  
    out.Waveforms(std::make_unique<std::remove_reference<decltype(out.Waveforms())>::type>());
    io::ElectroCardioGram::Marshall(in.m_Waveforms, out.Waveforms());
    

  }
  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineOxygenBottle
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsInhalerData& in, biogears::Inhaler& out)
  {
    io::Inhaler::UnMarshall(in, out);
    out.BioGearsSystem::LoadState();
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::Marshall(const biogears::Inhaler& in, CDM::BioGearsInhalerData& out)
  {
    io::Inhaler::Marshall(in, out);
  }
  //-----------------------------------------------------------------------------
}
}