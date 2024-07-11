#include "Substance.h"

#include "Property.h"

#include <biogears/cdm/properties/SEHistogramFractionVsLength.h>
#include <biogears/cdm/properties/SEProperties.h>

#include <biogears/cdm/enums/SESubstanceEnums.h>
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
  void Substance::UnMarshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out)
  {
    out.Clear();
    io::Property::UnMarshall(in.BronchioleModifier(), out.GetBronchioleModifier());
    io::Property::UnMarshall(in.InflammationCoefficient(), out.GetInflammationCoefficient());
    io::Property::UnMarshall(in.ParticulateSizeDistribution(), out.GetParticulateSizeDistribution());
  }
  void Substance::Marshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, BronchioleModifier)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, InflammationCoefficient)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, ParticulateSizeDistribution)
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePharmacokinetics
  void Substance::UnMarshall(const CDM::SubstancePharmacokineticsData& in, SESubstancePharmacokinetics& out)
  {
    out.Clear();

    if (in.Physicochemicals().present()) {
      UnMarshall(in.Physicochemicals(), out.GetPhysicochemicals());
    }

    SESubstanceTissuePharmacokinetics* fx;
    const CDM::SubstanceTissuePharmacokineticsData* fxData;
    for (unsigned int i = 0; i < in.TissueKinetics().size(); i++) {
      fxData = &in.TissueKinetics().at(i);
      fx = new SESubstanceTissuePharmacokinetics(fxData->Name(), out.GetLogger());
      UnMarshall(*fxData, *fx);
      out.m_TissueKinetics[fx->GetName()] = (fx);
    }
  }
  void Substance::Marshall(const SESubstancePharmacokinetics& in, CDM::SubstancePharmacokineticsData& out)
  {
    CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, Physicochemicals)

    for (auto itr : in.m_TissueKinetics) {
      auto tk_data = CDM::SubstanceTissuePharmacokineticsData();
      Marshall(*itr.second, tk_data);
      out.TissueKinetics().push_back(tk_data);
    }
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePhysicochemical
  void Substance::UnMarshall(const CDM::SubstancePhysicochemicalData& in, SESubstancePhysicochemical& out)
  {
    out.Clear();
    for (auto pKa : in.AcidDissociationConstant()) {
      SEScalar* pKScalar = new SEScalar();
      pKScalar->Load(pKa);
      out.m_AcidDissociationConstants.push_back(pKScalar);
    }

    out.m_BindingProtein = in.BindingProtein();
    // UnMarshall(in.BindingProtein(), out.m_BindingProtein);

    io::Property::UnMarshall(in.BloodPlasmaRatio(), out.GetBloodPlasmaRatio());
    io::Property::UnMarshall(in.FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());

    out.SetIonicState(in.IonicState());
    // UnMarshall(in.IonicState(), out.m_IonicState);

    io::Property::UnMarshall(in.LogP(), out.GetLogP());
    io::Property::UnMarshall(in.HydrogenBondCount(), out.GetHydrogenBondCount());
    io::Property::UnMarshall(in.PolarSurfaceArea(), out.GetPolarSurfaceArea());
  }
  void Substance::Marshall(const SESubstancePhysicochemical& in, CDM::SubstancePhysicochemicalData& out)
  {
    out.AcidDissociationConstant().clear();
    for (auto pKa : in.m_AcidDissociationConstants) {
      out.AcidDissociationConstant().push_back(CDM::ScalarData());
      io::Property::Marshall(*pKa, out.AcidDissociationConstant().back());
    }

    if (in.HasBindingProtein()) {
      out.BindingProtein(in.m_BindingProtein);
    }
    if (in.HasIonicState()) {
      out.IonicState(in.m_IonicState);
    }

    // SE_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, BindingProtein)
    // SE_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, IonicState)

    CDM_PROPERTY_MARSHALL_HELPER(in, out, BloodPlasmaRatio)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FractionUnboundInPlasma)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, LogP)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, HydrogenBondCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PolarSurfaceArea)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceTissuePharmacokinetics
  void Substance::UnMarshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out)
  {
    out.Clear();
    out.m_Name = in.Name();
    io::Property::UnMarshall(in.PartitionCoefficient(), out.GetPartitionCoefficient());
  }
  void Substance::Marshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out)
  {
    out.Name(in.m_Name);

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PartitionCoefficient)
  }
  //-----------------------------------------------------------------------------
  // class SESubstancePharmacodynamics
  void Substance::UnMarshall(const CDM::SubstancePharmacodynamicsData& in, SESubstancePharmacodynamics& out)
  {
    UnMarshall(in.Bronchodilation(), out.GetBronchodilation());
    UnMarshall(in.DiastolicPressureModifier(), out.GetDiastolicPressureModifier());

    io::Property::UnMarshall(in.EMaxShapeParameter(), out.GetEMaxShapeParameter());

    UnMarshall(in.FeverModifier(), out.GetFeverModifier());
    UnMarshall(in.HeartRateModifier(), out.GetHeartRateModifier());
    UnMarshall(in.HemorrhageModifier(), out.GetHemorrhageModifier());
    UnMarshall(in.NeuromuscularBlock(), out.GetNeuromuscularBlock());
    UnMarshall(in.PainModifier(), out.GetPainModifier());
    UnMarshall(in.PupilReactivityModifier(), out.GetPupilReactivityModifier());
    UnMarshall(in.PupilSizeModifier(), out.GetPupilSizeModifier());
    UnMarshall(in.RespirationRateModifier(), out.GetRespirationRateModifier());
    UnMarshall(in.Sedation(), out.GetSedation());
    UnMarshall(in.SystolicPressureModifier(), out.GetSystolicPressureModifier());
    UnMarshall(in.TidalVolumeModifier(), out.GetTidalVolumeModifier());
    UnMarshall(in.TubularPermeabilityModifier(), out.GetTubularPermeabilityModifier());
    UnMarshall(in.CentralNervousModifier(), out.GetCentralNervousModifier());

    io::Property::UnMarshall(in.AntibacterialEffect(), out.GetAntibacterialEffect());
    io::Property::UnMarshall(in.EffectSiteRateConstant(), out.GetEffectSiteRateConstant());

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
  void Substance::Marshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out)
  {
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, Bronchodilation)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, DiastolicPressureModifier)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EMaxShapeParameter)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, FeverModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, HeartRateModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, HemorrhageModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, NeuromuscularBlock)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, PainModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, PupilReactivityModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, PupilSizeModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, RespirationRateModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, Sedation)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, SystolicPressureModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, TidalVolumeModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, TubularPermeabilityModifier)
    CDM_SUBSTANCE_MARSHALL_HELPER(in, out, CentralNervousModifier)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, AntibacterialEffect)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EffectSiteRateConstant)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceClearance
  void Substance::UnMarshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out)
  {
    out.Clear();
    // Make sure dups match
    if (in.Systemic().present() && in.RenalDynamics().present()) {
      if (in.RenalDynamics()->Clearance().present() && in.Systemic().get().RenalClearance().value() != in.RenalDynamics()->Clearance().get().value()) {
        out.Warning("Multiple Renal Clearances specified, but not the same. RenalDynamics will override Systemic.");
      }
    }
    if (in.Systemic().present() && in.RenalDynamics().present()) {
      if (in.RenalDynamics()->Regulation().present() && in.Systemic().get().FractionUnboundInPlasma().value() != in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma().value()) {
        out.Warning("Multiple FractionUnboundInPlasma values specified, but not the same. RenalDynamics will override Systemic.");
      }
    }

    if (in.Systemic().present()) {
      out.SetSystemic(true);
      io::Property::UnMarshall(in.Systemic().get().FractionExcretedInFeces(), out.GetFractionExcretedInFeces());
      io::Property::UnMarshall(in.Systemic().get().FractionExcretedInUrine(), out.GetFractionExcretedInUrine());
      io::Property::UnMarshall(in.Systemic().get().FractionMetabolizedInGut(), out.GetFractionMetabolizedInGut());
      io::Property::UnMarshall(in.Systemic().get().FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());
      io::Property::UnMarshall(in.Systemic().get().IntrinsicClearance(), out.GetIntrinsicClearance());
      io::Property::UnMarshall(in.Systemic().get().RenalClearance(), out.GetRenalClearance());
      io::Property::UnMarshall(in.Systemic().get().SystemicClearance(), out.GetSystemicClearance());
    }

    if (in.RenalDynamics().present()) {
      if (in.RenalDynamics()->Regulation().present()) {
        out.m_RenalDynamic = RenalDynamic::Regulation;
        out.SetChargeInBlood(in.RenalDynamics()->Regulation().get().ChargeInBlood());
        // UnMarshall(in.RenalDynamics()->Regulation().get().ChargeInBlood(), out.m_ChargeInBlood);

        io::Property::UnMarshall(in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma(), out.GetFractionUnboundInPlasma());
        io::Property::UnMarshall(in.RenalDynamics()->Regulation().get().ReabsorptionRatio(), out.GetRenalReabsorptionRatio());
        io::Property::UnMarshall(in.RenalDynamics()->Regulation().get().TransportMaximum(), out.GetRenalTransportMaximum());
      } else if (in.RenalDynamics()->Clearance().present()) {
        out.m_RenalDynamic = RenalDynamic::Clearance;
        io::Property::UnMarshall(in.RenalDynamics()->Clearance(), out.GetRenalClearance());
      }

      if (in.RenalDynamics()->FiltrationRate().present()) {
        io::Property::UnMarshall(in.RenalDynamics()->FiltrationRate(), out.GetRenalFiltrationRate());
      }
      if (in.RenalDynamics()->GlomerularFilterability().present()) {
        io::Property::UnMarshall(in.RenalDynamics()->GlomerularFilterability(), out.GetGlomerularFilterability());
      }
      if (in.RenalDynamics()->ReabsorptionRate().present()) {
        io::Property::UnMarshall(in.RenalDynamics()->ReabsorptionRate(), out.GetRenalReabsorptionRate());
      }
      if (in.RenalDynamics()->ExcretionRate().present()) {
        io::Property::UnMarshall(in.RenalDynamics()->ExcretionRate(), out.GetRenalExcretionRate());
      }
    }

    if (in.CellRegulation().present()) {
      out.m_hasCellular = true;
      io::Property::UnMarshall(in.CellRegulation()->CellBirthRate(), out.GetCellBirthRate());
      io::Property::UnMarshall(in.CellRegulation()->CellDeathRate(), out.GetCellDeathRate());
    }
  }
  void Substance::Marshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out)
  {
    if (in.HasSystemic()) {
      out.Systemic(std::make_unique<CDM::Systemic>());
      auto& sys = out.Systemic().get();

      CDM_PROPERTY_MARSHALL_HELPER(in, sys, FractionExcretedInFeces)
      CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, sys, FractionExcretedInUrine)
      CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, sys, FractionMetabolizedInGut)
      CDM_PROPERTY_MARSHALL_HELPER(in, sys, FractionUnboundInPlasma)
      CDM_PROPERTY_MARSHALL_HELPER(in, sys, RenalClearance)
      CDM_PROPERTY_MARSHALL_HELPER(in, sys, IntrinsicClearance)
      CDM_PROPERTY_MARSHALL_HELPER(in, sys, SystemicClearance)
    }

    if (in.HasRenalDynamic()) {
      out.RenalDynamics(std::make_unique<CDM::RenalDynamics>());
      auto& rd = out.RenalDynamics().get();

      if (in.m_RenalDynamic == RenalDynamic::Clearance && in.HasRenalClearance()) {
        io::Property::Marshall(*in.m_RenalClearance, rd.Clearance());
      } else if (in.m_RenalDynamic == RenalDynamic::Regulation) {
        rd.Regulation(std::make_unique<CDM::Regulation>());
        auto& rd_regulation = rd.Regulation().get();

        if (in.HasChargeInBlood()) {
          rd_regulation.ChargeInBlood(in.m_ChargeInBlood);
        }
        // SE_SUBSTANCE_ENUM_MARSHALL_HELPER(in,rd_regulation,ChargeInBlood)

        if (in.m_FractionUnboundInPlasma && in.m_FractionUnboundInPlasma->IsValid()) {
          out.RenalDynamics()->Regulation()->FractionUnboundInPlasma(std::make_unique<std::remove_reference<decltype(out.RenalDynamics()->Regulation()->FractionUnboundInPlasma())>::type>());
          io::Property::Marshall(*in.m_FractionUnboundInPlasma, out.RenalDynamics()->Regulation()->FractionUnboundInPlasma());
        }

        if (in.m_RenalReabsorptionRatio && in.m_RenalReabsorptionRatio->IsValid()) {
          rd_regulation.ReabsorptionRatio(std::make_unique<std::remove_reference<decltype(rd_regulation.ReabsorptionRatio())>::type>());
          io::Property::Marshall(*in.m_RenalReabsorptionRatio, rd_regulation.ReabsorptionRatio());
        }

        if (in.m_RenalTransportMaximum && in.m_RenalTransportMaximum->IsValid()) {
          rd_regulation.TransportMaximum(std::make_unique<std::remove_reference<decltype(rd_regulation.TransportMaximum())>::type>());
          io::Property::Marshall(*in.m_RenalTransportMaximum, rd_regulation.TransportMaximum());
        }
      }
      if (in.HasGlomerularFilterability()) {
        io::Property::Marshall(*in.m_GlomerularFilterability, rd.GlomerularFilterability());
      }
      if (in.HasRenalFiltrationRate()) {
        io::Property::Marshall(*in.m_RenalFiltrationRate, rd.FiltrationRate());
      }
      if (in.HasRenalReabsorptionRate()) {
        io::Property::Marshall(*in.m_RenalReabsorptionRate, rd.ReabsorptionRate());
      }
      if (in.HasRenalExcretionRate()) {
        io::Property::Marshall(*in.m_RenalExcretionRate, rd.ExcretionRate());
      }
      CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, rd, GlomerularFilterability)

      if (in.m_RenalFiltrationRate && in.m_RenalFiltrationRate->IsValid()) {
        io::Property::Marshall(*in.m_RenalFiltrationRate, rd.FiltrationRate());
      }

      if (in.m_RenalExcretionRate && in.m_RenalExcretionRate->IsValid()) {
        io::Property::Marshall(*in.m_RenalExcretionRate, rd.ReabsorptionRate());
      }

      if (in.m_RenalExcretionRate && in.m_RenalExcretionRate->IsValid()) {
        io::Property::Marshall(*in.m_RenalExcretionRate, rd.ExcretionRate());
      }
    }

    if (in.HasCellular()) {
      out.CellRegulation(std::make_unique<CDM::CellRegulation>());
      auto& cell = out.CellRegulation().get();
      CDM_PROPERTY_MARSHALL_HELPER(in, cell, CellDeathRate)
      CDM_PROPERTY_MARSHALL_HELPER(in, cell, CellBirthRate)
    }
  }
  //-----------------------------------------------------------------------------
  // class SESubstance
  void Substance::UnMarshall(const CDM::SubstanceData& in, SESubstance& out)
  {
    out.Clear();
    out.m_Name = in.Name();

    if (in.State().present()) {
      out.m_State = in.State().get();
    }
    if (in.Classification().present()) {
      out.m_Classification = in.Classification().get();
      // io::Substance::UnMarshall(in.State(), out.m_State);
      // io::Substance::UnMarshall(in.Classification(), out.m_Classification);

      io::Property::UnMarshall(in.Density(), out.GetDensity());
      io::Property::UnMarshall(in.MolarMass(), out.GetMolarMass());

      io::Property::UnMarshall(in.MaximumDiffusionFlux(), out.GetMaximumDiffusionFlux());
      io::Property::UnMarshall(in.MichaelisCoefficient(), out.GetMichaelisCoefficient());
      io::Property::UnMarshall(in.MembraneResistance(), out.GetMembraneResistance());
      io::Property::UnMarshall(in.BloodConcentration(), out.GetBloodConcentration());
      io::Property::UnMarshall(in.MassInBody(), out.GetMassInBody());
      io::Property::UnMarshall(in.MassInBlood(), out.GetMassInBlood());
      io::Property::UnMarshall(in.MassInTissue(), out.GetMassInTissue());
      io::Property::UnMarshall(in.PlasmaConcentration(), out.GetPlasmaConcentration());
      io::Property::UnMarshall(in.EffectSiteConcentration(), out.GetEffectSiteConcentration());
      io::Property::UnMarshall(in.SystemicMassCleared(), out.GetSystemicMassCleared());
      io::Property::UnMarshall(in.TissueConcentration(), out.GetTissueConcentration());
      io::Property::UnMarshall(in.AlveolarTransfer(), out.GetAlveolarTransfer());
      io::Property::UnMarshall(in.DiffusingCapacity(), out.GetDiffusingCapacity());
      io::Property::UnMarshall(in.EndTidalFraction(), out.GetEndTidalFraction());
      io::Property::UnMarshall(in.EndTidalPressure(), out.GetEndTidalPressure());
      io::Property::UnMarshall(in.RelativeDiffusionCoefficient(), out.GetRelativeDiffusionCoefficient());
      io::Property::UnMarshall(in.SolubilityCoefficient(), out.GetSolubilityCoefficient());
      io::Property::UnMarshall(in.AreaUnderCurve(), out.GetAreaUnderCurve());

      UnMarshall(in.Aerosolization(), out.GetAerosolization());
      UnMarshall(in.Clearance(), out.GetClearance());
      UnMarshall(in.Pharmacokinetics(), out.GetPK());
      UnMarshall(in.Pharmacodynamics(), out.GetPD());

      if (out.HasClearance() && out.HasPK() && out.GetPK().HasPhysicochemicals()
          && out.GetClearance().HasFractionUnboundInPlasma()
          && !out.GetClearance().GetFractionUnboundInPlasma().Equals(out.GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma())) {

        throw CommonDataModelException("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
      }
    }
  }
  void Substance::Marshall(const SESubstance& in, CDM::SubstanceData& out)
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

    // SE_OPTIONAL_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, State)
    // SE_OPTIONAL_SUBSTANCE_ENUM_MARSHALL_HELPER(in, out, Classification)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Density)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MolarMass)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MaximumDiffusionFlux)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MichaelisCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MembraneResistance)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, BloodConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassInBody)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassInBlood)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MassInTissue)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PlasmaConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EffectSiteConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SystemicMassCleared)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, TissueConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AreaUnderCurve)

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AlveolarTransfer)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, DiffusingCapacity)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EndTidalFraction)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, EndTidalPressure)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, SolubilityCoefficient)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RelativeDiffusionCoefficient)

    CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, Aerosolization)
    CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, Clearance)
    CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, Pharmacokinetics)
    CDM_OPTIONAL_SUBSTANCE_MARSHALL_HELPER(in, out, Pharmacodynamics)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceCompound
  void Substance::UnMarshall(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr, SESubstanceCompound& out)
  {
    out.Clear();
    out.m_Name = in.Name();

    if (in.BloodRHFactor().present()) {
      out.SetRhFactor(in.BloodRHFactor().get());
    }

    if (in.Classification().present()) {
      out.SetClassification(in.Classification().get()); 
      // UnMarshall(in.Classification().get(), out.m_Classification);
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
      UnMarshall(*ccData, *cc);
      out.m_Components.push_back(*cc);
    }
  }
  void Substance::Marshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    }

    if (in.HasRhFactor()) {
      out.BloodRHFactor(in.GetRhFactor());
    }

    CDM_ENUM_MARSHALL_HELPER(in, out, Classification)
    //if (in.HasClassification()) {
    //  io::Substance::Marshall(in.m_Classification, out.Classification());
    //}

    std::vector<SESubstanceConcentration> m_Components;

    for (unsigned int i = 0; i < in.m_Components.size(); i++) {
      auto data = CDM::SubstanceConcentrationData();
      Marshall(in.m_Components.at(i), data);
      out.Component().push_back(data);
    }
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceFraction
  void Substance::UnMarshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out)
  {
    io::Property::UnMarshall(in.FractionAmount(), out.GetFractionAmount());
  }
  void Substance::Marshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out)
  {
    out.Name(in.m_Substance.GetName());
    CDM_PROPERTY_MARSHALL_HELPER(in, out, FractionAmount)
  }
  //-----------------------------------------------------------------------------
  // class SESubstanceConcentration
  void Substance::UnMarshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out)
  {
    out.Clear();
    io::Property::UnMarshall(in.Concentration(), out.GetConcentration());
  }
  void Substance::Marshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out)
  {
    out.Name(in.m_Substance->GetName());

    if (in.m_Concentration.IsValid()) {
      out.Concentration(std::make_unique<std::remove_reference<decltype(out.Concentration())>::type>());
      io::Property::Marshall(in.m_Concentration, out.Concentration());
    }
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out)
  {
    io::Property::UnMarshall(in.EMax(), out.GetEMax());
    io::Property::UnMarshall(in.EC50(), out.GetEC50());
  }
  void Substance::Marshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out)
  {
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EMax)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, EC50)
  }
  //-----------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Copy(const SE& in, SE& out)
  {
    XSD data;
    Substance::Marshall(in, data);
    Substance::UnMarshall(data, out);
  }
  //-----------------------------------------------------------------------------
  template <typename SE, typename XSD>
  void Copy(const SE& in, const SESubstanceManager& subMgr, SE& out)
  {
    XSD data;
    Substance::Marshall(in, data);
    Substance::UnMarshall(data, subMgr, out);
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
    Substance::Marshall(in, data);
    Substance::UnMarshall(data, subMgr, out);
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
  // SESubstanceState
  void Substance::UnMarshall(const CDM::enumSubstanceState& in, SESubstanceState& out)
  {
    switch (in) {
    case CDM::enumSubstanceState::Solid:
      out = SESubstanceState::Solid;
      break;
    case CDM::enumSubstanceState::Liquid:
      out = SESubstanceState::Liquid;
      break;
    case CDM::enumSubstanceState::Gas:
      out = SESubstanceState::Gas;
      break;
    case CDM::enumSubstanceState::Molecular:
      out = SESubstanceState::Molecular;
      break;
    default:
      out = SESubstanceState::Invalid;
      break;
    }
  }
  void Substance::Marshall(const SESubstanceState& in, CDM::enumSubstanceState& out)
  {
    switch (in) {
    case SESubstanceState::Solid:
      out = CDM::enumSubstanceState::Solid;
      break;
    case SESubstanceState::Liquid:
      out = CDM::enumSubstanceState::Liquid;
      break;
    case SESubstanceState::Gas:
      out = CDM::enumSubstanceState::Gas;
      break;
    case SESubstanceState::Molecular:
      out = CDM::enumSubstanceState::Molecular;
      break;
    default:
      out = (CDM::enumSubstanceState::value)-1;
      break;
    }
  }
  // SESubstanceIonicState
  void Substance::UnMarshall(const CDM::enumSubstanceIonicState& in, SESubstanceIonicState& out)
  {
    switch (in) {
    case CDM::enumSubstanceIonicState::WeakBase:
      out = SESubstanceIonicState::WeakBase;
      break;
    case CDM::enumSubstanceIonicState::Base:
      out = SESubstanceIonicState::Base;
      break;
    case CDM::enumSubstanceIonicState::Acid:
      out = SESubstanceIonicState::Acid;
      break;
    case CDM::enumSubstanceIonicState::Neutral:
      out = SESubstanceIonicState::Neutral;
      break;
    case CDM::enumSubstanceIonicState::Zwitterion:
      out = SESubstanceIonicState::Zwitterion;
      break;
    default:
      out = SESubstanceIonicState::Invalid;
      break;
    }
  }
  void Substance::Marshall(const SESubstanceIonicState& in, CDM::enumSubstanceIonicState& out)
  {
    switch (in) {
    case SESubstanceIonicState::WeakBase:
      out = CDM::enumSubstanceIonicState::WeakBase;
      break;
    case SESubstanceIonicState::Base:
      out = CDM::enumSubstanceIonicState::Base;
      break;
    case SESubstanceIonicState::Acid:
      out = CDM::enumSubstanceIonicState::Acid;
      break;
    case SESubstanceIonicState::Neutral:
      out = CDM::enumSubstanceIonicState::Neutral;
      break;
    case SESubstanceIonicState::Zwitterion:
      out = CDM::enumSubstanceIonicState::Zwitterion;
      break;
    default:
      out = (CDM::enumSubstanceIonicState::value)-1;
      break;
    }
  }
  // SESubstanceBindingProtein
  void Substance::UnMarshall(const CDM::enumSubstanceBindingProtein& in, SESubstanceBindingProtein& out)
  {
    switch (in) {
    case CDM::enumSubstanceBindingProtein::Albumin:
      out = SESubstanceBindingProtein::Albumin;
      break;
    case CDM::enumSubstanceBindingProtein::Lipoprotein:
      out = SESubstanceBindingProtein::Lipoprotein;
      break;
    case CDM::enumSubstanceBindingProtein::AAG:
      out = SESubstanceBindingProtein::AAG;
      break;
    default:
      out = SESubstanceBindingProtein::Invalid;
      break;
    }
  }
  void Substance::Marshall(const SESubstanceBindingProtein& in, CDM::enumSubstanceBindingProtein& out)
  {
    switch (in) {
    case SESubstanceBindingProtein::Albumin:
      out = CDM::enumSubstanceBindingProtein::Albumin;
      break;
    case SESubstanceBindingProtein::Lipoprotein:
      out = CDM::enumSubstanceBindingProtein::Lipoprotein;
      break;
    case SESubstanceBindingProtein::AAG:
      out = CDM::enumSubstanceBindingProtein::AAG;
      break;
    default:
      out = (CDM::enumSubstanceBindingProtein::value)-1;
      break;
    }
  }
  //  SECharge
  void Substance::UnMarshall(const CDM::enumCharge& in, SECharge& out)
  {
    switch (in) {
    case CDM::enumCharge::Positive:
      out = SECharge::Positive;
      break;
    case CDM::enumCharge::Neutral:
      out = SECharge::Neutral;
      break;
    case CDM::enumCharge::Negative:
      out = SECharge::Negative;
      break;
    default:
      out = SECharge::Invalid;
      break;
    }
  }
  void Substance::Marshall(const SECharge& in, CDM::enumCharge& out)
  {
    switch (in) {
    case SECharge::Positive:
      out = CDM::enumCharge::Positive;
      break;
    case SECharge::Neutral:
      out = CDM::enumCharge::Neutral;
      break;
    case SECharge::Negative:
      out = CDM::enumCharge::Negative;
      break;
    default:
      out = (CDM::enumCharge::value)-1;
      break;
    }
  }
  //  SESubstanceClass
  void Substance::UnMarshall(const CDM::enumSubstanceClass& in, SESubstanceClass& out)
  {
    switch (in) {
    case CDM::enumSubstanceClass::Anesthetic:
      out = SESubstanceClass::Anesthetic;
      break;
    case CDM::enumSubstanceClass::Antibiotic:
      out = SESubstanceClass::Antibiotic;
      break;
    case CDM::enumSubstanceClass::WholeBlood:
      out = SESubstanceClass::WholeBlood;
      break;
    case CDM::enumSubstanceClass::Ion:
      out = SESubstanceClass::Ion;
      break;
    case CDM::enumSubstanceClass::Opioid:
      out = SESubstanceClass::Opioid;
      break;
    case CDM::enumSubstanceClass::ReversalAgent:
      out = SESubstanceClass::ReversalAgent;
      break;
    case CDM::enumSubstanceClass::Sedative:
      out = SESubstanceClass::Sedative;
      break;
    default:
      out = SESubstanceClass::Invalid;
      break;
    }
  }
  void Substance::Marshall(const SESubstanceClass& in, CDM::enumSubstanceClass& out)
  {
    switch (in) {
    case SESubstanceClass::Anesthetic:
      out = CDM::enumSubstanceClass::Anesthetic;
      break;
    case SESubstanceClass::Antibiotic:
      out = CDM::enumSubstanceClass::Antibiotic;
      break;
    case SESubstanceClass::WholeBlood:
      out = CDM::enumSubstanceClass::WholeBlood;
      break;
    case SESubstanceClass::Ion:
      out = CDM::enumSubstanceClass::Ion;
      break;
    case SESubstanceClass::Opioid:
      out = CDM::enumSubstanceClass::Opioid;
      break;
    case SESubstanceClass::ReversalAgent:
      out = CDM::enumSubstanceClass::ReversalAgent;
      break;
    case SESubstanceClass::Sedative:
      out = CDM::enumSubstanceClass::Sedative;
      break;
    default:
      out = (CDM::enumSubstanceClass::value)-1;
      break;
    }
  }

} // Namespace IO

// Operators
bool operator==(CDM::enumSubstanceState const& lhs, SESubstanceState const& rhs)
{
  switch (rhs) {
  case SESubstanceState::Solid:
    return (CDM::enumSubstanceState::Solid == lhs);
  case SESubstanceState::Liquid:
    return (CDM::enumSubstanceState::Liquid == lhs);
  case SESubstanceState::Gas:
    return (CDM::enumSubstanceState::Gas == lhs);
  case SESubstanceState::Molecular:
    return (CDM::enumSubstanceState::Molecular == lhs);
  case SESubstanceState::Invalid:
    return (-1 == lhs);
  default:
    return false;
  }
}

bool operator==(CDM::enumSubstanceIonicState const& lhs, SESubstanceIonicState const& rhs)
{
  switch (rhs) {
  case SESubstanceIonicState::WeakBase:
    return (CDM::enumSubstanceIonicState::WeakBase == lhs);
  case SESubstanceIonicState::Base:
    return (CDM::enumSubstanceIonicState::Base == lhs);
  case SESubstanceIonicState::Acid:
    return (CDM::enumSubstanceIonicState::Acid == lhs);
  case SESubstanceIonicState::Neutral:
    return (CDM::enumSubstanceIonicState::Neutral == lhs);
  case SESubstanceIonicState::Zwitterion:
    return (CDM::enumSubstanceIonicState::Zwitterion == lhs);
  case SESubstanceIonicState::Invalid:
    return (-1 == lhs);
  default:
    return false;
  }
}

bool operator==(CDM::enumSubstanceBindingProtein const& lhs, SESubstanceBindingProtein const& rhs)
{
  switch (rhs) {
  case SESubstanceBindingProtein::Albumin:
    return (CDM::enumSubstanceBindingProtein::Albumin == lhs);
  case SESubstanceBindingProtein::Lipoprotein:
    return (CDM::enumSubstanceBindingProtein::Lipoprotein == lhs);
  case SESubstanceBindingProtein::AAG:
    return (CDM::enumSubstanceBindingProtein::AAG == lhs);
  case SESubstanceBindingProtein::Invalid:
    return (-1 == lhs);
  default:
    return false;
  }
}

bool operator==(CDM::enumCharge const& lhs, SECharge const& rhs)
{
  switch (rhs) {
  case SECharge::Positive:
    return (CDM::enumCharge::Positive == lhs);
  case SECharge::Neutral:
    return (CDM::enumCharge::Neutral == lhs);
  case SECharge::Negative:
    return (CDM::enumCharge::Negative == lhs);
  case SECharge::Invalid:
    return (-1 == lhs);
  default:
    return false;
  }
}

bool operator==(CDM::enumSubstanceClass const& lhs, SESubstanceClass const& rhs)
{
  switch (rhs) {
  case SESubstanceClass::Anesthetic:
    return (CDM::enumSubstanceClass::Anesthetic == lhs);
  case SESubstanceClass::Antibiotic:
    return (CDM::enumSubstanceClass::Antibiotic == lhs);
  case SESubstanceClass::WholeBlood:
    return (CDM::enumSubstanceClass::WholeBlood == lhs);
  case SESubstanceClass::Ion:
    return (CDM::enumSubstanceClass::Ion == lhs);
  case SESubstanceClass::Opioid:
    return (CDM::enumSubstanceClass::Opioid == lhs);
  case SESubstanceClass::ReversalAgent:
    return (CDM::enumSubstanceClass::ReversalAgent == lhs);
  case SESubstanceClass::Sedative:
    return (CDM::enumSubstanceClass::Sedative == lhs);
  case SESubstanceClass::Invalid:
    return (-1 == lhs);
  default:
    return false;
  }
}
} // Namespace Biogears