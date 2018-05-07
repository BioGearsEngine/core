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

package mil.tatrc.physiology.datamodel.system.physiology;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.GastrointestinalSystemData;
import mil.tatrc.physiology.datamodel.patient.nutrition.SENutrition;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEGastrointestinalSystem extends SEPhysiologySystem implements SESystem
{
	protected SEScalarVolumePerTime chymeAbsorptionRate;
	protected SENutrition			      stomachContents;

	public SEGastrointestinalSystem()
	{
		chymeAbsorptionRate = null;
		stomachContents = null;
	}

	public void reset()
	{		
		if (chymeAbsorptionRate != null)
			chymeAbsorptionRate.invalidate();
		if (stomachContents != null)
			stomachContents.reset();
	}

	public boolean load(GastrointestinalSystemData in)
	{		
		if (in.getChymeAbsorptionRate() != null)
			getChymeAbsorptionRate().load(in.getChymeAbsorptionRate());
		if (in.getStomachContents() != null)
			getStomachContents().load(in.getStomachContents());

		return true;
	}

	public GastrointestinalSystemData unload()
	{
		GastrointestinalSystemData data = CDMSerializer.objFactory.createGastrointestinalSystemData();
		unload(data);
		return data;
	}

	protected void unload(GastrointestinalSystemData data)
	{		
		if (getChymeAbsorptionRate() != null)
			data.setChymeAbsorptionRate(chymeAbsorptionRate.unload());
		if (getStomachContents() != null)
			data.setStomachContents(stomachContents.unload());
	}

	public boolean hasChymeAbsorptionRate()
	{
		return chymeAbsorptionRate == null ? false : chymeAbsorptionRate.isValid();
	}
	public SEScalarVolumePerTime getChymeAbsorptionRate()
	{
		if (chymeAbsorptionRate == null)
			chymeAbsorptionRate = new SEScalarVolumePerTime();
		return chymeAbsorptionRate;
	}

	public boolean hasStomachContents()
	{
		return stomachContents == null ? false : true;
	}
	public SENutrition getStomachContents()
	{
		if (stomachContents == null)
			stomachContents = new SENutrition();
		return stomachContents;
	}
}
