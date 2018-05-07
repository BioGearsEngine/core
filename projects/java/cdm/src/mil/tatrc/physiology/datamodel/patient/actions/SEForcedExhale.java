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
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class SEForcedExhale extends SEConsciousRespirationCommand
{
	protected SEScalar0To1 expiratoryReserveVolumeFraction;
	protected SEScalarTime period;
	
	public SEForcedExhale()
	{
		expiratoryReserveVolumeFraction = null;
		period = null;
	}
	
	public void copy(SEForcedExhale other)
	{
		if(this==other)
			return;
		
		expiratoryReserveVolumeFraction = other.expiratoryReserveVolumeFraction;
		period = other.period;
	}
	
	public SEConsciousRespirationCommand getCopy()
	{
		SEConsciousRespirationCommand command = new SEForcedExhale();
		command.copy(this);
		
		return command;
	}
	
	public void reset()
	{
		super.reset();
		
		if (expiratoryReserveVolumeFraction != null)
			expiratoryReserveVolumeFraction.invalidate();
		if (period != null)
			period.invalidate();
	}
	
	public boolean isValid()
	{
		return hasExpiratoryReserveVolumeFraction() && hasPeriod();
	}
	
	public boolean load(ForcedExhaleData in)
	{
		super.load(in);
		
		getExpiratoryReserveVolumeFraction().load(in.getExpiratoryReserveVolumeFraction());
		getPeriod().load(in.getPeriod());
		
		return isValid();
	}
	
	public ForcedExhaleData unload()
	{
		ForcedExhaleData data = CDMSerializer.objFactory.createForcedExhaleData();
		unload(data);
		
		return data;
	}
	
	protected void unload(ForcedExhaleData data)
	{
		super.unload(data);
		
		if (hasExpiratoryReserveVolumeFraction())
			data.setExpiratoryReserveVolumeFraction(expiratoryReserveVolumeFraction.unload());
		if (hasPeriod())
			data.setPeriod(period.unload());
	}
	
	public boolean hasExpiratoryReserveVolumeFraction()
	{
		return expiratoryReserveVolumeFraction != null;
	}
	public SEScalar0To1 getExpiratoryReserveVolumeFraction()
	{
		if (expiratoryReserveVolumeFraction == null)
			expiratoryReserveVolumeFraction = new SEScalar0To1();
		return expiratoryReserveVolumeFraction;
	}
	
	public boolean hasPeriod()
	{
		return period != null;
	}
	public SEScalarTime getPeriod()
	{
		if (period == null)
			period = new SEScalarTime();
		return period;
	}
	
	public String toString()
	{
		if (expiratoryReserveVolumeFraction != null &&
			period != null)
			return "Forced Exhale" 
					+ "\n\tExpiratory Reserve Volume Fraction: " + getExpiratoryReserveVolumeFraction()
					+ "\n\tPeriod: " + getPeriod();
		else
			return "Action not specified properly";
	}
}
