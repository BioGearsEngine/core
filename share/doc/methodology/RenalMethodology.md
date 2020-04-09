%Renal Methodology {#RenalMethodology}
==========================

Overview
========

@anchor renal-abstract
Abstract
--------

The %Renal system's purpose in the body is to filter blood. Each kidney is modeled in the %BioGears Engine using a single, lumped nephron to represent its behavior. By using this circuit analogue of a lumped nephron, %BioGears accurately simulates the major functions of the kidney: filtration, clearance, secretion, and reabsorption. Through these nephron-level processes, we are able to accurately recreate the mechanism by which urine is produced and blood is filtered. The renal system is complex and able to mechanistically and accurately recreate the clearance of the substances currently in %BioGears. The system passes the majority of validation and demonstrates the correct trends in scenarios that test specific functionality.

@anchor renal-introduction
Introduction
------------
### %Renal Physiology

The renal (or urinary) system's primary job is filtering the blood to remove waste and manage
fluid volume, which helps the body maintain homeostasis.  

Figure 1 shows the entire system and is labeled with the following:

1. Urinary system
2. Kidney
3. %Renal pelvis
4. Ureter
5. Urinary bladder
6. Urethra (Left side with frontal section)
7. Adrenal gland
8. %Renal artery and vein
9. Inferior vena cava
10. Abdominal aorta
11. Common iliac artery and vein
12. Liver
13. Large intestine
14. Pelvis

@image html RenalSystem.png
<center>
<i>
Figure 1. This is an illustration of the human renal system.  The bounds of the system are generally accepted to be the combination of everything encompassing #1-6 (urinary system) and #8 renal artery and vein. @cite Jmarchn2010Urinary
</i>
</center><br>

After the blood enters the kidneys through the renal arteries, it is distributed into the nephrons through the branching afferent arteriole network. From the arterioles, the blood then travels into the glomerular capillaries. The capillary wall's permeability coupled with the pressure difference across the permeable layer allows filtration of about 20 percent @cite hall2011guyton of the incoming blood into the Bowman's space and downstream into the tubules, vas rectum and, eventually, the collecting duct where the fluid travels through the ureters and into the bladder in the form of urine. 

The average glomerular filtration rate (GFR) in a human is around 120 mL/min @cite valtin1995renal between both kidneys. The filtered fluid primarily consists of the smaller particles in the blood, as a general rule less than 10 nm in diameter @cite valtin1995renal. This filtered fluid will eventually become urine; however, most of the fluid will be reabsorbed back into the blood to preserve substance and fluid levels in the body. Some substances, such as creatinine, are not reabsorbed, making these good a clinical markers of GFR. 

Pressure and GFR are tightly regulated through the tubuloglomerular feedback mechanism of the kidney. This mechanism allows for an increase or decrease in the diameter of the arteries on either side of the glomerular capillaries, effectively regulating down stream fluid flow and protecting the fragile capillaries of the glomerulus.

Once filtered, reabsorption and secretion both occur along the length of the tubules into the collecting ducts. In this area, the body reabsorbs many of the substances and, on average, over 99% of the water back into the blood @cite hall2011guyton. Reabsorption is heavily dependent on the sodium concentration of the blood, the downstream pressure at the afferent arteriole, and osmotic and oncotic pressure gradients along the tubular cell walls and intra-vascular space. The kidneys influence the osmolarity of the urine by adjusting the amount of sodium and water excreted to the bladder. Secretion allows for further filtration of the blood by the kidneys via active transport of substances back into the tubules from the peritubular capillaries. All of these processes are tightly regulated and allow for careful management of substances levels within the entire body. 
Any water or substance that is not reabsorbed is moved to the bladder as urine through the collecting ducts.

By regulating the substance content of the blood, the kidneys are responsible for maintaining blood pH and 
nitrogen content over long periods of time. Nitrogen waste from the liver is excreted from the kidneys as urea, one of the primary constituents of urine. Bicarbonate is used by the blood as a buffer against much of the acidic content of the body. The kidneys are able to reabsorb virtually all of the filtered bicarbonate and even reclaim bicarbonate that has been used to buffer acids that are filtered into the renal tubules @cite valtin1995renal. 

### %Renal Modeling

The %BioGears %Renal model lumps each kidney into a single nephron (Figure 2). The  fluid flow through the kidney is modeled with an electrical circuit analogue, and the substance transport is a combination of the generic transporter and active filtration/reabsorption/secretion based on feedback and substance parameters. Alternatively, excretion can be directly applied and is governed via clearance equations - as with the drug model (@ref DrugsMethodology). This is consistent with other %BioGears system models, which also employ lumped parameter modeling.

@image html LumpedNephron.png
<center>
<i>
Figure 2. The basic anatomical structure of the nephron. The blood is filtered into the Bowman's capsule from the Glomerular Capillaries. Most of the fluid is then reabsorbed into the Peritubluar Capillaries.  The remaining fluid and substances continue through the tubules and are excreted into the bladder. @cite Madhero882010Nephron
</i>
</center><br>  
@anchor renal-system
System Design
=============

@anchor renal-background
Background and Scope
--------------------

### Requirements

The %Renal system needs to meet the following requirements in %BioGears:
- General kidney functions @cite guyton2006medical
	- Excretion of metabolic waste products and foreign chemicals
	- Regulation of water and electrolyte balances
	- Regulation of body fluid osmolality and electrolyte concentrations
	- Regulation of arterial pressure
	- Regulation of acid-base balance
	- Secretion, metabolism, and excretion of hormones
	- Gluconeogenesis
- Specific effects required by the Engine
	- Nutritional states based on feeding, starvation, water loads, dehydration, and salt intake
	- %Renal stenosis and hypoperfusion
	- General hematology: electrolytes, ions, blood gases, protein, urea, creatinine, hemoglobin, hematocrit, RBC, WBC, and Acid/Base Balance
	- System parameters: Urine volume, osmolality, GFR, and BUN
	- %Respiratory acidosis/alkalosis, metabolic acidosis
	- %Drugs pharmacokinetics handling
	- Extensible and flexible model
	- Maintain conservation of mass and homeostasis

### Existing

There have been numerous approaches to renal modeling in the past. The most significant results stem from a region-based modeling 
approach that discretizes the depth and allows varying permeability along the length of the nephron @cite layton2011mathematical. 
Focusing on the region encompassing the collecting duct allows for consideration of the flow in the tubules as 
well as interactions with the interstitial space. This space plays a key role in the re-absorption process and is modeled with striking detail.
 This model has been shown to be an extremely accurate representation of the urine concentrating mechanism along the length of the 
 short and long nephron and has been shown to accurately model the two feedback mechanisms that control glomerular filtration and 
 blood flow: the myogenic response and the tubuloglomerular feedback. This model uses a region-based approach to model a single 
 collecting duct, then it scales results up to a system-wide representation. The idea of representing the kidney as a functional
 nephron is something that many other researchers in the past have used to accomplish very promising results regarding kidney functionality 
 @cite moss2009computational, @cite moss2014hormonal, @cite stephenson1976model. 


In each of these efforts the nephron is broken into its distinct sections and the reabsorption of substances and fluid is calculated
at each point. Each calculation is done in series until the filtered fluid reaches the collecting ducts and exits as urine. By assuming 
the results carry over to the other nephrons in the kidney, these models are able to create realistic urine outputs under most conditions.
While these models can show the tubular transport of substances and fluid in great detail, some do not directly show the renal
hemodynamics at work to control glomerular pressure and GFR. In addition, many of these results are steady state rather than time-dependent, which limits the range of applications and outputs.

Many models use the Starling equation (Equation 1) to calculate the fluid flow between the tubules and blood. While this is generally considered to be an accurate representation of the fluid flow, this approach assumes a constant oncotic pressure. This makes the hydrostatic pressures the only dynamic inputs to the Starling equation. The accuracy of this approach can be enhanced by accounting for differences in the substance content in vascular compartments in the glomerular capillaries and deriving the respective oncotic pressures, which allows for a better representation of the pressure differential that drives the flow. 

\f[{J_v} = {K_f}\left( {\left[ {{P_c} - {P_i}} \right] - \sigma \left[ {{\pi _c} - {\pi _i}} \right]} \right)\f]
<center>
*Equation 1*
</center><br>


J<sub>v</sub> represents net fluid flow, P<sub>c</sub> and P<sub>i</sub> represent the capillary and interstitial hydrostatic pressures,
&pi;<sub>c</sub> and &pi;<sub>i</sub> are the capillary and interstitial oncotic pressures, respectively, K<sub>f</sub> is the filtration coefficient, and &sigma; is the reflection coefficient. The equation uses a V=IR approach to derive the flow from the total pressure differential and the resistance, which is represented by the filtration and reflection coefficients.

There are other existing models that show the hemodynamics present within the kidney. The model shown by Moss @cite moss2009computational uses a network of nephrons and a fluid
circuit to represent the kidney. The fluid circuit can affect the delivery of blood to the various glomerular capillaries by changing the resistance that represents the afferent 
arteriole. This allows the model to represent the physiological feedback from changes in sodium reabsorption, which can have notable effects on renal blood flow. The fidelity
of this sort of model is very flexible, as it is entirely dependent on the detail put into the nephron. Unfortunately, as the complexity of the nephron model or the
desired size of the network increases, it can become computationally intensive. This can cause problems when attempting to use this approach as part of a real-time model.

### Approach

The %Renal model includes both the upper and lower urinary tracts.  The urine formation in the kidneys is simulated using a single lumped nephron model, which is the internal functional element shown as #2 in Figure 1. The computational representation of the system includes the renal artery that feeds into the afferent arteriole. Filtration takes place through the glomerular capillaries, and re-absorption occurs through the tubules into the peritubular capillaries. The peritubular capillaries connect into the vena cava, allowing transport back into the cardiovascular system. The ureter and bladder make up the rest of the kidney model in %BioGears. This model of the renal system allows for tubuloglomerular feedback and re-absorption feedback. These mechanisms allow for diuresis under heavy pressure loads and filtrate regulation under variable pressure scenarios. 

The %Renal fluid circuit is inserted in the the circulatory system and replaces a calibrated three-element (two resistances and one compliance) Windkessel model that is used for %Cardiovascular validation.  In this way, the standalone circuit at a resting physiologic state matches from a total fluid mechanics standpoint.

The transport of substances is a combination of the generic methodology and active filtration at specific locations. However, if a substance has a defined renal clearance parameter, as with drugs, that value is directly applied and is only dependent on blood flow changes.

@anchor renal-dataflow
Data Flow
---------

The %Renal System determines its state at every time step through a three-step process: Preprocess, Process, and Postprocess (Figure 3). In general, Preprocess sets the circuit element values based on feedback mechanisms and engine settings/actions. Process uses the generic circuit calculator to compute the entire state of the circuit. Postprocess is used to advance time. More specifics about these steps are detailed below.

### Reset, Conditions, and Initialization

During initialization of the system, the filterability parameter for each defined substance is calculated.  This is only done and set once because it is only dependent on the substance's molecular weight and charge in blood, which do not change.

The two %Renal system conditions are also applied using the engine stabilization methodology.  %Renal stenosis and consume meal are discussed in detail later.

### Preprocess

Preprocess is called by the engine to apply the system-specific feedback mechanisms.  Each of these mechanisms for the %Renal system are described individually and in detail later.

### Process

The generic circuit solving and transporting are done on the combined circulatory system inside the %Cardiovascular system.  After the new state of the circuit is determined, active transport is performed, and the system data is calculated and set.

### Post Process

The Postprocess step moves values calculated in the Process step from the next time step calculation to the current time step calculation. The current implementation has no specific postprocess functionality for the %Renal system. All postprocessing is done on the combined circulatory system inside the %Cardiovascular system.

### Assessments

There is one assessment that is done within the %Renal system - urinalysis.  This is a data call that requires further calculations and analysis to create outputs.  It is discussed in detail later.

<img src="./images/Renal/RenalFlowChart.png" width="1100">
<center>
<i>
Figure 3. The data flow for the %Renal System consists of Preprocess, Process, and Postprocess. Preprocess determines the circuit element values based on feedback mechanisms and engine actions. Process uses generic circuit methodology to solve the circuit for pressures, volumes, and flows along the paths and on nodes. Postprocess updates these quantities to the next time step and then begins advancing time, which causes the loop to repeat.
</i>
</center><br>

@anchor renal-features
Features, Capabilities, and Dependencies
----------------------------------------

### Circuit

The %BioGears %Renal circuit (Figure 4) determines blood and urine pressure, flow, and volume, organized by compartments.  These compartments are comprised of lumped parameter models that use resistors, capacitors, pressure sources, and valves.  The number of lumped parameter models used to represent the %Renal System was chosen to provide a level of fidelity that meets the requirements of the overall project and to provide sufficient system capability.
@anchor renal-circuit
@image html RenalCircuit.png
<center>
<i>
Figure 4. The %Renal circuit is made up of nodes and paths with elements.  The fluid mechanics of the system are calculated assuming no transition between blood and urine.  The substance quantities are what distinguish the two fluid types.  Everything above the dashed line can be considered blood.  Fluid below the dashed line transitions to urine as substance filtration and reabsorption change its make-up. The "T" labeled paths are location where transport of each substance is calculated by the %Renal system directly.  All others are done generically.
</i>
</center><br>

Nodes serve as the connection points for paths and are the locations at which
pressures are measured. Each %Renal node contains a pressure value, which is given
with respect to the atmospheric reference node (indicated in the diagram by the
equipotential symbol). Nodes can also hold the values for volume or the mass and
concentration of substances. Paths contain information about the flow (volume
per time) and volumes of compliances. The %BioGears @ref CircuitMethodology document
contains more information about %BioGears circuit definitions and modeling. The %BioGears @ref SubstanceTransportMethodology
contains more information about the substance transport and mass and concentration calculations.

Dynamic pressure sources are used at the filtration and reabsorption locations to model the colloid osmotic pressures caused by protein in the fluid.  These pressures, in combination with the hydrostatic pressures, give a correct total pressure difference to cause a correct net fluid flow.

The bladder is currently modeled as a constant pressure source and the volume is calculated directly based on the flow through that source.  Future versions will likely replace the source with a compliance and incorporate a peristaltic flow model.

The compliance and volume of the %Renal circuit are determined by validated volume data and model assumptions made regarding the various nodes in the renal system. These compliances allow for some volume variability as the capillaries experience pressure and flow changes.

The volume distribution of the %Renal system is as follows:
- Kidneys = 69.6 mL
	- Right Kidney = 34.8 mL
		- Right Small Vasculature = 17.4 mL
			- Right Glomerular Capillary (compliance) = 3.48 mL
			- Right Peritubular Capillary = 3.48 mL
			- Right Efferent Arteriole = 3.48 mL
			- Right Bowmans Capsule = 3.48
			- Right Afferent Arteriole = 3.48 mL
		- Right Large Vasculature = 17.4 mL
			- Right %Renal Vein (compliance) = 5.8 mL
			- Right %Renal Artery (compliance) = 5.8 mL
			- Right Tubules = 5.8
	- Left Kidney = 34.8 mL
		- Left Small Vasculature = 17.4 mL
			- Left Glomerular Capillary (compliance) = 3.48 mL
			- Left Peritubular Capillary = 3.48 mL
			- Left Efferent Arteriole = 3.48 mL
			- Left Bowmans Capsule = 3.48
			- Left Afferent Arteriole = 3.48 mL
		- Left Large Vasculature = 17.4 mL
			- Left %Renal Vein (compliance) = 5.8 mL
			- Left %Renal Artery (compliance) = 5.8 mL
			- Left Tubules = 5.8

The %Renal circuit contains 11 resistances for each kidney, and a shared urethra resistance - see Figure 5.  The urethra (number 12) is typically set as an open switch (approximately infinite resistance), unless the patient is urinating, when it becomes closed to allow flow out of the bladder.

@image html RenalResistances.png
<center>
<i>
Figure 5. The resistances in the left kidney are numbered for further description.
</i>
</center><br>

Baseline resistances were determined by leveraging reference pressure and flow values from literature.  Each resistance is calculated using Equation 2, where R is the resistance along the path, &Delta;P is the pressure difference between the source and target nodes, and F is the fluid flow through the path.

\f[R = \frac{{\Delta P}}{F}\f]
<center>
*Equation 2*
</center><br>

The untuned baseline resistance values were calculated using the values in Table 1.  These values and the model were verified with unit testing using a static arteriole pressure equal to the normal mean value.

<br><center>
<i>Table 1. The baseline resistances used in the %Renal circuit @cite guyton2006medical. The numbering matches that of the previous figure.</i>
</center>

| Location | Number in Figure | Beginning Pressure (mmHg) | Ending Pressure (mmHg)| Flow (mL/min) | Resulting Baseline Resistance (mmHg/mL/min) |
| :---- | :---- | :---- | :---- | :---- | :---- |
| Arteries | 1 | 100 | 100 | 600 | 0.0 |
| RenalArteries | 2 | 100 | 85 | 600 | 0.0250 |
| AfferentArteriole | 3 | 85 | 60 | 600 | 0.0417 |
| GlomerularCapillaries | 4 | 60 | 59 | 537.5 | 0.0019 |
| EfferentArteriole | 5 | 59 | 18 | 537.5 | 0.0763 |
| PeritubularCapillaries | 6 | 18 | 8 | 600 | 0.0167 |
| RenalVeins | 7 | 8 | 4 | 600 | 0.0066 |
| GlomerularFilter | 8 | 28 | 18 | 62.5 | 0.1600 |
| Tubules | 9 | 18 | 6 | 62.5 | 0.1920 |
| Reabsorption | 10 | -9 | -19 | 62 | 0.1613 |
| Ureter | 11 | 6 | 4 | 0.52 | 3.846 |
| Urethra (during urination) | 12 | 4 | 0 | 1320 | 0.0030 |
@anchor renal-dependencies
### Patient Variability

The total vascular volume of the %renal system in %BioGears is defined to be a fraction of the body total, making kidney volume directly dependent upon the patient data: height, weight, and gender. The volume fraction of both kidneys is set to be 0.0202, or about 1/50th of total blood volume @cite valtin1995renal. This volume is then divided in half to populate the total vascular volume of one kidney. For a further breakdown of the volume of each kidney compartment see @ref renal-features "Features".


### Feedback

#### Colloid Osmotic Pressure

The net filtration and reabsorption pressures are determined by the sum of the hydrostatic and colloid osmotic forces across the membranes.  While the hydrostatic pressure is automatically handled using the generic circuit algorithms, the colloid osmotic pressure needs to be determined specifically.  This is done by using the Landis-Pappenheimer equation (Equation 4) that is dependent on total protein @cite khazaei2008new.  Since the %BioGears engine tracks albumin, a constant relationship between total protein and albumin can be leveraged using Equation 3.  *C<sub>TP</sub>* is the local concentration of total protein, *C<sub>A</sub>* is the local concentration of albmuin, and *P<sub>CO</sub>* is colloid osmotic pressure.

\f[{C_{TP}} = 1.6{C_A}\f]
<center>
*Equation 3*
</center><br>

\f[{P_{CO}} = 2.1{C_{TP}} + 0.16C_{_{TP}}^2 + 0.009C_{_{TP}}^3\f]
<center>
*Equation 4*
</center><br>

The Landis-Pappenheimer equation is applied at the pressure sources numbered in Figure 6.

@image html RenalPressureSources.png
<center>
<i>
Figure 6. The four pressure sources that represent the colloid osmotic pressure are determined via the local albumin concentration.
</i>
</center><br>

Table 2 gives the typical colloid osmotic pressure values.  The tubular osmotic pressure is set constant in our model and does not change because we are not modeling the interstitial space.  The remaining osmotic pressures will increase and decrease in relation to the local albumin concentration variations.  See Table 5 for %BioGears resulting values when albumin concentration feedback is applied.

<br><center>
<i>Table 2. The baseline colloid osmotic pressure values used for both initialization and model validation @cite guyton2006medical. The numbering matches that of the previous figure. </i>
</center>

| Location | Number in Figure | Baseline Colloid Osmotic Pressure (mmHg) |
| :---- | :---- | :---- |
| GlomerularCapillariesOsmoticPressure | 1 | -32 |
| BowmansCapsulesOsmoticPressure | 2 | 0 |
| TubulesOsmoticPressure | 3 | -15 |
| PeritubularCapillariesOsmoticPressure | 4 | -32 |

#### Fluid Permeability

The hydraulic and colloid osmotic pressure gradients across the glomerular and pertitubular capillary membranes cause the movement of fluid. The rate of fluid movement is a function of the fluid permeability of the membranes, the total membrane surface area, and the pressure gradient. Considering a nominal pressure that is equal to the linear integral of the effective pressure over the length of the glomerular capillary (with the effective pressure being the difference between the hydrostatic and colloid osmotic pressure gradients at each point along the length of the capillary), the glomerular filtration rate is proportional to the nominal pressure, as shown in Equation 5.

\f[GFR = {L_p}{S_f}\int {\left[ {\left( {{P_g}\left( x \right) - {P_B}} \right) - \sigma \left( {{\pi _g}\left( x \right) - {\pi _B}} \right)} \right]dx}  = {L_p}{S_f}\Delta P\f]
<center>
*Equation 5*
</center><br>

*L<sub>p</sub>* is the hydraulic conductivity of the membrane and S<sub>f</sub> is the surface area. Relating the equation above to the linear fluid dynamics equations, it is apparent that the resistance to flow can be modeled as 1/(*L<sub>p</sub>* *S<sub>f</sub>*). The product, *L<sub>p</sub>* *S<sub>f</sub>*, has been called the glomerular filtration coefficient @cite tuma2011microcirculation. A similar reabsorption coefficient can be derived from the equations governing the reabsorption of water in the renal tubules @cite tuma2011microcirculation.

The hydraulic permeability is computed for use in the %BioGears model using filtration coefficients reported in literature as well as surface area estimates. The surface area of the glomerular capillaries is estimated to be about 2.0 square meters per human kidney @cite valentin2002icrp. The surface area of the peritubular capillaries is larger than the glomerular capillaries @cite tuma2011microcirculation. However, how much larger is difficult to quantify. For the %BioGears model, it is assumed that the surface area of the peritubular capillaries is 25 percent larger than the surface area of the glomerular capillaries. Using the glomerular filtration coefficient value of 13 mL/min-mmHg for both kidneys as reported in @cite tuma2011microcirculation (6.5 mL/min-mmHg each kidney assuming an equal distribution of surface area), the hydraulic conductivity of the glomerular capillaries is calculated to be 3.67647 mL/m2-min-mmHg. Likewise, using the reabsorption coefficient value of 10  mL/min-mmHg for both kidneys @cite tuma2011microcirculation, the hydraulic conductivity of the peritubular capillaries is computed to be 2.91747 mL/m2-min-mmHg.

These values are applied in the model as the resistances circled in Figure 7 and are determined using Equation 6.  *R<sub>Filt</sub>* is the membrane fluid resistance, *k<sub>p</sub>* is the permeability, and *A* is the total membrane area.

\f[{R_{filt}} = \frac{1}{{{k_p}A}}\f]
<center>
*Equation 6*
</center><br>

@image html Filtration.png
<center>
<i>
Figure 7. The membrane fluid filtration for both glomerular filtration and peritubular reabsorption locations are modeled as the resistances circled in red.
</i>
</center><br>

#### Ultrafiltration

Individual substance transport from the glomerular capillaries to the bowmans capsules is directly proportional to the fluid glomerular filtration rate and filterability value based on molecular size and charge.  The glomerular capillary membrane is thicker than most other capillaries, but it is also much more porous and filters fluid at a high rate @cite guyton2006medical.  Ultrafiltration is a largely passive mechanism.  

Empirical data was used to determine a generic relationship between molecular weight and molecular radius.  Values for water, glucose, inulin, myoglobin, hemoglobin, and albumin were used to determine the best fit shown in Figure 8 @cite rhoades2003medical.

<center><img src="./images/Renal/MolecularRadius.png" width="600"></center>
<center>
<i>
Figure 8. This is the best fit of molecules present in the blood molecular weight to molecular radius.  The resulting equation shows very good fit.
</i>
</center><br>

Electrical charge of each molecule has an effect on the total filterability.  A negative charge restricts filtration, where as a positive charge filters more readily.  Figure 9 shows the relative filterability best fit for positively charged, neutral, and negatively charged molecules as a function of molecular radius (determined using the relationship in Figure 8).

<center><img src="./images/Renal/RelativeFilterability.png" width="600"></center>
<center>
<i>
Figure 9. Effect of size and electrical charge of dextran on its filterability by the glomerular capillaries @cite bohrer1978permselectivity.  A value of 1.0 indicates that the substance is filtered as freely as water.  This relationship can be extrapollated to all substances.  The resulting equations show very good fit.
</i>
</center><br>

#### Reabsorption

Reabsorption works in much the same way as glomerular filtration, with additional active mechanisms that are different for each substance. For the %BioGears %Renal model, all of the active transport mechanisms are lumped into a single reabsorption ratio parameter defined for each substance. This ratio represents the extent to which the substance is reabsorbed from the tubules to the peritubular capillaries in relation to the fluid flow. Therefore, a value of 1.0 indicates that the substance is reabsorbed at the same rate as water, values below 1.0 indicate that it is reabsorbed more avidly than water, and values above 1.0 indicate that it is reabsorbed less avidly than water. Each substance also has a transport maximum value defined that prevents substance mass movement at a specific transient value. A value of infinity is allowed for both reabsorption-specific substance parameters.

##### Reabsorption Feedback

Pressure natriuresis and diuresis are simulated in the %BioGears %Renal model through reabsorption premeability modifiers. In the kidneys, as arterial pressure increases, shear stresses are developed along the cell walls. These stresses induce the release of nitric oxide, which is diffused downstream to the tubules of the nephron. This serves to decrease the sodium reabsorption rate through select entry pathways @cite navar1999kidney. This decrease in sodium resabsorptioin decreases the osmotic gradient along this fluid path, consequently leading to a coupled decrease in fluid reabsorption. %BioGears does not currently support nitric oxide, so we model this mechanism by coupling the fluid permeability characteristics of the tubules as function of arterial pressure. This allows for the connection to downstream pressure changes and sodium/water reabsorption. A second order polynomial (See Equation 7) is fit to the data taken from @cite guyton2006medical that scales the tubules' fluid permeability as a function of arterial pressure. This permeability then directly affects the resistance along the reabsorption pathway, leading to a decrease or increase in water/sodium transport. Diuretic administration can also inhibit the reabsorption by modifying the tubular lumen permeabilty as a function of plasma concentration of Furosemide, @ref DrugsMethodology.

<center><img src="./plots/Renal/MAP_vs_UPR.jpg" width="600"></center>
<center> 
<i>Figure 10 shows the urine production rate as a function of the mean arterial pressure. Blue is experimental data and red is %BioGears simulation data. Diuresis takes place in response to increased blood pressure, in agreement with renal functionality.</i>
</center><br>


\f[{k_p} = \left( {2.01 \times {{10}^{ - 6}}} \right)P_A^2 - \left( {8.1 \times {{10}^{ - 4}}} \right)P_A^{} + 9.4 \times {10^{ - 2}}\f]
<center>
*Equation 7*
</center><br>

##### Diuretic Response
In addition to alterations in permeability due to ion concentration and perfusion pressure, the renal system's reabsorption pathway can also be effected by diuretic concentrations in the blood plasma. These concentrations directly effect the tubular permeability, simulating Furosmide binding to the Na-K-2Cl symporter. This effect is accounted for by calculating the response due to plasma concentrations in the Drug methodology. This permeability modifier is then stored and accessed in the %Renal system each time step to calculate the reabsorption pathway resistance: 

\f[{k_p}_{i+1} =  {k_p}_i * K_{mod}(C_p) \f]
<center>
*Equation 8*
</center><br>
Here *K<sub>mod</sub>(C<sub>p</sub>)* is the modification coefficient calculated as a hill type functional response to Furosemide blood plasma concentration, *C<sub>p</sub>*.


### Secretion 
Flow of substances can be directed from the peritubular capillaries into the tubules through secretion. As fluid travels along the tubules and into the collecting ducts of the nephron, various stages of the journey either reabsorb or secret substances and water back into and out of the fluid. One of the most important tasks of the renal system is handling of potassium levels in the blood stream. Secretion is one of the main ways that the renal system handles this task with active transport mechanisms located along the tubular cell wall. %BioGears simulates this active handling of potassium by allowing for secretion into the ureter from peritubular capillaries. Mass is calculated and tightly regulated by the %BioGears renal system by actively moving any mass above a certain threshold directly into the urine. %BioGears does not currently support disturbances to the permeable layer and so shifts in potassium levels in the body will not currently be possible. 

#### Bladder and Excretion

Excretion of fluid and substances to the bladder is done generically by the circuit solver (@ref CircuitMethodology) and transporter (@ref SubstanceTransportMethodology).

#### Tubuloglomerular Feedback

One of the main mechanisms for kidney regulation of renal blood flow due to changes in the mean arterial pressure is tubuloglomerular feedback. This is caused by a transient increase in GFR that leads to an increased sodium delivery to the macula densa @cite rhoades2003medical. For the %BioGears %Renal model, the macula densa is lumped with all tubules. When the mass flow rate of sodium changes in the tubules, a response is applied.

Within an autoregulatory range of 80 to 180 mmHg for the MAP, the renal blood flow is is maintained by either constricting (increased resistance) or dilating (decreased resistance) the afferent areteriole @cite rhoades2003medical. A higher sodium mass flow rate in the tubules leads to a higher resistance, and a lower mass flow rate leads to a lower resistance (see Figure 11). The minimum and maximum resistances for the afferent arteriole were determined through a unit test to be 1.7 mmHg/mL-s (at MAP of 80 mmHg) and 12.382 mmHg/mL-s (at MAP of 180 mmHg).

@image html TubuloglomerularFeedbackFlow.png
<center>
<i>
Figure 11. A flow diagram showing the tubuloglomerular response to increased and decreased MAP.  @cite rhoades2003medical
</i>
</center><br>

High frequency oscillations of the tubuloglomerular feedback are damped using the tuning constant, alpha, in Equation 8.  *R<sub>Aff</sub>* is the afferent arteriole resistance where i and i+1 signify the current and the next resistance respectively, ||&Delta; U<sub>Na</sub>|| is the normalized change in sodium flow into the tubules. Normalization is used to decrease sensitivity of the response. Large fluctuations are not only physiologically incorrect but also create instabilities during heartbeats. The sodium mass flow setpoint is the expected value with a stable standard patient. Figure 13 indicates fair agreement between the mechanics of %BioGears and experimental data. Autoregulation is handled entirely from the TGF response, with the myogenic response not modeled currently.

\f[R_{Aff}^{i + 1} * = \frac{{R_{Aff}^i}}{{R_{Aff}^{i + 1}}} + \alpha \left\| {\Delta {U_{Na}}} \right\|\f]
<center>
*Equation 9*
</center><br>


@image html TubuloglomerularFeedback.png
<center>
<i>
Figure 12. The circle shows the location used to determine the sodium value, and the resistance where the feedback is applied is denoted with an oval.
</i>
</center><br>

<center>
<table border="0">
<tr>
    <td><img src="./plots/Renal/MAP_vs_GFR.jpg" width="550"></td>
    <td><img src="./plots/Renal/MAP_vs_RBF.jpg" width="550"></td>
</tr>
</table>
</center>
<center>
<i>
Figure 13. The plots show the flow response to variations in mean arterial pressure: blue indicates experimental data and red indicates %BioGears simulation data. Autoregulatory plateau is seen in GFR and RBF. 
</i>
</center><br>


#### Osmoreceptor Feedback

When osmolarity (plasma sodium concentration) increases/decreases above/below normal the osmoreceptor feedback system compensates in the way shown in Figure 14. Increased water permeability in the distal nephron segments causes increased water reabsorption and excretion of a small volume of concentrated urine @cite guyton2006medical. This mechanism tends to keep the plasma sodium concentration stable and was calibrated with extended multi-hour simulations.

@image html OsmoreceptorFeedbackFlow.png
<center>
<i>
Figure 14. A flow diagram showing the osmoreceptor response to increased and decreased plasma sodium concentration.  @cite guyton2006medical
</i>
</center><br>

The sensitivity of the osmoreceptor feedback is calibrated using the tuning constant *x* in Equation 9.  *k<sub>P</sub>* is the reabsorption fluid permeability, *C<sub>SP</sub>* is the sodium plasma concentration in the tubules, and *C<sub>SP,Set</sub>* is the setpoint.  The sodium plasma concentration setpoint is the expected value with a stable standard patient.

\f[{k_p} *  = {\left( {\frac{{{C_{SP}}}}{{{C_{SP,Set}}}}} \right)^x}\f]
<center>
*Equation 10*
</center><br>

#### Gluconeogenesis

Gluconeogenesis is a metabolic pathway that results in the generation of glucose from non-carbohydrate carbon substrates, such as lactate.  Within the %BioGears %Renal system, this process is applied by converting excreted lactate at a one-to-one mass ratio into reabsorbed glucose. This effectively removes all lactate from the urine until the transport maximum is achieved. If the converted mass exceeds the transport maximum, it is capped and the remainder continues to the urine.

#### %Renal Clearance

Some substance in the engine specify a single renal clearance parameter (e.g. drugs) instead of inputs required for the previously described active transport mechanisms.  If so, that value is directly applied and is only dependent on blood flow and plasma concentration changes.  Details on this behavior are in @ref DrugsMethodology.

### Dependencies

The %Renal system is directly connected to the %Cardiovascular system at the aorta and vena cava.  The boundary condition feedback in and out is automatically handled through the node and path interactions.  It also shares a reference node.



@anchor renal-assumptionlimitations
Assumptions and Limitations
---------------------------

There are several assumptions that were used in creating this %Renal model:
- All nephrons in each kidney are lumped together
- Secretion effects are lumped with reabsorption
- No specific substance metabolism is modeled
- Both kidneys are identical
- The difference between blood and urine is substance dependent only; they are the same fluid from a physics standpoint

@anchor renal-conditions
Conditions
----------

### Consume Meal

The consume meal condition takes the resting renal clearance of each substance and extrapolates how much would be removed during a specified fasting period.  The substances are then decremented accordingly.  The loss of fluid is handled by the %Energy system (@ref EnergyMethodology).  In this way, both dehydration and starvation can be modeled.  See the @ref GastrointestinalMethodology document for more information.

@anchor renal-actions
Actions
-------

### Urinate

Urination empties the bladder. This can be called as an action or will automatically occur with the functional incontinence event. This action decreases the urethra resistance to allow the bladder to empty to 1 mL.  The fluid mechanics and transport are done generically by the circuit solver and transporter respectively.

<center><img src="./plots/Renal/12hr_Urination.jpg" width="800"></center>
<center> 
<i>Figure 15. Bladder volume over time is shown during a twelve hour simulation. Once the bladder reaches the maximum allowable volume, 400mL, the patient will experience functional incontinence.</i>
</center><br>

@anchor renal-events
Events
------

### Diuresis

Diuresis is the state of producing urine at a significantly faster rate than normal. In %BioGears, we consider diuresis to occur when the average production rate of urine exceeds 8 mL/min @cite upsdell1988diuretic, and the event is reversed when the average production rate of urine falls below 7.5 mL/min.

### Antidiuresis

Antidiuresis occurs when small quantities of urine are being produced at an osmolarity greater than that of the plasma. For %BioGears, this event is thrown when the average urine production rate is less than 0.5 mL/min and the urine osmolarity is greater than 280 mOsm/L @cite valtin1995renal. The event is reversed when either the urine production rate rises above 0.55 mL/min or the urine osmolarity falls below 275 mOsm/L.

### Natriuresis

Natriuresis is the state of an unusually high rate of sodium excretion into the urine. In %BioGears, the setpoint for natriuresis is an average sodium excretion rate of 14.4 mg/min, or 6 times the resting sodium excretion rate @cite moss2014hormonal. The event is reversed when the average sodium excretion rate falls below 14.0 mg/min.

### Functional Incontinence

Functional incontinence occurs when the bladder becomes too full of urine and automatcially urinates. When the urine volume exceeds the maximum volume of the bladder in %BioGears, 400 mL, the functional incontinence event is thrown and a urinate action is automatically executed.

@anchor renal-assessments
Assessments
-----------

### Urinalysis

Validation of the urine panel is done by analyzing at the resting physiologic quantities. Urine color is determined based on the osmolality of the urine according to Table 3 below. Urine blood content registers as positive if the concentration of hemoglobin in the urine is greater than 0.15 ug/mL @cite walker1990clinical. Currently, the presence of blood does not affect the color of the urine, with urine color being determined entirely by osmolality of the urine, table 3.

<br><center>
<i>Table 3. Healthy urine color is determined by the osmolality of the urine. The colors used in %BioGears were derived from @cite kovacs1999urine . </i>
</center>
|Urine Osmolality                     |Urine Color              | 
|------------------------             |------------------------ |
|UrineOsmolality <= 400 mOsm/kg		  |Pale Yellow	            |
|400 < UrineOsmolality <=750 mOsm/kg  |Yellow		            |
|UrineOsmolality > 750 mOsm/kg        |Light Brown              |

<br><center>
<i>Table 4. Urinalysis is conducted as described by Roxe @cite walker1990clinical . In the current scope of %BioGears, there are no scenarios where blood, glucose, ketones, or 
proteins are expected in the urine in significant quantities, which is confirmed below. Specific gravity and color are each dependent on the osmolarity of the urine and are within
expected values for a healthy individual at rest.</i>
</center>
@insert doc/validation/UrinalysisValidationTable.md

Results and Conclusions
=======================

@anchor renal-restingvalidation
Validation - Resting Physiologic State
--------------------------------------

Validation results for system and compartment quantities for a resting standard patient are listed in Tables 5 and 6. System-level quantities show favorable agreement with validation values. The pressure value discrepancies in urine compartments can be largely explained by the %Renal model's lack of a specific extracellular space that affects both the hydrostatic and osmotic pressures.

<br><center>
*Table 5. Validation of the resting physiologic state system-level outputs from %BioGears compared 
to referenced values.*
</center>

@insert doc/validation/RenalValidationTable.md

<br><center>
*Table 6. Validation of the resting physiologic state comparison of compartment-level outputs from %BioGears
to referenced values.*
</center>

@insert doc/validation/RenalCompartmentsValidationTable.md

Validation results for substance parameters that are determined and/or applied by the %Renal system are shown in Table 7.  These values are highly dependent on substance parameters and show favorable overall agreement for the resting standard patient. Sodium is especially critical for the osmoreceptor and tubuloglomerular feedback mechanisms.

<br><center>
*Table 7. Validation of the resting physiologic state substance parameter outputs from %BioGears compared 
to referenced values.*
</center>

@insert doc/validation/RenalSubstancesValidationTable.md

@anchor renal-actionconditionvalidation
Validation - Actions and Conditions
--------------------

<center>
<i>
Table 8. There are seven scenarios that test renal capabilities, saline ingestion, water ingestion, unilateral
and bilateral stenosis, Two severities of hemorrhage, and high altitude environment change. Each scenario is designed to test the mechanical functionality of the renal system, its feedback mechanisms that control filtration and re-absorption, or the system's substance handling. We have a total of 35 green, 6 yellow, and 0 red parameters among the 3 
scenarios indicating a very good agreement with the validation data.
</i>
</center>
|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	SalineIngestion	|	Patient ingests 1L of hypertonic saline	|<span class="success">	2	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	RenalStenosisModerateUnilateral	|	60% occlusion of left kidney	|<span class="success">	6	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	RenalStenosisSevereBilateral	|	90 % occlusion of both kidneys	|<span class="success">	6	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	HemorrhageClass2NoFluid	|	250 mL/min hemorrhage for 240 seconds	|<span class="success">	8	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	HemorrhageClass4NoFluid	|	250 mL/min hemorrhage for 490seconds	|<span class="success">	8	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	HighAltitudeEnvironmentChange	|	High altitude environment change for 900s 	|<span class="success">	1	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|	WaterIngestion	|	Patient ingests 1 L of fluid	|<span class="success">	3	</span>|<span class="warning">	1	</span>|<span class="danger">	0	</span>|
|	Starvation	|	Patient has not eaten for 72 hrs	|<span class="success">	1	</span>|<span class="warning">	3	</span>|<span class="danger">	0	</span>|
|		|	Total	|<span class="success">	35	</span>|<span class="warning">	10	</span>|<span class="danger">	0	</span>|


### Hemorrhage - Action

The renal system regulates glomerular filtration and renal blood flow through the tubuloglomerular feedback mechanism. This mechanism is modeled through constriction and dilation of the afferent arteriole as a function of sodium flow rate into the tubules. This mechanism is tested through two different hemorrhage scenarios: HemorrhageClass2NoFluid and HemorrhageClass4NoFluid. In each scenario, the body loses blood at a rate of approximately 250 mL/min with the differences in class being the length of the bleeding. As the patient bleeds, blood volume and mean arterial pressure decrease, causing a reduction in blood flow to the renal system. As this decrease happens, the TGF system tries to compensate by dilating the afferent arteriole, reducing the resistance to flow into the glomerular capillaries. This balances the filtrate up to a point, but as blood volume and pressure continue to decrease, renal blood flow and glomerular filtration rate see reductions in line with past research @cite corcoran1943effects. This is more pronounced as the hemorrhage is allowed to persist. Qualitatively, the renal system in %BioGears displays the mechanical trends seen in past research. 

Table 9 shows the effect seen in the %Renal system. For a complete write-up of the hemorrhage scenarios, see the @ref CardiovascularMethodology.

<center>
<i>
Table 9. In this scenario, the patient is given a class II hemorrhage, which causes changes in MAP and blood volume as the bleeding progresses.
Quantification of the renal system during and after the bleed ends are displayed here. The renal system shows responses in line with actual patients.
</i>
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Glomerular Filtration Rate (mL/min)	|	Mean Arterial Pressure (mmHg)	|	%Renal Blood Flow(mL/min)	|	Urine Production Rate (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate 100 mL/min Hemorrhage	|		|	30	|	930	|<span class="success">	Decrease @cite corcoran1943effects	</span>|<span class="success">	Decrease @cite corcoran1943effects	</span>|<span class="success">	decrease @cite corcoran1943effects	</span>|<span class="success">	Decrease @cite corcoran1943effects	</span>|
|	Stop Hemorrhage	|		|	930	|	1230	|<span class="success">	Slight Increase @cite corcoran1943effects	</span>|<span class="success">	Steady @cite corcoran1943effects	</span>|<span class="success">	Slight Increase @cite corcoran1943effects	</span>|<span class="success">	Steady @cite corcoran1943effects	</span>|


The class 4 hemorrhage as the same bleed rate but the bleeding persists for a longer duration: 490 seconds. %Renal function displays the same trends but with a further reduction in glomerular filtration and renal blood flow due to the duration of the bleed action. 

<center>
<i>
Table 10. In this scenario the patient is given a class IV hemorrhage, which causes changes in MAP and blood volume as the bleeding progresses.
Quantification of the renal system during and after the bleed ends are displayed here. The renal system shows responses in line with actual patients.
</i>
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Glomerular Filtration Rate (mL/min)	|	Mean Arterial Pressure (mmHg)	|	%Renal Blood Flow(mL/min)	|	Urine Production Rate (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate 150 mL/min Hemorrhage	|		|	30	|	1830	|<span class="success">	Decrease @cite corcoran1943effects	</span>|<span class="success">	Decrease @cite corcoran1943effects	</span>|<span class="success">	decrease @cite corcoran1943effects	</span>|<span class="success">	Decrease @cite corcoran1943effects	</span>|
|	Stop Hemorrhage	|		|	1830	|	2130	|<span class="success">	Slight Increase @cite corcoran1943effects	</span>|<span class="success">	Steady @cite corcoran1943effects	</span>|<span class="success">	Slight Increase @cite corcoran1943effects	</span>|<span class="success">	Steady @cite corcoran1943effects</span>	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Renal/HemorrhageGFR.jpg" width="550"></td>
    <td><img src="./plots/Renal/HemorrhageMAP.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Renal/HemorrhageRBF.jpg" width="550"></td>
    <td><img src="./plots/Renal/HemorrhageArterioleResistance.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Renal/HemorrhageClass4NoFluidLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>Figure 16. In this scenario the patient is given a class IV hemorrhage, which causes changes in MAP as the bleeding progresses.
The top left figure shows the mean arterial pressure response to the hemorrhage. The top right and bottom left figures display the glomerular filtration and renal blood flow  
dropping with the pressure drop. This response is mitigated through dilation of the afferent arteriole, seen in the bottom right figure.</i>
</center><br>

### High Altitude - Action 

After a rapid change in altitude, epinephrine is released in response to the O2 deficit in the myocardium. This causes an increase in mean
arterial pressure, which triggers a renal tubuloglomerular feedback response. The afferent arteriole resistance increases in an attempt to maintain
GFR and renal blood flow. The MAP increases quickly and stabilizes at approximately 97 mmHg, which causes initial increases in GFR and %Renal
Blood Flow. These changes are quickly accounted for through the regulation from the tubuloglomerular feedback as expected.

Table 11 shows the effect seen in the %Renal system. For a complete write-up of the High Altitude scenario see the @ref EnergyMethodology.

<center>
<i>
Table 11. In this scenario, the patient environment is changed to simulate the effects of altitude. The acute response increases MAP,
as shown in the top-left plot. The bottom-right plot shows the afferent arteriole resistance increasing to accommodate the changes in input pressure. The other two plots show
the resulting affects on GFR and renal blood flow. Note the increases in renal blood flow and GFR just before 100 seconds, which
are attenuated as the afferent arteriole resistance increases.
</i>
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Glomerular Filtration Rate (mL/min)	|	%Renal Blood Flow(mL/min)	|	Urine Production Rate (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Initiate %Environment Change	|		|	50	|	900	|<span class="warning">	Slight Increase @cite guyton2006medical	</span>|<span class="warning">	Slight Increase @cite guyton2006medical	</span>|<span class="success">	Increase @cite guyton2006medical	</span>|


<center>
<table border="0">
<tr>
    <td><img src="./plots/Renal/AltitudeMAP.jpg" width="550"></td>
    <td><img src="./plots/Renal/AltitudeGFR.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Renal/AltitudeRBF.jpg" width="550"></td>
    <td><img src="./plots/Renal/AltitudeArterioleResistance.jpg" width="550"></td>
</tr>
<tr>
    <td colspan="2"><img src="./plots/Renal/HighAltitudeEnvironmentChangeLegend.jpg" width="1100"></td>
</tr>
</table>
</center>
<center><i>
Figure 17. In this scenario, the patient environment is changed to simulate the effects of altitude. The acute response increases MAP,
as shown in the top left figure. The top right and bottom left figures show the mechanical affects of this pressure change in the glomerular filtration rate and the renal blood flow, respectively.  The final figure shows the afferent arteriole resistance increasing by about 30% to accommodate the changes in input pressure. 
</i>
</center><br>

### Salt Ingestion - Action

The salt ingestion scenario simulates the patient drinking 1 L of seawater. Since the osmolarity of seawater exceeds the concentrating capacity of the
human renal system @cite hall2011guyton, this provides a relevant edge case that pushes the kidneys to the limit of their ability to clear the excess
sodium. A 30 minute condition is applied to allow some time to pass for digestion before the effects are analyzed. After the ingestion of seawater, there is a significant change 
in the concentration of sodium in the blood. Since %BioGears does not currently model the fluid shift seen from the intracellular to extracellular space in response 
to changes in osmolarity, this causes a drastic drop in urine production rate as seen in Table 12 @cite hall2011guyton. The resulting urine
becomes hyperosmotic relative to the plasma and contains a high concentration of sodium.

<center>
<i>
Table 12. Urine production rate stays steady. Kidney doesn't quite respond accordingly due to such slight sodium changes. Chronic salt loading causes much more pronounced changed in renal function in patients, something %BioGears isn't meant to handle now.
</i>
</center>
|	Actions	|	Notes	|	Occurrence Time (s)	|	Urine Production Rate (mL/min)	|	Mean Arterial Pressure (mmHg)	|	Urine Chloride Concentration (g/L)	|	Urine Urea Concentration (g/L)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	After stabilization patient given 1L of water with 100g NaCl 	|		|	0	|<span class="success">	Slight Increase @cite dean1949renal	</span>|<span class="success">	No Change @cite dean1949renal	</span>|<span class="Warning">	Increased Then Normal @cite dean1949renal	</span>|<span class="Warning">	Increase @cite dean1949renal	</span>|

### Water Ingestion - Action

The water ingestion scenario simulates the patient drinking 1 liter of water with no other nutrients included. This causes a lowering of blood plasma concentration. This reaction leads to a decrease in the permeability of the tubules leading to an increase in urine production rate. The renal system in %BioGears shows a marked increase of dilute urine in line with that seen in a patient @cite baldes1934effect. 

<center>
<i>
Table 13. Urine production rate and plasma sodium concentrations behave as expected. Bladder substance handling is still not quite in line with experimental results.
</i>
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Urine Production Rate (mL/min)	|	Mean Arterial Pressure (mmHg)	|	Plasma Sodium Concentration (mg/L)	|	Urine sodium concentration (mg/L)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	After stabilization patient given 1L of water	|		|	0	|	14400	|<span class="success">	Increase @cite baldes1934effect	</span>|<span class="success">	No Change @cite baldes1934effect	</span>|<span class="success">	Decrease @cite baldes1934effect	</span>|<span class="warning">	Decrease Then Slight increase @cite baldes1934effect	</span>|



### Dehydration
%BioGears does not currently support this functionality, although scenarios can be made and run for days at a time without fluid intake, simulating dehydration.

Dehydration conditions demonstrate the %Renal system's capability to conserve substances. As the blood volume is lowered, the production rate of urine is decreased
to a minimum by reabsorbing as much of the filtered water as possible. The outputs of the urine content are dependent on what food has been consumed during the dehydration
period. As the osmolarity of the plasma increases due to the drop in water volume, the renal system compensates by conserving as much water as possible @cite hall2011guyton.
This is accompanied by a drop in sodium excretion as it is reabsorbed to create a strong osmotic gradient to reabsorb the water. The renal methodology has no direct input in 
how dehydration is implemented, aside from clearing substances from the blood according to the time dictated by the consume meal condition and their clearance rates.

For other effects of dehydration on physiology see @ref EnergyMethodology.

<br><center>
<i>Table 14. %Renal behavior changes during dehydration. The excretion of substances slows as the water is conserved. There is also a notable
decrease in sodium excretion and urine sodium concentration in keeping with the validation data. </i>
</center>
|Parameter                    |Normal Values             |Dehydration              | 
|------------------------             |------------------------ |------------------------ |
|Urine Production Rate (mL/min))		  |	  1.1          | 0.07                |
|Urine Sodium Excretion Rate (mg/min) |		  2.4          |     0.65                  |
|Urine Sodium Concentration  (g/L)|        0.717      |       0.1           |

### Starvation
When nutrients from food are restricted, the kidneys attempt to conserve substances in the blood, which leads to decreases in urinary concentrations. This is accomplished 
by increasing reabsorptive capacity where it is possible to, namely with sodium. The %BioGears starvation scenario provides the patient with enough water to last the 4 day starvation 
period, which leads to decreasing concentrations of substances in the blood. The %renal methodology has no direct input in how starvation is implemented, aside from clearing substances
from the blood according to the time dictated by the consume meal condition.

For other effects of starvation on physiology see @ref EnergyMethodology .

<center>
<i>
Figure 15. There is inconsistent agreement with the validation detail in the %renal response to starvation. The middling responses in urine sodium,
calcium, and creatinine are due to an unvalidated drop in urine production rate, which skews the concentration values.
</i>
</center>
|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (days)	|	Urine Sodium Concentration (mmol/L)	|	Urine Calcium Concentration (mmol/L)	|	Creatine Concentration (mmol/L)	|	Urine Creatine Clearance (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	Patient is experiencing starvation	|		|	0	|	4	|<span class="warning">	Decrease @cite sapir1975ketones	</span>|<span class="warning">	Decrease @cite sapir1975ketones	</span>|<span class="success">	Increase @cite sapir1975ketones	</span>|<span class="success">	Decrease @cite sapir1975ketones	</span>|

### %Renal Stenosis - Condition

The %Renal Stenosis condition is validated against two different scenarios, testing severity and location of the condition. The moderate unilateral stenosis places a 60 percent severity on the left kidney and none on the right. The direct effect of a stenosis is a seen through a decrease in renal blood flow and glomerular filtration rate. Total vascular resistance and mean arterial pressure is increased because of the increased resistance on the renal artery. This causes a drop in cardiac output via the baroreceptor feedback response to the increased vascular resistance. The blood volume is steady in %BioGears due to a lack of angiotensin 2 and aldesterone hormonal responses.

<center>
<i>
Table 16. Direct affects of blood flow and glomerular filtration are seen in the stenosis condition. All cardiovascular effects meet validation except that blood volume decreases caused by release of aldesterone and angiotensin 2. 
</i>
</center>
|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Blood Volume (mL)	|	Systemic Vascular Resistance (mmHg/mL/s)	|	Cardiac Output (mL/min)	|	Mean Arterial Pressure (mmHg)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	%Renal Blood Flow (mL/min)	|	GFR (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	%Renal Stenosis 	|	60% unilateral occlusion of kidneys 	|	120	|<span class="warning">	Increase @cite klabunde2011cardiovascular	</span>|<span class="success">	Increase @cite klabunde2011cardiovascular	</span>|<span class="warning">	Mild decrease @cite anderson1990development	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Decrease @cite textor2011atherosclerotic	</span>|<span class="success">	Decrease @cite textor2011atherosclerotic	</span>|


The severe bilateral stenosis condition tests the effects of kidney function due to a 90 percent increase in renal artery resistance on both kidneys. Like the unilateral condition, the kidney and cardiovascular function meet validation. A more pronounced decrease in renal blood flow and glomerular filtration rate can be seen due to the increased severity of the stenosis. 

<center>
<i>
Table 17. There is good agreement in %BioGears in response to the 90% bilateral stenosis. The effect on pressure is significant enough
to meet validation. The blood volume is unaffected. The systemic vascular resistance increases as expected along with the corresponding 
decrease in cardiac output. The renal blood flow and GFR each decrease as expected.
</i>
</center>
|	Condition	|	Notes	|	Sampled Scenario Time (s)	|	Blood Volume (mL)	|	Systemic Vascular Resistance (mmHg/mL/s)	|	Cardiac Output (mL/min)	|	Mean Arterial Pressure (mmHg)	|	Systolic Pressure (mmHg)	|	Diastolic Pressure (mmHg)	|	%Renal Blood Flow (mL/min)	|	GFR (mL/min)	|
|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|	------------------------	|
|	%Renal Stenosis 	|	90% bilateral occlusion of kidneys 	|	120	|<span class="warning">	Increase @cite klabunde2011cardiovascular	</span>|<span class="success">	Increase @cite klabunde2011cardiovascular	</span>|<span class="warning">	Mild decrease @cite anderson1990development	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Increase @cite debrunye2006assessment	</span>|<span class="success">	Decrease @cite textor2011atherosclerotic	</span>|<span class="success">	Decrease @cite textor2011atherosclerotic	</span>|

@anchor renal-conclusions
Conclusions
-----------

%BioGears has implemented a functional renal model in conjunction with a whole-body physiology engine. Even with the simplified single-node nephron model, the system is able to accurately follow the overall renal transport of fluid and substances. The capabilities of the renal model allow for fluctuations in urine production rate, changes in blood pressure, and variations in clearance rates, as well as allowing for autogregulation with accurate responses to pressure changes and sodium concentration changes in the tubules.

@anchor renal-futurework
Future Work
===========

Coming Soon
-----------

These additions are expected in the near future:
- Acid-base balance reinvestigation
- Improvements to make the %Cardiovascular and %Renal systems more independent and modular
- Better handling of substance clearance

@anchor renal-recommended
Recommended Improvements
------------------------

These are some recommended improvements and additions:
- Peristalsis model
- Insulin as an optional substance for kidney validation/verification
- Remaining urinalysis substances
- Renin and angiotensin control
- Handle drugs similarly to metabolites
- Model kidney failure
- Catheterization
- Kidney stones
- Dialysis machine model

@anchor renal-appendices
Appendices
==========

Acronyms
--------

BUN - Blood Urea Nitrogen
	
GFR - Glomerular Filtration Rate

IV - Intravenous

MAP - Mean Arterial Pressure

RBC - Red Blood Cells

WBC - White Blood Cells

mL - Milliliters

min - Minutes

kg - Kilograms

g - Grams

ug - Micrograms

L - Liters

mol - Moles

mOsm - Milliosmoles

mmHg - Millimeters of Mercury

Data Model Implementation
-------------------------

@ref RenalSystemTable "Renal"

@anchor renal-compartments
Compartments
------------

* LeftKidney
	* LeftRenalArtery
	* LeftNephron
		* LeftAfferentArteriole
		* LeftEfferentArteriole
		* LeftGlomerularCapillaries
		* LeftBowmansCapsules
		* LeftTubules
	* LeftRenalVein
	
* RightKidney
	* RightRenalArtery
	* RightNephron
		* RightAfferentArteriole
		* RightEfferentArteriole
		* RightGlomerularCapillaries
		* RightBowmansCapsules
		* RightTubules
	* RightRenalVein
* Ureters
	* LeftUreter
	* RightUreter
* Bladder
