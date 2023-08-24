#include "SubstanceQuantity.h"

#include "Property.h"

#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>

#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
namespace io {
  // class SESubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::SubstanceQuantityData& in, SESubstanceQuantity& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(const SESubstanceQuantity& in, CDM::SubstanceQuantityData& out)
  {
    out.Substance(in.m_Substance.GetName());
  }
  // class SEGasSubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::GasSubstanceQuantityData& in, SEGasSubstanceQuantity& out)
  {
    Marshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {
      io::Property::Marshall(in.PartialPressure(), out.GetPartialPressure());
      io::Property::Marshall(in.Volume(), out.GetVolume());
      io::Property::Marshall(in.VolumeFraction(), out.GetVolumeFraction());
    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(SEGasSubstanceQuantity& in, CDM::GasSubstanceQuantityData& out)
  {
    UnMarshall(static_cast<const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PartialPressure)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Volume)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, VolumeFraction)
  }
  //-----------------------------------------------------------------------------
  // class SELiquidSubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::LiquidSubstanceQuantityData& in, SELiquidSubstanceQuantity& out)
  {
    Marshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {
      io::Property::Marshall(in.Concentration(), out.GetConcentration());
      io::Property::Marshall(in.Mass(), out.GetMass());
      io::Property::Marshall(in.MassCleared(), out.GetMassCleared());
      io::Property::Marshall(in.MassDeposited(), out.GetMassDeposited());
      io::Property::Marshall(in.MassExcreted(), out.GetMassExcreted());
      io::Property::Marshall(in.Molarity(), out.GetMolarity());
      io::Property::Marshall(in.Saturation(), out.GetSaturation());
      io::Property::Marshall(in.PartialPressure(), out.GetPartialPressure());
    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(SELiquidSubstanceQuantity& in, CDM::LiquidSubstanceQuantityData& out)
  {
    UnMarshall(static_cast<const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Concentration)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Mass)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassCleared)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassDeposited)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassExcreted)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Molarity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Saturation)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PartialPressure)
  }
}
}