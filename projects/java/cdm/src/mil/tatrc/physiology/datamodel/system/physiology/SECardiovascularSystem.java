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
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SECardiovascularSystem extends SEPhysiologySystem implements SESystem
{
	protected SEScalarPressure                  arterialPressure;
	protected SEScalarVolume                    bloodVolume;
	protected SEScalarVolumePerTimeArea         cardiacIndex;
	protected SEScalarVolumePerTime             cardiacOutput;
	protected SEScalarPressure                  centralVenousPressure;
	protected SEScalarVolumePerTime             cerebralBloodFlow;
	protected SEScalarPressure                  cerebralPerfusionPressure;
	protected SEScalarPressure                  diastolicArterialPressure;
	protected SEScalarFraction                  heartEjectionFraction;
	protected SEScalarFrequency                 heartRate;
	protected EnumHeartRhythm 		              heartRhythm;
	protected SEScalarVolume                    heartStrokeVolume;
	protected SEScalarPressure                  intracranialPressure;
	protected SEScalarPressure                  meanArterialPressure;
	protected SEScalarPressure			            meanArterialCarbonDioxidePartialPressure;
	protected SEScalarPressure			            meanArterialCarbonDioxidePartialPressureDelta;
	protected SEScalarPressure                  meanCentralVenousPressure;
	protected SEScalarVolumePerTime		          meanSkinFlow;
	protected SEScalarPressure                  pulmonaryArterialPressure;
	protected SEScalarPressure                  pulmonaryCapillariesWedgePressure;
	protected SEScalarPressure                  pulmonaryDiastolicArterialPressure;
	protected SEScalarPressure                  pulmonaryMeanArterialPressure;
	protected SEScalarVolumePerTime             pulmonaryMeanCapillaryFlow;
	protected SEScalarVolumePerTime             pulmonaryMeanShuntFlow;
	protected SEScalarPressure                  pulmonarySystolicArterialPressure;
	protected SEScalarFlowResistance            pulmonaryVascularResistance;
	protected SEScalarPressureTimePerVolumeArea pulmonaryVascularResistanceIndex;
	protected SEScalarPressure                  pulsePressure;
	protected SEScalarFlowResistance            systemicVascularResistance;
	protected SEScalarPressure                  systolicArterialPressure;

	public SECardiovascularSystem()
	{
		arterialPressure = null;
		meanArterialPressure = null;
		bloodVolume = null;
		cardiacIndex = null;
		cardiacOutput = null;
		centralVenousPressure = null;
		cerebralBloodFlow = null;
		cerebralPerfusionPressure = null;
		meanCentralVenousPressure = null;
		diastolicArterialPressure = null;	
		heartEjectionFraction = null;
		heartRate = null;
		heartRhythm = null;
		heartStrokeVolume = null;
		intracranialPressure = null;
		pulmonaryArterialPressure = null;
		pulmonaryCapillariesWedgePressure = null;
		pulmonaryDiastolicArterialPressure = null;
		pulmonaryMeanArterialPressure = null;
		pulmonaryMeanCapillaryFlow = null;
		pulmonaryMeanShuntFlow = null;
		pulmonarySystolicArterialPressure = null;
		pulmonaryVascularResistance = null;
		pulmonaryVascularResistanceIndex = null;
		pulsePressure = null;
		systemicVascularResistance = null;
		systolicArterialPressure = null;
	}

	public void reset()
	{
		if (arterialPressure != null)
			arterialPressure.invalidate();
		if (meanArterialPressure != null)
			meanArterialPressure.invalidate();
		if (bloodVolume != null)
			bloodVolume.invalidate();
		if (cardiacIndex != null)
			cardiacIndex.invalidate();
		if (cardiacOutput != null)
			cardiacOutput.invalidate();
		if (centralVenousPressure != null)
			centralVenousPressure.invalidate();
		if (cerebralBloodFlow != null)
			cerebralBloodFlow.invalidate();
		if (cerebralPerfusionPressure != null)
			cerebralPerfusionPressure.invalidate();
		if (meanCentralVenousPressure != null)
			meanCentralVenousPressure.invalidate();
		if (diastolicArterialPressure != null)
			diastolicArterialPressure.invalidate();	
		if (heartEjectionFraction != null)
			heartEjectionFraction.invalidate();
		if (intracranialPressure != null)
			intracranialPressure.invalidate();
		this.heartRhythm = null;
		if (heartStrokeVolume != null)
			heartStrokeVolume.invalidate();
		if (pulmonaryArterialPressure != null)
			pulmonaryArterialPressure.invalidate();
		if (pulmonaryCapillariesWedgePressure != null)
			pulmonaryCapillariesWedgePressure.invalidate();
		if (pulmonaryDiastolicArterialPressure != null)
			pulmonaryDiastolicArterialPressure.invalidate();
		if (pulmonaryMeanArterialPressure != null)
			pulmonaryMeanArterialPressure.invalidate();		
		if (pulmonaryMeanCapillaryFlow != null)
			pulmonaryMeanCapillaryFlow.invalidate();
		if (pulmonaryMeanShuntFlow != null)
			pulmonaryMeanShuntFlow.invalidate();
		if (pulmonarySystolicArterialPressure != null)
			pulmonarySystolicArterialPressure.invalidate();		
		if (pulmonaryVascularResistance != null)
			pulmonaryVascularResistance.invalidate();		
		if (pulmonaryVascularResistanceIndex != null)
			pulmonaryVascularResistanceIndex.invalidate();		
		if (pulsePressure != null)
			pulsePressure.invalidate();
		if (systemicVascularResistance != null)
			systemicVascularResistance.invalidate();		
		if (systolicArterialPressure != null)
			systolicArterialPressure.invalidate();
		if (meanArterialCarbonDioxidePartialPressure != null)
			meanArterialCarbonDioxidePartialPressure.invalidate();
		if (meanArterialCarbonDioxidePartialPressureDelta != null)
			meanArterialCarbonDioxidePartialPressureDelta.invalidate();
		if (meanSkinFlow != null)
			meanSkinFlow.invalidate();
	}

	public boolean load(CardiovascularSystemData in)
	{
		if (in.getArterialPressure() != null)
			getArterialPressure().load(in.getArterialPressure());
		if (in.getMeanArterialPressure() != null)
			getMeanArterialPressure().load(in.getMeanArterialPressure());
		if (in.getBloodVolume() != null)
			getBloodVolume().load(in.getBloodVolume());
		if (in.getCardiacIndex() != null)
			getCardiacIndex().load(in.getCardiacIndex());
		if (in.getCardiacOutput() != null)
			getCardiacOutput().load(in.getCardiacOutput());
		if (in.getCentralVenousPressure() != null)
			getCentralVenousPressure().load(in.getCentralVenousPressure());
		if (in.getCerebralBloodFlow() != null)
			getCerebralBloodFlow().load(in.getCerebralBloodFlow());
		if (in.getCerebralPerfusionPressure() != null)
			getCerebralPerfusionPressure().load(in.getCerebralPerfusionPressure());
		if (in.getMeanCentralVenousPressure() != null)
			getMeanCentralVenousPressure().load(in.getMeanCentralVenousPressure());
		if (in.getDiastolicArterialPressure() != null)
			getDiastolicArterialPressure().load(in.getDiastolicArterialPressure());	
		if (in.getHeartEjectionFraction() != null)
			getHeartEjectionFraction().load(in.getHeartEjectionFraction());
		if (in.getHeartRate() != null)
			getHeartRate().load(in.getHeartRate());		
		if(in.getHeartRhythm()!=null)
			this.setHeartRhythm(in.getHeartRhythm());
		if (in.getHeartStrokeVolume() != null)
			getHeartStrokeVolume().load(in.getHeartStrokeVolume());
		if (in.getIntracranialPressure() != null)
			getIntracranialPressure().load(in.getIntracranialPressure());
		if (in.getPulmonaryArterialPressure() != null)
			getPulmonaryArterialPressure().load(in.getPulmonaryArterialPressure());
		if (in.getPulmonaryCapillariesWedgePressure() != null)
			getPulmonaryCapillariesWedgePressure().load(in.getPulmonaryCapillariesWedgePressure());
		if (in.getPulmonaryDiastolicArterialPressure() != null)
			getPulmonaryDiastolicArterialPressure().load(in.getPulmonaryDiastolicArterialPressure());
		if (in.getPulmonaryMeanArterialPressure() != null)
			getPulmonaryMeanArterialPressure().load(in.getPulmonaryMeanArterialPressure());
		if (in.getPulmonaryMeanCapillaryFlow() != null)
			getPulmonaryMeanCapillaryFlow().load(in.getPulmonaryMeanCapillaryFlow());
		if (in.getPulmonaryMeanShuntFlow() != null)
			getPulmonaryMeanShuntFlow().load(in.getPulmonaryMeanShuntFlow());
		if (in.getPulmonarySystolicArterialPressure() != null)
			getPulmonarySystolicArterialPressure().load(in.getPulmonarySystolicArterialPressure());
		if (in.getPulmonaryVascularResistance() != null)
			getPulmonaryVascularResistance().load(in.getPulmonaryVascularResistance());
		if (in.getPulmonaryVascularResistanceIndex() != null)
			getPulmonaryVascularResistanceIndex().load(in.getPulmonaryVascularResistanceIndex());
		if (in.getPulsePressure() != null)
			getPulsePressure().load(in.getPulsePressure());
		if (in.getSystemicVascularResistance() != null)
			getSystemicVascularResistance().load(in.getSystemicVascularResistance());
		if (in.getSystolicArterialPressure() != null)
			getSystolicArterialPressure().load(in.getSystolicArterialPressure());
		if (in.getMeanArterialCarbonDioxidePartialPressure() != null)
			getMeanArterialCarbonDioxidePartialPressure().load(in.getMeanArterialCarbonDioxidePartialPressure());
		if (in.getMeanArterialCarbonDioxidePartialPressureDelta() != null)
			getMeanArterialCarbonDioxidePartialPressureDelta().load(in.getMeanArterialCarbonDioxidePartialPressureDelta());
		if (in.getMeanSkinFlow() != null)
			getMeanSkinFlow().load(in.getMeanSkinFlow());

		return true;
	}

	public CardiovascularSystemData unload()
	{
		CardiovascularSystemData data = CDMSerializer.objFactory.createCardiovascularSystemData();
		unload(data);
		return data;
	}

	protected void unload(CardiovascularSystemData data)
	{
		if (arterialPressure != null)
			data.setArterialPressure(arterialPressure.unload()); 
		if (meanArterialPressure != null)
			data.setMeanArterialPressure(meanArterialPressure.unload()); 
		if (bloodVolume != null)
			data.setBloodVolume(bloodVolume.unload()); 
		if (cardiacIndex != null)
			data.setCardiacIndex(cardiacIndex.unload()); 
		if (cardiacOutput != null)
			data.setCardiacOutput(cardiacOutput.unload()); 
		if (centralVenousPressure != null)
			data.setCentralVenousPressure(centralVenousPressure.unload()); 
		if (cerebralBloodFlow != null)
			data.setCerebralBloodFlow(cerebralBloodFlow.unload()); 
		if (cerebralPerfusionPressure != null)
			data.setCerebralPerfusionPressure(cerebralPerfusionPressure.unload()); 
		if (meanCentralVenousPressure != null)
			data.setMeanCentralVenousPressure(meanCentralVenousPressure.unload()); 
		if (diastolicArterialPressure != null)
			data.setDiastolicArterialPressure(diastolicArterialPressure.unload()); 
		if (heartEjectionFraction != null)
			data.setHeartEjectionFraction(heartEjectionFraction.unload()); 
		if (heartRate != null)
			data.setHeartRate(heartRate.unload()); 
		if(hasHeartRhythm())
			data.setHeartRhythm(this.heartRhythm);
		if (heartStrokeVolume != null)
			data.setHeartStrokeVolume(heartStrokeVolume.unload());
		if (intracranialPressure != null)
			data.setIntracranialPressure(intracranialPressure.unload());
		if (pulmonaryArterialPressure != null)
			data.setPulmonaryArterialPressure(pulmonaryArterialPressure.unload());
		if (pulmonaryCapillariesWedgePressure != null)
			data.setPulmonaryCapillariesWedgePressure(pulmonaryCapillariesWedgePressure.unload()); 
		if (pulmonaryDiastolicArterialPressure != null)
			data.setPulmonaryDiastolicArterialPressure(pulmonaryDiastolicArterialPressure.unload()); 
		if (pulmonaryMeanArterialPressure != null)
			data.setPulmonaryMeanArterialPressure(pulmonaryMeanArterialPressure.unload()); 
		if (pulmonaryMeanCapillaryFlow != null)
			data.setPulmonaryMeanCapillaryFlow(pulmonaryMeanCapillaryFlow.unload()); 
		if (pulmonaryMeanShuntFlow != null)
			data.setPulmonaryMeanShuntFlow(pulmonaryMeanShuntFlow.unload()); 
		if (pulmonarySystolicArterialPressure != null)
			data.setPulmonarySystolicArterialPressure(pulmonarySystolicArterialPressure.unload()); 
		if (pulmonaryVascularResistance != null)
			data.setPulmonaryVascularResistance(pulmonaryVascularResistance.unload()); 
		if (pulmonaryVascularResistanceIndex != null)
			data.setPulmonaryVascularResistanceIndex(pulmonaryVascularResistanceIndex.unload()); 
		if (pulsePressure != null)
			data.setPulsePressure(pulsePressure.unload());
		if (systemicVascularResistance != null)
			data.setSystemicVascularResistance(systemicVascularResistance.unload());		
		if (systolicArterialPressure != null)
			data.setSystolicArterialPressure(systolicArterialPressure.unload());
		if (getMeanArterialCarbonDioxidePartialPressure() != null)
			data.setMeanArterialCarbonDioxidePartialPressure(meanArterialCarbonDioxidePartialPressure.unload());
		if (getMeanArterialCarbonDioxidePartialPressureDelta() != null)
			data.setMeanArterialCarbonDioxidePartialPressureDelta(meanArterialCarbonDioxidePartialPressureDelta.unload());
		if (getMeanSkinFlow() != null)
			data.setMeanSkinFlow(meanSkinFlow.unload());
	}

	/*
	 * Arterial Pressure
	 */
	public boolean hasArterialPressure()
	{
		return arterialPressure == null ? false : arterialPressure.isValid();
	}
	public SEScalarPressure getArterialPressure()
	{
		if (arterialPressure == null)
			arterialPressure = new SEScalarPressure();
		return arterialPressure;
	}

	/*
	 * Mean Arterial Pressure
	 */
	public boolean hasMeanArterialPressure()
	{
		return meanArterialPressure == null ? false : meanArterialPressure.isValid();
	}
	public SEScalarPressure getMeanArterialPressure()
	{
		if (meanArterialPressure == null)
			meanArterialPressure = new SEScalarPressure();
		return meanArterialPressure;
	}

	/*
	 * Blood Volume
	 */
	public boolean hasBloodVolume()
	{
		return bloodVolume == null ? false : bloodVolume.isValid();
	}
	public SEScalarVolume getBloodVolume()
	{
		if (bloodVolume == null)
			bloodVolume = new SEScalarVolume();
		return bloodVolume;
	}
	
	public boolean hasCardiacIndex()
	{
		return cardiacIndex == null ? false : cardiacIndex.isValid();
	}
	public SEScalarVolumePerTimeArea getCardiacIndex()
	{
		if (cardiacIndex == null)
			cardiacIndex = new SEScalarVolumePerTimeArea();
		return cardiacIndex;
	}

	/*
	 * Cardiac Output
	 */
	public boolean hasCardiacOutput()
	{
		return cardiacOutput == null ? false : cardiacOutput.isValid();
	}
	public SEScalarVolumePerTime getCardiacOutput()
	{
		if (cardiacOutput == null)
			cardiacOutput = new SEScalarVolumePerTime();
		return cardiacOutput;
	}

	/*
	 * Central Venous Pressure
	 */
	public boolean hasCentralVenousPressure()
	{
		return centralVenousPressure == null ? false : centralVenousPressure.isValid();
	}
	public SEScalarPressure getCentralVenousPressure()
	{
		if (centralVenousPressure == null)
			centralVenousPressure = new SEScalarPressure();
		return centralVenousPressure;
	}

	public boolean hasCerebralBloodFlow()
	{
		return cerebralBloodFlow == null ? false : cerebralBloodFlow.isValid();
	}
	public SEScalarVolumePerTime getCerebralBloodFlow()
	{
		if (cerebralBloodFlow == null)
			cerebralBloodFlow = new SEScalarVolumePerTime();
		return cerebralBloodFlow;
	}

	public boolean hasCerebralPerfusionPressure()
	{
		return cerebralPerfusionPressure == null ? false : cerebralPerfusionPressure.isValid();
	}
	public SEScalarPressure getCerebralPerfusionPressure()
	{
		if (cerebralPerfusionPressure == null)
			cerebralPerfusionPressure = new SEScalarPressure();
		return cerebralPerfusionPressure;
	}

	/*
	 * Mean Central Venous Pressure
	 */
	public boolean hasMeanCentralVenousPressure()
	{
		return meanCentralVenousPressure == null ? false : meanCentralVenousPressure.isValid();
	}
	public SEScalarPressure getMeanCentralVenousPressure()
	{
		if (meanCentralVenousPressure == null)
			meanCentralVenousPressure = new SEScalarPressure();
		return meanCentralVenousPressure;
	}

	/*
	 * Diastolic Arterial Pressure
	 */
	public boolean hasDiastolicArterialPressure()
	{
		return diastolicArterialPressure == null ? false : diastolicArterialPressure.isValid();
	}
	public SEScalarPressure getDiastolicArterialPressure()
	{
		if (diastolicArterialPressure == null)
			diastolicArterialPressure = new SEScalarPressure();
		return diastolicArterialPressure;
	}

	/*
	 * Heart Ejection Fraction
	 */
	public boolean hasHeartEjectionFraction()
	{
		return heartEjectionFraction == null ? false : heartEjectionFraction.isValid();
	}
	public SEScalarFraction getHeartEjectionFraction()
	{
		if (heartEjectionFraction == null)
			heartEjectionFraction = new SEScalarFraction();
		return heartEjectionFraction;
	}

	/*
	 * Heart Rate
	 */
	public boolean hasHeartRate()
	{
		return heartRate == null ? false : heartRate.isValid();
	}
	public SEScalarFrequency getHeartRate()
	{
		if (heartRate == null)
			heartRate = new SEScalarFrequency();
		return heartRate;
	}

	/*
	 * Heart Rhythm
	 */
	public EnumHeartRhythm	    getHeartRhythm() { return this.heartRhythm;}
	public void    				setHeartRhythm(EnumHeartRhythm heartRhythm){this.heartRhythm=heartRhythm;}
	public boolean              hasHeartRhythm(){return this.heartRhythm==null?false:true;}

	/*
	 * Heart Stroke Volume
	 */
	public boolean hasHeartStrokeVolume()
	{
		return heartStrokeVolume == null ? false : heartStrokeVolume.isValid();
	}
	public SEScalarVolume getHeartStrokeVolume()
	{
		if (heartStrokeVolume == null)
			heartStrokeVolume = new SEScalarVolume();
		return heartStrokeVolume;
	}

	public boolean hasIntracranialPressure()
	{
		return intracranialPressure == null ? false : intracranialPressure.isValid();
	}
	public SEScalarPressure getIntracranialPressure()
	{
		if (intracranialPressure == null)
			intracranialPressure = new SEScalarPressure();
		return intracranialPressure;
	}

	/*
	 * Pulmonary Arterial Pressure
	 */
	public boolean hasPulmonaryArterialPressure()
	{
		return pulmonaryArterialPressure == null ? false : pulmonaryArterialPressure.isValid();
	}
	public SEScalarPressure getPulmonaryArterialPressure()
	{
		if (pulmonaryArterialPressure == null)
			pulmonaryArterialPressure = new SEScalarPressure();
		return pulmonaryArterialPressure;
	}

	/*
	 * Pulmonary Capillaries Wedge Pressure
	 */
	public boolean hasPulmonaryCapillariesWedgePressure()
	{
		return pulmonaryCapillariesWedgePressure == null ? false : pulmonaryCapillariesWedgePressure.isValid();
	}
	public SEScalarPressure getPulmonaryCapillariesWedgePressure()
	{
		if (pulmonaryCapillariesWedgePressure == null)
			pulmonaryCapillariesWedgePressure = new SEScalarPressure();
		return pulmonaryCapillariesWedgePressure;
	}

	/*
	 * Pulmonary Diastolic Arterial Pressure
	 */
	public boolean hasPulmonaryDiastolicArterialPressure()
	{
		return pulmonaryDiastolicArterialPressure == null ? false : pulmonaryDiastolicArterialPressure.isValid();
	}
	public SEScalarPressure getPulmonaryDiastolicArterialPressure()
	{
		if (pulmonaryDiastolicArterialPressure == null)
			pulmonaryDiastolicArterialPressure = new SEScalarPressure();
		return pulmonaryDiastolicArterialPressure;
	}

	/*
	 * Pulmonary Mean Arterial Pressure
	 */
	public boolean hasPulmonaryMeanArterialPressure()
	{
		return pulmonaryMeanArterialPressure == null ? false : pulmonaryMeanArterialPressure.isValid();
	}
	public SEScalarPressure getPulmonaryMeanArterialPressure()
	{
		if (pulmonaryMeanArterialPressure == null)
			pulmonaryMeanArterialPressure = new SEScalarPressure();
		return pulmonaryMeanArterialPressure;
	}

	/*
	 * Pulmonary Mean Capillary Flow
	 */
	public boolean hasPulmonaryMeanCapillaryFlow()
	{
		return pulmonaryMeanCapillaryFlow == null ? false : pulmonaryMeanCapillaryFlow.isValid();
	}
	public SEScalarVolumePerTime getPulmonaryMeanCapillaryFlow()
	{
		if (pulmonaryMeanCapillaryFlow == null)
			pulmonaryMeanCapillaryFlow = new SEScalarVolumePerTime();
		return pulmonaryMeanCapillaryFlow;
	}

	/*
	 * Pulmonary Mean Shunt Flow
	 */
	public boolean hasPulmonaryMeanShuntFlow()
	{
		return pulmonaryMeanShuntFlow == null ? false : pulmonaryMeanShuntFlow.isValid();
	}
	public SEScalarVolumePerTime getPulmonaryMeanShuntFlow()
	{
		if (pulmonaryMeanShuntFlow == null)
			pulmonaryMeanShuntFlow = new SEScalarVolumePerTime();
		return pulmonaryMeanShuntFlow;
	}

	/*
	 * Pulmonary Systolic Arterial Pressure
	 */
	public boolean hasPulmonarySystolicArterialPressure()
	{
		return pulmonarySystolicArterialPressure == null ? false : pulmonarySystolicArterialPressure.isValid();
	}
	public SEScalarPressure getPulmonarySystolicArterialPressure()
	{
		if (pulmonarySystolicArterialPressure == null)
			pulmonarySystolicArterialPressure = new SEScalarPressure();
		return pulmonarySystolicArterialPressure;
	}
	
	public boolean hasPulmonaryVascularResistance()
	{
		return pulmonaryVascularResistance == null ? false : pulmonaryVascularResistance.isValid();
	}
	public SEScalarFlowResistance getPulmonaryVascularResistance()
	{
		if (pulmonaryVascularResistance == null)
			pulmonaryVascularResistance = new SEScalarFlowResistance();
		return pulmonaryVascularResistance;
	}
	
	public boolean hasPulmonaryVascularResistanceIndex()
	{
		return pulmonaryVascularResistanceIndex == null ? false : pulmonaryVascularResistanceIndex.isValid();
	}
	public SEScalarPressureTimePerVolumeArea getPulmonaryVascularResistanceIndex()
	{
		if (pulmonaryVascularResistanceIndex == null)
			pulmonaryVascularResistanceIndex = new SEScalarPressureTimePerVolumeArea();
		return pulmonaryVascularResistanceIndex;
	}

	/*
	 * Pulse Pressure
	 */
	public boolean hasPulsePressure()
	{
		return pulsePressure == null ? false : pulsePressure.isValid();
	}
	public SEScalarPressure getPulsePressure()
	{
		if (pulsePressure == null)
			pulsePressure = new SEScalarPressure();
		return pulsePressure;
	}

	/*
	 * Systemic Vascular Resistance
	 */
	public boolean hasSystemicVascularResistance()
	{
		return systemicVascularResistance == null ? false : systemicVascularResistance.isValid();
	}
	public SEScalarFlowResistance getSystemicVascularResistance()
	{
		if (systemicVascularResistance == null)
			systemicVascularResistance = new SEScalarFlowResistance();
		return systemicVascularResistance;
	}

	/*
	 * Systolic Arterial Pressure
	 */
	public boolean hasSystolicArterialPressure()
	{
		return systolicArterialPressure == null ? false : systolicArterialPressure.isValid();
	}
	public SEScalarPressure getSystolicArterialPressure()
	{
		if (systolicArterialPressure == null)
			systolicArterialPressure = new SEScalarPressure();
		return systolicArterialPressure;
	}

	/*
	 * Mean Arterial Carbon Dioxide Partial Pressure
	 */
	public boolean hasMeanArterialCarbonDioxidePartialPressure()
	{
		return meanArterialCarbonDioxidePartialPressure == null ? false : meanArterialCarbonDioxidePartialPressure.isValid();
	}
	public SEScalarPressure getMeanArterialCarbonDioxidePartialPressure()
	{
		if (meanArterialCarbonDioxidePartialPressure == null)
			meanArterialCarbonDioxidePartialPressure = new SEScalarPressure();
		return meanArterialCarbonDioxidePartialPressure;
	}

	/*
	 * Mean Arterial Carbon Dioxide Partial Pressure Delta
	 */
	public boolean hasMeanArterialCarbonDioxidePartialPressureDelta()
	{
		return meanArterialCarbonDioxidePartialPressureDelta == null ? false : meanArterialCarbonDioxidePartialPressureDelta.isValid();
	}
	public SEScalarPressure getMeanArterialCarbonDioxidePartialPressureDelta()
	{
		if (meanArterialCarbonDioxidePartialPressureDelta == null)
			meanArterialCarbonDioxidePartialPressureDelta = new SEScalarPressure();
		return meanArterialCarbonDioxidePartialPressureDelta;
	}

	/*
	 * Mean Skin Flow
	 */
	public boolean hasMeanSkinFlow()
	{
		return meanSkinFlow == null ? false : meanSkinFlow.isValid();
	}
	public SEScalarVolumePerTime getMeanSkinFlow()
	{
		if (meanSkinFlow == null)
			meanSkinFlow = new SEScalarVolumePerTime();
		return meanSkinFlow;
	}
}
