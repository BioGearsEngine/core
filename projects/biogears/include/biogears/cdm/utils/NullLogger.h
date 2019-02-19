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
class NullLogger;

#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(pop)

class BIOGEARS_API NullLogger : public Logger {
public:
  NullLogger(const std::string& logFilename = Loggable::empty);
  virtual ~NullLogger();

  void LogToConsole(bool b);

  void ResetLogFile(const std::string& logFilename = Loggable::empty);

  void SetLogLevel(log4cpp::Priority::Value priority);
  log4cpp::Priority::Value GetLogLevel();

  virtual void SetLogTime(const SEScalarTime* time);

  virtual void SetForward(LoggerForward* forward);
  virtual bool HasForward();

  virtual void Debug(const std::string& msg, const std::string& origin = Loggable::empty);
  virtual void Debug(std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Debug(std::ostream& msg, const std::string& origin = Loggable::empty);

  virtual void Info(const std::string& msg, const std::string& origin = Loggable::empty);
  virtual void Info(std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Info(const std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Info(std::ostream& msg, const std::string& origin = Loggable::empty);

  virtual void Warning(const std::string& msg, const std::string& origin = Loggable::empty);
  virtual void Warning(std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Warning(std::ostream& msg, const std::string& origin = Loggable::empty);

  virtual void Error(const std::string& msg, const std::string& origin = Loggable::empty);
  virtual void Error(std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Error(std::ostream& msg, const std::string& origin = Loggable::empty);

  virtual void Fatal(const std::string& msg, const std::string& origin = Loggable::empty);
  virtual void Fatal(std::stringstream& msg, const std::string& origin = Loggable::empty);
  virtual void Fatal(std::ostream& msg, const std::string& origin = Loggable::empty);

protected:
  virtual std::string FormatLogMessage(const std::string& origin, const std::string& msg);

  LoggerForward* m_Forward;
  log4cpp::Category* m_Log;
  log4cpp::Appender* m_FileAppender;
  log4cpp::Appender* m_ConsoleAppender;
  const SEScalarTime* m_time;
  std::stringstream m_ss;
};
}