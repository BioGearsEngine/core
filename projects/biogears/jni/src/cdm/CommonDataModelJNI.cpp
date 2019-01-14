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
//External Inc lude
#include <jni.h>
//Project Includes
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/cdm/Serializer.h>
#include <biogears/cdm/test/CommonDataModelTest.h>
#include <biogears/cdm/utils/unitconversion/UCCommon.h>
#include <biogears/cdm/utils/unitconversion/PScalar.h>

using namespace biogears;
extern "C" JNIEXPORT jlong JNICALL Java_mil_tatrc_physiology_datamodel_testing_CDMUnitTestDriver_nativeAllocate(JNIEnv* env, jobject obj)
{
  CommonDataModelTest* executor = new CommonDataModelTest();
  executor->GetLogger()->LogToConsole(false);
  return reinterpret_cast<jlong>(executor);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_datamodel_testing_CDMUnitTestDriver_nativeDelete(JNIEnv* env, jobject obj, jlong ptr)
{
  CommonDataModelTest* executor = reinterpret_cast<CommonDataModelTest*>(ptr);
  SAFE_DELETE(executor);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_datamodel_testing_CDMUnitTestDriver_nativeExecute(JNIEnv* env, jobject obj, jlong ptr, jstring test, jstring toDir)
{
  const char* testName = env->GetStringUTFChars(test, JNI_FALSE);
  const char* outputDir = env->GetStringUTFChars(toDir, JNI_FALSE);
  CommonDataModelTest* executor = reinterpret_cast<CommonDataModelTest*>(ptr);
  executor->RunTest(testName, outputDir);
  env->ReleaseStringUTFChars(test, testName);
  env->ReleaseStringUTFChars(toDir, outputDir);
}

extern "C" JNIEXPORT void JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeInitialize(JNIEnv* env, jobject obj, jstring wrkDir)
{
  const char* dir = env->GetStringUTFChars(wrkDir, JNI_FALSE);
  CUnitConversionEngine::GetEngine().SetWorkingDirectory(dir);
  env->ReleaseStringUTFChars(wrkDir, dir);
}

// Tests if a value can be converted from 1 unit to another.
extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeIsCompatibleWithUnit(JNIEnv* env, jobject obj, jstring testUnit, jstring unit)
{
  const char* unitBuf = env->GetStringUTFChars(unit, JNI_FALSE);
  const char* testUnitBuf = env->GetStringUTFChars(testUnit, JNI_FALSE);
  try {
    CCompoundUnit theUnit(unitBuf);
    env->ReleaseStringUTFChars(unit, unitBuf);
    unitBuf = nullptr;
    // Assign to testUnit the ith string in the array

    CCompoundUnit theTestUnit(testUnitBuf);
    env->ReleaseStringUTFChars(testUnit, testUnitBuf);
    testUnitBuf = nullptr;

    // See if they are of the same quantity type (Dimension)
    if (*theUnit.GetDimension() == *theTestUnit.GetDimension())
      return JNI_TRUE;
    // See if the quantity types (Dimensions) are convertable
    double fromExp;
    CCompoundUnit mappingUnit;
    CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();
    if (uce.GetQuantityConversionParams(theUnit.GetDimension(), theTestUnit.GetDimension(), fromExp, mappingUnit))
      return JNI_TRUE;
  } catch (std::exception ex) {
    std::cerr << ex.what() << std::endl;
    if (unitBuf != nullptr)
      env->ReleaseStringUTFChars(unit, unitBuf);
    if (testUnitBuf != nullptr)
      env->ReleaseStringUTFChars(testUnit, testUnitBuf);
  }
  return JNI_FALSE;
}

// tests if a unit is the same quantity type as the provided unitTemplate
extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeIsOfQuantityType(JNIEnv* env, jobject obj, jstring templateUnit, jstring unit)
{
  const char* unitBuf = env->GetStringUTFChars(unit, JNI_FALSE);
  const char* templateUnitBuf = env->GetStringUTFChars(templateUnit, JNI_FALSE);
  try {
    CCompoundUnit theUnit(unitBuf);
    env->ReleaseStringUTFChars(unit, unitBuf);
    unitBuf = nullptr;

    // Assign to testUnit the ith string in the array
    CCompoundUnit theTemplateUnit(templateUnitBuf);
    env->ReleaseStringUTFChars(templateUnit, templateUnitBuf);
    templateUnitBuf = nullptr;

    // See if they are of the same quantity type (Dimension)
    if (*theUnit.GetDimension() == *theTemplateUnit.GetDimension())
      return JNI_TRUE;
  } catch (std::exception ex) {
    std::cerr << ex.what() << std::endl;
    if (unitBuf != nullptr)
      env->ReleaseStringUTFChars(unit, unitBuf);
    if (templateUnitBuf != nullptr)
      env->ReleaseStringUTFChars(templateUnit, templateUnitBuf);
  }
  return JNI_FALSE;
}

// tests if a unit is the same quantity type as the provided unitTemplates
// list is an array of unit strings, like {"J", "cm", "Hz"}. (This example works for testing ELambdaNu.)
extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeIsCompatibleWithUnitList(JNIEnv* env, jobject obj, jobjectArray list, jstring unit)
{
  if (list == nullptr)
    return JNI_FALSE;

  jboolean rtn = JNI_FALSE;
  const char* unitBuf = env->GetStringUTFChars(unit, JNI_FALSE);
  CCompoundUnit theUnit;
  try {
    theUnit.ParseString(unitBuf);
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    env->ReleaseStringUTFChars(unit, unitBuf);
    return JNI_FALSE;
  }
  env->ReleaseStringUTFChars(unit, unitBuf);

  // Assign to numStrings the number of strings in the java array
  jstring testUnit;
  jsize numStrings = env->GetArrayLength(list);

  double fromExp;
  CCompoundUnit mappingUnit;
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  for (int i = 0; i < numStrings; i++) {
    // Assign to testUnit the ith string in the array
    testUnit = static_cast<jstring>(env->GetObjectArrayElement(list, i));
    const char* testUnitBuf = (char*)env->GetStringUTFChars(testUnit, JNI_FALSE);

    CCompoundUnit theTestUnit;
    try {
      theTestUnit.ParseString(testUnitBuf);
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      env->ReleaseStringUTFChars(testUnit, testUnitBuf);
      return JNI_FALSE;
    }
    env->ReleaseStringUTFChars(testUnit, testUnitBuf);
    // See if they are of the same quantity type (Dimension)
    if (*theUnit.GetDimension() == *theTestUnit.GetDimension()) {
      rtn = JNI_TRUE;
      break;
    }
    // See if the quantity types (Dimensions) are convertable
    if (uce.GetQuantityConversionParams(theUnit.GetDimension(), theTestUnit.GetDimension(), fromExp, mappingUnit)) {
      rtn = JNI_TRUE;
      break;
    }
  }
  return rtn;
}

// list is an array of quantity type strings, like {"Energy", "Distance", "Frequency"}. (This example also works for testing ELambdaNu.)
extern "C" JNIEXPORT jboolean JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeIsOfQuantityTypes(JNIEnv* env, jobject obj, jobjectArray templateUnits, jstring unit)
{
  if (templateUnits == nullptr)
    return JNI_FALSE;

  jboolean rtn = JNI_FALSE;
  const char* unitBuf = env->GetStringUTFChars(unit, JNI_FALSE);
  CCompoundUnit theUnit;
  try {
    theUnit.ParseString(unitBuf);
    unitBuf = nullptr;
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    if (unitBuf != nullptr)
      env->ReleaseStringUTFChars(unit, unitBuf);
    return JNI_FALSE;
  }
  env->ReleaseStringUTFChars(unit, unitBuf);

  // Assign to numStrings the number of strings in the java array
  jstring testUnit;
  jsize numStrings = env->GetArrayLength(templateUnits);

  for (int i = 0; i < numStrings; i++) {
    // Assign to testUnit the ith string in the array
    testUnit = static_cast<jstring>(env->GetObjectArrayElement(templateUnits, i));
    const char* testUnitBuf = (char*)env->GetStringUTFChars(testUnit, JNI_FALSE);

    CCompoundUnit theTestUnit;
    try {
      theTestUnit.ParseString(testUnitBuf);
      testUnitBuf = nullptr;
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      if (testUnitBuf != nullptr)
        env->ReleaseStringUTFChars(testUnit, testUnitBuf);
      return JNI_FALSE;
    }
    env->ReleaseStringUTFChars(testUnit, testUnitBuf);

    // See if they are of the same quantity type (Dimension)
    if (*theUnit.GetDimension() == *theTestUnit.GetDimension()) {
      rtn = JNI_TRUE;
      break;
    }
  }
  return rtn;
}
// basic conversion
extern "C" JNIEXPORT jdouble JNICALL Java_mil_tatrc_physiology_utilities_UnitConverter_nativeConvert(JNIEnv* env, jobject obj, jdouble val, jstring unit_from, jstring unit_to)
{
  const char* uFrom = (const char*)env->GetStringUTFChars(unit_from, JNI_FALSE);
  const char* uTo = (char*)env->GetStringUTFChars(unit_to, JNI_FALSE);
  CPScalar cp(val, uFrom);
  jdouble jD = cp(uTo).GetValue();
  env->ReleaseStringUTFChars(unit_from, uFrom);
  env->ReleaseStringUTFChars(unit_to, uTo);
  return jD;
}
// Conversion routine is as follows:
// to convert x km to y ft, where bkm is km bias value, Bkm is km bigness value, bft is ft bias value, and Bft is ft bigness value
// y = ((x-bkm)*Bkm/Bft)+bft

extern "C"
  // Returns true if succeeds
  JNIEXPORT jboolean JNICALL
  Java_mil_tatrc_physiology_utilities_UnitConverter_nativeGenerateUnitConverterData(JNIEnv* env, jclass obj, jobject qtcd)
{
  int i;
  size_t fromDim, toDim;

  // Get ref to UnitConversionEngine singleton
  CUnitConversionEngine& uce = CUnitConversionEngine::GetEngine();

  // Prepare method IDs for later invocation
  jclass qtcdClass = env->GetObjectClass(qtcd);
  jmethodID SetNumberOfTypes;
  jmethodID SetMappingUnitBignessAndExponent;

  SetNumberOfTypes = env->GetMethodID(qtcdClass, "setNumberOfTypes", "(I)V");
  if (SetNumberOfTypes == nullptr) {
    std::cerr << "Unable to resolve method SetNumberOfTypes via JNI\n";
  }

  SetMappingUnitBignessAndExponent = env->GetMethodID(qtcdClass, "setMappingUnitBignessAndExponent",
    "(IIDD)V");
  if (SetMappingUnitBignessAndExponent == nullptr) {
    std::cerr << "Unable to resolve method SetMappingUnitBignessAndExponent via JNI\n";
  }

  // Get the storage for the type index, bigness, and bias arrays from the qtcd object
  // The java side should have already allocated storage for these arrays when it was given
  // the array of unit strings
  jintArray jTypeIdx;
  jdoubleArray jBigness;
  jdoubleArray jBias;
  jobjectArray jUnitList;

  jint* typeIdx;
  jdouble* bigness;
  jdouble* bias;

  jfieldID tempFieldID;
  jboolean isCopy;

  CCompoundUnit tempCU;
  const CUnitDimension* tempUD;

  tempFieldID = env->GetFieldID(qtcdClass, "fTypeIdx", "[I");
  if (tempFieldID == nullptr) {
    std::cerr << "Unable to resolve field TypeIdx via JNI\n";
  }
  jTypeIdx = static_cast<jintArray>(env->GetObjectField(qtcd, tempFieldID));
  typeIdx = env->GetIntArrayElements(jTypeIdx, &isCopy);

  tempFieldID = env->GetFieldID(qtcdClass, "fBigness", "[D");
  if (tempFieldID == nullptr) {
    std::cerr << "Unable to resolve field Bigness via JNI\n";
  }
  jBigness = static_cast<jdoubleArray>(env->GetObjectField(qtcd, tempFieldID));
  bigness = env->GetDoubleArrayElements(jBigness, &isCopy);

  tempFieldID = env->GetFieldID(qtcdClass, "fBias", "[D");
  if (tempFieldID == nullptr) {
    std::cerr << "Unable to resolve field Bias via JNI\n";
  }
  jBias = static_cast<jdoubleArray>(env->GetObjectField(qtcd, tempFieldID));
  bias = env->GetDoubleArrayElements(jBias, &isCopy);

  tempFieldID = env->GetFieldID(qtcdClass, "fUnitList", "[Ljava/lang/String;");
  if (tempFieldID == nullptr) {
    std::cerr << "Unable to resolve field TypeIdx via JNI\n";
  }
  jUnitList = static_cast<jobjectArray>(env->GetObjectField(qtcd, tempFieldID));
  int numUnits = env->GetArrayLength(jUnitList);

  // Turn the array of string objects holding the unit strings into a list we can work with
  // We'll keep each of the jstrings and pinned arrays of java chars in a vector so we can release them later,
  // and we'll also make a vector of C++ strings so that we don't keep constructing them
  // repeatedly later.
  std::vector<jstring> JStringVec(numUnits);
  std::vector<const char*> UTFCharVec(numUnits);
  std::vector<std::string> StringVec(numUnits);

  for (i = 0; i < numUnits; ++i) {
    JStringVec[i] = static_cast<jstring>(env->GetObjectArrayElement(jUnitList, i));
    UTFCharVec[i] = env->GetStringUTFChars(JStringVec[i], &isCopy);
    StringVec[i] = UTFCharVec[i];
  }

  // Create vector of dimension pointers. We'll search through these naively, but we shouldn't
  // have more than two or three dimensions to work with at a time
  std::vector<CUnitDimension> DimVec;
  bool dimFound;
  size_t dimIdx, numDims;

  // Loop over each string array element
  for (int unitIdx = 0; unitIdx < numUnits; ++unitIdx) {
    // Create CompoundUnit from string
    tempCU.clear();
    try {
      tempCU.ParseString(StringVec[unitIdx]);
    } catch (std::exception& ex) {
      std::cerr << ex.what() << std::endl;
      continue;
    }

    // Get UnitDimension from Compound Unit
    tempUD = tempCU.GetDimension();

    dimFound = false;
    numDims = DimVec.size();

    // Loop over all elements in vector of dimension pointers
    for (dimIdx = 0; dimIdx < numDims; ++dimIdx) {
      // If current element matches our UnitDimension, set Found  and DimIdx, and break
      if (*tempUD == DimVec[dimIdx]) {
        dimFound = true;
        break;
      }
    }
    if (!dimFound) {
      DimVec.push_back(*tempUD);
      // dimIdx should already be where we want it
    }

    // Set TypeID for current string equal to dimIdx
    typeIdx[unitIdx] = static_cast<jint>(dimIdx);

    // Get bigness and bias of current CompoundUnit and store in corresponding vector elements
    bigness[unitIdx] = tempCU.GetBigness();
    bias[unitIdx] = tempCU.GetBias();
  }

  // Invoke SetNumberOfTypes method with length of dimension pointer vector as param
  numDims = DimVec.size();

  env->CallVoidMethod(qtcd, SetNumberOfTypes, numDims);

  double fromExp;
  CCompoundUnit mappingUnit;
  double mappingUnitBigness;

  // Loop "From" dimension over all the dimension pointers
  for (fromDim = 0; fromDim < numDims; ++fromDim) {
    // Loop "To" dimension over all the dimension pointers
    for (toDim = 0; toDim < numDims; ++toDim) {

      if (fromDim != toDim) {
        // Get exponent and mapping unit from UCE
        uce.GetQuantityConversionParams(&DimVec[fromDim], &DimVec[toDim], fromExp, mappingUnit);
        // Get bigness of mapping unit
        mappingUnitBigness = mappingUnit.GetBigness();
      } else {
        // exponent and mapping bigness are both 1.0 (identity transform)
        fromExp = 1.0;
        mappingUnitBigness = 1.0;
      }

      // Invoke SetMappingUnitBignessAndExponent method on qtcd object
      env->CallVoidMethod(qtcd, SetMappingUnitBignessAndExponent, fromDim, toDim, mappingUnitBigness, fromExp);
    }
  }

  // Release strings holding units
  for (i = 0; i < numUnits; ++i) {
    env->ReleaseStringUTFChars(JStringVec[i], UTFCharVec[i]);
  }

  // Release the storage for the type index, bigness, and bias arrays from the qtcd object
  env->ReleaseIntArrayElements(jTypeIdx, typeIdx, 0);
  env->ReleaseDoubleArrayElements(jBigness, bigness, 0);
  env->ReleaseDoubleArrayElements(jBias, bias, 0);

  return JNI_TRUE;
}
