package mil.tatrc.physiology.utilities.testing;


import java.io.*;
import java.util.*;
import mil.tatrc.physiology.utilities.FileUtils;
import mil.tatrc.physiology.utilities.Log;

/**
 * This class will reconfigure the .config files in bin
 * based on a single input
 */
public class Reconfiguration
{

	public static void main(String[] args)
	{
		if(args.length!=1)
		{
			Log.info("One input argument expected");
			return;
		}
		
		// Get a list of all the .config files in the bin
		List<String> configFiles = FileUtils.findFiles("./", ".config", false);
		
		if(args[0].equalsIgnoreCase("serialization"))
		{
			serializationConfiguration(configFiles);
			return;
		}
		Log.error("Unsupported configuration : " +args[0]);
	}
	
	protected static void serializationConfiguration(List<String> configFiles)
	{
		for(String configFile : configFiles)
		{
			BufferedReader br=null;
			StringBuffer outbuff = new StringBuffer();
			try
			{
				br = new BufferedReader(new FileReader(configFile));
				String line;	
				boolean usesScenarioDriver=false;
				boolean hasUseStates=false;
				boolean hasAutoSerialization=false;
				while ((line = br.readLine()) != null)
				{
					// Uncomment if it's commented
					if(line.indexOf("UseStates")>-1)
					{
						hasUseStates=true;
						line = line.replace("#", "");
					}
					else if(line.indexOf("AutoSerialization")>-1)
					{
						hasAutoSerialization=true;
						line = line.replace("#", "");
					}
					else if(line.indexOf("ScenarioTestDriver")>-1)
					{// We can rewrite this file
						usesScenarioDriver=true;
					}					
					outbuff.append(line+"\n");
				}
				br.close();
				
				if(usesScenarioDriver)
				{
					BufferedWriter bwr = new BufferedWriter(new FileWriter(new File(configFile)));
					
					if(!hasUseStates||!hasAutoSerialization)
					{
						bwr.append("#Serialization Configuration Parameters:\n");
						if(!hasUseStates)
							bwr.append("UseStates=true\n");
						if(!hasAutoSerialization)
							bwr.append("AutoSerialization=./states/testing/,5,Off,On,On\n");
						bwr.append("\n");
					}
					
					bwr.write(outbuff.toString());
					bwr.flush();
					bwr.close();
				}
			}
			catch(Exception ex)
			{
				Log.error("Error reconfiguring config file : "+configFile, ex);
			}	
		}
	}
}
