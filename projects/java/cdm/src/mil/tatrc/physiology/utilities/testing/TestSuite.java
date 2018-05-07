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

/**
 * 
 */
package mil.tatrc.physiology.utilities.testing;

import java.util.*;

import mil.tatrc.physiology.datamodel.CDMSerializer;
import mil.tatrc.physiology.datamodel.SEEqualOptions;
import mil.tatrc.physiology.datamodel.SEEqualOptions.PathTrack;
import mil.tatrc.physiology.datamodel.SEEqualOptions.Track;
import mil.tatrc.physiology.datamodel.bind.*;
import mil.tatrc.physiology.datamodel.properties.CommonUnits.TimeUnit;
import mil.tatrc.physiology.datamodel.properties.SEFunction;
import mil.tatrc.physiology.utilities.*;


/**
 * @author abray, boday
 *
 */
public class TestSuite
{
  /** Internal data object */
  private TestReportData.TestSuite testSuite = null;
  /** Current Test Case being performed */
  private TestReportData.TestSuite.TestCase testCase = null;
  /** time objects for testSuite and testCase**/
  private ScalarTimeData testSuiteTime = null;
  private ScalarTimeData testCaseTime = null;
  /** Manual way to fail the case, other way is to Log an error */
  protected boolean failCase = false;
  /** Case Equal Options */
  protected SEEqualOptions caseEqualOptions;
  /** List of caseEqOpts to merge for suite Eq Stats */
  private List<SEEqualOptions> runCaseEqOpts = new ArrayList<SEEqualOptions>();
  /** Listen to the log for any errors */
  protected TestSuiteListener logListener = null;
  /** The failure message for when we are testing exceptions in the code
   *  i.e. The TestCase is expected to fail, if the case does not fail, this message
   *  will be reported in the log.  */
  protected String failureMessage = "";
  /** Marks for the running case, inputs, outputs, algorithm for test case */
  protected Set<String> caseMarks = new HashSet<String>();
  /** The location to look for any external database/dataset files  */
  protected String dbDirectory = "./";
 
  public TestSuite()
  {
    this.logListener = new TestSuiteListener();
    this.logListener.setSuite(this);
    reset();
  }
  
  
  public void load(TestReportData.TestSuite in)
  {
    this.reset();
    
    testSuite.setErrors(in.getErrors());
    testSuite.setName(in.getName());
    testSuite.setPerformed(in.isPerformed());
    testSuite.setTests(in.getTests());
    testSuite.setTime(in.getTime());
    testSuite.getRequirement().addAll(in.getRequirement());
    if(in.getTestCase()!=null&&in.getTestCase().size()>0)
    {
    	testSuite.getTestCase().addAll(in.getTestCase());
    	testCase = testSuite.getTestCase().get(0);
    }    
  }
  
  public TestReportData.TestSuite unload()
  {
    return this.testSuite;
  }
  
  public void reset()
  {
    this.testSuite = null;
    this.testSuite = CDMSerializer.objFactory.createTestReportDataTestSuite();
    this.testSuite.setName("TestSuite");
    ScalarTimeData value = CDMSerializer.objFactory.createScalarTimeData();
    this.testSuite.setTime(value);
    this.runCaseEqOpts.clear();
    this.caseEqualOptions = null;
    this.testCase = null;
    this.caseMarks.clear();
  }

  /**
   * Set the directory to look for any external
   * database/dataset files
   * @param dbDir
   */
  public void setDatabaseDirectory(String dbDir)
  {
    this.dbDirectory=dbDir;
  }
  
  public String getDatabaseDirectory()
  {
    return dbDirectory;
  }
  
  public void setName(String name){this.testSuite.setName(name);}
  public String getName(){return this.testSuite.getName();}
  
  public void setErrors(int errs){this.testSuite.setErrors(errs);}
  public int getErrors(){return this.testSuite.getErrors();}
  
  public void setTests(int numTests){this.testSuite.setTests(numTests);}
  public int getTests(){return this.testSuite.getTests();}
  
  public void setTime(double time, TimeUnit unit)
  {
	  this.testSuite.getTime().setValue(time);
	  this.testSuite.getTime().setUnit(unit.toString());
  }
  public double getTime(){return this.testSuite.getTime().getValue();}
  
  public void setPerformed(boolean b){this.testSuite.setPerformed(b);}
  public boolean isPerformed(){return this.testSuite.isPerformed();}
  
  public void addRequirement(String req) { this.testSuite.getRequirement().add(req); }
  public List<String> getRequirements() { return this.testSuite.getRequirement(); }      
  
  /**
   * This case is expected to fail.
   * If it does not fail, the provided failureMessage will
   * be reported, and the case will be failed
   * @param caseName
   * @param failureMessage
   */
  public void startExceptionCase(String caseName, String failureMessage)
  {   
    startCase(caseName);
    this.failureMessage = failureMessage;
  }
  
  public void startCase(String caseName)
  {
    if (this.testCase != null)
      endCase();
    
    failCase=false;
    testCase = new TestReportData.TestSuite.TestCase();
    testSuiteTime = new ScalarTimeData();
    testCaseTime = new ScalarTimeData();
    testSuite.getTestCase().add(testCase);
    testSuite.setTests(testSuite.getTestCase().size());
    testSuite.setTime(testSuiteTime);
    testCase.setTime(testCaseTime);
    testCaseTime.setValue(0);
    testCaseTime.setUnit("s");
    testCase.setName(caseName);
    caseMarks.clear();
    
    this.caseEqualOptions = new SEEqualOptions();
    /** TODO Should there be a suiteOptions these can base off of? */
    caseEqualOptions.trackError(true);
    caseEqualOptions.trackDifferences(true);
    
    TimingProfile.start(caseName);
  }
  
  /** 
   * Add the classification of any input/output/algorithm data
   * this test case is testing.
   * If there are failures, the failure messages and 
   * differences data should be protected at this classification
   * Until someone determines what the classification is of what 
   * was output 
   **/
  public void addCaseClassificationMark(String mark)
  {
    this.caseMarks.add(mark);
  }
  
  public boolean hasCase()
  {
    return this.testCase!=null;
  }
  
  /**
   * Gets the test case that is currently running in the suite
   * @return TestReportData.TestSuite.TestCase
   */
  public TestReportData.TestSuite.TestCase getCase()
  {
    return this.testCase;
  }
  
  public SEEqualOptions getCaseEqualOptions()
  {
    return this.caseEqualOptions;
  }
  
  public boolean caseFailed()
  {
    if(this.testCase!=null)
      return this.testCase.getFailure().size()>0 || this.failCase;
    return false;
  }
  
  public void failCase()
  {
    this.failCase = true;
  }
 
  public void endCase()
  {
    if(this.testCase==null)
      return;
    
    if(failCase)
      this.testCase.getFailure().add("Case Failed");
    this.failCase = false;
    
    double caseTime = TimingProfile.profile(this.testCase.getName());
    // Add the metrics
    testCase.getTime().setValue(caseTime);
    
    if (this.failureMessage.isEmpty())
    {// Normal Test Case
      if(this.testCase.getFailure().size()>0)
      {// Case Failed
        int errors = this.testSuite.getErrors();    
        this.testSuite.setErrors(errors+1);
        runCaseEqOpts.add(caseEqualOptions);
        // Not sure what's in the failure messages,
        // So mark it with the I/O HWM        
        equalErrorToVVCase(testCase.getCaseEqualError(), caseEqualOptions);
      }
    }
    else
    {// Exception Test Case
      if (this.testCase.getFailure().size() == 0)
      {//Case Failed: Should have produced failures in the test
        int errors = this.testSuite.getErrors();
        this.testSuite.setErrors(errors+1);
        this.testCase.getFailure().add(failureMessage);
        this.failureMessage = "";
      }
      else
      {//Clear out the failures we correctly produced in the test
        this.testCase.getFailure().clear();
      }
    }
    this.testCase = null;
    // Update suite: time
    double suiteTime = this.testSuite.getTime().getValue();
    this.testSuite.getTime().setValue(suiteTime+caseTime);
    this.testSuite.getTime().setUnit("s");
    this.caseMarks.clear();
  }
  
  public void finalize()
  {
    endCase();
    // Merge all the failed case statistics
    if (!runCaseEqOpts.isEmpty())
    {
      SEEqualOptions merged = new SEEqualOptions();
      merged.setLimit(runCaseEqOpts.get(0).getLimit());
      for (SEEqualOptions runOpts : runCaseEqOpts)
        merged.appendTrackMap(runOpts.getMapTrack());
      equalErrorToVVCase(testSuite.getSuiteEqualError(), merged);
    }
  }
  
  protected void equalErrorToVVCase(List<TestErrorStatisticsData> errStats, SEEqualOptions eqOpts)
  {
    Track t;
    List<Double> error;
    TestErrorStatisticsData stats;
    Map<PathTrack, Track> map=eqOpts.getMapTrack();
    for(PathTrack pt : map.keySet())
    { 
      t = map.get(pt);
      stats = CDMSerializer.objFactory.createTestErrorStatisticsData();
      stats.setPropertyName(pt.getCDMPath());
      stats.setComputedPropertyID(pt.getToIDPath());
      stats.setExpectedPropertyID(pt.getFromIDPath());
      stats.getDifferences().addAll(t.getDifferences());
      stats.setPercentTolerance(eqOpts.getLimit());
      error = t.getEqualError();
      if(!error.isEmpty())
      {
        // Unit?
        stats.setNumberOfErrors(error.size());
        stats.setMinimumError(DoubleUtils.getMin(error));
        stats.setMaximumError(DoubleUtils.getMax(error));
        stats.setAverageError(DoubleUtils.getAverage(error));
        stats.setStandardDeviation(DoubleUtils.getStandardDeviation(error));
        // Bins
        SEFunction errBin = binEqualErrors(eqOpts.getLimit(),error);
        if(errBin!=null)
          stats.setPercentToleranceVsNumErrors(errBin.unload());
      }
      errStats.add(stats);
    }
  }
  
  protected static final double[] baseBins = {0.5,0.4,0.3,0.2,0.1,0.01,0.001,0.0001,0.00001,0.000001}; 
  public static SEFunction binEqualErrors(double limit, List<Double> error)
  {
    if(error==null||error.isEmpty())
      return null;
    
    SEFunction errBin = new SEFunction();
    // Make some temps and sort bins
    boolean added=false;
    List<Double> bins=new ArrayList<Double>();
    for(double d : baseBins)
    {
      if(d>limit)
        bins.add(d);
      else if(d<=limit)
      {
        added=true;
        bins.add(limit);
        break;
      }
    }
    if(!added)
      bins.add(limit);
    
    // Add the ABS, as we can get -error back
    List<Double> errs=new ArrayList<Double>();
    for(Double e : error)
      errs.add(Math.abs(e));
    Collections.sort(errs,Collections.reverseOrder());
    
    List<Double> numErrs=new ArrayList<Double>();
    int next=0;
    double num=0;
    double b=bins.get(next);
    for(int i=0; i<errs.size();)
    {
      if(errs.get(i)>=b)
      {
        num++;
        i++;
      }
      else
      {
        if(next+1==bins.size())
          break;
        
        numErrs.add(next,num);
        num=0;
        next++;
        b=bins.get(next);
      }
    }
    numErrs.add(next,num);
    // It's possible we binned all the errors and need to set other bins to zero
    for(int i=next+1;i<bins.size();i++)
      numErrs.add(i,0.);
      
    
    double[] tmp1 = new double[bins.size()];
    double[] tmp2 = new double[bins.size()];
    for(int i=0; i<bins.size(); i++)
    {
      tmp1[i]=bins.get(i);
      tmp2[i]=numErrs.get(i);
    }
    errBin.setIndependent(tmp1);
    errBin.setDependent(tmp2);
    return errBin;
  }
}
