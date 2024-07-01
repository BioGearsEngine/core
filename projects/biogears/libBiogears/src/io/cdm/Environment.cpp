#include "Environment.h"

#include "Environment.h"
#include "EnvironmentConditions.h"
#include "Property.h"
#include "Substance.h"
#include "System.h"

#include <biogears/cdm/properties/SEScalarArea.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarHeatConductancePerArea.h>
#include <biogears/cdm/properties/SEScalarHeatResistance.h>
#include <biogears/cdm/properties/SEScalarHeatResistanceArea.h>
#include <biogears/cdm/properties/SEScalarLengthPerTime.h>
#include <biogears/cdm/properties/SEScalarPower.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>

#include <biogears/cdm/system/environment/SEActiveCooling.h>
#include <biogears/cdm/system/environment/SEActiveHeating.h>
#include <biogears/cdm/system/environment/SEAppliedTemperature.h>
#include <biogears/cdm/system/environment/SEEnvironment.h>
#include <biogears/cdm/system/environment/conditions/SEEnvironmentCondition.h>
#include <biogears/cdm/system/environment/conditions/SEInitialEnvironment.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>

namespace biogears {
namespace io {
  // class SEEnvironmentalConditions
  void Environment::UnMarshall(const CDM::EnvironmentalConditionsData& in, SEEnvironmentalConditions& out)
  {
    out.Clear();
    if (in.Name().present()) {
      out.SetName(in.Name().get());
    }

    io::Environment::UnMarshall(in.SurroundingType(), out.m_SurroundingType);

    io::Property::UnMarshall(in.AirDensity(), out.GetAirDensity());
    io::Property::UnMarshall(in.AirVelocity(), out.GetAirVelocity());
    io::Property::UnMarshall(in.AmbientTemperature(), out.GetAmbientTemperature());
    io::Property::UnMarshall(in.AtmosphericPressure(), out.GetAtmosphericPressure());
    io::Property::UnMarshall(in.ClothingResistance(), out.GetClothingResistance());
    io::Property::UnMarshall(in.Emissivity(), out.GetEmissivity());
    io::Property::UnMarshall(in.MeanRadiantTemperature(), out.GetMeanRadiantTemperature());
    io::Property::UnMarshall(in.RelativeHumidity(), out.GetRelativeHumidity());
    io::Property::UnMarshall(in.RespirationAmbientTemperature(), out.GetRespirationAmbientTemperature());

    SESubstance* sub;
    for (const CDM::SubstanceFractionData& sfData : in.AmbientGas()) {
      sub = out.m_Substances.GetSubstance(sfData.Name());
      if (sub == nullptr) {
        throw CommonDataModelException("Substance not found : " + sfData.Name());
      }
      if (sub->GetState() != SESubstanceState::Gas) {
        throw CommonDataModelException("Substance not gas : " + sfData.Name());
      }
      SESubstanceFraction* sf = new SESubstanceFraction(*sub);
      io::Substance::UnMarshall(sfData, *sf);
      out.m_AmbientGases.push_back(sf);
      out.m_cAmbientGases.push_back(sf);
    }

    for (const CDM::SubstanceConcentrationData& scData : in.AmbientAerosol()) {
      sub = out.m_Substances.GetSubstance(scData.Name());
      if (sub == nullptr) {
        throw CommonDataModelException("Substance not found : " + scData.Name());
      }
      if (sub->GetState() != SESubstanceState::Solid && sub->GetState() != SESubstanceState::Liquid) {
        throw CommonDataModelException("Substance not a liquid or solid : " + scData.Name());
      }
      SESubstanceConcentration* sc = new SESubstanceConcentration(*sub);
      io::Substance::UnMarshall(scData, *sc);
      out.m_AmbientAerosols.push_back(sc);
      out.m_cAmbientAerosols.push_back(sc);
    }
  }
  void Environment::Marshall(const SEEnvironmentalConditions& in, CDM::EnvironmentalConditionsData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    } else {
      out.Name("Local Environment Conditions");
    }

    auto item = std::make_unique<CDM::enumSurroundingType>();
    io::Environment::Marshall(in.m_SurroundingType, *item);
    out.SurroundingType(std::move(item));

    if (in.m_AirDensity && in.m_AirDensity->IsValid()) {
      io::Property::Marshall(*in.m_AirDensity, out.AirDensity());
    };
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AirVelocity);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AmbientTemperature);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AtmosphericPressure);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ClothingResistance);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Emissivity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanRadiantTemperature)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RelativeHumidity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationAmbientTemperature)

    for (SESubstanceFraction* sf : in.m_AmbientGases) {
      auto sfData = std::make_unique<CDM::SubstanceFractionData>();
      io::Substance::Marshall(*sf, *sfData);
      out.AmbientGas().push_back(std::move(sfData));
    }

    for (SESubstanceConcentration* sc : in.m_AmbientAerosols) {
      auto scData = std::make_unique<CDM::SubstanceConcentrationData>();
      io::Substance::Marshall(*sc, *scData);
      out.AmbientAerosol().push_back(std::move(scData));
    }
  }
  //----------------------------------------------------------------------------------
  // class SEActiveHeating
  void Environment::UnMarshall(const CDM::ActiveHeatingData& in, SEActiveHeating& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Power(), out.GetPower(), rd);
    io::Property::UnMarshall(in.SurfaceArea(), out.GetSurfaceArea(), rd);
    io::Property::UnMarshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction(), rd);
  }
  void Environment::Marshall(const SEActiveHeating& in, CDM::ActiveHeatingData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Power);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceArea)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceAreaFraction);
  }
  //----------------------------------------------------------------------------------
  // class SEActiveCooling
  void Environment::UnMarshall(const CDM::ActiveCoolingData& in, SEActiveCooling& out, std::default_random_engine* rd)
  {
    io::Property::UnMarshall(in.Power(), out.GetPower(), rd);
    io::Property::UnMarshall(in.SurfaceArea(), out.GetSurfaceArea(), rd);
    io::Property::UnMarshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction(), rd);
  }
  void Environment::Marshall(const SEActiveCooling& in, CDM::ActiveCoolingData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, Power);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceArea);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceAreaFraction);
  }
  //----------------------------------------------------------------------------------
  // class SEAppliedTemperature
  void Environment::UnMarshall(const CDM::AppliedTemperatureData& in, SEAppliedTemperature& out)
  {
    out.Clear();
    if (in.State().present()) {
      io::Property::UnMarshall(in.State().get(), out.m_State);
    }
    io::Property::UnMarshall(in.Temperature(), out.GetTemperature());
    io::Property::UnMarshall(in.SurfaceArea(), out.GetSurfaceArea());
    io::Property::UnMarshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction());
  }
  void Environment::Marshall(const SEAppliedTemperature& in, CDM::AppliedTemperatureData& out)
  {
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Temperature);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceArea);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SurfaceAreaFraction);
    io::Property::Marshall(in.m_State, out.State());
  }
  //----------------------------------------------------------------------------------
  // class SEEnvironment
  void Environment::UnMarshall(const CDM::EnvironmentData& in, SEEnvironment& out)
  {
    io::System::UnMarshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    if (in.Name().present()) {
      out.m_Name = in.Name().get();
    } else {
      out.m_Name = "Local Environment";
    }

    UnMarshall(in.Conditions(), out.GetConditions());
    io::Property::UnMarshall(in.ConvectiveHeatLoss(), out.GetConvectiveHeatLoss());
    io::Property::UnMarshall(in.ConvectiveHeatTranferCoefficient(), out.GetConvectiveHeatTranferCoefficient());
    io::Property::UnMarshall(in.EvaporativeHeatLoss(), out.GetEvaporativeHeatLoss());
    io::Property::UnMarshall(in.EvaporativeHeatTranferCoefficient(), out.GetEvaporativeHeatTranferCoefficient());
    io::Property::UnMarshall(in.RadiativeHeatLoss(), out.GetRadiativeHeatLoss());
    io::Property::UnMarshall(in.RadiativeHeatTranferCoefficient(), out.GetRadiativeHeatTranferCoefficient());
    io::Property::UnMarshall(in.RespirationHeatLoss(), out.GetRespirationHeatLoss());
    io::Property::UnMarshall(in.SkinHeatLoss(), out.GetSkinHeatLoss());

    out.StateChange();
  }
  void Environment::Marshall(const SEEnvironment& in, CDM::EnvironmentData& out)
  {

    io::System::Marshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    if (in.HasName()) {
      out.Name(in.m_Name);
    } else {
      out.Name("Unknown Environment");
    }

    CDM_OPTIONAL_ENVIRONMENT_MARSHALL_HELPER(in, out, Conditions);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ConvectiveHeatLoss);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ConvectiveHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EvaporativeHeatLoss);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EvaporativeHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RadiativeHeatLoss);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RadiativeHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RespirationHeatLoss);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SkinHeatLoss);
  }
  //-----------------------------------------------------------------------------
  // class SESurroundingType;
  void Environment::UnMarshall(const CDM::enumSurroundingType& in, SESurroundingType& out)
  {
    try {
      switch (in) {
        case CDM::enumSurroundingType::Air:
          out = SESurroundingType::Air;
          break;
        case CDM::enumSurroundingType::Water:
          out = SESurroundingType::Water;
          break;
        default:
          out = SESurroundingType::Invalid;
          break;
      }
    } catch ( xsd::cxx::tree::unexpected_enumerator<char> ) {
        out = SESurroundingType::Invalid;
    }
  }
  void Environment::Marshall(const SESurroundingType& in, CDM::enumSurroundingType& out)
  {
    switch (in) {
    case SESurroundingType::Air:
      out = CDM::enumSurroundingType::Air;
      break;
    case SESurroundingType::Water:
      out = CDM::enumSurroundingType::Water;
      break;
    default:
out = "";
      break;
    }
  }
  //----------------------------------------------------------------------------------
}
bool operator==(CDM::enumSurroundingType const& lhs, SESurroundingType const& rhs)
{
  switch (rhs) {
  case SESurroundingType ::Air:
    return (CDM::enumSurroundingType::Air == lhs);
  case SESurroundingType ::Water:
    return (CDM::enumSurroundingType::Water == lhs);
  case SESurroundingType ::Invalid:
    return ((CDM::enumSurroundingType::value)-1 == lhs);
  default:
    return false;
  }
}

}
