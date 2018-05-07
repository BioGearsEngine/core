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
import mil.tatrc.physiology.datamodel.bind.ComprehensiveMetabolicPanelData;
import mil.tatrc.physiology.datamodel.properties.*;

public class SEComprehensiveMetabolicPanel extends SEPatientAssessment
{
  protected SEScalarMassPerVolume   albumin;
  protected SEScalarMassPerVolume   ALP;
  protected SEScalarMassPerVolume   ALT;
  protected SEScalarMassPerVolume   AST;
  protected SEScalarMassPerVolume   BUN;
  protected SEScalarMassPerVolume   calcium;
  protected SEScalarAmountPerVolume chloride;
  protected SEScalarAmountPerVolume CO2;  
  protected SEScalarMassPerVolume   creatinine;
  protected SEScalarMassPerVolume   glucose;
  protected SEScalarAmountPerVolume potassium;
  protected SEScalarAmountPerVolume sodium;
  protected SEScalarMassPerVolume   totalBilirubin;
  protected SEScalarMassPerVolume   totalProtein;
  
  public void clear()
  {
    super.clear();
    this.albumin = null;
    this.ALP = null;
    this.ALT = null;
    this.AST = null;
    this.BUN = null;
    this.calcium = null;
    this.chloride = null;
    this.CO2 = null;
    this.creatinine = null;
    this.glucose = null;
    this.potassium = null;
    this.sodium = null;
    this.totalBilirubin = null;
    this.totalProtein = null;
  }
  
  public void reset()
  {
    super.reset();
    if(this.albumin!=null)
      this.albumin.invalidate();
    if(this.ALP!=null)
      this.ALP.invalidate();
    if(this.ALT!=null)
      this.ALT.invalidate();
    if(this.AST!=null)
      this.AST.invalidate();
    if(this.BUN!=null)
      this.BUN.invalidate();
    if(this.calcium!=null)
      this.calcium.invalidate();
    if(this.chloride!=null)
      this.chloride.invalidate();
    if(this.CO2!=null)
      this.CO2.invalidate();
    if(this.creatinine!=null)
      this.creatinine.invalidate();
    if(this.glucose!=null)
      this.glucose.invalidate();
    if(this.potassium!=null)
      this.potassium.invalidate();
    if(this.sodium!=null)
      this.sodium.invalidate();
    if(this.totalBilirubin!=null)
      this.totalBilirubin.invalidate();
    if(this.totalProtein!=null)
      this.totalProtein.invalidate();
  }
  
  public boolean load(ComprehensiveMetabolicPanelData in)
  {
    super.load(in);
    if(in.getAlbumin()!=null)
      this.getAlbumin().load(in.getAlbumin());
    if(in.getALP()!=null)
      this.getALP().load(in.getALP());
    if(in.getALT()!=null)
      this.getALT().load(in.getALT());
    if(in.getAST()!=null)
      this.getAST().load(in.getAST());
    if(in.getBUN()!=null)
      this.getBUN().load(in.getBUN());
    if(in.getCalcium()!=null)
      this.getCalcium().load(in.getCalcium());
    if(in.getChloride()!=null)
      this.getChloride().load(in.getChloride());
    if(in.getCO2()!=null)
      this.getCO2().load(in.getCO2());
    if(in.getCreatinine()!=null)
      this.getCreatinine().load(in.getCreatinine());
    if(in.getGlucose()!=null)
      this.getGlucose().load(in.getGlucose());
    if(in.getPotassium()!=null)
      this.getPotassium().load(in.getPotassium());
    if(in.getSodium()!=null)
      this.getSodium().load(in.getSodium());
    if(in.getTotalBilirubin()!=null)
      this.getTotalBilirubin().load(in.getTotalBilirubin());
    if(in.getTotalProtein()!=null)
      this.getTotalProtein().load(in.getTotalProtein());
    return true;
  }
  
  public ComprehensiveMetabolicPanelData unload()
  {
    ComprehensiveMetabolicPanelData data = CDMSerializer.objFactory.createComprehensiveMetabolicPanelData();
    unload(data);
    return data;
  }
  
  protected void unload(ComprehensiveMetabolicPanelData data)
  {
    super.unload(data);
    if (albumin != null)
      data.setAlbumin(albumin.unload());
    if (ALP != null)
      data.setALP(ALP.unload());
    if (ALT != null)
      data.setALT(ALT.unload());
    if (AST != null)
      data.setAST(AST.unload());
    if (BUN != null)
      data.setBUN(BUN.unload());
    if (calcium != null)
      data.setCalcium(calcium.unload());
    if (chloride != null)
      data.setChloride(chloride.unload());
    if (CO2 != null)
      data.setCO2(CO2.unload());
    if (creatinine != null)
      data.setCreatinine(creatinine.unload());
    if (glucose != null)
      data.setGlucose(glucose.unload());
    if (potassium != null)
      data.setPotassium(potassium.unload());
    if (sodium != null)
      data.setSodium(sodium.unload());
    if (totalBilirubin != null)
      data.setTotalBilirubin(totalBilirubin.unload());
    if (totalProtein != null)
      data.setTotalProtein(totalProtein.unload());
  }
  
  public boolean hasAlbumin()
  {
    return albumin == null ? false : albumin.isValid();
  }
  public SEScalarMassPerVolume getAlbumin()
  {
    if (albumin == null)
      albumin = new SEScalarMassPerVolume();
    return albumin;
  }
  
  public boolean hasALP()
  {
    return ALP == null ? false : ALP.isValid();
  }
  public SEScalarMassPerVolume getALP()
  {
    if (ALP == null)
      ALP = new SEScalarMassPerVolume();
    return ALP;
  }
  
  public boolean hasALT()
  {
    return ALT == null ? false : ALT.isValid();
  }
  public SEScalarMassPerVolume getALT()
  {
    if (ALT == null)
      ALT = new SEScalarMassPerVolume();
    return ALT;
  }
  
  public boolean hasAST()
  {
    return AST == null ? false : AST.isValid();
  }
  public SEScalarMassPerVolume getAST()
  {
    if (AST == null)
      AST = new SEScalarMassPerVolume();
    return AST;
  }
  
  public boolean hasBUN()
  {
    return BUN == null ? false : BUN.isValid();
  }
  public SEScalarMassPerVolume getBUN()
  {
    if (BUN == null)
      BUN = new SEScalarMassPerVolume();
    return BUN;
  }
  
  public boolean hasCalcium()
  {
    return calcium == null ? false : calcium.isValid();
  }
  public SEScalarMassPerVolume getCalcium()
  {
    if (calcium == null)
      calcium = new SEScalarMassPerVolume();
    return calcium;
  }
  
  public boolean hasChloride()
  {
    return chloride == null ? false : chloride.isValid();
  }
  public SEScalarAmountPerVolume getChloride()
  {
    if (chloride == null)
      chloride = new SEScalarAmountPerVolume();
    return chloride;
  }
  
  public boolean hasCO2()
  {
    return CO2 == null ? false : CO2.isValid();
  }
  public SEScalarAmountPerVolume getCO2()
  {
    if (CO2 == null)
      CO2 = new SEScalarAmountPerVolume();
    return CO2;
  }
  
  public boolean hasCreatinine()
  {
    return creatinine == null ? false : creatinine.isValid();
  }
  public SEScalarMassPerVolume getCreatinine()
  {
    if (creatinine == null)
      creatinine = new SEScalarMassPerVolume();
    return creatinine;
  }
  
  public boolean hasGlucose()
  {
    return glucose == null ? false : glucose.isValid();
  }
  public SEScalarMassPerVolume getGlucose()
  {
    if (glucose == null)
      glucose = new SEScalarMassPerVolume();
    return glucose;
  }
  
  public boolean hasPotassium()
  {
    return potassium == null ? false : potassium.isValid();
  }
  public SEScalarAmountPerVolume getPotassium()
  {
    if (potassium == null)
      potassium = new SEScalarAmountPerVolume();
    return potassium;
  }
  
  public boolean hasSodium()
  {
    return sodium == null ? false : sodium.isValid();
  }
  public SEScalarAmountPerVolume getSodium()
  {
    if (sodium == null)
      sodium = new SEScalarAmountPerVolume();
    return sodium;
  }
  
  public boolean hasTotalBilirubin()
  {
    return totalBilirubin == null ? false : totalBilirubin.isValid();
  }
  public SEScalarMassPerVolume getTotalBilirubin()
  {
    if (totalBilirubin == null)
      totalBilirubin = new SEScalarMassPerVolume();
    return totalBilirubin;
  }
  
  public boolean hasTotalProtein()
  {
    return totalProtein == null ? false : totalProtein.isValid();
  }
  public SEScalarMassPerVolume getTotalProtein()
  {
    if (totalProtein == null)
      totalProtein = new SEScalarMassPerVolume();
    return totalProtein;
  }

}
