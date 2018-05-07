%Environment Methodology {#EnvironmentMethodology}
==========================

@anchor environment-overview
Overview
========

Abstract
--------
The %BioGears&reg; %Environment system contains and controls everything external to the patient and equipment (e.g. @ref AnesthesiaMachineMethodology). It sets the ambient boundary conditions for both fluid and thermal systems. The %Environment system provides a means to add airborne substance values, and calculates the heat transfer due to convection (natural and forced), radiation, and evaporation.

@anchor environment-intro
Introduction
------------
The %BioGears %Environment system defines the ambient conditions for the %BioGears body. The %Environment system controls everything from the phase of the environmental fluid (i.e. gas when is a room or liquid when submerged in water) to the volume fractions of constituent gases in the environmental fluid when that fluid is a gas. The %Environment system also computes heat exchange between the body and the environment as well as and substance delivery to the body-environment interfaces.

@anchor environment-system-design
System Design
=============
Background and Scope
--------------------
The many physiological functions within the human body depend heavily on the conditions of the environment within which the body is located. Thus, the %BioGears %Environment system is required to determine patient boundary conditions to enable a generic, extensible approach to modeling physiology within the body. Though the @ref CDM (CDM), the %Environment system interacts with and informs other %BioGears systems, particularly the [Respiratory](@ref RespiratoryMethodology), [Cardiovascular](@ref CardiovascularMethodology), and [Energy](@ref EnergyMethodology) systems, and the %Environment system was developed to include hooks for future integration into various applications such as games and sensor systems.

### Requirements
The %Environment system is used to meet the following requirements in %BioGears:
- Meteorological effects
- Surrounding atmospheric gas properties, including pulmonary agent exposure
- Altitude effects
- Heat transfer into and out of the skin and lungs
- Clothing resistances
- Ambient temperature changes
- Ambient humidity changes
- Multiple surrounding fluid types (i.e. submerged in water vs. air)
- Air velocity effects on convection
- Solar and surrounding material (i.e. walls) considerations
- Active heating and cooling

### Existing Research
One of the most comprehensive analysis of human interaction with typical thermal environments is contained within the American Society of Heating, Refrigerating, and Air-Conditioning Engineers (ASHRAE) Handbook - specifically the Thermal Comfort chapter @cite handbook2013fundamentals. The International Organization for Standardization (ISO) standard 7730:2005: Ergonomics of the Thermal %Environment contains additional data and analyses @cite ISOPDF. Both of these publications are focused on occupant thermal comfort for indoor environment planning and thus make assumptions that are not valid for some of the extreme environments required for %BioGears. Data and analyses from these publications were heavily leveraged during development of the %BioGears environment system. Other existing research includes the high-level heat transfer analysis focused on medical and biological applications reported in Ref. @cite shitzer1985heat, as well as experimental and computational explorations of thermoregulation @cite mccutchan1951respiratory @cite oliveira2008measuring @cite de1997convective @cite fiala1999computer.

### Approach
The %Environment system was designed with as few assumptions and parameter bounds as possible to allow simulations of both standard and extreme environments. The system is dynamic. By leveraging the@ref CDM and the generic [circuit](@ref CircuitMethodology) and [convective transport](@ref SubstanceTransportMethodology) solvers, the %BioGears body will react differently to varying initial environments and to changes in the environment during simulation.  %Environment parameters can be changed through specifying a separate environment file, or by manually updating the parameters through the environment state change action (for more details see the @ref SDK). Table 1 shows these inputs, which set the substance, fluid, and thermal properties external to the patient.

<br><center>
<i>Table 1. The parameters that can be set using the %Environment condition.  Note that air density is a calculated value inside the class.</i>
</center>

| Parameter | Use | Unit Type |
| :----     | :----   | :----   |
| Air Density | Thermal | MassPerVolume |
| Air Velocity | Thermal | LengthPerTime |  
| Ambient Temperature | Thermal | Temperature |
| Atmospheric Pressure | Fluid | Pressure |
| Clothing Resistance | Thermal | HeatResistanceArea |
| Emissivity | Thermal | Fraction |
| Mean Radiant Temperature | Thermal | Temperature |
| Relative Humidity | Thermal | Fraction |
| Respiration Ambient Temperature | Thermal | Temperature | 
| Ambient Substance| Substance | Fraction |

The Thermal Application action is handled by the %Environment system and has the following options (with unit type):

- Active Heating
	- Heating Power (Power)
	- Heated Surface Area Fraction (Fraction)
	- Heated Surface Area (Area)
- Active Cooling
	- Cooling Power (Power)
	- Cooled Surface Area Fraction (Fraction)
	- Cooled Surface Area (Area)
- Applied Temperature
	- Applied Temperature (Temperature)
	- Applied Surface Area Fraction (Fraction)
	- Applied Surface Area (Area)

### Substance %Environment

Ambient substance volume fractions can be modified through the %Environment system. The environmental fraction of typical atmospheric gases (e.g. O2, CO2, N2, etc.) and concentrations of airborne agents in suspension (i.e. aerosols) can be set and/or dynamically adjusted. The gases and aerosols enter the @ref RespiratoryMethodology "Respiratory System" with the generic convective @ref SubstanceTransportMethodology.

### Fluid %Environment

Many meteorologic parameters such as ambient temperature, wind speed, atmospheric pressure, and humidity are set and used by the %Environment system.  The atmospheric pressure is set directly on the %Respiratory and Anesthesia Machine reference (ambient) node values, which automatically propagates effects throughout.  The other thermal values are used to determine the %Environment circuit element values described in more detail below.

### Thermal %Environment

The remaining parameters are used for setting and manipulating the surrounding thermal environment.  A simplified one-dimensional representation of the thermodynamic process solved by the %Environment system is shown in Figure 1 @cite handbook2013fundamentals .  The %Environment circuit connects directly to the %Energy system circuit to balance with metabolic rate and body heat storage.

@image html Environment2DModel.png
<center>
<i>Figure 1. Human thermoregulation in an environment can be shown with a simplified one dimensional model.  The net heat production is transferred to the environment through the skin surface and respiratory tract.</i>
</center><br>


Data Flow
---------
### Initialize

During resting stabilization, the patient reaches a homeostatic value using the standard environment. Following that steady-state starting point, the user has the option to modify any environment values as a condition. The simulation will not start running until a new steady-state is achieved using these new parameters.

### Preprocess

Preprocess uses feedback to calculate thermal properties and circuit element values for the next engine state.  The Preprocess methods are described in more detail in the next section.

### Process

The generic circuit methodology developed for the %BioGears Engine is used to solve for the pressure, flow, and volume at each node or path. For more details, see the @ref CircuitMethodology.

### Post Process

The Post Process step moves everything calculated in Process from the next time step calculation to the current time step calculation. This allows all other systems access to the information when completing their Preprocess analysis for the next time step.

<center><img src="./images/Environment/EnvironmentFlow.png" width="900"></center>
<center>
<i>Figure 2. The data flow for the %Environment System consists of Preprocess, Process, and Post Process. Preprocess determines the circuit element values based on feedback mechanisms and engine actions. Preprocess also updates system-level quantities via the Calculate.  Process uses generic circuit methodology to solve the %Environment thermal circuit for temperatures and heat flows along the paths and on nodes. Post Process updates these quantities to the next time step and then begins advancing time, which causes the loop to repeat.</i>
</center><br>


@anchor environment-features
Features and Capabilities
-------------------------
### Definitions
Acronyms and a nomenclature table are available in the [appendix](@ref environment-appendices).

### Circuit
The one-dimensional model shown in Figure 1 is further simplified to a zero-dimensional model (Figure 3) by averaging the thermal effects over the entire patient.  The %Environment system uses the generic circuit solver in the same way as fluid systems (@ref CircuitMethodology).

The Ambient node contains both thermal properties (temperature), fluid properties (pressure), and substance properties (volume fraction, aerosol concentration).  It is assigned as the %Respiratory and Anesthesia Machine circuit&rsquo;s reference node, and therefore, interacts with them directly.  Any changes to the Ambient node properties automatically propagates through the other systems.

@image html EnvironmentCircuit.png
<center>
<i>Figure 3. The %Environment circuit consists of 6 nodes that are connected via 8 paths. There are 8 parameters representing circuit elements that are modified by feedback mechanisms or actions. The circuit is used to estimate all environment thermal properties each time step.</i>
</center><br>

The clothing thermal insulation/resistance in this model is an average lumped value over the entire patient's body.  Because clothing insulation cannot be measured for most routine engineering application, tables of measure values for various ensembles can be used as in Table 2 @cite handbook2013fundamentals.  When a premeasured ensemble cannot be found to match, estimated ensemble insulation can be determined via a summation of the individual garmets as in Table 3 @cite handbook2013fundamentals.  An important note is that the clothing resistance remains constant once set, and does not automatically take into account sweat saturation or changes due to submersion.  These effects could be accounted for by external determination of a new thermal resistance value and set using the environment change action.

<br><center>
<i>Table 2. Typical insulation for clothing ensembles.</i>
</center>
| Ensemble Description | Icl (clo) |
| :----     | :----   |
| Walking shorts, short-sleeved shirt | 0.36 |
| Trousers, short-sleeved shirt | 0.57 |
| Trousers, long-sleeved shirt | 0.61 |
| Same as above, plus suit jacket | 0.96 |
| Same as above, plus vest and T-shirt | 0.96 |
| Trousers, long-sleeved shirt, long-sleeved sweater, T-shirt | 1.01 |
| Same as above, plus suit jacket and long underwear bottoms | 1.30 |
| Sweat pants, sweat shirt | 0.74 |
| Long-sleeved pajama top, long pajama trousers, short 3/4 sleeved robe, slippers (no socks) | 0.96 |
| Knee-length skirt, short-sleeved shirt, panty hose, sandals | 0.54 |
| Knee-length skirt, long-sleeved shirt, full slip, panty hose | 0.67 |
| Knee-length skirt, long-sleeved shirt, half slip, panty hose, long-sleeved sweater | 1.10 |
| Knee-length skirt, long-sleeved shirt, half slip, panty hose, suit jacket | 1.04 |
| Ankle-length skirt, long-sleeved shirt, suit jacket, panty hose | 1.10 |
| Long-sleeved coveralls, T-shirt | 0.72 |
| Overalls, long-sleeved shirt, T-shirt | 0.89 |
| Insulated coveralls, long-sleeved thermal underwear, long underwear bottoms | 1.37 |

<br><center>
<i>Table 3. Typical garment insulation.</i>
</center>
| Garment Description | Icl (clo) | Garment Description	 | Icl (clo) |
| :----     | :----   | :----   | :----   |
| **Underwear** |  | **Dress and Skirts** |  |
| Bra | 0.01 | Skirt (thin) | 0.14 |
| Panties | 0.03 | Skirt (thick) | 0.23 |
| Men's briefs | 0.04 | Sleeveless, scoop neck (thin) | 0.23 |
| T-shirt | 0.08 | Sleeveless, scoop neck (thick), i.e., jumper | 0.27 |
| Half-slip | 0.14 | Short-sleeve shirtdress (thin) | 0.29 |
| Long underwear bottoms | 0.15 | Long-sleeve shirtdress (thin) | 0.33 |
| Full slip | 0.16 | Long-sleeve shirtdress (thick) | 0.47 |
| Long underwear top | 0.20 |  |  |
| **Footwear** |  | **Sweaters** |  |
| Ankle-length athletic socks | 0.02 | Sleeveless vest (thin) | 0.13 |
| Pantyhose/stockings | 0.02 | Sleeveless vest (thick) | 0.22 |
| Sandals/thongs | 0.02 | Long-sleeve (thin) | 0.25 |
| Shoes | 0.02 | Long-sleeve (thick) | 0.36 |
| Slippers (quilted, pile lined) | 0.03 |  |  |
| Calf-length socks | 0.03 | **Suit Jackets and Vests (lined)** |  |
| Knee socks (thick) | 0.06 | Sleeveless vest (thin) | 0.10 |
| Boots | 0.10 | Sleeveless vest (thick) | 0.17 |
| **Shirts and Blouses** |  | Single-breasted (thin) | 0.36 |
| Sleeveless/scoop-neck blouse | 0.12 | Single-breasted (thick) | 0.44 |
| Short-sleeve knit sport shirt | 0.17 | Double-breasted (thin) | 0.42 |
| Short-sleeve dress shirt | 0.19 | Double-breasted (thick) | 0.48 |
| Long-sleeve dress shirt | 0.25 |  |  |
| Long-sleeve flannel shirt | 0.34 | **Sleepwear and Robes** |  |
| Long-sleeve sweatshirt | 0.34 | Sleeveless short gown (thin) | 0.18 |
| **Trousers and Coveralls** |  | Sleeveless long gown (thin) | 0.20 |
| Short shorts | 0.06 | Short-sleeve hospital gown | 0.31 |
| Walking shorts | 0.08 | Short-sleeve short robe (thin) | 0.34 |
| Straight trousers (thin) | 0.15 | Short-sleeve pajamas (thin) | 0.42 |
| Straight trousers (thick) | 0.24 | Long-sleeve long gown (thick) | 0.46 |
| Sweatpants | 0.28 | Long-sleeve short wrap robe (thick) | 0.48 |
| Overalls | 0.30 | Long-sleeve pajamas (thick) | 0.57 |
| Coveralls | 0.49 | Long-sleeve long wrap robe (thick) | 0.69 |

@anchor environment-aerosol
### Aerosol
An aerosol is a colloidal suspension of either solid or liquid droplet particles suspended in a gas (typically the surrounding air). Real aerosols can be monodispersed but are more often polydispersed, having a range of particle diameters. Particle deposition and retention in the lung depends on a number of, factors including particle aerodynamic diameter @cite carvalho2011influence @cite newman1985aerosol. Other deposition factors, including inhalation flow rate and tidal volume @cite kolanjiyil2016computationally, are accounted for by the breathing patterns simulated by the %BioGears engine. In order to avoid the computational cost of tracking millions of individual particles or many groups of particles in a distribution throughout the respiratory tract and into the body, %BioGears aerosols are modeled as a single concentration in the environment. The location of deposition and the deposition efficiency at each location is determined by first computing a particle-size-independent deposition efficiency for each compartment from a defined size-distribution histogram. The size-independent deposition efficiencies are computed from the deposition fractions which are determined for each compartment from the particle diameter using the equations fit by Hinds to the ICRP66 simulation results @cite hinds1999aerosol, as described by @cite rostami2009computational. The deposition fraction equations from @cite rostami2009computational are plotted in Fig. 4, and example mean depositions fractions for a particular histogram are shown. For the %BioGears implementation it is assumed that all particles are spherical, thus diameter is the aerodynamic diameter.

@image html depositionfractions.png
<center>
<i>Figure 4. The deposition fraction in each compartment is computed as a function of particle size, and the deposition fraction for a collection of particles within a histogram bin is computed from the mean deposition fraction within the boundaries of the bin. An 8 bin partition is shown in the figure for illustration purposes. %BioGears supports histograms with an arbitrary number of bins.</i>
</center><br>

The size-independent deposition efficiencies are computed from the deposition fractions using Equation 31.
@anchor SIDE
<center>
\f[\mathrm{K_B=}\frac{\left( b_1f_1 + \cdots + b_nf_n \right)}{\left( \left( 1-a_1 \right)f_1 + \cdots + \left(1-a_n \right)f_n \right)} \f]
*Equation 1.*
</center><br>

Here *K<sub>B</sub>* is the size independent deposition efficiency coefficient for compartment *B*, *b<sub>i</sub>* is the fraction of total particles of size *i* entering the body that deposit in compartment *B*, and *f<sub>i</sub>* is the fraction of total particles in the environment that are of size *i*. Likewise, *a<sub>i</sub>* is the fraction of total particles of size *i* entering the body that deposit in compartment *A*. This equation is derived by first considering a polydispersed aerosol with particles divided by size into *n* number of bins. The total mass of particles in an aliquot of to-be-inhaled air prior to inhalation is equal to the sum of the masses of the particles in each bin, as described by Equation 2.
<center>
\f[m_t = \Sigma_{i=1}^{n}m_i \f]
*Equation 2.*
</center><br>

The mass of the particles in each bin must be defined by a fraction specified in a histogram. The size of the histogram is arbitrary. The only requirement is that a histogram is supplied rather than a density function. Equation 3 describes the mass of particle *i* within the aliquot of aerosol.
<center>
\f[m_i = f_im_t \f]
*Equation 3.*
</center><br>

Likewise, the total mass of particles in compartment *A* is equal to the sum of the masses of the particles in each bin, and the same is true for compartments *B*, *C*, *D*, etc., described by Equation 4.
<center>
\f[m_{tA} = \Sigma_{i=1}^{n}m_{iA} \f]
*Equation 4.*
</center><br>

The mass of particles in compartment *B* is also equal to the mass of the particles that did not deposit in the adjacent upstream compartment *A*.
<center>
\f[m_{iB} = \left(1-a_i \right)m_i \f]
*Equation 5.*
</center><br>

Combining Equations 2-5 yields Equation 6.
<center>
\f[m_{iB} = \left( \left(1-a_1 \right)f_1 + \cdots + \left(1-a_n \right)f_n \right)m_t \f]
*Equation 6.*
</center><br>

The total mass of particles that deposit in compartment *B* is some fraction of the total particles in compartment *B*, which is the sum of the amount deposited from each bin in compartment *B*.
<center>
\f[K_{B}m_{tB} = \left(b_1f_1 + \cdots + b_nf_n \right)m_t \f]
*Equation 7.*
</center><br>

Rearranging Equation 7 and combining with Equation 6 yields [Equation 1](@ref SIDE), the size-independent deposition efficiency coefficient for compartment *B*.

The mass deposited in each compartment is computed by multiplying the size-independent deposition efficiency coefficient by the mass in the compartment at each time step. Once an aerosol substance deposits in a respiratory compartment it stays in the respiratory compartment. In other words, coughing is not productive in %BioGears. The direct effect of a deposited substance depends on the mass deposited and the value of the inflammation coefficient that is defined in the substance file. The inflammation coefficient defines the amount of damage, by any mode, that the substance does to the respiratory tissue. Figure 5 visually describes the direct effects of an aerosol on the %BioGears %Respiratory system.

<img src="./images/Environment/AerosolEffects.png" width="700">
<center>
<i>Figure 5. Diagram describing aerosol transport into the respiratory compartments and the associated effects.</i>
</center><br>

In addition to the direct effects, deposited aerosol substances can also have indirect effects. After liquid aerosol substances deposit they can diffuse into the surrounding tissue and then into the blood stream. If the liquid aerosol has pharmacodynamic properties defined, it's effects will be realized when it enters the blood and the plasma concentration becomes non-zero.

Currently, Albuterol and smoke particulate have histogram data and are modeled as polydisperse aerosols in the environment with the former being delivered via an inhaler and the latter being a concentration in the ambient environment. Smoke in %BioGears is modeled as a solid aerosol.

The solid smoke particulate in %BioGears is a model of the particulate generated through the combustion of organic material in a forest fire. We assume that the fire is well evolved in time to mitigate the bi-modal behavior seen during the initial ignition and smoldering @cite zhang2012chemical. This allows us to model the size distribution as a lognormal curve. The forest fire smoke particle size distribution histogram is given in table 4.

<br><center>
<i>Table 4. Histogram for wood smoke particulate in the environment. </i>
</center>
| Amount (fraction) | Diameter (microns) |
| ----------------- |------------------- |
| 0.015 | 1.0e-4 - 1.0e-3 | 
| 0.035 | 1.0e-3 - 1.0e-2 |
| 0.9   | 1.0e-2 - 1.0e-1 |
| 0.035 | 1.0e-1-1.0      |
| 0.015 | 1.0-1.0e1       |
| 0     | 1.0e1 - 1.0 e2  |

Figure 6 summarizes the fate and effects of liquid and smoke aerosols in %BioGears.
<img src="./images/Environment/AerosolFate.png" width="700">
<center>
<i>Figure 6. Diagram summarizing the transport into the respiratory compartments, effects, and the fate of liquid and solid particle aerosols.</i>
</center><br>

@anchor environment-carbon-monoxide
### Carbon Monoxide
Carbon monoxide (CO) is a toxic gas that is odorless, colorless, and tasteless. Carbon monoxide binds to the ferrous-heme site of the hemoglobin molecule in the same way that oxygen does (it also cannot bind to the oxidized methemoglobin sites), thus the maximal oxygen capacity falls to the extent that CO is bound. The subunits of the hemoglobin molecule are collectively in a tense state (T) or a relaxed state (R). All four subunits are always in the same state. The hemoglobin molecule is in the tense state when less oxygen is bound and it has a lower affinity for oxygen in the T state. When enough oxygen is bound, all subunits snap into the R state whether or not they are bound to oxygen, and the hemoglobin molecule has ~150 times the affinity for oxygen. It is this snapping that creates the steep portion of the binding curve and allows for the reversible carriage of oxygen by the blood. Like oxygen, as carbon monoxide binds to hemoglobin it can push the molecule from the relaxed to the tensed state, thus increasing the affinity for oxygen and reducing the amount of oxygen released from the hemoglobin molecule when it enters the oxygen depleted systemic capillaries. Additionally, carbon monoxide binds competitively to hemoglobin. For every molecule of CO bound to a heme site, there is one less site for oxygen to bind. The net result of these two phenomena, but predominately due to the former, is that there is less oxygen delivery to the tissues when CO is present in the blood.

There are three components to the %BioGears carbon monoxide model: transfer across the alveolar membrane, binding to hemoglobin, and the effects of binding on the oxygen binding curve. Obviously these three components are interrelated. As CO binds to hemoglobin in the pulmonary capillaries, less CO is dissolved in fluid and therefore the partial pressure gradient between the alveoli and the pulmonary capillary blood is higher, driving an increase in Fick's law diffusion resulting in more carbon monoxide in the blood. As more CO enters the blood, more binds to hemoglobin and the oxygen-hemoglobin saturation curve shifts to the left. The alveolar transfer of carbon monoxide is simulated in %BioGears by assuming that the membrane diffusivity of CO is similar to oxygen and then by leveraging the alveolar exchange model described in the @ref TissueMethodology report. Models for the binding of CO to hemoglobin and the effects on the oxygen saturation curve are discussed separately below.

#### Carbon Monoxide Binding
The distribution of carbon monoxide species (dissolved and bound to hemoglobin) in each vascular compartment is computed from the conservation of mass and the Haldane relationship described in @cite bruce2003multicompartment. Canservation of mass is described by Equation 8.
<center>
\f[T_{CO} = CO_{Hb} + k_{CO}P_{CO} \f]
*Equation 8.*
</center><br>
where *T<sub>CO</sub>* is the total carbon monoxide in the blood, *CO<sub>Hb</sub>* is the carbon monoxide bound to hemoglobin, *P<sub>CO</sub>* is the partial pressure of carbon monoxide in the blood, and *k<sub>CO</sub>* is the Henry's Law solubility coefficient for carbon monoxide. Equation 9, the Haldane relationship, provides the second equation necessary to solve for the two unknown values. In Equation 9, *M<sub>H</sub>* is the Haldane affinity ratio for hemoglobin, *P<sub>gas</sub>* is the partial pressure of the gas, and *Gas<sub>Hb</sub>* is the amount of gas bound to hemoglobin.
<center>
\f[M_H \frac{P_{CO}}{CO_{Hb}} = \frac{P_{O_2}}{O_{2_{Hb}}} \f]
*Equation 9.*
</center><br>
The model is implemented by first totaling the carbon monoxide in a compartment post diffusion, then computing the partial pressure of carbon monoxide, and finally computing the carboxyhemoglobin in the compartment. After the target distribution is calculated, the hemoglobin species (unbound, oxyhemoglobin, carboxyhemoglobin, carbaminohemoglobin, and oxycarbaminohemoglobin) amounts are updated by assuming that unbound hemoglobin is consumed first followed by oxyhemoglobin then oxycarbaminohemoglobin and finally carbaminohemoglobin. If all hemoglobin is converted to carboxyhemoglobin before the necessary adjustment is made to conserve CO mass, then the remaining CO is distributed back to the dissolved species. This acts as an automatic negative feedback device to ensure that the perfusion contribution to the diffusing capacity is saturated when the hemoglobin becomes saturated. See the [C++ code](@ref CalculateCarbonMonoxideSpeciesDistribution) for more details.

#### Carbon Monoxide Effects on Oxygen Saturation Curve
The oxygen saturation curve effects model implemented in %BioGears is adapted from the regression model described by @ref bruce2003multicompartment. We have simplified the model by assuming a linear relationship between carboxyhemoglobin and both the Hill coefficient and the 50% saturation shaping parameter. We then applied the linear relationship to the Dash and Bassingthwaithe oxygen saturation model already implemented in %BioGears (see the [Tissue Methodology](@ref tissue-approach)). Equations 10 and 11 describe the adjustments made to the oxygen saturation curve in the presence of carboxyhemoglobin, and Figure 7 shows the shift in the curve at various carboxyhemoglobin concentrations. In the equations, *&eta;* is the Hill coefficient, *S<sub>CO</sub>* is the fraction of carboxyhemoglobin to total hemoglobin (i.e. CO saturation), and *P<sub>50</sub>* is the standard partial pressure of oxygen at 50% oxygen saturation.
<center>
\f[\eta = 1.7 - 1.1 \cdot S_{CO} \f]
*Equation 10.*
</center><br>
<center>
\f[P_{50} = 26.8 - 20 \cdot S_{CO} \f]
*Equation 11.*
</center><br>

@image html leftShiftCO.png
<center>
<i>Figure 7. Oxygen concentration at partial pressure of oxygen in the blood with carboxyhemoglobin present. The dashed lines show what the curves would look like without the characteristic leftward shift.</i>
</center><br>

@anchor environment-dependencies
### Dependencies

The %Environment circuit is directly connected through the skin and core to the %Energy system for full body thermoregulation.  This boundary condition feedback in and out is automatically handled through the node and path interactions. It also shares an ambient node with the %Respiratory and Anesthesia Machine systems.

The %Environment system relies on inputs from several other systems.  The sweat rate from the %Energy system is needed to determine the amount of heat lost through convection and evaporation.  The respiration rate from the respiratory system is needed to determine the amount of heat lost via the respiratory track.

### Supplemental Values

Numerous patient and thermal properties are determined and set in the supplemental values method for later calculations.

Since the one dimensional model used for the %Environment thermal circuit needs to be averaged over the patient&rsquo;s skin surface area, an approximate value must be determined.  The most widely used equation to determine body surface area without direct measurement is the Du Bois formula presented in Equation 12 @cite handbook2013fundamentals , where mass is in kg and height is in m.

\f[{A_d}{\rm{ = 0}}.{\rm{202}}{m^{{\rm{0}}.{\rm{425}}}}{l^{{\rm{0}}.{\rm{725}}}}\f]
<center>
*Equation 12.*
</center><br>

Several properties of water are needed at varying temperatures to determine the heat transfer occurring between the skin/clothing/lungs and the air.  Four of these are determined using the best fit of property tables or experimental data, as shown in Figure 8.

@image html EnvironmentWaterPropertyGraphs.png
<center>
<i>Figure 8. These four properties of water are intermediate values needed for further calculations.  A, B, and C are plotted from data presented in a table @cite chase1985journal , while D is reproduced from data in a plot given by experimental data(d) @cite Cordes2014Heat .  Note in B that 1 Centipoise is equal to 0.001 N-s/m<sup>2</sup>.</i>
</center><br>

The first three properties (Figure 8 A-C) are used later for determining the heat transfer when the patient is submerged in water.  Finding a best fit for these plots yields:

\f[{\mathrm{c}}_{\mathrm{p,w}}=-1e^{-7}{\mathrm{T}}^{\mathrm{3}}_{\mathrm{\infty }}+3e^{-5}{\mathrm{T}}^{\mathrm{2}}_{\mathrm{\infty }}-0.0018{\mathrm{T}}_{\mathrm{\infty }}+4.2093\f]
<center>
*Equation 13.*
</center><br>

\f[\mu \mathrm{\ }=-3e^{-6}{\mathrm{T}}^{\mathrm{3}}_{\mathrm{\infty }}+0.0006{\mathrm{T}}^{\mathrm{2}}_{\mathrm{\infty }}-0.0462{\mathrm{T}}_{\mathrm{\infty }}+1.7412\f]
<center>
*Equation 14.*
</center><br>

\f[\mathrm{\alpha }\mathrm{\ }=6e^{-7}{\mathrm{T}}^{\mathrm{3}}_{\mathrm{\infty }}+0.0001{\mathrm{T}}^{\mathrm{2}}_{\mathrm{\infty }}+0.016{\mathrm{T}}_{\mathrm{\infty }}-0.0632\f]
<center>
*Equation 15.*
</center><br>

The heat of vaporization (Figure 8 D) is the enthalpy change required to transform a given quantity of a substance from a liquid into a gas and is needed for determining the heat lost through respiration.  Finding a piecewise best fit yields:

\f[{\mathrm{h}}_{\mathrm{fg}}\mathrm{=}\left\{ \begin{array}{c}
-0.10042t^2_a+22.173t_a+46375,~t_a\mathrm{\le }\mathrm{560K} \\ 
-1.5625t^2_a+1662.5t_a-\mathrm{414000,}~t_a\mathrm{>560K} \end{array}
\right.\f]
<center>
*Equation 16.*
</center><br>

The water vapor pressure at the skin and in ambient air is determined using the piecewise Antoine Equation given by Equation 17 @cite antoine1888tensions .  The resulting pressure is in units of mmHg and is a function of temperature in degrees Celsius. The constants are given by Table 5.

\f[p\mathrm{=}{\mathrm{10}}^{\left(A\mathrm{-}\frac{B}{C\mathrm{+}t}\right)}\f]
<center>
*Equation 17.*
</center><br>

<br><center>
<i>Table 5. Antione equation constants are used to calculate the water vapor pressure.</i>
</center>
| A       | B       | C       | t min (degrees C) | t max (degrees C) |
| :----   | :----   | :----   | :----             | :----             |
| 8.07131 | 1730.63 | 233.426 | 1                 | 100               |
| 8.14019 | 1810.94 | 44.485  | 99                | 374               |

The density of air is then determined using Equations 18-20 @cite Shelquist2015introduction .

\f[p_d\mathrm{=}p_t\mathrm{-}p_v\f]
<center>
*Equation 18.*
</center><br>

\f[p_v\mathrm{=}\mathrm{\phi }p_a\f]
<center>
*Equation 19.*
</center><br>

\f[\rho \mathrm{=}\frac{p_dM_d\mathrm{+}p_vM_v}{Ut_a}\f]
<center>
*Equation 20.*
</center><br>

The Lewis Relationship is the ratio of mass transfer coefficient to heat transfer coefficient, which is needed later in the determination of the heat loss due to evaporation.  For the air-water system with low concentration of water vapor, it is assumed the adiabatic saturation temperature and wet-bulb temperature are equal @cite Hewitt2015LEWIS , and is therefore given by:

\f[\beta \mathrm{=}\frac{\mathrm{1}}{\rho c_p}\f]
<center>
*Equation 21.*
</center><br>

### Radiation

The radiation heat transfer coefficient is used to determine the radiation resistance element value in the %Environment thermal circuit.  Equation 22 is implemented to determine this coefficient each time step.  The mean radiant temperature is a known value set by the user, and the clothing temperature is solved directly by the circuit and used as a feedback mechanism.  The ratio of the effective area to the surface area (Ar/Ad) is set constant to 0.73, which is typical for a standing person.  In contrast, 0.70 is typical for a sitting person @cite handbook2013fundamentals .  

\f[{\mathrm{h}}_r\mathrm{=4}\varepsilon \sigma \frac{A_r}{A_D}{\left(\frac{t_{cl}\mathrm{+}t_{mr}}{\mathrm{2}}\right)}^{\mathrm{3}}\f]
<center>
*Equation 22.*
</center><br>

Figure 9 highlights which elements are modified by the radiation method and by which parameters.

@image html EnvironmentRadiationCircuit.png
<center>
<i>Figure 9. Elements that are directly affected by the radiation calculations are boxed and labeled with parameters that cause them to change.</i>
</center><br>

### Convection

The equation chosen to account for convective effects comes from a study that used experimental data from a segmented manikin in a climate chamber @cite de1997convective .  A general-purpose forced convection equation was produced that is suitable for application to both seated and standing postures.  The air velocity is a known value set by the user, taking into account the combination of wind and patient movement.  The convective heat transfer coefficient is given by:

\f[{\mathrm{h}}_c\mathrm{=10.3}v^{\mathrm{0.6}}\f]
<center>
*Equation 23.*
</center><br>

Figure 6 highlights which elements are modified by the convection method and by which parameters.

@image html EnvironmentConvectionCircuit.png
<center>
<i>Figure 10. Elements that are directly affected by the convection calculations are boxed and labeled with parameters that cause them to change.</i>
</center><br>

### Evaporation

Evaporative heat loss is directly solved and implemented in the circuit as a heat sink on the skin.  It depends on the amount of moisture on the skin and the difference between the water vapor pressure at the skin and the ambient environment.  The evaporative heat transfer coefficient for the outer air layer of a nude or clothed person can be estimated from the convective heat transfer coefficient using the Lewis Relationship (Equation 24) @cite handbook2013fundamentals.  

\f[{\mathrm{h}}_e\mathrm{=}\beta {\mathrm{h}}_c\f]
<center>
*Equation 24.*
</center><br>

Clothing can then be taken into account to determine the maximum evaporative potential @cite kenny1998encyclopaedia :

\f[F_{pcl}\mathrm{=}\frac{\mathrm{1}}{\mathrm{1+2.}\mathrm{22}{\mathrm{h}}_cR_{cl}}\f]
<center>
*Equation 25.*
</center><br>

\f[E_{max}\mathrm{=}{\mathrm{h}}_eF_{pcl}\left(p_{sk,s}\mathrm{-}p_a\right)\f]
<center>
*Equation 26.*
</center><br>

Skin wettedness is an important factor to determine evaporative heat loss @cite handbook2013fundamentals :

\f[w_{rsw}\mathrm{=}\frac{E_{rsw}}{E_{max}}\f]
<center>
*Equation 27.*
</center><br>

\f[E_{dif}\mathrm{=}\left(\mathrm{1-}w_{rsw}\right)\mathrm{0.06}E_{max}\f]
<center>
*Equation 28.*
</center><br>

Evaporative heat loss by sweating is directly proportional to the rate of sweat generation through Equation 29 @cite handbook2013fundamentals .  The sweat rate is set by the %Energy system.

\f[E_{rsw}\mathrm{=}{\dot{m}}_{rsw}{\mathrm{h}}_{fg}\f]
<center>
*Equation 29.*
</center><br>

\f[E_{sk}\mathrm{=}E_{rsw}\mathrm{+}E_{dif}\f]
<center>
*Equation 30.*
</center><br>

Figure 11 highlights which elements are modified by the evaporation method and by which parameters.

@image html EnvironmentEvaporationCircuit.png
<center>
<i>Figure 11. The evaporation flow source is directly calculated by the evaporation calculations, which is boxed in the circuit diagram.</i>
</center><br>

### Respiration

%Respiratory heat exchange is taken into account with varying temperatures and humidity of inspired air.

Note that this calculation will be the same for both surrounding fluids&mdash;air and water.  It is assumed that the patient&rsquo;s head remains above water when breathing.  If the patient holds their breath to fully submerge, the respiration rate should be zero, which will correctly eliminate any heat transfer through the lungs.

The unitless specific humidity (mass/mass) is needed to determine the inspiratory-expiratory humidity difference.  Equation 31 solves for this knowing the ambient temperature (in Kelvin), relative humidity, and local atmospheric pressure @cite david2011humidity&mdash;all user defined values.

\f[\mathrm{H=}\frac{\mathrm{100}\mathrm{\phi }}{\mathrm{0.263}p_t}e^{\left(\frac{\mathrm{17.67}\left(t_a\mathrm{-}\mathrm{273.16}\right)}{t_a\mathrm{-}\mathrm{29.65}}\right)}\f]
<center>
*Equation 31.*
</center><br>

The humidity difference is then given by (ta in degrees F) @cite mccutchan1951respiratory :

\f[\mathrm{\Delta }W\mathrm{=0.02645+0.0000361}t_a\mathrm{-}\mathrm{0.798}H\f]
<center>
*Equation 32.*
</center><br>

Since heat and water vapor are transferred in the lungs, expired air is saturated with water vapor at the temperature of the respiratory tract @cite shitzer1985heat .  The respiration rate is given by the %Respiratory system and used to determine the following heat loss:

\f[q_{res,lat}\mathrm{=}{\mathrm{h}}_{fg}{\dot{m}}_{res}\mathrm{\Delta }W\f]
<center>
*Equation 33.*
</center><br>

\f[q_{res,sen}\mathrm{=}{\dot{m}}_{res}\rho c_p\left(t_{res}\mathrm{-}t_{a,res}\right)\f]
<center>
*Equation 34.*
</center><br>

\f[q_{res}\mathrm{=}q_{res,lat}\mathrm{+}q_{res,sen}\f]
<center>
*Equation 35.*
</center><br>

Figure 12 highlights the source element that is modified by the respiration method.

@image html EnvironmentRespirationCircuit.png
<center>
<i>Figure 12. The respiratory flow source is directly calculated by the respiratory calculations, which is boxed in the circuit diagram.</i>
</center><br>

### Submerged

Submerging the patient is done by using the environment change action or condition to set the surrounding type enumeration to water.  The ambient temperature will be the water temperature.  The clothing resistance will be the value when submerged, likely fully saturated or modified to a wet suit value. The currently implemented model assumes fresh water only, with a water pressure of 0.1 MPa (sea level).  It also assumes that radiation and evaporation effects are negligible.

The thermal conductivity of the water can be solved using @cite ramires1995standard :

\f[T\mathrm{=}\frac{T_{\mathrm{\infty }}}{\mathrm{298.15}}\f]
<center>
*Equation 36.*
</center><br>

\f[k\mathrm{=0.6065}\left(\mathrm{-}\mathrm{1.48445}~\mathrm{+4.12292}~T\mathrm{+-1.63866}~T^{\mathrm{2}}\right)\f]
<center>
*Equation 37.*
</center><br>

\f[\nu \mathrm{=}\frac{\mu }{\rho }\f]
<center>
*Equation 38.*
</center><br>

Equation 39 solves for the Prandtl number, which is the dimensionless ratio of momentum diffusivity (kinematic viscosity) to thermal diffusivity.

\f[Pr\mathrm{=}\frac{c_{p,w}\mu }{k}\f]
<center>
*Equation 39.*
</center><br>

Equation 40 solves for the Grashof number, which is the dimensionless value that approximates the ratio of the buoyancy to viscous force acting on a fluid.

\f[Gr\mathrm{=}\frac{g\mathrm{\alpha }\left(T_s\mathrm{-}T_{\mathrm{\infty }}\right)D^{\mathrm{3}}}{{\mathrm{\nu }}^{\mathrm{2}}}\f]
<center>
*Equation 40.*
</center><br>

The heat transfer coefficient can is determined by @cite boutelier2003experimental :

\f[{\mathrm{h}}_c\mathrm{=0.09}\left(Gr\mathrm{-}Pr\right)\mathrm{0.275}\f]
<center>
*Equation 41.*
</center><br>

Figure 13 highlights which elements are modified when submerged and by which parameters.  Radiation is handled much like an open switch in a circuit via the resistor and does not allow any transfer.  The evaporation transfer source is directly set to zero.

@image html EnvironmentSubmergedCircuit.png
<center>
<i>Figure 13. Elements that are directly affected by the submerged calculations are boxed and labeled with the parameters that cause them to change.</i>
</center><br>

### Outputs

The temperature at each node and heat flow on each path in the %Environment thermal circuit is calculated every time step.  The following system data is also set: 
- Convective Heat Loss (Power)
- Convective Heat Transfer Coefficient (Heat Conductance per Area)
- Evaporative Heat Loss (Power)
- Evaporative Heat Transfer Coefficient (Heat Conductance per Area)
- Radiative Heat Loss (Power)
- Radiative Heat Transfer Coefficient (Heat Conductance per Area)
- Respiration Heat Loss (Power)
- Skin Heat Loss (Power)

@anchor environment-assumptions
Assumptions and Limitations
---------------------------

The Ambient node operates under some pretenses for its interaction with other systems.  Since the atmospheric pressure is used to set the pulmonary reference nodes, everything in those systems is referenced to zero pressure.  This is in contrast to reporting the difference from atmospheric pressure, as is often the standard.  It is further assumed that all ambient substance volume fractions set the sum to 1.  The included scenarios generally use nitrogen in place of other inert gases.

Beyond the inferences mentioned previously in this document, some high-level assumptions for the %Environment system as a whole:
- Whole body average heat transfer, i.e., lumped parameters
- Standing patient position
- Standard air composition for heat transfer calculations
- Air is an ideal gas for heat transfer calculations
- Ignore movement of water when submerged
- No transitional heat transfer mechanics after submersion has ended (not accounting for water mass remaining on skin)
- No endogenous production of carbon monoxide
	- Endogenous production is very small so that even at very low environmental concentrations alveolar diffusion dominates
- The only carbon monoxide exposure pathophysiology simulated is tissue hypoxia
	- We do not model CO mediated damage at the cellular level (i.e. no oxidative stress or lipid peroxygenation)

@anchor environment-conditions
Conditions
----------

The environment change condition has the option to be done directly or by reading an environment xml file.  Like other conditions, the engine will stabilize the patient within that environment before running a simulation.  This is modeled most like a rapid decompression, and not necessarily the same as a patient that has lived in a "non-standard" environment and undergone physiologic changes (e.g., Himalayan Sherpas with high altitude adaptations).
@anchor environment-actions
Actions
-------

The environment change action has the option to be done directly or by reading an environment xml file.  Any of the parameters shown in Table 1 can be modified in this manor during runtime.

The thermal application action can directly add heat, remove heat, or set a temperature over any percentage of the patient&rsquo;s body.  They can be called individually or will sum together if called in combination.  These actions will set the active heat flow source and/or the active temperature source shown in Figure 14.  The applied area or fraction is used to determine the average amount to apply to the patient, since the %Environment is modeled as a one-dimensional circuit.  When actively heating or cooling, the total power is directly scaled using the fraction of the body that is covered using the patient skin surface area.  When applying a specific temperature, the average temperature will be applied between it and the ambient value, weighted by the fraction of the body covered.

@image html EnvironmentActiveCircuit.png
<center>
<i>Figure 14. The active flow source is directly set by the heating and/or cooling actions.  It is boxed in the circuit diagram.</i>
</center><br>

@anchor environment-events
## Events
There are currently no events in the %Environment system.

@anchor environment-results
Results and Conclusions
=======================

%Verification
--------------------------------------

Results using the equations from ISO 7730:2005 were used to compare to %BioGears %Environment outputs.  The standard focuses mostly on two thermal comfort metrics, Predictive Mean Vote and Predictive Percentage Dissatisfied @cite ISOPDF.  Although it is intended for the ergonomics of thermal environments, intermediate values can be extracted for the values shown in Table 6.  The connection points that connect to the %Energy system are replaced with a heat source equal to the metabolic rate, shown in Figure 15.  The other circuit elements are set to match the Inputs columns in Table 6.  The static circuit is run until everything balances and steady state is reached.  The clothing surface temperature is collected directly from the clothing node and system heat loss values are collected directly from the flow in the appropriate paths.

The following assumptions are made for this verification test:
- Complete conservation of thermal energy with none stored
- Constant respiration rate of 16 breaths/min
- Constant sweat rate of 1e-5 kg/s

@image html EnvironmentVerificationCircuit.png
<center>
<i>Figure 15. The %Energy system is replaced for the %Environment unit test by a single heat source to represent the metabolic rate.  None of the energy is stored, but instead directly affects the %Environment.</i>
</center><br>

The results in Table 6 have the skin temperatures calculated by %BioGears significantly higher than that by the ISO standard code.  However, the individual heat loss values seem reasonable and show the correct trends based on the inputs.  The %Environment model conserves energy as seen in the last column with the difference in heat produced by the metabolic rate to the sum of the heat loss by all four means.  The ISO standard does not conserve energy in the same way, and makes several looser assumptions that the %Environment system does not.  This could explain many of the discrepancies.  The requirement for the %Environment system to be able to handle extreme environments with feedback from other systems requires it to more strictly adhere to a rigorous physics based modeling approach.

<br><center>
<i>Table 6. The published ISO 7730:2005 Appendix D: Computer Program for Calculation PMV and PPD
Written in BASIC code @cite ISOPDF was used to verify the %BioGears %Environment calculations are reasonable.  Deviations can be explained by the fact that %BioGears successfully conserves energy, while the ISO 7730:2005 outputs do not.</i>
</center>
<center><img src="./images/Environment/EnvironmentVerification.png" width="1100"></center>

Validation
----------
###Forest Fire Fighter
This scenario simulates exposure to a moderate/severe forest fire . This includes a suspension of wood particulate concentration of 2.9 mg/m^3 and a concentration of carbon monoxide of 20 ppm. Environmental composition data for this scenario simulation was taken from @cite reinhardt2000smoke. Spirometry data was determined through expiratory flow curve taken during conscious respiration. Figure 16 details the difference in mass deposited in two separate respiratory compartments (following from equations 1-7) and the difference in expiratory flow before and after smoke inhalation. Slight decreases to the expiratory flow maximum are seen and coincide fairly well with validated data @cite betchley1997pulmonary. 

<center>
<table border="0">
<tr>
    <td><img src="./plots/Environment/AvleoliMassDeposited.jpg" width="550"></td>
    <td><img src="./plots/Environment/DeadSpaceMassDeposited.jpg" width="550"></td>
</tr>
<tr>
    <td><img src="./plots/Environment/ExpiratoryFlowAfter.jpg" width="550"></td>
    <td><img src="./plots/Environment/ExpiratoryFlowBefore.jpg" width="550"></td>
</tr>
</table>
</center>
<center><i>Figure 16. The top two figures show mass deposited into two different respiratory compartments during exposure to a forest fire. Different lung compartments have different mass deposition as a function of the particle mass distribution histogram. The lower figures show the difference in expiratory flow before and after smoke inhalation. The differences a very minor but are generally in line with validated data, Table 7.</i>
</center><br>

<center>
<i>
Table 7.
</i>
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Functional Vital Capacity (% from baseline PFT) 	|	Functional expiratory volume over 1 second (%Change)	|	Forced expiratory flow (25-75) (%Change)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Environment Change: Exposed to moderate/severe forest fire 	|	Patient is exposed to smoke and CO inhalation 	|	10	|	240	|<span class="success">	no change	</span>|<span class="success">	no change	</span>|<span class="success">	No Change	</span>|
|	PFT administered before and after exposure to forest fire	|	Validation is computed against pre-exposed patient data	|	5 and 310	|	310	|<span class="success">	Decrease by 1% @cite betchley1997pulmonary	</span>|<span class="warning">	Decrease 3% @cite betchley1997pulmonary	</span>|<span class="success">	Decrease 6% @cite betchley1997pulmonary	</span>|

###Carbon Monoxide
There are two scenarios used to validate carbon monoxide poisoning. The first is an exposure to CO at 200 ppm, which is the National Institute for Occupational Safety and Health ceiling @ref osha_co. The second is an exposure to CO at 5000 ppm, which is an extreme exposure that should result in death. Because the many of the symptoms of CO poisoning are not modeled in %BioGears (e.g. head ache, nausea, syncope), validation is conducted by comparing the carboxyhemoglobin kinetics to the Coburn-Forster-Kane equation @ref coburn1965considerations and to data presented in @ref stewart1975effect.

<center>
<i>
Table 8.
</i>
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Pulse Oximetry	|	Oxygen Saturation	|	Percent Carboxyhemoglobin	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Environment Change: Carbon Monoxide 200ppm	|	Person is exposed to 200ppm carbon monoxide. The maximal steady-state concentration 	|	5	|	N/A	|<span class="success">	95 to 98% @cite guyton2006medical	</span>|<span class="success">	95 to 98% @cite guyton2006medical	</span>|<span class="success">	Approx. 0 @cite boron2012medical pg. 690	</span>|
|	None	|	10 minutes	|	N/A	|	605	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Decreased	</span>|<span class="warning">	1 to 2 % @cite stewart1975effect	</span>|
|	None	|	30 minutes	|	N/A	|	1805	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Decreased	</span>|<span class="warning">	3 to 4 % @cite stewart1975effect	</span>|
|	None	|	1 hour	|	N/A	|	3605	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Decreased	</span>|<span class="warning">	5 to 6 % @cite stewart1975effect	</span>|
|	None	|	4 hours	|	N/A	|	12005	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Decreased	</span>|<span class="success">	12 to 16% @cite stewart1975effect	</span>|

<center>
<i>
Table 9.
</i>
</center>
|	Action	|	Notes	|	Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Pulse Oximetry	|	Oxygen Saturation	|	Percent Carboxyhemoglobin	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Environment Change: Carbon Monoxide 5000ppm	|	Person is exposed to 5000ppm carbon monoxide. The maximal steady-state concentration 	|	5	|	N/A	|<span class="success">	95 to 98% @cite guyton2006medical	</span>|<span class="success">	95 to 98% @cite guyton2006medical	</span>|<span class="success">	Approx. 0 @cite boron2012medical pg. 690	</span>|
|	None	|	1 hour	|	N/A	|	3605	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Decreased	</span>|<span class="success">	approx. 80% @cite stewart1975effect Increased @cite turino1981effect	</span>|
|	None	|	2 hours - Irreversible state should happen by this point	|	N/A	|	7205	|<span class="success">	Normal @cite rose2016carbon	</span>|<span class="success">	Death (Irreversible State)	</span>|<span class="success">	Death (Irreversible State)	</span>|

See the @ref EnergyMethodology for validation in varying and extreme environments.

See the @ref DrugsMethodology for validation of airborne agents.

Conclusions
-----------

The %Environment System successfully allows the user to manipulate parameters external to the patient.
@anchor environment-future
Future Work
===========

Coming Soon
-----------

There are no planned near term additions.

Recommended Improvements
------------------------

Here are some recommended improvements:
- Add the ability to define submerged surrounding liquid properties (e.g., sea water at depth)
- Add a segmented model, instead of an averaged whole body approach
- Account for patient posture
- Add Thermal Comfort Assessment - Predictive Mean Vote (PMV) and Predictive Percentage Dissatisfied (PPD)
- Integrate 3D models for indoor, outdoor, urban, etc. to modify values&mdash;especially mean radiant temperature and velocity (wind & motion)
- Add the ability to auto-populate environment parameters based on location and time of year
	- From lat/lon and date meteorological inputs
	- From real-time data
- Add radiation analysis

@anchor environment-appendices
Appendices
==========

Acronyms
--------

CDM - Common Data Model

PMV - Predictive Mean Vote

PPD - Predictive Percentage Dissatisfied

The variables presented in Table 10 are used throughout this document.
<br><center>
<i>Table 10. There are many parameters used to determine the %Environment&rsquo;s thermal state.</i>
</center>
<center><img src="./images/Environment/EnvironmentVariables.png" width="1100"></center>

Data Model Implementation
-------------------------

@ref EnvironmentTable "Environment"

@anchor environment-compartments
Compartments
------------

- Active
- Ambient
- Clothing
- Enclosure
- ExternalCore
- ExternalSkin
- ExternalGround