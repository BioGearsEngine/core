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
import java.io.File;

import javax.swing.JPanel;
import javax.swing.JSplitPane;

import mil.tatrc.physiology.biogears.gui.controls.ConsoleCtrl;
import mil.tatrc.physiology.biogears.gui.controls.ConsoleListener;
import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.utilities.Log;

/**
 * This panel is a split
 * It holds the Graph and Cancel Button on top
 * And the Log Console on bottom
 * @author abray
 *
 */
public class ScenarioCalcPanel extends JPanel
{
	protected ScenarioCalcDisplay  display;
	protected ConsoleCtrl          console;
	protected ConsoleListener sceListener;
	
	
	public ScenarioCalcPanel(SEScenario scenario)
	{
		super(new BorderLayout());
		// Split Pane
		JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		split.setDividerLocation(650);
		split.setEnabled(false);// I don't want you moving the split for now
		// Add the console to the JPanel
		this.console = new ConsoleCtrl();
		this.console.setVisible(true);
		split.setBottomComponent(this.console);
		// Add a listner that listens to the calc and to the log messages in the display class
		// And outputs those logs to the console
		this.sceListener = new ConsoleListener(this.console);
		Log.addAppender(this.sceListener);
		// Add the display
		this.display = new ScenarioCalcDisplay(scenario, this.sceListener);
		split.setTopComponent(this.display);		
		add(split);		
	}
	
	public void finalize()
	{
		Log.removeAppender(this.sceListener);
	}
}
