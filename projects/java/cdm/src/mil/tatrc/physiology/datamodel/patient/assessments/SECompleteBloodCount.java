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
import mil.tatrc.physiology.datamodel.bind.CompleteBloodCountData;
import mil.tatrc.physiology.datamodel.properties.*;

public class SECompleteBloodCount extends SEPatientAssessment
{
  protected SEScalarFraction        hematocrit;
  protected SEScalarMassPerVolume   hemoglobin;
  protected SEScalarAmountPerVolume plateletCount;
  protected SEScalarMassPerAmount   meanCorpuscularHemoglobin;
  protected SEScalarMassPerVolume   meanCorpuscularHemoglobinConcentration;
  protected SEScalarVolume          meanCorpuscularVolume;
  protected SEScalarAmountPerVolume redBloodCellCount;
  protected SEScalarAmountPerVolume whiteBloodCellCount;
  
  public void clear()
  {
    super.clear();
    this.hematocrit = null;
    this.hemoglobin = null;
    this.plateletCount = null;
    this.meanCorpuscularHemoglobin = null;
    this.meanCorpuscularHemoglobinConcentration = null;
    this.meanCorpuscularVolume = null;
    this.redBloodCellCount = null;
    this.whiteBloodCellCount = null;
  }
  
  public void reset()
  {
    super.reset();
    if(this.hematocrit!=null)
      this.hematocrit.invalidate();
    if(this.hemoglobin!=null)
      this.hemoglobin.invalidate();
    if(this.plateletCount!=null)
      this.plateletCount.invalidate();
    if(this.meanCorpuscularHemoglobin!=null)
      this.meanCorpuscularHemoglobin.invalidate();
    if(this.meanCorpuscularHemoglobinConcentration!=null)
      this.meanCorpuscularHemoglobinConcentration.invalidate();
    if(this.meanCorpuscularVolume!=null)
      this.meanCorpuscularVolume.invalidate();
    if(this.redBloodCellCount!=null)
      this.redBloodCellCount.invalidate();
    if(this.whiteBloodCellCount!=null)
      this.whiteBloodCellCount.invalidate();
  }
  
  public boolean load(CompleteBloodCountData in)
  {
    super.load(in);
    if(in.getHematocrit()!=null)
      this.getHematocrit().load(in.getHematocrit());
    if(in.getHemoglobin()!=null)
      this.getHemoglobin().load(in.getHemoglobin());
    if(in.getPlateletCount()!=null)
      this.getPlateletCount().load(in.getPlateletCount());
    if(in.getMeanCorpuscularHemoglobin()!=null)
      this.getMeanCorpuscularHemoglobin().load(in.getMeanCorpuscularHemoglobin());
    if(in.getMeanCorpuscularHemoglobinConcentration()!=null)
      this.getMeanCorpuscularHemoglobinConcentration().load(in.getMeanCorpuscularHemoglobinConcentration());
    if(in.getMeanCorpuscularVolume()!=null)
      this.getMeanCorpuscularVolume().load(in.getMeanCorpuscularVolume());
    if(in.getRedBloodCellCount()!=null)
      this.getRedBloodCellCount().load(in.getRedBloodCellCount());
    if(in.getWhiteBloodCellCount()!=null)
      this.getWhiteBloodCellCount().load(in.getWhiteBloodCellCount());
    return true;
  }
  
  public CompleteBloodCountData unload()
  {
    CompleteBloodCountData data = CDMSerializer.objFactory.createCompleteBloodCountData();
    unload(data);
    return data;
  }
  
  protected void unload(CompleteBloodCountData data)
  {
    super.unload(data);
    if (hematocrit != null)
      data.setHematocrit(hematocrit.unload());
    if (hemoglobin != null)
      data.setHemoglobin(hemoglobin.unload());
    if (plateletCount != null)
      data.setPlateletCount(plateletCount.unload());
    if (meanCorpuscularHemoglobin != null)
      data.setMeanCorpuscularHemoglobin(meanCorpuscularHemoglobin.unload());
    if (meanCorpuscularHemoglobinConcentration != null)
      data.setMeanCorpuscularHemoglobinConcentration(meanCorpuscularHemoglobinConcentration.unload());
    if (meanCorpuscularVolume != null)
      data.setMeanCorpuscularVolume(meanCorpuscularVolume.unload());
    if (redBloodCellCount != null)
      data.setRedBloodCellCount(redBloodCellCount.unload());
    if (whiteBloodCellCount != null)
      data.setWhiteBloodCellCount(whiteBloodCellCount.unload());
  }
  
  public boolean hasHematocrit()
  {
    return hematocrit == null ? false : hematocrit.isValid();
  }
  public SEScalarFraction getHematocrit()
  {
    if (hematocrit == null)
      hematocrit = new SEScalarFraction();
    return hematocrit;
  }
  
  public boolean hasHemoglobin()
  {
    return hemoglobin == null ? false : hemoglobin.isValid();
  }
  public SEScalarMassPerVolume getHemoglobin()
  {
    if (hemoglobin == null)
      hemoglobin = new SEScalarMassPerVolume();
    return hemoglobin;
  }
  
  public boolean hasPlateletCount()
  {
    return plateletCount == null ? false : plateletCount.isValid();
  }
  public SEScalarAmountPerVolume getPlateletCount()
  {
    if (plateletCount == null)
      plateletCount = new SEScalarAmountPerVolume();
    return plateletCount;
  }
  
  public boolean hasMeanCorpuscularHemoglobin()
  {
    return meanCorpuscularHemoglobin == null ? false : meanCorpuscularHemoglobin.isValid();
  }
  public SEScalarMassPerAmount getMeanCorpuscularHemoglobin()
  {
    if (meanCorpuscularHemoglobin == null)
      meanCorpuscularHemoglobin = new SEScalarMassPerAmount();
    return meanCorpuscularHemoglobin;
  }
  
  public boolean hasMeanCorpuscularHemoglobinConcentration()
  {
    return meanCorpuscularHemoglobinConcentration == null ? false : meanCorpuscularHemoglobinConcentration.isValid();
  }
  public SEScalarMassPerVolume getMeanCorpuscularHemoglobinConcentration()
  {
    if (meanCorpuscularHemoglobinConcentration == null)
      meanCorpuscularHemoglobinConcentration = new SEScalarMassPerVolume();
    return meanCorpuscularHemoglobinConcentration;
  }
  
  public boolean hasMeanCorpuscularVolume()
  {
    return meanCorpuscularVolume == null ? false : meanCorpuscularVolume.isValid();
  }
  public SEScalarVolume getMeanCorpuscularVolume()
  {
    if (meanCorpuscularVolume == null)
      meanCorpuscularVolume = new SEScalarVolume();
    return meanCorpuscularVolume;
  }
  
  public boolean hasRedBloodCellCount()
  {
    return redBloodCellCount == null ? false : redBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getRedBloodCellCount()
  {
    if (redBloodCellCount == null)
      redBloodCellCount = new SEScalarAmountPerVolume();
    return redBloodCellCount;
  }
  
  public boolean hasWhiteBloodCellCount()
  {
    return whiteBloodCellCount == null ? false : whiteBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getWhiteBloodCellCount()
  {
    if (whiteBloodCellCount == null)
      whiteBloodCellCount = new SEScalarAmountPerVolume();
    return whiteBloodCellCount;
  }

}
