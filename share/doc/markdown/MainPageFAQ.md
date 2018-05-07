## What is %BioGears?

%BioGears is a C++ based, open source, multi-platform (Windows, Mac, and Linux), comprehensive human physiology
engine that will drive medical education, research, and training technologies. 
%BioGears enables accurate and consistent physiology
simulation across the medical community. The engine can be used as a
standalone application or integrated with simulators, sensor interfaces,
and models of all fidelities.

%BioGears high-level objectives include:
-   Create a publicly available physiology research platform that
    enables accurate and consistent simulated physiology across training
    applications
-   Lower the barrier to create medical training content
-   Engage the community to develop and extend physiology models
-   Meet the training needs of the military
-   Expand the body of knowledge regarding the use of simulated
    physiology for medical education
	
## What can %BioGears do?

An instance of a %BioGears engine models a single patient's physiology.
- The patient is defined by parameters, such as height, weight, systolic and diastolic pressure.
- You can initialize the patient with specific chronic and/or disease states via conditions.
- You can modify the patients external environmental conditions (weather, submerge in water, etc.)
- You can apply various actions (acute insults/injuries, interventions, conscious breathing, exercise, etc.) to be applied to the patient.
- The patient can also interact with equipment models, such as an Anesthesia and/or an %ECG Machine as well as an %Inhaler via actions.

## What kind of data can I get from %BioGears?

Available data is defined within the %BioGears Engine in three major ways:
-	System data, such as %Cardiovascular, %Respiratory, etc.
	-	Analogous to system vitals
	  -	Examples: heart rate, oxygen consumption, mean arterial pressure, etc.
-	Compartment data
	-	Flow, pressure, and volume related to specific part of the body or component of equipment
	  - Examples: Blood flow to the brain, Right Lung Volume, Right Heart Pressure
	- Substance specific data related to a specific part of the body or component of equipment
	  -	Examples: The Extracellular concentration of succinylcholine in the brain tissue, anesthesia machine gas inlet oxygen volume fraction
-	Assessments
	-	Formed at the level of a clinician's report, Intended to give general patient overviews
	  -	Example: Pulmonary Function Test
    
## What are your models and how did you validate them?

%BioGears is a closed loop total body physiology model that combines physics-based lumped parameter models 
and control system feedback mechanisms to model real-time system-level 
physiologic behaviors. Lumped parameter models use electrical circuit analogs 
to represent the major physiologic systems. 

For validation, our team uses a combination of peer-reviewed publications 
and subject matter expert judgment. Our team has:
- Defined key parameters for each system for validation
- Collected published data in the form of waveforms, and max, min, and mean values
- Used custom written tools to compare data, perform analysis, and generate plots and tables of results

A primary purpose of model validation is to ensure that the model has an appropriate domain of validity 
given the goals of the modeling process. Our validation effort is driven by the goals of the project.
For that reason, we do not have the resources to perform a rigorous sensitivity analysis or some of the other 
tools associated with a general validation protocol. We are not aware of any existing third-party validation effort. 
We would welcome and support, in as much as we are able, any validation or uncertainty quantification effort by the community.

We provide reports for each @ref SystemMethodology included in the %BioGears Engine.
<br>  
These documents cover the design, implementation, assumptions, limitations, and validation of each model. 

#### Physiology Systems
@secreflist
 @refitem BloodChemistryMethodology "Blood Chemistry"
 @refitem CardiovascularMethodology "Cardiovascular"
 @refitem DrugsMethodology "Drugs"
 @refitem EndocrineMethodology "Endocrine"
 @refitem EnergyMethodology "Energy"
 @refitem EnvironmentMethodology "Environment"
 @refitem GastrointestinalMethodology "Gastrointestinal"
 @refitem NervousMethodology "Nervous"
 @refitem RenalMethodology "Renal"
 @refitem RespiratoryMethodology "Respiratory"
 @refitem TissueMethodology "Tissue"
@endsecreflist 
#### Equipment
@secreflist
 @refitem AnesthesiaMachineMethodology "Anesthesia Machine"
 @refitem InhalerMethodology "Inhaler"
@endsecreflist  
#### Modeling Support
@secreflist
 @refitem PatientMethodology "Patient Variability"
 @refitem CircuitMethodology "Circuits"
 @refitem SubstanceTransportMethodology "Substance Transport"
@endsecreflist 
    
## How do I run %BioGears?    

The @ref Toolkit is intended for users (e.g., researchers, educators, or curious
individuals) who wish to execute %BioGears and view the physiological
effects of the patient. The Toolkit will create and plot a time-based,
comma delimited file of calculated physiological outputs.

You can download the Toolkit here.<br>

The Toolkit unzips about 15&nbsp;MB of files.
We recommend having at least 1&nbsp;GB of available disk space.
- The result files generated from the command line executable and GUI average 20&nbsp;MB.
- Graphed results file from the GUI or graphing script can generate file sets of several hundred MB.
	
## How do I code with %BioGears?

%BioGears has developed a modular architecture to reduce costs for applications that need a physiology engine as well as want to develop or extend a physiology model.

This architecture contains :
- A @ref CDM containing set of classes and generic algorithms for lumped parameter physiology modeling 
- A standard @ref physeng for controlling a physiology engine via these CDM objects

We created a @ref SDK (SDK) to help developers integrate
the %BioGears Engine into software applications. This SDK provides
pre-built libraries and headers, as well as examples of how to programmatically
using the provided interfaces. The provided application programming
interfaces (APIs) provide full control over the engine to execute
various actions and retrieve a range of calculated physiological
outputs.

You can download the SDK here.<br>

## How can I modify %BioGears, or integrate my model with %BioGears?

%BioGears uses an extensible architecture to promote integration with external
models with varying levels of fidelity. System-level model fidelity can be 
increased or decreased by adding or removing nodes and sub-circuits.

All integration/extension will require a custom build of our @ref SourceCode.
The @ref CDM provides a standard for data interchange between models. 
The deliberate identification of data requirements must precede any model modification or addition to determine 
if an extension of the Common Data Model is required. If the existing data model is sufficient to meet your modeling needs, 
you may be able to implement changes satisfactorily just by modifying the source code for the physiologic system 
of interest. If a Common Data Model extension is necessary, modification of the source code becomes more complicated. The 
quickest and easiest way to modify %BioGears to meet your needs is to [work with us](https://www.biogearsengine.com/workwithus "Work with us").
We can help with requirements definition, provide development support, and/or make modifications for you.

You can download the source code here.<br>
The source unzips around 350&nbsp;MB of files.<br>
We recommend having at least 5&nbsp;GB of disk space dedicated to building %BioGears.

#### How do I ensure my changes/model are good?

We include scenarios and their results for verification and validation. 
These results provide a baseline we can use to measure deviations to results when the code is modified.
As changes are implemented in the code base, we rerun all scenarios and compare the new results with baseline results to see how the implemented changes manifest in %BioGears system data. 
Any new result that is over 2% error is marked as a failure. 
This data is used extensively to validate each system individually, as well as the combined effects of insults and interventions. See the Methodology Reports for more details.  
The scenarios output requests match the columns in the results file; we recommend that these scenarios remain unmodified.

You can download the data sets here.<br>
The complete data set unzips around 2&nbsp;GB of files.<br>
Execution of all scenarios provided can generate up to 20&nbsp;GB of results data.