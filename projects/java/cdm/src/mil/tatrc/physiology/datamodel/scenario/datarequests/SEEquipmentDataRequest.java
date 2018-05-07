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

package mil.tatrc.physiology.datamodel.scenario.datarequests;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.EquipmentDataRequestData;

public class SEEquipmentDataRequest extends SEDataRequest
{
	protected String type;
	public SEEquipmentDataRequest()
	{
		type=null;
	}
	
	public void reset()
	{
		super.reset();
		type=null;
	}
	
	public boolean load(EquipmentDataRequestData in)
	{
		super.load(in);
		this.type = in.getType();
		return true;
	}
	
	public EquipmentDataRequestData unload()
	{
		EquipmentDataRequestData data = CDMSerializer.objFactory.createEquipmentDataRequestData();
		unload(data);
		return data;
	}
	
	protected void unload(EquipmentDataRequestData data)
	{
		super.unload(data);
		data.setType(this.type);
	}
	
	public int hashCode()
	{
		int c = super.hashCode();
		if(type!=null)
			c = 31 * c + type.hashCode();
		return c;
	}
	
	public boolean hasType(){return this.type != null;}
	public String getType(){return this.type; }
	public void setType(String t){this.type = t;}
}
