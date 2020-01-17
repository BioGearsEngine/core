%Gastrointestinal Methodology {#GastrointestinalMethodology}
===========================

@anchor GI-overview
Overview
========

Abstract
--------

The %BioGears %Gastrointestinal System models the ingestion of macro-nutrients and their subsequent digestion and transportation into the %Cardiovascular system. 
Digestion rates for each macro-nutrient are calculated by their interaction with the corresponding stomach enzyme. Although many enzymes participate in this digestion process, %BioGears assumes lipase, amylase, and trypsin are the enzymes facilitating digestion of fat, carbohydrates, and proteins. Each macro-nutrient, after reacting with its digestive enzyme, is modeled in %BioGears as only one substance. Fat is modeled as triacylglycerol (tripalmitin), carbohydrate is modeled as glucose, and protein is modeled by a lumped amino acids substance that represents alanine. We currently do no support the various other products produced during the digestion and absorption cycle within the GI system. Once digestion takes place, the product is moved into the small intestine chyme compartment, where it is absorbed into the vasculature through the intestinal walls via the major sodium co-transporters.


@anchor GI-introduction
Introduction
------------

### %Gastrointestinal Physiology

The %Gastrointestinal tract is responsible for consuming and digesting food, absorbing nutrients, and expelling waste.
The tract consists of the stomach and intestines breaking down food into usable nutrients for the body. 
After being eaten, food is stored in the stomach, gradually digested, then released into the gut chyme. 
At this point, nutrients are either quickly absorbed into the blood through the intestinal wall or further broken down prior to absorption. 
An overview of the digestive tract is shown in Figure 1.

@image html DigestiveTractDetail.png
<center>
<i>
Figure 1. The human digestive tract is composed of several distinct sections @cite LadyofHats2006Digestive. 
Currently, the %BioGears gastrointestinal model replicates the behavior of the stomach and small intestine.
</i>
</center><br>

@anchor GI-system
System Design
=============

@anchor GI-background
Background and Scope
--------------------
The %Gastorintestinal System was created so that end users could supply and replenish water and substances in the body to counterbalance losses associated with exercise and clearance.
It provides a low-fidelity enzyme kinetic model coupled with gastrointestinal secretions to supply  the major sodium co-transporter SGLUT1. Sodium absorption is directly coupled to movement of amino acids and glucose into the vasculature. Fat is handled separately with direct absorption of triacylglycerol into the blood stream, with future work targeting an expanded lymph system to facilitate more accurate fat absorption modeling and transport. Other major ions and water have constant digestion and absorption rates. Future work to properly model the major ion transport proteins will help enhance this current implementation.

@anchor GI-dataflow
Data Flow
---------

### Reset

The stomach is initialized with configurable amounts of each macronutrient, sodium, calcium, and water.

### Preprocess

#### Gastric Secretion

A static gastric secretion flow rate of 2.46 mL/min is used to manually move water volume from the gut tissue into the stomach.

#### Digestion

Digestion of nutrients in %BioGears is modeled as an enzymatic process that aims to replicate product formation in the human body as nutrients travel through the gastrointestinal system. The enzyme-substrate interaction and product formation is modeled using Michaelis-Menten kinetics describing the following process: 

\f[\left[ E \right] + \left[ S \right] \mathbin{\lower.3ex\hbox{$\buildrel\textstyle\rightarrow\over
{\smash{\leftarrow}}$}} \left[ {ES} \right] \to \left[ E \right] + \left[ P \right].\f]
<center>
*Equation 1*
</center><br>


We model this rate formation of product as a non-linear function of substrate concentration: 

\f[\frac{{dp}}{{dt}} = \frac{{{V_{\max }}\left[ S \right]}}{{{K_m} + \left[ S \right]}}.\f]
<center>
*Equation 2*
</center><br>

Then multiply by our timestep to compute total mass of product moved into the intestinal chyme:

\f[\Delta P = \left( {\frac{{{V_{\max }}\left[ S \right]}}{{{K_m} + \left[ S \right]}}} \right)\Delta t\f]
<center>
*Equation 3*
</center><br>

Where *&Delta;P* is in units of mass and is computed each time step given the current substrate concentration on the stomach. Rate constants were taken from experimental papers, often times from in vitro studies, and can be seen in table 1. Even though these may not be perfect numbers, the mouth to cecum transit times line up well with experimental data.

<br><center>
<i>Table 1. Michaelis–Menten kinetic parameters for nutrient digestion model. Parameters for this model are taken from @cite botham1997lipolysis @cite gangadharan2009biochemical @cite ohta1986purification</i>
</center>

| Nutrient | Enzyme | Product | Vmax (mg/min) | Km (mg/mL) | 
| :---- | :---- | :---- | :---- | :---- | 
| Fat | Lipase | Triacylglycerol | 3.40 | 0.34 | 
| Carbohydrate | Amylase | Glucose | 7.11 | 3.076 | 
| Protein | Trypsin | Amino Acids | 7.1 | 0.71 | 


 
#### Chyme Secretion
The human body uses gastric sections to produce bile, which has the enzymes and pH level necessary to digest the nutrients eaten into absorbable substances that fuel metabolism. This process of secretions is generally performed by the pancreas and is a function of composition and quantity of the food eaten. %BioGears, during digestion, assumes full availability of these digestive enzymes. Substances are then secreted into the small intestine. Bile salts are critical in digestion and absorption of nutrients into the bloodstream and are needed to facilitate our model of absorption so %BioGears mimics this functionality seen in the body.

We model this functionality similarly to an enzyme kinetics model by using a non-linear function of stomach mass to compute sodium secreted. %BioGears models sodium mass transfer as a Hill-type non-linear function of total digested product in the small intestine chyme: 

\f[\Delta N{a_{\sec }} = \left( {\frac{{{V_{\max }}{N_{mass}}}}{{{K_m} + {N_{mass}}}}} \right)\Delta t\f]
<br><center>
*Equation 4 sodium secretion function*
</center>

Where *N<sub>mass</sub>* is the maximum total mass of a nutrient currently in the stomach. Sodium is currently the only supported ion to be included in this process, but the model could be extended to include hydrogen and bicarbonate in order to validate pH levels of the chyme.

#### Absorption
Absorption of digested nutrients is facilitated through sodium uptake. In a similar manner to secretion absorption is modeled as a non-linear function of product mass in the chyme, as in equation 4. Once absorbed sodium mass is computed, co-transport of glucose and amino acids begins. We assume that 1 mg of sodium can transport 2 mg of glucose or 1 mg of amino acids in our simplified model. This allows for total transit times that are in line with validated data @cite ladas1989reproducible

A static absorption flow rate of 3.3 mL/min is used to manually move water volume from the gut chyme into small intestine vasculature.

### Conditions

#### ConsumeMeal

The ConsumeMeal condition provides a nutrition object along with an elapsed time since those nutrients were in the stomach.
The GI System will then zero out the contents of the stomach and gut chyme and add the nutrient data provided in the condition to the stomach.
The digestion algorithm is executed for the provided elapsed time, removing nutrient mass from the stomach and adding it to substance quantities in the gut chyme.
Next, the absorption algorithm will be executed for the provided elapsed time, removing substance mass from the gut chyme and distributing that mass throughout the blood and tissues.

### Actions

#### ConsumeNutrients
Each nutrient is optional in the ConsumeNutrients action. If a nutrient mass is provided with no accompanying digestion rate, the engine will default to the rate specified in the %BioGears configuration. 
If a digestion rate is provided for a macronutrient, it will be combined with the current stomach digestion rate for that macronutrient by volume weighting the two rates together.

### Process
There is no system specific function for Process in the %Gastrointestinal System.

### Post Process
There is no system specific function for Post Process in the %Gastrointestinal System.

### Assessments
There are no system specific assessments in the %Gastrointestinal System.

<br>
<img src="./images/GI/GastrointestinalDataFlow.png" width="600">
<center>
*Figure 2. The data flow for the %Gastrointestinal System consists of a Reset, Conditions, Preprocess, Process, Post Process, and Assessments. 
Only the Reset, Conditions, and Preprocess have system-specific functionality.*
</center><br>

@anchor GI-features
Features, Capabilities, and Dependencies 
----------------------
### Circuit

@anchor GI-circuit
@image html GICircuit.png
<center>
<i>
Figure 3. The %GI circuit is made up of nodes and paths with elements and is connected to extravascular tissue and the cardiovascular system.
</i>
</center><br>

The constant secretion of fluid into the digestive tract allows for macronutrients to be slowly digested and eventually absorbed through the intestinal wall @cite hall2011guyton. In %BioGears, these macronutrients are placed into the CV system via an absorption rate that mirrors that of the human GI tract. The calculated flow rate transports substance from the small intestine C1 node to the CV system's small intestine 1 node @ref GI-circuit. Similar transport connects the tissue and the small intestine and determines water balance and secretion rate.
The digestion rate of macronutrients is heavily dependent on the nature of the source food, as the digestive tract needs to free substances from the rest of the food's molecular structure before it can be absorbed @cite wolever1991glycemic.
Fats spend several hours being emulsified by the small intestine and are then quickly absorbed once exposed to pancreatic enzymes @cite hall2011guyton. 
Many of the small ions are quickly absorbed with active transport in a manner similar to the %Renal system @cite hall2011guyton.
An exception to this is calcium, which is carefully regulated by parathyroid hormone to ensure that the body's requirements are met @cite hall2011guyton.
Water is absorbed via the osmotic gradient that either naturally exists between the gut chyme and the blood or that is created by the active transport of substances across the intestinal wall @cite hall2011guyton.


@anchor GI-dependencies
Dependencies
------------
The %Gastrointestinal System is not dependent on any other system, but other systems are dependent on GI to provided and replenish substances that they use.

@anchor GI-assumptions
Assumptions and Limitations
---------------------------

Currently there is no modeling of defecation or the lower portion of the digestive tract. This means that all food that is ingested will eventually be absorbed.
The current digestion methodology is rudimentary and assumes all digestive rates are constant and move with the water flow after leaving the stomach. Active transport is used across cell walls with no diffusion currently implemented. Contraction of the small intestines to allow for transport and defecation is also not modeled.

@anchor GI-results
Results and Conclusions
=======================
Validation - Resting Physiologic State
--------------------------------------

The %BioGears %Gastrointestinal System was validated qualitatively by observing the relative changes in the substance and nutrient masses between the stomach contents
and the gut contents. The transport to the CV system was then shown by demonstrating an increase in substance concentration in the blood.
Concentrations are not solely a function of GI; other systems are potentially using substances in the blood.

<center>
*Table 2. Macronutrient mass in the stomach, gut, and blood as a function of time.*
</center>

|  Macronutrient/Substance |                           Stomach Mass(g) vs. Time(s)                            |                               Gut Mass(g) vs. Time(s)                                  |                     Blood Concentration (ug/mL) vs. Time (s)                         |
|:---:                     |:---:                                                                             |:---:                                                                                   |:---:                                                                                 |
| Carbohydrate/Glucose     | <img src="./plots/GI/CarbMeal_StomachCarbs.jpg" height="100" width="200"> | <img src="./plots/GI/CarbMeal_IntestineGlucose.jpg" height="100" width="200">    | <img src="./plots/GI/CarbMeal_BloodGlucose.jpg" height="100" width="200">    |
| Fat/Triacylglycerol           | <img src="./plots/GI/FatMeal_StomachFat.jpg" height="100" width="200">          | <img src="./plots/GI/FatMeal_IntestineTriacylglycerol.jpg" height="100" width="200"> | <img src="./plots/GI/FatMeal_BloodTriacylglycerol.jpg" height="100" width="200"> |
| Protein/AminoAcids             | <img src="./plots/GI/ProteinMeal_StomachProtein.jpg" height="100" width="200">      | <img src="./plots/GI/12hr_IntestineUrea.jpg" height="100" width="200">       | <img src="./plots/GI/12hr_BloodUrea.jpg" height="100" width="200">       |
| Calcium                  | <img src="./plots/GI/12hr_StomachCalcium.jpg" height="100" width="200">      | <img src="./plots/GI/CarbMeal_IntestineCalcium.jpg" height="100" width="200">    | <img src="./plots/GI/12hr_BloodCalcium.jpg" height="100" width="200">    |
| Sodium                   | <img src="./plots/GI/12hr_StomachSodium.jpg" height="100" width="200">       | <img src="./plots/GI/12hr_IntestineSodium.jpg" height="100" width="200">     | <img src="./plots/GI/12hr_BloodSodium.jpg" height="100" width="200">     |
| Water                    | <img src="./plots/GI/12hr_StomachWater.jpg" height="100" width="200">        | <img src="./plots/GI/12hr_IntestineVolume.jpg" height="100" width="200">          | <img src="./plots/GI/CarbMeal_BV.jpg" height="100" width="200">             |


As seen in Table 2, the macronutrient masses in the stomach are depleted based on different digestion rates and eventually reach zero. 
The center column of plots shows the associated mass of substances in the gut chyme increase as the nutrients are removed from the stomach.
Concentrations are provided in the far right column demonstrating the effect of the increasing mass on the blood concentrations.
These overall trends meet the expectations of the model performance.

Validation - Actions
--------------------
### Consume Meal Standard Validation 
Mouth to cecum transit times are validated against experimental data @cite ladas1989reproducible. Total transit is on the slow end of reported ranges of patients ingesting a standard meal but still within the deviation found in the study. 

<center>
*Table 3. Validated transit times are in line with experimental data @cite ladas1989reproducible @cite seimon2013gastric.*
</center>

|	Actions	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (hr)	|	Gastric emptying time (carbs) (min)	|	Mouth-to-Cecum Transit Time (min)	|	Blood Glucose Concentration return time (min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	---	|	Give a meal to the patient: 26g carbohydrate, 6g protein, 30g fat	|	30s	|	4hr	|<span class="success">	40-90 min @cite ladas1989reproducible	</span>|<span class="success">	64-144 min @cite ladas1989reproducible	</span>|<span class="success">	120-180 min @cite seimon2013gastric	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/GI/StandardMeal_IntestineSodium.jpg" width="550"></td>
    <td><img src="./plots/GI/StandardMeal_IntestineGlucose.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>
Figure 4. The plots show sodium secretion in the chyme as a response to digested glucose load after a standard meal. The secretion and absorption functionality work together to absorb the nutrients into the body at the proper time scales. 
</i>

</center><br>

<center>
<table border="0">
<tr>
    <td><img src="./plots/GI/StandardMeal_StomachCarb.jpg" width="550"></td>
    <td><img src="./plots/GI/StandardMeal_BloodGlucose.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>
Figure 5. Times taken for digestion to take place to affect blood glucose levels are seen here. The internal nutrient kinetics models use glucose as it enters the bloodstream or stores excess as glycogen. Times scales are at values seen in experimental data. 
</i>
</center><br>

@anchor GI-conclusions
Conclusions
-----------

The %BioGears %Gastrointestinal System lays a foundation for future efforts in modeling digestive physiology. The flexible definition of food allows for easy extensibility
of the system by creating the ability for users to add to a library of meals. 

@anchor GI-futurework
Future Work
===========

Coming Soon
-----------
The osmolarity of the digested material is not currently measured. Implementing this would allow for the creation of a more realistic osmolar gradient to determine the water absorption rate and gastric secretion. Permeability of the tissue matrix and substance concentration levels will be added to allow for actual diffusion of substances and fluid instead of active transport being implemented currently.

@anchor GI-recommended
Recommended Improvements
------------------------

- More %Substances: By expanding the list of %substances %BioGears works with, the %Gastrointestinal model will inherently gain fidelity as mechanisms
	are constructed to properly handle new inputs.

- Defecation: The %Gastrointestinal system should eventually be able to handle undigestible material and the fluid loss due to defecation. This
	will also allow for a variety of disease states to be implemented.
	
- Vomiting: This can be added either as a symptom or an intervention to aid someone who has ingested a harmful substance.

@anchor GI-appendices
Appendices
==========

Acronyms
--------

GI - %Gastrointestinal

CV - %Cardiovascular


Data Model Implementation
-------------------------

@ref CardiovascularSystemTable "Cardiovascular"

@ref GastrointestinalSystemTable "Gastrointestinal"

Compartments
------------

The GI system is integrated into the %cardiovascular system, please refer to the compartement description associated with @ref cardiovascular-appendices "Cardiovascular Methodology".