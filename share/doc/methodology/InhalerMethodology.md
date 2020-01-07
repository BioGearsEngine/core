%Inhaler Methodology {#InhalerMethodology}
==============================

@anchor inhaler-overview
Overview
========
@anchor inhaler-abstract
Abstract
--------

The %BioGears&reg; %Inhaler Model is a generic representation of a pressurized metered dose inhaler (pMDI).  The results show an excellent correlation with the expected trends.  Future work will address the current limitations of the system, including incorrect use of the device and re-inhalation of exhaled drug dose.

<center><img src="./images/Inhaler/Inhaler_Figure01.png" width="400"></center>
<center>
<i>Figure 1. A pressurized metered dose inhaler @cite Wikiphoto2016How.</i>
</center>
@anchor inhaler-intro
Introduction
------------

pMDIs are portable, user-actuated devices used to administer drug doses into the lungs and airways. The drug is either a liquid or a solid in liquid suspension, stored in a small pressurized cylinder with a propellant spray. When the device is actuated, a fixed amount of the pressurized suspension is released through a nozzle. As the suspension reaches atmospheric pressure at the nozzle exit, the nozzle geometry and rapid evaporation of the propellant causes the suspension to be aerosolized into droplets that are sufficiently small to remain in suspension in the air for several seconds. The properties of propellant and nozzle design, strongly dictate the efficiency with which the liquid suspension is aerosolized @cite newman2005principles .  

The user coordinates actuation of the device with a controlled inhalation, so that the aerosolized dose is inhaled deep into the lungs. Immediately prior to actuation, the user exhales as much as possible to their minimum lung volume (residual volume). As the device is actuated, the user begins a steady inhalation until they have reached their maximum inhalation volume (vital capacity).  At the end of the inhalation, the user holds their breath for up to 10 seconds and then exhales. In this way, the user maximizes the amount of inhaled dose that is deposited in the lungs. 

To help poor coordinators, the pMDI is often used in conjunction with a spacer. A spacer is simply a closed volume in front of the nozzle that is held between the pMDI and the mouth. When the pMDI is actuated, the aerosolized dose is initially contained within the spacer. If the user begins inhalation too late or breaths in too shallow, the spacer holds the aerosolized dose and extends the time over which the user can inhale and still maximize lung deposition.  
@anchor inhaler-system
System Design
=============

Background and Scope
--------------------

### Aerosol Deposition 

Most pMDI's deliver 100-200 &mu;g of a substance per actuation, but a large portion of the dose does not actually make it into the lungs. Many studies, including one by Newman @cite newman1991improvement in 1991, showed that as little as 20% of the drugs delivered by the pMDI actually make it into the lung tissue. The actual amount depends strongly on droplet size and the technique of the user. Users categorized as "good coordinators" average 20% lung deposition, but can often get much better depending on how "good" they are.  "Bad coordinators" - those that actuate the inhaler too early, too late, or fail to inhale sufficiently - do much worse. Table 1 summarizes the results from Newman's study.  

<center>
<i>Table 1. Mean percentage or aerosol dose located at various sites after inhalation by Good and Bad coordinators @cite newman1991improvement . </i>
</center>

| Site | Correct: "Good Coordinators" | Incorrect: "Bad Coordinators" |
| --- | --- | --- |
| Lungs | 18.6% | 7.2% |
| Oropharynx | 64.6% | 67.7% |
| Actuator | 16.1% | 23.5% |
| Exhaled | 0.7% | 1.7% |

Regardless of how coordinated the user is with the device, a large portion of the dose is generally deposited in the oropharynx, lost in the actuator and nozzle, or is exhaled. In %BioGears, the portion of aerosolized dose lost to deposition in oropharynx region is estimated using an empirical relation developed for a study conducted by Yeh in 1996 @cite yeh1996comparisons ,

\f[ODF = \frac{1}{{1 + {{\left( {\frac{{\rho {d^2}Q}}{{30000}}} \right)}^{ - 1.37}}}}\f]
<center>
*Equation 1.*
</center><br> 

In this relation, the oral deposition fraction (ODF) is a function of droplet density, <i>&rho;</i>(g/cm<SUP>3</SUP>), droplet diameter, d (&mu;m), and flow rate, Q (cm<SUP>3</SUP>/sec). Nozzle and actuator losses are generally design dependent and will vary depending on the specific pMDI used. In %BioGears, nozzle losses are treated as a property of the pMDI.

### Requirements
The inhaler implementation is used to meet the requirement to administer a beta agonist (e.g., albuterol) as an intervention action for acute asthma.

### Approach
In %BioGears, the pMDI is modeled as a simple circuit (Figure 2) conditionally appended to the existing %Respiratory System circuit model. The inhaler circuit consists of a single inhaler node with a fixed volume connected to the external environment &ldquo;ground.&rdquo; If a spacer is specified in the scenario, the volume of the spacer is added to the inhaler node volume. When the pMDI is actuated during a scenario, the inhaler circuit is connected to the mouth node of the respiratory model, replacing the connection from the mouth to the external environment. Atmospheric air initially fills the inhaler volume and airflow into and out of the respiratory system temporarily passes through the inhaler node. 

@image html Inhaler_Figure02.png
<center>
<i>Figure 2. %Inhaler circuit (red) connected to the %Respiratory System circuit. The inhaler circuit consists of a single inhaler node that is added the respiratory circuit when the pMDI is actuated.</i>
</center><br>

To model coordinated use of the pMDI, conscious breathing actions were implemented.  The end volumes and time lengths for exhalation, inhalation, and holding breath are specified as parameters for these actions, along with pMDI actuation timing.  When the pMDI is actuated, the drug dose is added to the atmospheric air in the inhaler node. Depending on airflow direction, the air/drug mixture in the inhaler node flows into the mouth or out into the external environment. A fraction of the drug that flows through the mouth and trachea is removed from the system per Equation 1. The remaining drug mass flows into and out of the lungs and alveoli. The drug mass in the inhaler node is assessed each time step during the scenario. When the drug mass in the inhaler node drops to approximately zero, the inhaler is disconnected from the respiratory circuit.

Once in the alveoli, the drug diffuses into the blood stream. See the @ref DrugsMethodology for PK and PD effects. 
@anchor inhaler-dataflow
Data Flow
---------

### Initialize
The volume of the inhaler node is filled with atmospheric air at the beginning of the scenario. 

### Preprocess
The system determines if and when an inhaler actuation command is initiated. 

When an actuation command is detected, the following occurs:
- The inhaler circuit is connected to the respiratory circuit.
- If specified in the scenario, the spacer volume is added to the inhaler node volume.
- The drug dose is added to the inhaler node volume and mixed with air. 
- The system begins tracking the mass of drug remaining in the inhaler node.

While the inhaler circuit remains connected to the respiratory circuit, the following occurs:
- A fraction of drug mass in the trachea is removed from the system each time step to model onopharynx deposition.
- If the drug mass in the inhaler node drops to near zero, the inhaler circuit is disconnected.

After the inhaler circuit is disconnected from the respiratory circuit, the following occurs:
- If specified in the scenario, the spacer volume is subtracted from the inhaler node volume.

Coordinated use of the pMDI is dependent on the conscious breathing actions executed by the %Respiratory System during the Preprocess step. Refer to the @ref RespiratoryMethodology for conscious breathing actions.

### Process

The current %BioGears implementation has no specific Process functionality for the inhaler. %Inhaler processing is currently done in the %Respiratory System with the combined circuit methodology.

### Post Process

The Post Process step moves values calculated in the Process step from the next time step calculation to the current time step calculation. The current %BioGears has no specific Post Process functionality for the inhaler. All postprocessing is done in the %Respiratory System with the combined circuit methodology.

The following figure presents the data flow of the %Inhaler data processing steps. 

<center><img src="./images/Inhaler/Inhaler_Figure03.png" width="550"></center>

<center>
<i>Figure 3. All primary inhaler activity occurs during the preprocessing step.</i>
</center>
@anchor inhaler-features
Features and Capabilities
-------------------------

### Features

#### Connecting to the %Respiratory Circuit
When the pMDI is used by a patient, there is a direct connection that allows air to flow freely between the inhaler and respiratory circuits.  Both individually defined circuits are combined into a single circuit that is then used for calculations.

#### pMDI (%Inhaler) Settings
<center>
*Table 2. The table shows the basic settings parameters used in BioGears as inputs to use the pMDI*
</center>

| Preset Parameter | Description |
| --- | --- |
| Substance | The name of the substance being administered.  The substance must be one of the gaseous substances handled by BioGears. |
| Metered Dose (ug) | Dose mass. |
| Nozzle Loss | Fraction of the dose that is lost in actuation nozzle. |
| Spacer Volume (mL) | Volume of the spacer used with the pMDI.  Optional. |

#### Important drug properties
Losses due to drug deposition in the oropharynx region are estimated based on the droplet diameter and density. The droplet density used by the model is the density of the suspension (liquid solution including the drug).  Typically, the density of these suspensions is close to that of water @cite yeh1996comparisons .  Droplet diameters typically range between 0.5 and 6.0 &mu;m. 
@anchor inhaler-dependencies
### Dependencies
The %BioGears pMDI interacts with the %Respiratory System through a connection that delivers atmospheric air and drugs into the %Respiratory System (@ref RespiratoryMethodology). The two systems are connected via a path joining the mouth node of the %Respiratory System to the inhaler node (see Figure 2). Before and after the pMDI is actuated, the mouth node of the %Respiratory System is connected to the atmosphere through the %Environment System that serves as a ground node for the %Respiratory System. 

When the pMDI is actuated, a network of combined circuits that include the elements from both the %Respiratory System and the inhaler is created. When the combined circuit is generated at the run-time, the ground environment node connected to the mouth node of the %Respiratory System is replaced by the inhaler node that represents the nozzle, becoming one combined circuit.

Coordinated use of the pMDI depends on the uses of the conscious breathing action/command set. Refer to the @ref RespiratoryMethodology for details.
@anchor inhaler-assumptions
Assumptions and Limitations
---------------------------
- The modeling approach used by %BioGears assumes a monodisperse aerosol (constant droplet diameter). pMDI&rsquo;s are known to generate a polydisperse aerosol. Moreover, evaporation decreases droplet diameter within a short timescale. Both of these effects are known to be significant but are not currently addressed in the %BioGears inhaler model. 
- The system transport of the administered drug is treated as a gas. As a result, the model does not address drug deposition and absorption through respiratory airway surfaces other than the alveoli. All diffusion into the bloodstream occurs as a result of gas transfer from the alveoli into the pulmonary capillaries. Other transport mechanisms, such as the dissolving of deposited mass into the mucous membranes or the ingestion of orally-deposited drug mass are not addressed. 
- The re-inhalation of exhaled drug mass is not addressed. Drug mass that is exhaled is presumed lost.
- Losses due to drug deposition on airway surfaces during exhalation are not addressed. Although losses during inhalation are handled, the flow geometry is more complex during exhalation and is more difficult to model. 
@anchor inhaler-actions
Actions
-------

### Actuation

pMDI actuation is part of the conscious breathing action/command set described in the @ref RespiratoryMethodology.  
@anchor inhaler-events
Events
------

### Multiple Actuations

If the inhaler is actuated too soon after an initial actuation, %BioGears triggers an event to alert the user.
@anchor inhaler-results
Results and Conclusions
=======================
@anchor inhaler-actionsvalidation
Validation - Actions
--------------------

pMDI actuation and coordinated breathing actions were validated in several scenarios. A summary of this validation is shown in Table 3. For each scenario, the table shows the total number of results in each category. For many investigated scenarios, the model shows good agreement with the expected trends. For the scenarios that did not match with the expected trends, improvements are planned for future %BioGears Engine releases.

<center><br>
*Table 3. Cumulative validation results for %Inhaler specific conditions and actions scenarios.*
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	Inhaler_OneActuation	|	Single actuation of pMDI with 90 ug dose of albuterol	|<span class="success">	12	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Inhaler_OneActuationWithSpacer	|	Single actuation of pMDI with 90 ug dose of albuterol using a 500 mL spacer	|<span class="success">	12	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Inhaler_TwoActuations	|	Two actuations of pMDI with 90 ug doses of albuterol (180 ug total)	|<span class="success">	24	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Inhaler_OneActuationIncorrectUse	|	Single actuation of pMDI with 90 ug dose of albuterol, 3 seconds before inhale	|<span class="success">	12	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Inhaler_OneActuationWithSpacerIncorrectUse	|	Single actuation of pMDI with 90 ug dose of albuterol using a 500 mL spacer, 3 seconds before inhale	|<span class="success">	12	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|		|	Total	|<span class="success">	72	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|

### Single Actuation with Correct Use

The single actuation scenarios with and without a spacer use the same setup and actions otherwise.  Therefore, they produce extremely similar results. Validation results for a single pMDI actuation using a metered dose of 90 &mu;g of albuterol with good coordination. Results agree very well with published data.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Inhaler/Inhaler_OneActuation_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Inhaler/Inhaler_OneActuation_AlbuterolConcentration.jpg" width="550"></td>
</tr>
</table>
</center>
<center><i>Figure 4. Select outputs from the single actuation scenario. With and without a spacer give extremely similar results.</i></center>

<center><br>
<i>Table 4. Validation matrix for physiological responses due to inhaler single actuation with correct use. With and without a spacer give the same results.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Albuterol Mass in Alveoli (ug)	|	Trachea Flow - Peak Flow  (L/min)	|	Total Lung Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Forced Exhale	|	Patient exhales: ERV Fraction = 1.0 (100%), Period = 3 sec	|	30	|	33	|<span class="success">	0.0 ug	</span>|<span class="success">	Negative Value	</span>|<span class="success">	Residual Volume (~1.2L)	</span>|
|	Inhaler Actuation & Forced Inhale	|	Patient activates inhaler at beginning of forced inhale: Substance = Albuterol, dose = 90 ug, Nozzle Loss = 0.04. Patient inhales: IC Fraction = 1.0 (100%), Period = 5 sec	|	33	|	38	|<span class="success">	Peak 47% to 65% of total dose @cite usmani2005regional (42.4 to 59.0 ug)	</span>|<span class="success">	Positive Value	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Hold Breath	|	Patient holds breath: Period = 10 sec	|	38	|	48	|<span class="success">	< Peak	</span>|<span class="success">	0 L/min	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Normal Breathing	|	Patient returns to normal breathing	|	48	|	180	|<span class="success">	> 0.0 ug	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|

### Single Actuation with Poor Coordination

These scenarios use the inhaler about as incorrectly as possible by activating it at the moment the exhale begins - thereby, blowing the albuterol out of the inhaler into the environment.

#### Without Spacer

The incorrect use, single actuation scenarios with and without a spacer use the same setup and actions otherwise.  Therefore, they produce similar results, except for the amount of alburterol that gets into the body. Validation results for a single pMDI actuation using a metered dose of 90 &mu;g of albuterol with poor coordination agree very well with published data.  Without the spacer present, when the user exhales, the entire dose is lost to the exterior environment. In reality, some of this dose is inhaled from the remaining aerosolized mass in the surrounding air. The inhalation of exhaled albuterol is not currently modeled.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Inhaler/Inhaler_OneActuationIncorrectUse_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Inhaler/Inhaler_OneActuationIncorrectUse_AlbuterolMass.jpg" width="550"></td>
</tr>
</table>
</center>
<center><i>Figure 5. Select outputs from the single actuation scenario with incorrect use and without the use of a spacer.</i></center>

#### With Spacer

When the spacer is included with the inhaler, a small amount of albuterol still gets into the body.  

<center>
<table border="0">
<tr>
    <td><img src="./plots/Inhaler/Inhaler_OneActuationWithSpacerIncorrectUse_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Inhaler/Inhaler_OneActuationWithSpacerIncorrectUse_AlbuterolMass.jpg" width="550"></td>
</tr>
</table>
</center>
<center><i>Figure 6. Select outputs from the single actuation scenario with incorrect use and with the use of a spacer.</i></center>

<center><br>
<i>Table 5. Validation matrix for physiological responses due to inhaler single actuation with incorrect use. The same matrix can be used to analyze both with and without the spacer.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Albuterol Mass in Alveoli (ug)	|	Trachea Flow - Peak Flow  (L/min)	|	Total Lung Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Forced Exhale	|	Patient exhales: ERV Fraction = 1.0 (100%), Period = 3 sec	|	30	|	33	|<span class="success">	0.0 ug	</span>|<span class="success">	Negative Value	</span>|<span class="success">	Residual Volume (~1.2L)	</span>|
|	Inhaler Actuation & Forced Inhale	|	Patient activates inhaler at beginning of forced inhale: Substance = Albuterol, dose = 90 ug, Nozzle Loss = 0.04. Patient inhales: IC Fraction = 1.0 (100%), Period = 5 sec	|	33	|	38	|<span class="success">	Peak 0% to 12% of total dose @cite newman1991improvement (0.0 to 10.8 ug)	</span>|<span class="success">	Positive Value	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Hold Breath	|	Patient holds breath: Period = 10 sec	|	38	|	48	|<span class="success">	< Peak	</span>|<span class="success">	0 L/min	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Normal Breathing	|	Patient returns to normal breathing	|	48	|	180	|<span class="success">	< Peak	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|


### Two Actuations

Below are validation results for two non-concurrent pMDI actuations using a metered dose of 90 &mu;g of albuterol per actuation (180 &mu;g total) with good coordination. Results agree very well with data. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/Inhaler/Inhaler_TwoActuations_TotalLungVolume.jpg" width="550"></td>
    <td><img src="./plots/Inhaler/Inhaler_TwoActuations_AlbuterolConcentration.jpg" width="550"></td>
</tr>
</table>
</center>
<center><i>Figure 7. Select outputs from the double actuation scenario with correct use.</i></center>

<center><br>
<i>Table 6. Validation matrix for physiological responses due to inhaler double actuation with correct use.</i>
</center>

|	Segment	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Albuterol Mass in Alveoli (ug)	|	Trachea Flow - Peak Flow  (L/min)	|	Total Lung Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Forced Exhale	|	Patient exhales: ERV Fraction = 1.0 (100%), Period = 3 sec	|	30	|	33	|<span class="success">	0.0 ug	</span>|<span class="success">	Negative Value	</span>|<span class="success">	Residual Volume (~1.2L)	</span>|
|	Inhaler Actuation & Forced Inhale	|	Patient activates inhaler at beginning of forced inhale: Substance = Albuterol, dose = 90 ug, Nozzle Loss = 0.04. Patient inhales: IC Fraction = 1.0 (100%), Period = 5 sec	|	33	|	38	|<span class="success">	Peak 47% to 65% of total dose @cite usmani2005regional (42.4 to 59.0 ug)	</span>|<span class="success">	Positive Value	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Hold Breath	|	Patient holds breath: Period = 10 sec	|	38	|	48	|<span class="success">	< Peak	</span>|<span class="success">	0 L/min	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Normal Breathing	|	Patient returns to normal breathing	|	48	|	85	|<span class="success">	> 0.0 ug	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|
|	Forced Exhale	|	Patient exhales: ERV Fraction = 1.0 (100%), Period = 3 sec	|	85	|	88	|<span class="success">	> 0.0 ug	</span>|<span class="success">	Negative Value	</span>|<span class="success">	Residual Volume (~1.2L)	</span>|
|	Inhaler Actuation & Forced Inhale	|	Patient activates inhaler at beginning of forced inhale: Substance = Albuterol, dose = 90 ug, Nozzle Loss = 0.04. Patient inhales: IC Fraction = 1.0 (100%), Period = 5 sec	|	88	|	93	|<span class="success">	Peak 47% to 65% of total dose @cite usmani2005regional (42.4 to 59.0 ug)	</span>|<span class="success">	Positive Value	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Hold Breath	|	Patient holds breath: Period = 10 sec	|	93	|	103	|<span class="success">	< Peak	</span>|<span class="success">	0 L/min	</span>|<span class="success">	Total Lung Capacity (~6.2L)	</span>|
|	Normal Breathing	|	Patient returns to normal breathing	|	103	|	235	|<span class="success">	> 0.0 ug	</span>|<span class="success">	Normal	</span>|<span class="success">	Normal	</span>|

@anchor inhaler-conclusions
Conclusions
-----------
The %BioGears inhaler implementation was developed to simulate the administration of inhaled drugs and support the administration of a beta-agonist to intervene in the case of acute asthma.  The results from the validation cases show the successful modeling of albuterol administration using a pMDI. The model also supports multiple use of a pMDI as shown in another validation case. 
@anchor inhaler-future
Future Work
===========

Coming Soon
-----------

There are no planned near term additions.

@anchor inhaler-improvements
Recommended Improvements
------------------------

- The system transport of inhaled drugs is currently treated as gaseous. As a result, the model does not address drug deposition and absorption through respiratory airway surfaces or through oral ingestion. Albuterol is actually a solid in a liquid suspension, but %BioGears treats it as a gas with assumed gas diffusion and transport properties. All diffusion into the bloodstream occurs as a result of gas transfer from the alveoli into the pulmonary capillaries. The development of a model that includes lung and airway particle deposition could improve realism and allow drug transport through other tissues (such as the bronchial tubes or intestine lining), which are known to be a significant transport mechanisms. 
- %BioGears currently assumes that aerosolized drugs are monodisperse. The development of a polydisperse model for %BioGears that includes droplet evaporation could improve realism. 
- In %BioGears, aerosolized drug that is is exhaled after initial inhalation is currently lost. The ability to model the re-inhalation of exhaled drug would improve the simulation of uncoordinated use of the inhaler. 
@anchor inhaler-appendices
Appendices
==========

Data Model Implementation
-------------------------

@ref InhalerTable "Inhaler"

Acronyms
--------

pMDI - Pressurized Metered Dose %Inhaler

Compartments
------------

- Mouthpiece