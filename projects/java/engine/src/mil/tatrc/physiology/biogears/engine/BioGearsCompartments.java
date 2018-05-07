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

/** Note these need to match what's in BioGearsPhysiologyEngine.h */

public class BioGearsCompartments
{
	public enum Chyme { SmallIntestine("SmallIntestineChyme");
		private final String longName;       
    private Chyme(String s) { longName = s; }
    public String getLongName() { return this.longName;}
    static public String longValueOf(String s) 
    { 
    	for(Chyme c : values())
    		if(c.name().equals(s))
    			return c.longName;
    	return null;
    }
	};
	
	public enum Pulmonary { Mouth("Mouth"), Stomach("Stomach"), Carina("Carina"), Lungs("PulmonaryLungs"), 
		                      LeftLung("LeftLungPulmonary"), LeftDeadSpace("LeftDeadSpace"), LeftAlveoli("LeftAlveoli"),
		                      LeftAlveoliLeak("LeftAlveoliLeak"), LeftChestLeak("LeftChestLeak"),
		                      RightLung("RightLungPulmonary"), RightDeadSpace("RightDeadSpace"), RightAlveoli("RightAlveoli"),
		                      RightAlveoliLeak("RightAlveoliLeak"), RightChestLeak("RightChestLeak"),
		                      PleuralCavity("PleuralCavity"), LeftPleuralCavity("LeftPleuralCavity"), RightPleuralCavity("RightPleuralCavity");		                      
    private final String longName;       
    private Pulmonary(String s) { longName = s; }
    public String getLongName() { return this.longName;}
    static public String longValueOf(String s) 
    { 
    	for(Pulmonary c : values())
    		if(c.name().equals(s))
    			return c.longName;
    	return null;
    }
	};
	
	public enum Tissue { Bone("BoneTissue"), Brain("BrainTissue"), Fat("FatTissue"), Gut("GutTissue"), 
		                   LeftKidney("LeftKidneyTissue"), LeftLung("LeftLungTissue"), Liver("LiverTissue"), Muscle("MuscleTissue"), 
		                   Myocardium("MyocardiumTissue"), RightKidney("RightKidneyTissue"), RightLung("RightLungTissue"), Skin("SkinTissue"), 
		                   Spleen("SpleenTissue");
		private final String longName;       
    private Tissue(String s) { longName = s; }
    public String getLongName() { return this.longName;}
    static public String longValueOf(String s) 
    { 
    	for(Tissue c : values())
    		if(c.name().equals(s))
    			return c.longName;
    	return null;
    }
  };
  
  public enum Extravascular { BoneExtracellular("BoneTissueExtracellular"), BrainExtracellular("BrainTissueExtracellular"), FatExtracellular("FatTissueExtracellular"), GutExtracellular("GutTissueExtracellular"), 
  	                          LeftKidneyExtracellular("LeftKidneyTissueExtracellular"), LeftLungExtracellular("LeftLungTissueExtracellular"), LiverExtracellular("LiverTissueExtracellular"), MuscleExtracellular("MuscleTissueExtracellular"), 
  	                          MyocardiumExtracellular("MyocardiumTissueExtracellular"), RightKidneyExtracellular("RightKidneyTissueExtracellular"), RightLungExtracellular("RightLungTissueExtracellular"), SkinExtracellular("SkinTissueExtracellular"), 
  	                          SpleenExtracellular("SpleenTissueExtracellular"),
  	                          BoneIntracellular("BoneTissueIntracellular"), BrainIntracellular("BrainTissueIntracellular"), FatIntracellular("FatTissueIntracellular"), GutIntracellular("GutTissueIntracellular"), 
  	                          LeftKidneyIntracellular("LeftKidneyTissueIntracellular"), LeftLungIntracellular("LeftLungTissueIntracellular"), LiverIntracellular("LiverTissueIntracellular"), MuscleIntracellular("MuscleTissueIntracellular"), 
  	                          MyocardiumIntracellular("MyocardiumTissueIntracellular"), RightKidneyIntracellular("RightKidneyTissueIntracellular"), RightLungIntracellular("RightLungTissueIntracellular"), SkinIntracellular("SkinTissueIntracellular"), 
  	                          SpleenIntracellular("SpleenTissueIntracellular");
  	private final String longName;       
  	private Extravascular(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Extravascular c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };
  
  public enum Lymph { Lymph("Lymph");
  	private final String longName;       
  	private Lymph(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Lymph c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };  	
	
	public enum Urine { Ureters("Ureters"), LeftUreter("LeftUreter"),  RightUreter("RightUreter"), Bladder("Bladder");
		private final String longName;       
  	private Urine(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Urine c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };
	
	public enum Vascular {  Aorta("Aorta"), Heart("Heart"), Myocardium("MyocardiumVasculature"), LeftHeart("LeftHeart"), RightHeart("RightHeart"), 
		                      Pericardium("Pericardium"), VenaCava("VenaCava"), Lungs("LungsVasculature"),    
                          PulmonaryArteries("PulmonaryArteries"), PulmonaryCapillaries("PulmonaryCapillaries"), PulmonaryVeins("PulmonaryVeins"),
                          LeftLung("LeftLungVasculature"), LeftPulmonaryArteries("LeftPulmonaryArteries"), LeftPulmonaryCapillaries("LeftPulmonaryCapillaries"), LeftPulmonaryVeins("LeftPulmonaryVeins"), 
                          RightLung("RightLungVasculature"), RightPulmonaryArteries("RightPulmonaryArteries"), RightPulmonaryCapillaries("RightPulmonaryCapillaries"), RightPulmonaryVeins("RightPulmonaryVeins"),  
                          Kidneys("KidneyVasculature"), LeftKidney("LeftKidneyVasculature"), LeftRenalArtery("LeftRenalArtery"), 
                          LeftNephron("LeftNephron"), LeftAfferentArteriole("LeftAfferentArteriole"), LeftGlomerularCapillaries("LeftGlomerularCapillaries"), 
                          LeftEfferentArteriole("LeftEfferentArteriole"), LeftPeritubularCapillaries("LeftPeritubularCapillaries"), 
                          LeftBowmansCapsules("LeftBowmansCapsules"), LeftTubules("LeftTubules"), 
                          LeftRenalVein("LeftRenalVein"), RightKidney("RightKidneyVasculature"), RightRenalArtery("RightRenalArtery"), RightNephron("RightNephron"), 
                          RightAfferentArteriole("RightAfferentArteriole"), RightGlomerularCapillaries("RightGlomerularCapillaries"), RightBowmansCapsules("RightBowmansCapsules"), RightTubules("RightTubules"),
                          RightEfferentArteriole("RightEfferentArteriole"), RightPeritubularCapillaries("RightPeritubularCapillaries"), RightRenalVein("RightRenalVein"), 
                          Bone("BoneVasculature"), Brain("BrainVasculature"), Fat("FatVasculature"), Gut("GutVasculature"), Splanchnic("SplanchnicVasculature"), SmallIntestine("SmallIntestineVasculature"), LargeIntestine("LargeIntestineVasculature"), 
                          Liver("LiverVasculature"), Spleen("SpleenVasculature"), Skin("SkinVasculature"), Muscle("MuscleVasculature"), LeftArm("LeftArmVasculature"), LeftLeg("LeftLegVasculature"), RightArm("RightArmVasculature"), RightLeg("RightLegVasculature");
		private final String longName;       
  	private Vascular(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Vascular c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
	};
                          
  public enum Environment { Ambient("Ambient");
  	private final String longName;       
  	private Environment(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Environment c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };  	
                                                   
  public enum AnesthesiaMachine { AnesthesiaConnection("AnesthesiaConnection"), ExpiratoryLimb("ExpiratoryLimb"), GasInlet("GasInlet"), 
  	                              GasSource("GasSource"), InspiratoryLimb("InspiratoryLimb"), ReliefValve("ReliefValve"), Scrubber("Scrubber"), 
  	                              Selector("Selector"), Squeeze("Squeeze"), Ventilator("Ventilator"), VentilatorConnection("VentilatorConnection"), YPiece("YPiece"); 
    private final String longName;       
  	private AnesthesiaMachine(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(AnesthesiaMachine c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };


  public enum Inhaler { Mouthpiece("Mouthpiece");
  	private final String longName;       
  	private Inhaler(String s) { longName = s; }
  	public String getLongName() { return this.longName;}
  	static public String longValueOf(String s) 
  	{ 
  		for(Inhaler c : values())
  			if(c.name().equals(s))
  				return c.longName;
  		return null;
  	}
  };  	                              
}
