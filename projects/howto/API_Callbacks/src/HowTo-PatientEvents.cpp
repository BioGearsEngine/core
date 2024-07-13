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

#include <iostream>
// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstance.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Controller/BioGears.h>

#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineChamber.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineAction.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/utils/SEEventHandler.h>


//!
//!    BioGears has two methods of handling patient events. 
//! 
//!    A user can override the SEEventHandler interface and setup
//!    A class that will be called for biogears events. THis is a resonable approach
//!    If you need to react to a plethra of events as a single function can be coded
//!    with a switch handler for every biogears event. The downside  is as this example shows
//!    Some biogears events happen very often and this class will be called everytime any event triggers
//!    Not just the ones you code for
//! 
//!    virtual void HandlePatientEvent(SEPatientEventType::value type, bool active, const SEScalarTime* time = nullptr) = 0;
//!    virtual void HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const SEScalarTime* time = nullptr) = 0;
//! 
//!    The alternative method is to provide callback functions for a single event. This is a more narrow approach where if you choose the same handler
//!    Could be assigned to multiple events or seperate handlers for each event you wish to react to. As of biogears 7.0 this is the preferred and more 
//!    performant method of programatically reacting to biogears events.
//!    In this example we use both methods simultaniously to provide some additonal flare to the log
//!    In this HOwTo the anesthesia machine is used to kill a patient providing multi events to demonstrate against. Enjoy

using namespace biogears;

using namespace biogears;
// Make a custom event handler that you can connect to your code (See EngineUse for more info)
class CustomEventHandler : public SEEventHandler {
private:
  Logger* m_Logger;

public:
  CustomEventHandler()
    : SEEventHandler()
  {
  }
  void HandlePatientEvent(SEPatientEventType type, bool active, const SEScalarTime* time = nullptr) override
  {

    static int event_count = 0;
    if ( event_count++ % 31 != 0){
      return ;
    }

    std::cout << "\n";
    std::cout << "  OO O o o o...      _______________________________________ \n";
    std::cout << "  O     ____          |                                      |\n";

    std::string event;
    std::string marquee_1 = "                                      ";
    std::string marquee_2 = "                                      ";
    std::string prefix = " ][_n_i_| (   ooo___  |";
    std::string end = "|\n";

    switch (type) {
    case SEPatientEventType::AcuteLungInjury:
      event = "AcuteLungInjury";
      break;
    case SEPatientEventType::AcuteRespiratoryDistress:
      event = "AcuteRespiratoryDistress";
      break;
    case SEPatientEventType::Antidiuresis:
      event = "Antidiuresis";
      break;
    case SEPatientEventType::Asystole:
      event = "Asystoley";
      break;
    case SEPatientEventType::Bradycardia:
      event = "Bradycardia";
      break;
    case SEPatientEventType::Bradypnea:
      event = "Bradypnea";
      break;
    case SEPatientEventType::BrainOxygenDeficit:
      event = "BrainOxygenDeficit";
      break;
    case SEPatientEventType::CardiacArrest:
      event = "CardiacArrest";
      break;
    case SEPatientEventType::CardiogenicShock:
      event = "CardiogenicShock";
      break;
    case SEPatientEventType::CriticalBrainOxygenDeficit:
      event = "CriticalBrainOxygenDeficit";
      break;
    case SEPatientEventType::Dehydration:
      event = "Dehydration";
      break;
    case SEPatientEventType::Diuresis:
      event = "AcuteLungInjury";
      break;
    case SEPatientEventType::Fasciculation:
      event = "Fasciculation";
      break;
    case SEPatientEventType::Fatigue:
      event = "Fatigue";
      break;
    case SEPatientEventType::FunctionalIncontinence:
      event = "FunctionalIncontinence";
      break;
    case SEPatientEventType::HemolyticTransfusionReaction:
      event = "HemolyticTransfusionReaction";
      break;
    case SEPatientEventType::Hypercapnia:
      event = "Hypercapnia";
      break;
    case SEPatientEventType::Hyperglycemia:
      event = "Hyperglycemia";
      break;
    case SEPatientEventType::MildHyperkalemia:
      event = "MildHyperkalemia";
      break;
    case SEPatientEventType::SevereHyperkalemia:
      event = "SevereHyperkalemia";
      break;
    case SEPatientEventType::MildHypernatremia:
      event = "MildHypernatremia";
      break;
    case SEPatientEventType::SevereHypernatremia:
      event = "SevereHypernatremia";
      break;
    case SEPatientEventType::Hyperthermia:
      event = "Hyperthermia";
      break;
    case SEPatientEventType::Hypoglycemia:
      event = "Hypoglycemia";
      break;
    case SEPatientEventType::HypoglycemicShock:
      event = "HypoglycemicShock";
      break;
    case SEPatientEventType::HypoglycemicComa:
      event = "HypoglycemicComa";
      break;
    case SEPatientEventType::MildHypothermia:
      event = "MildHypothermia";
      break;
    case SEPatientEventType::MildHypokalemia:
      event = "MildHypokalemia";
      break;
    case SEPatientEventType::SevereHypokalemia:
      event = "SevereHypokalemia";
      break;
    case SEPatientEventType::MildHyponatremia:
      event = "MildHyponatremia";
      break;
    case SEPatientEventType::SevereHyponatremia:
      event = "SevereHyponatremia";
      break;
    case SEPatientEventType::Hypoxia:
      event = "Hypoxia";
      break;
    case SEPatientEventType::HypovolemicShock:
      event = "HypovolemicShock";
      break;
    case SEPatientEventType::IntracranialHypertension:
      event = "IntracranialHypertension";
      break;
    case SEPatientEventType::IntracranialHypotension:
      event = "IntracranialHypotension";
      break;
    case SEPatientEventType::IrreversibleState:
      event = "IrreversibleState";
      break;
    case SEPatientEventType::Ketoacidosis:
      event = "Ketoacidosis";
      break;
    case SEPatientEventType::LacticAcidosis:
      event = "LacticAcidosis";
      break;
    case SEPatientEventType::LiverGlycogenDepleted:
      event = "LiverGlycogenDepleted";
      break;
    case SEPatientEventType::MaximumPulmonaryVentilationRate:
      event = "MaximumPulmonaryVentilationRate";
      break;
    case SEPatientEventType::MetabolicAcidosis:
      event = "MetabolicAcidosis";
      break;
    case SEPatientEventType::MetabolicAlkalosis:
      event = "MetabolicAlkalosis";
      break;
    case SEPatientEventType::MuscleCatabolism:
      event = "MuscleCatabolism";
      break;
    case SEPatientEventType::MuscleGlycogenDepleted:
      event = "MuscleGlycogenDepleted";
      break;
    case SEPatientEventType::MyocardiumOxygenDeficit:
      event = "MyocardiumOxygenDeficit";
      break;
    case SEPatientEventType::Natriuresis:
      event = "Natriuresis";
      break;
    case SEPatientEventType::NutritionDepleted:
      event = "NutritionDepleted";
      break;
    case SEPatientEventType::PulselessRhythm:
      event = "PulselessRhythm";
      break;
    case SEPatientEventType::RenalHypoperfusion:
      event = "RenalHypoperfusion";
      break;
    case SEPatientEventType::RespiratoryAcidosis:
      event = "RespiratoryAcidosis";
      break;
    case SEPatientEventType::RespiratoryAlkalosis:
      event = "RespiratoryAlkalosis";
      break;
    case SEPatientEventType::SevereAcuteRespiratoryDistress:
      event = "SevereAcuteRespiratoryDistress";
      break;
    case SEPatientEventType::StartOfCardiacCycle:
      event = "AcuteLungInjury";
      break;
    case SEPatientEventType::StartOfExhale:
      event = "StartOfExhale";
      break;
    case SEPatientEventType::StartOfInhale:
      event = "StartOfInhale";
      break;
    case SEPatientEventType::SevereSepsis:
      event = "SevereSepsis";
      break;
    case SEPatientEventType::Tachycardia:
      event = "Tachycardia";
      break;
    case SEPatientEventType::Tachypnea:
      event = "Tachypnea";
      break;
    case SEPatientEventType::_TotalPatientEvents:
      event = "TotalPatientEvents";
      break;
    }
    std::string state = (active) ? "(On)" : "(Off)";
    if (event.size() < marquee_1.size()) {
      marquee_1.replace(marquee_1.begin() + (marquee_1.size() / 2) - (event.size() / 2),
                        marquee_1.begin() + (marquee_1.size() / 2) + (event.size() / 2) + (state.size() % 2),
                        event.begin(), event.end());
    } else {
      marquee_1 = event;
    }
    if (state.size() < marquee_2.size()) {
      marquee_2.replace(marquee_2.begin() + (marquee_2.size() / 2) - (state.size() / 2),
                        marquee_2.begin() + (marquee_2.size() / 2) + (state.size() / 2) + (state.size() % 2),
                        state.begin(), state.end());
    } else {
      marquee_1 = state;
    }
    std::cout << prefix << marquee_1 << end;
    std::cout << prefix << marquee_2 << end;
    std::cout << "(__________|_[______]_|______________________________________|\n";
    std::cout << "  0--0--0      0  0      0       0     0        0        0    \n";
    std::cout << std::endl;
  }
  void HandleAnesthesiaMachineEvent(SEAnesthesiaMachineEvent type, bool active, const SEScalarTime* time = nullptr) override
  {
  }
};


//--------------------------------------------------------------------------------------------------
/// \brief
///
///
/// \details
///
//--------------------------------------------------------------------------------------------------

void hypoxia_callback(bool active)
{
  std::cout << "\n"
  << R"(
________00000000000___________000000000000_________
______00000000_____00000___000000_____0000000______
____0000000_____________000______________00000_____
___0000000_______________0_________________0000____
__000000____________________________________0000___
__00000_____________________________________ 0000__
_00000___________Hypoxia Event______________00000__
_00000_____________________________________000000__
__000000_________________________________0000000___
___0000000______________________________0000000____
_____000000____________________________000000______
_______000000________________________000000________
__________00000_____________________0000___________
_____________0000_________________0000_____________
_______________0000_____________000________________
_________________000_________000___________________
_________________ __000_____00_____________________
______________________00__00_______________________
________________________00_________________________

   )"
  << std::endl;
};
void respiratory_acidosis_callback(bool active)
{
  std::cout << "\n"
  << R"(
                                        .,,cccd$$$$$$$$$$$ccc,
                                    ,cc$$$$$$$$$$$$$$$$$$$$$$$$$cc,
                                  ,d$$$$$$$$$$$$$$$$"J$$$$$$$$$$$$$$c,
    Respiratory                 d$$$$$$$$$$$$$$$$$$,$" ,,`?$$$$$$$$$$$$L
        Acidosis              ,$$$$$$$$$$$$$$$$$$$$$',J$$$$$$$$$$$$$$$$$b
          Callback           ,$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$i `$h
                             $$$$$$$$$$$$$$$$$$$$$$$$$P'  "$$$$$$$$$$$h $$
                            ;$$$$$$$$$$$$$$$$$$$$$$$$F,$$$h,?$$$$$$$$$$h$F
                            `$$$$$$$$$$$$$$$$$$$$$$$F:??$$$:)$$$$P",. $$F
                             ?$$$$$$$$$$$$$$$$$$$$$$(   `$$ J$$F"d$$F,$F
                              ?$$$$$$$$$$$$$$$$$$$$$h,  :P'J$$F  ,$F,$"
                               ?$$$$$$$$$$$$$$$$$$$$$$$ccd$$`$h, ",d$
                                "$$$$$$$$$$$$$$$$$$$$$$$$",cdc $$$$"
                       ,uu,      `?$$$$$$$$$$$$$$$$$$$$$$$$$$$c$$$$h
                   .,d$$$$$$$cc,   `$$$$$$$$$$$$$$$$??$$$$$$$$$$$$$$$,
                 ,d$$$$$$$$$$$$$$$bcccc,,??$$$$$$ccf `"??$$$$??$$$$$$$
                d$$$$$$$$$$$$$$$$$$$$$$$$$h`?$$$$$$h`:...  d$$$$$$$$P
               d$$$$$$$$$$$$$$$$$$$$$$$$$$$$`$$$$$$$hc,,cd$$$$$$$$P"
           =$$?$$$$$$$$P' ?$$$$$$$$$$$$$$$$$;$$$$$$$$$???????",,
              =$$$$$$F       `"?????$$$$$$$$$$$$$$$$$$$$$$$$$$$$$bc
              d$$F"?$$k ,ccc$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$i
       .     ,ccc$$c`""u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P",$$$$$$$$$$$$h
    ,d$$$L  J$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" `""$$$??$$$$$$$
  ,d$$$$$$c,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$F       `?J$$$$$$$'
 ,$$$$$$$$$$h`$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$F           ?$$$$$$$P""=,
,$$$F?$$$$$$$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$F              3$$$$II"?$h,
$$$$$`$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P"               ;$$$??$$$,"?"
$$$$F ?$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P",z'                3$$h   ?$F
       `?$$$$$$$$$$$$$$$??$$$$$$$$$PF"',d$P"                  "?$F
          """""""         ,z$$$$$$$$$$$$$P
                         J$$$$$$$$$$$$$$F
                        ,$$$$$$$$$$$$$$F
                        :$$$$$c?$$$$PF'
                        `$$$$$$$P
                         `?$$$$F
   )"
  << std::endl;
};
void tachycardia_callback(bool active)
{
  std::cout << "\n"
  << R"(
                    +T+++++++T+++
                    +++|+++++++|+++|
    Tacycardia      ++++++++++++++/+
        Event       ++++++++++++++/++
                    +++++++++++++/++++
                    +++++:::iiiII/++++.
                    IIIIIIIIIIII/Ii++++
                    ITTTTTTTTTT/III++++.
                    "IIIIIIIIIIiIIII++++
                      "IIIIIIIIIiIIII+++.
                        "IIIIIIIIiIIIi+++
                          "IIIIIIIiIII+++
                            "I/\IIIiIII++
                             ///\IIIiIIi+
              .o8OOOOOOOOOOOOOo/IIIIIiIIi
            oOOOOOOOOOOOOOOOOOOOOIIIIIiI
           888888888888OOOOOOOOOOOOIIIII
      o8OOOOOOOOOOOOOOO88888OOOOOOOOIIIT
    oOOOOOOOOOOOOOOOOOOOOOOO888OOOOOb
   8OOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOOOb
   OOOOOOOOOOOOOOOOOOOOOOOOOOOOO88OOOOb
   OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO8OOOO.
   OOOO~~~~~~~~~~~~~~~~OOOOOOOOOOOO8OOOb
   OO~ oo8888888888888oo ~~OOOOOOOOO8OOO
   8 oO888888888888888888oo ~~OOOOOOO8OO
   8 OO888888888888888888888oo OOOOOO8OO
   ~8 O8888888888888888888888888 ~OOOOOP
     ~O88 ~888~Y88Y88P    888888>-~OOOO
       88b ~~ .888 ~~    d88888| ( OOOO
        88Xooo88888ooood8888888/ /XooO~
         ~88888888888888888888| |OOX|
         //88888888888888888:',:OOO|
       ,'/OOOO888888888888~<-
       |   Y ..  ,O\******    _ / | |<_
     ___~-'8o8OO88OX8OOOO    ~ Y  '/O8|
   ,'   `-_OOOOOOOXX|OOOO8o.  .A  /OX/>
  /...     \X8OOOXX/|OOOOO|OOOOO8X\X/ <_
 888888b    \XXX8X/:|OOOOO|\8XXXXX      \
d88888888    |\X8/::|8XXXX|\IXX~  \ d8.  \
888888888b   \ \/ ::lXXXXXl ~~ \--_|888   \
Y88DR88888    |  .::"-----"    /_-_|~8~   |
 888888888b   \  :::          /_ -_|      |
 8888GUZ888    | ::          /_ -_ |      |
  888888888b   \:::         /_ -_ /       [
   888888888    |`:        /  -_ /       ]
    88888888b     |       |     /        |
     88888888b    [       |    (        /
      88888888   ]        |            /
       ~888888b  |         \         ,'
         ~88888  |          `.___,--'
           ~~88_/
   )"
  << std::endl;
};
void hypercapnia_callback(bool active)
{
  std::cout << "\n"
  << R"(
                             _,,aaaaa,,_
                          _,dP"''    `""""Ya,_
                       ,aP"'                `"Yb,_
                     ,8"'                       `"8a,
                   ,8"                             `"8,_
                 ,8"                                  "Yb,
               ,8"       Hypecapnia                     `8,
              dP'           Event                        8I
            ,8"                           bg,_          ,P'
           ,8'                              "Y8"Ya,,,,ad"
          ,d"                            a,_ I8   `"""'
         ,8'                              ""888
         dP     __                           `Yb,
        dP'  _,d8P::::Y8b,                     `Ya
   ,adba8',d88P::;;::;;;:"b:::Ya,_               Ya
  dP":::"Y88P:;P"""YP"""Yb;::::::"Ya,             "Y,
  8:::::::Yb;d" _  "_    dI:::::::::"Yb,__,,gd88ba,db
  Yb:::::::"8(,8P _d8   d8:::::::::::::Y88P"::::::Y8I
  `Yb;:::::::""::"":b,,dP::::::::::::::::::;aaa;:::8(
    `Y8a;:::::::::::::::::::::;;::::::::::8P""Y8)::8I
      8b"ba::::::::::::::::;adP:::::::::::":::dP::;8'
      `8b;::::::::::::;aad888P::::::::::::::;dP::;8'
       `8b;::::::::""""88"  d::::::::::b;:::::;;dP'
         "Yb;::::::::::Y8bad::::::::::;"8Paaa""'
           `"Y8a;;;:::::::::::::;;aadP""
               ``""Y88bbbdddd88P""8b,
                        _,d8"::::::"8b,
                      ,dP8"::::::;;:::"b,
                    ,dP"8:::::::Yb;::::"b,
                  ,8P:dP:::::::::Yb;::::"b,
               _,dP:;8":::::::::::Yb;::::"b
     ,aaaaaa,,d8P:::8":::::::::::;dP:::::;8
  ,ad":;;:::::"::::8"::::::::::;dP::::::;dI
 dP";adP":::::;:;dP;::::aaaad88"::::::adP:8b,___
d8:::8;;;aadP"::8'Y8:d8P"::::::::::;dP";d"'`Yb:"b
8I:::;""":::::;dP I8P"::::::::::;a8"a8P"     "b:P
Yb::::"8baa8"""'  8;:;d"::::::::d8P"'         8"
 "YbaaP::8;P      `8;d::;a::;;;;dP           ,8
    `"Y8P"'         Yb;;d::;aadP"           ,d'
                     "YP:::"P'             ,d'
                       "8bdP'    _        ,8'
      Normand         ,8"`""Yba,d"      ,d"
      Veilleux       ,P'     d"8'     ,d"
                    ,8'     d'8'     ,P'
                    (b      8 I      8,
                     Y,     Y,Y,     `b,
               ____   "8,__ `Y,Y,     `Y""b,
           ,adP""""b8P""""""""Ybdb,        Y,
         ,dP"    ,dP'            `""       `8
        ,8"     ,P'                        ,P
        8'      8)                        ,8'
        8,      Yb                      ,aP'
        `Ya      Yb                  ,ad"'
          "Ya,___ "Ya             ,ad"'
            ``""""""`Yba,,,,,,,adP"'
   )"
  << std::endl;
};
void acute_respiratory_distress_callback(bool active)
{
  std::cout << "\n"
  << R"(
 Acute       .--._
  Respiratory  `.  `-.
    Distress   .-'    `.
             .'      _..:._
            `.  .--.'      `.
            _.'  \.      .--.\
          .'      |     |    |
         `--.     |  .--|    D
             `;  /'\/   ,`---'@
           .'  .'   '._ `-.__.'
         .'  .'      _.`---'
       .'--''   .   `-..__.--.
jgs ~-=  =-~_-   `-..___(  ===;
    ~-=  - -    .'     
   )"
  << std::endl;
};
void irreversible_state_callback(bool active)
{
  std::cout << "\n"
  << R"(
__________________|      |____________________________________________
     ,--.    ,--.          ,--.   ,--.     You Partient Died! LOLZ
    |oo  | _  \  `.       | oo | |  oo|
o  o|~~  |(_) /   ;       | ~~ | |  ~~|o  o  o  o  o  o  o  o  o  o  o
    |/\/\|   '._,'        |/\/\| |/\/\|
__________________        ____________________________________________
                  |      |dwb
   )"
  << std::endl;
};

int HowToPatientEvents()
{
  // Create the engine and load the patient
  std::unique_ptr<BioGearsEngine> bg = std::make_unique<BioGearsEngine>("HowToPain.log"
    );
  bg->GetLogger()->Info("HowToPain");
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return 1;
  }

  auto biogears = dynamic_cast<BioGears*>(bg.get());
  biogears->GetPatient().SetEventCallback( SEPatientEventType::Hypoxia, hypoxia_callback);
  biogears->GetPatient().SetEventCallback( SEPatientEventType::RespiratoryAcidosis, respiratory_acidosis_callback);
  biogears->GetPatient().SetEventCallback( SEPatientEventType::Tachycardia, tachycardia_callback);
  biogears->GetPatient().SetEventCallback( SEPatientEventType::Hypercapnia, hypercapnia_callback);
  biogears->GetPatient().SetEventCallback( SEPatientEventType::AcuteRespiratoryDistress, acute_respiratory_distress_callback);
  biogears->GetPatient().SetEventCallback( SEPatientEventType::IrreversibleState, irreversible_state_callback);

  CustomEventHandler eventHandler;
  bg->SetEventHandler(&eventHandler);

  SEAnesthesiaMachineConfiguration machine { bg->GetSubstanceManager() };
  auto& machine_config = machine.GetConfiguration();
  bg->AdvanceModelTime(50.0, TimeUnit::s);

  machine_config.SetConnection(SEAnesthesiaMachineConnection::Mask);
  machine_config.GetInletFlow().SetValue(5.0, biogears::VolumePerTimeUnit::L_Per_min);
  machine_config.GetInspiratoryExpiratoryRatio().SetValue(.5);
  machine_config.GetOxygenFraction().SetValue(0.23);
  machine_config.SetOxygenSource(SEAnesthesiaMachineOxygenSource::Wall);
  machine_config.GetPositiveEndExpiredPressure().SetValue(0.0, biogears::PressureUnit::cmH2O);
  machine_config.SetPrimaryGas(SEAnesthesiaMachinePrimaryGas::Nitrogen);
  machine_config.GetRespiratoryRate().SetValue(16, biogears::FrequencyUnit::Per_min);
  machine_config.GetVentilatorPressure().SetValue(0.0, biogears::PressureUnit::cmH2O);
  machine_config.GetOxygenBottleOne().GetVolume().SetValue(660, biogears::VolumeUnit::L);
  machine_config.GetOxygenBottleTwo().GetVolume().SetValue(660, biogears::VolumeUnit::L);

  bg->ProcessAction(machine);

  bg->AdvanceModelTime(50.0, TimeUnit::s);

  machine_config.GetLeftChamber().SetState(SEOnOff::On);
  machine_config.GetLeftChamber().GetSubstanceFraction().SetValue(0.05);
  machine_config.GetLeftChamber().SetSubstance(*bg->GetSubstanceManager().GetSubstance("Desflurane"));
  bg->ProcessAction(machine);

  bg->AdvanceModelTime(0.5, TimeUnit::hr);

  machine_config.GetLeftChamber().SetState(SEOnOff::Off);
  bg->ProcessAction(machine);

  bg->AdvanceModelTime(2, TimeUnit::hr);

  bg->GetLogger()->Info("Finished");
  return 0;
}
int main ( int argc, char* argv[] ) {
  return HowToPatientEvents();
}
