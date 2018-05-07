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

//#include "Logger.h"
#include "TimingProfile.h"
#include <windows.h>

void main()
{
	int sleep = 10;
	int iterations = 10000;
	TimingProfile *profile = new TimingProfile();


	profile->LogProfiles(false);

	profile->Start("Pause");
	Sleep(sleep);
	profile->Pause("Pause");
	Sleep(sleep);
	profile->Start("Pause");
	Sleep(sleep);
	profile->Pause("Pause");


	profile->Start("Outer");
	profile->Start("Inner");
	profile->Pause("Inner");
	for(int i = 0; i < iterations; i++)
	{		
		profile->Start("Inner");
		Sleep(sleep);
		profile->Pause("Inner");
	}
	double outer = profile->Profile("Outer");
	double inner = profile->Profile("Inner");
	double pause = profile->Profile("Pause");
	
	std::cout<<"Iterations : "<<iterations<<" Expect about a profile of "<<sleep*iterations<<std::endl;
	std::cout<<"Inner "<<inner<<std::endl;
	std::cout<<"Outer "<<outer<<std::endl;
	std::cout << "approximate overhead per profile call : " << (outer - inner) / (2 * iterations) << " ms" <<std::endl;
	std::cout << "Pause 2 times expected out " << 2 * sleep << ": " << pause << std::endl;
}