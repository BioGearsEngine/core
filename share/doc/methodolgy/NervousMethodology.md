%Nervous Methodology {#NervousMethodology}
==========================

@anchor nervous-overview
Overview
========
Abstract
--------
The %BioGears %Nervous System is a preliminary system representing the central %nervous system (CNS) and peripheral %nervous system (PNS). Currently, the system includes baroreceptor and chemoreceptor feedback models that are used to regulate arterial pressure and blood-gas levels.  Additionally, the %Nervous System provides for tracking of parameters related to traumatic brain injury events, pain stimulus response, and drug effects.  In the future, other feedback mechanisms will be added for improved modeling of homeostatic and crisis states.

@anchor nervous-introduction
Introduction
------------
The %Nervous System in the human body is comprised of the central nervous system (CNS) and the the peripheral %nervous system (PNS). The CNS contains the brain and the spinal cord, while the PNS is subdivided into the somatic, autonomic, and enteric nervous systems. The somatic system controls voluntary movement. The autonomic nervous system is further subdivided into the parasympathetic and sympathetic nervous system. The sympathetic nervous system is activated in times of stress and crisis, commonly referred to as the "flight or fight" behaviors. The parasympathetic nervous system controls homeostasis functions during the relaxed state. The enteric nervous system controls the gastrointestinal system. Many of these behaviors are tightly connected to the %Endocrine System. In many cases, the %Nervous System receptors trigger hormone releases that cause systemic changes, such as dilation and constriction of vessels, heart contractility changes, and respiration changes @cite guyton2006medical. 

%BioGears will focus on a select few mechanisms within the %Nervous System to provide feedback for both homeostasis and crisis behavior in the body. The list is shown below:
 - Baroreceptors
 - Chemoreceptors
 - Local autoregulation
 - Temperature Effects
 - Generic parasympathetic and sympathetic effects
 - Pain stimulus

 Additionally, %BioGears will model the following features related to brain function: 
 - Simple model of traumatic brain injury (TBI)
 - Intracranial pressure
 - Pupillary response to drugs and TBI

@anchor nervous-system-design
System Design
=============
Background and Scope
--------------------
The %BioGears Nervous System contains 


Data Flow
---------
An overview of the data flow in the %BioGears %Nervous system is shown in Figure 1.

### Initialization and Stabilization
The %BioGears initialization and stabilization is described in detail in the [stabilization section](@ref system-stabilization) of the @ref SystemMethodology report. The mean arterial pressure set-point is updated after the %Cardiovascular system reaches a homeostatic state.

### Preprocess
#### Baroreceptor Feedback
The baroreceptor mechanism provides rapid negative feedback control of arterial pressure. A drop in arterial pressure is sensed by the baroreceptors and leads to an increase in heart rate, heart elastance, and vessel distensibility. These changes operate with the goal of maintaining arterial pressure at its healthy resting level. The baroreceptor mechanism may be divided into three parts: afferent, CNS, and efferent. The afferent part contains the receptors, which detect changes in the MAP. After that, the CNS portions the response into either sympathetic or parasympathetic. Lastly, the efferent part is used to provide feedback to the vasculature and organs within the %Cardiovascular System @cite ottesen2004applied. The model chosen for %BioGears only models the CNS and efferent portion of the baroreceptors. The fidelity of the model does require the modeling of the actual stretch receptors in the aorta or carotid arteries to provide the necessary feedback.

#### Chemoreceptor Feedback
The chemoreceptor mechanism provides feedback control to regulate the partial pressures of oxygen and carbon dioxide in the blood.  The response is divided in to two components:  peripheral and central.  Peripheral chemoreceptors (which reside in the aortic arch and carotid arteries) are sensitive to deviations in both oxygen and carbon dioxide pressures.  The central chemoreptors, so named because of their location in the central nervous system, respond only the carbon dioxide pressure changes.  Assuming that each receptor class operates independently, the feedback from the central and peripheral chemoreceptors is summed to generate a respiratory and a cardiovascular response.  The respiratory response sets the patient ventilation (which effects both the tidal volume and respiration rate) to restore blood gas levels to their set points.  Likewise, a cardiovascular response is initiated by altering the patient heart rate.

#### Check Pain Stimulus
Pain can be initiated in BioGears via a Pain Stimulus action.  The BioGears pain response takes into account both the magnitude of the pain action and the susceptibility of the virtual patient to pain (set in the Patient definition file).  The result is an output  corresponding to the pain Visual Analog Scale (VAS) on a 0-10 interval.  The VAS score dictates the patient cardiovascular and respiratory response to the pain stimulus.  Increased epinephrine production is also stimulated.  

### Process
#### Check %Nervous Status
Events associated with nervous system functionality are evaluated.  First, intracranial pressure is checked to determine whether or not to throw the Intracranial Hypertension or Intracranial Hypotension events.  Then, the presence of fasciculation (muscle spasms) is evaluated based on the activity of the compounds Sarin and Succinylcholine.  The latter drug generally produces transient fasciculation which disappear when neuromuscular block is achieved @cite appiah2004pharmacology.  For a description of the neurological effects of Sarin, see the [Nerve Agent: Sarin](@ref sarin-nerve) section in the Drugs documentation.  It should be noted that fasciculation can also be caused by an ion imbalance; however, this functionality is not yet active.  When completed, the criteria for this type of event will be calcium deficiency. If the patient's arterial calcium concentration falls below one milligram per deciliter, then fasciculation will be triggered @cite gropper2013nutrition.

#### SetPupilEffects
Pupil modifiers are retrieved from the %Drugs System and combined with any modifiers from TBI before being applied to the eyes.

### Post Process
There is no system specific functionality for the %Nervous System in Post Process.

@anchor nervous-assessments
### Assessments
Assessments in %BioGears are data collected and packaged to resemble a report or analysis that might be ordered by a physician. No %BioGears assessments are associated with the %Nervous System.

<br>
<img src="./images/Nervous/NervousDataFlow.png" width="800">
<center>
*Figure 1. The data flow for the %Nervous System consists of Preprocess, Process and Postprocess. In Preprocess, the baroreceptor feedback is calculated. In Process, the brain metrics are checked to see if event thresholds have been reached and pupil effects are determined and applied.*
</center><br>

@anchor nervous-features
Features, Capabilities, and Dependencies
----------------------------------------
### Baroreceptors
The baroreceptor model implemented in %BioGears is adapted from the models described by Ottesen et al @cite ottesen2004applied. The model is used to drive the current MAP towards the resting set-point of the patient. This is accomplished by calculating the sympathetic and parasympathetic response. The fraction of the baroreceptor response that corresponds to the sympathetic effects is determined from Equation 1. The parsympathetic fraction is shown in Equation 2.

\f[ \eta_{s} (P_{a}) = \left[ 1+ \left( \frac{P_{a}}{P_{a,setpoint}} \right)^{ \nu} \right]^{-1} \f]
<center>
*Equation 1.*
</center><br>

\f[ \eta_{p} (P_{a}) = \left[ 1+ \left( \frac{P_{a}}{P_{a,setpoint}} \right)^{- \nu} \right]^{-1} \f]
<center>
*Equation 2.*
</center><br>

Where &nu; is a parameter that represents the response slope of the baroreceptors, <b>p</b><sub>a</sub> is the current MAP, and <b>p</b><sub>a,setpoint</sub> is the MAP set-point. An example of the sympathetic and parasympathetic responses as a function of MAP are shown in Figure 1. These were calculated with an assumed MAP set-point of 87 mmHg.

<img src="./plots/Nervous/Response_Fractions.jpg" width="550">
<center>
*Figure 2. The sympathetic and parasympatheric response fractions are displayed as a function of mean arterial pressure (MAP). Both fractional forms show asymptotic behavior as divergence from the MAP set-point occurs. The response fractions are additive, always summing to a value of 1.0. At homeostasis (MAP equal to the set-point), the fractions are both equal to 0.5.*
</center><br>

As described in the [cardiovascular](@ref cardiovascular-initialize) methodology report, the %BioGears %cardiovascular system is initialized according to patient definitions and the stabilized to a homeostatic state. The set-point is the resultant mean arterial pressure following the engine stabilization period. The set-point is adjusted dynamically with certain actions and insults, as shown in Equation 3.

\f[ P^{n+1}_{a,setpoint} = P^{n}_{a,setpoint} + \Delta P_{a,drugs} + \Delta P_{a,exercise} \f]
<center>
*Equation 3.*
</center><br>

Where &Delta;<b>p</b><sub>a,drugs</sub> and &Delta;<b>p</b><sub>a,exercise</sub> are changes in MAP due to drugs and exercise, respectively. 

The sympathetic and parasympathetic fractional responses are used to determine the response of the following %cardiovascular parameters:
- heart rate
- heart elastance
- systemic vascular resistance
- systemic vascular compliance

This is accomplished by tracking the time-dependent values of each parameter relative to their value at the MAP set-point. The time-dependent behavior of these parameters is presented via a set of ordinary differential equations, as shown in Equations 4-7.

\f[ \frac{dx_{HR}}{dt} = \left(- \frac{1}{ \tau_{HR}} \right) \left( -x_{HR} + \alpha_{HR} \eta_{s}(P_{a}) + \beta_{HR} \eta_{p}(P_{a}) + \gamma_{HR} \right) \f]
<center>
*Equation 4.* 
</center><br>

\f[ \frac{dx_{E}}{dt} = \left(- \frac{1}{ \tau_{E}} \right) \left( -x_{E} + \alpha_{E} \eta_{s}(P_{a}) + \gamma_{E} \right) \f]
<center>
*Equation 5.*
</center><br>

\f[ \frac{dx_{R}}{dt} = \left(- \frac{1}{ \tau_{R}} \right) \left( -x_{R} + \alpha_{R} \eta_{s}(P_{a}) + \gamma_{R} \right) \f]
<center>
*Equation 6.*
</center><br>

\f[ \frac{dx_{C}}{dt} = \left(- \frac{1}{ \tau_{C}} \right) \left( -x_{C} + \alpha_{C} \eta_{p}(P_{a}) + \gamma_{C} \right) \f]
<center>
*Equation 7.*
</center><br>

Where x<sub>HR</sub>, x<sub>E</sub>, x<sub>R</sub> and x<sub>C</sub> are the relative values of heart rate, heart elastance, vascular resistance and vascular compliance, respectively. &tau;<sub>HR</sub>, &tau;<sub>E</sub>, &tau;<sub>R</sub> and &tau;<sub>C</sub> are the time constants for heart rate, heart elastance, vascular resistance and vascular compliance, respectively. The remaining &alpha;, &beta; and &gamma; parameters are a set of tuning variables used to achieve the correct responses in the %Cardiovascular System during arterial pressure shifts. Note that the heart rate feedback is a function of both the sympathetic response and parasympathetic response, whereas the elastance feedback and vascular tone feedback depend on the sympathetic or parasympathetic responses individually. Figure 3 shows the normalized response curves.

<table border="0">
<tr>
    <td><img src="./plots/Nervous/Normalized_HeartElastance.jpg" width="550"></td>
    <td><img src="./plots/Nervous/Normalized_HeartRate.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Nervous/Normalized_SystemicCompliance.jpg" width="550"></td>
    <td><img src="./plots/Nervous/Normalized_SystemicResistance.jpg" width="550"></td>
</tr>
</table>
<center> *Figure 3. The plot array demonstrates the normalized organ responses to sympathetic or parasympathetic activity, plotted against the normalized mean arterial pressure.* </center>

### Chemoreceptors
The chemoreceptors are cells that are sensitive to reduced oxygen and excess carbon dioxide. Excitation of the chemoreceptors stimulates the sympathetic nervous system. The chemoreceptors contribute significantly to the control of respiratory function, and they are included in the [respiratory control model](@ref respiratory-chemoreceptors) developed for %BioGears. As sympathetic activators, the chemoreceptors also increase the heart rate and contractility. The complete mechanisms of chemoreceptor feedback are complicated and beyond the current scope of %BioGears, so a phenomenological model was developed to elicit an appropriate response to hypoxia and hypercapnia. Only the heart rate effects of chemoreceptor stimulation are modeled in the current version of %BioGears, but contractility modification will be included in a future release.  Figure 4 shows the chemoreceptor effect on heart rate. In the figure, the abscissa values represent a fractional deviation of gas concentration from baseline, and the ordinate values show the resultant change in heart rate as a fraction of the baseline heart rate. The final heart rate modification due to chemoreceptors is the sum of the oxygen and carbon dioxide effects. For example, severe hypoxia and severe hypercapnia will result in a three-fold increase in heart rate (baseline + 2 * baseline).

<img src="./plots/Nervous/ChemoreceptorsModifiers.jpg" width="600">
<center>
*Figure 4. The %BioGears chemoreceptor model is a phenomenological model which elicits a tuned response to hypoxia and/or hypercapnia. A reverse effect is also present, but at a much lesser magnitude.*
</center><br>

### TBI
Traumatic brain injuries are relatively common, affecting millions annually. They are usually defined as a blunt or penetrating injury to the head that disrupts normal brain function. Furthermore, they are classified as either focal (e.g. cerebral contusions, lacerations, and hematomas) or diffuse (e.g. concussions and diffuse axonal injuries) @cite adoni2007pupillary. The scope of the %BioGears TBI model is somewhat limited by the low fidelity of the modeled brain. The brain is represented in the current %BioGears %Cardiovascular circuit with only two resistors and a compliance, all within one compartment. Because the circuit is modeled in this way, TBI is considered as an acute, non-localized, non-penetrating injury from a circuit perspective. Thus, the TBI model can account for intracranial pressure and cerebral blood flow on the basis of the whole brain, but not to specific areas of the brain. However, %BioGears does provide for three injury inputs: diffuse, right focal, and left focal.  Similarly to the @ref RenalMethodology "Renal System", the %BioGears brain circuit could be expanded to accommodate a higher-fidelity brain model.

Three important metrics are used to evaluate patients with traumatic brain injuries: intracranial pressure (ICP), cerebral blood flow (CBF), and cerebral perfusion pressure (CPP). Patients with brain injuries often exhibit increased intracranial pressure, decreased cerebral blood flow, and a cerebral perfusion pressure outside of a normal range @cite steiner2006monitoring. Cerebral perfusion pressure is defined as

\f[ CPP = MAP - ICP \f]
<center>
*Equation 8.*
</center><br>

 Where MAP is the mean arterial pressure. In order to model these behaviors, the Brain Injury action in %BioGears will modify the resistors of the brain circuit, which is shown in Figure 5 below. The brain circuit is a section of the @ref cardiovascular-features "cardiovascular circuit".

<img src="./images/Nervous/BrainCircuit.png" width="550">
<center>
*Figure 5. The %BioGears brain is represented by two resistors and a compliance. The upstream resistor, R1, is connected to the aorta, and the downstream resistor, R2, is connected to the vena cava.*
</center><br>

By increasing R1 and R2, the ICP can be increased while CBF decreases. The resistors are tuned based on the severity (on a scale from 0 to 1) of TBI such that ICP is above 25 mmHg and CBF is near 8 mL per 100 grams of brain tissue per minute for the most severe injury.

### Pupillary Response
The pupil's diameter is controlled by the autonomic nervous system in order to affect the amount of light entering the eye. This diameter control is carried out by two muscles: the pupilloconstrictor, which is parasympathetically controlled (via the neurotransmitter acetylcholine), and the pupillodilator, which is sympathetically controlled (via the neurotransmitter norepinephrine) @cite adoni2007pupillary. The synaptic pathways for these two pupil effects differ, and thus, any deviation from normal pupil behavior can shed light on any interference, whether that be a brain injury causing increased pressure on a nerve or a drug effect interfering with synaptic transmission. Because pupil examination is informative yet minimally invasive, it is an excellent tool for helping to diagnose neurological problems. The PERRLA assessment is commonly used to classify pupillary behavior: Pupils Equal, Round, Reactive to Light, and Accomodating. Pupillary response in %BioGears is modeled by tracking any modification to pupil size and light reactivity for each eye. Both TBI @ref nervous-actions "actions" and @ref drugs-pharmacodynamics "drugs" can apply modifiers to pupil size and reactivity.

Pupil size and reactivity can vary patient-to-patient based on many factors, including age, mental and emotional state, and ambient light conditions @cite winn1994factors. Furthermore, pupillary assessments like PERRLA are often qualitative rather than quantitative. Because of this, %BioGears models pupillary modifiers rather than discrete pupil sizes and size changes over time. In this way, the pupillary effects can be assessed qualitatively without the need for baseline patient values for pupil size and reactivity.

There are two ways to affect the pupils in %BioGears: drug pharmacodynamic effects and TBI. For a discussion of pharmacodynamic effects on pupillary response, see @ref drugs-pharmacodynamics "Drugs Methodology". The other method of influencing pupillary response is TBI. Increasing intracranial pressures constrict ocular nerves, causing pupillary disruptions @cite kingsly2012severe. Because of this relationship between ICP and pupil effects, ICP is the metric on which pupillary modifiers are based. For the pupil size modifier, Equation 9 was developed so that pupil size (m<sub>s</sub>) begins to see effects when ICP increases above around 20 mmHg, the pressure at which recoverable brain damage is often observed @cite steiner2006monitoring, hitting its maximum slope at 22.5 mmHg, and leveling off as ICP approaches 25 mmHg. For pupil light reactivity (m<sub>r</sub>), the curve seen in Equation 10 was developed so that it remains at 0 until ICP approaches 20 mmHg, then drops off sharply towards -1 as ICP approaches 25 mmHg. For both of these modifiers, a 1 represents "maximal" effect, -1 represents "minimal" effect, and 0 represents no effect. So a pupil size modifier of 1 would mean the pupil size is at its maximum possible diameter.

\f[m_s=1/(1+(e^{-2.3\left(ICP-22.5\right)})\f] 
<center>
*Equation 9.*
</center><br>

\f[m_{r} =-.001*10^{.3(ICP-15)} \f] 
<center>
*Equation 10.*
</center><br>

For diffuse injuries, these modifiers are applied to both eyes. For focal injuries, the modifiers are applied to the eye on the same side as the injury, as this reflects the most common observed behavior @cite kingsly2012severe.

@anchor nervous-variability
### Patient Variability
The baroreceptor reflex gains and time constants are independent of patient configuration. However, set-points are computed after stabilization, and the baroreceptor reflex drives towards a configuration-specific homeostasis. Because TBI uses multiplicative factors to modify the brain section of the cardiovascular circuit, the patient variability of TBI is the same as that seen in the %Cardiovascular System. There is no patient variability inherent in %BioGears pupillary response model, as it uses modifiers instead of baseline values. A detailed discussion of patient configuration and variability in %BioGears is available in the @ref PatientMethodology report.
@anchor nervous-dependencies
### Dependencies
The baroreceptors interact directly with the %cardiovascular system by modifying %cardiovascular circuit and heart driver parameters. These include heart rate, left and right heart elastance, systemic vascular resistance and systemic vascular compliance. The %cardiovascular parameters are modified by first calculating the normalized change in the parameter. This normalized change is passed into the common data model (CDM) where the %cardiovascular system may extract the change and then apply it locally as a fractional adjustment. The resultant hemodynamic changes feedback directly to the MAP, and the updated MAP is used by the %Nervous system in the next time slice to compute new normalized baroreceptor effects. TBI also directly modifies the cardiovascular circuit, specifically both the upstream and downstream brain resistors. Finally, the %Drugs system can contribute additional pupillary effects, that combine with possible TBI pupil effects via simple summation.

@anchor nervous-assumptions
Assumptions and Limitations
---------------------------
The current implementation of the baroreceptor model does not track adjustments in unstressed volume. Currently %BioGears does not correctly represent the physiologic unstressed volume under resting conditions. Therefore, these changes to the unstressed volume cannot be reflected in the %BioGears engine. This may be addressed in future releases of the engine.

The %BioGears TBI model assumes an acute injury to the brain; chronic effects are not considered, and neither are consciousness assessments. The brain is considered to be one compartment and is not subdivided into discrete functional areas. This means that there is no circuit distinction between a diffuse TBI and a Left Focal TBI. The only difference between the TBI actions is the expected pupil effect. TBI metric validation assumes a supine adult. The current TBI model does not consider specific effects of CO<sub>2</sub> saturation, O<sub>2</sub> saturation, or blood pH on CBF.

@anchor nervous-actions
Actions
-------

### Insults

#### Brain Injury
The Brain Injury action refers to an acute injury to the brain. Whereas in a real patient, a TBI could have manifold effects, both acute and chronic, %BioGears only considers the effects of TBI on brain vascular resistance and pupillary response. The effects of the Brain Injury action depend on the severity value from 0 to 1 and a Type (Diffuse, Left Focal, or Right Focal) assigned to the injury. A severity of 0 will result in a multiplicative factor of 1 being applied to both the upstream and downstream resistors in the brain, which equates to no deviation from the normal, uninjured state. For an injury severity of 1, the upstream resistor is assigned a multiplicative factor of 4.775 and the downstream resistor is assigned a multiplicative factor of 30.409. Severity values between 0 and 1 are converted to multiplicative factors linearly. Increased flow resistance results in increased ICP and decreased CBF. A Diffuse-type injury affects both eyes equally, whereas a Left Focal injury affects only the left eye, and a Right Focal injury affects only the right eye.

@anchor nervous-events
Events
------

### Intracranial Hypertension
Intracranial Hypertension is triggered when the intracranial pressure exceeds 25 mmHg. The event is reversed when the pressure returns below 24 mmHg.

### Intracranial Hypertension
Intracranial Hypotension is triggered when the intracranial pressure drops below 7 mmHg. The event is reversed when the pressure returns above 7.5 mmHg.

@anchor nervous-results
Results and Conclusions
=======================

Validation - Resting Physiologic State
--------------------------------------
No resting state physiology validation was completed, because the baroreceptors do not provide feedback during the initial resting and chronic stabilization phases. For more information on the stabilization phases, see @ref SystemMethodology.

Validation - Actions and Conditions
--------------------
### Baroreceptor Reflex
The baroreceptor reflex is validated through simulation of an acute hemorrhage scenario. This scenario begins with the healthy male patient. After a short time a massive hemorrhage (bleeding rate of ~1000 mL/min) is initiated, and after 30 seconds the bleeding is stopped. The responses for cardiac output, heart rate, systemic vascular resistance and blood volume are shown in Figure 6. The responses shown in the plot are initially driven by decreasing blood volume, resulting in decreased preload and a subsequent reduction in cardiac output. The baroreceptors attempt to maintain the arterial pressure by increasing the heart rate and contractility, increasing the systemic vascular resistance, and reducing vascular compliance. For this specific hemorrhage scenario, there is an expected 30% increase in heart rate, 15-20% decrease in cardiac output, and 10-15% increase in systemic vascular resistance @cite hosomi1979effect. %BioGears baroreceptor feedback does a fair job reaching these values, though it is a bit aggressive in doing so. The heart rate increase observed is much higher than expected, but the changes in cardiac output and systemic vascular resistance are in line with expectations. The validation results are shown in tabular form below. Additional validation related to the baraoreceptor reflex and hemodynamic stability can be found in the @ref CardiovascularMethodology report.

<img src="./plots/Nervous/Baroreceptors_CardiacOutput.jpg" width="1100">
<img src="./plots/Nervous/Baroreceptors_HR.jpg" width="1100">
<img src="./plots/Nervous/Baroreceptors_VascularResistance.jpg" width="1100">
<img src="./plots/Nervous/BaroreceptorsLegend.jpg" width="1100">
<center> *Figure 6. The hemodynamic response to hemorrhage with feedback from the baroreceptor reflex.* </center>

<center><br>
*Table 1. The cardiac output, heart rate, systemic vascular resistance and blood volume are shown as a function of time for the acute hemorrhage scenario. There is a noticeable decrease in blood volume and cardiac output due to the fluid loss. The baroreceptor response in this situation leads to an increase in heart rate and systemic vascular resistance in an attempt to maintain arterial pressure.*
</center>
|	Action	|	Notes	|	Sampled Scenario Time (s)	|	Heart Rate (beats/min)	|	Cardiac Output (mL/min)	|	Systemic Vascular Resistance (mmHg s/mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|
|	Hemorrhage	|	10% blood loss in 30 s	|	325	|<span class="warning">	Increase ~30% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|<span class="success">	Decrease ~15-20% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|<span class="success">	Increase 10-15% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|

### Brain Injury
The Brain Injury action is validated through repeated application and removal of increasing severities of TBI. The scenario begins with a healthy male patient. After a short time, a mild brain injury (Severity = 0.2, Type = Diffuse) is applied, and the patient is allowed to stabilize before the injury state is removed (only one TBI action can be in effect at a time, so adding a Diffuse Severity 0 TBI removes all TBI effects). This process is repeated for a more severe injury (Severity = 0.75, Type = Left Focal) and severe (Severity = 1, Type = Right Focal) brain injury. We expect to see increases in ICP, with the most severe injury resulting in an ICP greater than 25 mmHg and decreases in CBF, with the most severe case approaching 8 mL per 100 grams of brain per minute (108 mL per minute for the validated patient) @cite bergeronSME @cite steiner2006monitoring. The scenario shows good agreement for these values, and the patient even dies after some time with the most severe injury. We expect CPP to either increase above its maximum normal value or decrease below its minimum normal value, but, though we see a drop, it isn't quite as pronounced as expected @cite steiner2006monitoring. We can also see that for the low severity injury, ICP doesn't quite reach the threshold to strongly affect the pupils. For the Left Focal injury, only the left pupil is affected, and for the Right Focal injury, only the right pupil is affected.

<table>
<tr>
<td><img src="./plots/Nervous/TBI_ICP.jpg" width="550">
</td>
<td><img src="./plots/Nervous/TBI_CBF.jpg" width="550">
</td>
</tr>
<tr>
<td><img src="./plots/Nervous/TBI_MAP.jpg" width="550">
</td>
<td><img src="./plots/Nervous/TBI_CPP.jpg" width="550">
</td>
</tr>
</table>
<img src="./plots/Nervous/TBILegend.jpg" width="800">
<center>
*Figure 7. Traumatic brain injury response at three different severity levels.*
</center>

<br>
<br>

<center>
<img src="./plots/Nervous/TBI_PupilICP.jpg" width="1200">
<img src="./plots/Nervous/TBI_LeftSize.jpg" width="1200">
<img src="./plots/Nervous/TBI_LeftReactivity.jpg" width="1200">
<img src="./plots/Nervous/TBI_RightSize.jpg" width="1200">
<img src="./plots/Nervous/TBI_RightReactivity.jpg" width="1200">
<img src="./plots/Nervous/PupilLegend.jpg" width="1200">
</center>
<center>
*Figure 8. Pupillary response to the same TBI scenario as shown in Figure 5 where increasing severities are applied first as Diffuse, then as Left Focal, then as Right Focal.*
</center><br>

<center><br>
*Table 2. The validation data for the TBI scenario shows good agreement with expected results.*
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Intracranial Pressure (mmHg)	|	Cerebral Blood Flow (mL/min)	|	Cerebral Perfusion Pressure (mmHg)	|	Heart Rate (1/min)	|	Respiration Rate (1/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Brain Injury	|	Severity 0.2, CPP=MAP-ICP, StandardMale brain mass=1450g	|	20	|	600	|<span class="warning">	10% Increase @cite bergeronSME	</span>|<span class="success">	Decrease @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="warning">	0-10% Decrease @cite bergeronSME	</span>|<span class="warning">	0-10% Decrease @cite bergeronSME	</span>|
|	Brain Injury	|	Severity 0	|	620	|	900	|<span class="success">	7-15 mmHg @cite steiner2006monitoring	</span>|<span class="success">	50-65 mL/100g/min @cite guyton2006medical	</span>|<span class="success">	60-98 mmHg	</span>|<span class="success">	72 @cite guyton2006medical	</span>|<span class="success">	[12.0, 20.0], [13.0, 19.0] @cite silverthorn2013human @cite mantoni2007reduced	</span>|
|	Brain Injury	|	Severity 0.75	|	920	|	1700	|<span class="success">	Increase @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="warning">	0-15% Decrease @cite bergeronSME	</span>|<span class="warning">	0-20% Decrease @cite bergeronSME	</span>|
|	Brain Injury	|	Severity 0	|	1720	|	2000	|<span class="success">	7-15 mmHg @cite steiner2006monitoring	</span>|<span class="success">	50-65 mL/100g/min @cite guyton2006medical	</span>|<span class="success">	60-98 mmHg	</span>|<span class="success">	72 @cite guyton2006medical	</span>|<span class="success">	[12.0, 20.0], [13.0, 19.0] @cite silverthorn2013human @cite mantoni2007reduced	</span>|
|	Brain Injury	|	Severity 1	|	2020	|	3220	|<span class="success">	>25 mmHg @cite steiner2006monitoring	</span>|<span class="success">	<8 mL/100g/min @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="warning">	Decrease @cite bergeronSME	</span>|<span class="warning">	Decrease @cite bergeronSME	</span>|

@anchor nervous-conclusions
Conclusions
-----------
The %Nervous System is currently in a preliminary state that contains only a baroreceptor feedback model, basic TBI, and pupillary response. The baroreceptor feedback is used to control rapid changes in arterial pressure by adjusting heart rate, heart elastance, and vascular resistance and compliance. The baroreceptor model has been validated by comparing the %BioGears outputs to experimental data for hemorrhage. It currently shows good agreement with the expected trends, but the magnitude of the response is not a strong as the validation data. This is due to a large total vascular compliance, which allows for large changes in blood volume with small changes in pressure. Future adjustments to the %cardiovascular circuit would correct the vascular compliance and improve the accuracy of the model. The TBI model shows good agreement for the most prominent TBI metrics, ICP and CBF, for acute brain injuries. Pupillary response behaves as expected and arms %BioGears with yet another tool for matching output with clinical data.

@anchor nervous-future
Future Work
===========
Coming Soon
-----------
- Chemoreceptor modification of heart contractility
- Local autoregulation
- Improved sympathetic and parasympathetic control

Recommended Improvements
------------------------
- Including unstressed volume in the tone model
- Patient variability incorporated into the baroreceptor reflex model constants
- Positional variability
- Higher-fidelity brain model with localized injuries
- Interventions to treat TBI like drainage of cerebrospinal fluid, hypothermic treatment, hyperventilation, and mannitol therapy
- Consciousness model, including reduced level of consciousness with carbon monoxide and other intoxications
- Sleep
- Heart rate and respiration rate effects from TBI

Appendices
==========
Acronyms
--------
CBF - Cerebral Blood Flow

CNS - Central %Nervous System

CPP - Cerebral Perfusion Pressure

ICP - Intracranial Pressure

PERRLA - Pupils Equal, Round, Reactive to Light, and Accommodating

PNS - Peripheral %Nervous System

MAP	- Mean Arterial Pressure

mmHg - Millimeters mercury

Data Model Implementation
-------------------------
@ref NervousSystemTable "Nervous"
