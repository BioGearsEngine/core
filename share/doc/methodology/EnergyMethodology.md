%Energy Methodology {#EnergyMethodology}
==========================

Overview
========

@anchor energy-abstract
Abstract
--------
The %BioGears&reg; %Energy system provides a methodology for both thermal and metabolic regulation within the body. Heat transfer is modeled by a body thermal [circuit](@ref CircuitMethodology) that is connected to the [environment](@ref EnvironmentMethodology) circuit. A heat source at the core of the body circuit represents the metabolic heat production. In addition to generating heat, the dynamic metabolic rate is also used to compute the metabolic production and consumption rates of gases and other substances in the body. This system has been [validated](@ref energy-results) both under resting and dynamic conditions, such as exercise and external environment changes.

Introduction
------------
The %Energy system is responsible for simulating metabolism and regulating internal temperature. Metabolism is simulated through the consumption of nutrients in the tissues. Glucose and fat are the nutrients most readily used as an energy source for the human body. Oxygen is used to metabolize these nutrients during aerobic metabolism. After entering the pulmonary capillaries, oxygen is transported via the circulatory system and diffuses into the tissue where it is used in the aerobic reaction. The resulting by-product, carbon dioxide, diffuses out of tissue, traveling through the circulation before exiting the body through the pulmonary capillaries @cite guyton2006medical. Glucose can also be used to produce energy under anaerobic conditions, and lactate (lactic acid) is the resulting by-product. The lactate enters the circulation and can be reconverted to glucose via gluconeogenesis in the liver and kidneys. During periods of prolonged starvation, the liver produces ketones. Acetoacetate is the predominant ketone synthesized. After it is produced, acetoacetate may be used as an energy source for the brain, myocardium, and muscle @cite gropper2013nutrition. 

The body's temperature is well-regulated. Core temperature regulation can be achieved by varying heat production (e.g. shivering) or heat exchange (e.g. sweating). In %BioGears, variations in the external environment or physical activity can lead to changes in the core temperature, triggering thermal feedback. Examples of thermal feedback during shifts in core temperature are sweating and shivering. Sweating is initiated if the core temperature is too high @cite herman2007physics. The increased evaporation of water from the surface of the skin leads to an increase in heat transferred out of the body. Changes in core temperature directly affect other systems. For example, a low core temperature causes blood flow to be shunted away from the skin, resulting in less heat transfer from the core to the skin @cite guyton2006medical. These feedback mechanisms are used to maintain thermal homeostasis in a variety of environmental conditions.

@anchor energy-system-design
System Design
=============

Background and Scope
--------------------

### Requirements
The %BioGears energy system is required to simulate the effects of exercise and elevated physical activity as well as react to temperature and pressure changes in the environment.

### Approach
The %BioGears %Energy system is a physical model of heat transfer combined with a collection of empirical equations for heat production and exchange obtained from literature. Additional equations are derived from stoichiometric relationships and empirical data found in the literature, particularly the governing equations for the metabolic [production and consumption](@ref tissue-metabolic-production) of substances. It uses a thermal circuit to simulate heat transfer through the body, with the body circuit connected to the [environment](@ref EnvironmentMethodology) circuit. The body thermal circuit consists of a core node, representing core temperature, and a skin node, which represents the lumped peripheral temperature. The body thermal circuit is shown in Figure 1.

<img src="./images/Energy/internalThermal.png" width="400">
<center>
*Figure 1. The body thermal circuit consists of two nodes and four paths. Two additional paths exist, connecting to the environment thermal circuit. The circuit is used to model the dynamic core and skin temperatures.*
</center><br>

The path elements consist of an internal heat flow source, a core heat capacitance, a skin heat capacitance, and a variable resistor from the core to skin. These four elements represent metabolic heat generation, the discretized heat capacity of the human body, and the convective heat transfer due to blood flow. The heat capacities of the core and skin are computed by proportioning the mass-averaged heat capacity found in literature based on the mass fraction of the skin @cite herman2007physics. The variable core to skin resistance is computed dynamically during simulation, and is inversely proportional to the skin blood flow. A decrease in blood flow leads to an increase in heat transfer resistance, and vice versa. When connected to the [environment](@ref EnvironmentMethodology) thermal circuit, the core and skin temperatures dynamically react to the environmental conditions (e.g. temperature, pressure, and humidity).

The [metabolic production and consumption](@ref tissue-metabolic-production) of the %Energy system is determined by the rate of change of substances (nutrients, ions, gases) in the tissue, relying on [advective](@ref CircuitMethodology) and [diffusive](@ref TissueMethodology) transport methodologies.

### Thermal Regulation
Thermal regulation in the %Energy system occurs through manipulation of the metabolic rate or through external losses (sweating). The thermal feedback mechanisms are a direct implementation of those discussed by Herman @cite herman2007physics. For high core temperature, a control equation specifies the sweat rate as a function of the difference between the current core temperature and the set-point (Equation 1). Sweat is removed from the body via a path connected between the skin and the environment (Figure 2).

<img src="./images/Energy/sweat.png" width="400">
<center>
*Figure 2. Sweat is removed from the body via a flow source path connected to ground. The flow source rate is computed using Equation 1.*
</center><br>

\f[ \dot{m}_{sweat} = \frac{h_{sweat} \left(T_{core} - T_{core,setpoint} \right)}{\rho_{sweat}} \f]
<center>
*Equation 1.*
</center><br>

For severe increases in core temperature (hyperthermia), the rate of chemical reactions can increase, leading to a subsequent rise in metabolic rate. This value was quantified from Pate et al. @cite pate2001thermal as an eleven percent increase in metabolic rate for every degree increase in core temperature. This is described from the power relationship between metabolic rate and core temperature, shown in Equation 2.

\f[ \dot{Q}^{n+1}_{metabolic} = \dot{Q}^{n}_{metabolic} * \left(1.11 \right)^{T_{core}-T_{core,high}} \f]
<center>
*Equation 2.*
</center><br>

For low core temperatures, the maximum metabolic rate achievable through shivering is defined from the summit metabolism. This value is given by Herman as:

\f[ \dot{Q}_{metabolic} = 21.0m^{0.75}_{b} \f]
<center>
*Equation 3.*
</center><br>

If the summit metabolism is not enough to adequately maintain internal temperature, then the metabolic rate will begin to decrease at a rate of four percent for every degree drop @cite mallet2002hypothermia. The power relationship in Equation 4 is used to describe this phenomenon.

\f[ \dot{Q}^{n+1}_{metabolic} = \dot{Q}^{n}_{metabolic} * \left(0.96 \right)^{T_{core,low}-T_{core}} \f]
<center>
*Equation 4.*
</center><br>

Under resting conditions, the metabolic rate is determined from the Harris-Benedict formula. This formula gives the metabolic requirements in kilo-calories per day as a function of weight, height, age, and sex @cite roza1984metabolic . This empirical formula is shown in Equation 5.

<center>
<table border="0">
<tr>
<td> Men: </td> <td> \f[ \dot{Q}_{metabolic} = 88.632 + 13.397*m_{b} + 4.799*H - 5.677*A \f] </td>
</tr>
<tr>
<td> Women: </td> <td> \f[ \dot{Q}_{metabolic} = 447.593 + 9.247*m_{b} + 3.098*H - 4.330*A \f] </td>
</tr>
</table>
</center>
<center>
*Equation 5.*
</center><br>

@anchor energy-data-flow
Data Flow
---------
The %Energy System determines its state at every timestep through a three-step process: Preprocess, Process, and Postprocess. In general, Preprocess determines the circuit element values based on feedback mechanisms and engine settings/actions. Process uses the generic circuit calculator to compute the entire state of the circuit and fill in all pertinent values. Postprocess is used to advance time.

### Preprocess
#### Calculate Metabolic Heat Generation
The metabolic rate is dictated by the current state of the core temperature. This function includes states of increased metabolic rate due to shivering or severe hyperthermia. Additionally, an extreme drop in core temperature leads to decreasing metabolic rate. If the core temperature does not meet any of the criteria for increases/decreases, the metabolic rate will be calculated as the basal metabolic rate.

#### Calculate Sweat Rate
The sweat rate is calculated from a core temperature control function. The mass lost due to sweating is accounted for, and a flow source from the skin extravascular to ground path is updated to ensure fluid loss.  Sodium, potassium, and chloride are removed from the body (specifically, the skin tissue extracellular compartment) as a function of the amount of fluid lost and each ion's respective average
concentration in sweat ([Na<sup>+</sup>]=51.0 mM, [Cl<sup>-</sup>]=40.0 mM, [K<sup>+</sup>]=6.0 mM @cite shirreffs1997whole, @cite maughan2004fluid). 

#### Update Heat Resistance 
The variable core to skin heat transfer resistance is updated according to the inverse of the skin blood flow. 

#### Exercise
The exercise function adds to the body&rsquo;s basal metabolic rate a value that is specified by the exercise action. The metabolic rate set-point is specified by the action but limited by the amount of energy available. The energy available depends on the body's nutrient stores and ability to supply oxygen to the muscle tissue, where all exercise work occurs.

### Process

#### Process Temperature Circuit
The generic circuit methodology developed for the %BioGears Engine is used to solve for the temperature and heat transfer rate at each node or path. For more details, see @ref CircuitMethodology.

#### Calculate Vital Signs
The core and skin temperatures are recorded in this function. In addition, the current metabolic state of the patient may trigger the following [events](@ref energy-events): hypothermia, hyperthermia, and metabolic acidosis/alkalosis. These events are only triggered if the current state falls within the criteria of the specific event.

### Postprocess
The Postprocess step moves everything calculated in Process from the next timestep calculation to the current timestep calculation. This allows all other systems access to the information when completing their preprocess analysis for the next timestep.

<br>
<img src="./images/Energy/EnergyDataFlow.png" width="900">
<center>
<i>Figure 3. The data flow for the %Energy System consists of Preprocess, Process, and Postprocess. Preprocess determines the circuit element values based on feedback mechanisms engine actions. Process uses the generic @ref CircuitMethodology to solve the temperature circuit for temperatures and heat transfer rates. Postprocess updates these quantities to the next time step and then begins advancing time, which causes the loop to repeat.</i>
</center><br>

@anchor energy-features
Features, Capabilities, and Dependencies
----------------------------------------
The %Energy system is connected with every other %BioGears system. %Energy directly affects the %Cardiovascular System through modification of vascular tone, heart rate, and heart contractility. This leads to modifications in the cardiac output and flow distribution in the cardiovascular circuit. In addition, there is indirect feedback through the modification of oxygen consumption and carbon dioxide production. Increased carbon dioxide production can lead to higher arterial carbon dioxide, which, in the case of severe hypercapnia, causes an %Endocrine response in the form of epinephrine and norepinephrine release. There is direct feedback on the %Respiratory system via modification of the ventilation frequency and driver pressure. The previously mentioned oxygen consumption and carbon dioxide production changes indirectly based on feedback to %Respiratory driver, which is dependent on the arterial carbon dioxide and arterial oxygen. There is a dependence on the %Gastrointestinal and %Renal systems since they are the methods by which nutrients enter the body and metabolic wastes are cleared from the body. The %Tissue System's freeing and storing of nutrients, such as hepatic and muscle glycogen, affects the total achievable metabolic rate requested in the %Energy System.

The %Environment circuit is directly connected to the energy&rsquo;s internal temperature circuit to form the total temperature circuit. Modifications on the metabolic rate will therefore have a direct feedback on the environment temperature circuit, while changes in the ambient environment temperature will have a direct feedback on the metabolic rate.

Additional dependence on the cardiovascular system comes from the extravascular to vascular connection. The cardiovascular circuit is connected to the extravascular circuit via singular paths from the tissue-equivalent vascular node to the tissue node. These singular paths are formed by a flow resistance and contain a compliance which stores the extravascular volume.

@anchor energy-variability
### Patient Variability
The basal metabolic rate is computed from the sex, height, weight, and age of the patient using the Harris-Benedict formula and will be directly affected by patient variability. Other indirect effects, such as fluid compositional changes due to sweating, may be observed. As with all of the %BioGears systems, the energy system is validated using the %BioGears Standard Male patient. A detailed discussion of patient variability in %BioGears is available in the @ref PatientMethodology report.

@anchor energy-assumptions
Assumptions and Limitations
---------------------------
The %Energy system assumes a bulk representation of some of its metabolic substances to avoid tracking all possible biochemical reactants and products. It assumes that all carbohydrates are stored in the tissue as glucose. A similar assumption is made for lipids. The bulk representation of all lipids is tripalmitin. It is not converted into its fatty acid constituents in the blood stream. This assumption is carried through to ketones. All ketones are assumed to be acetoacetate; beta-hydroxybutyrate and acetone are not modeled. Furthermore, when amino acids are considered from a metabolic standpoint, it can be assumed that they are all alanine. It is assumed that all of these substances move with the flow into and out of the extravascular space.

@anchor energy-actions
Actions
-------
@anchor energy-exercise
### Exercise
The exercise action is initiated by specifying the exercise intensity or the desired work rate for the patient to achieve. The exercise intensity is a number between 0 and 1 that defines the requested fraction of the body's maximal work rate, which if not specified in the patient file is calculated using the patient's age and gender @cite plowman2013exercise. For instance, the average adult male has a maximal work rate of approximately 1200 Watts @cite hall2011guyton; therefore, a user-specified exercise intensity of 0.5 would request a work rate of 600 Watts from the body. Table 1 shows the approximate exercise intensity for some common activities executed by an average adult male. The approximate mechanical power produced by an adult male is also included in the table.

<br><center>
*Table 1. The approximate exercise intensity for some common activities and the corresponding approximate mechanical power (desired work rate). Note that the values in the table correspond to a body with a 1200 Watt maximum work rate capability.*
</center>
|Activity |Exercise Intensity | Mechanical Power (W) |
|-------- |----------------- |--------------------- |
|All muscles in the body working all-out @cite hall2011guyton                                |1.0  |1200 |
|All-out cycling with resistance, sustainable for approx. 8 seconds @cite billaut2003maximal |0.8  |1000 |
|Rowing 6.1 m/s on an ergometer @cite concept2016concept                                     |0.54 |650  |
|Running at about 7.1 m/s (16 mph) @cite johnson2000exercise                                 |0.36 |430  |
|'Typical' Level Cycling @cite herman2007physics                                             |0.1  |120  |
|Jogging at about 2.2 m/s (5 mph) @cite johnson2000exercise                                  |0.06 |70   |
|Rest  |0.0 |0 |

The exercise capacity of the body is physiologically and psychologically limited @cite noakes2012fatigue. The amount of work produced by the exercise action in %BioGears is limited by the nutrients available for consumption; there is currently no psychological fatigue implemented in %BioGears. For more information about the consumption of nutrients and their relation to available energy, see the @ref TissueMethodology.

There are three output parameters associated with the exercise action: the achieved exercise level, the fatigue level, and the total work rate level.
- **Achieved exercise level**: This is the fraction of requested work that the body is able to achieve. For instance, if a work rate of 300 Watts is requested and the body is able to deliver 300 Watts, then the achieved exercise level will be 1.0. After some time, the body's energy levels will be depleted, and it will only be able to produce a fraction of the requested work rate, so the achieved exercise level will be less than 1.0.
- **Fatigue level**: This is the exhaustion level of the body. A fatigue level of 1.0 indicates that the body has no energy available to do work. At low work rates the fatigue level increases slowly, and the rate of fatigue increase is related to type of metabolism producing the energy (i.e. aerobic or anaerobic).
- **Total work rate level**: This is the fraction of the full work rate capacity of the body at which the body is currently working. For example, if the rested body is capable of producing a maximum of 1200 Watts of power for some amount of time, then that body working a rate of 600 Watts would have a work rate level of 0.5. This output is similar to the exercise intensity input.

@anchor energy-conditions
Conditions
----------

### Starvation
The starvation functionality simulates an extended time period without nutrient intake. This is accomplished by depleting nutrient storage and blood substance concentrations based on the duration of starvation. For relatively short (<5 days) periods of starvation, both the body's hepatic glycogen and temporary protein stores are depleted, and so these substances' starved values can be set based on long-duration %BioGears runs, in which the behaviors of substances are already validated. Similarly, stored fat, muscle glycogen, and muscle mass deplete at nearly linear rates, so these can also be interpolated based on long-duration %BioGears runs. Body weight and other patient parameters based on mass proportions are set based on these values. Blood concentrations of glucose and ketones don't deplete in a linear fashion, so those starvation values are set based on data from @cite zauner2000resting, @cite garber1974hepatic, @cite owen1971human, and @cite elia1984mineral. These blood concentrations use a second-order fit to data up to 3 days, after which the concentrations are set to a constant, lowered value.

### Dehydration
**The dehydration condition is disabled in the current release. An improved dehydration condition will be included with the next release.**
Dehydration functionality is simulated completely in the %Energy system. Similar to starvation, dehydration assumes a coarse timestep over which the fluid loss would occur. According to the Journal of Sports Medicine and Physical Fitness @cite shirreffs2000hydration , the average loss rate on the first day without liquid intake is 2600 milliliters. This rate decreases to 1600 milliliters for each subsequent day. The %Energy system uses these average rates and the time since last liquid consumption to calculate volume lost. The volume decrement is distributed via volume weighting instantaneously to all vascular and tissue compartments. 

### Hyperhidrosis and Hypohidrosis
**Excessive sweating and a lack of sweating are conditions that can be modified by the "Hyperhidrosis" parameter in the patient file. Using a scalar from -1 to 1, a more negative value will scale down the patient's sweat rate more in order to simulate hypohidrosis while a more positive value increases sweat rate to demonstrate hyperhidrosis @cite shih1983autonomic. The default value of zero refers to the standard sweat rate of a person, while a value of -1 can simulate the rare condition in which a person is unable to sweat at all.
<img src="./images/Energy/SweatRate.png" width="900">
<img src="./images/Energy/SkinTemp.png" width="900">
<img src="./images/Energy/CoreTemp.png" width="900">


@anchor energy-events
Events
------
### Hypothermia
Hypothermia is defined as a decrease in core temperature below 35 degrees Celsius @cite mallet2002hypothermia. In %BioGears, this is triggered due to changes in the external environment, leading to increased heat transfer off the skin surface.

### Hyperthermia
Hyperthermia is defined as an increase in core temperature above 38.8 degrees Celsius @cite mallet2002hypothermia . This may result from vigorous exercise, harsh environments, or infection. In %BioGears, this event is achievable through the exercise action or by specifying a high-temperature environment.

### Dehydration
Dehydration is classified by the World Health Organization as a three percent decrease in patient body mass due to fluid loss @cite who2005dehydration. This is tested by summing the total fluid mass on the cardiovascular and extravascular circuits at each timestep. Currently, this event can be triggered during a hemorrhage action or through prolonged exercise.

### Fatigue
The fatigue event is triggered whenever a tissue cannot provide the work it is asked to provide. Essentially, this means that a tissue either lacked oxygen or any available nutrients to fully meet its energy demand. The fatigue event is removed when there is no longer a deficit.

@anchor energy-results
Results and Conclusions
=======================

Validation - Resting Physiologic State
--------------------------------------
<br><center>
*Table 2. The %Energy system properties consist of core and skin temperature, as well as metabolic production rates. The properties show good agreement with the validation values.*
</center>
@insert doc/validation/EnergyValidationTable.md

The resting system properties for the energy system are temperatures and metabolic production rates. From Table 2, it is shown that the temperatures meet the validation criteria extremely well for resting conditions. Ketone production rate is currently low because only the brain uses ketones, and then only during starved states. Because ketone behavior is most important during starved states, it was tuned to perform at better long-term timescales, where this validation occurs only over a few minutes. Lactate production is low likely due to its clearance not being tied to gluconeogenesis. This will be addressed in an upcoming release. Resting state temperatures are good.

Validation - Actions and Conditions
--------------------
The %Energy and %Environment systems were validated for six scenarios encompassing a variety actions and environments. These include: exercise, cold water submersion (including active heating), carbon monoxide exposure, smoke inhalation, and a high altitude environment change. These scenarios were meant to test the dynamic feedback of the energy environment systems on both thermal regulation and internal physiology. There is good agreement with the expected results.

<center>
*Table 3. The dynamic response from the %Energy and %Environment systems were validated for six scenarios: Two Exercise scenarios, Cold Water Submersion, a carbon monoxide scenario, a forest fire scenario, and High Altitude. The scenarios demonstrate a good response to exercise and environmental conditions.*
</center>
|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	ColdWaterSubmersion	|	Patient is submerged in 10 C water for 1 hour. Removed for 10 minutes, and then actively heated for 20 minutes	|<span class="success">	17	</span>|<span class="warning">	2	</span>|<span class="danger">	5	</span>|
|	HighAltitude	|	Patient enters 4000 m elevation environment for 15 minutes	|<span class="success">	3	</span>|<span class="warning">	3	</span>|<span class="danger">	0	</span>|
|	ExerciseStages	|	Exercise for six minutes at increasing intensity with 2 minutes of rest in between stages.	|<span class="success">	78	</span>|<span class="warning">	12	</span>|<span class="danger">	22	</span>|
|	ExerciseVO2max	|	Exercise at high intensity (430 watts) to exhaustion.	|<span class="success">	1	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	ExerciseSustained	|	Exercise at moderate intensity to demonstrate sweat loss capability.	|<span class="success">	0	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	CarbonMonoxide	|	Varied exposure to environmental carbon monoxide.	|<span class="success">	20	</span>|<span class="warning">	4	</span>|<span class="danger">	0	</span>|
|	ForestFireFighter	|	Exposure to smoke and carbon monoxide.	|<span class="success">	5	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Starvation	|	Patient consumes no nutrients for 72 hours.	|<span class="success">	10	</span>|<span class="warning">	1	</span>|<span class="danger">	2	</span>|
|		|	Total	|<span class="success">	134	</span>|<span class="warning">	24	</span>|<span class="danger">	29	</span>|


@anchor energy-exercise-validation
### Exercise
There are four scenarios for validation of the exercise action. The first scenario (Tables 4a-d) takes advantage of the abundance of experiments using variations of the Bruce protocol to study exercise physiology. In this scenario, the exercise intensity is periodically increased, with short breaks in between each exercise period. The second scenario (Table 5) qualitatively validates that a work rate threshold exists. The scenario is based on the assertion by @cite johnson2000exercise that a body working around the threshold for maximal oxygen uptake (approximately 430 watts) will only be able to perform at the desired level for about three minutes before reaching physiological limits. 
In the third scenario (Table 6 and Figure 4), an individual exercises at a moderate to heavy intensity for an hour and the total fluid and ion loss from sweat is assessed. Lastly, Table 7 demonstrates the change in core and skin temperatures during exercise in specific environmental conditions.  The quantitative validation results are shown in the table below.

<center><br>
*Table 4a. The dynamic response to exercise. 15 measures are compared to data reported in literature at 8 different times during increasing exercise intensity with intermittent rest, for a total of 120 points of comparison. The first intensity/rest period is shown in Table 4a.*
</center>
|	Segment	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Core Temperature (Celsius)	|	Peripheral Temperature (Celsius)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Stroke Volume (mL)	|	Cardiac Output (L/min)	|	Gut Blood Flow (L/min)	|	Systemic Vascular Resistance (mmHg-min/L)	|	Respiration Rate (breaths/min)	|	Tidal Volume (L)	|	%Respiratory Exchange Ratio	|	Oxygen Consumption (L/min)	|	Urine Production Rate (mL/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.0375	|	45 watts	|	30	|	389	|<span class="success">	Increase @cite christie1987cardiac, @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="warning">	156% of baseline @cite christie1987cardiac	</span>|<span class="warningr">	Increase 18.4% @cite christie1987cardiac	</span>|<span class="warning">	No change @cite christie1987cardiac	</span>|<span class="success">	1% increase  @cite christie1987cardiac	</span>|<span class="danger">	41.6% increase  @cite christie1987cardiac	</span>|<span class="danger">	225% of base  @cite christie1987cardiac	</span>|<span class="danger">	reduced  @cite perko1998gutflow	</span>|<span class="success">	decrease  @cite perko1998gutflow	</span>|<span class="success">	Increase  @cite guenette2007respiratory	</span>|<span class="success">	increase  @cite guenette2007respiratory	</span>|<span class="danger">	increase, maximum  ~1.21  @cite koutlianos2013indirect	</span>|<span class="danger">	1.33  @cite christie1987cardiac	</span>|<span class="success">	decrease  @cite melin2001comparison	</span>|
|	Exercise Severity 0	|	rest 2 minutes	|	390	|	509	|<span class="success">	Increase @cite christie1987cardiac or no change @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	Decreasing 	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	No change @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|

<center><br>

*Table 4b. The second intensity/rest period.*
</center>
|	Segment	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Core Temperature (Celsius)	|	Peripheral Temperature (Celsius)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Stroke Volume (mL)	|	Cardiac Output (L/min)	|	Gut Blood Flow (L/min)	|	Systemic Vascular Resistance (mmHg-min/L)	|	Respiration Rate (breaths/min)	|	Tidal Volume (L)	|	Exchange Ratio (Or %Respiratory Quotient)	|	Oxygen Consumption (L/min)	|	Urine Production Rate (mL/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.075	|	90 watts	|	510	|	869	|<span class="success">	Increase @cite christie1987cardiac, @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="warning">	197% of baseline @cite christie1987cardiac	</span>|<span class="warning">	34.4% increase @cite christie1987cardiac	</span>|<span class="warning">	No change @cite christie1987cardiac	</span>|<span class="success">	8.38% increase  @cite christie1987cardiac	</span>|<span class="danger">	29.9% increase  @cite christie1987cardiac	</span>|<span class="danger">	267% of base  @cite christie1987cardiac	</span>|<span class="danger">	reduced  @cite perko1998gutflow	</span>|<span class="success">	decrease  @cite perko1998gutflow	</span>|<span class="success">	Increase  @cite guenette2007respiratory	</span>|<span class="success">	increase  @cite guenette2007respiratory	</span>|<span class="danger">	increase, maximum  ~1.21  @cite koutlianos2013indirect	</span>|<span class="danger">	2.05  @cite christie1987cardiac	</span>|<span class="success">	decrease  @cite melin2001comparison	</span>|
|	Exercise Severity 0	|	rest 2 minutes	|	870	|	989	|<span class="success">	Increase @cite christie1987cardiac or no change @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	Decreasing 	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	No change @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|

<center><br>

*Table 4c. The third intensity/rest period.*
</center>
|	Segment	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Core Temperature (Celsius)	|	Peripheral Temperature (Celsius)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Stroke Volume (mL)	|	Cardiac Output (L/min)	|	Gut Blood Flow (L/min)	|	Systemic Vascular Resistance (mmHg-min/L)	|	Respiration Rate (breaths/min)	|	Tidal Volume (L)	|	Exchange Ratio (Or %Respiratory Quotient)	|	Oxygen Consumption (L/min)	|	Urine Production Rate (mL/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.1583	|	190 W	|	990	|	1349	|<span class="success">	Increase @cite christie1987cardiac, @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	237% of baseline @cite christie1987cardiac	</span>|<span class="warning">	46.4% increase @cite christie1987cardiac	</span>|<span class="warning">	No change @cite christie1987cardiac	</span>|<span class="warning">	14.1% increase  @cite christie1987cardiac	</span>|<span class="danger">	36.4% increase  @cite christie1987cardiac	</span>|<span class="danger">	336% of base  @cite christie1987cardiac	</span>|<span class="danger">	reduced 25-50%  @cite perko1998gutflow	</span>|<span class="success">	decrease  @cite perko1998gutflow	</span>|<span class="success">	Increase  @cite guenette2007respiratory	</span>|<span class="success">	increase  @cite guenette2007respiratory	</span>|<span class="danger">	increase, maximum  ~1.21  @cite koutlianos2013indirect	</span>|<span class="danger">	2.75  @cite christie1987cardiac	</span>|<span class="success">	decrease  @cite melin2001comparison	</span>|
|	Exercise Severity 0	|	rest 2 minutes	|	1350	|	1469	|<span class="success">	Increase @cite christie1987cardiac or no change @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	Decreasing 	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	No change @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|

<center><br>

*Table 4d. The fourth intensity/rest period.*
</center>
|	Segment	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Core Temperature (Celsius)	|	Peripheral Temperature (Celsius)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Stroke Volume (mL)	|	Cardiac Output (L/min)	|	Gut Blood Flow (L/min)	|	Systemic Vascular Resistance (mmHg-min/L)	|	Respiration Rate (breaths/min)	|	Tidal Volume (L)	|	Exchange Ratio (Or %Respiratory Quotient)	|	Oxygen Consumption (L/min)	|	Urine Production Rate (mL/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.3583	|	430 W	|	1470	|	1829	|<span class="success">	Increase @cite christie1987cardiac, @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	257% of baseline @cite christie1987cardiac	</span>|<span class="warning">	56.8% increase @cite christie1987cardiac	</span>|<span class="warning">	No change @cite christie1987cardiac	</span>|<span class="warning">	117.9% increase  @cite christie1987cardiac	</span>|<span class="danger">	31.2% increase  @cite christie1987cardiac	</span>|<span class="danger">	350% of base  @cite christie1987cardiac	</span>|<span class="danger">	reduced 25-50%  @cite perko1998gutflow	</span>|<span class="success">	decrease  @cite perko1998gutflow	</span>|<span class="danger">	59 +/- 9  @cite guenette2007respiratory	</span>|<span class="danger">	3.1 +/- 0.4  @cite guenette2007respiratory	</span>|<span class="danger">	increase, maximum  ~1.21  @cite koutlianos2013indirect	</span>|<span class="danger">	3.36  @cite christie1987cardiac	</span>|<span class="success">	decrease  @cite melin2001comparison	</span>|
|	Exercise Severity 0	|	rest 2 minutes	|	1830	|	1950	|<span class="success">	Increase @cite christie1987cardiac or no change @cite byrne2007ingestible	</span>|<span class="success">	No change or decrease @cite griffin1993temperature	</span>|<span class="success">	Decreasing 	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	No change @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|<span class="success">	Toward baseline @cite christie1987cardiac	</span>|	No Data	|

<center><br>

*Table 5. The fatigue scenario validation results.*
</center>
|	Segment	|	Notes	|	Occurrence Time (s)	|	Duration of Full Exercise	|
|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.3583	|	Exercise intensity of 0.3583 is a requested work rate of 430 watts.	|	N/A	|<span class="success">	~ 3 min @cite johnson2000exercise	</span>|

<center><br>

*Table 6.  The effect of moderately hard exercise on body fluid and ion loss (See Figure 4).*
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Fluid Mass Lost	|
|	---	|	---	|	---	|	---	|	---	|
|	Exercise Severity 0.234	|	Working at about 65% of VO2 max	|	30	|	3630	|<span class="warning">	0.7-1.2 kg @cite baker2009comparison	</span>|

<center>
<table border="0">
<tr>
    <td><img src="./plots/Energy/SustainedExercise_PatientWeight.jpg" width="550"></td>
    <td><img src="./plots/Energy/SustainedExercise_Sodium.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Energy/SustainedExercise_Chloride.jpg" width="550"></td>
    <td><img src="./plots/Energy/SustainedExercise_Potassium.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 4.  After hard exercise for an hour, the patient loses about 0.5 kg of fluid (approximately 0.5 L).  The cumulative losses of sodium, chloride, and potassium are 650 mg, 930 mg, and 102 mg, respectively.  Note that the mass of each ion in the extracellular skin tissue compartment decreases as the body sweats.</i>
</center><br>

*Table 7.  The results for changes in the thermal circuit during exercise in the heat.*
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Skin Temperature	|	Core Temperature	|
|	---	|	---	|	---	|	---	|	---	|	---	|
|	Exercise in 37 degC	|	Working at about 50% of VO2 max	|	2400	|	4260	|<span class="warning">	35.7 degC @cite tsuji2011effect	</span>|	38.7 degC @cite tsuji2011effect	</span>|



There are several physiological measures that are failing validation for the exercise action. However, most of the failing measures will be addressed by improvements to the chemoreceptor model planned as a part of the %BioGears nervous system. The oxygen consumption and urine production measures may still fail validation during exercise after the nervous system is fully implemented. The development team is currently making plans to address those failures.





### Cold Water Submersion

<center>
<img src="./plots/Energy/ColdWaterSubmersion_TotalMetabolicRate.jpg" width="1100">
<img src="./plots/Energy/ColdWaterSubmersion_CoreTemp.jpg" width="1100">
<img src="./plots/Energy/ColdWaterSubmersion_SkinTemp.jpg" width="1100">
<img src="./plots/Energy/ColdWaterSubmersionLegend.jpg" width="1100">
</center>
<center>
*Figure 5. The cold water submersion begins by submerging the patient in water that has a temperature of 10 degrees Celsius. A direct result of this is a drop in skin temperature due to an increased heat loss from the skin surface. The patient remains in the water for one hour, during which the metabolic rate increases via shivering. This rise in metabolic rate allows for the core temperature to stabilize just above 35 degrees Celsius. After one hour, the patient is removed and then active heating begins ten minutes later. The result of active heating is an increase in core and skin temperature, and a decline in the patient&rsquo;s metabolic rate.*
</center><br>
<center>
*Table 7. The cold water submersion scenario results.*
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Core Temperature (Celsius)	|	Peripheral Temperature (Celsius)	|	Metabolic Rate	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Mean Arterial Pressure (mmHg)	|	Cardiac Output (L/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Environment File Change: Submerged in 5 C water	|	Patient is submerged in cold for 60 minutes	|	50	|	3650	|<span class="success">	Decrease to 35 degC @cite williams2005rewarming	</span>|<span class="success">	Decrease 	</span>|<span class="success">	Increase to between 300 and 350 W @cite williams2005rewarming	</span>|<span class="danger">	Decrease @cite garrioch2004body	</span>|<span class="success">	No change @cite christie1987cardiac	</span>|<span class="success">	If mild hypothermia (Core Temp 32-35 C) Increase @cite mallet2002hypothermia	</span>|<span class="success">	If mild hypothermia (Core Temp 32-35 C) Increase @cite mallet2002hypothermia	</span>|<span class="success">	If mild hypothermia (Core Temp 32-35 C) Increase If moderate or severe (Core Temp < 32 C) Decrease @cite mallet2002hypothermia	</span>|
|	Environment File Change to Standard Environment	|		|	3650	|	4250	|<span class="warning">	Decrease below 35 @cite williams2005rewarming	</span>|<span class="success">	Increase 	</span>|<span class="warning">	Increase above 350 W @cite williams2005rewarming	</span>|<span class="danger">	Decrease @cite talebipour2006sauna	</span>|<span class="danger">	Decrease @cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|
|	Active Heating at 1000 btu/hr	|		|	480	|	5450	|<span class="success">	Increase to between 36 and 38 degC @cite williams2005rewarming	</span>|<span class="success">	Increase 	</span>|<span class="success">	Decreasing to below 80 W @cite williams2005rewarming	</span>|<span class="danger">	Decrease @cite talebipour2006sauna	</span>|<span class="danger">	Decrease@cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|<span class="success">	Decrease @cite talebipour2006sauna	</span>|

At a scenario time of 50 seconds, the patient is submerged in water with a temperature of 10 degrees Celsius. The patient remains submerged for one hour, and then is removed from the water. The patient remains in the ambient environment for 10 minutes before active heating is applied. The power output of the heating device is 340 btu/hr. Active heating continues for 20 minutes, and then the scenario ends. The immediate response to cold water submersion is a drop in skin temperature. This occurs due to a dramatic increase in the skin convective heat transfer due to a shift from air convection to water convection. The core temperature declines due to increased heat transfer from core to skin. The drop in core temperature initiates a shivering response, leading to a rise in metabolic rate. Additional feedback comes in the form of vasoconstriction of the skin blood vessels. This helps to reduce the heat transferred from the core to skin. These two forms of feedback allow the core temperature to stabilize around 35 degrees Celsius. There is an increase in heart rate and respiration rate, initially due to a nervous response to the exposure to cold water. The increase in respiration rate and heart rate are maintained to accommodate the increased oxygen consumption and carbon dioxide production at the elevated metabolic rate. Following the return to the ambient environment, there is an immediate rise in skin temperature, with the core temperature following. The core temperature rise is not expected during this time period according to validation data. A continued decline in core temperature is expected. The observed increase in core temperature is due to a low heat capacitance used in the model and not accounting for residual water remaining on the skin surface after being removed from the water. The capacitance can be adjusted to a higher value, which would lead to the core temperature responding more slowly to changes in heat transfer over time. In addition, the %Environment heat transfer methodology could be updated to account for additional heat transfer due to water remaining on the skin surface after exiting submersion. At the onset of active heating, the skin temperature and core temperature continue to rise. This coincides with the expected validation outcome. 

### High Altitude Change
<table border="0">
<tr>
    <td><img src="./plots/Energy/HighAltitude_HR.jpg" width="550"></td>
    <td><img src="./plots/Energy/HighAltitude_O2PP.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Energy/HighAltitude_O2Sat.jpg" width="550"></td>
    <td><img src="./plots/Energy/HighAltitude_RR.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Energy/HighAltitudeLegend.jpg" width="1100"></td>
</tr>
</table>
<center>
*Figure 6. The patient is placed at a high altitude with an atmospheric pressure of 525 mmHg. The immediate result is a drop in arterial oxygen due to the decreased environment oxygen partial pressure. This leads to a drop in oxygen saturation and an increase in the patient heart rate. The patient stabilizes, acquiring a new resting physiologic state at the reduced pressure. The exaggerated increase in heart rate is due to a catecholamine release secondary to  hypoxia, a [known issue](@ref known-issues) in %BioGears*
</center><br>

</center><br>
<center>
*Table 8. The high altitude change scenario results.*
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Heart Rate (beats/min)	|	Oxygen Saturation (%)	|	Respiration Rate (breaths/min)	|	Systolic Blood Pressure	|	Cardiac Output (mL/min)	|	Myocardium Flow (mL/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	%Environment File Change to an altitude of 3000 meters	|	Atmospheric pressure at 3000m is approximately 70 kPa	|	50 s	|	950	|<span class="success">	7% increase @cite zhang2014highaltitude	</span>|<span class="success">	7% decrease @cite zhang2014highaltitude	|<span class="warning">	5% increase @cite zhang2014highaltitude	</span>|<span class="warning">	Decrease @cite bartsch2007highaltitude	</span>|<span class="warning">	No change @cite bartsch2007highaltitude	</span>|<span class="success">	Increase @cite bartsch2007highaltitude	</span>|


This scenario is used to simulate the effects of low oxygen due to high altitude. At 30 seconds, the surrounding environment is changed to an equivalent elevation of 4000 meters, resulting in a drop in atmospheric pressure from 760 mmHg to 525 mmHg. The patient remains in the reduced pressure environment for 15 minutes. The immediate response is a decrease in arterial oxygen due to the reduced partial pressure in the environment. This leads to a decrease in the blood oxygen saturation, which is in line with the expected validation data. The reduced arterial oxygen causes an increase in the heart rate. This increase is currently determined from epinephrine release due to a hypoxia event being triggered. Another expected result of the epinephrine release is a respiration rate increase. This effect is nullified from the reduction in atmospheric carbon dioxide, which allows for larger quantities of carbon dioxide to diffuse out of the alveoli. The increased diffusion leads to lower arterial carbon dioxide partial pressures, which is used to drive the respiratory frequency. This counter balances the effect of epinephrine in the system, thus causing a slight decrease in respiration rate.

### Starvation
The starvation condition is validated at 72 hours, which allows for direct comparison of blood concentration values in @cite zauner2000starvation, @cite garber1974hepatic, @cite owen1971human, and @cite elia1984mineral. Since the glucose and ketone concentrations for the starvation condition are directly set based on these sources, it is no surprise that they meet validation. More satisfaction is gained from the passing values for insulin, respiratory quotient, gluconeogenesis rate, ketogenesis rate, and urea, since those values are not directly set by the starvation condition, but are instead generated by other metabolic processes in %BioGears. These lend credibility to the metabolic models. The metrics that did not pass are body weight and plasma ion concentrations, both of which have outstanding tasks for future %BioGears improvements.

<center>
*Table 9. The starvation condition scenario results.*
</center>
|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Aorta Glucose Concentration (mg/dL)	|	Vena Cava Insulin Concentration (ug/L)	|	Aorta Ketones Concentration (mg/dL)	|	%Respiratory Quotient	|	Body Weight (kg)	|	%Hepatic Gluconeogenesis Rate (g/day)	|	Ketogenesis Rate (g/day)	|	Aorta Triacylglycerol Concentration (mg/dL)	|	Aorta Urea Concentration (mg/dL)	|	Sodium Plasma Concentration (mg/dL)	|	Potassium Plasma Concentration (mg/dL)	|	Calcium Plasma Concentration (mg/dL)	|	Chloride Plasma Concentration (mg/dL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	72 hour starvation condition applied	|	For ketones, acetoacetate and beta hydroxybutyrate values were summed, when available.	|	60	|<span class="success">	64.86 @cite zauner2000resting 61.25 @cite garber1974hepatic 66.47 @cite owen1971human 61.43 @cite elia1984mineral	</span>|<span class="success">	.337 @cite zauner2000resting .129 @cite garber1974hepatic .637 @cite owen1971human	</span>|<span class="success">	43.58 (beta hydroxybutyrate only) @cite zauner2000resting 25.52 @cite garber1974hepatic 31.26 @cite owen1971human 43.49 @elia1984mineral	</span>|<span class="success">	.72 @cite zauner2000resting	</span>|<span class="danger">	2.5% Decrease @cite zauner2000resting	</span>|<span class="success">	[99,124] @cite garber1974hepatic 165 @cite rothman1991quantitation	</span>|<span class="success">	[83,115] @cite garber1974hepatic	</span>|<span class="success">	75.89 @cite zauner2000resting	</span>|<span class="success">	24.62 @cite elia1984mineral	</span>|<span class="success">	311.28 @cite elia1984mineral	</span>|<span class="danger">	15.48  @cite elia1984mineral	</span>|<span class="success">	9.7 @cite elia1984mineral	</span>|<span class="warning">	354.5 @cite elia1984mineral	</span>|


### Dehydration

Conclusions
-----------
The %BioGears %Energy system has provided a method for handling metabolic consumption and production and heat transfer in the human body. This system is fundamentally connected to all of the other systems, and provides dynamic feedback according to changes in the external environment. In its current state, the energy system accurately calculates resting thermal physiology and nutrient consumption and production. The energy system can be used by developers who wish to model thermal changes due to the environment or through strenuous activity. This may be a simple simulation, or connection with a mannequin for real-time interfacing.

@anchor energy-future
Future Work
===========

Coming Soon
-----------
- The maximum work rate will scale with physical conditioning, fed/starvation levels, and body composition.
- The nervous system will contain numerous chemoreceptor-related feedback mechanisms that will correct many of the exercise validation failures such as heart rate and blood pressure.
- The response of body core temperature to exercise will be improved.
- Sweat composition will include more than just water.

Recommended Improvements
------------------------
- Increased discretization of the temperature circuit: This would allow for increased fidelity of the temperature solution and prediction of varying levels of water submersion.
- Include psychological effects. For example, incorporate psychology into fatigue using a 'mettle' parameter.
- Switch exercise accounting from physical work rate to metabolic heat production.
- A true basal metabolic rate with differentiation from resting metabolic rate.

@anchor energy-appendices
Appendices
==========

Acronyms
--------
RQ - %Respiratory Quotient
BMR - Basal Metabolic Rate

Data Model Implementation
-------------------------
@ref EnergySystemTable "Energy"

@anchor energy-compartments
Compartments
------------
- InternalCore
- InternalSkin
- InternalGround
