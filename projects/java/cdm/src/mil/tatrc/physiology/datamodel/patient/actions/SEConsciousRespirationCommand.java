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

package mil.tatrc.physiology.datamodel.patient.actions;

import mil.tatrc.physiology.datamodel.bind.*;

public abstract class SEConsciousRespirationCommand
{
	protected String comment;
	
	public SEConsciousRespirationCommand()
	{
	}
	
	public void copy(SEConsciousRespirationCommand other)
	{
		if(this==other)
			return;
		
		comment = other.comment;
	}
	
	public abstract SEConsciousRespirationCommand getCopy();
	
	public void reset()
	{
		comment = "";
	}
	
	public boolean load(ConsciousRespirationCommandData in)
	{
		comment = in.getComment();
		
		return true;
	}
	
	protected void unload(ConsciousRespirationCommandData data)
	{
		if (hasComment())
			data.setComment(comment);
	}
	
	public boolean hasComment()
	{
		return comment != null && !comment.isEmpty();
	}
	public String getComment()
	{
		return comment;
	}
	public void setComment(String s)
	{
		comment = s;
	}
	
	public abstract boolean isValid();
	public abstract String toString();
}
