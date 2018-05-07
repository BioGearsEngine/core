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

package mil.tatrc.physiology.datamodel.scenario.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnumSerializationType;
import mil.tatrc.physiology.datamodel.bind.SerializeStateData;

public class SESerializeState extends SEAction 
{
	protected String                filename;
	protected EnumSerializationType type;
	
	public SESerializeState() 
	{
	  filename = "";
	  type = null;
	}
	
	public void copy(SESerializeState other)
	{
		if(this==other)
			return;
		super.copy(other);
		this.filename = other.filename;
		this.type = other.type;
	}
	
	public void reset() 
	{
		super.reset();
		filename = "";
		type = null;
	}
	
	public boolean isValid()
	{
		return hasType();
	}

	public boolean load(SerializeStateData in) 
	{
		super.load(in);
		this.type = in.getType();
		setFilename(in.getFilename());
		return isValid();
	}
	
	public SerializeStateData unload()
	{
		SerializeStateData data = CDMSerializer.objFactory.createSerializeStateData();
		unload(data);
		return data;
	}	
	protected void unload(SerializeStateData data)
	{
		super.unload(data);
		if(hasFilename())
			data.setFilename(this.filename);
		else
			data.setFilename("");
		if(hasType())
			data.setType(this.type);
	}
	
	public boolean hasFilename()
	{
		return !(filename==null || filename.isEmpty());
	}
	public String getFilename() 
	{
		return filename;
	}
	public void setFilename(String filename)
	{
	  this.filename = filename;
	}
	public void invalidateFilename()
	{
	  this.filename = "";
	}
	
	public boolean hasType()
	{
		return type!=null;
	}
	public EnumSerializationType getType() 
	{
		return type;
	}
	public void setType(EnumSerializationType t)
	{
	  this.type = t;
	}
	public void invalidateType()
	{
	  this.type = null;
	}
	
	public String toString() 
	{
		return "Save State" 
				+ "\n\tType: " + getType()
				+ "\n\tFilename: " + getFilename();
	}
}
