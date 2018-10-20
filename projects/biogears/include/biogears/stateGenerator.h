#include <string>
#include <biogears/findAndReplace.h>

class stateGenerator : public LoggerForward
{
public:
  stateGenerator();
  ~stateGenerator();
  int runScenario(int patientNum, std::string& XMLFileName, std::string& XMLString);
};