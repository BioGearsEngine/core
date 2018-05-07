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
package mil.tatrc.physiology.utilities.csv;

import java.io.IOException;
import java.util.*;

import mil.tatrc.physiology.utilities.DoubleUtils;
import mil.tatrc.physiology.utilities.Log;



/** 
 * I am making this class to open up a CSV file
 * grab some data from it and compute means and
 * combine data as I see fit. Just a simple utility.
 * @author bwelch
 *
 */
public class CSVAnalysis 
{
	public static void main(String[] args) 
	{
		if(args.length!=1)
		{
			Log.error("You need to provide ONE file");
			return;
		}
		String fileName = args[0];
		Log.setFileName(fileName.substring(0,fileName.length()-4)+"Analysis.log");
		
		CSVAnalysis me = new CSVAnalysis();
		try
		{
			me.analyzeCSV(fileName);
		}
		catch(Exception ex)
		{
			Log.error("Could not analyze file "+fileName, ex);
		}
	}
	
	public List<Double> getValues(Map<String, List<Double>> results, String header)
	{
		return getValues(results, header, 0, 0);
	};
	
	public List<Double> getValues(Map<String, List<Double>> results, String header, int subsetStartTimestep, int subsetEndTimestep)
	{
		if(subsetStartTimestep <= 0 && subsetEndTimestep <= 0)
		{
			return results.get(header);
		}
		else
		{
			return results.get(header).subList(subsetStartTimestep, subsetEndTimestep);
		}
	};

	public void analyzeCSV(String fileName) throws IOException
	{
		CSVContents csv = new CSVContents(fileName);
		
		Map<String, List<Double>> results = new HashMap<String, List<Double>>();
		csv.readAll(results);

		int subsetStartTimestep = -1;	//use -1 to deactivate (will look at all timesteps in this case)
		int subsetEndTimestep = -1;
		
		List<Double> columnVals = getValues(results, "Aorta1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    double mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Aorta Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftHeart1ToAorta2_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Aorta Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Brain1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Brain Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToBrain1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Brain Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Bone1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Bone Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToBone1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Bone Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Fat1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Fat Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToFat1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Fat Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftKidney1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightKidney1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//sum both kidneys
	    Log.info("Kidneys Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLeftKidney1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Aorta1ToRightKidney1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//sum both kidneys
	    Log.info("Kidneys Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LargeIntestine_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Large Intestine Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLargeIntestine_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Large Intestine Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftArm1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Arm Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLeftArm1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Arm Inflow Mean(mL/min) " + mean*60);
	    
	    //L and R heart have pressures, too
	    columnVals = getValues(results, "LeftHeart1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Heart Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftHeart2ToLeftHeart1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Heart Inflow Mean(mL/min) " + mean*60);
	    columnVals = getValues(results, "LeftHeart1_Pressure_mmHg", subsetStartTimestep, subsetEndTimestep);//Should this be LeftHeart2?
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Heart Pressure Mean(mmHg) " + mean);
	    
	    columnVals = getValues(results, "LeftKidney1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Kidney Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLeftKidney1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Kidney Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftLeg1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Leg Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLeftLeg1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Leg Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryArteries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Pulmonary Arteries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Pulmonary Arteries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryCapillaries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Pulmonary Capillaries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftPulmonaryArteriesToLeftPulmonaryCapillaries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryVeins_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Left Pulmonary Veins Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftPulmonaryArteriesToLeftPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "LeftPulmonaryCapillariesToLeftPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//both caps and arteries flow into veins
	    Log.info("Left Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Liver1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Liver Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToLiver1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "PortalVeinToLiver1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//both aorta and portal vein flow to liver
	    Log.info("Liver Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Muscle1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Muscle Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToMuscle1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Muscle Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Myocardium1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Myocardium Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToMyocardium1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Myocardium Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryArteries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryArteries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Pulmonary Arteries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftIntermediatePulmonaryArteriesToLeftPulmonaryArteries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightIntermediatePulmonaryArteriesToRightPulmonaryArteries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Pulmonary Arteries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryCapillaries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryCapillaries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Pulmonary Capillaries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftPulmonaryArteriesToLeftPulmonaryCapillaries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryArteriesToRightPulmonaryCapillaries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "LeftPulmonaryVeins_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryVeins_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Pulmonary Veins Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "LeftPulmonaryArteriesToLeftPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "LeftPulmonaryCapillariesToLeftPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//both caps and arteries flow into veins
	    columnVals = getValues(results, "RightPulmonaryArteriesToRightPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryCapillariesToRightPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//both caps and arteries flow into veins
	    Log.info("Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "RightArm1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Arm Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToRightArm1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Arm Inflow Mean(mL/min) " + mean*60);
	    
	    //L and R heart have pressures, too
	    columnVals = getValues(results, "RightHeart1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Heart Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "RightHeart2ToRightHeart1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Heart Inflow Mean(mL/min) " + mean*60);
	    columnVals = getValues(results, "RightHeart1_Pressure_mmHg", subsetStartTimestep, subsetEndTimestep);//RightHeart2?
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Heart Pressure Mean(mmHg) " + mean);
	    
	    columnVals = getValues(results, "RightKidney1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Kidney Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToRightKidney1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Kidney Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "RightLeg1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Leg Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToRightLeg1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Leg Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "RightPulmonaryArteries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Pulmonary Arteries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "RightIntermediatePulmonaryArteriesToRightPulmonaryArteries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Pulmonary Arteries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "RightPulmonaryCapillaries_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Pulmonary Capillaries Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "RightPulmonaryArteriesToRightPulmonaryCapillaries_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "RightPulmonaryVeins_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Right Pulmonary Veins Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "RightPulmonaryArteriesToRightPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightPulmonaryCapillariesToRightPulmonaryVeins_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);	//both caps and arteries flow into veins
	    Log.info("Right Pulmonary Capillaries Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Skin1_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Skin Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToSkin1_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Skin Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "SmallIntestine_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Small Intestine Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToSmallIntestine_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Small Intestine Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Splanchnic_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Splanchnic Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToSplanchnic_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Splanchnic Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "Spleen_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Spleen Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Aorta1ToSpleen_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Spleen Inflow Mean(mL/min) " + mean*60);
	    
	    columnVals = getValues(results, "VenaCava_Volume_mL", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    Log.info("Vena Cava Volume Mean(mL) " + mean);
	    columnVals = getValues(results, "Bone2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightKidney2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "LeftKidney2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Liver2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Skin2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Muscle2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Fat2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "LeftArm2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightArm2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "LeftLeg2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "RightLeg2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Myocardium2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    columnVals = getValues(results, "Brain2ToVenaCava_Flow_mL/s", subsetStartTimestep, subsetEndTimestep);
	    mean = mean + DoubleUtils.getAverage(columnVals);
	    Log.info("Vena Cava Inflow Mean(mL/min) " + mean*60);
	}
}
