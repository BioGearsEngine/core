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

package mil.tatrc.physiology.datamodel.substance.quantity;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.GasSubstanceQuantityData;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarPressure;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolume;
import mil.tatrc.physiology.datamodel.substance.SESubstance;

public class SEGasSubstanceQuantity extends SESubstanceQuantity
{
  protected SEScalarPressure      partialPressure;
  protected SEScalarVolume        volume;
  protected SEScalarFraction      volumeFraction;

  public SEGasSubstanceQuantity(SESubstance s)
  {
  	super(s);
    partialPressure = null;
    volume = null;
    volumeFraction = null;
  }

  public void reset()
  {
    super.reset();
   if (partialPressure != null)
      partialPressure.invalidate();
    if (volume != null)
      volume.invalidate();  
    if (volumeFraction != null)
      volumeFraction.invalidate();  
  }

  public boolean load(GasSubstanceQuantityData in)
  {
    super.load(in);
    if (in.getPartialPressure() != null) 
      getPartialPressure().load(in.getPartialPressure()); 
    if (in.getVolume() != null) 
      getVolume().load(in.getVolume()); 
    if (in.getVolumeFraction() != null) 
      getVolumeFraction().load(in.getVolumeFraction()); 

    return true;
  }

  public GasSubstanceQuantityData unload()
  {
    GasSubstanceQuantityData data = CDMSerializer.objFactory.createGasSubstanceQuantityData();
    unload(data);
    return data;
  }

  protected void unload(GasSubstanceQuantityData data)
  {
    super.unload(data);
    if (hasPartialPressure())
      data.setPartialPressure(partialPressure.unload());
    if (hasVolume())
      data.setVolume(volume.unload());
    if (hasVolumeFraction())
      data.setVolumeFraction(volumeFraction.unload());    
  }	

  public boolean hasPartialPressure()
  {
    return partialPressure == null ? false : partialPressure.isValid();
  }
  public SEScalarPressure getPartialPressure()
  {
    if (partialPressure == null)
      partialPressure = new SEScalarPressure();
    return partialPressure;
  }

  public boolean hasVolume()
  {
    return volume == null ? false : volume.isValid();
  }
  public SEScalarVolume getVolume()
  {
    if (volume == null)
      volume = new SEScalarVolume();
    return volume;
  }
  
  public boolean hasVolumeFraction()
  {
    return volumeFraction == null ? false : volumeFraction.isValid();
  }
  public SEScalarFraction getVolumeFraction()
  {
    if (volumeFraction == null)
      volumeFraction = new SEScalarFraction();
    return volumeFraction;
  }
}
