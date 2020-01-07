%Hepatic Methodology {#HepaticMethodology}
=====================

Overview
========
@anchor hepatic-abstract
Abstract
--------

The %BioGears&reg; %Hepatic System provides the starting point for modeling the many functions of the liver. The current implementation focuses on nutrient management, including albumin production, glycogenolysis, glycogenesis, lipogenesis, and gluconeogenesis.

System Design
=============
@anchor hepatic-background
Background and Scope
--------------------

The %Hepatic System has many functions in the human body, including detoxification, protein synthesis, the production of digestive enzymes, regulation of metabolism, decomposition of red blood cells, and production of hormones. The liver is a highly specialized organ, mostly consisting of hepatocytes. Many hepatic functions were previously lumped into the %BioGears %Tissue System, but with the expansion of nutrient modeling, a %Hepatic System was called for. The %Hepatic System in %BioGears manages the storage of glucose as glycogen, the production of the protein albumin, the generation of triacylglycerol as tripalmitin in cases of nutrient excess, and the generation of glucose and ketones via gluconeogenesis in times of nutrient shortage.

@anchor heptaic-dataflow
Data Flow
---------

<br>
<img src="./images/Hepatic/HepaticDataFlow.PNG" width="800">
<center>
*Figure 1. The data flow for the %Hepatic System consists of PreProcess, Process, and PostProcess. ProduceAlbumin occurs in the PreProcess step, while all other %Hepatic functionality occurs in the Process step.*
</center><br>

### Reset, Conditions, and Initialization
Glycogen values are initialized to full. This replaces the previous version's intialization with a meal in the stomach. There are currently no conditions in the %Hepatic System.

### Preprocess
#### Produce Albumin
The liver produces albumin as part of normal function. A flat rate of albumin production is estimated as 0.15 mg/s, as found in @cite Jarnum1972plasma. At each time step, the mass is added to the liver extracellular compartment. Improvements to the albumin production and transport model are [planned](@ref hepatic-comingsoon).

### Process
#### Glycogenesis
When the body has high blood glucose, the liver converts blood-borne glucose molecules to the chain-form glycogen, which it then stores for times when blood glucose drops. This process, as well as the following Process methods, is governed by the hormones insulin and glucagon, the modeling of which can be read about in @ref EndocrineMethodology.

#### Glycogenolysis
Glycogenolysis is essentially the reverse of glycogenesis. Glycogen is broken down into glucose and dumped into the blood.

#### Lipogenesis
Triacylglycerol (as tripalmitin) is generated from excess glucose or amino acids (modeled as alanine).

#### Gluconeogenesis
Glucose is generated from three different sources in times of low blood glucose: generation from lactate (produced during anaerobic metabolism), synthesis from amino acids, or conversion of the glycerol backbone of triacylglycerol. Note that ketogenesis (the generation of acetoacetate) happens concurrently with the conversion of the triacylglycerol's glycerol backbone.

### Post Process
There is no system specific function for Post Process in the %Hepatic System.

### Assessments
There are no assessments in the %Hepatic System.

@anchor hepatic-features
Features, Capabilities, and Dependencies
----------------------------------------

The behavior of the %BioGears %Hepatic System is driven by the two blood-borne hormones insulin and glucagon. The ratio of these hormones, the so-called [hormone factor] (@ref endocrine-hormone-factor) determines whether the liver works to pull glucose from the blood (converting it to glycogen or triacylglycerol) or replenish blood glucose (by breaking down glycogen or converting amino acids, lactate, or glycerol). Once the direction of glucose change is determined, the stoichiometric relationships between reactants and products are used to increment substances as necessary. The general approach used is to define an upper and lower rate for the given process, then to use a logistic function to model the transition between rates as a function of hormone factor. The logistic equation is of the form

\f[R=R_{L} +\frac{R_{U} -R_{L} }{1+e^{-S(HF-O)} } \f] 
<center>
*Equation 1.*
</center><br>
Where *R* is the resultant rate of substance conversion, *R<sub>L</sub>* is the lower substance conversion rate, *R<sub>U</sub>* is the upper substance conversion rate, *S* is a shape modifier for the sigmoid, *O* is an offset value, and *HF* is the hormone factor in the blood compartment.

### Glycogenesis
The liver can store up to 5-8% of its weight as glycogen @cite guyton2006medical, and the total mass of the liver in %BioGears varies based on patient characteristics. The %Hepatic System sets the maximum value for liver glycogen to 6.5% of its mass. The muscle tissue also contains glycogen, up to 1-3% of muscle mass, but this glycogen is limited to the muscle, since when it is broken down, it retains phosphorylation and cannot diffuse @cite guyton2006medical. The rate of glycogenesis is estimated from @cite tardie2008glycogen, which states that under normal circumstances, 2% of total body glycogen is regenerated every hour after depletion. Furthermore, with ideal feeding, that rate can rise to 5% of total glycogen replenished each hour. In @cite rothman1991quantitation, the glycogenesis rate after a meal following a 64 hour fast is in agreement with this range. Thus, the glycogenesis rate logistic function is given two clear inputs, and the shape modifier and offset were empirically set to give good response. Note that for liver glycogenesis, the hormone factor is checked in the liver, whereas for muscle glycogenesis, it is checked in the muscle. Figure 1 below shows glycogenesis in action during a %BioGears run.

<img src="./images/Hepatic/Glycogenesis.png" width="800">
<center>
*Figure 1. Liver glycogen increases after ingesting 100g of carbohydrates at 30 minutes. Once the meal is digested and absorbed, the liver glycogen increases within the rates described.*
</center><br>

### Glycogenolysis
Glycogen is broken down in times of low blood glucose, such as during exercise or fasting. The lower glycogenolysis rate was calculated from @cite rothman1991quantitation, which shows a nearly linear rate for the first 22 hours of a fast, with a slowdown as the liver runs out of glycogen in later days of fasting. This resting glycogenolysis rate is echoed in @cite petersen2004regulation, which shows a similar resting rate. As metabolic demands increase under exercise conditions (when the hormone factor would drop), the glycogenolysis rate increases @cite petersen2004regulation. Note that muscle glycogenolysis does not occur in the same manner as hepatic glycogenolysis, and so is unaffected by this rate calculation. For more information on muscle glycogenolysis, see the [Tissue System] (@ref tissue-metabolic-production). Figure 2 below shows glycogenolysis during a 6 hour fast.

<img src="./images/Hepatic/Glycogenolysis.png" width="800">
<center>
*Figure 2. Liver glycogen drops from its initial full value as fasting progresses.*
</center><br>

### Lipogenesis
The liver has the ability to convert glucose or amino acids to lipid in times of excess. De novo hepatic lipogenesis rates are given by @cite mcdevitt2001denovo. Adipose tissue is also shown to contribute to generation of new lipid @cite chascione1987effect, giving a higher whole-body rate of lipogenesis. The two functions are lumped together in this one method, and the rate varies logistically between them. Because the amount of lipid derived from amino acid or the lipid derived from glucose depends on the amount of each precursor available, an assumption is made that the amount of lipid generated from either is proportional to the concentration in the liver. Thus, if there is twice as much amino acid in the liver as glucose, there will be twice as much lipid generated from amino acid. Lipogenesis is an energy-consuming process, but it is assumed that the energy cost is rolled into the basal metabolic rate. Lipogenesis results in carbon dioxide production, and if amino acids are converted, urea is produced as well. Figure 3 below shows instantaneous lipogenesis activity over 12 hours when ingesting 100g of carbohydrates.

<img src="./images/Hepatic/Lipogenesis.png" width="800">
<center>
*Figure 3. Lipogenesis occurs as long as there is a positive hormone factor.*
</center><br>

@anchor hepatic-gluconeogenesis
#### Gluconeogenesis
%Hepatic gluconeogenesis contributes heavily to maintenance of blood glucose. Three primary chemical conversions result in production of new glucose: the recombination of lactate into glucose, the removal and conversion of the glycerol backbone of triacylglycerol, and the conversion of amino acid into glucose precursor and urea @cite boulpaep2009medical. Because of this, gluconeogenesis is an important mechanism during fasting and starvation. In %BioGears, empirical rates are used for most of the gluconeogenic processes, tuned using values from @cite garber1974hepatic. For now, the liver converts any lactate in its extracellular compartment into glucose each timestep. Breakdown of triacylglycerol accomplishes two goals: generation of new glucose from the glycerol backbone and ketogenesis via cleaving of the palmitate chains. Note that %BioGears assumes that all ketones are acetoacetate; acetone and beta-hydroxybutyrate are not considered. Also note that ketogenesis is an oxygen-consuming process that contributes to the respiratory quotient. The breakdown of amino acids occurs at a constant rate, and urea is generated as a byproduct. All processes are stoichiometrically consistent. The total rate of glucose production and ketone production, as well as the proportions of substrates used to generate those substances, are generally in line with those outlined in @cite garber1974hepatic.

@anchor hepatic-dependencies
### Dependencies

The %Hepatic System is dependent on the key systems of the %BioGears Engine. The %Gastrointestinal System determines how much of each nutrient is absorbed into the blood. The %Endocrine System governs the synthesis rate of insulin and glucagon, which regulate %Hepatic processes, and the %Tissue System governs the diffusion of these nutrients between vascular and extravascular compartments.

@anchor hepatic-assumptions
Assumptions and Limitations
---------------------------

One major assumtion in %BioGears' implementation of nutrition is that fat remains in its triacylglycerol form. The formation of chylomicrons, the breakdown and reconsititution from fatty acids and the participation of albumin, or the packaging of LDLs are not considered. Lipogenesis is an energy-consuming process, but we assume that the energy required is rolled up in the basal metabolic rate. Additionally, while the substrates converted to fat in lipogenesis really depend on the presence and concentrations of facilitating enzymes, we assume that the proportion of glucose converted to fat with respect to amino acid is relative to the concentrations of glucose and amino acid in the liver, since the enzymes are not modeled. One limitation of the current implementation is that glucose is the only substance that directly affects the production of insulin and glucagon, when in reality both consumed protein and released epinephrine can affect these hormones. Many assumptions are made in the simple model of lactate generation and reconversion, the primary one being that gluconeogenesis is not rate limited in this respect.

Many of these assumptions are planned to be addressed or reduced in upcoming releases.

Conditions
----------
There are no conditions associated with the %BioGears %Hepatic system.

Actions
-------
There are currently no actions in the %Hepatic system.

Events
------
- LiverGlycogenDepleted: Set when liver glycogen drops below one gram. Removed when liver glycogen returns above 10 grams.
- MuscleGlycogenDepleted: Set when muscle glycogen drops below one gram. Removed when muscle glycogen returns above 10 grams.

Results and Conclusions
=======================

@anchor hepatic-validation
Validation - Resting Physiologic State
--------------------------------------

The %Hepatic System regulates substances that are validated in other systems. For validation of resting physiological state, see the relevant sections in the [Tissue] (@ref tissue-validation) and [Gastrointestinal] (@ref GI-results) Systems.

Validation - Actions
--------------------------------------
There are currently no validated actions or conditions associated with the %Hepatic system. However, some of its funcitonality is validated in the [Energy] (@ref energy-results) System.

@anchor hepatic-conclusions
Conclusions
-----------

The %Hepatic System provides functionality for the storage and conversion of ingested nutrients to maintain physiological homeostasis.

Future Work
===========
@anchor hepatic-comingsoon
Coming Soon
-----------

Recommended Improvements
------------------------
- FFA/Albumin binding and better fat metabolism
- Epinephrine interactions with insulin/glucagon/exercise

Appendices
==========

Data Model Implementation
-------------------------

@ref HepaticSystemTable "Hepatic" 