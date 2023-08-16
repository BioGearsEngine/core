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

      io::Property::Marshall(in.MeteredDose(), out.GetMeteredDose());
      io::Property::Marshall(in.NozzleLoss(), out.GetNozzleLoss());
      io::Property::Marshall(in.SpacerVolume(), out.GetSpacerVolume());

    if (in.Substance().present())
      out.SetSubstance(out.m_Substances.GetSubstance(in.Substance().get()));

    out.StateChange();
  }
  //----------------------------------------------------------------------------------
  void Inhaler::UnMarshall(const SEInhaler& in, CDM::InhalerData& out)
  {
    CDM_ENUM_UNMARSHAL_HELPER(in,out,State)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeteredDose)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, NozzleLoss)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SpacerVolume)
    if (in.HasSubstance())
      out.Substance(in.m_Substance->GetName());
  }
  //----------------------------------------------------------------------------------
}
}