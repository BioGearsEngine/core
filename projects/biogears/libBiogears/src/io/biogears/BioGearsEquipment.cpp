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
  // class SEAnesthesiaMachine
  void BiogearsEquipment::Marshall(const CDM::BioGearsAnesthesiaMachineData& in, AnesthesiaMachine& out)
  {
    io::Anesthesia::Marshall(in, out);

    out.BioGearsSystem::LoadState();

    out.m_inhaling = in.Inhaling();
    io::Property::Marshall(in.CurrentBreathingCycleTime(), out.m_currentbreathingCycleTime);
    io::Property::Marshall(in.InspirationTime(), out.m_inspirationTime);
    io::Property::Marshall(in.OxygenInletVolumeFraction(), out.m_O2InletVolumeFraction);
    io::Property::Marshall(in.TotalBreathingCycleTime(), out.m_totalBreathingCycleTime);
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::UnMarshall(const AnesthesiaMachine& in, CDM::BioGearsAnesthesiaMachineData& out)
  {
    io::Anesthesia::UnMarshall(in, out);

    out.Inhaling(in.m_inhaling);

    io::Property::UnMarshall(in.m_currentbreathingCycleTime, out.CurrentBreathingCycleTime());
    io::Property::UnMarshall(in.m_inspirationTime, out.InspirationTime());
    io::Property::UnMarshall(in.m_O2InletVolumeFraction, out.OxygenInletVolumeFraction());
    io::Property::UnMarshall(in.m_totalBreathingCycleTime, out.TotalBreathingCycleTime());
  }
  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineChamber
  void BiogearsEquipment::Marshall(const CDM::BioGearsElectroCardioGramData& in, ECG& out)
  {
    io::ElectroCardioGram::Marshall(in, out);

    out.BioGearsSystem::LoadState();

    io::Property::Marshall(in.HeartRythmTime(), out.m_HeartRhythmTime);
    io::Property::Marshall(in.HeartRythmPeriod(), out.m_HeartRhythmPeriod);
    io::ElectroCardioGram::Marshall(in.Waveforms(), out.m_Waveforms);

    out.m_Waveforms.SetLeadElectricPotential(3, out.GetLead3ElectricPotential());
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::UnMarshall(const ECG& in, CDM::BioGearsElectroCardioGramData& out)
  {
    io::ElectroCardioGram::UnMarshall(in, out);

    io::Property::UnMarshall(in.m_HeartRhythmTime, out.HeartRythmTime());
    io::Property::UnMarshall(in.m_HeartRhythmPeriod, out.HeartRythmPeriod());
    io::ElectroCardioGram::UnMarshall(in.m_Waveforms, out.Waveforms());
  }
  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineOxygenBottle
  void BiogearsEquipment::Marshall(const CDM::BioGearsInhalerData& in, biogears::Inhaler& out)
  {
    io::Inhaler::Marshall(in,out);
    out.BioGearsSystem::LoadState();
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::UnMarshall(const biogears::Inhaler& in, CDM::BioGearsInhalerData& out)
  {
    io::Inhaler::UnMarshall(in, out);
  }
  //-----------------------------------------------------------------------------
}
}