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

/**
 * @author fmenozzi
 */
package mil.tatrc.physiology.biogears.gui.scenario;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;
import javax.swing.plaf.basic.BasicArrowButton;

public final class ResultsWindow extends JFrame implements ActionListener, KeyListener
{
	// TODO Move things not being listened to back into constructor
	private final int FRAME_WIDTH 	= 1100;
	private final int FRAME_HEIGHT	= 700;
	
	private final int HOR_GAP		= 5;
	private final int VER_GAP		= 5;
	
	private final JFrame frame 		= this;
	
	private File resultsDir;
	
	private List<String> resultsImagesPaths; 
	
	private int currentImageIndex 	= -1;
	
	private JPanel guiPanel 		= new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
	private ImagePanel imagePanel	= new ImagePanel();
	private JPanel controlsPanel	= new JPanel(new GridLayout(1,3));
	
	private JButton selectButton	= new JButton("Send To");
	private JButton closeButton 	= new JButton("Close");
	
	private JButton prevButton		= new BasicArrowButton(BasicArrowButton.WEST);
	private JButton nextButton		= new BasicArrowButton(BasicArrowButton.EAST);
	
	private JLabel imageNumberLabel;
	
	private JCheckBox errorCheckBox		= new JCheckBox("Show Error", false);
	private JCheckBox residualCheckBox	= new JCheckBox("Show Residual", false);
	private JCheckBox standardCheckBox	= new JCheckBox("Show Standard", true);
	
	private class ImagePanel extends JPanel
	{
		private Image img;
		
		@Override
		public void paintComponent(Graphics g) 
		{
			g.drawImage(img, 0, 0, getWidth(), getHeight(), null);
		}
		
		public void setImage(String imageFilePath)
		{
			this.img = new ImageIcon(imageFilePath).getImage();
			validate();
			repaint();
		}
	}
	
	public ResultsWindow(File resultsDir)
	{
		super();
		
		errorCheckBox.setEnabled(false);
		residualCheckBox.setEnabled(false);
		
		this.resultsDir = resultsDir;
		
		this.addKeyListener(this);
		this.setFocusable(true);
		
		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.addWindowListener(new WindowAdapter()
		{
			@Override
			public void windowClosing(WindowEvent e)
			{
				frame.dispose();		    
			}
		});

		resultsImagesPaths = new ArrayList<String>();
		for (File imageFile : resultsDir.listFiles())
			if (imageFile.getAbsolutePath().endsWith(".jpg"))
				resultsImagesPaths.add(imageFile.getAbsolutePath());
		
		if(resultsImagesPaths.isEmpty())
		{
		  this.setTitle("No Results Found");
		  return;
		}
		else
		  this.setTitle("Results - " + resultsImagesPaths.get(0));

		this.setSize(new Dimension(FRAME_WIDTH, FRAME_HEIGHT));
			
		imagePanel.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		
		JPanel selectClosePanel = new JPanel(new FlowLayout());
		selectButton.setToolTipText("Open a file with the default extension viewer");
		selectButton.addActionListener(this);
		closeButton.addActionListener(this);
		selectClosePanel.add(selectButton);
		selectClosePanel.add(closeButton);
		controlsPanel.add(selectClosePanel);
		
		JPanel centerControlPanel = new JPanel(new BorderLayout(HOR_GAP, VER_GAP));
		
		JPanel leftRightPanel = new JPanel(new FlowLayout());
		prevButton.addActionListener(this);
		nextButton.addActionListener(this);
		leftRightPanel.add(new JLabel("Prev"));
		leftRightPanel.add(prevButton);
		leftRightPanel.add(nextButton);
		leftRightPanel.add(new JLabel("Next"));
		
		JPanel imageNumberPanel = new JPanel(new FlowLayout());
		imageNumberLabel = new JLabel("1 of " + resultsImagesPaths.size());
		imageNumberPanel.add(imageNumberLabel);
		
		gotoNextImage();
		
		centerControlPanel.add(leftRightPanel, BorderLayout.CENTER);
		centerControlPanel.add(imageNumberPanel, BorderLayout.SOUTH);
		
		controlsPanel.add(centerControlPanel);
				
		JPanel checkboxPanel = new JPanel(new FlowLayout());
		errorCheckBox.addActionListener(this);
		residualCheckBox.addActionListener(this);
		standardCheckBox.addActionListener(this);
		checkboxPanel.add(standardCheckBox);
		checkboxPanel.add(errorCheckBox);
		checkboxPanel.add(residualCheckBox);	
		controlsPanel.add(checkboxPanel);
		
		controlsPanel.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		
		guiPanel.add(imagePanel, BorderLayout.CENTER);
		guiPanel.add(controlsPanel, BorderLayout.SOUTH);
		
		this.add(guiPanel);
		
		this.setVisible(true);
	}
	
	private void decrementCurrentImageIndex()
	{
		if (currentImageIndex == 0)
			currentImageIndex = resultsImagesPaths.size() - 1;
		else
			currentImageIndex--;
	}
	
	private void incrementCurrentImageIndex()
	{
		if (currentImageIndex == resultsImagesPaths.size() - 1)
			currentImageIndex = 0;
		else
			currentImageIndex++;
	}
	
	private String getCurrentImagePath()
	{
		return resultsImagesPaths.get(currentImageIndex);
	}
	
	private boolean currentImageIsResidualImage()
	{
		return getCurrentImagePath().contains("Residual");
	}
	
	private boolean currentImageIsErrorImage()
	{
		return getCurrentImagePath().contains("Error");
	}
	
	private boolean currentImageIsStandardImage()
	{
		return ! (currentImageIsErrorImage() || currentImageIsResidualImage());
	}
	
	private boolean nothingIsChecked()
	{
		return ! (standardCheckBox.isSelected() || errorCheckBox.isSelected() || residualCheckBox.isSelected());
	}
	
	private void gotoPreviousImage()
	{
		if (nothingIsChecked())
			return;
		
		decrementCurrentImageIndex();
		
		while (true)
		{
			if (currentImageIsErrorImage() && errorCheckBox.isSelected())
				break;
			else if (currentImageIsResidualImage() && residualCheckBox.isSelected())
				break;
			else if (currentImageIsStandardImage() && standardCheckBox.isSelected())
				break;
			
			decrementCurrentImageIndex();
		}
		
		imagePanel.setImage(getCurrentImagePath());
		imageNumberLabel.setText(currentImageIndex+1 + " of " + resultsImagesPaths.size());
		this.setTitle("Results - " + getCurrentImagePath());
	}
	
	private void gotoNextImage()
	{
		if (nothingIsChecked())
			return;
		
		incrementCurrentImageIndex();
		
		while (true)
		{
			if (currentImageIsErrorImage() && errorCheckBox.isSelected())
				break;
			else if (currentImageIsResidualImage() && residualCheckBox.isSelected())
				break;
			else if (currentImageIsStandardImage() && standardCheckBox.isSelected())
				break;
			
			incrementCurrentImageIndex();
		}
		
		imagePanel.setImage(getCurrentImagePath());
		imageNumberLabel.setText(currentImageIndex+1 + " of " + resultsImagesPaths.size());
		this.setTitle("Results - " + getCurrentImagePath());
	}

	@Override
	public void actionPerformed(ActionEvent e) 
	{
		Object src = e.getSource();
		
		if (src == this.selectButton)
		{
			JFileChooser fileChooser = new JFileChooser(resultsDir.getAbsoluteFile());
			fileChooser.setDialogTitle("Open File With Default Viewer");
			fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
			fileChooser.setSelectedFile(new File(getCurrentImagePath()));
			
			if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) 
			{		
				File file = fileChooser.getSelectedFile();
				try 
				{
					Desktop.getDesktop().open(file);
				} 
				catch (IOException ex) 
				{
					JOptionPane.showMessageDialog(this, "Unable to open " + file.getName(), "Error", JOptionPane.ERROR_MESSAGE);
				}
			} 
		}
		else if (src == this.closeButton)
		{
			this.dispose();
		}
		else if (src == this.prevButton)
		{
			gotoPreviousImage();
		}
		else if (src == this.nextButton)
		{
			gotoNextImage();
		}
	}

	@Override
	public void keyTyped(KeyEvent e) {}

	@Override
	public void keyPressed(KeyEvent e) {}

	@Override
	public void keyReleased(KeyEvent e) 
	{
		// TODO There's a bug somewhere with setting the focus to this frame
		int keyCode = e.getKeyCode();
		if (keyCode == KeyEvent.VK_LEFT || keyCode == KeyEvent.VK_KP_LEFT)
			gotoPreviousImage();
		else if (keyCode == KeyEvent.VK_RIGHT || keyCode == KeyEvent.VK_KP_RIGHT)
			gotoNextImage();
	}
}
