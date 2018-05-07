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
import mil.tatrc.physiology.datamodel.bind.ExerciseData;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;

public class SEExercise extends SEPatientAction
{
	protected SEScalar0To1 intensity;
	
	public SEExercise()
	{
		intensity=null;
	}
	
	public void copy(SEExercise other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.intensity != null)
			getIntensity().set(other.getIntensity());
		else if (intensity != null)
		  intensity.invalidate();		
	}
	
	public void reset()
	{
		super.reset();
		if (intensity != null)
		  intensity.invalidate();
	}
	
	public boolean isValid()
	{
		return hasIntensity();
	}
	
	public boolean load(ExerciseData in)
	{
		super.load(in);
		getIntensity().load(in.getIntensity());
		return isValid();
	}
	
	public ExerciseData unload() 
	{
		ExerciseData data = CDMSerializer.objFactory.createExerciseData();
		unload(data);
		return data;
	}
	
	protected void unload(ExerciseData data)
	{
		super.unload(data);
		if (intensity != null)
			data.setIntensity(intensity.unload());
	}
	
	public boolean hasIntensity()
	{
		return intensity == null ? false : intensity.isValid();
	}
	public SEScalar0To1 getIntensity()
	{
		if (intensity == null)
		  intensity = new SEScalar0To1();
		return intensity;
	}
	
	public String toString() 
	{
		if (hasIntensity())
			return "Exercise" 
					+ "\n\tIntensity: " + getIntensity(); 		
		else
			return "Action not properly specified";
	}
}
