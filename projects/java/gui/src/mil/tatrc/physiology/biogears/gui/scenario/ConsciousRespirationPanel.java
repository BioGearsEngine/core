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
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import javax.swing.plaf.basic.BasicArrowButton;

import mil.tatrc.physiology.biogears.gui.GUIContext;
import mil.tatrc.physiology.biogears.gui.scenario.DynamicPropertyPanel.Row;
import mil.tatrc.physiology.datamodel.patient.actions.SEBreathHold;
import mil.tatrc.physiology.datamodel.patient.actions.SEConsciousRespiration;
import mil.tatrc.physiology.datamodel.patient.actions.SEConsciousRespirationCommand;
import mil.tatrc.physiology.datamodel.patient.actions.SEForcedExhale;
import mil.tatrc.physiology.datamodel.patient.actions.SEForcedInhale;
import mil.tatrc.physiology.datamodel.patient.actions.SEUseInhaler;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.FindObjects.BagMethod;

public class ConsciousRespirationPanel extends JPanel implements ActionListener
{
	private final static int HOR_GAP	= 5;
	private final static int VER_GAP	= 2;

	private SEConsciousRespiration consResp;
	private JPanel  commands      = new JPanel();
	private JButton forcedInhale	= new JButton("Inhale");
	private JButton forcedExhale	= new JButton("Exhale");
	private JButton breathHold		= new JButton("HoldBreath");
	private JButton useInhaler	  = new JButton("Use Inhaler");
		
	private JButton okButton		  = new JButton("OK");
	private JButton cancelButton	= new JButton("Cancel");
	protected boolean cancelled=false;
	
	private Map<String,List<String>> propertyOptions;
	
	private List<CommandPanel> cmds = new ArrayList<CommandPanel>();
		
	public ConsciousRespirationPanel(SEConsciousRespiration cr, Map<String,List<String>> propertyOptions)
	{		
		this.consResp = cr;
		this.propertyOptions = propertyOptions;
		this.setLayout(new BorderLayout(HOR_GAP, VER_GAP));
		
		commands.setLayout(new BoxLayout(commands, BoxLayout.Y_AXIS));			
		JScrollPane scrollPane = new JScrollPane(commands);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		scrollPane.getViewport().setPreferredSize(new Dimension(200,400));
					
		JPanel controlsPanel = new JPanel(new GridBagLayout());
		forcedInhale.addActionListener(this);
		forcedExhale.addActionListener(this);
		breathHold.addActionListener(this);
		useInhaler.addActionListener(this);
		okButton.addActionListener(this);
		cancelButton.addActionListener(this);
		Dimension d = new Dimension(90, 20);		
		forcedInhale.setPreferredSize(d);
		forcedExhale.setPreferredSize(d);
		breathHold.setPreferredSize(d);
		useInhaler.setPreferredSize(d);
		okButton.setPreferredSize(d);
		cancelButton.setPreferredSize(d);
		
		forcedInhale.setMinimumSize(d);
		forcedExhale.setMinimumSize(d);
		breathHold.setMinimumSize(d);
		useInhaler.setMinimumSize(d);
		okButton.setMinimumSize(d);
		cancelButton.setMinimumSize(d);
		
		forcedInhale.setMaximumSize(d);
		forcedExhale.setMaximumSize(d);
		breathHold.setMaximumSize(d);
		useInhaler.setMaximumSize(d);
		okButton.setMaximumSize(d);
		cancelButton.setMaximumSize(d);

		controlsPanel.add(forcedInhale, GUIContext.createGBC(0,0, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));
		controlsPanel.add(forcedExhale, GUIContext.createGBC(1,0, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));
		controlsPanel.add(breathHold, GUIContext.createGBC(2,0, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));
		controlsPanel.add(useInhaler, GUIContext.createGBC(3,0, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));
		controlsPanel.add(okButton, GUIContext.createGBC(1,1, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));
		controlsPanel.add(cancelButton, GUIContext.createGBC(2,1, 1,1, 1,1, GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(5,3,5,3)));		
		
		JPanel controlsPnt = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
		controlsPnt.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));		
		controlsPnt.add(controlsPanel,BorderLayout.CENTER);
		
		this.add(scrollPane, BorderLayout.CENTER);
		this.add(controlsPnt, BorderLayout.SOUTH);
		
		for(SEConsciousRespirationCommand cmd : cr.getCommands())
			AddCommand(cmd);
		
		// TODO Comment
	}
	
	protected void AddCommand(SEConsciousRespirationCommand cmd)
	{
		CommandPanel cp = new CommandPanel(this,cmd,propertyOptions);
		cmds.add(cp);		
		commands.add(cp);
		commands.revalidate();
		commands.repaint();
	}
	
	protected static class CommandPanel extends JPanel
	{			
		JButton deleteCmd	    = new JButton("Delete");
		JButton upCmd	        = new BasicArrowButton(BasicArrowButton.NORTH);
		JButton downCmd	      = new BasicArrowButton(BasicArrowButton.SOUTH);
		Map<String,Row> rowMap;
	
		public CommandPanel(ActionListener pnt, SEConsciousRespirationCommand cmd, Map<String, List<String>> propertyOptions)
		{
			super();
			setLayout(new BorderLayout(HOR_GAP, VER_GAP));
			Border b = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
			TitledBorder title = BorderFactory.createTitledBorder(b, cmd.getClass().getSimpleName().substring(2));
      title.setTitleJustification(TitledBorder.LEFT);
			setBorder(title);
			
			downCmd.addActionListener(pnt);
			deleteCmd.addActionListener(pnt);
			upCmd.addActionListener(pnt);
			
			JPanel  controlsPanel  = new JPanel(new FlowLayout(FlowLayout.CENTER));			
			controlsPanel.add(downCmd);
			controlsPanel.add(deleteCmd);
			controlsPanel.add(upCmd);
		
			JPanel cmdFields = new JPanel(new GridBagLayout());

			rowMap = new HashMap<String,Row>();
			List<String> order = new ArrayList<String>();
			List<BagMethod> bags = new ArrayList<BagMethod>();
			try{
				DynamicPropertyPanel.getPropertyBags(cmd, bags, null, null);
			}catch(Exception ex){ Log.error("Having trouble with this class "+cmd.getClass().getSimpleName(),ex);}
			for(BagMethod bag : bags)
			{
				if(bag.returnType == List.class)
					continue;// Have to handle these with custom code above

				Row row = new Row(bag, propertyOptions);

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
				cmdFields.add(row.label, GUIContext.createGBC(0,y, 0.01,1, 1,1, GridBagConstraints.EAST, GridBagConstraints.BOTH, new Insets(2,5,0,5)));
				cmdFields.add(row.value, GUIContext.createGBC(1,y, 1,1, 1,1, GridBagConstraints.WEST, GridBagConstraints.HORIZONTAL, new Insets(2,0,0,5)));
				y++;
			}					
			add(cmdFields, BorderLayout.CENTER);
			add(controlsPanel,BorderLayout.SOUTH);
		}
		
		public String pullData()
		{
			for(Row row : rowMap.values())
			{
				String err=row.pullData();
				if(err!=null && !err.isEmpty())
					return err;
			}			
			return null;
		}
	}
	
	public void actionPerformed(ActionEvent e)
	{
		Object src = e.getSource();

		if(src == forcedInhale)
		{
			SEConsciousRespirationCommand cmd = new SEForcedInhale();
			this.consResp.getCommands().add(cmd);
			AddCommand(cmd);
			return;
		}
		else if(src == forcedExhale)
		{
			SEConsciousRespirationCommand cmd = new SEForcedExhale();
			this.consResp.getCommands().add(cmd);
			AddCommand(cmd);
			return;
		}
		else if(src == breathHold)
		{
			SEConsciousRespirationCommand cmd = new SEBreathHold();
			this.consResp.getCommands().add(cmd);
			AddCommand(cmd);
			return;
		}
		else if(src == useInhaler)
		{
			SEConsciousRespirationCommand cmd = new SEUseInhaler();
			this.consResp.getCommands().add(cmd);
			AddCommand(cmd);
			return;
		}		
		else if(src == this.okButton)
		{	
			// Pull data from the dialog and update the action
			for(CommandPanel cmd : cmds)
			{
				String err = cmd.pullData();
				if(err!=null&&!err.isEmpty())
				{
					JOptionPane.showMessageDialog(this, err);
					return;
				}
			}
			if(!this.consResp.isValid())
			{
				JOptionPane.showMessageDialog(this, "Action is not vaild");
				return;
			}
			((Window) getRootPane().getParent()).dispose();
		}
		else if (src == this.cancelButton)
		{			
			this.cancelled = true;
			((Window) getRootPane().getParent()).dispose();
		}	
		int i=-1;
		boolean clicked=false;
		for(CommandPanel cmd : this.cmds)
		{
			i++;
			if(src == cmd.upCmd)
			{
				if(i>0)
				{					
					CommandPanel c1 = cmds.get(i);
					CommandPanel c0 = cmds.get(i-1);					
					commands.add(c0, i);
					commands.add(c1, i-1);					
					cmds.set(i,   c0);
					cmds.set(i-1, c1);
					// also switch the commands in the action
					SEConsciousRespirationCommand cmd1 = consResp.getCommands().get(i);
					SEConsciousRespirationCommand cmd0 = consResp.getCommands().get(i-1);
					consResp.getCommands().set(i,   cmd0);
					consResp.getCommands().set(i-1, cmd1);
				}
				clicked=true;
			}
			else if(src == cmd.deleteCmd)
			{
				commands.remove(cmds.remove(i));
				consResp.getCommands().remove(i);
				clicked=true;
			}
			else if(src == cmd.downCmd)
			{
				if(i<cmds.size()-1)
				{
					CommandPanel cNextToLast = cmds.get(i);
					CommandPanel cEnd = cmds.get(i+1);					
					commands.add(cEnd, i);
					commands.add(cNextToLast, i+1);					
					cmds.set(i,   cEnd);
					cmds.set(i+1, cNextToLast);
					// also switch the commands in the action
					SEConsciousRespirationCommand cmdNextToLast = consResp.getCommands().get(i);
					SEConsciousRespirationCommand cmdEnd = consResp.getCommands().get(i+1);
					consResp.getCommands().set(i,   cmdEnd);
					consResp.getCommands().set(i+1, cmdNextToLast);
				}
				clicked=true;
			}
			if(clicked)
			{								
				commands.revalidate();
				commands.repaint();
				return;
			}
		}
	}

	
	
}
