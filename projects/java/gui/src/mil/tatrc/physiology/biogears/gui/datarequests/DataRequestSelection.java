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

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.*;

import javax.swing.JComboBox;
import javax.swing.JPanel;

import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl.ScalarProperty;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.StringUtils;

public abstract class DataRequestSelection extends JPanel implements ActionListener
{
	protected JComboBox<String>              categoryComboBox= new JComboBox<String>();
	
	protected JComboBox<String>              activePropBox = null;
	protected JComboBox<String>              activeUnitBox = null;

	protected Map<String, String>            property2category         = new HashMap<String,String>();
	protected Map<String, JComboBox<String>> category2propertyComboBox = new LinkedHashMap<String, JComboBox<String>>();
	protected Map<String, JComboBox<String>> property2unitComboBox     = new LinkedHashMap<String, JComboBox<String>>();
	
	protected ActionListener                 pntListener;

  public DataRequestSelection(ActionListener pnt, Map<String,List<ScalarProperty>> categories2properties)
  {
  	super(new FlowLayout(FlowLayout.LEADING, 5, 5));  	
  	this.pntListener = pnt;
    
  	if(categories2properties != null)
  	{
  		for(String category : categories2properties.keySet())
  		{
  			categoryComboBox.addItem(category);  		
  			JComboBox<String> propertiesComboBox = new JComboBox<String>();

  			category2propertyComboBox.put(category, propertiesComboBox);

  			for(ScalarProperty p : categories2properties.get(category))
  			{
  				propertiesComboBox.addItem(p.name);  
  				if(property2category.containsKey(p.name))
  					Log.info("I already mapped "+p.name);
  				property2category.put(p.name,category);
  				JComboBox<String> unitsComboBox = new JComboBox<String>();
  				property2unitComboBox.put(p.name, unitsComboBox);
  				if(p.units != null)
  					for(String unit : p.units)
  						unitsComboBox.addItem(unit);
  				else
  					unitsComboBox.addItem("Unitless");
  			}  		
  			propertiesComboBox.addActionListener(this);
  		}  	 
  		categoryComboBox.addActionListener(this);

  		this.add(categoryComboBox);
  		activePropBox = category2propertyComboBox.get(categoryComboBox.getSelectedItem());
  		this.add(activePropBox);
  		activeUnitBox = property2unitComboBox.get(activePropBox.getSelectedItem());
  		this.add(activeUnitBox);
  	}
  }
  
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource() == this.categoryComboBox)
		{
			this.remove(activePropBox);
	  	this.remove(activeUnitBox);	  	
	  	this.activePropBox = this.category2propertyComboBox.get(this.categoryComboBox.getSelectedItem());
	  	this.activeUnitBox = this.property2unitComboBox.get(this.activePropBox.getSelectedItem());
	  	this.add(activePropBox);
	  	this.add(activeUnitBox);
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
		if(!property2category.containsKey(property) || !property2unitComboBox.containsKey(property))
			label = "Unknown property "+property;
		else
		{
			if(dr.hasUnit())
				label = property2category.get(property)+" "+property+" ("+dr.getUnit()+")";
			else
				label = property2category.get(property)+" "+property+" ("+this.property2unitComboBox.get(property).getItemAt(0)+")";
		}
		DataRequestRow row = new DataRequestRow(this.pntListener, label, dr);
		return row;
	}
	
	public abstract DataRequestRow createRow();
}
