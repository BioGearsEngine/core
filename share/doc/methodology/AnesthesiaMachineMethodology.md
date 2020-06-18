Anesthesia Machine Methodology {#AnesthesiaMachineMethodology}
==========================

@anchor anesthesia-overview
# Overview
@anchor anesthesia-abstract
## Abstract

The %BioGears Anesthesia Model is a generic representation of mechanical ventilation and 
inhaled agent administration. It models a semi-closed circuit breathing system. The 
Anesthesia System employs a pressure-control ventilation mode for positive-pressure 
ventilation and gas delivery. The model uses a constant ventilator pressure source to drive 
inspiration and release pressure during expiration.
This model provides an important system required for the training 
and simulation of
a complex medical field. 
A variety of common equipment failures associated with 
anesthesia machines are available with this system. The results of this system were 
qualitatively evaluated for all common failures
modeled. The results show an excellent correlation with the expected trends. 
Future work will address the current limitations of the system and development 
of a more robust Anesthesia Machine.
@anchor anesthesia-intro
## Introduction

### Anesthesia Machines and Positive-Pressure Ventilation

An anesthesia machine is used to both control a patient's gas exchange through mechanical ventilation, or positive-pressure ventilation, and administer inhaled anesthetic agents. These machines are highly 
sophisticated, and they require training and problem-solving skills for appropriate use and avoidance of user- and equipment-related failures. While there are relatively few companies manufacturing 
anesthesia machines, there is a large variety in models and types of machines. This provides a wide range of complexity and functionality that is dependent on a facility's expectations and requirements. 
The administration of anesthesia also requires the delivery of multiple drugs that are chosen based on the individual patient's condition and the procedure type and duration. The combination of equipment and drug 
administration makes anesthesiology a difficult discipline that requires significant training to avoid failures that could lead to patient injury or death. @cite roth2007anesthesia @cite morgan2006clinical

In general, anesthesia machines contain the following basic components @cite roth2007anesthesia @cite morgan2006clinical :

-   Compressed gas cylinders - These contain oxygen and/or nitrogen for portable use with the machine. Oxygen may also be supplied through a wall port.

-   Vaporizer - Inhaled agents are inserted into the machine in liquid form and must be aerosolized for patient administration.

-   Unidirectional valves - Valves open and close during the inspiratory and expiratory cycles to prevent rebreathing of carbon dioxide.

-   Carbon dioxide absorber - Carbon dioxide is scrubbed from the anesthesia machine circuit using different absorbers.

-   Ventilator - The bellows supplies gas flow to the patient.

-	Tubing - All equipment is connected through tubing that can become tangled or develop leaks if not monitored regularly.

Airway management is also associated with a number of components that provide air to the patient depending on the patient's condition and surgery specifics. These components include the following @cite stackhouse2007airway :

-   Mask - A mask can be placed on the patient's face to supply supplemental oxygen or an inhaled anesthetic. 

-   Endotracheal tube (ETT) - This tube can be placed in the trachea through the intubation process for positive-pressure ventilation for a fully sedated patient. These come in a variety of styles and sizes
    that must be chosen based on the patient.

-   Laryngeal Mask Airway (LMA) - This device has become common for patients with a difficult airway. It combines a silicone rubber mask with a flexible shaft to provide positive-pressure ventilation to a fully
    sedated patient without requiring intubation. 

The large number of components, the sophisticated computer controls, and the high variability in patient needs can lead to user errors with anesthesia equipment. Significant pre-surgery planning is required to ensure an appropriate airway plan is in place. Equipment failures are not uncommon with anesthesia machines. These failures range from minor tube leaks and tangles to a failure of the ventilator to provide adequate pressure
for respiration. @cite morgan2006clinical
	
The %BioGears Anesthesia Machine Model is designed to provide a generic anesthesia machine capable of simulating mechanical ventilation and the administration of inhaled anesthetic agents. This model is capable of powering training simulations regarding the use of anesthesia equipment. In combination with the rest of the %BioGears Engine, the model provides a whole-body approach promoting training on drug administration, airway
management, maintenance of anesthesia, transitions from negative- to positive-pressure ventilation, and diagnosis and problem-solving related to equipment failures. 
@anchor anesthesia-system
# System Design
@anchor anesthesia-background
## Background and Scope

### %BioGears Anesthesia Machine History

The %BioGears Anesthesia Machine has its origin in the work of Dr. N. Ty Smith and colleagues at the University of California at San Diego. Their
work on this subject can be traced back to a series of cardiopulmonary and drug distribution models that they developed @cite Smith1972multiplemodel , @cite Zwart1972analog . For
example, they developed multicompartment physiology and pharmacology models describing the uptake and distribution of halothane
@cite FukuiSmith1981hybrid . The early models of the group employed analog and hybrid computers. Later, they developed models on standard personal computers that formed
a basis for the screen-based simulator originally known as SLEEPER. This program eventually evolved into an advanced anesthesia simulator
called BODY simulation that was distributed by the Advanced Simulation Corporation. The BODY simulation program consisted of cardiovascular, respiratory, 
and drug models that formed the basis for the HumanSim physiology engine, of which the %BioGears Engine is an extension.  The major components that control the gas flow and delivery of the anesthetic drugs are derived from the HumanSim engine. Various modifications, including the integration of the Anesthesia System as part of the closed circuit-based common data model, led to the current version of the %BioGears Anesthesia Delivery System. As it stands currently, the Anesthesia System is under development and its use has some restrictions as described in the Features section below. Future releases will provide a more rigorously tested Anesthesia System that is fully integrated with the respiratory and drug systems.
@anchor anesthesia-flow
## Data Flow

### Preprocess

The ventilator and valves operate via time-based cycles based on the settings. There are four phases to each complete cycle, as shown in Figure 1.

<center><img src="./images/AnesthesiaMachine/Cycles.png" width="550"></center>
<center><i>Figure 1. Flow diagram showing the ventilatory cycles as implemented in the %BioGears %Anesthesia Machine.</i></center> 

#### Process Actions

Process Actions deals with modification of the anesthesia machine
parameters and circuit properties due to actions (insults) as
specified by the user. The insults are described in the Actions section
below.

#### Update Resistances

The breathing circuit of the commonly used Anesthesia Machine consists of unidirectional valves
on the inspiratory and expiratory limbs that control the flow of gases during the inspiration and 
expiration phases. The %BioGears Anesthesia Machine models the unidirectional valves using
resistors and logical on/off conditions that serve as switches. These resistors are updated depending 
on the phase of the respiration cycle. During the inspiratory phase of the breathing cycle, the resistor representing the
unidirectional valve on the inspiratory limb is lowered to a pre-set value (an analogue to a closed electrical circuit), opening it to flow.  At the same time, the 
unidirectional valve at the expiratory limb is closed to flow by employing a very high resistance (an open circuit). The process is reversed during the expiratory phase of 
the breathing cycle.  In addition to being modified during normal breathing cycles, the valve resistances are also adjusted to
model various equipment failures such as inspiratory/expiratory valve leaks and obstructions. 

#### Ventilator Pressure Calculation

In the use of the Anesthesia Machine, there are two basic ventilation modes for precise control of ventilation. These are the pressure controlled and volume-controlled modes.
In the volume-controlled mode, the Anesthesia Machine delivers a preset tidal volume 
to generate ventilation. In this delivery mode, positive-pressure ventilation is delivered at a
predetermined breathing frequency and tidal volume to provide predictable minute
ventilation. In the pressure-controlled ventilation mode, a set plateau (control) pressure  is 
delivered at each breathing 
cycle. The tidal volume delivered to the patient is derived from the control ventilator pressure and the
 lung compliance of 
the patient. The current version of the %BioGears Anesthesia Machine employs the pressure-controlled 
ventilator. During each inspiratory phase, a constant control pressure that
serves as a driving pressure source is applied to the ventilator. This pressure drives flow across 
the inspiratory limb path to supply gas to the patient. During the expiratory phase, the ventilator
driver pressure drops, allowing gas to return to the ventilator. In the
engine, the volume of the ventilator is adjusted based on the gas flow into the ventilator node. 

#### Gas Inlet Flow Calculation

The gas inlet is modeled as a flow source to add substances in the form of air and/or inhaled drugs to the system. The inlet flow is set by the total
flow parameter, and the volume fractions are based on gas source
concentrations.

#### Breathing Cycle Calculation

The %BioGears Engine calculates the inspiratory and expiratory phase times based on 
a preset respiration rate and inspiration-expiration ratio parameters that are selected as 
inputs for the Anesthesia Machine configurations. 

### Process

The current %BioGears implementation has no specific circuit or transport process
functionality for the anesthesia machine. Anesthesia Machine processing
is currently done in the %Respiratory System with the combined circuit
methodology.  However, the substance volume fractions and volumes are updated to remove carbon dioxide in the scrubber during this step.

### Postprocess

The Postprocess step moves values calculated in the Process step from the next
time step calculation to the current time step calculation. The current %BioGears has no 
specific postprocess functionality for the anesthesia machine. All postprocessing is done in the
%Respiratory System with the combined circuit methodology.

The following figure presents the data flow of the %Anesthesia System data processing steps.

<center><img src="./images/AnesthesiaMachine/AnesthesiaMachineDataFlow.png" width="550"></center>
<center><i>Figure 2. The Anesthesia Machine system has three processing steps at the highest level. Each time step, Preprocess prepares the circuit to be calculated; Process performs the calculation to determine the entire circuit state; and Postprocess advances time.</i></center>    
<br>
@anchor anesthesia-features
## Features and Capabilities

### The %BioGears Anesthesia Machine Circuit

The %BioGears %Anesthesia Machine System represents a generic mechanical ventilation system. 
It models a semi-closed anesthesia breathing circuit that employs a pressure-control 
mode of positive-pressure ventilation. The Model is designed to represent the
functions of the ventilator, breathing circuit, and vaporizer typical of an
Anesthesia Delivery System. To incorporate these functions, the
Anesthesia Machine utilizes seven major compartments. These
compartments correspond to the three flow tubes (the Y-piece and the
inspiratory and expiratory tubes), the CO2 absorber,
the fresh gas inlet of the breathing circuit, the ventilator, 
and the mask compartments. The fresh gas inlet allows the flow of 
volatile anesthetic medications and fresh gas into the breathing circuit. 
The vaporizer functionality is incorporated 
by adjusting the concentration of the gas mixture delivered through the 
fresh gas inlet. In anesthesia machine systems, vaporizers are designed to hold 
only one specific volatile anesthetic medication. The %BioGears Anesthesia 
Model reproduces this constraint by employing conditions that represent
left and right chambers, each of which hold
a specific inhaled agent. The model treats the agents in these chambers as vapors and 
mixes them with other gases to be delivered into the breathing circuit, adjusting the total gas mixture accordingly.  In particular, the model deducts the volume
fraction of the nitrogen gas proportional to the oxygen and/or inhaled volatile gases concentration. 

In this way, the %BioGears Anesthesia Machine captures the effect of the vaporizer functionality. 
For supplemental oxygen delivery, the model offers a separate wall port or an oxygen bottle as options. The model
allows for two oxygen bottles that can be used sequentially, depending on the length of 
anesthesia delivery and level of oxygen depletion. In the event of endotracheal
intubation, the mask compartment is replaced by the endotracheal tube
compartment. These two compartments are used interchangeably. The
inspiratory and expiratory flow tube compartments are regulated by
unidirectional valve switches and allow gas flow into and out of the
lung. The ventilator compartment supports positive-pressure ventilation for oxygen delivery and ventilation. Finally, 
the CO2 absorption compartment provides critical functionality by reducing the amount of
inhaled CO2 in re-breathed gas. The diagram in Figure 3 presents the
compartmental view of the Anesthesia Delivery System.

<center><img src="./images/AnesthesiaMachine/AnesthesiaMachineGasFlowDiagram.png" width="650"></center>
<center>
*Figure 3. Compartmental view of the Anesthesia System. The %BioGears Anesthesia 
Machine consists of seven major compartments, as shown in the breathing circuit. 
These are the mask/endotracheal tube compartment, the Y-piece, the inspiratory 
and expiratory limbs, the fresh gas inlet, the CO2 absorber, 
and the ventilator compartments.  The model also includes a relief valve path that emulates 
the exhaust scavenging interface.  
The mask and the endotracheal tube compartments are interchangeable. 
The unidirectional 
switches shown on the inspiratory and expiratory limbs permit unidirectional airflow 
in the patient's breathing cycle. The connecting lines represent the flow of gas between 
compartments and 
the arrows show the flow direction. The red lines and the associated arrows in the figure 
show flow direction during the inspiratory phase.  The %BioGears
Anesthesia Machine also interacts with the %Respiratory System through a circuit 
path that links the two systems at the airway node.*
</center><br>

Compartments in the Anesthesia System are modeled as a network of
resistors to account for the resistance to flow through each compartment. 
The ventilator is designed to accumulate volume on its node based on the flow 
across its path. When mechanical ventilation is not
invoked, the model switches to spontaneous breathing via negative
pressure ventilation that is driven by the respiratory muscle pressure.

The gas flow in the compartments is driven
by pressure gradients across the resistors
in the breathing circuit. The flow resistances in the breathing circuit are primarily due to the the unidirectional valves and other regions of constriction.  Since practically
all of the pressure drops occur across these resistors, the circuit diagram
(Figure 4) representing the %Anesthesia %Machine is a condensed version of the compartmental
diagram (Figure 3). A single node encapsulates both the mask and Y-piece, as well as a current source which can be used to model a leak at the mask. The exhaust node can represent either a leak to the atmosphere or a scavenging system that relieves pressure build up. Normally, 
the exhaust is connected to a breathing bag reservoir; however, the current version
lacks such a reservoir. Instead, the exhaust in the circuit diagram serves as a current 
source that facilitates appropriate pressure maintenance. 

@image html AnesthesiaMachineCircuitDiagram.png
<center>
*Figure 4. Circuit diagram of the Anesthesia Machine. The circuit 
employs the ventilator as a driver pressure source. 
One of the two pressure sources is selected using the switch. 
The fresh gas inlet serves as a current source that drives fresh gas and anesthetic drugs 
into the breathing circuit. When the Anesthesia Machine is turned on, the switch between 
the mask (represented as AnesthesiaConnection) and the mouth node of the 
%Respiratory System becomes closed. The switch between the mask and the mouth node remains 
open when the Anesthesia Machine is turned off. The unidirectional valves of 
the Anesthesia Machine are handled by assigning very high resistance on the 
inspiratory or expiratory limbs depending on the breathing phase.*
</center><br>

### Connecting to the %Respiratory Circuit

When an anesthesia machine is used on a patient, there is a direct
connection that allows air to flow freely between them. In the same
fashion, the Anesthesia Machine and %Respiratory circuits in %BioGears are
directly connected and allowed to share the same fluid. When the machine
is turned on, both individually defined circuits are combined into a
single circuit that is then used for calculations.

### The %BioGears Anesthesia Machine Settings

Anesthesia machines operate in a specific mode for positive-pressure
delivery. As discussed above, there are two modes of ventilation commonly used.
The %BioGears Anesthesia Machine employs a pressure-controlled ventilator
for positive-pressure ventilation. 

To achieve this functionality, a set of parameters are pre-defined to control the Anesthesia Machine configuration. The %BioGears
Anesthesia Machine assigns the ventilator pressure,
respiration rate, inspiration-to-expiration ratio, relief valve
pressure, and positive end expired pressure as input parameters. 
Additionally, the %BioGears model assigns a set of parameters to control the fresh gas
flow and the various gas fractions delivered to the breathing circuit.
Table 1 lists the Anesthesia Machine configuration settings
currently used in the engine. It should be noted that the current
version of the Anesthesia Machine Model is restricted to the values
listed in the table until a wider range of values are tested. Future
versions will allow user-defined Anesthesia Machine settings.

<center>
*Table 1. Inputs to Anesthesia Machine*
</center>
|	Parameter	|	Description 	|
|	---	|	---	|
|	<b>General Anesthesia Machine Settings</b>	|		|
|	State (on/off)	|	Determines if the anesthesia machine is running or not	|
|	VentilatorPressure	|	The maximum ventilator pressure applied each cycle	|
|	RespiratoryRate	|	Frequency of the ventilator	|
|	PositiveEndExpiredPressure	|	The minimum ventilator pressure applied each cycle	|
|	InspiratoryExpiratoryRatio	|	The relative ratio of the period of positive and negative flow in and out of the lungs	|
|	ReliefValvePressure	|	The pressure at which air will be vented out of the machine - pressure will not be able to go above this	|
|	<b>Gas Inlet Settings</b>	|		|
|	InletFlow	|	The quantity of flow from the Oxygen Source	|
|	OxygenFraction	|	Sets the volume fraction of oxygen in delivered mixture	|
|	PrimaryGas (Nitrogen/Air)	|	Sets a selection for primary gas, such as air (ambient) or nitrogen	|
|	OxygenSource (Bottle 1/Bottle 2/Wall)	|	Sets the source of the gas composition to be used	|
|	OxygenBottleOneVolume	|	Sets the volume of the first  oxygen Source tank	|
|	OxygenBottleTwoVolume	|	Sets the volume of the second  oxygen Source tank	|
|	<b>Related Settings</b>	|		|
|	AirwayMode (Free/Mask/Tube)	|	Used to add or remove the mask or tube via Actions	|
|	SubstanceAdministration	|	Substances are added to the vaporizer (i.e. gas source) by specifying a substance and chamber (left or right)	|

@anchor anesthesia-example
#### Example Scenario: Standard Settings
All BioGears anesthesia machine scenarios assume that the patient is fully unconsious and requires total ventilation support (see [Assumptions/Limitations](@ref anesthesia-assumptions).  Full patient sedation is accomplished by administering a sedative from the BioGears Drug library, such as propofol or midazolam.  A neuromuscular blocker such as succinylcholine or rocuronium can be co-administered to simulate situations in which the airway muscles must be forcibly relaxed.  Our demonstration uses an induction dose of 1.8 mg/kg of propofol followed by a maintenance propofol infusion of 140 ug/kg/min (@cite becker2012pharmacodynamic).  When the patient is deeply sedated, we intubate and configure the anesthesia machine with the following settings:
		- Ventilator Pressure = 10 cmH2O
		- Positive End Expired Pressure = 1 cmH2O
		- Inspiratory:Expiratory Ratio = 0.5
		- Respiratory Rate = 12 /min
		- Inlet Flow = 5.0 L/min
		- Oxygen Fraction = 0.25
		- Primary Gas = Nitrogen

We maintain the anesthesia machine for 45 minutes (to simulate some procedure), discontinue the propofol infusion, wait 15 minutes for the sedation to begin to wear off, and then take the patient off the ventilator.  Figure 5 shows the results of this simulation.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaDemo_RASS.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaDemo_Propofol.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaDemo_RespirationRate.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaDemo_Ventilation.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 12. Select outputs from the inspiratory valve obstruction scenario.</i></center>

@anchor anesthesia-dependencies
### Dependencies

The %BioGears Anesthesia Machine interacts with 
the %Respiratory System, delivering gases and anesthetic drugs into the
%Respiratory System (see @ref RespiratoryMethodology). The two 
systems are connected to each other via a path between the mouth node of the
 %Respiratory System and the mask/endotracheal node (AnesthesiaConnection 
in Figure 4) of the anesthesia
system. During spontaneous ventilation, the mouth node of the
%Respiratory System is connected to the atmosphere via the %Environment System. 
This serves as a ground node for the %Respiratory System. 

When the Anesthesia Machine is turned on, a network of combined circuits that
includes the elements from both the %Respiratory and Anesthesia Systems is
created. When the combined circuit is generated at runtime, the
ground environment node connected to the mouth node of the %Respiratory System is
replaced by the AnesthesiaConnection node that represents the mask/endotracheal node.
As a result, the %BioGears Anesthesia Delivery System interacts with the flow resistances of the %Respiratory System. In
this regard, the ventilator driver pressure serves as a positive-pressure source for 
the combined circuit, allowing the Anesthesia System to manage pressure-controlled ventilation.
To provide air for the breathing circuit, the Anesthesia Machine is linked to the %Environment 
System, which regulates the gas concentration and atmospheric pressure. The %BioGears engine
%Anesthesia System has system data that allows for selection of the primary gas (air, 
nitrogen, etc.). Once the primary gas is selected, the delivery of supplemental 
oxygen and/or anesthetic gases is adjusted according the composition and fraction of gases derived from the 
%Environment System (see @ref EnvironmentMethodology).
@anchor anesthesia-assumptions
## Assumptions and Limitations

The %BioGears Anesthesia Machine is a generic model representing components crucial to the administration of 
inhaled agents and mechanical ventilation. This system is intended to provide
a model that can simulate the response of the body to positive-pressure ventilation, 
drug administration, and equipment failures. The model employs a pressure-controlled, positive-pressure ventilation mode and sets
a constant ventilator pressure to drive inspiration.  

Currently, there are two limitations with the model. First, the model does not allow 
manual breathing using a bag reservoir as a pressure source. Secondly, the current version of 
the model sets a constant respiration rate consistent with pressure-controlled or volume-controlled ventilators. 
This modeling choice is necessary for modeling a fully paralyzed patient. However, for anesthetic 
cases that do not require complete muscle paralysis, a synchronized ventilation that allows for spontaneous breathing would be necessary.
Future efforts may be directed towards rectifying this limitation.
@anchor anesthesia-actions
## Actions

The %BioGears Anesthesia Machine can model a variety of different equipment
failures. Breathing circuit disconnection is a leading cause of critical
incidents in anesthesia @cite roth2007anesthesia @cite morgan2006clinical .
Disconnections, which are effectively leaks, can be complete or partial. Some of these
failures are modeled in %BioGears by changing a resistance value based on
the severity. Figure 6 shows the resistance value that is set based on
the insult severity.  This mapping will be referenced with respect to the actions in this
section. We have chosen a resistance of 1000 cmH2O-s/L to be associated with a fully open 
switch (closed valve) and a resistance of 0.001 cmH2O-s/L to represent a fully 
closed switch (open valve). Intermediate resistances map logarithmically between those two
extents. Several insults modify the same resistance that leads to ground
(atmosphere) because the change in flow produced by the ventilator
reaches the patient in the same way.

@image html AMSeverityMap.png

<center>
*Figure 6. This plot shows the resistance value that is set based on the
leak severity modifier. The severity is mapped logarithmically in a
manner that allows the highest severity to allow virtually all flow through
the path and the lowest severity to allow almost no flow through the
path. Obstructions use the same function with the severity reversed
(i.e., 1-severity).*
</center><br>

### Connection Leaks

The following three types of leaks are all implemented via the same strategy in the Anesthesia Machine code. The mask/tube leaks and the Y-piece disconnect can be called simultaneously, and the severities will be combined.

#### Mask Leak

One of the most common equipment failures in the anesthetic delivery system
is a mask leak. If a mask is being used on an Anesthesia Machine and is improperly
secured or has damage, leaks can occur. The %BioGears Engine models mask
leaks by varying a resistance based on the severity in the manner previously
described and shown in Figure 6.

#### Tube Leak

Endotracheal tube cuff leaks are often caused by structural defects in the endotracheal tube, 
but may also arise from improper placement or a failure to fully inflate the cuff.  Large leaks can lead to inadequate ventilation. The %BioGears Engine models endotracheal tube 
leaks by varying a resistance based on severity
in the manner previously described and shown in Figure 5.

#### Y-Piece Disconnect

The most common disconnection site is at the Y-piece @cite roth2007anesthesia @cite morgan2006clinical .
The %BioGears Engine models endotracheal tube leaks by varying a resistance based on the severity in 
the manner previously described and shown
in Figure 6. In the case of both a tube and Y-piece leak, the severities are combined and bounded by an upper value of one.

### Valve Leaks

Inspiratory and expiratory valves leaks are similar to endotracheal tube cuff leaks. 
The valves are modeled as resistors that alternate
between very high and very low resistance values to mimic a switch opening and closing 
every breath cycle. This prevents rebreathing of carbon 
dioxide during the respiration cycle. If there is a leak present, the valve closed (i.e., high resistance) 
value is modified based on the severity
in the manner previously described and shown in Figure 6.

### Valve Obstructions

Anesthesia Machine tracheal tubes can become kinked, and hoses throughout the breathing circuit are subject to occlusions by external
mechanical forces that can impinge the flow @cite roth2007anesthesia @cite morgan2006clinical .
%BioGears models both inspiratory and expiratory valve obstructions similarly to leaks, by modifying the valve resistance values. If there is an
obstruction present, the valve open (i.e., low resistance) value is modified based on the severity in the manner previously described and shown
in Figure 6.

### Ventilator Pressure Loss

Both piston and bellows ventilators can sometimes experience mechanical failures that prevent them from filling completely. This causes the driving pressure of the 
ventilator to be less than the target. %BioGears uses a ventilator failure severity to linearly reduce the pressure produced by the ventilator, i.e., the pressure source value. The
pressure source can range from the full calculated value to a zero pressure. Failure in ventilator pressure may affect the amount of tidal volume delivered to the patient, which 
in turn reduces the amount of fresh gas delivered to the body, affecting the oxygen saturation level and other physiological parameters.

### Soda Lime Failure

Re-breathing circuits direct expired gases through soda lime granules to remove CO2. To ensure that a soda lime canister (CO2 absorber) is
functioning properly, it must be replaced regularly. If it is not monitored and replaced, the absorber will become less effective or completely unable
to remove CO2 from the breathing circuit. The %BioGears Anesthesia Machine models soda lime failure by reducing the amount of CO2 removed. The severity
linearly decreases the amount of CO2 removed.

### Vaporizer Failure

Vaporizers in anesthesia machines convert volatile anesthetic medications from a liquid state to a vapor at a predetermined concentration for patient delivery. Anesthesia Machine 
vaporizers can fail due to as leaks in seals and O-rings or electronics failures. The %BioGears Engine models vaporizer
failures by reducing the inhaled agent volume fraction at the vaporizer flow source linearly with the severity.

### Oxygen Supply Failures

Supplemental oxygen is supplied to the Anesthesia System either from the wall source or an oxygen tank bottle. 
Many scenarios, such as problems with the oxygen pipeline, can cause disruption of gas delivery. To identify these issues, 
pressures in the gas supplies are often checked. The %BioGears Anesthesia System attempts to capture such 
equipment failures by triggering oxygen wall pressure loss and oxygen tank pressure loss conditions. When the
oxygen pressure loss conditions are triggered, the model
removes the supplemental oxygen delivered to the gas flow inlet by setting the oxygen inlet volume 
fractions to zero.
@anchor anesthesia-events
Events
------

### Oxygen Bottle Exhausted

The %BioGears %Anesthesia system triggers the oxygen bottle tank exhausted  event to alert 
the user of the depletion of each of the oxygen bottles when they are selected as a source of 
supplemental oxygen. 

### Relief Valve Active 

The %BioGears %Anesthesia system triggers the relief valve active event to
alert the user when the relief valve is closed (in the electric analogue sense), 
and a condition the alerts the user when the pressure has reached the threshold value. 
@anchor anesthesia-results
# Results and Conclusions
@anchor anesthesia-settingsvalidation
## Validation - Settings

The Anesthesia Machine Settings are fully dynamic and do not have any bounds enforced.  A scenario that varies these settings in several different combinations is included with the %BioGears deployment and produces the outputs shown in Figure 7.  This scenario also tests the relief valve functionality and causes the active event to be logged when the Ventilator Pressure is set too high.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaMachineVariedConfiguration_VentilatorVolume.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaMachineVariedConfiguration_TotalLungVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaMachineVariedConfiguration_RR.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/AnesthesiaMachineVariedConfiguration_TidalVolume.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/AMVariedLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 7. These plots show the successful implementation of various Anesthesia Machine settings. The first region has the machine connected and provides supplemental Oxygen only. The other three regions follw the administration of Succinylcholine provided to stop unassisted breathing. The upper plot shows compartment data from the ventilator and relief valves. The relief valve is shown to be active by the blue flow plot when the ventilator pressure is set too high. These plots exemplify the interconnectivity of the Anesthesia Machine with the %Respiratory System.</i></center><br>
@anchor anesthesia-actionsvalidation
## Validation - Actions

All equipment failures in the Anesthesia Machine system were validated quantitatively where possible and qualitatively elsewhere by comparing the engine output to expected trends and values. For each scenario, the table shows the total number of results in each category. For many investigated scenarios, the model shows good agreement with the expected trends. For the scenarios that did not match with the expected trends, improvements are planned for future %BioGears Engine releases.

<center><br>
Table 2. Cumulative validation results for Anesthesia Machine specific conditions and actions scenarios.
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	EndotrachealTubeLeakVaried	|	Tests endotracheal tube leak incidence; Succinylcholine (Succs) injected and anesthesia machine is turned on. Endotracheal tube is intubated and multiple levels (severity:  0.5, 1.0)  of tube cuff leak is tested.	|<span class="success">	21	</span>|<span class="warning">	5	</span>|<span class="danger">	4	</span>|
|	ExpiratoryValveLeakVaried	|	Tests multiple levels (severity: 0.5, 1.0) of expiratory valve leak event. Succs injected, anesthesia machine is turned on, and mask is placed. 	|<span class="success">	25	</span>|<span class="warning">	5	</span>|<span class="danger">	0	</span>|
|	ExpiratoryValveObstructionVaried	|	Tests multiple levels (severity: 0.5, 1.0) of expiratory valve obstruction event. Succs injected, anesthesia machine is turned on, and mask is placed. 	|<span class="success">	23	</span>|<span class="warning">	4	</span>|<span class="danger">	3	</span>|
|	InspiratoryValveLeakVaried	|	Tests multiple levels (severity: 0.5, 1.0) of inspiratory valve leak event. Succs injected, anesthesia machine is turned on, and mask is placed. 	|<span class="success">	28	</span>|<span class="warning">	1	</span>|<span class="danger">	1	</span>|
|	InspiratoryValveObstructionVaried	|	Tests multiple levels (severity: 0.5, 1.0) of inspiratory valve obstruction event. Succs injected, anesthesia machine is turned on, and mask is placed. 	|<span class="success">	23	</span>|<span class="warning">	3	</span>|<span class="danger">	4	</span>|
|	MaskLeakVaried	|	Tests multiple levels (severity: 0.5, 1.0) of mask leak event. Succs injected, anesthesia machine is turned on, and mask is placed. 	|<span class="success">	21	</span>|<span class="warning">	5	</span>|<span class="danger">	4	</span>|
|	OxygenTankPressureLoss	|	Tests oxygen tank pressure loss event. Succs injected, anesthesia machine with oxygen bottle tank as oxygen source is turned on, and mask is placed. Oxygen bottle tank loss event is triggered followed by a replacement with a second oxygen bottle.	|<span class="success">	28	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	OxygenWallPressureLoss	|	Tests oxygen wall pressure loss event. Succs injected, anesthesia machine turned on, and mask is placed. Oxygen wall port is used as oxygen source. Then oxygen wall pressure loss event is triggered followed by a replacement of the wall port oxygen source by oxygen bottle tank.	|<span class="success">	22	</span>|<span class="warning">	3	</span>|<span class="danger">	0	</span>|
|	SodaLimeFailureVaried	|	Tests multiple severity (0.5, 1.0) soda lime CO2 scrubber failure event. Succs injected, anesthesia machine turned on, and mask is placed. 	|<span class="success">	27	</span>|<span class="warning">	2	</span>|<span class="danger">	1	</span>|
|	VaporizorFailureVaried	|	Tests multiple severity (0.5, 1.0) vaporizer failure event. Succs injected, anesthesia machine turned on, and mask is placed. 	|<span class="success">	8	</span>|<span class="warning">	4	</span>|<span class="danger">	0	</span>|
|	VentilatorPressureLossVaried	|	Tests multiple severity (0.5, 1.0) of ventilator pressure loss event. Succs injected, anesthesia machine turned on, and mask is placed. 	|<span class="success">	26	</span>|<span class="warning">	3	</span>|<span class="danger">	1	</span>|
|	YPieceDisconnectVaried	|	Tests multiple levels  (severity: 0.5, 1.0) of Y-piece disconnection event. Succs injected, anesthesia machine turned on, and mask is placed. 	|<span class="success">	21	</span>|<span class="warning">	5	</span>|<span class="danger">	4	</span>|
|		|	Total	|<span class="success">	252	</span>|<span class="warning">	42	</span>|<span class="danger">	22	</span>|


#### Connection Leaks

##### Mask Leak

The mask leak, endotracheal tube leak, and Y-piece disconnect scenarios use the same severities at the same times.  Since they share a methodology in implementation by modifying a leak resistor, they produce the same results.  Each scenario begins by administering a neuromuscular blocker to cause a cessation of normal 
respiration. Mechanical ventilation then begins using a mask to supply the gas. The severity of 
the mask leak / endotracheal tube leak / Y-piece disconnect was varied. The oxygen supplied to the patient is expected to decrease with a leak, which in turn 
causes the oxygen saturation to drop and the carbon dioxide 
in the bloodstream to increase. This response is more pronounced
with increasing severity.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/MaskLeakVaried_Inflow.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/MaskLeakVaried_TotalLungVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/MaskLeakVaried_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/MaskLeakVaried_CO2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/MaskLeakLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 8. Select outputs from the Mask Leak scenario. MaskLeakVaried, EndotrachealTubeLeakVaried, and YPieceDisconnectVaried all give the same results.</i></center>

<center><br>
<i>Table 3. Validation matrix for physiological responses due to varying severities of leaks. MaskLeakVaried, EndotrachealTubeLeakVaried, and YPieceDisconnectVaried all give the same results.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite morgan2006clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite morgan2006clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with leak severity of 0.0.	|	Place mask / insert tube. Zero severity has no effect.	|	90	|	150	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on leak with severity 0.5.	|	Leak not severe enough to noticeably drop O2 Sat. Causes pressures below FRC, leading to higher TV.	|	150	|	240	|<span class="success">	No Change 	</span>|<span class="warning">	Begins to drop according to desaturation curve 	</span>|<span class="danger">	Drop in tidal volume @cite el2013endotracheal	</span>|<span class="danger">	Begins to drop w/o full tidal volume 	</span>|<span class="danger">	Begins to increase w/o full tidal volume 	</span>|
|	Remove the leak.	|		|	240	|	330	|<span class="success">	No Change 	</span>|<span class="warning">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on leak again  with severity 1.0.	|	Total lung volume goes to approx. zero - RR is based on waveform, so still registers.	|	330	|	420	|<span class="danger">	Goes to Zero @cite el2013endotracheal	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite el2013endotracheal	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Remove the leak.	|		|	420	|	510	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of full tidal volume 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with full tidal volume 	</span>|<span class="success">	Begins to decrease with full tidal volume 	</span>|


#### Valve Leaks

The inspiratory and expiratory valve leak scenarios begin by administering a neuromuscular 
blocker to cause a cessation of normal respiration. Mechanical ventilation then begins using a 
mask to supply the gas. The severity of the leaks are varied.

##### Expiratory Valve Leak

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveLeakVaried_Inflow.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveLeakVaried_EndTidalCO2.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveLeakVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveLeakVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/ExpiratoryValveLeakLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 9. Select outputs from the expiratory valve leak scenario.</i></center>

<center><br>
<i>Table 4. Validation matrix for physiological responses due to varying severities of an expiratory valve leak.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite morgan2006clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite morgan2006clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with expiratory valve leak severity of 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on expiratory valve leak with severity 0.5.	|		|	180	|	360	|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="warning">	Increases leading to hypercapnia if untreated @cite morgan2006clinical	</span>|
|	Turn off expiratory valve leak.	|		|	360	|	540	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="warning">	Begins to decrease slowly return of functionality 	</span>|
|	Turn on expiratory valve leak with severity 1.0.	|	Aorta CO2 not likely to increase all the way to hypercapnia.	|	540	|	720	|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="success">	No Change @cite morgan2006clinical	</span>|<span class="warning">	No Change @cite morgan2006clinical	</span>|<span class="success">	Increases leading to hypercapnia if untreated @cite morgan2006clinical	</span>|
|	Turn off expiratory valve leak.	|		|	720	|	900	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="warning">	No Change 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|


##### Inspiratory Valve Leak

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveLeakVaried_Inflow.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveLeakVaried_EndTidalCO2.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveLeakVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveLeakVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/InspiratoryValveLeakLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 10. Select outputs from the inspiratory valve leak scenario.</i></center>

<center><br>
<i>Table 5. Validation matrix for physiological responses due to varying severities of an inspiratory valve leak.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with inspiratory valve leak severity of 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on inspiratory valve leak with severity 0.5.	|		|	180	|	360	|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="danger">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off expiratory valve obstruction.	|		|	360	|	540	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|
|	Turn on inspiratory valve leak with severity 1.0.	|		|	540	|	720	|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off expiratory valve obstruction.	|		|	720	|	900	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|


#### Valve Obstructions

The inspiratory and expiratory valve obstruction scenarios begin by administering a neuromuscular 
blocker to cause a cessation of normal respiration. Mechanical ventilation then begins using a 
mask to supply the gas. 
The severity of the obstruction was varied. A full obstruction has the potential to cause a complete 
cessation of respiration or tidal volume due to flow obstruction. Since the neuromuscular blocker
 freezes the patient's spontaneous breathing, the full obstruction of the valves limits the flow of air
 that serves as positive-pressure ventilation to inflate the lungs. 

##### Expiratory Valve Obstruction

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveObstructionVaried_Inflow.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveObstructionVaried_EndTidalCO2.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveObstructionVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/ExpiratoryValveObstructionVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/ExpiratoryValveObstructionLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 11. Select outputs from the expiratory valve obstruction scenario.</i></center>

<center><br>
<i>Table 6. Validation matrix for physiological responses due to varying severities of an expiratory valve obstruction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with expiratory valve obstruction severity of 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on expiratory valve obstruction with severity 0.5.	|	Leak not severe enough to noticeably increase Aorta CO2.	|	180	|	360	|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="warning">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="danger">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off expiratory valve obstruction.	|		|	360	|	540	|<span class="success">	No Change 	</span>|<span class="warning">	Begins to increase with return of respiration 	</span>|<span class="warning">	No Change 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|
|	Turn on expiratory valve obstruction with severity 1.0.	|	Total lung volume goes to approx. zero - RR is based on waveform, so still registers.	|	540	|	720	|<span class="danger">	Goes to zero @cite Roth2000Anesthesia	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to zero @cite Roth2000Anesthesia	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off expiratory valve obstruction.	|		|	720	|	900	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of full tidal volume 	</span>|<span class="danger">	No Change 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|


##### Inspiratory Valve Obstruction

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionVaried_Inflow.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionVaried_EndTidalCO2.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/InspiratoryValveObstructionLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 12. Select outputs from the inspiratory valve obstruction scenario.</i></center>

<center><br>
<i>Table 7. Validation matrix for physiological responses due to varying severities of an inspiratory valve obstruction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with inspiratory valve obstruction severity of 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on inspiratory valve obstruction with severity 0.5.	|		|	180	|	360	|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="success">	No Change @cite Morgan2006Clinical	</span>|<span class="danger">	Reduced tidal volume (minor)@cite Roth2000Anesthesia	</span>|<span class="danger">	Begins to decrease without full tidal volume (very minor if at all) @cite Roth2000Anesthesia	</span>|<span class="danger">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off inspiratory valve obstruction.	|		|	360	|	540	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="warning">	Return to patient normal volume with unobstruction 	</span>|<span class="success">	Begins to increase with full tidal volume 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|
|	Turn on inspiratory valve obstruction with severity 1.0.	|	Total lung volume goes to approx. zero - RR is based on waveform, so still registers.	|	540	|	720	|<span class="danger">	Goes to zero @cite Roth2000Anesthesia	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	system obstruction reduces tidal volume @cite Roth2000Anesthesia	</span>|<span class="success">	Begins to decrease without full tidal volume @cite Roth2000Anesthesia	</span>|<span class="success">	Increases leading to hypercapnia if untreated @cite Morgan2006Clinical	</span>|
|	Turn off inspiratory valve obstruction.	|		|	720	|	900	|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with full tidal volume 	</span>|<span class="warning">	Return to patient normal volume with unobstruction 	</span>|<span class="success">	Begins to increase with full tidal volume 	</span>|<span class="success">	Begins to decrease slowly return of functionality 	</span>|


#### Ventilator Pressure Loss

The ventilator failure scenario begins by administering a neuromuscular blocker to cause a 
cessation of normal respiration. Mechanical ventilation then begins using a mask to supply the gas. 
The severity of the failure was varied. At the full failure, a cessation of respiration was expected. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/VentilatorPressureLossVaried_VentilatorVolume.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/VentilatorPressureLossVaried_TotalLungVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/VentilatorPressureLossVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/VentilatorPressureLossVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/VentilatorPressureLossLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 13. Select outputs from the ventilator pressure loss scenario.</i></center>

<center><br>
<i>Table 8. Validation matrix for physiological responses due to varying severities of a ventilator pressure loss.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with ventilator pressure loss severity 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on ventilator pressure loss with severity 0.5.	|		|	180	|	270	|<span class="success">	May drop or remain at 16 depending on pressure supplied by ventilator @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop according to desaturation curve @cite Morgan2006Clinical	</span>|<span class="success">	Drops without full ventilator pressure @cite Morgan2006Clinical	</span>|<span class="success">	Begins to decrease with lack of full tidal volume @cite Morgan2006Clinical	</span>|<span class="success">	Begins to increase with lack of full tidal volume@cite Morgan2006Clinical	</span>|
|	Turn off Ventilator pressure loss.	|		|	270	|	360	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration machine function @cite Morgan2006Clinical	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with a return of ventilator function @cite Morgan2006Clinical	</span>|<span class="success">	Begins to decrease with a return of ventilator function @cite Morgan2006Clinical	</span>|
|	Turn on ventilator pressure loss with severity 1.0.	|		|	360	|	450	|<span class="danger">	Drops with lack of ventilator pressure @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop according to desaturation curve @cite Morgan2006Clinical	</span>|<span class="success">	Goes to zero with a lack of ventilator pressure @cite Morgan2006Clinical	</span>|<span class="success">	Begins to decrease with lack of oxygen @cite Morgan2006Clinical	</span>|<span class="success">	Begins to increase with lack of ventilator function @cite Morgan2006Clinical	</span>|
|	Turn off Ventilator pressure loss.	|		|	450	|	540	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration machine function @cite Morgan2006Clinical	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with a return of ventilator function @cite Morgan2006Clinical	</span>|<span class="success">	Begins to decrease with a return of ventilator function @cite Morgan2006Clinical	</span>|

#### Soda Lime Failure

The soda lime failure scenario begins by administering a neuromuscular blocker to cause a 
cessation of normal respiration. Mechanical ventilation then begins using a mask to supply the gas. 
The severity of the failure was varied. As expected, respiration rate, tidal volume, and oxygen levels 
remain stable. Carbon dioxide in the blood increases. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/SodaLimeFailureVaried_InletCO2.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/SodaLimeFailureVaried_EndTidalCO2.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/SodaLimeFailureVaried_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/SodaLimeFailureVaried_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/SodaLimeFailureLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 14. Select outputs from the soda lime failure scenario.</i></center>

<center><br>
<i>Table 9. Validation matrix for physiological responses due to varying severities of a soda lime failure.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine with soda lime failure severity 0.0.	|	Place mask. Zero severity has no effect.	|	90	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration	</span>|
|	Turn on soda lime failure with severity 0.5.	|	CO2 not scrubbed as effectively.	|	180	|	360	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change	</span>|<span class="success">	No Change 	</span>|<span class="danger">	Begins to increase w/ failure to absorb CO2 @cite Roth2000Anesthesia	</span>|
|	Turn off soda lime failure.	|		|	360	|	540	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease with functional equipment @cite Roth2000Anesthesia	</span>|
|	Turn on soda lime failure with severity 1.0.	|	CO2 not scrubbed at all.	|	540	|	720	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change	</span>|<span class="warning">	No Change 	</span>|<span class="success">	Begins to increase w/ failure to absorb CO2 @cite Roth2000Anesthesia	</span>|
|	Turn off soda lime failure.	|		|	720	|	810	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease with functional equipment @cite Roth2000Anesthesia	</span>|


#### Vaporizer Failure

The vaporizer failure scenario begins by administering supplemental oxygen via a mask to 
the patient. As previously noted, this is a limitation of the system. Mechanical ventilation 
supplies gas at a prescribed
respiration rate, regardless of whether the patient is conscious and breathing naturally. 
The Anesthesia Machine is then turned on with an inhaled agent, 
desflurane, being supplied. The severity of the failure 
was varied. At a severity of one, the vaporizer supplied no desflurane to the patient, and no changes 
were observed. This was the expected outcome. The drug amount increases in the system and drug effects are 
observed for all other severity levels.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/VaporizerFailureVaried_DesfluraneConcentration.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/VaporizerFailureLegend.jpg" width="800"></td>
</tr>
</table>
</center>
<center><i>Figure 15. Select output from the vaporizer failure scenario.</i></center>

<center><br>
<i>Table 10. Validation matrix for physiological responses due to varying severities of a vaporizer failure.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Drug Plasma Concentration (ug/mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Connect Anesthesia Machine. 	|	Place mask. 	|	30	|	120	|<span class="success">	No Significant Change 	</span>|<span class="success">	No Significant Change 	</span>|<span class="success">	Zero 	</span>|
|	Administer drug via vaporizer. Turn on vaporizer failure of severity 1.0.	|	Full vaporizer failure - no drug in body.	|	120	|	300	|<span class="warning">	No Change 	</span>|<span class="warning">	No Change 	</span>|<span class="success">	Zero 	</span>|
|	Decrease vaporizer failure severity to 0.5.	|		|	300	|	480	|<span class="warning">	Moderate Increase @cite Morgan2006Clinical	</span>|<span class="warning">	Moderate Increase @cite Morgan2006Clinical	</span>|<span class="success">	Increase 	</span>|
|	Turn off vaporizer failure.	|		|	480	|	660	|<span class="success">	Continued drug response @cite Morgan2006Clinical	</span>|<span class="success">	Continued drug response @cite Morgan2006Clinical	</span>|<span class="success">	Increase at faster rate 	</span>|

#### Oxygen Supply Failures

##### Oxygen Wall Pressure Loss

The oxygen wall pressure loss failure scenario begins by administering a neuromuscular blocker to cause a 
cessation of normal respiration. Mechanical ventilation then begins using a mask to supply the gas. 
Oxygen is supplied from the wall source, and the patient is stabilized. Then, oxygen wall port 
pressure source loss is triggered, and the physiological responses of the patient are observed. The process is 
repeated by turning the oxygen wall pressure loss condition on and off, 
followed by the use of the oxygen bottle tank as a source.
The oxygen supply failure due to O2 wall pressure loss causes a drop in arterial oxygen level, while re-supplying the system with
oxygen by turning off the failure brings the oxygen level to normal.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/OxygenWallPressureLoss_InletO2.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/OxygenWallPressureLoss_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/OxygenWallPressureLoss_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/OxygenWallPressureLoss_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/OxygenWallPressureLossLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 16. Select outputs from the oxygen wall pressure loss scenario.</i></center>

<center><br>
<i>Table 11. Validation matrix for physiological responses due to an oxygen wall pressure loss.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	90	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine.	|	Place mask. Use wall oxygen as oxygen source.	|	90	|	210	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on oxygen wall port pressure loss.	|		|	210	|	270	|<span class="success">	No Change 	</span>|<span class="success">	Begins to drop according to desaturation curve @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease with lack of oxygen @cite Morgan2006Clinical	</span>|<span class="success">	No significant change 	</span>|
|	Turn off oxygen wall port pressure loss.	|		|	270	|	390	|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with return of oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with return of oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No significant change 	</span>|
|	Switch from wall port to oxygen bottle.	|		|	390	|	570	|<span class="success">	No Change 	</span>|<span class="success">	Continues to increase with oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="warning">	No Change 	</span>|<span class="warning">	No significant change 	</span>|


##### Oxygen Tank Pressure Loss

The %BioGears Anesthesia System has two oxygen tanks as additional oxygen supply sources. Under normal 
conditions, the model allows 
selection between the two tanks as substitutes for the wall source. To address the effect of 
oxygen tank pressure loss, the model employs a scenario that begins 
by administering a neuromuscular blocker to disrupt normal respiration. Then, the Anesthesia Machine is turned on, a 
mask is placed (similarly to the wall pressure loss scenario), and an oxygen bottle tank is set as the
oxygen source. The patient is stabilized first for some time. Then, the oxygen tank pressure loss event is triggered and the 
physiological responses of the patient are examined. The process is repeated by switching  
the oxygen tank pressure loss state on and off.  Finally, the original oxygen bottle tank 
is replaced with the second tank.
The model responded well and produced expected behavior, showing 
a decline in oxygen saturation and arterial oxygen partial pressure.

<center>
<table border="0">
<tr>
    <td><img src="./plots/AnesthesiaMachine/OxygenTankPressureLoss_InletO2.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/OxygenTankPressureLoss_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/AnesthesiaMachine/OxygenTankPressureLoss_CO2PP.jpg" width="550"></td>
    <td><img src="./plots/AnesthesiaMachine/OxygenTankPressureLoss_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/AnesthesiaMachine/OxygenTankPressureLossLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 17. Select outputs from the oxygen tank pressure loss scenario.</i></center>

<center><br>
<i>Table 12. Validation matrix for physiological responses due to an oxygen tank pressure loss.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation	|	Tidal Volume (mL)	|	Aorta Oxygen Partial Pressure (mmHg)	|	Aorta Carbon Dioxide Partial Pressure (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer succinylcholine.	|	Neuromuscular blocker agent is injected and patient stops breathing.	|	30	|	60	|<span class="success">	Goes to Zero @cite Morgan2006Clinical                         Goes to Zero @cite dukeSME    	</span>|<span class="success">	Begins to drop according to desaturation curve 	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical	</span>|<span class="success">	Begins to drop w/o respiration 	</span>|<span class="success">	Begins to increase w/o respiration 	</span>|
|	Turn on Anesthesia Machine.	|	Place mask. Use oxygen bottle tank as oxygen source.	|	60	|	180	|<span class="success">	16 from machine setting 	</span>|<span class="success">	Begins to increase with return of respiration 	</span>|<span class="warning">	Patient Normal Volume 	</span>|<span class="success">	Begins to increase with respiration 	</span>|<span class="success">	Begins to decrease with respiration 	</span>|
|	Turn on oxygen tank pressure loss.	|		|	180	|	210	|<span class="success">	No Change 	</span>|<span class="success">	Begins to drop according to desaturation curve @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease with lack of oxygen @cite Morgan2006Clinical	</span>|<span class="success">	No significant change 	</span>|
|	Turn off oxygen tank pressure loss.	|		|	210	|	246	|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with return of oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with return of oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No significant change 	</span>|
|	Oxygen take runs out.	|		|	246	|	270	|<span class="success">	No Change 	</span>|<span class="success">	Begins to drop according to desaturation curve @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to decrease with lack of oxygen @cite Morgan2006Clinical	</span>|<span class="success">	No significant change 	</span>|
|	Replace oxygen tank.	|	Use the second bottle.	|	270	|	390	|<span class="success">	No Change 	</span>|<span class="success">	Continues to increase with oxygen supply @cite Morgan2006Clinical	</span>|<span class="success">	No Change 	</span>|<span class="success">	Begins to increase with return of oxygen supply @cite Morgan2006Clinical	</span>|<span class="warning">	No significant change 	</span>|


@anchor anesthesia-conclusion
## Conclusion

The %BioGears Anesthesia Machine provides a generic representation of a complex and specialized system.  The model predicts physiological response to mechanical ventilation, inhaled agent administration, and common equipment 
failures.  This system is a strong addition to the %BioGears Engine with the potential for 
future development. This system has previously been used to power the training application 
HumanSim: Sedation and Airway, which
was funded by TATRC under contract number W81XWH-11-C-0045.

@anchor anesthesia-improvements
# Future Work

## Coming Soon

There are no planned near term additions.

## Recommended Improvements

The %BioGears Engine modularity could be taken advantage of to add parameters and elements 
for specific equipment models and manufacturers. 

Including  bag squeeze capability to
allow the use of manual ventilation as an additional option will expand 
the functionality of the %BioGears Anesthesia Machine system.

Further functionality to the %BioGears Anesthesia Machine system includes the 
implementation of  synchronized ventilation
that allows spontaneous breathing along with positive-pressure ventilation.
@anchor anesthesia-appendices
# Appendices

## Data Model Implementation

@ref AnesthesiaMachineTable "Anesthesia Machine"

## Acronyms

ETT - Endotracheal tube

## Compartments

* AnesthesiaConnection
* ExpiratoryLimb
* GasInlet
* GasSource
* InspiratoryLimb
* ReliefValve
* Scrubber
* Selector
* Squeeze
* Ventilator
* VentilatorConnection
* YPiece
