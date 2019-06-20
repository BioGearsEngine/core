%Drugs Methodology {#DrugsMethodology}
=====================

@anchor drugs-overview
Overview
========

@anchor drugs-abstract
Abstract
--------
%Substances, including drugs, gases, and hormones, are transported through the body within the bloodstream and airflow in the %Cardiovascular and %Respiratory Systems, respectively. This provides important nutrients and waste removal for normal physiologic function. The %BioGears&reg; engine models advection in both fluid systems (see @ref SubstanceTransportMethodology). Transvascular transport is simulated using one of several diffusion models (see @ref TissueMethodology) for substances generically. Drug transvascular transport is modeled with a physiologically-based pharmacokinetic (PBPK) model, and the physiologic effects on the body are modeled with a low-fidelity pharmacodynamic (PD) model.

@anchor drugs-introduction
Introduction
------------
%Drugs influence the body by moving it out of homeostasis for a specific purpose, such as sedation for surgical intervention, or to address a patient condition, such as analgesic intervention. After a drug enters the patient&rsquo;s body (through intravenous administration, absorption through the gastrointestinal system, or inhalation of an agent), the substance ultimately is transported through the bloodstream for uptake by the tissues and organs. The process of clearing a drug from the system is accomplished in a combination of three methods: renal clearance, hepatic clearance, and systemic clearance. Depending on the drug, it may be processed by either the kidney, the liver, or both for removal from the system as a waste product. Additionally, drugs are metabolized by the body and removed from the bloodstream. These methods all reduce the amount of drugs in the system. The drug transport and clearance methodologies in %BioGears are described below.

@anchor drugs-system-design
System Design
=============

Background and Scope
--------------------
*Pharmacokinetics* is the study of the relationship between the dose of a drug and the time-evolution of the concentration of the drug throughout the body. The relationship between the concentration of a drug and the physiological response is described by the *pharmacodynamics* @cite rosenbaum2012basic. %BioGears has both pharmacokinetic and pharmacodynamic models. Because it is physiologically-based, the pharmacokinetic model has resolution at the same scale as the cardiovascular and respiratory systems. Physiologically-based pharmacokinetic (PBPK) modeling is a technique that mathematically models the distribution, uptake, metabolism, and clearance of a drug with consideration of the relevant physiological processes @cite khalil2011physiologically. The PBPK model provides more resolution than the classic one and two compartment models. The PBPK model also provides a base on which to build a receptor-based pharmacodynamic (PD) model, although the %BioGears PD model is currently a lower-fidelity phenomenological model based solely on the concentration of a drug in the blood. %Drugs impact body-level physiologic behavior such as heart rate, respiration rate, and blood pressure as result of cellular-level interactions caused by a drug that changes the behavior of physiologic entities, such as blood vessels and electrical activity in the heart. The cellular-level processes are not modeled in %BioGears, and instead the PD model is a direct relationship between concentration and maximum effect for a body-level response.

@anchor drugs-data-flow
Data Flow
---------
### Preprocess
#### Administer %Substances and Administer IV %Substances
All substances added or removed from the %BioGears body are incremented or decremented from the appropriate compartment during the Preprocess step. For example, if substances are administered via a bolus injection or through intravenous administration, the substance mass is increased in the vena cava compartment.

### Process
The generic substance methodology developed for the %BioGears Engine is used to solve for the mass, concentration, substance volume, and volume fraction in each compartment. After the masses and concentrations have been updated, the system-level effects of any circulating drugs are calculated using the [pharmacodynamic](@ref drugs-pharmacodynamics) model.

### Post Process
The Post Process step moves everything calculated in Process from the next time step calculation to the current time step calculation. %Substances have no specific Post Process functionality. Each system post processes their circuit and moves the mass, concentration, substance volume, and volume fraction from next to current.

<img src="./images/Drugs/DrugsDataFlow.png" width="900">
<center>
<i>Figure 1. The data flow begins with a Preprocess step where substances entering or exiting the %BioGears body are incremented or decremented. During the Process step, the mass and concentration (blood) or substance volume and volume fraction (air/gas) are calculated for each compartment in a system. During Process, the effects of any drugs in the system are calculated. There is no specific Post Process functionality in substances. However, as other systems post process their circuits, the substance values are moved from the next time step to the current time step calculation.</i>
</center><br>

Features, Capabilities, and Dependencies
----------------------------------------
Pharmacokinetic models quantify the time evolution of drug distribution from the administration of a dose. Pharmacodynamic models, on the other hand, characterize the events from the arrival of the drug at the site of action to the “onset, magnitude and duration of the biological response" @cite rosenbaum2012basic. The %BioGears %Drug system contains both pharmacokinetic and pharmacodynamic models.

@anchor pharmacokinetics
### Pharmacokinetics
The pharmacokinetic (PK) methodology provides a means for simulating the time-evolution of the distribution of a drug throughout the body. This is accomplished by using the administration actions discussed in the [Actions](@ref drugs-Actions) section below, the [transport methodology](@ref CircuitMethodology), the [perfusion limited diffusion](@ref tissue-perfusionLimitedDiffusion) methodology, and the substance files discussed in the [Common Data Model](@ref SubstanceTable) documentation. Once a drug is administered, it either enters the %Cardiovascular System (liquid) or the %Respiratory System (inhaled). If it is an inhaled drug, modeled in %BioGears as a gas, the drug will move into the alveoli through advection then diffuse into the %Cardiovascular System through alveoli transfer. Drug effects, described below in the [Pharmacodynamics](@ref drugs-pharmacodynamics) section, are then computed based on the concentration of the drug in the plasma. Note that the plasma concentration calculation is currently incorrect. The concentration of a drug is currently computed by dividing the mass of the drug in a compartment by the volume of plasma, as computed from the hematocrit. Implicit in the plasma concentration calculation is the assumption that all of the drug is in the extracellular space. In actuality, some of the drug will cross blood-cell membranes, and the concentration of drug in the plasma and in the blood is a function of many factors, including the lipophilicity. We will address this error in the [future](@ref drugs-future).

#### Partition Coefficient
The drugs circulate around the cardiovascular circuit via the generic transport methodology described in the @ref CircuitMethodology. However, drugs diffuse from the %Cardiovascular System into the tissues via [perfusion limited diffusion](@ref tissue-perfusionLimitedDiffusion). For each drug, the physicochemical properties are used to calculate the partition coefficient. This partition coefficient describes the tendency of a particular drug to diffuse across the barrier between the cardiovascular and tissue spaces. Partition coefficients are calculated using physiochemical properties defined in the substance file associated with a drug.  For a very weak base, an acid, or a neutral, Equation 1 is used to calculate the partition coefficient.

\f[{K}_{pu} = f_{EW} + \frac{X * f_{IW}}{Y} + \frac{P * f_{NL} + (0.3 * P + 0.7) * f_{NP}}{Y} + \left[\left(\frac{1}{f_{u}} - 1 - \left(\frac{P * f_{NL} + (0.3 * P + 0.7) * f_{NP}}{Y}\right)\right) * \frac{PR_{T}}{PR_{B}}\right] \f] 
<center>
<i>Equation 1.</i>
</center><br>

For moderate to strong bases, Equation 2 is used to calculate the partition coefficient.

\f[{K}_{pu} = f_{EW} + \frac{1 + 10^{pK_{a}-pH_{IW}} * f_{IW}}{1 + 10^{pK_{a}-pH_{p}}} + \frac{K_{a}*AP_T*10^{pK_{a}-pH_{IW}}}{1+10^{pK_{a}-pH_{p}}} + \frac{P * f_{NL} + (0.3 * P + 0.7) * f_{NP}}{1+10^{pK_{a}-pH_{p}}}\f] 
<center>
<i>Equation 2.</i>
</center><br>

Where *X* and *Y* are the different relationships for pH, as shown in Table 1, *f<sub>IW</sub>* is the fraction of intracellular water, *f<sub>EW</sub>* is the fraction of extracellular water,  *f<sub>NP</sub>* is the fraction of neutral phospholipids in the tissue, *f<sub>NL</sub>* is the fraction of lipids in the tissue, *P* is the octanol:water partition coefficient for the drug, *f<sub>u</sub>* is the fraction of the drug unbound in plasma,  *f<sub>NL,P</sub>* is the fraction of neutral lipids in plasma, *f<sub>NP,P</sub>* is the fraction of phospholipids in plasma,  and *PR<sub>T</sub>* / *PR<sub>B</sub>* is the tissue to plasma ratio of the binding protein. 

<center>
<i>Table 1. The pH effects on the partition coefficient are described based on the drug type, i.e., very weak base, acid, or neutral. X relates the drug property pK<sub>a</sub> to the pH<sub>IW</sub>, intracellular water pH, while Y relates the pK<sub>a</sub> to the pH of the plasma.</i> </center>
| Ionic State    |    X    |    Y    |
| -------------- | :-----: | :-----: |
| Acid           | \f[1 + 10^{pH_{IW}-pK_{a}}\f] | \f[1 + 10^{pH_{p}-pK_{a}}\f] |
| Very Weak Base | \f[1 + 10^{pK_{a}-pH_{IW}}\f] | \f[1 + 10^{pK_{a}-pH_{p}}\f] |
| Neutral        | 1 | 1 |
<br>

A number of these values are drug parameters found in the substance file definitions. However, many of them are organ/compartment dependent as shown in Tables 2 and 3. 

<center>
<i>Table 2. The volume fractions for neutral lipids and phospholipids vary by compartment. These values are specified for each compartment as outlined below and used to calculate the partition coefficient of each drug for each tissue compartment in the %BioGears engine. These values were found in @cite rodgers2005physiologically.</i>
</center>

|	%Tissue Compartment	|	Extracellular Water (*f<sub>EW</sub>*) 	|   Intracellular Water (*f<sub>IW</sub>*)  |    Neutral Lipd (*f<sub>NL</sub>*)  |  Neutral Phospholipid (*f<sub>NP</sub>*)   |
| :---- | :---- | :---- |  :---- | :---- |
|	Adipose (Fat)	|  0.135  |  0.017  |  0.853  |  0.0016  |
|	Bone	        |  0.1    |  0.346  |  0.017  |  0.0017  |
|	Brain           |  0.162  |  0.620  |  0.039  |  0.0015  |
|   Gut             |  0.282  |  0.475  |  0.038  |  0.0125  |
|   Kidney          |  0.273  |  0.483  |  0.012  |  0.2420  |
|   Liver           |  0.161  |  0.573  |  0.014  |  0.2400  |
|   Lung            |  0.336  |  0.446  |  0.022  |  0.0128  |
|   Muscle          |  0.118  |  0.630  |  0.010  |  0.0072  |
|   Myocardium      |  0.320  |  0.456  |  0.014  |  0.0111  |
|   Skin            |  0.382  |  0.291  |  0.060  |  0.0044  |
|   Spleen          |  0.207  |  0.579  |  0.0077 |  0.0113  |


<center>
<i>Table 3. The tissue to plasma protein binding ratios are shown for each tissue compartment in the %BioGears Engine. Two proteins are available for drug binding, albumin and lipoprotein. The binding protein for each drug is specified in the substance file. These values were found in @cite rodgers2005physiologically.</i>
</center>

|	%Tissue Compartment	|	%Tissue to Plasma Albumin Ratio	|   %Tissue to Plasma Lipoprotein Ratio  |
| :---- | :---- | :---- |
|	Adipose (Fat)	|  0.049  |  0.068  |
|	Bone	        |  0.100  |  0.050  | 
|	Brain           |  0.048  |  0.041  |
|   Gut             |  0.158  |  0.0141 |
|   Kidney          |  0.130  |  0.137  |
|   Liver           |  0.086  |  0.161  |
|   Lung            |  0.212  |  0.168  |
|   Muscle          |  0.064  |  0.059  |
|   Myocardium      |  0.157  |  0.160  |
|   Skin            |  0.277  |  0.096  |
|   Spleen          |  0.097  |  0.207  |

The %BioGears implementation of the partition coefficients calculation and all values shown in the tables are derived from the literature @cite rodgers2005physiologically @cite rodgers2006physiologically @cite khalil2011physiologically @cite huisinga2012modeling @cite valentin2002icrp.

If it is not possible to obtain the information required to compute the partition coefficient for a drug, or if the calculation is not valid for a particular drug for any reason, it is possible to override the computation by directly inputting a constant partition coefficient in the drug file. Note that, to use this functionality, it is best to input a coefficient for every tissue compartment in the model. %BioGears currently uses the coefficient-computation override methodology for Albuterol, Ketamine, Morphine, and Sarin. The parameters used to calculate each partition coefficient were found in published data and/or tuned to get the desired kinetic response.

@anchor drugs-diffusion
#### Diffusion
The above partition coefficients are used to calculate the diffusion of a drug across the tissue-vascular barrier. All current drugs in the %BioGears Engine use perfusion limited diffusion as found in @cite khalil2011physiologically @cite huisinga2012modeling. For details on perfusion-limited and other diffusion modalities, see the @ref TissueMethodology.

In the current version of %BioGears, all drugs diffuse by the perfusion-limited model described above. An alternative means for modeling drug kinetics would be to leverage the other diffusion methods described in the @ref TissueMethodology report. The other diffusion models were developed primarily for the transmembrane transport of ions and a few specific organic molecules (e.g. glucose), and they have not been tested for drugs.

@anchor drugs-clearance
#### Clearance
The PBPK model represents renal, hepatic, and systemic clearance. The renal clearance rate, intrinsic clearance rate, and the systemic clearance rate are specified in the substance file in units of milliliter-blood per second per kilogram. The intrinsic clearance rate is used to calculate the hepatic clearance, as shown in Equation 3.

\f[Cl_{H} = \frac{Q_{H}*f_{u}*Cl_{I}*BW}{Q_{H} + f_{u}*Cl_{I}*BW} \f]
<center>
<i>Equation 3.</i>
</center><br>

Where *Cl<sub>H</sub>* is the hepatic clearance, *f<sub>u</sub>* is the fraction of the drug unbound in plasma, *Cl<sub>I</sub>* is the intrinsic clearance, and *BW* is the patient body weight.

The amount of drug removed (cleared) from the system is found by calculating the amount of fluid volume that can be cleared by a healthy organ. The volume is calculated, as shown in Equation 4. This process is completed for the renal and hepatic clearance. 

\f[V_{Cl} = Cl_{H}*BW*dt \f]
<center>
<i>Equation 4.</i>
</center><br>

Where *V<sub>cl</sub>* is the volume cleared, *Cl<sub>H</sub>* is the organ clearance rate, *BW* is the body weight, and *dt* is the time step. The actual mass of the substance removed during this process is found by determining the amount of the drug in the volume cleared, as shown in Equation 5.

\f[M_{Cl} = V_{Cl}*C \f]
<center>
<i>Equation 5.</i>
</center><br>

Where *M<sub>Cl</sub>* is the mass cleared, *V<sub>Cl</sub>* is the volume cleared, and *C* is the concentration in the tissue. The systemic clearance represents the total clearance for the body, including the renal and hepatic clearance. Therefore, the total volume cleared is calculated, then the renal and hepatic clearance volumes are removed, as shown in Equation 6. The &ldquo;remaining&rdquo; systemic mass cleared is calculated as shown in Equation 5. Half of the mass cleared from renal clearance is removed from each of the kidney tissue compartments, the mass cleared from hepatic clearance is removed from the liver tissue compartment, and the mass removed from the &ldquo;remaining&rdquo; systemic clearance is removed from the vena cava compartment. This is assumed to be from various metabolic processes in the plasma.

\f[V_{Cl} = \left(Cl_{s}*BW*dt\right) - V_{ClR} - V_{ClH} \f]
<center>
<i>Equation 6.</i>
</center><br>

Where *V<sub>Cl</sub>* is the remaining systemic volume cleared, *Cl<sub>S</sub>* is the systemic clearance rate, *BW* is the patient body weight, *dt* is the time step, *V<sub>ClR</sub>* is the renal volume cleared, and *V<sub>ClH</sub>* is the hepatic volume cleared.

@anchor drugs-pharmacodynamics
### Pharmacodynamics
Pharmacodynamic (PD) models characterize the effect that a drug has on the organism. The current version of %BioGears contains two pharmacodynamic models.
- A simple dose-response model with a response biomarker functionally linked to the plasma concentration of a drug via a first order rate constant.
- A site-of-action model with a local response functionally linked to the local tissue concentration of a drug

#### Effect Site Model
Development of the %BioGears pharmaocodynamic model began with a characterization of expected results by subject matter experts for a small library of drugs. The expected results were limited to whole-body biomarkers similar to vital signs (e.g. changes in heart rate, respiration rate, blood pressure, etc.). We treated the expected results as surrogate endpoints and used the common sigmoidal E<sub>max</sub> model to characterize the effects.  The model was initially calibrated to drug plasma concentration but has been updated to account for delays in equilibration between the concentration in the plasma and at the site-of-action.  This was accomplished by introducing a hypothetical "effect site" compartment, transfer to which from the plasma is governed by a drug-specific first order rate constant (*k<sub>e</sub>*) @cite lotsch2005pharmacokinetic.  The utility of this strategy is evident when considering a drug such as morphine, which exhibits delays in pharmacological activity due to its slow diffusion across the blood-brain barrier to the central nervous system, where it primarily targets mu-opioid receptors @cite lotsch2002pharmacokinetic.  In the case of drugs for which *k<sub>e</sub>* data was not available, the effect site concentration is assumed to be the plasma concentration as before.     

In its first iteration, the E<sub>max</sub> model referred to the expected effect of the drug rather than the maximum effect of the drug. For that reason, overdose modeling is not possible for most drugs in %BioGears. We are working to obtain empirical data from case studies that will allow us to adjust drug response curves to describe non-clinical dose regimes.  Currently, only morphine has been completed with satisfactory overdose functionality (see [Opioid Intoxication](@ref opioids)).  The pharmacodynamic effects included in the %BioGears effect concentration PD model are described in Table 4.

<center>
<i>Table 4. Pharmacodynamic effects modeled in the %BioGears Engine with descriptions and relevant ranges.</i>
</center>

|	Pharmacodynamic Effect	|	Description 	|   Available Values   |
| :---- | :---- | :---- |
|	Bronchodilation	|	Drug effect on the bronchii radii. A maximal effect of 1.0 will reverse the constriction of an asthma attack. | 0 to 1 |
|	Diastolic Pressure	|	Change in the diastolic pressure. Given as a fraction. | -1 to 1 |
|	Heart Rate	|	Change in the heart rate. Given as a fraction. | -1 to 1 |
|	Hemorrhage Flow	|	Change in hemorrhage bleeding rate, with negative reducing the effect of hemorrhage. | -1 to 1 |
|   Neuromuscular Block |  Drug&rsquo;s strength as a neuromuscular block, with a 1.0 as the strongest block.  | 0 to 1 |
|   Pupil Size Modifier |  Drug effect on the pupil diameter. Given as a modifier. | -1 to 1 |
|   Pupil Reactivity Modifier |  Drug effect on the pupil reactivity to light. Given as a modifier. | -1 to 1 |
|   Respiration Rate | Change in the respiration rate. Given as a fraction. | -1 to 1 |
|   Sedation  |   Drug&rsquo;s strength as a sedative, with a 1.0 as the strongest sedative.  | 0 to 1 |
|   Systolic Pressure  | Change in the systolic pressure. Given as a fraction. | -1 to 1 |
|   Tidal Volume  |  Change in the tidal volume. Given as a fraction. | -1 to 1 |
|   Tubular Permeability | Drug localized effect on tubulear permeability. Given as a severity, 1.0 complete reabsorption block | 0 to 1 | 
|   Central %Nervous Response | Drug depressant effect on receptors in the central nervous system directing chemoreceptor action.  Given as a severity with 1.0 being complete suppression| 0 to 1 | 

The drug effects are specified for each drug in the substance file. The level of effect is calculated based on the expected effect (or, for some drugs, the maximum effect) of the drug and the current effect site concentration for the drug, as shown in Equation 7 @cite mortensen2008introduction and Equation 8 @cite rosenbaum2012basic.  

\f[\frac{dC_{e}}{dt} = k_{e}*(C_{p}-C_{e}) \f].
<center>
<i>Equation 7.</i>
</center><br>

\f[\Delta E = E_{bl}*\frac{E_{m}*C_{e}^\eta}{EC_{50}^\eta+C_{e}^\eta} \f]
<center>
<i>Equation 8.</i>
</center><br>

According to Equation 7, the rate at which the effect compartment concentration (*C<sub>e</sub>*) equilibrates with the plasma concentration (*C<sub>p</sub>*) is directly proportional to the difference between (*C<sub>p</sub>*) and (*C<sub>e</sub>*) and a first order rate constant, *k<sub>e</sub>*.  Note that it is assumed that the rates of influx to and efflux from the effect compartment are equivalent.  (*C<sub>e</sub>*) is utilized in Equation 8, where *E<sub>m</sub>* is the expected (or maximum) effect of the drug, *EC<sub>50</sub>* is the concentration at 50% of the effect, *E<sub>bl</sub>* is the baseline for that effect (i.e., heart rate baseline), <i>&Delta;E</i> is the calculated effect of the drug, and <i>&eta;</i> is the slope factor @cite rosenbaum2012basic.  This procedure is repeated for each of the effects in Table 4. 

The drug effect is applied as a fraction of the baseline for the biomarker, which is a patient variable in %BioGears. For example, a person with a resting heart rate of 72 bpm may have a physiological process happening during simulation, perhaps as a condition, which increases his/her baseline heart rate to 80 bpm. If this patient is given a drug with a heart rate effect of 0.2, then at maximum the patient's heart rate will be 96 bpm (80 + 0.2 * 80). If the same drug were given to the same patient but without the condition, the maximum heart rate will be 86.4 bpm (72 + 0.2 * 72).

The *EC<sub>50</sub>* values which could not be inferred from literature were estimated from the maximum concentration of the drug at a standard adult dose, as shown in Equation 9.

\f[EC_{50} = \frac{C_{max}}{32} \f]
<center>
<i>Equation 9.</i>
</center><br>

Where *C<sub>max</sub>* is the maximum plasma concentration for the standard adult dose and *EC<sub>50</sub>* is the concentration at which 50% of the effect should be present.

@anchor-drugs-interactions
####Drug-Drug Interactions
Drugs can be administered in combination in %BioGears.  However, in most cases the interaction will modeled only by stacking the pharmacodynamic modifiers associated with each drug.  Such an approach is appropriate in very limited circumstances.  We currently have two reversal agents whose interactions with their targets are modeled in greater detail:  Pralidoxime--an organophosphate antagonist (see [Sarin Model] (@ref sarin-nerve))--and naloxone, which opposes opioids such as Morphine and Fentanyl.  Naloxone is an interesting case because it exerts virtually no pharmacodynamic effects on its own; it operates solely by competitively binding with opioids for mu receptors in the central nervous system @cite goodman1996goodman.  The impact of competitive antagonists on the effect of an agonist such as an opioid can be modeled by Equation 10 @cite chang2011quantitative. 

\f[\Delta E = E_{bl}*\frac{E_{m}*C_{e}^\eta}{EC_{50}^\eta*(1+\frac{C_I}{K_I})^\eta+C_{e}^\eta}\f]
<center>
<i>Equation 10.</i>
</center><br>   

The parameters E<sub>bl</sub>, E<sub>m</sub>, C<sub>e</sub>, and EC<sub>50</sub> are defined as previously with respect to the agonist.  K<sub>I</sub> captures the tendency of the antagonist to displace the agonist from the shared target receptor (lower implies greater potency).  Conveniently, when no antagonist is present, Equation 10 reduces to Equation 8, allowing it to be implemented with minimal additional modeling effort.  Naloxone reversal of opiod induced respiratory depression is currently the only competitive antagonism scenario modeled in this way (see [Reversal of Opioid Induced Respiratory Depression](@ref opioids)).  We are working to incorporate partial and non-competitive antagonism, which will allow us to describe increasingly complex interactions.

@anchor drugs-cardiovascular-effects
##### %Cardiovascular Effects
Equation 8 provides a straightforward calculation for the heart rate. However, to be applied in the cardiovascular system, the drug effects need to be translated to the effects on the mean arterial pressure and the pulse pressure. This is accomplished using Equations 11 and 12. These equations were developed using the relationship between diastolic and systolic pressure and the mean arterial pressure @cite guyton2006medical.

\f[\Delta MAP = \left(2* \Delta DBP \right) + \frac{\Delta SBP}{3} \f]
<center>
<i>Equation 11.</i>
</center><br>

\f[\Delta P_{pulse} = \Delta SBP - \Delta DBP \f]
<center>
<i>Equation 12.</i>
</center><br>

Where *MAP* is the mean arterial blood pressure, *DBP* is the diastolic blood pressure, and *SBP* is the systolic blood pressure. These changes to the heart rate, mean arterial pressure, and pulse pressure are then system outputs for the drug system. The changes are then applied in the %Cardiovascular System. For more details on the implementation of the effects see the @ref CardiovascularMethodology.

@anchor drugs-respiratory-effects
##### %Respiratory Effects
As with the %Cardiovascular System, the respiratory and tidal volume effects are relatively straight forward to apply. The neuromuscular block and sedation levels are calculated to have an effect level between zero and one, where zero is no effect and one is a maximal effect. The detail of how these are implemented can be found in the @ref RespiratoryMethodology.

#### Site-of-Action Models
There are currently three drugs which utilize a localized PD effects model: Albuterol, Furosemide, and Sarin.

##### %Respiratory Effects
Aerosolized substances have a direct effect on the respiratory system. There are two direct-effect aerosol substance properties.
- BronchioleModifier
	- A parameter which controls the dilation (positive value) or constriction (negative value) of the bronchioles in the presence of the substance
- InflammationCoefficient
	- A parameter to quantify the propensity of the aerosol to cause inflammation of the respiratory tissues

For example, Albuterol is an aerosolized drug that typically enters the body via nebulizer or metered-dose inhaler. Accordingly, in %BioGears Albuterol enters the body through the respiratory tract and diffuses into the respiratory tissues (see the [Aerosol Section](@ref environment-aerosol) in the @ref EnvironmentMethodology report). The BronchioleModifier for Albuterol is set to 1 in the substance file, meaning that Albuterol has the capability to maximally dilate the bronchioles. The bronchodilatory effects of Albuterol are applied directly at the site of action, and the actual bronchodilation is a function of the concentration of Albuterol in the respiratory tissues.  The nerve agent Sarin has the opposite effect of Albuterol, inducing severe bronchoconstriction.  As such, the BronchioleModifier for Sarin is set to a negative value.  See the [Nerve Agent:  Sarin](@ref sarin-nerve) section for more details. 

##### %Renal Effects
The pharmacodynamic effects of diuretics locally target the renal system's functionality. Local binding of the Na-K-2Cl symporter by Furosemide (Lasix) in the tubular lumen stimulates an increase in urine production and ion excretion. In %BioGears we simulate this effect by adjusting the reabsorption permeability as a function of plasma concentration of Furosemide. This enables simulation of the whole-body drug effects, but the model does not use the local concentration of drug in the tubular lumen as the primary determinant of the reabsorption adjustment. A mechanistic model based on traditional receptor theory which combines the tissue properties of receptor concentration and signal transduction with the drug properties of intrinsic efficacy and affinity to form a model of drug response would use the local tissue concentration as the first link in a causal chain leading to permeability changes and the appropriate downstream effects. This type of mechanistic model is recommended as future work. For details on the reabsorption and urine production of the renal system, see the @ref RenalMethodology report. The results of the Lasix administration simulation are compared to data found in literature and presented in the [validation section](@ref drugs-furosemide).

@anchor drugs-substances
### %Substances
The full list of drugs available in the %BioGears library can be found in Table&nbsp;5 in the Validation section. Additional substances, such as oxygen, carbon dioxide, and hemoglobin that are available in the %BioGears Engine can be found in the @ref BloodChemistryMethodology.

@anchor drugs-variability
### Patient Variability
It is possible to customize the body that is simulated by %BioGears. For a detailed discussion of patient variability in %BioGears please see the @ref PatientMethodology report. Variability in body morphology will obviously have an effect on the pharmacokinetics of a drug. For example, a larger body may have a larger plasma volume, and thus the plasma concentration for a specific dose of a drug will be less than the same dose administered to a smaller body. Additionally, the partitioning of drugs is dependent on the relative masses of the physiological compartments, and storage is affected by distribution. Lipophilic drugs may appear to remain longer in the %BioGears plasma in patients who are configured with a higher body fat fraction. Patient configuration will also impact the pharmacodynamics. Maximum drug effects are computed as a fraction of baseline physiology. Changing a patient's baseline parameter (e.g. heart rate baseline) will also change the maximum effect (e.g. maximum heart rate) at a specific concentration of a drug. All drug validation is performed on a %BioGears Standard Male.

@anchor drugs-dependencies
### Dependencies
The substance calculations rely on the flow values calculated by the @ref CircuitMethodology to calculate the movement of substances from compartment to compartment in each circuit. Any limitations and assumptions associated with this methodology will also be associated with the %Drugs Methodology. The pharmocokinetic modeling is reliant on accurate values for the physical chemical properties of drugs. If these parameters are unknown or estimated from animal studies, the calculation for the partition coefficient may become unreliable.  For some drugs, this information may be difficult to determine. The %BioGears model also allows the user to supply partition coefficients directly. Some third party resources, both open source and commercially available, may provide this data for users, allowing incorporation of the data into the %BioGears full body simulation.

While this model is primarily used for drugs, it could be used for any substance in the body. %BioGears uses the clearance methodology for several additional substances. The modular approach to these calculations allows for the use of any combination of the three methodologies, PK, clearance, and PD.

The substance values and calculations are used by a variety of systems to trigger actions and responses, define physiologic set points, scale circuit parameters, and modify driver frequencies. Many of these triggers and responses are discussed in the @ref BloodChemistryMethodology and the @ref EndocrineMethodology. The drug-specific effects are listed and described briefly in Table&nbsp;5. The implementation of these calculated responses can be found in the @ref CardiovascularMethodology and the @ref RespiratoryMethodology.

@anchor drugs-assumptions
### Assumptions and Limitations
The %Drugs Methodology is based on the assumption that all substances are suspended in the fluid (blood or air, depending on the system) and move through the circuits with the flow. While this does allow for renal and hepatic clearance and metabolic elimination of substances, it does not allow for particle deposition. This assumption is valid for the level of fidelity of the model. In the future, particle deposition can be addressed by adding a more detailed substance methodology with factors for deposition within an organ, tissue, or blood vessel. Also, not included is a method for modeling vaporized substances, such as albuterol.  In the future, we will add this methodology to allow a liquid in the respiratory tract and produce a more realistic representation of the behavior.

The drug model addresses many of the earlier limitation of the %BioGears drug model. This includes creating a substance file that uses physiochemical properties to define the pharmacokinetic properties and intuitive modifiers to calculate the pharmacodynamic effects. However, this implementation of a PD model works best with a drug with immediate effects, such as anesthesia medications. This implementation has not been tested for drugs with a later onset due to secondary mechanisms. This is an area for future work. Additionally, no drug interactions or intoxication is modeled.

@anchor drugs-Actions
Actions
-------

@anchor drugs-interventions
### Interventions

#### Bolus Administration

A bolus administration represents an injection of a substance in a single dose. The substance, concentration, and amount are specified by the user. The total mass for the dose is calculated by multiplying the concentration and the amount. The total mass is then incrementally added to the vena cava compartment over the course of two seconds to represent the slow depression of a plunger. Once added to the vena cava, the substance will circulate through the %Cardiovascular System as part of the blood flow. The effects of the substance on the physiologic output will vary with the substance.

#### IV Administration

Intravenous administration represents administration of a compound substance (a single entity with multiple substances, i.e., saline) at a specified rate over time into the venous system. The compound substance, the rate, and the bag volume are specified by the user. At each time step, the mass of the substances are incremented based on the amount of compound substance entering the system. The mass is calculated by taking the flow entering the system at each time step multiplied by the known concentration of each substance within the compound substance. This mass is then added to the vena cava at each time step until the intravenous administration is stopped. The added substances will then circulate through the %Cardiovascular System as part of the blood flow. The effect on the physiologic output will vary based on the compound substance selected.

#### IV Infusion

Titration represents the administration of a single substance at a specified rate and concentration over time into the venous system. The user selects a single substance and specifies the concentration and rate with titration. As with intravenous administration, the mass of the substance is incremented based on the amount of the substance entering the system. The mass is calculated by taking the flow entering the system at each time step multiplied by the known concentration of each substance within the compound substance. This mass is then added to the vena cava at each time step until the intravenous administration is stopped. The added substances then circulate through the %Cardiovascular System as part of the blood flow. The effect on the physiologic output will vary based on the compound substance selected.

@anchor drugs-results
Results and Conclusions
=======================

Validation - Resting Physiologic State
--------------------------------------

The %Drugs Methodology has no resting state validation.

@anchor drugs-validation-actions
Validation - Actions
--------------------

### Bolus Administration

All drugs in the %BioGears library were validated qualitatively or with subject matter expert input. The bolus injection methodology was validated by comparing the PK and PD results to literature. See the sections below for more detail. 

The Succinylcholine scenario shows a drug impacting the major %BioGears systems. Succinylcholine (Figure&nbsp;2) initially drops the heart rate and blood pressure approximately 5&ndash;10%. As expected, the respiration drops to zero within 60&ndash;90 seconds to represent the paralysis associated with the neuromuscular blocker. The oxygen saturation drops slowly as the concentration of oxygen in the bloodstream drops, leading to hypoxia. Epinephrine is released in response to hypoxia, causing an increase in heart rate and blood pressure (@ref EndocrineMethodology). This is an example of the interdependent nature of the %BioGears systems and the chain reaction drug administration can cause.

<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Succinylcholine_HeartRate.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Succinylcholine_MeanArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Succinylcholine_RespirationRate.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Succinylcholine_OxygenSaturation.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Drugs/SuccinylcholineLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center>
<i>Figure 2. The neuromuscular blocker, Succinylcholine, causes a cessation of respiration due to its paralytic effects. The heart rate and blood pressure initially decrease due to the system-level drug effects. As the oxygen decreases,  hypoxia occurs, resulting in an increase in heart rate and blood pressure.</i>
</center><br>

#### Intravenous Administration
Validation of intravenous administration of a hypotonic solution and blood products can be found in the @ref CardiovascularMethodology.

@anchor drugs-validation-pharmacokinetic
Validation - Pharmacokinetic
--------------------

The PK model was validated by comparing the plasma concentration calculated by the %BioGears Engine to plasma concentration curves found in the literature. The comparisons are shown in Figures 3&ndash;17. No available data was found for Epinephrine, so this drug was only validated from a PD perspective.

<img src="./plots/Drugs/Albuterol.jpg" width="1100">
<center>
<i>Figure 3. Albuterol is an aerosol, but it is modeled as a gas in %BioGears. Alveolar exchange takes place bidirectionally as long as the "gas" (aerosol) is in the blood or in the environment. For that reason, the concentration of Albuterol in the plasma diminishes much faster in %BioGears than demonstrated in literature @cite anderson1998pharmacokinetics. This is a known issue and an area of future work for %BioGears.</i>
</center><br>

<img src="./plots/Drugs/Desflurane.jpg" width="1100">
<center>
<i>Figure 4. The kinetics of inhaled anesthetics like desflurane are generally described by the end tidal fraction of the anesthetic @cite flood2015stoelting.  Validation data assumes an inspired concentration of 5% desflurane administered for 30 minutes @cite lu2004pharmacokinetics followed by a two hour wash out time @cite yasuda1991comparison.  %BioGears accurately captures the speed of wash-in but underestimates the speed with which desflurane washes out.</i>
</center><br>

<img src="./plots/Drugs/Fentanyl.jpg" width="1100">
<center>
<i>Figure 5. The physical chemical properties of fentanyl found in the literature were used to produce a good fit for the plasma concentration. The experimental data was found in @cite shankaran2013physiologically.</i>
</center><br>

<img src="./plots/Drugs/Furosemide.jpg" width="1100">
<center>
<i>Figure 6. The physical chemical properties of furosemide found in the literature were used to produce a good fit for the plasma concentration. The experimental data was found in @cite hammarlund1985acute.</i>
</center><br>

<img src="./plots/Drugs/Ketamine.jpg" width="1100">
<center>
<i>Figure 7. The partition coefficients were calculated from initial physical chemical properties found for Ketamine. However, the fit was unsatisfactory, so the partition coefficients were directly estimated for this drug to produce a good fit with the plasma concentration curve found in @cite xie2003analgesic.</i>
</center><br>

<img src="./plots/Drugs/Midazolam.jpg" width="1100">
<center>
<i>Figure 8. The %BioGears curve for midazolam plasma concentration shows a good overall fit with the experimental data found in @cite schwagmeier1998midazolam. However, the clearance dominant (end) portion of the curve shows that %BioGears should be clearing more quickly, while midazolam appears to move into the tissues too quickly during the diffusion dominant portion of the curve (beginning).</i>
</center><br>

<img src="./plots/Drugs/Morphine.jpg" width="1100">
<center>
<i>Figure 9. The partition coefficients for morphine estimated in @cite edginton2006development were used in lieu of those estimated using physiochemical data.  Doing so provided a better fit to the experimental data found in @cite lotsch2002pharmacokinetic.</i>
</center><br>

<img src="./plots/Drugs/Naloxone.jpg" width="1100">
<center>
<i>Figure 10.  The partition coefficients were calculated from initial physical chemical properties found for Naloxone. They produce a good fit with the plasma concentration curve found in @cite aitkenhead1984pharmacokinetics.</i>
</center><br>

<img src="./plots/Drugs/Pralidoxime.jpg" width="1100">
<center>
<i>Figure 11. The pralidoxime experimental data found in @cite abbara2010pharmacokinetic was collected after an intramuscular injection. This administration method is available in %BioGears, so was used to validate the drug. The overall fit between the %BioGears data and the experimental data was good. A slightly higher concentration throughout was evident and may be attributed to the amount of drug that never exits the muscle when this type of injection occurs. In the future, a loss factor could be added to represent this drug amount.</i>
</center><br>

<img src="./plots/Drugs/Prednisone.jpg" width="1100">
<center>
<i>Figure 12.  The physical chemical properties of prednisone found in the literature were used to produce a good fit for the plasma concentration. The experimental data was found in @cite rose1981dose.</i>
</center><br>

<img src="./plots/Drugs/Propofol.jpg" width="1100">
<center>
<i>Figure 13. The propofol experimental data was collected during and after an infusion of propofol. %BioGears is able to support a rate and concentration input for an infusion; therefore, the infusion methodology was used to adminster propofol in the engine. The infusion explains the initial curve evident in the plasma concentration. The initial diffusion dominant portion of the curve shows a reduced diffusion into the tissues compared to the experimental data; however, the clearance dominant portion of the curve shows good agreement with the data found in @cite ward2002pharmacodynamics.</i>
</center><br>

<img src="./plots/Drugs/Rocuronium.jpg" width="1100">
<center>
<i>Figure 14. The %BioGears calculated plasma concentration curve for Rocuronium shows good agreement with the experimental data found in @cite de2014analysis. This is particularly true for the diffusion dominant portion of the curve, while the %BioGears clearance is less than that shown in the experimental data.</i>
</center><br>

<img src="./plots/Drugs/Sarin.jpg" width="1100">
<center>
<i>Figure 15. Pharmacokinetic data for Sarin inhalation is non-existent with respect to humans and in short supply with respect to animals.  Most animal studies administer Sarin either orally or intravenously.  The data above was obtained from a study in which guinea pigs inhaled a potentially lethal dose of Soman (another organophosphate nerve agent) @cite gupta2009handbook.  The %BioGears simulation assumes a 6-minute exposure to Sarin aerosol at a concentration of 15 mg/m<sup>3</sup>.  These results indicate reasonable uptake and diffusion of Sarin via the lungs.</i>
</center><br>

<img src="./plots/Drugs/Succinylcholine.jpg" width="1100">
<center>
<i>Figure 16. The succinylcholine pharmacokinetics were initially calculated from the values found in literature for the physical chemical properties. However, this was significantly adjusted to attempt to match the calculated plasma concentration to the experimental data found in @cite roy2002concentration.</i>
</center><br>

<img src="./plots/Drugs/TranexamicAcid.jpg" width="1100">
<center>
<i>Figure 17. The succinylcholine pharmacokinetics were initially calculated from the values found in literature for the physical chemical properties. However, this was significantly adjusted to attempt to match the calculated plasma concentration to the experimental data found in @cite roy2002concentration.</i>
</center><br>

<img src="./plots/Drugs/Vasopressin.jpg" width="1100">
<center>
<i>Figure 18. Vasopressin is naturally produced in the body, but the dose given here is much greater than basal levels so this should not be a confounding factor.  The predicted values are generally in agreement with the data reported in @cite glanzer1982hemodynamic</i>
</center><br>

The above results show that, in general, the model chosen for %BioGears produces good agreement between experimental and calculated data. However, it also reveals that for some drugs, this model does not appear to be valid. Future work will investigate the incorporation of vaporized substance methodologies, and additional partition coefficient calculations that may prove more accurate for specific drug types. Further statistical analysis will also be undertaken to assess the model performance rather than solely relying on a qualitative analysis.

@anchor drugs-validation-pharmacodynamic
Validation - Pharmacodynamic
--------------------

The pharmacodynamic effects of the drugs were validated by comparing the effects of each drug to a number of clinical parameters. A summary of the drug validation is shown in Table&nbsp;5. More details on the individual scenario validation can be found in the <a href="DrugsValidation.pdf">Drugs Scenario Validation Matrix</a>.

<center>
<i>Table 5. The %BioGears drug library was validated qualitatively with published values and using subject matter expertise. Green indicates good agreement with validation data, yellow indicates agreement with a general trend with some minor disagreement, and red indicates a disagreement with the validation data. The number indicates the number of output parameters for each category of validation success or failure.</i>
</center>

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	Albuterol	|	Administer 90 ug metered dose of Albuterol	|<span class="success">	6	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Desflurane	|	Administer Desflurane 6% in air	|<span class="success">	4	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Epinephrine	|	Administer 10 ug injection of Epinephrine	|<span class="success">	4	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Fentanyl	|	Administer  300 ug injection of Fentanyl	|<span class="success">	5	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Furosemide	|	Administer 40 mg injection of Furosemide	|<span class="success">	3	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Ketamine	|	Administer 115 mg injection of Ketamine	|<span class="success">	4	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Midazolam	|	Administer 5 mg injection of Midazolam	|<span class="success">	5	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Morphine	|	Administer 2.75 mg/min IV Morphine for 4 minutes	|<span class="success">	6	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Morphine Overdose	|	Administer 8.2 mg/min IV Morphine for 24 minutes	|<span class="success">	3	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Naloxone	|	Administer 300 ug injection of Fentanyl followed by 200 ug injection of Naloxone *Validating only Naloxone	|<span class="success">	5	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Prednisone	|	Administer 20 mg injection of Prednisone	|<span class="success">	5	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Propofol	|	Administer 135 mg injection of Propofol	|<span class="success">	4	</span>|<span class="warning">	0	</span>|<span class="danger">	1	</span>|
|	Rocuronium	|	Administer 52 mg injection of Rocuronium	|<span class="success">	3	</span>|<span class="warning">	0	</span>|<span class="danger">	2	</span>|
|	Sarin	|	Exposure to five concentrations in environment	|<span class="success">	21	</span>|<span class="warning">	4	</span>|<span class="danger">	0	</span>|
|	Succinylcholine	|	Administer 96 mg injection of Succinylcholine	|<span class="success">	3	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	Tranexamic Acid	|	Administer 200 mg injection of TXA	|<span class="success">	8	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Vasopressin	|	Administer 0.04, 0.18, and 0.25 ug	|<span class="success">	3	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|		|	Total	|<span class="success">	92	</span>|<span class="warning">	16	</span>|<span class="danger">	3	</span>|

<center>
<i>Table 6. The validation results for liquid-modeled drugs.</i>
</center>

|	Event	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate  (beats/min)	|	Systolic Pressure  (mmHg)	|	Diastolic Pressure  (mmHg)	|	Respiration Rate  (breaths/min)	|	Oxygen Saturation	|	Hemorrhage Flow (mL/s)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Epinephrine - 10ug	|	Drug Onset < 1 minute	|	30	|	30	|<span class="success">	25-50% Increase @cite dukeSME	</span>|<span class="warning">	25-50% Increase @cite dukeSME 	</span>|<span class="success">	25-50% Increase @cite dukeSME 	</span>|<span class="success">	NC @cite dukeSME	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|
|	Administer Fentanyl - 300 ug	|	Drug Onset  2-4 minutes	|	30	|	600	|<span class="success">	Stable @cite Morgan2006Clinical p192-7, @cite PaulGBarash2009 p277; 5-10% Decrease @cite dukeSME  	</span>|<span class="success">	Stable @cite Morgan2006Clinical p192-7, @cite PaulGBarash2009 p277; 5-10% Decrease @cite dukeSME  	</span>|<span class="success">	Stable @cite Morgan2006Clinical p192-7, @cite PaulGBarash2009 p277; 5-10% Decrease @cite dukeSME  	</span>|<span class="success">	Decrease ˜ 50% @cite mildh2001concentration, @cite yassen2007mechanism	</span>|<span class="success">	Decrease below 95% @cite mildh2001concentration	</span>|<span class="success">	NC 	</span>|
|	Administer Ketamine - 115 mg	|	Drug Onset < 1 minute	|	30	|	300	|<span class="success">	Moderate Increase @cite Morgan2006Clinical p200; 15-25% Increase @cite dukeSME   	</span>|<span class="warning">	Moderate Increase @cite Morgan2006Clinical p200; 15-25% Increase @cite dukeSME   	</span>|<span class="success">	Moderate Increase @cite Morgan2006Clinical p200; 15-25% Increase @cite dukeSME   	</span>|<span class="success">	Mild Decrease @cite Morgan2006Clinical p200; 25-50% Decrease @cite dukeSME     	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|
|	Administer Midazolam - 5 mg	|	Drug Onset in 1-3 min and lasts 30-60 minutes	|	30	|	70	|<span class="success">	Mild Increase @cite Morgan2006Clinical p200; 5-10% Decrease @cite dukeSME	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical p200; 15-25% Decrease @cite dukeSME    	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical p200; 15-25% Decrease @cite dukeSME    	</span>|<span class="success">	Moderate Decrease @cite Morgan2006Clinical p200; Marked Decrease To 0 @cite dukeSME    	</span>|<span class="success">	As long as some respiration rate, little change. If RR=0, then begins to drop along O2 curve. 	</span>|<span class="success">	NC 	</span>|
|	Administer Morphine - 2.75 mg/min	|	Constant rate of infusion for 4 minutes (11 mg total)	|	30-270	|	5000	|<span class="success">	Decrease 4-10% @cite thompson1995respiratory	</span>|<span class="success">	Minimal Decrease @cite thompson1995respiratory or NC @cite goodman1996goodman	</span>|<span class="success">	Minimal Decrease @cite thompson1995respiratory or NC @cite goodman1996goodman	</span>|<span class="success">	Decrease 25-30% @cite olofsen2010naloxone	</span>|<span class="success">	Remain above 95% @cite shapiro2005frequency	</span>|<span class="success">	NC 	</span>|
|	Administer Naloxone - 200 ug -  in response to Fentanyl	|	Naloxone is modeled as a competitive inhibitor of opioids; it exhibits minimal PD effects alone	|	30 = 300 ug Fentanyl; 300 = 200 ug Naloxone	|	1000	|<span class="success">	Return to resting state @cite goodman1996goodman	</span>|<span class="success">	Return to resting state @cite goodman1996goodman	</span>|<span class="success">	Return to resting state @cite goodman1996goodman	</span>|<span class="success">	Return to resting baseline @cite goodman1996goodman	</span>|<span class="success">	Increase  	</span>|<span class="success">	NC 	</span>|
|	Administer Prednisone - 20 mg	|	Drug Onset in less than 1 minute	|	30	|	100	|<span class="success">	NC @cite safety  	</span>|<span class="success">	NC @cite safety  	</span>|<span class="success">	NC @cite safety  	</span>|<span class="success">	NC @cite safety  	</span>|<span class="success">	NC @cite safety  	</span>|<span class="success">	NC 	</span>|
|	Administer Propofol - 200 mg	|	Drug Onset in less than 1 minute	|	30	|	70	|<span class="danger">	NC @cite Morgan2006Clinical p200; NC @cite dukeSME     	</span>|<span class="success">	Marked Decrease @cite Morgan2006Clinical p200; 25-40% Decrease @cite dukeSME    	</span>|<span class="success">	Marked Decrease @cite Morgan2006Clinical p200; 25-40% Decrease @cite dukeSME    	</span>|<span class="success">	Marked Decrease @cite Morgan2006Clinical p200; 100% Decrease @cite dukeSME    	</span>|<span class="success">	Begins to drop according to O2 curve 	</span>|<span class="success">	NC 	</span>|
|	Administer Rocuronium - 50 mg	|	Drug Onset in 60-90 seconds	|	30	|	100	|<span class="danger">	NC @cite PaulGBarash2009 p299; NC @cite dukeSME     	</span>|<span class="success">	NC @cite PaulGBarash2009 p299; NC @cite dukeSME     	</span>|<span class="danger">	NC @cite PaulGBarash2009 p299; NC @cite dukeSME     	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical p224; Goes to Zero @cite dukeSME     	</span>|<span class="success">	Begins to drop according to O2 curve 	</span>|<span class="success">	NC 	</span>|
|	Administer Succinycholine - 77 mg	|	Drug Onset < 1 minute	|	30	|	70	|<span class="warning">	Mild  Increase @cite Morgan2006Clinical pp210-5; 5-10% Decrease @cite dukeSME    	</span>|<span class="warning">	Mild  Increase @cite Morgan2006Clinical pp210-5; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Mild  Increase @cite Morgan2006Clinical pp210-5; 5-10% Decrease @cite dukeSME    	</span>|<span class="success">	Goes to Zero @cite Morgan2006Clinical p224; Goes to Zero @cite dukeSME     	</span>|<span class="success">	Begins to drop according to O2 curve 	</span>|<span class="success">	NC 	</span>|
|	Administer TXA -- 200 mg	|	Infusion over 15 mins	|	30	|	30	|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|<span class="success">	NC 	</span>|<span class="success">	Decrease 29-55% </span>|
|	Administer Vasopressin -- 0.04, 0.18, and 0.25 ug	|	Infusions are successive, each lasts 20 minutes	|	30, 1230, 2430	|	2430	|<span class="success">	Decrease ˜ 25% @cite mohring1980greatly	</span>|<span class="warning">	Increase 7-8 mmHg @cite mohring1980greatly	</span>|<span class="warning">	Increase 7-8 mmHg @cite mohring1980greatly	</span>|<span class="success">	NC @cite mohring1980greatly	</span>|<span class="success">	NC @cite mohring1980greatly	</span>|<span class="success">	NC 	</span>|




<center>
<i>Table 7. The validation results for gas-modeled drugs.</i>
</center>

|	Event	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate (beats/min)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|	Oxygen Saturation	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Albuterol - 10ug	|	Drug Onset < 1 minute	|	30	|	200	|<span class="success">	25-50% Increase @cite gumbhir1998pharmacokinetic 	</span>|<span class="warning">	Slight Increase @cite gumbhir1998pharmacokinetic 	</span>|<span class="success">	Slight Increase @cite gumbhir1998pharmacokinetic 	</span>|<span class="warning">	NC @cite gumbhir1998pharmacokinetic	</span>|<span class="success">	NC @cite gumbhir1998pharmacokinetic	</span>|<span class="success">	NC 	</span>|


<center>
<i>Table 8. The validation results for furosemide.</i>
</center>

|	Event	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (min)	|	Urine Production Rate (mL/min)	|	Chloride Excretion (mmol/min)	|	Sodium Excretion (mol/min)	|	Vascular Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Furosemide - 40 mg	|	Drug Onset < 1 minute	|	30	|	450	|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="success">	300-400% Increase @cite lahav1992intermittent 	</span>|<span class="warning">	Decrease (around 500-1000 mL) @cite hammarlund1985acute	</span>|

<br>

@anchor drugs-furosemide
### Diuretic-Furosemide 
PD effects for the diuretic Furosemide are handled differently than other drugs in the %BioGears patient. Localized effects are implemented via a tubular permeability modifier. This effectively simulates Furosemide's effect on the tubular luminal Na-K-Cl co-transporter, inhibiting reabsorption of fluid and ions into the vasculature, as shown in Figure 19. The permeability modifier is a function of drug concentration in the blood plasma. These concentrations determine appropriate tubular reabsorption inhibition by reducing the permeability of the tubular lumen (for more information, see @ref RenalMethodology). This effectively increases the urine production rate and ion excretion, and reduces the patient's total blood volume. To see the effects of the drug on the cardiovascular system during ventricular systolic dysfunction, see @ref cardiovascular-validation-conditions "Cardiovascular System".

<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Furosemide_BloodVolume.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Furosemide_ChlorideClearance.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Furosemide_SodiumClearance.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Furosemide_UrineProductionRate.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Drugs/FurosemideLegend.jpg" width="500"></td>
</tr>
</table>
</center>
<center>
<i>Figure 19. The diuretic furosemide causes an inhibition of reabsorption due to its effect on the tubular lumen co-transporter. Total blood volume decreases as urine production and ion excretion increase. Eventually, as the body clears the drug, urine production and ion excretion both begin to return to normal.</i>
</center><br>


@anchor drug-desflurane
###Volatile Anesthetic-Desflurane


<center>
<i>Table 9. Data was obtained from studies in which healthy volunteers breathed desflurane spontaneously without any other anesthetic agent (i.e. nitrous oxide, propofol, etc.) @cite weiskopf1991cardiovascular @cite lockhart1991depression.</i>
</center>

|	Event	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Heart Rate  (beats/min)	|	Mean Arterial Pressure  (mmHg)	|	Systemic Vascular Resistance  (mmHg-s/mL)	|	Cardiac Output (L/min)	|	Respiration Rate (breaths/min)	|	Tidal Volume (mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer-Desflurane 6%	|	Drug Onset < 1 minute	|	80	|	1000	|<span class="success">	Approx. 20% increase @cite weiskopf1991cardiovascular p169	</span>|<span class="warning">	Approx. 34% decrease @cite weiskopf1991cardiovascular p169	</span>|<span class="success">	Approx. 54% decrease @cite weiskopf1991cardiovascular p169	</span>|<span class="warning">	Approx. 30% increase @cite weiskopf1991cardiovascular p168	</span>|<span class="success">	Approx. 60% increase @cite lockhart1991depression p485	</span>|<span class="success">	Approx. 35% decrease @cite lockhart1991depression p485	</span>|

<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Desflurane_SystemicVascularResistance.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Desflurane_MeanArterialPressure.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Desflurane_HeartRate.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Desflurane_CardiacOutput.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Desflurane_TidalVolume.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Desflurane_RespirationRate.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Drugs/Desflurane_Legend.jpg" width="400"></td>
</tr>
</table>
</center>
<center>
<i>Figure 20.  The anesthetic desflurane suppresses mean arterial blood pressure by decreasing systemic vascular resistance.  Heart rate increases and, in the case of spontaneously breathing patients @cite weiskopf1991cardiovascular, so too does cardiac output (though not to the degree predicted).  Desflurane also impacts the respiratory system by increasing respiration rate and decreasing tidal volume.  </i>
</center><br>

@anchor advanced-models
##Advanced Models
As we work to continuously improve the %BioGears %Drugs System, we have introduced models that incorporate more advanced features such as Overdose, Drug-Drug Interactions, and Target/Receptor Occupancy.  These new capabilities are currently available with respect to the opioids morphine and fentanyl and to the nerve agent Sarin.  We plan to generalize these advanced components to the remaining %drugs in our library with the goal of increasing overall model accuracy while maintaining flexibility.    

###Opioids
@anchor opioids
####Morphine Intoxication
Morphine is the first drug in the %BioGears library with parameters tuned to model overdose scenarios. The %BioGears overdose scenario is modeled after a case report in which a patient received 200 mg of intravenous Morphine over 24 minutes from a defective patient controlled analgesic (PCA) device and experienced severe opioid ventilatory impairment (OIVI) @cite doyle2008another.  Morphine PD parameters were tuned to produce symptoms consistent with OIVI (respiratory depression, hypercapnia, and hypoxia) using both this report and other supporting literature (see Table 10).  Figure 21 compares the severe overdose scenario to the standard Morphine dose.  Note the prolonged nature of the pharmacological effects, which is a function of Morphine's low effect site rate constant (k<sub>e</sub> = 0.0026 1/min). In this scenario, the patient does not enter an irreversible state (though one will be triggered at higher doses), making our model somewhat conservative.  We took such a conservative approach because opioid sensitivity is extremely variable among patients, particularly between men and women @cite overdyk2014opioid.  It should also be mentioned that the tidal volume predicted by our model (not shown) erroneously demonstrates a substantial increase.  This is a known issue resulting from the manner in which tidal volume and respiration rate are coupled in the current %BioGears respiratory system (see @ref RespiratoryMethodology) and will be an area of future work.   
 
<center>
<i>Table 10. Comparison of the %BioGears response to a standard dose of Morphine (@cite thompson1995respiratory) and an overdose of Morphine (@cite doyle2008another).  The overdose produces symptoms consistent with opioid induced ventilatory impairment (OIVI) and bradycardia.</i>
</center>


|	Event	|	Notes	|	Action Occurrence Time (s)	|	Sample Scenario Time (s)	|	Respiration Rate (breaths/min)	|	Arterial Carbon Dioxide Pressure (mmHg)	|	Oxygen Saturation (%)	|	Heart Rate (bpm)	|	Blood Pressure (mmg)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Administer Morphine-2.75 mg/min	|	Infused at constant rate over 4 minutes (11 mg total)	|	30-270	|	5000	|<span class="success">	Decrease 25-30% @cite olofsen2010naloxone	</span>|<span class="success">	Increase < 4 mmHg @cite thompson1995respiratory	</span>|<span class="success">	Remain above 95% @cite shapiro2005frequency	</span>|<span class="success">	Decrease 4-10% @cite thompson1995respiratory	</span>|<span class="success">	Minimal Decrease @cite thompson1995respiratory or NC @cite goodman1996goodman	</span>|
|	Administer Morphine-8.2 mg/min	|	Infused at constant rate over 24 minutes (~197 mg total)	|	30-1470	|	7200	|<span class="success">	<=6 @cite goodman1996goodman, @cite landais2014severe	</span>|<span class="success">	>55 mmHg  @cite macintyre2011opioids	</span>|<span class="warning">	<85% for six minutes @cite dahan2010incidence	</span>|<span class="success">	<60 @cite noble2014opioid	</span>|<span class="warning">	Hypotension (SBP < 90 and DBP < 60) @cite noble2014opioid	</span>|



<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Morphine_RespirationRate.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Morphine_OxygenSaturation.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Morphine_ArterialCarbonDioxidePartialPressure.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Morphine_HeartRate.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Drugs/MorphineOverdose_EventTracker.jpg" width="1100"></td>
</tr>
</table>
</center>
<i>Figure 21.  The Morphine overdose scenario shows the difference in effect betweeen a standard dose (red) an overdose (blue).  The timeline below the plots shows the collection of respiratory events thrown in %BioGears such as respiratory distress, hypoxia, and hypercapnia.</i>
</center><br>

####Reversal of Opioid Induced %Respiratory Depression
Severe respiratory depression resulting from opioid usage is commonly treated with naloxone.  Naloxone competes with opioids at mu receptors in the central nervous system and, when bound, exerts essentially no pharmacodynamic effects @cite goodman1996goodman.  Thus, as more opioids are displaced, symptoms progressively improve.  This competitive interaction can be modeled using Equation 10.  The inhibition constant (K<sub>I</sub>) for Naloxone was estimated in two stages (see Figure 22) from data published in @cite olofsen2010naloxone regarding the combined effects of Morphine and Naloxone on respiration.  First, the EC<sub>50</sub> for Morphine was estimated from human volunteers who were administered Morphine followed by placebo.  This experimental EC<sub>50</sub> was then used to fit Equation 10 to data collected from individuals who were given Morphine followed by Naloxone and determine the best value for K<sub>I</sub>.  Since the EC<sub>50</sub> for Morphine utilized by %BioGears is not equivalent to the one determined from this study (this is due to the fact that %BioGears is tuned for multiple pharmacodynamic responses and not just breathing rate), the ratio of the estimated EC<sub>50</sub> to K<sub>I</sub> was used to determine the %BioGears K<sub>I</sub>.  It was assumed that this K<sub>I</sub> is appropriate for the interaction of Naloxone and Fentanyl, which seems appropriate given that Fentanyl acts predominantly on the same receptor as Morphine.  

<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/MorphineWithPlaceboFit.jpg" width="550"></td>
    <td><img src="./plots/Drugs/MorphineWithNaloxoneFit.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 22.  Non-linear least squares regression after estimation of Morphine EC<sub>50</sub> and Naloxone K<sub>I</sub>.  Experimental data was obtained from @cite olofsen2010naloxone in which patients were administered 0.15mg/kg intravenously followed by a placebo (left) or 400 ug of intravenous Naloxone (right).</i>
</center><br>


Figures 23-24 demonstrate the effect of administering naloxone after respiratory depression induced by Fentanyl and Morphine.  Note that while the effects of Fentanyl are completely reversed after a standard Naloxone dose (115 ug @cite olofsen2010naloxone), depression of respiratory activity persists in the case of morphine (renarcotization).  In the case of massive overdose, the degree of renarcotization necessitates a second dose of naloxone.  This result is due to the fact that Naloxone has a longer elimination half-life than Fentanyl but a significantly shorter one than Morphine (see Table 6 for Naloxone validation).

<img src="./plots/Drugs/Naloxone_FentanylStandard.jpg" width="550">
<center>
<i>Figure 23. Naloxone reversal of respiratory depression induced by Fentanyl.  Fentanyl protocol and pharmacodynamic parameters were obtained from @cite yassen2007mechanism.  Green = Infuse 300 ug of Fentanyl; Black = End of Infusion; Magenta = Administer 200 ug of IV naloxone.</i>
</center><br>


<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Naloxone_MorphineStandard.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Naloxone_MorphineOverdose.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>Figure 24.  Naloxone reversal of minor respiratory depression induced by standard dose of morphine (left) and major respiratory depression induced by morphine overdose (right).  Left Figure:  Green = Administer 10.5 mg bolus of intravenous morphine; Black = Adminster 200 ug of Naloxone.  Right Figure:  Green = Begin intravenous morphine infusion of 8 mg/min; Black = End of infusion; Magenta = Administer 800 ug of intravenous Naloxone; Yellow = Adminster 400 ug of intravenous Naloxone.</i>
</center><br>

@anchor sarin-nerve
###Nerve Gas:  Sarin

The Chemical Weapons Convention (CWC) was initiated in 1993 to ban the synthesis of chemical warfare agents and outline a plan for the destruction of existing chemical stockpiles @romano2008chemical.  Though certainly a positive development, chemical weapons, particularly nerve gases, remain of considerable interest due to their potential use as agents of terror.  Perhaps the most well-known instance of chemical terrorism is the 1995 attack perpetrated by the extremist group Aum Shinrikyo on the Tokyo subway using the nerve agent Sarin @cite gupta2009handbook.  That same compound, Sarin, has been more recently implicated in chemical attacks in Syria (a relatively new member to the CWC) in 2013 and 2017 @cite davenport2017sarin.  We thus have selected Sarin as our model nerve agent. 

Sarin belongs to a class of chemicals called organophosphates which exert their physiological effects by inhibiting the activity of the enzyme acetylcholinesterase.  Acetylcholinesterase (AChE) catalyzes the breakdown of the neurotransmitter acetylcholine (ACh) in the peripheral and central nervous systems @cite black1996chemistry.  In the absence of AChE, ACh remains in high concentrations in synaptic junctions and continues to stimulate nerve signal transduction.  Neural overstimulation leads to hyperactivity at receptors associated with muscle tissue, secretory glands, and the respiratory center (though seemingly disparate symptoms can occur as a result of ACh’s role in both the sympathetic and parasympathetic nervous responses) @cite goodman1996goodman.  Prolonged exposure leads to death induced by central respiratory failure and paralysis of diaphragm muscles @cite black1996chemistry. 

To compound matters, the bound Sarin-AChE complex does not spontaneously reverse; rather, it undergoes a subsequent reaction—called “aging”—which permanently deactivates AChE @cite delfino2009organophosphorus.  The aging process is the target of efforts to identify organophosphate-induced toxicity reversal agents.  If the initial binding of Sarin to AChE can be reversed prior to aging, the functionality of the enzyme can be salvaged and the Sarin eliminated from the body.  One class of compounds that has received close attention is the oximes, of which Pralidoxime (2-PAM) is the most frequently cited @cite luo2007vitro.  As Pralidoxime is also in the %BioGears library, we easily incorporate it in our model.

Since it is so toxic at low concentrations, the generic Emax model applied to other %BioGears drugs is not readily extendable to Sarin.  We instead model the interaction of Sarin with acetylcholinesterase directly.  In particular, we describe the binding of Sarin to red blood cell acetylcholinesterase (RBC-AChE).  Erythrocyte-bound AChE plays no role in the nervous system and is instead thought to act as a buffer for anti-acetylcholinesterase agents @cite gupta2009handbook.  However, RBC-AChE is easier to assay than neuromuscular AChE and the relative activity of RBC-AChE has been reported to be moderately correlated with organophosphate symptomology @cite voicu2010toxicokinetics.  Thus, we can feed the fraction of RBC-AChE inhibited to our pharmacodynamics model.  It should be noted that, in practice, high variability in RBC-AChE activity in humans limits the effectiveness of this approach from a diagnostic viewpoint.  Symptoms can appear without apparent RBC-AChE inhibition and 100% inhibition can occur with only mild symptoms @cite voicu2010toxicokinetics.  However, such a caveat is not as concerning from a pure modeling standpoint.

The kinetic equations governing Sarin inhibition of AChE are given below with the following conventions:  S = Sarin, A = acetylcholinesterase (erythrocyte bound), SA = sarin-acetylcholinesterase complex, X = aged (permanently deactivated) AChE, and P = Pralidoxime.  Note that Sarin and Pralidoxime clearances are not stated because they are handled by existing %BioGears functionality.  Also note that Pralidoxime interaction with the Sarin-AChE complex follows Michaelis-Menten kinetics and that the resulting Sarin-Pralidoxime complex is not explicitly tracked.  Initial concentrations of all species except RBC-AChE are assumed to be 0.  According to the literature, an initial RBC-AChE of 8.0 nM can be expected @cite gupta2009handbook.  Rate constants were obtained from @cite gupta2009handbook, @cite rodriguez2015model, and @cite grob1958effects.  The constants *k<sub>syn</sub>* and *k<sub>deg</sub>*, which respectively represent basal synthesis and degradation of RBC-AChE, do not greatly impact the model but are included for completeness.  Sarin is assumed to reach the bloodstream via the lungs after inhalation (the most likely route of exposure given the high volatility of Sarin) @cite romano2007chemical.  The particle-size histogram of aerosolized Sarin was adjusted to achieve a plasma concentration consistent with those observed in animal studies (for more on the methodology of inhaled particles in %BioGears, see the [Aerosol Section](@ref environment-aerosol)).  Sarin is introduced to the simulation via an %Environment Change (see [Environment Actions](@ref environment-actions)).

\f[\frac{d[S]}{dt} = -k_{S}*[S][A] \f]
<center>
<i>Equation 13.</i>
</center><br>
\f[\frac{d[A]}{dt} = -k_{S}*[S][A]-k_{deg}[A]+k_{syn}+\frac{k_{r}[SA][P]}{K_{D}+[P]} \f]
<center>
<i>Equation 14.</i>
</center><br>
\f[\frac{d[SA]}{dt} = k_{S}*[S][A]-k_{age}[SA]-\frac{k_{r}[SA][P]}{K_{D}+[P]} \f]
<center>
<i>Equation 15.</i>
</center><br>
\f[\frac{d[X]}{dt} = k_{age}[SA] \f]
<center>
<i>Equation 16.</i>
</center><br>
\f[\frac{d[P]}{dt} = -\frac{k_{r}[SA][P]}{K_{D}+[P]} \f]
<center>
<i>Equation 17.</i>
</center><br>

For reasons that are hopefully obvious, the validation metrics for Sarin exposure in humans (Tables 12 and 13) are predominantly qualitative.  Estimates for the degree of RBC-AChE inhibition are either extrapolated from studies involving extremely low concentrations or drawn from case studies of accidental exposures.  Figure 25 compares the predicted physiological responses to threshold and severe exposures.  Figure 26 highlights the RBC-AChE salvaging action of Pralidoxime.


<center>
<i>Table 11:  Scenarios for Five Levels of Exposure to Sarin via Inhalation Ranging from Threshold (Mild Physiological Effects) to Lethal</i>
</center>

|	Scenario	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Red Blood Cell Acetylcholinesterase Activity	|	Pupil Size	|	Respiratory Function	|	Cardiovascular Function	|	Blood Chemistry	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	 Threshold Sarin Aerosol, Concentration = 0.2 mg/m^3	|	20 minute exposure; CT = 4 mg-min/m^3	|	30	|	1200	|<span class="success">	Slightly decreased @cite nozaki1997relationship	</span>| <span class="success">	~50% diameter reduction @cite national2009acute	</span>| <span class="success">	Mild bronchoconstriction @cite gupta2009handbook	</span>| <span class="success">	Little to no change @cite national2009acute	</span>| <span class="success">	Little to no change @cite national2009acute	</span>|
|	Moderate Sarin Aerosol, Concentration (Short) =  5.0 mg/m^3	|	3 minute exposure; CT = 15 mg/m^3	|	30	|	1100	|<span class="success">	40-50% inhibited @cite gupta2009handbook	</span>| <span class="success">	> 90% diameter reduction @cite national2009acute	</span>| <span class="success">	Respiratory discomfort @cite sidell1997medical (quantified by Pulmonary Resistance Increase) 	</span>| <span class="success">	Little to no change	</span>| <span class="warning">	Little to no change	</span>|
|	Moderate Sarin Aerosol, Concentration (Long) = 0.5 mg/m^3	|	30 minute exposure; CT = 15 mg/m^3	|	30	|	2700	|<span class="success">	40-50% inhibited @cite gupta2009handbook	</span>| <span class="success">	> 90% diameter reduction @cite national2009acute	</span>| <span class="success">	Respiratory discomfort @cite sidell1997medical (quantified by Pulmonary Resistance Increase)	</span>| <span class="warning">	Little to no change	</span>| <span class="warning">	Little to no change	</span>|
|	Severe Sarin Aerosol, Concentration = 4 mg/m^3	|	10 minute exposure; CT = 40 mg-min/m^3	|	30	|	1500	|<span class="success">	75-80% inhibited @cite gupta2009handbook	</span>| <span class="success">	Maximum constriction	</span>| <span class="success">	Depressed ventilation; decreased tidal volume @cite zhuang2008inhalation	</span>| <span class="warning">	Bradycardia and decreased MAP @cite hulse2014respiratory	</span>| <span class="success">	Increased increased PCO2 and decreased P02 @cite zhuang2008inhalation	</span>|
|	Lethal Sarin Aerosol, Concentration = 15 mg/m^3	|	6 minute exposure; CT = 90 mg-min/m^3	|	30	|	1050	|<span class="success">	100% inhibited	</span>| <span class="success">	Maximum constriction	</span>| <span class="success">	Fatal Respiratory Distress @cite black1996chemistry	</span>| <span class="warning">	Bradycardia and decreased MAP @cite hulse2014respiratory	</span>| <span class="success">	Increased PCO2, Decreased PO2 (Hypercapnia & Hypoxia) @cite zhuang2008inhalation	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Sarin_RBCComparison.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Sarin_PulmonaryComparison.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Sarin_TidalComparison.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Sarin_HeartRateComparison.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Drugs/Sarin_OxygenComparison.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Sarin_PupillaryComparison.jpg" width="550"></td>
</tr>
</table>
</center>
<i>Figure 25.  Predicted responses to threshold (red) and severe (blue) exposures to Sarin inhaled from the environment.  Note that constriction of the pupils (miosis) occurs even at very low Sarin concentrations.  The difference in Pulmonary Resistance is due to deposition of Sarin in the upper respiratory tract, which leads to bronchoconstriction and difficulty breathing.  As bronchoconstriction increases, the patient has trouble breathing deeply, resulting in a decrease in tidal volume. </i>
</center><br>

<center>
<i>Table 13:  A severe inhaled dose of Sarin followed by intravenous administration of Pralidoxime </i>
</center>

|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Red Blood Cell Acetylcholinesterase Activity	|	Pupil Size	|	%Respiratory Function	|	%Cardiovascular Function	|	Blood Chemistry	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	 Severe Sarin Aerosol, Concentration = 20 mg/m^3	|	2 minute exposure; CT = 40 mg-min/m^3	|	30	|	1530	|<span class="success">	75-80% inhibited @cite gupta2009handbook	</span>| <span class="success">	Maximum constriction	</span>| <span class="success">	Depressed ventilation; decreased tidal volume @cite zhuang2008inhalation	</span>| <span class="warning">	Bradycardia and decreased MAP	</span>| <span class="success">	Increased increased PCO2 and decreased P02	</span>|
|	Pralidoxime 15mg/kg Iv @cite gupta2009handbook	|		|	1530	|	2430	|<span class="success">	Increase in activity @cite sidell1997medical	</span>| <span class="success">	No change	</span>| <span class="success">	Increased respiration rate @cite rxlist	</span>| <span class="success">	Increased heart rate @cite rxlist	</span>| <span class="success">	Decreased PCO2 and increased PO2	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Drugs/Sarin_RBCWithPralidoxime.jpg" width="550"></td>
    <td><img src="./plots/Drugs/Sarin_O2WithPralidoxime.jpg" width="550"></td>
</tr>
</table>
</center>
<i>Figure 26.  A severe Sarin exposure (green line = beginning of exposure, black line = end of exposure) followed by administration of 1 g intravenous Pralidoxime (pink line).  The red blood cell acetylcholinesterase levels increase, though not to baseline because a fraction of the RBC-AChE still undergoes irreversible aging.  The oxygen saturation is shown to demonstrate the positive effect that Pralidoxime has on respiratory function</i>
</center><br>


@anchor drugs-conclusions
Conclusions
-----------

In general, the drugs in the %BioGears library have strong agreement with both the published data and subject matter expertise. The biggest limitation lies in the transient response to drugs. Currently, the system tolerances provide a response within 30&ndash;60 seconds for all drugs, regardless of the physiologic onset time. The drug response also wears off quickly for all drugs. It is difficult to specify the desired pharmacodynamic response without a well thought out set of parameters. These issues will be addressed as part of future work on the %BioGears Engine by validating the pharmacokinetic model and implementing and validating a pharmacodynamic model.

These models and the combination of these models represent a positive step forward for drug simulation. The parameters are intuitive and accurate for each drug and additional drugs can be added for an expanded drug library.

@anchor drugs-future
Future Work
===========

Coming Soon
-----------
- Intoxication models for drugs beyond Morphine
- Expanded drug library
- Integration of receptor-based pharmacodynamics
- Multiple types of drug-drug interactions

Recommended Improvements
------------------------
- Long-term drug effects, including physiological changes
- Additional drugs 
- Improved kinetic model for aerosols and vapors
- Higher fidelity models of other routes, including epithelial cell (integumentary, gastrointestinal) absorption
- Drug sensitivity variation

@anchor drugs-appendices
# Appendices

## Acronyms

MAP - Mean Arterial Pressure

PK - Pharmacokinetic

PD - Pharmacodynamic

SDK - Software Development Kit

SMEs - Subject Matter Experts

Data Model Implementation
-------------------------
@ref DrugSystemTable "Drug"
