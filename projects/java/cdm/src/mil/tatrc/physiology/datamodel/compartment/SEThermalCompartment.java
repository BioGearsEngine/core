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

package mil.tatrc.physiology.datamodel.compartment;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ThermalCompartmentData;
import mil.tatrc.physiology.datamodel.properties.SEScalarEnergy;
import mil.tatrc.physiology.datamodel.properties.SEScalarPower;
import mil.tatrc.physiology.datamodel.properties.SEScalarTemperature;

public class SEThermalCompartment extends SECompartment
{
  protected SEScalarPower heatTransferRateIn;
  protected SEScalarPower heatTransferRateOut;
  protected SEScalarTemperature temperature;
  protected SEScalarEnergy heat;

  public SEThermalCompartment()
  {
    heatTransferRateIn = null;
    heatTransferRateOut = null;
    temperature = null;
    heat = null;
  }

  public void reset()
  {
    super.reset();
    if (heatTransferRateIn != null)
      heatTransferRateIn.invalidate();
    if (heatTransferRateOut != null)
      heatTransferRateOut.invalidate();
    if (temperature != null)
      temperature.invalidate();
    if (heat != null)
      heat.invalidate();
  }

  public boolean load(ThermalCompartmentData in)
  {
    super.load(in);
    if (in.getHeatTransferRateIn() != null)
      getHeatTransferRateIn().load(in.getHeatTransferRateIn());
    if (in.getHeatTransferRateOut() != null)
      getHeatTransferRateOut().load(in.getHeatTransferRateOut());
    if (in.getTemperature() != null)
      getTemperature().load(in.getTemperature());
    if (in.getHeat() != null)
      getHeat().load(in.getHeat());

    return true;
  }

  public ThermalCompartmentData unload()
  {
    ThermalCompartmentData to = CDMSerializer.objFactory.createThermalCompartmentData();
    unload(to);
    return to;		
  }

  protected void unload(ThermalCompartmentData data)
  {
    super.unload(data);
    if (getHeatTransferRateIn() != null)
      data.setHeatTransferRateIn(heatTransferRateIn.unload());
    if (getHeatTransferRateOut() != null)
      data.setHeatTransferRateOut(heatTransferRateOut.unload());
    if (getTemperature() != null)
      data.setTemperature(temperature.unload());
    if (getHeat() != null)
      data.setHeat(heat.unload());
  }

  public boolean hasHeatTransferRateIn()
  {
    return heatTransferRateIn == null ? false : heatTransferRateIn.isValid();
  }
  public SEScalarPower getHeatTransferRateIn()
  {
    if (heatTransferRateIn == null)
      heatTransferRateIn = new SEScalarPower();
    return heatTransferRateIn;
  }
  
  public boolean hasHeatTransferRateOut()
  {
    return heatTransferRateOut == null ? false : heatTransferRateOut.isValid();
  }
  public SEScalarPower getHeatTransferRateOut()
  {
    if (heatTransferRateOut == null)
      heatTransferRateOut = new SEScalarPower();
    return heatTransferRateOut;
  }

  public boolean hasTemperature()
  {
    return temperature == null ? false : temperature.isValid();
  }
  public SEScalarTemperature getTemperature()
  {
    if (temperature == null)
      temperature = new SEScalarTemperature();
    return temperature;
  }

  public boolean hasHeat()
  {
    return heat == null ? false : heat.isValid();
  }
  public SEScalarEnergy getHeat()
  {
    if (heat == null)
      heat = new SEScalarEnergy();
    return heat;
  }
}
