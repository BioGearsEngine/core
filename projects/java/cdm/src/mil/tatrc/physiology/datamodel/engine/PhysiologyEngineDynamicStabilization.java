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
package mil.tatrc.physiology.datamodel.engine;

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineDynamicStabilizationCriteriaData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineDynamicStabilizationCriteriaPropertyData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineDynamicStabilizationData;
import mil.tatrc.physiology.datamodel.bind.PhysiologyEngineDynamicConditionStabilizationData;
import mil.tatrc.physiology.datamodel.properties.SEScalarTime;

public class PhysiologyEngineDynamicStabilization extends PhysiologyEngineStabilization
{
  public class Criteria
  {
    public SEScalarTime convergenceTime=new SEScalarTime();
    public SEScalarTime minimumReactionTime=new SEScalarTime();
    public SEScalarTime maximumAllowedStabilizationTime=new SEScalarTime();
    
    public List<PropertyConvergence> properties=new ArrayList<PropertyConvergence>();
    
    public void load(PhysiologyEngineDynamicStabilizationCriteriaData in)
    {
      convergenceTime.load(in.getConvergenceTime());
      minimumReactionTime.load(in.getMinimumReactionTime());
      maximumAllowedStabilizationTime.load(in.getMaximumAllowedStabilizationTime());      
      for(PhysiologyEngineDynamicStabilizationCriteriaPropertyData pcData : in.getPropertyConvergence())
      {
        PropertyConvergence pc = new PropertyConvergence();
        pc.name = pcData.getName();
        pc.percentDifference = pcData.getPercentDifference();
      }
    }
    public PhysiologyEngineDynamicStabilizationCriteriaData unload()
    {
      PhysiologyEngineDynamicStabilizationCriteriaData to = new PhysiologyEngineDynamicStabilizationCriteriaData();
      to.setConvergenceTime(this.convergenceTime.unload());
      to.setMinimumReactionTime(this.minimumReactionTime.unload());
      to.setMaximumAllowedStabilizationTime(this.maximumAllowedStabilizationTime.unload());
      for(PropertyConvergence pc : this.properties)
      {
        PhysiologyEngineDynamicStabilizationCriteriaPropertyData pcData = CDMSerializer.objFactory.createPhysiologyEngineDynamicStabilizationCriteriaPropertyData();
        pcData.setName(pc.name);
        pcData.setPercentDifference(pc.percentDifference);
        to.getPropertyConvergence().add(pcData);
      }
      return to;
    }
    public void createProperty(double percentDifference, String property)
    {
      PropertyConvergence pc = new PropertyConvergence();
      pc.name=property;
      pc.percentDifference=percentDifference;
      this.properties.add(pc);
    }
  }
  public class PropertyConvergence
  {
    public String name;
    public double percentDifference;
  }
  
  
  protected Criteria restingStabilizationCriteria;
  protected Criteria feedbackStabilizationCriteria;
  protected Map<String,Criteria> conditionStabilizationCriteria;
  
  public PhysiologyEngineDynamicStabilization()
  {
    super();
    this.conditionStabilizationCriteria=new HashMap<String,Criteria>();
  }
  
  public void reset()
  {
    super.reset();
    this.restingStabilizationCriteria=null;
    this.feedbackStabilizationCriteria=null;
    if(this.conditionStabilizationCriteria!=null)
      this.conditionStabilizationCriteria.clear();
  }
  
  public boolean load(PhysiologyEngineDynamicStabilizationData in) 
  {
    super.load(in);
    getRestingStabilizationCriteria().load(in.getRestingStabilizationCriteria());
    if(in.getFeedbackStabilizationCriteria()!=null)
      getFeedbackStabilizationCriteria().load(in.getFeedbackStabilizationCriteria());
    for(PhysiologyEngineDynamicConditionStabilizationData cData : in.getConditionStabilization())
    {
      Criteria c = createCondition(cData.getName());
      c.load(cData.getCriteria());
    }
    return true;
  }
  
  public PhysiologyEngineDynamicStabilizationData unload()
  {
    PhysiologyEngineDynamicStabilizationData to = CDMSerializer.objFactory.createPhysiologyEngineDynamicStabilizationData();
    unload(to);
    return to;
  }
  
  protected void unload(PhysiologyEngineDynamicStabilizationData data)
  {
    super.unload(data);
    if(this.hasRestingStabilizationCriteria())
      data.setRestingStabilizationCriteria(getRestingStabilizationCriteria().unload());
    if(this.hasFeedbackStabilizationCriteria())
      data.setFeedbackStabilizationCriteria(getFeedbackStabilizationCriteria().unload());
    for(String name : this.conditionStabilizationCriteria.keySet())
    {
      PhysiologyEngineDynamicConditionStabilizationData c = CDMSerializer.objFactory.createPhysiologyEngineDynamicConditionStabilizationData();
      c.setName(name);
      c.setCriteria(this.conditionStabilizationCriteria.get(name).unload());
      data.getConditionStabilization().add(c);
    }
  }
  
  public boolean hasRestingStabilizationCriteria()
  {
    return restingStabilizationCriteria != null;
  }
  public Criteria getRestingStabilizationCriteria()
  {
    if (restingStabilizationCriteria == null)
      restingStabilizationCriteria = new Criteria();
    return restingStabilizationCriteria;
  }
  
  public boolean hasFeedbackStabilizationCriteria()
  {
    return feedbackStabilizationCriteria != null;
  }
  public Criteria getFeedbackStabilizationCriteria()
  {
    if (feedbackStabilizationCriteria == null)
      feedbackStabilizationCriteria = new Criteria();
    return feedbackStabilizationCriteria;
  }
  
  public Criteria createCondition(String type)
  {
    Criteria c = new Criteria();
    this.conditionStabilizationCriteria.put(type, c);
    return c;
  }
  public boolean hasCondition(String type)
  {
    return this.conditionStabilizationCriteria.containsKey(type);
  }
  public Criteria getCondition(String type)
  {
    return this.conditionStabilizationCriteria.get(type);
  }
}
