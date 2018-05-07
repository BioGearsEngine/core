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
package mil.tatrc.physiology.biogears.engine;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import mil.tatrc.physiology.datamodel.bind.EnumAnesthesiaMachineEvent;
import mil.tatrc.physiology.datamodel.bind.EnumPatientEvent;
import mil.tatrc.physiology.datamodel.compartment.*;
import mil.tatrc.physiology.datamodel.patient.SEPatient;
import mil.tatrc.physiology.datamodel.patient.nutrition.SENutrition;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.scenario.datarequests.*;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceTissuePharmacokinetics;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.datamodel.system.SESystem;
import mil.tatrc.physiology.datamodel.system.environment.*;
import mil.tatrc.physiology.datamodel.system.equipment.anesthesia.*;
import mil.tatrc.physiology.datamodel.system.equipment.electrocardiogram.*;
import mil.tatrc.physiology.datamodel.system.equipment.inhaler.*;
import mil.tatrc.physiology.datamodel.system.physiology.*;
import mil.tatrc.physiology.datamodel.utilities.SEEventHandler;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.LogListener;
import mil.tatrc.physiology.utilities.Pair;
import mil.tatrc.physiology.utilities.UnitConverter;

public class BioGears
{
  public SEScalarTime                     timeStep;
  public SEScalarTime                     currentTime;
  
  public SESubstanceManager               substanceManager;
  public SEPatient                        patient;
  // Physiology systems
  public SEBloodChemistrySystem           bloodChemistry;
  public SECardiovascularSystem           cardiovascular;
  public SEEndocrineSystem                endocrine;
  public SEEnergySystem                   energy;
  public SEGastrointestinalSystem         gastrointestinal;
  public SERenalSystem                    renal;
  public SENervousSystem                  nervous;
  public SERespiratorySystem              respiratory;
  public SETissueSystem                   tissue;
  public SEDrugSystem                     drug;
  // Environment 
  public SEEnvironment                    environment;
  // Anesthesia
  public SEAnesthesiaMachine              anesthesiaMachine;
  // ECG
  public SEElectroCardioGram              ecg;
  // Inhaler
  public SEInhaler                        inhaler;
  
  // Compartments
  public SECompartmentManager             compartments;
  
  protected CDMUpdatedCallback            cdmCallback;
  protected LogListener                   listener;
  protected SEEventHandler                eventHandler;
  
  protected long nativeObj;
  protected synchronized native long nativeAllocate(String logFile);
  protected synchronized native void nativeDelete(long nativeObj);
  
  protected List<Pair<SEDataRequest,SEScalar>> dataRequests = new ArrayList<Pair<SEDataRequest,SEScalar>>();  
  
  static
  {  
    //TODO: Create Static Loader Class that utilizes cmake configure_file 	  	
  	UnitConverter.initialize(System.getProperty("user.dir"));
  	ArrayList<Path> libs = new ArrayList<Path>();
  	libs.add(Paths.get("BioGearsEngine"));
  	libs.add(Paths.get("BioGearsEngineTest"));
  	libs.add(Paths.get("BioGearsEngineJNI"));	  	
    FileUtils.loadLibraries(libs);
  }

  public BioGears()
  {
    this.nativeObj=0;
  }

  public void finalize()
  {
    this.cleanUp();
  }
  
  /**
   * Note this does not reset listeners and callback pointers
   */
  public void reset()
  {
  	if(timeStep==null)
  	{
  		timeStep = new SEScalarTime();
      currentTime = new SEScalarTime();
      
      substanceManager = new SESubstanceManager();
      patient = new SEPatient();
      // Physiology systems
      bloodChemistry = new SEBloodChemistrySystem();
      cardiovascular = new SECardiovascularSystem();
      endocrine = new SEEndocrineSystem();
      energy = new SEEnergySystem();
      gastrointestinal = new SEGastrointestinalSystem();
      renal = new SERenalSystem();
      nervous = new SENervousSystem();
      respiratory = new SERespiratorySystem();
      tissue = new SETissueSystem();
      drug = new SEDrugSystem();
      // Environment 
      environment = new SEEnvironment();
      // Anesthesia
      anesthesiaMachine = new SEAnesthesiaMachine();
      // ECG
      ecg = new SEElectroCardioGram();
      // Inhaler
      inhaler = new SEInhaler();
      
      compartments = new SECompartmentManager();
      
      dataRequests = new ArrayList<Pair<SEDataRequest,SEScalar>>();
  	}
    currentTime.invalidate();
    substanceManager.loadSubstanceDirectory();
    patient.reset();
    bloodChemistry.reset();
    cardiovascular.reset();
    endocrine.reset();
    energy.reset();
    gastrointestinal.reset();
    renal.reset();
    nervous.reset();
    respiratory.reset();
    tissue.reset();
    drug.reset();
    environment.reset();
    anesthesiaMachine.reset();
    ecg.reset();
    inhaler.reset();
    compartments.clear();
    
    // I am not clearing these, end user manages these
    //cdmCallback=null;
    //listener = null;
    //eventHandler = null;
  }

  public void cleanUp()
  {
    if(this.nativeObj!=0)
    {
      nativeDelete(this.nativeObj);
      this.nativeObj=0;
    }
    this.listener = null;
    FileUtils.delete(new File(System.getProperty("user.home") + "/config/"));
    FileUtils.delete(new File(System.getProperty("user.home") + "/ecg/"));
    FileUtils.delete(new File(System.getProperty("user.home") + "/patients/"));
    FileUtils.delete(new File(System.getProperty("user.home") + "/environments/"));
    FileUtils.delete(new File(System.getProperty("user.home") + "/xsd/"));
    FileUtils.delete(new File(System.getProperty("user.home") + "/BioGearsConfiguration.xml"));
  }

  public void setListener(LogListener listener)
  {
    this.listener = listener;
  }
  
  public void setEventHandler(SEEventHandler handler)
  {
    eventHandler = handler;
  }
  
  /**
   * At the end of every AdvanceTime call, C++
   * will push all the data requested from C++ to Java
   * If this is not null, this callback will be called
   * after all data has been set on the Java side.
   * @param cb
   */
  public void setCDMCallback(CDMUpdatedCallback cb)
  {
  	this.cdmCallback = cb;
  }
  /** 
   * C++ will call this method
   * @param type 0 = patient, 1 = anesthesia machine
   * @param event enumeration index
   * @param active if it's on or off
   */
  protected void handleEvent(int type, int event, boolean active, double time_s)
  {
    switch(type)
    {
      case 0:
        if(eventHandler!=null)
          eventHandler.handlePatientEvent(EnumPatientEvent.values()[event],active,new SEScalarTime(time_s,TimeUnit.s));
        this.patient.setEvent(EnumPatientEvent.values()[event],active);
        break;
      case 1:
        if(eventHandler!=null)
          eventHandler.handleAnesthesiaMachineEvent(EnumAnesthesiaMachineEvent.values()[event],active,new SEScalarTime(time_s,TimeUnit.s));
        this.anesthesiaMachine.setEvent(EnumAnesthesiaMachineEvent.values()[event],active);
        break;
      default:
          Log.error("Unsupported event type "+type);
    }
  }
  
  public List<Pair<SEDataRequest,SEScalar>> getDataRequestPairs()
  {
    return Collections.unmodifiableList(this.dataRequests);
  }
  
  protected void updateCDM(double time_s, double[] data)
  {
    int i=0;
    currentTime.setValue(time_s,TimeUnit.s);    
    for(Pair<SEDataRequest,SEScalar> e : dataRequests)
    {
      e.getR().setValue(data[i++],e.getL().getUnit());
    }
    if(this.cdmCallback!=null)
      cdmCallback.update(time_s);
  }
  
  /**
   * You can provided a data request with out a unit
   * And the engine will update the request with what
   * ever it calculated the requested value in.
   * The method will be called to inform us what 
   * unit the data given to updateCDM. If the data 
   * request had a unit specified in the first place,
   * this will be the same, but if no unit was specified
   * in the data request, this information is needed
   * when putting the received value into is associated scalar.
   * @param units
   */
  protected void setCDMHeadings(String[] units)
  {
    int i=0;
    for(Pair<SEDataRequest,SEScalar> e : dataRequests)
    {
      // Rip out the units from the headings
      String unit = units[i++];
      int idx = unit.indexOf('(');
      if(idx > -1)
      {
        unit = unit.substring(idx+1, unit.length()-1);
        unit = unit.replace("_", " ");
        e.getL().setUnit(unit);
      }
      else
        e.getL().setUnit("");// Unitless
    }
  }
  
  // Process all the data requests, and map them to CDM scalars
  protected boolean requestData(SEDataRequestManager drMgr)
  {
  	dataRequests.clear();
  	
    // Gather our systems up into a list to make things easier
    List<SESystem> systems = new ArrayList<SESystem>();
    systems.add(bloodChemistry);
    systems.add(cardiovascular);
    systems.add(endocrine);
    systems.add(energy);
    systems.add(gastrointestinal);
    systems.add(renal);
    systems.add(nervous);
    systems.add(respiratory);
    systems.add(tissue);
    systems.add(drug);
    systems.add(energy);
    
    List<SESystem> equipment = new ArrayList<SESystem>();
    equipment.add(anesthesiaMachine);
    equipment.add(ecg);
    equipment.add(inhaler);
    
    for(SEDataRequest dr : drMgr.getRequestedData())
    {
      if(dr instanceof SEPhysiologyDataRequest)
      {
        SEScalar scalar = null;
        for (SESystem sys : systems)
        {
          scalar = SEScalar.getScalar(sys,dr.getName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
            break;
          }
          // Check to see if the request is for a child class
          // I could do this more generically with reflection...
          // I know there should be a dash

          if (sys instanceof SEGastrointestinalSystem && dr.getName().startsWith("StomachContents"))
          {
            String scalarNameWithoutStomach = dr.getName().substring(dr.getName().indexOf('-')+1);
            SENutrition nutrition = ((SEGastrointestinalSystem)sys).getStomachContents();

            scalar = SEScalar.getScalar(nutrition, scalarNameWithoutStomach);
            if (scalar != null)
            {
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
              break;
            }
          }
          
          if (sys instanceof SEDrugSystem && dr.getName().startsWith("PupillaryResponse"))
          {
            String propertyName = dr.getName().substring(dr.getName().indexOf('-')+1);
            SEPupillaryResponse pupillaryResponse = ((SEDrugSystem)sys).getPupillaryResponse();

            scalar = SEScalar.getScalar(pupillaryResponse, propertyName);
            if (scalar != null)
            {
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
              break;
            }
          }
          
          if (sys instanceof SENervousSystem && (dr.getName().startsWith("LeftEyePupillaryResponse") || dr.getName().startsWith("RightEyePupillaryResponse")))
          {
          	String propertyName = dr.getName().substring(dr.getName().indexOf('-')+1);
          	
          	
            SEPupillaryResponse pupillaryResponse=null;
            if(dr.getName().startsWith("Left"))
            	pupillaryResponse = ((SENervousSystem)sys).getLeftEyePupillaryResponse();
            else if(dr.getName().startsWith("Right"))
            	pupillaryResponse = ((SENervousSystem)sys).getRightEyePupillaryResponse();
            
            scalar = SEScalar.getScalar(pupillaryResponse, propertyName);
            if (scalar != null)
            {
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
              break;
            }
          }
        }
        if(scalar!=null)
          continue;
        Log.error("Unable to find system property " + dr.getName());
        return false;
      }
      else if(dr instanceof SEGasCompartmentDataRequest)
      {
      	SEGasCompartmentDataRequest cdr = (SEGasCompartmentDataRequest)dr;
        SEGasCompartment cmpt = compartments.GetGasCompartment(cdr.getCompartment());        
        if(cdr.hasSubstance())
        {
        	dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt.getSubstanceQuantity(cdr.getSubstance()), dr.getName())));
          continue;
        }
        else
        {
          dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getName())));
          continue;
        }
      }
      else if(dr instanceof SELiquidCompartmentDataRequest)
      {
      	SELiquidCompartmentDataRequest cdr = (SELiquidCompartmentDataRequest)dr;
        SELiquidCompartment cmpt = compartments.GetLiquidCompartment(cdr.getCompartment());        
        if(cdr.hasSubstance())
        {
        	dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt.getSubstanceQuantity(cdr.getSubstance()), dr.getName())));
          continue;
        }
        else
        {
          dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getName())));
          continue;
        }
      }
      else if(dr instanceof SEThermalCompartmentDataRequest)
      {
      	SEThermalCompartmentDataRequest cdr = (SEThermalCompartmentDataRequest)dr;
        SEThermalCompartment cmpt = compartments.GetThermalCompartment(cdr.getCompartment());        
        dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getName())));
        continue;
      }      
      else if(dr instanceof SETissueCompartmentDataRequest)
      {
      	SETissueCompartmentDataRequest cdr = (SETissueCompartmentDataRequest)dr;
        SETissueCompartment cmpt = compartments.GetTissueCompartment(cdr.getCompartment()); 
        dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getName())));
        continue;
      }
      else if(dr instanceof SEPatientDataRequest)
      {
        String name = dr.getName();
        if(dr.getName().startsWith("Patient"))
          name = dr.getName().substring(7);
        SEScalar scalar = SEScalar.getScalar(this.patient, name);
        if (scalar != null)
        {
          dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
          continue;
        }
        Log.error("Unable to find patient property " + dr.getName());
        return false;
      }
      else if(dr instanceof SESubstanceDataRequest)
      {
        SESubstanceDataRequest sdr = (SESubstanceDataRequest)dr;
        SESubstance sub = this.substanceManager.getSubstance(sdr.getSubstance().getName());
        if(sdr.hasCompartment())
        {
        	if(sdr.getName().equals("PartitionCoefficient"))
        	{
        		SESubstanceTissuePharmacokinetics subTk = sub.getPK().getTissueKinetics(sdr.getCompartment());
        		dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,subTk.getPartitionCoefficient()));
        		continue;
        	}
        	else
        	{
        		Log.error("Do not have a property " + dr.getName() +" associated with compartments on a substance");
            return false;
        	}
        }
        else
        {
          SEScalar scalar = SEScalar.getScalar(sub, dr.getName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
            continue;
          }
        }
        Log.error("Unable to find substance property " + dr.getName());
        return false;
      }
      else if(dr instanceof SEEnvironmentDataRequest)
      {
      	SEScalar scalar = SEScalar.getScalar(environment, dr.getName());
      	if (scalar != null)
      	{
      		dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
      		continue;
      	}
      	Log.error("Unable to find environment property " + dr.getName());
      	return false;
      }    
      else if(dr instanceof SEEquipmentDataRequest)
      {
        SEScalar scalar = null;
        for(SESystem equip : equipment)
        {
          scalar = SEScalar.getScalar(equip, dr.getName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
            break;
          }
        }
        if(scalar!=null)
          continue;
        Log.error("Unable to find equipment property " + dr.getName());
        return false;
      }       
      else
      {
        Log.error("Unsupported data request type");
        return false;
      }
    }
    
    // Make sure nothing is mapped to null
    for(Pair<SEDataRequest,SEScalar> p : dataRequests)
    {
      if(p.getL()==null || p.getR()==null)
      {
        Log.error("We have an invalid data request pair");
        return false;
      }
    }
    return true;
  }
  
//TODO I could make origin, the Scenario/engine name we are running
 protected void LogDebug(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleDebug(msg, origin);
 }
 protected void LogInfo(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleInfo(msg, origin);
 }
 protected void LogWarning(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleWarn(msg, origin);
 }
 protected void LogError(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleError(msg, origin);
 }
 protected void LogFatal(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleFatal(msg, origin);
 }
}
