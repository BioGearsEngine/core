%Respiratory Methodology {#RespiratoryMethodology}
========================

@anchor respiratory-overview
Overview
========

@anchor respiratory-abstract
Abstract
--------

The %Respiratory System supplies oxygen and removes waste carbon dioxide from the body through a combination of ventilation and gas exchange across the blood-gas barrier (pulmonary capillary-alveoli interface). The %BioGears %Respiratory System is designed to model the ventilatory behavior (both positive- and negative-pressure) of the patient %Respiratory System using electrical analogue lumped parameter models. The %Respiratory Model employs realistic pressure source signal and chemical stimuli feedback mechanisms as drivers for spontaneous ventilation. The model handles several patient conditions, including tension pneumothorax and airway obstruction. The majority of the lung values investigated for the overall model matched the validation data found in publications. Patient conditions also showed strong agreement with clinically significant output parameters, i.e., respiration rate, oxygen saturation, heart rate, and blood pressure.

@anchor respiratory-intro
## Introduction

@anchor respiratory-physiology
### %Respiratory Physiology

The human %Respiratory System consists of the upper airways (region above the cricoid cartilage), the lower airways, the lungs, and the respiratory muscles. The lower airways begin at the trachea and extend to the bronchi, bronchioles, and the alveoli. At the carina, the trachea divides into two mainstem bronchi, the right and left. The bronchi bifurcate into smaller bronchioles that continue branching for up to 23 generations, forming the tracheobronchial tree that terminates with the alveoli. Alveolar ducts and alveolar sacs are the operating units of the lungs where gas exchange occurs with the pulmonary capillaries. The first several generations of airways, where no gas exchange occurs, constitute the anatomic dead space and are referred to as the conducting zone. In contrast, alveolar ducts and sacs that terminate the tracheobronchial tree are referred to as the respiration zone.

@image html Respiratory_Figure01_SystemDiagram.png
<center>
<i>Figure 1. The %Respiratory System consists of the upper and lower airways. The diaphragm acts as a respiratory muscle taking part in the ventilatory driver mechanics. The trachea branches into the right and left bronchi, each of which further bifurcates into multiple generations of smaller bronchioles. These bronchioles form the tracheobronchial tree, which terminates at the alveoli. @cite LadyofHats2014Respiratory </i>
</center><br>

The alveolar-capillary gas exchange is facilitated by the ventilation process, which is driven by the intercostal muscles, the diaphragm, and the chest wall recoil. These mechanisms work in tandem to actively drive fresh air into the lungs and passively remove gases from the lungs. Attached to the chest wall is a thin layer of membrane (pleura) that folds back onto itself, forming two layers, known as the visceral and parietal pleurals. The pleural cavity is filled with fluid. The pressure in this space, known as the intrapleural pressure, is normally slightly below the atmospheric pressure. Even when no inspiratory muscles are contracting, the mechanical interaction between the lung and the chest wall pulls the two pleural membranes apart, resulting in a slightly decreased intrapleural pressure (-3 cm H<SUB>2</SUB>O to -5 cm H<SUB>2</SUB>O) @cite Levitzky2013pulmonary .

@anchor respiratory-math
### Mathematical Model

Mathematical modeling of the respiratory physiology dates back to the work published by Gray in 1945 @cite gray1951pulmonary . Gray provided the first mathematical description for the chemical control of pulmonary ventilation. Later, Gordins et al. developed the first dynamic model of the respiratory system in 1954 @cite grodins1954respiratory . Several mathematical models followed after that, including the work by Guyton and collaborators in 1965 @cite milhorn1965mathematical and others ( @cite grodins1967mathematical , @cite khoo1982factors , @cite saunders1980breathing , @cite ursino2004interaction ). Many of the published models describe a specific aspect of the respiratory physiology in considerable detail. To name a few, Lorandi et al. @cite lorandi2003parametric employed a mathematical model to describe the mechanical properties of the lungs, Murray et al. @cite murray1977techniques described the gas exchange properties of the lungs, Wiberg et al. @cite wiberg1979modeling and Bache et al. @cite bache1981time described the effect of higher levels of CO<SUB>2</SUB> or anesthetic gases on breathing, and Whipp et al. @cite whipp1995obligatory developed a mathematical model to describe the respiratory anaerobiosis in skeletal muscle.

Many mathematical models of mechanical ventilation employ the lumped parameter model that represents the entire ventilation process with a small number of unknowns. The simplest lumped parameter model of mechanical ventilation assumes the conducting zone can be identified with a pipe that connects a collection of alveoli to the atmosphere and exerts pneumatic resistance to the flow. This type of model can be solved with a low computational cost, which reduces runtime. For whole body models/simulations, this is an important requirement. The disadvantage of lumped parameter models can lie in the large number of parameters that can result from required circuit parameters. It is important to identify the key features and behaviors of any model to intelligently reduce the number of required parameters.

The most important parameters in the lumped parameter model of mechanical
ventilation correspond to the elastic behavior of the lung and the flow
resistance of the airways. The thoracic cage and the lung tissue exhibit an
elastic behavior that can be represented with a single compliance or multiple
compliances. The compliance *C* is calculated by taking the ratio of the volume <i>&delta;V</i>
and the pressure <i>&delta;P</i> variations as:

\f[C=\frac{\delta V}{\delta P} \f] 
<center>
<i>Equation 1.</i>
</center><br> 

As a first-order approximation, the volume of the functional unit can be
approximated as:

\f[V(P+\delta P)=V(P)+C\delta P\f] 
<center>
<i>Equation 2.</i>
</center><br> 

In the %Respiratory System, the main source of flow resistance arises from the
flow of air through the branches in the conducting zone. Mathematical models
using the lumped parameter model select functional units for these regions and
designate the variable <i>R</i> for pneumatic flow resistance. The pressure drop <i>&Delta;P</i> across
the respiratory tree can thus be calculated by using Ohm's law analogue as

\f[\Delta P=RQ\f] 
<center>
<i>Equation 3.</i>
</center><br> 

where <i>Q</i> is the volumetric flow rate. The above relation assumes the flow
is laminar and the gas is incompressible. For laminar, viscous, and
incompressible flow, the Hagen-Poiseuille equation relates the pressure drop <i>&Delta;P</i>
in a fluid flowing through a cylindrical pipe of length <i>l</i> and radius <i>r</i> as

\f[\Delta P=\frac{8\mu l}{\pi r^{4} } Q\f] 
<center>
<i>Equation 4.</i>
</center><br> 

where <i>&Mu;</i> is the dynamic viscosity. By defining the flow resistance <i>R</i> as

\f[R=\frac{8\mu l}{\pi r^{4} } \f] 
<center>
<i>Equation 5.</i>
</center><br> 

a relation analogous to Ohm's law can be derived.

@anchor respiratory-systemdesign
System Design
=============

@anchor respiratory-background
Background and Scope
--------------------

### Existing

The %BioGears %Respiratory Model has its roots in the mathematical model of
Yashuri Fukui and N. Ty Smith @cite FukuiSmith1981hybrid . The researchers
developed a lumped parameter mathematical model to describe the uptake and
distribution of halothane. Their %Respiratory Model consisted of two pulmonary
compartments corresponding to the dead space and the alveoli @cite FukuiSmith1981hybrid. 
The %Respiratory Model in the %BioGears Engine is an
extension of the work by Fukui and Smith. This model was developed and released
by Advanced Simulation Corporation as part of the simulator, Body Simulation for
Anesthesia&trade;. This later formed the backbone of the HumanSim&trade; physiology engine,
which continues to provide realistic physiology for several serious training
games in the HumanSim product line, including HumanSim: Combat Medic and
HumanSim: Sedation and Airway @cite Clipp2012Humansim . The HumanSim
physiology engine is the starting point for the %BioGears Physiology Engine. The
basic elements of the %Respiratory System remain the same as the HumanSim
physiology engine; however, the respiratory circuit has been further developed
to allow realistic mechanical responses to pathological conditions.

### Approach

The current version of the %BioGears %Respiratory Model represents the two lungs
and associated airways as five major functional units, or compartments, that are
designated as the carina, right and left anatomic dead space, and right and
left alveoli. In the model, the carina compartment represents the anatomical region
from the airway at the trachea. The right and left anatomic dead
space compartments represent the bronchi and their branching bronchioles that
are part of the conducting airways below the carina. The right and left alveoli
compartments correspond to the collection of alveoli where gas exchange occurs
between the airways and the %Cardiovascular System. The right and left chest wall
compartments represent the right and left sides of the thoracic wall. The new model additionally accounts for the pleural
cavity through circuit elements that allow flow into the pleural space in the
event of respiratory insults that involve gas leak either from the alveoli or
the thoracic wall. To account for flow through the esophagus, an incidence that
may occur during mechanical ventilation (positive-pressure ventilation), the
model provides subordinate compartments representing the esophageal passage and
the stomach. The model also consists of a pressure signal generator representing
the respiratory muscle pressure source driver.

@anchor respiratory-dataflow
Data Flow
---------

The %Respiratory System determines its state at every time step through a three
step process: Preprocess, Process, and Postprocess. In general, Preprocess
determines the circuit element values based on feedback mechanisms and engine
settings/actions. Process uses the generic circuit calculator to compute the
entire state of the circuit and fill in all pertinent values. Postprocess is
used to advance time.

### Initialize

At the beginning of a simulation, patient parameters are used to modify the muscle (pressure source) driver functionality to achieve the specified values at the end of the resting stabilization period - see the @ref respiratory-variability "Patient Variability" section for more details. After resting stabilization is achieved, any user-selected conditions are implemented to reach a new homeostatic point - see the @ref respiratory-conditions "Conditions" section for more details.

### Preprocess

#### Update Pleural Compliance

The compliances of the left and right pleural space are modified as a function of volume.

#### Process Actions

Process Actions deals with modification of the respiratory parameters and
circuit properties due to actions (insults or interventions) specified by the
user.

#### %Respiratory Driver

The respiratory muscle pressure source that drives spontaneous ventilation is
calculated based on chemical stimuli feedback control mechanism.

### Process

The generic circuit methodology developed for the %BioGears Engine is used to
solve for the pressure, flow, and volume at each node or path. For more details
on this, see the @ref CircuitMethodology. Substance volumes and volume
fractions (concentrations) are also calculated during this step. See the @ref SubstanceTransportMethodology for more details.

The Calculate Vital Signs function uses the circuit pressure, flow, and volume
to calculate important system-level quantities for the current time step.

### Postprocess

The Postprocess step moves values calculated in the Process step from the next
time step calculation to the current time step calculation. This allows all
other systems access to the information when completing their Preprocess
analysis during the next time step.

### Assessments

Assessments are called outside of the system to allow compiling of information from multiple systems.

<center><img src="./images/Respiratory/Respiratory_Figure02.png" width="900"></center>
<center> 
<i>Figure 2. The data flow for the %Respiratory System consists of Preprocess,
Process, and Postprocess. Preprocess sets the circuit element values based on
feedback mechanisms and engine actions. Process utilizes generic circuit
methodology to solve the respiratory circuit for pressures, volumes, and flows
along paths and on nodes. Process also updates system level quantities by
calculating the vital signs. Postprocess updates these quantities to the next
time step and then begins advancing time.</i>
</center><br>

@anchor respiratory-features
Features and Capabilities
-------------------------

### %Respiratory Circuit

The %BioGears %Respiratory System designates a set of functional elements, or
compartments, to model mechanical ventilation. The functional elements are
represented by an electric analogue circuit comprised of resistors, capacitors,
switches, diodes, and power sources. The latter represents the driving pressure
from the respiratory muscles. The resistors and capacitors represent the
resistance to flow through the airways and the elastic nature of the airways,
alveoli, and thoracic walls.

The equivalent of an electric switch is used to transition between different
ventilation conditions or flow pathways. For example, the electric analogue switch
accounts for incidents that permit flow through the esophageal tract while
prohibiting flow through the trachea. Unidirectional flow in the respiratory
system is handled through electric analogue diodes that allow flow in one
direction, preventing flow in the opposite direction. Such functional elements
are employed to represent insults that allow unidirectional gas flow into the
pleural cavity through an opening at the alveoli or the thoracic wall.

<img src="./images/Respiratory/Respiratory_Figure03.png" width="650">
<center>
<i>Figure 3. Circuit diagram of the %Respiratory System. The diagram depicts a
closed circuit of the seven major compartments (trachea, right and left anatomic
dead space, right and left alveoli, and right and left chest wall) and the four
subordinate compartments (right and left pleural, esophagus, and stomach). Each
of the alveoli and anatomic dead space compartments are represented by a
combination of resistor and capacitor. The remaining compartments are
represented by resistors or capacitors. The circuit also depicts the muscle
pressure source that serves as the driver for the %Respiratory System. Unless changed
for insults and interventions, the subordinate compartments have "infinitely"
large resistors and behave as open electrical switches.</i>
</center><br>

In the circuit model, the carina and right and left anatomic dead spaces are
represented by resistors to account for pneumatic resistance that impedes flow
of gas across the conducting zones. A capacitor representing the compliance of a
compartment is added to the nodes designated as anatomic dead spaces to account
for the elastic behavior of the lower airways. Each of the right and left
alveoli compartments are represented by a combination of resistors and
capacitors (compliances) to account for the elastic behavior of the alveoli. The
right and left chest wall compartments are represented by variable compliance
that allows flexibility to mechanical insults. Based on the electrical circuit
analogue, the model predicts the dynamic properties of the %Respiratory System.
Figure 3 depicts the network of respiratory circuit elements and their
interconnections.

The %BioGears respiratory circuit employs circuit nodes and paths to represent
physiological state variables belonging to the %Respiratory System's functional
units. In this representation, the pressures across the compartmental units are
designated to the nodes, while all other variables (flow, volume, hydraulic
resistances, and compliances) are assigned to the paths on the circuit. At any
instant of time, the flow <i>Q</i> on a path across a resistor <i>R</i> can be calculated
using the pressure difference <i>&Delta;P</i> between the nodes across the path as <i>Q=&Delta;P/R</i>.
Similarly, the volume change <i>&Delta;V</i> of a respiratory element with compliance <i>C</i> can
be calculated based on the pressure difference <i>&Delta;P</i> between the nodes connected by
the path as <i>&Delta;V=C&Delta;P</i>. The time evolution of the pressures at each node in the
circuit is solved using the %Circuit Solver as described in the @ref
CircuitMethodology.

@anchor respiratory-variability
### Patient Variability

Several patient parameters are set/calculated outside of the %Respiratory System at the beginning of a simulation (See @ref PatientMethodology).  The patient parameters that are used as inputs to the %Respiratory System are:
- Respiration Rate Baseline: used to set the driver frequency
- Functional Residual Capacity: used to set the driver default pressure
- Total Lung Capacity: used to set the driver maximum allowable pressure
- Right Lung Ratio: used in the scaling equation for inspiratory-expiratory ratio for conditions
- Basal Metabolic Rate: used for metabolic effects
- Vital Capacity: used to determine the tidal volume plateau in the driver piecewise target alveolar ventilation function

The Pulmonary Function Test also pulls all lung volumes and capacities, and conscious respiration uses several of the initial capacity values to calculate the driver pressure needed.

Several patient parameters are updated at the end of each stabilization segment (Resting, Conditions, and Feedback).  This allows the simulation to reach new homeostatic points that take into account the whole-body state based on both internal and external factors.  The patient parameters that are reset by the %Respiratory System are:
- Respiration Rate Baseline: from Respiration Rate system data value
- Tidal Volume Baseline: from Tidal Volume system data value
- Functional Residual Capacity: from calculated instantaneous value
- Vital Capacity: calculated as [TLC - RV]
- Expiratory Reserve Volume: calculated as [FRC - RV]
- Inspiratory Reserve Volume: calculated as [TLC - FRC - TV]
- Inspiratory Capacity: calculated as [TLC - FRC]

The patient Alveoli Surface Area is also modified when COPD and lobar pneumonia effects are applied.

@anchor respiratory-feedback
### Feedback

#### Driver Pressure Source

The %BioGears %Respiratory System interacts with other systems in the engine to
receive feedback and adjust spontaneous breathing for homeostasis. To
accurately model the respiratory response under various physiological and
pathological conditions, a robust %Respiratory Model that responds to mechanical
stresses and chemical stimuli is required. To this end, the %BioGears
%Respiratory System employs a time-dependent pressure source based on a chemical
feedback mechanism that mimics the respiratory response to blood gas levels as
sensed by the central and peripheral chemoreceptors. The pressure source
represents the muscle pressure source signal and serves as an input power source
to drive the inspiration and expiration phases of the breathing cycle. 

During inhalation, the driver pressure source is set to a negative value. The end of the exhalation cycle represents the initial
conditions of free breathing, where the alveolar pressure equals the atmospheric
pressure and no air flows
into the lungs. When the inspiratory muscles are not contracting, the mechanical
interaction between the lungs and the chest wall creates a sub-atmospheric intrapleural
pressure. The value of the
driver pressure in the model is selected to meet the unstressed condition at the
pleural node. In the case of mechanical ventilation, the anesthesia machine
controls the pressure at the airway node for positive pressure ventilation. More
details on positive pressure ventilation can be found in the @ref AnesthesiaMachineMethodology.

For a realistic muscle pressure source signal, the %BioGears %Respiratory System
adopted the mathematical model proposed by Fresnel, et al. @cite Fresnel2014musclePressure , which is
based on clinical data. Accordingly, the time series of the respiratory muscle
pressure, <i>P<sub>mus</sub></i>, is given by,

\f[P_{mus} =\left\{\begin{array}{l} {P_{\max } (1-\exp (-\frac{f_{v} +4P_{0.1} }{10} *t)),0<t\le T_{I} } \\ {P_{\max } (\exp (-\frac{f_{v} +\frac{P_{0.1} }{2} }{10} *t)),T_{I} <t\le T_{tot} } \end{array}\right. \f] 
<center>
<i>Equation 6.</i>
</center><br> 

Where <i>T<sub>I</sub></i> and <i>T<sub>tot</sub></i> are the inspiratory and total respiration times, respectively.  <i>f<sub>v</sub>=1/T<sub>tot</sub></i> is the ventilation frequency, <i>P<sub>0.1</sub></i> is the mouth occlusion pressure, and <i>P<sub>max</sub></i> is the maximum muscle pressure that specifies the amplitude of the pressure source signal.  <i>P<sub>max</sub></i> is estimated using a pressure/volume curve constructed from simulation data. The %BioGears %Respiratory System circuit was exercised over a range of pressures to produce a curve showing lung volume as a function of driver pressure (Figure 4). Given the desired lung volume (based on target tidal volume, see Equation 13), P<sub>max</sub> can be estimated. 

<img src="./images/Respiratory/Respiratory_Figure04.png" width="550">
<center> 
<i>Figure 4. Maximum driver pressure, P<sub>max</sub>, as a function of lung volume.</i>
</center><br>

The curve in Figure 4 is parameterized to produce the relation 

\f[P_{\max } =-0.3743V^{5} +7.4105V^{4} -57.076V^{3} +214.11V^{2} -409.97V+262.22\f] 
<center>
<i>Equation 7.</i>
</center><br> 

In this manner, P<sub>max</sub> is related to target tidal volume <i>V<sub>T</sub></i>. 
In the model, <i>T<sub>I</sub></i> and <i>T<sub>tot</sub></i> are related to <i>f<sub>v</sub></i> 
through the relation @cite Fresnel2014musclePressure 

\f[\frac{T_{I} }{T_{tot} } =0.0125f_{v} +0.125\f] 
<center>
<i>Equation 8.</i>
</center><br> 
@anchor respiratory-chemoreceptors
The Fresnel model uses pre-selected ventilation frequencies to model various physiological and pathological conditions. The %BioGears %Respiratory System extended the Fresnel, et. al. model by incorporating a chemical stimuli feedback mechanism that contributes to the overall blood gas regulation. As a chemical feedback mechanism, past works used empirical relationships between minute ventilation, <i>V<sup><b>.</b></sup><sub>E</sub></i>, or alveolar ventilation, <i>V<sup><b>.</b></sup><sub>A</sub></i>, and the blood gas partial pressures that represent the respiratory response to chemical stimuli at the peripheral and central chemoreceptors @cite Khoo1982chemicalFeedback , @cite Batzel2005chemicalFeedback . The %BioGears %Respiratory Model adopted the mathematical relation  that links the alveolar ventilation with the blood gas levels. The resulting mathematical relationship implemented in the %BioGears %Respiratory System is 

\f[\dot{V}_{A} =G_{p} e^{-0.05P_{a} O_{2} } \max (0,P_{aCO_{2} } -I_{p} )+G_{c} \max (0,P_{aCO_{2} } -I_{c} )\f] 
<center>
<i>Equation 9.</i>
</center><br> 

<i>P<sub>aO</sub><sub>2</sub></i> and <i>P<sub>aCO</sub><sub>2</sub></i> are the arterial oxygen and carbon dioxide partial pressures, respectively. <i>I<sub>p</sub></i> and <i>I<sub>c</sub></i> are the cutoff threshold constants, and <i>G<sub>p</sub></i> and <i>G<sub>c</sub></i> are the peripheral and central controller gain constants, respectively. The value of <i>f<sub>v</sub></i> used in the pressure source corresponds to the target  respiration rate that the engine must attain to ensure accurate blood gas levels. It is related to the minute ventilation, <i>V<sup><b>.</b></sup><sub>E</sub></i>, as shown in the equation below

\f[f_{v} =\dot{V}_{E} /V_{T} \f] 
<center>
<i>Equation 10.</i>
</center><br> 

where <i>V<sup><b>.</b></sup><sub>E</sub></i> is calculated using the relation

\f[\dot{V}_{E} =\dot{V}_{A} +\dot{V}_{D} \f] 
<center>
<i>Equation 11.</i>
</center><br> 

<i>V<sup><b>.</b></sup><sub>D</sub></i> is the dead space ventilation and is obtained by taking the product of the dead space volume and the respiration rate. The target tidal volume <i>V<sub>T</sub></i> needed to predict <i>f<sub>v</sub></i>  using Equation 10 is calculated from the  pulmonary ventilation based on a piecewise linear relationship between the tidal volume and the minute ventilation as shown by Watson @cite watson1974tidalVolume . In the article, the author presented data that  describes the relationship between the minute ventilation and tidal volume by straight line. The data is reproduced in Figure 5 below.

<img src="./images/Respiratory/Respiratory_Figure05.png" width="800">
<center>
<i>Figure 5. The figure shows data from literature that presents the linear relationship between the minute ventilation 
and tidal volume. The relationship was derived from a line fit of experimental data with a wide range of varying carbon dioxide, exercise, postures, alveolar gas tensions, adrenaline, mild acidaemia, alkalosis, morphine, mederidine, mild hypoxia, and breathing through a small fixed resistance.  All test cases matched this trend of two intersecting straight lines.  The figure is reproduced from @cite watson1974tidalVolume .</i>
</center><br> 

The data in the article shows that the minute ventilation 
can be described by two intersecting straight lines. 
Up to about half of the vital capacity <i>V<sub>C</sub></i>, the minute ventilation,  
<i>V<sup><b>.</b></sup><sub>E</sub></i>, and the tidal volume, <i>V<sub>T</sub></i>, are related as

\f[\dot{V}_{E} =m(V_{T} -c)\f] 
<center>
<i>Equation 12.</i>
</center><br> 

where <i>m</i> is the slope and <i>c</i> is the x-intercept of the minute ventilation versus tidal volume plot. The data 
shows that the minute ventilation is constant above half of the vital capacity. Based on this observation, the 
%BioGears %Respiratory Model employs the linear relation given below to predict the target tidal 
volume from the minute ventilation.

\f[V_{T} =\left\{\begin{array}{l} {c+\dot{V}_{E} /m,V_{T} \le V_{C} } \\ {0.5*V_{C} ,V_{T} >V_{C} } \end{array}\right. \f] 
<center>
<i>Equation 13.</i>
</center><br> 

Where <i>m</i> and <i>c</i> are constant parameters determined during initialization. During the initial parameterization, the minute ventilation is plotted against the vital capacity to determine <i>c</i> by taking the x-intercept of <i>V<sup><b>.</b></sup><sub>E</sub></i> vs <i>V<sub>T</sub></i> plot. Then, the slope is adjusted to meet the initial baseline parameters of the patient. These parameters include the baseline respiration rate and tidal volume, where the latter is estimated from the patient's weight. 

Once <i>m</i> and <i>c</i> are selected this way, they are used as patient parameters for determining the target tidal volume from the minute ventilation that is determined by the feedback mechanism. This provides the target tidal volume that must be attained to respond to the chemical stimuli feedback. This value is then communicated back to the target ventilation frequency <i>f<sub>v</sub></i>, which in turn adjusts the patient's breathing frequency through the muscle pressure given in Equation 6. The muscle pressure then drives sufficient gas into the lungs in response to the chemical stimuli, completing the feedback loop. 

In the calculation of the target tidal volume, the %Respiratory Model allows the target tidal volume to increase linearly with the calculated minute ventilation as long as the target volume is below half the vital capacity. In the event that the calculated target volume is above half of the vital capacity, the target volume is set to a constant value of 0.5V<sub>C</sub> as shown in the above equation. In the model, the lung vital capacity V<sub>C</sub> is calculated from the patient's total lung capacity and residual volume as described in the sections below.

The model described above is implemented in the %BioGears Engine with reference values and model parameters that are tuned to meet validation data. The reference and tuned values for the model parameters are shown in Table 1 below.  Note that our model is tuned to use the reference values without modification.

<center>
<i>Table 1: %Respiratory driver model parameters and reference values used in the chemical feedback mechanism implementations.</i>
|Parameter (units)                        |Reference Value                           |Model Value             |
|------------------------                 |------------------------                  |------------------------|
|G<sub>p</sub>                   		  |30.24 @cite Batzel2005chemicalFeedback    |30.24                    |
|G<sub>c</sub>                     		  |1.44  @cite Batzel2005chemicalFeedback    |1.44                     |
|I<sub>p</sub>, I<sub>c</sub>(mmHg)       |35.5  @cite Batzel2005chemicalFeedback    |35.5                     |
|P<sub>0.1</sub>(cmH<SUB>2</SUB>O)                   |0.75  @cite Budwiser2007chemicalFeedback  |0.75                    |
</center><br>

Figure 6 depicts the time-dependent driver pressure source of the %BioGears %Respiratory System as obtained during simulation of the standard patient model of the engine (77 kg adult male) under normal physiological conditions. For comparison, the driver pressure is plotted with the alveolar, intrapleural, and transpulmonary pressures. The figure shows the pressures for several breathing cycles. As seen in Figure 6, the model driver pressure exhibits distinct waveforms during the inspiration and expiration phases. These patterns represent the active distension and passive relaxation behaviors of the inspiratory muscles. As a result of such input, the model distinguishes between the active inspiratory and passive expiratory phases of the breathing cycle. The time-dependent muscle pressure together with the atmospheric pressure and the compliances act in tandem to generate the pleural and alveolar pressure waveforms shown in the figure.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/Driver_Pressure.jpg" width="800"></td>    
</tr>
<tr>
    <td><img src="./plots/Respiratory/Pleural_and_Alveoli_Pressure.jpg" width="800"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/Transpulmonary_Pressure.jpg" width="800"></td>
</tr>
</table>
</center>

<center>
<i>Figure 6. The driver pressure, or pressure source, that serves as an electrical analogue voltage source for the respiratory circuit is plotted along with the alveolar, intrapleural, and transpulmonary pressures.  The pressure source generates a sub-atmospheric intrapleural pressure that facilitates the inspiration and expiration phases of spontaneous breathing.</i>
</center><br>

#### Standard Lung Volumes and Capacities

There are a number of standard lung volumes and capacities that are measured
during different stages of normal and deep breathing cycles. The inspiratory
reserve volume (IRV), tidal volume (V<sub>T</sub>), expiratory reserve volume (ERV), and
residual volume (RV) correspond to the four standard lung volumes. The
inspiratory capacity (IC), forced residual capacity (FRC), vital capacity (V<sub>C</sub>),
and total lung capacity (TLC) are the four standard lung capacities that consist
of two or more standard lung volumes. These volumes and capacities are good
diagnostics for lung functionality, and the %BioGears %Respiratory Model reports
their values as outputs. As mentioned above, some of the parameters are obtained
from patient data as input variables. The TLC and FRC are two of these
parameters that are drawn from the patient data. For the standard patient in the
model, TLC and FRC are set to be 6.0 L and 2.4 L, respectively. Using these
parameters as inputs, the engine calculates the other standard lung volumes and
capacities as described below.

##### Residual volume (RV)

The residual volume is the volume of gas remaining in the lungs after maximal
exhalation. As mentioned above, the %BioGears %Respiratory Model approximates the residual volume
based on the patient weight (RV = 16.0 mL/kg) (@cite Corning2007pulmonary , 
@cite Rennie2013pulmonary ). 
For the standard patient in the model 
with 77 kg weight, the residual volume
RV=1.23 L. Typical values of RV vary in the literature. For example, for 70 kg
patients: RV=1.5 L @cite Levitzky2013pulmonary , 1.2 L @cite silverthorn2013human, 
and 1.682 L @cite stocks1995reference . The %BioGears
engine employs weight-based relation and the values used in the engine are close 
to those found in the literature @cite silverthorn2013human .

##### Expiratory Reserve Volume (ERV)

ERV is the maximum volume below the tidal volume that can be expired during
maximal forced expiration. ERV can be calculated as

\f[ERV=FRC-RV\f] 
<center>
<i>Equation 14.</i>
</center><br> 

In this equation, both FRC and RV are input values obtained from weight-based
relation. For the standard patient in the model (77 kg adult male), FRC=2.31 L, and
RV=1.23 L, thus ERV becomes ERV=1.08 L. Typically, reported values for 
ERV are around 1.1 L @cite guyton2006medical .

##### Tidal volume (V<sub>T</sub>)

Tidal volume corresponds to the volume of air inspired or expired in a single
breathing cycle during normal quiet breathing. For a healthy 70 kg adult, the
tidal volume is 540 ml per breath. The tidal volume can be calculated by
numerically integrating the volumetric flow rate of inspired air flowing through
the trachea. The %BioGears %Respiratory Model calculates the tidal volume by
taking the difference between the maximum and minimum total lung volumes during
each breathing cycle.

Figure 7 depicts the typical lung volume waveform for multiple breathing
cycles. The %BioGears %Respiratory Model outputs the value of V<sub>T</sub> for each
breathing cycle. Figure 7 presents the plot of the total lung volume and V<sub>T</sub> as a
function of time.

<center><img src="./plots/Respiratory/TidalVolume_from_TotalLungVolume.jpg" width="800"></center>

<center>
<i>Figure 7. This shows the relationship of the total lung volume with the tidal
volume. The tidal volume for each cycle is determined by taking the difference between the maximum and
minimum values of the total lung volume, and is therefore only updated at the end of each full cycle.</i>
</center><br>

##### Inspiratory Reserve Volume (IRV)

IRV is the additional volume, above the tidal volume, that can be inspired
during maximal forced inspiration. IRV can be calculated from total lung
capacity (TLC) using the relation

\f[IRV=TLC-FRC-V_{T} \f] 
<center>
<i>Equation 15.</i>
</center><br> 

Both TLC and FRC are weight-based inputs to the model, whereas V<sub>T</sub> is calculated
as described above. Using TLC=6.16 L (i.e., 80 mL/kg) and FRC=2.31 L (i.e., 30 mL/kg) of
the standard patient in the model, IRV becomes 3.307 L for an average V<sub>T</sub> of
543 mL, where the average V<sub>T</sub> is calculated by taking the time average of V<sub>T</sub>
described above. Using weight-based tidal volume of V<sub>T</sub> = 7 mL/kg @cite Levitzky2013pulmonary , IRV 
can be shown to be 3.31 L for 77 kg patient, in good agreement with the value from 
the model.

##### Vital Capacity (V<sub>C</sub>)

V<sub>C</sub>  is the volume of air that can be expired after maximal inspiration. V<sub>C</sub> can be
calculated as

\f[V_{C} =IRV+V_{T} +ERV\f] 
<center>
<i>Equation 16.</i>
</center><br> 

V<sub>C</sub>  can also be calculated using TLC as:

\f[V_{C} =TLC-RV\f] 
<center>
<i>Equation 17.</i>
</center><br> 

Again, both TLC and RV are weight-based inputs to the model, and V<sub>T</sub> is calculated
as described above. For the standard patient in the model with TLC=6.16 L (80
L/kg) and RV=1.23 L, V<sub>C</sub>=4.93 L. Typical values of V<sub>C</sub>  reported in 
the literature are around 4.6 L @cite silverthorn2013human . The value in the engine corresponds to 
a weight-based vital capacity.

##### Inspiratory Capacity (IC)

The inspiratory capacity is another standard lung capacity that can be
calculated from TLC and FRC as

\f[IC=TLC-FRC\f] 
<center>
<i>Equation 18.</i>
</center><br> 

In the model, both TLC and FRC are weight-based input variables, and IC can be
calculated using the above equation. From the values in the model, IC can be
determined to be 3.85 L.

#### Ventilation

##### Respiration Rate (RR)

As described above, the %BioGears %Respiratory Model employs chemical feedback
mechanisms to regulate the ventilation frequency that affects the breathing cycle
through the respiratory driver. The breathing frequency is adjusted in
accordance to the arterial O<SUB>2</SUB> and CO<SUB>2</SUB> levels and other modifiers, such as drug and metabolic effects.
The engine switches between the inspiratory and expiratory phases based on the
predicted ventilation frequency. The respiration rate is then calculated by
measuring the time taken for a complete breathing cycle and converting it to the
number of breaths per minute. Typically, the respiration rate of a healthy adult
is 16 breaths/min @cite Levitzky2013pulmonary . A similar value is obtained
for the standard patient under normal tidal breathing.

##### Total Pulmonary Ventilation

The total pulmonary ventilation (or minute ventilation or minute volume) is the
volume of air moved into the lungs per minute. Minute ventilation (V<sup><b>.</b></sup><sub>E</sub>) is the
product of tidal volume (V<sub>T</sub>) and respiration rate (RR), i.e.,

\f[\dot{V}_{E} =V_{T} *RR\f] 
<center>
<i>Equation 19.</i>
</center><br> 

The %BioGears %Respiratory Model calculates both V<sub>T</sub> and RR from the simulation data. 
V<sup><b>.</b></sup><sub>E</sub> can thus be determined from the above equation by using the average values of V<sub>T</sub> and
RR. For the standard patient in the model, under normal physiological conditions,
the average values of V<sub>T</sub> and RR are 0.540 L and 16 breaths/min,
respectively. The total pulmonary ventilation obtained from the model equals
6.53 L/min. Typical weight-based value of minute volume is 84 mL/min/kg @cite Levitzky2013pulmonary . 
Using the mass 
of the standard patient (77 kg), the expected value of V<sup><b>.</b></sup><sub>E</sub> is 6.48 L/min, which is close to 
the value found from the engine.

##### Alveolar Ventilation

The term alveolar ventilation corresponds to the volume of air entering and
leaving the alveoli per minute @cite Levitzky2013pulmonary . The alveolar
ventilation is calculated as V<sup><b>.</b></sup><sub>A</sub>=(V<sub>T</sub>-V<sub>D</sub>)\*RR where V<sup><b>.</b></sup><sub>A</sub> is the alveolar
ventilation in liters per minute and V<sub>D</sub> is the volume of the conducting airways
that is referred to as the anatomical dead space. This is the region of
respiratory tract where no gas exchange takes place. In the %BioGears
%Respiratory Model, the volume of the dead space is calculated from the values
assigned to the right and left anatomic dead space nodes. These nodes have continuously changing volumes due the compliances that are connected to the
nodes. The right and left anatomic dead space volumes when
compared to the right and left alveoli volumes are shown in Figure 8. When the patient weight is factored into the 
calculation, the alveolar ventilation predicted from the model is close to the expected value.

<center><img src="./plots/Respiratory/Alveoli_and_Dead_Space_Volumes.jpg" width="800"></center>
<center>
<i>Figure 8. The right and left anatomic dead space volumes together with the right and left 
alveoli volumes. The difference in the alveoli volumes is due to the difference in 
 the lung ratio of the right and left lungs. The right and left lung ratios of the standard patient 
 in the %BioGears %Respiratory Model are 0.525 and 0.475, respectively. The left and right dead space volumes are equivalent.</i>
</center><br>

##### Tracheal Airflow

Airflow is measured by taking the instantaneous pressure difference across a
fixed resistance. The %BioGears %Respiratory Model measures tracheal airflow
<i>Q<sub>trachea</sub></i> by using the instantaneous pressure difference across the tracheal
resistance <i>R<sub>trachea</sub></i> as:

\f[Q_{trachea} =\frac{P_{mouth} -P_{carina} }{R_{trachea} } \f] 
<center>
<i>Equation 20.</i>
</center><br> 

<i>P<sub>mouth</sub></i> and <i>P<sub>carina</sub></i> are the pressures at the mouth and the carina nodes,
respectively (see circuit diagram, Figure 3). The figure shown below presents the absolute flow rate (no distinction for flow direction) for one breathing cycle.

<center><img src="./plots/Respiratory/Total_Flow_Through_Trachea.jpg" width="800"></center>
<center>
<i>Figure 9. Tracheal airflow and total lung volume during one typical breathing
cycle. At the peak of the inspiration phase, the flow rate goes to zero.</i>
</center><br>

#### Alveolar Pressure

The instantaneous pressures at the nodes of the respiratory circuit are
calculated by solving the circuit matrix equation as described in the @ref
CircuitMethodology. The pressures at the right and left alveoli nodes of the
%Respiratory Model represent the alveolar pressure. Typically, the values of
the alveolar pressure vary in the range from -1.8 cm H<SUB>2</SUB>O to 1.8 cm H<SUB>2</SUB>O (relative
to atmospheric pressure) during the inspiration and expiration phases of the
breathing cycle @cite otis1947measurement . The figure
below depicts the alveolar pressure along with lung
volume for one breathing cycle. The alveolar pressure in the engine is absolute (not relative
to atmospheric pressure), so the relative pressure can be determined by subtracting the standard atmospheric pressure of 1033 cmH2O - giving outputs close to the range of  -1.8 cm H<SUB>2</SUB>O to 1.8 cm H<SUB>2</SUB>O
found in the literature @cite otis1947measurement . 

<center><img src="./plots/Respiratory/Lung_Pressure_And_Volume.jpg" width="800"></center>
<center>
<i>Figure 10. Typical lung pressures. The plot shows the instantaneous pressure of
the left alveoli for one breathing cycle. For comparison, the plot also
shows the total lung volume for the same breathing cycle. As seen in this figure,
the lung volume increases as the alveolar pressure falls below the atmospheric pressure of 1033 cm H<SUB>2</SUB>O.
This creates a pressure differential between the airway node and the alveoli,
allowing air to flow into the lungs. When the alveolar pressure goes above 
1033 cm H<SUB>2</SUB>O, the lung volume decreases from its peak, representing the
expiration phase.</i>
</center><br>

##### Transpulmonary pressure

Transpulmonary pressure is defined as the difference between the alveolar 
 and the intrapleural pressures. The %BioGears %Respiratory System derives the transpulmonary 
 pressure from the calculated values of the alveolar pressure and intrapleural pressures. 
 The alveolar pressure is obtained by taking the average of the left and right 
 alveolar pressures. Similarly, the total intrapleural pressure is obtained by taking the average of the 
 the left and right pleural pressures. The plots shown below compare the transpulmonary pressure from the %BioGears 
 Engine with that found in literature @cite guyton2006medical. Some variations in the waveforms and 
 possibly the average values is a consequence of the specific driver pressure and 
 patient parameters employed in the engine.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/BioGears_Pressures.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/Guyton_Lung_Pressures.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/BioGears_Lung_Volume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/Guyton_Lung_Volume.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 11. A plot showing the transpulmonary pressure obtained from the %BioGears engine with 
that found and digitized from literature @cite guyton2006medical. The left %BioGears plots use absolute pressure, while the right Guyton plots use the pressure difference from ambient (1033 cmH2O).  For comparison, the plot also shows the lung volumes from the two sources.</i>
</center><br>

#### Pressure-Volume (P-V) curve
One method of characterizing the lungs' elastic behavior is to use a diagram
that relates the lung volume changes to changes in pleural pressure. The pressure-volume 
curve of a healthy person shows hysteresis during the inspiratory and expiratory phases. 
Figure 12 presents the pressure-volume diagram of data extracted from the %BioGears
%Respiratory Model. For comparison, the plot also shows a P-V diagram reproduced from literature
@cite guyton2006medical . The figures show the plot of lung volume changes versus pleural pressure
 for one breathing cycle. The pleural pressure from the model is the average of the right and left pleural 
 pressures. The lung volume change corresponds to the change in the total lung volume  
 during a complete breathing cycle.  
As shown in the figure, the %BioGears %Respiratory Model mimics the expected 
hysteresis of the P-V curve.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/BioGears_Pulmonary_Compliance.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/Guyton_Pulmonary_Compliance.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 12. The pressure-volume curve for the standard patient of the %BioGears %Respiratory Model 
under normal physiological conditions. For comparison, the figure includes plot reproduced from 
literature @cite guyton2006medical . The plot from the model shows the expected hysteresis of the P-V 
diagram observed in a healthy person.</i>
</center><br>

#### Partial Pressures of %Respiratory Gases

For any gas mixture, the partial pressure P<sub>gas</sub> of a particular gas in the
mixture can be calculated based on the total pressure P<sub>total</sub> of all gases in the
mixture and the fractional concentration F<sub>gas</sub> of the gas as

\f[P_{gas} =F_{gas} *P_{total} \f] 
<center>
<i>Equation 21.</i>
</center><br> 

The %Respiratory Model calculates the partial pressure of a gas at any node
based on the total pressure and the volume fraction of the gas at the node. The
node volume fraction of the gas and the node pressure are calculated in a manner
described in the @ref SubstanceTransportMethodology and @ref CircuitMethodology,
respectively. The %Respiratory Model employs the above equation to predict the
partial pressure of gases in the %Respiratory System. The sections below present
the results of alveolar and tracheal partial pressures of O<SUB>2</SUB> and CO<SUB>2</SUB>.

##### Alveolar O<SUB>2</SUB> Partial Pressure

The %BioGears Engine calculates the O<SUB>2</SUB> partial pressure P<sub>Lung<sub>O<SUB>2</SUB></sub/></sub/> at each
alveoli node by using the oxygen volume fraction VF<sub>Lung<sub>O<SUB>2</SUB></sub/></sub/> and the total
pressure P<sub>Lung</sub/> at the alveoli nodes as

\f[P_{LungO_{2} } =VF_{LungO_{2} } *P_{Lung} \f] 
<center>
<i>Equation 22.</i>
</center><br> 

The alveolar O<SUB>2</SUB> partial pressure can thus be determined by taking the average of
O<SUB>2</SUB> partial pressures at the two alveoli nodes. In the model, the alveoli node
pressures are gauge pressures and are expressed relative to the atmospheric
pressure. The model converts these relative pressures to their absolute
pressures when calculating the gas partial pressures. The %BioGears Engine
assumes the inspired air is heated and humidified. Therefore, the water vapor
pressure at normal body temperature (P<sub>H<SUB>2</SUB>O</sub>=47 mm Hg) is subtracted from the
standard atmospheric pressure of P<sub>B</sub>=760 mm Hg when the gas partial pressure is
calculated using the absolute lung pressure, i.e.,

\f[P_{LungO_{2} } =VF_{LungO_{2} } *(P_{B} -P_{H_{2} O} +P_{Lung} )\f] 
<center>
<i>Equation 23.</i>
</center><br> 

Figure 13 depicts the plot of P<sub>LungO<SUB>2</SUB></sub>  for the left and right alveoli
of the standard patient. Typically, the average alveolar partial pressure of oxygen 
 is 104 mmHg @cite Levitzky2013pulmonary . The value from the engine is close to that of the 
 literature.

<center><img src="./plots/Respiratory/Alveolar_Oxygen_Partial_Pressure.jpg" width="800"></center>
<center>
<i>Figure 13. Alveolar O<SUB>2</SUB> partial pressure. The partial pressure of O<SUB>2</SUB> at the two
alveoli nodes is calculated by using the pressure and the O<SUB>2</SUB> volume fraction at
each alveoli node. The plot shows the value of O<SUB>2</SUB> partial pressure as a function
of time over the course of multiple breathing cycles.</i>
</center><br>

##### Alveolar CO<SUB>2</SUB> Partial Pressure

The alveolar CO<SUB>2</SUB> partial pressure is calculated in the same manner as the oxygen
partial pressure. Figure 14 depicts the plot of alveolar CO<SUB>2</SUB> partial
pressure for the left and right alveoli nodes. Typically, the average alveolar CO<SUB>2</SUB> partial pressure is 40 mmHg @cite Levitzky2013pulmonary .
The prediction from the engine is close to the expected
literature value.

<center><img src="./plots/Respiratory/Alveolar_Carbon_Dioxide_Partial_Pressure.jpg" width="800"></center>
<center>
<i>Figure 14. Alveolar CO<SUB>2</SUB> partial pressure. The partial pressure of CO<SUB>2</SUB> at the two
alveoli is calculated by using the pressure and the CO<SUB>2</SUB> volume fraction at each
alveoli node. The plot shows the value of the CO<SUB>2</SUB> partial pressure as a function
of time over the course of multiple breathing cycles.</i>
</center><br>

##### Tracheal CO<SUB>2</SUB> Partial Pressure

Tracheal CO<SUB>2</SUB> partial pressure is calculated in the same manner as the alveolar
CO<SUB>2</SUB> partial pressure. The engine calculates the tracheal partial pressure by
using the pressure at the carina node. Recall that the carina node belongs to
the node where the trachea branches into the mainstem bronchi. The figure below
depicts the time variation of tracheal CO<SUB>2</SUB> partial pressure. The waveform for
CO<SUB>2</SUB> partial pressure appears similar to those found in normal capnograph at the mouth
(Fig.3.9, Ref @cite Levitzky2013pulmonary ). 

<center><img src="./plots/Respiratory/Trachea_Carbon_Dioxide_Partial_Pressure.jpg" width="800"></center>
<center>
<i>Figure 15. Tracheal CO<SUB>2</SUB> partial pressure. The partial pressure of CO<SUB>2</SUB> at the
trachea is calculated by using the pressure and the CO<SUB>2</SUB> volume fraction at the
carina node. The plot shows the value of tracheal CO<SUB>2</SUB> partial pressure over the course of one breathing cycle.</i>
</center><br>

##### Tracheal O<SUB>2</SUB> partial pressure

The O<SUB>2</SUB> partial pressure at the trachea is calculated in the same manner as the
alveolar O<SUB>2</SUB> partial pressure. As mentioned for CO<SUB>2</SUB> partial pressure, the model
calculates the tracheal O<SUB>2</SUB> partial pressure by making use of the pressure at the
carina node. The output of the tracheal O<SUB>2</SUB> partial
pressure calculation is presented in Figure 16.

<center><img src="./plots/Respiratory/Trachea_Oxygen_Partial_Pressure.jpg" width="800"></center>
<center>
<i>Figure 16. Tracheal O<SUB>2</SUB> partial pressure. The partial pressure of O<SUB>2</SUB> at the
trachea is calculated by using the pressure and the O<SUB>2</SUB> volume fraction at the
carina node. The plot shows the value of tracheal O<SUB>2</SUB> partial pressure over the course of one breathing cycle.</i>
</center><br>

@anchor respiratory-dependencies
### Dependencies

The %BioGears %Respiratory System interacts with other physiological
systems either directly or indirectly through processes that involve the
transport and exchange of gases. These interdependencies are discussed below.

#### Gas Transport

The transport of gases in the %BioGears %Respiratory Model is handled through
the Transport functionality of the engine, where mass conservation based
on volume fraction and volumetric flow rate of gases at the nodes and paths of
the respiratory circuit is employed (see the @ref SubstanceTransportMethodology for
details). During free breathing, the ambient atmospheric volume fractions are used as an input to the %Respiratory Model.
 
By using methods described in the @ref SubstanceTransportMethodology, the %BioGears
Engine calculates the volume fraction of gases at the nodes associated with the
respiratory functional units. The %Respiratory System uses the calculated volume
fractions and predicts various physiological parameters. For example, the gas volume
fraction at the end of expiration, referred to as end-tidal gas concentration,
can be monitored based on the results from the @ref SubstanceTransportMethodology
calculation. The end-tidal gas concentration is an important clinical parameter
for monitoring patients and preventing mishaps related to insufficient
ventilation or inappropriate gas concentration during anesthesia and immediate
recovery @cite linko1989monitoring. Monitoring end tidal CO<SUB>2</SUB> (ETCO<SUB>2</SUB>) is a
widely established clinical practice for verification of endotracheal tube
placement and is also one of the standard requirements for monitoring patients
in transport @cite donald2006end. The %Respiratory Model provides the
end-tidal gas concentration based on the expired gas volume fractions at the
airway node.

#### %Environment

The %BioGears %Respiratory System interacts with the %Environment System for
the atmospheric pressure values assigned to the mouth node. Changes to the 
environmental conditions, such as changes in altitude, ambient temperature, humidity, 
and others, can affect the breathing pattern of the patient. The %Respiratory System 
interacts with the %Environment System to utilize the values of 
ambient pressures and gas concentrations that reflect the environmental condition.
The %Respiratory System also interacts with the %Environment System for proper handling 
of inhaled gases that can arise from environmental incidents.

#### Alveolar Gas Exchange

At the alveoli-capillary interface, gas transfer occurs. The primary goal is to
transfer incoming oxygen into the bloodstream for transport to other organs and
to transfer waste carbon dioxide out of the body. This mechanism is also used to
transport inhaled agents into the bloodstream from the
%Respiratory System. For more details on gas transport, see @ref
SubstanceTransportMethodology and @ref BloodChemistryMethodology.

In the %BioGears %Respiratory Model, the spontaneous respiration rate is adjusted based on a chemical feedback mechanism
that depends on the arterial oxygen and carbon dioxide levels as described
above. The arterial oxygen and carbon dioxide levels depend on the level of O<SUB>2</SUB>
consumption and CO<SUB>2</SUB> production in the circulatory system, which in turn affects
the gas exchange at the alveolar-capillary interface. As the arterial O<SUB>2</SUB> and CO<SUB>2</SUB>
levels change, the breathing rate, tidal volume, and alveolar ventilation also change,
which in turn facilitates efficient gas exchange between the atmosphere and the
body. This presents an example of how the %BioGears Engine integrates the
circulatory and %Respiratory Systems to regulate the blood gas levels.

#### Drug Effects

As drugs circulate through the system, they affect the %Respiratory System. The drug effects on respiratory rate and tidal volume are calculated in the %Drugs System. These effects are applied to the respiratory driver by modifying the frequency and amplitude of the breathing cycle. During the respiratory driver calculations, the respiratory rate and tidal volume changes that are calculated in the %Drugs System are applied to the resulting respiratory rate and tidal volume calculated in the driver. Additionally, a neuromuscular block level is applied based on the drug effects.  If the neuromuscular block level is above 0.15 on a scale of 0 to 1.0, then the respiratory rate and tidal volume are set to zero. This represents the paralytic effects of a neuromuscular block agent.  The value of 0.15 was chosen to satisfy the block duration for succinylcholine and rocuronium. The strength of these effects, including the block effect, are calculated based on the plasma concentration and the drug parameters in the substance files in the %Drugs System.  For more information on this calculation see @ref DrugsMethodology.

#### Metabolic Effects

A metabolic modifier is set by the %Energy System (@ref EnergyMethodology) to drive the system to reasonable levels achievable during increased metabolic exertion.  The modifier is tuned to achieve the correct respiratory response for near maximal exercise, and a linear relationship is assumed. This modifier is a direct multiplier to the target alveolar ventilation input into the system driver, and it causes an increase in both tidal volume and respiration rate.

#### Anesthesia Machine Connection

The %Respiratory System can be hooked up to the anesthesia machine for positive-pressure ventilation (see the @ref AnesthesiaMachineMethodology).  This is achieved by connecting the two fluid circuits.  The anesthesia connection node is merely connected to the respiratory mouth node to allow for automatic calculation of the fluid mechanics by the circuit solver and transport by the substance transporter.  The mechanistic cascading effects are automatically achieved, and everything else is modeled exactly the same as when the systems are disconnected.

@anchor respiratory-outputs
###Outputs

At each time iteration, the %Circuit Solver calculates the values of the
state variables for that particular time. Using the calculated state variables,
the model predicts various physiological parameters of mechanical ventilation. Many of the calculated system data outputs are derived from the difference between the minimum and maximum lung volumes and the time between Occurrences - e.g., tidal volume and respiration rate.

Other values, like the pulmonary resistance and compliance, are determined instantaneously.  The pulmonary resistance is calculated by taking the ratio of 
the pressure difference between the mouth <i>P<sub>mouth</sub></i> and the alveoli <i>P<sub>alveoli</sub></i> and 
the flow across the trachea <i>Q<sub>trachea</sub></i> as

\f[R_{pulm} =\frac{P_{mouth} -P_{alveoli} }{Q_{trachea} } \f] 
<center>
<i>Equation 24.</i>
</center><br> 

The %BioGears %Respiratory Model calculates the pulmonary compliance <i>C<sub>pulm</sub></i> by dividing the tidal 
volume <i>V<sub>T</sub></i> by the intrapleural pressure <i>P<sub>pleu</sub></i> difference as

\f[C_{pulm} =\frac{V_{T} }{P_{pleau(\max )} -P_{pleu(\min )} } \f] 
<center>
<i>Equation 25.</i>
</center><br> 
 
Here <i>P<sub>pleu(min)</sub></i> and <i>P<sub>pleu(max)</sub></i> are the minimum and maximum respective pressures at the 
right and left pleural nodes.

@anchor respiratory-assumptions
Assumptions and Limitations
---------------------------

### Assumptions

As in most lumped parameter models of mechanical ventilation, the %BioGears
%Respiratory Model is based on two main parameters: the resistance R and the
compliance C (or elastance E). Similar to their electrical analogues, these
elements form a closed circuit to represent the energy dissipation and storage
properties of the normal tidal ventilation. One extension of the linear model
is the addition of inertance in the lumped parameter model. Inertance is
not included in the model on the assumption that inertia does not play a
significant role under conventional tidal breathing, as opposed to high-frequency
ventilation.

In the model, the %Respiratory System is assumed to behave linearly in that the
hydraulic resistance R is assumed to be independent of the flow rate Q, and the
elastance E or the compliance C of the elastic component is assumed to be
independent of the volume V. Therefore, under normal physiological conditions,
the circuit elements (resistors and capacitors) of the %BioGears %Respiratory
System are treated as constants. However, their values can be adjusted when
addressing pathological conditions.

The %Respiratory Model does not distinguish between different functional units
in the upper airway, the anatomical region where the inspired air is heated,
humidified, and filtered. In this region, the flow of air is turbulent, as
opposed to the laminar flow in lower airways. That means that the Ohm's analogue
pressure-volume relation cannot be applied to this region. However, the model
treats the upper airway as part of the mouth node and assumes laminar,
incompressible flow for the remaining airway.

### Limitations

The current version of the %Respiratory Model does not account for the upper
airways, and the detailed nature of upper airway flows will not be captured in
the model. It is known that flows at the upper airways are turbulent, while flows
in the lower respiratory tract are laminar. The absence of upper airways in
the model can underestimate the airway flow. In this regard, the lumped
parameter model is inherently at a disadvantage in handling complex geometries
and flows that can represent detailed upper airway pathology. However, the model
is sufficiently detailed to reproduce the respiratory responses associated with a
number of airway pathologies at relatively low computational cost.

Airway inertance is assumed to be negligible at normal respiratory frequencies.
However, analysis of high-frequency ventilation (HFV) of up to 40 Hz ventilatory
frequency would require the inclusion of inertance @cite rozanek2008design .
In mechanical ventilation, HFV is a useful procedure that permits significantly
decreased pressure amplitude and tidal volume, preventing trauma in artificial
ventilation. When compared to conventional mechanical ventilation, HFV is
considered to be the preferred method of mechanical ventilation in patients with
acute lung injury @cite Krishnan2000hfv . The small tidal volumes needed in
HFV are thought to offer an advantage over conventional ventilation in limiting
additional lung injury from mechanical ventilation @cite Krishnan2000hfv . The
current version of the %Respiratory Model handles conventional mechanical
ventilation but not HFV.

The model makes no distinction between different generations of bifurcating
airways. Therefore, factors affecting the regional ventilation and perfusion of
the lungs cannot be captured by the model.

@anchor respiratory-conditions
Conditions
----------

#### Chronic Obstructive Pulmonary Disease

Chronic Obstructive Pulmonary Disease (COPD) is an obstructive lung disease characterized by chronically reduced airflow into the lungs. Symptoms typically include increased respiration rate, decreased  tidal volume, decreased expiratory flow, and reduced oxygen saturation @cite man2003contemporary . There are also indications that COPD affects amino acid metabolism, causing muscle wasting and altered blood concentrations of certain amino acids @cite engelen2000factors. Unlike asthma, the reduction in lung airflow does not generally improve with medication. Symptoms typically worsen over time, often leading to death. COPD is caused by damage to the small airways in the lungs and destruction of the alveolar membranes. When tissue damage occurs primarily in the airways, the condition is called chronic bronchitis. When tissue destruction is focused in the alveoli, the condition is called emphysema. In %BioGears, COPD is a manifestation of both conditions, although one condition may dominate. 

%BioGears simulates COPD by modeling damage to the small airways and alveolar membranes. COPD severity is controlled by two severity values, a chronic bronchitis severity value and an emphysema severity value. Chronic bronchitis severity is used to scale the airflow resistance through the lower airways in the circuit model, simulating airway tissue damage and scarring. Increasing chronic bronchitis severity increases the airflow resistance through the lower airways. The function used to determine airflow resistance for COPD is the same as that used for asthma (see Figure 18) and is based on chronic bronchitis severity.

Emphysema severity is used to scale destruction of the alveolar membranes. Destruction of the alveolar membranes decreases the effective surface area for gas exchange, thereby reducing alveolar gas transfer. In cases of severe emphysema, up to 80% of the alveolar membranes are destroyed, with a corresponding reduction in effective gas diffusion surface area @cite guyton2006medical. %BioGears scales the gas diffusion surface area using a multiplier based on emphysema severity.  The function used to determine the gas diffusion surface area multiplier is plotted in Figure 17. 

Changes in amino acid metabolism are shifted by static percentages based on data from @cite mathur1999cerebral and @cite engelen2000enhanced. The protein breakdown rate is increased, as well as the fraction of muscle that is required to produce energy anaerobically regardless of O2 presence. These changes result in the increased protein turnover and elevated lactate concentrations observed in COPD patients.

Additionally, %BioGears models destruction of lung tissue with an increase in alveolar compliance.  The function used to determine the compliance multiplier is plotted in Figure 18. 

The destruction of the alveolar membranes also destroys the pulmonary capillaries embedded in the membranes. To model pulmonary capillary destruction, %BioGears increases the pulmonary capillary flow resistance based on severity. Although increased pulmonary capillary resistance is related to alveolar membrane destruction, and therefore associated with emphysema, %BioGears uses either emphysema severity or chronic bronchitis severity (whichever is higher) to determine the pulmonary capillary resistance multiplier - see Figure 19. This was done in an attempt to model increased blood pressure and elevated heart rate, which are symptoms of both emphysema and chronic bronchitis. Increasing the capillary resistance should increase arterial blood pressure as the heart pumps harder to overcome the increased resistance in the lungs. However, we were unable to successfully model either increased blood pressure or elevated heart rate for COPD, as the validation results show (see @ref respiratory-results "Results and Conclusions"). This will be the focus of later efforts to improve the model.  

Decreased Inspiration-Expiration (IE) ratio is another pathophysiologic feature of COPD.  As with asthma, the normal IE ratio is scaled using a multiplier based on severity. Either chronic bronchitis severity or emphysema severity (whichever is higher) is used to determine the IE ratio scaling multiplier - see Figure 20. 

<table border="0">
<tr>
    <td><img src="./images/Respiratory/Respiratory_Figure17_GasDiffusion.png" width="550"></td>
    <td><img src="./images/Respiratory/Respiratory_Figure18_AlveoliCompliance_COPD.png" width="550"></td>
</tr>
<tr>
    <td><center><i>Figure 17. Gas diffusion surface area multiplier as a function of emphysema severity. Gas diffusion surface area decreases with increasing emphysema severity.</i></center></td>
    <td><center><i>Figure 18. Alveoli compliance multiplier as a function of emphysema severity. Alveoli compliance increases with increasing emphysema severity.</i></center></td>
</tr>
<tr>
    <td><img src="./images/Respiratory/Respiratory_Figure19_PCResistance.png" width="550"></td>
    <td><img src="./images/Respiratory/Respiratory_Figure20_IERatio_COPD.png" width="550"></td>
</tr>
<tr>
    <td><center><i>Figure 19. Pulmonary capillary flow resistance multiplier as a function of COPD severity. Pulmonary capillary flow resistance increases with increasing chronic bronchitis or emphysema severity, whichever is higher.</i></center></td>
    <td><center><i>Figure 20. IE ratio multiplier as a function of COPD severity. IE ratio decreases with increasing chronic bronchitis or emphysema severity, whichever is higher.</i></center></td>
</tr>
</table>

#### Lobar Pneumonia

Lobar pneumonia is a form of pneumonia that affects one or more lobes of the lungs.  Symptoms typically include increased respiration rate, decreased  tidal volume, reduced oxygen saturation, decreased IE ratio, and increased body temperature @cite ebell2006outpatient .  As fluid fills portions of the lung, it becomes more difficult to breathe. Fluid also reduces the effective gas diffusion surface area in the alveoli, reducing alveolar transfer of oxygen and carbon dioxide into and out of the bloodstream @cite guyton2006medical . %BioGears simulates lobar pneumonia by decreasing the alveoli compliance in the respiratory circuit, which models increased breathing difficulty due to fluid congestion in the alveoli. Similarly, gas diffusion surface area is reduced using the same function as for COPD (see Figure 17). Decreased IE ratio is pathophysiologic feature of lobar pneumonia.  Like COPD, the normal IE ratio is scaled using a multiplier based on severity. %BioGears uses the same function as for COPD to determine the IE ratio multiplier (see Figure 20).   

<img src="./images/Respiratory/Respiratory_Figure21_AlveoliCompliance_LP.png" width="550">
<center>
<i>Figure 21. Alveoli compliance multiplier as a function of lobar pneumonia severity. Alveoli compliance decreases with increasing severity.</i>
</center><br>

#### Impaired Alveolar Exchange

The impaired alveolar exchange generically models an unspecified reduction of effective alveolar surface area.  This condition causes less effective gas exchange between the %Respiratory and %Cardiovascular systems.  The user can specify either a fraction or area value of the surface area to remove.

@anchor respiratory-actions
Actions
-------

### Insults

#### Airway Obstruction

Airway obstruction refers to a blockage in the airway that partially or wholly
prevents airflow into or out of the lungs. Airway obstructions
can occur at the lower and upper airways. They can
be caused by foreign objects, allergic reactions, infections, inflammations,
toxic gases, and other reasons. The pathological symptoms and physiological
manifestations of airway obstructions are as diverse as the causes, and the severities
of the obstructions follow suit. Dyspnea (breathing difficulty) is the
obvious common symptom of airway obstruction. The complications of breathing
difficulty include hypoxia (low oxygen levels) and hypercapnia (high carbon
dioxide levels), respiratory acidosis, and others.

The current version of the %BioGears %Respiratory Model attempts to reproduce
the physiological responses arising from foreign body upper airway obstruction.
Foreign body airway obstruction is fairly common during anesthesia. One example
is the case reported by @cite Roy2005airwayObstruction where the oxygen
saturation (SPO<SUB>2</SUB>) of a patient dropped to 40 percent due to a blockage by a
parasitic nematode during the use of a laryngeal mask airway. The %Respiratory
Model simulates airway obstruction by increasing the flow resistance across the
path connecting the airway node to the carina node. This path corresponds to the
flow through the trachea. The model then calculates the physiological responses
due to increased airway resistance.

<img src="./images/Respiratory/Respiratory_Figure22.png" width="550">
<center>
<i>Figure 22. Logarithmically increasing resistance values used to increase the airway resistance in relation
to the airway obstruction severity levels.</i>
</center><br>

#### Bronchoconstriction

A patient with bronchospasm experiences a sudden constriction of the muscles in
the walls of the bronchioles, decreasing the airway diameter. Such decrease in
the bronchial airway diameter, or bronchoconstriction, results in the reduction of
gas flow into or out of the lungs and causes difficulty in both spontaneous and
mechanical ventilation. A number of factors can trigger bronchospasms, including
a foreign body in the airway and stimulation of an endotracheal tube in patients
with reactive disease @cite Woodworth2012anesthesia . The %BioGears
%Respiratory Model accounts for this respiratory distress by adjusting the
resistance of the bronchi units. The increase in bronchial airway resistance results in a
decrease in gas flow in and out of the alveoli, which in turn affects the gas
concentration in the circulatory system. The engine then responds to the
respiratory distress in proportion to the level of bronchoconstriction.

#### Intubation

The Intubation action sets the type of intubation present: off, tracheal, esophageal, right mainstem, or left mainstem. Tracheal intubation is the successful state. See @ref AnesthesiaMachineMethodology for more details about intubation and mechanical ventilation.

##### Mainstem intubation

During endotracheal intubation, a flexible tube is inserted orally or nasally
down into the trachea to facilitate ventilation of the lungs during mechanical
ventilation. A potential complication of endotracheal intubation is mainstem
intubation, an incident where the tube is inadvertently placed too deep into one
of the two mainstem bronchi. In such incidents, the unintubated lung does not
contribute to gas exchange, and a right-to-left shunt can occur, resulting in
hypoxia @cite divatia2005complications . Furthermore, the intubated lung can
be hyperinflated, predisposing to overdistension and barotrauma @cite divatia2005complications .

Right mainstem intubation is the common complication of endotracheal intubation
because of the shallower angle that the right mainstem bronchus makes with the
trachea, though %BioGears includes the ability to simulate left mainstem intubation as well. The %BioGears %Respiratory Model attempts to simulate this incidence by
manipulating the resistance representing the opposite bronchus functional unit of
the %Respiratory System. A numerically large resistance is selected for this
circuit element for complete blockage of flow into the lung. The model then
evaluates the physiological responses arising from the incidence.

##### Esophageal Intubation

Another complication of endotracheal intubation is esophageal
intubation, an incident where the endotracheal tube is inadvertently placed in
the esophagus. When such an incidence occurs, the lung does not receive air. The
%BioGears %Respiratory System models esophageal intubation by opening
the airflow path leading to the esophagus and closing the flow path leading to
the trachea compartment. This is accomplished in the model by assigning
a numerically large value for the tracheal resistance and assigning a small
resistance to the esophageal compartment. The lack of airflow into the lungs
affects the amount of alveolar-capillary gas exchange, which in turn affects the
hemodynamic and respiratory responses of the engine.

#### Tension Pneumothorax

Pneumothorax is an abnormal accumulation of air in the pleural cavity, i.e., a
thin space between the visceral and parietal pleura separating lungs from the
chest wall, due to factors that cause air to escape into the pleural space. The
accumulation of air in the pleural space limits the space available for full lung
inflation and increases the intrathoracic pressure, causing a collapse in the ipsilateral lung depending on the type and
severity of the incidence. Pneumothoraces are classified as traumatic and
nontraumatic or spontaneous based on their cause. Pneuomothoraces are 
classified as open (sucking chest wound) or closed (intact thoracic cage)
pnumothoraces @cite sharma2008principles .

The %BioGears %Respiratory Model is capable of simulating traumatic tension
pneumothorax. Tension pneumothorax is a life-threatening condition that occurs
when air escapes into the pleural space on inspiration, but cannot return on
expiration due to a one-way valve effect of the injury. This causes a
progressive build-up of air within the pleural space. The %Respiratory
System implements a model for tension pneumothorax by including subordinate
circuit elements that represent the flow and trapping of air in the pleural
space. The circuit elements represent both the open and closed pneumothorax
condition. In the open tension pneumothorax condition, air is allowed to enter
from the atmosphere into the pleural space as in penetrating chest wound
traumas. In the closed tension pneumothorax case, the model allows air to escape
from the lung into the plural space as in laceration of the lung following blunt
trauma. The model evaluates the physiological responses arising from increased
intrapleural pressure and leakage in the airflow of the respiratory circuit.

<img src="./images/Respiratory/PneumoCirucit.png" width="550">
<center>
<i>Figure 23. Both lungs in the engine have elements to mimic the effects of open and closed tension pneumothorax insults as well as chest occlusive dressing (for open) and needle decompression (for both) interventions. The red boxes denote these additional elements.</i>
</center><br>

<img src="./images/Respiratory/PneumoLeakResistance.png" width="550">
<center>
<i>Figure 24. An exponential function is used to map pnemothorax action severity to leak resistance.  This same function is used for both the open and the closed tension pneumothorax.</i>
</center><br>

#### Acute Asthma

Asthma is a common inflammatory disease of the airways (bronchi and bronchioles) where air flow into the lungs is partially obstructed. Acute asthma is an exacerbation of asthma that does not respond to standard treatments. Symptoms include elevated respiration rate, labored breathing, and a reduction in oxygen saturation, among others.  It is generally considered a life-threatening obstruction of the airway requiring immediate medical attention. During an acute asthma attack, airflow is partially obstructed during exhalation, and flow resistance is 5 to 15 times normal @cite papiris2002asthma . %BioGears simulates this by increasing the airway flow resistance in the circuit model. The function used to determine the airway flow resistance multiplier is shown in the figure below. The asthma attack severity is governed by a specified severity value. The higher the severity, the more severe the asthma attack, and the higher the resistance values are set.

Additionally, the inspiratory/expiratory (IE) ratio decreases during an acute asthma attack. It takes a noticeably longer time to exhale than inhale @cite papiris2002asthma .  In %BioGears, the normal IE ratio is scaled using a multiplier based on severity to model decreased IE ratio during an acute asthma attack.

<table border="0">
<tr>
    <td><img src="./images/Respiratory/Respiratory_Figure25_AirwayResistance.png" width="550"></td>
    <td><img src="./images/Respiratory/Respiratory_Figure26_IERatio_Asthma.png" width="550"></td>
</tr>
<tr>
    <td><center><i>Figure 25. Airway flow resistance multiplier as a function of asthma severity. Airway flow resistance increases (during exhalation) with increasing asthma severity.</i></center></td>
    <td><center><i>Figure 26. IE ratio multiplier as a function of asthma severity. IE ratio decreases with increasing asthma severity.</i></center></td>
</tr>
</table>

@anchor respiratory-interventions
### Interventions

#### Occlusive Dressing

The management of an open tension pneumothorax requires sealing the open chest wound with an occlusive dressing. Such intervention slows the progression of tension pneumothorax and may ensure recovery if timely intervention takes place. The %BioGears %Respiratory Model simulates occlusive dressing by assigning large flow resistance across the element that serves as an electrical analogue open switch for the path linking the pleural cavity to the environment. Based on this implementation, the model calculates the physiological responses arising from numerical equivalent occlusive dressing.

#### Needle Decompression

Tension pneumothorax is a life-threatening condition that requires immediate intervention to relieve the air trapped in the pleural space and reduce the intrapleural pressure to the sub-atmospheric level. Needle decompression is a procedure that allows air to escape from the pleural space into the environment. This procedure relieves the accumulated air, allowing full expansion of the affected lung. The %BioGears %Respiratory Model is capable of simulating needle decompression by incorporating a circuit element that permits airflow based on pressure differential between the pleural space and the environment. The model then evaluates physiological responses to the intervention.

### Conscious Respiration 

Conscious respiration consists of a set of commands that model forced exhalation, forced inhalation, holding breath, and inhaler actuation. Collectively, they allow %BioGears to model coordinated use of an inhaler.  It should be noted that the conscious respiration action begins immediately when called, and will continue until completed while the simulation continues.  Users will likely want to advance time for the duration of the conscious respiration command before attempting other actions or completing a scenario.  The following commands can be used in any order and will wait until the completion of the previous command to begin:

- <b>Forced Exhale</b>: The "depth" or "shallowness" of the exhalation is determined by the expiratory reserve volume fraction, which specifies what portion of the expiratory reserve volume to exhale. If set to 1.0, the patient exhales his or her entire expiratory reserve volume, leaving only the residual volume in the lungs. The time period over which to execute the exhale is determined by the period. 
- <b>Forced Inhale</b>: The "depth" or "shallowness" of the inhalation is determined by the inspiratory capacity fraction, which specifies what portion of the inspiratory capacity to inhale. If set to 1.0, the patient inhales his or her entire inspiratory capacity, completely filling the lungs. The time period over which to execute the inhale is determined by the period. 
- <b>Breath Hold</b>: The time period to hold breath is determined by the period. 
- <b>Use %Inhaler</b>: The %Inhaler command is interpreted by the inhaler equipment (@ref InhalerMethodology).

Conscious respiration has any number of potential applications and is likely to be implemented to attain proper breathing while using an inhaler, generate a spirometry curve, or simulate coughing.  Figure 27 shows the %BioGears results for a cough scenario that leverages the conscious respiration action compared to empirical data.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/BioGears_Cough_Flow.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/Experimental_Cough_Flow.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 27. The airflow curve of a simulated cough in %BioGears generated from a conscious respiration with quick forced inhale and exhale in series. For comparison, the figure includes a plot reproduced from 
literature determined by a voluntary cough immediately following office-based vocal fold medialization injections @cite ruddy2014improved.</i>
</center><br>

### Mechanical ventilation

Mechanical ventilation allows the user to specify an instantaneous pressure and/or flow value at the respiratory connection point (i.e., mouth).  The substance volume fractions at the connection can also be specified.  If no volume fractions are given, the ambient values set in the %Environment system will be used by default (see @ref EnvironmentMethodology).  All settings will remain constant during the simulation, unless removed or modified.  This action is likely to be most beneficial implemented in combination with real-world sensors.  Manikin or task trainer values can be fed into the the engine to synchronize in real-time.

@anchor respiratory-events
Events
------

### Maximum Pulmonary Ventilation Rate

The %BioGears %Respiratory system triggers Maximum Pulmonary Ventilation Rate event to alert the user when the patient pulmonary ventilation exceeds maximum value.

### Bradypnea

Bradypnea is defined as an abnormally low breathing rate. Normal breathing rates for healthy adults are relatively standard, but can vary based upon condition. Low breathing rates can decrease total ventilation if not compensated for, which can lead to hypercapnia or hypoxia. In the %BioGears Engine, bradypnea occurs when the respiratory rate falls below 10 breaths per minute @cite overdyk2007continuous and returns to normal when it rises back above 10.5 breaths per minute.

### Tachypnea

Tachypnea is defined as high breathing rate. Normal respiration rate in an adult ranges 12 - 20 breaths per minute. Tachypnea occurs when breathing rate rises above 20 breaths per minute. The  %BioGears Engine engine outputs a tachypnea event when the patient's respiration rate is above 20 breaths per minute. 

### %Respiratory Acidosis

%Respiratory acidosis is triggered when the blood pH drops below 7.37.  An irreversible state (similar to a death state) is reached when the blood pH drops below 6.5.  See @ref EnergyMethodology for more details about acidosis.

### %Respiratory Alkalosis

%Respiratory alkalosis is triggered when the blood pH rises above 7.45.  An irreversible state (similar to a death state) is reached when the blood pH rises above 8.5.  See @ref EnergyMethodology for more details about alkalosis.

### Acute %Respiratory Distress

There are three levels of Acute %Respiratory Distress (ARD) - mild, moderate, and severe.  These levels are based on Carrico Index thresholds that provide insight into the effectiveness of the alveolar transfer of oxygen. 

@anchor respiratory-assessments
Assessments
-----------

### Pulmonary Function Test

The pulmonary function test allows the user to request a collection of respiratory data at a specified time step. This data is then used to produce a waveform representing normal inspiration and expiration. The waveform period is dictated by the respiration rate at the time of the pulmonary function test. The waveform magnitude is calculated from the tidal volume at the specified time and the functional residual capacity. The waveform is calculated for an artificial time of 60 seconds. This time is neither engine time nor real time, but instead is an assumed time, and the complete 60 second duration is calculated at the current engine time step. This does not indicate the duration of an actual pulmonary function test, but it does allow visualization of many breathing cycles. Within two periods of the 60 second duration, the waveform magnitude is modified to produce the inspiratory and expiratory reserve volume. The inspiratory and expiratory reserve volumes are presented as a forced inspiration and expiration. To model this effect, the period of the breathing cycle is dilated during the magnitude increase. At the conclusion of the imposed 60 second duration, a lung volume plot is produced  (see the figure in @ref respiratory-assessmentvalidation "Validation - Assessments") that displays the generated lung volume waveform.

@anchor respiratory-results
Results and Conclusions
=======================

The %BioGears %Respiratory System was validated quantitatively and qualitatively under resting physiological conditions and transient (action-induced) conditions. The validation is specified with a color coding system, with green indicating good agreement with trends/values, yellow indicating moderate agreement with trends/values, and red indicating poor agreement with trends/values.

@anchor respiratory-validation
Validation - Resting Physiologic State
--------------------------------------

The %BioGears %Respiratory Model outputs a number of system-level and compartment-level resting physiologic parameters. The tables below compare the values of system- and compartment-level parameters obtained from the model with referenced values. The outputs from the model correspond to the system- and compartment-level respiratory related values of the standard patient (77kg adult male)  under resting physiologic conditions. As shown in Table 3, the majority of the physiological variables show a good match with the literature. The outputs for the major respiratory variables, such as respiration rate, tidal volume, and pulmonary ventilation specifically match well with the reference data. 

<center>
<i>Table 3. Validation of the resting physiologic state of the %Respiratory System. The table shows comparison of system-level outputs from %BioGears to referenced values. System-level outputs show favorable agreement with validation data. The deviations in end tidal carbon dioxide fraction and transpulmonary pressure can be attributed to the tuned parameters used in the model that are selected to meet the major system level physiological parameters.</i>
</center><br>

@insert doc/validation/RespiratoryValidationTable.md

<center>
<i>Table 4. Validation table for the resting physiologic states. The table shows comparison of compartment-level outputs from %BioGears to referenced values. The majority of the variables show good match with the validation data. There are significant deviations in the carina flow and the dead space volumes. Future versions will address these issues.</i>
</center><br>

@insert doc/validation/RespiratoryCompartmentsValidationTable.md

@anchor respiratory-scenario
Scenario Validation
-------------------

The actions and interventions associated with the %Respiratory System were validated quantitatively where possible and qualitatively elsewhere by comparing the engine output to expected trends and values. For each scenario, the table shows the total number of results in each category. For many investigated scenarios, the model shows good agreement with the expected trends. For the scenarios that did not match with the expected trends, improvements are planned for future %BioGears Engine releases.

<center><br>
*Table 5. Cumulative validation results for %Respiratory specific conditions and actions scenarios.*
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	AsthmaAttackModerateAcute	|	Moderate acute asthma attack	|<span class="success">	18	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	AsthmaAttackSevereAcute	|	Severe acute asthma attack	|<span class="success">	14	</span>|<span class="warning">	3	</span>|<span class="danger">	1	</span>|
|	AsthmaAttackLifeThreateningAcute	|	Life threatening acute asthma attack	|<span class="success">	17	</span>|<span class="warning">	0	</span>|<span class="danger">	1	</span>|
|	COPDSevereEmphysema	|	GOLD Stage III Emphysema	|<span class="success">	4	</span>|<span class="warning">	5	</span>|<span class="danger">	1	</span>|
|	COPDSevereBronchitisLeft	|	Severe Chronic Bronchitis	|<span class="success">	4	</span>|<span class="warning">	5	</span>|<span class="danger">	1	</span>|
|	LobarPneumoniaSevereLeftLobe	|	Severe Lobar Pneumonia in one lobe in the left lung	|<span class="success">	5	</span>|<span class="warning">	2	</span>|<span class="danger">	2	</span>|
|	LobarPneumoniaSevereRightLung	|	Severe Lobar Pneumonia in two lobes of right lung	|<span class="success">	7	</span>|<span class="warning">	0	</span>|<span class="danger">	2	</span>|
|	LobarPneumoniaModerateBothLungs	|	Moderate Lobar Pneumonia in both lungs	|<span class="success">	6	</span>|<span class="warning">	0	</span>|<span class="danger">	3	</span>|
|	TensionPneumothoraxOpenVaried	|	Varied open pneumothorax severities and interventions	|<span class="success">	27	</span>|<span class="warning">	4	</span>|<span class="danger">	11	</span>|
|	TensionPneumothoraxClosedVaried	|	Varied closed pneumothorax severities and interventions	|<span class="success">	21	</span>|<span class="warning">	4	</span>|<span class="danger">	10	</span>|
|	TensionPneumothoraxBilateral	|	Open and closed bilateral pneumothorax and interventions	|<span class="success">	16	</span>|<span class="warning">	7	</span>|<span class="danger">	5	</span>|
|	AirwayObstructionVaried	|	Airway Obstruction with varying severities	|<span class="success">	30	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Bronchoconstriction	|	Bronchoconstriction with varying severities	|<span class="success">	25	</span>|<span class="warning">	1	</span>|<span class="danger">	4	</span>|
|	MainstemIntubation	|	Right and left mainstem intubation and correction (with Succs)	|<span class="success">	25	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	EsophagealIntubation	|	Esophageal intubation and correction (with Succs)	|<span class="success">	15	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Apnea	|	Varied severities of respiratory apnea	|<span class="success">	6	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|		|	Total	|<span class="success">	240	</span>|<span class="warning">	31	</span>|<span class="danger">	41	</span>|


@anchor respiratory-conditionvalidation
Validation - Conditions
-----------------------

### COPD

The COPD condition was validated against two scenarios. The severe emphysema scenario (Emphysema Severity = 0.7, Bronchitis Severity = 0.5) attempts to model Gold Stage III Emphysema. Here, most metrics show moderate to good agreement with trends, but a few poor results were obtained. We plan to investigate these issues in later versions.

<center><br>
<i>Table 6. Validation matrix for severe emphysema. This condition corresponds to GOLD Stage III COPD  @cite man2003contemporary  @cite perez2009can . The table shows the %BioGears engine output compared to validation data for respiratory and hemodynamic values.</i>
</center>

|	Segment	|	Notes	|	Sampled Scenario Time (s)	|	Trachea Flow - Peak Expiratory Flow  (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|	Aorta Lactate (mg/dL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Severe Emphysema: Bronchitis Severity = 0.5 Emphysema Severity = 0.7 Both Lungs 100%	|	GOLD Stage III	|	120	|<span class="success">	< 70% of Normal @cite perez2009can	</span>|<span class="success">	Increase,  Tachypnea, Dypsnea,  > 30 @cite gunning2003pathophysiology	</span>|<span class="success">	< 60% of normal @cite gunning2003pathophysiology	</span>|<span class="warning">	Increase, Tachycardia	</span>|<span class="danger">	Increase,   Pulmonary Hypertension ,  > 140 mm Hg @cite keller2003pathophysiology, @cite scharf2002hemodynamic	</span>|<span class="warning">	< 90%  @cite man2003contemporary; < 89% @cite keller2003pathophysiology	</span>|<span class="warning">	Decrease,  Hypoxemia @cite keller2003pathophysiology; < 55 mm Hg  @cite man2003contemporary, @cite keller2003pathophysiology	</span>|<span class="warning">	Increase, Hypercapnia, > 55 mmHg @cite keller2003pathophysiology	</span>|<span class="success">	Decrease @cite van1991physical	</span>|<span class="warning">	Increase @cite mathur1999cerebral	</span>|

<center><br>
<i>Table 7. Validation matrix for severe chronic bronchitis. The table shows the %BioGears engine output compared to validation data for respiratory and hemodynamic values.</i>
</center>

|	Segment	|	Notes	|	Sampled Scenario Time (s)	|	Trachea Flow - Peak Expiratory Flow  (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|	Aorta Lactate (mg/dL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Severe Chronic Bronchitis: Bronchitis Severity = 0.65 Emphysema Severity = 0.5 Both Lungs 100%	|		|	120	|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="success">	> 20 @cite bergeronSME	</span>|<span class="success">	< 60% of normal @cite bergeronSME	</span>|<span class="warning">	Increase,  Tachycardia @cite bergeronSME	</span>|<span class="danger">	Increase,  Pulmonary Hypertension,  > 140 mm Hg @cite bergeronSME	</span>|<span class="warning">	< 90% @cite bergeronSME	</span>|<span class="warning">	Decrease,  Hypoxemia,  < 55 mm Hg  @cite bergeronSME	</span>|<span class="warning">	Increase,   Hypercapnia > 55 mmHg   @cite bergeronSME	</span>|<span class="success">	Decrease @cite van1991physical, @cite bergeronSME	</span>|<span class="warning">	Increase @cite mathur1999cerebral	</span>|

### Lobar Pneumonia

The lobar pneumonia condition was validated against three scenarios; severe pneumonia in the one lobe of the left lung (Severity = 0.7), severe pneumonia in two lobes of the right lung (Severity = 0.7), and moderate pneumonia in both lungs (Severity = 0.2). %BioGears does not model the discrete lobes of each lung, so infected-lobe behavior is modeled by applying pneumonia severity against a fraction of each lung. The fraction approximates the portion of the lung (by volume) that the lobe(s) occupy. In the case of the left lung, there are two lobes. Each lobe constitutes approximately 50% of the whole lung. It is understood that the actual volume distribution is not equal for the two lobes, but we use this as an approximation. The same is true for the right lung, which has three lobes. In all three scenarios, expected general trends are observed for most measures. However, heart rate and blood pressure do not follow expected trends. This is similar to COPD, where heart rate and blood pressure also do not always behave as expected. Additionally, increasing core body temperature is also a symptom of lobar pneumonia that fails to agree with the validation data. However, we expected this disagreement since %BioGears doesn't currently model elevated body temperature in response to infection. This is something we plan to address in subsequent releases. 
<center><br>
<i>Table 8. Validation matrix for severe lobar pneumonia concentrated in one lobe in the left lung. The table shows the %BioGears engine output compared to validation data for respiratory and hemodynamic values.</i>
</center>

|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|	Core Body Temperature  (Degrees C)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Severe Lobar Pneumonia:  Severity = 0.70 Left Lung = 50% Right Lung = 0%	|	Severe Lobar Pneumonia in one lobe in the left lung. Current model does not include metabolic effects.	|	120	|<span class="success">	Increase,  Tachypnea, Dypsnea  > 20 @cite ebell2007predicting	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="warning">	Increase,  Tachycardia, > 100 @cite ebell2007predicting	</span>|<span class="danger">	Decrease, < 90 mm Hg @cite fine1997prediction	</span>|<span class="warning">	Decrease , < 95% @cite majumdar2010oxygen	</span>|<span class="danger">	Decrease,  Hypoxemia @cite fine1997prediction < 60 mm Hg	</span>|<span class="success">	Increase @cite bergeronSME	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="danger">	Increase,   > 37.8 C (100 F) @cite ebell2007predicting	</span>|

<center><br>
<i>Table 9. Validation matrix for severe lobar pneumonia concentrated in two lobes in the right lung. The table shows the %BioGears engine output compared to validation data for respiratory and hemodynamic values.</i>
</center>

|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|	Core Body Temperature  (Degrees C)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Severe Lobar Pneumonia: Right Lung Severity = 0.70 Left Lung = 0% Right Lung  = 67%	|	Severe lobar pneumonia in two lobes of the  right lung. Current model does not include metabolic effects.	|	120	|<span class="success">	Increase,  Tachypnea, Dypsnea  > 20 @cite ebell2007predicting	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="warning">	Increase,  Tachycardia, > 100 @cite ebell2007predicting	</span>|<span class="danger">	Decrease, < 90 mm Hg @cite fine1997prediction	</span>|<span class="success">	Decrease , < 95% @cite majumdar2010oxygen	</span>|<span class="warning">	Decrease,  Hypoxemia @cite fine1997prediction < 60 mm Hg	</span>|<span class="success">	Increase @cite bergeronSME	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="danger">	Increase,   > 37.8 C (100 F) @cite ebell2007predicting	</span>|

<center><br>
<i>Table 10. Validation matrix for moderate lobar pneumonia in both lungs. The table shows the %BioGears engine output compared to validation data for respiratory and hemodynamic values.</i>
</center>

|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|	Core Body Temperature  (Degrees C)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Moderate Lobar Pneumonia: Severity = 0.2 Left Lung = 100% Right Lung  = 100%	|	Moderate pneumonia in both lungs. Current model does not include metabolic effects.	|	120	|<span class="success">	Increase,  Tachypnea, Dypsnea  > 20 @cite ebell2007predicting	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="danger">	Increase,  Tachycardia, > 100 @cite ebell2007predicting	</span>|<span class="danger">	Decrease, < 90 mm Hg @cite fine1997prediction	</span>|<span class="warning">	Decrease , < 95% @cite majumdar2010oxygen	</span>|<span class="danger">	Decrease,  Hypoxemia @cite fine1997prediction < 60 mm Hg	</span>|<span class="success">	Increase @cite bergeronSME	</span>|<span class="success">	Decrease @cite bergeronSME	</span>|<span class="danger">	Increase,   > 37.8 C (100 F) @cite ebell2007predicting	</span>|

@anchor respiratory-actionvalidation
Validation - Actions
--------------------

### Airway Obstruction

The airway obstruction action was validated with a scenario that applies an airway obstruction action with varying severities, 0.3, 0.6, and 1.0. Varying the severity of the obstruction simulates partial obstructions and a complete obstruction. The severity levels logarithmically increase the airway resistance, as shown in Figure 22. 

The resulting outputs from the %BioGears engine are shown in Table 11 for common clinical parameters, such as heart rate, respiratory rate, and oxygen saturation. In the event of complete airway obstruction, a drop in oxygen saturation and respiration rate are expected. In contrast, hemodynamic variables, such as heart rate, are expected to increase. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/AirwayObstruction_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AirwayObstruction_CO2PP.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/AirwayObstruction_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AirwayObstruction_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/AirwayObstructionLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 28. Select outputs from the airway obstruction scenario.</i></center>
 
<center><br>
<i>Table 11. Validation matrix for physiological responses due to varying severities of airway obstruction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Trigger airway obstruction;  severity = 0.3	|	Represents the partial (severity = 0.3) obstruction of airway due to  foreign body (obstructing material). Not severe enough to trigger CV responses.	|	30	|	180	|<span class="success">	 Increase  @cite Morgan2006Clinical	</span>|<span class="success">	Increase or no change @cite bergeronSME	</span>|<span class="success">	Increase or no change @cite bergeronSME(case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Increase @cite bergeronSME (case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Decrease @cite Morgan2006Clinical	</span>|
|	Remove airway obstruction	|	Airway obstruction is turned off.	|	210	|	300	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|
|	Turn on airway obstruction;  severity = 0.6	|	Represents the partial obstruction (severity = 0.6) of airway due to  foreign body.	|	510	|	180	|<span class="success">	Increase > Severity 0.3  @cite Morgan2006Clinical	</span>|<span class="success">	Increase @cite bergeronSME	</span>|<span class="success">	Increase @cite bergeronSME  (case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Increase @cite bergeronSME (case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Decrease < Severity 0.3 @cite Morgan2006Clinical	</span>|
|	Remove airway obstruction	|	Airway obstruction is turned off.	|	690	|	300	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|
|	Turn on airway obstruction;  severity = 1.0	|	Represents the complete obstruction (severity = 1.0) of airway due to  foreign body.	|	990	|	180	|<span class="success">	Increase  > Severity 0.6 @cite Morgan2006Clinical	</span>|<span class="success">	Increase @cite bergeronSME	</span>|<span class="success">	Increase @cite bergeronSME  (case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Increase @cite bergeronSME  (case study of awake patient @cite izakson2013complete)	</span>|<span class="success">	Decrease < Severity 0.6 @cite Morgan2006Clinical	</span>|
|	Remove airway obstruction	|	Airway obstruction is turned off.	|	1170	|	500	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|

### Bronchoconstriction

The bronchoconstriction action was validated with a scenario that applies a series of bronchoconstrictions with varying severities, 0.3, 0.6, and 1.0 to the patient. Varying the severity of the obstruction simulates the constriction of the bronchii, leading to obstructed air flow. The severity levels logarithmically increase the airway resistance, as shown in Figure 22.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/BronchoConstrictionVaried_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/BronchoConstrictionVaried_CO2PP.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/BronchoConstrictionVaried_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/BronchoConstrictionVaried_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/BronchoConstrictionVariedLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 29. Select outputs from the bronchoconstriction scenario.</i></center>

<center><br>
<i>Table 12. Validation matrix for physiological responses due to varying severities of bronchoconstriction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sampled Scenario Time  (s)	|	Respiration Rate (breaths/min)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Trigger Bronchoconstriction; severity 0.3	|	Trigger bronchoconstriction of severity 0.3 by Increasing effective breathing resistances on bronchi. Not severe enough to trigger CV responses.	|	30	|	180	|<span class="warning">	Decrease  @cite Morgan2006Clinical	</span>|<span class="success">	No change	</span>|<span class="success">	No change	</span>|<span class="success">	No change	</span>|<span class="success">	Decrease  @cite Morgan2006Clinical	</span>|
|	Remove bronchoconstriction	|	Bronchoconstriction is turned off.	|	210	|	300	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|
|	Trigger Bronchoconstriction;   severity = 0.6	|	Trigger bronchoconstriction of severity 0.6 by Increasing effective breathing resistances on bronchi.	|	510	|	180	|<span class="success">	Increase > Severity 0.3  @cite Morgan2006Clinical	</span>|<span class="success">	Increase  tachycardia  @cite faust2002anesthesiology, @cite Morgan2006Clinical	</span>|<span class="danger">	Decrease hypotension  @cite faust2002anesthesiology	</span>|<span class="danger">	Decrease hypotension  @cite faust2002anesthesiology	</span>|<span class="success">	Decrease < Severity 0.3   @cite Morgan2006Clinical	</span>|
|	Remove bronchoconstriction	|	Bronchoconstriction is turned off.	|	690	|	300	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|
|	Trigger Bronchoconstriction;   severity = 1.0	|	Trigger bronchoconstriction of severity 1.0 by Increasing effective breathing resistances on bronchi.	|	990	|	180	|<span class="success">	Increase  > Severity 0.6 @cite Morgan2006Clinical	</span>|<span class="success">	Increase  tachycardia  @cite faust2002anesthesiology, @cite Morgan2006Clinical	</span>|<span class="danger">	Decrease hypotension  @cite faust2002anesthesiology	</span>|<span class="danger">	Decrease hypotension  @cite faust2002anesthesiology	</span>|<span class="success">	Decrease < Severity 0.6   @cite Morgan2006Clinical	</span>|
|	Remove bronchoconstriction	|	Bronchoconstriction is turned off.	|	1170	|	500	|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|<span class="success">	returns to resting physiology 	</span>|

### Pneumothorax

Three scenarios were used to validate tension pneumothorax and its associated interventions, occlusive dressing, and needle decompression. Table 13 demonstrates the open tension pneumothorax on the left side of the chest at increasing severities treated with an occlusive dressing and needle decompression. The %BioGears engine output for relevant parameters was compared to the validation data. The blood pressures (systolic, diastolic, and mean arterial pressure) do not show hypotension during the open wound pneumothorax incidence. Other respiratory and cardiovascular outputs show good agreement with published data. Vital signs become more dire the longer the pneumothorax is applied without intervention and as the severity (i.e. hole size) is increased. Evidence suggests that patients have progressive respiratory deterioration with final respiratory arrest @cite leigh2005tension. However, the current model does not include a definition for respiratory arrest and will continue to run until another irreversible state is reached.

After an occlusive dressing is added to seal the hole and needle decompression is applied, the patient's condition improves and vitals begin returning toward normal.  This is as expected compared to the validation data. 

It is important to note nervous system responses of a conscious patient due to pain or panicking are not included with the Pneumothorax action. Some of these responses can be attained by including an Acute Stress action (see @ref endocrine-actions "Endocrine Methodology").

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxOpenVaried_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/TensionPneumothoraxOpenVaried_LeftLungVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxOpenVaried_O2Sat.jpg" width="550"></td>
	<td><img src="./plots/Respiratory/TensionPneumothoraxOpenVaried_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/TensionPneumothoraxOpenVariedLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 30. Select outputs from the open tension pneumothorax scenario.</i></center>

<center><br>
<i>Table 13. Validation matrix for physiological responses due to varying severities of open tension pneumothorax, as well as occlusive dressing and needle decompression interventions.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Tension Pneumothorax;  severity = 0.0	|	A severity of zero should not change the leak resistance from the default open switch value.	|	0	|	15	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|
|	Tension Pneumothorax;  severity = 0.3	|	Represents a "sucking" chest wound on the left lung side with a mild severity (medium size hole).	|	30	|	300	|<span class="success">	 No change or Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	 No change or Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|
|	Tension Pneumothorax;  severity = 0.6	|	Represents a "sucking" chest wound on the left lung side with a moderate severity (large size hole).	|	360	|	600	|<span class="success">	 Increase @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	Increase @cite leigh2005tension	</span>|<span class="success">	Increase @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|<span class="success">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|
|	Tension Pneumothorax;  severity = 1.0	|	Represents a "sucking" chest wound on the left lung side with the severe severity (maximum size hole).  Should lead to full collapse.	|	660	|	900	|<span class="success">	 Clinical sign: Tachypnea  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	Increase @cite leigh2005tension	</span>|<span class="success">	Clinical sign: Tachycardia  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension @cite bond200968w, @cite leigh2005tension	</span>|<span class="success">	Clinical sign: Hypoxia  @cite bond200968w, @cite leigh2005tension	</span>|
|	Apply Chest Occlusive dressing on the left side 	|	Represents the closing of chest wound on the left side	|	960	|	1000	|<span class="success">	No Change or Increased  @cite bergeronSME	</span>|<span class="success">	No Change 	</span>|<span class="success">	Tachycardia > 120 @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Modest decrease to no change  @cite bergeronSME	</span>|
|	Apply needle decompression	|	Needle decompression is applied on the left side of the chest	|	1020	|	1320	|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="warning">	Returning toward normal @cite bergeronSME	</span>|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="success">	Normal @cite bergeronSME	</span>|

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxClosedVaried_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/TensionPneumothoraxClosedVaried_RightLungVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxClosedVaried_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/TensionPneumothoraxClosedVaried_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/TensionPneumothoraxClosedVariedLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 31. Select outputs from the closed tension pneumothorax scenario.</i></center>

<center><br>
<i>Table 14. Validation matrix for physiological responses due to varying severities of closed tension pneumothorax, as well as needle decompression interventions.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Tension Pneumothorax;  severity = 0.0	|	A severity of zero should not change the leak resistance from the default open switch value.	|	0	|	15	|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|<span class="success">	No Change 	</span>|
|	Tension Pneumothorax;  severity = 0.3	|	Represents a leak on the right lung side with a mild severity (medium size hole).	|	30	|	300	|<span class="success">	 No change or Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	 No change or Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	 No change or Decrease  @cite bond200968w, @cite leigh2005tension 	</span>|
|	Tension Pneumothorax;  severity = 0.6	|	Represents a leak on the right lung side with a moderate severity (large size hole).	|	360	|	600	|<span class="success">	 Increase @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	Increase @cite leigh2005tension	</span>|<span class="success">	Increase  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|<span class="success">	Decrease @cite bond200968w, @cite leigh2005tension	</span>|
|	Tension Pneumothorax;  severity = 1.0	|	Represents a leak on the right lung side with the severe severity (maximum size hole).  Should lead to full collapse.	|	660	|	900	|<span class="success">	 Clinical sign: Tachypnea      @cite bond200968w, @cite leigh2005tension 	</span>|<span class="success">	Increase @cite leigh2005tension	</span>|<span class="success">	Clinical sign: Tachycardia   @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Clinical sign: Hypotension  @cite bond200968w, @cite leigh2005tension	</span>|<span class="success">	Clinical sign: Hypoxia   @cite bond200968w, @cite leigh2005tension	</span>|
|	Apply needle decompression	|	Needle decompression is applied on the right side of the chest	|	960	|	1260	|<span class="success">	Slightly lower tachypnea  @cite bergeronSME	</span>|<span class="warning">	Returning toward normal @cite bergeronSME	</span>|<span class="success">	Tachycardia, but may be lower value  @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="success">	Normal @cite bergeronSME	</span>|

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxBilateral_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/TensionPneumothoraxBilateral_TranspulmonaryPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/TensionPneumothoraxBilateral_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/TensionPneumothoraxBilateral_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/TensionPneumothoraxBilateralLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 32. Select outputs from the bilateral tension pnemothorax scenario.</i></center>

<center><br>
<i>Table 15. Validation matrix for physiological responses due to open tension pneumothorax on one side in combination with closed tension pneumothorax on the other side, as well as occlusive dressing for the open side and needle decompression interventions for both sides.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Tension Pneumothorax;  Severity: 0.5 Type: Open Side: Left Tension Pneumothorax;  Severity: 0.5 Type: Closed Side: Right	|	Represents a leak on the right and left lung sides simultaneously with a moderate severity (large size hole).	|	30	|	300	|<span class="success">	 Increase  @cite bond200968w, @cite leigh2005tension 	</span>|<span class="danger">	Increase @cite leigh2005tension	</span>|<span class="danger">	Increase  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease  @cite bond200968w, @cite leigh2005tension	</span>|<span class="danger">	Decrease  @cite bond200968w, @cite leigh2005tension	</span>|<span class="success">	Decrease  @cite bond200968w, @cite leigh2005tension	</span>|
|	Apply Chest Occlusive dressing on the left side 	|	Represents the closing of chest wound on the left side	|	330	|	360	|<span class="success">	No Change or Increased  @cite bergeronSME	</span>|<span class="success">	No Change 	</span>|<span class="warning">	Tachycardia > 120 @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Slightly improved to no change  @cite bergeronSME	</span>|<span class="success">	Modest decrease to no change  @cite bergeronSME	</span>|
|	Apply needle decompression on the left side 	|	Needle decompression is applied on the left side of the chest	|	390	|	400	|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="warning">	Diminished from normal @cite bergeronSME	</span>|<span class="warning">	Diminished from normal @cite bergeronSME	</span>|<span class="warning">	Diminished from normal @cite bergeronSME	</span>|<span class="success">	Returning toward normal @cite bergeronSME	</span>|
|	Apply needle decompression on the right side 	|	Needle decompression is applied on the right side of the chest	|	420	|	720	|<span class="success">	Slightly lower tachypnea  @cite bergeronSME	</span>|<span class="success">	Returning toward normal @cite bergeronSME	</span>|<span class="success">	Tachycardia, but may be lower value  @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="warning">	Slightly diminished from normal @cite bergeronSME	</span>|<span class="success">	Normal @cite bergeronSME	</span>|


### Mainstem Intubation

The right and left mainstem intubation actions were validated with a scenario that simulates mechanical ventilation with improper tracheal tube placement. In this scenario, the patient is injected with succinylcholine, then mechanically ventilated through the placement of the endotracheal tube. During the intubation action, the tube is incorrectly placed, leading to the right and left mainstem intubation. The mainstem intubations are then set to the trachea for proper function.

Common clinical vital signs were compared to validation data, as shown in Table 16.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/MainstemIntubation_LeftLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/MainstemIntubation_TidalVolume.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/MainstemIntubation_RightLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/MainstemIntubation_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/MainstemIntubationLegend.jpg" width="800"></td>
</tr>
</table>
</center>
<center><i>Figure 33. Select outputs from the right mainstem intubation scenario.</i></center>

<center><br>
<i>Table 16. Validation matrix for physiological responses due to varying severities of right mainstem intubation and correction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sampled Scenario Time  (s)	|	Respiration Rate (breaths/min)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Inject succinylcholine; concentration = 4820 ug/ml; Dose = 20 ml	|	Neuromuscular blocker agent is administered to represent events that facilitate endotracheal intubation.	|	30	|	60	|<span class="success">	Goes to Zero @cite Morgan2006Clinical             @cite dukeSME	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical                       5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical                      5-10% Decrease @cite dukeSME	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical                       5-10% Decrease @cite dukeSME	</span>|<span class="success">	Begins to drop according to preoxygenated O2 curve 	</span>|
|	Turn on Anesthesia Machine and insert   Endotracheal (ET) tube in the right mainstem	|	Anesthesia machine is turned on to supply oxygen to the subject. ET tube is inserted, but it is set in the right mainstem.	|	60	|	180	|<span class="success">	16  (set by machine) 	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	Continues to drop; 93-95% @cite bergeronSME	</span>|
|	Reset ET tube in the trachea.	|	 ET tube is adjusted to be set in the trachea (good depth).	|	180	|	480	|<span class="success">	16  (set by machine) 	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|
|	Turn on Anesthesia Machine and insert   Endotracheal (ET) tube in the left mainstem	|	ET tube is adjusted, but it is set in the left mainstem.	|	480	|	600	|<span class="success">	16  (set by machine) 	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	No Change @cite dukeSME; Increase above normal due to cardiovascular stress  @cite bergeronSME	</span>|<span class="success">	Continues to drop; 93-95% @cite bergeronSME	</span>|
|	Reset ET tube in the trachea.	|	 ET tube is adjusted to be set in the trachea (good depth).	|	600	|	900	|<span class="success">	16  (set by machine) 	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|


### Esophageal Intubation

The esophageal intubation action was validated with a scenario with multiple intubation attempts, a failed esophageal intubation following by a successful endotracheal intubation. In this scenario, the patient is injected with succinylcholine, followed by mechanical ventilation via an endotracheal tube. The first attempt at intubation results in an esophageal intubation. A second attempt is successful. 

All clinical outputs validated agreed with expected trends. As previously mentioned, oxygen saturation shows a decrease as expected within the sampled time. A longer run time for the scenario would show a sharper decrease in the oxygen saturation. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/EsophagealIntubation_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/EsophagealIntubation_O2Sat.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/EsophagealIntubation_StomachInflow.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/EsophagealIntubation_MAP.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/EsophagealIntubationLegend.jpg" width="800"></td>
</tr>
</table>
</center>
<center><i>Figure 34. Select outputs from the esophageal intubation scenario.</i></center>

<center><br>
<i>Table 17. Validation matrix for physiological responses due esophageal intubation and correction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time  (s)	|	Sample Scenario Time  (s)	|	Respiration Rate  (breaths/min)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Oxygen Saturation  (mmHg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Succinylcholine - 96.4mg	|	Drug Onset < 1 minute	|	30	|	30-50	|<span class="success">	Goes to Zero @cite Morgan2006Clinical             @cite dukeSME	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical;                       5-10% Decrease @cite dukeSME    	</span>|<span class="succes">	Mild  Increase @cite Morgan2006Clinical;                       5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical;                       5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Begins to drop according to preoxygenated O2 curve 	</span>|
|	Esophageal Intubation	|	This represents failed  endotracheal intubation that led to esophageal intubation	|	60	|	120	|<span class="success">	Zero 	</span>|<span class="success">	 If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	 If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	 If undetected and O2 level drops significantly, Mild Increase @cite dukeSME	</span>|<span class="success">	continue to decrease along O2 curve 	</span>|
|	Successful Intubation	|	Represents successful endotracheal intubation	|	180	|	500	|<span class="success">	16  (set by machine) 	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|<span class="success">	Towards normal @cite bergeronSME	</span>|

### Asthma

The acute asthma action was validated against three scenarios: moderate (severity = 0.3), severe (severity = 0.7), and life-threatening (severity = 0.9). In all three cases, general trends for respiration rate, trachea flow, tidal volume, and IE ratio fall in line with expected behavior. CO<SUB>2</SUB> partial pressure and O<SUB>2</SUB> saturation also tend to agree with expected values. However, blood pressure and heart rate are less consistent. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackModerateAcute_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackModerateAcute_IERatio.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackModerateAcute_O2PP.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackModerateAcute_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/AsthmaAttackModerateAcuteLegend.jpg" width="300"></td>
</tr>
</table>
</center>
<center><i>Figure 35. Select outputs from the moderate airway obstruction scenario.</i></center>

<center><br>
<i>Table 18. Validation matrix for physiological responses due to moderate acute asthma and correction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Trachea Flow - Peak Expiratory Flow  (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Moderate acute asthma attack (Severity = 0.3)	|		|	30	|	550	|<span class="success">	75% to 90% of normal @cite britishGuideline2014asthma	</span>|<span class="success">	Slight Increase @cite britishGuideline2014asthma	</span>|<span class="success">	 ~95% of normal @cite limmer2011emergency	</span>|<span class="success">	No Change @cite britishGuideline2014asthma	</span>|<span class="success">	No Change @cite papiris2001clinical	</span>|<span class="success">	No Change @cite britishGuideline2014asthma	</span>|<span class="success">	No Change @cite britishGuideline2014asthma	</span>|<span class="success">	No Change @cite britishGuideline2014asthma	</span>|<span class="success">	Decrease @cite van1991physical	</span>|
|	No asthma (Severity = 0.0)	|		|	580	|	775	|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackSevereAcute_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackSevereAcute_IERatio.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackSevereAcute_O2PP.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackSevereAcute_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/AsthmaAttackSevereAcuteLegend.jpg" width="300"></td>
</tr>
</table>
</center>
<center><i>Figure 36. Select outputs from the severe asthma attack scenario.</i></center>

<center><br>
<i>Table 19. Validation matrix for physiological responses due to severe acute asthma and correction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Trachea Flow - Peak Expiratory Flow  (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Severe acute asthma attack (Severity = 0.7)	|		|	30	|	550	|<span class="success">	< 50% of Normal @cite papiris2001clinical, @cite britishGuideline2014asthma	</span>|<span class="success">	> 25 @cite britishGuideline2014asthma	</span>|<span class="success">	< 60% of normal @cite limmer2011emergency	</span>|<span class="warning">	> 120 @cite papiris2001clinical; > 110 @cite britishGuideline2014asthma	</span>|<span class="danger">	Decrease Pulsus Paradoxus 85 mm Hg @cite papiris2001clinical	</span>|<span class="warning">	< 90% @cite papiris2001clinical; < 92% @cite britishGuideline2014asthma	</span>|<span class="warning">	Decrease Mild Hypoxemia @cite bergeronSME; < 60 mmHg @cite papiris2001clinical, @cite britishGuideline2014asthma	</span>|<span class="success">	Normal or Slight Increase > 45 mm Hg @cite papiris2001clinical, @cite britishGuideline2014asthma 	</span>|<span class="success">	Decrease @cite van1991physical	</span>|
|	No asthma (Severity = 0.0)	|		|	580	|	775	|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackLifeThreateningAcute_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackLifeThreateningAcute_IERatio.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Respiratory/AsthmaAttackLifeThreateningAcute_O2PP.jpg" width="550"></td>
    <td><img src="./plots/Respiratory/AsthmaAttackLifeThreateningAcute_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Respiratory/AsthmaAttackLifeThreateningAcuteLegend.jpg" width="300"></td>
</tr>
</table>
</center>
<center><i>Figure 37. Select outputs from the life threatening asthma attack scenario.</i></center>

<center><br>
<i>Table 20. Validation matrix for physiological responses due to life threatening acute asthma and correction.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Trachea Flow - Peak Expiratory Flow  (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Oxygen Saturation	|	PaO2 (mmHg)	|	PaCO2 (mmHg)	|	IERatio	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Life threatening acute asthma attack (Severity = 0.9) 	|		|	30	|	550	|<span class="success">	< 33% of Normal @cite britishGuideline2014asthma	</span>|<span class="success">	> 25 @cite britishGuideline2014asthma	</span>|<span class="success">	< 60% of normal @cite limmer2011emergency	</span>|<span class="success">	> 120 @cite papiris2001clinical; > 110 @cite britishGuideline2014asthma Arrhythmia, @cite britishGuideline2014asthma	</span>|<span class="danger">	Decrease NO Pulsus Paradoxus 85 mm Hg @cite papiris2001clinical	</span>|<span class="success">	< 92% @cite britishGuideline2014asthma	</span>|<span class="success">	< 60 mmHg @cite britishGuideline2014asthma	</span>|<span class="success">	Normal or Slight Increase > 45 mm Hg @cite papiris2001clinical, @cite britishGuideline2014asthma 	</span>|<span class="success">	Decrease @cite van1991physical	</span>|
|	No asthma (Severity = 0.0)	|		|	580	|	775	|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|

### Apnea

Apnea directly effects the achieved respiratory driver (breathing muscles) pressure amplitude.  The more severe the apnea, the lower the tidal volume.

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Tidal Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|
|	Apnea: Severity = 0.3	|	Mild	|	30	|	210	|<span class="success">	Decrease to ~70% of healthy	</span>|
|	Apnea: Severity = 0.0	|	Healthy	|	210	|	510	|<span class="success">	Normal	</span>|
|	Apnea: Severity = 0.6	|	Moderate	|	510	|	690	|<span class="success">	Decrease to ~30% of healthy	</span>|
|	Apnea: Severity = 0.0	|	Healthy	|	690	|	990	|<span class="success">	Normal	</span>|
|	Apnea: Severity = 1.0	|	Severe	|	990	|	1170	|<span class="success">	Decrease to 0	</span>|
|	Apnea: Severity = 0.0	|	Healthy	|	1170	|	1650	|<span class="success">	Normal	</span>|

### Conscious Respiration 

Conscious respiration is validated as part of inhaler usage described in the %Inhaler methodology section @ref InhalerMethodology.

@anchor respiratory-assessmentvalidation
Validation - Assessments
------------------------

### Pulmonary Function Test

Validation of the pulmonary function test may be concluded from the validation of the resting physiologic quantities. The collection of values produced from the pulmonary function test were validated above. Additional validation comes by comparing the general shape of the produced waveform to an expected waveform. The plot shown in Figure 38 presents a way of representing the %BioGears pulmonary function test visually. There is excellent agreement with the plot produced from the engine and the plot found in the literature @cite Kapwatt2014Lungvolumes.

<center><img src="./plots/Respiratory/Pulmonary_Function_Test_Results.jpg" width="1000"></center>
<center>
<i>Figure 38. The lung volume plot from the pulmonary function test displays the lung volume waveform. The waveform has a frequency associated with the respiration rate of the patient at the time of the pulmonary function test. The inspiratory and expiratory reserve volumes are shown with a dilated period to represent a forced component of the inspiration and expiration. The lung volume plot shown is presented with 100 data points.</i>
</center><br>

@anchor respiratory-conclusion
Conclusion
----------

Mathematical modeling of the pulmonary physiology dates back to the work of Gray (1945) and others, including Guyton and collaborators @cite milhorn1965mathematical . Since then several mathematical models have been developed that describe aspects of the pulmonary physiology in considerable detail. However, the scope and complexity of the human physiology demands a model that integrates the dynamic respiratory physiology with other systems such as cardiovascular, renal, and others. As a result, there is a need for an integrative modeling environment that serves as a tool for predicting aspects of the human physiology and pathology.

The %BioGears %Respiratory Model provides a system-level computational model that allows for real-time simulation of normal pulmonary physiology and respiratory distresses. The model employs a circuit-based lumped parameter mathematical model that allows for the characterization of patient-specific respiratory mechanics at a relatively low computational burden. The coupling of the %BioGears %Respiratory System with the Circulatory System through gas exchange and respiratory distress modifiers permits robust prediction of respiratory effects on hemodynamic variables. Accordingly, the current version of the %BioGears %Respiratory System is capable of predicting respiratory and hemodynamic responses to various insults like airway obstruction, bronchoconstriction, pneumothorax, and others.

As part of a modeling tool for various clinical settings, the %Respiratory Model can be hooked up to the anesthesia machine for mechanical ventilation. During positive pressure ventilation, the %Respiratory Model permits the transport of inhaled volatile gases and others from the anesthesia machine to the circulatory system. Such an integrative approach allows for the examination of respiratory and cardiovascular responses to various stimuli and stresses as observed in practical application.

The %Respiratory System, along with the other systems in the engine, can serve as a training tool for exploring the dynamic interaction between physiological systems. Researchers can benefit from the integration of higher-fidelity modules into the existing model for more detailed and project-specific analysis. In its current form, the %Respiratory Model is adapted to a specific patient using physiological parameters selected from patient data. This limits the practical application of the model to the tested standard model. However, the common data model platform gives the user the ease and flexibility to select parameters to experiment their own patient setting.

@anchor respiratory-future
Future Work
===========

@anchor respiratory-coming
Coming Soon
-----------

@anchor respiratory-improvements
Recommended Improvements
------------------------

The development of an integrative model like the %BioGears %Respiratory System provides opportunities for exploring various pulmonary diseases and distresses. One common pulmonary distress that can be modeled by the %BioGears %Respiratory Model corresponds to the Acute %Respiratory Distress Syndrome (ARDS). This condition compromises alveolar gas transfer and leads to low levels of oxygen in the blood. Future funding may be sought for incorporating and validating ARDS model in the %BioGears %Respiratory System.

A respiratory control mechanism for rhythmic breathing could be added to the engine. Rhythmic breathing is achieved by the respiratory neurons in the medullary respiratory center that give rise to activity in the efferent nerves ennervating the respiratory muscles. The neural control network that relates the respiratory neuron output to the mechanical airflow is not incorporated in the current version of the %Respiratory Model. The current version of the %BioGears  %Respiratory Model has succeeded in controlling the breathing cycle through parameters that  adjust the spontaneous breathing based on a chemical control feedback mechanism.  While this development allows robust chemical stimuli responses,  incorporation of the neural circuitry model that controls the ventilation process can provide a more accurate implementation of the %Respiratory System that includes behavioral control of breathing.

@anchor respiratory-appendices
Appendices
==========

Acronyms
--------

ARD - Acute %Respiratory Distress

RV - Residual volume

ERV - Expiratory reserve volume

V<sub>T</sub> - Tidal volume

IRV - Inspiratory reserve volume

V<sub>C</sub>  - Vital capacity

IC - Inspiratory capacity

FRC - Functional residual capacity

TLC - Total lung capacity

RR - Respiration rate

V<sup><b>.</b></sup><sub>E</sub> - Minute ventilation

V<sup><b>.</b></sup><sub>A</sub> - Alveolar ventilation

Data Model Implementation
-------------------------

@ref RespiratorySystemTable "Respiratory"

Compartments
------------

* Mouth
* Stomach
* Carina
* Lungs
	* LeftLung
		* LeftDeadSpace
		* LeftAlveoli
	* RightLung, RightLungPulmonary
		* RightDeadSpace
		* RightAlveoli
* PleuralCavity
	* LeftPleuralCavity
	* RightPleuralCavity
* LeftAlveoliLeak
* LeftChestLeak
* RightAlveoliLeak
* RightChestLeak
