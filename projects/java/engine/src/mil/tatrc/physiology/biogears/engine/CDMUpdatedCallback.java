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

import mil.tatrc.physiology.datamodel.scenario.datarequests.SEDataRequestManager;

public abstract class CDMUpdatedCallback
{
  protected double callbackFrequency_s;
  protected SEDataRequestManager drMgr;
  public CDMUpdatedCallback(double updateFrequency_s)
  {
  	this.drMgr = null;
    callbackFrequency_s = updateFrequency_s;
  }
  public void setDataRequests(SEDataRequestManager drMgr)
  {
  	this.drMgr = drMgr;
  }
  
  public abstract void update(double time_s);
}
