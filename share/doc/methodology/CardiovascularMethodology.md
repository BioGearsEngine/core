%Cardiovascular Methodology {#CardiovascularMethodology}
==========================

@anchor cardiovascular-overview
Overview
========

Abstract
--------
The %BioGears&reg; %Cardiovascular system is a closed-loop series of compartments that represent the human circulatory system. The cardiovascular system is sometimes described as two separate circulations: the systemic circulation and the pulmonary circulation. Both circulations, as well as the lymph, are represented in the %BioGears %Cardiovascular system. The dynamical state of the %Cardiovascular System is determined at every timestep through a three-step process. First, intra- and inter-system feedback is applied during the preprocess step. Next, the state of the circulatory system is determined by solving an equivalent circuit. Finally, the state of the system is updated in preparation for time advancement. Through system and action validation, we demonstrate the accuracy of the model at resting physiology and a under variety of cardiovascular related insults and interventions with appropriate system feedback. 

Introduction
------------
### %Cardiovascular Physiology
The cardiovascular system is a large organ system comprised of the heart and the blood vessels. It serves as the body's primary transport and distribution system.  As mentioned above, it is frequently subdivided into the the systemic circulation and the pulmonary circulation. In the systemic circulation, oxygenated blood leaves the left side of the heart, travels  through arteries and into the capillaries, and then returns as deoxygenated blood through the veins to the right side of the heart. Upon exiting the right side of the heart, the deoxygenated blood enters the pulmonary circulation through the pulmonary arteries, is re-oxygenated in the pulmonary capillaries, and then returns to the left side of the heart through the pulmonary veins (Figure&nbsp;1). Whether the cardiovascular circulation is thought of as one closed circuit with a single dual-purpose pump or two interconnected circuits with separate synchronous pumps, the combination of systemic and pulmonary vasculature is a critical distribution and exchange network.  It provides vital oxygen to the tissues and removes toxic carbon dioxide while distributing nutrients and other substances necessary for healthy physiologic function.

@image html CardiovascularSystemDiagram.png
<center>
<i>Figure&nbsp;1. The %Cardiovascular System with pulmonary and systemic circulations of the human body @cite KVDP2014Circulatory . 
Both the pulmonary and systemic circulation originate in the heart, which acts as a pump driving the blood through the entire body. Blood enters the heart from the vena cava via the right atrium and transitions to the right ventricle, which sends it to the pulmonary arteries and into the lungs. Gas exchange occurs in the vascular arterioles, creating oxygen-rich blood. This blood then returns to the heart via the pulmonary veins to the left atrium. The oxygen-rich blood enters the systemic circulation through the left ventricle, providing oxygen to the rest of the body.</i>
</center><br>

The cardiovascular system is one of two systems that comprise the circulatory system, the other being the lymphatic system. The %BioGears cardiovascular system includes a low-resolution lymphatic circulation. Because the resolution of the lymph circulation in %BioGears is considerably lower than the resolution of the cardiovascular circulation, we refer to the comprehensive circuit as the circulatory circuit, but we maintain the term *cardiovascular* as system nomenclature to highlight the modeling focus. In other words, the %BioGears lymph circulation is not a physiological model of the human lymphatic system. It only serves as an accessory return for interstitial fluid. More information about the lymph model and the other accessory advection models is available in the @ref TissueMethodology report.

### %Cardiovascular Modeling
#### Brief Introduction to %Cardiovascular Modeling
%Cardiovascular (CV) models range from high-resolution, small-region-of-interest models @cite taylor1999predictive @cite wan2002one @cite formaggia2003one to low-resolution, whole-body simulations @cite heldt2004computational. Increased resolution may provide a more accurate solution in a localized region at the expense of decreasing the size of the modeling region. However, the level of accuracy achieved with such a model is not always necesssary to answer the question posed. Three-dimensional (3D) computational fluid dynamics (CFD) models are generally limited to short segments of the arterial or venous tree due to high computational requirements. These high-resolution models are well-suited for investigating complex geometries following surgical intervention, such as the Fontan Circulation @cite whitehead2007nonlinear @cite de1996use. However, they are not suitable for modeling long-term or systemic effects in the CV System @cite timmons1995cardiovascular because it is only feasible to simulate a few cardiac cycles.

One-dimensional (1D) models can also predict the pressure and flow wave dynamics within a region of interest @cite womersley1955xxiv @cite womersley1957oscillatory and have the advantage of a lower computational cost than full CFD models. Like CFD models, 1D models begin with the Navier-Stokes equations, but they are reduced to a hyperbolic partial differential equation by including an axisymmetric assumption. The resultant equation dictates a constant pressure in the radial direction under the assumption that the radius of the vessel is small relative to a characteristic wave length @cite vcanic2003mathematical. These models are well-suited for investigating specific CV problems @cite spilker2007morphometry @cite clipp2009impedance and have been coupled with dynamic downstream effects to investigate problems over numerous cardiac cycles @cite clipp2012evaluation. However, the computational cost associated with 1D models still renders them unsuitable for modeling the complete closed-circuit dynamics of the circulation over the entire range of vessel diameters and lengths in the human body.

The lumped parameter method of hemodynamic modeling assumes that characteristics of organ vessels or vessel segments can be  &ldquo;lumped&rdquo; into representative parameters. For instance, the viscosity of a fluid can be lumped together with the geometry of a vessel segment to derive an effective resistance to blood flow. If a tube is rigid, the pressure-flow dynamics are completely characterized by the resistance to flow in the tube. If a tube is elastic, an additional compliance parameter can be used to characterize the impedance and calculate the pressure-flow dynamics in the segment. In humans, small arteries can be approximated as rigid @cite olufsen2004deriving, while the corresponding veins are significantly (up to \~24 times) more compliant @cite hall2011guyton.

Otto Frank first formulated the quantitative model (called a Windkessel model after the wave-filtering air chamber in archaic firefighting equipment) relating systemic blood pressure with vessel elasticity @cite frank1899grundform. 
In the analogous electrical circuit (see @ref CircuitMethodology), the Windkessel model includes a capacitor--representing the compliance of the artery, connected in parallel with a resistor, representing the resistance to flow through the vessel. An alternate three-element form of the Windkessel model was developed by Westerhof et al. @cite westerhof1969analog that includes a black-box, two-terminal input circuit (representing an additional resistor), to quantify the characteristic impedance of the arterial tree. Similar applications of the electric analogue circuit have been applied to form CV circuits that can be classified as multi-segment or multi-system models.

#### %BioGears Lumped Parameter Model
Lumped parameter models have proven suitable for larger-scale, longer time-scale simulations due to lower computational requirements and sufficient fidelity. A multi-organ model can run in real time while still producing an accurate blood pressure and flow waveform prediction. The drawback to this type of model is the large number of parameters requiring definition or tuning. However, significant work has been targeted toward determining appropriate parameter values @cite bronzino1999biomedical @cite kokalari2013review @cite milivsic2004analysis @cite olufsen2004deriving @cite rideout1967difference @cite westerhof2009arterial. This body of work, in particular Heldt&rsquo;s open source CV simulator that includes lumped parameter models @cite heldt2004computational @cite hester2011hummod @cite summers2008development, supports the decision to implement these models in %BioGears.

@anchor cardiovascular-system-design
System Design
=============
Background and Scope
--------------------
### History of the %BioGears %Cardiovascular Model
The %BioGears CV Model has its origins in Guyton&apos;s four-compartment (three vascular, plus one heart) model of the CV System designed to analyze the effect of varying circulatory factors on cardiac output @cite guyton1955effect. Rideout, et al. used Guyton&rsquo;s foundation and an electric-hydraulic analogy to streamline the generation of difference-differential equations for modeling fluid flow in distensible vessels @cite rideout1967difference. Yasuhiro Fukui leveraged the previous work to model the CV and respiratory systems and their interactions @cite fukui1972study. The development of a mass and momentum transport model of the CV System allowed for the simulation of interactions between the CV System and angiotensin @cite masuzawa1992cardiovascular. Following the success of the angiotensin-cardiovascular simulator, development of the drug-interaction model continued and eventually led to an anesthesiology simulator that incorporated CV, respiratory, and drug models @cite smith1995pc. This simulator, released by Advanced Simulation Corporation as Body Simulation for Anesthesia&tm;, formed the backbone of the HumanSim&tm; physiology engine, which continues to provide realistic physiology for several serious training games in the HumanSim product line, including HumanSim: Sedation and Airway @cite Clipp2012Humansim. The HumanSim physiology engine is the starting-point for the %BioGears Engine. The basic building blocks of the CV System remain as described in Masuzawa et al. @cite masuzawa1992cardiovascular; however, the CV circuit has been further developed to provide a more accurate CV simulation and drive other %BioGears systems through intersystem dependencies.

Data Flow
---------
The state of the CV System is determined at every time step through a three-step process: Preprocess, Process, and Postprocess (Figure&nbsp;2). In the Preprocess step, feedback from other systems, as well as intrasystem feedback, is processed in preparation for determining the state of the system. Process uses the %BioGears [circuit](@ref CircuitMethodology) calculator to compute the new state of the system. Postprocess is used to prepare the system for the advancement of time. More specifics about these steps are detailed below.

@anchor cardiovascular-initialize
### Initialization and Stabilization
First, the patient-specific homeostatic state of the cardiovascular circuit is computed. Next, all system parameters are initialized. The %Cardiovascular System is then initialized to the resting state. Conditions are then applied by modifying system and patient parameters and restabilizing the engine. The available @ref cardiovascular-conditions "conditions" in the %Cardiovascular System are anemia, heart failure, pericardial effusion, and pulmonary shunt.

#### Tune Circuit
In the tune circuit step, the resistors and capacitors associated with tissue compartments are tuned during stabilization to achieve the mean arterial pressure given in the patient file.

### Preprocess
#### Begin Cardiac Cycle
Cardiac cycle calculations include methodology for updating the driving force (heart contraction and relaxation) of the CV System throughout the duration of a CV cycle (a single heart beat). This includes a set of systolic calculations that updates contractility at the beginning of the cycle to represent a heart contraction. Modifications to heart rate and heart compliance are calculated by BeginCardiacCycle and applied for the remainder of the current cardiac cycle. Changes to things like heart rate and heart contractility can only occur at the top of the current cardiac cycle after the last cardiac cycle has completed. This helps to avoid discontinuous behavior such as the complete cessation of heart function mid-contraction.

#### Calculate Heart Elastance
This method tracks the progress of the current cardiac cycle and modifies the compliance of the left and right heart to drive the cardiovascular circuit. The reduced compliance at the beginning of the cycle acts to increase the pressure, driving flow out of the heart. The compliance is then reduced, allowing flow into the heart. The reduction and increase in compliance represents the systolic and diastolic portions of the cardiac cycle, respectively. The compliance is driven by a Hill-type elastance equation @cite stergiopulos1996elastance.

#### Process Actions
Process Actions modifies the CV parameters and circuit properties due to actions (insults or interventions) specified by the user. The @ref cardiovascular-actions "actions" found in the action process are: CPR, hemorrhage, pericardial effusion, and cardiac arrest.

### Process
The generic circuit methodology developed for the %BioGears Engine is used to solve for the pressure, flow, and volume at each node or path in the equivalent circuit. For more details, see @ref CircuitMethodology.

#### Calculate Vital Signs
This function takes the current timestep&rsquo;s circuit quantities to calculate important system-level quantities for the current time step. The system pressures and flow rates related to shunting are calculated here. In addition, the events hypovolemia, tachycardia, bradycardia, and asystole are triggered in this function.

### Postprocess
The Postprocess step moves everything calculated in Process from the next time step calculation to the current time step calculation. This allows all other systems access to the information when completing their Preprocess analysis for the next time step.

### Assessments
Assessments in %BioGears are data collected and packaged to resemble a report or analysis that might be ordered by a physician. No %BioGears assessments are associated with the %Cardiovascular system.

<br>
<img src="./images/Cardiovascular/CardiovascularDataFlow.png" width="900">
<center>
*Figure 2. The data flow for the CV System consists of Preprocess, Process, and Postprocess. Preprocess determines the circuit element values based on feedback mechanisms and engine actions. Preprocess also updates system-level quantities via the CalculateVitalSigns and cardiac cycle calculations. Process uses generic circuit methodology to solve the CV circuit for pressures, volumes, and flows along the paths and on nodes. Postprocess updates these quantities to the next time step and then begins advancing time, which causes the loop to repeat.*
</center><br>

@anchor cardiovascular-features
Features, Capabilities, and Dependencies
----------------------------------------
### The %BioGears %Cardiovascular Circuit
The %BioGears CV circuit (Figure&nbsp;3) estimates blood pressure, flow, and volume for organs that are represented by several compartments. These compartments are comprised of lumped parameter models that use resistors and capacitors. Inductors may also be used to model inertial effects. The system is discretized into nodes that are connected by paths (see @ref CircuitMethodology). The circuit used to represent the CV System was designed to provide a level of resolution and fidelity that meets the [requirements](@ref MainPageFAQ) of the overall project.

For example, to provide a means for clearing drugs and substances from the bloodstream, the liver and kidneys must have blood flow, pressure, and volume calculations. Another example is the four extremities (right and left arms and legs) that provide extremity hemorrhage capabilities, having been implemented in a previous project (HumanSim: Combat Medic). In this way, the lumped parameter model provides a mechanism for increasing fidelity as required by the anatomic region or physiologic condition being modeled. The large thoracic arteries are lumped together into one &ldquo;Aorta&rdquo; compartment, which is represented by four nodes and three paths. The fidelity of any compartment could be easily improved by increasing the level of discretization. By adding nodes and paths, the %BioGears &ldquo;Aorta&rdquo; could become the &ldquo;Ascending Aorta&rdquo; and &ldquo;Descending Aorta&rdquo; to accommodate the fidelity demands of the other systems. This could provide an opportunity to model more complex geometries and pathologies, such as stenosis. Figure 3 shows the %BioGears cardiovascular circuit. For clarity, the more discretized [renal circuit](@ref renal-circuit) is not shown in this diagram. 
<img src="./images/Cardiovascular/BioGearsCardiovascularCircuit.png" width="900">
<center>
*Figure 3. The cardiovascular circuit consists of nodes that are connected via paths. These segments of nodes and paths are mapped to several compartments which represent the anatomy of the cardiovascular system. The circuit is used to estimate the blood pressure, flow, and volume of these anatomical compartments.*
</center><br>

Nodes serve as the connection points for paths and are the locations at which pressures are measured. Each CV node contains a pressure value, which is given with respect to the atmospheric reference node (indicated in the diagram by the equipotential symbol). Paths contain information about the flow (volume per time). The %BioGears @ref CircuitMethodology document contains more information about %BioGears circuit definitions and modeling. The %BioGears @ref SubstanceTransportMethodology contains more information about the substance transport. In general, nodes contain "across" information and paths contain "through" information.

The elements of the %BioGears CV circuit are used to model the fluid dynamics of the human CV System (hemodynamics). The hemodynamic pressure and flow are calculated from the lumped parameters that are determined by the circuit element. The equations used to calculate pressure and flow are shown below. These equations are automatically generated and solved simultaneously by the %BioGears [Circuit Solver](@ref CircuitMethodology).

Derived values for the hemodynamic parameters are available, particularly for specific vessel segments @cite olufsen2004deriving @cite rideout1972cardiovascular. However, in the %BioGears Engine, these parameters are tuned to provide the physiologic response of a given [patient profile](@ref PatientMethodology). We completed the tuning process by choosing estimates for each parameter value based on the existing system values (Body and HumanSim) or based on physiologic data. We then analyzed the model outputs and adjusted parameters to obtain organ and system-level outputs that satisfied the validation requirements. The @ref cardiovascular-results "Results and Conclusions" section describes validation in more detail.

### The %BioGears Heart
The %BioGears heart generates pressure that drives the hemodynamics through a variable capacitor that simulates the changing elastance of the myocardium throughout the cardiac cycle. The %BioGears heart has two sides, left and right, simulating the two sides of the human heart. The atria are not included in the heart model; only the ventricular behavior is modeled.

#### Heart Elastance and Compliance
The heart compliance is calculated from the inverse of the heart elastance. The heart elastance model used in %BioGears is adapted from the one developed by Stergiopulos&nbsp;et&nbsp;al @cite stergiopulos1996elastance. This model utilizes a double Hill function to represent heart elastance over the cardiac cycle time period. It was chosen due to its ability to scale with increasing or decreasing cardiac cycle times. The functional form for elastance of both left and right ventricles is shown in Equation&nbsp;1 and Equation&nbsp;2.

\f[E_{v} (t)=(E_{\max ,v} -E_{\min ,v} )\left(\frac{f(t)}{f_{\max } } \right)+E_{\min ,v} \f] 
<center>
*Equation 1.*
</center><br>

Where *E<sub>max,v</sub>* is the maximum ventricle elastance in mmHg per mL, *E<sub>min,v</sub>* is the minimum ventricle elastance in mmHg per mL, and *f<sub>max</sub>* is the maximum value of the double Hill over the cardiac cycle length.  The double Hill function *f(t)* is given by

\f[f(t)=\left[\frac{\left(\frac{t}{\alpha _{1} T} \right)^{n_{1} } }{1+\left(\frac{t}{\alpha _{1} T} \right)^{n_{1} } } \right]\left[\frac{1}{1+\left(\frac{t}{\alpha _{2} T} \right)^{n_{2} } } \right]\f] 
<center>
*Equation 2.*
</center><br>

Where &alpha;<sub>1</sub> , &alpha;<sub>2</sub> , *n<sub>1</sub>*, and *n<sub>2</sub>* are shape parameters used to determine the distribution of the double Hill function, *T* is the cardiac cycle time period, and *t* is the current time within the cardiac cycle.

The relationship between the elastance and compliance in %BioGears is shown in Figure&nbsp;4.
<img src="./plots/Cardiovascular/ComplianceVsElastance.jpg" width="1100">
<center>
*Figure 4. The left heart compliance and elastance are shown to be inversely related to each other. The elastance represents the change in pressure per change in volume, while the compliance is the change in volume per change in pressure. These quantities define the contraction of the heart, which drives the pressure and flow of the cardiovascular circuit.*
</center><br>

#### Heart Pressure, Volume, and Flow
The variable compliance, which is used to model heart contraction and relaxation, yields pressure and volume changes that drive the flow through the CV circuit. This variable compliance driver allows the pressures and volumes to be calculated within the heart, as shown in Figure&nbsp;5.

<img src="./plots/Cardiovascular/PressureVsVolume.jpg" width="1100">
<center>
*Figure 5. Relationship between pressure and volume in the left heart throughout the cardiac cycle. The relaxation of the heart muscle is modeled by increasing the compliance, resulting in an increase in left heart volume with a relatively constant left heart pressure. The contraction is represented by a rapid decrease in the compliance, leading to large pressure increases for small volume additions. This large pressure value drives the fluid out of the heart with flow rates calculated based on the circuit solution.*
</center><br>

A pressure-volume curve is used to represent the evolution of the cardiac cycle from the systolic contraction to disatolic relaxation. The pressure-volume curve for the left ventricle is shown in Figure&nbsp;6. Starting from the bottom left and moving clockwise, the curve demonstrates a rapid increase in pressure with no change in volume. This indicates the systolic contraction of the cardiac cycle. Following this, the pressure declines rapidly as the heart expands during diastole. The last portion of the curve shows decreasing volume at constant pressure. Normally, the pressure would decrease slightly due to the imperfect mitral valve, which does not close instantly. %BioGears uses ideal valves, which close instantaneously, causing the pressure to be maintained as volume decreases.

<img src="./plots/Cardiovascular/PVLoop.jpg" width="400">
<center>
*Figure 6. The pressure-volume curve for the left ventricle is represented as a pressure vs. volume plot. It demonstrates the the contracting and relaxing portions of the cardiac cycle. In addition, the curve demonstrates the use of ideal valves in the %BioGears heart due to instantaneous changes in volume at a set pressure.*
</center><br>

### Drug Effects
As drugs circulate through the system, they affect the %Cardiovascular System. The drug effects on heart rate, mean arterial pressure, and pulse pressure are calculated in the %Drugs System. These effects are applied to the heart driver by incrementing the frequency of the heart based on the system parameter calculated in the %Drugs System. Additionally, the mean arterial pressure is modified by incrementing the resistance of the blood vessels. At each timestep, the resistors are incremented on each cardiovascular path. In the future, the pulse pressure will be modified by changing the heart elastance. However, no drugs in the %BioGears library currently cause pulse pressure changes; therefore, this step has not been necessary. The strength of these effects are calculated based on the plasma concentration and the drug parameters in the substance files.  For more information on this calculation see @ref DrugsMethodology.

### Electrocardiogram
The %BioGears electrocardiogram (%ECG) machine outputs an %ECG waveform to represent the currently supported heart rhythms. The waveform is not a calculated value; the current %BioGears %ECG system does not model the electrical activity of the heart. This data is stored in a text file and interpolated to match the %BioGears timestep. Currently, we support tachycardia, bradycardia, asystole, and normal sinus. Figure 7 shows an example of the %ECG normal sinus output. By allowing the points on the waveform to sum together based on the length of the heart beat compared to the length of the %ECG waveform, a tachycardic waveform will have a compressed P and T wave as expected in the validation data. For asystole, %BioGears outputs 0&nbsp;mV.

<img src="./images/Cardiovascular/BasicECGWaveform.png" width="300">
<center>
<i>Figure 7. The %ECG system produces a normal sinus waveform with the expected features.</i>
</center><br>

@anchor cardiovascular-variability
### Patient Variability
The state of the cardiovascular system is heavily dependent on the patient. The cardiovascular circuit parameters are all set based on the patient configuration. For example, the proportion of blood flow to the fat compartment depends on the sex of the patient, and the initial flow proportions dictate initial vascular resistance. Likewise, the volume of the fat compartment depends on the sex, body fat fraction, height, and weight of the patient, and the initial volume and pressure are used to compute the initial vascular compliance for paths associated with a compartment. After initial values are computed, the cardiovascular circuit parameters are adjusted to meet target pressures, which are also specified in the patient file. Because the cardiovascular system is profoundly dependent on the patient configuration, several stabilization iterations are sometimes necessary to achieve a specific desired state. For that reason, we make modifications to the parameters to move the circuit closer to a %BioGears Standard Male patient state to speed stabilization for that patient, and other custom states may require longer stabilization periods. A detailed discussion of patient configuration and variability in %BioGears is available in the @ref PatientMethodology report.
@anchor cardiovascular-dependencies
### Dependencies
The other %BioGears systems depend on the CV System to function accurately and appropriately. The CV System provides the medium (blood) and the energy (pressure differentials and subsequent flow) to transport substances throughout the %BioGears system. A complete list of %BioGears substances and their descriptions can be found in the @ref SubstanceTransportMethodology. Similarly, the %BioGears CV System is dependent on all of the other systems. Feedback from other systems influences the cardiac behavior via heart rate and elastance modifiers and the systemic circulation through resistance modifications to the circuits. For example, as the plasma concentration of a drug increases, the elastance of the heart, heart rate, and systemic resistance are modified through the pharmacodynamic effects (multipliers), resulting in changes to the blood flow, pressure, and volume calculations. Additionally, baroreceptor feedback leads to modification of heart rate, elastance, and vascular resistance and compliance in order to regulate the arterial pressure. For more information on baroreceptor feedback see @ref NervousMethodology.

Gas exchange is one area that closely ties the respiratory system, substances, and the CV system together. Gas exchange is calculated based on the partial pressures of the gases in the lungs and the capillary beds (@ref SubstanceTransportMethodology). If respiration was to stop due to a drug effect or physiologic condition, such as airway obstruction, the gas exchange calculations would be affected. Oxygen would stop diffusing into the CV System from the %Respiratory System, resulting in an oxygen deficit in the cardiac tissue.

@anchor cardiovascular-assumptions
Assumptions and Limitations
---------------------------
The constant-compliance equations in the CV System assume a linear diastolic pressure-volume relationship in the vessels (&Delta;V = C&Delta;P) representative of an elastic vessel wall. This assumption is appropriate for many blood vessels under normal physiologic conditions; however, research shows that many vessel walls are more appropriately modeled as visco-elastic walls @cite alastruey2011visco. While the non-linear pressure-volume relationship within the heart is modeled using a variable capacitor, the visco-elastic nature of the blood vessels is not addressed in this model. Rigid and elastic wall properties have been used to model the hemodynamic behavior in high-fidelity models with success @cite alastruey2011visco. Therefore, the level of fidelity of the lumped parameter model and resulting simulation support the use of elastic walls.

The CV model has a waveform limitation because of our ideal diode model. We assume the valves are perfect Booleans. In reality, there is a transition time between the open and closed states of a valve. During this time, a small amount of retrograde flow may be present @cite halliwill2010retro . In the future, we hope to incorporate retrograde flow through a Zener diode. However, the current methodology does allow retrograde flow at any location without a diode. The Zener diode implementation will provide a mechanism for future modeling efforts, including abnormal heart valves that result in significant retrograde flow.

@anchor cardiovascular-conditions
Conditions
----------
#### Anemia
Anemia conditions reduce the oxygen-carrying capacity of the blood. The %BioGears Engine models iron deficiency anemia as a chronic condition, which is characterized by a decrease in hemoglobin concentration and subsequent decreases in hematocrit and blood viscosity @cite Duke1969Hemodynamic. These factors lead to a decrease in systemic vascular resistance @cite guyton2006medical. The %BioGears Engine currently supports up to a 30% decrease in hemoglobin. After engine stabilization, the chronic condition reduces the hemoglobin throughout the circuit and reduces the systemic vascular resistance to represent the change in viscosity. The engine then re-stabilizes based on the chronic condition criteria. For more information, see @ref SystemMethodology. There is an observable increase in venous return due to the decreased systemic vascular resistance. As validation data supports, there are no observable effects from the decreased oxygen-carrying capacity at rest. These effects will be evident in the future with incorporation of exercise.

#### Arrhythmias
A heart arrhythmia is a deviation from the normal sinus rhythm seen in a healthy heart beat. In %BioGears, we have implemented two chronic arrhythmias, sinus bradycardia and sinus tachycardia. Bradycardia is a low resting heart rate (less than 60&nbsp;beats per minute) @cite mangrum2000evaluation, which is generally considered benign when cardiac output is stable. Tachycardia is a high resting heart rate (greater than 100&nbsp;beats per minute) @cite yusuf2005deciphering. After engine stabilization, the heart rate baseline is modified. After the engine re-stabilizes based on the chronic condition criteria (see @ref SystemMethodology for details), the heart rate value, and corresponding changes in the %ECG waveform, are the only clear sign of the condition. This matches the validation data. A limitation of the current chronic tachycardia model is a lack of changes to the compression force. Currently, only the rate is affected.

#### Heart Failure
Heart failure refers to a malfunctioning of the heart, resulting in inadequate cardiac output. The current mode of heart failure being modeled in %BioGears is chronic left ventricular systolic dysfunction. After engine stabilization, the chronic heart failure is modeled with a 45% reduction in the contractility of the left heart. After the engine has re-stabilized using the chronic condition criteria (@ref SystemMethodology), the heart ejection fraction of 60% is reduced to 31% as noted in the validation data @cite chatterjee2007systolic. The reduced contractility leads to a decrease in heart stroke volume, which causes an immediate drop in the cardiac output and arterial pressures. The baroreceptor reflex responds with an increase in heart rate in an attempt to return mean arterial pressure to its normal value.

#### Pericardial Effusion
A chronic pericardial effusion is an accumulation of fluid in the pericardial sac that surrounds the heart. This accumulation occurs over a long period of time, resulting in the body making some accommodation to these changes. The result is an increased pressure on the heart, which leads to strained performance. This performance strain is less severe than in an acute effusion. Due to this increasing pressure, there is a decrease in stroke volume, which leads to a decrease in cardiac output and arterial pressures. To compensate for reduced stroke volume, the baroreceptor response raises the heart rate in an attempt to restore cardiac output.

@anchor cardiovascular-actions
Actions
-------
### Insults

#### Cardiac Arrest
The cardiac arrest action is used to externally initiate a cardiac arrest event (see @ref cardiovascular-arrest "Cardiac Arrest" event below). The cardiac arrest event is a cessation of all cardiac and respiratory function. The cardiac arrest event can be triggered by systems of the biogears engine when physiological thresholds are reached (physiological cardiac arrest), or it can be triggered by the cardiac arrest action (interface-initiated cardiac arrest). The cardiac arrest insult can be thought of as an idiopathic sudden cardiac arrest.

#### Hemorrhage
A hemorrhage is a substantial blood loss that may lead to inadequate supply of oxygen to tissues and vital organs. Reduced blood volume leads to reduced cardiac output, reduced mean arterial pressure, and an increase in heart rate due to baroreceptor response.  Hemorrhages are initiated in %BioGears by specifying a physiological compartment and a severity (on a scale of 0 to 1).  Table 1 identifies the compartments which currently support hemorrhages.  Looking to the potential inclusion of the Hemorrhage Action in a broader Injury Model, %BioGears Hemorrhage scenarios output an injury code corresponding to the Military Combat Injury Scale (MCIS).  Furthermore, the %BioGears Software Development Kit (SDK) demonstrates a method for initiating a Hemorrhage via the MCIS.  An MCIS code contains five digits, the first of which defines the wound severity (on a scale of 1-5).  The second digit defines the region in which the wound occured (Head, Torso, Arm, Leg, Multiple), while the third through fifth digits provide further specificity (i.e. tissue type, organ, etc).  Wound resolution in %BioGears is not detailed enough to require all five digits; in instances where digits are not required, they are simply set to 0.  Table 1 shows sample injury codes for each compartment that supports hemorrhages, as well as information about how the code is derived.  For a complete description of the development of the MCIS, see @cite lawnick2013combat (list of sample codes in Supplemental Digital Content 3).


<center>
*Table 1 . Valid compartments in which hemorrhages can be defined in %BioGears (Column 1) with sample injury code output (Column 2, note that S = severity)) and derivation of code (Columns 3-5)*
</center>

|%BioGears Compartment		|Injury Code|	Region (Digit 2)	|	Subregion (Digit 3)	|	Specific Site (Digit 4)	|	
|------------------------	|-----------|	--------------------|	--------------------|	-----------------------	|
|	Head					| S1610		|	Head				|	Vessels				|	Cranial					|
|	Major Artery			| S2640		|	Torso				|	Vessels				|	Arterial				|
|	Vena Cava				| S2660		|	Torso				|	Vessels				|	Other vessel (Assign to Vena Cava)	|
|	Lung					| S2710		|	Torso				|	Chest Organ			|	Lung					|
|	Myocardium				| S2720		|	Torso				|	Chest Organ			|	Myocardium				|
|	Liver					| S2810		|	Torso				|	Abdominal Organ		|	Liver					|
|	Spleen					| S2820		|	Torso				|	Abdominal Organ		|	Spleen					|
|	Splanchnic				| S2830		|	Torso				|	Abdominal Organ		|	Pancreas				|
|	Kidney					| S2840		|	Torso				|	Abdominal Organ		|	Kidney					|
|	Small Intestine			| S2850		|	Torso				|	Abdominal Organ		|	GI Tract (Assign to Small Intestine)|
|	Large Intestine			| S2860		|	Torso				|	Abdominal Organ		|	Other (Assign to Large Intestine)	|
|	Arm						| S3000		|	Arm					|	----				|	----					|
|	Leg						| S4000		|	Leg					|	----				|	----					|



During initialization, circuit pathways with open switches connecting each compartment to ground are defined to provide potential bleeding routes.  The compartment provided in the Hemorrhage Action is then mapped to the appropriate pathway, closing the switch and adjusting its resistance to flow as a function of wound severity.  Multiple hemorrhages may exist at any given time. Bleeding rates--from which the total blood volume is calculated--are obtained by sampling the flows from relevant circuit pathways to ground.  Figure 8 highlights the pulsatile nature of the bleeding rate, which trends downward as the total blood volume decreases.  Concurrently, arterial pressures and cardiac output drop. The baroreceptor reflex initiates an increase in heart rate to compensate for the reduced mean arterial pressure.  

Associated with the decrease in total blood volume is a decrease in blood-borne substances that will be linearly proportional to the bleed rate. For more specific information regarding these substances and their loss due to bleeding, see @ref BloodChemistryMethodology and @ref SubstanceTransportMethodology. Figure 8 shows the blood volume and hemoglobin content before, during, and after a Class 2 hemorrhage (15-30% blood loss @cite garrioch2004body) event with no intervention other than the cessation of hemorrhage. Figure 9 shows a hemorrhage event with subsequent saline administration. Note that the hemoglobin content remains diminished as the blood volume recovers with IV saline. By comparison, [Figure 12](@ref cardiovascular-blood-administration) shows a blood-product intervention following a hemorrhage event. In that figure, the hemoglobin increases with the blood infusion.

<table>
<tr>
<td><img src="./plots/Cardiovascular/Class2NoFluid_BloodVolume.jpg" width="550">
</td>
<td><img src="./plots/Cardiovascular/Class2NoFluid_Hb.jpg" width="550">
</td>
</tr>
</table>
<img src="./plots/Cardiovascular/Class2NoFluid_Legend.jpg" width="500">
<center>
*Figure 8. Blood volume and hemoglobin content before, during, and after a Class 2 hemorrhage event with no subsequent intervention.*
</center><br>

<table>
<tr>
<td><img src="./plots/Cardiovascular/Class2Saline_BloodVolume.jpg" width="550">
</td>
<td><img src="./plots/Cardiovascular/Class2Saline_Hb.jpg" width="550">
</td>
</tr>
</table>
<img src="./plots/Cardiovascular/Class2Saline_Legend.jpg" width="500">
<center>
*Figure 9. Blood volume and hemoglobin content before, during, and after a Class 2 hemorrhage event with a subsequent infusion of saline.*
</center><br>

#### Pericardial Effusion
The pericardial effusion action is used to model acute pericardial effusion by adding a flow source on the pericardium. This action leads to a volume accumulation over the course of the simulation. The accumulated volume is used to calculate a pressure source that is applied to the left and right heart. This pressure source is identical to the one used in the pericardial effusion condition. For the pericardial effusion action, the strain-rate dependent compliance of the pericardium is modeled so that the change in intrapericardial pressure is a function of flow rate and the current volume of the pericardium @cite Metoyer2014Modeling.

### Interventions

#### Cardiopulmonary Resuscitation (CPR)
CPR can be preformed to maintain some perfusion through the induced hemodynamic action. The American Heart Association recommends performing chest compressions at a rate of 100&nbsp;per minute with enough force to achieve a chest-compression depth of 5&nbsp;cm @cite berg2010adult. Gruben et al estimated a required force of approximately 400&nbsp;N to achieve the required depth @cite gruben1990system. Kim et al found that stroke volumes of 12&ndash;35&nbsp;mL and an effective cardiac output of 1200&ndash;3500&nbsp;mL/min could be achieved with the proper compression technique. The aforementioned hemodynamics are accompanied by an increase in systolic and diastolic pressures and, by extension, mean arterial pressure @cite kim2008direction.

Chest compressions are simulated in the %BioGears Engine by adjusting the value of the pressure source that is connected between the common equipotential node and the capacitors that represent the right and left cardiac compliances. The pressure source represents the intrathoracic pressure, thus a positive pressure value represents an increase in intrathoracic pressure. The pressure on the source is calculated from the input force and the biometrics of the patient.

The CPR force of compression can be expressed to the %BioGears Engine in one of two ways. A direct application method allows a user to explicitly set the applied force at the time of the CPR compression call. Successive compressions are achieved by calling a CPR compression at a desired force, calling for an advance in simulation time, and then calling the next desired force. In this way, the shape of the time-dependent force curve is explicitly set by the user. The other method of applying a compression force during CPR is to use the CPR force scale. In this method, the user can call for a compression by selecting a scale value between 0 and 1, with 0 being no force and 1 being the maximum possible force. The maximum possible compression force, currently 500&nbsp;N, is based on the work of Arbogast et al @cite arbogast2006anterior. When the force scale is used, the evolution of the force curve in time is set by the %BioGears engine. A bell shaped force curve is used in accordance with @cite stanley2012recreating. Once a force scale compression is called for, another compression will not be possible until the current compression has fully evolved. A new explicit force will also be prohibited until the compression is complete. In the current %BioGears Engine, recovery from cardiac arrest is not possible as the only arrest rhythm is asystole. Consequently, it is not possible to reverse a cardiac arrest event with defibrillation. Of course, it is never possible to convert an ineffective rhythm with CPR alone.

The compression-only CPR in the %BioGears Engine is consistent with AHA guidelines for lay rescuer adult CPR @cite hazinski2010. The addition of rescue breathing for health-care provider CPR is mentioned as a recommended improvement to the %BioGears Engine.

#### Hemorrhage Cessation
A &ldquo;tourniquet&rdquo; may be applied to a bleeding distal portion of the body to reduce blood flow entering that portion and effectively stopping the bleeding. With the bleeding managed, vital signs should return to their normal state after a sufficient period of time. A &ldquo;tourniquet&rdquo; may be simulated in the %BioGears Engine by creating a Hemorrhage action in the bleeding compartment with a decreased severity.  A hemorrhage can be stopped completely by entering a severity of 0.  If the hemorrhage leads to significant blood loss, reducing the severity of that hemorrhage will lead to a return of the arterial pressures as the baroreceptor reflex (which is not affected by blood loss) exerts itself. As the MAP increases, heart rate will begin to decrease in proportion to the error control dictated by the baroreceptor reflex. Lastly, cardiac output will begin to increase to a new, stabilized value associated with the new total blood volume.

#### Tranexamic Acid
Tranexamic Acid (TXA) may be injected into the body via substance infusion to effectively reduce hemorrhage bleeding over time. Pharmacodynamics of TXA show quick dispersion in the blood with very quick effects taking place at the effect site. Depending on the severity and location of the hemorrhage, TXA increases the chance of survival by slowing the bleeding either entirely or enough to get the patient help. With the bleeding managed, vital signs should eventually return to their normal state. If the hemorrhage is stopped completely, there will be a return of the arterial pressures as the baroreceptor reflex (which is not affected by blood loss) exerts itself. As the MAP increases, heart rate will begin to decrease in proportion to the error control dictated by the baroreceptor reflex. Lastly, cardiac output will begin to increase to a new, stabilized value associated with the new total blood volume.

#### Intravenous Fluid Administration
Intravenous fluid administration is a continual injection of a compatible fluid into the veins of a patient that has suffered from fluid loss. Due to increasing volume from intravenous fluid administration, blood volume and arterial pressures will increase. Stroke volume will increase due to increased venous return, which will cause an increase in cardiac output. The baroreceptor response will lead to a decrease in the heart rate.

Intravenous fluid administration is simulated by applying a flow source to the vena cava. The flow source and duration of the administration is dictated by the user in the form of a flow rate and IV bag volume. This results in an increase in total blood volume, heart stroke volume, cardiac output, and arterial pressures. Due to increasing arterial pressures, the baroreceptor reflex will begin to decrease heart rate according to the error between the mean arterial pressure and its set point. Additional effects may occur depending on the type of fluid administered. Currently, the user may administer blood or saline. Each of these fluids is considered a compound substance with multiple substance components. Further information on the effect of substances during fluid administration can be found in @ref SubstanceTransportMethodology.

@anchor cardiovascular-events
##Events

### Asystole
Asystole is defined as a cessation of the heart beat. Asystole is currently triggered by an oxygen deficit in the myocardium (a partial pressure for oxygen less than 25&nbsp;mmHg) or by a heart rate of less than 26&nbsp;beats per minute @cite guinness2005lowest. The %BioGears Engine continues to process during Asystole, which allows for resuscitation efforts, such as CPR, to be attempted. If the Asystole is not addressed, the engine will reach an irreversible state due to a lack of oxygen in the brain. More detail on oxygen deficits can be found in @ref BloodChemistryMethodology.

@anchor cardiovascular-arrest
### Cardiac Arrest
Cardiac Arrest is a condition in which the pumping of the heart is no longer effective @cite nhlbi2011sca. The Cardiac Arrest event can be triggered in the %BioGears engine either by the Cardiac Arrest action or by the evolution of engine physiology. For instance, an oxygen deficit in the heart muscle will trigger both an asystole event (see below) and a cardiac arrest event. In the current version of the %BioGears engine, the only rhythm associated with cardiac arrest is asystole, but the cardiac arrest event is included to facilitate control and to allow the future inclusion of other ineffective rhythms such as ventricular fibrillation. In the current %BioGears engine, it is not possible to recover from cardiac arrest. It is, however, possible to maintain some perfusion by performing chest compressions (see CPR).

### Cardiogenic Shock
In general, the term "shock" refers to inadequate perfusion of the tissues. The several categories of shock serve to signify the origin of the disturbance. Cardiogenic shock is inadequate perfusion due a reduction in the pumping capability of the heart. In %BioGears, the Cardiogenic Shock event is activated when the cardiac index () is below 2.2 (L/min-m^2) *and* the systolic blood pressure is less than 90.0 (mmHg) *and* the pulmonary capillary wedge pressure is greater than 15.0 (mmHg) @cite dhakam2008review.

### Hypovolemia
Hypovolemia is defined as a reduction in total blood volume by 35&nbsp;percent @cite guyton2006medical. Typically, this is classified by elevated heart rate and decreased arterial pressure. In %BioGears, hypovolemia is triggered during a hemorrhage when blood volume has fallen below 65&nbsp;percent of its normal value. 

@anchor cardiovascular-results
Results and Conclusions
=======================
The %BioGears %Cardiovascular System was validated quantitatively and qualitatively under resting physiologic conditions and transient (action-induced) conditions. The validation is specified with a color coding system, with green indicating good agreement with trends/values, yellow indicating moderate agreement with trends/values, and shades red indicating poor agreement with trends/values. 

@anchor cardiovascular-validation-resting
Validation - Resting Physiologic State
--------------------------------------
Validation results for system and compartment quantities are listed in Tables&nbsp;1 and&nbsp;2. System-level quantities show favorable agreement with validation values. Heart rate, arterial pressures, blood volume, heart stroke volume, and cardiac output are the predominant CV System quantities. These values agree, on average, within ~8 percent of the expected values for the healthy standard patient. 

<br><center>
*Table 2. Validation of the resting physiologic state comparison of system-level outputs from %BioGears to referenced values. System-level outputs show favorable agreement with validation data.*
</center>
@insert doc/validation/CardiovascularValidationTable.md

<br><center>
*Table 3. Validation of the resting physiologic state comparison of compartment-level outputs from %BioGears to referenced values. The compartments are currently validated on a flow/volume basis. Flows and most of the volumes show good agreement with validation values.*
</center>
@insert doc/validation/CardiovascularCompartmentsValidationTable.md

Compartment-level quantities show reasonable agreement with the validation values. All of the flows match the reference values within ~10&nbsp;percent. The volumes show some moderate differences for a few specific compartments. The aorta compartment volume is much smaller than the validated value. The compliance on this compartment needed to remain low in order to preserve the arterial pressure waveform, which led to less volume than expected. Similarly, the vena cava compliance was set in order to maintain the correct cardiac output and arterial pressures; therefore, its expected volume was limited. The right heart pressures and volumes show some disagreement with the validation data. The minimum values for right heart pressure and volume are much lower than valid ranges. This is due to restriction of unstressed volume in the right heart, which currently has an unstressed volume of zero. An increase in unstressed volume would shift the pressure volume minimums up, while also preserving the maximum values within their respective ranges. The %Cardiovascular System is tuned to vitals output validation (Table&nbsp;2), as well as good agreement with insults' and interventions&rsquo; expected trends and values (see the following section).  In addition, compartment validation was achieved on a reasonable level.

The arterial pressure waveform was validated according to the plot shown in Figure&nbsp;10. It displays the %BioGears arterial pressure against measured arterial pressure. The diastolic and systolic pressures were validated using data shown in Table&nbsp;1. To validate the waveform shape and demonstrate the overall feature match of the %BioGears pressure waveform with the  validation data, we used a waveform from PhysioNet @cite goldberger2000physiobank . However, the patient heart rate and parameters are slightly different than the %BioGears patient. This led to timing discrepancies and differences in the diastolic and systolic pressures. To demonstrate the waveform feature matching, a separate axis is used for each data set. In the future, we may create a patient more representative of the one used for the PhysioNet waveform. This would show both the ability of the engine to model different patients and the pressure waveform feature matching.  The shapes of both waveforms match well, showing rapid pressure increases as the heart contraction begins to occur. The main difference in the shape of each plot is the small pressure oscillations that occur after the initial pressure drop in the validation data. This is the dicrotic notch,  which occurs from slight flow reversal from the aorta back into the left ventricle before the valves close @cite korakianitis2005notch . This is not currently being modeled in %BioGears, but improvements in the circuit model, including the addition of inertance and diodes that allow retrograde flow, will likely enable the %BioGears waveform to capture more detail.

<center>
<table>
<tr>
<td><img src="./plots/Cardiovascular/BioGearsPressure.jpg" width="400"></td>
<td><img src="./plots/Cardiovascular/PhysioNetPressure.jpg" width="400"></td>
</tr>
</table>
<i>Figure 10. Arterial pressure waveform comparisons. The diastolic and systolic pressures were validated using the data shown in Table&nbsp;1. To validate the waveform shape and demonstrate the overall feature match of the %BioGears pressure waveform with the  validation data, a waveform was found on PhysioNet @cite goldberger2000physiobank . However, the patient heart rate and parameters are slightly different than the %BioGears patient. This led to timing discrepancies and differences in the diastolic and systolic pressures. To demonstrate the waveform feature matching, a separate axis is used for each data set. Both the validation waveform and the %BioGears waveform show sharp increases in pressure during the systolic period. After the contraction occurs, the pressure begins decreasing and that is where the main difference in the engine and the validation data occur. There is a dip and subsequent rise in the arterial pressure that occurs due to the dicrotic notch, which the engine does not capture.</i>
</center><br>

@anchor cardiovascular-validation-conditions
Validation - Actions and Conditions
--------------------
All actions in the CV System were validated. A summary of this validation is shown in Table&nbsp;4. More details on each individual scenario's validation can be found below.

<center>
*Table 4. Actions associated with the %Cardiovascular System were validated qualitatively by comparing the engine output to expected trends. Engine results show favorable agreement (green), some agreement (yellow), or no agreement (red). The table shows the total number of results analyzed for each scenario. Results mostly matched expected trends.*
</center>

|	Scenario 	|	Description	|	Validation Type	|	Good	|	Decent	|	Bad	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Pericardial Effusion - Chronic	|	Patient has an effused pericardium with an accumulated volume of 500 ml.	|	Qualitative	|<span class="success">	9	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Ventricular Systolic Failure	|	Chronic heart failure is initiated, which causes a permanent reduction in heart contractility for the scenario duration.	|	Qualitative	|<span class="success">	8	</span>|<span class="warning">	0	</span>|<span class="danger">	1	</span>|
|	Hemorrhage Class 2 - Blood	|	Bleed occurring in an artery, with a blood transfusion occurring afterwards.	|	Qualitative	|<span class="success">	18	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Hemorrhage Class 2 - Saline	|	Bleed occurring in an artery, with intravenous saline administration occurring afterwards.	|	Qualitative	|<span class="success">	15	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Hemorrhage Class 2 - No Fluid	|	Bleed occurring in an artery.	|	Qualitative	|<span class="success">	10	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Hemorrhage Class 4 - No Fluid	|	Bleed occurring in an artery.	|	Qualitative	|<span class="success">	5	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Hemorrhage - Combat Medic 1	|	Bleeding in the right leg and vena cava. After bleeding has stopped, intravenous saline is administered.	|	Qualitative	|<span class="success">	21	</span>|<span class="warning">	3	</span>|<span class="danger">	1	</span>|
|	Hemorrhage - Combat Medic 2	|	Bleeding in the right leg and vena cava. After bleeding has stopped, a bolus morphine injection is given and intravenous saline is administered.	|	Qualitative	|<span class="success">	21	</span>|<span class="warning">	7	</span>|<span class="danger">	2	</span>|
	Hemorrhage-TXA Intervention	|	Bleeding in the right leg followed by infusion of intravenous tranexamic acid (TXA)	|	Qualitative	|<span class="success">	8	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Pulmonary Shunt - 10%	|	A pulmonary shunt occurs, which reduces the effective oxygen transfer to the pulmonary capillaries.	|	Qualitative	|<span class="success">	1	</span>|<span class="warning">	2	</span>|<span class="danger">	1	</span>|
|	Pulmonary Shunt - 30%	|	A pulmonary shunt of increasing severity occurs, which reduces the effective oxygen transfer to the pulmonary capillaries.	|	Qualitative	|<span class="success">	1	</span>|<span class="warning">	1	</span>|<span class="danger">	2	</span>|
|	IV Fluids	|	Intravenous saline administration on a healthy patient.	|	Qualitative	|<span class="success">	6	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Sinus Bradycardia	|	Heart rate set to 50 beats per minute.	|	Qualitative	|<span class="success">	5	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Sinus Tachycardia	|	Heart rate set to 140 beats per minute.	|	Qualitative	|<span class="success">	5	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Anemia - 30%	|	Hemoglobin content reduced by 30  percent.	|	Qualitative	|<span class="success">	7	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	CPR 	|	Cardiac arrest is initiated, and CPR is performed.	|	Qualitative	|<span class="success">	16	</span>|<span class="warning">	6	</span>|<span class="danger">	2	</span>|


### Cardiopulmonary Resuscitation (CPR)
There are two CPR scenarios for validation. The first scenario validates the CPR methodology using the explicit force setting functionality, and the second scenario uses the force scale methodology (see CPR intervention documentation above). In both scenarios, cardiac arrest is initiated externally. Ten seconds later, compressions begin. In both scenarios, the compression rate is set to 80 per minute, and the force is set to 70 pounds to match the conditions in @cite redberg1993physiology. Supplemental literature sources were used to validate outputs not available in @cite redberg1993physiology. All of the physiological variables were within validation ranges in both scenarios with the exception of mean arterial pressure and ejection fraction. The mean arterial pressure in the %BioGears engine is slightly higher than expected. This is most likely due to the fact that the intravascular pressures are higher than those reported in @cite redberg1993physiology. However, the %BioGears pressures are within ranges reported in other references @cite kim2008direction , @cite gruben1990system. The ejection fraction is considerably lower in %BioGears during CPR than the value reported in @cite kim2008direction. %BioGears ejection fraction is lower because blood tends to pool in the %BioGears right heart during cardiac arrest. The validation failures that occur right at cardiac arrest are mostly due residual dynamics following asystole in %BioGears. Errors associated with the cessation of heart function in %BioGears are a known issue, and resolving this issue is a part of the cardiac arrest recommended improvements discussed [below](@ref cardiovascular-future).  

<br><center>
*Table 5. Cardiopulmonary resuscitation (CPR) validation results.*
</center>
|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate(beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Stroke Volume (mL)	|	Carotid Artery (Brain) Flow (mL/min)	|	Ejection Fraction (%)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Cardiac Arrest	|	30	|	35	|<span class="success">	0	</span>|<span class="success">	N/A	</span>|<span class="success">	N/A	</span>|<span class="success">	33 +/- 10 @cite kim2008direction %BioGears 96	</span>|<span class="warning">	0	</span>|<span class="warning">	0	</span>|<span class="warning">	Approach 0 	</span>|<span class="warning">	0	</span>|
|	Chest Compressions	|	40	|	165-170	|<span class="success">	80 per minute [Direct calculation] %BioGears 80	</span>|<span class="success">	39.27 @cite redberg1993physiology Approx. 70-80 @cite gruben1990system 105 +/- 41 @cite kim2008direction %BioGears 43.14	</span>|<span class="success">	13.97 @cite redberg2014physiology Approx. 40 @cite gruben1990system 33 +/- 10 @cite kim2008direction %BioGears 30	</span>|<span class="warning">	21.13 @cite redberg1993physiology %BioGears 34.12	</span>|<span class="success">	17-27% Normal @cite kim2008direction %BioGears 27%	</span>|<span class="success">	19.7 @cite redberg2014physiology 25 +/- 8 @cite kim2008direction %BioGears 21	</span>|<span class="success">	> 0 during compression (translated from dog study) @cite rudikoff1980mechanisms %BioGears 3.5 ml/s Average	</span>|<span class="danger">	34 +/- 16 @cite kim2008direction %BioGears 4%	</span>|
|	Chest CompressionsForce Scale	|	40	|	165-170	|<span class="success">	80 per minute [Direct calculation] %BioGears 80	</span>|<span class="success">	39.27 @cite redberg1993physiology Approx. 70-80 @cite gruben1990system 105 +/- 41 @cite kim2008direction %BioGears 40	</span>|<span class="success">	13.97 @cite redberg1993physiology Approx. 40 @cite gruben1990system 33 +/- 10 @cite kim2008direction %BioGears 28	</span>|<span class="warning">	21.13 @cite redberg1993physiology %BioGears 33	</span>|<span class="success">	17-27% Normal @cite kim2008direction %BioGears 25%	</span>|<span class="success">	19.7 @cite redberg2014physiology 25 +/- 8 @cite kim2008direction %BioGears 20.20	</span>|<span class="success">	> 0 during compression (translated from dog study) @cite rudikoff1980mechanisms %BioGears 3.5 ml/s Average	</span>|<span class="danger">	34 +/- 16 @cite kim2008direction %BioGears 4%	</span>|


### Heart Failure
The heart failure scenario has a ventricular systolic dysfunction condition applied to the patient prior to stabilization. The effect is modeled as a chronic condition, meaning there is a permanent change in the heart contractility for the entirety of the&nbsp; scenario duration. After sustaining heart failure for 20 minutes the patient then receives a diuretic to reduce congestion in the body. This follows a standardized treatment of heart failure patients and displays %BioGears' ability to not only simulate the problem but also to provide and investigate the solution. The PD effects of furosemide are detailed in the @ref DrugsMethodology, but the results of increased urine production are seen here on the total blood volume of the patient. The heart failure scenario was validated qualitatively, with only the diastolic pressure failing to match the expected behavior.

<img src="./plots/Cardiovascular/HeartFailure.jpg" width="1100">
<center>
<i>Figure 11. Total blood volume due to the localized effect on the renal tubular lumen can be seen here after administration of the diuretic furosemide.</i>
</center><br>

<br><center>
*Table 6a. Heart failure validation results (Condition).*
</center>
|	Condition	|	Sampled Scenario Time (min)	|	Heart Ejection Fraction	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|	Pulmonary Capillaries Wedge Pressure (mmHg)	|	Oxygen Saturation	|	Systemic Vascular Resistance	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Ventricular Systolic Failure	|	20	|<span class="success">	Decrease @cite klabunde2012cvconcepts 31% @cite chatterjee2007systolic	</span>|<span class="success">	Mild Increase (89 +/- 22) @cite gheorghiade2009acute	</span>|<span class="success">	No change, increase, or decrease @cite gheorghiade2009acute	</span>|<span class="danger">	No change, increase, or decrease @cite gheorghiade2009acute	</span>|<span class="success">	Decrease @cite gheorghiade2009acute	</span>|<span class="success">	Decrease @cite gheorghiade2009acute	</span>|<span class="success">	Increase @cite gheorghiade2009acute	</span>|<span class="success">	NC	</span>|<span class="success">	Increase @cite klabunde2012cvconcepts	</span>|


<br><center>
*Table 6b. Heart failure validation results (furosemide administration).*
</center>
|	Action	|	Sampled Scenario Time (min)	|	Urine Production Rate (mL/min)	|	Chloride Excretion (mmol/min)	|	Sodium Excretion (mol/min)	|	Vascular Volume (mL)	|
|	------------------------	|	------------------------	|	---	|	---	|	---	|	---	|
|	Furosemide Administration	|	120	|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="success">	Decrease (around 500-1000 mL) @cite hammarlund1985acute	</span>|


### Hemorrhage
The hemorrhage action is tested using several scenarios. The class 2 hemorrhage scenario with blood intravenous (IV) administration begins with a healthy patient. After a few seconds, a severe arterial (severity value = 0.8) hemorrhage is initiated. The hemorrhage continues for ten minutes before the bleeding is stopped. After two minutes, 500&nbsp;mL of IV blood is administered intravenously over five minutes. The other hemorrhage scenarios are similar but with different subsequent interventions. There are also two multi-compartment hemorrhage scenarios that demonstrate bleeding from other compartments (the spleen and leg). Finally, tranexamic acid is validated as an antifibrinolytic agent based on its ability to reduce bleeding of a hemorrhage. Figure 11 demonstrates the time-evolution of select data, and the validation results are displayed in Tables 7a-h.

The results show decreases in the systolic pressure and minor increases in the diastolic pressure during the course of the hemorrhage. In response to the decreasing arterial pressures, the baroreceptor response raises the heart rate. Note that fluid can shift between the intravascular and extravascular space. This shift is evident in the period between cessation of hemorrhage and the start of the infusion (top-left panel of Figure 11).

Following the completion of the hemorrhage, intravenous blood is administered. The validation of this action can be found in the IV Fluid Administration section, with the exception of hemoglobin content. There will be an increase in hemoglobin content directly proportional to the amount of blood added from the IV.

@anchor cardiovascular-blood-administration
<table>
<tr>
<td><img src="./plots/Cardiovascular/Class2Blood_BloodVolume.jpg" width="550">
</td>
<td><img src="./plots/Cardiovascular/Class2Blood_Diastolic.jpg" width="550">
</td>
</tr>
<tr>
<td><img src="./plots/Cardiovascular/Class2Blood_Hb.jpg" width="550">
</td>
<td><img src="./plots/Cardiovascular/Class2Blood_Systolic.jpg" width="550">
</td>
</tr>
</table>
<img src="./plots/Cardiovascular/Class2Blood_Legend.jpg" width="1100">
<center>
*Figure 12. The class 2 hemorrhage scenario shows the blood volume decreasing after a severe arterial hemorrhage. The blood hemoglobin content follows this exact trend. At the conclusion of the bleed, the blood volume and hemoglobin are at a lower value. Five hundred (500) milliliters of blood is then administered intravenously over the course of 5&nbsp;minutes. Both the blood volume and hemoglobin content increase linearly with this administration.*
</center>

<br><center>
*Table 7a. Class 2 hemorrhage with blood administration validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|	Respiration Rate (/min)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhage from Aorta	|	Injury Code:  42640 (Severity = 4)			|	30	|	900	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Slight decrease or no change at all @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|
|	Stop Hemorrhage	|	We see an increase in BV because of pressure differentials allowing fluid transfer from tissues. This is not accounted for in the calculated value.			|	900	|	1020	|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Slight increase or no change at all @cite garrioch2004body	</span>|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Start IV Fluids: Blood at 100 mL/min with a 500 mL bag	|				|	1020	|	1420	|<span class="success">	Decrease @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|


<br><center>
*Table 7b. Class 2 hemorrhage with saline administration validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhage from Aorta	|	Injury Code:  42640 (Severity = 4)			|	30	|	900	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Slight decrease or no change at all @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage	|	We see an increase in BV because of pressure differentials allowing fluid transfer from tissues. This is not accounted for in the calculated value.			|	900	|	1020	|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Slight increase or no change at all @cite garrioch2004body	</span>|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|
|	Start IV Fluids: Saline at 100 mL/min with a 500 mL bag	|				|	1020	|	1420	|<span class="success">	Decrease @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|


<br><center>
*Table 7c. Class 2 hemorrhage with no intervention validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhage from Aorta	|	Injury Code:  42640			|	30	|	630	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Slight decrease or no change at all @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage	|	We see an increase in BV because of pressure differentials allowing fluid transfer from tissues. This is not accounted for in the calculated value.			|	630	|	1030	|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Slight increase or no change at all @cite garrioch2004body	</span>|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|


<br><center>
*Table 7d. Class 4 hemorrhage with no intervention validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhage from Aorta	|	Injury Code:  41630			|	30	|	1030	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|



<br><center>
*Table 7e. Multi-compartment hemorrhage with saline administration validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Stroke Volume (mL)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhages in Spleen (Severity 4) and Leg (Severity 4)	|	Injury Codes:  42820, 44000			|	30	|	530	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Reduce bleeding rate in Spleen (Severity 3)	|	Injury Code:  32820			|	530	|	590	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage in Leg	|	Injury Code:  04000			|	590	|	830	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="danger">	Increase @cite garrioch2004body	</span>|<span class="warning">	Decrease @cite garrioch2004body	</span>|<span class="warning">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage in Spleen	|	Injury Code:  02820			|	830	|	1070	|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="warning">	Increase @cite Chalmers1967	</span>|<span class="success">	Decrease @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|<span class="success">	Increase @cite Chalmers1967	</span>|
|	Administer 500 mL of Saline at a rate of 100 mL/min	|				|	1070	|	1570	|<span class="success">	Decrease @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success>	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|


<br><center>
*Table 7f. Multi-compartment hemorrhage with saline and morphine administration validation results.*
</center>
|	Action	|	Notes			|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate Hemorrhages in Spleen (Severity 4) and Leg (Severity 4)	|	Injury Codes:  42820, 44000			|	30	|	530	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Reduce bleeding rate in Spleen (Severity 3)	|	Injury Code:  32820			|	530	|	590	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage in Leg	|	Injury Code:  04000			|	590	|	830	|<span class="success">	Increase @cite garrioch2004body	</span>|<span class="success">	Decrease @cite garrioch2004body	</span>|<span class="danger">	Increase @cite garrioch2004body	</span>|<span class="warning">	Decrease @cite garrioch2004body	</span>|<span class="warning">	Decrease @cite garrioch2004body	</span>|
|	Stop Hemorrhage in Spleen	|	Injury Code:  02820			|	830	|	1070	|<span class="warning">	Decrease @cite Chalmers1967	</span>|<span class="warning">	Increase @cite Chalmers1967	</span>|<span class="warning">	Decrease @cite Chalmers1967	</span>|<span class="warning">	Increase @cite Chalmers1967	</span>|<span class="warning">	Increase @cite Chalmers1967	</span>|
|	Administer bolus of morphine (20 mL) 	|				|	1070	|	1080	|<span class="success">	Mild Decrease @cite Morgan2006Clinical	</span>|<span class="success">	Decrease - dose dependent @cite Morgan2006Clinical	</span>|<span class="success">	Decrease - dose dependent @cite Morgan2006Clinical	</span>|<span class="success">	Decrease @cite Morgan2006Clinical	</span>|<span class="danger">	Decrease @cite Morgan2006Clinical	</span>|
|	Administer 500 mL of Saline at a rate of 100 mL/min	|				|	1080	|	1580	|<span class="success">	Decrease @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|

<br><center>
*Table 7g. Low severity hemorrhage with immediate infusion of tranexamic acid monitored over twelve hours (validation results).*
</center>
|	Action	|	Notes			|	Action Occurrence Time (hr)	|	Sampled Scenario Time (s)	|	Blood Volume-No Intervention	|	Expected Flow Change with TXA	|	Blood Volume-TXA Intervention	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Hemorrhage in Right Leg (5.0 mL/min), begin TXA Infusion	|	Initiate hemorrhage			|	0	|	0	|	5401.7	|	0	|<span class="success">	5401.7 @cite kushioka2017high	</span>|
|		|				|	--	|	2	|	5013.1	|	29.3	|<span class="success">	5285.6 @cite kushioka2017high	</span>|
|		|				|	--	|	4	|	4528.9	|	48.1	|<span class="success">	5047.2 @cite kushioka2017high	</span>|
|		|				|	--	|	6	|	4107.1	|	52.9	|<span class="success">	4859.6 @cite kushioka2017high	</span>|
|		|				|	--	|	8	|	3709.5	|	55.4	|<span class="success">	4674.2 @cite kushioka2017high	</span>|
|		|				|	--	|	10	|	3355.9	|	55.2	|<span class="success">	4505.1 @cite kushioka2017high	</span>|
|		|				|	--	|	12	|	3047	|	55.6	|<span class="success">	4342.0 @cite kushioka2017high	</span>|

<br><center>
*Table 7h. Chance of survival increase when applying TXA as tested by time of survival during an extreme hemorrhage with and without intervention (validation results).*
</center>
|	Hemorrhage Survival Time (s)	|	TXA Survival Time (s)	|	Percent Change	|	Time To Death	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	4691.4	| 5341.2 | +13.9 |<span class="success">	Increase @cite roberts2013crash	</span>|


### Pericardial Effusion
The pericardial effusion scenario has a chronic effusion applied to the patient with a volume accumulation on the pericardium of 500&nbsp;milliliters. There is a decrease in stroke volume, arterial pressures, and cardiac output. This is due to increasing intrapericardial pressure leading to a reduction in end diastolic volume. The validation trends somewhat follow this same behavior. 

<br><center>
*Table 8. Pericardial Effusion validation results.*
</center>
|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Heart Rate (/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Cardiac Output (mL/min)	|	Heart Stroke Volume (mL)	|	Pericardium Pressure (mmHg)	|	Pericardium Volume (mL)	|	Pulmonary Capillaries Wedge Pressure (mmHg)	|	Oxygen Saturation	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Pericardial Effusion Condition	|	Volume accumulation on pericardium is set at 500 ml.	|	120	|<span class="success">	Slight increase or no change @cite spodick1998tamp	</span>|<span class="success">	Slight decrease @cite spodick1998tamp	</span>|<span class="success">	Slight decrease @cite spodick1998tamp	</span>|<span class="success">	Decrease @cite spodick1998tamp	</span>|<span class="success">	Decrease @cite spodick1998tamp	</span>|<span class="success">	Increase @cite spodick1998tamp	</span>|<span class="success">	Increase @cite spodick1998tamp	</span>|<span class="success">	Slight increase @cite spodick1998tamp	</span>|<span class="success">	NC 	</span>|

### IV Fluid Administration
IV fluid administration was validated by administering 500&nbsp;mL of saline over the course of five minutes. This action was validated qualitatively by a subject matter expert and through direct calculation. The engine output followed expected trends. The subject matter expert predicted an increasing arterial pressure and blood volume. The expected total blood volume increase was quantified by directly adding the volume of saline to the total blood volume. The actual increase in blood volume is slightly less than direct addition due to fluid shifting to the extravascular space. This shift is slightly exaggerated because the pressure differential caused by the saline administration neglects the osmotic pressure gradient. This is [known](@ref tissue-future) deficiency that will be corrected in a future release. Overall, the IV fluid administration action showed excellent agreement with the subject matter expert's predicted trends.

<br><center>
*Table 9. IV saline administration validation results.*
</center>
|	Action	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate	|	Cardiac Output	|	Diastolic Pressure (mmHg)	|	Systolic Pressure (mmHg)	|	Stroke Volume (mL)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Administering 500 mL of Saline at a rate of 100 mL/min	|	30	|	570	|<span class="success">	Minor Decrease  @cite metoyer2016SME	</span>|<span class="success">	Increase  @cite metoyer2016SME	</span>|<span class="success">	Mild Increase @cite metoyer2016SME	</span>|<span class="success">	Mild Increase @cite metoyer2016SME	</span>|<span class="success">	Increase @cite metoyer2016SME	</span>|

### Sinus Bradycardia
Sinus bradycardia was validated by setting the baseline heart rate to 50&nbsp;beats per minute. The condition was allowed to stabilize in keeping with chronic condition  methodology; for more information, see @ref SystemMethodology. The cardiac output remains relatively unchanged due to compensatory increases in the stroke volume, which is expected in asymptomatic sinus bradycardia @cite mangrum2000evaluation. The validation data that was found for  systolic and diastolic pressures was vague, only mentioning that the condition was often asymptomatic, indicating relatively normal pressure values. The systolic and diastolic pressures in %BioGears do change slightly as the heart driver accommodates the increased stroke volume; however, the values remain within normal bounds at 50&nbsp;beats per minute. The %ECG waveform in bradycardia is similar to a normal sinus waveform, with the exception of an extended R-R interval due to a slower heart rate (see Figure 12), which is also seen in the %BioGears output. Other systemic data is not significantly changed.

@image html SinusBradycardia_ECGPedia.png
@image html SinusBradycardia_BioGears.PNG
<center>
<i>Figure 13. The increased R-R interval is evident in both waveforms. This is the primary indication of the low heart rate. Validation image courtesy of @cite vanderBilt2010sinus .</i>
</center><br>

<br><center>
*Table 10. Bradycardia validation results.*
</center>
|	Condition	|	Notes	|	Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate (beats/min)	|	Mean Arterial Pressure (mmHg)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation (mmHg)	|	Cardiac Output(mL/min)	|	Stroke Volume (mL)	|	ECG Output (mV)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Bradycardia	|	Heart Rate set to 50 beats per minute (<60 considered bradycardia)	|	0	|	120	|<span class="success">	50 @cite mangrum2000evaluation	</span>|<span class="warning">	Decrease @cite klabunde2012cvconcepts	</span>|<span class="warning">	No Relevant changes @cite mangrum2000evaluation	</span>|<span class="success">	No Relevant changes @cite mangrum2000evaluation	</span>|<span class="success">	Decrease @cite klabunde2012cvconcepts	</span>|<span class="success">	Increase @cite mangrum2000evaluation	</span>|<span class="success">	Sinus @cite mangrum2000evaluation	</span>|

### Sinus Tachycardia
Sinus Tachycardia was validated by setting the baseline heart rate to 110&nbsp;beats per minute. The condition was allowed to stabilize in keeping with chronic condition  methodology; for more information, see @ref SystemMethodology. Validation data predicted a decrease in stroke volume with the increase in heart rate @cite aroesty1985simultaneous, which was also found in the %BioGears engine. Because of the decrease in stroke volume, the cardiac  output remains relatively unchanged. This is due to the model not currently affecting compression force, only compression rate. The %ECG output in tachycardia is generally similar to normal sinus; however, in some cases, the T wave can experience constructive  interference from the following heart beat&apos;s P wave. This is shown in the %BioGears %ECG output seen in Figure 13.

@image html SinusTachycardia_Physionet.png
@image html SinusTachycardia_BioGears.PNG
<center>
<i>Figure 14. Due to the high heart rate, the %BioGears output is summing together the P and T waves. In the image from PhysioNet, the output is not summed together as dramatically, due to the slight physiological compression of the waveform that the current %ECG system and heart model do not support. @cite healey2005detecting @cite goldberger2000physiobank</i>
</center><br>

<br><center>
*Table 11. Tachycardia validation results.*
</center>
|	Condition	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate(beats/min)	|	Mean Arterial Pressure (mmHg)	|	Respiration Rate (breaths/min)	|	Oxygen Saturation (mmHg)	|	Cardiac Output(mL/min)	|	Stroke Volume (mL)	|	%ECG Output (mV)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Tachycardia	|	Variable.Set Heart Rate = 110 beats per minute; in Aroestyl, patients were not ideal, a number had previous heart conditions or coronary artery disease. Tachycardia was achieved via atrial pacemakers	|	0	|	120	|<span class="success">	110 @cite yusuf2005deciphering	</span>|<span class="warning">	13% increase @cite still2005prevalence	</span>|<span class="warning">	NC @cite yusuf2005deciphering	</span>|<span class="success">	NC @cite yusuf2005deciphering	</span>|<span class="success">	Dependent on SV and HR @cite sohn2007hemodynamic	</span>|<span class="success">	Decreases as Heart Rate increases @cite aroesty1985simultaneous	</span>|<span class="success">	Normal sinus/Tachycardia Waveform 	</span>|

### Anemia
The anemia condition reduces the oxygen carrying capacity of the blood. The anemia validation results are shown in Table&nbsp;12, and were in excellent agreement with literature.

<center>
*Table 12. Anemia validation results.*
</center>
|	Condition	|	Notes			|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Hemoglobin Concentration (g/dL)	|	Heart Rate (/min)	|	Cardiac Output (L/min)	|	Stroke Volume (mL/beat)	|	Oxygen Saturation %	|	Respiration Rate (/min)	|	Hematocrit %	|	Systemic Vascular Resistance (mmHg*min/L)	|
|	------------------------	|	------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Anemia	|	30% severity			|	0	|	120	|<span class="success">	10.3 as determined by initial hemoglobin and severity [set by severity]	</span>|<span class="success">	Approximately no change from normal @cite duke1969hemodynamic	</span>|<span class="success">	Slight increase from normal with increase in stroke volume @cite duke1969hemodynamic	</span>|<span class="success">	Slight increase with stroke volume @cite duke1969hemodynamic	</span>|<span class="success">	94.5 @cite duke1969hemodynamic; Should not affect oxygen saturation, especially at our severities @cite moss2016SME	</span>|<span class="success">	NC until exertion @cite toy2000fatigue	</span>|<span class="warning">	Decrease as determined by severity and initial hemoglobin @cite duke1969hemodynamic	</span>|<span class="success">	Decrease @cite guyton2006medical	</span>|

@anchor cardiovascular-conclusions
Conclusions
-----------
The %BioGears CV System uses circuit methodology to simulate blood flow and physiological connection to other systems within the human body. This system provides accurate results for resting physiology and allows for the simulation of a variety of CV-related insults and interventions with appropriate system feedback. Circuit modeling of the CV System is both quick and effective, and the %BioGears implementation allows the user to easily modify the circuit to vary the model resolution and fidelity or integrate their own models.

@anchor cardiovascular-future
Future Work
===========

Recommended Improvements
------------------------

An area of potential future advancement lies in the inertance of the %Cardiovascular System. The %BioGears @ref CircuitMethodology has the ability to incorporate inertance into the lumped parameter models. In the future, this could be added to the %Cardiovascular Model to provide a more accurate blood pressure waveform.

Another potential area for improvement is simulation of a tourniquet. An intervention could be added to simulate the increased resistance to flow and external pressure application that would be present with the application of a tourniquet.

The cardiac arrest functionality also needs improvement. Like in the human body, most of the %BioGears systems require a beating heart to function properly. Also like in the human body, the %BioGears systems tend to go haywire when the heart stops. However, the ways in which the %BioGears systems go haywire deviate from the human physiological systems' response during cardiac arrest. Improvements to the engine functionality during cardiac arrest would allow for many desirable scenarios, including Advanced Cardiac Life Support (ACLS) scenarios where some recovery of the patient is actually possible. As described in the CPR section, recovery from cardiac arrest is currently impossible in the %BioGears engine. Rescue breathing would also be a valuable improvement.

@anchor cardiovascular-appendices
Appendices
==========

Acronyms
--------
AHA - American Heart Association

CFD - Computational Fluid Dynamics

CPR - Cardiopulmonary Resuscitation

CV - %Cardiovascular

%ECG - Electrocardiogram

MAP - Mean Arterial Pressure

MCIS - Military Injury Combat Scale

mL - Milliliters

mmHg - Millimeters mercury

Data Model Implementation
-------------------------
@ref CardiovascularSystemTable "Cardiovascular"

@ref ElectroCardioGramTable "ElectroCardioGram"

@ref TissueSystemTable "Tissue"

@anchor cardiovascular-compartments
Compartments
------------
- Aorta
- Heart
 - Myocardium
 - LeftHeart
 - RightHeart
 - Pericardium
- VenaCava

- PulmonaryArteries
- PulmonaryCapillaries
- PulmonaryVeins
- Lungs
 - LeftLung
  - LeftPulmonaryArteries
  - LeftPulmonaryCapillaries
  - LeftPulmonaryVeins
 - RightLung
  - RightPulmonaryArteries
  - RightPulmonaryCapillaries
  - RightPulmonaryVeins

- Kidneys
 - LeftKidney
  - LeftRenalArtery
  - LeftNephron
   - LeftAfferentArteriole
   - LeftGlomerularCapillaries
   - LeftEfferentArteriole
   - LeftPeritubularCapillaries
   - LeftBowmansCapsules
   - LeftTubules
  - LeftRenalVein
 - RightKidney
  - RightRenalArtery
  - RightNephron
   - RightAfferentArteriole
   - RightGlomerularCapillaries
   - RightEfferentArteriole
   - RightPeritubularCapillaries
   - RightBowmansCapsules
   - RightTubules
  - RightRenalVein

- Bone
- Brain
- Fat
- Gut
 - Splanchnic
 - SmallIntestine
 - LargeIntestine
- Liver
- Spleen
- Skin
- Muscle
- LeftArm
- LeftLeg
- RightArm
- RightLeg
- Ground
