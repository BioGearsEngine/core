%Nervous Methodology {#NervousMethodology}
==========================

@anchor nervous-overview
Overview
========
Abstract
--------
The %BioGears %Nervous System models the central %nervous system (CNS) and peripheral %nervous system (PNS). Currently, the system models afferent signal generation as a result of baroreceptor, chemoreceptor, lung-strech receptor activity.  These signal are processed by a rudimentary central nervous system (CNS) that relays commands to efferent effectors.  Additionally, the %Nervous System provides tracks parameters related to traumatic brain injury events, pain stimulus response, and drug effects.  In the future, other feedback mechanisms will be added for improved modeling of homeostatic and crisis states.

@anchor nervous-introduction
Introduction
------------
The %Nervous System in the human body is comprised of the central nervous system (CNS) and the the peripheral %nervous system (PNS). The CNS contains the brain and the spinal cord, while the PNS is subdivided into the somatic, autonomic, and enteric nervous systems. The somatic system controls voluntary movement. The autonomic nervous system is further subdivided into the parasympathetic and sympathetic nervous system. The sympathetic nervous system is activated in times of stress and crisis, commonly referred to as the "flight or fight" behaviors. The parasympathetic nervous system controls homeostasis functions during the relaxed state. The enteric nervous system controls the gastrointestinal system. Many of these behaviors are tightly connected to the %Endocrine System. In many cases, the %Nervous System receptors trigger hormone releases that cause systemic changes, such as dilation and constriction of vessels, heart contractility changes, and respiration changes @cite guyton2006medical. 

%BioGears will focus on a select few mechanisms within the %Nervous System to provide feedback for both homeostasis and crisis behavior in the body. The list is shown below:
 - Baroreceptors (aortic, carotid, and low-pressure)
 - Chemoreceptors
 - Pulmonary stretch receptors
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
The %BioGears Nervous System began as a low-fidelity model that tracked metrics associated with brain injury.  Over time, feedback models local to other %BioGears systems (i.e. baroreceptors, chemoreceptors) were relocated to the %Nervous System with the goal of moving towards a centralized %Nervous model that serves as a global controller.  The primary difficulty with such a model is that most detailed %Nervous models rely on differential equations that output nerve firing rates.  If the frequencies of these firing rates are too high compared to the time-step used to model the equations, the solution will be unstable.  Fortunately, as %BioGears stability has been improved, the simulation time-step has been decreased to its current value of 0.02 s.  This step size allowed us to work toward strengthenging the interactions between the existing models.

Data Flow
---------
An overview of the data flow in the %BioGears %Nervous system is shown in Figure 1.

### Initialization and Stabilization
The %BioGears initialization and stabilization is described in detail in the [stabilization section](@ref system-stabilization) of the @ref SystemMethodology report. Setpoints related to Nervous model are reset once the %Cardiovascular system reaches a homeostatic state.  These include the baseline baroreceptor operating pressure, the baseline oxygen and carbon dioxide blood gas levels, the baseline sympathetic and parasympathetic firing rates, and the baseline cerebral blood flow.

### Preprocess
#### Afferent Signal Generation
This method surveys the current state of the patient (blood pressure, blood-gas levels, lung capacity) and relays information to the central nervous system.
##### Baroreceptor Feedback
The baroreceptor mechanism provides rapid negative feedback control of arterial pressure. A drop in arterial pressure is sensed by the baroreceptors and leads to an increase in sympathetic activity and vagal (parasympathetic) withdrawal. These changes operate with the goal of maintaining arterial pressure at its healthy resting level.  %BioGears distinguishes between aortic, carotid, and low-pressure (cardiopulmonary) receptors.  Aortic and carotid receptors are both sensitive to changes in systolic arterial pressure, but their relative locations in the body affect this response.  Aortic baroreceptors respond to the transmural pressure between the aorta and the intrapleural space.  The carotid baroreceptors, located a distance above the heart, are affected by pressure head (except when an individual is lying down).  Low-pressure receptors are located near the venous return to the heart and are therefore sensitive to the central venous pressure and pleural pressure. %BioGears uses a stress-strain relationship to calculate the signal generated at the aortic and carotid baroreceptors @cite randall2019model and a first-order, low-pass filter to generate the low-pressure receptors signal @cite lim2013cardiovascular.
 
##### Chemoreceptor Feedback
The chemoreceptor mechanism provides feedback control to regulate the partial pressures of oxygen and carbon dioxide in the blood.  The response is divided in to two components:  peripheral and central.  Peripheral chemoreceptors (which reside in the aortic arch and carotid arteries) are sensitive to deviations in both oxygen and carbon dioxide pressures.  The central chemoreptors, so named because of their location in the central nervous system, respond only the carbon dioxide pressure changes.  Assuming that each receptor class operates independently, the feedback from the central and peripheral chemoreceptors is summed to generate changes in respiration frequency and respiratory driver pressure to restore blood gas levels to their set points.  The peripheral chemoreceptor signal is also processed by the central nervous model to account for its contribution to cardiovascular activity.    

##### Pulmonary Stretch Receptors
Stretch receptors respond to changes in lung volume.  They tend to balance the cardiovascular effects induced by peripheral chemoreceptors @cite ursino2000acute.  

#### Central Nervous (CNS) Response
This method consolidates the signals generated by the afferent models and adjusts the relative firing rates of the sympathetic and parasympathetic arms of the nervous system.  The sympathetic signal consists of two outputs:  a sinoatrial portion that affects heart rate and elastance and a peripheral portion that affects systemic resistance and unstressed venous volume.  The parasympathetic signal only acts upon the heart rate.  The threshold for sympthetic firing is lowered under hypoxic conditions to model the ischemic response @cite ursino2000acute.

#### Efferent Response
The efferent response carries out the work of updating the state of the cardiovascular system according to the autonomic response determined by the CNS.  This model determines the normalized changes in heart rate, heart elastance, peripheral vessel resistance, and peripheral venous compliance.  The peripheral resistance response distinguishes between the splanchnic, extrasplanchnic, and skeletomuscular regions of the body to account for preferential blood-flow shunting during certain extreme responses (e.g. maintaining flow to vital organs during hemorrhage).

#### Local Autoregulation
The brain, muscle, and myocardium prioritize blood flow and oxygen delivery.  %BioGears models cerebral autoregulation in which vessels in the brain constrict or dilate to maintain constant cerebral blood flow, perfusion pressure, and oxygen saturation.  %BioGears also models local regulation in the vessels of the muscle and myocardium in which vasodilation occurs in response to oxygen depletion.

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
%BioGears calculates the strain exerted on the aortic and carotid baroreceptors as @cite randall2019model

\f[ \epsilon_{w} = 1 - \sqrt{\frac{1 + \exp(-q_w\left(P_{input} - s_w\right))}{A+\exp(-q_w\left(P_{input}-s_w\right))}} \f]
<center>
*Equation 1.*
</center><br>

The input pressure (P<sub>input</sub>) is the systolic arterial pressure (P<sub>sys</sub>) for the carotid baroreceptors and the difference between systolic pressure and pleural pressure (P<sub>systolic</sub> - P<sub>pleural</sub>) for aortic baroreceptors.  The parameters for Equation 1 are maximum stressed to unstressed vessel cross-sectional area (A), steepness (q<sub>w</sub>), and operating point (s<sub>w</sub>) and the output is wall strain ($\epsilon_w$).  As described in the [cardiovascular](@ref cardiovascular-initialize) methodology report, the%BioGears %cardiovascular system is initialized according to patient definitions and stabilized to a homeostatic state. The baroreceptor operating pressure is the systolic pressure following the engine stabilization period. This operating point is adjusted dynamically with certain actions and insults, as shown in Equation 2.

\f[ s_w = P_{sys, set} + \Delta P_{drugs} + \Delta P_{exercise} + \Delta P_{pain} \f]
<center>
*Equation 2.*
</center><br>

Where &Delta;<b>p</b><sub>drugs</sub>, &Delta;<b>p</b><sub>exercise</sub>, and &Delta;<b>p</b><sub>pain</sub> are changes in apparent operating point due to certain drugs, exercise, and pain response.  It is known that aortic and carotid baroreceptors adapt to sustained changes in arterial pressure.  We model adaptation by adjusting the operating point as a function of systolic arterial pressure @cite pruett2013model

\f[ \frac{ds_w}{dt} = k_{adapt}\left(P_{sys} - s_w\right) \f].
<center>
 *Equation 3.*
</center><br>

The value of k<sub>adapt</sub> reflects a half-time of approximately 16 hours for the baroreceptors to acclimate to a sustained step-change in systolic pressure @cite pruett2013 model. 

The baroreceptors are assumed to operate as a spring-dashpot system.  The strain induced by the stress $\epsilon_w $ is determined by the differential equation @cite randall2019model

\f[ \frac{d \epsilon_{b}}{dt} = \frac{-\epsilon_{b} + K_b \epsilon_{w}}{\tau_{b}} \f]
<center>
*Equation 4.*
</center><br>

The strain signal $\epsilon_b$ determined for each baroreceptor class (aortic and carotid) is processed in the Central Nervous response.

The low-pressure baroreceptor signal is generated by sampling the transmural pressure between the venous return (CVP) and the pleural space @cite lim2013cardiovascular.

\f[ \frac{d\tilde{P_{cp}}}{dt} = \left(\frac{1}{\tau_{cp}}\right)\left(-\tilde{P_{cp}} + \left(CVP - P_{pleural}\right)\right) \f]
<center>
*Equation 5*
</center><br>

\f[ f_{cp} = \frac{f_{cp,max}}{1.0 + \exp(\left(CVP_{base} - P_{pleural,base}\right) - \tilde{P_{cp}})} \f]
<center>
*Equation 6*
</center><br>

Equation 5 acts as a low-pass filter that reduces signal noise and Equation 6 generates a firing rate (f<sub>cp</sub>) that is processed by the CNS. &tau;<sub>cp</sub> and f<sub>cp,max</sub> are the time constant for the response and the maximum firing rate, while CVP<sub>base</sub> and P<sub>pleural,base</sub> represent the normal central venous and pleural pressures.


### Chemoreceptors
The chemoreceptors are cells that are sensitive to changes in blood gas concentration. Peripheral chemosensitive cells--located in the aortic arch--detect fluctuations in both arterial carbon dioxide and oxygen partial pressures.  The central chemoreceptors, which reside in the central nervous system (CNS), respond to changes in cerebral blood pH caused by irregular carbon dioxide levels.  It is assumed in this model that carbon dioxide transport across the blood-brain barrier and equilibration therein are rapid processes; that is, it assumed that the central chemoreceptor response is proportional to deviations in arterial carbon dioxide.  

Control of respiration is based on the model developed by Albanese, Magosso, and Ursino in @cite ursino2000acute, @cite magosso2001mathematical, @cite ursino2002theoretical, and @cite albanese2015integrated.  It is assumed that the central (c) and peripheral (p) feedback define deviations from baseline patient respiration frequency (f) and peak respiratory driver pressure (P<sub>max</sub>), and that their contributions are independent (and thus additive).  

\f[f_{target} = f_{base} + \Delta f_c + \Delta f_p \f]
<center>
*Equation 7.*
</center><br>

\f[P_{max,target} = P_{max,base} + \Delta P_{max,c} + \Delta P_{max,p} \f]
<center>
*Equation 8.*
</center><br>

Clearly, the blood gas levels are normal, central V<sub>c</sub> and peripheral V<sub>p</sub> changes in ventilation are 0, and target ventilation V<sub>target</sub> equals patient baseline ventilation V<sub>base</sub>.

The evolution of the central chemoreceptor feedback is assumed to be a function of arterial carbon dioxide partial pressure (P<sub>CO<sub>2</sub></sub>) deviation from its normal set point (P<sub>CO<sub>2,set</sub></sub> = 40 mmHg).

\f[ \frac{d\Delta E_{c}}{dt} = \left(\frac{1}{ \tau_{E,c}} \right) \left( -\Delta{E_{c}} + g_{E,c}\left(P_{CO_{2}}(t) - P_{CO_{2,set}}\right)\right) \f]
<center>
*Equation 9.*
</center><br>

The effects (E) are frequency (f) and driver amplitude (P) with associated time constants (&tau;<sub>c</sub>) and controller gains (&tau;g<sub>c</sub>).  Equation 9 is identical to that reported in @albenese2015integrated, except that a delay term is omitted.  This omission is a result of being currently unable to model delay differential equations in %BioGears.  

The form of the peripheral feedback in Equation 10 is similar to Equation 8, but the response is dictated by the combined effects of carbon dioxide pressure and oxygen saturation.  Equations 11-12 show how this firing rate (f(t)) is calculated.  As before, this equation is applied twice per effect (E = {f, P}).

\f[ \frac{d\Delta E_{p}}{dt} = \left(\frac{1}{ \tau_{E,p}} \right) \left( -\Delta E_{p} + g_{E,p}\left(f(t) - f_{set}\right)\right) \f]
<center>
*Equation 10.*
</center><br>

\f[ \frac{df}{dt} = \frac{1}{\tau_{f}}\left(-f(t) + \psi\right) \f]
<center>
*Equation 11.*
</center><br>

\f[ \psi = \frac{f_{max} + f_{min}*exp\left(\frac{P_{O_{2}} - P_{O_{2,half}}}{k_{O_{2}}}\right)}{1 + exp\left(\frac{P_{O_{2}} - P_{O_{2,half}}}{k_{O_{2}}}\right)}*\left[K*ln\left(\frac{P_{CO_{2}}}{P_{CO_{2,set}}}\right) + \gamma\right] \f]
<center>
*Equation 12.*
</center><br>

Equation 12 is a sigmoid that outputs a chemoreceptor firing rate (bounded by f<sub>min</sub> and f<sub>max</sub>) based on the current arterial oxygen pressure (P<sub>O<sub>2</sub></sub>).  The variables P<sub>O<sub>2,half</sub></sub> and k<sub>O<sub>2</sub></sub> are the half-max and slope parameters that define the shape of the sigmoid, a shape which is increasingly adjusted as the arterial carbon dioxide pressure (P<sub>CO<sub>2</sub></sub>) deviates from normal (P<sub>CO<sub>2,set</sub></sub>).  K and &gamma; are tuning parameters required to maintain a steady output when oxygen and carbon dioxide pressures are at their normal values.  Values for all parameters in Equations 9-12 are available in @cite magosso2001mathematical.

### Pulmonary Stretch Receptors
Pulmonary stretch receptors are modeled as in @cite ursino2000acute, except that total lung volume (V<sub>lung</sub>) is used as the input rather than tidal volume.  We make this change because the total lung volume in %BioGears is a more stable output than tidal volume.

\f[ \frac{df_{ps}}{dt} = \frac{-f_{ps} + G_{ps}V_{lung}}{\tau_{ps}} \f]
<center>
*Equation 13.*
</center><br>

### Central Nervous Response
The CNS model outputs three signals: a sympathetic efferent signal to the sinoatrial node (f<sub>es,s</sub>), a sympathetic efferent signal to the periphery (f<sub>es,p</sub>), and an efferent vagal (parasympathetic) signal to the sinoatrial node (f<sub>ev</sub>).  These signals are determined by weighting the following inputs from the afferent arm of the nervous system: aortic baroreceptors (f<sub>ab</sub>), carotid baroreceptors (f<sub>cb</sub>), cardiopulmonary baroreceptors (f<sub>cp</sub>), peripheral chemoreceptors (f<sub>pc</sub>), and pulmonary stretch receptors (f<sub>ps</sub>).  The equations for the three efferent signals have been developed in @cite ursino2000acute, @cite magosso2001mathematical, @cite magosso2001theoretical, and @cite albenese2015integrated.  Weighting parameters (W<sub>j</sub>) were established from these sources as well as from @cite lim2013cardiovascular and @cite liang2006simulation.

The sympathetic efferent firing rates are modeled as sigmoids:

\f[ f_{es,s} = f_{es,\infty} + \left(f_{es,0} - f_{es,\infty}\right)exp(k_{es}\left(W_{s,ab}f_{ab} + W_{s,cb}f_{cb} + W_{s,pc}f_{pc} + W_{cp}\left(f_{cp} - f_{cp,0}\right) - \theta_{s}\right)) \f]
<center>
*Equation 14.*
</center><br>

\f[ f_{es,p} = f_{es,\infty} + \left(f_{es,0} - f_{es,\infty}\right)exp(k_{es}\left(W_{p,ab}f_{ab} + W_{p,cb}f_{cb} + W_{p,pc}f_{pc} + W_{p,ps}f_{ps} + W_{cp}\left(f_{cp} - f_{cp,0}\right) - \theta_{p}\right)) \f]
<center>
*Equation 15.*
</center><br>

The parameters &theta;<sub>s</sub> and &theta;<sub>p</sub> refer to the firing thresholds of the sinoatrial and peripheral sympathetic signals.  These thresholds are adjusted under hypoxic and hypercapnic scenarios to model the ischemic response @cite Magosso2001Mathematical.

\f[ \frac{d\Delta\theta_{O_2,j}}{dt} = \frac{1}{\tau_{isc}}\left(-\Delta\theta_{O_2,j} + \frac{\chi_{j}}{1+\exp(\frac{P_{O_2}-h_{O_2,j}}{k_{isc,j}})}\right) \f]
 <center>
*Equation 16.*
</center><br>

\f[ \frac{d\Delta\theta_{CO_2,j}}{dt}= \frac{-\theta_{CO_2,j}+g_{c,j}\left(P_{CO_2}-P_{CO_2,n}\right)}{\tau_c} \f]
 <center>
*Equation 17.*
</center><br>

\f[ \theta_{j} = \theta_{j,n} - \Delta\theta_{O_2,j} - \Delta\theta_{CO_2,j} \f]
 <center>
*Equation 18.*
</center><br>

In Equations 16-18, the subscript "j" refers to either the sinoatrial or peripheral ischemic response.  The parameters &theta;<sub>j,n</sub> refer to the basal values the thresholds take on at normal blood-gas levels.  The maximum slope and half-saturation parameters (h<sub>O2,j</sub> and k<sub>isc</sub>, respectively) determine the overall shape of the response.

The parasympathetic signal is a sigmoidal function of the baroreceptor firing rate and linear with respect to the chemoreceptor and pulmonary stretch signals.

\f[ f_{ev} = \left(\frac{f_{ev,0} + f_{ev,\infty}\exp(\frac{f_{b,avg}-f_{b,avg,0}}{k_{ev}})}{1+\exp(\frac{f_{b,avg}-f_{b,avg,0}}{k_{ev}})}\right) + W_{v,pc}f_{pc}+W_{v,ps}f_{ps} - \theta_{ev} \f]
 <center>
*Equation 19.*
</center><br>

Unlike the sympathetic efferent pathway, the parasympathetic path assumes a constant firing theshold \(\theta_{ev}\).  The value f<sub>b,avg</sub> is the average firing rate of the carotid and aortic baroreceptors.  The low-pressure baroreceptors do not effect the parasympthetic signal.

### Efferent Response    
The three signals relayed by the CNS (f<sub>es,s</sub>, f<sub>es,p</sub>, and f<sub>ev</sub>) are used to update the cardiovascular model using effector equations outlined in @cite ursino2000acute.  These equations have been normalized so that their outputs represent a fractional change from baseline.  As in @cite ursino2000acute, the response to the sympathetic activation is assumed to involve a logarithmic transformation, whereas the vagal response is linear.  Furthermore, the sympathetic signals are bounded at the minimum signal f<sub>es,min</sub> to maintain continuity.  In each of the normalized equations, the values &sigma;<sub>0</sub> correspond to the effector input at the baseline sympathetic and parasympathetic firing rates.  

#### Heart Rate
\f[ \frac{d \sigma_{HR, s}}{dt} = \frac{-\sigma_{HR,s} + G_{HR,s}\ln(f_{es,s}-f_{es,min}+1}{\tau_{HR,s}} \f]
 <center>
*Equation 20.*
</center><br>

\f[ \frac{d \sigma_{HR, v}}{dt} = \frac{-\sigma_{HR,v} + G_{HR,v}f_{ev}}{\tau_{HR,v}} \f]
 <center>
*Equation 21.*
</center><br>

\f[ HR_{norm} = \frac{1 + \sigma_{HR,s,0} + \sigma_{HR,v,0}}{1 + \sigma_{HR,s} + \sigma_{HR,v}} \f]
 <center>
*Equation 22.*
</center><br>

In Equation 22, the initial state of the heart rate appears in the numerator because Equations 21-22 reflect changes in heart period.  Thus, inverting the ratio of next heart period to baseline heart period gives the normalized change in heart rate.

#### Heart Elastance

\f[ \frac{d \sigma_{E}}{dt} = \frac{-\sigma_{E} + G_{E}\ln(f_{es,s}-f_{es,min}+1}{\tau_{E}} \f]
 <center>
*Equation 23.*
</center><br>

\f[ E_{norm} = \frac{1 + \sigma_{E}}{1 + \sigma_{E,0}} \f]
 <center>
*Equation 24.*
</center><br>

#### Vessel Resistance

\f[ \frac{d \sigma_{R,j}}{dt} = \frac{-\sigma_{R,j} + G_{R,j}\ln(f_{es,p}-f_{es,min}+1}{\tau_{R,j}} \f]
 <center>
*Equation 25.*
</center><br>

\f[ R_{j,norm} = \frac{1 + \sigma_{R,j}}{1 + \sigma_{R,j,0}} \f]
 <center>
*Equation 26.*
</center><br>
  
In Equations 25-26, j = splanchnic, extrasplanchnic, or muscle.  Each region has distinct controller gains and time constants associated with it.

#### Veinous Compliance
\f[ \frac{d \sigma_{C}}{dt} = \frac{-\sigma_{C} + G_{C}\ln(f_{es,p}-f_{es,min}+1}{\tau_{C}} \f]
 <center>
*Equation 27.*
</center><br>

\f[ C_{norm} = \frac{1 + \sigma_{C}}{1 + \sigma_{C,0}} \f]
 <center>
*Equation 28.*
</center><br>

The normalized compliance is applied to each fluid capacitor in the systemic portion of the BioGears cardiovascular circuit.

### Local Autoregulation
#### Cerebral Autoregulation
The cerebral autoregulation model is based on that of @cite ursino1998interaction and @cite ursino2001role.  The model distinguishes between changes in large arteries (upstream resistor in the BioGears cerebral circuit) and small arteries (downstream resistor).  The large arteries respond to changes in cerebral perfusion pressure (CPP), while the small arteries are sensitive to changes in cerebral blood flow (CBF).  Each response is attenuated by cerebral venous oxygen saturaton and cerebral carbon dioxide levels.  In all equations, a subscript "n" refers to the normal, basal value of the parameter.

The response of large arteries to changes in CPP is governed by @cite ursino1998interaction

\f[\frac{dx_{auto,l}}{dt} = \frac{-x_{auto,l} + G_{auto,l}\left(CPP - CPP_{n}\right)}{\tau_{auto,l}} \f]
<center>
*Equation 29.*
</center><br>  

The response of small arteries to CBF is likewise given by @cite ursino1998interaction 
\f[\frac{dx_{auto,s}}{dt} = \frac{-x_{auto,s} + G_{auto,l}\left(\frac{CBF-CBF_n}{CBF_n}\right)}{\tau_{auto,s}} \f]
<center>
*Equation 30.*
</center><br>  

The effects of carbon dioxide @cite ursino1998interaction and oxygen saturation @cite ursino2001role are tracked as follows:

\f[\frac{dx_{CO_2}}{dt} = \frac{-x_{CO_2} + A_{CO_2}G_{CO_2}log_{10}\left(\frac{P_{CO_2}}{P_{CO_2,n}}\right)}{\tau_{CO_2}} \f]
<center>
*Equation 31.*
</center><br> 

\f[ A_{CO_2} = \left(1+\exp(-K_{CO_2}\left(\frac{CBF-CBF_n}{CBF_n}\right) - b_{CO_2})\right)^{-1} \f]
<center>
*Equation 32.*
</center><br> 

\f[\frac{dx_{O_2}}{dt} = \frac{-x_{O_2} + G_{O_2}\left(S_{v,O_2}-S_{v,O_2,n}\right)}{\tau_{O_2}} \f]
<center>
*Equation 33.*
</center><br>

The total autoregulatory effects used to scale the large and small cerebral resistances are:

\f[ x_{large} = x_{auto,l} + x_{CO_2} + x_{O_2} \f]
\f[ x_{small} = x_{auto,s} + x_{CO_2} + x_{O_2} \f]
<center>
*Equation 34.*
</center><br>

#### Coronary and Skeletomuscular Autoregulation
Autoregulation in the heart and muscle is modeled similarly to @cite ursino2000acute.  The autoregulatory parameter for each location is:

\f[ \frac{dx_{O_2,j}}{dt} = \frac{-x_{O_2,j} + G_{O_2,j}\left(C_{v,O_2,j}-C_{v,O_2,jn}\right)}{\tau_{O_2,j}} \f]
<center>
*Equation 35.*
</center><br>

where j is either the muscle or heart, C<sub>v,O2</sub> is the current venous oxygen concentration, and C<sub>v,O2,n</sub> is the baseline venous oxygen concentration.

The change in resistance of the circuit path providing blood flow to the heart is 

\f[ R_{h} = \frac{R_{h,0}}{1 + x_{O_2,h}}\f]
<center>
*Equation 36.*
</center><br>
 
in which R<sub>h,0</sub> is the baseline resistance (constant).  The expression describing the change in muscle resistance is similar to Equation 36.

\f[ R_{m} = \frac{R_{m,next}}{1 + x_{O_2,m}}\f]
<center>
*Equation 37.*
</center><br>

It should be noted, however, that because the muscle resistance is also controlled by the sympathetic response, the resitance against which the autoregulation acts is that determined by Equation 26.
  
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

#### Pain Stimulus
The Pain Stimulus action triggers changes to the cardiovascular, respiratory, and endocrine systems that are associated with sympathetic activation.  Action initiation requires a severity (0-1 scale) and a location.  If multiple locations are specified, the pain severities are summed until the upper boundary of 1 is reached.  The severity is assumed to correspond directly to the Visual Analog Scale (VAS) used to quantify pain in clinical settings (e.g. Pain Severity = 0.5 --> VAS = 5).  To account for the fact that individuals perceive pain differently, a Pain Susceptibility parameter was added to the %BioGears patient definition.  This modifier has a range of -1 to 1, with positive values implying heightened sensitivity to pain and negative values indicating blunted sensitivity.  If not susceptibility is set in the patient file, then a value of 0 is assumed (i.e. "average susceptibility).  The susceptibility parameter is added to the pain severity when a stimulus action is applied.  For example, when exposed to a pain stimulus with a severity of 0.5, two patients with pain susceptibilities of -0.1 and 0.2 would report respective pain levels of 0.4 and 0.7 (4 and 7 on the Visual Analog Scale).

The pain scale calculated from the action severity and patient susceptibility is used to establish changes in physiology.  Heart rate, respiration rate, and blood pressure all increase as the consolidated pain scale increases.  Much of this response is achieved by increasing production of epinephrine in the %BioGears %Endocrine system.  Administration of analgesic drugs (such as Morphine or Fentanyl) lowers the pain score and reverses these physiological trends.   

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

### Chemoreceptor Reflex
The performance of the Chemoreceptor model in %BioGears was validated under both hypercanpic and hypoxic conditions.  Hypercapnic validation was based on the study of Reynolds, Milhorn, and Holloman @cite reynolds1972transient, in which healthy volunteers breathed air with supernormal carbon dioxide levels (3%, 5%, 6%, and 7% CO<sub>2</sub>).  Likewise, hypoxic validation scenarios used data from Reynolds and Milhorn @cite reynolds1973transient.  Volunteers in this study breathed 7%, 8%, and 9% O<sub>2</sub> in nitrogen. 


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
