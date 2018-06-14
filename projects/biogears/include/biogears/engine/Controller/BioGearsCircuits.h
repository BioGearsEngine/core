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

#pragma once
class BioGears;

#include <biogears/cdm/circuit/SECircuitManager.h>

#include <biogears/schema/enumOnOff.hxx>

/**
* @brief Manages all circuits associated with all %BioGears systems/equipement
*/
class BIOGEARS_API BioGearsCircuits : public SECircuitManager {
public:
  BioGearsCircuits(BioGears& data);
  virtual ~BioGearsCircuits();
  void Clear();

  virtual bool Load(const CDM::CircuitManagerData& in);

  virtual void SetReadOnlyFluid(bool b);

  // Active methods return the circuit configuration to be calculated
  SEFluidCircuit& GetActiveCardiovascularCircuit();
  SEFluidCircuit& GetCardiovascularCircuit();
  SEFluidCircuit& GetRenalCircuit();

  SEFluidCircuit& GetActiveRespiratoryCircuit();
  SEFluidCircuit& GetRespiratoryCircuit();
  SEFluidCircuit& GetRespiratoryAndAnesthesiaMachineCircuit();
  SEFluidCircuit& GetRespiratoryAndInhalerCircuit();
  SEFluidCircuit& GetRespiratoryAndMechanicalVentilatorCircuit();

  SEThermalCircuit& GetTemperatureCircuit();
  SEThermalCircuit& GetExternalTemperatureCircuit();
  SEThermalCircuit& GetInternalTemperatureCircuit();

  SEFluidCircuit& GetAnesthesiaMachineCircuit();

protected:
  BioGears& m_data;

  // Note, I am not making a tissue or inhaler standalone circuit
  // this are super simple circuits and can't be tested by themselves

  // The combined cardiovascular circuit only needs to be built once,
  // but we can build it different ways, with or without renal and tissue subcircuits.
  // It depends on how the engine is configured, but this configuration does not change
  // once it's set. Tests can also configure how it's built and test appropriately.

  SEFluidCircuit* m_CombinedCardiovascularCircuit;

  SEFluidCircuit* m_CardiovascularCircuit;
  SEFluidCircuit* m_RenalCircuit;

  SEFluidCircuit* m_RespiratoryCircuit;

  SEFluidCircuit* m_AnesthesiaMachineCircuit;

  // Based on if equipment is hooked up, we have to build out the combination of
  // these 2 circuits and graphs as we don't want to dynamically modify circuits
  // It's quicker and easier to test these combiniation circuits
  SEFluidCircuit* m_CombinedRespiratoryAnesthesiaCircuit;
  SEFluidCircuit* m_CombinedRespiratoryInhalerCircuit;
  SEFluidCircuit* m_CombinedRespiratoryMechanicalVentilatorCircuit;

  SEThermalCircuit* m_TemperatureCircuit;
  SEThermalCircuit* m_InternalTemperatureCircuit; // Attempt at circuit independence
  SEThermalCircuit* m_ExternalTemperatureCircuit; // Attempt at circuit independence
};

///////////////////////////////
// Respiratory Circuit Enums //
///////////////////////////////

namespace mil {
namespace tatrc {
  namespace physiology {
    namespace biogears {

      class Circuits {
      public:
        DEFINE_STATIC_STRING(FullCardiovascular);
        DEFINE_STATIC_STRING(Cardiovascular);
        DEFINE_STATIC_STRING(Renal);
        DEFINE_STATIC_STRING(Respiratory);
        DEFINE_STATIC_STRING(AnesthesiaMachine);
        DEFINE_STATIC_STRING(RespiratoryAnesthesia);
        DEFINE_STATIC_STRING(RespiratoryInhaler);
        DEFINE_STATIC_STRING(RespiratoryMechanicalVentilator);
        DEFINE_STATIC_STRING(Temperature);
        DEFINE_STATIC_STRING(InternalTemperature);
        DEFINE_STATIC_STRING(ExternalTemperature);
      };

      ///////////////////////////
      // Chyme Circuit Enums //
      ///////////////////////////

      class ChymeNode {
      public:
        DEFINE_STATIC_STRING(SmallIntestineC1);
      };

      class ChymePath {
      public:
        DEFINE_STATIC_STRING(SmallIntestineC1ToSmallIntestine1);
        DEFINE_STATIC_STRING(GroundToSmallIntestineC1);
        DEFINE_STATIC_STRING(GutE3ToGroundGI);
      };

      class RespiratoryNode {
      public:
        DEFINE_STATIC_STRING(Carina);

        DEFINE_STATIC_STRING(LeftAlveoli);
        DEFINE_STATIC_STRING(LeftAlveoliLeak);
        DEFINE_STATIC_STRING(LeftAnatomicDeadSpace);
        DEFINE_STATIC_STRING(LeftChestLeak);
        DEFINE_STATIC_STRING(LeftPleural);
        DEFINE_STATIC_STRING(LeftPleuralConnection);

        DEFINE_STATIC_STRING(Mouth);

        DEFINE_STATIC_STRING(RightAlveoli);
        DEFINE_STATIC_STRING(RightAlveoliLeak);
        DEFINE_STATIC_STRING(RightAnatomicDeadSpace);
        DEFINE_STATIC_STRING(RightChestLeak);
        DEFINE_STATIC_STRING(RightPleural);
        DEFINE_STATIC_STRING(RightPleuralConnection);

        DEFINE_STATIC_STRING(Stomach);
        DEFINE_STATIC_STRING(RespiratoryMuscle);
      };

      class RespiratoryPath {
      public:
        DEFINE_STATIC_STRING(CarinaToLeftAnatomicDeadSpace);
        DEFINE_STATIC_STRING(CarinaToRightAnatomicDeadSpace);

        DEFINE_STATIC_STRING(EnvironmentToLeftChestLeak);
        DEFINE_STATIC_STRING(EnvironmentToMouth);
        DEFINE_STATIC_STRING(EnvironmentToRightChestLeak);

        DEFINE_STATIC_STRING(LeftAlveoliLeakToLeftPleural);
        DEFINE_STATIC_STRING(LeftAlveoliToLeftAlveoliLeak);
        DEFINE_STATIC_STRING(LeftAlveoliToLeftPleuralConnection);
        DEFINE_STATIC_STRING(LeftPleuralConnectionToLeftPleural);
        DEFINE_STATIC_STRING(LeftAnatomicDeadSpaceToLeftAlveoli);
        DEFINE_STATIC_STRING(LeftAnatomicDeadSpaceToLeftPleuralConnection);
        DEFINE_STATIC_STRING(LeftChestLeakToLeftPleural);
        DEFINE_STATIC_STRING(LeftPleuralToEnvironment);
        DEFINE_STATIC_STRING(LeftPleuralToRespiratoryMuscle);

        DEFINE_STATIC_STRING(MouthToCarina);
        DEFINE_STATIC_STRING(MouthToStomach);

        DEFINE_STATIC_STRING(RightAlveoliLeakToRightPleural);
        DEFINE_STATIC_STRING(RightAlveoliToRightAlveoliLeak);
        DEFINE_STATIC_STRING(RightAlveoliToRightPleuralConnection);
        DEFINE_STATIC_STRING(RightPleuralConnectionToRightPleural);
        DEFINE_STATIC_STRING(RightAnatomicDeadSpaceToRightAlveoli);
        DEFINE_STATIC_STRING(RightAnatomicDeadSpaceToRightPleuralConnection);
        DEFINE_STATIC_STRING(RightChestLeakToRightPleural);
        DEFINE_STATIC_STRING(RightPleuralToEnvironment);
        DEFINE_STATIC_STRING(RightPleuralToRespiratoryMuscle);

        DEFINE_STATIC_STRING(EnvironmentToRespiratoryMuscle);
        DEFINE_STATIC_STRING(StomachToEnvironment);
      };

      //////////////////////////////////////
      // Anesthesia Machine Circuit Enums //
      //////////////////////////////////////

      class AnesthesiaMachineNode {
      public:
        DEFINE_STATIC_STRING(AnesthesiaConnection);
        DEFINE_STATIC_STRING(ExpiratoryLimb);
        DEFINE_STATIC_STRING(GasInlet);
        DEFINE_STATIC_STRING(GasSource);
        DEFINE_STATIC_STRING(InspiratoryLimb);
        DEFINE_STATIC_STRING(ReliefValve);
        DEFINE_STATIC_STRING(Scrubber);
        DEFINE_STATIC_STRING(Selector);
        DEFINE_STATIC_STRING(Ventilator);
        DEFINE_STATIC_STRING(VentilatorConnection);
        DEFINE_STATIC_STRING(YPiece);
      };

      class AnesthesiaMachinePath {
      public:
        DEFINE_STATIC_STRING(EnvironmentToVentilator);
        DEFINE_STATIC_STRING(EnvironmentToReliefValve);
        DEFINE_STATIC_STRING(VentilatorToVentilatorConnection);
        DEFINE_STATIC_STRING(VentilatorConnectionToSelector);
        DEFINE_STATIC_STRING(SelectorToReliefValve);
        DEFINE_STATIC_STRING(SelectorToScrubber);
        DEFINE_STATIC_STRING(ScrubberToGasInlet);
        DEFINE_STATIC_STRING(EnvironmentToGasSource);
        DEFINE_STATIC_STRING(GasSourceToGasInlet);
        DEFINE_STATIC_STRING(GasInletToInspiratoryLimb);
        DEFINE_STATIC_STRING(InspiratoryLimbToYPiece);
        DEFINE_STATIC_STRING(YPieceToExpiratoryLimb);
        DEFINE_STATIC_STRING(ExpiratoryLimbToSelector);
        DEFINE_STATIC_STRING(YPieceToAnesthesiaConnection);
        DEFINE_STATIC_STRING(AnesthesiaConnectionToEnvironment);
        DEFINE_STATIC_STRING(SelectorToEnvironment);
      };

      class CombinedAnesthesiaMachinePath {
      public:
        DEFINE_STATIC_STRING(AnesthesiaConnectionToMouth);
        DEFINE_STATIC_STRING(GroundConnection);
      };

      ///////////////////////////
      // Inhaler Circuit Enums //
      ///////////////////////////

      class InhalerNode {
      public:
        DEFINE_STATIC_STRING(Mouthpiece);
      };

      class InhalerPath {
      public:
        DEFINE_STATIC_STRING(EnvironmentToMouthpiece);
        DEFINE_STATIC_STRING(MouthpieceToMouth);
      };

      //////////////////////////////////////////
      // Mechanical Ventilator Circuit Enums //
      ////////////////////////////////////////

      class MechanicalVentilatorNode {
      public:
        DEFINE_STATIC_STRING_EX(Connection, MechanicalVentilatorConnection);
      };

      class MechanicalVentilatorPath {
      public:
        DEFINE_STATIC_STRING_EX(ConnectionToMouth, MechanicalVentilatorConnectionToMouth);
        DEFINE_STATIC_STRING_EX(GroundToConnection, MechanicalVentilatorGroundToConnection);
      };

      ///////////////////////////////////
      // Environment Gas Circuit Enums //
      ///////////////////////////////////

      class EnvironmentNode {
      public:
        DEFINE_STATIC_STRING(Ambient);
      };

      ///////////////////////////////////////////
      // External Temperature Circuit Enums //
      ///////////////////////////////////////////

      class ExternalTemperatureNode {
      public:
        DEFINE_STATIC_STRING(Active);
        DEFINE_STATIC_STRING(Clothing);
        DEFINE_STATIC_STRING(Enclosure);
        DEFINE_STATIC_STRING(ExternalCore);
        DEFINE_STATIC_STRING(ExternalSkin);
        DEFINE_STATIC_STRING(ExternalGround);
        DEFINE_STATIC_STRING(Ambient);
      };

      class ExternalTemperaturePath {
      public:
        DEFINE_STATIC_STRING(ActiveToClothing);
        DEFINE_STATIC_STRING(ClothingToEnclosure);
        DEFINE_STATIC_STRING(ClothingToEnvironment);
        DEFINE_STATIC_STRING(ExternalCoreToGround);
        DEFINE_STATIC_STRING(GroundToActive);
        DEFINE_STATIC_STRING(GroundToClothing);
        DEFINE_STATIC_STRING(GroundToEnclosure);
        DEFINE_STATIC_STRING(GroundToEnvironment);
        DEFINE_STATIC_STRING(ExternalSkinToGround);
        DEFINE_STATIC_STRING(ExternalSkinToClothing);
      };

      ////////////////////////////////////////
      // Internal Temperature Circuit Enums //
      ////////////////////////////////////////

      class InternalTemperatureNode {
      public:
        DEFINE_STATIC_STRING(InternalCore);
        DEFINE_STATIC_STRING(InternalSkin);
        DEFINE_STATIC_STRING(InternalGround);
      };

      class InternalTemperaturePath {
      public:
        DEFINE_STATIC_STRING(GroundToInternalCore);
        DEFINE_STATIC_STRING(InternalCoreToInternalSkin);
        DEFINE_STATIC_STRING(InternalCoreToGround);
        DEFINE_STATIC_STRING(InternalSkinToGround);
      };

      ////////////////////////////////////////
      // Combined Temperature Circuit Enums //
      ////////////////////////////////////////

      class CombinedTemperaturePath {
      public:
        DEFINE_STATIC_STRING(InternalCoreToExternalCore);
        DEFINE_STATIC_STRING(InternalSkinToExternalSkin);
      };

      class CardiovascularNode {
      public:
        DEFINE_STATIC_STRING(RightHeart1);
        DEFINE_STATIC_STRING(RightHeart2);
        DEFINE_STATIC_STRING(RightHeart3);

        DEFINE_STATIC_STRING(MainPulmonaryArteries);
        DEFINE_STATIC_STRING(LeftIntermediatePulmonaryArteries);
        DEFINE_STATIC_STRING(LeftPulmonaryArteries);
        DEFINE_STATIC_STRING(RightIntermediatePulmonaryArteries);
        DEFINE_STATIC_STRING(RightPulmonaryArteries);

        DEFINE_STATIC_STRING(LeftPulmonaryCapillaries);
        DEFINE_STATIC_STRING(RightPulmonaryCapillaries);

        DEFINE_STATIC_STRING(LeftIntermediatePulmonaryVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryVeins);
        DEFINE_STATIC_STRING(RightIntermediatePulmonaryVeins);
        DEFINE_STATIC_STRING(RightPulmonaryVeins);

        DEFINE_STATIC_STRING(LeftHeart1);
        DEFINE_STATIC_STRING(LeftHeart2);
        DEFINE_STATIC_STRING(LeftHeart3);

        DEFINE_STATIC_STRING(Aorta1);
        DEFINE_STATIC_STRING(Aorta2);
        DEFINE_STATIC_STRING(Aorta3);

        DEFINE_STATIC_STRING(Bone1);
        DEFINE_STATIC_STRING(Bone2);

        DEFINE_STATIC_STRING(Brain1);
        DEFINE_STATIC_STRING(Brain2);

        DEFINE_STATIC_STRING(Fat1);
        DEFINE_STATIC_STRING(Fat2);

        DEFINE_STATIC_STRING(LargeIntestine1);

        DEFINE_STATIC_STRING(Liver1);
        DEFINE_STATIC_STRING(Liver2);
        DEFINE_STATIC_STRING(PortalVein1);

        DEFINE_STATIC_STRING(LeftArm1);
        DEFINE_STATIC_STRING(LeftArm2);

        DEFINE_STATIC_STRING(LeftKidney1);
        DEFINE_STATIC_STRING(LeftKidney2);

        DEFINE_STATIC_STRING(LeftLeg1);
        DEFINE_STATIC_STRING(LeftLeg2);

        DEFINE_STATIC_STRING(Muscle1);
        DEFINE_STATIC_STRING(Muscle2);

        DEFINE_STATIC_STRING(Myocardium1);
        DEFINE_STATIC_STRING(Myocardium2);

        DEFINE_STATIC_STRING(Pericardium1);

        DEFINE_STATIC_STRING(RightArm1);
        DEFINE_STATIC_STRING(RightArm2);

        DEFINE_STATIC_STRING(RightKidney1);
        DEFINE_STATIC_STRING(RightKidney2);

        DEFINE_STATIC_STRING(RightLeg1);
        DEFINE_STATIC_STRING(RightLeg2);

        DEFINE_STATIC_STRING(Skin1);
        DEFINE_STATIC_STRING(Skin2);

        DEFINE_STATIC_STRING(SmallIntestine1);

        DEFINE_STATIC_STRING(Splanchnic1);

        DEFINE_STATIC_STRING(Spleen1);

        DEFINE_STATIC_STRING(VenaCava);

        DEFINE_STATIC_STRING(Ground);
      };

      class CardiovascularPath {
      public:
        // Heart and Lungs
        DEFINE_STATIC_STRING(VenaCavaToRightHeart2);
        DEFINE_STATIC_STRING(RightHeart2ToRightHeart1);
        DEFINE_STATIC_STRING(RightHeart1ToRightHeart3);
        DEFINE_STATIC_STRING(RightHeart3ToGround);
        DEFINE_STATIC_STRING(RightHeart1ToMainPulmonaryArteries);
        DEFINE_STATIC_STRING(MainPulmonaryArteriesToRightIntermediatePulmonaryArteries);
        DEFINE_STATIC_STRING(RightIntermediatePulmonaryArteriesToRightPulmonaryArteries);
        DEFINE_STATIC_STRING(RightPulmonaryArteriesToRightPulmonaryVeins);
        DEFINE_STATIC_STRING(RightPulmonaryArteriesToRightPulmonaryCapillaries);
        DEFINE_STATIC_STRING(RightPulmonaryArteriesToGround);
        DEFINE_STATIC_STRING(RightPulmonaryCapillariesToRightPulmonaryVeins);
        DEFINE_STATIC_STRING(RightPulmonaryCapillariesToGround);
        DEFINE_STATIC_STRING(RightPulmonaryVeinsToRightIntermediatePulmonaryVeins);
        DEFINE_STATIC_STRING(RightPulmonaryVeinsToGround);
        DEFINE_STATIC_STRING(RightIntermediatePulmonaryVeinsToLeftHeart2);
        DEFINE_STATIC_STRING(MainPulmonaryArteriesToLeftIntermediatePulmonaryArteries);
        DEFINE_STATIC_STRING(LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries);
        DEFINE_STATIC_STRING(LeftPulmonaryArteriesToLeftPulmonaryVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryArteriesToLeftPulmonaryCapillaries);
        DEFINE_STATIC_STRING(LeftPulmonaryArteriesToGround);
        DEFINE_STATIC_STRING(LeftPulmonaryCapillariesToGround);
        DEFINE_STATIC_STRING(LeftPulmonaryCapillariesToLeftPulmonaryVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryVeinsToLeftIntermediatePulmonaryVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryVeinsToGround);
        DEFINE_STATIC_STRING(LeftIntermediatePulmonaryVeinsToLeftHeart2)
        DEFINE_STATIC_STRING(LeftHeart2ToLeftHeart1);
        DEFINE_STATIC_STRING(LeftHeart1ToLeftHeart3);
        DEFINE_STATIC_STRING(LeftHeart3ToGround);
        DEFINE_STATIC_STRING(LeftHeart1ToAorta2);
        DEFINE_STATIC_STRING(Aorta2ToAorta3);
        DEFINE_STATIC_STRING(Aorta3ToAorta1);
        DEFINE_STATIC_STRING(Aorta1ToGround);
        // Brain
        DEFINE_STATIC_STRING(Aorta1ToBrain1);
        DEFINE_STATIC_STRING(Brain1ToGround);
        DEFINE_STATIC_STRING(Brain1ToBrain2);
        DEFINE_STATIC_STRING(Brain2ToVenaCava);
        // Bone
        DEFINE_STATIC_STRING(Aorta1ToBone1);
        DEFINE_STATIC_STRING(Bone1ToGround);
        DEFINE_STATIC_STRING(Bone1ToBone2);
        DEFINE_STATIC_STRING(Bone2ToVenaCava);
        // Fat
        DEFINE_STATIC_STRING(Aorta1ToFat1);
        DEFINE_STATIC_STRING(Fat1ToGround);
        DEFINE_STATIC_STRING(Fat1ToFat2);
        DEFINE_STATIC_STRING(Fat2ToVenaCava);
        // Large Intestine
        DEFINE_STATIC_STRING(Aorta1ToLargeIntestine);
        DEFINE_STATIC_STRING(LargeIntestineToGround);
        DEFINE_STATIC_STRING(LargeIntestineToPortalVein);
        // Left Arm
        DEFINE_STATIC_STRING(Aorta1ToLeftArm1);
        DEFINE_STATIC_STRING(LeftArm1ToGround);
        DEFINE_STATIC_STRING(LeftArm1ToLeftArm2);
        DEFINE_STATIC_STRING(LeftArm2ToVenaCava);
        // Left Kidney
        DEFINE_STATIC_STRING(Aorta1ToLeftKidney1);
        DEFINE_STATIC_STRING(LeftKidney1ToGround);
        DEFINE_STATIC_STRING(LeftKidney1ToLeftKidney2);
        DEFINE_STATIC_STRING(LeftKidney2ToVenaCava);
        // Left Leg
        DEFINE_STATIC_STRING(Aorta1ToLeftLeg1);
        DEFINE_STATIC_STRING(LeftLeg1ToGround);
        DEFINE_STATIC_STRING(LeftLeg1ToLeftLeg2);
        DEFINE_STATIC_STRING(LeftLeg2ToVenaCava);
        // Liver
        DEFINE_STATIC_STRING(Aorta1ToLiver1);
        DEFINE_STATIC_STRING(Liver1ToGround);
        DEFINE_STATIC_STRING(PortalVeinToLiver1);
        DEFINE_STATIC_STRING(Liver1ToLiver2);
        DEFINE_STATIC_STRING(Liver2ToVenaCava);
        // Muscle
        DEFINE_STATIC_STRING(Aorta1ToMuscle1);
        DEFINE_STATIC_STRING(Muscle1ToGround);
        DEFINE_STATIC_STRING(Muscle1ToMuscle2);
        DEFINE_STATIC_STRING(Muscle2ToVenaCava);
        // Myocardium
        DEFINE_STATIC_STRING(Aorta1ToMyocardium1);
        DEFINE_STATIC_STRING(Myocardium1ToGround);
        DEFINE_STATIC_STRING(Myocardium1ToMyocardium2);
        DEFINE_STATIC_STRING(Myocardium2ToVenaCava);
        // Pericardium
        DEFINE_STATIC_STRING(Pericardium1ToGround);
        DEFINE_STATIC_STRING(GroundToPericardium1);
        // Right Arm
        DEFINE_STATIC_STRING(Aorta1ToRightArm1);
        DEFINE_STATIC_STRING(RightArm1ToGround);
        DEFINE_STATIC_STRING(RightArm1ToRightArm2);
        DEFINE_STATIC_STRING(RightArm2ToVenaCava);
        // Right Kidney
        DEFINE_STATIC_STRING(Aorta1ToRightKidney1);
        DEFINE_STATIC_STRING(RightKidney1ToGround);
        DEFINE_STATIC_STRING(RightKidney1ToRightKidney2);
        DEFINE_STATIC_STRING(RightKidney2ToVenaCava);
        // Right Leg
        DEFINE_STATIC_STRING(Aorta1ToRightLeg1);
        DEFINE_STATIC_STRING(RightLeg1ToGround);
        DEFINE_STATIC_STRING(RightLeg1ToRightLeg2);
        DEFINE_STATIC_STRING(RightLeg2ToVenaCava);
        // Skin
        DEFINE_STATIC_STRING(Aorta1ToSkin1);
        DEFINE_STATIC_STRING(Skin1ToGround);
        DEFINE_STATIC_STRING(Skin1ToSkin2);
        DEFINE_STATIC_STRING(Skin2ToVenaCava);
        // Small Intestine
        DEFINE_STATIC_STRING(Aorta1ToSmallIntestine);
        DEFINE_STATIC_STRING(SmallIntestineToGround);
        DEFINE_STATIC_STRING(SmallIntestineToPortalVein);
        // Splanchnic
        DEFINE_STATIC_STRING(Aorta1ToSplanchnic);
        DEFINE_STATIC_STRING(SplanchnicToGround);
        DEFINE_STATIC_STRING(SplanchnicToPortalVein);
        // Spleen
        DEFINE_STATIC_STRING(Aorta1ToSpleen);
        DEFINE_STATIC_STRING(SpleenToGround);
        DEFINE_STATIC_STRING(SpleenToPortalVein);
        // Vena Cava
        DEFINE_STATIC_STRING(VenaCavaToGround);
        DEFINE_STATIC_STRING(VenaCavaBleed);
        DEFINE_STATIC_STRING(IVToVenaCava);
        //Hemorrhage
        DEFINE_STATIC_STRING(AortaBleed);
        DEFINE_STATIC_STRING(BrainBleed);
        DEFINE_STATIC_STRING(HeartBleed);
        DEFINE_STATIC_STRING(LungBleed);
        DEFINE_STATIC_STRING(ArmBleed);
        DEFINE_STATIC_STRING(SpleenBleed);
        DEFINE_STATIC_STRING(SmallIntestineBleed);
        DEFINE_STATIC_STRING(LargeIntestineBleed);
        DEFINE_STATIC_STRING(SplanchnicBleed);
        DEFINE_STATIC_STRING(KidneyBleed);
        DEFINE_STATIC_STRING(LiverBleed);
        DEFINE_STATIC_STRING(PortalBleed);
        DEFINE_STATIC_STRING(LegBleed);
      };

      class TissueNode {
      public:
        DEFINE_STATIC_STRING(BoneE1);
        DEFINE_STATIC_STRING(BoneE2);
        DEFINE_STATIC_STRING(BoneE3);
        DEFINE_STATIC_STRING(BoneI);
        DEFINE_STATIC_STRING(BrainE1);
        DEFINE_STATIC_STRING(BrainE2);
        DEFINE_STATIC_STRING(BrainE3);
        DEFINE_STATIC_STRING(BrainI);
        DEFINE_STATIC_STRING(FatE1);
        DEFINE_STATIC_STRING(FatE2);
        DEFINE_STATIC_STRING(FatE3);
        DEFINE_STATIC_STRING(FatI);
        DEFINE_STATIC_STRING(GutE1);
        DEFINE_STATIC_STRING(GutE2);
        DEFINE_STATIC_STRING(GutE3);
        DEFINE_STATIC_STRING(GutI);
        DEFINE_STATIC_STRING(LiverE1);
        DEFINE_STATIC_STRING(LiverE2);
        DEFINE_STATIC_STRING(LiverE3);
        DEFINE_STATIC_STRING(LiverI);
        DEFINE_STATIC_STRING(LeftKidneyE1);
        DEFINE_STATIC_STRING(LeftKidneyE2);
        DEFINE_STATIC_STRING(LeftKidneyE3);
        DEFINE_STATIC_STRING(LeftKidneyI);
        DEFINE_STATIC_STRING(LeftLungE1);
        DEFINE_STATIC_STRING(LeftLungE2);
        DEFINE_STATIC_STRING(LeftLungE3);
        DEFINE_STATIC_STRING(LeftLungI);
        DEFINE_STATIC_STRING(Lymph);
        DEFINE_STATIC_STRING(MuscleE1);
        DEFINE_STATIC_STRING(MuscleE2);
        DEFINE_STATIC_STRING(MuscleE3)
        DEFINE_STATIC_STRING(MuscleI);
        ;
        DEFINE_STATIC_STRING(MyocardiumE1);
        DEFINE_STATIC_STRING(MyocardiumE2);
        DEFINE_STATIC_STRING(MyocardiumE3);
        DEFINE_STATIC_STRING(MyocardiumI);
        DEFINE_STATIC_STRING(RightLungE1);
        DEFINE_STATIC_STRING(RightLungE2);
        DEFINE_STATIC_STRING(RightLungE3);
        DEFINE_STATIC_STRING(RightLungI);
        DEFINE_STATIC_STRING(RightKidneyE1);
        DEFINE_STATIC_STRING(RightKidneyE2);
        DEFINE_STATIC_STRING(RightKidneyE3);
        DEFINE_STATIC_STRING(RightKidneyI);
        DEFINE_STATIC_STRING(SkinE1);
        DEFINE_STATIC_STRING(SkinE2);
        DEFINE_STATIC_STRING(SkinE3);
        DEFINE_STATIC_STRING(SkinI);
        DEFINE_STATIC_STRING(SpleenE1);
        DEFINE_STATIC_STRING(SpleenE2);
        DEFINE_STATIC_STRING(SpleenE3);
        DEFINE_STATIC_STRING(SpleenI);
      };

      class TissuePath {
      public:
        DEFINE_STATIC_STRING(BoneVToBoneE1);
        DEFINE_STATIC_STRING(BoneE1ToBoneE2);
        DEFINE_STATIC_STRING(BoneE2ToBoneE3);
        DEFINE_STATIC_STRING(BoneE3ToBoneI);
        DEFINE_STATIC_STRING(BoneE3ToGround);
        DEFINE_STATIC_STRING(BoneIToGround);
        DEFINE_STATIC_STRING(BoneE3ToLymph);

        DEFINE_STATIC_STRING(BrainVToBrainE1);
        DEFINE_STATIC_STRING(BrainE1ToBrainE2);
        DEFINE_STATIC_STRING(BrainE2ToBrainE3);
        DEFINE_STATIC_STRING(BrainE3ToBrainI);
        DEFINE_STATIC_STRING(BrainE3ToGround);
        DEFINE_STATIC_STRING(BrainIToGround);
        DEFINE_STATIC_STRING(BrainE3ToLymph);

        DEFINE_STATIC_STRING(FatVToFatE1);
        DEFINE_STATIC_STRING(FatE1ToFatE2);
        DEFINE_STATIC_STRING(FatE2ToFatE3);
        DEFINE_STATIC_STRING(FatE3ToFatI);
        DEFINE_STATIC_STRING(FatE3ToGround);
        DEFINE_STATIC_STRING(FatIToGround);
        DEFINE_STATIC_STRING(FatE3ToLymph);

        DEFINE_STATIC_STRING(SmallIntestineVToGutE1);
        DEFINE_STATIC_STRING(LargeIntestineVToGutE1);
        DEFINE_STATIC_STRING(SplanchnicVToGutE1);
        DEFINE_STATIC_STRING(GutE1ToGutE2);
        DEFINE_STATIC_STRING(GutE2ToGutE3);
        DEFINE_STATIC_STRING(GutE3ToGutI);
        DEFINE_STATIC_STRING(GutE3ToGround);
        DEFINE_STATIC_STRING(GutIToGround);
        DEFINE_STATIC_STRING(GutE3ToLymph);

        DEFINE_STATIC_STRING(LiverVToLiverE1);
        DEFINE_STATIC_STRING(LiverE1ToLiverE2);
        DEFINE_STATIC_STRING(LiverE2ToLiverE3);
        DEFINE_STATIC_STRING(LiverE3ToLiverI);
        DEFINE_STATIC_STRING(LiverE3ToGround);
        DEFINE_STATIC_STRING(LiverIToGround);
        DEFINE_STATIC_STRING(LiverE3ToLymph);

        DEFINE_STATIC_STRING(LeftKidneyVToLeftKidneyE1);
        DEFINE_STATIC_STRING(LeftKidneyE1ToLeftKidneyE2);
        DEFINE_STATIC_STRING(LeftKidneyE2ToLeftKidneyE3);
        DEFINE_STATIC_STRING(LeftKidneyE3ToLeftKidneyI);
        DEFINE_STATIC_STRING(LeftKidneyE3ToGround);
        DEFINE_STATIC_STRING(LeftKidneyIToGround);
        DEFINE_STATIC_STRING(LeftKidneyE3ToLymph);

        DEFINE_STATIC_STRING(LeftLungVToLeftLungE1);
        DEFINE_STATIC_STRING(LeftLungE1ToLeftLungE2);
        DEFINE_STATIC_STRING(LeftLungE2ToLeftLungE3);
        DEFINE_STATIC_STRING(LeftLungE3ToLeftLungI);
        DEFINE_STATIC_STRING(LeftLungE3ToGround);
        DEFINE_STATIC_STRING(LeftLungIToGround);
        DEFINE_STATIC_STRING(LeftLungE3ToLymph);

        DEFINE_STATIC_STRING(LymphToVenaCava);

        DEFINE_STATIC_STRING(MuscleVToMuscleE1);
        DEFINE_STATIC_STRING(MuscleE1ToMuscleE2);
        DEFINE_STATIC_STRING(MuscleE2ToMuscleE3);
        DEFINE_STATIC_STRING(MuscleE3ToMuscleI);
        DEFINE_STATIC_STRING(MuscleE3ToGround);
        DEFINE_STATIC_STRING(MuscleIToGround);
        DEFINE_STATIC_STRING(MuscleE3ToLymph);

        DEFINE_STATIC_STRING(MyocardiumVToMyocardiumE1);
        DEFINE_STATIC_STRING(MyocardiumE1ToMyocardiumE2);
        DEFINE_STATIC_STRING(MyocardiumE2ToMyocardiumE3);
        DEFINE_STATIC_STRING(MyocardiumE3ToMyocardiumI);
        DEFINE_STATIC_STRING(MyocardiumE3ToGround);
        DEFINE_STATIC_STRING(MyocardiumIToGround);
        DEFINE_STATIC_STRING(MyocardiumE3ToLymph);

        DEFINE_STATIC_STRING(RightLungVToRightLungE1);
        DEFINE_STATIC_STRING(RightLungE1ToRightLungE2);
        DEFINE_STATIC_STRING(RightLungE2ToRightLungE3);
        DEFINE_STATIC_STRING(RightLungE3ToRightLungI);
        DEFINE_STATIC_STRING(RightLungE3ToGround);
        DEFINE_STATIC_STRING(RightLungIToGround);
        DEFINE_STATIC_STRING(RightLungE3ToLymph);

        DEFINE_STATIC_STRING(RightKidneyVToRightKidneyE1);
        DEFINE_STATIC_STRING(RightKidneyE1ToRightKidneyE2);
        DEFINE_STATIC_STRING(RightKidneyE2ToRightKidneyE3);
        DEFINE_STATIC_STRING(RightKidneyE3ToRightKidneyI);
        DEFINE_STATIC_STRING(RightKidneyE3ToGround);
        DEFINE_STATIC_STRING(RightKidneyIToGround);
        DEFINE_STATIC_STRING(RightKidneyE3ToLymph);

        DEFINE_STATIC_STRING(SkinVToSkinE1);
        DEFINE_STATIC_STRING(SkinE1ToSkinE2);
        DEFINE_STATIC_STRING(SkinE2ToSkinE3);
        DEFINE_STATIC_STRING(SkinE3ToSkinI);
        DEFINE_STATIC_STRING(SkinE3ToGround);
        DEFINE_STATIC_STRING(SkinIToGround);
        DEFINE_STATIC_STRING(SkinE3ToLymph);
        DEFINE_STATIC_STRING(SkinSweating);

        DEFINE_STATIC_STRING(SpleenVToSpleenE1);
        DEFINE_STATIC_STRING(SpleenE1ToSpleenE2);
        DEFINE_STATIC_STRING(SpleenE2ToSpleenE3);
        DEFINE_STATIC_STRING(SpleenE3ToSpleenI);
        DEFINE_STATIC_STRING(SpleenE3ToGround);
        DEFINE_STATIC_STRING(SpleenIToGround);
        DEFINE_STATIC_STRING(SpleenE3ToLymph);
      };

      class RenalNode {
      public:
        // Blood
        DEFINE_STATIC_STRING(RightAortaConnection);
        DEFINE_STATIC_STRING(RightRenalArtery);
        DEFINE_STATIC_STRING(RightAfferentArteriole);
        DEFINE_STATIC_STRING(RightGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightNetGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightEfferentArteriole);
        DEFINE_STATIC_STRING(RightPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightRenalVein);
        DEFINE_STATIC_STRING(RightVenaCavaConnection);
        // Urine
        DEFINE_STATIC_STRING(RightBowmansCapsules);
        DEFINE_STATIC_STRING(RightNetBowmansCapsules);
        DEFINE_STATIC_STRING(RightTubules);
        DEFINE_STATIC_STRING(RightNetTubules);
        DEFINE_STATIC_STRING(RightUreter);

        // Blood
        DEFINE_STATIC_STRING(LeftAortaConnection);
        DEFINE_STATIC_STRING(LeftRenalArtery);
        DEFINE_STATIC_STRING(LeftAfferentArteriole);
        DEFINE_STATIC_STRING(LeftGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftNetGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftEfferentArteriole);
        DEFINE_STATIC_STRING(LeftPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftRenalVein);
        DEFINE_STATIC_STRING(LeftVenaCavaConnection);
        // Urine
        DEFINE_STATIC_STRING(LeftNetBowmansCapsules);
        DEFINE_STATIC_STRING(LeftBowmansCapsules);
        DEFINE_STATIC_STRING(LeftTubules);
        DEFINE_STATIC_STRING(LeftNetTubules);
        DEFINE_STATIC_STRING(LeftUreter);

        DEFINE_STATIC_STRING(Bladder);

        DEFINE_STATIC_STRING(Ground);
      };

      class RenalPath {
      public:
        DEFINE_STATIC_STRING(RightAortaConnectionToRenalArtery);
        DEFINE_STATIC_STRING(RightRenalArteryToAfferentArteriole);
        DEFINE_STATIC_STRING(RightRenalArteryCompliance);
        DEFINE_STATIC_STRING(RightAfferentArterioleToGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightGlomerularCapillariesToEfferentArteriole);
        DEFINE_STATIC_STRING(RightGlomerularCapillariesCompliance);
        DEFINE_STATIC_STRING(RightEfferentArterioleToPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightPeritubularCapillariesToRenalVein);
        DEFINE_STATIC_STRING(RightRenalVeinToVenaCavaConnection);
        DEFINE_STATIC_STRING(RightRenalVeinCompliance);

        DEFINE_STATIC_STRING(RightGlomerularCapillariesToNetGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightNetGlomerularCapillariesToNetBowmansCapsules);
        DEFINE_STATIC_STRING(RightBowmansCapsulesToNetBowmansCapsules);
        DEFINE_STATIC_STRING(RightBowmansCapsulesToTubules);
        DEFINE_STATIC_STRING(RightTubulesToUreter);
        DEFINE_STATIC_STRING(RightTubulesToNetTubules);
        DEFINE_STATIC_STRING(RightNetTubulesToNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightPeritubularCapillariesToNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightUreterToBladder);

        DEFINE_STATIC_STRING(LeftAortaConnectionToRenalArtery);
        DEFINE_STATIC_STRING(LeftRenalArteryToAfferentArteriole);
        DEFINE_STATIC_STRING(LeftRenalArteryCompliance);
        DEFINE_STATIC_STRING(LeftAfferentArterioleToGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftGlomerularCapillariesToEfferentArteriole);
        DEFINE_STATIC_STRING(LeftGlomerularCapillariesCompliance);
        DEFINE_STATIC_STRING(LeftEfferentArterioleToPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftPeritubularCapillariesToRenalVein);
        DEFINE_STATIC_STRING(LeftRenalVeinToVenaCavaConnection);
        DEFINE_STATIC_STRING(LeftRenalVeinCompliance);

        DEFINE_STATIC_STRING(LeftGlomerularCapillariesToNetGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftNetGlomerularCapillariesToNetBowmansCapsules);
        DEFINE_STATIC_STRING(LeftBowmansCapsulesToNetBowmansCapsules);
        DEFINE_STATIC_STRING(LeftBowmansCapsulesToTubules);
        DEFINE_STATIC_STRING(LeftTubulesToUreter);
        DEFINE_STATIC_STRING(LeftTubulesToNetTubules);
        DEFINE_STATIC_STRING(LeftNetTubulesToNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftPeritubularCapillariesToNetPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftUreterToBladder);

        DEFINE_STATIC_STRING(BladderToGroundPressure);
        DEFINE_STATIC_STRING(BladderToGroundUrinate);
      };

      class DigestionNode {
      public:
        DEFINE_STATIC_STRING(GutChyme);
      };

      class DigestionPath {
      public:
        DEFINE_STATIC_STRING(GutChymeToSmallIntestineVascular);
      };
    }
  }
}
}
