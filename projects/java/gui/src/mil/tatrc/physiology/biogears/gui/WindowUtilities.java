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

package mil.tatrc.physiology.biogears.gui;

import javax.swing.*;

import java.awt.*;

/** A few utilities that simplify using windows in Swing.
 *  1998-99 Marty Hall, http://www.apl.jhu.edu/~hall/java/
 */

public class WindowUtilities 
{

	/** Tell system to use native look and feel, as in previous
	 *  releases. Metal (Java) LAF is the default otherwise.
	 */

	public static void setNativeLookAndFeel() 
	{
		try 
		{
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} 
		catch(Exception e) 
		{
			System.out.println("Error setting native LAF: " + e);
		}
	}

	public static void setJavaLookAndFeel()
	{
		try 
		{
			UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
		} 
		catch(Exception e) 
		{
			System.out.println("Error setting Java LAF: " + e);
		}
	}

	public static void setMotifLookAndFeel() 
	{
		try 
		{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.motif.MotifLookAndFeel");
		} 
		catch(Exception e) 
		{
			System.out.println("Error setting Motif LAF: " + e);
		}
	}

	/** A simplified way to see a JPanel or other Container.
	 *  Pops up a JFrame with specified Container as the content pane.
	 */

	public static JFrame openInJFrame(Container content,
			int width,
			int height,
			String title,
			Color bgColor) 
	{
		JFrame frame = new JFrame(title);
		frame.setBackground(bgColor);
		content.setBackground(bgColor);
		frame.setSize(width, height);
		frame.setContentPane(content);
		frame.addWindowListener(new ExitListener());
		frame.setVisible(true);
		return(frame);
	}

	/** Uses Color.white as the background color. */

	public static JFrame openInJFrame(Container content,
			int width,
			int height,
			String title) 
	{
		return(openInJFrame(content, width, height, title, Color.white));
	}

	/** Uses Color.white as the background color, and the
	 *  name of the Container's class as the JFrame title.
	 */

	public static JFrame openInJFrame(Container content,
			int width,
			int height) 
	{
		return(openInJFrame(content, width, height,
				content.getClass().getName(),
				Color.white));
	}
}
