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
import mil.tatrc.physiology.datamodel.bind.EnumClarityIndicator;
import mil.tatrc.physiology.datamodel.bind.EnumPresenceIndicator;
import mil.tatrc.physiology.datamodel.bind.EnumUrineColor;
import mil.tatrc.physiology.datamodel.bind.UrinalysisData;
import mil.tatrc.physiology.datamodel.properties.*;

public class SEUrinalysis extends SEPatientAssessment
{
  protected EnumUrineColor          color;
  protected EnumClarityIndicator    apperance;
  protected EnumPresenceIndicator   glucose;
  protected EnumPresenceIndicator   ketone;
  protected SEScalar                bilirubin;
  protected SEScalar                specificGravity;
  protected EnumPresenceIndicator   blood;
  protected SEScalar                pH;  
  protected EnumPresenceIndicator   protein;
  protected SEScalarMassPerVolume   urobilinogen;
  protected EnumPresenceIndicator   nitrite;
  protected EnumPresenceIndicator   leukocyteEsterase;
  
  public void clear()
  {
    super.clear();
    this.color = null;
    this.apperance = null;
    this.urobilinogen = null;
    this.ketone = null;
    this.bilirubin = null;
    this.specificGravity = null;
    this.blood = null;
    this.pH = null;
    this.protein = null;
    this.urobilinogen = null;
    this.nitrite = null;
    this.leukocyteEsterase = null;
  }
  
  public void reset()
  {
    super.reset();
    this.color = null;
    this.apperance = null;
    this.urobilinogen = null;
    this.ketone = null;
    if(this.bilirubin!=null)
      this.bilirubin.invalidate();
    if(this.specificGravity!=null)
      this.specificGravity.invalidate();    
    this.blood = null;
    if(this.pH!=null)
      this.pH.invalidate();    
    this.protein = null;
    if(this.urobilinogen!=null)
      this.urobilinogen.invalidate();    
    this.nitrite = null;
    this.leukocyteEsterase = null;   
  }
  
  public boolean load(UrinalysisData in)
  {
    super.load(in);
    if(in.getColor()!=null)
      this.setColor(in.getColor());
    if(in.getAppearance()!=null)
      this.setAppearance(in.getAppearance());
    if(in.getGlucose()!=null)
      this.setGlucose(in.getGlucose());
    if(in.getKetone()!=null)
      this.setKetone(in.getKetone());
    if(in.getBilirubin()!=null)
      this.getBilirubin().load(in.getBilirubin());
    if(in.getSpecificGravity()!=null)
      this.getSpecificGravity().load(in.getSpecificGravity());
    if(in.getBlood()!=null)
      this.setBlood(in.getBlood());
    if(in.getPH()!=null)
      this.getPH().load(in.getPH());
    if(in.getProtein()!=null)
      this.setProtein(in.getProtein());
    if(in.getUrobilinogen()!=null)
      this.getUrobilinogen().load(in.getUrobilinogen());
    if(in.getNitrite()!=null)
      this.setNitrite(in.getNitrite());
    if(in.getLeukocyteEsterase()!=null)
      this.setLeukocyteEsterase(in.getLeukocyteEsterase());
    return true;
  }
  
  public UrinalysisData unload()
  {
    UrinalysisData data = CDMSerializer.objFactory.createUrinalysisData();
    unload(data);
    return data;
  }
  
  protected void unload(UrinalysisData data)
  {
    super.unload(data);
    if (color != null)
      data.setColor(color);
    if (apperance != null)
      data.setAppearance(apperance);
    if (glucose != null)
      data.setGlucose(glucose);
    if (ketone != null)
      data.setKetone(ketone);
    if (bilirubin != null)
      data.setBilirubin(bilirubin.unload());
    if (specificGravity != null)
      data.setSpecificGravity(specificGravity.unload());
    if (blood != null)
      data.setBlood(blood);
    if (pH != null)
      data.setPH(pH.unload());
    if (protein != null)
      data.setProtein(protein);
    if (urobilinogen != null)
      data.setUrobilinogen(urobilinogen.unload());
    if (nitrite != null)
      data.setNitrite(nitrite);
    if (leukocyteEsterase != null)
      data.setLeukocyteEsterase(leukocyteEsterase);
  }
  
  public EnumUrineColor  getColor() { return this.color;}
  public void    setColor(EnumUrineColor color){this.color=color;}
  public boolean hasColor(){return this.color==null?false:true;}
  
  public EnumClarityIndicator  getAppearance() { return this.apperance;}
  public void    setAppearance(EnumClarityIndicator a){this.apperance=a;}
  public boolean hasAppearance(){return this.apperance==null?false:true;}
  
  public EnumPresenceIndicator  getGlucose() { return this.glucose;}
  public void    setGlucose(EnumPresenceIndicator g){this.glucose=g;}
  public boolean hasGlucose(){return this.glucose==null?false:true;}
  
  public EnumPresenceIndicator  getKetone() { return this.ketone;}
  public void    setKetone(EnumPresenceIndicator k){this.ketone=k;}
  public boolean hasKetone(){return this.ketone==null?false:true;}
  
  public boolean hasBilirubin()
  {
    return bilirubin == null ? false : bilirubin.isValid();
  }
  public SEScalar getBilirubin()
  {
    if (bilirubin == null)
      bilirubin = new SEScalar();
    return bilirubin;
  }
  
  public boolean hasSpecificGravity()
  {
    return specificGravity == null ? false : specificGravity.isValid();
  }
  public SEScalar getSpecificGravity()
  {
    if (specificGravity == null)
      specificGravity = new SEScalar();
    return specificGravity;
  }
  
  public EnumPresenceIndicator  getBlood() { return this.blood;}
  public void    setBlood(EnumPresenceIndicator b){this.blood=b;}
  public boolean hasBlood(){return this.blood==null?false:true;}
  
  public boolean hasPH()
  {
    return pH == null ? false : pH.isValid();
  }
  public SEScalar getPH()
  {
    if (pH == null)
      pH = new SEScalar();
    return pH;
  }
  
  public EnumPresenceIndicator  getProtein() { return this.protein;}
  public void    setProtein(EnumPresenceIndicator p){this.protein=p;}
  public boolean hasProtein(){return this.protein==null?false:true;}
 
  
  public boolean hasUrobilinogen()
  {
    return urobilinogen == null ? false : urobilinogen.isValid();
  }
  public SEScalarMassPerVolume getUrobilinogen()
  {
    if (urobilinogen == null)
      urobilinogen = new SEScalarMassPerVolume();
    return urobilinogen;
  }
  
  public EnumPresenceIndicator  getNitrite() { return this.nitrite;}
  public void    setNitrite(EnumPresenceIndicator n){this.nitrite=n;}
  public boolean hasNitrite(){return this.nitrite==null?false:true;}
  
  public EnumPresenceIndicator  getLeukocyteEsterase() { return this.leukocyteEsterase;}
  public void    setLeukocyteEsterase(EnumPresenceIndicator l){this.leukocyteEsterase=l;}
  public boolean hasLeukocyteEsterase(){return this.leukocyteEsterase==null?false:true;}
}
