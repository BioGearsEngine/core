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

package mil.tatrc.physiology.datamodel.substance;

import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.utilities.StringUtils;

public class SESubstanceAerosolization
{
	protected SEScalarNeg1To1              bronchioleModifier;
	protected SEScalar0To1                 inflammationCoefficient;
	protected SEHistogramFractionVsLength  particulateSizeDistribution;
	
	public SESubstanceAerosolization()
	{
		
	}
	
	public void reset()
	{
    if(this.bronchioleModifier!=null)
      this.bronchioleModifier.invalidate();
    if(this.inflammationCoefficient!=null)
      this.inflammationCoefficient.invalidate();
    if(this.particulateSizeDistribution!=null)
      this.particulateSizeDistribution.invalidate();
	}
	
	public boolean isValid()
	{
	  if(!hasInflammationCoefficient() || !hasParticulateSizeDistribution() || !hasBronchioleModifier())
      return false;
	  return true;
	}
	
	public boolean load(SubstanceAerosolizationData data)
	{
		this.reset();

		if(data.getBronchioleModifier()!=null)
		  this.getBronchioleModifier().load(data.getBronchioleModifier());
		if(data.getInflammationCoefficient()!=null)
		  this.getInflammationCoefficient().load(data.getInflammationCoefficient());
		if(data.getParticulateSizeDistribution()!=null)
		  this.getParticulateSizeDistribution().load(data.getParticulateSizeDistribution());
		
		
		return true;
	}
	
	public SubstanceAerosolizationData unload()
	{
	  if(!isValid())
	    return null;
		SubstanceAerosolizationData to = CDMSerializer.objFactory.createSubstanceAerosolizationData();
		unload(to);
		return to;
	}
	
	protected void unload(SubstanceAerosolizationData to)
	{
	  if(hasBronchioleModifier())
	    to.setBronchioleModifier(this.bronchioleModifier.unload());
	  if(hasInflammationCoefficient())
	    to.setInflammationCoefficient(this.inflammationCoefficient.unload());
	  if(hasParticulateSizeDistribution())
	    to.setParticulateSizeDistribution(this.particulateSizeDistribution.unload());
	}
	
  public SEScalarNeg1To1 getBronchioleModifier() 
	{ 
		if(this.bronchioleModifier==null)
			this.bronchioleModifier=new SEScalarNeg1To1();
		return this.bronchioleModifier;
	}
	public boolean hasBronchioleModifier() {return this.bronchioleModifier==null?false:this.bronchioleModifier.isValid();}

  public SEScalar0To1 getInflammationCoefficient() 
	{ 
		if(this.inflammationCoefficient==null)
			this.inflammationCoefficient=new SEScalar0To1();
		return this.inflammationCoefficient;
	}
	public boolean hasInflammationCoefficient() {return this.inflammationCoefficient==null?false:this.inflammationCoefficient.isValid();}

	
	public SEHistogramFractionVsLength getParticulateSizeDistribution() 
	{ 
		if(this.particulateSizeDistribution==null)
			this.particulateSizeDistribution=new SEHistogramFractionVsLength();
		return this.particulateSizeDistribution;
	}
	public boolean hasParticulateSizeDistribution() {return this.particulateSizeDistribution==null?false:this.particulateSizeDistribution.isValid();}

}
