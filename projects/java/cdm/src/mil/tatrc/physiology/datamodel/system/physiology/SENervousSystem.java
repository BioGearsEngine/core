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
import mil.tatrc.physiology.datamodel.bind.NervousSystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SENervousSystem extends SEPhysiologySystem implements SESystem
{
	protected SEScalar					  baroreceptorHeartRateScale;
	protected SEScalar					  baroreceptorHeartElastanceScale;
	protected SEScalar					  baroreceptorResistanceScale;
	protected SEScalar					  baroreceptorComplianceScale;
	protected SEPupillaryResponse leftPupillaryResponse;
	protected SEPupillaryResponse rightPupillaryResponse;
	protected SEScalar					  chemoreceptorHeartRateScale;
	protected SEScalar					  chemoreceptorHeartElastanceScale;

	public SENervousSystem()
	{
		baroreceptorHeartRateScale=null;
		baroreceptorHeartElastanceScale=null;
		baroreceptorResistanceScale=null;
		baroreceptorComplianceScale=null;
		leftPupillaryResponse=null;
		rightPupillaryResponse=null;
		chemoreceptorHeartRateScale=null;
		chemoreceptorHeartElastanceScale=null;
	}

	public void reset()
	{		
		if(baroreceptorHeartRateScale!=null)
			baroreceptorHeartRateScale.invalidate();
		if(baroreceptorHeartElastanceScale!=null)
			baroreceptorHeartElastanceScale.invalidate();
		if(baroreceptorResistanceScale!=null)
			baroreceptorResistanceScale.invalidate();
		if(baroreceptorComplianceScale!=null)
			baroreceptorComplianceScale.invalidate();
		if(leftPupillaryResponse != null)
			this.leftPupillaryResponse.reset();
		if(rightPupillaryResponse != null)
			this.rightPupillaryResponse.reset();
		if(chemoreceptorHeartRateScale!=null)
			chemoreceptorHeartRateScale.invalidate();
		if(chemoreceptorHeartElastanceScale!=null)
			chemoreceptorHeartElastanceScale.invalidate();
	}

	public boolean load(NervousSystemData in)
	{		
		if (in.getBaroreceptorHeartRateScale() != null)
			getBaroreceptorHeartRateScale().load(in.getBaroreceptorHeartRateScale());
		if (in.getBaroreceptorHeartElastanceScale() != null)
			getBaroreceptorHeartElastanceScale().load(in.getBaroreceptorHeartElastanceScale());
		if (in.getBaroreceptorResistanceScale() != null)
			getBaroreceptorResistanceScale().load(in.getBaroreceptorResistanceScale());
		if (in.getBaroreceptorComplianceScale() != null)
			getBaroreceptorComplianceScale().load(in.getBaroreceptorComplianceScale());
		
		if (in.getChemoreceptorHeartRateScale() != null)
			getChemoreceptorHeartRateScale().load(in.getChemoreceptorHeartRateScale());
		if (in.getChemoreceptorHeartElastanceScale() != null)
			getChemoreceptorHeartElastanceScale().load(in.getChemoreceptorHeartElastanceScale());

		if(in.getLeftEyePupillaryResponse()!=null)
			this.getLeftEyePupillaryResponse().load(in.getLeftEyePupillaryResponse());
		if(in.getRightEyePupillaryResponse()!=null)
			this.getRightEyePupillaryResponse().load(in.getRightEyePupillaryResponse());

		return true;
	}

	public NervousSystemData unload()
	{
		NervousSystemData data = CDMSerializer.objFactory.createNervousSystemData();
		unload(data);
		return data;
	}

	protected void unload(NervousSystemData data)
	{		
		if (getBaroreceptorHeartRateScale() != null)
			data.setBaroreceptorHeartRateScale(baroreceptorHeartRateScale.unload());
		if (getBaroreceptorHeartElastanceScale() != null)
			data.setBaroreceptorHeartElastanceScale(baroreceptorHeartElastanceScale.unload());
		if (getBaroreceptorResistanceScale() != null)
			data.setBaroreceptorResistanceScale(baroreceptorResistanceScale.unload());
		if (getBaroreceptorComplianceScale() != null)
			data.setBaroreceptorComplianceScale(baroreceptorComplianceScale.unload());
		
		if (getChemoreceptorHeartRateScale() != null)
			data.setChemoreceptorHeartRateScale(chemoreceptorHeartRateScale.unload());
		if (getChemoreceptorHeartElastanceScale() != null)
			data.setChemoreceptorHeartElastanceScale(chemoreceptorHeartElastanceScale.unload());

		if(hasLeftEyePupillaryResponse())
			data.setLeftEyePupillaryResponse(this.leftPupillaryResponse.unload());
		if(hasRightEyePupillaryResponse())
			data.setRightEyePupillaryResponse(this.rightPupillaryResponse.unload());
	}

	public boolean hasBaroreceptorHeartRateScale()
	{
		return baroreceptorHeartRateScale == null ? false : baroreceptorHeartRateScale.isValid();
	}
	public SEScalar getBaroreceptorHeartRateScale()
	{
		if (baroreceptorHeartRateScale == null)
			baroreceptorHeartRateScale = new SEScalar();
		return baroreceptorHeartRateScale;
	}

	public boolean hasBaroreceptorHeartElastanceScale()
	{
		return baroreceptorHeartElastanceScale == null ? false : baroreceptorHeartElastanceScale.isValid();
	}
	public SEScalar getBaroreceptorHeartElastanceScale()
	{
		if (baroreceptorHeartElastanceScale == null)
			baroreceptorHeartElastanceScale = new SEScalar();
		return baroreceptorHeartElastanceScale;
	}

	public boolean hasBaroreceptorResistanceScale()
	{
		return baroreceptorResistanceScale == null ? false : baroreceptorResistanceScale.isValid();
	}
	public SEScalar getBaroreceptorResistanceScale()
	{
		if (baroreceptorResistanceScale == null)
			baroreceptorResistanceScale = new SEScalar();
		return baroreceptorResistanceScale;
	}

	public boolean hasBaroreceptorComplianceScale()
	{
		return baroreceptorComplianceScale == null ? false : baroreceptorComplianceScale.isValid();
	}
	public SEScalar getBaroreceptorComplianceScale()
	{
		if (baroreceptorComplianceScale == null)
			baroreceptorComplianceScale = new SEScalar();
		return baroreceptorComplianceScale;
	}

	public boolean hasChemoreceptorHeartRateScale()
	{
		return chemoreceptorHeartRateScale == null ? false : chemoreceptorHeartRateScale.isValid();
	}
	public SEScalar getChemoreceptorHeartRateScale()
	{
		if (chemoreceptorHeartRateScale == null)
			chemoreceptorHeartRateScale = new SEScalar();
		return chemoreceptorHeartRateScale;
	}

	public boolean hasChemoreceptorHeartElastanceScale()
	{
		return chemoreceptorHeartElastanceScale == null ? false : chemoreceptorHeartElastanceScale.isValid();
	}
	public SEScalar getChemoreceptorHeartElastanceScale()
	{
		if (chemoreceptorHeartElastanceScale == null)
			chemoreceptorHeartElastanceScale = new SEScalar();
		return chemoreceptorHeartElastanceScale;
	}

	public SEPupillaryResponse getLeftEyePupillaryResponse() 
	{ 
		if(this.leftPupillaryResponse==null)
			this.leftPupillaryResponse=new SEPupillaryResponse();
		return this.leftPupillaryResponse;
	}	
	public boolean  hasLeftEyePupillaryResponse() {return this.leftPupillaryResponse!=null;}

	public SEPupillaryResponse getRightEyePupillaryResponse() 
	{ 
		if(this.rightPupillaryResponse==null)
			this.rightPupillaryResponse=new SEPupillaryResponse();
		return this.rightPupillaryResponse;
	}	
	public boolean  hasRightEyePupillaryResponse() {return this.rightPupillaryResponse!=null;}


}
