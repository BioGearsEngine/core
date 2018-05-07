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

package mil.tatrc.physiology.biogears.gui.datarequests;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl.ScalarProperty;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.nutrition.SENutrition;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SECompartmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEEnvironmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEEquipmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEPatientDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEPhysiologyDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SESubstanceDataRequest;
import mil.tatrc.physiology.datamodel.system.SESystem;
import mil.tatrc.physiology.datamodel.system.environment.SEEnvironment;
import mil.tatrc.physiology.datamodel.system.physiology.SEGastrointestinalSystem;
import mil.tatrc.physiology.utilities.FindObjects;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;

public class DataRequestPanel extends JPanel implements ActionListener
{    
	protected final int HOR_GAP = 5;
	protected final int VER_GAP = 5;
  protected SEScenario scenario;
  
  protected JPanel                       listPanel;
	protected JScrollPane                  scrollPane;
	protected JButton                      addSysRequest;
	protected JButton                      addCmptRequest;
	protected JButton                      addEquipRequest;
	protected JButton                      addSubRequest;
	protected JPanel                       selectionGroup;
	
	protected DataRequestSelection         activeSelector;
	protected SystemRequestSelection       systemSelection;
	protected EquipmentRequestSelection    equipmentSelection;
	protected CompartmentRequestSelection  compartmentSelection;
	protected SubstanceRequestSelection    substanceSelection;
	
	protected JButton                      addRequest;
	protected Map<JButton, DataRequestRow> listPanelRows = new HashMap<JButton, DataRequestRow>();
	

	
	public DataRequestPanel(LayoutManager layout, SEScenario scenario)
	{
		super(layout);
		
		this.scenario = scenario;
		
		listPanel = new JPanel();
		listPanel.setLayout(new BoxLayout(listPanel, BoxLayout.Y_AXIS));

		// SYSTEM / PATIENT Requests
		Map<String,List<ScalarProperty>> systems2properties = new LinkedHashMap<String,List<ScalarProperty>>();
		systems2properties.put("Patient", ScalarCtrl.getScalarProperties(SEPatient.class, false));
		systems2properties.put("Environment", ScalarCtrl.getScalarProperties(SEEnvironment.class, false));		
		Set<Class<? extends SESystem>> physiologyClasses = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.physiology", SESystem.class);
		for(Class<?> phys : physiologyClasses)
		{
			
			List<ScalarProperty> props = ScalarCtrl.getScalarProperties(phys, false);
			if(phys == SEGastrointestinalSystem.class)
			{// Need to specially/manually add the properties of the child 'nutrientsinstomach' class with the matching name used in C++ (Yeah, this is pretty messy)
				List<ScalarProperty> stomachProps = ScalarCtrl.getScalarProperties(SENutrition.class, false);
				for(ScalarProperty p : stomachProps)
				{
					p.name = "Stomach "+p.name;
					props.add(p);
				}				
			}
			systems2properties.put(StringUtils.spaceCamelCase(phys.getSimpleName().substring(2)), props);
		}
		systemSelection = new SystemRequestSelection(this,systems2properties);
		
		// EQUIPMENT Requests
		Map<String,List<ScalarProperty>> equipment2properties = new LinkedHashMap<String,List<ScalarProperty>>();
		Set<Class<? extends SESystem>> equipmentClasses = FindObjects.findClassSubTypes("mil.tatrc.physiology.datamodel.system.equipment", SESystem.class);
		for(Class<?> equip : equipmentClasses)
		{
			equipment2properties.put(equip.getSimpleName().substring(2), ScalarCtrl.getScalarProperties(equip, false));
		}
		equipmentSelection = new EquipmentRequestSelection(this,equipment2properties);
		
		// SUBSTANCE Requests
		substanceSelection = new SubstanceRequestSelection(this, this.scenario.getSubstanceManager());
		
		// COMPARTMENT Requests
		compartmentSelection = new CompartmentRequestSelection(this, this.scenario.getSubstanceManager());
		
		
		activeSelector = null;
		addRequest = new JButton("Add");
		addRequest.addActionListener(this);
		
		JScrollPane scrollPane = new JScrollPane(listPanel);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		this.add(scrollPane, BorderLayout.CENTER);

		DataRequestRow row;
		for (SEDataRequest dr : this.scenario.getDataRequests().getRequestedData())
		{
			row = null;
			if(dr instanceof SEPhysiologyDataRequest || // These 3 are packed into the same drop down
				 dr instanceof SEPatientDataRequest    ||
				 dr instanceof SEEnvironmentDataRequest)
				row = systemSelection.createRow(dr);
			else if(dr instanceof SEEquipmentDataRequest)
				row = equipmentSelection.createRow(dr);
			else if(dr instanceof SESubstanceDataRequest)		
				row = substanceSelection.createRow(dr);	
			else if(dr instanceof SECompartmentDataRequest)
				row = compartmentSelection.createRow(dr);

			if(row == null)			
				Log.error("Unhandled Data Request Type : "+dr.getClass().getSimpleName());
			else
			{
				listPanel.add(row);
				listPanelRows.put(row.removeMe, row);
			}
		}

		JPanel requestCreation = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
		JLabel drLabel = new JLabel();
		drLabel.setText("Create a Data Request : ");
		
		JPanel buttonPanel = new JPanel(new FlowLayout());
		addSysRequest = new JButton("System");
		addSysRequest.addActionListener(this);
		buttonPanel.add(addSysRequest);
		addCmptRequest = new JButton("Compartment");
		addCmptRequest.addActionListener(this);
		buttonPanel.add(addCmptRequest);		
		addEquipRequest = new JButton("Equipment");
		addEquipRequest.addActionListener(this);
		buttonPanel.add(addEquipRequest);
		addSubRequest = new JButton("Substance");
		addSubRequest.addActionListener(this);
		buttonPanel.add(addSubRequest);		
		
		selectionGroup = new JPanel(new FlowLayout(FlowLayout.CENTER));
		
		requestCreation.add(drLabel, BorderLayout.NORTH);
		requestCreation.add(buttonPanel, BorderLayout.CENTER);
		requestCreation.add(selectionGroup, BorderLayout.SOUTH);
		this.add(requestCreation, BorderLayout.SOUTH);
	}
	
	public void updateScenario()
	{
		this.scenario.getDataRequests().getRequestedData().clear();
		for(DataRequestRow row : this.listPanelRows.values())
			this.scenario.getDataRequests().getRequestedData().add(row.dr);
	}
	
	public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == addSysRequest)
		{
			if(activeSelector != systemSelection)
			{
				if(activeSelector != null)
					this.selectionGroup.removeAll();
				this.activeSelector = this.systemSelection;
				this.selectionGroup.add(systemSelection);
				this.selectionGroup.add(this.addRequest);
			}
		}
		else if(e.getSource() == addCmptRequest)
		{
			if(activeSelector != compartmentSelection)
			{
				if(activeSelector != null)
					this.selectionGroup.removeAll();
				this.activeSelector = this.compartmentSelection;
				this.selectionGroup.add(compartmentSelection);
				this.selectionGroup.add(this.addRequest);
			}
		}
		else if(e.getSource() == addEquipRequest)
		{
			if(activeSelector != equipmentSelection)
			{
				if(activeSelector != null)
					this.selectionGroup.removeAll();
				this.activeSelector = this.equipmentSelection;
				this.selectionGroup.add(equipmentSelection);
				this.selectionGroup.add(this.addRequest);
			}
		}
		else if(e.getSource() == addSubRequest)
		{
			if(activeSelector != substanceSelection)
			{
				if(activeSelector != null)
					this.selectionGroup.removeAll();
				this.activeSelector = this.substanceSelection;
				this.selectionGroup.add(substanceSelection);
				this.selectionGroup.add(this.addRequest);
			}
		}		
		else if(e.getSource() == addRequest)
		{
			if(activeSelector != null)
			{
				DataRequestRow row = this.activeSelector.createRow();
				this.listPanel.add(row);
				this.listPanelRows.put(row.removeMe, row);
				this.selectionGroup.removeAll();
				this.activeSelector = null;
			}
		}
		else if(e.getSource().getClass() == JButton.class)
		{
			JButton b = (JButton)e.getSource();
			if(listPanelRows.containsKey(b))
			{
				this.listPanel.remove(listPanelRows.get(b));
				this.listPanelRows.remove(b);
			}
			else
				Log.error("I DON'T KNOW WHO THIS BUTTON IS!!");
		}
		this.revalidate();
		this.repaint();
	}
	
	public static boolean containsString(JComboBox<String> cb, String s)
	{
	  boolean found=false;
    for(int i=0; i<cb.getItemCount(); i++)
    {
      if(cb.getItemAt(i).equals(s))
      {
        found=true;
        break;
      }
    }
    return found;
	}
}