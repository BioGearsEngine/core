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
 * 
 */
package mil.tatrc.physiology.biogears.gui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

/**
 * @author abray
 * http://java.sun.com/docs/books/tutorial/uiswing/components/menu.html
 */
public class MainMenu extends JMenuBar implements ActionListener
{  	
	// File
	private static String NEW =  "New";
	private static String OPEN = "Open";
	private static String SAVE = "Save";
	private static String EXIT = "Exit";
	
	// View
	private static String LOG = "Log";

	private JFileChooser fileChooser;

	public MainMenu()
	{
		fileChooser = new JFileChooser();
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);	
		fileChooser.setFileFilter(new FileNameExtensionFilter("XML File", "xml"));
		
		JMenuItem menuItem;

		// File Menu
		JMenu menuFile = new JMenu("File");
		
		menuItem = new JMenuItem(NEW);
		menuItem.addActionListener(this);
		menuFile.add(menuItem);
		
		menuItem = new JMenuItem(OPEN);
		menuItem.addActionListener(this);
		menuFile.add(menuItem);
		
		menuItem = new JMenuItem(SAVE);
		menuItem.addActionListener(this);
		menuFile.add(menuItem);
		menuFile.addSeparator();
		
		menuItem = new JMenuItem(EXIT);
		menuItem.addActionListener(this);
		menuFile.add(menuItem);
		menuFile.addSeparator();
		
		add(menuFile);

		// View Menu
		JMenu menuView = new JMenu("View");
		menuItem = new JMenuItem(LOG);
		menuItem.addActionListener(this);
		menuView.add(menuItem);
		
		add(menuView);
	}

	/**
	 * Action Performed when user clicks a menu item
	 */
	public void actionPerformed(ActionEvent ae)
	{
		String command = ae.getActionCommand();
		if (command.equals(NEW))
		{
			GUIContext.getScenarioEditor().clear();
		}
		else if (command.equals(OPEN))
		{
			fileChooser.setCurrentDirectory(new File(System.getProperty("user.dir")));
			if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) 
			{		
				File file = fileChooser.getSelectedFile().getAbsoluteFile();
				GUIContext.getConsole().println("Opening " + file.getName() + "...");
				if (GUIContext.getScenarioEditor().loadScenarioFile(file))
				{	
					GUIContext.getConsole().println("Done");	
					GUIContext.postFixWindowName(file.getAbsolutePath());
				}
				else
				{
					GUIContext.getConsole().println("ERROR: Problem opening " + file.getName());
				}
			} 
			else 
			{
				GUIContext.getConsole().println("Open command cancelled by user.");
			}
		}		
		else if (command.equals(SAVE))
		{			
			fileChooser.setCurrentDirectory(new File(System.getProperty("user.dir")));
			if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION)
			{
				File file = fileChooser.getSelectedFile();
				GUIContext.getConsole().println("Saving as " + file.getName() + "...");
				if (GUIContext.getScenarioEditor().saveScenarioFileAs(file.getAbsolutePath()))
				{
					GUIContext.getConsole().println("Done");
					// TODO Remove the '*' from the name, if present
				}
			}
			else
			{
				GUIContext.getConsole().println("Save command cancelled by user.");
			}		
		}
		else if (command.equals(LOG))
		{
			boolean b = GUIContext.getConsole().isVisible();
			GUIContext.getConsole().setVisible(!b);
			if (!b)
				GUIContext.getLogSplit().setDividerLocation(GUIContext.INITIAL_DIVIDER_LOCATION);
		}
		else if (command.equals(EXIT))
		{
			//exit the program
			System.exit(0);
		}
	}
}
