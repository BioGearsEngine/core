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
import java.awt.GridLayout;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;

public class DataRequestRow extends JPanel
{
	public JButton removeMe;
	public SEDataRequest dr;
	
	DataRequestRow(ActionListener pnt, String text, SEDataRequest dr)
	{
		super();
		GridLayout layout = new GridLayout(0,2);
		this.setLayout(layout);
		
		JPanel left = new JPanel(new FlowLayout(FlowLayout.LEFT));
		JPanel right = new JPanel(new FlowLayout(FlowLayout.RIGHT));
	
		JLabel label = new JLabel();
		label.setText(text);
		left.add(label);
		
		removeMe = new JButton("Remove");
		right.add(removeMe);
		removeMe.addActionListener(pnt);
		
		this.add(left);
		this.add(right);		
		
		this.dr = dr;
	}
}
