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

package mil.tatrc.physiology.datamodel.patient.actions;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.UrinateData;

public class SEUrinate extends SEPatientAction
{
	public SEUrinate()
	{

	}
	
	public void copy(SEUrinate other)
	{
		if(this==other)
			return;
		super.copy(other);		
	}
	
	public void reset()
	{
		super.reset();
	}
	
	public boolean isValid()
	{
		return true;
	}
	
	public boolean load(UrinateData in)
	{
		super.load(in);
		return isValid();
	}
	
	public UrinateData unload() 
	{
		UrinateData data = CDMSerializer.objFactory.createUrinateData();
		unload(data);
		return data;
	}
	
	protected void unload(UrinateData data)
	{
		super.unload(data);
	}
	
	
	public String toString() 
	{
			return "Urinate";
	}
}
