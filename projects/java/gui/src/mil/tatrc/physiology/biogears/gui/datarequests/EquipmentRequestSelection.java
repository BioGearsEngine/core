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

import java.awt.event.ActionListener;
import java.util.*;


import mil.tatrc.physiology.biogears.gui.controls.ScalarCtrl.ScalarProperty;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEEquipmentDataRequest;
import mil.tatrc.physiology.utilities.StringUtils;



public class EquipmentRequestSelection extends DataRequestSelection
{


  public EquipmentRequestSelection(ActionListener pnt, Map<String,List<ScalarProperty>> categories2properties)
  {
  	super(pnt, categories2properties);
  }
  
	public DataRequestRow createRow()
	{
		String[] labels = new String[3];
		labels[0] = (String)this.categoryComboBox.getSelectedItem();
		labels[1] = (String)this.activePropBox.getSelectedItem();
		labels[2] = (String)this.activeUnitBox.getSelectedItem();
		
		SEEquipmentDataRequest edr = new SEEquipmentDataRequest();
		edr.setType(StringUtils.removeSpaces(labels[0]));
		edr.setName(StringUtils.removeSpaces(labels[1]));
		edr.setUnit(labels[2]);
		
		DataRequestRow row = new DataRequestRow(this.pntListener, labels[0]+" "+labels[1]+"("+labels[2]+")", edr);
		return row;
	}
}
