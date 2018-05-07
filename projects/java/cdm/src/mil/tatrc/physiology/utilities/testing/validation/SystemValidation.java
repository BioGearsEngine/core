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
package mil.tatrc.physiology.utilities.testing.validation;

import java.util.Arrays;

import mil.tatrc.physiology.utilities.Log;

public class SystemValidation extends ValdiationTool
{
  public SystemValidation()
  {
    DEST_DIRECTORY = "./validation/";
    DEFAULT_DIRECTORY = "doc/validation/";
    DEFAULT_FILE = "SystemValidationData.xlsx";
    TABLE_TYPE = "System";
  }
  public static void main(String[] args)
  {
    Log.setFileName("ValidationTool.log");
    Log.info("Running with agrs : "+Arrays.toString(args));
    SystemValidation me = new SystemValidation();
    if(args.length!=3)
    {
      me.loadData("Local","MSVC","x86",true);
    }
    else
    {      
      me.loadData(args[0], args[1], args[2],true);
    }
  }

}
