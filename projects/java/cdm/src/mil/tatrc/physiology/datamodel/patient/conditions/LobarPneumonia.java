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

package mil.tatrc.physiology.datamodel.patient.conditions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.LobarPneumoniaData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class LobarPneumonia extends SEPatientCondition
{
	protected SEScalar0To1 severity;
	protected SEScalar0To1 leftLungAffected;
	protected SEScalar0To1 rightLungAffected;
	
	public LobarPneumonia()
	{
		severity = null;
		leftLungAffected = null;
		rightLungAffected = null;
	}
	
	public void reset()
	{
		super.reset();
		if (severity != null)
			severity.invalidate();
		if (leftLungAffected != null)
		  leftLungAffected.invalidate();
		if (rightLungAffected != null)
		  rightLungAffected.invalidate();
	}
	
	public void copy(LobarPneumonia other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.severity != null)
			getSeverity().set(other.getSeverity());
		if (other.leftLungAffected != null)
      getLeftLungAffected().set(other.getLeftLungAffected());
		if (other.rightLungAffected != null)
      getRightLungAffected().set(other.getRightLungAffected());
	}
	
	public boolean load(LobarPneumoniaData in) 
	{
		super.load(in);
		getSeverity().load(in.getSeverity());
		getLeftLungAffected().load(in.getLeftLungAffected());
		getRightLungAffected().load(in.getRightLungAffected());
		return true;
	}
	
	public LobarPneumoniaData unload() 
	{
		LobarPneumoniaData to = CDMSerializer.objFactory.createLobarPneumoniaData();
		unload(to);
		return to;
	}
	
	protected void unload(LobarPneumoniaData data)
	{
		super.unload(data);
		if (severity != null)
			data.setSeverity(severity.unload());
		if (leftLungAffected != null)
      data.setLeftLungAffected(leftLungAffected.unload());
		if (rightLungAffected != null)
      data.setRightLungAffected(rightLungAffected.unload());
	}
	
	public boolean hasSeverity()
	{
		return severity == null ? false : severity.isValid();
	}
	public SEScalar0To1 getSeverity()
	{
		if (severity == null)
			severity = new SEScalar0To1();
		return severity;
	}
	
	public boolean hasLeftLungAffected()
  {
    return leftLungAffected == null ? false : leftLungAffected.isValid();
  }
  public SEScalar0To1 getLeftLungAffected()
  {
    if (leftLungAffected == null)
      leftLungAffected = new SEScalar0To1();
    return leftLungAffected;
  }
  
  public boolean hasRightLungAffected()
  {
    return rightLungAffected == null ? false : rightLungAffected.isValid();
  }
  public SEScalar0To1 getRightLungAffected()
  {
    if (rightLungAffected == null)
      rightLungAffected = new SEScalar0To1();
    return rightLungAffected;
  }
	
	public String toString()
	{
		return "Lobar Pneumonia" 
				+ "\n\tSeverity: " + getSeverity()
        + "\n\tLeftLungAffected: " + getLeftLungAffected()
        + "\n\tRightLungAffected: " + getRightLungAffected();
	}

	
}
