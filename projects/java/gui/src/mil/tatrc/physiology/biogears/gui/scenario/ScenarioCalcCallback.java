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

package mil.tatrc.physiology.biogears.gui.scenario;

import java.util.Collection;

import mil.tatrc.physiology.biogears.engine.BioGears;
import mil.tatrc.physiology.biogears.engine.CDMUpdatedCallback;
import mil.tatrc.physiology.biogears.gui.scenario.ScenarioCalcDisplay.ChartDataRequestScalar;
import mil.tatrc.physiology.datamodel.properties.SEScalar;
import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequest;
import mil.tatrc.physiology.utilities.Log;
import mil.tatrc.physiology.utilities.Pair;

public class ScenarioCalcCallback extends CDMUpdatedCallback
{
  protected BioGears bg;
	protected Collection<ChartDataRequestScalar> charts;

	public ScenarioCalcCallback(BioGears bg, Collection<ChartDataRequestScalar> charts, double updateFrequency_s)
	{
	  super(updateFrequency_s);	
	  this.bg = bg;
	  this.charts = charts;
	}

	public void update(double time_s)
	{
		double d;
		for(ChartDataRequestScalar chart : charts)
		{
		  if(chart.scalar == null)
		  {
		    for(Pair<SEDataRequest,SEScalar> p : bg.getDataRequestPairs())
		    {
		      if(p.getL() == chart.dataRequest)
		      {
		        chart.scalar = p.getR();
		        break;
		      }
		    }
		    if(chart.scalar == null)
		      Log.error("Could not find scalar for chart " + chart.graphLine.getLabel());
		  }
			d=0;
			if(chart.scalar.isValid())
				d=chart.scalar.getValue();	
			if(Double.isInfinite(d))
			  d=Double.NaN;
			chart.graphLine.addPoint(time_s,d);
			chart.zoom();
		}
	}
}
