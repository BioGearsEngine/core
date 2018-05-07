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
import mil.tatrc.physiology.datamodel.bind.ChronicRenalStenosisData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class ChronicRenalStenosis extends SEPatientCondition
{
	protected SEScalar0To1 leftKidneySeverity;
	protected SEScalar0To1 rightKidneySeverity;
	
	public ChronicRenalStenosis()
	{
		leftKidneySeverity = null;
		rightKidneySeverity = null;
	}
	
	public void reset()
	{
		super.reset();
		if (leftKidneySeverity != null)
			leftKidneySeverity.invalidate();
		if (rightKidneySeverity != null)
			rightKidneySeverity.invalidate();
	}
	
	public void copy(ChronicRenalStenosis other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.leftKidneySeverity != null)
			getLeftKidneySeverity().set(other.getLeftKidneySeverity());
		else if (leftKidneySeverity != null)
			leftKidneySeverity.invalidate();
		
		if (other.rightKidneySeverity != null)
			getRightKidneySeverity().set(other.getRightKidneySeverity());
		else if (rightKidneySeverity != null)
			rightKidneySeverity.invalidate();
	}
	
	public boolean load(ChronicRenalStenosisData in) 
	{
		super.load(in);
		getLeftKidneySeverity().load(in.getLeftKidneySeverity());
		getRightKidneySeverity().load(in.getRightKidneySeverity());
		return true;
	}
	
	public ChronicRenalStenosisData unload() 
	{
		ChronicRenalStenosisData to = CDMSerializer.objFactory.createChronicRenalStenosisData();
		unload(to);
		return to;
	}
	
	protected void unload(ChronicRenalStenosisData data)
	{
		super.unload(data);
		if (leftKidneySeverity != null)
			data.setLeftKidneySeverity(leftKidneySeverity.unload());
		if (rightKidneySeverity != null)
			data.setRightKidneySeverity(rightKidneySeverity.unload());
	}
	
	public boolean hasLeftKidneySeverity()
	{
		return leftKidneySeverity == null ? false : leftKidneySeverity.isValid();
	}
	public SEScalar0To1 getLeftKidneySeverity()
	{
		if (leftKidneySeverity == null)
			leftKidneySeverity = new SEScalar0To1();
		return leftKidneySeverity;
	}
	
	public boolean hasRightKidneySeverity()
	{
		return rightKidneySeverity == null ? false : rightKidneySeverity.isValid();
	}
	public SEScalar0To1 getRightKidneySeverity()
	{
		if (rightKidneySeverity == null)
			rightKidneySeverity = new SEScalar0To1();
		return rightKidneySeverity;
	}
	
	public String toString()
	{
		return "Chronic Renal Stenosis" 
				+ "\n\tLeftKidneySeverity: " + getLeftKidneySeverity()
				+ "\n\tRightKidneySeverity: " + getRightKidneySeverity();
	}

	
}
