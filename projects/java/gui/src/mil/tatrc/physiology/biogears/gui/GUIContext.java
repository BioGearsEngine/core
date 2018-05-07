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
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.Insets;

import javax.swing.JFrame;
import javax.swing.JSplitPane;
import javax.swing.UIManager;

import mil.tatrc.physiology.biogears.gui.controls.ConsoleCtrl;
import mil.tatrc.physiology.biogears.gui.scenario.ScenarioEditor;

/**
 * @author abray
 * I don't know if this is a good idea or not.
 * Not sure the best way for ui components to talk to each other
 * It works for now...
 */
public class GUIContext
{
  protected JFrame         appFrame;
  protected ConsoleCtrl    console;
  //protected JSplitPane   treeVsCanvas;
  protected JSplitPane     logSplit;
  protected ScenarioEditor scenarioEditor;
  
  protected static int INITIAL_DIVIDER_LOCATION;
  
  protected static final String NAME = "BioGears Scenario Driver";
  
  protected GUIContext()
  {
  	Font labelFont = UIManager.getFont("Label.font");
    //Font textareaFont = UIManager.getFont("TextArea.font");
    //tweak:
    UIManager.put("TextArea.font", labelFont);
    
    appFrame=new JFrame(NAME);
    console=new ConsoleCtrl();
    logSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
    scenarioEditor = new ScenarioEditor();
    
    INITIAL_DIVIDER_LOCATION = logSplit.getDividerLocation();
  }
  protected static GUIContext me=null;
  
  public static void postFixWindowName(String name)
  {
  	if(me==null)
      me=new GUIContext();
    me.appFrame.setTitle(NAME+" - "+name);
  }
  
  public static JFrame getAppFrame()
  {
    if(me==null)
      me=new GUIContext();
    return me.appFrame;
  }
  
  public static ConsoleCtrl getConsole()
  {
    if(me==null)
      me=new GUIContext();
    return me.console;
  }
  
  public static ScenarioEditor getScenarioEditor()
  {
  	if(me==null)
      me=new GUIContext();
    return me.scenarioEditor;
  }
/*  
  public static JSplitPane getTreeVsCanvas()
  {
    if(me==null)
      me=new GUIContext();
    return me.treeVsCanvas;
  }
*/  
  public static JSplitPane getLogSplit()
  {
    if(me==null)
      me=new GUIContext();
    return me.logSplit;
  }
  
  public static GridBagConstraints createGBC(int x,int y, double wx,double wy, int w,int h, int a, int f, Insets i) 
  {
    GridBagConstraints gbc = new GridBagConstraints();
    gbc.gridx = x;
    gbc.gridy = y;
    gbc.weightx = 1;
    gbc.weighty = 1;
    gbc.gridwidth = w;
    gbc.gridheight = h;
    gbc.anchor = a;
    gbc.fill = f;
    gbc.insets = i;
    return gbc;
 }
  
}
