#include <iostream>

#include "HowTo-BurnWoundPainStimulus.h"


int main( int argc, char* argv[] )
{
  // To run multiple TBSA values
  double lowestTBSA = 30.0;
  double highestTBSA = 40.0;
  double tbsaIncrement = 5.0;
  for (double tbsa = lowestTBSA; tbsa <= highestTBSA; tbsa += tbsaIncrement) {
    //HowToBurnWoundPainStimulus();
    int docTBSA = (int)(tbsa);
    std::string strTBSA = std::to_string(docTBSA);
    std::string logName = "./states/BurnWoundStates/HowToBurnWound";
    logName.append(strTBSA);
    logName.append(".log");
    BurnThread burn(logName, tbsa);
    burn.FluidLoading(tbsa);
  }
}
