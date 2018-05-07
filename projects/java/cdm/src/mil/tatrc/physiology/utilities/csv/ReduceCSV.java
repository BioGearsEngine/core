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

import java.io.*;
import mil.tatrc.physiology.utilities.Log;

public class ReduceCSV
{
	public static void main(String[] args) throws IOException
	{    
		Log.setFileName("ReduceHeaders.log");

		String filename = "\\\\sed-hatteras.us.ara.com\\Projects\\Biogears\\BasicStandard12hrResults.txt";

		Log.info("Looking at file: " + filename);
		File fIn = new File(filename);
		if(!fIn.exists())
		{
			Log.error("Say what!?, I don't see that file "+filename);
			return;
		}
		FileInputStream fStream = new FileInputStream(filename);
		DataInputStream in = new DataInputStream(fStream);
		BufferedReader bIn = new BufferedReader(new InputStreamReader(in));

		File fOut = new File("Reduced"+fIn.getName());
		FileOutputStream oStream = new FileOutputStream(fOut);
		BufferedWriter bOut = new BufferedWriter(new OutputStreamWriter(oStream));

		String line = bIn.readLine(); 
		bOut.write(line);
		bOut.newLine();

		int i=0;
		String aLine;
		while ((aLine=bIn.readLine()) != null) 
		{       
			if(i++ %90 == 0)
			{				
				bOut.write(aLine);
				bOut.newLine();
			}
		}       
		bIn.close();
		bOut.close();          
	}
}
