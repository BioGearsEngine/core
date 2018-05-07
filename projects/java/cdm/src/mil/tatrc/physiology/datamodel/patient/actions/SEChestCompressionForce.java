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

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ChestCompressionForceData;
import mil.tatrc.physiology.datamodel.properties.SEScalarForce;

public class SEChestCompressionForce extends SEChestCompression
{
	protected SEScalarForce Force;
	
	public SEChestCompressionForce()
	{
		Force = null;
	}
	
	public SEChestCompressionForce(SEChestCompressionForce other)
	{
		this();
		copy(other);
	}
	
	public void copy(SEChestCompressionForce other)
	{
		if (other.Force != null)
			getForce().set(other.Force);
		else if (Force != null)
			Force.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (Force != null)
			Force.invalidate();
	}
	
	public boolean isValid()
	{
		return hasForce();
	}
	
	public boolean load(ChestCompressionForceData in)
	{
		super.load(in);
		getForce().load(in.getForce());
		return isValid();
	}
	
	public ChestCompressionForceData unload()
	{
		ChestCompressionForceData data = CDMSerializer.objFactory.createChestCompressionForceData();
		unload(data);
		return data;
	}
	
	protected void unload(ChestCompressionForceData data)
	{
		super.unload(data);
		if (Force != null)
			data.setForce(Force.unload());
	}
	
	public boolean hasForce()
	{
		return Force == null ? false : Force.isValid();
	}
	public SEScalarForce getForce()
	{
		if (Force == null)
			Force = new SEScalarForce();
		return Force;
	}
	
	public String toString()
	{
		if (Force != null)
			return "Chest Compression" 
					+ "\n\tForce: " + getForce();
		else
			return "Action not specified properly";
	}
}