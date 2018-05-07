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
import java.util.LinkedHashMap;
import java.util.Map;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.SwingConstants;

import mil.tatrc.physiology.biogears.engine.BioGearsCompartments;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl.ScalarProperty;
import mil.tatrc.physiology.datamodel.compartment.SEGasCompartment;
import mil.tatrc.physiology.datamodel.compartment.SELiquidCompartment;
import mil.tatrc.physiology.datamodel.compartment.SETissueCompartment;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SECompartmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEGasCompartmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SELiquidCompartmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SESubstanceDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEThermalCompartmentDataRequest;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SETissueCompartmentDataRequest;
import mil.tatrc.physiology.datamodel.substance.*;
import mil.tatrc.physiology.datamodel.substance.quantity.SEGasSubstanceQuantity;
import mil.tatrc.physiology.datamodel.substance.quantity.SELiquidSubstanceQuantity;
import mil.tatrc.physiology.utilities.StringUtils;

public class CompartmentRequestSelection extends DataRequestSelection
{
	protected SESubstanceManager subMgr;
	protected JCheckBox          subCheck;
  // I am  using categoryComboBox for the cmpt types (see BioGearsCompartments)
	protected JComboBox<String>  activeCmptBox;
	protected JComboBox<String>  cmptNameComboBox  = new JComboBox<String>();
	protected JComboBox<String>  cmptPropsBox      = new JComboBox<String>();
	protected JComboBox<String>  cmptSubPropsBox   = new JComboBox<String>();
	protected JComboBox<String>  subsBox   = new JComboBox<String>();
	
	protected Map<String, JComboBox<String>> category2cmptNameComboBox = new LinkedHashMap<String, JComboBox<String>>();
	protected Map<String, JComboBox<String>> category2propsComboBox    = new LinkedHashMap<String, JComboBox<String>>();
	protected Map<String, JComboBox<String>> category2subQComboBox    = new LinkedHashMap<String, JComboBox<String>>();
	protected Map<String,Enum<?>> category2enum = new LinkedHashMap<String,Enum<?>>();
	
	protected JComboBox<String> gasPropsComboBox         = new JComboBox<String>();
	protected JComboBox<String> gasSubQPropsComboBox     = new JComboBox<String>();
	protected JComboBox<String> liquidPropsComboBox      = new JComboBox<String>();
	protected JComboBox<String> liquidSubQPropsComboBox  = new JComboBox<String>();
	protected JComboBox<String> tissuePropsComboBox      = new JComboBox<String>();
	protected JComboBox<String> thermalPropsComboBox     = new JComboBox<String>();	
	
	JComboBox<String> chymeCmptsComboBox = new JComboBox<String>();
	JComboBox<String> pulmonaryCmptsComboBox = new JComboBox<String>();
	JComboBox<String> tissueCmptsComboBox = new JComboBox<String>();
	JComboBox<String> extravascularCmptsComboBox = new JComboBox<String>();
	JComboBox<String> lymphCmptsComboBox = new JComboBox<String>();
	JComboBox<String> urineCmptsComboBox = new JComboBox<String>();
	JComboBox<String> vascularCmptsComboBox = new JComboBox<String>();
	
	JComboBox<String> environmentCmptsComboBox = new JComboBox<String>();
	JComboBox<String> aerosolCmptsComboBox = new JComboBox<String>();
	JComboBox<String> anesthesiaMachineCmptsComboBox = new JComboBox<String>();
	JComboBox<String> inhalerCmptsComboBox = new JComboBox<String>();
	
  public CompartmentRequestSelection(ActionListener pnt, SESubstanceManager subMgr)
  {
  	super(pnt, null);
  	this.subMgr = subMgr;
  	  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SEGasCompartment.class, false))
  	{
  		gasPropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  			JComboBox<String> unitsComboBox = new JComboBox<String>();
  			property2unitComboBox.put(p.name, unitsComboBox);
  			if(p.units != null)
  				for(String unit : p.units)
  					unitsComboBox.addItem(unit);
  			else
  				unitsComboBox.addItem("Unitless");
  		}
  	}
  	gasPropsComboBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SEGasSubstanceQuantity.class, false))
  	{
  		gasSubQPropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  			JComboBox<String> unitsComboBox = new JComboBox<String>();
  			property2unitComboBox.put(p.name, unitsComboBox);
  			if(p.units != null)
  				for(String unit : p.units)
  					unitsComboBox.addItem(unit);
  			else
  				unitsComboBox.addItem("Unitless");
  		}
  	}
  	gasSubQPropsComboBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SELiquidCompartment.class, false))
  	{
  		liquidPropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  			JComboBox<String> unitsComboBox = new JComboBox<String>();
  			property2unitComboBox.put(p.name, unitsComboBox);
  			if(p.units != null)
  				for(String unit : p.units)
  					unitsComboBox.addItem(unit);
  			else
  				unitsComboBox.addItem("Unitless");
  		}	
  	}
  	liquidPropsComboBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SELiquidSubstanceQuantity.class, false))
  	{
  		liquidSubQPropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  			JComboBox<String> unitsComboBox = new JComboBox<String>();
  			property2unitComboBox.put(p.name, unitsComboBox);
  			if(p.units != null)
  				for(String unit : p.units)
  					unitsComboBox.addItem(unit);
  			else
  				unitsComboBox.addItem("Unitless");
  		}
  	}
  	liquidSubQPropsComboBox.addActionListener(this);
  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SETissueCompartment.class, false))
  	{
  		tissuePropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  			JComboBox<String> unitsComboBox = new JComboBox<String>();
  			property2unitComboBox.put(p.name, unitsComboBox);
  			if(p.units != null)
  				for(String unit : p.units)
  					unitsComboBox.addItem(unit);
  			else
  				unitsComboBox.addItem("Unitless");
  		}		
  	}
  	tissuePropsComboBox.addActionListener(this);
/*  	
  	for(ScalarProperty p : ScalarCtrl.getScalarProperties(SEThermalCompartment.class, false))
  	{
  		thermalPropsComboBox.addItem(p.name);
  		if(!property2unitComboBox.containsKey(p.name))
  		{
  		JComboBox<String> unitsComboBox = new JComboBox<String>();
  		property2unitComboBox.put(p.name, unitsComboBox);
			if(p.units != null)
				for(String unit : p.units)
					unitsComboBox.addItem(unit);
			else
				unitsComboBox.addItem("Unitless");
  		}
  	}
  	thermalPropsComboBox.addActionListener(this);
*/  	
  	categoryComboBox.addItem("Chyme");
  	for(Enum<?> e : BioGearsCompartments.Chyme.values())
  		chymeCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Chyme", chymeCmptsComboBox);
  	category2propsComboBox.put("Chyme", liquidPropsComboBox);
  	category2subQComboBox.put("Chyme", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Pulmonary");
  	for(Enum<?> e : BioGearsCompartments.Pulmonary.values())
  		pulmonaryCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Pulmonary", pulmonaryCmptsComboBox);
  	category2propsComboBox.put("Pulmonary", gasPropsComboBox);
  	category2subQComboBox.put("Pulmonary", gasSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Tissue");
  	for(Enum<?> e : BioGearsCompartments.Tissue.values())
  		tissueCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Tissue", tissueCmptsComboBox);
  	category2propsComboBox.put("Tissue", tissuePropsComboBox);
  	// No Substances - category2subQComboBox
  	
  	categoryComboBox.addItem("Extravascular");
  	for(Enum<?> e : BioGearsCompartments.Extravascular.values())
  		extravascularCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Extravascular", extravascularCmptsComboBox);
  	category2propsComboBox.put("Extravascular", liquidPropsComboBox);
  	category2subQComboBox.put("Extravascular", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Lymph");
  	for(Enum<?> e : BioGearsCompartments.Lymph.values())
  		lymphCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Lymph", lymphCmptsComboBox);
  	category2propsComboBox.put("Lymph", liquidPropsComboBox);
  	category2subQComboBox.put("Lymph", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Urine");
  	for(Enum<?> e : BioGearsCompartments.Urine.values())
  		urineCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Urine", urineCmptsComboBox);
  	category2propsComboBox.put("Urine", liquidPropsComboBox);
  	category2subQComboBox.put("Urine", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Vascular");
  	for(Enum<?> e : BioGearsCompartments.Vascular.values())
  		vascularCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Vascular", vascularCmptsComboBox);
  	category2propsComboBox.put("Vascular", liquidPropsComboBox);
  	category2subQComboBox.put("Vascular", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Environment");
  	for(Enum<?> e : BioGearsCompartments.Environment.values())
  		environmentCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Environment", environmentCmptsComboBox);
  	category2propsComboBox.put("Environment", gasPropsComboBox);
  	category2subQComboBox.put("Environment", gasSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Aerosol");
  	for(Enum<?> e : BioGearsCompartments.Pulmonary.values())
  		aerosolCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Aerosol", aerosolCmptsComboBox);
  	category2propsComboBox.put("Aerosol", liquidPropsComboBox);
  	category2subQComboBox.put("Aerosol", liquidSubQPropsComboBox);
  	
  	categoryComboBox.addItem("AnesthesiaMachine");
  	for(Enum<?> e : BioGearsCompartments.AnesthesiaMachine.values())
  		anesthesiaMachineCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("AnesthesiaMachine", anesthesiaMachineCmptsComboBox);
  	category2propsComboBox.put("AnesthesiaMachine", gasPropsComboBox);
  	category2subQComboBox.put("AnesthesiaMachine", gasSubQPropsComboBox);
  	
  	categoryComboBox.addItem("Inhaler");
  	for(Enum<?> e : BioGearsCompartments.Inhaler.values())
  		inhalerCmptsComboBox.addItem(StringUtils.spaceCamelCase(e.name()));
  	category2cmptNameComboBox.put("Inhaler", inhalerCmptsComboBox);
  	category2propsComboBox.put("Inhaler", gasPropsComboBox);
  	category2subQComboBox.put("Inhaler", gasSubQPropsComboBox);
  	
  	categoryComboBox.addActionListener(this);
  	
  	for(SESubstance s : subMgr.getSubstances())  	
  		subsBox.addItem(s.getName());
  	subsBox.addActionListener(this);
  	
  	
  	subCheck = new JCheckBox("Substance");
  	subCheck.setHorizontalAlignment(SwingConstants.CENTER);
  	subCheck.setVerticalAlignment(SwingConstants.TOP);
  	subCheck.setHorizontalTextPosition(SwingConstants.CENTER);
  	subCheck.setVerticalTextPosition(SwingConstants.BOTTOM);
  	subCheck.addActionListener(this);
    	      	
  	this.add(subCheck);
  	this.add(categoryComboBox);
  	activeCmptBox = category2cmptNameComboBox.get(categoryComboBox.getSelectedItem());
  	this.add(activeCmptBox);
  	activePropBox = category2propsComboBox.get(categoryComboBox.getSelectedItem());
  	this.add(activePropBox);
  	activeUnitBox  = property2unitComboBox.get(activePropBox.getSelectedItem());
		this.add(activeUnitBox);  
  }
  
  public void actionPerformed(ActionEvent e)
	{
  	if(e.getSource() == this.subCheck)
  	{
  		this.removeAll();
  		this.add(subCheck);
			this.add(categoryComboBox);
			this.add(activeCmptBox);
  		if(this.subCheck.isSelected())
  		{
  			this.add(subsBox);
  			activePropBox = category2subQComboBox.get(categoryComboBox.getSelectedItem());
  	  	this.add(activePropBox);
  	  	activeUnitBox  = property2unitComboBox.get(activePropBox.getSelectedItem());
  			this.add(activeUnitBox);  
  		}
  		else
  		{
  			activePropBox = category2propsComboBox.get(categoryComboBox.getSelectedItem());
  	  	this.add(activePropBox);
  	  	activeUnitBox  = property2unitComboBox.get(activePropBox.getSelectedItem());
  			this.add(activeUnitBox);  
  		}
  	}
  	else if(e.getSource() == this.categoryComboBox)
  	{
  		this.removeAll();
  		if(category2subQComboBox.containsKey(categoryComboBox.getSelectedItem()))
  		{
	  		this.add(subCheck);
	  		this.subCheck.setSelected(false);
  		}
			this.add(categoryComboBox);
  		activeCmptBox = category2cmptNameComboBox.get(categoryComboBox.getSelectedItem());
    	this.add(activeCmptBox);
    	activePropBox = category2propsComboBox.get(categoryComboBox.getSelectedItem());
    	this.add(activePropBox);
    	activeUnitBox  = property2unitComboBox.get(activePropBox.getSelectedItem());
  		this.add(activeUnitBox);  
  	}
  	else if(e.getSource() == this.activePropBox)
		{
			this.remove(activeUnitBox);	  	
			activeUnitBox  = property2unitComboBox.get(activePropBox.getSelectedItem());
	  	this.add(activeUnitBox);
		}			
		this.revalidate();
		this.repaint();
	}
  
  public DataRequestRow createRow(SEDataRequest dr)
	{
		String label = "Unknown";
		String cmpt;
		String substance = "";
		String property = StringUtils.spaceCamelCase(dr.getName());
				
		if(dr instanceof SEGasCompartmentDataRequest)
		{
			SEGasCompartmentDataRequest gdr = (SEGasCompartmentDataRequest)dr;
			cmpt = StringUtils.spaceCamelCase(gdr.getCompartment());
			if(gdr.hasSubstance())
				substance = gdr.getSubstance().getName() + " ";
			
			if(dr.hasUnit())
				label = cmpt +" "+substance+property+" ("+dr.getUnit()+")";
			else
				label = cmpt +" "+substance+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		else if(dr instanceof SELiquidCompartmentDataRequest)
		{
			SELiquidCompartmentDataRequest ldr = (SELiquidCompartmentDataRequest)dr;
			cmpt = StringUtils.spaceCamelCase(ldr.getCompartment());
			if(ldr.hasSubstance())
				substance = ldr.getSubstance().getName() + " ";
			
			if(dr.hasUnit())
				label = cmpt +" "+substance+property+" ("+dr.getUnit()+")";
			else
				label = cmpt +" "+substance+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		else if(dr instanceof SETissueCompartmentDataRequest)
		{
			SETissueCompartmentDataRequest tdr = (SETissueCompartmentDataRequest)dr;
			cmpt = StringUtils.spaceCamelCase(tdr.getCompartment());
					
			if(dr.hasUnit())
				label = cmpt +" "+property+" ("+dr.getUnit()+")";
			else
				label = cmpt +" "+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		else if(dr instanceof SEThermalCompartmentDataRequest)
		{
			SEThermalCompartmentDataRequest tdr = (SEThermalCompartmentDataRequest)dr;
			cmpt = StringUtils.spaceCamelCase(tdr.getCompartment());
					
			if(dr.hasUnit())
				label = cmpt +" "+property+" ("+dr.getUnit()+")";
			else
				label = cmpt +" "+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		
		DataRequestRow row = new DataRequestRow(this.pntListener, label, dr);
		return row;
	}
  
	public DataRequestRow createRow()
	{
		String label = "";
		String property = StringUtils.removeSpaces((String)this.activePropBox.getSelectedItem());
		String unit = (String)this.activeUnitBox.getSelectedItem();
		String cmpt = StringUtils.removeSpaces((String)this.activeCmptBox.getSelectedItem());
		if(this.activeCmptBox == chymeCmptsComboBox)
			cmpt = BioGearsCompartments.Chyme.longValueOf(cmpt);
		else if(this.activeCmptBox == pulmonaryCmptsComboBox)
			cmpt = BioGearsCompartments.Pulmonary.longValueOf(cmpt);
		else if(this.activeCmptBox == tissueCmptsComboBox)
			cmpt = BioGearsCompartments.Tissue.longValueOf(cmpt);
		else if(this.activeCmptBox == extravascularCmptsComboBox)
			cmpt = BioGearsCompartments.Extravascular.longValueOf(cmpt);
		else if(this.activeCmptBox == lymphCmptsComboBox)
			cmpt = BioGearsCompartments.Lymph.longValueOf(cmpt);
		else if(this.activeCmptBox == urineCmptsComboBox)
			cmpt = BioGearsCompartments.Urine.longValueOf(cmpt);
		else if(this.activeCmptBox == vascularCmptsComboBox)
			cmpt = BioGearsCompartments.Vascular.longValueOf(cmpt);
		else if(this.activeCmptBox == environmentCmptsComboBox)
			cmpt = BioGearsCompartments.Environment.longValueOf(cmpt);
		else if(this.activeCmptBox == aerosolCmptsComboBox)
			cmpt = BioGearsCompartments.Pulmonary.longValueOf(cmpt);
		else if(this.activeCmptBox == anesthesiaMachineCmptsComboBox)
			cmpt = BioGearsCompartments.AnesthesiaMachine.longValueOf(cmpt);
		else if(this.activeCmptBox == inhalerCmptsComboBox)
			cmpt = BioGearsCompartments.Inhaler.longValueOf(cmpt);
		
		SEDataRequest dr = null;
		if(activePropBox == gasPropsComboBox || activePropBox == gasSubQPropsComboBox)
		{
			SEGasCompartmentDataRequest gdr = new SEGasCompartmentDataRequest();
			gdr.setCompartment(cmpt);
			String substance = "";
			if(this.subCheck.isSelected())
			{
				substance = (String)this.subsBox.getSelectedItem()+" ";
				gdr.setSubstance(subMgr.getSubstance(substance.trim()));
			}
			gdr.setName(StringUtils.removeSpaces(property));
			gdr.setUnit(unit);
			dr = gdr;
			label = StringUtils.spaceCamelCase(cmpt) +" "+substance+StringUtils.spaceCamelCase(property)+" ("+unit+")";
		}
		else if(activePropBox == liquidPropsComboBox || activePropBox == liquidSubQPropsComboBox)
		{
			SELiquidCompartmentDataRequest ldr = new SELiquidCompartmentDataRequest();
			ldr.setCompartment(cmpt);
			String substance = "";
			if(this.subCheck.isSelected())
			{
				substance = (String)this.subsBox.getSelectedItem()+" ";
				ldr.setSubstance(subMgr.getSubstance(substance.trim()));
			}
			ldr.setName(StringUtils.removeSpaces(property));
			ldr.setUnit(unit);			
			dr = ldr;
			label = StringUtils.spaceCamelCase(cmpt) +" "+substance+StringUtils.spaceCamelCase(property)+" ("+unit+")";
		}
		else if(activePropBox == tissuePropsComboBox)
		{
			SETissueCompartmentDataRequest tdr = new SETissueCompartmentDataRequest();
			tdr.setCompartment(cmpt);
			tdr.setName(StringUtils.removeSpaces(property));
			tdr.setUnit(unit);			
			dr = tdr;
			label = StringUtils.spaceCamelCase(cmpt) +" "+StringUtils.spaceCamelCase(property)+" ("+unit+")";
		}
		else if(activePropBox == thermalPropsComboBox)
		{
			SEThermalCompartmentDataRequest tdr = new SEThermalCompartmentDataRequest();
			tdr.setCompartment(cmpt);
			tdr.setName(StringUtils.removeSpaces(property));
			tdr.setUnit(unit);			
			dr = tdr;
			label = StringUtils.spaceCamelCase(cmpt) +" "+StringUtils.spaceCamelCase(property)+" ("+unit+")";
		}
		
		DataRequestRow row = new DataRequestRow(this.pntListener, label, dr);
		return row;
	}
}
