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

package mil.tatrc.physiology.utilities;

import org.apache.log4j.Level;

public abstract class LogListener 
{
	protected Level level;
	protected boolean handleProgress;
	
	public LogListener()
	{
		level=Log.getLevel();
		handleProgress=false;
		listen(true);
	}
	
	public void listen(boolean b)
  {
	  if(b)
	    Log.addAppender(this);
	  else
	    Log.removeAppender(this);
  }
	
	public final void setLevel(Level lvl)
	{
		this.level=lvl;
	}
	public final void handleProgress(boolean b)
	{
		this.handleProgress=b;
	}
	
	public final void trace(String msg)
	{
		if(Level.TRACE.isGreaterOrEqual(this.level))
		{
		  handleTrace(msg);
		}
	}
	public void handleTrace(String msg){}
	
	public final void trace(String msg, String origin)
	{
		if(Level.TRACE.isGreaterOrEqual(this.level))
		{
			handleTrace(msg,origin);
		}
	}
	public void handleTrace(String msg, String origin){handleTrace(origin + " : " + msg);}
	
	public final void trace(String msg, Throwable t)
	{
		if(Level.TRACE.isGreaterOrEqual(this.level))
		{
			handleTrace(msg,t);
		}
	}
	public void handleTrace(String msg, Throwable t){handleTrace(msg+" "+t.getMessage());}
	
	public final void debug(String msg)
	{
		if(Level.DEBUG.isGreaterOrEqual(this.level))
		{
		  handleDebug(msg);
		}
	}
	public void handleDebug(String msg){}
	
	public final void debug(String msg, String origin)
	{
		if(Level.DEBUG.isGreaterOrEqual(this.level))
		{
		  handleDebug(msg,origin);
		}
	}
	public void handleDebug(String msg, String origin){handleDebug(origin + " : " + msg);}
	
	public final void debug(String msg, Throwable t)
	{
		if(Level.DEBUG.isGreaterOrEqual(this.level))
		{
		  handleDebug(msg,t);
		}
	}
	public void handleDebug(String msg, Throwable t){handleDebug(msg+" "+t.getMessage());}
	
	public final void info(String msg)
	{
		if(Level.INFO.isGreaterOrEqual(this.level))
		{
			handleInfo(msg);
		}
	}
	public void handleInfo(String msg){}
	
	public final void info(String msg, String origin)
	{
		if(Level.INFO.isGreaterOrEqual(this.level))
		{
		  handleInfo(msg,origin);
		}
	}
	public void handleInfo(String msg, String origin){handleInfo(origin + " : " + msg);}
	
	public final void info(String msg, Throwable t)
	{
		if(Level.INFO.isGreaterOrEqual(this.level))
		{
		  handleInfo(msg,t);
		}
	}
	public void handleInfo(String msg, Throwable t){handleInfo(msg+" "+t.getMessage());}
	
	public final void warn(String msg)
	{
		if(Level.WARN.isGreaterOrEqual(this.level))
		{
		  handleWarn(msg);
		}
	}
	public void handleWarn(String msg){}
	
	public final void warn(String msg, String origin)
	{
		if(Level.WARN.isGreaterOrEqual(this.level))
		{
		  handleWarn(msg,origin);
		}
	}
	public void handleWarn(String msg, String origin){handleWarn(origin + " : " + msg);}
	
	public final void warn(String msg, Throwable t)
	{
		if(Level.WARN.isGreaterOrEqual(this.level))
		{
		  handleWarn(msg,t);
		}
	}
	public void handleWarn(String msg, Throwable t){handleWarn(msg+" "+t.getMessage());}
	
	public final void error(String msg)
	{
		if(Level.ERROR.isGreaterOrEqual(this.level))
		{
		  handleError(msg);
		}
	}
	public void handleError(String msg){}
	
	public final void error(String msg, String origin)
	{
		if(Level.ERROR.isGreaterOrEqual(this.level))
		{
		  handleError(msg,origin);
		}
	}
	public void handleError(String msg, String origin){handleError(origin + " : " + msg);}
	
	public final void error(String msg, Throwable t)
	{
		if(Level.ERROR.isGreaterOrEqual(this.level))
		{
		  handleError(msg,t);
		}
	}
	public void handleError(String msg, Throwable t){handleError(msg+" "+t.getMessage());}
	
	public final void error(Throwable t)
	{
		if(Level.ERROR.isGreaterOrEqual(this.level))
		{
		  handleError(t);
		}
	}
	public void handleError(Throwable t){handleError(t.getMessage());}
	
	public final void fatal(String msg)
	{
		if(Level.FATAL.isGreaterOrEqual(this.level))
		{
		  handleFatal(msg);
		}
	}
	public void handleFatal(String msg){}
	
	public final void fatal(String msg, String origin)
	{
		if(Level.FATAL.isGreaterOrEqual(this.level))
		{
		  handleFatal(msg,origin);
		}
	}
	public void handleFatal(String msg, String origin){handleFatal(origin + " : " + msg);}
	
	public final void fatal(String msg, Throwable t)
	{
		if(Level.FATAL.isGreaterOrEqual(this.level))
		{
		  handleFatal(msg,t);
		}
	}
	public void handleFatal(String msg, Throwable t){handleFatal(msg+" "+t.getMessage());}
	
	public final void fatal(Throwable t)
	{
		if(Level.FATAL.isGreaterOrEqual(this.level))
		{
			handleFatal(t);
		}
	}
	public void handleFatal(Throwable t){handleFatal(t.getMessage());}
	
	public final void progress(String state, String info, float percent)
	{
		if(this.handleProgress)
		{
		  handleProgress(state, info, percent);
		}
	}
	public void handleProgress(String status, String info, float percent){}
}
