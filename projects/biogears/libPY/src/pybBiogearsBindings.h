/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under

the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#pragma once


#include <biogears//pybSECompartment.h>
#include <biogears//pybSECompartmentManager.h>
#include <biogears//pybPhysiologyEngineTrack.h>
#include <biogears//pybSEAcuteRespiratoryDistress.h>
#include <biogears//pybSEAcuteStress.h>
#include <biogears//pybSEAirwayObstruction.h>
#include <biogears//pybSEApnea.h>
#include <biogears//pybSEAsthmaAttack.h>
#include <biogears//pybSEBrainInjury.h>
#include <biogears//pybSEBreathHold.h>
#include <biogears//pybSEBronchoconstriction.h>
#include <biogears//pybSEBurnWound.h>
#include <biogears//pybSECardiacArrest.h>
#include <biogears//pybSEChestCompressionForce.h>
#include <biogears//pybSEChestCompressionForceScale.h>
#include <biogears//pybSEChestOcclusiveDressing.h>
#include <biogears//pybSEConsciousRespiration.h>
#include <biogears//pybSEConsumeNutrients.h>
#include <biogears//pybSEExercise.h>
#include <biogears//pybSEForcedExhale.h>
#include <biogears//pybSEForcedInhale.h>
#include <biogears//pybSEHemmorrhage.h>
#include <biogears//pybSEInfection.h>
#include <biogears//pybSEIntubation.h>
#include <biogears//pybSEMechanicalVentilation.h>
#include <biogears//pybSENeedleDecompression.h>
#include <biogears//pybSEOverride.h>
#include <biogears//pybSEPainStimulus.h>
#include <biogears//pybSEPatientAction.h>
#include <biogears//pybSEPatientAssessmentRequest.h>
#include <biogears//pybSEPericardialEffusion.h>
#include <biogears//pybSEPupillaryResponse.h>
#include <biogears//pybSESleep.h>
#include <biogears//pybSESubstanceBolus.h>
#include <biogears//pybSESubstanceCompoundInfusion.h>
#include <biogears//pybSESubstanceInfusion.h>
#include <biogears//pybSESubstanceNasalDose.h>
#include <biogears//pybSESubstanceOralDose.h>
#include <biogears//pybSETensionPneumothroax.h>
#include <biogears//pybSETourniquet.h>
#include <biogears//pybSEUrinate.h>
#include <biogears//pybSEUseInhaler.h>
#include <biogears//pybSECompleteBloodCount.h>
#include <biogears//pybSEComprehensiveMetabolicPanel.h>
#include <biogears//pybSEPulmonaryFunctionTest.h>
#include <biogears//pybSESequentialOrganFailureAssessment.h>
#include <biogears//pybSEUrinalysis.h>
#include <biogears//pybSEUrinalysisMicroscopic.h>
#include <biogears//pybSEChronicAnemia.h>
#include <biogears//pybSEChronicObstructivePulmonaryDisease.h>
#include <biogears//pybSEChronicPericardialEffusion.h>
#include <biogears//pybSEChronicRenalStenosis.h>
#include <biogears//pybSEChronicVentricularSystolicDysfunction.h>
#include <biogears//pybSEDehydration.h>
#include <biogears//pybSEDiabetesType1.h>
#include <biogears//pybSEDiabetesType2.h>
#include <biogears//pybSEImpairedAlveolarExchange.h>
#include <biogears//pybSELobarPneumonia.h>
#include <biogears//pybSEStarvation.h>
#include <biogears//pybSENutrition.h>
#include <biogears//pybSEPatient.h>
#include <biogears//pybConstants.h>
#include <biogears//pybSEScalar0To1.h>
#include <biogears//pybSEAction.h>
#include <biogears//pybSEActionManager.h>
#include <biogears//pybSEAdvanceTime.h>
#include <biogears//pybSEAnesthesiaMachineActionCollection.h>
#include <biogears//pybSEConditionManager.h>
#include <biogears//pybSEEnvironmentActionCollection.h>
#include <biogears//pybSEInhalerActionCollection.h>
#include <biogears//pybSEPatientActionCollection.h>
#include <biogears//pybSEScenario.h>
#include <biogears//pybSEScenarioAutoSerialization.h>
#include <biogears//pybSEScenarioExec.h>
#include <biogears//pybSEScenarioInitialParameters.h>
#include <biogears//pybSESerializeState.h>
#include <biogears//pybSECompartmentDataRequest.h>
#include <biogears//pybSECompartmentSubstanceDataRequest.h>
#include <biogears//pybSEDataRequest.h>
#include <biogears//pybSEDataRequestManager.h>
#include <biogears//pybSEEnvironmentDataRequest.h>
#include <biogears//pybSEEquipmentDataRequest.h>
#include <biogears//pybSEGasCompartmentDataRequest.h>
#include <biogears//pybSELiquidCompartmentDataRequest.h>
#include <biogears//pybSEPatientDataRequest.h>
#include <biogears//pybSEPhysiologyDataRequest.h>
#include <biogears//pybSESubstanceDataRequest.h>
#include <biogears//pybSEThermalCompartmentDataRequest.h>
#include <biogears//pybSETissueCompartmentDataRequest.h>
#include <biogears//pybSESubstance.h>
#include <biogears//pybSESubstanceAerosolization.h>
#include <biogears//pybSESubstanceClearance.h>
#include <biogears//pybSESubstanceCompound.h>
#include <biogears//pybSESubstanceConcentration.h>
#include <biogears//pybSESubstanceFraction.h>
#include <biogears//pybSESubstanceManager.h>
#include <biogears//pybSESubstancePharmacodynamics.h>
#include <biogears//pybSESubstancePharmacokinetics.h>
#include <biogears//pybSESubstancePhysicochemicals.h>
#include <biogears//pybSESubstanceTissuePharmacokinetics.h>
#include <biogears//pybSEBloodChemistrySystem.h>
#include <biogears//pybSECardiovascularSystem.h>
#include <biogears//pybSEDrugSystem.h>
#include <biogears//pybSEEndocrineSystem.h>
#include <biogears//pybSEEnergySystem.h>
#include <biogears//pybSEGastrointestinalSystem.h>
#include <biogears//pybSEHepaticSystem.h>
#include <biogears//pybSENervousSystem.h>
#include <biogears//pybSERenalSystem.h>
#include <biogears//pybSERespiratorySystem.h>
#include <biogears//pybSETissueSystem.h>
#include <biogears//pybSESystem.h>
#include <biogears//pybLogger.h>
#include <biogears//pybBioGearsEngine.h>
#include <biogears//pybBioGearsEngineHeader.h>
#include <biogears//pybSEScalar.h>
#include <biogears//pybcardio.h>
#include <biogears//pybscalarpressure.h>


void define_pybSECompartment(py::module_ &m)
void define_pybSECompartmentManager(py::module_ &m)
void define_pybPhysiologyEngineTrack(py::module_ &m) {
void define_pybSEAcuteRespiratoryDistress(py::module_ &m) {
void define_pybSEAcuteStress(py::module_ &m) {
void define_pybSEAirwayObstruction(py::module_ &m) {
void define_pybSEApnea(py::module_ &m) {
void define_pybSEAsthmaAttack(py::module_ &m) {
void define_pybSEBrainInjury(py::module_ &m) {
void define_pybSEBreathHold(py::module_ &m)
void define_pybSEBronchoconstriction(py::module_ &m)
void define_pybSEBurnWound(py::module_ &m)
void define_pybSECardiacArrest(py::module_ &m)
void define_pybSEChestCompressionForce(py::module_ &m)
void define_pybSEChestCompressionForceScale(py::module_ &m)
void define_pybSEChestOcclusiveDressing(py::module_ &m)
void define_pybSEConsciousRespiration(py::module_ &m)
void define_pybSEConsumeNutrients(py::module_ &m)
void define_pybSEExercise(py::module_ &m)
void define_pybSEForcedExhale(py::module_ &m)
void define_pybSEForcedInhale(py::module_ &m)
void define_pybSEHemmorrhage(py::module_ &m)
void define_pybSEInfection(py::module_ &m)
void define_pybSEIntubation(py::module_ &m)
void define_pybSEMechanicalVentilation(py::module_ &m)
void define_pybSENeedleDecompression(py::module_ &m)
void define_pybSEOverride(py::module_ &m)
void define_pybSEPainStimulus(py::module_ &m)
void define_pybSEPatientAction(py::module_ &m) {
void define_pybSEPatientAssessmentRequest(py::module_ &m)
void define_pybSEPericardialEffusion(py::module_ &m)
void define_pybSEPupillaryResponse(py::module_ &m)
void define_pybSESleep(py::module_ &m)
void define_pybSESubstanceBolus(py::module_ &m)
void define_pybSESubstanceCompoundInfusion(py::module_ &m)
void define_pybSESubstanceInfusion(py::module_ &m)
void define_pybSESubstanceNasalDose(py::module_ &m)
void define_pybSESubstanceOralDose(py::module_ &m)
void define_pybSETensionPneumothroax(py::module_ &m)
void define_pybSETourniquet(py::module_ &m)
void define_pybSEUrinate(py::module_ &m)
void define_pybSEUseInhaler(py::module_ &m)
void define_pybSECompleteBloodCount(py::module_ &m)
void define_pybSEComprehensiveMetabolicPanel(py::module_ &m)
void define_pybSEPulmonaryFunctionTest(py::module_ &m)
void define_pybSESequentialOrganFailureAssessment(py::module_ &m)
void define_pybSEUrinalysis(py::module_ &m)
void define_pybSEUrinalysisMicroscopic(py::module_ &m)
void define_pybSEChronicAnemia(py::module_ &m)
void define_pybSEChronicObstructivePulmonaryDisease(py::module_ &m)
void define_pybSEChronicPericardialEffusion(py::module_ &m)
void define_pybSEChronicRenalStenosis(py::module_ &m)
void define_pybSEChronicVentricularSystolicDysfunction(py::module_ &m)
void define_pybSEDehydration(py::module_ &m)
void define_pybSEDiabetesType1(py::module_ &m)
void define_pybSEDiabetesType2(py::module_ &m)
void define_pybSEImpairedAlveolarExchange(py::module_ &m)
void define_pybSELobarPneumonia(py::module_ &m)
void define_pybSEStarvation(py::module_ &m)
void define_pybSENutrition(py::module_ &m)
void define_pybSEPatient(py::module_ &m)
void define_pybConstants(py::module_ &m)
void define_pybSEScalar0To1(py::module_ &m)
void define_pybSEAction(py::module_ &m)
void define_pybSEActionManager(py::module_ &m)
void define_pybSEAdvanceTime(py::module_ &m)
void define_pybSEAnesthesiaMachineActionCollection(py::module_ &m)
void define_pybSEConditionManager(py::module_ &m)
void define_pybSEEnvironmentActionCollection(py::module_ &m)
void define_pybSEInhalerActionCollection(py::module_ &m)
void define_pybSEPatientActionCollection(py::module_ &m)
void define_pybSEScenario(py::module_ &m)
void define_pybSEScenarioAutoSerialization(py::module_ &m)
void define_pybSEScenarioExec(py::module_ &m)
void define_pybSEScenarioInitialParameters(py::module_ &m)
void define_pybSESerializeState(py::module_ &m)
void define_pybSECompartmentDataRequest(py::module_ &m)
void define_pybSECompartmentSubstanceDataRequest(py::module_ &m)
void define_pybSEDataRequest(py::module_ &m)
void define_pybSEDataRequestManager(py::module_ &m)
void define_pybSEEnvironmentDataRequest(py::module_ &m)
void define_pybSEEquipmentDataRequest(py::module_ &m)
void define_pybSEGasCompartmentDataRequest(py::module_ &m)
void define_pybSELiquidCompartmentDataRequest(py::module_ &m)
void define_pybSEPatientDataRequest(py::module_ &m)
void define_pybSEPhysiologyDataRequest(py::module_ &m)
void define_pybSESubstanceDataRequest(py::module_ &m)
void define_pybSEThermalCompartmentDataRequest(py::module_ &m)
void define_pybSETissueCompartmentDataRequest(py::module_ &m)
void define_pybSESubstance(py::module_ &m)
void define_pybSESubstanceAerosolization(py::module_ &m)
void define_pybSESubstanceClearance(py::module_ &m)
void define_pybSESubstanceCompound(py::module_ &m)
void define_pybSESubstanceConcentration(py::module_ &m)
void define_pybSESubstanceFraction(py::module_ &m)
void define_pybSESubstanceManager(py::module_ &m)
void define_pybSESubstancePharmacodynamics(py::module_ &m)
void define_pybSESubstancePharmacokinetics(py::module_ &m)
void define_pybSESubstancePhysicochemicals(py::module_ &m)
void define_pybSESubstanceTissuePharmacokinetics(py::module_ &m)
void define_pybSEBloodChemistrySystem(py::module_ &m)
void define_pybSECardiovascularSystem(py::module_ &m)
void define_pybSEDrugSystem(py::module_ &m)
void define_pybSEEndocrineSystem(py::module_ &m)
void define_pybSEEnergySystem(py::module_ &m)
void define_pybSEGastrointestinalSystem(py::module_ &m)
void define_pybSEHepaticSystem(py::module_ &m)
void define_pybSENervousSystem(py::module_ &m)
void define_pybSERenalSystem(py::module_ &m)
void define_pybSERespiratorySystem(py::module_ &m)
void define_pybSETissueSystem(py::module_ &m)
void define_pybSESystem(py::module_ &m)
void define_pybLogger(py::module_ &m) {
void define_pybBioGearsEngine(py::module_ &m)
void define_pybBioGearsEngineHeader(py::module_ &m)
void define_pybSEScalar(py::module_ &m)
void define_pybcardio(py::module_ &m) {
void define_pybscalarpressure(py::module_ &m) {