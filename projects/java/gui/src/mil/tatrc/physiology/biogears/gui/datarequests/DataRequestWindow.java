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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EtchedBorder;
import javax.swing.filechooser.FileNameExtensionFilter;

import mil.tatrc.physiology.biogears.gui.GUIContext;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;

public class DataRequestWindow extends JFrame implements ActionListener
{	
	private final int FRAME_WIDTH	  = 800;
	private final int FRAME_HEIGHT	= 600;

	private final int HOR_GAP		= 5;
	private final int VER_GAP		= 5;
	
	private final String DEFAULT_TITLE = "Data Requests";

	private SEScenario       scenario;

	private JPanel           guiPanel;
	private DataRequestPanel dataRequestPanel;

	private JButton    resultFileBtn = new JButton("SetResults");
	
	private JButton    okButton		  = new JButton("OK");
	private JButton    cancelButton	= new JButton("Cancel");

	private JLabel     sampleLabel  = new JLabel();
	private JTextField sampleRate   = new JTextField();
	
	private JFileChooser fileChooser = new JFileChooser();

	public DataRequestWindow(SEScenario scenario)
	{
		super();

		this.setSize(new Dimension(FRAME_WIDTH, FRAME_HEIGHT));
		if(scenario.getDataRequests().hasFilename())
			this.setTitle(scenario.getDataRequests().getFilename());
		else
			this.setTitle(DEFAULT_TITLE);
		this.setResizable(false);

		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.addWindowListener(new WindowAdapter()
		{
			@Override
			public void windowClosing(WindowEvent e)
			{
				dispose();	
			}
		});

		this.scenario = scenario;

		guiPanel = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));

		JPanel controlsPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
		okButton.addActionListener(this);
		cancelButton.addActionListener(this);
		controlsPanel.add(okButton);
		controlsPanel.add(cancelButton);
		
		JPanel samplePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
		sampleLabel.setText("Output file samples per second: ");
		sampleRate.setPreferredSize(new Dimension(30, 20));
		if(scenario.getDataRequests().getSamplesPerSecond()>0)
			sampleRate.setText(String.valueOf(scenario.getDataRequests().getSamplesPerSecond()));
		samplePanel.add(this.sampleLabel);
		samplePanel.add(this.sampleRate);
		
		JPanel controlsPnt = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
		controlsPnt.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		
		JPanel resultsPnt = new JPanel(new FlowLayout(FlowLayout.LEFT));
		resultsPnt.setPreferredSize(new Dimension(215, 20));		
		resultsPnt.add(resultFileBtn);
		resultFileBtn.addActionListener(this);
		
		controlsPnt.add(resultsPnt,BorderLayout.WEST);
		controlsPnt.add(controlsPanel,BorderLayout.CENTER);
		controlsPnt.add(samplePanel,BorderLayout.EAST);
		

		dataRequestPanel = new DataRequestPanel(new BorderLayout(HOR_GAP, VER_GAP), this.scenario);
		guiPanel.add(dataRequestPanel, BorderLayout.CENTER);
		guiPanel.add(controlsPnt, BorderLayout.SOUTH);

		this.add(guiPanel);

		this.setVisible(true);
	}
	
	public void actionPerformed(ActionEvent e) 
	{
		Object src = e.getSource();

		if (src == this.okButton)
		{	
			this.dataRequestPanel.updateScenario();
			if(!this.getTitle().equals(DEFAULT_TITLE))
				this.scenario.getDataRequests().setFilename(this.getTitle());
			String samplesPerSecond = this.sampleRate.getText();
			if(samplesPerSecond!=null&&!samplesPerSecond.isEmpty())
			{
				try
				{
					this.scenario.getDataRequests().setSamplesPerSecond(Double.parseDouble(samplesPerSecond));
				}
				catch(Exception ex)
				{
					this.scenario.getDataRequests().setSamplesPerSecond(0);
				}
			}
			
			this.dispose();
		}
		else if (src == this.cancelButton)
		{
			this.dispose();
		}
		else if (src == resultFileBtn)
		{			
			fileChooser.setFileFilter(new FileNameExtensionFilter("txt File", "txt"));
			fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);	
			fileChooser.setCurrentDirectory(new File(System.getProperty("user.dir")));
			if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION)
			{
				File file = fileChooser.getSelectedFile();
				this.setTitle(file.getAbsolutePath());
			}
		}
	}
}
