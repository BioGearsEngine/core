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

import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import mil.tatrc.physiology.datamodel.bind.DataRequestData;
import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.bind.DataRequestsData;
import mil.tatrc.physiology.datamodel.substance.SESubstance;
import mil.tatrc.physiology.datamodel.substance.SESubstanceManager;
import mil.tatrc.physiology.utilities.Log;

public class SEDataRequestManager
{
	protected String                        filename;
	// TODO Decimal Formatting Data
	protected List<SEDataRequest>           requestedData;	
	protected double                        samplesPerSecond;
	
	public SEDataRequestManager()
	{
		this.filename = "";
		this.samplesPerSecond = 0;
		this.requestedData = new ArrayList<SEDataRequest>();
	}
	
	public void reset() 
	{
		this.filename = "";
	  this.samplesPerSecond = 0;
		requestedData.clear();
	}
	
	public boolean load(DataRequestsData in, SESubstanceManager subMgr)
	{
		reset();
		
		if(in.getFilename()!=null)
			this.filename = in.getFilename();
		if(in.getSamplesPerSecond()!=null)
			this.samplesPerSecond = in.getSamplesPerSecond();
		
		try
		{
			for (DataRequestData dataRequestData : in.getDataRequest())
			{
				String className = dataRequestData.getClass().getName().replaceAll("bind.", "scenario.datarequests.SE");
				className = className.substring(0, className.lastIndexOf("Data"));
				Class<?> klass = Class.forName(className);
				
				Object instance = null;
                for(Constructor<?> ctor : klass.getConstructors())
                {
                    Class<?>[] params = ctor.getParameterTypes();
                    if(params.length>0)
                    {
                        if(params[0].isAssignableFrom(SESubstance.class))
                        {
                            Method m=dataRequestData.getClass().getMethod("getSubstance");
                            String subName = (String)m.invoke(dataRequestData);
                            instance=ctor.newInstance(subMgr.getSubstance(subName));
                        }
                    }
                }
                if(instance==null)
                    instance = klass.newInstance();
				
				// Find the right "load" method
				Method loadMethod = null;
				for (Method m : klass.getMethods())
				{
					if  (m.getName().equals("load"))
					{
						loadMethod = m;
						break;
					}
				}
				if (loadMethod.getParameterTypes().length == 2)
					loadMethod.invoke(instance, dataRequestData, subMgr);
				else
					loadMethod.invoke(instance, dataRequestData);
				
				this.requestedData.add((SEDataRequest) instance);
			}
		}
		catch (Exception e)
		{
			Log.error("Unsupported Data Request received in scenario file", e);
			return false;
		}
		return true;
	}
	
	public DataRequestsData unload()
	{
		DataRequestsData data = CDMSerializer.objFactory.createDataRequestsData();
		unload(data);
		return data;
	}
	protected void unload(DataRequestsData data)
	{
		if(this.hasFilename())
			data.setFilename(this.filename);
		if(this.samplesPerSecond>0)
			data.setSamplesPerSecond(this.samplesPerSecond);
		for(SEDataRequest dr : this.requestedData)
			data.getDataRequest().add(dr.unload());
	}
	
	public boolean hasFilename()
	{
		return this.filename!=null&&!this.filename.isEmpty();
	}
	public void setFilename(String f)
	{
		this.filename = f;
	}
	public String getFilename()
	{
		return this.filename;
	}
	
	public void setSamplesPerSecond(double d)
	{
		this.samplesPerSecond=d;
	}
	public double getSamplesPerSecond()
	{
		return this.samplesPerSecond;
	}
	
	public List<SEDataRequest> getRequestedData() 
	{
		return requestedData;
	}
	
}
