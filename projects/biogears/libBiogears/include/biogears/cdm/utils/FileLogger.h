#pragma once
#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#include <log4cpp/Category.hh>
#pragma warning(pop)

#include <biogears/cdm/utils/Logger.h>
#include <biogears/cdm/CommonDataModel.h>

namespace biogears {
class FileLogger;

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(pop)

class BIOGEARS_API FileLogger : public Logger {
	FileLogger(const std::string& logFilename = Loggable::empty);
	virtual ~FileLogger();
	void ResetLogFile(const std::string& logFilename = Loggable::empty);
};
}