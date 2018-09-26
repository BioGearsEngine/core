/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include <biogears/cdm/test/CommonDataModelTest.h>

#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/tissue/SETissueCompartment.h>
#include <biogears/cdm/engine/PhysiologyEngineConfiguration.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarMassPerMass.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/utils/TimingProfile.h>
#include <biogears/cdm/utils/testing/SETestCase.h>
#include <biogears/cdm/utils/testing/SETestReport.h>
#include <biogears/cdm/utils/testing/SETestSuite.h>

namespace biogears {
void CommonDataModelTest::TissueCompartmentTest(const std::string& rptDirectory)
{
  TimingProfile pTimer;

  m_Logger->ResetLogFile(rptDirectory + "\\TissueCompartmentTest.log");
  SESubstanceManager subMgr(m_Logger);
  subMgr.LoadSubstanceDirectory();

  m_PercentTolerance = 0.2;
  m_OutDirectory = rptDirectory;

  SETestReport testReport(m_Logger);
  SETestSuite& testSuite = testReport.CreateTestSuite();
  testSuite.SetName("TissuCompartmenteTest");

  // Tissues do not support hierarchies, and LiquidCompartments run the gambit for Extracelluar/Intracellular
  // So I am only going to Create a tissue cmpt and test serialization

  pTimer.Start("Serialization");
  SETestCase& testCase = testSuite.CreateTestCase();
  testCase.SetName("CompartmentTissueTest");

  double AcidicPhospohlipidConcentration_mg_Per_g = 20;
  double MatrixVolume_mL = 5000;
  double NeutralLipidsVolumeFraction = 0.6;
  double NeutralPhospholipidsVolumeFraction = 0.77;
  double TissueToPlasmaAlbuminRatio = 1.0;
  double TissueToPlasmaAlphaAcidGlycoproteinRatio = 0.8;
  double TissueToPlasmaLipoproteinRatio = 0.54;
  //double TotalMass_mg = 333;

  double ec_pH = 7.14;
  double ec_WaterVolumeFraction = 0.67;

  double ic_pH = 7.42;
  double ic_WaterVolumeFraction = 0.76;

  SECompartmentManager cmptMgr(subMgr);
  SETissueCompartment* tissue = &cmptMgr.CreateTissueCompartment("Tissue");
  tissue->GetAcidicPhospohlipidConcentration().SetValue(AcidicPhospohlipidConcentration_mg_Per_g, MassPerMassUnit::mg_Per_g);
  tissue->GetMatrixVolume().SetValue(MatrixVolume_mL, VolumeUnit::mL);
  tissue->GetNeutralLipidsVolumeFraction().SetValue(NeutralLipidsVolumeFraction);
  tissue->GetNeutralPhospholipidsVolumeFraction().SetValue(NeutralPhospholipidsVolumeFraction);
  tissue->GetTissueToPlasmaAlbuminRatio().SetValue(TissueToPlasmaAlbuminRatio);
  tissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio().SetValue(TissueToPlasmaAlphaAcidGlycoproteinRatio);
  tissue->GetTissueToPlasmaLipoproteinRatio().SetValue(TissueToPlasmaLipoproteinRatio);
  cmptMgr.StateChange();

  TestCompartmentSerialization(cmptMgr, m_OutDirectory + "/TestTissueSerialization.xml");
  tissue = cmptMgr.GetTissueCompartment("Tissue");
  const SETissueCompartment* cTissue = cmptMgr.GetTissueCompartment("Tissue");

  if (GeneralMath::PercentTolerance(tissue->GetAcidicPhospohlipidConcentration().GetValue(MassPerMassUnit::mg_Per_g), AcidicPhospohlipidConcentration_mg_Per_g) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " AcidicPhospohlipidConcentration is not correct : " << tissue->GetAcidicPhospohlipidConcentration().GetValue(MassPerMassUnit::mg_Per_g) << " expected " << AcidicPhospohlipidConcentration_mg_Per_g;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetAcidicPhospohlipidConcentration(MassPerMassUnit::mg_Per_g), AcidicPhospohlipidConcentration_mg_Per_g) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const AcidicPhospohlipidConcentration is not correct : " << cTissue->GetAcidicPhospohlipidConcentration(MassPerMassUnit::mg_Per_g) << " expected " << AcidicPhospohlipidConcentration_mg_Per_g;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetMatrixVolume().GetValue(VolumeUnit::mL), MatrixVolume_mL) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " MatrixVolume_mL is not correct : " << tissue->GetMatrixVolume().GetValue(VolumeUnit::mL) << " expected " << MatrixVolume_mL;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetMatrixVolume(VolumeUnit::mL), MatrixVolume_mL) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const MatrixVolume_mL is not correct : " << cTissue->GetMatrixVolume(VolumeUnit::mL) << " expected " << MatrixVolume_mL;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetNeutralLipidsVolumeFraction().GetValue(), NeutralLipidsVolumeFraction) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " NeutralLipidsVolumeFraction is not correct : " << tissue->GetNeutralLipidsVolumeFraction().GetValue() << " expected " << NeutralLipidsVolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetNeutralLipidsVolumeFraction(), NeutralLipidsVolumeFraction) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const NeutralLipidsVolumeFraction is not correct : " << cTissue->GetNeutralLipidsVolumeFraction() << " expected " << NeutralLipidsVolumeFraction;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetNeutralPhospholipidsVolumeFraction().GetValue(), NeutralPhospholipidsVolumeFraction) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " NeutralPhospholipidsVolumeFraction is not correct : " << tissue->GetNeutralPhospholipidsVolumeFraction().GetValue() << " expected " << NeutralPhospholipidsVolumeFraction;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetNeutralPhospholipidsVolumeFraction(), NeutralPhospholipidsVolumeFraction) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const NeutralPhospholipidsVolumeFraction is not correct : " << cTissue->GetNeutralPhospholipidsVolumeFraction() << " expected " << NeutralPhospholipidsVolumeFraction;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetTissueToPlasmaAlbuminRatio().GetValue(), TissueToPlasmaAlbuminRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " TissueToPlasmaAlbuminRatio is not correct : " << tissue->GetTissueToPlasmaAlbuminRatio().GetValue() << " expected " << TissueToPlasmaAlbuminRatio;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetTissueToPlasmaAlbuminRatio(), TissueToPlasmaAlbuminRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const TissueToPlasmaAlbuminRatio is not correct : " << cTissue->GetTissueToPlasmaAlbuminRatio() << " expected " << TissueToPlasmaAlbuminRatio;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio().GetValue(), TissueToPlasmaAlphaAcidGlycoproteinRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " TissueToPlasmaAlphaAcidGlycoproteinRatio is not correct : " << tissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio().GetValue() << " expected " << TissueToPlasmaAlphaAcidGlycoproteinRatio;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio(), TissueToPlasmaAlphaAcidGlycoproteinRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const TissueToPlasmaAlphaAcidGlycoproteinRatio is not correct : " << cTissue->GetTissueToPlasmaAlphaAcidGlycoproteinRatio() << " expected " << TissueToPlasmaAlphaAcidGlycoproteinRatio;
    testCase.AddFailure(m_ss);
  }

  if (GeneralMath::PercentTolerance(tissue->GetTissueToPlasmaLipoproteinRatio().GetValue(), TissueToPlasmaLipoproteinRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " TissueToPlasmaLipoproteinRatio is not correct : " << tissue->GetTissueToPlasmaLipoproteinRatio().GetValue() << " expected " << TissueToPlasmaLipoproteinRatio;
    testCase.AddFailure(m_ss);
  }
  if (GeneralMath::PercentTolerance(cTissue->GetTissueToPlasmaLipoproteinRatio(), TissueToPlasmaLipoproteinRatio) > m_PercentTolerance) {
    m_ss << cTissue->GetName() << " const TissueToPlasmaLipoproteinRatio is not correct : " << cTissue->GetTissueToPlasmaLipoproteinRatio() << " expected " << TissueToPlasmaLipoproteinRatio;
    testCase.AddFailure(m_ss);
  }

  testCase.GetDuration().SetValue(pTimer.GetElapsedTime_s("Serialization"), TimeUnit::s);

  testReport.WriteFile(rptDirectory + "/TissueCompartmentTestReport.xml");
}
}