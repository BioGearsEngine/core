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

//Standard Includes
#include <iostream>
#include <memory>
//External Includes
#include <jni.h>
//Project Includes
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/engine/Controller/BioGearsEngine.h>
#include <biogears/engine/Equipment/ECG.h>
#include <biogears/cdm/scenario/SEScenarioExec.h>
#include <biogears/cdm/utils/SEEventHandler.h>

class BioGearsEngineJNI : public biogears::LoggerForward, public biogears::SEScenarioCustomExec, public biogears::SEEventHandler
{
public:
	BioGearsEngineJNI(const std::string& logFile);
	~BioGearsEngineJNI();

	void Reset();

	void ForwardDebug(const std::string&  msg, const std::string&  origin);
	void ForwardInfo(const std::string&  msg, const std::string&  origin);
	void ForwardWarning(const std::string&  msg, const std::string&  origin);
	void ForwardError(const std::string&  msg, const std::string&  origin);
	void ForwardFatal(const std::string&  msg, const std::string&  origin);
	void CustomExec(double time_s, biogears::PhysiologyEngine* engine);
	void PushData(double time_s);

	void HandlePatientEvent(CDM::enumPatientEvent::value type, bool active, const biogears::SEScalarTime* time = nullptr);
	void HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const biogears::SEScalarTime* time = nullptr);

	JNIEnv*    jniEnv;
	jobject    jniObj;
  biogears::DataTrack* trk;
  biogears::SEScenarioExec* exec;
	bool       firstUpdate;
	int        update_cnt;
	int        updateFrequency_cnt;
	std::unique_ptr<biogears::PhysiologyEngine> eng;
	
	jmethodID jniDebugMethodID;
  jmethodID jniInfoMethodID;
  jmethodID jniWarnMethodID;
  jmethodID jniErrorMethodID;
  jmethodID jniFatalMethodID;
};
