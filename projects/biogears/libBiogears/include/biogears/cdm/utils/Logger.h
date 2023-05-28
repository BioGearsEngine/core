
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

OPEN_BIOGEARS_NAMESPACE

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

  virtual void Debug(std::istream& msg) const;
  virtual void Info(std::istream& msg) const;
  virtual void Warning(std::istream& msg) const;
  virtual void Error(std::istream& msg) const;
  virtual void Fatal(std::istream& msg) const;

  virtual void Debug(std::istream&& msg) const;
  virtual void Info(std::istream&& msg) const;
  virtual void Warning(std::istream&& msg) const;
  virtual void Error(std::istream&& msg) const;
  virtual void Fatal(std::istream&& msg) const;

  std::string m_origin;
  Logger* m_Logger;
};

CLOSE_BIOGEARS_NAMESPACE

OPEN_BIOGEARS_NAMESPACE
#pragma warning(push)
#pragma warning(disable : 4100)
class BIOGEARS_API LoggerForward {
public:
  virtual void Debug(char const* msg) const = 0;
  virtual void Info(char const* msg) const = 0;
  virtual void Warning(char const* msg) const = 0;
  virtual void Error(char const* msg) const = 0;
  virtual void Fatal(char const* msg) const = 0;
};

#pragma warning(pop)

CLOSE_BIOGEARS_NAMESPACE

OPEN_BIOGEARS_NAMESPACE

class BIOGEARS_API Logger {
  friend Loggable;

public:
  enum LogLevel {
    eFatal = 0,
    eDebug,
    eError,
    eException,
    eWarning,
    eInfo,
    eStabilization,
    eAll
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

  virtual void Debug(std::istream& ss) const;
  virtual void Info(std::istream& ss) const;
  virtual void Warning(std::istream& ss) const;
  virtual void Error(std::istream& ss) const;
  virtual void Fatal(std::istream& ss) const;

  std::weak_ptr<IOManager> GetIoManager() const;
  void SetIoManager(IOManager const&); //< Logger will setup a copy of the IOManager to distributed amongs the Engine components

protected:
  void LogMessage(std::istream&, std::string const& origin, LogLevel) const;
  void LogMessage(std::istream&&, std::string const& origin, LogLevel) const;
  void LogMessage(std::string const& msg, std::string const& origin, LogLevel priority) const;
  void LogMessage(std::string&& msg, std::string const& origin, LogLevel priority) const;

private:
  struct Implementation;
  std::unique_ptr<Implementation> m_impl;
};

inline std::ostream& operator<<(std::ostream& os, Logger::LogLevel level)
{
  os << ((Logger::eAll == level)           ? "ALL"
           : (Logger::eFatal== level)     ? "FATAL"
           : (Logger::eDebug == level)     ? "DEBUG"
           : (Logger::eError == level)     ? "ERROR"
           : (Logger::eException== level) ? "EXECPTION"
           : (Logger::eWarning == level)   ? "WARNING"
           : (Logger::eInfo == level)      ? "INFO"
                                          : "UNKNWON");
  return os;
}
inline std::string ToString(Logger::LogLevel level)
{
  return (Logger::eAll == level)    ? "ALL"
    : (Logger::eFatal == level)     ? "FATAL"
    : (Logger::eDebug == level)     ? "DEBUG"
    : (Logger::eError == level)     ? "ERROR"
    : (Logger::eException== level) ? "EXECPTION"
    : (Logger::eWarning == level)   ? "WARNING"
    : (Logger::eInfo == level)      ? "INFO"
                                   : "UNKNWON";
}

inline Logger::LogLevel FromString(std::string str)
{
  return ("ALL" == str)    ? Logger::eAll
    : ("FATAL" == str)     ? Logger::eFatal
    : ("DEBUG" == str)     ? Logger::eDebug
    : ("ERROR" == str)     ? Logger::eError
    : ("EXECPTION" == str) ? Logger::eException
    : ("WARNING" == str)   ? Logger::eWarning
    : ("INFO" == str)      ? Logger::eInfo
                           : Logger::eAll;
}

CLOSE_BIOGEARS_NAMESPACE
