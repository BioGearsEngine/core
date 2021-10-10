#include "Inhaler.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarVolume.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/equipment/Inhaler/SEInhaler.h>

namespace biogears {
namespace io {
  void Inhaler::Marshall(const CDM::InhalerData& in, SEInhaler& out)
  {
    out.Clear();
    if (in.State().present())
      out.SetState(in.State().get());
    if (in.MeteredDose().present())
      io::Property::Marshall(in.MeteredDose(), out.GetMeteredDose());
    if (in.NozzleLoss().present())
      io::Property::Marshall(in.NozzleLoss(), out.GetNozzleLoss());
    if (in.SpacerVolume().present())
      io::Property::Marshall(in.SpacerVolume(), out.GetSpacerVolume());
    if (in.Substance().present())
      out.SetSubstance(out.m_Substances.GetSubstance(in.Substance().get()));
    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Inhaler::UnMarshall(const SEInhaler& in, CDM::InhalerData& out)
  {
    if (in.HasState())
      out.State(in.m_State);
    if (in.HasMeteredDose())
      io::Property::UnMarshall(*in.m_MeteredDose, out.MeteredDose());
    if (in.HasNozzleLoss())
      io::Property::UnMarshall(*in.m_NozzleLoss, out.NozzleLoss());
    if (in.HasSpacerVolume())
      io::Property::UnMarshall(*in.m_SpacerVolume, out.SpacerVolume());
    if (in.HasSubstance())
      out.Substance(in.m_Substance->GetName());
  }
  //----------------------------------------------------------------------------------
}
}