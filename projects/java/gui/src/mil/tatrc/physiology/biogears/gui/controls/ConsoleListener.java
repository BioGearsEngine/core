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

package mil.tatrc.physiology.biogears.gui.controls;

import mil.tatrc.physiology.utilities.LogListener;

/** 
 * This class will pass any log messages from the engine to the gui console
 * @author abray
 */
public class ConsoleListener extends LogListener
{
	protected ConsoleCtrl    console;

	public ConsoleListener(ConsoleCtrl console)
	{
		if (console == null)
			throw new RuntimeException("Null console provided");
		this.console=console;
	}

	public void handleDebug(String msg)
	{
		this.console.println("DEBUG : " + msg);
	}
	public void handleInfo(String msg)
	{
		this.console.println("INFO : " + msg);
	}
	public void handleWarn(String msg)
	{
		this.console.println("WARN : " + msg);
	}
	public void handleError(String msg)
	{
		this.console.println("ERROR : " + msg);
	}
	public void handleFatal(String msg)
	{
		this.console.println("FATAL : " + msg);
	}
}
