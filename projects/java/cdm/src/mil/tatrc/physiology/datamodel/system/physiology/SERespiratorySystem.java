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
import mil.tatrc.physiology.datamodel.bind.RespiratorySystemData;
import mil.tatrc.physiology.datamodel.properties.*;
import mil.tatrc.physiology.datamodel.system.SESystem;

public class SERespiratorySystem extends SEPhysiologySystem implements SESystem
{
	protected SEScalarPressure        alveolarArterialGradient;
	protected SEScalarPressure        carricoIndex;
  protected SEScalarFraction     		endTidalCarbonDioxideFraction;
  protected SEScalarPressure     		endTidalCarbonDioxidePressure;
	protected SEScalarVolumePerTime 	expiratoryFlow;
	protected SEScalar      			    inspiratoryExpiratoryRatio;
	protected SEScalarVolumePerTime 	inspiratoryFlow;
	protected SEScalarFlowCompliance	pulmonaryCompliance;
	protected SEScalarFlowResistance  pulmonaryResistance;
	protected SEScalarPressure        respirationDriverPressure;
	protected SEScalarPressure        respirationMusclePressure;
	protected SEScalarFrequency     	respirationRate;
	protected SEScalar					      specificVentilation;
	protected SEScalarVolume        	tidalVolume;
	protected SEScalarVolumePerTime 	totalAlveolarVentilation;
	protected SEScalarVolumePerTime 	totalDeadSpaceVentilation;
	protected SEScalarVolume	   		  totalLungVolume;
	protected SEScalarVolumePerTime 	totalPulmonaryVentilation;
	protected SEScalarPressure			  transpulmonaryPressure;
	
	
	public SERespiratorySystem()
	{		
		alveolarArterialGradient = null;
		carricoIndex = null;	
	  endTidalCarbonDioxideFraction = null;
	  endTidalCarbonDioxidePressure = null;
	  expiratoryFlow = null;
		inspiratoryExpiratoryRatio = null;
		inspiratoryFlow = null;
		pulmonaryCompliance = null;
		pulmonaryResistance = null;
		respirationDriverPressure = null;
		respirationMusclePressure = null;
		respirationRate = null;
		specificVentilation = null;
		tidalVolume = null;
		totalAlveolarVentilation = null;
		totalDeadSpaceVentilation = null;
		totalLungVolume = null;
		totalPulmonaryVentilation = null;
		transpulmonaryPressure = null;
	}
	
	public void reset()
	{
		if (alveolarArterialGradient != null)
			alveolarArterialGradient.invalidate();
		if (carricoIndex != null)
			carricoIndex.invalidate();
		if (endTidalCarbonDioxideFraction != null)
			endTidalCarbonDioxideFraction.invalidate();
		if (endTidalCarbonDioxidePressure != null)
			endTidalCarbonDioxidePressure.invalidate();
		if (expiratoryFlow != null)
			expiratoryFlow.invalidate();		
		if (inspiratoryExpiratoryRatio != null)
			inspiratoryExpiratoryRatio.invalidate();
		if (inspiratoryFlow != null)
			inspiratoryFlow.invalidate();
		if (pulmonaryCompliance != null)
			pulmonaryCompliance.invalidate();
		if (pulmonaryResistance != null)
			pulmonaryResistance.invalidate();
		if (respirationDriverPressure != null)
			respirationDriverPressure.invalidate();
		if (respirationMusclePressure != null)
			respirationMusclePressure.invalidate();
		if (respirationRate != null)
			respirationRate.invalidate();
		if (specificVentilation != null)
			specificVentilation.invalidate();
		if (tidalVolume != null)
			tidalVolume.invalidate();
		if (totalAlveolarVentilation != null)
			totalAlveolarVentilation.invalidate();
		if (totalDeadSpaceVentilation != null)
			totalDeadSpaceVentilation.invalidate();
		if (totalLungVolume != null)
			totalLungVolume.invalidate();
		if (totalPulmonaryVentilation != null)
			totalPulmonaryVentilation.invalidate();
		if (transpulmonaryPressure != null)
			transpulmonaryPressure.invalidate();
	}
	
	public boolean load(RespiratorySystemData in)
	{
	  if (in.getAlveolarArterialGradient() != null)
	  	getAlveolarArterialGradient().load(in.getAlveolarArterialGradient());
	  if (in.getCarricoIndex() != null)
	  	getCarricoIndex().load(in.getCarricoIndex());
	  if (in.getEndTidalCarbonDioxideFraction() != null)
			getEndTidalCarbonDioxideFraction().load(in.getEndTidalCarbonDioxideFraction());
	  if (in.getEndTidalCarbonDioxidePressure() != null)
	  	getEndTidalCarbonDioxidePressure().load(in.getEndTidalCarbonDioxidePressure());
	  if (in.getExpiratoryFlow() != null)
	  	getExpiratoryFlow().load(in.getExpiratoryFlow());
		if (in.getInspiratoryExpiratoryRatio() != null)
			getInspiratoryExpiratoryRatio().load(in.getInspiratoryExpiratoryRatio()); 
		if (in.getInspiratoryFlow() != null)
			getInspiratoryFlow().load(in.getInspiratoryFlow()); 
		if (in.getPulmonaryCompliance() != null)
			getPulmonaryCompliance().load(in.getPulmonaryCompliance()); 
		if (in.getRespirationDriverPressure() != null)
			getRespirationDriverPressure().load(in.getRespirationDriverPressure());
		if (in.getRespirationMusclePressure() != null)
			getRespirationMusclePressure().load(in.getRespirationMusclePressure());
		if (in.getRespirationRate() != null)
      getRespirationRate().load(in.getRespirationRate());
		if (in.getSpecificVentilation() != null)
			getSpecificVentilation().load(in.getSpecificVentilation());
		if (in.getTidalVolume() != null)
			getTidalVolume().load(in.getTidalVolume()); 
		if (in.getTotalAlveolarVentilation() != null)
			getTotalAlveolarVentilation().load(in.getTotalAlveolarVentilation());
		if (in.getTotalDeadSpaceVentilation() != null)
			getTotalDeadSpaceVentilation().load(in.getTotalDeadSpaceVentilation());
		if (in.getTotalLungVolume() != null)
			getTotalLungVolume().load(in.getTotalLungVolume());
		if (in.getTotalPulmonaryVentilation() != null)
			getTotalPulmonaryVentilation().load(in.getTotalPulmonaryVentilation()); 
		if (in.getTranspulmonaryPressure() != null)
			getTranspulmonaryPressure().load(in.getTranspulmonaryPressure());		

		return true;
	}
	
	public RespiratorySystemData unload()
	{
		RespiratorySystemData data = CDMSerializer.objFactory.createRespiratorySystemData();
		unload(data);
		return data;
	}
	
	protected void unload(RespiratorySystemData data)
	{
	  if (alveolarArterialGradient != null)
			data.setAlveolarArterialGradient(alveolarArterialGradient.unload());		
	  if (carricoIndex != null)
			data.setCarricoIndex(carricoIndex.unload());		
	  if (endTidalCarbonDioxideFraction != null)
			data.setEndTidalCarbonDioxideFraction(endTidalCarbonDioxideFraction.unload());		
	  if (endTidalCarbonDioxidePressure != null)
			data.setEndTidalCarbonDioxidePressure(endTidalCarbonDioxidePressure.unload());		
	  if (expiratoryFlow != null)
			data.setExpiratoryFlow(expiratoryFlow.unload());		
		if (inspiratoryExpiratoryRatio != null)
			data.setInspiratoryExpiratoryRatio(inspiratoryExpiratoryRatio.unload());		
		if (inspiratoryFlow != null)
			data.setInspiratoryFlow(inspiratoryFlow.unload());		
		if (pulmonaryCompliance != null)
			data.setPulmonaryCompliance(pulmonaryCompliance.unload());
		if (pulmonaryResistance != null)
			data.setPulmonaryResistance(pulmonaryResistance.unload());	
		if (respirationDriverPressure != null)
      data.setRespirationDriverPressure(respirationDriverPressure.unload());
		if (respirationMusclePressure != null)
      data.setRespirationMusclePressure(respirationMusclePressure.unload());
		if (respirationRate != null)
			data.setRespirationRate(respirationRate.unload());
		if (specificVentilation != null)
			data.setSpecificVentilation(specificVentilation.unload());		
		if (tidalVolume != null)
			data.setTidalVolume(tidalVolume.unload());
		if (totalAlveolarVentilation != null)
			data.setTotalAlveolarVentilation(totalAlveolarVentilation.unload());
		if (totalDeadSpaceVentilation != null)
			data.setTotalDeadSpaceVentilation(totalDeadSpaceVentilation.unload());
		if (totalLungVolume != null)
			data.setTotalLungVolume(totalLungVolume.unload());		
		if (totalPulmonaryVentilation != null)
			data.setTotalPulmonaryVentilation(totalPulmonaryVentilation.unload());
		if (transpulmonaryPressure != null)
			data.setTranspulmonaryPressure(transpulmonaryPressure.unload());
	}
	
	public boolean hasAlveolarArterialGradient()
	{
		return alveolarArterialGradient == null ? false : alveolarArterialGradient.isValid();
	}
	public SEScalarPressure getAlveolarArterialGradient()
	{
		if (alveolarArterialGradient == null)
			alveolarArterialGradient = new SEScalarPressure();
		return alveolarArterialGradient;
	}
	
	public boolean hasCarricoIndex()
	{
		return carricoIndex == null ? false : carricoIndex.isValid();
	}
	public SEScalarPressure getCarricoIndex()
	{
		if (carricoIndex == null)
			carricoIndex = new SEScalarPressure();
		return carricoIndex;
	}
	
	/*
	 * End Tidal Carbon Dioxide Fraction
	 */
	public boolean hasEndTidalCarbonDioxideFraction()
	{
		return endTidalCarbonDioxideFraction == null ? false : endTidalCarbonDioxideFraction.isValid();
	}
	public SEScalarFraction getEndTidalCarbonDioxideFraction()
	{
		if (endTidalCarbonDioxideFraction == null)
			endTidalCarbonDioxideFraction = new SEScalarFraction();
		return endTidalCarbonDioxideFraction;
	}
	
	public boolean hasEndTidalCarbonDioxidePressure()
	{
		return endTidalCarbonDioxidePressure == null ? false : endTidalCarbonDioxidePressure.isValid();
	}
	public SEScalarPressure getEndTidalCarbonDioxidePressure()
	{
		if (endTidalCarbonDioxidePressure == null)
			endTidalCarbonDioxidePressure = new SEScalarPressure();
		return endTidalCarbonDioxidePressure;
	}
	
	public boolean hasExpiratoryFlow()
	{
		return expiratoryFlow == null ? false : expiratoryFlow.isValid();
	}
	public SEScalarVolumePerTime getExpiratoryFlow()
	{
		if (expiratoryFlow == null)
			expiratoryFlow = new SEScalarVolumePerTime();
		return expiratoryFlow;
	}
	
	/*
	 * Inspiratory Expiratory Ratio
	 */
	public boolean hasInspiratoryExpiratoryRatio()
	{
		return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
	}
	public SEScalar getInspiratoryExpiratoryRatio()
	{
		if (inspiratoryExpiratoryRatio == null)
			inspiratoryExpiratoryRatio = new SEScalar();
		return inspiratoryExpiratoryRatio;
	}
	
	public boolean hasInspiratoryFlow()
	{
		return inspiratoryFlow == null ? false : inspiratoryFlow.isValid();
	}
	public SEScalarVolumePerTime getInspiratoryFlow()
	{
		if (inspiratoryFlow == null)
			inspiratoryFlow = new SEScalarVolumePerTime();
		return inspiratoryFlow;
	}
	
	/*
	 * Pulmonary Compliance
	 */
	public boolean hasPulmonaryCompliance()
	{
		return pulmonaryCompliance == null ? false : pulmonaryCompliance.isValid();
	}
	public SEScalarFlowCompliance getPulmonaryCompliance()
	{
		if (pulmonaryCompliance == null)
			pulmonaryCompliance = new SEScalarFlowCompliance();
		return pulmonaryCompliance;
	}
	
	/*
	 * Pulmonary Resistance
	 */
	public boolean hasPulmonaryResistance()
	{
		return pulmonaryResistance == null ? false : pulmonaryResistance.isValid();
	}
	public SEScalarFlowResistance getPulmonaryResistance()
	{
		if (pulmonaryResistance == null)
			pulmonaryResistance = new SEScalarFlowResistance();
		return pulmonaryResistance;
	}
	
	/*
	 * Respiration Rate
	 */
	public boolean hasRespirationDriverPressure()
	{
		return respirationDriverPressure == null ? false : respirationDriverPressure.isValid();
	}
	public SEScalarPressure getRespirationDriverPressure()
	{
		if (respirationDriverPressure == null)
		  respirationDriverPressure = new SEScalarPressure();
		return respirationDriverPressure;
	}
	
	public boolean hasRespirationMusclePressure()
	{
		return respirationMusclePressure == null ? false : respirationMusclePressure.isValid();
	}
	public SEScalarPressure getRespirationMusclePressure()
	{
		if (respirationMusclePressure == null)
		  respirationMusclePressure = new SEScalarPressure();
		return respirationMusclePressure;
	}
	
	public boolean hasRespirationRate()
  {
    return respirationRate == null ? false : respirationRate.isValid();
  }
  public SEScalarFrequency getRespirationRate()
  {
    if (respirationRate == null)
      respirationRate = new SEScalarFrequency();
    return respirationRate;
  }
	
	/*
	 * Specific Ventilation
	 */
	public boolean hasSpecificVentilation()
	{
		return specificVentilation == null ? false : specificVentilation.isValid();
	}
	public SEScalar getSpecificVentilation()
	{
		if (specificVentilation == null)
			specificVentilation = new SEScalar();
		return specificVentilation;
	}
	
	/*
	 * Tidal Volume
	 */
	public boolean hasTidalVolume()
	{
		return tidalVolume == null ? false : tidalVolume.isValid();
	}
	public SEScalarVolume getTidalVolume()
	{
		if (tidalVolume == null)
			tidalVolume = new SEScalarVolume();
		return tidalVolume;
	}
	
	/*
	 * Total Alveolar Ventilation
	 */
	public boolean hasTotalAlveolarVentilation()
	{
		return totalAlveolarVentilation == null ? false : totalAlveolarVentilation.isValid();
	}
	public SEScalarVolumePerTime getTotalAlveolarVentilation()
	{
		if (totalAlveolarVentilation == null)
			totalAlveolarVentilation = new SEScalarVolumePerTime();
		return totalAlveolarVentilation;
	}
	
	/*
	 * Total Dead Space Ventilation
	 */
	public boolean hasTotalDeadSpaceVentilation()
	{
		return totalDeadSpaceVentilation == null ? false : totalDeadSpaceVentilation.isValid();
	}
	public SEScalarVolumePerTime getTotalDeadSpaceVentilation()
	{
		if (totalDeadSpaceVentilation == null)
			totalDeadSpaceVentilation = new SEScalarVolumePerTime();
		return totalDeadSpaceVentilation;
	}
	
	/*
	 * Total Lung Volume
	 */
	public boolean hasTotalLungVolume()
	{
		return totalLungVolume == null ? false : totalLungVolume.isValid();
	}
	public SEScalarVolume getTotalLungVolume()
	{
		if (totalLungVolume == null)
			totalLungVolume = new SEScalarVolume();
		return totalLungVolume;
	}
	
	/*
	 * Total Pulmonary Ventilation
	 */
	public boolean hasTotalPulmonaryVentilation()
	{
		return totalPulmonaryVentilation == null ? false : totalPulmonaryVentilation.isValid();
	}
	public SEScalarVolumePerTime getTotalPulmonaryVentilation()
	{
		if (totalPulmonaryVentilation == null)
			totalPulmonaryVentilation = new SEScalarVolumePerTime();
		return totalPulmonaryVentilation;
	}
	
	/*
	 * Transpulmonary Pressure
	 */
	public boolean hasTranspulmonaryPressure()
	{
		return transpulmonaryPressure == null ? false : transpulmonaryPressure.isValid();
	}
	public SEScalarPressure getTranspulmonaryPressure()
	{
		if (transpulmonaryPressure == null)
			transpulmonaryPressure = new SEScalarPressure();
		return transpulmonaryPressure;
	}
}
