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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.SwingConstants;

import mil.tatrc.physiology.biogears.engine.BioGearsCompartments;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl.ScalarProperty;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SESubstanceDataRequest;
import mil.tatrc.physiology.datamodel.substance.*;
import mil.tatrc.physiology.utilities.StringUtils;

public class SubstanceRequestSelection extends DataRequestSelection
{
	protected SESubstanceManager subMgr;
	protected JCheckBox          subCheck;
	protected JComboBox<String>  subPropsBox   = new JComboBox<String>();
	protected JComboBox<String>  subFxPropsBox = new JComboBox<String>();
	protected JComboBox<String>  cmptComboBox  = new JComboBox<String>();
	
  public SubstanceRequestSelection(ActionListener pnt, SESubstanceManager subMgr)
  {
  	super(pnt, null);
  	this.subMgr = subMgr;
  	
  	for(SESubstance s : subMgr.getSubstances())  	
  		categoryComboBox.addItem(s.getName());
  	categoryComboBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SESubstance.class, false))
  	{
  		subPropsBox.addItem(p.name);
  		JComboBox<String> unitsComboBox = new JComboBox<String>();
			property2unitComboBox.put(p.name, unitsComboBox);
			if(p.units != null)
				for(String unit : p.units)
					unitsComboBox.addItem(unit);
			else
				unitsComboBox.addItem("Unitless");  		
  	}
  	subPropsBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SESubstanceTissuePharmacokinetics.class, false))
  	{
  		subFxPropsBox.addItem(p.name);
  		JComboBox<String> unitsComboBox = new JComboBox<String>();
			property2unitComboBox.put(p.name, unitsComboBox);
			if(p.units != null)
				for(String unit : p.units)
					unitsComboBox.addItem(unit);
			else
				unitsComboBox.addItem("Unitless");  		
  	}
  	subFxPropsBox.addActionListener(this);
  	
  	for(Enum<?> e : BioGearsCompartments.Tissue.values())
  		cmptComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	
  	subCheck = new JCheckBox("Tissue Pk");
  	subCheck.setHorizontalAlignment(SwingConstants.CENTER);
  	subCheck.setVerticalAlignment(SwingConstants.TOP);
  	subCheck.setHorizontalTextPosition(SwingConstants.CENTER);
  	subCheck.setVerticalTextPosition(SwingConstants.BOTTOM);
  	subCheck.addActionListener(this);
    	    
  	
  	this.add(subCheck);
  	this.add(categoryComboBox);
  	this.add(subPropsBox);
		activePropBox = subPropsBox;
  	activeUnitBox = property2unitComboBox.get(activePropBox.getSelectedItem());
		this.add(activeUnitBox);  
  }
  
  public void actionPerformed(ActionEvent e)
	{
  	if(e.getSource() == this.subCheck)
  	{
  		this.removeAll();
  		this.add(subCheck);
			this.add(categoryComboBox);
  		if(this.subCheck.isSelected())
  		{
  			this.add(cmptComboBox);
  			this.add(subFxPropsBox);
  			activePropBox = subFxPropsBox;
  	  	activeUnitBox = property2unitComboBox.get(activePropBox.getSelectedItem());
  			this.add(activeUnitBox);  
  		}
  		else
  		{
  			this.add(subPropsBox);
  			activePropBox = subPropsBox;
  	  	activeUnitBox = property2unitComboBox.get(activePropBox.getSelectedItem());
  			this.add(activeUnitBox);  
  		}
  	}
  	else if(e.getSource() == this.activePropBox)
		{
			this.remove(activeUnitBox);	  	
	  	this.activeUnitBox = this.property2unitComboBox.get(this.activePropBox.getSelectedItem());
	  	this.add(activeUnitBox);
		}			
		this.revalidate();
		this.repaint();
	}
  
  public DataRequestRow createRow(SEDataRequest dr)
	{
		String label;
		String property = StringUtils.spaceCamelCase(dr.getName());
		
		SESubstanceDataRequest sdr = (SESubstanceDataRequest)dr;
		String substance = sdr.getSubstance().getName();
		
		if(!sdr.hasCompartment())
		{
			if(dr.hasUnit())
				label = substance +" "+property+" ("+dr.getUnit()+")";
			else
				label = substance +" "+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		else
		{
			if(dr.hasUnit())
				label = substance +" "+StringUtils.spaceCamelCase(sdr.getCompartment())+" "+property+" ("+dr.getUnit()+")";
			else
				label = substance +" "+StringUtils.spaceCamelCase(sdr.getCompartment())+" "+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		DataRequestRow row = new DataRequestRow(this.pntListener, label, dr);
		return row;
	}
  
	public DataRequestRow createRow()
	{
		String[] labels = new String[4];
		labels[0] = (String)this.categoryComboBox.getSelectedItem();
		labels[1] = (String)this.cmptComboBox.getSelectedItem();
		labels[2] = (String)this.activePropBox.getSelectedItem();
		labels[3] = (String)this.activeUnitBox.getSelectedItem();
		
		SESubstanceDataRequest sdr = new SESubstanceDataRequest(subMgr.getSubstance(labels[0]));
		if(activePropBox == subFxPropsBox)
		{
			sdr.setCompartment(StringUtils.removeSpaces(labels[1]));
			labels[1] += " ";
		}
		else
			labels[1] = "";
		sdr.setName(StringUtils.removeSpaces(labels[2]));
		sdr.setUnit(labels[3]);
			
		DataRequestRow row = new DataRequestRow(this.pntListener, labels[0]+" "+labels[1]+labels[2]+"("+labels[3]+")", sdr);
		return row;
	}
}
