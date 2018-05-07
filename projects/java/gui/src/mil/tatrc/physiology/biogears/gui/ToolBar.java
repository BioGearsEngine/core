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

import javax.swing.*;

/**
 * @author abray
 *
 */
public class ToolBar extends JToolBar
{
  public ToolBar()
  {
    super("BioGears Tools");
    addSeparator();
    //this.addButtons();
    this.setFloatable(true);
    this.setSize(128, 32);
  }
  
  protected void addButtons()
  {
  	/*
    ToolBarAction human = new ToolBarAction("Human", new ImageIcon(
    "images\\human.png"), "Human", 'H');
    add(human);
    //addSeparator();
    ToolBarAction earth = new ToolBarAction("Earth", new ImageIcon(
    "images\\earth.png"), "Earth", 'E');
    add(earth);
    addSeparator();
    */
  }
  
  

}
