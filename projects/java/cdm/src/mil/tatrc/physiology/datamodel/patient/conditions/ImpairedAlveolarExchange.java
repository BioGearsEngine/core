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

package mil.tatrc.physiology.datamodel.patient.conditions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.ImpairedAlveolarExchangeData;
import mil.tatrc.physiology.datamodel.properties.SEScalar0To1;
import mil.tatrc.physiology.datamodel.properties.SEScalarArea;

public class ImpairedAlveolarExchange extends SEPatientCondition
{
	protected SEScalarArea impairedSurfaceArea;
	protected SEScalar0To1 impairedFraction;
	
	public ImpairedAlveolarExchange()
	{
		impairedSurfaceArea = null;
		impairedFraction = null;
	}
	
	public void copy(ImpairedAlveolarExchange other)
	{
		if(this==other)
			return;
		super.copy(other);
		if (other.impairedSurfaceArea != null)
			getImpairedSurfaceArea().set(other.getImpairedSurfaceArea());
		else if (impairedSurfaceArea != null)
			impairedSurfaceArea.invalidate();
		if (other.impairedFraction != null)
			getImpairedFraction().set(other.getImpairedFraction());
		else if (impairedFraction != null)
			impairedFraction.invalidate();
	}
	
	public void reset()
	{
		super.reset();
		if (impairedSurfaceArea != null)
			impairedSurfaceArea.invalidate();
		if (impairedFraction != null)
			impairedFraction.invalidate();
	}
	
	public boolean isValid()
	{
		return hasImpairedSurfaceArea() || hasImpairedFraction();
	}
	
	public boolean load(ImpairedAlveolarExchangeData in)
	{
		super.load(in);
		if(in.getImpairedSurfaceArea()!=null)
			getImpairedSurfaceArea().load(in.getImpairedSurfaceArea());
		if(in.getImpairedFraction()!=null)
			getImpairedFraction().load(in.getImpairedFraction());
		return isValid();
	}
	
	public ImpairedAlveolarExchangeData unload()
	{
		ImpairedAlveolarExchangeData data = CDMSerializer.objFactory.createImpairedAlveolarExchangeData();
		unload(data);
		return data;
	}
	
	protected void unload(ImpairedAlveolarExchangeData data)
	{
		super.unload(data);
		if (impairedSurfaceArea != null)
			data.setImpairedSurfaceArea(impairedSurfaceArea.unload());
		if (impairedFraction != null)
			data.setImpairedFraction(impairedFraction.unload());
	}
	
	public boolean hasImpairedSurfaceArea()
	{
		return impairedSurfaceArea == null ? false : impairedSurfaceArea.isValid();
	}
	public SEScalarArea getImpairedSurfaceArea()
	{
		if (impairedSurfaceArea == null)
			impairedSurfaceArea = new SEScalarArea();
		return impairedSurfaceArea;
	}
	
	public boolean hasImpairedFraction()
	{
		return impairedFraction == null ? false : impairedFraction.isValid();
	}
	public SEScalar0To1 getImpairedFraction()
	{
		if (impairedFraction == null)
			impairedFraction = new SEScalar0To1();
		return impairedFraction;
	}
	
	public String toString()
	{
		if (impairedFraction != null)
			return "ImpairedAlveolarExchange" 
					+ "\n\tImpairedFraction: " + getImpairedFraction();
		else if (impairedSurfaceArea != null)
			return "ImpairedAlveolarExchange" 
					+ "\n\tImpairedSurfaceArea: " + getImpairedSurfaceArea();
		else
			return "Action not specified properly";
	}
}
