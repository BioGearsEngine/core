Version {#version}
=======

We have taken the following approach to versioning our products : Major.Minor.Patch-ReleaseStage

Our version number sematic is MAJOR.MINOR.PATCH, where :
- MAJOR changes when we make incompatible API changes,
- MINOR changes when we add functionality in a backwards-compatible manner, and
- PATCH changes when we make backwards-compatible bug fixes.
- Release Stage - We have extended this versioning with a <a href="http://en.wikipedia.org/wiki/Software_release_life_cycle">release stage</a>

This versioning follows the <a href="http://semver.org">Semantic Versioning 2.0.0</a> format.

## %BioGears Version History

## 6.2 (September 30, 2017)

The latest deployment includes the following notable updates:
- General bug fixes, system improvements, and tools/solver improvements
- Intoxication model for Morphine
  - Effects site concentration to allow for delayed PD reactions 
  - Central nervous modifier to model depressed feedback mechanisms 
  - Noloxone reversal agent model
- New Hemorrhage model 
  - MCIS support for combat injury coding 
  - Location and severity can be flagged through MCIS
  - Resistance paths handle bleed out to simulate more realistic flow behavior
- Glucagon hormone 
- New nutrient kinetics model 
  - New metabolic production and consumption method
  - Protein storage and release model (amino acids in muscle)
  - Fat storage and release model 
  - Interactions with the %Hepatic system
- New %Hepatic system 
  - Maintains blood glucose from other substances like lactate through new gluconeogenesis method
  - Lipogenesis method generates triacylglycerol due to excess glucose and amino acid
  - Glycogenesis and glycogenolysis to maintain blood glucose levels
- Updated exercise model 
  - Coupled to nutrient kinetic handling and metabolic need
- Updated diffusion method
- New %Gastrointestinal model 
  - Enzyme kinetics determine digestion of nutrients
  - Absorption facilitated through sodium co-transporter 
  - Gastric secretion function allows for bile formation in chyme
- Desflurane update and fix


### 6.1.1 (March 30, 2017)
Minor bug fixes

### 6.1.0 (March 10, 2017)

The latest deployment includes the following notable updates:
- General bug fixes, system improvements, and tools/solver improvements
- Improved Epinephrine methodology
- Improved Pupillary State for both Drug and %Nervous methodology
- Improved %Renal Tubuloglomerular Feedback
- Added cardiovascular chemoreceptor feedback
- Added Diuretic drug effects (Furosemide)
- Aerosolization of Solids and Liquids
  - Improves administration of Albuterol
  - New Smoke Particulate substance and smoke inhalation modeling
- Carbon Monoxide support
- New data requests and events
- New Conditions
  - Impaired Alveolar Exchange
- New Actions
  - Acute Stress
  - Apnea
  - Brain Injury
  - Intubation now supports Leftmainstem, Rightmainstem, Esopageal, and Tracheal types
  - Mechanical Ventilation

### 6.0.1 (December 15, 2016)

The latest deployment includes the following notable updates:
- General Bug fixes and system improvements
- Improved simulation runtime to ~5x real-time 
- Serialization of Engine State
- Updated GUI
- Patient variability support
- New and improved substance transporter methodology
- New and improved blood acid-base balance methodology

### 5.1.0 (March 4, 2016)

- General Bug fixes
- Improved Exercise Model
	- Exercise Action Intensity is now a fraction of work capacity (1200W)
	- New Fatigue Model
	- Removed Borg scale from Exercise action
- Improved Pulmonary Hemodynamics
	- Improved Pulmonary pressures
	- Removed Pulmonary Shunt Condition
- Updated cardiovascular validation data
- Results files are now CSV files

### 5.0.0-beta (December 18, 2015)

- General Bug fixes
- New Heat Stroke showcase scenario
- Physiology Interface Changes
	- Created a Java interface for controlling the %BioGears C++ engine
		- Added examples and the %BioGears.jar to the %BioGears SDK
	- Removed methods for executing a scenario from the Physiology Engine Interface
		- Use a SEScenarioExec class for executing a scenario, see HowTo-RunScenario.cpp in the SDK 	
- Baroreceptors	
	- %Nervous System responds to changes in mean arterial blood pressure by modifying the cardiovascular model

### 4.0.0-beta (October 12, 2015)

- Bug fixes and improved system calibration
- New target platforms
	- Windows
		- MSVC 32 & 64 bit 
		- MinGW 32 bit (GCC)
	- Mac
		- Xcode (Clang)
	- Linux
		- Ubuntu (GCC)
    - Raspberry Pi
        - Raspbian
- Improved compartment methodology
- Existing system upgrades
	- %Inhaler addition to equipment
	- %Respiratory conscious breathing
	- %Cardiovascular cardiac arrest and CPR redesign
	- Revamped %Renal System
	- Revamped %Gastrointestinal System
	- Updates to all other systems
- Updated pharmacokinetic and new pharmacodynmic model
	- Calculates partition coefficients based on physical chemical properties
	- Uses blood-tissue partition coefficients to calculate diffusion
	- Intrinsic, %renal, and systemic clearance remove drugs from the body
	- Pharmacodynamic effects are calculated for ten clinical outputs
- New showcase scenarios
    - Asthma Attack
	- %Environment Exposure
- New Patient Assessments
	- Complete Blood Count
	- Complete Metabolic Panel
	- Urinalysis
	
### 3.0.0-alpha (July 24, 2015)

- Bug fixes and improved system calibration
- New target platforms
	- Windows
		- MSVC 32 & 64 bit 
		- MinGW 32 bit (GCC)
	- Mac
		- XCode (Clang)
	- Linux
		- Ubuntu linix
- Engine time step increased from 1/165s to 1/90s for improved simulation speed
- New tissue integration with diffusion between vascular and extravascular
- Corrected O2 consumption and CO2 production within the tissues
	- New O2 and CO2 Hemoglobin binding methodology
	- Corrected environment/ambient volume fractions
- Mass and volume transfer calculations corrected for bifurcations and volume-less nodes (infinite volume) 
- Several new systems
	- System Interactions
	- %Renal
	- %Gastrointestinal
	- %Energy
- Existing system upgrades
	- %Environment - thermal functionality
	- %Respiratory
		- Asthma
		- COPD
			- Bronchitis
			- Emphysema
		- Lobar Pneumonia
	- %Cardiovascular - removed baroreceptor model and response incorporated into actions
- Updated pharmacokinetic model
	- New physiochemical parameters
	- Partition coefficient calculation
	- Perfusion limited diffusion
	- %Renal, hepatic, and systemic clearance
	
### 2.0.0-alpha (March 20, 2015)

- Bug fixes and improved system calibration
- CMake compilation
- New Dynamic Stabilization methodology
- New Conditions methodology
- Circuit Calculator
	- Polarized elements
	- More unit tests and validation
- %Cardiovascular updates
	- New/Updated Actions/Conditions
		- CPR
		- Anemia
		- Arrhythmias
		- Pericardial Effusion
		- Ventricular Systolic Dysfunction
		- Pulmonary Shunt
	- Redesigned circuit with more organs represented
- %Respiratory updates
	- Redesigned circuit
	- Redesigned driver
- Anesthesia Machine updates
	- Redesigned circuit
	- Redesigned ventilator
- Improved gas exchange
- New %Environment System
- New %ECG methodology	

### 1.0.0-alpha (October 1, 2014)

Initial Release