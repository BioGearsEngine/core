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
  void Environment::Marshall(const CDM::EnvironmentalConditionsData& in, SEEnvironmentalConditions& out)
  {
    out.Clear();
    if (in.Name().present()) {
      out.SetName(in.Name().get());
    }
    if (in.SurroundingType().present()) {
      out.SetSurroundingType(in.SurroundingType().get());
    }
    io::Property::Marshall(in.AirDensity(), out.GetAirDensity());
    io::Property::Marshall(in.AirVelocity(), out.GetAirVelocity());
    io::Property::Marshall(in.AmbientTemperature(), out.GetAmbientTemperature());
    io::Property::Marshall(in.AtmosphericPressure(), out.GetAtmosphericPressure());
    io::Property::Marshall(in.ClothingResistance(), out.GetClothingResistance());
    io::Property::Marshall(in.Emissivity(), out.GetEmissivity());
    io::Property::Marshall(in.MeanRadiantTemperature(), out.GetMeanRadiantTemperature());
    io::Property::Marshall(in.RelativeHumidity(), out.GetRelativeHumidity());
    io::Property::Marshall(in.RespirationAmbientTemperature(), out.GetRespirationAmbientTemperature());

    SESubstance* sub;
    for (const CDM::SubstanceFractionData& sfData : in.AmbientGas()) {
      sub = out.m_Substances.GetSubstance(sfData.Name());
      if (sub == nullptr) {
        throw CommonDataModelException("Substance not found : " + sfData.Name());
      }
      if (sub->GetState() != CDM::enumSubstanceState::Gas) {
        throw CommonDataModelException("Substance not gas : " + sfData.Name());
      }
      SESubstanceFraction* sf = new SESubstanceFraction(*sub);
      io::Substance::Marshall(sfData, *sf);
      out.m_AmbientGases.push_back(sf);
      out.m_cAmbientGases.push_back(sf);
    }

    for (const CDM::SubstanceConcentrationData& scData : in.AmbientAerosol()) {
      sub = out.m_Substances.GetSubstance(scData.Name());
      if (sub == nullptr) {
        throw CommonDataModelException("Substance not found : " + scData.Name());
      }
      if (sub->GetState() != CDM::enumSubstanceState::Solid && sub->GetState() != CDM::enumSubstanceState::Liquid) {
        throw CommonDataModelException("Substance not a liquid or solid : " + scData.Name());
      }
      SESubstanceConcentration* sc = new SESubstanceConcentration(*sub);
      io::Substance::Marshall(scData, *sc);
      out.m_AmbientAerosols.push_back(sc);
      out.m_cAmbientAerosols.push_back(sc);
    }
  }
  void Environment::UnMarshall(const SEEnvironmentalConditions& in, CDM::EnvironmentalConditionsData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    } else {
      out.Name("Local Environment Conditions");
    }
    if (in.HasSurroundingType()) {
      out.SurroundingType(in.m_SurroundingType);
    }
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AirDensity);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AirVelocity);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AmbientTemperature);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AtmosphericPressure);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ClothingResistance);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Emissivity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MeanRadiantTemperature)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RelativeHumidity)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationAmbientTemperature)

    for (SESubstanceFraction* sf : in.m_AmbientGases) {
      auto sfData = std::make_unique<CDM::SubstanceFractionData>();
      io::Substance::UnMarshall(*sf, *sfData);
      out.AmbientGas().push_back(std::move(sfData));
    }

    for (SESubstanceConcentration* sc : in.m_AmbientAerosols) {
      auto scData = std::make_unique<CDM::SubstanceConcentrationData>();
      io::Substance::UnMarshall(*sc, *scData);
      out.AmbientAerosol().push_back(std::move(scData));
    }
  }
  //----------------------------------------------------------------------------------
  // class SEActiveHeating
  void Environment::Marshall(const CDM::ActiveHeatingData& in, SEActiveHeating& out)
  {
    io::Property::Marshall(in.Power(), out.GetPower());
    io::Property::Marshall(in.SurfaceArea(), out.GetSurfaceArea());
    io::Property::Marshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction());
  }
  void Environment::UnMarshall(const SEActiveHeating& in, CDM::ActiveHeatingData& out)
  {
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Power);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceArea)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceAreaFraction);
  }
  //----------------------------------------------------------------------------------
  // class SEActiveCooling
  void Environment::Marshall(const CDM::ActiveCoolingData& in, SEActiveCooling& out)
  {
    io::Property::Marshall(in.Power(), out.GetPower());
    io::Property::Marshall(in.SurfaceArea(), out.GetSurfaceArea());
    io::Property::Marshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction());
  }
  void Environment::UnMarshall(const SEActiveCooling& in, CDM::ActiveCoolingData& out)
  {
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, Power);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceArea);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceAreaFraction);
  }
  //----------------------------------------------------------------------------------
  // class SEAppliedTemperature
  void Environment::Marshall(const CDM::AppliedTemperatureData& in, SEAppliedTemperature& out)
  {
    out.Clear();
    if (in.State().present()) {
      out.m_State = in.State().get();
    }
    io::Property::Marshall(in.Temperature(), out.GetTemperature());
    io::Property::Marshall(in.SurfaceArea(), out.GetSurfaceArea());
    io::Property::Marshall(in.SurfaceAreaFraction(), out.GetSurfaceAreaFraction());
  }
  void Environment::UnMarshall(const SEAppliedTemperature& in, CDM::AppliedTemperatureData& out)
  {
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Temperature);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceArea);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SurfaceAreaFraction);
    out.State(in.m_State);
  }
  //----------------------------------------------------------------------------------
  // class SEEnvironment
  void Environment::Marshall(const CDM::EnvironmentData& in, SEEnvironment& out)
  {
    io::System::Marshall(static_cast<const CDM::SystemData&>(in), static_cast<SESystem&>(out));

    if (in.Name().present()) {
      out.m_Name = in.Name().get();
    } else {
      out.m_Name = "Local Environment";
    }

    Marshall(in.Conditions(), out.GetConditions());
    io::Property::Marshall(in.ConvectiveHeatLoss(), out.GetConvectiveHeatLoss());
    io::Property::Marshall(in.ConvectiveHeatTranferCoefficient(), out.GetConvectiveHeatTranferCoefficient());
    io::Property::Marshall(in.EvaporativeHeatLoss(), out.GetEvaporativeHeatLoss());
    io::Property::Marshall(in.EvaporativeHeatTranferCoefficient(), out.GetEvaporativeHeatTranferCoefficient());
    io::Property::Marshall(in.RadiativeHeatLoss(), out.GetRadiativeHeatLoss());
    io::Property::Marshall(in.RadiativeHeatTranferCoefficient(), out.GetRadiativeHeatTranferCoefficient());
    io::Property::Marshall(in.RespirationHeatLoss(), out.GetRespirationHeatLoss());
    io::Property::Marshall(in.SkinHeatLoss(), out.GetSkinHeatLoss());

    out.StateChange();
  }
  void Environment::UnMarshall(const SEEnvironment& in, CDM::EnvironmentData& out)
  {

    io::System::UnMarshall(static_cast<const SESystem&>(in), static_cast<CDM::SystemData&>(out));

    if (in.HasName()) {
      out.Name(in.m_Name);
    } else {
      out.Name("Unknown Environment");
    }

    CDM_OPTIONAL_ENVIRONMENT_UNMARSHAL_HELPER(in, out, Conditions);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ConvectiveHeatLoss);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, ConvectiveHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EvaporativeHeatLoss);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EvaporativeHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RadiativeHeatLoss);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RadiativeHeatTranferCoefficient);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RespirationHeatLoss);
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SkinHeatLoss);

  }
  //----------------------------------------------------------------------------------
}
}