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
import mil.tatrc.physiology.datamodel.bind.DrugSystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SEDrugSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarFraction    bronchodilationLevel;
  protected SEScalarFrequency   heartRateChange;
  protected SEScalarPressure    meanBloodPressureChange;
  protected SEScalarFraction    neuromuscularBlockLevel;
  protected SEScalarPressure    pulsePressureChange;
  protected SEPupillaryResponse pupillaryResponse;
  protected SEScalarFrequency   respirationRateChange;
  protected SEScalarFraction    sedationLevel;
  protected SEScalarVolume      tidalVolumeChange;
  protected SEScalarFraction    tubularPermeabilityChange;
  protected SEScalarFraction		centralNervousResponse;

  public SEDrugSystem()
  {
    bronchodilationLevel = null;
    heartRateChange = null;
    meanBloodPressureChange = null;
    neuromuscularBlockLevel = null;
    pulsePressureChange = null;
    pupillaryResponse = null;
    respirationRateChange = null;
    sedationLevel = null;
    tidalVolumeChange = null;
    tubularPermeabilityChange = null;
    centralNervousResponse = null;
  }

  public void reset()
  {
    if (bronchodilationLevel != null)
      bronchodilationLevel.invalidate();
    if (heartRateChange != null)
      heartRateChange.invalidate();
    if (meanBloodPressureChange != null)
      meanBloodPressureChange.invalidate();
    if (neuromuscularBlockLevel != null)
      neuromuscularBlockLevel.invalidate();
    if (pulsePressureChange != null)
      pulsePressureChange.invalidate();
    if (pupillaryResponse != null)
    	pupillaryResponse.reset();
    if (respirationRateChange != null)
      respirationRateChange.invalidate();
    if (sedationLevel != null)
      sedationLevel.invalidate();
    if (tidalVolumeChange != null)
      tidalVolumeChange.invalidate();
    if (tubularPermeabilityChange != null)
    	tubularPermeabilityChange.invalidate();
    if (centralNervousResponse != null)
    	centralNervousResponse.invalidate();
  }

  public boolean load(DrugSystemData in)
  {
    if (in.getBronchodilationLevel() != null)
      getBronchodilationLevel().load(in.getBronchodilationLevel());
    if (in.getHeartRateChange() != null)
      getHeartRateChange().load(in.getHeartRateChange());
    if (in.getMeanBloodPressureChange() != null)
      getMeanBloodPressureChange().load(in.getMeanBloodPressureChange());
    if (in.getNeuromuscularBlockLevel() != null)
      getNeuromuscularBlockLevel().load(in.getNeuromuscularBlockLevel());
    if (in.getPulsePressureChange() != null)
      getPulsePressureChange().load(in.getPulsePressureChange());
    if (in.getPupillaryResponse() != null)
    	getPupillaryResponse().load(in.getPupillaryResponse());
    if (in.getRespirationRateChange() != null)
      getRespirationRateChange().load(in.getRespirationRateChange());
    if (in.getSedationLevel() != null)
      getSedationLevel().load(in.getSedationLevel());
    if (in.getTidalVolumeChange() != null)
      getTidalVolumeChange().load(in.getTidalVolumeChange());
    if (in.getTubularPermeabilityChange() != null)
    	getTubularPermeabilityChange().load(in.getTubularPermeabilityChange());
    if(in.getCentralNervousResponse() != null)
    	getCentralNervousResponse().load(in.getCentralNervousResponse());

    return true;
  }

  public DrugSystemData unload()
  {
    DrugSystemData data = CDMSerializer.objFactory.createDrugSystemData();
    unload(data);
    return data;
  }

  protected void unload(DrugSystemData data)
  {
    if (bronchodilationLevel != null)
      data.setBronchodilationLevel(bronchodilationLevel.unload());
    if (heartRateChange != null)
      data.setHeartRateChange(heartRateChange.unload());
    if (meanBloodPressureChange != null)
      data.setMeanBloodPressureChange(meanBloodPressureChange.unload());
    if (neuromuscularBlockLevel != null)
      data.setNeuromuscularBlockLevel(neuromuscularBlockLevel.unload());
    if (pulsePressureChange != null)
      data.setPulsePressureChange(pulsePressureChange.unload());
    if (pupillaryResponse != null)
      data.setPupillaryResponse(pupillaryResponse.unload());
    if (respirationRateChange != null)
      data.setRespirationRateChange(respirationRateChange.unload());
    if (sedationLevel != null)
      data.setSedationLevel(sedationLevel.unload());
    if (tidalVolumeChange != null)
      data.setTidalVolumeChange(tidalVolumeChange.unload());
    if (tubularPermeabilityChange != null)
      data.setTubularPermeabilityChange(tubularPermeabilityChange.unload());
    if (centralNervousResponse != null)
    	data.setCentralNervousResponse(centralNervousResponse.unload());
  }
  
  public SEScalarFraction getBronchodilationLevel()
  {
    if (bronchodilationLevel == null)
      bronchodilationLevel = new SEScalarFraction();
    return bronchodilationLevel;
  }
  public boolean hasBronchodilationLevel()
  {
    return bronchodilationLevel == null ? false : bronchodilationLevel.isValid();
  }
  
  public SEScalarFrequency getHeartRateChange()
  {
    if (heartRateChange == null)
      heartRateChange = new SEScalarFrequency();
    return heartRateChange;
  }
  public boolean hasHeartRateChange()
  {
    return heartRateChange == null ? false : heartRateChange.isValid();
  }
  
  public SEScalarPressure getMeanBloodPressureChange()
  {
    if (meanBloodPressureChange == null)
      meanBloodPressureChange = new SEScalarPressure();
    return meanBloodPressureChange;
  }
  public boolean hasMeanBloodPressureChange()
  {
    return meanBloodPressureChange == null ? false : meanBloodPressureChange.isValid();
  }
  
  public SEScalarFraction getNeuromuscularBlockLevel()
  {
    if (neuromuscularBlockLevel == null)
      neuromuscularBlockLevel = new SEScalarFraction();
    return neuromuscularBlockLevel;
  }
  public boolean hasNeuromuscularBlockLevel()
  {
    return neuromuscularBlockLevel == null ? false : neuromuscularBlockLevel.isValid();
  }
  
  public SEScalarPressure getPulsePressureChange()
  {
    if (pulsePressureChange == null)
      pulsePressureChange = new SEScalarPressure();
    return pulsePressureChange;
  }
  public boolean hasPulsePressureChange()
  {
    return pulsePressureChange == null ? false : pulsePressureChange.isValid();
  }
  
  public SEPupillaryResponse getPupillaryResponse()
  {
    if (pupillaryResponse == null)
    	pupillaryResponse = new SEPupillaryResponse();
    return pupillaryResponse;
  }
  public boolean hasPupillaryResponse()
  {
    return pupillaryResponse != null;
  }
  
  public SEScalarFrequency getRespirationRateChange()
  {
    if (respirationRateChange == null)
      respirationRateChange = new SEScalarFrequency();
    return respirationRateChange;
  }
  public boolean hasRespirationRateChange()
  {
    return respirationRateChange == null ? false : respirationRateChange.isValid();
  }
  
  public SEScalarFraction getSedationLevel()
  {
    if (sedationLevel == null)
      sedationLevel = new SEScalarFraction();
    return sedationLevel;
  }
  public boolean hasSedationLevel()
  {
    return sedationLevel == null ? false : sedationLevel.isValid();
  }
  
  public SEScalarVolume getTidalVolumeChange()
  {
    if (tidalVolumeChange == null)
      tidalVolumeChange = new SEScalarVolume();
    return tidalVolumeChange;
  }
  public boolean hasTidalVolumeChange()
  {
    return tidalVolumeChange == null ? false : tidalVolumeChange.isValid();
  }
  
  public SEScalarFraction getTubularPermeabilityChange()
  {
    if (tubularPermeabilityChange == null)
    	tubularPermeabilityChange = new SEScalarFraction();
    return tubularPermeabilityChange;
  }
  public boolean hasTubularPermeabilityChange()
  {
    return tubularPermeabilityChange == null ? false : tubularPermeabilityChange.isValid();
  }
  
  public SEScalarFraction getCentralNervousResponse()
  {
  	if(centralNervousResponse == null)
  		centralNervousResponse = new SEScalarFraction();
  	return centralNervousResponse;
  }
  public boolean hasCentralNervousResponse()
  {
  	return centralNervousResponse == null ? false : centralNervousResponse.isValid();
  }
}
