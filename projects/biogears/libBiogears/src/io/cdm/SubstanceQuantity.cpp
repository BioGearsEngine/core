#include "SubstanceQuantity.h"

#include "Property.h"

#include <biogears/cdm/compartment/substances/SESubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SEGasSubstanceQuantity.h>
#include <biogears/cdm/compartment/substances/SELiquidSubstanceQuantity.h>

#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>

namespace biogears {
namespace io {
  //class SESubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::SubstanceQuantityData& in, SESubstanceQuantity& out)
  {
    out.Clear();
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(const SESubstanceQuantity& in, CDM::SubstanceQuantityData& out)
  {
    out.Substance(in.m_Substance.GetName());
  }
  //class SEGasSubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::GasSubstanceQuantityData& in, SEGasSubstanceQuantity& out)
  {
    Marshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {
      if (in.PartialPressure().present()) {
        io::Property::Marshall(in.PartialPressure(), out.GetPartialPressure());
      }
      if (in.Volume().present()) {
        io::Property::Marshall(in.Volume(), out.GetVolume());
      }
      if (in.VolumeFraction().present()) {
        io::Property::Marshall(in.VolumeFraction(), out.GetVolumeFraction());
      }
    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(SEGasSubstanceQuantity& in, CDM::GasSubstanceQuantityData& out)
  {
    UnMarshall(static_cast < const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...
    if (in.HasPartialPressure()) {
      io::Property::UnMarshall(in.GetPartialPressure(), out.PartialPressure());
    }
    if (in.HasVolume()) {
      io::Property::UnMarshall(in.GetVolume(), out.Volume());
    }
    if (in.HasVolumeFraction()) {
      io::Property::UnMarshall(in.GetVolumeFraction(), out.VolumeFraction());
    }
  }
  //-----------------------------------------------------------------------------
  //class SELiquidSubstanceQuantity
  void SubstanceQuantity::Marshall(const CDM::LiquidSubstanceQuantityData& in, SELiquidSubstanceQuantity& out)
  {
    Marshall(static_cast<const CDM::SubstanceQuantityData&>(in), static_cast<SESubstanceQuantity&>(out));
    if (!out.m_Compartment.HasChildren()) {
      if (in.Concentration().present())
        io::Property::Marshall(in.Concentration(), out.GetConcentration());
      if (in.Mass().present())
        io::Property::Marshall(in.Mass(), out.GetMass());
      if (in.MassCleared().present())
        io::Property::Marshall(in.MassCleared(), out.GetMassCleared());
      if (in.MassDeposited().present())
        io::Property::Marshall(in.MassDeposited(), out.GetMassDeposited());
      if (in.MassExcreted().present())
        io::Property::Marshall(in.MassExcreted(), out.GetMassExcreted());
      if (in.Molarity().present())
        io::Property::Marshall(in.Molarity(), out.GetMolarity());
      if (in.PartialPressure().present())
        io::Property::Marshall(in.PartialPressure(), out.GetPartialPressure());
      if (in.Saturation().present())
        io::Property::Marshall(in.Saturation(), out.GetSaturation());
    }
  }
  //-----------------------------------------------------------------------------
  void SubstanceQuantity::UnMarshall(SELiquidSubstanceQuantity& in, CDM::LiquidSubstanceQuantityData& out)
  {
    UnMarshall(static_cast<const SESubstanceQuantity&>(in), static_cast<CDM::SubstanceQuantityData&>(out));
    // Even if you have children, I am unloading everything, this makes the xml actually usefull...
    if (in.HasConcentration())
      io::Property::UnMarshall(in.GetConcentration(), out.Concentration());
    if (in.HasMass())
      io::Property::UnMarshall(in.GetMass(), out.Mass());
    if (in.m_MassCleared != nullptr)
      io::Property::UnMarshall(*in.m_MassCleared, out.MassCleared());
    if (in.m_MassDeposited != nullptr)
      io::Property::UnMarshall(*in.m_MassDeposited, out.MassDeposited());
    if (in.m_MassExcreted != nullptr)
      io::Property::UnMarshall(*in.m_MassExcreted, out.MassExcreted());
    if (in.HasMolarity())
      io::Property::UnMarshall(in.GetMolarity(), out.Molarity());
    if (in.HasPartialPressure())
      io::Property::UnMarshall(in.GetPartialPressure(), out.PartialPressure());
    if (in.HasSaturation())
      io::Property::UnMarshall(in.GetSaturation(), out.Saturation());
  }
}
}