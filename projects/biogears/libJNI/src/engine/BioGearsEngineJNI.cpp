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
#include <biogears/engine/BioGearsEngineJNI.h>
#include <biogears/exports.h>
//Project Includes
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SEUrinalysis.h>
#include <biogears/cdm/scenario/SEAction.h>
#include <biogears/cdm/scenario/SECondition.h>
#include <biogears/cdm/scenario/requests/SEDataRequest.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenario.h>
#include <biogears/engine/Controller/Scenario/BioGearsScenarioExec.h>
#include <biogears/engine/test/BioGearsEngineTest.h>

using namespace biogears;
extern "C" JNIEXPORT jlong JNICALL Java_mil_tatrc_physiology_biogears_testing_BGEUnitTestDriver_nativeAllocate(JNIEnv* env, jobject obj)
{
  BioGearsEngineTest* executor = new BioGearsEngineTest();
  return reinterpret_cast<jlong>(executor);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_testing_BGEUnitTestDriver_nativeDelete(JNIEnv* env, jobject obj, jlong ptr)
{
  BioGearsEngineTest* executor = reinterpret_cast<BioGearsEngineTest*>(ptr);
  SAFE_DELETE(executor);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_testing_BGEUnitTestDriver_nativeExecute(JNIEnv* env, jobject obj, jlong ptr, jstring test, jstring toDir)
{
  const char* testName = env->GetStringUTFChars(test, JNI_FALSE);
  const char* outputDir = env->GetStringUTFChars(toDir, JNI_FALSE);
  BioGearsEngineTest* executor = reinterpret_cast<BioGearsEngineTest*>(ptr);
  executor->RunTest(testName, outputDir);
  env->ReleaseStringUTFChars(test, testName);
  env->ReleaseStringUTFChars(toDir, outputDir);
}

extern "C" JNIEXPORT jlong JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGears_nativeAllocate(JNIEnv* env, jobject obj, jstring logFile)
{
  const char* logF = env->GetStringUTFChars(logFile, JNI_FALSE);
  BioGearsEngineJNI* engineJNI = new BioGearsEngineJNI(logF);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  env->ReleaseStringUTFChars(logFile, logF);
  return reinterpret_cast<jlong>(engineJNI);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGears_nativeDelete(JNIEnv* env, jobject obj, jlong ptr)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  SAFE_DELETE(engineJNI);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeReset(JNIEnv* env, jobject obj, jlong ptr)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  engineJNI->Reset();
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsScenarioExec_nativeExecuteScenario(JNIEnv* env, jobject obj, jlong ptr, jstring scenarioXML, jstring outputFile, double updateFreq_s)
{
  const char* sceXML = env->GetStringUTFChars(scenarioXML, JNI_FALSE);
  const char* dataF = env->GetStringUTFChars(outputFile, JNI_FALSE);
  try {
    BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
    engineJNI->updateFrequency_cnt = (int)(updateFreq_s / engineJNI->eng->GetTimeStep(TimeUnit::s));
    engineJNI->jniEnv = env;
    engineJNI->jniObj = obj;
    // Load up the xml and run the scenario
    BioGearsScenario sce(engineJNI->eng->GetSubstanceManager());
    std::istringstream istr(sceXML);
    xml_schema::properties properties;
    properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "./xsd/BioGearsDataModel.xsd");
    sce.Load(*CDM::Scenario(istr, 0, properties));
    engineJNI->exec = new BioGearsScenarioExec(*engineJNI->eng);
    engineJNI->exec->Execute(sce, dataF, updateFreq_s <= 0 ? nullptr : engineJNI);
    SAFE_DELETE(engineJNI->exec);
  } catch (const xml_schema::exception& e) {
    std::cerr << e << std::endl;
  } catch (std::exception& ex) {
    std::cerr << "TODO Handle this Failure : " << ex.what() << std::endl;
  }
  env->ReleaseStringUTFChars(scenarioXML, sceXML);
  env->ReleaseStringUTFChars(outputFile, dataF);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsScenarioExec_nativeCancelScenario(JNIEnv* env, jobject obj, jlong ptr)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  //engineJNI->jniEnv = env; I am not doing this because the cancel comes in on another thread
  //engineJNI->jniObj = obj; and it has valid copies of the env and obj for its context
  if (engineJNI->exec != nullptr)
    engineJNI->exec->Cancel(); // This will not do anything with JNI
}

extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeLoadState(JNIEnv* env, jobject obj, jlong ptr, jstring stateFilename, jdouble simTime_s, jstring dataRequestsXML)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  const char* pStateFilename = env->GetStringUTFChars(stateFilename, JNI_FALSE);

  const char* drXML = env->GetStringUTFChars(dataRequestsXML, JNI_FALSE);
  // Load up the data requests
  xml_schema::properties properties;
  properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "./xsd/BioGearsDataModel.xsd");
  std::istringstream drstr(drXML);
  std::unique_ptr<CDM::DataRequestsData> drData(CDM::DataRequests(drstr, 0, properties));
  engineJNI->eng->GetEngineTrack()->GetDataRequestManager().Load(*drData, engineJNI->eng->GetSubstanceManager());
  // Ok, crank 'er up!

  jboolean bRet;
  SEScalarTime simTime;
  if (simTime_s >= 0) {
    simTime.SetValue(simTime_s, TimeUnit::s);
    bRet = engineJNI->eng->LoadState(pStateFilename, &simTime);
  } else {
    std::cout << "Loading... " << std::endl;
    bRet = engineJNI->eng->LoadState(pStateFilename);
  }
  engineJNI->eng->SetEventHandler(engineJNI);

  env->ReleaseStringUTFChars(stateFilename, pStateFilename);
  env->ReleaseStringUTFChars(dataRequestsXML, drXML);
  return bRet;
}

extern "C" JNIEXPORT jstring JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeSaveState(JNIEnv* env, jobject obj, jlong ptr, jstring stateFilename)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  const char* pStateFilename = env->GetStringUTFChars(stateFilename, JNI_FALSE);
  std::unique_ptr<CDM::BioGearsStateData> data((CDM::BioGearsStateData*)(engineJNI->eng->SaveState(pStateFilename).release()));
  env->ReleaseStringUTFChars(stateFilename, pStateFilename);

  xml_schema::namespace_infomap map;
  map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
  map[""].schema = "./xsd/BioGearsDataModel.xsd";

  data->contentVersion(BGE::Version);
  std::stringstream stream;
  CDM::BioGearsState(stream, *data, map);
  jstring stateXML = env->NewStringUTF(stream.str().c_str());
  env->ReleaseStringUTFChars(stateFilename, pStateFilename);
  return stateXML;
}

extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeInitializeEngine(JNIEnv* env, jobject obj, jlong ptr, jstring patientXML, jstring conditionsXML, jstring dataRequestsXML)
{

  bool ret = false;
  const char* pXML = env->GetStringUTFChars(patientXML, JNI_FALSE);
  const char* cXML = nullptr;
  if (conditionsXML != nullptr)
    cXML = env->GetStringUTFChars(conditionsXML, JNI_FALSE);
  const char* drXML = env->GetStringUTFChars(dataRequestsXML, JNI_FALSE);
  try {
    BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
    engineJNI->jniEnv = env;
    engineJNI->jniObj = obj;
    // Schema location
    xml_schema::properties properties;
    properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "./xsd/BioGearsDataModel.xsd");
    // Load up the patient
    SEPatient patient(nullptr);
    std::istringstream pstr(pXML);
    patient.Load(*CDM::Patient(pstr, 0, properties));
    // Load up the conditions
    std::vector<const SECondition*> conditions;
    if (cXML != nullptr) {
      std::istringstream cstr(cXML);
      std::unique_ptr<CDM::ConditionListData> cList(CDM::ConditionList(cstr, 0, properties));
      for (unsigned int i = 0; i < cList->Condition().size(); i++) {
        conditions.push_back(SECondition::newFromBind(cList->Condition()[i], engineJNI->eng->GetSubstanceManager()));
      }
    }
    // Load up the data requests
    std::istringstream drstr(drXML);
    std::unique_ptr<CDM::DataRequestsData> drData(CDM::DataRequests(drstr, 0, properties));
    engineJNI->eng->GetEngineTrack()->GetDataRequestManager().Load(*drData, engineJNI->eng->GetSubstanceManager());
    // Ok, crank 'er up!
    ret = engineJNI->eng->InitializeEngine(patient, &conditions);
    engineJNI->eng->SetEventHandler(engineJNI);
    DELETE_VECTOR(conditions); // Clean up the conditions, the engine makes copies of them
  } catch (const xml_schema::exception& e) {
    ret = false;
    std::cerr << e << std::endl;
  } catch (std::exception& ex) {
    ret = false;
    std::cerr << "TODO Handle this Failure : " << ex.what() << std::endl;
  }
  env->ReleaseStringUTFChars(patientXML, pXML);
  env->ReleaseStringUTFChars(conditionsXML, cXML);
  env->ReleaseStringUTFChars(dataRequestsXML, drXML);
  return ret;
}

extern "C" JNIEXPORT bool JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeAdvanceTimeStep(JNIEnv* env, jobject obj, jlong ptr)
{
  bool success = true;
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  try {
    engineJNI->eng->AdvanceModelTime();
  } catch (CommonDataModelException& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (std::exception& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (...) {
    success = false;
  }
  if (success) {
    double currentTime_s = engineJNI->eng->GetSimulationTime(TimeUnit::s);
    engineJNI->eng->GetEngineTrack()->TrackData(currentTime_s);
    engineJNI->PushData(currentTime_s);
  }
  return success;
}

extern "C" JNIEXPORT bool JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeAdvanceTime(JNIEnv* env, jobject obj, jlong ptr, jdouble time_s)
{
  bool success = true;
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  try {
    engineJNI->eng->AdvanceModelTime(time_s, TimeUnit::s);
  } catch (CommonDataModelException& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (std::exception& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (...) {
    success = false;
  }
  if (success) {
    double currentTime_s = engineJNI->eng->GetSimulationTime(TimeUnit::s);
    engineJNI->eng->GetEngineTrack()->TrackData(currentTime_s);
    engineJNI->PushData(currentTime_s);
  }
  return success;
}

extern "C" JNIEXPORT bool JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeProcessActions(JNIEnv* env, jobject obj, jlong ptr, jstring actionsXML)
{
  bool success = true;
  if (actionsXML == nullptr)
    return success;
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;
  const char* aXML = env->GetStringUTFChars(actionsXML, JNI_FALSE);
  std::istringstream astr(aXML);
  // Schema location
  xml_schema::properties properties;
  properties.schema_location("uri:/mil/tatrc/physiology/datamodel", "./xsd/BioGearsDataModel.xsd");
  std::unique_ptr<CDM::ActionListData> aList(CDM::ActionList(astr, 0, properties));
  try {
    for (unsigned int i = 0; i < aList->Action().size(); i++) {
      SEAction* a = SEAction::newFromBind(aList->Action()[i], engineJNI->eng->GetSubstanceManager());
      if (!engineJNI->eng->ProcessAction(*a))
        success = false;
      SAFE_DELETE(a);
    }
  } catch (CommonDataModelException& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (std::exception& ex) {
    success = false;
    engineJNI->GetLogger()->Error(ex.what());
  } catch (...) {
    success = false;
  }
  env->ReleaseStringUTFChars(actionsXML, aXML);

  return success;
}

extern "C" JNIEXPORT jstring JNICALL Java_mil_tatrc_physiology_biogears_engine_BioGearsEngine_nativeGetAssessment(JNIEnv* env, jobject obj, jlong ptr, jint type)
{
  BioGearsEngineJNI* engineJNI = reinterpret_cast<BioGearsEngineJNI*>(ptr);
  engineJNI->jniEnv = env;
  engineJNI->jniObj = obj;

  jstring assessmentXML;

  xml_schema::namespace_infomap map;
  map[""].name = "uri:/mil/tatrc/phsyiology/datamodel";
  map[""].schema = "./xsd/BioGearsDataModel.xsd";

  std::stringstream stream;
  switch (type) {
  case 0: // PFT
  {
    SEPulmonaryFunctionTest pft(engineJNI->eng->GetLogger());
    engineJNI->eng->GetPatientAssessment(pft);
    std::unique_ptr<CDM::PulmonaryFunctionTestData> unloaded(pft.Unload());
    unloaded->contentVersion(BGE::Version);
    CDM::PulmonaryFunctionTest(stream, *unloaded, map);
    break;
  }
  case 1: // CBC
  {
    SECompleteBloodCount cbc(engineJNI->eng->GetLogger());
    engineJNI->eng->GetPatientAssessment(cbc);
    std::unique_ptr<CDM::CompleteBloodCountData> unloaded(cbc.Unload());
    unloaded->contentVersion(BGE::Version);
    CompleteBloodCount(stream, *unloaded, map);
    break;
  }
  case 2: // CMP
  {
    SEComprehensiveMetabolicPanel cmp(engineJNI->eng->GetLogger());
    engineJNI->eng->GetPatientAssessment(cmp);
    std::unique_ptr<CDM::ComprehensiveMetabolicPanelData> unloaded(cmp.Unload());
    unloaded->contentVersion(BGE::Version);
    ComprehensiveMetabolicPanel(stream, *unloaded, map);
    break;
  }
  case 3: // U
  {
    SEUrinalysis u(engineJNI->eng->GetLogger());
    engineJNI->eng->GetPatientAssessment(u);
    std::unique_ptr<CDM::UrinalysisData> unloaded(u.Unload());
    unloaded->contentVersion(BGE::Version);
    Urinalysis(stream, *unloaded, map);
    break;
  }
  default:
    stream << "Unsupported assessment type";
  };

  assessmentXML = env->NewStringUTF(stream.str().c_str());
  return assessmentXML;
}

BioGearsEngineJNI::BioGearsEngineJNI(const std::string& logFile)
  : SEEventHandler(nullptr)
{ // No logger needed for the event handler, at this point
  Reset();
  eng = CreateBioGearsEngine(logFile);
  eng->GetLogger()->SetForward(this);
  eng->GetLogger()->LogToConsole(false);
  trk = &eng->GetEngineTrack()->GetDataTrack();
}

BioGearsEngineJNI::~BioGearsEngineJNI()
{
  Reset();
}

void BioGearsEngineJNI::Reset()
{
  exec = nullptr;
  firstUpdate = true;
  jniEnv = nullptr;
  jniObj = nullptr;
  jniDebugMethodID = nullptr;
  jniInfoMethodID = nullptr;
  jniWarnMethodID = nullptr;
  jniErrorMethodID = nullptr;
  jniFatalMethodID = nullptr;
  update_cnt = 0;
  updateFrequency_cnt = 45; // About every half second
}

void BioGearsEngineJNI::CustomExec(double time_s, PhysiologyEngine* engine)
{
  if (update_cnt++ > updateFrequency_cnt) {
    PushData(time_s);
    update_cnt = 0;
  }
}
void BioGearsEngineJNI::PushData(double time_s)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jmethodID m;
    std::vector<std::string>& headings = trk->GetHeadings();

    if (firstUpdate) {
      firstUpdate = false;
      jobjectArray sary = jniEnv->NewObjectArray(static_cast<jsize>(headings.size()), jniEnv->FindClass("java/lang/String"), jniEnv->NewStringUTF(""));
      for (unsigned int i = 0; i < headings.size(); i++)
        jniEnv->SetObjectArrayElement(sary, i, jniEnv->NewStringUTF(headings[i].c_str()));
      m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "setCDMHeadings", "([Ljava/lang/String;)V");
      if (m == nullptr)
        std::cerr << "Can't find setCDMHeadings method in Java" << std::endl;
      jniEnv->CallVoidMethod(jniObj, m, sary);
    }

    // Gather up the requested data into an array and pass it over to java
    // The order is set in the header order
    jdoubleArray ary = jniEnv->NewDoubleArray(static_cast<jsize>(trk->GetProbes()->size()));

    jboolean isCopy = JNI_FALSE;
    jdouble* reqData = jniEnv->GetDoubleArrayElements(ary, &isCopy);
    for (unsigned int i = 0; i < headings.size(); i++)
      reqData[i] = trk->GetProbe(headings[i]);
    if (isCopy == JNI_TRUE)
      jniEnv->ReleaseDoubleArrayElements(ary, reqData, JNI_COMMIT);

    m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "updateCDM", "(D[D)V");
    if (m == nullptr)
      std::cerr << "Can't find updateCDM method in Java" << std::endl;
    jniEnv->CallVoidMethod(jniObj, m, time_s, ary);
  }
}

void BioGearsEngineJNI::ForwardDebug(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniDebugMethodID == nullptr)
      jniDebugMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogDebug", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniDebugMethodID, m, o);
  }
}

void BioGearsEngineJNI::ForwardInfo(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniInfoMethodID == nullptr)
      jniInfoMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogInfo", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniInfoMethodID, m, o);
  }
}

void BioGearsEngineJNI::ForwardWarning(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniWarnMethodID == nullptr)
      jniWarnMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogWarning", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniWarnMethodID, m, o);
  }
}

void BioGearsEngineJNI::ForwardError(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniErrorMethodID == nullptr)
      jniErrorMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogError", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniErrorMethodID, m, o);
  }
}

void BioGearsEngineJNI::ForwardFatal(const std::string& msg, const std::string& origin)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jstring m = jniEnv->NewStringUTF(msg.c_str());
    jstring o = jniEnv->NewStringUTF(origin.c_str());
    if (jniFatalMethodID == nullptr)
      jniFatalMethodID = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "LogFatal", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->CallVoidMethod(jniObj, jniFatalMethodID, m, o);
  }
  std::string err;
  err.append(msg);
  err.append(" ");
  err.append(origin);
  throw PhysiologyEngineException(err);
}

void BioGearsEngineJNI::HandlePatientEvent(CDM::enumPatientEvent::value type, bool active, const SEScalarTime* time)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jmethodID m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "handleEvent", "(IIZD)V");
    if (m == nullptr)
      std::cerr << "Can't find handleEvent method in Java" << std::endl;
    jniEnv->CallVoidMethod(jniObj, m, 0, type, active, time != nullptr ? time->GetValue(TimeUnit::s) : 0);
  }
}
void BioGearsEngineJNI::HandleAnesthesiaMachineEvent(CDM::enumAnesthesiaMachineEvent::value type, bool active, const SEScalarTime* time)
{
  if (jniEnv != nullptr && jniObj != nullptr) {
    jmethodID m = jniEnv->GetMethodID(jniEnv->GetObjectClass(jniObj), "handleEvent", "(IIZD)V");
    if (m == nullptr)
      std::cerr << "Can't find handleEvent method in Java" << std::endl;
    jniEnv->CallVoidMethod(jniObj, m, 1, type, active, time != nullptr ? time->GetValue(TimeUnit::s) : 0);
  }
}
