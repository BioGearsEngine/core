#include "PatientAssessments.h"

#include "Property.h"

#include <biogears/cdm/patient/assessments/SEArterialBloodGasAnalysis.h>
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/cdm/patient/assessments/SEProthrombinTime.h>
#include <biogears/cdm/patient/assessments/SEPsychomotorVigilanceTask.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SESequentialOrganFailureAssessment.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>

#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalarAmount.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

namespace biogears {
namespace io {
  // class SEPatientAssessment
  void PatientAssessments::UnMarshall(const CDM::PatientAssessmentData& in, SEPatientAssessment& out)
  {
    // Empty Marshall
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEPatientAssessment& in, CDM::PatientAssessmentData& out)
  {
    // Empty UnMarshall
  }
  //----------------------------------------------------------------------------------
  // class SEArterialBloodGasAnalysis
  void PatientAssessments::UnMarshall(const CDM::ArterialBloodGasAnalysisData& in, SEArterialBloodGasAnalysis& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::UnMarshall(in.pH(), out.GetpH());
    io::Property::UnMarshall(in.PartialPressureOxygen(), out.GetPartialPressureOxygen());
    io::Property::UnMarshall(in.PartialPressureCarbonDioxide(), out.GetPartialPressureCarbonDioxide());
    io::Property::UnMarshall(in.BaseExcess(), out.GetBaseExcess());
    io::Property::UnMarshall(in.StandardBicarbonate(), out.GetStandardBicarbonate());
    io::Property::UnMarshall(in.OxygenSaturation(), out.GetOxygenSaturation());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEArterialBloodGasAnalysis& in, CDM::ArterialBloodGasAnalysisData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    CDM_PROPERTY_MARSHALL_HELPER(in, out, pH)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, PartialPressureOxygen)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, PartialPressureCarbonDioxide)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, BaseExcess)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, StandardBicarbonate)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, OxygenSaturation)
  }
  //----------------------------------------------------------------------------------
  // class SEPulmonaryFunctionTest
  void PatientAssessments::UnMarshall(const CDM::PulmonaryFunctionTestData& in, SEPulmonaryFunctionTest& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::UnMarshall(in.ExpiratoryReserveVolume(), out.GetExpiratoryReserveVolume());
    io::Property::UnMarshall(in.ForcedVitalCapacity(), out.GetForcedVitalCapacity());
    io::Property::UnMarshall(in.ForcedExpiratoryVolume(), out.GetForcedExpiratoryVolume());
    io::Property::UnMarshall(in.ForcedExpiratoryFlow(), out.GetForcedExpiratoryFlow());
    io::Property::UnMarshall(in.FunctionalResidualCapacity(), out.GetFunctionalResidualCapacity());
    io::Property::UnMarshall(in.InspiratoryReserveVolume(), out.GetInspiratoryReserveVolume());
    io::Property::UnMarshall(in.MaximumVoluntaryVentilation(), out.GetMaximumVoluntaryVentilation());
    io::Property::UnMarshall(in.PeakExpiratoryFlow(), out.GetPeakExpiratoryFlow());
    io::Property::UnMarshall(in.ResidualVolume(), out.GetResidualVolume());
    io::Property::UnMarshall(in.SlowVitalCapacity(), out.GetSlowVitalCapacity());
    io::Property::UnMarshall(in.TotalLungCapacity(), out.GetTotalLungCapacity());
    io::Property::UnMarshall(in.VitalCapacity(), out.GetVitalCapacity());
    io::Property::UnMarshall(in.LungVolumePlot(), out.GetLungVolumePlot());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEPulmonaryFunctionTest& in, CDM::PulmonaryFunctionTestData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));

    if (in.m_ExpiratoryReserveVolume != nullptr)
      io::Property::Marshall(*in.m_ExpiratoryReserveVolume, out.ExpiratoryReserveVolume());
    if (in.m_ForcedVitalCapacity != nullptr)
      io::Property::Marshall(*in.m_ForcedVitalCapacity, out.ForcedVitalCapacity());
    if (in.m_ForcedExpiratoryVolume != nullptr)
      io::Property::Marshall(*in.m_ForcedExpiratoryVolume, out.ForcedExpiratoryVolume());
    if (in.m_ForcedExpiratoryFlow != nullptr)
      io::Property::Marshall(*in.m_ForcedExpiratoryFlow, out.ForcedExpiratoryFlow());
    if (in.m_FunctionalResidualCapacity != nullptr)
      io::Property::Marshall(*in.m_FunctionalResidualCapacity, out.FunctionalResidualCapacity());
    if (in.m_InspiratoryCapacity != nullptr)
      io::Property::Marshall(*in.m_InspiratoryCapacity, out.InspiratoryCapacity());
    if (in.m_InspiratoryReserveVolume != nullptr)
      io::Property::Marshall(*in.m_InspiratoryReserveVolume, out.InspiratoryReserveVolume());
    if (in.m_MaximumVoluntaryVentilation != nullptr)
      io::Property::Marshall(*in.m_MaximumVoluntaryVentilation, out.MaximumVoluntaryVentilation());
    if (in.m_PeakExpiratoryFlow != nullptr)
      io::Property::Marshall(*in.m_PeakExpiratoryFlow, out.PeakExpiratoryFlow());
    if (in.m_ResidualVolume != nullptr)
      io::Property::Marshall(*in.m_ResidualVolume, out.ResidualVolume());
    if (in.m_SlowVitalCapacity != nullptr)
      io::Property::Marshall(*in.m_SlowVitalCapacity, out.SlowVitalCapacity());
    if (in.m_TotalLungCapacity != nullptr)
      io::Property::Marshall(*in.m_TotalLungCapacity, out.TotalLungCapacity());
    if (in.m_VitalCapacity != nullptr)
      io::Property::Marshall(*in.m_VitalCapacity, out.VitalCapacity());
    if (in.m_LungVolumePlot != nullptr)
      io::Property::Marshall(*in.m_LungVolumePlot, out.LungVolumePlot());
  }
  //----------------------------------------------------------------------------------
  // class SECompleteBloodCount
  void PatientAssessments::UnMarshall(const CDM::CompleteBloodCountData& in, SECompleteBloodCount& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::UnMarshall(in.Hematocrit(), out.GetHematocrit());
    io::Property::UnMarshall(in.Hemoglobin(), out.GetHemoglobin());
    io::Property::UnMarshall(in.LymphocyteCellCount(), out.GetLymphocyteCellCount());
    io::Property::UnMarshall(in.PlateletCount(), out.GetPlateletCount());
    io::Property::UnMarshall(in.MeanCorpuscularHemoglobin(), out.GetMeanCorpuscularHemoglobin());
    io::Property::UnMarshall(in.MeanCorpuscularHemoglobinConcentration(), out.GetMeanCorpuscularHemoglobinConcentration());
    io::Property::UnMarshall(in.MeanCorpuscularVolume(), out.GetMeanCorpuscularVolume());
    io::Property::UnMarshall(in.NeutrophilCellCount(), out.GetNeutrophilCount());
    io::Property::UnMarshall(in.RedBloodCellCount(), out.GetRedBloodCellCount());
    io::Property::UnMarshall(in.WhiteBloodCellCount(), out.GetWhiteBloodCellCount());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SECompleteBloodCount& in, CDM::CompleteBloodCountData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));

    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Hematocrit)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, Hemoglobin)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, LymphocyteCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, PlateletCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCorpuscularHemoglobin)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCorpuscularHemoglobinConcentration)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, MeanCorpuscularVolume)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, NeutrophilCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, RedBloodCellCount)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, WhiteBloodCellCount)

  }
  //----------------------------------------------------------------------------------
  // class SEComprehensiveMetabolicPanel
  void PatientAssessments::UnMarshall(const CDM::ComprehensiveMetabolicPanelData& in, SEComprehensiveMetabolicPanel& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::UnMarshall(in.Albumin(), out.GetAlbumin());
    io::Property::UnMarshall(in.ALP(), out.GetALP());
    io::Property::UnMarshall(in.ALT(), out.GetALT());
    io::Property::UnMarshall(in.AST(), out.GetAST());
    io::Property::UnMarshall(in.BUN(), out.GetBUN());
    io::Property::UnMarshall(in.Calcium(), out.GetCalcium());
    io::Property::UnMarshall(in.CO2(), out.GetCO2());
    io::Property::UnMarshall(in.Creatinine(), out.GetCreatinine());
    io::Property::UnMarshall(in.Glucose(), out.GetGlucose());
    io::Property::UnMarshall(in.Potassium(), out.GetPotassium());
    io::Property::UnMarshall(in.Sodium(), out.GetSodium());
    io::Property::UnMarshall(in.TotalBilirubin(), out.GetTotalBilirubin());
    io::Property::UnMarshall(in.TotalProtein(), out.GetTotalProtein());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEComprehensiveMetabolicPanel& in, CDM::ComprehensiveMetabolicPanelData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasAlbumin()) {
      io::Property::Marshall(*in.m_Albumin, out.Albumin());
    }
    if (in.HasALP()) {
      io::Property::Marshall(*in.m_ALP, out.ALP());
    }
    if (in.HasALT()) {
      io::Property::Marshall(*in.m_ALT, out.ALT());
    }
    if (in.HasAST()) {
      io::Property::Marshall(*in.m_AST, out.AST());
    }
    if (in.HasBUN()) {
      io::Property::Marshall(*in.m_BUN, out.BUN());
    }
    if (in.HasCalcium()) {
      io::Property::Marshall(*in.m_Calcium, out.Calcium());
    }
    if (in.HasChloride()) {
      io::Property::Marshall(*in.m_Chloride, out.Chloride());
    }
    if (in.HasCO2()) {
      io::Property::Marshall(*in.m_CO2, out.CO2());
    }
    if (in.HasCreatinine()) {
      io::Property::Marshall(*in.m_Creatinine, out.Creatinine());
    }
    if (in.HasGlucose()) {
      io::Property::Marshall(*in.m_Glucose, out.Glucose());
    }
    if (in.HasPotassium()) {
      io::Property::Marshall(*in.m_Potassium, out.Potassium());
    }
    if (in.HasSodium()) {
      io::Property::Marshall(*in.m_Sodium, out.Sodium());
    }
    if (in.HasTotalBilirubin()) {
      io::Property::Marshall(*in.m_TotalBilirubin, out.TotalBilirubin());
    }
    if (in.HasTotalProtein()) {
      io::Property::Marshall(*in.m_TotalProtein, out.TotalProtein());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEUrinalysis
  void PatientAssessments::UnMarshall(const CDM::UrinalysisData& in, SEUrinalysis& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    if (in.Color().present())
      out.m_Color = in.Color().get();
    if (in.Appearance().present())
      out.m_Appearance = in.Appearance().get();
    if (in.Glucose().present())
      out.m_Glucose = in.Glucose().get();
    if (in.Ketone().present())
      out.m_Ketone = in.Ketone().get();
    io::Property::UnMarshall(in.Bilirubin(), out.GetBilirubinResult());
    io::Property::UnMarshall(in.SpecificGravity(), out.GetSpecificGravityResult());
    if (in.Blood().present())
      out.m_Blood = in.Blood().get();
    io::Property::UnMarshall(in.pH(), out.GetPHResult());
    if (in.Protein().present())
      out.m_Protein = in.Protein().get();
    io::Property::UnMarshall(in.Urobilinogen(), out.GetUrobilinogenResult());
    if (in.Nitrite().present())
      out.m_Nitrite = in.Nitrite().get();
    if (in.LeukocyteEsterase().present())
      out.m_LeukocyteEsterase = in.LeukocyteEsterase().get();
    UnMarshall(in.Microscopic(), out.GetMicroscopicResult());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEUrinalysis& in, CDM::UrinalysisData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasColorResult())
      out.Color(in.m_Color);
    if (in.HasAppearanceResult())
      out.Appearance(in.m_Appearance);
    if (in.HasGlucoseResult())
      out.Glucose(in.m_Glucose);
    if (in.HasKetoneResult())
      out.Ketone(in.m_Ketone);
    if (in.HasBilirubinResult()) {
      io::Property::Marshall(*in.m_Bilirubin, out.Bilirubin());
    }
    if (in.HasSpecificGravityResult()) {
      io::Property::Marshall(*in.m_SpecificGravity, out.SpecificGravity());
    }
    if (in.HasBloodResult())
      out.Blood(in.m_Blood);
    if (in.HasPHResult()) {
      io::Property::Marshall(*in.m_pH, out.pH());
    }
    if (in.HasProteinResult())
      out.Protein(in.m_Protein);
    if (in.HasUrobilinogenResult()) {
      io::Property::Marshall(*in.m_Urobilinogen, out.Urobilinogen());
    }
    if (in.HasNitriteResult())
      out.Nitrite(in.m_Nitrite);
    if (in.HasLeukocyteEsteraseResult())
      out.LeukocyteEsterase(in.m_LeukocyteEsterase);

    if (in.HasMicroscopicResult())
      Marshall(*in.m_Microscopic, out.Microscopic());
  }
  //----------------------------------------------------------------------------------
  //class SEUrinalysisMicroscopic
  void PatientAssessments::UnMarshall(const CDM::UrinalysisMicroscopicData& in, SEUrinalysisMicroscopic& out)
  {
    UnMarshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));

    out.m_ObservationType = in.ObservationType();
    io::Property::UnMarshall(in.RedBloodCells(), out.GetRedBloodCellsResult());
    io::Property::UnMarshall(in.WhiteBloodCells(), out.GetWhiteBloodCellsResult());

    if (in.EpithelialCells().present())
      out.m_EpithelialCells = in.EpithelialCells().get();
    io::Property::UnMarshall(in.Casts(), out.GetCastsResult());

    if (in.Crystals().present())
      out.m_Crystals = in.Crystals().get();
    if (in.Bacteria().present())
      out.m_Bacteria = in.Bacteria().get();
    if (in.Trichomonads().present())
      out.m_Trichomonads = in.Trichomonads().get();
    if (in.Yeast().present())
      out.m_Yeast = in.Yeast().get();
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::Marshall(const SEUrinalysisMicroscopic& in, CDM::UrinalysisMicroscopicData& out)
  {
    Marshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasObservationType())
      out.ObservationType(in.m_ObservationType);
    if (in.HasRedBloodCellsResult()) {
      io::Property::Marshall(*in.m_RedBloodCells, out.RedBloodCells());
    }
    if (in.HasWhiteBloodCellsResult()) {
      io::Property::Marshall(*in.m_WhiteBloodCells, out.WhiteBloodCells());
    }
    if (in.HasEpithelialCellsResult())
      out.EpithelialCells(in.m_EpithelialCells);
    if (in.HasCastsResult()) {
      io::Property::Marshall(*in.m_Casts, out.Casts());
    }
    if (in.HasCrystalsResult())
      out.Crystals(in.m_Crystals);
    if (in.HasBacteriaResult())
      out.Bacteria(in.m_Bacteria);
    if (in.HasTrichomonadsResult())
      out.Trichomonads(in.m_Trichomonads);
    if (in.HasYeastResult())
      out.Yeast(in.m_Yeast);
  }
  //----------------------------------------------------------------------------------
  // class SEProthrombinTime.h
  void PatientAssessments::UnMarshall(const CDM::ProthrombinTimeData& in, SEProthrombinTime& out)
  {
    io::PatientAssessments::UnMarshall(in, (SEPatientAssessment&)out);
    io::Property::UnMarshall(in.InternationalNormalizedRatio(), out.GetInternationalNormalizedRatio());
  }

  void PatientAssessments::Marshall(const SEProthrombinTime& in, CDM::ProthrombinTimeData& out)
  {
    io::PatientAssessments::Marshall((SEPatientAssessment const&)in, out);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, InternationalNormalizedRatio)
  }
  //----------------------------------------------------------------------------------
  // class SEPsychomotorVigilanceTask.h
  void PatientAssessments::UnMarshall(const CDM::PsychomotorVigilanceTaskData& in, SEPsychomotorVigilanceTask& out)
  {
    io::PatientAssessments::UnMarshall(in, (SEPatientAssessment&)out);
    io::Property::UnMarshall(in.AttentionLapses(), out.GetAttentionLapses());
    io::Property::UnMarshall(in.ReactionTime(), out.GetReactionTime());
  }
  void PatientAssessments::Marshall(const SEPsychomotorVigilanceTask& in, CDM::PsychomotorVigilanceTaskData& out)
  {
    io::PatientAssessments::Marshall((SEPatientAssessment const&)in, out);
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, AttentionLapses)
    CDM_OPTIONAL_PROPERTY_MARSHALL_HELPER(in, out, ReactionTime)
  }
  //----------------------------------------------------------------------------------
  // class SESequentialOrganFailureAssessment.h
  void PatientAssessments::UnMarshall(const CDM::SequentialOrganFailureAssessmentData& in, SESequentialOrganFailureAssessment& out)
  {
    io::PatientAssessments::UnMarshall(in, (SEPatientAssessment&)out);
    io::Property::UnMarshall(in.RespirationSOFA(), out.GetRespirationSOFA());
    io::Property::UnMarshall(in.CoagulationSOFA(), out.GetCoagulationSOFA());
    io::Property::UnMarshall(in.LiverSOFA(), out.GetLiverSOFA());
    io::Property::UnMarshall(in.CardiovascularSOFA(), out.GetCardiovascularSOFA());
    io::Property::UnMarshall(in.CentralNervousSOFA(), out.GetCentralNervousSOFA());
    io::Property::UnMarshall(in.RenalSOFA(), out.GetRenalSOFA());
  }
  void PatientAssessments::Marshall(const SESequentialOrganFailureAssessment& in, CDM::SequentialOrganFailureAssessmentData& out)
  {
    io::PatientAssessments::Marshall((SEPatientAssessment const&)in, out);
    CDM_PROPERTY_MARSHALL_HELPER(in, out, RespirationSOFA)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, CoagulationSOFA)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, LiverSOFA)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, CardiovascularSOFA)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, CentralNervousSOFA)
    CDM_PROPERTY_MARSHALL_HELPER(in, out, RenalSOFA)
  }

}
}