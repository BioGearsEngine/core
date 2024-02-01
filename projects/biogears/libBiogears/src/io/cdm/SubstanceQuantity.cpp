#include "SubstanceQuantity.h"

#include "Property.h"

#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>

#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
namespace io {
  //class SESubstanceQuantity
  void SubstanceQuantity::UnMarshall(const CDM::SubstanceQuantityData& in, SESubstanceQuantity& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::Marshall(const SESubstanceQuantity& in, CDM::SubstanceQuantityData& out)
  {
    out.Substance(in.m_Substance.GetName());
  }
  //class SEGasSubstanceQuantity
  void SubstanceQuantity::UnMarshall(const CDM::GasSubstanceQuantityData& in, SEGasSubstanceQuantity& out)
  {
    UnMarshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {

      io::Property::UnMarshall(in.PartialPressure(), out.GetPartialPressure());
      io::Property::UnMarshall(in.Volume(), out.GetVolume());
      io::Property::UnMarshall(in.VolumeFraction(), out.GetVolumeFraction());

    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::Marshall(SEGasSubstanceQuantity& in, CDM::GasSubstanceQuantityData& out)
  {
    Marshall(static_cast<const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PartialPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Volume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, VolumeFraction)
  }
  //-----------------------------------------------------------------------------
  // class SELiquidSubstanceQuantity
  void SubstanceQuantity::UnMarshall(const CDM::LiquidSubstanceQuantityData& in, SELiquidSubstanceQuantity& out)
  {
    UnMarshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {

      io::Property::UnMarshall(in.Concentration(), out.GetConcentration());
      io::Property::UnMarshall(in.Mass(), out.GetMass());
      io::Property::UnMarshall(in.MassCleared(), out.GetMassCleared());
      io::Property::UnMarshall(in.MassDeposited(), out.GetMassDeposited());
      io::Property::UnMarshall(in.MassExcreted(), out.GetMassExcreted());
      io::Property::UnMarshall(in.Molarity(), out.GetMolarity());
      io::Property::UnMarshall(in.Saturation(), out.GetSaturation());
      io::Property::UnMarshall(in.PartialPressure(), out.GetPartialPressure());
    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::Marshall(SELiquidSubstanceQuantity& in, CDM::LiquidSubstanceQuantityData& out)
  {
    Marshall(static_cast<const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Concentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Mass)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassCleared)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassDeposited)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassExcreted)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Molarity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Saturation)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PartialPressure)
  }
}
}