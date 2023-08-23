#include "Substance.h"

#include "Property.h"

#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePhysicochemical.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>

namespace biogears {
namespace io {
  // class SESubstanceAerosolization
  void Substance::Marshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out)
  {
    out.Clear();
    io::Property::Marshall(in.BronchioleModifier(), out.GetBronchioleModifier());
    io::Property::Marshall(in.InflammationCoefficient(), out.GetInflammationCoefficient());
    io::Property::Marshall(in.ParticulateSizeDistribution(), out.GetParticulateSizeDistribution());
  }
  void Substance::UnMarshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out)
  {
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, BronchioleModifier)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, InflammationCoefficient)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, ParticulateSizeDistribution)
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePharmacokinetics
  void Substance::Marshall(const CDM::SubstancePharmacokineticsData& in, SESubstancePharmacokinetics& out)
  {
    out.Clear();

    if (in.Physicochemicals().present()) {
      Marshall(in.Physicochemicals(), out.GetPhysicochemicals());
    }

    SESubstanceTissuePharmacokinetics* fx;
    const CDM::SubstanceTissuePharmacokineticsData* fxData;
    for (unsigned int i = 0; i < in.TissueKinetics().size(); i++) {
      fxData = &in.TissueKinetics().at(i);
      fx = new SESubstanceTissuePharmacokinetics(fxData->Name(), out.GetLogger());
      Marshall(*fxData, *fx);
      out.m_TissueKinetics[fx->GetName()] = (fx);
    }
  }
  void Substance::UnMarshall(const SESubstancePharmacokinetics& in, CDM::SubstancePharmacokineticsData& out)
  {
    CDM_OPTIONAL_SUBSTANCE_UNMARSHAL_HELPER(in, out, Physicochemicals)

    for (auto itr : in.m_TissueKinetics) {
      auto tk_data = CDM::SubstanceTissuePharmacokineticsData();
      UnMarshall(*itr.second, tk_data);
      out.TissueKinetics().push_back(tk_data);
    }
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePhysicochemical
  void Substance::Marshall(const CDM::SubstancePhysicochemicalData& in, SESubstancePhysicochemical& out)
  {
    out.Clear();
    for (auto pKa : in.AcidDissociationConstant()) {
      SEScalar* pKScalar = new SEScalar();
      pKScalar->Load(pKa);
      out.m_AcidDissociationConstants.push_back(pKScalar);
    }

    out.m_BindingProtein = in.BindingProtein();
    io::Property::Marshall(in.BloodPlasmaRatio(), out.GetBloodPlasmaRatio());
    io::Property::Marshall(in.FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());
    out.SetIonicState(in.IonicState());
    io::Property::Marshall(in.LogP(), out.GetLogP());
    io::Property::Marshall(in.HydrogenBondCount(), out.GetHydrogenBondCount());
    io::Property::Marshall(in.PolarSurfaceArea(), out.GetPolarSurfaceArea());
  }
  void Substance::UnMarshall(const SESubstancePhysicochemical& in, CDM::SubstancePhysicochemicalData& out)
  {
    out.AcidDissociationConstant().clear();
    for (auto pKa : in.m_AcidDissociationConstants) {
      out.AcidDissociationConstant().push_back(CDM::ScalarData());
      io::Property::UnMarshall(*pKa, out.AcidDissociationConstant().back());
    }

    if (in.HasBindingProtein()) {
      out.BindingProtein(in.m_BindingProtein);
    }

    if (in.HasIonicState()) {
      out.IonicState(in.m_IonicState);
    }

    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, BloodPlasmaRatio)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, FractionUnboundInPlasma)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, LogP)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, HydrogenBondCount)
    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PolarSurfaceArea)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceTissuePharmacokinetics
  void Substance::Marshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out)
  {
    out.Clear();
    out.m_Name = in.Name();
    io::Property::Marshall(in.PartitionCoefficient(), out.GetPartitionCoefficient());
  }
  void Substance::UnMarshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out)
  {
    out.Name(in.m_Name);

    CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PartitionCoefficient)
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePharmacodynamics
  void Substance::Marshall(const CDM::SubstancePharmacodynamicsData& in, SESubstancePharmacodynamics& out)
  {
    Marshall(in.Bronchodilation(), out.GetBronchodilation());
    Marshall(in.DiastolicPressureModifier(), out.GetDiastolicPressureModifier());

    io::Property::Marshall(in.EMaxShapeParameter(), out.GetEMaxShapeParameter());

    Marshall(in.FeverModifier(), out.GetFeverModifier());
    Marshall(in.HeartRateModifier(), out.GetHeartRateModifier());
    Marshall(in.HemorrhageModifier(), out.GetHemorrhageModifier());
    Marshall(in.NeuromuscularBlock(), out.GetNeuromuscularBlock());
    Marshall(in.PainModifier(), out.GetPainModifier());
    Marshall(in.PupilReactivityModifier(), out.GetPupilReactivityModifier());
    Marshall(in.PupilSizeModifier(), out.GetPupilSizeModifier());
    Marshall(in.RespirationRateModifier(), out.GetRespirationRateModifier());
    Marshall(in.Sedation(), out.GetSedation());
    Marshall(in.SystolicPressureModifier(), out.GetSystolicPressureModifier());
    Marshall(in.TidalVolumeModifier(), out.GetTidalVolumeModifier());
    Marshall(in.TubularPermeabilityModifier(), out.GetTubularPermeabilityModifier());
    Marshall(in.CentralNervousModifier(), out.GetCentralNervousModifier());

    io::Property::Marshall(in.AntibacterialEffect(), out.GetAntibacterialEffect());
    io::Property::Marshall(in.EffectSiteRateConstant(), out.GetEffectSiteRateConstant());

    // Set up map (Antibiotic effect not added to modifier list because it is implemented different from other modifiers)
    out.m_Modifiers.clear();
    out.m_Modifiers["Bronchodilation"] = out.m_Bronchodilation;
    out.m_Modifiers["CentralNervous"] = out.m_CentralNervousModifier;
    out.m_Modifiers["DiastolicPressure"] = out.m_DiastolicPressureModifier;
    out.m_Modifiers["Fever"] = out.m_FeverModifier;
    out.m_Modifiers["HeartRate"] = out.m_HeartRateModifier;
    out.m_Modifiers["Hemorrhage"] = out.m_HemorrhageModifier;
    out.m_Modifiers["NeuromuscularBlock"] = out.m_NeuromuscularBlock;
    out.m_Modifiers["Pain"] = out.m_PainModifier;
    out.m_Modifiers["PupilReactivity"] = out.m_PupilReactivityModifier;
    out.m_Modifiers["PupilSize"] = out.m_PupilSizeModifier;
    out.m_Modifiers["RespirationRate"] = out.m_RespirationRateModifier;
    out.m_Modifiers["Sedation"] = out.m_Sedation;
    out.m_Modifiers["SystolicPressure"] = out.m_SystolicPressureModifier;
    out.m_Modifiers["TidalVolume"] = out.m_TidalVolumeModifier;
    out.m_Modifiers["TubularPermeability"] = out.m_TubularPermeabilityModifier;
  }
  void Substance::UnMarshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out)
  {
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, Bronchodilation)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, DiastolicPressureModifier)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, EMaxShapeParameter)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, FeverModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, HeartRateModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, HemorrhageModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, NeuromuscularBlock)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, PainModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, PupilReactivityModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, PupilSizeModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, RespirationRateModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, Sedation)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, SystolicPressureModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, TidalVolumeModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, TubularPermeabilityModifier)
    CDM_SUBSTANCE_UNMARSHAL_HELPER(in, out, CentralNervousModifier)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, AntibacterialEffect)
    CDM_PROPERTY_UNMARSHAL_HELPER(in, out, EffectSiteRateConstant)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceClearance
  void Substance::Marshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out)
  {
    out.Clear();
    // Make sure dups match
    if (in.Systemic().present()
        && in.RenalDynamics().present()
        && in.RenalDynamics()->Clearance().present()
        && in.Systemic().get().RenalClearance().value() != in.RenalDynamics()->Clearance().get().value()) {
      throw CommonDataModelException("Multiple Renal Clearances specified, but not the same. These must match at this time.");
    }
    if (in.Systemic().present()
        && in.RenalDynamics().present()
        && in.RenalDynamics()->Regulation().present()
        && in.Systemic().get().FractionUnboundInPlasma().value() != in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma().value()) {
      throw CommonDataModelException("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
    }

    if (in.Systemic().present()) {
      out.SetSystemic(true);
      io::Property::Marshall(in.Systemic().get().FractionExcretedInFeces(), out.GetFractionExcretedInFeces());
      io::Property::Marshall(in.Systemic().get().FractionExcretedInUrine(), out.GetFractionExcretedInUrine());
      io::Property::Marshall(in.Systemic().get().FractionMetabolizedInGut(), out.GetFractionMetabolizedInGut());
      io::Property::Marshall(in.Systemic().get().FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());
      io::Property::Marshall(in.Systemic().get().IntrinsicClearance(), out.GetIntrinsicClearance());
      io::Property::Marshall(in.Systemic().get().RenalClearance(), out.GetRenalClearance());
      io::Property::Marshall(in.Systemic().get().SystemicClearance(), out.GetSystemicClearance());
    }

    if (in.RenalDynamics().present()) {
      if (in.RenalDynamics()->Regulation().present()) {
        out.m_RenalDynamic = RenalDynamic::Regulation;
        out.SetChargeInBlood(in.RenalDynamics()->Regulation().get().ChargeInBlood());
        io::Property::Marshall(in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());
        io::Property::Marshall(in.RenalDynamics()->Regulation().get().ReabsorptionRatio(), out.GetRenalReabsorptionRatio());
        io::Property::Marshall(in.RenalDynamics()->Regulation().get().TransportMaximum(), out.GetRenalTransportMaximum());
      } else if (in.RenalDynamics()->Clearance().present()) {
        out.m_RenalDynamic = RenalDynamic::Clearance;
        io::Property::Marshall(in.RenalDynamics()->Clearance(), out.GetRenalClearance());
      }

      if (in.RenalDynamics()->FiltrationRate().present()) {
        io::Property::Marshall(in.RenalDynamics()->FiltrationRate(), out.GetRenalFiltrationRate());
      }
      if (in.RenalDynamics()->GlomerularFilterability().present()) {
        io::Property::Marshall(in.RenalDynamics()->GlomerularFilterability(), out.GetGlomerularFilterability());
      }
      if (in.RenalDynamics()->ReabsorptionRate().present()) {
        io::Property::Marshall(in.RenalDynamics()->ReabsorptionRate(), out.GetRenalReabsorptionRate());
      }
      if (in.RenalDynamics()->ExcretionRate().present()) {
        io::Property::Marshall(in.RenalDynamics()->ExcretionRate(), out.GetRenalExcretionRate());
      }
    }

    if (in.CellRegulation().present()) {
      out.m_hasCellular = true;
      io::Property::Marshall(in.CellRegulation()->CellBirthRate(), out.GetCellBirthRate());
      io::Property::Marshall(in.CellRegulation()->CellDeathRate(), out.GetCellDeathRate());
    }
  }
  void Substance::UnMarshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out)
  {
    if (in.HasSystemic()) {
      out.Systemic(std::make_unique<CDM::Systemic>());
      auto& sys = out.Systemic().get();

      CDM_PROPERTY_UNMARSHAL_HELPER(in, sys, FractionExcretedInFeces)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, sys, FractionExcretedInUrine)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, sys, FractionMetabolizedInGut)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, sys, FractionUnboundInPlasma)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, sys, RenalClearance)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, sys, IntrinsicClearance)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, sys, SystemicClearance)
    }

    if (in.HasRenalDynamic()) {
      out.RenalDynamics(std::make_unique<CDM::RenalDynamics>());
      auto& rd = out.RenalDynamics().get();

      if (in.m_RenalDynamic == RenalDynamic::Clearance && in.HasRenalClearance()) {
        io::Property::UnMarshall(*in.m_RenalClearance, rd.Clearance());
      } else if (in.m_RenalDynamic == RenalDynamic::Regulation) {
        rd.Regulation(std::make_unique<CDM::Regulation>());
        auto& rd_regulation = rd.Regulation().get();

        if (in.HasChargeInBlood()) {
          rd_regulation.ChargeInBlood(in.m_ChargeInBlood);
        }

        if (in.m_FractionUnboundInPlasma && in.m_FractionUnboundInPlasma->IsValid()) {
          out.RenalDynamics()->Regulation()->FractionUnboundInPlasma(std::make_unique<std::remove_reference<decltype(out.RenalDynamics()->Regulation()->FractionUnboundInPlasma())>::type>());
          io::Property::UnMarshall(*in.m_FractionUnboundInPlasma, out.RenalDynamics()->Regulation()->FractionUnboundInPlasma());
        }

        if (in.m_RenalReabsorptionRatio && in.m_RenalReabsorptionRatio->IsValid()) {
          rd_regulation.ReabsorptionRatio(std::make_unique<std::remove_reference<decltype(rd_regulation.ReabsorptionRatio())>::type>());
          io::Property::UnMarshall(*in.m_RenalReabsorptionRatio, rd_regulation.ReabsorptionRatio());
        }

        if (in.m_RenalTransportMaximum && in.m_RenalTransportMaximum->IsValid()) {
          rd_regulation.TransportMaximum(std::make_unique<std::remove_reference<decltype(rd_regulation.TransportMaximum())>::type>());
          io::Property::UnMarshall(*in.m_RenalTransportMaximum, rd_regulation.TransportMaximum());
        }
      }
      if (in.HasGlomerularFilterability()) {
        io::Property::UnMarshall(*in.m_GlomerularFilterability, rd.GlomerularFilterability());
      }
      if (in.HasRenalFiltrationRate()) {
        io::Property::UnMarshall(*in.m_RenalFiltrationRate, rd.FiltrationRate());
      }
      if (in.HasRenalReabsorptionRate()) {
        io::Property::UnMarshall(*in.m_RenalReabsorptionRate, rd.ReabsorptionRate());
      }
      if (in.HasRenalExcretionRate()) {
        io::Property::UnMarshall(*in.m_RenalExcretionRate, rd.ExcretionRate());
      }
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, rd, GlomerularFilterability)

      if (in.m_RenalFiltrationRate && in.m_RenalFiltrationRate->IsValid()) {
        io::Property::UnMarshall(*in.m_RenalFiltrationRate, rd.FiltrationRate());
      }

      if (in.m_RenalExcretionRate && in.m_RenalExcretionRate->IsValid()) {
        io::Property::UnMarshall(*in.m_RenalExcretionRate, rd.ReabsorptionRate());
      }

      if (in.m_RenalExcretionRate && in.m_RenalExcretionRate->IsValid()) {
        io::Property::UnMarshall(*in.m_RenalExcretionRate, rd.ExcretionRate());
      }
    }

    if (in.HasCellular()) {
      out.CellRegulation(std::make_unique<CDM::CellRegulation>());
      auto& cell = out.CellRegulation().get();
      CDM_PROPERTY_UNMARSHAL_HELPER(in, cell, CellDeathRate)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, cell, CellBirthRate)
    }
  }
  //-----------------------------------------------------------------------------
  // class SESubstance
  void Substance::Marshall(const CDM::SubstanceData& in, SESubstance& out)
  {
    out.Clear();
    out.m_Name = in.Name();

    if (in.State().present()) {
      out.m_State = in.State().get();
    }
    if (in.Classification().present()) {
      out.m_Classification = in.Classification().get();
    }

      io::Property::Marshall(in.Density(), out.GetDensity());
      io::Property::Marshall(in.MolarMass(), out.GetMolarMass());
    
      io::Property::Marshall(in.MaximumDiffusionFlux(), out.GetMaximumDiffusionFlux());
      io::Property::Marshall(in.MichaelisCoefficient(), out.GetMichaelisCoefficient());
      io::Property::Marshall(in.MembraneResistance(), out.GetMembraneResistance());
      io::Property::Marshall(in.BloodConcentration(), out.GetBloodConcentration());
      io::Property::Marshall(in.MassInBody(), out.GetMassInBody());
      io::Property::Marshall(in.MassInBlood(), out.GetMassInBlood()); 
      io::Property::Marshall(in.MassInTissue(), out.GetMassInTissue());
      io::Property::Marshall(in.PlasmaConcentration(), out.GetPlasmaConcentration());
      io::Property::Marshall(in.EffectSiteConcentration(), out.GetEffectSiteConcentration());
      io::Property::Marshall(in.SystemicMassCleared(), out.GetSystemicMassCleared());
      io::Property::Marshall(in.TissueConcentration(), out.GetTissueConcentration());
      io::Property::Marshall(in.AlveolarTransfer(), out.GetAlveolarTransfer());
      io::Property::Marshall(in.DiffusingCapacity(), out.GetDiffusingCapacity());
      io::Property::Marshall(in.EndTidalFraction(), out.GetEndTidalFraction());
      io::Property::Marshall(in.EndTidalPressure(), out.GetEndTidalPressure());
      io::Property::Marshall(in.RelativeDiffusionCoefficient(), out.GetRelativeDiffusionCoefficient());
      io::Property::Marshall(in.SolubilityCoefficient(), out.GetSolubilityCoefficient());
      io::Property::Marshall(in.AreaUnderCurve(), out.GetAreaUnderCurve());

      Marshall(in.Aerosolization(), out.GetAerosolization());
      Marshall(in.Clearance(), out.GetClearance());
      Marshall(in.Pharmacokinetics(), out.GetPK());
      Marshall(in.Pharmacodynamics(), out.GetPD());

      if (out.HasClearance() && out.HasPK() && out.GetPK().HasPhysicochemicals()
          && out.GetClearance().HasFractionUnboundInPlasma()
          && !out.GetClearance().GetFractionUnboundInPlasma().Equals(out.GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma())) {

        throw CommonDataModelException("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
      }
    }
    void Substance::UnMarshall(const SESubstance& in, CDM::SubstanceData& out)
    {
      if (in.HasName()) {
        out.Name(in.m_Name);
      } else {
        out.Name("Unknown Substance");
      }
      if (in.HasState()) {
        out.State(in.m_State);
      }
      if (in.HasClassification()) {
        out.Classification(in.m_Classification);
      }
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, Density)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MolarMass)

      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MaximumDiffusionFlux)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MichaelisCoefficient)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MembraneResistance)

      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, BloodConcentration)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassInBody)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassInBlood)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, MassInTissue)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, PlasmaConcentration)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EffectSiteConcentration)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SystemicMassCleared)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, TissueConcentration)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AreaUnderCurve)

      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, AlveolarTransfer)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, DiffusingCapacity)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EndTidalFraction)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, EndTidalPressure)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, SolubilityCoefficient)
      CDM_OPTIONAL_PROPERTY_UNMARSHAL_HELPER(in, out, RelativeDiffusionCoefficient)

      CDM_OPTIONAL_SUBSTANCE_UNMARSHAL_HELPER(in, out, Aerosolization)
      CDM_OPTIONAL_SUBSTANCE_UNMARSHAL_HELPER(in, out, Clearance)
      CDM_OPTIONAL_SUBSTANCE_UNMARSHAL_HELPER(in, out, Pharmacokinetics)
      CDM_OPTIONAL_SUBSTANCE_UNMARSHAL_HELPER(in, out, Pharmacodynamics)
    }
    //-----------------------------------------------------------------------------
    // class SESubstanceCompound
    void Substance::Marshall(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr, SESubstanceCompound& out)
    {
      out.Clear();
      out.m_Name = in.Name();

      if (in.BloodRHFactor().present()) {
        out.SetRhFactor(in.BloodRHFactor().get());
      }

      if (in.Classification().present()) {
        out.SetClassification(in.Classification().get());
      }

      std::string err;
      SESubstance* substance = nullptr;
      SESubstanceConcentration* cc;
      CDM::SubstanceConcentrationData* ccData;
      for (unsigned int i = 0; i < in.Component().size(); i++) {
        ccData = (CDM::SubstanceConcentrationData*)&in.Component().at(i);
        substance = subMgr.GetSubstance(ccData->Name());
        if (substance == nullptr) {
          /// \error Could not load find substance compound component for specified substance
          err = "Could not load find substance compound component : ";
          err += ccData->Name();
          throw CommonDataModelException(err);
        }
        cc = new SESubstanceConcentration(*substance);
        Marshall(*ccData, *cc);
        out.m_Components.push_back(*cc);
      }
    }
    void Substance::UnMarshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out)
    {
      if (in.HasName()) {
        out.Name(in.m_Name);
      }

      if (in.HasRhFactor()) {
        out.BloodRHFactor(in.GetRhFactor());
      }

      CDM_ENUM_UNMARSHAL_HELPER(in, out, Classification)

      std::vector<SESubstanceConcentration> m_Components;

      for (unsigned int i = 0; i < in.m_Components.size(); i++) {
        auto data = CDM::SubstanceConcentrationData();
        UnMarshall(in.m_Components.at(i), data);
        out.Component().push_back(data);
      }
    }
    //-----------------------------------------------------------------------------
    // class SESubstanceFraction
    void Substance::Marshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out)
    {
      io::Property::Marshall(in.FractionAmount(), out.GetFractionAmount());
    }
    void Substance::UnMarshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out)
    {
      out.Name(in.m_Substance.GetName());
      CDM_PROPERTY_UNMARSHAL_HELPER(in, out, FractionAmount)
    }
    //-----------------------------------------------------------------------------
    // class SESubstanceConcentration
    void Substance::Marshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out)
    {
      out.Clear();
      io::Property::Marshall(in.Concentration(), out.GetConcentration());
    }
    void Substance::UnMarshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out)
    {
      out.Name(in.m_Substance->GetName());

      if (in.m_Concentration.IsValid()) {
        out.Concentration(std::make_unique<std::remove_reference<decltype(out.Concentration())>::type>());
        io::Property::UnMarshall(in.m_Concentration, out.Concentration());
      }
    }
    //-----------------------------------------------------------------------------
    void Substance::Marshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out)
    {
      io::Property::Marshall(in.EMax(), out.GetEMax());
      io::Property::Marshall(in.EC50(), out.GetEC50());
    }
    void Substance::UnMarshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out)
    {
      CDM_PROPERTY_UNMARSHAL_HELPER(in, out, EMax)
      CDM_PROPERTY_UNMARSHAL_HELPER(in, out, EC50)
    }
    //-----------------------------------------------------------------------------
    template <typename SE, typename XSD>
    void Copy(const SE& in, SE& out)
    {
      XSD data;
      Substance::UnMarshall(in, data);
      Substance::Marshall(data, out);
    }
    //-----------------------------------------------------------------------------
    template <typename SE, typename XSD>
    void Copy(const SE& in, const SESubstanceManager& subMgr, SE& out)
    {
      XSD data;
      Substance::UnMarshall(in, data);
      Substance::Marshall(data, subMgr, out);
    }
    //-----------------------------------------------------------------------------
    void Substance::Copy(const SESubstanceClearance& in, SESubstanceClearance& out)
    {
      ::biogears::io::Copy<SESubstanceClearance, CDM::SubstanceClearanceData>(in, out);
    }
    //-----------------------------------------------------------------------------
    void Substance::Copy(const SESubstance& in, SESubstance& out)
    {
      ::biogears::io::Copy<SESubstance, CDM::SubstanceData>(in, out);
    }
    //-----------------------------------------------------------------------------
    void Substance::Copy(const SESubstanceCompound& in, const SESubstanceManager& subMgr, SESubstanceCompound& out)
    {
      ::biogears::io::Copy<SESubstanceCompound, CDM::SubstanceCompoundData>(in, subMgr, out);
      CDM::SubstanceCompoundData data;
      Substance::UnMarshall(in, data);
      Substance::Marshall(data, subMgr, out);
    }
    //-----------------------------------------------------------------------------
    void Substance::Copy(const SESubstanceFraction& in, SESubstanceFraction& out)
    {
      ::biogears::io::Copy<SESubstanceFraction, CDM::SubstanceFractionData>(in, out);
    }
    //-----------------------------------------------------------------------------
    void Substance::Copy(const SESubstanceConcentration& in, SESubstanceConcentration& out)
    {
      ::biogears::io::Copy<SESubstanceConcentration, CDM::SubstanceConcentrationData>(in, out);
    }
    //-----------------------------------------------------------------------------
  }
}