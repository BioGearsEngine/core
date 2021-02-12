#include "Substance.h"

#include "Property.h"

#include <biogears/cdm/properties/SEScalar.h>
#include <biogears/cdm/properties/SEScalarElectricResistance.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarInversePressure.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerAmount.h>
#include <biogears/cdm/properties/SEScalarMassPerAreaTime.h>
#include <biogears/cdm/properties/SEScalarMassPerTime.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimeMass.h>
#include <biogears/cdm/properties/SEScalarVolumePerTimePressure.h>

#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceAerosolization.h>
#include <biogears/cdm/substance/SESubstanceClearance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceConcentration.h>
#include <biogears/cdm/substance/SESubstanceFraction.h >
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstancePharmacodynamics.h>
#include <biogears/cdm/substance/SESubstancePharmacokinetics.h>
#include <biogears/cdm/substance/SESubstancePhysicochemicals.h>
#include <biogears/cdm/substance/SESubstanceTissuePharmacokinetics.h>

namespace biogears {
namespace io {
  //class SESubstanceAerosolization
  void Substance::Marshall(const CDM::SubstanceAerosolizationData& in, SESubstanceAerosolization& out)
  {
    out.Clear();
    io::Property::Marshall(in.BronchioleModifier(), out.GetBronchioleModifier());
    io::Property::Marshall(in.InflammationCoefficient(), out.GetInflammationCoefficient());
    io::Property::Marshall(in.ParticulateSizeDistribution(), out.GetParticulateSizeDistribution());
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceAerosolization& in, CDM::SubstanceAerosolizationData& out)
  {
    if (in.HasBronchioleModifier()) {
      io::Property::UnMarshall(*in.m_BronchioleModifier, out.BronchioleModifier());
    }
    if (in.HasInflammationCoefficient()) {
      io::Property::UnMarshall(*in.m_InflammationCoefficient, out.InflammationCoefficient());
    }
    if (in.HasParticulateSizeDistribution()) {
      io::Property::UnMarshall(*in.m_ParticulateSizeDistribution, out.ParticulateSizeDistribution());
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstancePharmacokinetics
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
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstancePharmacokinetics& in, CDM::SubstancePharmacokineticsData& out)
  {
    if (in.HasPhysicochemicals()) {
      UnMarshall(*in.m_Physicochemicals, out.Physicochemicals());
    }

    for (auto itr : in.m_TissueKinetics) {
      auto tk_data = CDM::SubstanceTissuePharmacokineticsData();
      UnMarshall(*itr.second, tk_data);
      out.TissueKinetics().push_back(tk_data);
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstancePhysicochemical
  void Substance::Marshall(const CDM::SubstancePhysicochemicalData& in, SESubstancePhysicochemicals& out)
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
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstancePhysicochemicals& in, CDM::SubstancePhysicochemicalData& out)
  {
    out.AcidDissociationConstant().clear();
    for (auto pKa : in.m_AcidDissociationConstants) {
      out.AcidDissociationConstant().push_back(CDM::ScalarData());
      io::Property::UnMarshall(*pKa, out.AcidDissociationConstant().back());
    }

    if (in.HasBindingProtein()) {
      out.BindingProtein(in.m_BindingProtein);
    }
    if (in.HasBloodPlasmaRatio()) {
      io::Property::UnMarshall(*in.m_BloodPlasmaRatio, out.BloodPlasmaRatio());
    }
    if (in.HasFractionUnboundInPlasma()) {
      io::Property::UnMarshall(*in.m_FractionUnboundInPlasma, out.FractionUnboundInPlasma());
    }
    if (in.HasIonicState()) {
      out.IonicState(in.m_IonicState);
    }
    if (in.HasLogP()) {
      io::Property::UnMarshall(*in.m_LogP, out.LogP());
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstanceTissuePharmacokinetics
  void Substance::Marshall(const CDM::SubstanceTissuePharmacokineticsData& in, SESubstanceTissuePharmacokinetics& out)
  {
    out.Clear();
    if (in.PartitionCoefficient().present()) {
      io::Property::Marshall(in.PartitionCoefficient(), out.GetPartitionCoefficient());
    }
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceTissuePharmacokinetics& in, CDM::SubstanceTissuePharmacokineticsData& out)
  {
    out.Name(in.m_Name);
    if (in.m_PartitionCoefficient != nullptr) {
      io::Property::UnMarshall(*in.m_PartitionCoefficient, out.PartitionCoefficient());
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstancePharmacodynamics
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

    //Set up map (Antibiotic effect not added to modifier list because it is implemented different from other modifiers)
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
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstancePharmacodynamics& in, CDM::SubstancePharmacodynamicsData& out)
  {
    if (in.HasBronchodilation())
      UnMarshall(*in.m_Bronchodilation, out.Bronchodilation());
    if (in.HasDiastolicPressureModifier())
      UnMarshall(*in.m_DiastolicPressureModifier, out.DiastolicPressureModifier());
    if (in.HasEMaxShapeParameter())
      io::Property::UnMarshall(*in.m_EMaxShapeParameter, out.EMaxShapeParameter());
    if (in.HasFeverModifier())
      UnMarshall(*in.m_FeverModifier, out.FeverModifier());
    if (in.HasHeartRateModifier())
      UnMarshall(*in.m_HeartRateModifier, out.HeartRateModifier());
    if (in.HasHemorrhageModifier())
      UnMarshall(*in.m_HemorrhageModifier, out.HemorrhageModifier());
    if (in.HasNeuromuscularBlock())
      UnMarshall(*in.m_NeuromuscularBlock, out.NeuromuscularBlock());
    if (in.HasPainModifier())
      UnMarshall(*in.m_PainModifier, out.PainModifier());
    if (in.HasPupilReactivityModifier())
      UnMarshall(*in.m_PupilReactivityModifier, out.PupilReactivityModifier());
    if (in.HasPupilSizeModifier())
      UnMarshall(*in.m_PupilReactivityModifier, out.PupilSizeModifier());
    if (in.HasRespirationRateModifier())
      UnMarshall(*in.m_RespirationRateModifier, out.RespirationRateModifier());
    if (in.HasSedation())
      UnMarshall(*in.m_Sedation, out.Sedation());
    if (in.HasSystolicPressureModifier())
      UnMarshall(*in.m_SystolicPressureModifier, out.SystolicPressureModifier());
    if (in.HasTidalVolumeModifier())
      UnMarshall(*in.m_TidalVolumeModifier, out.TidalVolumeModifier());
    if (in.HasTubularPermeabilityModifier())
      UnMarshall(*in.m_TubularPermeabilityModifier, out.TubularPermeabilityModifier());
    if (in.HasCentralNervousModifier())
      UnMarshall(*in.m_CentralNervousModifier, out.CentralNervousModifier());
    if (in.HasAntibacterialEffect())
      io::Property::UnMarshall(*in.m_AntibacterialEffect, out.AntibacterialEffect());
    if (in.HasEffectSiteRateConstant())
      io::Property::UnMarshall(*in.m_EffectSiteRateConstant, out.EffectSiteRateConstant());
  }
  //-----------------------------------------------------------------------------
  //class SESubstanceClearance
  void Substance::Marshall(const CDM::SubstanceClearanceData& in, SESubstanceClearance& out)
  {
    out.Clear();
    // Make sure dups match
    if (in.Systemic().present() && in.RenalDynamics()->Clearance().present() && in.Systemic().get().RenalClearance().value() != in.RenalDynamics()->Clearance().get().value()) {
      throw CommonDataModelException("Multiple Renal Clearances specified, but not the same. These must match at this time.");
    }
    if (in.Systemic().present() && in.RenalDynamics()->Regulation().present() && in.Systemic().get().FractionUnboundInPlasma().value() != in.RenalDynamics()->Regulation().get().FractionUnboundInPlasma().value()) {
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
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceClearance& in, CDM::SubstanceClearanceData& out)
  {
    if (in.HasSystemic()) {
      CDM::Systemic* sys(new CDM::Systemic());
      out.Systemic(std::unique_ptr<CDM::Systemic>(sys));

      if (in.HasFractionExcretedInFeces()) {
        io::Property::UnMarshall(*in.m_FractionExcretedInFeces, sys->FractionExcretedInFeces());
      }
      if (in.HasFractionExcretedInUrine()) {
        io::Property::UnMarshall(*in.m_FractionExcretedInUrine, sys->FractionExcretedInUrine());
      }
      if (in.HasFractionMetabolizedInGut()) {
        io::Property::UnMarshall(*in.m_FractionMetabolizedInGut, sys->FractionMetabolizedInGut());
      }
      if (in.HasFractionUnboundInPlasma()) {
        io::Property::UnMarshall(*in.m_FractionUnboundInPlasma, sys->FractionUnboundInPlasma());
      }
      if (in.HasRenalClearance()) {
        io::Property::UnMarshall(*in.m_RenalClearance, sys->RenalClearance());
      }
      if (in.HasIntrinsicClearance()) {
        io::Property::UnMarshall(*in.m_IntrinsicClearance, sys->IntrinsicClearance());
      }
      if (in.HasSystemicClearance()) {
        io::Property::UnMarshall(*in.m_SystemicClearance, sys->SystemicClearance());
      }
    }

    if (in.HasRenalDynamic()) {
      CDM::RenalDynamics* rd(new CDM::RenalDynamics());
      out.RenalDynamics(std::unique_ptr<CDM::RenalDynamics>(rd));

      if (in.m_RenalDynamic == RenalDynamic::Clearance && in.HasRenalClearance()) {
        io::Property::UnMarshall(*in.m_RenalClearance, rd->Clearance());
      } else if (in.m_RenalDynamic == RenalDynamic::Regulation) {
        CDM::Regulation* rdr(new CDM::Regulation());
        rd->Regulation(std::unique_ptr<CDM::Regulation>(rdr));

        if (in.HasChargeInBlood()) {
          rdr->ChargeInBlood(in.m_ChargeInBlood);
        }
        if (in.HasFractionUnboundInPlasma()) {
          io::Property::UnMarshall(*in.m_FractionUnboundInPlasma, rdr->FractionUnboundInPlasma());
        }
        if (in.HasRenalReabsorptionRatio()) {
          io::Property::UnMarshall(*in.m_RenalReabsorptionRatio, rdr->ReabsorptionRatio());
        }
        if (in.HasRenalTransportMaximum()) {
          io::Property::UnMarshall(*in.m_RenalTransportMaximum, rdr->TransportMaximum());
        }
      }
      if (in.HasGlomerularFilterability()) {
        io::Property::UnMarshall(*in.m_GlomerularFilterability, rd->GlomerularFilterability());
      }
      if (in.HasRenalFiltrationRate()) {
        io::Property::UnMarshall(*in.m_RenalFiltrationRate, rd->FiltrationRate());
      }
      if (in.HasRenalReabsorptionRate()) {
        io::Property::UnMarshall(*in.m_RenalReabsorptionRate, rd->ReabsorptionRate());
      }
      if (in.HasRenalExcretionRate()) {
        io::Property::UnMarshall(*in.m_RenalExcretionRate, rd->ExcretionRate());
      }
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstance
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
    if (in.Density().present()) {
      io::Property::Marshall(in.Density(), out.GetDensity());
    }
    if (in.MolarMass().present()) {
      io::Property::Marshall(in.MolarMass(), out.GetMolarMass());
    }

    if (in.MaximumDiffusionFlux().present()) {
      io::Property::Marshall(in.MaximumDiffusionFlux(), out.GetMaximumDiffusionFlux());
    }
    if (in.MichaelisCoefficient().present()) {
      io::Property::Marshall(in.MichaelisCoefficient(), out.GetMichaelisCoefficient());
    }
    if (in.MembraneResistance().present()) {
      io::Property::Marshall(in.MembraneResistance(), out.GetMembraneResistance());
    }

    if (in.BloodConcentration().present()) {
      io::Property::Marshall(in.BloodConcentration(), out.GetBloodConcentration());
    }
    if (in.MassInBody().present()) {
      io::Property::Marshall(in.MassInBody(), out.GetMassInBody());
    }
    if (in.MassInBlood().present()) {
      io::Property::Marshall(in.MassInBlood(), out.GetMassInBlood());
    }
    if (in.MassInTissue().present()) {
      io::Property::Marshall(in.MassInTissue(), out.GetMassInTissue());
    }
    if (in.PlasmaConcentration().present()) {
      io::Property::Marshall(in.PlasmaConcentration(), out.GetPlasmaConcentration());
    }
    if (in.EffectSiteConcentration().present()) {
      io::Property::Marshall(in.EffectSiteConcentration(), out.GetEffectSiteConcentration());
    }
    if (in.SystemicMassCleared().present()) {
      io::Property::Marshall(in.SystemicMassCleared(), out.GetSystemicMassCleared());
    }
    if (in.TissueConcentration().present()) {
      io::Property::Marshall(in.TissueConcentration(), out.GetTissueConcentration());
    }

    if (in.AlveolarTransfer().present()) {
      io::Property::Marshall(in.AlveolarTransfer(), out.GetAlveolarTransfer());
    }
    if (in.DiffusingCapacity().present()) {
      io::Property::Marshall(in.DiffusingCapacity(), out.GetDiffusingCapacity());
    }
    if (in.EndTidalFraction().present()) {
      io::Property::Marshall(in.EndTidalFraction(), out.GetEndTidalFraction());
    }
    if (in.EndTidalPressure().present()) {
      io::Property::Marshall(in.EndTidalPressure(), out.GetEndTidalPressure());
    }
    if (in.RelativeDiffusionCoefficient().present()) {
      io::Property::Marshall(in.RelativeDiffusionCoefficient(), out.GetRelativeDiffusionCoefficient());
    }
    if (in.SolubilityCoefficient().present()) {
      io::Property::Marshall(in.SolubilityCoefficient(), out.GetSolubilityCoefficient());
    }

    if (in.Aerosolization().present()) {
      Marshall(in.Aerosolization(), out.GetAerosolization());
    }
    if (in.Clearance().present()) {
      Marshall(in.Clearance(), out.GetClearance());
    }
    if (in.Pharmacokinetics().present()) {
      Marshall(in.Pharmacokinetics(), out.GetPK());
    }
    if (in.Pharmacodynamics().present()) {
      Marshall(in.Pharmacodynamics(), out.GetPD());
    }

    if (out.HasClearance() && out.HasPK() && out.GetPK().HasPhysicochemicals()
        && out.GetClearance().HasFractionUnboundInPlasma()
        && !out.GetClearance().GetFractionUnboundInPlasma().Equals(out.GetPK().GetPhysicochemicals().GetFractionUnboundInPlasma())) {

      throw CommonDataModelException("Multiple FractionUnboundInPlasma values specified, but not the same. These must match at this time.");
    }
  }
  //-----------------------------------------------------------------------------
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
    if (in.HasDensity()) {
      io::Property::UnMarshall(*in.m_Density, out.Density());
    }
    if (in.HasMolarMass()) {
      io::Property::UnMarshall(*in.m_MolarMass, out.MolarMass());
    }

    if (in.HasMaximumDiffusionFlux()) {
      io::Property::UnMarshall(*in.m_MaximumDiffusionFlux, out.MaximumDiffusionFlux());
    }
    if (in.HasMichaelisCoefficient()) {
      io::Property::UnMarshall(*in.m_MichaelisCoefficient, out.MichaelisCoefficient());
    }
    if (in.HasMembraneResistance()) {
      io::Property::UnMarshall(*in.m_MembraneResistance, out.MembraneResistance());
    }

    if (in.HasBloodConcentration()) {
      io::Property::UnMarshall(*in.m_BloodConcentration, out.BloodConcentration());
    }
    if (in.HasMassInBody()) {
      io::Property::UnMarshall(*in.m_MassInBody, out.MassInBody());
    }
    if (in.HasMassInBlood()) {
      io::Property::UnMarshall(*in.m_MassInBlood, out.MassInBlood());
    }
    if (in.HasMassInTissue()) {
      io::Property::UnMarshall(*in.m_MassInTissue, out.MassInTissue());
    }
    if (in.HasPlasmaConcentration()) {
      io::Property::UnMarshall(*in.m_PlasmaConcentration, out.PlasmaConcentration());
    }
    if (in.HasEffectSiteConcentration()) {
      io::Property::UnMarshall(*in.m_EffectSiteConcentration, out.EffectSiteConcentration());
    }
    if (in.HasSystemicMassCleared()) {
      io::Property::UnMarshall(*in.m_SystemicMassCleared, out.SystemicMassCleared());
    }
    if (in.HasTissueConcentration()) {
      io::Property::UnMarshall(*in.m_TissueConcentration, out.TissueConcentration());
    }

    if (in.HasAlveolarTransfer()) {
      io::Property::UnMarshall(*in.m_AlveolarTransfer, out.AlveolarTransfer());
    }
    if (in.HasDiffusingCapacity()) {
      io::Property::UnMarshall(*in.m_DiffusingCapacity, out.DiffusingCapacity());
    }
    if (in.HasEndTidalFraction()) {
      io::Property::UnMarshall(*in.m_EndTidalFraction, out.EndTidalFraction());
    }
    if (in.HasEndTidalPressure()) {
      io::Property::UnMarshall(*in.m_EndTidalPressure, out.EndTidalPressure());
    }
    if (in.HasSolubilityCoefficient()) {
      io::Property::UnMarshall(*in.m_SolubilityCoefficient, out.SolubilityCoefficient());
    }
    if (in.HasRelativeDiffusionCoefficient()) {
      io::Property::UnMarshall(*in.m_RelativeDiffusionCoefficient, out.RelativeDiffusionCoefficient());
    }

    if (in.HasAerosolization()) {
      UnMarshall(*in.m_Aerosolization, out.Aerosolization());
    }
    if (in.HasClearance()) {
      UnMarshall(*in.m_Clearance, out.Clearance());
    }
    if (in.HasPK()) {
      UnMarshall(*in.m_PK, out.Pharmacokinetics());
    }
    if (in.HasPD()) {
      UnMarshall(*in.m_PD, out.Pharmacodynamics());
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstanceCompound
  void Substance::Marshall(const CDM::SubstanceCompoundData& in, const SESubstanceManager& subMgr, SESubstanceCompound& out)
  {
    out.Clear();
    out.m_Name = in.Name();

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
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceCompound& in, CDM::SubstanceCompoundData& out)
  {
    if (in.HasName()) {
      out.Name(in.m_Name);
    }

    for (unsigned int i = 0; i < in.m_Components.size(); i++) {
      auto data = CDM::SubstanceConcentrationData();
      UnMarshall(in.m_Components.at(i), data);
      out.Component().push_back(data);
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstanceFraction
  void Substance::Marshall(const CDM::SubstanceFractionData& in, SESubstanceFraction& out)
  {
    io::Property::Marshall(in.FractionAmount(), out.GetFractionAmount());
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceFraction& in, CDM::SubstanceFractionData& out)
  {
    out.Name(in.m_Substance.GetName());
    if (in.HasFractionAmount()) {
      io::Property::UnMarshall(*in.m_FractionAmount, out.FractionAmount());
    }
  }
  //-----------------------------------------------------------------------------
  //class SESubstanceConcentration
  void Substance::Marshall(const CDM::SubstanceConcentrationData& in, SESubstanceConcentration& out)
  {
    out.Clear();
    io::Property::Marshall(in.Concentration(), out.GetConcentration());
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SESubstanceConcentration& in, CDM::SubstanceConcentrationData& out)
  {
    out.Name(in.m_Substance->GetName());
    if (in.HasConcentration()) {
      io::Property::UnMarshall(in.m_Concentration, out.Concentration());
    }
  }
  //-----------------------------------------------------------------------------
  void Substance::Marshall(const CDM::PharmacodynamicModifierData& in, SEPharmacodynamicModifier& out)
  {
    io::Property::Marshall(in.EMax(), out.GetEMax());
    io::Property::Marshall(in.EC50(), out.GetEC50());
  }
  //-----------------------------------------------------------------------------
  void Substance::UnMarshall(const SEPharmacodynamicModifier& in, CDM::PharmacodynamicModifierData& out)
  {
    if (in.HasEMax()) {
      io::Property::UnMarshall(*in.m_EMax, out.EMax());
    }
    if (in.HasEC50()) {
      io::Property::UnMarshall(*in.m_EC50, out.EC50());
    }
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
  void Copy(const SE& in,  const SESubstanceManager& subMgr, SE& out)
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