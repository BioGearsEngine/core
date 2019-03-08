#include <biogears/cdm/utils/NullLogger.h>

namespace biogears {

NullLogger::NullLogger(const std::string& logFilename) : Logger("NullLogger") {}
NullLogger::~NullLogger() {}
void NullLogger::LogToConsole(bool b) {}
void NullLogger::ResetLogFile(const std::string& logFilename) {}
void NullLogger::SetLogLevel(log4cpp::Priority::Value priority) {}
log4cpp::Priority::Value NullLogger::GetLogLevel()
{
  return log4cpp::Priority::INFO;
}
void NullLogger::SetLogTime(const SEScalarTime* time) {}
void NullLogger::SetForward(LoggerForward* forward) {}
bool NullLogger::HasForward()
{
  return false;
}
void NullLogger::Debug(const std::string& msg, const std::string& origin) {}
void NullLogger::Debug(std::ostream& msg, const std::string& origin) {}
void NullLogger::Debug(std::stringstream& msg, const std::string& origin) {}
void NullLogger::Info(const std::string& msg, const std::string& origin) {}
void NullLogger::Info(const std::stringstream& msg, const std::string& origin) {}
void NullLogger::Info(std::ostream& msg, const std::string& origin) {}
void NullLogger::Info(std::stringstream& msg, const std::string& origin) {}
void NullLogger::Warning(const std::string& msg, const std::string& origin) {}
void NullLogger::Warning(std::ostream& msg, const std::string& origin) {}
void NullLogger::Warning(std::stringstream& msg, const std::string& origin) {}
void NullLogger::Error(const std::string& msg, const std::string& origin) {}
void NullLogger::Error(std::ostream& msg, const std::string& origin) {}
void NullLogger::Error(std::stringstream& msg, const std::string& origin) {}
void NullLogger::Fatal(const std::string& msg, const std::string& origin) {}
void NullLogger::Fatal(std::ostream& msg, const std::string& origin) {}
void NullLogger::Fatal(std::stringstream& msg, const std::string& origin) {}
std::string NullLogger::FormatLogMessage(const std::string& origin, const std::string& msg)
{
  return "";
}
}