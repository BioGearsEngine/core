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
package mil.tatrc.physiology.datamodel.patient.assessments;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.PulmonaryFunctionTestData;
import mil.tatrc.physiology.datamodel.properties.*;

public class SEPulmonaryFunctionTest extends SEPatientAssessment
{
  protected SEScalarVolume         expiratoryReserveVolume;
  protected SEScalarVolume         forcedVitalCapacity;
  protected SEScalarVolume         forcedExpiratoryVolume;
  protected SEScalarVolumePerTime  forcedExpiratoryFlow;
  protected SEScalarVolume         functionalResidualCapacity;
  protected SEScalarVolume         inspiratoryCapacity;
  protected SEScalarVolume         inspiratoryReserveVolume;
  protected SEScalarVolume         maximumVoluntaryVentilation;  
  protected SEScalarVolumePerTime  peakExpiratoryFlow;
  protected SEScalarVolume         residualVolume;
  protected SEScalarVolume         slowVitalCapacity;
  protected SEScalarVolume         totalLungCapacity;
  protected SEScalarVolume         vitalCapacity;
  protected SEFunctionVolumeVsTime lungVolumePlot;
  
  public void clear()
  {
    super.clear();
    this.expiratoryReserveVolume = null;
    this.forcedVitalCapacity = null;
    this.forcedExpiratoryVolume = null;
    this.forcedExpiratoryFlow = null;
    this.functionalResidualCapacity = null;
    this.inspiratoryCapacity = null;
    this.inspiratoryReserveVolume = null;
    this.maximumVoluntaryVentilation = null;
    this.peakExpiratoryFlow = null;
    this.residualVolume = null;
    this.slowVitalCapacity = null;
    this.totalLungCapacity = null;
    this.vitalCapacity = null;
    this.lungVolumePlot = null;
  }
  
  public void reset()
  {
    super.reset();
    if(this.expiratoryReserveVolume!=null)
      this.expiratoryReserveVolume.invalidate();
    if(this.forcedVitalCapacity!=null)
      this.forcedVitalCapacity.invalidate();
    if(this.forcedExpiratoryVolume!=null)
      this.forcedExpiratoryVolume.invalidate();
    if(this.forcedExpiratoryFlow!=null)
      this.forcedExpiratoryFlow.invalidate();
    if(this.functionalResidualCapacity!=null)
      this.functionalResidualCapacity.invalidate();
    if(this.inspiratoryCapacity!=null)
      this.inspiratoryCapacity.invalidate();
    if(this.inspiratoryReserveVolume!=null)
      this.inspiratoryReserveVolume.invalidate();
    if(this.maximumVoluntaryVentilation!=null)
      this.maximumVoluntaryVentilation.invalidate();
    if(this.peakExpiratoryFlow!=null)
      this.peakExpiratoryFlow.invalidate();
    if(this.residualVolume!=null)
      this.residualVolume.invalidate();
    if(this.slowVitalCapacity!=null)
      this.slowVitalCapacity.invalidate();
    if(this.totalLungCapacity!=null)
      this.totalLungCapacity.invalidate();
    if(this.vitalCapacity!=null)
      this.vitalCapacity.invalidate();
    if(this.lungVolumePlot!=null)
      this.lungVolumePlot.invalidate();
  }
  
  public boolean load(PulmonaryFunctionTestData in)
  {
    super.load(in);
    if(in.getExpiratoryReserveVolume()!=null)
      this.getExpiratoryReserveVolume().load(in.getExpiratoryReserveVolume());
    if(in.getForcedVitalCapacity()!=null)
      this.getForcedVitalCapacity().load(in.getForcedVitalCapacity());
    if(in.getForcedExpiratoryVolume()!=null)
      this.getForcedExpiratoryVolume().load(in.getForcedExpiratoryVolume());
    if(in.getForcedExpiratoryFlow()!=null)
      this.getForcedExpiratoryFlow().load(in.getForcedExpiratoryFlow());
    if(in.getFunctionalResidualCapacity()!=null)
      this.getFunctionalResidualCapacity().load(in.getFunctionalResidualCapacity());
    if(in.getInspiratoryCapacity()!=null)
      this.getInspiratoryCapacity().load(in.getInspiratoryCapacity());
    if(in.getInspiratoryReserveVolume()!=null)
      this.getInspiratoryReserveVolume().load(in.getInspiratoryReserveVolume());
    if(in.getMaximumVoluntaryVentilation()!=null)
      this.getMaximumVoluntaryVentilation().load(in.getMaximumVoluntaryVentilation());
    if(in.getPeakExpiratoryFlow()!=null)
      this.getPeakExpiratoryFlow().load(in.getPeakExpiratoryFlow());
    if(in.getResidualVolume()!=null)
      this.getResidualVolume().load(in.getResidualVolume());
    if(in.getSlowVitalCapacity()!=null)
      this.getSlowVitalCapacity().load(in.getSlowVitalCapacity());
    if(in.getTotalLungCapacity()!=null)
      this.getTotalLungCapacity().load(in.getTotalLungCapacity());
    if(in.getVitalCapacity()!=null)
      this.getVitalCapacity().load(in.getVitalCapacity());
    if(in.getLungVolumePlot()!=null)
      this.getLungVolumePlot().load(in.getLungVolumePlot());
    return true;
  }
  
  public PulmonaryFunctionTestData unload()
  {
    PulmonaryFunctionTestData data = CDMSerializer.objFactory.createPulmonaryFunctionTestData();
    unload(data);
    return data;
  }
  
  protected void unload(PulmonaryFunctionTestData data)
  {
    super.unload(data);
    if (expiratoryReserveVolume != null)
      data.setExpiratoryReserveVolume(expiratoryReserveVolume.unload());
    if (forcedVitalCapacity != null)
      data.setForcedVitalCapacity(forcedVitalCapacity.unload());
    if (forcedExpiratoryVolume != null)
      data.setForcedExpiratoryVolume(forcedExpiratoryVolume.unload());
    if (forcedExpiratoryFlow != null)
      data.setForcedExpiratoryFlow(forcedExpiratoryFlow.unload());
    if (functionalResidualCapacity != null)
      data.setFunctionalResidualCapacity(functionalResidualCapacity.unload());
    if (inspiratoryCapacity != null)
      data.setInspiratoryCapacity(inspiratoryCapacity.unload());
    if (inspiratoryReserveVolume != null)
      data.setInspiratoryReserveVolume(inspiratoryReserveVolume.unload());
    if (maximumVoluntaryVentilation != null)
      data.setMaximumVoluntaryVentilation(maximumVoluntaryVentilation.unload());
    if (peakExpiratoryFlow != null)
      data.setPeakExpiratoryFlow(peakExpiratoryFlow.unload());
    if (residualVolume != null)
      data.setResidualVolume(residualVolume.unload());
    if (slowVitalCapacity != null)
      data.setSlowVitalCapacity(slowVitalCapacity.unload());
    if (totalLungCapacity != null)
      data.setTotalLungCapacity(totalLungCapacity.unload());
    if (vitalCapacity != null)
      data.setVitalCapacity(vitalCapacity.unload());
    if (lungVolumePlot != null)
      data.setLungVolumePlot(lungVolumePlot.unload());
  }
  
  public boolean hasExpiratoryReserveVolume()
  {
    return expiratoryReserveVolume == null ? false : expiratoryReserveVolume.isValid();
  }
  public SEScalarVolume getExpiratoryReserveVolume()
  {
    if (expiratoryReserveVolume == null)
      expiratoryReserveVolume = new SEScalarVolume();
    return expiratoryReserveVolume;
  }
  
  public boolean hasForcedVitalCapacity()
  {
    return forcedVitalCapacity == null ? false : forcedVitalCapacity.isValid();
  }
  public SEScalarVolume getForcedVitalCapacity()
  {
    if (forcedVitalCapacity == null)
      forcedVitalCapacity = new SEScalarVolume();
    return forcedVitalCapacity;
  }
  
  public boolean hasForcedExpiratoryVolume()
  {
    return forcedExpiratoryVolume == null ? false : forcedExpiratoryVolume.isValid();
  }
  public SEScalarVolume getForcedExpiratoryVolume()
  {
    if (forcedExpiratoryVolume == null)
      forcedExpiratoryVolume = new SEScalarVolume();
    return forcedExpiratoryVolume;
  }
  
  public boolean hasForcedExpiratoryFlow()
  {
    return forcedExpiratoryFlow == null ? false : forcedExpiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getForcedExpiratoryFlow()
  {
    if (forcedExpiratoryFlow == null)
      forcedExpiratoryFlow = new SEScalarVolumePerTime();
    return forcedExpiratoryFlow;
  }
  
  public boolean hasFunctionalResidualCapacity()
  {
    return functionalResidualCapacity == null ? false : functionalResidualCapacity.isValid();
  }
  public SEScalarVolume getFunctionalResidualCapacity()
  {
    if (functionalResidualCapacity == null)
      functionalResidualCapacity = new SEScalarVolume();
    return functionalResidualCapacity;
  }
  
  public boolean hasInspiratoryCapacity()
  {
    return inspiratoryCapacity == null ? false : inspiratoryCapacity.isValid();
  }
  public SEScalarVolume getInspiratoryCapacity()
  {
    if (inspiratoryCapacity == null)
      inspiratoryCapacity = new SEScalarVolume();
    return inspiratoryCapacity;
  }
  
  public boolean hasInspiratoryReserveVolume()
  {
    return inspiratoryReserveVolume == null ? false : inspiratoryReserveVolume.isValid();
  }
  public SEScalarVolume getInspiratoryReserveVolume()
  {
    if (inspiratoryReserveVolume == null)
      inspiratoryReserveVolume = new SEScalarVolume();
    return inspiratoryReserveVolume;
  }
  
  public boolean hasMaximumVoluntaryVentilation()
  {
    return maximumVoluntaryVentilation == null ? false : maximumVoluntaryVentilation.isValid();
  }
  public SEScalarVolume getMaximumVoluntaryVentilation()
  {
    if (maximumVoluntaryVentilation == null)
      maximumVoluntaryVentilation = new SEScalarVolume();
    return maximumVoluntaryVentilation;
  }
  
  public boolean hasPeakExpiratoryFlow()
  {
    return peakExpiratoryFlow == null ? false : peakExpiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getPeakExpiratoryFlow()
  {
    if (peakExpiratoryFlow == null)
      peakExpiratoryFlow = new SEScalarVolumePerTime();
    return peakExpiratoryFlow;
  }
  
  public boolean hasResidualVolume()
  {
    return residualVolume == null ? false : residualVolume.isValid();
  }
  public SEScalarVolume getResidualVolume()
  {
    if (residualVolume == null)
      residualVolume = new SEScalarVolume();
    return residualVolume;
  }
  
  public boolean hasSlowVitalCapacity()
  {
    return slowVitalCapacity == null ? false : slowVitalCapacity.isValid();
  }
  public SEScalarVolume getSlowVitalCapacity()
  {
    if (slowVitalCapacity == null)
      slowVitalCapacity = new SEScalarVolume();
    return slowVitalCapacity;
  }
  
  public boolean hasTotalLungCapacity()
  {
    return totalLungCapacity == null ? false : totalLungCapacity.isValid();
  }
  public SEScalarVolume getTotalLungCapacity()
  {
    if (totalLungCapacity == null)
      totalLungCapacity = new SEScalarVolume();
    return totalLungCapacity;
  }
  
  public boolean hasVitalCapacity()
  {
    return vitalCapacity == null ? false : vitalCapacity.isValid();
  }
  public SEScalarVolume getVitalCapacity()
  {
    if (vitalCapacity == null)
      vitalCapacity = new SEScalarVolume();
    return vitalCapacity;
  }
  
  public boolean hasLungVolumePlot()
  {
    return lungVolumePlot == null ? false : lungVolumePlot.isValid();
  }
  public SEFunctionVolumeVsTime getLungVolumePlot()
  {
    if (lungVolumePlot == null)
      lungVolumePlot = new SEFunctionVolumeVsTime();
    return lungVolumePlot;
  }

}
