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

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;
import javax.swing.border.EtchedBorder;

import mil.tatrc.physiology.biogears.gui.GUIContext;
import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.scenario.actions.SEAction;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceCompound;
import mil.tatrc.physiology.datamodel.substance.SESubstanceFraction;
import mil.tatrc.physiology.datamodel.system.environment.SEEnvironmentalConditions;
import mil.tatrc.physiology.datamodel.system.environment.actions.SEEnvironmentChange;
import mil.tatrc.physiology.datamodel.system.environment.conditions.InitialEnvironment;
import mil.tatrc.physiology.utilities.FindObjects;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.FindObjects.BagMethod;

public class DynamicPropertyPanel extends JPanel implements ActionListener
{
	private final int HOR_GAP	= 5;
	private final int VER_GAP	= 5;

	private Object obj;
	private JButton okButton		= new JButton("OK");
	private JButton cancelButton	= new JButton("Cancel");
	protected Boolean cancelled=false;
	
	List<Row> rows = new ArrayList<Row>();
	
	protected static class Row
	{
		public BagMethod bag;
		
		public   JLabel label;
		public   JPanel value;
		
		protected JFormattedTextField scalarValue;
		protected JComboBox<String>   scalarUnits;
		protected JComboBox<String>   enumComboBox;
		protected JComboBox<String>   optionsComboBox;
		protected JTextArea           comment;
		protected JTextField          stringValue;
		 
		Row(BagMethod b, Map<String,List<String>> propertyOptions)
		{			
			this.bag = b;
    
			String name;
			if(bag.myName != null && !bag.myName.equals("Configuration") && !bag.myName.equals("Conditions"))
				name = bag.myName+bag.propertyName;
			else
				name = bag.propertyName;
			label = new JLabel(name);
			value = new JPanel(new FlowLayout(FlowLayout.LEFT));
			
			if(propertyOptions !=null && propertyOptions.containsKey(bag.propertyName))
			{
				optionsComboBox = new JComboBox<String>();
				for(String s : propertyOptions.get(bag.propertyName))
					optionsComboBox.addItem(s);
				value.add(optionsComboBox);
				try
				{
					if((Boolean)bag.has.invoke(bag.me))
					{
						if(String.class.isAssignableFrom(bag.returnType))
						{
							String s = (String)bag.get.invoke(bag.me);
							optionsComboBox.setSelectedItem(s);
						}
						else
						{
							// Should be a child object with a getName method... if not,  you're fuct
							Object o = bag.get.invoke(bag.me);
							Method h = o.getClass().getMethod("hasName");
							if((Boolean)h.invoke(o))
							{
								Method g = o.getClass().getMethod("getName");
								String s = (String)g.invoke(o);
								optionsComboBox.setSelectedItem(s);
							}
							else
								optionsComboBox.setSelectedIndex(-1);
						}
					}			
					else
						optionsComboBox.setSelectedIndex(-1);
				}
				catch(Exception ex)
				{
					Log.error("Unable to set enum field : "+bag.propertyName,ex);
				}
			}
			else if(SEScalar.class.isAssignableFrom(bag.returnType))
			{				
				// Create Controls
				NumberFormat ni = NumberFormat.getNumberInstance();
				ni.setMaximumFractionDigits(9);
				scalarValue = new JFormattedTextField();
				scalarValue.setColumns(10);
				scalarValue.setPreferredSize(new Dimension(80,22));
				value.add(scalarValue);
								
				scalarUnits = new JComboBox<String>();
				value.add(scalarUnits);
				List<String> units = ScalarCtrl.getUnitsList((Class<? extends SEScalar>)bag.returnType);
				if(units==null || units.isEmpty())					
					scalarUnits.addItem("Unitless");
				else
				{
					for(String unit : units)
					{
						scalarUnits.addItem(unit);
					}
				}	
				
				// Fill out or default values
				try
				{
					if((Boolean)bag.has.invoke(bag.me))
					{
						SEScalar s = (SEScalar)bag.get.invoke(bag.me);
						scalarValue.setText(Double.toString(s.getValue()));
						scalarUnits.setSelectedItem(s.getUnit());
					}
				}
				catch(Exception ex)
				{
					Log.error("Unable to set scalar field : "+bag.propertyName,ex);
				}
			}	
			else if(bag.returnType.isEnum())
			{
				// Create controls
				enumComboBox = new JComboBox<String>();
				for (Object e: bag.returnType.getEnumConstants()) 
				{
					enumComboBox.addItem(e.toString());
					value.add(enumComboBox);
				}
			  // Fill out or default values
				try
				{
					if((Boolean)bag.has.invoke(bag.me))
					{
						Enum<?> e = (Enum<?>)bag.get.invoke(bag.me);
						enumComboBox.setSelectedItem(e.name());
					}			
					else
						enumComboBox.setSelectedIndex(-1);
				}
				catch(Exception ex)
				{
					Log.error("Unable to set enum field : "+bag.propertyName,ex);
				}
			}						
			else if(String.class.isAssignableFrom(bag.returnType))
			{
				
				if(bag.propertyName.equals("Comment"))
				{
					comment = new JTextArea();
					comment.setLineWrap(true);
					comment.setWrapStyleWord(true);
					JScrollPane scroll = new JScrollPane(comment);
					scroll.setPreferredSize(new Dimension(175,30));
					scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);					
					value.add(scroll);
					try
					{
						if((Boolean)bag.has.invoke(bag.me))
							comment.setText((String)bag.get.invoke(bag.me));
					}
					catch(Exception ex)
					{
						Log.error("Unable to set enum field : "+bag.propertyName,ex);
					}
				}
				else
				{
					stringValue = new JTextField();
					stringValue.setPreferredSize(new Dimension(225,22));
					value.add(stringValue);
					try
					{
						if((Boolean)bag.has.invoke(bag.me))
							comment.setText((String)bag.get.invoke(bag.me));
					}
					catch(Exception ex)
					{
						Log.error("Unable to set enum field : "+bag.propertyName,ex);
					}
				}				
			}	
		}
		
		public String pullData()
		{
			if(optionsComboBox != null)
			{
				try 
				{					
					if(String.class.isAssignableFrom(bag.returnType))
					{
						Method m = bag.me.getClass().getMethod("set"+bag.propertyName, bag.returnType);
						m.invoke(bag.me, optionsComboBox.getSelectedItem());
					}
					else
					{
					  // Should be a child object with a setName method... if not,  you're fuct
						Object o = bag.get.invoke(bag.me);
						if(o==null)
						{
							o=bag.returnType.newInstance();
							Method m = bag.me.getClass().getMethod("set"+bag.propertyName, bag.returnType);
							m.invoke(bag.me, o);
						}
						Method s = o.getClass().getMethod("setName",String.class);
						s.invoke(o, optionsComboBox.getSelectedItem());
					}
				}catch(Exception ex)
				{
					Log.error("Cannot find options set method for : "+bag.propertyName,ex);
				}
			}
			else if(scalarValue != null)
			{
				try
				{
					if(scalarValue.getText()==null || scalarValue.getText().isEmpty())
					{
						SEScalar s = (SEScalar)bag.get.invoke(bag.me);
						s.invalidate();
					}
					else
					{
						double v = 0;
						try{
							v=Double.parseDouble(scalarValue.getText());
						}catch(Exception ex){Log.error("Scalar value must be numerical for property : "+bag.propertyName);}
						String unit = (String)scalarUnits.getSelectedItem();


						SEScalar s = (SEScalar)bag.get.invoke(bag.me);
						if(s instanceof SEScalarFraction)
						{
							if(v<0 || v>1)				
								return bag.propertyName+" is a fraction. Must be between [0,1]";
						}
						if(unit.equals("Unitless"))
							s.setValue(v);
						else
							s.setValue(v,unit);					
					}
				}
				catch(Exception ex)
				{
					Log.error("get method is not setup propertly for property : "+bag.propertyName);
				}
			}
			else if(enumComboBox != null)
			{
				if(enumComboBox.getSelectedIndex()==-1)
				{
					
				}
				else
				{
					try{
						String selection = (String)enumComboBox.getSelectedItem();
						Method set = bag.me.getClass().getMethod("set"+bag.propertyName, bag.returnType);
						for (Object o: bag.returnType.getEnumConstants()) 
						{
							if(o.toString().equals(selection))
							{
								set.invoke(bag.me,o);
								break;
							}
						}				
					}catch(Exception ex){Log.error("Unable to update enum property : "+bag.propertyName,ex);}
				}
			}
			else if(stringValue != null)
			{
				try{
					Method set = bag.me.getClass().getMethod("set"+bag.propertyName, bag.returnType);
					set.invoke(bag.me, stringValue.getText());
				}catch(Exception ex){Log.error("Unable to update string property : "+bag.propertyName,ex);}
			}
			else if(comment != null)
			{
				if(bag.me instanceof SEAction)
					((SEAction)bag.me).setComment(comment.getText());
			}
			else
				Log.error("This Action Row is unmapped to the action");
			
			return null;
		}
		
	}
	
	public DynamicPropertyPanel(Object obj, Map<String,List<String>> propertyOptions)
	{		
		this.obj = obj;
		this.setLayout(new BorderLayout(HOR_GAP, VER_GAP));

		JPanel actionFields = new JPanel();
		actionFields.setLayout(new GridBagLayout());
		
		JScrollPane scrollPane = new JScrollPane(actionFields);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		
				
		List<String> skipProperties = new ArrayList<String>();
		skipProperties.add("ScenarioTime");
		skipProperties.add("ConditionsFile");
		skipProperties.add("ConfigurationFile");
		skipProperties.add("MCIS");
		
		List<String> order = new ArrayList<String>();
		Map<String,Row> rowMap = new HashMap<String,Row>();
		List<BagMethod> bags = new ArrayList<BagMethod>();
		try{
			getPropertyBags(obj, bags, skipProperties, null);
		}catch(Exception ex){ Log.error("Having trouble with this class "+obj.getClass().getSimpleName(),ex);}
		
		if(obj.getClass() == SEEnvironmentChange.class || obj.getClass() == InitialEnvironment.class)
		{
			SEEnvironmentalConditions ec=null;
			if(obj.getClass() == SEEnvironmentChange.class)
				ec = ((SEEnvironmentChange)obj).getConditions();
			else if(obj.getClass() == InitialEnvironment.class)
				ec = ((InitialEnvironment)obj).getConditions();
			
					
			try
			{	
				SESubstance O2 = new SESubstance();	
				O2.setName("Oxygen");			
				SESubstanceFraction o2 = ec.getAmbientGas(O2);
				BagMethod o2bag = new BagMethod();
				o2bag.me = o2;
				o2bag.propertyName = "Fraction of Oxygen";
				o2bag.has = o2.getClass().getMethod("hasFractionAmount");
				o2bag.get = o2.getClass().getMethod("getFractionAmount");
				o2bag.returnType = SEScalarFraction.class;
				bags.add(o2bag);
				
				SESubstance CO2 = new SESubstance();	
				CO2.setName("CarbonDioxide");			
				SESubstanceFraction co2 = ec.getAmbientGas(CO2);
				BagMethod co2bag = new BagMethod();
				co2bag.me = co2;
				co2bag.propertyName = "Fraction of CarbonDioxide";
				co2bag.has = co2.getClass().getMethod("hasFractionAmount");
				co2bag.get = co2.getClass().getMethod("getFractionAmount");
				co2bag.returnType = SEScalarFraction.class;
				bags.add(co2bag);
				
				SESubstance N2 = new SESubstance();	
				N2.setName("Nitrogen");			
				SESubstanceFraction n2 = ec.getAmbientGas(N2);
				BagMethod n2bag = new BagMethod();
				n2bag.me = n2;
				n2bag.propertyName = "Fraction of Nitrogen";
				n2bag.has = n2.getClass().getMethod("hasFractionAmount");
				n2bag.get = n2.getClass().getMethod("getFractionAmount");
				n2bag.returnType = SEScalarFraction.class;
				bags.add(n2bag);
			}catch(Exception ex){Log.error("We should be cool here",ex);}
		}
		
		for(BagMethod bag : bags)
		{
			if(bag.returnType == List.class)
				continue;// Have to handle these with custom code above

			Row row = new Row(bag, propertyOptions);
			rows.add(row);

			if(row.label.getText().equals("Comment"))
			{
				rowMap.put(row.label.getText(),row);
			}
			else
			{
				order.add(row.label.getText());
				rowMap.put(row.label.getText(),row);
			}
		}		
		Collections.sort(order);	
		order.add("Comment");// Put this last
		int y=0;
		for(String name : order)
		{
			Row row = rowMap.get(name);
			if(row == null)
				continue;
			actionFields.add(row.label, GUIContext.createGBC(0,y, 0.01,1, 1,1, GridBagConstraints.EAST, GridBagConstraints.BOTH, new Insets(5,5,0,5)));
			actionFields.add(row.value, GUIContext.createGBC(1,y, 1,1, 1,1, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(5,0,0,5)));
			y++;
		}
		
		JPanel controlsPanel = new JPanel(new FlowLayout());
		okButton.addActionListener(this);
		cancelButton.addActionListener(this);
		controlsPanel.add(okButton);
		controlsPanel.add(cancelButton);		
		
		JPanel controlsPnt = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
		controlsPnt.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));		
		controlsPnt.add(controlsPanel,BorderLayout.CENTER);
		
		this.add(scrollPane, BorderLayout.CENTER);
		this.add(controlsPnt, BorderLayout.SOUTH);
	}
	
	public static void getPropertyBags(Object obj, List<BagMethod> bags, List<String> skipProperties, String name) throws IllegalAccessException, IllegalArgumentException, InvocationTargetException
	{
		for(BagMethod bag : FindObjects.getBagMethods(obj.getClass(), skipProperties))
	  {
			bag.myName = name;
			bag.me     = obj;
			if(SEScalar.class.isAssignableFrom(bag.returnType))
				bags.add(bag);
			else if(bag.returnType.isEnum())
				bags.add(bag);
			else if(String.class.isAssignableFrom(bag.returnType))
        bags.add(bag);
			else if(SESubstance.class.isAssignableFrom(bag.returnType))
				bags.add(bag);
			else if(SESubstanceCompound.class.isAssignableFrom(bag.returnType))
				bags.add(bag);
      else if(bag.returnType.getSimpleName().startsWith("SE"))
      {
      	getPropertyBags(bag.get.invoke(obj),bags,skipProperties,bag.propertyName);
      }
      else if(bag.returnType == List.class && bag.propertyName.equals("AmbientSubstance") && bag.me.getClass() == SEEnvironmentalConditions.class)
      	bags.add(bag);// I am going to have to handle this with specialness 
      else
				Log.warn("Unknown property class : "+bag.returnType.getSimpleName());
		}
	}

	public void actionPerformed(ActionEvent e)
	{
		Object src = e.getSource();

		if (src == this.okButton)
		{	
			// Pull data from the dialog and update the action
			for(Row row : rows)
			{
				String err = row.pullData();
				if(err!=null&&!err.isEmpty())
				{
					JOptionPane.showMessageDialog(this, err);
					return;
				}
			}
			if(SEAction.class.isAssignableFrom(obj.getClass()))
			{
				if(!((SEAction)obj).isValid())
				{
					JOptionPane.showMessageDialog(this, "Action is not vaild");
					return;
				}
			}
			((Window) getRootPane().getParent()).dispose();
		}
		else if (src == this.cancelButton)
		{
			this.cancelled = true;
			((Window) getRootPane().getParent()).dispose();
		}		
	}

	
	
}
