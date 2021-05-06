
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
#include <memory>
#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#include <log4cpp/Category.hh>
#pragma warning(pop)

#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>

#include <biogears/io/io-manager.h>

namespace biogears {

class Logger;
class SEScalarTime;

class BIOGEARS_API Loggable {
public:
  static const std::string empty;
  static const char* empty_cStr;

  Loggable();
  Loggable(Logger* log);
  virtual ~Loggable();

  virtual Logger* GetLogger() const;

protected:
  virtual void Debug(const char* msg, const char* origin = "") const;
  virtual void Debug(const std::string& msg, const std::string& origin = empty) const;
  virtual void Debug(std::ostream& msg, const std::string& origin = empty) const;

  virtual void Info(const char* msg, const char* origin = "") const;
  virtual void Info(const std::string& msg, const std::string& origin = empty) const;
  virtual void Info(std::ostream& msg, const std::string& origin = empty) const;

  virtual void Warning(const char* msg, const char* origin = "") const;
  virtual void Warning(const std::string& msg, const std::string& origin = empty) const;
  virtual void Warning(std::ostream& msg, const std::string& origin = empty) const;

  virtual void Error(const char* msg, const char* origin = "") const;
  virtual void Error(const std::string msg, const std::string origin = empty) const;
  virtual void Error(std::ostream& msg, const std::string& origin = empty) const;

  virtual void Fatal(const char* msg, const char* origin = "") const;
  virtual void Fatal(const std::string& msg, const std::string& origin = empty) const;
  virtual void Fatal(std::ostream& msg, const std::string& origin = empty) const;

  Logger* m_Logger;
};
}

namespace biogears {
#pragma warning(push)
#pragma warning(disable : 4100)
class BIOGEARS_API LoggerForward {
public:
  virtual void ForwardDebug(const std::string& msg, const std::string& origin) = 0;
  virtual void ForwardInfo(const std::string& msg, const std::string& origin) = 0;
  virtual void ForwardWarning(const std::string& msg, const std::string& origin) = 0;
  virtual void ForwardError(const std::string& msg, const std::string& origin) = 0;
  virtual void ForwardFatal(const std::string& msg, const std::string& origin) = 0;
};

#pragma warning(pop)
}

namespace biogears {
class BIOGEARS_API Logger {
  friend Loggable;

public:
  explicit Logger(const std::string& logFilename = Loggable::empty);
  explicit Logger(const std::string& logFilename, IOManager const& io);
  explicit Logger(const char* logFilename );
  explicit Logger(const char* logFilename, IOManager const& io);

  virtual ~Logger();

  void LogToConsole(bool log_to_console);
  void FormatMessages(bool format_messages);

  void ResetLogFile(const std::string& logFilename = Loggable::empty);
  void ResetLogFile(const char* logFilename);

  void SetLogLevel(log4cpp::Priority::Value priority) const;
  void SetConsoleLogLevel(log4cpp::Priority::Value priority) const;
  log4cpp::Priority::Value GetLogLevel();

  virtual void SetLogTime(const SEScalarTime* time);

  void SetsetConversionPattern(const std::string&);
  void SetConsolesetConversionPattern(const std::string&);

  virtual void SetForward(LoggerForward* forward);
  virtual bool HasForward();

  virtual void Debug(const std::string& msg, std::string const& origin = Loggable::empty) const;
  virtual void Info(const std::string& msg, std::string const& origin = Loggable::empty) const;
  virtual void Warning(const std::string& msg, std::string const& origin = Loggable::empty) const;
  virtual void Error(const std::string& msg, std::string const& origin = Loggable::empty) const;
  virtual void Fatal(const std::string& msg, std::string const& origin = Loggable::empty) const;

  virtual void Debug(std::ostream const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Info(std::ostream const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Warning(std::ostream const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Error(std::ostream const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Fatal(std::ostream const& msg, std::string const& origin = Loggable::empty) const;

  std::weak_ptr<IOManager> GetIoManager() const;
  void SetIoManager(IOManager const&); //< Logger will setup a copy of the IOManager to distributed amongs the Engine components

protected:
  virtual std::string FormatLogMessage(const std::string& origin, const std::string& msg) const;

  std::shared_ptr<IOManager> m_io;

  LoggerForward* m_Forward;
  log4cpp::Category* m_Log;
  log4cpp::Appender* m_FileAppender;
  log4cpp::Appender* m_ConsoleAppender;
  const SEScalarTime* m_time;
  mutable std::stringstream m_ss;
  bool m_FormatMessages;
  Logger* m_Logger;
};

}