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
package mil.tatrc.physiology.biogears.gui.controls;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;


/**
 * @author abray
 *
 */
public class ConsoleCtrl extends JPanel 
{
  protected JTextArea tArea;
  
  public ConsoleCtrl()
  {
    super(new BorderLayout()); 
    tArea = new JTextArea(5,80);
    tArea.setBackground(Color.white);
    tArea.setEditable(false);
    add(new JScrollPane(tArea));
    //consoleScroll.setBorder(BorderFactory.createCompoundBorder(raisedbevel,loweredbevel));
  }
  
  public void println(String msg)
  {
    tArea.append(msg+"\n");
    tArea.setCaretPosition(tArea.getDocument().getLength());
  }
}
