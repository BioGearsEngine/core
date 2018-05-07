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

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import javax.swing.*;



/**
 * @author cjackson
 *
 */
public class ToolBarAction extends AbstractAction 
{
  public ToolBarAction(String text, Icon icon, String description,  char accelerator) 
  {
    super(text, icon);
    putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke(accelerator,
      Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    putValue(SHORT_DESCRIPTION, description);
  }
  
  public void actionPerformed(ActionEvent arg0)
  {
    GUIContext.getConsole().println("Action Preformed: " + getValue(NAME));
  }
}
