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

package mil.tatrc.physiology.datamodel.system.equipment.inhaler;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EnumOnOff;
import mil.tatrc.physiology.datamodel.bind.InhalerData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEInhaler implements SESystem
{
  protected EnumOnOff state;
  protected SEScalarMass meteredDose;
  protected SEScalarFraction nozzleLoss;
  protected SEScalarVolume spacerVolume;
  protected SESubstance substance;

  public SEInhaler()
  {
    clear();
  }
  
  public void clear()
  {
    state = null;
    meteredDose = null;
    nozzleLoss = null;
    spacerVolume = null;
    substance = null;
  }

  public void reset()
  {
    state = null;
    if (meteredDose != null)
      meteredDose.invalidate();
    if (nozzleLoss != null)
      nozzleLoss.invalidate();
    if (spacerVolume != null)
      spacerVolume.invalidate();
    substance = null;
  }

  public void copy(SEInhaler from)
  {
    reset();

    setState(from.getState());
    meteredDose = from.getMeteredDose();
    nozzleLoss = from.getNozzleLoss();
    spacerVolume = from.getSpacerVolume();
    substance = from.substance;
  }

  public boolean load(InhalerData in, SESubstanceManager subMgr)
  {
    if (in.getState() != null)
      setState(in.getState());
    if (in.getMeteredDose() != null)
      getMeteredDose().load(in.getMeteredDose());
    if (in.getNozzleLoss() != null)
      getNozzleLoss().load(in.getNozzleLoss());
    if (in.getSpacerVolume() != null)
      getSpacerVolume().load(in.getSpacerVolume());
    if (in.getSubstance() != null)
      setSubstance(subMgr.getSubstance(in.getSubstance()));

    return true;
  }

  public InhalerData unload()
  {
    InhalerData data = CDMSerializer.objFactory.createInhalerData();
    unload(data);
    return data;
  }

  protected void unload(InhalerData data)
  {
    if (hasState())
      data.setState(state);
    if (getMeteredDose() != null)
      data.setMeteredDose(meteredDose.unload());
    if (getNozzleLoss() != null)
      data.setNozzleLoss(nozzleLoss.unload());
    if (getSpacerVolume() != null)
      data.setSpacerVolume(spacerVolume.unload());
    if (hasSubstance())
      data.setSubstance(substance.getName());
  }

  public EnumOnOff getState()
  {
    return state;
  }
  public void setState(EnumOnOff state)
  {
    this.state = state;
  }
  public boolean hasState()
  {
    return state == null ? false : true;
  }

  public boolean hasMeteredDose()
  {
    return meteredDose == null ? false : meteredDose.isValid();
  }
  public SEScalarMass getMeteredDose()
  {
    if (meteredDose == null)
      meteredDose = new SEScalarMass();
    return meteredDose;
  }

  public boolean hasNozzleLoss()
  {
    return nozzleLoss == null ? false : nozzleLoss.isValid();
  }
  public SEScalarFraction getNozzleLoss()
  {
    if (nozzleLoss == null)
      nozzleLoss = new SEScalarFraction();
    return nozzleLoss;
  }

  public boolean hasSpacerVolume()
  {
    return spacerVolume == null ? false : spacerVolume.isValid();
  }
  public SEScalarVolume getSpacerVolume()
  {
    if (spacerVolume == null)
      spacerVolume = new SEScalarVolume();
    return spacerVolume;
  }

  public SESubstance getSubstance()
  {
    return substance;
  }
  public void setSubstance(SESubstance s)
  {
    this.substance = s;
  }
  public boolean hasSubstance() 
  {
    return substance == null ? false : true;
  }
}