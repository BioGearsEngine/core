/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

package mil.tatrc.physiology.biogears.gui.scenario;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.*;
import javax.swing.plaf.basic.BasicArrowButton;

import mil.tatrc.physiology.biogears.gui.GUIContext;
import mil.tatrc.physiology.biogears.gui.datarequests.DataRequestWindow;
import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ScenarioData;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.actions.SEConsciousRespiration;
import mil.tatrc.physiology.datamodel.patient.actions.SEConsciousRespirationCommand;
import mil.tatrc.physiology.datamodel.patient.actions.SEHemorrhage;
import mil.tatrc.physiology.datamodel.patient.actions.SEPatientAction;
import mil.tatrc.physiology.datamodel.patient.actions.SESubstanceBolus;
import mil.tatrc.physiology.datamodel.patient.actions.SESubstanceCompoundInfusion;
import mil.tatrc.physiology.datamodel.patient.actions.SESubstanceInfusion;
import mil.tatrc.physiology.datamodel.patient.conditions.SEPatientCondition;
import mil.tatrc.physiology.datamodel.properties.CommonUnits;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.datamodel.scenario.SEScenarioInitialParameters;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAction;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAdvanceTime;
import mil.tatrc.physiology.datamodel.scenario.conditions.SECondition;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequestManager;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceCompound;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.environment.actions.SEEnvironmentAction;
import mil.tatrc.physiology.datamodel.system.environment.conditions.SEEnvironmentCondition;
import mil.tatrc.physiology.datamodel.system.equipment.anesthesia.actions.SEAnesthesiaMachineAction;
import mil.tatrc.physiology.datamodel.system.equipment.anesthesia.actions.SEAnesthesiaMachineConfiguration;
import mil.tatrc.physiology.datamodel.system.equipment.inhaler.actions.SEInhalerAction;
import mil.tatrc.physiology.datamodel.system.equipment.inhaler.actions.SEInhalerConfiguration;
import mil.tatrc.physiology.utilities.FindObjects;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;

public class ScenarioEditor extends JPanel implements ActionListener
{
	/*
	 * We'll put all the objects declared in the constructor up here as fields for now; 
	 * once we know which ones we'll need to listen to and which ones we won't, we'll 
	 * move the ones we don't need to listen to back into the constructor
	 */
	

	protected SESubstanceManager           subMgr;
	protected SEScenario                   scenario;
	protected Map<String, SEPatient>       patients = new HashMap<String,SEPatient>();
	
	protected JPanel                       gui;
	
	protected JPanel                       scenarioGrid;	
	protected JTextField                   scenarioName;
	protected JTextArea                    scenarioDescription;
	protected JScrollPane                  descriptionScroll;
	
	protected JRadioButton                 patientBtn;
	protected JComboBox<String>            patientFiles;
	protected DefaultComboBoxModel<String> patientFilesModel;
	protected String                       defaultPatient;
	protected JRadioButton                 stateBtn;
	protected JComboBox<String>            stateFiles;
	protected DefaultComboBoxModel<String> stateFilesModel;
	protected String                       defaultState;
	protected JButton                      editCondition;
	protected JButton                      clearCondition;
	protected DefaultListModel<JLabel>     conditionsListModel;
	protected JList<JLabel>                conditionsList;
	protected JScrollPane                  conditionsScroll;
	protected Set<Class<? extends SEPatientCondition>>        patientConditions;
	protected Set<Class<? extends SEEnvironmentCondition>>    environmentConditions;
	protected List<String>                 allConditionNames;
	protected Map<String,SECondition>      allConditions;
	

	protected DefaultListModel<JLabel>     actionsListModel;					
	protected JList<JLabel>                actionsList;
	protected JScrollPane                  actionsScroll;
	protected JButton                      addAction;
	protected DefaultListModel<JLabel>     activeActionsListModel;
	protected JList<JLabel>                activeActionsList;
	protected JScrollPane                  activeActionsScroll;
	protected JPanel                       activeActionsControls;
	protected JButton                      upAction;
	protected JButton                      downAction;
	protected JButton                      editAction;
	protected JButton                      deleteAction;
	protected Set<Class<? extends SEAction>>                  basicActions;
	protected Set<Class<? extends SEPatientAction>>           patientActions;
	protected Set<Class<? extends SEAnesthesiaMachineAction>> anesthesiaActions;
	protected Set<Class<? extends SEInhalerAction>>           inhalerActions;
	protected Set<Class<? extends SEEnvironmentAction>>       environmentActions;
	protected Set<Class<? extends SEAction>>                  allActions=new HashSet<Class<? extends SEAction>>();
	
	protected JButton                      outputButton;
	protected JButton                      executeButton; 
	protected JButton                      resultsButton;
	
	protected Map<Class<? extends SEAction>,Map<String,List<String>>> actionPropertyOptions;
	
  public void setSize(Component c, int width, int height)
  {
  	Dimension d = new Dimension(width,height);
  	c.setSize(d);
  	c.setMinimumSize(d);
  	c.setMaximumSize(d);
  	c.setPreferredSize(d);
  }
 
	public ScenarioEditor()
	{
		super();
		
		// I am going to set up specific options for specific properties for specific actions.. generically! 
		actionPropertyOptions = new HashMap<Class<? extends SEAction>,Map<String,List<String>>>();
		// Hemorrhage--this was already hacky with a list of "Left Arm, Right Arm, etc".  I extended the hackiness with more detailed compartments.
		List<String> hCmpts = new ArrayList<String>();	
		hCmpts.add("Head");
		hCmpts.add("Lung");
		hCmpts.add("Myocardium");
		hCmpts.add("Liver");
		hCmpts.add("Spleen");
		hCmpts.add("Splanchnic");
		hCmpts.add("Kidney");
		hCmpts.add("Small Intestine");
		hCmpts.add("Large Intestine");
		hCmpts.add("Major Artery");
		hCmpts.add("Vena Cava");
		hCmpts.add("Arm");
		hCmpts.add("Leg");
		Map<String, List<String>> hMap = new HashMap<String, List<String>>();
		hMap.put("Compartment", hCmpts);
		actionPropertyOptions.put(SEHemorrhage.class, hMap);
		// Substance Administrations
		List<String> liqDrugs = new ArrayList<String>();
		liqDrugs.add("Epinephrine");	
		liqDrugs.add("Fentanyl");
		liqDrugs.add("Furosemide");
		liqDrugs.add("Ketamine");
		liqDrugs.add("Midazolam");
		liqDrugs.add("Morphine");
		liqDrugs.add("Naloxone");
		liqDrugs.add("Norepinephrine");	
		liqDrugs.add("Pralidoxime");
		liqDrugs.add("Prednisone");
		liqDrugs.add("Propofol");
		liqDrugs.add("Rocuronium");
		liqDrugs.add("Succinylcholine");
		Map<String, List<String>> saMap = new HashMap<String, List<String>>();
		saMap.put("Substance", liqDrugs);
		actionPropertyOptions.put(SESubstanceBolus.class, saMap);
		actionPropertyOptions.put(SESubstanceInfusion.class, saMap);
		List<String> liqCmpds = new ArrayList<String>();
		liqCmpds.add("Blood");	
		liqCmpds.add("Saline");
		Map<String, List<String>> caMap = new HashMap<String, List<String>>();
		caMap.put("SubstanceCompound", liqCmpds);
		actionPropertyOptions.put(SESubstanceCompoundInfusion.class, caMap);
		// Anesthesia Machine Substances
		List<String> anesDrugs = new ArrayList<String>();
		anesDrugs.add("Cortexiphan");
		anesDrugs.add("Desflurane");
		Map<String, List<String>> anesMap = new HashMap<String, List<String>>();
		anesMap.put("Substance", anesDrugs);
		actionPropertyOptions.put(SEAnesthesiaMachineConfiguration.class, anesMap);
	  // Inhaler Substances
		List<String> iDrugs = new ArrayList<String>();
		iDrugs.add("Albuterol");
		Map<String, List<String>> iMap = new HashMap<String, List<String>>();
		iMap.put("Substance", iDrugs);
		actionPropertyOptions.put(SEInhalerConfiguration.class, iMap);
		
		gui = new JPanel();
		gui.setLayout(new BoxLayout(gui, BoxLayout.Y_AXIS));
		gui.setSize(new Dimension(600, 425));	

		// Scenario Name/Description
		scenarioGrid = new JPanel(new GridBagLayout());	
		
		// Name
		scenarioGrid.add(new JLabel("Name:",JLabel.LEFT), GUIContext.createGBC(0,1, 0.1,1, 1,1, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		scenarioName = new JTextField(10);		
		setSize(scenarioName,200, 25);
		scenarioGrid.add(scenarioName, GUIContext.createGBC(1,1, 0.7,1, 1,1, GridBagConstraints.EAST, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		//Description
		scenarioGrid.add(new JLabel("Description:",JLabel.LEFT), GUIContext.createGBC(0,2, 0.1,1, 1,1, GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		scenarioDescription = new JTextArea();
		scenarioDescription.setLineWrap(true);
		scenarioDescription.setWrapStyleWord(true);
		descriptionScroll = new JScrollPane(scenarioDescription);
		descriptionScroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);	setSize(descriptionScroll,200, 150);		
		scenarioGrid.add(descriptionScroll, GUIContext.createGBC(1,2, 0.7,1, 1,1, GridBagConstraints.NORTHEAST, GridBagConstraints.BOTH, new Insets(0,0,0,5)));

		// Engine State||Patient
		ButtonGroup initBtnGroup = new ButtonGroup();
		// State
		stateBtn = new JRadioButton("Engine State");		
		stateBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
		setSize(stateBtn,150, 25);
		initBtnGroup.add(stateBtn);
		stateBtn.addActionListener(this);
		scenarioGrid.add(stateBtn, GUIContext.createGBC(3,0, 0.1,1, 1,1, GridBagConstraints.PAGE_END, GridBagConstraints.HORIZONTAL, new Insets(0,0,0,5)));
		List<String> stateXMLs = getXMLFilenameList(new File(System.getProperty("user.dir") + "/states"));
		for(String sFile : stateXMLs)
		{			
			if(sFile.indexOf("StandardMale")>-1)
				defaultState = sFile;
		}
		stateFilesModel = new DefaultComboBoxModel<String>(stateXMLs.toArray(new String[] {}));
		stateFiles = new JComboBox<String>(stateFilesModel);
		setSize(stateFiles,150, 25);
		stateFiles.setSelectedIndex(-1);
		stateFiles.setEditable(true);
		stateFiles.setEnabled(true);
		stateFiles.addActionListener(this);
		scenarioGrid.add(stateFiles, GUIContext.createGBC(3,1, 0.1,1, 1,2, GridBagConstraints.PAGE_START, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		// Patient
		patientBtn = new JRadioButton("Patient File");
		patientBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
		setSize(patientBtn,150, 25);
		initBtnGroup.add(patientBtn);
		patientBtn.addActionListener(this);
		scenarioGrid.add(patientBtn, GUIContext.createGBC(2,0, 0.1,1, 1,1, GridBagConstraints.PAGE_END, GridBagConstraints.HORIZONTAL, new Insets(0,0,0,5)));
		List<String> patientXMLs = getXMLFilenameList(new File(System.getProperty("user.dir") + "/patients"));
		patientFilesModel = new DefaultComboBoxModel<String>(patientXMLs.toArray(new String[] {}));
		patientFiles = new JComboBox<String>(patientFilesModel);
		setSize(patientFiles,150, 25);
		patientFiles.setSelectedIndex(-1);
		patientFiles.setEditable(true);
		patientFiles.setEnabled(true);
		patientFiles.addActionListener(this);
		scenarioGrid.add(patientFiles, GUIContext.createGBC(2,1, 0.1,1, 1,2, GridBagConstraints.PAGE_START, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));	
		// Load up those patients
		for(String pFile : patientXMLs)
		{
			SEPatient patient = new SEPatient();
			patient.loadPatientFile("./patients/"+pFile);
			this.patients.put(pFile, patient);
			if(pFile.indexOf("StandardMale")>-1)
				defaultPatient = pFile;
		}
		// Conditions List
		JPanel conditionsPanel = new JPanel(new GridBagLayout());
		conditionsListModel = new DefaultListModel<JLabel>();
		conditionsList = new JList<JLabel>(conditionsListModel);	
		conditionsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		conditionsScroll = new JScrollPane(conditionsList);
		conditionsScroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		setSize(conditionsScroll,250, 150);
		allConditionNames = new ArrayList<String>();
		allConditions = new HashMap<String,SECondition>();
		Set<Class<? extends SEPatientCondition>> patientConditions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.patient.conditions", SEPatientCondition.class);		
		for(Class<? extends SEPatientCondition> c : patientConditions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			try{
				allConditionNames.add(StringUtils.spaceCamelCase(c.getSimpleName()));
				allConditions.put(StringUtils.spaceCamelCase(c.getSimpleName()),c.newInstance());
			}catch(Exception ex){Log.error("Unable to create condition : "+c.getSimpleName(),ex);}
		}
		Set<Class<? extends SEEnvironmentCondition>> envConditions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.environment.conditions", SEEnvironmentCondition.class);		
		for(Class<? extends SEEnvironmentCondition> c : envConditions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			try{
				allConditionNames.add(StringUtils.spaceCamelCase(c.getSimpleName()));
				allConditions.put(StringUtils.spaceCamelCase(c.getSimpleName()),c.newInstance());
			}catch(Exception ex){Log.error("Unable to create condition : "+c.getSimpleName(),ex);}
		}
		Collections.sort(allConditionNames);
		for(String s : allConditionNames)
			conditionsListModel.addElement(new JLabel(s+" (Inactive)"){public String toString() {return getText();}});
		conditionsPanel.add(conditionsScroll, GUIContext.createGBC(0,0, 1,1, 1,2, GridBagConstraints.NORTH, GridBagConstraints.BOTH, new Insets(0,0,0,5)));
		//  Buttons
		JPanel conditionButtonsPanel = new JPanel(new GridBagLayout());		
		editCondition = new JButton("Edit");
		editCondition.addActionListener(this);
		conditionButtonsPanel.add(editCondition, GUIContext.createGBC(0,0, 0.1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		clearCondition = new JButton("Clear");
		clearCondition.addActionListener(this);
		conditionButtonsPanel.add(clearCondition, GUIContext.createGBC(0,1, 0.1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		conditionsPanel.add(conditionButtonsPanel, GUIContext.createGBC(1,0, 0.1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));		
		scenarioGrid.add(conditionsPanel, GUIContext.createGBC(2,2, 0.05,1, 2,2, GridBagConstraints.NORTH, GridBagConstraints.BOTH, new Insets(0,0,0,0)));			
				
		// Available Actions			
		List<String> bActions = new ArrayList<String>();
		basicActions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.scenario.actions", SEAction.class);		
		for(Class<?> c : basicActions)
		{
			bActions.add(StringUtils.spaceCamelCase(c.getSimpleName().substring(2)));
		}
		Collections.sort(bActions);
		allActions.addAll(basicActions);
		List<String> pActions = new ArrayList<String>();
		patientActions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.patient.actions", SEPatientAction.class);		
		for(Class<?> c : patientActions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			pActions.add(StringUtils.spaceCamelCase(c.getSimpleName().substring(2)));
		}
		Collections.sort(pActions);
		allActions.addAll(patientActions);
		List<String> eActions = new ArrayList<String>();
		environmentActions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.environment.actions", SEEnvironmentAction.class);		
		for(Class<?> c : environmentActions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			eActions.add(StringUtils.spaceCamelCase(c.getSimpleName().substring(2)));
		}		
		Collections.sort(eActions);
		allActions.addAll(environmentActions);
		List<String> aActions = new ArrayList<String>();
		anesthesiaActions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.equipment.anesthesia.actions", SEAnesthesiaMachineAction.class);		
		for(Class<?> c : anesthesiaActions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			aActions.add(StringUtils.spaceCamelCase(c.getSimpleName().substring(2)));
		}		
		Collections.sort(aActions);
		allActions.addAll(anesthesiaActions);
		List<String> iActions = new ArrayList<String>();
		inhalerActions = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.equipment.inhaler.actions", SEInhalerAction.class);		
		for(Class<?> c : inhalerActions)
		{
			if(Modifier.isAbstract(c.getModifiers()))
				continue;
			iActions.add(StringUtils.spaceCamelCase(c.getSimpleName().substring(2)));
		}		
		Collections.sort(iActions);
		allActions.addAll(inhalerActions);
		
		
	  // Actions
	  scenarioGrid.add(new JLabel("Actions:",JLabel.LEFT), GUIContext.createGBC(0,5, 0,1, 1,1, GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));		
		actionsListModel = new DefaultListModel<JLabel>();
		actionsList = new JList<JLabel>(actionsListModel);	
		actionsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		actionsScroll = new JScrollPane(actionsList);
		actionsScroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		setSize(actionsScroll,200, 250);
		for(String s : bActions)
			actionsListModel.addElement(new JLabel(s){public String toString() {return getText();}});	
		//Patient Actions
		actionsListModel.addElement(new JLabel("<html><b>Patient Actions</b></html>"){public String toString() {return getText();}});	
		for(String s : pActions)
			actionsListModel.addElement(new JLabel("  "+s){public String toString() {return getText();}});	
		// Environment Actions
		actionsListModel.addElement(new JLabel("<html><b>Environment Actions</b></html>"){public String toString() {return getText();}});	
		for(String s : eActions)
			actionsListModel.addElement(new JLabel("  "+s){public String toString() {return getText();}});	
		//Anesthesia Machine Actions
		actionsListModel.addElement(new JLabel("<html><b>Anesthesia Machine Actions</b></html>"){public String toString() {return getText();}});
		for(String s : aActions)
			actionsListModel.addElement(new JLabel("  "+s){public String toString() {return getText();}});	
		//Inhaler Actions
		actionsListModel.addElement(new JLabel("<html><b>Inhaler Machine Actions</b></html>"){public String toString() {return getText();}});	
		for(String s : iActions)
			actionsListModel.addElement(new JLabel("  "+s){public String toString() {return getText();}});	
		scenarioGrid.add(actionsScroll, GUIContext.createGBC(1,5, 1,1, 1,1, GridBagConstraints.NORTHEAST, GridBagConstraints.BOTH, new Insets(5,0,0,5)));			
		// Active Actions
		JPanel activeActionsPanel = new JPanel(new GridBagLayout());
		activeActionsListModel = new DefaultListModel<JLabel>();
		activeActionsList = new JList<JLabel>(activeActionsListModel);	
		activeActionsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		activeActionsScroll = new JScrollPane(activeActionsList);
		activeActionsScroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		setSize(activeActionsScroll,265, 250);
		activeActionsPanel.add(activeActionsScroll, GUIContext.createGBC(0,0, 1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.BOTH, new Insets(0,0,0,5)));
		JPanel engineButtonsPanel = new JPanel(new GridBagLayout());
		outputButton = new JButton("Output"); 
		outputButton.addActionListener(this); 
		engineButtonsPanel.add(outputButton, GUIContext.createGBC(0,0, 1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		executeButton = new JButton("Execute"); 
		executeButton.addActionListener(this);
		engineButtonsPanel.add(executeButton, GUIContext.createGBC(0,1, 1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		resultsButton = new JButton("Results"); 
		resultsButton.addActionListener(this); 
		engineButtonsPanel.add(resultsButton, GUIContext.createGBC(0,2, 1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		activeActionsPanel.add(engineButtonsPanel, GUIContext.createGBC(1,0, 0.1,1, 1,1, GridBagConstraints.NORTH, GridBagConstraints.HORIZONTAL, new Insets(0,0,5,5)));
		
		
		activeActionsControls = new JPanel(new FlowLayout(FlowLayout.LEFT));		
		upAction = new BasicArrowButton(BasicArrowButton.NORTH);
		upAction.addActionListener(this);
		activeActionsControls.add(upAction);
		addAction = new JButton("Add");
		addAction.addActionListener(this);
		activeActionsControls.add(addAction);
		editAction = new JButton("Edit");
		editAction.addActionListener(this);
		activeActionsControls.add(editAction);
		deleteAction = new JButton("Delete");
		deleteAction.addActionListener(this);
		activeActionsControls.add(deleteAction);
		downAction = new BasicArrowButton(BasicArrowButton.SOUTH);
		downAction.addActionListener(this);
		activeActionsControls.add(downAction);
		activeActionsPanel.add(activeActionsControls, GUIContext.createGBC(0,1, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0,0,0,0)));
		
		scenarioGrid.add(activeActionsPanel, GUIContext.createGBC(2,5, 1,1, 2,1, GridBagConstraints.NORTH, GridBagConstraints.BOTH, new Insets(5,0,0,0)));

		gui.add(scenarioGrid);
		add(gui);
		
		subMgr = new SESubstanceManager();
		clear();	
	}
	
	public void clear()
	{
		this.scenario = new SEScenario(subMgr);			
		this.scenarioName.setText("MyScenario");
		this.scenarioDescription.setText("");		
		this.stateBtn.setSelected(true);
		this.stateFiles.setSelectedItem(defaultState);
		this.stateFiles.setEnabled(true);
		this.patientFiles.setEnabled(false);	
		this.patientFiles.setSelectedIndex(-1);
				
		this.conditionsList.removeAll();
		this.conditionsListModel.removeAllElements();
		for(String s : allConditionNames)
			conditionsListModel.addElement(new JLabel(s+" (Inactive)"){public String toString() {return getText();}});
		this.conditionsList.setEnabled(false);
		this.editCondition.setEnabled(false);
		this.clearCondition.setEnabled(false);
		this.activeActionsList.removeAll();
		this.activeActionsListModel.removeAllElements();		
	}

	public boolean loadScenarioFile(File file)
	{	
		clear();
    Object obj = CDMSerializer.readFile(file.getAbsolutePath());
		if(obj instanceof ScenarioData)
		{
			if(!this.scenario.load((ScenarioData)obj))
			{
				GUIContext.getConsole().println(file.getName()+" is not a valid scenario file");
				return false;
			}
		}
		else
		{
			GUIContext.getConsole().println(file.getName()+" is not a scenario file");
			return false;
		}
		
		this.scenarioName.setText(this.scenario.getName());
		this.scenarioDescription.setText(this.scenario.getDescription());
		
		SEDataRequestManager drs = this.scenario.getDataRequests();
		if(!drs.hasFilename())
		{
			String resultsFile = file.getAbsolutePath().replaceAll(".xml", "Results.txt");
			drs.setFilename(resultsFile);
		}
		
		// Let's see if we can replace a patient file with a state file
		if(this.scenario.hasInitialParameters() && this.scenario.getInitialParameters().getConditions().isEmpty())
		{
			SEScenarioInitialParameters params = this.scenario.getInitialParameters();
			String pFile = params.getPatientFile();
			if(pFile.startsWith("./patients/"))
				pFile = pFile.substring(10);
			File stateFile = new File("./states/"+pFile.substring(0, pFile.indexOf(".xml"))+"@0s.xml");
			if(stateFile.exists())
			{
				this.scenario.setEngineState(stateFile.getAbsolutePath());
				Log.info("I have replaced the patient file with the complementary engine state file");
			}
		}
		
		if(this.scenario.hasInitialParameters())
		{
			SEScenarioInitialParameters params = this.scenario.getInitialParameters();
			String pFile = params.getPatientFile();			
			if(pFile.startsWith("./patients/"))
				pFile = pFile.substring(10);
			this.patientFiles.setSelectedItem(pFile);
								
			SEPatient patient = this.patients.get(pFile);
			if (patient == null)
			{
				patient = new SEPatient();
				patient.loadPatientFile("./patients/"+pFile);
				this.patients.put(pFile, patient);
			}
			
			this.patientBtn.setSelected(true);						
			this.patientFiles.setEnabled(true);	
			this.stateFiles.setSelectedIndex(-1);
			this.stateFiles.setEnabled(false);
			this.conditionsList.setEnabled(true);
			this.editCondition.setEnabled(true);
			this.clearCondition.setEnabled(true);
			this.patientFiles.setSelectedItem(pFile);
							
			this.conditionsList.removeAll();
			this.conditionsListModel.removeAllElements();
			for(String s : allConditionNames)
				conditionsListModel.addElement(new JLabel(s+" (Inactive)"){public String toString() {return getText();}});
							
			for (SECondition c : params.getConditions())	
			{
				String name = StringUtils.spaceCamelCase(c.getClass().getSimpleName());
				conditionsList.setSelectedIndex(allConditionNames.indexOf(name));
				UpdateModelContent(c.toString(), conditionsListModel, conditionsList);
			}
		}
		else if(this.scenario.hasEngineState())
		{					
			this.stateBtn.setSelected(true);
			
			String sFile = this.scenario.getEngineState();
			if(sFile.startsWith("./states/"))
				sFile = sFile.substring(9);
			this.patientFiles.setSelectedItem(sFile);						
		}
			
		this.activeActionsList.removeAll();
		this.activeActionsListModel.removeAllElements();
		for (SEAction action : this.scenario.getActions())
			AddAction(action);
		UpdateActionTimes();
				
		return true;
	}
	
	public boolean saveScenarioFileAs(String fileName)
	{
		this.scenario.setName(this.scenarioName.getText());
		this.scenario.setDescription(this.scenarioDescription.getText());
		CDMSerializer.writeFile(fileName, this.scenario.unload());
		return true;
	}
	
	public List<String> getXMLFilenameList(File dir)
	{
		List<String> files = new ArrayList<String>();
		if(!dir.exists())
		{
			Log.error("XMLFilenameList cannot find directory : "+dir.getAbsolutePath());
			return files;
		}
		for (final File file : dir.listFiles())
			if (file.getName().endsWith(".xml"))
				files.add(file.getName());		
		return files;
	}

	public void actionPerformed(ActionEvent e)
	{
		Object src = e.getSource();
		
		if(src == this.stateBtn)
		{
			this.stateFiles.setEnabled(true);
			this.stateFiles.setSelectedItem(defaultState);
			
			this.patientFiles.setEnabled(false);	
			this.patientFiles.setSelectedIndex(-1);
			
			this.conditionsList.removeAll();
			this.conditionsListModel.removeAllElements();
			for(String s : allConditionNames)
				conditionsListModel.addElement(new JLabel(s+" (Inactive)"){public String toString() {return getText();}});
			this.conditionsList.setEnabled(false);
			this.editCondition.setEnabled(false);
			this.clearCondition.setEnabled(false);
			return;
		}
		else if(src == this.stateFiles)
		{
			if(this.stateFiles.getSelectedIndex()==-1)
				return;	
			this.scenario.invalidateInitialParameters();
			this.scenario.setEngineState("./states/"+this.stateFiles.getSelectedItem());
			return;
		}
		else if(src == this.patientBtn)
		{
			this.patientFiles.setEnabled(true);	
			this.stateFiles.setSelectedIndex(-1);
			this.stateFiles.setEnabled(false);
			this.conditionsList.setEnabled(true);
			this.editCondition.setEnabled(true);
			this.clearCondition.setEnabled(true);
			this.patientFiles.setSelectedItem(defaultPatient);
			return;
		}
		else if(src == this.patientFiles)
		{
			if(this.patientFiles.getSelectedIndex()==-1)
				return;			
			this.scenario.invalidateEngineState();
			this.scenario.getInitialParameters().setPatientFile((String)this.patientFiles.getSelectedItem());
			return;
		}
		else if(src == this.editCondition)
		{
			if(conditionsList.getSelectedIndex()==-1)
				return;
			String name = allConditionNames.get(conditionsList.getSelectedIndex());
			SECondition c = allConditions.get(name);						
			DynamicPropertyEditor dlg = new DynamicPropertyEditor(c, null);
			dlg.setVisible(true);			
			if(!dlg.wasCancelled())
			{
				if(!this.scenario.getInitialParameters().getConditions().contains(c))
					this.scenario.getInitialParameters().getConditions().add(c);
				UpdateModelContent(c.toString(), conditionsListModel, conditionsList);
			}
			return;
		}
		else if(src == this.clearCondition)
		{
			if(conditionsList.getSelectedIndex()==-1)
				return;
			String name = allConditionNames.get(conditionsList.getSelectedIndex());
			SECondition c = allConditions.get(name);								
			this.scenario.getInitialParameters().getConditions().remove(c);
			UpdateModelContent(name+" (Inactive)", conditionsListModel, conditionsList);
			return;
		}
		
		else if (src == this.addAction)
		{		
			String actionName = actionsList.getSelectedValue().getText().trim();
			if(actionName == null || actionName.indexOf("Actions")>-1)
				return;
			
			SEAction action = null;						
			for(Class<?> c : allActions)
			{
				try
				{
					if(c.getSimpleName().equals("SE"+StringUtils.removeSpaces(actionName)))				
					{
						action = (SEAction) c.newInstance();
						break;
					}
				}
				catch(Exception ex)
				{
					try					
					{						
						Constructor<?>[] ctors = c.getConstructors();
						for(Constructor<?> ctor : ctors)
						{
							if(ctor.getParameterTypes()[0]==SESubstance.class)
							{
								SESubstance sub = new SESubstance();
								action = (SEAction)ctor.newInstance(sub);
								break;
							}
							else if(ctor.getParameterTypes()[0]==SESubstanceCompound.class)
							{
								SESubstanceCompound cmpd = new SESubstanceCompound();
								action = (SEAction)ctor.newInstance(cmpd);
								break;
							}
						}						 										
					}
					catch(Exception ex2)
					{
						
					}
				}
			}
			if(action != null)
			{
				boolean cancelled=true;
				if(SEConsciousRespiration.class.isAssignableFrom(action.getClass()))
				{
					ConsciousRespirationEditor dlg = new ConsciousRespirationEditor((SEConsciousRespiration)action, actionPropertyOptions.get(action.getClass()));
					dlg.setVisible(true);
					cancelled = dlg.wasCancelled();
				}
				else
				{
					DynamicPropertyEditor dlg = new DynamicPropertyEditor(action, actionPropertyOptions.get(action.getClass()));
					dlg.setVisible(true);
					cancelled = dlg.wasCancelled();
				}
				
				if(!cancelled)
				{
					AddAction(action);
					this.scenario.getActions().add(action);
				}
			}
			else
			{
				GUIContext.getConsole().println("I am not sure what kind of action "+actionName+" is");
				return;
			}
		}
		else if (src == this.deleteAction)
		{
			int selectedIndex = activeActionsList.getSelectedIndex();
			if(selectedIndex > -1)
			{
				activeActionsListModel.remove(selectedIndex);
				this.scenario.getActions().remove(selectedIndex);

				if (selectedIndex >= activeActionsListModel.getSize())
					activeActionsList.setSelectedIndex(activeActionsListModel.getSize() - 1);
				else
					activeActionsList.setSelectedIndex(selectedIndex);
			}
			return;
		}
		else if (src == this.editAction)
		{
			int selectedIndex = activeActionsList.getSelectedIndex();
			if(selectedIndex > -1)
			{
				SEAction action = this.scenario.getActions().get(selectedIndex);
				
				if(SEConsciousRespiration.class.isAssignableFrom(action.getClass()))
				{
					ConsciousRespirationEditor dlg = new ConsciousRespirationEditor((SEConsciousRespiration)action, actionPropertyOptions.get(action.getClass()));
					dlg.setVisible(true);
				}
				else
				{
					DynamicPropertyEditor dlg = new DynamicPropertyEditor(action, actionPropertyOptions.get(action.getClass()));
					dlg.setVisible(true);
				}
				UpdateModelContent(this.scenario.getActions().get(selectedIndex).toString(), activeActionsListModel, activeActionsList);
			}
			return;
		}
		else if (src == this.upAction)
		{
			if (activeActionsList.getSelectedIndex() > 0)
			{
				int fromIndex = activeActionsList.getSelectedIndex();
				int toIndex = fromIndex - 1;
				
				SEAction aTemp = this.scenario.getActions().get(fromIndex);
				this.scenario.getActions().set(fromIndex, this.scenario.getActions().get(toIndex));
				this.scenario.getActions().set(toIndex, aTemp);
				
				JLabel temp = (JLabel) activeActionsListModel.get(fromIndex);
				activeActionsListModel.set(fromIndex, activeActionsListModel.get(toIndex));
				activeActionsListModel.set(toIndex, temp);
				
				activeActionsList.setSelectedIndex(toIndex);
			}
		}
		else if (src == this.downAction)
		{
			if (activeActionsList.getSelectedIndex() < activeActionsList.getModel().getSize() - 1)
			{
				int fromIndex = activeActionsList.getSelectedIndex();
				int toIndex = fromIndex + 1;
				
				SEAction aTemp = this.scenario.getActions().get(fromIndex);
				this.scenario.getActions().set(fromIndex, this.scenario.getActions().get(toIndex));
				this.scenario.getActions().set(toIndex, aTemp);
								
				JLabel temp = (JLabel) activeActionsListModel.get(fromIndex);
				activeActionsListModel.set(fromIndex, activeActionsListModel.get(toIndex));
				activeActionsListModel.set(toIndex, temp);
				
				activeActionsList.setSelectedIndex(toIndex);
			}
		}
		else if (src == this.executeButton)
		{			
			if (this.scenario.getDataRequests().getRequestedData().isEmpty())
			{
			    JOptionPane.showMessageDialog(this, "No data is requested for the current scenario", "No requested data", JOptionPane.ERROR_MESSAGE);
          return;
			}
			if (this.scenario.getActions().isEmpty())
			{
			    JOptionPane.showMessageDialog(this, "No actions in scenario", "No requested data", JOptionPane.ERROR_MESSAGE);
          return;
			}
			if (!this.scenario.hasEngineState() && !this.scenario.hasInitialParameters())
			{
				JOptionPane.showMessageDialog(this, "No patient data is set for the current scenario", "No requested data", JOptionPane.ERROR_MESSAGE);
        return;
			}
			this.scenario.setName(scenarioName.getText());
			this.scenario.setDescription(scenarioDescription.getText());
			SEDataRequestManager drs = this.scenario.getDataRequests();
			if(!drs.hasFilename())
				drs.setFilename("./"+this.scenario.getName()+"Results.txt");
			ScenarioCalcDialog dlg = new ScenarioCalcDialog(this.scenario);
			dlg.setVisible(true);
			return;
		}
		else if (src == this.resultsButton)
		{			
			if (!this.scenario.getDataRequests().hasFilename())
			{
			    JOptionPane.showMessageDialog(this, "No results file has been specified", "No requested data", JOptionPane.ERROR_MESSAGE);
          return;
			}
			String resultsFile = scenario.getDataRequests().getFilename().replaceAll("[\\\\]", "/");			
	    File resultsDir = new File(resultsFile.substring(0,resultsFile.lastIndexOf("/"))+"/"+scenario.getName()+"Graphs");
	    
			if (resultsDir.exists() && resultsDir.isDirectory())			
				new ResultsWindow(resultsDir);
			else
				JOptionPane.showMessageDialog(this, "\""+resultsDir.getAbsolutePath()+"\" does not exist or is not a directory", "No Results Found", JOptionPane.ERROR_MESSAGE);
			return;
		}
		else if (src == this.outputButton)
		{
			if (this.scenario == null)
			{
				JOptionPane.showMessageDialog(this, "No scenario is currently loaded", "No Scenario", JOptionPane.ERROR_MESSAGE);
				return;
			}
			
			new DataRequestWindow(scenario);
			return;
		}
		
	}
	
	private void UpdateActionTimes()
	{
		double totalTime_s = 0;
		for (SEAction action : this.scenario.getActions())
		{
			if (action instanceof SEAdvanceTime)
			    totalTime_s += ((SEAdvanceTime)action).getTime().getValue(CommonUnits.TimeUnit.s);
		  action.getScenarioTime().setValue(totalTime_s, TimeUnit.s);
		}
	}	
	private void AddAction(SEAction action)
	{		
		String actionContent = action.toString();
		if (action instanceof SEAdvanceTime)
		{		    
		   actionContent += " (Scenario time:" + (int)action.getScenarioTime().getValue(TimeUnit.s) + "s)";
		}			
		AddContentToModel(actionContent,activeActionsListModel);
	}
	private void AddContentToModel(String content, DefaultListModel<JLabel> list)
	{
		final String formattedAction;
		
		int firstNewlineTabIndex = content.indexOf("\n\t");
		content = content.replace("\n","<br>");
		content = content.replace("\t","...");
		if (firstNewlineTabIndex != -1)
		{   
			String boldedActionName = new StringBuilder(content).insert(firstNewlineTabIndex, "</b>").toString();
			formattedAction = ("<html><b>" + boldedActionName + "<br></html>");
		}
		else
		{
			formattedAction = "<html><b>" + content + "</b></html>";
		}
		list.addElement(new JLabel(formattedAction){public String toString() {return getText();}});
	}
	
	private void UpdateModelContent(String content, DefaultListModel<JLabel> model, JList<JLabel> list)
	{
		content = content.replace("\n","<br>");
		content = content.replace("\t","...");
		final String formattedContent;
		int firstBreakIndex = content.indexOf("<br>");
		if (firstBreakIndex != -1)
		{
			String boldedName = new StringBuilder(content).insert(firstBreakIndex, "</b>").toString();
			formattedContent = ("<html><b>" + boldedName + "<br></html>");
		}
		else if(content.indexOf("(Inactive)")>-1)
		{			
			formattedContent = "<html>" + content + "</html>";			
		}
		else
		{
			formattedContent = "<html><b>" + content + "</b></html>";
		}
		int idx = list.getSelectedIndex();
		model.insertElementAt(new JLabel(formattedContent){public String toString() {return getText();}}, idx);
		model.removeElementAt(idx+1);
		list.repaint();
	}
}
