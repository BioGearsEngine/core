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

import info.monitorenter.gui.chart.*;
import info.monitorenter.gui.chart.IAxis.*;
import info.monitorenter.gui.chart.traces.*;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;

import mil.tatrc.physiology.biogears.engine.BioGearsScenarioExec;
import mil.tatrc.physiology.biogears.gui.controls.ConsoleListener;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.datamodel.scenario.datarequests.*;
import mil.tatrc.physiology.utilities.Log;


public class ScenarioCalcDisplay extends JPanel implements ActionListener
{
	protected JButton 			   	    cancelCloseButton;
	protected JScrollPane           outputSelection;
	
	protected ScenarioCalcThread   	calcThread;
	protected ScenarioCalcCallback  calcCallback;
	protected ConsoleListener	calcListener;

	// Controls needed for inline action listener classes
	protected GridBagConstraints gbc = new GridBagConstraints();
	protected Dimension chartSize = new Dimension(920, 550);
	
	protected Map<String, SEDataRequest> labels2DataRequest = new HashMap<String,SEDataRequest>();
	protected Map<Integer, ChartDataRequestScalar> chartMap = new HashMap<Integer, ChartDataRequestScalar>();
	
	protected class ChartDataRequestScalar
	{
		public ZoomableChart chart;
		public ITrace2D      graphLine;
		public SEScalar 	   scalar;
		public SEDataRequest dataRequest;
		
		public ChartDataRequestScalar(ZoomableChart chart, ITrace2D graphLine, SEDataRequest dr)
		{
			this.chart 	     = chart;
			this.graphLine   = graphLine;
			this.dataRequest = dr;
		}
		
		protected double ymax=-Double.MAX_VALUE;
		protected double ymin=Double.MAX_VALUE;
		public void zoom()
		{
			if(graphLine.getMaxY()>ymax)
				ymax=graphLine.getMaxY();
			if(graphLine.getMinY()<ymin)
				ymin=graphLine.getMinY();
			double ymaxbuff;
			double yminbuff;			
			if(SEScalarFraction.class.isAssignableFrom(scalar.getClass()))
			{
				ymaxbuff=1;
				yminbuff=0;
			}
			else
			{
				double buff = (ymax-ymin)*1.25;
				if(buff<5)
					buff=5;
				ymaxbuff = ymax+buff;
				yminbuff = ymin-buff;
				if(yminbuff<0)
					yminbuff=0;
			}
			chart.zoom(graphLine.getMinX(), graphLine.getMaxX(), yminbuff, ymaxbuff);
		}
	}
	
	public ScenarioCalcDisplay(SEScenario scenario, ConsoleListener listener)
	{
		super();
		
		this.calcListener = listener;

		JPanel gui = new JPanel(new BorderLayout());

		// List the Requested data on the left side of the Dialog
		final JPanel rGrid = new JPanel(new GridBagLayout());
		Dimension gSize = new Dimension(1230, 600);
		rGrid.setMinimumSize(gSize);
		rGrid.setPreferredSize(gSize);
		rGrid.setMaximumSize(gSize);
		
		JPanel rRequests = new JPanel(new GridBagLayout());
		Border requestBdr = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
		rGrid.setBorder(BorderFactory.createTitledBorder(requestBdr, "Requested Data", TitledBorder.LEFT, TitledBorder.DEFAULT_POSITION));
		
		String [] requests = getDataRequestLabels(scenario);
		final JList<String> scenarioRequests = new JList<String>(requests);
		scenarioRequests.addMouseListener(new MouseListener() {
			public void mouseClicked(MouseEvent e)	{}
			public void mousePressed(MouseEvent e) 	
			{
				Chart2D chart = chartMap.get(scenarioRequests.getSelectedIndex()).chart;
				chart.validate();
				chart.setRequestedRepaint(true);
				
				rGrid.remove(2);
				rGrid.add(chart, gbc);
				rGrid.validate();
				rGrid.repaint();
			}
			public void mouseReleased(MouseEvent e)	{}
			public void mouseEntered(MouseEvent e) 	{}
			public void mouseExited(MouseEvent e) 	{}
		});
	
		outputSelection = new JScrollPane(scenarioRequests);
		scenarioRequests.setSelectedIndex(0);
		Dimension size = new Dimension(250, 550);
		outputSelection.setMinimumSize(size);
		outputSelection.setPreferredSize(size);
		outputSelection.setMaximumSize(size);
		gbc.gridx=0;
		gbc.gridy=0;
		rRequests.add(outputSelection);
		rGrid.add(rRequests, gbc);
		
		JPanel spacing = new JPanel(new GridBagLayout());
        Dimension space = new Dimension(25, 200);
        spacing.setMinimumSize(space);
        spacing.setPreferredSize(space);
        spacing.setMaximumSize(space);
        gbc.gridx=1;
        gbc.gridy=0;
        rGrid.add(spacing, gbc);

		// Create a chart on the Right side of the Dialog 
		loadMap(scenario,requests);
		
		Chart2D firstChart = chartMap.get(0).chart;
		gbc.gridx=2;
		gbc.gridy=0;
		rGrid.add(firstChart, gbc);

		gui.add(rGrid, BorderLayout.NORTH);
		
		JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
		
		this.cancelCloseButton = new JButton("Cancel");
		Dimension cSize = new Dimension(100, 25);
		this.cancelCloseButton.setMinimumSize(cSize);
		this.cancelCloseButton.setPreferredSize(cSize);
		this.cancelCloseButton.setMaximumSize(cSize);
		this.cancelCloseButton.addActionListener(this);
		
		buttonPanel.add(this.cancelCloseButton);
		gui.add(buttonPanel, BorderLayout.SOUTH);

		add(gui);

		BioGearsScenarioExec executor = new BioGearsScenarioExec();
		this.calcCallback = new ScenarioCalcCallback(executor, chartMap.values(), 0.33);
		this.calcThread = new ScenarioCalcThread(executor, scenario, this.calcListener, this.calcCallback, this);
		this.calcThread.start();
	}
	
	public boolean cleanUp()
	{
		if(this.cancelCloseButton.getText().equals("Close"))
			return true;
		
		int choice = JOptionPane.showConfirmDialog(this, "Cancel calculation and close window?", "Close", JOptionPane.OK_CANCEL_OPTION);
		if (choice == JOptionPane.YES_OPTION)
		{
			cancel();
			return true;
		}
		return false;
	}
	
	protected boolean cancelling=false;// We only want to process a cancel once
	protected void cancel()
	{
		if(this.calcThread.isAlive()&&!this.cancelling)
		{
			this.cancelling=true;
			// First Let's disable the buttons on this page while we wait for a cancel
			this.cancelCloseButton.setEnabled(false);
			this.cancelCloseButton.setText("Close");			
			int choice = JOptionPane.showConfirmDialog(this, "Create graphs?", "Close", JOptionPane.OK_CANCEL_OPTION);
			this.calcThread.cancel(choice == JOptionPane.YES_OPTION);					
			this.cancelCloseButton.setEnabled(true);			
		}
	}
	
	private void loadMap(SEScenario scenario, String[] labels)
	{
		int i=0;
		for (String label : labels)
		{
			ZoomableChart chart = new ZoomableChart();
			chart.setEnabled(false);
			chart.setMinimumSize(chartSize);
			chart.setPreferredSize(chartSize);
			chart.setMaximumSize(chartSize);
			chart.getAxisX().setAxisTitle(new AxisTitle(""));
	        chart.getAxisY().setAxisTitle(new AxisTitle(""));			
			ITrace2D graphLine = new Trace2DLtd(450);
								
			graphLine.setName("Time(s) vs "+label);
			graphLine.setPhysicalUnits("", "");
			graphLine.setColor(Color.BLUE);
			chart.addTrace(graphLine);			
			chartMap.put(i++, new ChartDataRequestScalar(chart,graphLine,labels2DataRequest.get(label)));
		}
	}
	
	private String[] getDataRequestLabels(SEScenario scenario)
	{
		// Note labels will get units when the engine provides the headers
		// So if no unit is given, the engine unit is used
		
		// Labels are built to mimic how the engine builds it's labels as well
		// So we can map the engine label to our gui label
		String label;
		List<String> dataRequestLabels = new ArrayList<String>();
		for (SEDataRequest dataRequest : scenario.getDataRequests().getRequestedData())
		{
			label="";
			if(dataRequest instanceof SEPhysiologyDataRequest)
			{
				label = dataRequest.getName();
			}
			else if(dataRequest instanceof SETissueCompartmentDataRequest)
			{
				SETissueCompartmentDataRequest dr=(SETissueCompartmentDataRequest)dataRequest;
				label = dr.getCompartment();				
				label += "-" + dr.getName();
			}
			else if(dataRequest instanceof SECompartmentSubstanceDataRequest)
			{
				SECompartmentSubstanceDataRequest dr=(SECompartmentSubstanceDataRequest)dataRequest;
				label = dr.getCompartment();
				if (dr.hasSubstance())
					label += "-" + dr.getSubstance().getName();				
				label += "-" + dr.getName();
			}
			else if(dataRequest instanceof SECompartmentDataRequest)
			{
			  SECompartmentDataRequest dr=(SECompartmentDataRequest)dataRequest;
				label = dr.getCompartment();						
				label += "-" + dr.getName();
			}
			else if(dataRequest instanceof SEEnvironmentDataRequest)
			{
				label = dataRequest.getName();
			}
			else if(dataRequest instanceof SEEquipmentDataRequest)
			{
				label = dataRequest.getName();
			}			
			else if(dataRequest instanceof SESubstanceDataRequest)
			{
				SESubstanceDataRequest dr = (SESubstanceDataRequest)dataRequest;
				label = dr.getSubstance().getName();
				if(dr.hasCompartment())
					label += "-"+dr.getCompartment();	
				label += "-" + dr.getName();
			}
			else if (dataRequest instanceof SEPatientDataRequest)
			{
				label = dataRequest.getName();
			}			
			else
				Log.error("Unsupported DataRequest "+dataRequest.getClass().getName());
			
			if(dataRequest.hasUnit())
			  label = label+"("+dataRequest.getUnit()+")";
			dataRequestLabels.add(label);
			labels2DataRequest.put(label, dataRequest);
		}
		Collections.sort(dataRequestLabels);		
		return dataRequestLabels.toArray(new String[] {});
	}

	public void actionPerformed(ActionEvent e)
	{
		Object src = e.getSource();
		
		if (src == this.cancelCloseButton)
		{	
			if (this.cancelCloseButton.getText().equals("Cancel"))
			{
				cancel();
			}
			else if (this.cancelCloseButton.getText().equals("Close"))
			{
					((Window) getRootPane().getParent()).dispose();
			}
		}
		else if(e.getSource() instanceof ScenarioCalcThread)
		{
			if(e.getActionCommand().equals("CalcComplete"))
			{
				this.cancelCloseButton.setEnabled(true);
				
			}			
			else if(e.getActionCommand().equals("Graphing"))
			{
				this.cancelCloseButton.setText("Close");
				this.cancelCloseButton.setEnabled(false);
			}
		}
		
	}

}
