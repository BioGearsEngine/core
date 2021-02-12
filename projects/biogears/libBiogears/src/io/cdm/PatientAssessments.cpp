#include "PatientAssessments.h"

#include "Property.h"

#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEPatientAssessment.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/patient/assessments/SEUrinalysisMicroscopic.h>

#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarAmount.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>

namespace biogears {
namespace io {
  //class SEPatientAssessment
  void PatientAssessments::Marshall(const CDM::PatientAssessmentData& in, SEPatientAssessment& out)
  {
    //Empty Marshall
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::UnMarshall(const SEPatientAssessment& in, CDM::PatientAssessmentData& out)
  {
    //Empty UnMarshall
  }
  //----------------------------------------------------------------------------------
  //class SEPulmonaryFunctionTest
  void PatientAssessments::Marshall(const CDM::PulmonaryFunctionTestData& in, SEPulmonaryFunctionTest& out)
  {
    Marshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::Marshall(in.ExpiratoryReserveVolume(), out.GetExpiratoryReserveVolume());
    io::Property::Marshall(in.ForcedVitalCapacity(), out.GetForcedVitalCapacity());
    io::Property::Marshall(in.ForcedExpiratoryVolume(), out.GetForcedExpiratoryVolume());
    io::Property::Marshall(in.ForcedExpiratoryFlow(), out.GetForcedExpiratoryFlow());
    io::Property::Marshall(in.FunctionalResidualCapacity(), out.GetFunctionalResidualCapacity());
    io::Property::Marshall(in.InspiratoryReserveVolume(), out.GetInspiratoryReserveVolume());
    io::Property::Marshall(in.MaximumVoluntaryVentilation(), out.GetMaximumVoluntaryVentilation());
    io::Property::Marshall(in.PeakExpiratoryFlow(), out.GetPeakExpiratoryFlow());
    io::Property::Marshall(in.ResidualVolume(), out.GetResidualVolume());
    io::Property::Marshall(in.SlowVitalCapacity(), out.GetSlowVitalCapacity());
    io::Property::Marshall(in.TotalLungCapacity(), out.GetTotalLungCapacity());
    io::Property::Marshall(in.VitalCapacity(), out.GetVitalCapacity());
    io::Property::Marshall(in.LungVolumePlot(), out.GetLungVolumePlot());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::UnMarshall(const SEPulmonaryFunctionTest& in, CDM::PulmonaryFunctionTestData& out)
  {
    UnMarshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));

    if (in.m_ExpiratoryReserveVolume != nullptr)
      io::Property::UnMarshall(*in.m_ExpiratoryReserveVolume, out.ExpiratoryReserveVolume());
    if (in.m_ForcedVitalCapacity != nullptr)
      io::Property::UnMarshall(*in.m_ForcedVitalCapacity, out.ForcedVitalCapacity());
    if (in.m_ForcedExpiratoryVolume != nullptr)
      io::Property::UnMarshall(*in.m_ForcedExpiratoryVolume, out.ForcedExpiratoryVolume());
    if (in.m_ForcedExpiratoryFlow != nullptr)
      io::Property::UnMarshall(*in.m_ForcedExpiratoryFlow, out.ForcedExpiratoryFlow());
    if (in.m_FunctionalResidualCapacity != nullptr)
      io::Property::UnMarshall(*in.m_FunctionalResidualCapacity, out.FunctionalResidualCapacity());
    if (in.m_InspiratoryCapacity != nullptr)
      io::Property::UnMarshall(*in.m_InspiratoryCapacity, out.InspiratoryCapacity());
    if (in.m_InspiratoryReserveVolume != nullptr)
      io::Property::UnMarshall(*in.m_InspiratoryReserveVolume, out.InspiratoryReserveVolume());
    if (in.m_MaximumVoluntaryVentilation != nullptr)
      io::Property::UnMarshall(*in.m_MaximumVoluntaryVentilation, out.MaximumVoluntaryVentilation());
    if (in.m_PeakExpiratoryFlow != nullptr)
      io::Property::UnMarshall(*in.m_PeakExpiratoryFlow, out.PeakExpiratoryFlow());
    if (in.m_ResidualVolume != nullptr)
      io::Property::UnMarshall(*in.m_ResidualVolume, out.ResidualVolume());
    if (in.m_SlowVitalCapacity != nullptr)
      io::Property::UnMarshall(*in.m_SlowVitalCapacity, out.SlowVitalCapacity());
    if (in.m_TotalLungCapacity != nullptr)
      io::Property::UnMarshall(*in.m_TotalLungCapacity, out.TotalLungCapacity());
    if (in.m_VitalCapacity != nullptr)
      io::Property::UnMarshall(*in.m_VitalCapacity, out.VitalCapacity());
    if (in.m_LungVolumePlot != nullptr)
      io::Property::UnMarshall(*in.m_LungVolumePlot, out.LungVolumePlot());
  }
  //----------------------------------------------------------------------------------
  //class SECompleteBloodCount
  void PatientAssessments::Marshall(const CDM::CompleteBloodCountData& in, SECompleteBloodCount& out)
  {
    Marshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::Marshall(in.Hematocrit(), out.GetHematocrit());
    io::Property::Marshall(in.Hemoglobin(), out.GetHemoglobin());
    io::Property::Marshall(in.PlateletCount(), out.GetPlateletCount());
    io::Property::Marshall(in.MeanCorpuscularHemoglobin(), out.GetMeanCorpuscularHemoglobin());
    io::Property::Marshall(in.MeanCorpuscularHemoglobinConcentration(), out.GetMeanCorpuscularHemoglobinConcentration());
    io::Property::Marshall(in.MeanCorpuscularVolume(), out.GetMeanCorpuscularVolume());
    io::Property::Marshall(in.RedBloodCellCount(), out.GetRedBloodCellCount());
    io::Property::Marshall(in.WhiteBloodCellCount(), out.GetWhiteBloodCellCount());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::UnMarshall(const SECompleteBloodCount& in, CDM::CompleteBloodCountData& out)
  {
    UnMarshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasHematocrit()) {
      io::Property::UnMarshall(*in.m_Hematocrit, out.Hematocrit());
    }
    if (in.HasHemoglobin()) {
      io::Property::UnMarshall(*in.m_Hemoglobin, out.Hemoglobin());
    }
    if (in.HasPlateletCount()) {
      io::Property::UnMarshall(*in.m_PlateletCount, out.PlateletCount());
    }
    if (in.HasMeanCorpuscularHemoglobin()) {
      io::Property::UnMarshall(*in.m_MeanCorpuscularHemoglobin, out.MeanCorpuscularHemoglobin());
    }
    if (in.HasMeanCorpuscularHemoglobinConcentration()) {
      io::Property::UnMarshall(*in.m_MeanCorpuscularHemoglobinConcentration, out.MeanCorpuscularHemoglobinConcentration());
    }
    if (in.HasMeanCorpuscularVolume()) {
      io::Property::UnMarshall(*in.m_MeanCorpuscularVolume, out.MeanCorpuscularVolume());
    }
    if (in.HasRedBloodCellCount()) {
      io::Property::UnMarshall(*in.m_RedBloodCellCount, out.RedBloodCellCount());
    }
    if (in.HasWhiteBloodCellCount()) {
      io::Property::UnMarshall(*in.m_WhiteBloodCellCount, out.WhiteBloodCellCount());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEComprehensiveMetabolicPanel
  void PatientAssessments::Marshall(const CDM::ComprehensiveMetabolicPanelData& in, SEComprehensiveMetabolicPanel& out)
  {
    Marshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    io::Property::Marshall(in.Albumin(), out.GetAlbumin());
    io::Property::Marshall(in.ALP(), out.GetALP());
    io::Property::Marshall(in.ALT(), out.GetALT());
    io::Property::Marshall(in.AST(), out.GetAST());
    io::Property::Marshall(in.BUN(), out.GetBUN());
    io::Property::Marshall(in.Calcium(), out.GetCalcium());
    io::Property::Marshall(in.CO2(), out.GetCO2());
    io::Property::Marshall(in.Creatinine(), out.GetCreatinine());
    io::Property::Marshall(in.Glucose(), out.GetGlucose());
    io::Property::Marshall(in.Potassium(), out.GetPotassium());
    io::Property::Marshall(in.Sodium(), out.GetSodium());
    io::Property::Marshall(in.TotalBilirubin(), out.GetTotalBilirubin());
    io::Property::Marshall(in.TotalProtein(), out.GetTotalProtein());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::UnMarshall(const SEComprehensiveMetabolicPanel& in, CDM::ComprehensiveMetabolicPanelData& out)
  {
    UnMarshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasAlbumin()) {
      io::Property::UnMarshall(*in.m_Albumin, out.Albumin());
    }
    if (in.HasALP()) {
      io::Property::UnMarshall(*in.m_ALP, out.ALP());
    }
    if (in.HasALT()) {
      io::Property::UnMarshall(*in.m_ALT, out.ALT());
    }
    if (in.HasAST()) {
      io::Property::UnMarshall(*in.m_AST, out.AST());
    }
    if (in.HasBUN()) {
      io::Property::UnMarshall(*in.m_BUN, out.BUN());
    }
    if (in.HasCalcium()) {
      io::Property::UnMarshall(*in.m_Calcium, out.Calcium());
    }
    if (in.HasChloride()) {
      io::Property::UnMarshall(*in.m_Chloride, out.Chloride());
    }
    if (in.HasCO2()) {
      io::Property::UnMarshall(*in.m_CO2, out.CO2());
    }
    if (in.HasCreatinine()) {
      io::Property::UnMarshall(*in.m_Creatinine, out.Creatinine());
    }
    if (in.HasGlucose()) {
      io::Property::UnMarshall(*in.m_Glucose, out.Glucose());
    }
    if (in.HasPotassium()) {
      io::Property::UnMarshall(*in.m_Potassium, out.Potassium());
    }
    if (in.HasSodium()) {
      io::Property::UnMarshall(*in.m_Sodium, out.Sodium());
    }
    if (in.HasTotalBilirubin()) {
      io::Property::UnMarshall(*in.m_TotalBilirubin, out.TotalBilirubin());
    }
    if (in.HasTotalProtein()) {
      io::Property::UnMarshall(*in.m_TotalProtein, out.TotalProtein());
    }
  }
  //----------------------------------------------------------------------------------
  //class SEUrinalysis
  void PatientAssessments::Marshall(const CDM::UrinalysisData& in, SEUrinalysis& out)
  {
    Marshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));
    if (in.Color().present())
      out.m_Color = in.Color().get();
    if (in.Appearance().present())
      out.m_Appearance = in.Appearance().get();
    if (in.Glucose().present())
      out.m_Glucose = in.Glucose().get();
    if (in.Ketone().present())
      out.m_Ketone = in.Ketone().get();
    io::Property::Marshall(in.Bilirubin(), out.GetBilirubinResult());
    io::Property::Marshall(in.SpecificGravity(), out.GetSpecificGravityResult());
    if (in.Blood().present())
      out.m_Blood = in.Blood().get();
    io::Property::Marshall(in.pH(), out.GetPHResult());
    if (in.Protein().present())
      out.m_Protein = in.Protein().get();
    io::Property::Marshall(in.Urobilinogen(), out.GetUrobilinogenResult());
    if (in.Nitrite().present())
      out.m_Nitrite = in.Nitrite().get();
    if (in.LeukocyteEsterase().present())
      out.m_LeukocyteEsterase = in.LeukocyteEsterase().get();
    Marshall(in.Microscopic(), out.GetMicroscopicResult());
  }
  //----------------------------------------------------------------------------------
  void PatientAssessments::UnMarshall(const SEUrinalysis& in, CDM::UrinalysisData& out)
  {
    UnMarshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasColorResult())
      out.Color(in.m_Color);
    if (in.HasAppearanceResult())
      out.Appearance(in.m_Appearance);
    if (in.HasGlucoseResult())
      out.Glucose(in.m_Glucose);
    if (in.HasKetoneResult())
      out.Ketone(in.m_Ketone);
    if (in.HasBilirubinResult()) {
      io::Property::UnMarshall(*in.m_Bilirubin, out.Bilirubin());
    }
    if (in.HasSpecificGravityResult()) {
      io::Property::UnMarshall(*in.m_SpecificGravity, out.SpecificGravity());
    }
    if (in.HasBloodResult())
      out.Blood(in.m_Blood);
    if (in.HasPHResult()) {
      io::Property::UnMarshall(*in.m_pH, out.pH());
    }
    if (in.HasProteinResult())
      out.Protein(in.m_Protein);
    if (in.HasUrobilinogenResult()) {
      io::Property::UnMarshall(*in.m_Urobilinogen, out.Urobilinogen());
    }
    if (in.HasNitriteResult())
      out.Nitrite(in.m_Nitrite);
    if (in.HasLeukocyteEsteraseResult())
      out.LeukocyteEsterase(in.m_LeukocyteEsterase);

    if (in.HasMicroscopicResult())
      UnMarshall(*in.m_Microscopic, out.Microscopic());
  }
  //----------------------------------------------------------------------------------
  //class SEUrinalysisMicroscopic
  void PatientAssessments::Marshall(const CDM::UrinalysisMicroscopicData& in, SEUrinalysisMicroscopic& out)
  {
    Marshall(static_cast<const CDM::PatientAssessmentData&>(in), static_cast<SEPatientAssessment&>(out));

    out.m_ObservationType = in.ObservationType();
    io::Property::Marshall(in.RedBloodCells(), out.GetRedBloodCellsResult());
    io::Property::Marshall(in.WhiteBloodCells(), out.GetWhiteBloodCellsResult());

    if (in.EpithelialCells().present())
      out.m_EpithelialCells = in.EpithelialCells().get();
    io::Property::Marshall(in.Casts(), out.GetCastsResult());

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
  void PatientAssessments::UnMarshall(const SEUrinalysisMicroscopic& in, CDM::UrinalysisMicroscopicData& out)
  {
    UnMarshall(static_cast<const SEPatientAssessment&>(in), static_cast<CDM::PatientAssessmentData&>(out));
    if (in.HasObservationType())
      out.ObservationType(in.m_ObservationType);
    if (in.HasRedBloodCellsResult()) {
      io::Property::UnMarshall(*in.m_RedBloodCells, out.RedBloodCells());
    }
    if (in.HasWhiteBloodCellsResult()) {
      io::Property::UnMarshall(*in.m_WhiteBloodCells, out.WhiteBloodCells());
    }
    if (in.HasEpithelialCellsResult())
      out.EpithelialCells(in.m_EpithelialCells);
    if (in.HasCastsResult()) {
      io::Property::UnMarshall(*in.m_Casts, out.Casts());
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
}
}