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
import mil.tatrc.physiology.datamodel.bind.ChestCompressionForceScaleData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class SEChestCompressionForceScale extends SEChestCompression
{
	protected SEScalar0To1 forceScale;
	protected SEScalarTime     forcePeriod;   
	
	public SEChestCompressionForceScale()
	{
		forceScale = null;
		forcePeriod = null;
	}
	
	public SEChestCompressionForceScale(SEChestCompressionForceScale other)
	{
		this();
		copy(other);
	}
	
	public void copy(SEChestCompressionForceScale other)
	{
		if (other.forceScale != null)
			getForceScale().set(other.forceScale);
		else if (forceScale != null)
			forceScale.invalidate();
		
		if (other.forcePeriod != null)
      getForcePeriod().set(other.forcePeriod);
    else if (forcePeriod != null)
      forcePeriod.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (forceScale != null)
			forceScale.invalidate();
		if (forcePeriod != null)
		  forcePeriod.invalidate();
	}
	
	public boolean isValid()
	{
		return hasForceScale() && hasForcePeriod();
	}
	
	public boolean load(ChestCompressionForceScaleData in)
	{
		super.load(in);
		getForceScale().load(in.getForceScale());
		if(getForcePeriod()!=null)
		  getForcePeriod().load(in.getForcePeriod());
		return isValid();
	}
	
	public ChestCompressionForceScaleData unload()
	{
		ChestCompressionForceScaleData data = CDMSerializer.objFactory.createChestCompressionForceScaleData();
		unload(data);
		return data;
	}
	
	protected void unload(ChestCompressionForceScaleData data)
	{
		super.unload(data);
		if (hasForceScale())
			data.setForceScale(forceScale.unload());
		if (hasForcePeriod())
      data.setForcePeriod(forcePeriod.unload());
	}
	
	public boolean hasForceScale()
	{
		return forceScale == null ? false : forceScale.isValid();
	}
	public SEScalar0To1 getForceScale()
	{
		if (forceScale == null)
			forceScale = new SEScalar0To1();
		return forceScale;
	}
	
	public boolean hasForcePeriod()
  {
    return forcePeriod == null ? false : forcePeriod.isValid();
  }
  public SEScalarTime getForcePeriod()
  {
    if (forcePeriod == null)
      forcePeriod = new SEScalarTime();
    return forcePeriod;
  }
	
	public String toString()
	{
		if (forceScale != null)
			return "Patient Intervention : Chest Compression" 
					+ "\n\tForceScale: " + (hasForceScale() ? getForceScale() : "Not Provided")
					+ "\n\tForcePeriod: " + (hasForcePeriod() ? getForcePeriod() : "Not Provided");
		else
			return "Action not specified properly";
	}
}