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
import java.util.List;
import java.util.Map;

import javax.swing.JDialog;
import javax.swing.JFrame;

import mil.tatrc.physiology.datamodel.scenario.actions.SEAction;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.Log;

public class DynamicPropertyEditor extends JDialog
{	
	private final int FRAME_WIDTH	= 420;
	private final int FRAME_HEIGHT= 300;
	
	private DynamicPropertyPanel panel;

	public DynamicPropertyEditor(Object obj, Map<String,List<String>> propertyOptions)
	{
		super();

		this.setModal(true);
		this.setTitle(obj.getClass().getSimpleName());	
		this.setSize(new Dimension(FRAME_WIDTH, FRAME_HEIGHT));
		this.setResizable(false);

		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		
		this.addWindowListener(new WindowAdapter()
		{
			@Override
			public void windowClosing(WindowEvent e)
			{
				panel.cancelled = true;
				dispose(); 
				try 
				{
					finalize();
				} 
				catch(Throwable ex) 
				{
					Log.error("Unable to close window",ex);
				}			
			}
		});
		
		this.panel = new DynamicPropertyPanel(obj,propertyOptions);
		this.add(this.panel);
	}
	public boolean wasCancelled(){ return panel.cancelled; }
}
