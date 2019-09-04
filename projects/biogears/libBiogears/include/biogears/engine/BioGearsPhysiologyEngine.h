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
#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>
// CDM Features in use
#include <biogears/cdm/engine/PhysiologyEngine.h>
#include <biogears/cdm/utils/FileUtils.h>
 
#include <memory>

namespace biogears {
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(biogears::Logger* logger = nullptr);
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const std::string logfile = "");
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const char* logfile = "");
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const std::string working_dir, const std::string logfile);
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const char* working_dir, const char* logfile);
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const std::string working_dir, biogears::Logger* logger);
BIOGEARS_API std::unique_ptr<biogears::PhysiologyEngine> CreateBioGearsEngine(const char* working_dir, biogears::Logger* logger);
}

namespace mil {
namespace tatrc {
  namespace physiology {
    namespace biogears {

      DEFINE_STATIC_STRING_EX(Version, BioGears_6 .1.1_beta);

      namespace Graph {

        DEFINE_STATIC_STRING(ActiveCardiovascular);
        DEFINE_STATIC_STRING(Cardiovascular);
        DEFINE_STATIC_STRING(Cerebral);
        DEFINE_STATIC_STRING(Renal);
        DEFINE_STATIC_STRING(Respiratory);
        DEFINE_STATIC_STRING(RespiratoryAndAnesthesiaMachine);
        DEFINE_STATIC_STRING(RespiratoryAndInhaler);
        DEFINE_STATIC_STRING(RespiratoryAndMechanicalVentilator);
        DEFINE_STATIC_STRING(Aerosol);
        DEFINE_STATIC_STRING(AerosolAndInhaler);
        DEFINE_STATIC_STRING(AnesthesiaMachine);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            ActiveCardiovascular, Cardiovascular, Renal, Respiratory, RespiratoryAndAnesthesiaMachine, RespiratoryAndInhaler, RespiratoryAndMechanicalVentilator, Aerosol, AerosolAndInhaler, AnesthesiaMachine
          };
          return _values;
        }
      };

      namespace ChymeCompartment {

        DEFINE_STATIC_STRING_EX(SmallIntestine, SmallIntestineChyme);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            SmallIntestine
          };
          return _values;
        }
      };

      namespace ChymeLink {

        DEFINE_STATIC_STRING(SmallIntestineChymeToVasculature);
        static const std::vector<std::string>& GetValues()
        {

          static std::vector<std::string> _values = {
            SmallIntestineChymeToVasculature
          };
          return _values;
        }
      };

      namespace PulmonaryCompartment {
        DEFINE_STATIC_STRING(Mouth);
        DEFINE_STATIC_STRING(Stomach);
        DEFINE_STATIC_STRING(Trachea);
        DEFINE_STATIC_STRING_EX(Lungs, PulmonaryLungs);
        DEFINE_STATIC_STRING_EX(LeftLung, LeftLungPulmonary);
        DEFINE_STATIC_STRING(LeftBronchi);
        DEFINE_STATIC_STRING(LeftAlveoli);
        DEFINE_STATIC_STRING_EX(RightLung, RightLungPulmonary);
        DEFINE_STATIC_STRING(RightBronchi);
        DEFINE_STATIC_STRING(RightAlveoli);
        DEFINE_STATIC_STRING(PleuralCavity);
        DEFINE_STATIC_STRING(LeftPleuralCavity);
        DEFINE_STATIC_STRING(RightPleuralCavity);

        DEFINE_STATIC_STRING(LeftAlveoliLeak);
        DEFINE_STATIC_STRING(LeftChestLeak);
        DEFINE_STATIC_STRING(RightAlveoliLeak);
        DEFINE_STATIC_STRING(RightChestLeak);

        static const std::vector<std::string>& GetValues()
        {

          static std::vector<std::string> _values = {
            Mouth, Stomach, Trachea, Lungs, LeftLung, LeftBronchi, LeftAlveoli, RightLung, RightBronchi, RightAlveoli, PleuralCavity, LeftPleuralCavity, RightPleuralCavity, LeftAlveoliLeak, LeftChestLeak, RightAlveoliLeak, RightChestLeak
          };
          return _values;
        }
      };

      namespace PulmonaryLink {

        DEFINE_STATIC_STRING(EnvironmentToMouth);
        DEFINE_STATIC_STRING(MouthToTrachea);
        DEFINE_STATIC_STRING(MouthToStomach);
        DEFINE_STATIC_STRING(TracheaToLeftBronchi);
        DEFINE_STATIC_STRING(LeftBronchiToAlveoli);
        DEFINE_STATIC_STRING(TracheaToRightBronchi);
        DEFINE_STATIC_STRING(RightBronchiToAlveoli);
        DEFINE_STATIC_STRING(EnvironmentToLeftChestLeak);
        DEFINE_STATIC_STRING(EnvironmentToRightChestLeak);
        DEFINE_STATIC_STRING(LeftAlveoliLeakToLeftPleuralCavity);
        DEFINE_STATIC_STRING(LeftAlveoliToLeftAlveoliLeak);
        DEFINE_STATIC_STRING(LeftChestLeakToLeftPleuralCavity);
        DEFINE_STATIC_STRING(LeftPleuralCavityToEnvironment);
        DEFINE_STATIC_STRING(RightAlveoliLeakToRightPleuralCavity);
        DEFINE_STATIC_STRING(RightAlveoliToRightAlveoliLeak);
        DEFINE_STATIC_STRING(RightChestLeakToRightPleuralCavity);
        DEFINE_STATIC_STRING(RightPleuralCavityToEnvironment);

        static const std::vector<std::string>& GetValues()
        {

          static std::vector<std::string> _values = {
            EnvironmentToMouth, MouthToTrachea, MouthToStomach, TracheaToLeftBronchi, LeftBronchiToAlveoli, TracheaToRightBronchi, RightBronchiToAlveoli, EnvironmentToLeftChestLeak, EnvironmentToRightChestLeak, LeftAlveoliLeakToLeftPleuralCavity, LeftAlveoliToLeftAlveoliLeak, LeftChestLeakToLeftPleuralCavity, LeftPleuralCavityToEnvironment, RightAlveoliLeakToRightPleuralCavity, RightAlveoliToRightAlveoliLeak, RightChestLeakToRightPleuralCavity, RightPleuralCavityToEnvironment
          };
          return _values;
        }
      };

      namespace TissueCompartment {

        DEFINE_STATIC_STRING_EX(Bone, BoneTissue);
        DEFINE_STATIC_STRING_EX(Brain, BrainTissue);
        DEFINE_STATIC_STRING_EX(Fat, FatTissue);
        DEFINE_STATIC_STRING_EX(Gut, GutTissue);
        DEFINE_STATIC_STRING_EX(LeftKidney, LeftKidneyTissue);
        DEFINE_STATIC_STRING_EX(LeftLung, LeftLungTissue);
        DEFINE_STATIC_STRING_EX(Liver, LiverTissue);
        DEFINE_STATIC_STRING_EX(Muscle, MuscleTissue);
        DEFINE_STATIC_STRING_EX(Myocardium, MyocardiumTissue);
        DEFINE_STATIC_STRING_EX(RightKidney, RightKidneyTissue);
        DEFINE_STATIC_STRING_EX(RightLung, RightLungTissue);
        DEFINE_STATIC_STRING_EX(Skin, SkinTissue);
        DEFINE_STATIC_STRING_EX(Spleen, SpleenTissue);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Bone, Brain, Fat, Gut, LeftKidney, LeftLung, Liver, Muscle, Myocardium, RightKidney, RightLung, Skin, Spleen
          };
          return _values;
        }
      };

      namespace ExtravascularCompartment {

        DEFINE_STATIC_STRING_EX(BoneExtracellular, BoneTissueExtracellular);
        DEFINE_STATIC_STRING_EX(BrainExtracellular, BrainTissueExtracellular);
        DEFINE_STATIC_STRING(CerebralSpinalFluid);
        DEFINE_STATIC_STRING_EX(FatExtracellular, FatTissueExtracellular);
        DEFINE_STATIC_STRING_EX(GutExtracellular, GutTissueExtracellular);
        DEFINE_STATIC_STRING_EX(LeftKidneyExtracellular, LeftKidneyTissueExtracellular);
        DEFINE_STATIC_STRING_EX(LeftLungExtracellular, LeftLungTissueExtracellular);
        DEFINE_STATIC_STRING_EX(LiverExtracellular, LiverTissueExtracellular);
        DEFINE_STATIC_STRING_EX(MuscleExtracellular, MuscleTissueExtracellular);
        DEFINE_STATIC_STRING_EX(MyocardiumExtracellular, MyocardiumTissueExtracellular);
        DEFINE_STATIC_STRING_EX(RightKidneyExtracellular, RightKidneyTissueExtracellular);
        DEFINE_STATIC_STRING_EX(RightLungExtracellular, RightLungTissueExtracellular);
        DEFINE_STATIC_STRING_EX(SkinExtracellular, SkinTissueExtracellular);
        DEFINE_STATIC_STRING_EX(SpleenExtracellular, SpleenTissueExtracellular);

        DEFINE_STATIC_STRING_EX(BoneIntracellular, BoneTissueIntracellular);
        DEFINE_STATIC_STRING_EX(BrainIntracellular, BrainTissueIntracellular);
        DEFINE_STATIC_STRING_EX(FatIntracellular, FatTissueIntracellular);
        DEFINE_STATIC_STRING_EX(GutIntracellular, GutTissueIntracellular);
        DEFINE_STATIC_STRING_EX(LeftKidneyIntracellular, LeftKidneyTissueIntracellular);
        DEFINE_STATIC_STRING_EX(LeftLungIntracellular, LeftLungTissueIntracellular);
        DEFINE_STATIC_STRING_EX(LiverIntracellular, LiverTissueIntracellular);
        DEFINE_STATIC_STRING_EX(MuscleIntracellular, MuscleTissueIntracellular);
        DEFINE_STATIC_STRING_EX(MyocardiumIntracellular, MyocardiumTissueIntracellular);
        DEFINE_STATIC_STRING_EX(RightKidneyIntracellular, RightKidneyTissueIntracellular);
        DEFINE_STATIC_STRING_EX(RightLungIntracellular, RightLungTissueIntracellular);
        DEFINE_STATIC_STRING_EX(SkinIntracellular, SkinTissueIntracellular);
        DEFINE_STATIC_STRING_EX(SpleenIntracellular, SpleenTissueIntracellular);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            BoneExtracellular, BrainExtracellular, CerebralSpinalFluid, FatExtracellular, GutExtracellular, LeftKidneyExtracellular, LeftLungExtracellular, LiverExtracellular, MuscleExtracellular, MyocardiumExtracellular, RightKidneyExtracellular, RightLungExtracellular, SkinExtracellular, SpleenExtracellular, BoneIntracellular, BrainIntracellular, FatIntracellular, GutIntracellular, LeftKidneyIntracellular, LeftLungIntracellular, LiverIntracellular, MuscleIntracellular, MyocardiumIntracellular, RightKidneyIntracellular, RightLungIntracellular, SkinIntracellular, SpleenIntracellular
          };
          return _values;
        }
      };

      namespace VascularCompartment {

        // Cardio
        DEFINE_STATIC_STRING(Aorta);
        DEFINE_STATIC_STRING(Heart);
        DEFINE_STATIC_STRING_EX(Myocardium, MyocardiumVasculature);
        DEFINE_STATIC_STRING(LeftHeart);
        DEFINE_STATIC_STRING(LeftAtrium);
        DEFINE_STATIC_STRING(LeftVentricle);
        DEFINE_STATIC_STRING(RightHeart);
        DEFINE_STATIC_STRING(RightAtrium);
        DEFINE_STATIC_STRING(RightVentricle);
        DEFINE_STATIC_STRING(Pericardium);
        DEFINE_STATIC_STRING(VenaCava);
        //Cerebral
        DEFINE_STATIC_STRING(NeckArteries);
        DEFINE_STATIC_STRING(CerebralArteries);
        DEFINE_STATIC_STRING(CerebralCapillaries);
        DEFINE_STATIC_STRING(CerebralVeins);
        DEFINE_STATIC_STRING(NeckVeins);

        // Pulmonary
        DEFINE_STATIC_STRING(PulmonaryArteries);
        DEFINE_STATIC_STRING(PulmonaryCapillaries);
        DEFINE_STATIC_STRING(PulmonaryVeins);
        DEFINE_STATIC_STRING_EX(Lungs, LungsVasculature);
        DEFINE_STATIC_STRING_EX(LeftLung, LeftLungVasculature);
        DEFINE_STATIC_STRING(LeftPulmonaryArteries);
        DEFINE_STATIC_STRING(LeftPulmonaryCapillaries);
        DEFINE_STATIC_STRING(LeftPulmonaryVeins);
        DEFINE_STATIC_STRING_EX(RightLung, RightLungVasculature);
        DEFINE_STATIC_STRING(RightPulmonaryArteries);
        DEFINE_STATIC_STRING(RightPulmonaryCapillaries);
        DEFINE_STATIC_STRING(RightPulmonaryVeins);
        // Renal
        DEFINE_STATIC_STRING_EX(Kidneys, KidneyVasculature);
        DEFINE_STATIC_STRING_EX(LeftKidney, LeftKidneyVasculature);
        DEFINE_STATIC_STRING(LeftRenalArtery);
        DEFINE_STATIC_STRING(LeftNephron);
        DEFINE_STATIC_STRING(LeftAfferentArteriole);
        DEFINE_STATIC_STRING(LeftGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftEfferentArteriole);
        DEFINE_STATIC_STRING(LeftPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftBowmansCapsules);
        DEFINE_STATIC_STRING(LeftTubules);
        DEFINE_STATIC_STRING(LeftRenalVein);
        DEFINE_STATIC_STRING_EX(RightKidney, RightKidneyVasculature);
        DEFINE_STATIC_STRING(RightRenalArtery);
        DEFINE_STATIC_STRING(RightNephron);
        DEFINE_STATIC_STRING(RightAfferentArteriole);
        DEFINE_STATIC_STRING(RightGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightEfferentArteriole);
        DEFINE_STATIC_STRING(RightPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightBowmansCapsules);
        DEFINE_STATIC_STRING(RightTubules);
        DEFINE_STATIC_STRING(RightRenalVein);
        // General Organs and Periphery
        DEFINE_STATIC_STRING_EX(Bone, BoneVasculature);
        DEFINE_STATIC_STRING_EX(Brain, BrainVasculature);
        DEFINE_STATIC_STRING_EX(Fat, FatVasculature);
        DEFINE_STATIC_STRING_EX(Gut, GutVasculature);
        DEFINE_STATIC_STRING_EX(Splanchnic, SplanchnicVasculature);
        DEFINE_STATIC_STRING_EX(SmallIntestine, SmallIntestineVasculature);
        DEFINE_STATIC_STRING_EX(LargeIntestine, LargeIntestineVasculature);
        DEFINE_STATIC_STRING_EX(Liver, LiverVasculature);
        DEFINE_STATIC_STRING_EX(Spleen, SpleenVasculature);
        DEFINE_STATIC_STRING_EX(Skin, SkinVasculature);
        DEFINE_STATIC_STRING_EX(Muscle, MuscleVasculature);
        DEFINE_STATIC_STRING_EX(LeftArm, LeftArmVasculature);
        DEFINE_STATIC_STRING_EX(LeftLeg, LeftLegVasculature);
        DEFINE_STATIC_STRING_EX(RightArm, RightArmVasculature);
        DEFINE_STATIC_STRING_EX(RightLeg, RightLegVasculature);

        DEFINE_STATIC_STRING(Ground);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Aorta, Heart, Myocardium, LeftHeart, LeftAtrium, LeftVentricle, RightHeart, RightAtrium, RightVentricle, Pericardium, VenaCava, CerebralArteries, CerebralCapillaries, CerebralVeins, NeckArteries, NeckVeins, PulmonaryArteries, PulmonaryCapillaries, PulmonaryVeins, Lungs, LeftLung, LeftPulmonaryArteries, LeftPulmonaryCapillaries, LeftPulmonaryVeins, RightLung, RightPulmonaryArteries, RightPulmonaryCapillaries, RightPulmonaryVeins, Kidneys, LeftKidney, LeftRenalArtery, LeftNephron, LeftAfferentArteriole, LeftGlomerularCapillaries, LeftEfferentArteriole, LeftPeritubularCapillaries, LeftBowmansCapsules, LeftTubules, LeftRenalVein, RightKidney, RightRenalArtery, RightNephron, RightAfferentArteriole, RightGlomerularCapillaries, RightEfferentArteriole, RightPeritubularCapillaries, RightBowmansCapsules, RightTubules, RightRenalVein, Bone, Brain, Fat, Gut, Splanchnic, SmallIntestine, LargeIntestine, Liver, Spleen, Skin, Muscle, LeftArm, LeftLeg, RightArm, RightLeg
            //,Ground
          };
          return _values;
        }
      };

      namespace VascularLink {

        // Heart and Lungs
        DEFINE_STATIC_STRING(VenaCavaToRightAtrium);
        DEFINE_STATIC_STRING(RightAtriumToRightVentricle);
        DEFINE_STATIC_STRING(RightVentricleToLeftPulmonaryArteries);
        DEFINE_STATIC_STRING(LeftPulmonaryArteriesToCapillaries);
        DEFINE_STATIC_STRING(LeftPulmonaryArteriesToVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryCapillariesToVeins);
        DEFINE_STATIC_STRING(LeftPulmonaryVeinsToLeftAtrium);
        DEFINE_STATIC_STRING(RightVentricleToRightPulmonaryArteries);
        DEFINE_STATIC_STRING(RightPulmonaryArteriesToCapillaries);
        DEFINE_STATIC_STRING(RightPulmonaryArteriesToVeins);
        DEFINE_STATIC_STRING(RightPulmonaryCapillariesToVeins);
        DEFINE_STATIC_STRING(RightPulmonaryVeinsToLeftAtrium);
        DEFINE_STATIC_STRING(LeftAtriumToLeftVentricle);
        DEFINE_STATIC_STRING(LeftVentricleToAorta);
        //Cerebral
        DEFINE_STATIC_STRING(AortaToNeckArteries);
        DEFINE_STATIC_STRING(NeckArteriesToCerebralArteries);
        DEFINE_STATIC_STRING(CerebralArteriesToCerebralCapillaries);
        DEFINE_STATIC_STRING(CerebralCapillariesToCerebralVeins);
        DEFINE_STATIC_STRING(CerebralVeinsToNeckVeins);
        DEFINE_STATIC_STRING(NeckVeinsToVenaCava);
        // Bone
        DEFINE_STATIC_STRING(AortaToBone);
        DEFINE_STATIC_STRING(BoneToVenaCava);
        // Brain
        DEFINE_STATIC_STRING(AortaToBrain);
        DEFINE_STATIC_STRING(BrainToVenaCava);
        // Fat
        DEFINE_STATIC_STRING(AortaToFat);
        DEFINE_STATIC_STRING(FatToVenaCava);
        // Large Intestine
        DEFINE_STATIC_STRING(AortaToLargeIntestine);
        DEFINE_STATIC_STRING(LargeIntestineToLiver);
        // Left Arm
        DEFINE_STATIC_STRING(AortaToLeftArm);
        DEFINE_STATIC_STRING(LeftArmToVenaCava);
        // Left Kidney
        DEFINE_STATIC_STRING(AortaToLeftKidney);
        DEFINE_STATIC_STRING(LeftKidneyToVenaCava);
        // Left Leg
        DEFINE_STATIC_STRING(AortaToLeftLeg);
        DEFINE_STATIC_STRING(LeftLegToVenaCava);
        // Liver
        DEFINE_STATIC_STRING(AortaToLiver);
        DEFINE_STATIC_STRING(LiverToVenaCava);
        // Muscle
        DEFINE_STATIC_STRING(AortaToMuscle);
        DEFINE_STATIC_STRING(MuscleToVenaCava);
        // Myocardium
        DEFINE_STATIC_STRING(AortaToMyocardium);
        DEFINE_STATIC_STRING(MyocardiumToVenaCava);
        // Right Arm
        DEFINE_STATIC_STRING(AortaToRightArm);
        DEFINE_STATIC_STRING(RightArmToVenaCava);
        // Right Kidney
        DEFINE_STATIC_STRING(AortaToRightKidney);
        DEFINE_STATIC_STRING(RightKidneyToVenaCava);
        // Right Leg
        DEFINE_STATIC_STRING(AortaToRightLeg);
        DEFINE_STATIC_STRING(RightLegToVenaCava);
        // Skin
        DEFINE_STATIC_STRING(AortaToSkin);
        DEFINE_STATIC_STRING(SkinToVenaCava);
        // Small Intestine
        DEFINE_STATIC_STRING(AortaToSmallIntestine);
        DEFINE_STATIC_STRING(SmallIntestineToLiver);
        // Splanchnic
        DEFINE_STATIC_STRING(AortaToSplanchnic);
        DEFINE_STATIC_STRING(SplanchnicToLiver);
        // Spleen
        DEFINE_STATIC_STRING(AortaToSpleen);
        DEFINE_STATIC_STRING(SpleenToLiver);

        // Hemorrhage and IV
        DEFINE_STATIC_STRING(VenaCavaIV);
        DEFINE_STATIC_STRING(VenaCavaHemorrhage);
        DEFINE_STATIC_STRING(AortaHemorrhage);
        DEFINE_STATIC_STRING(BrainHemorrhage);
        DEFINE_STATIC_STRING(HeartHemorrhage);
        DEFINE_STATIC_STRING(LeftLungHemorrhage);
        DEFINE_STATIC_STRING(RightLungHemorrhage);
        DEFINE_STATIC_STRING(LeftArmHemorrhage);
        DEFINE_STATIC_STRING(RightArmHemorrhage);
        DEFINE_STATIC_STRING(SpleenHemorrhage);
        DEFINE_STATIC_STRING(SmallIntestineHemorrhage);
        DEFINE_STATIC_STRING(LargeIntestineHemorrhage);
        DEFINE_STATIC_STRING(SplanchnicHemorrhage);
        DEFINE_STATIC_STRING(LeftKidneyHemorrhage);
        DEFINE_STATIC_STRING(RightKidneyHemorrhage);
        DEFINE_STATIC_STRING(LiverHemorrhage);
        DEFINE_STATIC_STRING(LeftLegHemorrhage);
        DEFINE_STATIC_STRING(RightLegHemorrhage);

        // Vascular To Tissue Links
        DEFINE_STATIC_STRING(BoneVascularToTissue);
        DEFINE_STATIC_STRING(BrainVascularToTissue);
        DEFINE_STATIC_STRING(FatVascularToTissue);
        DEFINE_STATIC_STRING(SmallIntestineVascularToTissue);
        DEFINE_STATIC_STRING(LargeIntestineVascularToTissue);
        DEFINE_STATIC_STRING(SplanchnicVascularToTissue);
        DEFINE_STATIC_STRING(LeftKidneyVascularToTissue);
        DEFINE_STATIC_STRING(LeftLungVascularToTissue);
        DEFINE_STATIC_STRING(LiverVascularToTissue);
        DEFINE_STATIC_STRING(MuscleVascularToTissue);
        DEFINE_STATIC_STRING(MyocardiumVascularToTissue);
        DEFINE_STATIC_STRING(RightKidneyVascularToTissue);
        DEFINE_STATIC_STRING(RightLungVascularToTissue);
        DEFINE_STATIC_STRING(SkinVascularToTissue);
        DEFINE_STATIC_STRING(SpleenVascularToTissue);

        // Renal Links
        // Reusing 'AortaToLeftKidney' to connect the Aorta To Left Renal Artery
        DEFINE_STATIC_STRING(LeftRenalArteryToAfferentArteriole);
        DEFINE_STATIC_STRING(LeftAfferentArterioleToGlomerularCapillaries);
        DEFINE_STATIC_STRING(LeftGlomerularCapillariesToEfferentArteriole);
        DEFINE_STATIC_STRING(LeftGlomerularCapillariesToBowmansCapsules);
        DEFINE_STATIC_STRING(LeftBowmansCapsulesToTubules);
        DEFINE_STATIC_STRING(LeftTubulesToPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftEfferentArterioleToPeritubularCapillaries);
        DEFINE_STATIC_STRING(LeftPeritubularCapillariesToRenalVein);
        // Reusing 'LeftKidneyToVenaCava' to connect the Left Renal Vein To Vena Cava

        // Reusing 'AortaToRightKidney' to connect the Aorta To Right Renal Artery
        DEFINE_STATIC_STRING(RightRenalArteryToAfferentArteriole);
        DEFINE_STATIC_STRING(RightAfferentArterioleToGlomerularCapillaries);
        DEFINE_STATIC_STRING(RightGlomerularCapillariesToEfferentArteriole);
        DEFINE_STATIC_STRING(RightGlomerularCapillariesToBowmansCapsules);
        DEFINE_STATIC_STRING(RightBowmansCapsulesToTubules);
        DEFINE_STATIC_STRING(RightTubulesToPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightEfferentArterioleToPeritubularCapillaries);
        DEFINE_STATIC_STRING(RightPeritubularCapillariesToRenalVein);
        // Reusing 'RightKidneyToVenaCava' to connect the Right Renal Vein To Vena Cava);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            VenaCavaToRightAtrium, RightAtriumToRightVentricle, RightVentricleToLeftPulmonaryArteries, LeftPulmonaryArteriesToCapillaries, LeftPulmonaryArteriesToVeins, LeftPulmonaryCapillariesToVeins, LeftPulmonaryVeinsToLeftAtrium, RightVentricleToRightPulmonaryArteries, RightPulmonaryArteriesToCapillaries, RightPulmonaryArteriesToVeins, RightPulmonaryCapillariesToVeins, RightPulmonaryVeinsToLeftAtrium, LeftAtriumToLeftVentricle, LeftVentricleToAorta, AortaToBone, BoneToVenaCava, AortaToBrain, BrainToVenaCava, AortaToFat, FatToVenaCava, AortaToLargeIntestine, LargeIntestineToLiver, AortaToLeftArm, LeftArmToVenaCava, AortaToLeftKidney, LeftKidneyToVenaCava, AortaToLeftLeg, LeftLegToVenaCava, AortaToLiver, LiverToVenaCava, AortaToMuscle, MuscleToVenaCava, AortaToMyocardium, MyocardiumToVenaCava, AortaToRightArm, RightArmToVenaCava, AortaToRightKidney, RightKidneyToVenaCava, AortaToRightLeg, RightLegToVenaCava, AortaToSkin, SkinToVenaCava, AortaToSmallIntestine, SmallIntestineToLiver, AortaToSplanchnic, SplanchnicToLiver, AortaToSpleen, SpleenToLiver

            ,
            AortaToNeckArteries, NeckArteriesToCerebralArteries, CerebralArteriesToCerebralCapillaries, CerebralCapillariesToCerebralVeins, CerebralVeinsToNeckVeins, NeckVeinsToVenaCava

            ,
            BoneVascularToTissue, BrainVascularToTissue, FatVascularToTissue, SmallIntestineVascularToTissue, LargeIntestineVascularToTissue, SplanchnicVascularToTissue, LeftKidneyVascularToTissue, LeftLungVascularToTissue, LiverVascularToTissue, MuscleVascularToTissue, MyocardiumVascularToTissue, RightKidneyVascularToTissue, RightLungVascularToTissue, SkinVascularToTissue, SpleenVascularToTissue

            ,
            LeftRenalArteryToAfferentArteriole, LeftAfferentArterioleToGlomerularCapillaries, LeftGlomerularCapillariesToEfferentArteriole, LeftGlomerularCapillariesToBowmansCapsules, LeftBowmansCapsulesToTubules, LeftTubulesToPeritubularCapillaries, LeftEfferentArterioleToPeritubularCapillaries, LeftPeritubularCapillariesToRenalVein, RightRenalArteryToAfferentArteriole, RightAfferentArterioleToGlomerularCapillaries, RightGlomerularCapillariesToEfferentArteriole, RightGlomerularCapillariesToBowmansCapsules, RightBowmansCapsulesToTubules, RightTubulesToPeritubularCapillaries, RightEfferentArterioleToPeritubularCapillaries, RightPeritubularCapillariesToRenalVein
          };
          return _values;
        }
      };

      namespace UrineCompartment {

        DEFINE_STATIC_STRING(Ureters);
        DEFINE_STATIC_STRING(LeftUreter);
        DEFINE_STATIC_STRING(RightUreter);
        DEFINE_STATIC_STRING(Bladder);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Ureters, LeftUreter, RightUreter, Bladder
          };
          return _values;
        }
      };

      namespace UrineLink {

        DEFINE_STATIC_STRING(LeftTubulesToUreter);
        DEFINE_STATIC_STRING(LeftUreterToBladder);

        DEFINE_STATIC_STRING(RightTubulesToUreter);
        DEFINE_STATIC_STRING(RightUreterToBladder);

        DEFINE_STATIC_STRING(BladderToGround);
        DEFINE_STATIC_STRING(BladderToGroundSource);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            LeftTubulesToUreter, LeftUreterToBladder, RightTubulesToUreter, RightUreterToBladder, BladderToGround, BladderToGroundSource
          };
          return _values;
        }
      };

      namespace LymphCompartment {

        DEFINE_STATIC_STRING(Lymph);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Lymph
          };
          return _values;
        }
      };

      namespace LymphLink {

        DEFINE_STATIC_STRING(BoneTissueToLymph);
        DEFINE_STATIC_STRING(BrainTissueToLymph);
        DEFINE_STATIC_STRING(FatTissueToLymph);
        DEFINE_STATIC_STRING(GutTissueToLymph);
        DEFINE_STATIC_STRING(LeftKidneyTissueToLymph);
        DEFINE_STATIC_STRING(LeftLungTissueToLymph);
        DEFINE_STATIC_STRING(LiverTissueToLymph);
        DEFINE_STATIC_STRING(MuscleTissueToLymph);
        DEFINE_STATIC_STRING(MyocardiumTissueToLymph);
        DEFINE_STATIC_STRING(RightKidneyTissueToLymph);
        DEFINE_STATIC_STRING(RightLungTissueToLymph);
        DEFINE_STATIC_STRING(SkinTissueToLymph);
        DEFINE_STATIC_STRING(SpleenTissueToLymph);

        DEFINE_STATIC_STRING(LymphToVenaCava);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            BoneTissueToLymph, BrainTissueToLymph, FatTissueToLymph, GutTissueToLymph, LeftKidneyTissueToLymph, LeftLungTissueToLymph, LiverTissueToLymph, MuscleTissueToLymph, MyocardiumTissueToLymph, RightKidneyTissueToLymph, RightLungTissueToLymph, SkinTissueToLymph, SpleenTissueToLymph, LymphToVenaCava
          };
          return _values;
        }
      };

      namespace TemperatureCompartment {

        DEFINE_STATIC_STRING(Active);
        DEFINE_STATIC_STRING(Ambient);
        DEFINE_STATIC_STRING(Clothing);
        DEFINE_STATIC_STRING(Enclosure);
        DEFINE_STATIC_STRING(ExternalCore);
        DEFINE_STATIC_STRING(ExternalSkin);
        DEFINE_STATIC_STRING(ExternalGround);
        DEFINE_STATIC_STRING(InternalCore);
        DEFINE_STATIC_STRING(InternalSkin);
        DEFINE_STATIC_STRING(InternalGround);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Active, Ambient, Clothing, Enclosure, ExternalCore, ExternalSkin, ExternalGround, InternalCore, InternalSkin, InternalGround
          };
          return _values;
        }
      };

      namespace TemperatureLink {

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
        DEFINE_STATIC_STRING(GroundToInternalCore);
        DEFINE_STATIC_STRING(InternalCoreToInternalSkin);
        DEFINE_STATIC_STRING(InternalCoreToGround);
        DEFINE_STATIC_STRING(InternalSkinToGround);
        DEFINE_STATIC_STRING(InternalCoreToExternalCore);
        DEFINE_STATIC_STRING(InternalSkinToExternalSkin);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            ActiveToClothing, ClothingToEnclosure, ClothingToEnvironment, ExternalCoreToGround, GroundToActive, GroundToClothing, GroundToEnclosure, GroundToEnvironment, ExternalSkinToGround, ExternalSkinToClothing, GroundToInternalCore, InternalCoreToInternalSkin, InternalCoreToGround, InternalSkinToGround, InternalCoreToExternalCore, InternalSkinToExternalSkin
          };
          return _values;
        }
      };

      namespace EnvironmentCompartment {

        DEFINE_STATIC_STRING(Ambient);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Ambient
          };
          return _values;
        }
      };

      namespace AnesthesiaMachineCompartment {

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

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            AnesthesiaConnection, ExpiratoryLimb, GasInlet, GasSource, InspiratoryLimb, ReliefValve, Scrubber, Selector, Ventilator, VentilatorConnection, YPiece
          };
          return _values;
        }
      };

      namespace AnesthesiaMachineLink {

        DEFINE_STATIC_STRING(VentilatorToSelector);
        DEFINE_STATIC_STRING(SelectorToReliefValve);
        DEFINE_STATIC_STRING(SelectorToScrubber);
        DEFINE_STATIC_STRING(ScrubberToGasInlet);
        DEFINE_STATIC_STRING(Exhaust);
        DEFINE_STATIC_STRING(GasSourceToGasInlet);
        DEFINE_STATIC_STRING(GasInletToInspiratoryLimb);
        DEFINE_STATIC_STRING(InspiratoryLimbToYPiece);
        DEFINE_STATIC_STRING(YPieceToExpiratoryLimb);
        DEFINE_STATIC_STRING(ExpiratoryLimbToSelector);
        DEFINE_STATIC_STRING(YPieceToAnesthesiaConnection);
        DEFINE_STATIC_STRING(AnesthesiaConnectionLeak);
        DEFINE_STATIC_STRING(Mask);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            VentilatorToSelector, SelectorToReliefValve, SelectorToScrubber, ScrubberToGasInlet, Exhaust, GasSourceToGasInlet, GasInletToInspiratoryLimb, InspiratoryLimbToYPiece, YPieceToExpiratoryLimb, ExpiratoryLimbToSelector, YPieceToAnesthesiaConnection, AnesthesiaConnectionLeak, Mask
          };
          return _values;
        }
      };

      namespace InhalerCompartment {

        DEFINE_STATIC_STRING(Mouthpiece);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Mouthpiece
          };
          return _values;
        }
      };

      namespace InhalerLink {

        DEFINE_STATIC_STRING(EnvironmentToMouthpiece);
        DEFINE_STATIC_STRING(MouthpieceToMouth);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            EnvironmentToMouthpiece, MouthpieceToMouth
          };
          return _values;
        }
      };

      namespace MechanicalVentilatorCompartment {

        DEFINE_STATIC_STRING(Connection);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            Connection
          };
          return _values;
        }
      };

      namespace MechanicalVentilatorLink {

        DEFINE_STATIC_STRING(ConnectionToMouth);

        static const std::vector<std::string>& GetValues()
        {
          static std::vector<std::string> _values = {
            ConnectionToMouth
          };
          return _values;
        }
      };

    } //namespace biogears
  } //namespace physiology
} //namespace tatrc
} //namespace mil

namespace BGE = mil::tatrc::physiology::biogears;
