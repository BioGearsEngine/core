Scenario XML Files {#ScenarioXMLFile} 
==================

%BioGears can be used to simulate various physiological scenarios.
The Common Data Model (CDM) provides a Scenario structure that can contain a set of instructions that can be used to drive %BioGears. 
Below you can see :
- How a scenario is structured in XSD Schema
- XML examples of all the actions, conditions, assessments supported by %BioGears 

A Scenario is a 'canned' instruction set with requested data to be output in a comma delimited file that is executed by the engine and will produce the same results data.
'FATAL' is used below to note boundary cases that will result in a fatal exception, stopping the engine.
	
If you would like execute scenarios, the CDM contains a class, @ref SEScenarioExec, that can execute any scenario with a physiology engine that implements the PhysiologyEngine interface.
The @ref Toolkit, also provides example scenario files and can also execute scenario files.

An example of a basic scenario file is shown below.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<?xml version="1.0" encoding="UTF-8"?>
<Scenario xmlns="uri:/mil/tatrc/physiology/datamodel" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsdVersion="v16.12" contentVersion="BioGears_6.0.1-beta" xsi:schemaLocation="">
  <Name>InitialStableState</Name>
  <Description>Start the engine at a state, this is for debugging said state</Description>
  <EngineStateFile>./states/StandardMale@0s.xml</EngineStateFile>
  
  <DataRequests>
	  <DataRequest xsi:type="PhysiologySystemDataRequestData" Name="HeartRate" Unit="1/min"/>
	  <DataRequest xsi:type="PhysiologySystemDataRequestData" Name="RespirationRate" Unit="1/min"/>
    <DataRequest xsi:type="PhysiologySystemDataRequestData" Name="OxygenSaturation" Unit="unitless"/>
    
	  <DataRequest xsi:type="GasCompartmentDataRequestData" Compartment="Carina"       Substance="Oxygen"        Name="PartialPressure" Unit="cmH2O" Precision="0"/>
	  <DataRequest xsi:type="GasCompartmentDataRequestData" Compartment="Carina"       Substance="CarbonDioxide" Name="PartialPressure" Unit="cmH2O" Precision="1"/>
	  	
	  <DataRequest xsi:type="LiquidCompartmentDataRequestData" Compartment="Aorta"     Substance="Oxygen"        Name="PartialPressure" Unit="mmHg" Precision="1"/>
	  <DataRequest xsi:type="LiquidCompartmentDataRequestData" Compartment="Aorta"     Substance="CarbonDioxide" Name="PartialPressure" Unit="mmHg" Precision="1"/>
  </DataRequests>
   
  <Action xsi:type="AdvanceTimeData">
    <Time value="2" unit="min"/>		
  </Action>
</Scenario>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The scenario allows for a name and description, but this is not used in execution.
The scenario contains the following execution information:
- What engine state to use
- A list of values to return from the engine
- A list of actions to execute over the course of the run

- - -

## Patient File and optional conditions

While it is recommened to use an Engine State when running a scenario, you do have the option to initialize the engine with a Patient File and optional conditions.
The specified patient file refers to an XML file containing @ref Patient_PatientData information.
Replace the <EngineStateFile> tag with the <InitialParameters> tag like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<?xml version="1.0" encoding="UTF-8"?>
<Scenario xmlns="uri:/mil/tatrc/physiology/datamodel" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsdVersion="v16.12" contentVersion="BioGears_6.0.1-beta" xsi:schemaLocation="">
    <Name>Anemia30</Name>
    <Description>Anemia onset, leading to 30% reduction in hemoblogin content</Description>
    <InitialParameters>
		  <PatientFile>StandardMale.xml</PatientFile>
		  <Condition xsi:type="ChronicAnemiaData">
			  <ReductionFactor value="0.3"/>
		  </Condition>
	  </InitialParameters>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Patient Conditions

Conditions give instructions to the engine to apply certian changes to the engine to simulate the specified conditions.
The following are links to the Condition class specification along with XML examples of conditions that can be used in making your own scenarios.

#### Chronic Anemia
@copybrief PatientConditions_ChronicAnemiaData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ChronicAnemiaData">
 <ReductionFactor value="0.3"/>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### COPD
@copybrief PatientConditions_ChronicObstructivePulmonaryDiseaseData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Condition xsi:type="ChronicObstructivePulmonaryDiseaseData">
  <BronchitisSeverity  value="0.65"/>
	<EmphysemaSeverity   value="0.50"/>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Chronic Pericardial Effusion
@copybrief PatientConditions_ChronicPericardialEffusionData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ChronicPericardialEffusionData">
    <AccumulatedVolume value="500" unit="mL"/>
 </Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Chronic %Renal Stenosis
@copybrief PatientConditions_ChronicRenalStenosisData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ChronicRenalStenosisData"> 
    <LeftKidneySeverity value="0.9"/>
    <RightKidneySeverity value="0.9"/>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Chronic Ventricular Systolic Dysfunction
@copybrief PatientConditions_ChronicVentricularSystolicDysfunctionData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ChronicVentricularSystolicDysfunctionData"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Consume Meal
@copybrief PatientConditions_ConsumeMealData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ConsumeMealData">
	<Meal>
		<Water value="1.0" unit="L"/>
		<ElapsedTime value="12.0" unit="hr"/>
	</Meal>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Impaired Alveolar Exchange
@copybrief PatientConditions_ImpairedAlveolarExchangeData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="ImpairedAlveolarExchangeData">
		<ImpairedSurfaceArea value="1.0" unit="m^2"/>
</Condition>

or

<Condition xsi:type="ImpairedAlveolarExchangeData">
		<ImpairedFraction value="0.20"/>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Lobar Pneumonia
@copybrief PatientConditions_LobarPneumoniaData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="LobarPneumoniaData">
	<Severity            value="0.70"/>
	<LeftLungAffected    value="0.00"/>
	<RightLungAffected   value="0.67"/>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Initial %Environment
@copybrief EnvironmentConditions_InitialEnvironmentData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Condition xsi:type="InitialEnvironmentData">
	<Conditions>
		<SurroundingType>Air</SurroundingType>
		<AirVelocity value="0.0" unit="m/s"/>
		<AmbientTemperature value="20.0" unit="degC"/>
		<AtmosphericPressure value="525.0" unit="mmHg"/>
		<ClothingResistance value="3.0" unit="clo"/>
		<Emissivity value="0.9"/>
		<MeanRadiantTemperature value="20.0" unit="degC"/>
		<RelativeHumidity value="1.0"/>
		<RespirationAmbientTemperature value="20.0" unit="degC"/>
		<AmbientSubstance Name="Nitrogen">
			<FractionAmount value="0.8576"/>
		</AmbientSubstance>
		<AmbientSubstance Name="Oxygen">
			<FractionAmount value="0.142"/>
		</AmbientSubstance>
		<AmbientSubstance Name="CarbonDioxide">
			<FractionAmount value="4.0E-4"/>
		</AmbientSubstance>
	</Conditions>
</Condition>

or

<!-- file must be in the ./bin/environemnts directory -->
<Condition xsi:type="InitialEnvironmentData">
	<ConditionsFile>Hypobaric3000m.xml</ConditionsFile>
</Condition>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - - 

## Data Requests

Currently there are four supported types of data requests:

### Physiology System Data

Physiology System data refers to all the data specified by SystemData and its derived types.

At this time, you do not need to specify the system name.
The Name attribute should be set to a System Property name. (e.g., HeartRate)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="PhysiologySystemDataRequestData" Name="HeartRate" Unit="1/min" />
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Compartment Data

Compartments refer to all the data specified on Compartments.
You can read more about compartments @ref CompartmentsInterface "here".

There are two amin types of Compartments, gas and liquid.

Data on the compartment itself: 
The Compartment attribute can be any of the enumerations defined by %BioGears. 
The Name attribute should be set to a Compartment Property name.
The Substance attribute is optional, and the if used the name will refer to a substance quantity property.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="GasCompartmentDataRequestData" Compartment="LeftAlveoli" Name="Pressure" Unit="cmH2O"/>		
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="GasCompartmentDataRequestData" Compartment="LeftAlveoli" Substance="Oxygen" Name="PartialPressure" Unit="mmHg"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="LiquidCompartmentDataRequestData" Compartment="Aorta" Name="Pressure" Unit="mmHg" />
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="LiquidCompartmentDataRequestData" Compartment="Aorta" Substance="Oxygen" Name="PartialPressure" Unit="mmHg"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Environment Data

Environment System data refers to all the data specified by @ref Environment_EnvironmentData and its derived types.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="EnvironmentDataRequestData"  Name="ConvectiveHeatLoss"    Unit="W"          Precision="2"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Equipment Data

System level data from a piece of equipment

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="EquipmentDataRequestData"  Type="ECG" Name="Lead3ElectricPotential"    Unit="mV"          Precision="0"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Substance Data

Substance data is provided about a substance and its current state in the body and on specific anatomy compartments

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="SubstanceDataRequestData" Substance="Morphine" Name="PlasmaConcentration" Unit="ug/mL"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<DataRequest xsi:type="SubstanceDataRequestData" Substance="Morphine" Compartment="LeftKidneyTissue" Name="MassCleared" Unit="ug"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

## General Actions

Actions give instructions to the engine to define what happens over the course of a scenario. 
Everything from advancing time, to starting a hemorrhage, to administering a drug is an action of some kind. 
The following are links to the Action class specification along with XML examples of actions that can be used in making your own scenarios.

- - -

#### Advance Time

@copybrief Scenario_AdvanceTimeData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AdvanceTimeData">
	<Time value="350" unit="s"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Serialize State
@copybrief Scenario_SerializeStateData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="SerializeStateData" Type="Save">
		<Filename></Filename><!-- No Filename, engine is going to auto generate a name, BioGears will do something like : StandardMale@0s.xml -->		
  </Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

Patient Insults
---------------
- - -

#### Acute Stress
@copybrief PatientActions_AcuteStressData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AcuteStressData">
   <Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Apnea
@copybrief PatientActions_ApneaData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ApneaData">
   <Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Airway Obstruction 
@copybrief PatientActions_AirwayObstructionData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AirwayObstructionData">
   <Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Asthma Attack
@copybrief PatientActions_AsthmaAttackData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AsthmaAttackData">
	<Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Brain Injury
@copybrief PatientActions_BrainInjuryData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.<br>
Types : Diffuse, LeftFocal, RightFocal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="BrainInjuryData" Type="Diffuse">
	<Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
		
#### Bronchoconstriction 
@copybrief PatientActions_BronchoconstrictionData <br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="BronchoconstrictionData">
	<Severity value="0.3"/>       
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Consume Nutrients 
@copybrief PatientActions_ConsumeNutrientsData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ConsumeMealData">
	<Nutrition>
		<Carbohydrate value="390.0" unit="g"/>
		<CarbohydrateDigestionRate value="0.5" unit="g/min"/>
		<Fat value="90.0" unit="g"/>
		<FatDigestionRate value="0.055" unit="g/min"/>
		<Protein value="56.0" unit="g"/>
		<ProteinDigestionRate value="0.071" unit="g/min"/>
		<Calcium value="1000.0" unit="mg"/>
		<Sodium value="1.5" unit="g"/>
		<Water value="3.7" unit="L"/>
	</Nutrition>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Cardiac Arrest 
@copybrief PatientActions_CardiacArrestData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="CardiacArrestData"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Exercise 
@copybrief PatientActions_ExerciseData <br>
An intensity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ExerciseData">
	<Intensity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ExerciseData">
	<BorgScale value="10.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Hemorrhage 
@copybrief PatientActions_HemorrhageData <br>
The Compartment attribute can be any of the enumerations defined in the enumAnatomy enumeration.<br>
FATAL: Cannot have bleeding rate greater than cardiac output or less than 0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="HemorrhageData" Compartment="RightLegVascular">
    <Rate value="250" unit="mL/min"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Pericardial Effusion
@copybrief PatientActions_PericardialEffusionData <br>
EffusionRate of the liquid
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="PericardialEffusionData" >
	<EffusionRate value="0.1" unit="mL/s"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Tension Pneumothorax 
@copybrief PatientActions_TensionPneumothoraxData <br>
The Type attribute can be "Open" or "Closed"<br>
The Side attribute can be "Left" or "Right"<br>
Severity value must be >=0.0 and <=1.0 <br>
A severity of 0 removes the action completely.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="TensionPneumothoraxData" Type="Closed" Side="Left">
        <Severity value="0.6"/>       
    </Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

Patient Interventions
---------------------

- - -
	
#### Chest Compression Force 
@copybrief PatientActions_ChestCompressionForceData <br>
Force is the specific magnitude to perform a compression with.<br>
Note, that patient should be in Cardiac Arrest before performing CPR
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ChestCompressionForceData">
    <Force value="100.0" unit="N"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
		
#### Chest Compression Force Scale 
@copybrief PatientActions_ChestCompressionForceScaleData <br>
ForceScale value must be >=0.0 and <=1.0<br>
Note, that patient should be in Cardiac Arrest before performing CPR
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ChestCompressionForceScaleData"> 
	<ForceScale value="0.73"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
		
#### Chest Occlusive Dressing 
@copybrief PatientActions_ChestOcclusiveDressingData <br>
The State attribute can be "On" or "Off" <br>
Side is either Left or Right<br>
FATAL: If the side specified does not have a pnumothorax 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ChestOcclusiveDressingData" State="On" Side="Left">
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Conscious Respiration 
@copybrief PatientActions_ConsciousRespirationData <br>
This action can contain 1 or more commands :
- @copybrief PatientActions_ForcedInhaleData
- @copybrief PatientActions_ForcedExhaleData
- @copybrief PatientActions_BreathHoldData
- @copybrief PatientActions_UseInhalerData

<br>
Commands will be processed in order.
The first commands will be proceesed instantly
When it has completed (run through it's Period),
the next command will be processed.
Other actions will be processed while these commands
are processed or waiting to be processed.
You may want to advance time for the sum of the 
command periods to ensure the body is doing what you 
expect it to.. Or not, depending on how you want 
the system to react.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ConsciousRespirationData">
	<Command xsi:type="ForcedExhaleData">
		<ExpiratoryReserveVolumeFraction value="1.0"/>
		<Period value="3.0" unit="s"/>			
	</Command>  
	<Command xsi:type="ForcedInhaleData">
		<InspiratoryCapacityFraction value="1.0"/>
		<Period value="5.0" unit="s"/>          
	</Command> 
	<Command xsi:type="UseInhalerData"/>	
	<Command xsi:type="BreathHoldData">
		<Period value="10.0" unit="s"/>          
	</Command> 
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Intubation 
@copybrief PatientActions_IntubationData <br>
Note: In order to 'turn off' an intubation, use'Off' as the Type  <br>
Types : Off, Esophageal, LeftMainstem, RightMainstem, Tracheal
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="IntubationData" Type="Tracheal"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

#### Mechanical Ventilation 
@copybrief PatientActions_MechanicalVentilationData <br>
You may provide Pressure and/or Flow. <br>
If you do not provide GasFractions, the environment gas fractions will be used. <br>
If you do provide Gas Fractions, they must add up to 1.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="MechanicalVentilationData" State="On">
  <Pressure value="10.0" unit="cmH2O"/>
  <Flow value="1.0" unit="mL/s"/>
  <GasFraction Name="Oxygen">
    <FractionAmount value="1.0"/>
  </GasFraction>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Needle Decompression 
@copybrief PatientActions_NeedleDecompressionData <br>
The Side attribute can be "Left" or "Right"<br>
The State attribute can be "On" or "Off"
FATAL: If the side specified does not have a pnumothorax 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="NeedleDecompressionData" State="On" Side="Left"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Urinate 
@copybrief PatientActions_UrinateData <br>
Action to empty the bladder. if not emptied, 
it will empty and throw an event.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="UrinateData"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Substance Bolus  
@copybrief PatientActions_SubstanceBolusData

The AdminRoute can be one of:
- "Intravenous"
- "Intramuscular"
- "Subcutaneous"
- "Oral"
- "Rectal"
- "Inhaled"

The Substance element should be set to a name of any of the %Substances.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="SubstanceBolusData" AdminRoute="Intravenous">
	<Substance>Succinylcholine</Substance>
	<Concentration value="4820" unit="ug/mL"/>
	<Dose value="30" unit="mL"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Substance Compound Infusion Fluids 
@copybrief PatientActions_SubstanceCompoundInfusionData <br>
The Substance Compound element should be set to a name of any of the %Substances Compounds. <br>
Set Rate to 0 to remove Action
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="SubstanceCompoundInfusionData">
	<SubstanceCompound>Saline</SubstanceCompound>
	<BagVolume value="500" unit="mL"/>
	<Rate value="100" unit="mL/min"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Substance Infusion 
@copybrief PatientActions_SubstanceInfusionData <br>
The Substance element should be set to a name of any of the %Substances. <br>
Set Rate to 0 to remove Action
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="SubstanceInfusionData" State="On" AdminRoute="Intravenous">
    <Substance>Succinylcholine</Substance>
    <Concentration value="5000" unit="ug/mL"/>
    <Rate value="100" unit="mL/min"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

Anesthesia Machine State
------------------------

- - -
#### Anesthesia Machine Configuration 
@copybrief AnesthesiaActions_AnesthesiaMachineConfigurationData <br>
NOTE: Each field is optional. <br>
Connection can be one of : Off, Mask, Tube <br>
Patient must be intubated to be connected as Tube <br>
Anesthesia machine will be disconneted if intubation is removed. <br>
Patient cannot be intubated to be connected as Mask <br>
Anesthesia machine will be disconneted if patient is then intubated. <br>
Cannot have inhaler and anesthesia machine on at the same time <br>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AnesthesiaMachineConfigurationData">
	<Configuration>
    <Connection>Mask</Connection>
		<InletFlow value="5.0" unit="L/min"/>
		<InspiratoryExpiratoryRatio value="0.5"/>
		<OxygenFraction value="0.25"/>
		<OxygenSource>Wall</OxygenSource>
		<PositiveEndExpiredPressure value="1.0" unit="cmH2O"/>
		<PrimaryGas>Nitrogen</PrimaryGas>
		<RespiratoryRate value="16.0" unit="1/min"/>
		<VentilatorPressure value="10.5" unit="cmH2O"/>
		<OxygenBottleOne>
			<Volume value="660" unit="L"/>
		</OxygenBottleOne>
		<OxygenBottleTwo>
			<Volume value="660" unit="L"/>
		</OxygenBottleTwo>
	</Configuration>
</Action>    
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="AnesthesiaMachineConfigurationData" >
	<Configuration>
		<Connection>Off</Connection>
	</Configuration>
</Action> 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
<Action xsi:type="AnesthesiaMachineConfigurationData">
	<Configuration>
		<LeftChamber>
      <State>On</State>
			<SubstanceFraction value="0.04"/>					
			<Substance>Desflurane</Substance>
		</LeftChamber>
	</Configuration>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
- - -

Anesthesia Machine Incidents
----------------------------

- - -
	
#### Oxygen TankPressure Loss 
@copybrief AnesthesiaActions_OxygenTankPressureLossData <br>
The State attribute can be "On" or "Off"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="OxygenTankPressureLossData" State="On"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Oxygen Wall Port Pressure Loss 
@copybrief AnesthesiaActions_OxygenWallPortPressureLossData <br>
The State attribute can be "On" or "Off"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="OxygenWallPortPressureLossData" State="On"/>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

Anesthesia Machine Failures
---------------------------

- - -
	
#### Expiratory Valve Leak 
@copybrief AnesthesiaActions_ExpiratoryValveLeakData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ExpiratoryValveLeakData" State="On">
    <Severity value="0.5"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Expiratory Valve Obstruction 
@copybrief AnesthesiaActions_ExpiratoryValveObstructionData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ExpiratoryValveObstructionData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Inspiratory Valve Leak 
@copybrief AnesthesiaActions_InspiratoryValveLeakData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="InspiratoryValveLeakData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Inspiratory Valve Obstruction 
@copybrief AnesthesiaActions_InspiratoryValveObstructionData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="InspiratoryValveObstructionData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Mask Leak 
@copybrief AnesthesiaActions_MaskLeakData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="MaskLeakData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Soda Lime Failure 
@copybrief AnesthesiaActions_SodaLimeFailureData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="SodaLimeFailureData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Tube Cuff Leak 
@copybrief AnesthesiaActions_TubeCuffLeakData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="TubeCuffLeakData" State="On">
    <Severity value="0.5"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Vaporizer Failure 
@copybrief AnesthesiaActions_VaporizerFailureData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="VaporizerFailureData" State="On">
    <Severity value="0.25"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Ventilator Pressure Loss 
@copybrief AnesthesiaActions_VentilatorPressureLossData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="VentilatorPressureLossData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -
	
#### Y Piece Disconnect 
@copybrief AnesthesiaActions_YPieceDisconnectData <br>
The State attribute can be "On" or "Off"<br>
Severity value must be >=0.0 and <=1.0 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="YPieceDisconnectData" State="On">
    <Severity value="1.0"/>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

%Inhaler State
------------------------

#### %Inhaler Configuration 
@copybrief InhalerActions_InhalerConfigurationData <br>
FATAL: Cannot have inhaler and anesthesia machine on at the same time
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="InhalerConfigurationData">
	<Configuration>
		<Substance>Albuterol</Substance>
		<MeteredDose value="90.0" unit="ug"/>
		<NozzleLoss value="0.04"/>
	</Configuration>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- - -

%Environment 
------------------------

- - -
	
#### %Environment Configuration State <br>
@copybrief EnvironmentActions_EnvironmentChangeData <br>
NOTE: Each field is optional.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="EnvironmentChangeData">
	<Conditions>
        <SurroundingType>Water</SurroundingType>
        <AirVelocity value="0.0" unit="m/s"/>
        <AmbientTemperature value="10.0" unit="degC"/>
        <AtmosphericPressure value="760.0" unit="mmHg"/>
        <ClothingResistance value="0.01" unit="clo"/>
        <Emissivity value="0.0"/>
        <MeanRadiantTemperature value="22.0" unit="degC"/>
        <RelativeHumidity value="1.0"/>
        <RespirationAmbientTemperature value="22.0" unit="degC"/>
        <AmbientSubstance Name="Nitrogen">
            <FractionAmount value="0.7901"/>
        </AmbientSubstance>
        <AmbientSubstance Name="Oxygen">
            <FractionAmount value="0.2095"/>
        </AmbientSubstance>
        <AmbientSubstance Name="CarbonDioxide">
            <FractionAmount value="4.0E-4"/>
        </AmbientSubstance>
	</Conditions>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#### Thermal Application <br>
@copybrief EnvironmentActions_ThermalApplicationData <br>
You must provide at least 1 activity, but up can also 
apply upto all 3 in one action.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ThermalApplicationData">
	<ActiveHeating>
		<Power value="500" unit="BTU/hr"/> 
		<SurfaceAreaFraction value="0.2" unit="unitless"/>  		
	</ActiveHeating>
	<ActiveCooling>
		<Power value="500" unit="BTU/hr"/> 
		<CSurfaceArea value="0.1" unit="m^2"/> 			
	</ActiveCooling>
	<AppliedTemperature>
    <State>On</State>
		<Temperature value="30" unit="degF"/> 
		<SurfaceAreaFraction value="1.0" unit="unitless"/> 			
	</AppliedTemperature>
</Action>	
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<Action xsi:type="ThermalApplicationData">		
	<AppliedTemperature>
    <State>On</State>
		<Temperature value="140" unit="degF"/> 
		<SurfaceAreaFraction value="0.9" unit="unitless"/> 			
	</AppliedTemperature>
</Action>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
