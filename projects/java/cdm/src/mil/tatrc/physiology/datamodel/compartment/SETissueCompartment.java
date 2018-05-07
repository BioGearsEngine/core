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
package mil.tatrc.physiology.datamodel.compartment;

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.TissueCompartmentData;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalarElectricPotential;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarMass;
import mil.tatrc.physiology.datamodel.properties.SEScalarMassPerMass;
import mil.tatrc.physiology.datamodel.properties.SEScalarVolume;

public class SETissueCompartment extends SECompartment
{
	protected SEScalarMassPerMass acidicPhospohlipidConcentration;	
	protected SEScalarVolume      matrixVolume;
  protected SEScalarElectricPotential		membranePotential;
  protected SEScalarFraction    neutralLipidsVolumeFraction;
  protected SEScalarFraction    neutralPhospholipidsVolumeFraction;  
  protected SEScalar            tissueToPlasmaAlbuminRatio;
  protected SEScalar            tissueToPlasmaAlphaAcidGlycoproteinRatio;
  protected SEScalar            tissueToPlasmaLipoproteinRatio;
  protected SEScalarMass        totalMass;
  
	public SETissueCompartment()
	{
		acidicPhospohlipidConcentration = null;
		matrixVolume = null;
    membranePotential = null;
		neutralLipidsVolumeFraction = null;
		neutralPhospholipidsVolumeFraction = null;    
		tissueToPlasmaAlbuminRatio = null;
		tissueToPlasmaAlphaAcidGlycoproteinRatio = null;
		tissueToPlasmaLipoproteinRatio = null;
		totalMass = null;
	}
	
	public void reset()
	{
	  super.reset();
		if (acidicPhospohlipidConcentration != null)
			acidicPhospohlipidConcentration.invalidate();
		if (matrixVolume != null)
			matrixVolume.invalidate();
    if(membranePotential != null)
    	membranePotential.invalidate();
		if (neutralLipidsVolumeFraction != null)
			neutralLipidsVolumeFraction.invalidate();
		if (neutralPhospholipidsVolumeFraction != null)
			neutralPhospholipidsVolumeFraction.invalidate();
		if (tissueToPlasmaAlbuminRatio != null)
			tissueToPlasmaAlbuminRatio.invalidate();
		if (tissueToPlasmaAlphaAcidGlycoproteinRatio != null)
			tissueToPlasmaAlphaAcidGlycoproteinRatio.invalidate();
		if (tissueToPlasmaLipoproteinRatio != null)
			tissueToPlasmaLipoproteinRatio.invalidate();
		if (totalMass != null)
			totalMass.invalidate();
	}
	
	public boolean load(TissueCompartmentData in)
	{
		super.load(in);
		if(in.getAcidicPhospohlipidConcentration()!=null)
			getAcidicPhospohlipidConcentration().load(in.getAcidicPhospohlipidConcentration());
		if(in.getMatrixVolume()!=null)
			getMatrixVolume().load(in.getMatrixVolume());
		if(in.getMembranePotential()!=null)
    	getMembranePotential().load(in.getMembranePotential());
		if(in.getNeutralLipidsVolumeFraction()!=null)
			getNeutralLipidsVolumeFraction().load(in.getNeutralLipidsVolumeFraction());
		if(in.getNeutralPhospholipidsVolumeFraction()!=null)
			getNeutralPhospholipidsVolumeFraction().load(in.getNeutralPhospholipidsVolumeFraction());
		if(in.getTissueToPlasmaAlbuminRatio()!=null)
			getTissueToPlasmaAlbuminRatio().load(in.getTissueToPlasmaAlbuminRatio());
		if(in.getTissueToPlasmaAlphaAcidGlycoproteinRatio()!=null)
			getTissueToPlasmaAlphaAcidGlycoproteinRatio().load(in.getTissueToPlasmaAlphaAcidGlycoproteinRatio());
		if(in.getTissueToPlasmaLipoproteinRatio()!=null)
			getTissueToPlasmaLipoproteinRatio().load(in.getTissueToPlasmaLipoproteinRatio());
		if(in.getTotalMass()!=null)
			getTotalMass().load(in.getTotalMass());
			
		return true;
	}
	
	public TissueCompartmentData unload()
	{
		TissueCompartmentData to = CDMSerializer.objFactory.createTissueCompartmentData();
		unload(to);
		return to;		
	}
	
	protected void unload(TissueCompartmentData data)
	{
	  super.unload(data);
	  if(hasAcidicPhospohlipidConcentration())
	  	data.setAcidicPhospohlipidConcentration(this.acidicPhospohlipidConcentration.unload());
	  if(hasMatrixVolume())
	  	data.setMatrixVolume(this.matrixVolume.unload());
	  if (hasMembranePotential())
    	data.setMembranePotential(membranePotential.unload());
	  if(hasNeutralLipidsVolumeFraction())
	  	data.setNeutralLipidsVolumeFraction(this.neutralLipidsVolumeFraction.unload());
	  if(hasNeutralPhospholipidsVolumeFraction())
	  	data.setNeutralPhospholipidsVolumeFraction(this.neutralPhospholipidsVolumeFraction.unload());
	  if(hasTissueToPlasmaAlbuminRatio())
	  	data.setTissueToPlasmaAlbuminRatio(this.tissueToPlasmaAlbuminRatio.unload());
	  if(hasTissueToPlasmaAlphaAcidGlycoproteinRatio())
	  	data.setTissueToPlasmaAlphaAcidGlycoproteinRatio(this.tissueToPlasmaAlphaAcidGlycoproteinRatio.unload());
	  if(hasTissueToPlasmaLipoproteinRatio())
	  	data.setTissueToPlasmaLipoproteinRatio(this.tissueToPlasmaLipoproteinRatio.unload());
	  if(hasTotalMass())
	  	data.setTotalMass(this.totalMass.unload());
	}

	public SEScalarMassPerMass getAcidicPhospohlipidConcentration() 
	{
		if (acidicPhospohlipidConcentration == null)
			acidicPhospohlipidConcentration = new SEScalarMassPerMass();
		return acidicPhospohlipidConcentration;
	}
	public boolean hasAcidicPhospohlipidConcentration()
	{
		return acidicPhospohlipidConcentration == null ? false : acidicPhospohlipidConcentration.isValid();
	}

	public SEScalarVolume getMatrixVolume() 
  {
    if (matrixVolume == null)
    	matrixVolume = new SEScalarVolume();
    return matrixVolume;
  }
  public boolean hasMatrixVolume()
  {
    return matrixVolume == null ? false : matrixVolume.isValid();
  }
  
  public boolean hasMembranePotential()
  {
    return membranePotential == null ? false : membranePotential.isValid();
  }
  public SEScalarElectricPotential getMembranePotential()
  {
    if (membranePotential == null)
    	membranePotential = new SEScalarElectricPotential();
    return membranePotential;
  }
  
  public SEScalarFraction getNeutralLipidsVolumeFraction() 
  {
    if (neutralLipidsVolumeFraction == null)
    	neutralLipidsVolumeFraction = new SEScalarFraction();
    return neutralLipidsVolumeFraction;
  }
  public boolean hasNeutralLipidsVolumeFraction()
  {
    return neutralLipidsVolumeFraction == null ? false : neutralLipidsVolumeFraction.isValid();
  }
  
  public SEScalarFraction getNeutralPhospholipidsVolumeFraction() 
  {
    if (neutralPhospholipidsVolumeFraction == null)
    	neutralPhospholipidsVolumeFraction = new SEScalarFraction();
    return neutralPhospholipidsVolumeFraction;
  }
  public boolean hasNeutralPhospholipidsVolumeFraction()
  {
    return neutralPhospholipidsVolumeFraction == null ? false : neutralPhospholipidsVolumeFraction.isValid();
  }
  
  public SEScalar getTissueToPlasmaAlbuminRatio() 
  {
    if (tissueToPlasmaAlbuminRatio == null)
    	tissueToPlasmaAlbuminRatio = new SEScalar();
    return tissueToPlasmaAlbuminRatio;
  }
  public boolean hasTissueToPlasmaAlbuminRatio()
  {
    return tissueToPlasmaAlbuminRatio == null ? false : tissueToPlasmaAlbuminRatio.isValid();
  }
  
  public SEScalar getTissueToPlasmaAlphaAcidGlycoproteinRatio() 
  {
    if (tissueToPlasmaAlphaAcidGlycoproteinRatio == null)
    	tissueToPlasmaAlphaAcidGlycoproteinRatio = new SEScalar();
    return tissueToPlasmaAlphaAcidGlycoproteinRatio;
  }
  public boolean hasTissueToPlasmaAlphaAcidGlycoproteinRatio()
  {
    return tissueToPlasmaAlphaAcidGlycoproteinRatio == null ? false : tissueToPlasmaAlphaAcidGlycoproteinRatio.isValid();
  }
  
  public SEScalar getTissueToPlasmaLipoproteinRatio() 
  {
    if (tissueToPlasmaLipoproteinRatio == null)
    	tissueToPlasmaLipoproteinRatio = new SEScalar();
    return tissueToPlasmaLipoproteinRatio;
  }
  public boolean hasTissueToPlasmaLipoproteinRatio()
  {
    return tissueToPlasmaLipoproteinRatio == null ? false : tissueToPlasmaLipoproteinRatio.isValid();
  }
  
  public SEScalarMass getTotalMass() 
  {
    if (totalMass == null)
    	totalMass = new SEScalarMass();
    return totalMass;
  }
  public boolean hasTotalMass()
  {
    return totalMass == null ? false : totalMass.isValid();
  }
}
