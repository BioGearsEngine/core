System Methodology {#SystemMethodology}
==================

# Overview

## Abstract

The %BioGears Engine is a fully customizable collection of physiologic
systems. It includes models for anatomy-based biological systems and
medical equipment. We developed an advanced engine through a
top-down approach, with the ability to easily increase fidelity and
manipulate system models. It can be integrated with other software and
hardware platforms and devices.

## Introduction

Computational modeling and analysis of the human physiology is extremely
useful for various applications. There are many approaches for simulating the body's response to various stimuli. Some existing
software uses state-based calculation that can only provide a limited
amount of data, by way of preprogrammed responses. Complex interactive
effects are difficult with this approach because it uses additive or
purely multiplicative responses.

Some existing high fidelity models use more complex computational fluid
dynamics (CFD). One major drawback of this approach is the significant computation resources required. Cellular, molecular, and genetic models are difficult
for whole body physiology. Using this low-level individual cell and
receptor methodology often does not provide the level of data a
clinician requires for informed decision-making. Physiologically-based
pharmacokinetic (PBPK) modeling focuses only on drug interactions, and
is generally designed to deal with specific pathophysiology.

%BioGears is designed to be a deterministic multiscale modeling application that can be
integrated with all other types of physiologic software. Models at
any anatomical level can be implemented within the existing
infrastructure.

@image html SystemFidelity.png
<center>
*Figure 1. %BioGears uses a top-down approach to model development with bottom-up hooks for expansion.*
</center><br>

# System Design

## Background and Scope

### Physiology Engine

The %BioGears Physiology Engine is the combination of lumped parameter
mathematical models that, together, simulate whole body physiology. Each
system contributes to the maintenance of homeostasis. The systems are
designed to be modular and each has the ability to work in
isolation. They are all reliant on the Common Data Model (CDM) connections and leverage
several generic solvers and transport algorithms.

The system as a whole needs to be easy to use and modify, modular, and
extensible to virtually any fidelities. Envisioned end user groups
include game developers, mannequin builders, educators, researchers,
sensor system developers, and trainers. Several of these users implement
%BioGears in a way that requires faster than real time processing without
access to computational resources that exceed a personal computer.

### Sub-Systems

There are two different types of systems defined in %BioGears - physiology-based and equipment-based.
Below is a list of the current %BioGears Systems with links to their methodology reports:

@anchor system-systems
Patient Variablity:
- @ref PatientMethodology

Physiologic:
- @ref BloodChemistryMethodology
- @ref CardiovascularMethodology
- @ref DrugsMethodology
- @ref EndocrineMethodology
- @ref EnergyMethodology
- @ref EnvironmentMethodology
- @ref GastrointestinalMethodology
- @ref NervousMethodology
- @ref RenalMethodology
- @ref RespiratoryMethodology
- @ref TissueMethodology
- @ref HepaticMethodology

Equipment:
- @ref AnesthesiaMachineMethodology
- @ref InhalerMethodology
- Electrocardiogram (%ECG) Methodology (see @ref CardiovascularMethodology)

### Solvers

The %BioGears suite of also comes with verified tools for extensibility and elimination of potential error sources:
- @ref CircuitMethodology
- @ref SubstanceTransportMethodology

## Data Flow

%BioGears uses differential equations with control-based feedback mechanisms to
dynamically respond to parameter changes. It is designed and tuned
around a stable resting physiologic state, with the ability to
model pathophysiology through insults and intervention action calls.
Many of the system circuit elements are modified based on substance
concentrations and external action (i.e., insult and intervention)
modifiers.

The Anesthesia Machine is directly connected to the %Respiratory System
through an inter-circuit connection. The current implementation creates
a combined circuit for analysis; therefore, the combined circuit is calculated as one
large circuit with a single fluid. %Substances can be administered by
inhalation directly to the %Respiratory System or through a connected
Anesthesia Machine through the vaporizer component.

The %Respiratory System transfers substances back and forth to the
%Cardiovascular System using Alveoli Transfer. The only direct connections
between the two systems are simple modifiers to the heart rate and a
cardiovascular resistance value during the tension pneumothorax insult.
The %Cardiovascular System is also tied to the %Endocrine System and %ECG.
%Substances can enter directly into the %Cardiovascular System through
bolus injections or the administration of an IV. 

Interactions between systems, such as alveoli transfer and diffusion between the extravascular and vascular space are modeled in the System Interactions methodology. This ensures each system is responsible for only its own behavior while capturing the behavior that occurs between systems.

<img src="./images/System/SystemFlow.png" width="600">
<center>
*Figure 2. Overall %BioGears Engine data flow diagram showing all of the
systems. Dashed lines are only present for certain actions.
Non-italicized elements are defined as classes in the engine.*
</center><br>

### %Timing

The entire %BioGears Engine works off of a transient analysis time step
of 0.02 s (50 Hz). All system states are recalculated every time step.
The differential equations that determine changes are linearized to
approximate system variables, but the small time step provides a very
accurate solution.

To provide the %BioGears systems with sufficient information, the CDM
maintains three time steps for elements and parameters. These times are:

-   Baseline - The original resting physiology set-point. Baseline
    values are often assigned by the patient parameters. These values
    generally only change hen conditions are applied.

-   Current - The time step that has already been calculated. The
    current values are used to determine the values for the next time
    step elements based on system feedback mechanisms.

-   Next - The next values are what will be used to calculate the system
    state on the next process call.

@anchor system-stabilization	
### Stabilization

The %BioGears Engine stabilizes with a multi-step process. The engine must be initialized and reach a stable state prior to modifying the patient condition. This is completed by using a dynamic stabilization protocol to execute the engine until a specified set of criteria are met, then any patient chronic conditions are applied. These conditions modify patient parameters and model values to represent the new patient state. The engine must restabilize using the dynamic protocol to achieve a stable state. This process is outlined in Figure 3.

<center><img src="./images/System/Stabilization.png" width="550"></center>
<center> 
<i>Figure 3. Overall %BioGears Engine stabilization protocol. This highlights the multi-step process required to initialize either a healthy or chronically ill patient prior to executing a scenario.</i>
</center><br>

#### Dynamic Stabilization

The %BioGears Engine must perform numerous cycles of the calculations to reach a point of
convergence for the output values, such as heart rate, tidal volume, systolic pressure, etc. When the engine has reached this convergence point, it is considered to be 
stable. During the stabilization period, the majority of feedback in the engine is inactive. An exception is the Tune Circuit methodology found in the %Cardiovascular System. This modifies the cardiovascular circuit parameters to achieve the mean arterial pressure specified in each patient file. For more information on this function, see the @ref CardiovascularMethodology. No actions or conditions
can be applied during the stabilization time. Because the patient parameters may vary in the patient file,
the time required for the solution to converge may vary. By using a dynamic stabilization algorithm, the engine
will be fully initialized and the solution will have converged at the conclusion of the stabilization period
for any patient. This stabilization algorithm also prevents an unnecessarily long time for a solution that may be required in the time that were 
statically specified for all scenarios and patients.

To determine the convergence time, %BioGears uses a dynamic stabilization algorithm. The stable outputs
were analyzed to determine the variation present in each individual output over time. This percent
variation was identified for the key outputs in each system. The percent difference between the
current time step and the next time step is calculated for output. The percent difference calculated
must be less than the identified convergence criteria for the output. This must be maintained for
the convergence time. This time is specified at approximately seven respiratory cycles 
(at 16 breath/min -> ~25 seconds). The respiratory cycle was chosen because it is the has the longest period. 
Convergence should occur over the longest periodic cycle in the engine to ensure variation is not at a low frequency.

Currently, the convergence criteria and time are specified in the CDM. In the future, the algorithm
implementation will remain in the CDM, but the convergence criteria (including time) will be moved to an XML file, similar
to patient and substance files, for easy manipulation by the user (see @ref PatientData and @ref SubstanceData for more information). 

#### Conditions

Patient conditions that are persistent or recurring are chronic conditions. The human body responds differently to chronic conditions
than it does to acute conditions. While the body is in an altered state, which may be weaker, the body's response to the condition becomes damped over time.
In short, the body reaches a new level of homeostasis that may not be considered healthy, but does not have the same continual
feedback mechanisms operating that occur when acute conditions cause a deviation from homeostasis. 

To implement these conditions in the %BioGears Engine, a two step stabilization process was implemented.
As discussed above, the dynamic stabilization criteria was required to ensure all outputs converged prior to
performing any actions (insults or intervention) in a given patient. This is true for conditions, as well. However,
after a condition is applied the solution must again converge to represent the body's new homeostatic state, that while different
from the healthy homeostasis, is still a stable patient state. 

The same convergence methodology is used for this second convergence. However, a set of convergence criteria for the outputs
must be specified for each condition. This criteria reflects the new variation in outputs over time that may exist during the condition.
Examples of these conditions are anemia, pericardial effusion, and arrhythmias. More information on these can be found in the
@ref CardiovascularMethodology.

There is no limit on the number of conditions that can be applied at one time. However, testing an validation of outputs when combining conditions has not been performed.  In the future, when a user applies multiple conditions, a merging algorithm will be used to combine the convergence criteria. This will specify the least stringent requirements as the final convergence criteria for the combined conditions.

### Preprocess

Preprocess is called for each system individually and is generally used
to modify the circuit elements based on feedback mechanisms and actions.
This is also where any system drivers are determined for the upcoming
time step, such as the heart contractility or ventilatory drivers.

At the start of every Preprocess step, the next time step circuit
element values are initialized to the stored baseline values. Generally
speaking, elements are updated by getting the next value and using
multipliers before setting it again. This allows for the "stacking" of
modifiers and the ability to alter the same element based on any number
of mechanisms.

### Process

Process generally determines the entire next time step circuit state. The
generic circuit solver is leveraged to automatically calculate unknown
circuit variables. %Substances are also transported throughout and
between systems with general equations - usually based on path flows or
absorption and diffusion coefficients and renal and hepatic clearance factors.

### Postprocess

Postprocess advances time by moving the next time step values to the
current time step values. The next values are then set to the baseline values in
preparation for the upcoming Preprocess call.

@image html SystemDataFlow.png
<center>
*Figure 4. This shows the repetitive three-step process used each time
step to determine the system states. These three processes are mirrored
in each system's code and are sequentially called by the %BioGears
Engine.*
</center><br>

### Interface

Externally available data is defined within the %BioGears Engine in three major ways:
-	System data
	-	Similar to vitals
	-	Set individually within the system classes at each time step
	-	Example: heart rate
-	Compartment data
	-	Anatomical definitions
	-	Mapped/assigned nodes and paths from which to draw information
	-	Theoretically encompasses any number of circuit nodes and paths
	-	Example: trachea oxygen partial pressure
-	Assessments
	-	Data collected and packaged to resemble a report or analysis that might be ordered by a physician
	-	Intended to give general patient overviews
	-	Calculated on demand
	-	Example: pulmonary function test

The %BioGears modeling approach takes the human body and conceptually divides it into various fluid compartments that represents a real division in terms of how portions of the body's water, solutes, and suspended elements are segregated @cite rhoades2012medical.  Compartments can be further discretized into smaller sub-compartments with a hierarchical relationship as you drill into various systems. In %BioGears, compartments can be defined to encapsulate circuit nodes that allow easy organization, access, and synchronization of all system parts.
	
Compartments are implemented in %BioGears as conceptual physical divisions of the body.  Anatomical data can be pulled from each compartment through optional node and path mapping. Compartments can be further discretized into smaller sub-compartments with a hierarchical relationship as you drill into various systems. In %BioGears, compartments can be defined to encapsulate circuit nodes that allow easy organization, access, and synchronization of all system parts. Figure 5 shows an example of how compartments can be defined in the %Cardiovascular System.
	
@image html CompartmentExample.png
<center>
<i>Figure 5. This is an example of possible %Cardiovascular System compartments.  This is for explanation purposes only and not necessarily indicative of how things are really defined.  See the @ref CardiovascularMethodology documentation for how they are really defined.</i>
</center><br>

Users can customize scenarios for %BioGears by modifying a variety of file types. For example, patients, substances, compound substances, and environments can all be modified
via the text files provided. This allows users to customize scenarios by tailoring patients and environmental conditions to the specific circumstances of the user. 
As an example of this flexibility, %Biogears provides a variety of patient files. These parameters in these patient files can be specified to represent different physiologic states for a patient. 
See @ref PatientMethodology for details.

For more details on interfacing with the %BioGears Engine, see the @ref engine documentation.

## Assumptions and Limitations

%BioGears uses a low fidelity clinical approach and is not necessarily
for high fidelity predictive purposes. It is also assumed to be contained
within the CDM.

See the individual system methodology documents for more specifics.

# Results and Conclusions

## Combined Effects Validation

Several scenarios have been developed and validated to ensure proper interaction not only within, but also between the %BioGears physiologic systems. These patients and scenarios were developed as part of the HumanSim: Sedation and Airway project funded by TATRC, contract number W81XWH-11-C-0045. The combined effects scenarios were validated qualitatively from available literature and from the opinions of subject matter experts (SME).  More detail on patient parameters can be found in the @ref PatientMethodology documentation. A summary of the validation is shown in Table 6.  Green indicates good agreement with validation data, 
yellow indicates agreement with a general trend with some minor disagreement, and red indicates a disagreement with the validation data. The number indicates the number of output parameters for each category of validation
success or failure.

<center>
*Table 6. Validation scenarios were completed for five patients subjected to a variety of insults and intervention: Cynthia, Gus, Hassan, Joel, and Nathan. The resulting effects of each individual insult and intervention were validated both qualitatively and from subject matter experts. Green indicates good agreement with validation data, yellow indicates agreement with a general trend with some minor disagreement, and red indicates a disagreement with the validation data. The number indicates the number of output parameters for each category of validation success or failure.*
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	Cynthia	|	Midazolam is injected causing an airway obstruction. Oxygen is supplied via a mask. Ketamine is administered and the airway obstruction is cleared. Rocuronium is administered. An endotracheal tube is set.	|<span class="success">	28	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Hassan	|	Oxygen is administered via a mask. Ketamine and succinylcholine are administered and the mask is removed. Right mainstem intubation occurs. The tube placement is corrected for a successful endotracheal intubation.	|<span class="success">	25	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Nathan	|	Nathan is injected with Fentanyl. Oxygen is administered via a mask. An oxygen wall port pressure loss occurs. The oxygen bottle is attached to supply oxygen.	|<span class="success">	19	</span>|<span class="warning">	0	</span>|<span class="danger">	1	</span>|
|	Gus	|	A ventilator mask is placed on Gus and oxygen is administered via a mask. Succinylcholine is injected and the mask is removed. A failed endotracheal intubation occurs as an esophageal intubation. Endotracheal intubation then occurs.	|<span class="success">	18	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Joel	|	Airway obstruction occurs at the beginning of the scenario. the obstruction is removed, and oxygen is supplied via a mask. Etomidate and succinylcholine are administered, and the mask is removed. An endotracheal tube is set.	|<span class="success">	23	</span>|<span class="warning">	2	</span>|<span class="danger">	5	</span>|
|		|	Total	|<span class="success">	113	</span>|<span class="warning">	6	</span>|<span class="danger">	6	</span>|

Although the tidal volume is not shown for these scenarios, it should be noted that mild to moderate discrepancies between the tidal volume set on the anesthesia machine and that observed for the patient were observed in %BioGears. This is also a common observation in clinical practice, because the anesthesia machine may have to overcome the patient condition and airway resistance to provide sufficient tidal volume @cite Morgan2006Clinical .

### Cynthia

The Cynthia scenario begins with the administration of midazolam at 50&nbsp;seconds with a full severity airway obstruction occurring at 110&nbsp;seconds. At 260&nbsp;seconds a ventilator mask is placed on Cynthia. Twenty seconds later, the airway obstruction is removed and 30&nbsp;milligrams of ketamine are administered via a bolus injection. Rocuronium is administered one minute later, and Cynthia is intubated 40&nbsp;seconds after that. The Cynthia scenario shows excellent agreement with the qualitative and expected SME trends. However, there is a minor inconsistency in the systolic and diastolic pressure decrease following the administration of midazolam.  The SME validation predicted a 15-25% decrease in systolic/diastolic pressures following the administration; however, the observed decrease was approximately 10%. This decrease was considered acceptable since other references predicted varying degrees of pressure decrease.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/Cynthia_HR.jpg" width="550"></td>
    <td><img src="./plots/System/Cynthia_ArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/Cynthia_RR.jpg" width="550"></td>
    <td><img src="./plots/System/Cynthia_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/CynthiaLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 6. Select outputs from the Cynthia combined effects scenario.</i></center>

<center><br>
*Table 7. The Cynthia scenario displays the effects of sequential midazolam administration, airway obstruction, ventilator mask, ketamine and rocuronium administration, and intubation. This scenario shows some good agreement with the expected qualitative and SME trends.*
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Midazolam - 3 mg	|	Drug Onset in 1-3 min	|	30	|	90	|<span class="success">	Mild Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical; 15-25% Decrease @cite dukeSME    	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical; 15-25% Decrease @cite dukeSME    	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical; Marked Decrease to 0 @cite dukeSME	</span>|<span class="success">	As long as some respiration rate, little change. If RR=0, then begins to drop along O2 curve. 	</span>|
|	Airway Obstruction of Severity 1.0	|	Represents an aspiration	|	90	|	240	|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	 NC @cite dukeSME	</span>|<span class="success">	Drop according to preoxygenated O2 curve  	</span>|
|	Apply Mask and Turn on Ventilator	|		|	240	|	250	|<span class="danger">	If above increased, Decrease @cite dukeSME	</span>|<span class="warning">	If above increased, Decrease @cite dukeSME	</span>|<span class="warning">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	Ventilator On; but complete block, so NC  	</span>|<span class="success">	Continues to decrease because still fully blocked  	</span>|
|	Administer Ketamine - 30 mg/ End of Airway Obstruction	|	Drug Onset < 2 minutes	|	250	|	310	|<span class="danger">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="success">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="success">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="warning">	Ventilator On; therefore, 16  	</span>|<span class="success">	Begins to increase because airway obstruction is cleared  	</span>|
|	Administer Rocuronium - 62mg	|	Drug Onset in 60-90 seconds	|	310	|	400	|<span class="success">	NC @cite PaulGBarash2009; NC @cite dukeSME     	</span>|<span class="success">	NC @cite PaulGBarash2009; NC @cite dukeSME     	</span>|<span class="success">	NC @cite PaulGBarash2009; NC @cite dukeSME     	</span>|<span class="success">	Ventilator On; therefore, 16  	</span>|<span class="success">	Continues to increase 	</span>|
|	Successful Intubation	|		|	400	|	900	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|


### Gus
A ventilator mask is applied to Gus at 50&nbsp;seconds, and succinylcholine is injected one minute later. After an additional minute, an unsuccessful intubation occurs, leading to the endotracheal tube residing within the esophagus. After two minutes the tube is removed and correctly set in the trachea. The produced results show excellent agreement with the expected validation trends. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/Gus_HR.jpg" width="550"></td>
    <td><img src="./plots/System/Gus_ArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/Gus_RR.jpg" width="550"></td>
    <td><img src="./plots/System/Gus_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/GusLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 7. Select outputs from the Gus combined effects scenario.</i></center>

<center><br>
*Table 8. The Gus scenario displays the effects of sequential ventilator mask application, succinylcholine injection, esophageal intubation and then a successful endotracheal intubation. This scenario shows good agreement with the SME and qualitative validation.*
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Mask Ventilate with Oxygen	|		|	30	|	90	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	Slight decrease to due anesthesia machine resistance ; NC @cite dukeSME	</span>|<span class="success">	100	</span>|
|	Administer Succinycholine - 100mg	|	Drug Onset < 1 minute	|	90	|	150	|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical; Goes to Zero @cite dukeSME     	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	Esophageal Intubation	|		|	150	|	270	|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="danger">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	Ventilator rate is 16  	</span>|<span class="success">	May slowly decrease along O2 curve @cite dukeSME	</span>|
|	Successful Intubation	|		|	270	|	770	|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="danger">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	16	</span>|<span class="success">	Begins to increase to a normal level @cite dukeSME	</span>|


### Hassan

At the beginning of the scenario, a ventilator mask is applied to Hassan. He then receives bolus injections of ketamine and succinylcholine. This leads to an increase in the heart rate and arterial pressures due to the ketamine injection. The respiration rate begins to decrease as the patient begins to lose consciousness. After the succinylcholine injection, there is a decrease in the heart rate and arterial pressures. In addition, the respiration rate falls to zero due to the neuromuscular block. At 180&nbsp;seconds, an endotracheal tube is set into the right bronchi, leading to only one lung being ventilated. This leads to a minor reduction in the oxygen saturation. Due to the reduced oxygen intake, the heart rate and arterial pressures begin to increase to compensate. The tube is reset into the trachea and the vital signs begin to return to normal. All of these trends follow the expected validation trends.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/Hassan_HR.jpg" width="550"></td>
    <td><img src="./plots/System/Hassan_ArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/Hassan_RR.jpg" width="550"></td>
    <td><img src="./plots/System/Hassan_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/HassanLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 8. Select outputs from the Hassan combined effects scenario.</i></center>

<center><br>
*Table 9. Hassan displays the effects of a right mainstem intubation after the patient has been injected with ketamine and succinylcholine. The engine output shows complete agreement with the validation trends.*
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Mask Ventilate with Oxygen	|		|	30	|	90	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	Slight decrease to due anesthesia machine resistance ; NC @cite dukeSME	</span>|<span class="success">	100 @cite dukeSME	</span>|
|	Administer Ketamine - 168 mg	|	Drug Onset < 1 minute	|	90	|	130	|<span class="success">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="success">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="success">	Moderate Increase @cite Morgan2006Clinical; 15-25% Increase @cite dukeSME    	</span>|<span class="success">	Mild Decrease @cite Morgan2006Clinical; 25-50% Decrease @cite dukeSME    	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	Administer Succinycholine - 115mg	|	Drug Onset < 1 minute	|	130	|	160	|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Ventilator is on; therefore, 16    	</span>|<span class="success">	Begins to drop according to preoxygenated O2 curve @cite dukeSME 	</span>|
|	Right Mainstem Intubation	|	Intent is to simulate tube migration	|	160	|	250	|<span class="warning">	NC, If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	NC, If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	NC, If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	16	</span>|<span class="success">	May slowly decrease along O2 curve @cite dukeSME	</span>|
|	Correct tube placement	|	Reset the intubation tube	|	250	|	450	|<span class="success">	NC, If above result in a Mild Increase, will return to normal @cite dukeSME	</span>|<span class="success">	NC, If above result in a Mild Increase, will return to normal @cite dukeSME	</span>|<span class="warning">	NC, If above result in a Mild Increase, will return to normal @cite dukeSME	</span>|<span class="success">	16	</span>|<span class="success">	Begins to increase to a normal level >97% @cite dukeSME	</span>|


### Joel
The Joel scenario begins with a full severity airway obstruction at 50&nbsp;seconds. This leads to decreasing oxygen saturation and increasing heart rate and arterial pressures due to the sympathetic (endocrine) response. At 170&nbsp;seconds, the obstruction is removed and the vital signs begin to return to normal. A ventilator mask is applied to Joel at 230&nbsp;seconds, and he receives a 27&nbsp;mg bolus injection of etomidate at 290&nbsp;seconds. This leads to decreasing arterial pressure. At 310&nbsp;seconds, a bolus injection of succinylcholine occurs before an endoctracheal tube is set 30&nbsp;seconds later. The administration of succinylcholine leads to immediate decreases in heart rate, arterial pressures, and oxygen saturation. After the intubation occurs, the oxygen saturation begins to return to normal due to adequate ventilation. All of these results show strong agreement with the subject matter expert's expected trends.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/Joel_HR.jpg" width="550"></td>
    <td><img src="./plots/System/Joel_ArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/Joel_RR.jpg" width="550"></td>
    <td><img src="./plots/System/Joel_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/JoelLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 9. Select outputs from the Joel combined effects scenario.</i></center>

<center><br>
*Table 10. The Joel scenario displays the effects of sequential airway obstruction, ventilator mask application, etomidate administration, succinylcholine administration, and intubation. The %BioGears results show excellent agreement with the validation trends.*
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Airway Obstruction of Severity 1.0	|	Represents an aspiration	|	30	|	150	|<span class="success">	NC until O2 drops then increase @cite dukeSME	</span>|<span class="warning">	NC until O2 drops then increase @cite dukeSME	</span>|<span class="success">	NC until O2 drops then increase @cite dukeSME	</span>|<span class="warning">	Goes to Zero @cite dukeSME	</span>|<span class="success">	Begins to drop according to O2 curve @cite dukeSME	</span>|
|	End Airway Obstruction	|	Represents suctioning	|	150	|	210	|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="danger">	Return to resting physiology @cite dukeSME	</span>|<span class="success">	Begins to increase to a normal level >97% @cite dukeSME	</span>|
|	Ventilate w/ O2 tank	|	O2 Source is set to Tank 1	|	210	|	270	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	Adminster Etomidate - 27 mg	|	No Etomidate in drug directory, so we use Katamine instead; Drug Onset < 1 minute	|	270	|	290	|<span class="success">	NC @cite Morgan2006Clinical; NC @cite dukeSME     	</span>|<span class="danger">	Mild  Decrease @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME     	</span>|<span class="danger">	Mild  Decrease @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME     	</span>|<span class="danger">	Mask Ventilation, so NC  	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	Administer Succinycholine - 115 mg	|	Drug Onset < 1 minute	|	290	|	320	|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME     	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME     	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME     	</span>|<span class="danger">	Mask Ventilation, so NC  	</span>|<span class="success">	Begins to drop according to preoxygenated O2 curve @cite dukeSME 	</span>|
|	Intubate	|		|	320	|	520	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	Begins to increase to a normal level >97% @cite PaulGBarash2009	</span>|



### Nathan
Nathan receives a bolus injection of fentanyl at a dose of 150&nbsp;micrograms at a scenario time of 50&nbsp;seconds, and a ventilator mask is placed at 140&nbsp;seconds. Due to the administration of fentanyl, there is an observed decrease in the heart rate, arterial pressures and respiration rate. There is very good agreement with the expected trends during this time period. The The respiration rate is expected to decrease by 15-25%; however, the observed increase is approximately 10%. This was acceptable due to the trend expressed in other validation resources. The oxygen wall connection loses pressure at 230&nbsp;seconds. This leads to decreasing oxygen saturation and increasing heart rate and arterial pressures due to the epinephrine response. Following this, the connection is reset to a secondary oxygen tank, and the vital signs return to normal. This behavior matches the validation trends.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/Nathan_HR.jpg" width="550"></td>
    <td><img src="./plots/System/Nathan_ArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/Nathan_RR.jpg" width="550"></td>
    <td><img src="./plots/System/Nathan_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/NathanLegend.jpg" width="800"></td>
</tr>
</table>
</center>
<center><i>Figure 10. Select outputs from the Nathan combined effects scenario.</i></center>

<center><br>
*Table 8. The Nathan scenario displays the effects of sequential bolus fentanyl injection, ventilator mask application, and oxygen wall pressure loss. The engine results show strong agreement with the validation trends.*
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Fentanyl - 150ug 	|	Drug Onset < 2 minutes	|	30	|	120	|<span class="success">	Stable @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME   	</span>|<span class="success">	Stable @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME   	</span>|<span class="success">	Stable @cite Morgan2006Clinical; 5-10% Decrease @cite dukeSME   	</span>|<span class="success">	Decrease @cite Morgan2006Clinical; 15-25% Decrease @cite dukeSME   	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	Apply Mask and Turn on Ventilator	|		|	120	|	210	|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="warning">	Slight decrease to due anesthesia machine resistance    NC @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|
|	O2 Wall Pressure Loss	|		|	210	|	340	|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="success">	If O2 decreases, Increase @cite dukeSME	</span>|<span class="warning">	NC @cite dukeSME	</span>|<span class="success">	If undetected long enough, Decrease @cite dukeSME	</span>|
|	Connect O2 Bottle 1 as O2 Source	|		|	340	|	840	|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	If above increased, Decrease @cite dukeSME	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	If above decreased, Increase @cite dukeSME	</span>|


## Showcase Scenarios Validation

Four scenarios were created to showcase the ability of the %BioGears open-source physiology engine to simulate complex and combinatory insults and interventions. The %BioGears Showcase Scenarios demonstrate the ability of the engine to fill the physiology simulation needs of the medical simulation community.

### Combat Multitrauma

A team of soldiers is conducting a patrol when an explosive device detonates, injuring one of the soldiers. The squad medic applies direct pressure to a hemorrhaging wound, and, suspecting a tension pneumothorax, performs a needle decompression. Direct pressure is not controlling the bleeding, so a tourniquet is applied. Morphine and IV fluids are administered.

See <a href="CombatMultitraumaValidation.pdf">Combat Multitrauma Validation Matrix</a> for complete details on validation.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/CombatMultitrauma_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/System/CombatMultitrauma_BloodVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/CombatMultitrauma_HR.jpg" width="550"></td>
    <td><img src="./plots/System/CombatMultitrauma_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/CombatMultitraumaLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 11. Select outputs from the Combat Multitrauma showcase scenario.</i></center>

### Asthma Attack

A 40 year old female with a history of asthma is having an asthma attack. She arrives at the hospital ten minutes after the beginning of the attack. A doctor administers albuterol and her condition improves.

See <a href="AsthmaAttackValidation.pdf">Asthma Attack Validation Matrix</a> for complete details on validation.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/AsthmaAttack_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/System/AsthmaAttack_TidalVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/AsthmaAttack_HR.jpg" width="550"></td>
    <td><img src="./plots/System/AsthmaAttack_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/AsthmaAttackLegend.jpg" width="700"></td>
</tr>
</table>
</center>
<center><i>Figure 12. Select outputs from the Asthma Attack showcase scenario.</i></center>

### Heat Stroke

A 25 year old male is hiking towards a rock formation to begin a recreational free climb. During the hike, he is working at ~10% of his maximum capacity. The man arrives at a rock formation and begins climbing at an intensity of ~1/2 of his maximum capacity. At the top of the rock formation, the man becomes dizzy and passes out. An off-duty medic takes action to treat a heat injury.

See <a href="HeatStrokeValidation.pdf">Heat Stroke Validation Matrix</a> for complete details on validation.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/HeatStroke_AchievedExerciseLevel.jpg" width="550"></td>
    <td><img src="./plots/System/HeatStroke_CoreTemp.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/HeatStroke_HR.jpg" width="550"></td>
    <td><img src="./plots/System/HeatStroke_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/HeatStrokeLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 13. Select outputs from the Heat Stroke showcase scenario.</i></center>

### %Environment Exposure

A 17 year old female leaves her Alaskan home in the mid-winter to retrieve a newspaper. The door closes as she exits the house. She is stuck outside for 45 minutes where it is -10<sup>o</sup>C. When the woman's housemates realize that she is outside, they bring her back in and sit her next to a fire.

See <a href="EnvironmentExposureValidation.pdf">Environment Exposure Validation Matrix</a> for complete details on validation.

<center>
<table border="0">
<tr>
    <td><img src="./plots/System/EnvironmentExposure_TotalMetabolicRate.jpg" width="550"></td>
    <td><img src="./plots/System/EnvironmentExposure_O2Consumption.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/System/EnvironmentExposure_HR.jpg" width="550"></td>
    <td><img src="./plots/System/EnvironmentExposure_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/System/EnvironmentExposureLegend.jpg" width="900"></td>
</tr>
</table>
</center>
<center><i>Figure 14. Select outputs from the %Environment Exposure showcase scenario.</i></center>

## Conclusion

The %BioGears Engine has successfully leveraged the CDM to model and
simulate whole body physiology. The interaction of the existing system
models was validated by looking at the combined effects of multiple actions on a variety of patients. Virtually
everything within the engine is modular and extensible. Fidelity can be
changed at any level within the system. %BioGears is a fully standalone
simulator, but can also implement or interface with external software.

# Future Work

We will continue to improve system modularity.  See the system specific methodology documents for more information:

@secreflist
  @refitem anesthesia-improvements "Anesthesia Machine Future Work"
  @refitem bloodchemistry-future "BloodChemistry Future Work"
  @refitem cardiovascular-future "Cardiovascular Future Work"
  @refitem drugs-future "Drugs Future Work"
  @refitem endrocrine-comingsoon "Endocrine Future Work"
  @refitem energy-future "Energy Future Work"
  @refitem environment-future "Environment Future Work"
  @refitem GI-futurework "Gastrointestinal Future Work"
  @refitem hepatic-comingsoon "Hepatic Future Work"
  @refitem inhaler-future "Inhaler Future Work"
  @refitem nervous-future "Nervous Future Work"  
  @refitem renal-futurework "Renal Future Work"
  @refitem respiratory-future "Respiratory Future Work"
  @refitem tissue-future "Tissue Future Work"
@endsecreflist

## Recommended Improvements

An area of potential future advancements for the %BioGears Engine includes the integration of differing time step and un-fixed time
step models. This could be achieved through simple
interpolation and down-sampling, where applicable. Other advanced system
dynamics, adaptive step sizes, and mixed fidelity techniques could also be investigated.

Investigations into the effects of biological variability and parameter (inputs and outputs) error bound and confidence intervals could be applied throughout.

# Appendices

## Data Model Implementation

BioGears

BioGearsConfiguration

BioGearsEngine

## Acronyms

CDM - Common Data Model

%ECG - Electrocardiography

CFD - Computational Fluid Dynamics

PBPK - Physiologically-Based Pharmacokinetic

SME - Subject Matter Expert
