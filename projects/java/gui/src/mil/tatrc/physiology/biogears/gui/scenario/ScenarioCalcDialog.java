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

import java.awt.Dimension;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

import javax.swing.JDialog;

import mil.tatrc.physiology.datamodel.scenario.SEScenario;
import mil.tatrc.physiology.utilities.Log;

public class ScenarioCalcDialog extends JDialog
{
	private ScenarioCalcPanel panel;
	public ScenarioCalcDialog(SEScenario scenario)
	{
		this.setTitle(scenario.getName());
		this.setModal(true);
		this.setSize(new Dimension(1280, 950));
		this.setResizable(true);
		this.panel = new ScenarioCalcPanel(scenario);
		this.add(this.panel);
		
		this.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
		
		final ScenarioCalcDialog me = this;
		this.addWindowListener(new WindowAdapter()
		{
			@Override
		    public void windowClosing(WindowEvent e)
			{
				if(me.panel.display.cleanUp())
				{
					me.dispose(); 
					try 
					{
						me.finalize();
					} 
					catch(Throwable ex) 
					{
						Log.error("Unable to close window",ex);
					}
				}					
			}
		});
	}

}
