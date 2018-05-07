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
import mil.tatrc.physiology.datamodel.bind.HepaticSystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEHepaticSystem extends SEPhysiologySystem implements SESystem
{
	protected SEScalarAmountPerTime ketoneProductionRate;
	protected SEScalarMassPerTime hepaticGluconeogenesisRate;
	
	public SEHepaticSystem()
	{
		ketoneProductionRate = null;
		hepaticGluconeogenesisRate = null;
		
	}

	public void reset()
	{		
		if (ketoneProductionRate != null)
      ketoneProductionRate.invalidate();
		if (hepaticGluconeogenesisRate != null)
			hepaticGluconeogenesisRate.invalidate();
	}

	public boolean load(HepaticSystemData in)
	{				
		
		if (in.getKetoneProductionRate() != null)
      getKetoneProductionRate().load(in.getKetoneProductionRate());
		if (in.getHepaticGluconeogenesisRate() != null)
      getHepaticGluconeogenesisRate().load(in.getHepaticGluconeogenesisRate());
		return true;
	}

	public HepaticSystemData unload()
	{
		HepaticSystemData data = CDMSerializer.objFactory.createHepaticSystemData();
		unload(data);
		return data;
	}

	protected void unload(HepaticSystemData data)
	{		
		if (hasKetoneProductionRate())
      data.setKetoneProductionRate(ketoneProductionRate.unload());
		if (hasHepaticGluconeogenesisRate())
      data.setHepaticGluconeogenesisRate(hepaticGluconeogenesisRate.unload());
	}
	
	public boolean hasKetoneProductionRate()
  {
    return ketoneProductionRate == null ? false : ketoneProductionRate.isValid();
  }
  public SEScalarAmountPerTime getKetoneProductionRate()
  {
    if (ketoneProductionRate == null)
      ketoneProductionRate = new SEScalarAmountPerTime();
    return ketoneProductionRate;
  }
  
  public boolean hasHepaticGluconeogenesisRate()
  {
    return hepaticGluconeogenesisRate == null ? false : hepaticGluconeogenesisRate.isValid();
  }
  public SEScalarMassPerTime getHepaticGluconeogenesisRate()
  {
    if (hepaticGluconeogenesisRate == null)
    	hepaticGluconeogenesisRate = new SEScalarMassPerTime();
    return hepaticGluconeogenesisRate;
  }

	
}
