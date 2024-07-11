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
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsAnesthesiaMachineData& in, AnesthesiaMachine& out)
  {
    io::Anesthesia::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    out.m_inhaling = in.Inhaling();
    io::Property::UnMarshall(in.CurrentBreathingCycleTime(), out.m_currentbreathingCycleTime);
    io::Property::UnMarshall(in.InspirationTime(), out.m_inspirationTime);
    io::Property::UnMarshall(in.OxygenInletVolumeFraction(), out.m_O2InletVolumeFraction);
    io::Property::UnMarshall(in.TotalBreathingCycleTime(), out.m_totalBreathingCycleTime);
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::Marshall(const AnesthesiaMachine& in, CDM::BioGearsAnesthesiaMachineData& out)
  {
    io::Anesthesia::Marshall(in, out);

    out.Inhaling(in.m_inhaling);

    io::Property::Marshall(in.m_currentbreathingCycleTime, out.CurrentBreathingCycleTime());
    io::Property::Marshall(in.m_inspirationTime, out.InspirationTime());
    io::Property::Marshall(in.m_O2InletVolumeFraction, out.OxygenInletVolumeFraction());
    io::Property::Marshall(in.m_totalBreathingCycleTime, out.TotalBreathingCycleTime());
  }
  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineChamber
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsElectroCardioGramData& in, ECG& out)
  {
    io::ElectroCardioGram::UnMarshall(in, out);

    out.BioGearsSystem::LoadState();

    io::Property::UnMarshall(in.HeartRythmTime(), out.m_HeartRhythmTime);
    io::Property::UnMarshall(in.HeartRythmPeriod(), out.m_HeartRhythmPeriod);
    io::ElectroCardioGram::UnMarshall(in.Waveforms(), out.m_Waveforms);

    out.m_Waveforms.SetLeadElectricPotential(3, out.GetLead3ElectricPotential());
  }
  //-----------------------------------------------------------------------------
  void BiogearsEquipment::Marshall(const ECG& in, CDM::BioGearsElectroCardioGramData& out)
  {
    io::ElectroCardioGram::Marshall(in, out);

    io::Property::Marshall(in.m_HeartRhythmTime, out.HeartRythmTime());
    io::Property::Marshall(in.m_HeartRhythmPeriod, out.HeartRythmPeriod());
    io::ElectroCardioGram::Marshall(in.m_Waveforms, out.Waveforms());
  }
  //-----------------------------------------------------------------------------
  // class SEAnesthesiaMachineOxygenBottle
  void BiogearsEquipment::UnMarshall(const CDM::BioGearsInhalerData& in, biogears::Inhaler& out)
  {
    io::Inhaler::UnMarshall(in,out);
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