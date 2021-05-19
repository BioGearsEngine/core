
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

#include <biogears/exports.h>

#include <biogears/cdm/CommonDataModel.h>

namespace biogears {

class Logger;
class SEScalarTime;
class IOManager;

class BIOGEARS_API Loggable {
public:
  static const std::string empty;
  static const char* empty_cStr;

  Loggable();
  Loggable(Logger* log, std::string const& origin = Loggable::empty);
  virtual ~Loggable();

  virtual Logger* GetLogger() const;

protected:
  virtual void Debug(std::string const& msg) const;
  virtual void Info(std::string const& msg) const;
  virtual void Warning(std::string const& msg) const;
  virtual void Error(std::string const& msg) const;
  virtual void Fatal(std::string const& msg) const;

  virtual void Debug(std::string const& msg, std::string const& origin) const;
  virtual void Info(std::string const& msg, std::string const& origin) const;
  virtual void Warning(std::string const& msg, std::string const& origin) const;
  virtual void Error(std::string const& msg, std::string const& origin) const;
  virtual void Fatal(std::string const& msg, std::string const& origin) const;

  virtual void Debug( std::ostream& msg) const;
  virtual void Info( std::ostream& msg) const;
  virtual void Warning( std::ostream& msg) const;
  virtual void Error( std::ostream& msg) const;
  virtual void Fatal( std::ostream& msg) const;

  std::string m_origin;
  Logger* m_Logger;
};
}

namespace biogears {
#pragma warning(push)
#pragma warning(disable : 4100)
class BIOGEARS_API LoggerForward {
public:
  virtual void Debug(std::string const& msg) const = 0;
  virtual void Info(std::string const& msg) const = 0;
  virtual void Warning(std::string const& msg) const = 0;
  virtual void Error(std::string const& msg) const = 0;
  virtual void Fatal(std::string const& msg) const = 0;
};

#pragma warning(pop)
}

namespace biogears {
class BIOGEARS_API Logger {
  friend Loggable;

public:
  enum LogLevel {
    FATAL = 0,
    DEBUG,
    ERROR,
    EXCEPTION,
    WARNING,
    INFO,
    STABILIZATION,
    ALL
  };

  explicit Logger(std::string const& logFilename = "biogears.log");
  explicit Logger(std::string const& logFilename, IOManager const& io);
  explicit Logger(const char* logFilename);
  explicit Logger(const char* logFilename, IOManager const& io);

  virtual ~Logger();

  void LogToConsole(bool log_to_console);
  void FormatMessages(bool format_messages);

  void ResetLogFile(std::string const& logFilename = Loggable::empty);
  void ResetLogFile(const char* logFilename);

  void SetLogLevel(LogLevel logLevel) const;
  void SetConsoleLogLevel(LogLevel priority) const;

  LogLevel GetLogLevel();

  virtual void SetLogTime(const SEScalarTime* time);

  void SetConversionPattern(std::string const&);
  void SetConsoleConversionPattern(std::string const&);

  virtual void SetForward(LoggerForward* forward);
  virtual bool HasForward();

  virtual void Debug(std::string const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Info(std::string const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Warning(std::string const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Error(std::string const& msg, std::string const& origin = Loggable::empty) const;
  virtual void Fatal(std::string const& msg, std::string const& origin = Loggable::empty) const;

  virtual void Debug(std::ostream & ss) const;
  virtual void Info(std::ostream & ss) const;
  virtual void Warning(std::ostream & ss) const;
  virtual void Error(std::ostream & ss) const;
  virtual void Fatal(std::ostream & ss) const;

  std::weak_ptr<IOManager> GetIoManager() const;
  void SetIoManager(IOManager const&); //< Logger will setup a copy of the IOManager to distributed amongs the Engine components

protected:
  virtual std::string FormatLogMessage(LogLevel priority, std::string const& pattern, std::string const& message, std::string const& origin_str) const;

private:
  struct Implementation;
  std::unique_ptr<Implementation> m_impl;
};

inline std::string ToString(Logger::LogLevel level)
{
  return (Logger::ALL == level)    ? "ALL"
    : (Logger::FATAL == level)     ? "FATAL"
    : (Logger::DEBUG == level)     ? "DEBUG"
    : (Logger::ERROR == level)     ? "ERROR"
    : (Logger::EXCEPTION == level) ? "EXECPTION"
    : (Logger::WARNING == level)   ? "WARNING"
    : (Logger::INFO == level)      ? "INFO"
                                   : "UNKNWON";
}

inline Logger::LogLevel FromString(std::string str)
{
  return ("ALL" == str)    ? Logger::ALL
    : ("FATAL" == str)     ? Logger::FATAL
    : ("DEBUG" == str)     ? Logger::DEBUG
    : ("ERROR" == str)     ? Logger::ERROR
    : ("EXECPTION" == str) ? Logger::EXCEPTION
    : ("WARNING" == str)   ? Logger::WARNING
    : ("INFO" == str)      ? Logger::INFO
                           : Logger::ALL;
}
}