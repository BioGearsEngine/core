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
import mil.tatrc.physiology.datamodel.bind.PupillaryResponseData;
import mil.tatrc.physiology.datamodel.properties.SEScalarFraction;
import mil.tatrc.physiology.datamodel.properties.SEScalarNeg1To1;

public class SEPupillaryResponse
{
  protected SEScalarNeg1To1  reactivityModifier;
  protected SEScalarNeg1To1  shapeModifier;
  protected SEScalarNeg1To1  sizeModifier;
  
  public SEPupillaryResponse()
  {
  	reactivityModifier = null;
  	shapeModifier = null;
  	sizeModifier = null;
  }

  public void reset()
  {
    if (reactivityModifier != null)
    	reactivityModifier.invalidate();
    if (shapeModifier != null)
    	shapeModifier.invalidate();
    if (sizeModifier != null)
    	sizeModifier.invalidate();
  }

  public boolean load(PupillaryResponseData in)
  {
    if (in.getReactivityModifier() != null)
    	getReactivityModifier().load(in.getReactivityModifier());
    if (in.getShapeModifier() != null)
      getShapeModifier().load(in.getShapeModifier());
    if (in.getSizeModifier() != null)
    	getSizeModifier().load(in.getSizeModifier());    

    return true;
  }

  public PupillaryResponseData unload()
  {
  	PupillaryResponseData data = CDMSerializer.objFactory.createPupillaryResponseData();
    unload(data);
    return data;
  }

  protected void unload(PupillaryResponseData data)
  {
    if (reactivityModifier != null)
      data.setReactivityModifier(reactivityModifier.unload());
    if (shapeModifier != null)
      data.setShapeModifier(shapeModifier.unload());
    if (sizeModifier != null)
      data.setSizeModifier(sizeModifier.unload());    
  }
  
  public SEScalarNeg1To1 getReactivityModifier()
  {
    if (reactivityModifier == null)
    	reactivityModifier = new SEScalarNeg1To1();
    return reactivityModifier;
  }
  public boolean hasReactivityModifier()
  {
    return reactivityModifier == null ? false : reactivityModifier.isValid();
  }
  
  public SEScalarNeg1To1 getShapeModifier()
  {
    if (shapeModifier == null)
    	shapeModifier = new SEScalarNeg1To1();
    return shapeModifier;
  }
  public boolean hasShapeModifier()
  {
    return shapeModifier == null ? false : shapeModifier.isValid();
  }
  
  public SEScalarNeg1To1 getSizeModifier()
  {
    if (sizeModifier == null)
    	sizeModifier = new SEScalarNeg1To1();
    return sizeModifier;
  }
  public boolean hasSizeModifier()
  {
    return sizeModifier == null ? false : sizeModifier.isValid();
  }
}
