Toolkit {#Toolkit} 
========

Toolkit Contents
----------------

The Toolkit contains the following:

- A collection of example @ref ScenarioXMLFile that can be found in the /bin/scenarios folder.
- A GUI provides a simple interface for creating, editing, and executing a scenario file, as well as creating a result and plots of requested data
- A script that executes a scenario file (via a C++ executable) and generates a result of requested data
- A script for generating plots of the result
- A script to compare results between two result files


<img src="./images/GUI/ToolKitDir.png" alt="BioGears Toolkit Directory">
<center>
*The extracted file structure, showing the location of the batch files for scripts and the GUI.*
</center>

- - -

GUI
---
<b>
The GUI is Java based, thus is reliant on a Java Runtime %Environment (JRE). 
You can get the JRE <a href="http://www.oracle.com/technetwork/java/javase/downloads/index.html">here.</a> 
JRE installation instructions are <a href="http://docs.oracle.com/goldengate/1212/gg-winux/GDRAD/java.htm">here.</a> 
To ensure you properly installed the Java Runtime %Environment, in your command window type "java-version". 
</b>


<b>Note:</b>  The GUI is able to run on both 32-bit and 64-bit Java Runtime Environments, the toolkit will automatically load the proper dlls associated with the JVM it loaded.
If the GUI has trouble opening and executing scenarios, there may be an architecture mismatch between the JRE and binaries.
Look at the JRE version string displayed in the GUI log window and make sure the corresponding binaries are present in the toolkit
folder in the release and release64 subfolders.
<br>

Start the GUI by double-clicking the BioGearsGUI.bat or BioGearsGUI.sh found in 
the bin directory of the extracted Toolkit download.<br>

<img src="./images/GUI/FullGUI.png" alt="BioGears GUI"> <br>

The main GUI window is where you will construct a scenario to be run by %BioGears. If you'd rather start from an existing scenario, open a scenario XML file via the File | Open menu option.
<img src="./images/GUI/MenuFile.png" alt="File Open"><br>

<br>
<b>Creating a Scenario</b>
<br>
To disgard all changes reated the current working scenario, use the File | New option to start over with a clean, empty scenario.

<br>
<b>Saving a Scenario</b>
<br>
To save a created scenario, use the File | Save option.

<br>
<b>Loading a Scenario</b>
<br>
To load a created scenario, use the File | Load option.<br>
When you load a scenario, if that scenario has a patient file and NO conditions, the GUI will automatically attempt to find and use the accompanying state file for the patient file.
The GUI will automatically change the patient file the associated engine state so as to avoid patient initialization.

###Name and Description###
If you loaded a scenario, you will notice the scenario name and description are populated in the Name and Description fields, respectively. 
If you are creating a new scenario from scratch, you can enter your own name and description.
<img src="./images/GUI/NameAndDescription.png" alt="Name and Description">

###Patient Information###
Choose a patient to be the subject of the scenario. Selecting the Patient File radio button will allow you to choose from a variety of available patients in the dropdown box below. 
Alternately, you can select the Engine State radio button to load a patient in a state after stabilization, which will reduce the scenario runtime. Note that you cannot 
add conditions to a scenario where an Engine State is loaded, but when loading from a Patient File, conditions can be added.
<img src="./images/GUI/PatientOptions.png" alt="Patient Options"> 
<br>
<b>Note:</b> Creation and editing of patients from the GUI is currently disabled in this release.

###Conditions###
If you are loading from a Patient File, you will notice a window with available conditions is available to edit. To add a condition to the selected patient, 
highlight the desired condition and click the Edit button. A window will appear where you can set any parameters for the condition. Note that if you leave a parameter 
blank, the scenario will use whatever value would have been used when no condition is applied. For example, when adding the Initial %Environment condition, 
changing only the AmbientTemperature field will result in the modified temperature being loaded along with the default values from the environment file for the other 
parameters. To remove a condition from the patient, highlight the condition and click the Clear button.
<img src="./images/GUI/Conditions.png" alt="Conditions"> 
<br>

###Actions###
The Actions window contains all available actions for the scenario. To add an action, select it from the list and click the Add button. Enter any action parameters 
in the window that pops up and click OK to accept. You will see the Action added in the right-side window. To edit an already added action, highlight it in the list 
and click the Edit button. You can also move a highlighted action up or down in the list by clicking the up and down arrows. Actions can be removed from the list by 
clicking the Delete button. Note that the Advance Time action must be added whenever you want the engine to progress, so a scenario with no Advance Time actions will 
not produce any meaningful data. Like with conditions, leaving certain action parameters blank will result in no change being applied when the action is initiated. 
<img src="./images/GUI/ScenarioActions.png" alt="Scenario Actions">

###Logs###
The Log area is located at the bottom of the GUI and contains information about what 
the user has been doing. The Log displays information, warnings, and error messages due to user interaction with the GUI.
<img src="./images/GUI/ConsoleLog.png" alt="Console Log">

###Scenario Execution###
The Scenario Execution options are available from the set of buttons in the Engine section.
<img src="./images/GUI/EngineOptions.png" alt="Engine Options">
These three buttons perform the following functions:
- Output - Choose the data to pull from the engine as the scenario executes
- Execute - Run the scenario and generate and plot the results
- Results - View the plots generated by the executed scenario<br>

<br>
<b>Output Button</b>
<br>
The Output button brings up a dialog where you can select which data you would like to pull from the engine as it runs. 
If the scenario file already contains data requests, those data requests are pre-loaded in the dialog.
You can specify a sampling rate by setting a value for 'Output file samples per second'. This will help in creating smaller output files, but note that the data is not interpolated in any way.
Clicking Cancel or the X in the title bar will discard any additions or removals you made to data requests.
Clicking OK will save the chosen data requests to the scenario XML file. 
You can set the name of the results file by clicking the SetResults button and specifying a .txt file in the location of your choice.<br>

<br>
<b>Removing Data Requests</b>
<br>
To remove a data request, simply click the Remove button next to the data request you want to remove.
<img src="./images/GUI/OutputWindow.png" alt="Output Window">

<br>
<b>Adding System Data Requests</b>
<br>
To add a System data request, click the System button. In the first dropdown menu, select the system in which your desired data resides. Next, specify the data and the data's unit. Finally, click Add to generate the data request.
<img src="./images/GUI/SystemDataRequests.png" alt="System Data Requests">

<br>
<b>Adding Compartment Data Requests</b>
<br>
The method for creating Compartment data requests is nearly identical to that of creating System data requests, just with a few more options.
<br>
- First choose the type of compartment that you are interested in, i.e. Vascular or %Tissue.
- The second column of drop-down boxes is for the particular compartment.
- Next choose the property you are interested in and the unit. 
- Finally, click Add to create the data request.
- <b>Note:</b> For all compartment types except %Tissue, you can check the Substance box to generate data requests for certain substances in a compartment.

<img src="./images/GUI/CompartmentDataRequests.png" alt="Compartment Data Requests" >
<img src="./images/GUI/CompartmentSubstances.png" alt="Compartment Substances" >

<br>
<b>Adding Equipment Data Requests</b>
<br>
Equipment data requests are handled the same as above; select your desired equipment, property, and unit, and click Add to generate the data request.
<img src="./images/GUI/EquipmentDataRequests.png" alt="Equipment Data Requests">

<br>
<b>Adding Substance Data Requests</b>
<br>
To add a Substance data request, select the desired substance, property, and unit in the drop-down boxes. You can create a data request for a particular tissue effect by checking the %Tissue Effect box and selecting the desired tissue.
<img src="./images/GUI/SubstanceDataRequests.png" alt="Substance Data Requests" >

<br>
<b>Execution</b>
<br>
Once the scenario has a patient, actions, and data requests, it is able to be executed.
Click the Execute button to bring up the Execute dialog where you can view data from the engine as it executes. Note that it can take a few seconds for the engine to stabilize and data to start appearing on the graph.
<img src="./images/GUI/Execution.png" alt="Execution">
<br>
The left text box shows all of the outputs requested for the scenario. Click on an output to view the graph for that property. 
<br>
As the engine executes the scenario, any messages, events, errors, etc., are logged in the Execution dialog console.
<img src="./images/GUI/ExecutionConsole.png" alt="Execution Console">
<br>
At any point during execution, you may cancel the calculation by clicking the Cancel button or clicking the X in the title bar.
<br>
After the engine has executed, the GUI creates graph plots of the requested data for the entire length of the scenario.
If you chose to cancel the calculation, you will be presented the option to create graphs of the data that the engine executed before you canceled. 
<img src="./images/GUI/CancelGraphs.png" alt="Cancel Graphs"><br>

The GUI will produce the following outputs in the directory you specified using the SetResults button :
- <ScenarioFileName>.log - A log file of all information produced from the execution. This contains the same content as the log console window.
- <ScenarioFileName>.txt - This file is a comma-delimited, time-ordered ASCII file that contains data values for each requested property for every time step of the engine.
- <ScenarioFileName><Assessment>@<Time> - If an Assessment action is in the scenario (e.g., Pulmonary Function Test), an XML file is written out containing the assessment data.
- <ScenarioFileName>Graphs - A directory containing plots for all of the requested data.

<br>
<b>Results</b>
<br>
Click the Results button to view the graphs created from the GUI. This viewer allows you to scroll through the plot images created. 
<br>
<b>Note:</b> The graphs are image files and do not allow for much detail or zoom capability. We recommend using a graphing application (e.g., DPLOT) that can read comma-delimited text files. 

<img src="./images/GUI/PlotViewer.png" alt="Plot Viewer">
<br>
Use the Send To button to open the graph image with a different viewer.
<br>
The following check boxes allow for viewing different images created:
<img src="./images/GUI/ShowPlots.png" alt="Show Plots">
<br>
%BioGears supports creation of the following plots:
- Standard - The default property vs time line plot.
- Error - (<b>Currently not generated by the GUI.</b>) When a baseline result file for a scenario is identified, the generated results file is compared to this baseline and this plot is the percent difference.
- Residual - (<b>Currently not generated by the GUI.</b>) When a baseline result file for a scenario is identified, the generated results file is compared to this baseline and this plot is the residual difference.
<br>
<br>

Command Line Execution
-----------------------
RunScenario.bat and RunScenario64.bat can be found in the bin directory of the extracted Toolkit download.<br>
These scripts will run BioGears in 32 or 64 bit executables.<br>
The input argument is the path to the scenario file you wish to run.
<img src="./images/GUI/ScenarioExe.png" alt="BioGears Scenario Driver Executable">

<br>This executable will produce the following outputs in the same directory as the active scenario:
- <ScenarioFileName>.log  - A log file of all information produced from the execution. This contains the same content output to the console window.
- <ScenarioFileName>Results.txt - This is a comma-delimited, time-ordered ASCII file that contains data values for each requested property for every time step of the engine.

Plot Script
-----------

GraphResults.bat can be found in the bin directory of the extracted Toolkit download.<br>
The input argument is the path to the scenario file results you wish to graph.
<img src="./images/GUI/GraphExe.png" alt="BioGears Results Plotter">

<br>When the execution of this script is complete, 
a directory is created at the same location as the scenario results file with the same name as the results file. 
This is the location where the graph images are created. 

Compare Script
--------------

CompareResults.bat can be found in the bin directory of the extracted Toolkit download.<br>
<img src="./images/GUI/CompareExe.png" alt="BioGears Results Comparison">
<br>This batch file needs to be run from the bin directory. The input arguments to the script are as follows:
- Expected results text file
- New results text file to compare to baseline 
- Percent Tolerance to use in comparisons (optional, default is 2%)
- Report differences true|false (true generates a LOT of data, if errors are found) (optional, default is false)

Full example: CompareResults.bat "TestingRespData.txt" RespiratoryOutput.txt 2.0 false

When the execution of this script is complete, 
a directory is created at the same location as the scenario results file with the same name as the results file. 
The following items are created in this directory: 
- Plot for each column of time vs. column value. Both baseline and expected lines are provided in each plot
- Plot for each column of the error over time between the baseline and the generated column data
- Plot for each column of the residual error over time between the baseline and the generated column data
- An XML report file detailing which columns did not meet match the baseline data if the Report Differences flag is true. EVERY column index that does not match the baseline is detailed in the report.

